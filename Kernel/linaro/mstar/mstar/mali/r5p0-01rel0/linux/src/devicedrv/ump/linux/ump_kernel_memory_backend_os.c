/*
 * Copyright (C) 2010-2011, 2013-2014 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* needed to detect kernel version specific code */
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#include <linux/semaphore.h>
#else /* pre 2.6.26 the file was in the arch specific location */
#include <asm/semaphore.h>
#endif

#include <linux/dma-mapping.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/atomic.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>
#ifdef UMP_MEMORY_PRE_ALLOCATION
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,1,0)
#include <linux/shrinker.h>
#endif
#include <linux/list.h>
#endif
#include "ump_kernel_common.h"
#include "ump_kernel_memory_backend.h"

#ifdef MSTAR
#ifndef PHYS_TO_BUS_ADDRESS_ADJUST
#include "mstar/mstar_chip.h"
#define PHYS_TO_BUS_ADDRESS_ADJUST  MSTAR_MIU0_BUS_BASE
#endif

#if MSTAR_CONTROL_MIU
#if defined(CONFIG_MP_MM_CMA)
#include <linux/cma_allocator.h>
extern void get_mali_alloc_strategy(unsigned long *ump, unsigned long *mali);
extern void set_mali_alloc_strategy(unsigned long ump, unsigned long mali);
#elif defined(CONFIG_MP_ION_PATCH_MSTAR)
extern void set_mali_alloc_strategy(unsigned long mali);
extern unsigned long get_mali_alloc_strategy();
#endif

static unsigned long get_ump_alloc_strategy_wrapper(void)
{
#if defined(CONFIG_MP_MM_CMA)
	unsigned long mali_strategy, ump_strategy;
	get_mali_alloc_strategy(&ump_strategy, &mali_strategy);
	return ump_strategy;
#elif defined(CONFIG_MP_ION_PATCH_MSTAR)
	return get_mali_alloc_strategy();
#endif
}

static void set_ump_alloc_strategy_wrapper(unsigned long mali_strategy)
{
#if defined(CONFIG_MP_MM_CMA)
	set_mali_alloc_strategy(mali_strategy, 0/*not used*/);
#elif defined(CONFIG_MP_ION_PATCH_MSTAR)
	set_mali_alloc_strategy(mali_strategy);
#endif
}
#endif
#endif

#ifdef PHYS_TO_BUS_ADDRESS_ADJUST
#define GET_BUS_ADDRESS(phy_addr)   ((phy_addr) + PHYS_TO_BUS_ADDRESS_ADJUST)
#else
#define GET_BUS_ADDRESS(phy_addr)   (phy_addr)
#endif /* MIU0_BUS_ADDRESS_BASE */

#define ADDR_TO_PAGE(addr) pfn_to_page((addr) >> PAGE_SHIFT)
#define INVALID_PAGE_ADDR ((unsigned long)-1)

typedef struct os_allocator {
	struct semaphore mutex;
	u32 num_pages_max;       /**< Maximum number of pages to allocate from the OS */
	u32 num_pages_allocated; /**< Number of pages allocated from the OS */
#ifdef UMP_MEMORY_PRE_ALLOCATION
	struct page *pre_allocated_pages[MSTAR_MIU_NUM]; 
	u32 num_pages_pre_allocated[MSTAR_MIU_NUM]; /***< pre_allocated_page only support non-cache pages */
	struct list_head list;
#endif
} os_allocator;

static unsigned long _kernel_page_addr_allocate(u32 is_cached);
static void os_free(void* ctx, ump_dd_mem * descriptor);
static int os_allocate(void* ctx, ump_dd_mem * descriptor);
static void os_memory_backend_destroy(ump_memory_backend * backend);
static u32 os_stat(struct ump_memory_backend *backend);

#ifdef UMP_MEMORY_PRE_ALLOCATION
extern unsigned int ump_max_pre_alloc_size;
#define UMP_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES (ump_max_pre_alloc_size / PAGE_SIZE)

static DEFINE_SPINLOCK(os_allocator_list_spinlock);
LIST_HEAD(os_allocator_list);

static int ump_mem_shrink(struct shrinker *shrinker, struct shrink_control *sc);
struct shrinker ump_mem_shrinker = {
	.shrink = ump_mem_shrink,
	.seeks = DEFAULT_SEEKS,
};

static u32 _get_current_num_pages_pre_allocated(void);
static u32 os_pre_allocate(void* ctx, u32 size, u32 is_cached);
static unsigned long _get_page_addr(void* ctx, int is_cached, int miu_id);
#else
#define _get_page_addr(ctx, is_cached, miu_id) _kernel_page_addr_allocate(is_cached)
#endif

