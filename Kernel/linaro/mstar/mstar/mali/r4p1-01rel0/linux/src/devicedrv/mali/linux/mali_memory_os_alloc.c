/*
 * Copyright (C) 2013-2014 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fs.h>
#include <linux/dma-mapping.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>

#include "mali_osk.h"
#include "mali_memory.h"
#include "mali_memory_os_alloc.h"
#include "mali_kernel_linux.h"

/* MSTAR patch begin */
#ifdef MSTAR
#include "mstar/mstar_chip.h"
#define PHYS_TO_BUS_ADDRESS_ADJUST  MSTAR_MIU0_BUS_BASE
#endif

#if defined(CONFIG_MP_MM_CMA)
#include <linux/cma_allocator.h>
extern void get_mali_alloc_strategy(unsigned long *ump, unsigned long * mali);
#endif
/* MSTAR patch end */

/* Minimum size of allocator page pool */
#define MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_MB * 256)
#define MALI_OS_MEMORY_POOL_TRIM_JIFFIES (10 * CONFIG_HZ) /* Default to 10s */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
/* Write combine dma_attrs */
static DEFINE_DMA_ATTRS(dma_attrs_wc);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 0, 0)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
static int mali_mem_os_shrink(int nr_to_scan, gfp_t gfp_mask);
#else
static int mali_mem_os_shrink(struct shrinker *shrinker, int nr_to_scan, gfp_t gfp_mask);
#endif
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 12, 0)
static int mali_mem_os_shrink(struct shrinker *shrinker, struct shrink_control *sc);
#else
static unsigned long mali_mem_os_shrink(struct shrinker *shrinker, struct shrink_control *sc);
static unsigned long mali_mem_os_shrink_count(struct shrinker *shrinker, struct shrink_control *sc);
#endif
#endif
static void mali_mem_os_trim_pool(struct work_struct *work);

static struct mali_mem_os_allocator {
	spinlock_t pool_lock;
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	struct list_head pool_pages[MSTAR_MIU_NUM];
	size_t pool_count[MSTAR_MIU_NUM];

	atomic_t allocated_pages[MSTAR_MIU_NUM];
#else
	struct list_head pool_pages;
	size_t pool_count;

	atomic_t allocated_pages;
#endif
/* MSTAR patch end */
	size_t allocation_limit;

	struct shrinker shrinker;
	struct delayed_work timed_shrinker;
	struct workqueue_struct *wq;
} mali_mem_os_allocator = {
	.pool_lock = __SPIN_LOCK_UNLOCKED(pool_lock),
/* MSTAR patch begin */
#if !defined(CONFIG_MP_MM_CMA) && !defined(CONFIG_MP_ION_PATCH_MSTAR)
	.pool_pages = LIST_HEAD_INIT(mali_mem_os_allocator.pool_pages),
	.pool_count = 0,

	.allocated_pages = ATOMIC_INIT(0),
#endif
/* MSTAR patch end */
	.allocation_limit = 0,

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 12, 0)
	.shrinker.shrink = mali_mem_os_shrink,
#else
	.shrinker.count_objects = mali_mem_os_shrink_count,
	.shrinker.scan_objects = mali_mem_os_shrink,
#endif
	.shrinker.seeks = DEFAULT_SEEKS,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
	.timed_shrinker = __DELAYED_WORK_INITIALIZER(mali_mem_os_allocator.timed_shrinker, mali_mem_os_trim_pool, TIMER_DEFERRABLE),
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 38)
	.timed_shrinker = __DEFERRED_WORK_INITIALIZER(mali_mem_os_allocator.timed_shrinker, mali_mem_os_trim_pool),
#else
	.timed_shrinker = __DELAYED_WORK_INITIALIZER(mali_mem_os_allocator.timed_shrinker, mali_mem_os_trim_pool),
#endif
};

static void mali_mem_os_free(mali_mem_allocation *descriptor)
{
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	int miu_id;
#endif
/* MSTAR patch end */

	LIST_HEAD(pages);

	MALI_DEBUG_ASSERT(MALI_MEM_OS == descriptor->type);

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	miu_id = descriptor->os_mem.miu_id;
	atomic_sub(descriptor->os_mem.count, &mali_mem_os_allocator.allocated_pages[miu_id]);
#else
	atomic_sub(descriptor->os_mem.count, &mali_mem_os_allocator.allocated_pages);
#endif
/* MSTAR patch end */

	/* Put pages on pool. */
	list_cut_position(&pages, &descriptor->os_mem.pages, descriptor->os_mem.pages.prev);

	spin_lock(&mali_mem_os_allocator.pool_lock);

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	list_splice(&pages, &mali_mem_os_allocator.pool_pages[miu_id]);
	mali_mem_os_allocator.pool_count[miu_id] += descriptor->os_mem.count;
#else
	list_splice(&pages, &mali_mem_os_allocator.pool_pages);
	mali_mem_os_allocator.pool_count += descriptor->os_mem.count;
#endif
	/* MSTAR patch end */

	spin_unlock(&mali_mem_os_allocator.pool_lock);

	/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	{
		int start_pool_trim_timer = 0;
		int i;
		for (i = 0; i < MSTAR_MIU_NUM; i++)
		{
			if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES < mali_mem_os_allocator.pool_count[i])
			{
				start_pool_trim_timer = 1;
				break;
			}
		}
		if (start_pool_trim_timer) {
			for (i = 0; i < MSTAR_MIU_NUM; i++)
			{
				MALI_DEBUG_PRINT(5, ("OS Mem: Starting pool[%d] trim timer %u\n",i , mali_mem_os_allocator.pool_count[i]));
			}
			queue_delayed_work(mali_mem_os_allocator.wq, &mali_mem_os_allocator.timed_shrinker, MALI_OS_MEMORY_POOL_TRIM_JIFFIES);
		}
	}