/*
 * Create OS memory backend
 */
ump_memory_backend * ump_os_memory_backend_create(const int max_allocation)
{
	ump_memory_backend * backend;
	os_allocator * info;
#ifdef UMP_MEMORY_PRE_ALLOCATION
	int miu_id;
#endif

	info = kmalloc(sizeof(os_allocator), GFP_KERNEL);
	if (NULL == info) {
		return NULL;
	}

	info->num_pages_max = max_allocation >> PAGE_SHIFT;
	info->num_pages_allocated = 0;

#ifdef UMP_MEMORY_PRE_ALLOCATION
	for (miu_id = 0; miu_id < ARRAY_SIZE(info->pre_allocated_pages); miu_id++)
	{
		info->pre_allocated_pages[miu_id] = NULL;
		info->num_pages_pre_allocated[miu_id] = 0;
	}

	INIT_LIST_HEAD(&info->list);
#endif
	sema_init(&info->mutex, 1);

	backend = kmalloc(sizeof(ump_memory_backend), GFP_KERNEL);
	if (NULL == backend) {
		kfree(info);
		return NULL;
	}

	backend->ctx = info;
	backend->allocate = os_allocate;
	backend->release = os_free;
	backend->shutdown = os_memory_backend_destroy;
	backend->stat = os_stat;
	backend->pre_allocate_physical_check = NULL;
	backend->adjust_to_mali_phys = NULL;

#ifdef UMP_MEMORY_PRE_ALLOCATION 
	{
		unsigned long flags;
		u32 current_pre_alloc_size;
		int is_cache = 0;

		spin_lock_irqsave(&os_allocator_list_spinlock, flags);

		if (list_empty(&os_allocator_list))
		{
			register_shrinker(&ump_mem_shrinker);
		}

		list_add_tail(&info->list, &os_allocator_list);
		spin_unlock_irqrestore(&os_allocator_list_spinlock, flags);

		/*pre-alloc pages in kdriver internal*/
		current_pre_alloc_size = os_pre_allocate((void *)info, ump_max_pre_alloc_size, is_cache);

		DBG_MSG(2, ("UMP pre-alloc 0x%X\n",(u32)current_pre_alloc_size));
	}
#endif
	return backend;
}



/*
 * Destroy specified OS memory backend
 */
static void os_memory_backend_destroy(ump_memory_backend * backend)
{
	os_allocator * info = (os_allocator*)backend->ctx;
#ifdef UMP_MEMORY_PRE_ALLOCATION
	int miu_id;
	unsigned long flags;

	spin_lock_irqsave(&os_allocator_list_spinlock, flags);

	list_del(&info->list);

	if (list_empty(&os_allocator_list))
	{
		unregister_shrinker(&ump_mem_shrinker);
	}

	spin_unlock_irqrestore(&os_allocator_list_spinlock, flags);

	for (miu_id = 0; miu_id < ARRAY_SIZE(info->pre_allocated_pages); miu_id++)
	{
		struct page *tmp_ptr_page, *ptr_page = info->pre_allocated_pages[miu_id];

		while (ptr_page != NULL)
		{	
			tmp_ptr_page = ptr_page;
			ptr_page = ptr_page->first_page;
			tmp_ptr_page->first_page = NULL;
#if MSTAR_CONTROL_MIU
			mali_free_page(tmp_ptr_page);
#else
			__free_page(tmp_ptr_page);
#endif
			info->num_pages_pre_allocated[miu_id]--;
			info->num_pages_allocated--;
		}

		info->pre_allocated_pages[miu_id] = NULL;
	}
#endif

	DBG_MSG_IF(1, 0 != info->num_pages_allocated, ("%d pages still in use during shutdown\n", info->num_pages_allocated));

	kfree(info);
	kfree(backend);
}

#if defined(CONFIG_MP_ION_PATCH_MSTAR)
extern struct page* mali_alloc_page();
extern void mali_free_page(struct page*);
#endif

/*
 * Allocate UMP memory
 */