#else
	if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES < mali_mem_os_allocator.pool_count) {
		MALI_DEBUG_PRINT(5, ("OS Mem: Starting pool trim timer %u\n", mali_mem_os_allocator.pool_count));
		queue_delayed_work(mali_mem_os_allocator.wq, &mali_mem_os_allocator.timed_shrinker, MALI_OS_MEMORY_POOL_TRIM_JIFFIES);
	}
#endif
/* MSTAR patch end */
}

/* MSTAR patch begin */
#if defined(CONFIG_MP_ION_PATCH_MSTAR)
extern struct page* mali_alloc_page();
extern void mali_free_page(struct page*);
#endif
/* MSTAR patch end */

static int mali_mem_os_alloc_pages(mali_mem_allocation *descriptor, u32 size)
{
	struct page *new_page, *tmp;
	LIST_HEAD(pages);
	size_t page_count = PAGE_ALIGN(size) / _MALI_OSK_MALI_PAGE_SIZE;
	size_t remaining = page_count;
	u32 i;
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	unsigned long mali_id;
	mali_id = get_mali_alloc_strategy_wrapper();
	descriptor->os_mem.miu_id = (int)mali_id;
#endif
/* MSTAR patch end */

	MALI_DEBUG_ASSERT_POINTER(descriptor);
	MALI_DEBUG_ASSERT(MALI_MEM_OS == descriptor->type);

	INIT_LIST_HEAD(&descriptor->os_mem.pages);
	descriptor->os_mem.count = page_count;

	/* Grab pages from pool. */
	{
		size_t pool_pages;
		spin_lock(&mali_mem_os_allocator.pool_lock);
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
		pool_pages = min(remaining, mali_mem_os_allocator.pool_count[mali_id]);
#else
		pool_pages = min(remaining, mali_mem_os_allocator.pool_count);
#endif
		/* MSTAR patch end */
		for (i = pool_pages; i > 0; i--) {
			/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
			BUG_ON(list_empty(&mali_mem_os_allocator.pool_pages[mali_id]));
			list_move(mali_mem_os_allocator.pool_pages[mali_id].next, &pages);
#else
			BUG_ON(list_empty(&mali_mem_os_allocator.pool_pages));
			list_move(mali_mem_os_allocator.pool_pages.next, &pages);
#endif
			/* MSTAR patch end */
		}
		/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
		mali_mem_os_allocator.pool_count[mali_id] -= pool_pages;
#else
		mali_mem_os_allocator.pool_count -= pool_pages;
#endif
/* MSTAR patch end */
		remaining -= pool_pages;
		spin_unlock(&mali_mem_os_allocator.pool_lock);
	}

	/* Process pages from pool. */
	i = 0;
	list_for_each_entry_safe(new_page, tmp, &pages, lru) {
		BUG_ON(NULL == new_page);

		list_move_tail(&new_page->lru, &descriptor->os_mem.pages);
	}

	/* Allocate new pages, if needed. */
	for (i = 0; i < remaining; i++) {
		dma_addr_t dma_addr;
		gfp_t flags = __GFP_ZERO | __GFP_REPEAT | __GFP_NOWARN | __GFP_COLD;
		int err;

/* MSTAR patch begin */
#ifdef MSTAR
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,1,1)) && !defined(CONFIG_MP_MM_MSTAR_3DALLOC_MIU0_1)
		flags |=  GFP_HIGHUSER;
#elif defined(CONFIG_ZONE_NORMAL_MIU0)
		flags |= (GFP_USER  | __GFP_NORMAL_MIU0);       /* MStar: allocate GPU memory from MIU0 */
#elif defined(CONFIG_ZONE_NORMAL_MIU1)
		flags |= (GFP_HIGHUSER | __GFP_NORMAL_MIU1);    /* MStar: allocate GPU memory from MIU1 */
#else
		flags |=  GFP_HIGHUSER;
#endif
#else /* MSTAR */
#if defined(CONFIG_ARM) && !defined(CONFIG_ARM_LPAE)
		flags |= GFP_HIGHUSER;
#else
		flags |= GFP_DMA32;
#endif
#endif
		/* MSTAR patch end */

		/* MSTAR patch begin */
#ifdef MSTAR
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
		new_page = mali_alloc_page();
#else
		new_page = alloc_page(flags);
#endif
#else
		new_page = alloc_page(flags);
#endif
/* MSTAR patch end */

		if (unlikely(NULL == new_page)) {
			/* Calculate the number of pages actually allocated, and free them. */
			descriptor->os_mem.count = (page_count - remaining) + i;
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
			atomic_add(descriptor->os_mem.count, &mali_mem_os_allocator.allocated_pages[mali_id]);
#else
			atomic_add(descriptor->os_mem.count, &mali_mem_os_allocator.allocated_pages);
#endif
/* MSTAR patch end */
			mali_mem_os_free(descriptor);
			return -ENOMEM;
		}

		/* Ensure page is flushed from CPU caches. */
		dma_addr = dma_map_page(&mali_platform_device->dev, new_page,
					0, _MALI_OSK_MALI_PAGE_SIZE, DMA_TO_DEVICE);

		err = dma_mapping_error(&mali_platform_device->dev, dma_addr);
		if (unlikely(err)) {
			MALI_DEBUG_PRINT_ERROR(("OS Mem: Failed to DMA map page %p: %u",
						new_page, err));
			__free_page(new_page);
			descriptor->os_mem.count = (page_count - remaining) + i;
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
			atomic_add(descriptor->os_mem.count, &mali_mem_os_allocator.allocated_pages[mali_id]);
#else
			atomic_add(descriptor->os_mem.count, &mali_mem_os_allocator.allocated_pages);
#endif
/* MSTAR patch end */
			mali_mem_os_free(descriptor);
			return -EFAULT;
		}

		/* Store page phys addr */
		SetPagePrivate(new_page);
		set_page_private(new_page, dma_addr);

		list_add_tail(&new_page->lru, &descriptor->os_mem.pages);
	}

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	atomic_add(page_count, &mali_mem_os_allocator.allocated_pages[mali_id]);
	{
		descriptor->num_pages_allocated_miu[mali_id] = page_count;
		int stop_pool_trim_timer = 1;

		for (i = 0; i < MSTAR_MIU_NUM; i++)
		{
			if(MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES > mali_mem_os_allocator.pool_count[i])
			{
				stop_pool_trim_timer = 0;
				break;
			}
		}
		if (stop_pool_trim_timer == 1) {

			for (i = 0; i < MSTAR_MIU_NUM; i++)
			{
				MALI_DEBUG_PRINT(4, ("OS Mem: Stopping pool trim timer, only %u pages on pool[%d]\n", mali_mem_os_allocator.pool_count[i],i));
			}
			cancel_delayed_work(&mali_mem_os_allocator.timed_shrinker);
		}
	}
#else
	atomic_add(page_count, &mali_mem_os_allocator.allocated_pages);

	if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES > mali_mem_os_allocator.pool_count) {
		MALI_DEBUG_PRINT(4, ("OS Mem: Stopping pool trim timer, only %u pages on pool\n", mali_mem_os_allocator.pool_count));
		cancel_delayed_work(&mali_mem_os_allocator.timed_shrinker);
	}
#endif
/* MSTAR patch end */

	return 0;
}

static int mali_mem_os_mali_map(mali_mem_allocation *descriptor, struct mali_session_data *session)
{
	struct mali_page_directory *pagedir = session->page_directory;
	struct page *page;
	_mali_osk_errcode_t err;
	u32 virt = descriptor->mali_mapping.addr;
	u32 prop = descriptor->mali_mapping.properties;

	MALI_DEBUG_ASSERT(MALI_MEM_OS == descriptor->type);

	err = mali_mem_mali_map_prepare(descriptor);
	if (_MALI_OSK_ERR_OK != err) {
		return -ENOMEM;
	}

	list_for_each_entry(page, &descriptor->os_mem.pages, lru) {
/* MSTAR patch begin */
#ifdef MSTAR
		dma_addr_t phys = page_private(page) - PHYS_TO_BUS_ADDRESS_ADJUST;
#else
		dma_addr_t phys = page_private(page);
#endif
/* MSTAR patch end */

#if defined(CONFIG_ARCH_DMA_ADDR_T_64BIT)
		/* Verify that the "physical" address is 32-bit and
		 * usable for Mali, when on a system with bus addresses
		 * wider than 32-bit. */
		MALI_DEBUG_ASSERT(0 == (phys >> 32));
#endif

		mali_mmu_pagedir_update(pagedir, virt, (mali_dma_addr)phys, MALI_MMU_PAGE_SIZE, prop);
		virt += MALI_MMU_PAGE_SIZE;
	}

	return 0;
}