static int os_allocate(void* ctx, ump_dd_mem * descriptor)
{
	u32 left;
	os_allocator * info;
	int pages_allocated = 0;
	int is_cached;
#if MSTAR_CONTROL_MIU
	int miu_id;
#endif
	BUG_ON(!descriptor);
	BUG_ON(!ctx);

	info = (os_allocator*)ctx;
	left = descriptor->size_bytes;
	is_cached = descriptor->is_cached;

	if (down_interruptible(&info->mutex)) {
		DBG_MSG(1, ("Failed to get mutex in os_allocate\n")); /* MStar modification */
		return 0; /* failure */
	}

	descriptor->backend_info = NULL;
	descriptor->nr_blocks = ((left + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1)) >> PAGE_SHIFT;
#if MSTAR_CONTROL_MIU
	descriptor->miu_id = get_ump_alloc_strategy_wrapper();
	miu_id = descriptor->miu_id;
#endif

	DBG_MSG(5, ("Allocating page array. Size: %lu\n", descriptor->nr_blocks * sizeof(ump_dd_physical_block)));

	descriptor->block_array = (ump_dd_physical_block *)vmalloc(sizeof(ump_dd_physical_block) * descriptor->nr_blocks);
	if (NULL == descriptor->block_array) {
		up(&info->mutex);
		DBG_MSG(1, ("Block array could not be allocated\n"));
		return 0; /* failure */
	}
#ifdef UMP_MEMORY_PRE_ALLOCATION
	while (left > 0 && (((info->num_pages_allocated + pages_allocated) < info->num_pages_max) || (info->num_pages_pre_allocated[miu_id] > 0)))
#else
	while (left > 0 && ((info->num_pages_allocated + pages_allocated) < info->num_pages_max))
#endif
	{
		unsigned long new_page_addr;
		new_page_addr = _get_page_addr(info, is_cached, miu_id);
		if (INVALID_PAGE_ADDR == new_page_addr)
		{
			break;
		}

		descriptor->block_array[pages_allocated].addr = new_page_addr;
		descriptor->block_array[pages_allocated].size = PAGE_SIZE;

#ifdef PHYS_TO_BUS_ADDRESS_ADJUST
		descriptor->block_array[pages_allocated].addr -= PHYS_TO_BUS_ADDRESS_ADJUST;
#endif

		DBG_MSG(5, ("Allocated page 0x%08lx cached: %d\n", descriptor->block_array[pages_allocated].addr, is_cached));

		if (left < PAGE_SIZE) {
			left = 0;
		} else {
			left -= PAGE_SIZE;
		}

		pages_allocated++;
	}

	DBG_MSG(5, ("Alloce for ID:%2d got %d pages, cached: %d\n", descriptor->secure_id,  pages_allocated));

	if (left)
	{
		DBG_MSG(1, ("Failed to allocate needed pages\n"));
#ifdef UMP_MEMORY_PRE_ALLOCATION
		DBG_MSG(1, ("info->num_pages_allocated %u\n", info->num_pages_allocated ));
		DBG_MSG(1, ("info->num_pages_pre_allocated[%d] %u\n", miu_id, info->num_pages_pre_allocated[miu_id] ));
#endif
		while(pages_allocated)
		{
			pages_allocated--;
			if ( !is_cached )
			{
#ifdef MSTAR
				dma_unmap_page(NULL, GET_BUS_ADDRESS(descriptor->block_array[pages_allocated].addr), PAGE_SIZE, DMA_BIDIRECTIONAL);
#else
				dma_unmap_page(NULL, descriptor->block_array[pages_allocated].addr, PAGE_SIZE, DMA_BIDIRECTIONAL);
#endif
			}
#ifdef MSTAR
#if MSTAR_CONTROL_MIU
			mali_free_page(ADDR_TO_PAGE(GET_BUS_ADDRESS(descriptor->block_array[pages_allocated].addr)));
#else
			__free_page(ADDR_TO_PAGE(GET_BUS_ADDRESS(descriptor->block_array[pages_allocated].addr)));
#endif
#else
			__free_page(pfn_to_page(descriptor->block_array[pages_allocated].addr >> PAGE_SHIFT));
#endif
		}

		up(&info->mutex);

		return 0; /* failure */
	}

	info->num_pages_allocated += pages_allocated;

	DBG_MSG(6, ("%d out of %d pages now allocated\n", info->num_pages_allocated, info->num_pages_max));

	up(&info->mutex);

	return 1; /* success*/
}


/*
 * Free specified UMP memory
 */