static void mali_mem_os_mali_unmap(struct mali_session_data *session, mali_mem_allocation *descriptor)
{
	mali_mem_mali_map_free(descriptor);
#if MSTAR_STATISTIC_MALI_MIU_USAGE
	int i;
	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		session->num_pages_allocated_miu[i] -= descriptor->num_pages_allocated_miu[i];
	}
#endif
}

static int mali_mem_os_cpu_map(mali_mem_allocation *descriptor, struct vm_area_struct *vma)
{
	struct page *page;
	int ret;
	unsigned long addr = vma->vm_start;

	list_for_each_entry(page, &descriptor->os_mem.pages, lru) {
		/* We should use vm_insert_page, but it does a dcache
		 * flush which makes it way slower than remap_pfn_range or vm_insert_pfn.
		ret = vm_insert_page(vma, addr, page);
		*/
		ret = vm_insert_pfn(vma, addr, page_to_pfn(page));

		if (unlikely(0 != ret)) {
			return -EFAULT;
		}
		addr += _MALI_OSK_MALI_PAGE_SIZE;
	}

	return 0;
}

mali_mem_allocation *mali_mem_os_alloc(u32 mali_addr, u32 size, struct vm_area_struct *vma, struct mali_session_data *session)
{
	mali_mem_allocation *descriptor;
	int err;

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	int i, alloc_bytes = 0;

	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		alloc_bytes += atomic_read(&mali_mem_os_allocator.allocated_pages[i]) * _MALI_OSK_MALI_PAGE_SIZE;
	}

	if (alloc_bytes + size > mali_mem_os_allocator.allocation_limit){
		MALI_DEBUG_PRINT(2, ("Mali Mem: Unable to allocate %u bytes. Currently allocated: %lu, max limit %lu\n",
		                     size,
		                     alloc_bytes,
		                     mali_mem_os_allocator.allocation_limit));
	}
#else
	if (atomic_read(&mali_mem_os_allocator.allocated_pages) * _MALI_OSK_MALI_PAGE_SIZE + size > mali_mem_os_allocator.allocation_limit) {
		MALI_DEBUG_PRINT(2, ("Mali Mem: Unable to allocate %u bytes. Currently allocated: %lu, max limit %lu\n",
				     size,
				     atomic_read(&mali_mem_os_allocator.allocated_pages) * _MALI_OSK_MALI_PAGE_SIZE,
				     mali_mem_os_allocator.allocation_limit));
		return NULL;
	}
#endif
/* MSTAR patch end */

	descriptor = mali_mem_descriptor_create(session, MALI_MEM_OS);
	if (NULL == descriptor) return NULL;

	descriptor->mali_mapping.addr = mali_addr;
	descriptor->size = size;
	descriptor->cpu_mapping.addr = (void __user *)vma->vm_start;
	descriptor->cpu_mapping.ref = 1;
#if MSTAR_STATISTIC_MALI_MIU_USAGE
	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		descriptor->num_pages_allocated_miu[i] = 0;
	}
#endif


	if (VM_SHARED == (VM_SHARED & vma->vm_flags)) {
		descriptor->mali_mapping.properties = MALI_MMU_FLAGS_DEFAULT;
	} else {
		/* Cached Mali memory mapping */
		descriptor->mali_mapping.properties = MALI_MMU_FLAGS_FORCE_GP_READ_ALLOCATE;
		vma->vm_flags |= VM_SHARED;
	}

	err = mali_mem_os_alloc_pages(descriptor, size); /* Allocate pages */
	if (0 != err) goto alloc_failed;

#if MSTAR_STATISTIC_MALI_MIU_USAGE
	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		session->num_pages_allocated_miu[i] += descriptor->num_pages_allocated_miu[i];
	}
#endif


	/* Take session memory lock */
	_mali_osk_mutex_wait(session->memory_lock);

	err = mali_mem_os_mali_map(descriptor, session); /* Map on Mali */
	if (0 != err) goto mali_map_failed;

	_mali_osk_mutex_signal(session->memory_lock);

	err = mali_mem_os_cpu_map(descriptor, vma); /* Map on CPU */
	if (0 != err) goto cpu_map_failed;

	return descriptor;

cpu_map_failed:
	mali_mem_os_mali_unmap(session, descriptor);
mali_map_failed:
	_mali_osk_mutex_signal(session->memory_lock);
	mali_mem_os_free(descriptor);
alloc_failed:
	mali_mem_descriptor_destroy(descriptor);
	MALI_DEBUG_PRINT(2, ("OS allocator: Failed to allocate memory (%d)\n", err));
	return NULL;
}

void mali_mem_os_release(mali_mem_allocation *descriptor)
{
	struct mali_session_data *session = descriptor->session;

	/* Unmap the memory from the mali virtual address space. */
	mali_mem_os_mali_unmap(session, descriptor);

	/* Free pages */
	mali_mem_os_free(descriptor);
}


#define MALI_MEM_OS_PAGE_TABLE_PAGE_POOL_SIZE 128
static struct {
	struct {
		mali_dma_addr phys;
		mali_io_address mapping;
	} page[MALI_MEM_OS_PAGE_TABLE_PAGE_POOL_SIZE];
	size_t count;
	spinlock_t lock;
} mali_mem_page_table_page_pool = {
	.count = 0,
	.lock = __SPIN_LOCK_UNLOCKED(pool_lock),
};

_mali_osk_errcode_t mali_mem_os_get_table_page(mali_dma_addr *phys, mali_io_address *mapping)
{
	_mali_osk_errcode_t ret = _MALI_OSK_ERR_NOMEM;
	dma_addr_t tmp_phys;

	spin_lock(&mali_mem_page_table_page_pool.lock);
	if (0 < mali_mem_page_table_page_pool.count) {
		u32 i = --mali_mem_page_table_page_pool.count;
		*phys = mali_mem_page_table_page_pool.page[i].phys;
		*mapping = mali_mem_page_table_page_pool.page[i].mapping;

		ret = _MALI_OSK_ERR_OK;
	}
	spin_unlock(&mali_mem_page_table_page_pool.lock);

	if (_MALI_OSK_ERR_OK != ret) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
		*mapping = dma_alloc_attrs(&mali_platform_device->dev,
					   _MALI_OSK_MALI_PAGE_SIZE, &tmp_phys,
					   GFP_KERNEL, &dma_attrs_wc);
#else
		*mapping = dma_alloc_writecombine(&mali_platform_device->dev,
						  _MALI_OSK_MALI_PAGE_SIZE, &tmp_phys, GFP_KERNEL);
#endif
		if (NULL != *mapping) {
			ret = _MALI_OSK_ERR_OK;

#if defined(CONFIG_ARCH_DMA_ADDR_T_64BIT)
			/* Verify that the "physical" address is 32-bit and
			 * usable for Mali, when on a system with bus addresses
			 * wider than 32-bit. */
			MALI_DEBUG_ASSERT(0 == (tmp_phys >> 32));
#endif

			*phys = (mali_dma_addr)tmp_phys;
		}
	}

/* MSTAR patch begin */
#ifdef MSTAR
	*phys -= PHYS_TO_BUS_ADDRESS_ADJUST;
#endif
/* MSTAR patch end */

	return ret;
}

void mali_mem_os_release_table_page(mali_dma_addr phys, void *virt)
{
/* MSTAR patch begin */
#ifdef MSTAR
		phys += PHYS_TO_BUS_ADDRESS_ADJUST;
#endif
/* MSTAR patch end */

	spin_lock(&mali_mem_page_table_page_pool.lock);
	if (MALI_MEM_OS_PAGE_TABLE_PAGE_POOL_SIZE > mali_mem_page_table_page_pool.count) {
		u32 i = mali_mem_page_table_page_pool.count;
		mali_mem_page_table_page_pool.page[i].phys = phys;
		mali_mem_page_table_page_pool.page[i].mapping = virt;

		++mali_mem_page_table_page_pool.count;

		spin_unlock(&mali_mem_page_table_page_pool.lock);
	} else {
		spin_unlock(&mali_mem_page_table_page_pool.lock);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
		dma_free_attrs(&mali_platform_device->dev,
			       _MALI_OSK_MALI_PAGE_SIZE, virt, phys,
			       &dma_attrs_wc);
#else
		dma_free_writecombine(&mali_platform_device->dev,
				      _MALI_OSK_MALI_PAGE_SIZE, virt, phys);
#endif
	}
}

static void mali_mem_os_free_page(struct page *page)
{
	BUG_ON(page_count(page) != 1);

	dma_unmap_page(&mali_platform_device->dev, page_private(page),
		       _MALI_OSK_MALI_PAGE_SIZE, DMA_TO_DEVICE);

	ClearPagePrivate(page);

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	mali_free_page(page);
#else
	__free_page(page);
#endif
/* MSTAR patch end */
}

/* The maximum number of page table pool pages to free in one go. */
#define MALI_MEM_OS_CHUNK_TO_FREE 64UL

/* Free a certain number of pages from the page table page pool.
 * The pool lock must be held when calling the function, and the lock will be
 * released before returning.
 */