static void os_free(void* ctx, ump_dd_mem * descriptor)
{
	os_allocator * info;
	int i;

#ifdef UMP_MEMORY_PRE_ALLOCATION
	u32 keep_pre_alloc_num = 0;
	int ump_id, is_cached; 
#endif
	BUG_ON(!ctx);
	BUG_ON(!descriptor);

	info = (os_allocator*)ctx;

	BUG_ON(descriptor->nr_blocks > info->num_pages_allocated);

	if (down_interruptible(&info->mutex)) {
		DBG_MSG(1, ("Failed to get mutex in os_free\n"));
		return;
	}

	DBG_MSG(5, ("Releasing %lu OS pages\n", descriptor->nr_blocks));

	info->num_pages_allocated -= descriptor->nr_blocks;

#ifdef UMP_MEMORY_PRE_ALLOCATION
	i = 0;
#if MSTAR_CONTROL_MIU
	ump_id = descriptor->miu_id;
	is_cached = descriptor->is_cached;
#endif
	if (info->num_pages_pre_allocated[ump_id] <= UMP_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES)
	{
		if ((info->num_pages_pre_allocated[ump_id] + descriptor->nr_blocks) > UMP_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES)
		{
			keep_pre_alloc_num = UMP_OS_MEMORY_KERNEL_BUFFER_SIZE_IN_PAGES - info->num_pages_pre_allocated[ump_id];
		}
		else
		{
			keep_pre_alloc_num = descriptor->nr_blocks;
		}
		
		for (i = 0 ; i < keep_pre_alloc_num ; i++)
		{
			struct page* old_page;

			old_page = ADDR_TO_PAGE(GET_BUS_ADDRESS(descriptor->block_array[i].addr));
			old_page->first_page = info->pre_allocated_pages[ump_id]; 
			info->pre_allocated_pages[ump_id] = old_page;
		}
		
		info->num_pages_pre_allocated[ump_id] += keep_pre_alloc_num;
		info->num_pages_allocated += keep_pre_alloc_num;
	}
#endif

	up(&info->mutex);
#ifdef UMP_MEMORY_PRE_ALLOCATION
	for ( ; i < descriptor->nr_blocks; i++)
#else
	for ( i = 0; i < descriptor->nr_blocks; i++)
#endif
	{
		DBG_MSG(6, ("Freeing physical page. Address: 0x%08lx\n", descriptor->block_array[i].addr));

		up(&info->mutex);

#ifdef MSTAR
		if (!descriptor->is_cached)
		{
			dma_unmap_page(NULL, GET_BUS_ADDRESS(descriptor->block_array[i].addr), PAGE_SIZE, DMA_BIDIRECTIONAL);
		}
#if MSTAR_CONTROL_MIU
		mali_free_page(ADDR_TO_PAGE(GET_BUS_ADDRESS(descriptor->block_array[i].addr)));
#else
		__free_page(ADDR_TO_PAGE(GET_BUS_ADDRESS(descriptor->block_array[i].addr)));
#endif
#else
		if (!descriptor->is_cached)
		{
			dma_unmap_page(NULL, descriptor->block_array[i].addr, PAGE_SIZE, DMA_BIDIRECTIONAL);
		}
		__free_page(ADDR_TO_PAGE(descriptor->block_array[i].addr));
#endif
	}
	
	vfree(descriptor->block_array);
}


static u32 os_stat(struct ump_memory_backend *backend)
{
	os_allocator *info;
	info = (os_allocator*)backend->ctx;
	return info->num_pages_allocated * _MALI_OSK_MALI_PAGE_SIZE;
}

static struct page * _kernel_alloc_page(u32 is_cached)
{
	struct page * new_page;

#if MSTAR_CONTROL_MIU
	new_page = mali_alloc_page();
#else
	if (is_cached)
	{
		new_page = alloc_page(GFP_HIGHUSER | __GFP_ZERO | __GFP_REPEAT | __GFP_NOWARN);
	}
	else
	{
		new_page = alloc_page(GFP_HIGHUSER | __GFP_ZERO | __GFP_REPEAT | __GFP_NOWARN | __GFP_COLD);
	}
#endif

	return new_page;
}

static unsigned long _kernel_page_addr_allocate(u32 is_cached)
{
	struct page * new_page = _kernel_alloc_page(is_cached);

	if (new_page != NULL)
	{
		/* Ensure page caches are flushed. */
		if (is_cached)
		{
			return page_to_phys(new_page);
		} 
		else
		{
			return dma_map_page(NULL, new_page, 0, PAGE_SIZE, DMA_BIDIRECTIONAL);
		}
	}
	else
	{
		return INVALID_PAGE_ADDR;
	}
}