static void mali_mem_os_page_table_pool_free(size_t nr_to_free)
{
	mali_dma_addr phys_arr[MALI_MEM_OS_CHUNK_TO_FREE];
	void *virt_arr[MALI_MEM_OS_CHUNK_TO_FREE];
	u32 i;

	MALI_DEBUG_ASSERT(nr_to_free <= MALI_MEM_OS_CHUNK_TO_FREE);

	/* Remove nr_to_free pages from the pool and store them locally on stack. */
	for (i = 0; i < nr_to_free; i++) {
		u32 pool_index = mali_mem_page_table_page_pool.count - i - 1;

		phys_arr[i] = mali_mem_page_table_page_pool.page[pool_index].phys;
		virt_arr[i] = mali_mem_page_table_page_pool.page[pool_index].mapping;
	}

	mali_mem_page_table_page_pool.count -= nr_to_free;

	spin_unlock(&mali_mem_page_table_page_pool.lock);

	/* After releasing the spinlock: free the pages we removed from the pool. */
	for (i = 0; i < nr_to_free; i++) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
		dma_free_attrs(&mali_platform_device->dev, _MALI_OSK_MALI_PAGE_SIZE,
			       virt_arr[i], (dma_addr_t)phys_arr[i], &dma_attrs_wc);
#else
		dma_free_writecombine(&mali_platform_device->dev,
				      _MALI_OSK_MALI_PAGE_SIZE,
				      virt_arr[i], (dma_addr_t)phys_arr[i]);
#endif
	}
}

static void mali_mem_os_trim_page_table_page_pool(void)
{
	size_t nr_to_free = 0;
/* MSTAR patch begin */
#ifdef MSTAR
	size_t nr_to_keep = 0;
#else
	size_t nr_to_keep;
#endif
/* MSTAR patch end */

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	{
		int i;
		for (i = 0; i < MSTAR_MIU_NUM; i++)
		{
			/* Keep 2 page table pages for each 1024 pages in the page cache. */
			nr_to_keep += mali_mem_os_allocator.pool_count[i] / 512;
		}
	}
#else
	/* Keep 2 page table pages for each 1024 pages in the page cache. */
	nr_to_keep = mali_mem_os_allocator.pool_count / 512;
#endif
/* MSTAR patch end */

	/* And a minimum of eight pages, to accomodate new sessions. */
	nr_to_keep += 8;

	if (0 == spin_trylock(&mali_mem_page_table_page_pool.lock)) return;

	if (nr_to_keep < mali_mem_page_table_page_pool.count) {
		nr_to_free = mali_mem_page_table_page_pool.count - nr_to_keep;
		nr_to_free = min((size_t)MALI_MEM_OS_CHUNK_TO_FREE, nr_to_free);
	}

	/* Pool lock will be released by the callee. */
	mali_mem_os_page_table_pool_free(nr_to_free);
}

static unsigned long mali_mem_os_shrink_count(struct shrinker *shrinker, struct shrink_control *sc)
{
/* MSTAR patch begin */
#ifdef MSTAR
	return (unsigned long)(mali_mem_os_allocator.pool_count + mali_mem_page_table_page_pool.count);
#else
	return mali_mem_os_allocator.pool_count + mali_mem_page_table_page_pool.count;
#endif
/* MSTAR patch end */
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 0, 0)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
static int mali_mem_os_shrink(int nr_to_scan, gfp_t gfp_mask)
#else
static int mali_mem_os_shrink(struct shrinker *shrinker, int nr_to_scan, gfp_t gfp_mask)
#endif /* Linux < 2.6.35 */
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 12, 0)
static int mali_mem_os_shrink(struct shrinker *shrinker, struct shrink_control *sc)
#else
static unsigned long mali_mem_os_shrink(struct shrinker *shrinker, struct shrink_control *sc)
#endif /* Linux < 3.12.0 */
#endif /* Linux < 3.0.0 */
{
	struct page *page, *tmp;
	unsigned long flags;
	struct list_head *le, pages;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 0, 0)
	int nr = nr_to_scan;
#else
	int nr = sc->nr_to_scan;
#endif

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	int i;
	size_t remaining_pool_count = 0;

	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		remaining_pool_count += mali_mem_os_allocator.pool_count[i];
	}

	if (0 == nr) {
		return remaining_pool_count + mali_mem_page_table_page_pool.count;
	}

	if (0 == remaining_pool_count) {
		/* No pages availble */
		return 0;
	}

	/* Release from general page pool */
	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		int miu_nr = min((size_t)nr, mali_mem_os_allocator.pool_count[i]);

		INIT_LIST_HEAD(&pages);

		if (miu_nr == 0) break;

		if (0 == spin_trylock_irqsave(&mali_mem_os_allocator.pool_lock, flags)) {
			/* Not able to lock. */
			return -1;
		}

		nr = nr - miu_nr;
		mali_mem_os_allocator.pool_count[i] -= miu_nr;
		remaining_pool_count -=miu_nr;
		list_for_each(le, &mali_mem_os_allocator.pool_pages[i]) {
			--miu_nr;
			if (0 == miu_nr) break;
		}
		list_cut_position(&pages, &mali_mem_os_allocator.pool_pages[i], le);
		spin_unlock_irqrestore(&mali_mem_os_allocator.pool_lock, flags);

		list_for_each_entry_safe(page, tmp, &pages, lru) {
			mali_mem_os_free_page(page);
		}
	}

	/* Release some pages from page table page pool */
	mali_mem_os_trim_page_table_page_pool();

	{
		int stop_pool_trim_timer = 1;

		for (i = 0; i < MSTAR_MIU_NUM; i++)
		{
			if(MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES > mali_mem_os_allocator.pool_count[i])
			{
				stop_pool_trim_timer = 0;
				break;
			}
		}
		if (stop_pool_trim_timer == 1) {
			for (i = 0; i < MSTAR_MIU_NUM; i++)
			{
				MALI_DEBUG_PRINT(5, ("OS Mem: Stopping pool trim timer, only %u pages on pool[%d]\n", mali_mem_os_allocator.pool_count,i));
			}
			cancel_delayed_work(&mali_mem_os_allocator.timed_shrinker);
		}
	}

	return remaining_pool_count + mali_mem_page_table_page_pool.count;