#ifdef UMP_MEMORY_PRE_ALLOCATION
static u32 os_pre_allocate(void* ctx, u32 size, u32 is_cached)
{
	struct page* new_page;
	os_allocator * info;
	u32 page_allocated = 0;
	u32 left = size;
#if MSTAR_CONTROL_MIU
	unsigned long ump_id = get_ump_alloc_strategy_wrapper();
#else
	unsigned long ump_id = 0;
#endif
	BUG_ON(!ctx);

	info = (os_allocator*)ctx;

	if (down_interruptible(&info->mutex))
	{
		DBG_MSG(1, ("Failed to get mutex in os_pre_allocate\n"));
		return 0;
	}

	while (left > 0 && ((info->num_pages_allocated + page_allocated) < info->num_pages_max))
	{
		new_page = _kernel_alloc_page(is_cached);
		if (NULL == new_page)
		{
			DBG_MSG(1, ("Failed to allocate page\n"));
			up(&info->mutex);
			return page_allocated * PAGE_SIZE;
		}
 
 		new_page->first_page = info->pre_allocated_pages[ump_id];
 		info->pre_allocated_pages[ump_id] = new_page;

		if (left < PAGE_SIZE)
		{
			left = 0;
		}
		else
		{
			left -= PAGE_SIZE;
		}

		page_allocated++;
	}

	info->num_pages_pre_allocated[ump_id] += page_allocated;
	info->num_pages_allocated += page_allocated;
	up(&info->mutex);

	return page_allocated * PAGE_SIZE;
}

/* This function must be called when ((os_allocator*)ctx)->mutex is locked */
static unsigned long _get_page_addr(void* ctx, int is_cached, int miu_id)
{
	os_allocator * info;

	BUG_ON(!ctx);

	info = (os_allocator*)ctx;

	if (info->pre_allocated_pages[miu_id] != NULL)
	{
		unsigned long new_page_addr;
		struct page* new_page = info->pre_allocated_pages[miu_id];
		
		info->pre_allocated_pages[miu_id] = new_page->first_page;
		new_page->first_page = NULL;
		
		if (is_cached)
		{
			new_page_addr = page_to_phys(new_page);
		}
		else
		{
			new_page_addr = dma_map_page(NULL, new_page, 0, PAGE_SIZE, DMA_BIDIRECTIONAL);
		}

		info->num_pages_allocated--;
		info->num_pages_pre_allocated[miu_id]--;

		return new_page_addr;
	}
	
	return _kernel_page_addr_allocate(is_cached);
}

static u32 _get_current_num_pages_pre_allocated(void)
{
	os_allocator* info;
	u32 num_pages_pre_allocated = 0;
	unsigned long flags;
	int miu_id;

	spin_lock_irqsave(&os_allocator_list_spinlock, flags);
	list_for_each_entry(info, &os_allocator_list, list)
	{
		for (miu_id = 0; miu_id < ARRAY_SIZE(info->pre_allocated_pages); miu_id++)
		{
			num_pages_pre_allocated += info->num_pages_pre_allocated[miu_id];
		}
	}
	spin_unlock_irqrestore(&os_allocator_list_spinlock, flags);

	return num_pages_pre_allocated;
}


static int ump_mem_shrink(struct shrinker *shrinker, struct shrink_control *sc)
{
	os_allocator* info;
	unsigned long flags;
	u32 _current_num_pages_pre_allocated;
	int nr = sc->nr_to_scan;

	int num_free_apges = 0;

	if (0 == nr)
	{
		return _get_current_num_pages_pre_allocated();
	}

	if (0 == _get_current_num_pages_pre_allocated())
	{
		/* No pages availble */
		return 0;
	}

	if (0 == spin_trylock_irqsave(&os_allocator_list_spinlock, flags))
	{
		/* Not able to lock. */
		return -1;
	}

	list_for_each_entry(info, &os_allocator_list, list)
	{
		int miu_id;

		if (down_trylock(&info->mutex))
		{
			continue;
		}

		for (miu_id = 0; miu_id < ARRAY_SIZE(info->pre_allocated_pages); miu_id++)
		{
			struct page *tmp_ptr_page, *ptr_page = info->pre_allocated_pages[miu_id];
			
			while (ptr_page != NULL)
			{	
				tmp_ptr_page = ptr_page;
				ptr_page = ptr_page->first_page;
				tmp_ptr_page->first_page = NULL;
#if MSTAR_CONTROL_MIU
				mali_free_page(tmp_ptr_page);
#else
				__free_page(tmp_ptr_page);
#endif
				info->pre_allocated_pages[miu_id] = ptr_page;
				info->num_pages_pre_allocated[miu_id]--;
				info->num_pages_allocated--;

				nr--;
				num_free_apges++;

				if (nr == 0)
				{
					up(&info->mutex);
					goto done;
				}
			}
		}

		up(&info->mutex);
	}

done:

	spin_unlock_irqrestore(&os_allocator_list_spinlock,flags);

	_current_num_pages_pre_allocated = _get_current_num_pages_pre_allocated();
	DBG_MSG(1,("UMP is shrinkered. Current UMP pre-alloc pages %u\n", _current_num_pages_pre_allocated));

	return _current_num_pages_pre_allocated;
}
#endif