#else
	if (0 == nr) {
		return mali_mem_os_shrink_count(shrinker, sc);
	}

	if (0 == spin_trylock_irqsave(&mali_mem_os_allocator.pool_lock, flags)) {
		/* Not able to lock. */
		return -1;
	}

	if (0 == mali_mem_os_allocator.pool_count) {
		/* No pages availble */
		spin_unlock_irqrestore(&mali_mem_os_allocator.pool_lock, flags);
		return 0;
	}

	/* Release from general page pool */
	nr = min((size_t)nr, mali_mem_os_allocator.pool_count);
	mali_mem_os_allocator.pool_count -= nr;
	list_for_each(le, &mali_mem_os_allocator.pool_pages) {
		--nr;
		if (0 == nr) break;
	}
	list_cut_position(&pages, &mali_mem_os_allocator.pool_pages, le);
	spin_unlock_irqrestore(&mali_mem_os_allocator.pool_lock, flags);

	list_for_each_entry_safe(page, tmp, &pages, lru) {
		mali_mem_os_free_page(page);
	}

	/* Release some pages from page table page pool */
	mali_mem_os_trim_page_table_page_pool();

	if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES > mali_mem_os_allocator.pool_count) {
		/* Pools are empty, stop timer */
		MALI_DEBUG_PRINT(5, ("Stopping timer, only %u pages on pool\n", mali_mem_os_allocator.pool_count));
		cancel_delayed_work(&mali_mem_os_allocator.timed_shrinker);
	}

	return mali_mem_os_allocator.pool_count + mali_mem_page_table_page_pool.count;
#endif
/* MSTAR patch end */
}

static void mali_mem_os_trim_pool(struct work_struct *data)
{
	struct page *page, *tmp;
	struct list_head *le;
	LIST_HEAD(pages);
	size_t nr_to_free;

	MALI_IGNORE(data);

	/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	/* Release from general page pool */
	{
		int i;
		for (i = 0 ;i < MSTAR_MIU_NUM; i++)
		{
			MALI_DEBUG_PRINT(3, ("OS Mem: Trimming pool[%u] %u\n",i , mali_mem_os_allocator.pool_count[i]));
			INIT_LIST_HEAD(&pages);
			spin_lock(&mali_mem_os_allocator.pool_lock);
			if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES < mali_mem_os_allocator.pool_count[i]) {
				size_t count = mali_mem_os_allocator.pool_count[i] - MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES;
				/* Free half the pages on the pool above the static limit. Or 64 pages, 256KB. */
				nr_to_free = max(count / 2, (size_t)64);

				mali_mem_os_allocator.pool_count[i] -= nr_to_free;
				list_for_each(le, &mali_mem_os_allocator.pool_pages[i]) {
					--nr_to_free;
					if (0 == nr_to_free) break;
				}
				list_cut_position(&pages, &mali_mem_os_allocator.pool_pages[i], le);
			}
			spin_unlock(&mali_mem_os_allocator.pool_lock);
			list_for_each_entry_safe(page, tmp, &pages, lru) {
				mali_mem_os_free_page(page);
			}
		}
	}
	/* Release some pages from page table page pool */
	mali_mem_os_trim_page_table_page_pool();

	{
		int start_pool_trim_timer = 0;
		int i;
		for (i = 0; i < MSTAR_MIU_NUM; i++)
		{
			if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES < mali_mem_os_allocator.pool_count[i])
			{
				start_pool_trim_timer = 1;
				break;
			}
		}
		if (start_pool_trim_timer) {
			for (i = 0; i < MSTAR_MIU_NUM; i++)
			{
				MALI_DEBUG_PRINT(4, ("OS Mem: Starting pool[%d] trim timer %u\n",i , mali_mem_os_allocator.pool_count[i]));
			}
			queue_delayed_work(mali_mem_os_allocator.wq, &mali_mem_os_allocator.timed_shrinker, MALI_OS_MEMORY_POOL_TRIM_JIFFIES);
		}
	}
#else
	MALI_DEBUG_PRINT(3, ("OS Mem: Trimming pool %u\n", mali_mem_os_allocator.pool_count));

	/* Release from general page pool */
	spin_lock(&mali_mem_os_allocator.pool_lock);
	if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES < mali_mem_os_allocator.pool_count) {
		size_t count = mali_mem_os_allocator.pool_count - MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES;
		const size_t min_to_free = min(64, MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES);

		/* Free half the pages on the pool above the static limit. Or 64 pages, 256KB. */
		nr_to_free = max(count / 2, min_to_free);

		mali_mem_os_allocator.pool_count -= nr_to_free;
		list_for_each(le, &mali_mem_os_allocator.pool_pages) {
			--nr_to_free;
			if (0 == nr_to_free) break;
		}
		list_cut_position(&pages, &mali_mem_os_allocator.pool_pages, le);
	}
	spin_unlock(&mali_mem_os_allocator.pool_lock);

	list_for_each_entry_safe(page, tmp, &pages, lru) {
		mali_mem_os_free_page(page);
	}

	/* Release some pages from page table page pool */
	mali_mem_os_trim_page_table_page_pool();

	if (MALI_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES < mali_mem_os_allocator.pool_count) {
		MALI_DEBUG_PRINT(4, ("OS Mem: Starting pool trim timer %u\n", mali_mem_os_allocator.pool_count));
		queue_delayed_work(mali_mem_os_allocator.wq, &mali_mem_os_allocator.timed_shrinker, MALI_OS_MEMORY_POOL_TRIM_JIFFIES);
	}
#endif
	/* MSTAR patch end */
}

_mali_osk_errcode_t mali_mem_os_init(void)
{
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	int i;
	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		INIT_LIST_HEAD(&(mali_mem_os_allocator.pool_pages[i]));
		mali_mem_os_allocator.pool_count[i] = 0;
		atomic_set(&(mali_mem_os_allocator.allocated_pages[i]),0);
	}
#endif
/* MSTAR patch end */

	mali_mem_os_allocator.wq = alloc_workqueue("mali-mem", WQ_UNBOUND, 1);
	if (NULL == mali_mem_os_allocator.wq) {
		return _MALI_OSK_ERR_NOMEM;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
	dma_set_attr(DMA_ATTR_WRITE_COMBINE, &dma_attrs_wc);
#endif

	register_shrinker(&mali_mem_os_allocator.shrinker);

	return _MALI_OSK_ERR_OK;
}

void mali_mem_os_term(void)
{
	struct page *page, *tmp;
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	int i;
#endif
/* MSTAR patch end */

	unregister_shrinker(&mali_mem_os_allocator.shrinker);
	cancel_delayed_work_sync(&mali_mem_os_allocator.timed_shrinker);
	destroy_workqueue(mali_mem_os_allocator.wq);

/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	for (i = 0; i < MSTAR_MIU_NUM; i++)
	{
		spin_lock(&mali_mem_os_allocator.pool_lock);
		list_for_each_entry_safe(page, tmp, &mali_mem_os_allocator.pool_pages[i], lru) {
			mali_mem_os_free_page(page);

			--mali_mem_os_allocator.pool_count[i];
		}
		BUG_ON(mali_mem_os_allocator.pool_count[i]);
		spin_unlock(&mali_mem_os_allocator.pool_lock);
	}
#else
	spin_lock(&mali_mem_os_allocator.pool_lock);
	list_for_each_entry_safe(page, tmp, &mali_mem_os_allocator.pool_pages, lru) {
		mali_mem_os_free_page(page);

		--mali_mem_os_allocator.pool_count;
	}
	BUG_ON(mali_mem_os_allocator.pool_count);
	spin_unlock(&mali_mem_os_allocator.pool_lock);
#endif
/* MSTAR patch end */

	/* Release from page table page pool */
	do {
		u32 nr_to_free;

		spin_lock(&mali_mem_page_table_page_pool.lock);

		nr_to_free = min((size_t)MALI_MEM_OS_CHUNK_TO_FREE, mali_mem_page_table_page_pool.count);

		/* Pool lock will be released by the callee. */
		mali_mem_os_page_table_pool_free(nr_to_free);
	} while (0 != mali_mem_page_table_page_pool.count);
}

_mali_osk_errcode_t mali_memory_core_resource_os_memory(u32 size)
{
	mali_mem_os_allocator.allocation_limit = size;

	MALI_SUCCESS;
}

u32 mali_mem_os_stat(void)
{
/* MSTAR patch begin */
#if defined(CONFIG_MP_MM_CMA) || defined(CONFIG_MP_ION_PATCH_MSTAR)
	int i , alloc_bytes = 0;
	for(i = 0; i < MSTAR_MIU_NUM; i++)
	{
		alloc_bytes += atomic_read(&mali_mem_os_allocator.allocated_pages[i]) * _MALI_OSK_MALI_PAGE_SIZE;
	}
	return alloc_bytes;
#else
	return atomic_read(&mali_mem_os_allocator.allocated_pages) * _MALI_OSK_MALI_PAGE_SIZE;
#endif
/* MSTAR patch end */
}
