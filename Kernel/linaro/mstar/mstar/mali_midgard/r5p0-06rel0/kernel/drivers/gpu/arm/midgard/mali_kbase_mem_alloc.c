/*
 *
 * (C) COPYRIGHT ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */





/**
 * @file mali_kbase_mem.c
 * Base kernel memory APIs
 */
#include <mali_kbase.h>
#include <linux/dma-mapping.h>
#include <linux/highmem.h>
#include <linux/mempool.h>
#include <linux/mm.h>
#include <linux/atomic.h>
#include <linux/version.h>

/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
#include "mstar/mstar_chip.h"

extern struct page* mali_alloc_page(void);
extern void mali_free_page(struct page* page);
extern unsigned long get_mali_alloc_strategy( void);

static int _get_miu_id(phys_addr_t addr)
{
#ifdef MSTAR_MIU2_BUS_BASE
	if (addr >= MSTAR_MIU2_BUS_BASE) {
		return 2;
	}
#endif
	if (addr >= MSTAR_MIU1_BUS_BASE) {
		return 1;
	} else if (addr >= MSTAR_MIU0_BUS_BASE) {
		return 0;
	} else {
		return -1;
	}
}
#endif

static unsigned long _get_current_total_free_size(struct kbase_mem_allocator *allocator)
{
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	int i = 0;
	unsigned long num = 0;

	for (i = 0 ; i < MSTAR_MIU_NUM ; i++) {
		num += atomic_read(&allocator->free_list_size[i]);
	}

	return num;
#else
	return atomic_read(&allocator->free_list_size);
#endif
}
/* MSTAR patch end */

int kbase_mem_lowlevel_init(struct kbase_device *kbdev)
{
	return 0;
}

void kbase_mem_lowlevel_term(struct kbase_device *kbdev)
{
	return;
}

static unsigned long kbase_mem_allocator_count(struct shrinker *s,
						struct shrink_control *sc)
{
	struct kbase_mem_allocator *allocator;

	allocator = container_of(s, struct kbase_mem_allocator, free_list_reclaimer);
	/* MSTAR patch begin */
	return _get_current_total_free_size(allocator);
	/* MSTAR patch end */
}

static unsigned long kbase_mem_allocator_scan(struct shrinker *s,
						struct shrink_control *sc)
{
	struct kbase_mem_allocator *allocator;
	int i;
	int freed;

	allocator = container_of(s, struct kbase_mem_allocator, free_list_reclaimer);

	might_sleep();

	/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	{
		struct list_head* head;
		atomic_t *free_size;
		int miu_id = 0;

		mutex_lock(&allocator->free_list_lock);
		i = MIN(_get_current_total_free_size(allocator), sc->nr_to_scan);
		freed = i;

		head = &allocator->free_list_head[miu_id];
		free_size = &allocator->free_list_size[miu_id];

		while (i--) {
			struct page *p;

			while (list_empty(head)) {
				miu_id++;
				BUG_ON(miu_id == MSTAR_MIU_NUM);

				head = &allocator->free_list_head[miu_id];
				free_size = &allocator->free_list_size[miu_id];
			}

			atomic_dec(free_size);

			p = list_first_entry(head,
						struct page, lru);
			list_del(&p->lru);
			ClearPagePrivate(p);
			mali_free_page(p);
		}
		mutex_unlock(&allocator->free_list_lock);
		return _get_current_total_free_size(allocator);
	}
#else
	mutex_lock(&allocator->free_list_lock);
	i = MIN(atomic_read(&allocator->free_list_size), sc->nr_to_scan);
	freed = i;

	atomic_sub(i, &allocator->free_list_size);

	while (i--) {
		struct page *p;

		BUG_ON(list_empty(&allocator->free_list_head));
		p = list_first_entry(&allocator->free_list_head,
					struct page, lru);
		list_del(&p->lru);
		ClearPagePrivate(p);
		__free_page(p);
	}
	mutex_unlock(&allocator->free_list_lock);
	return atomic_read(&allocator->free_list_size);
#endif
	/* MSTAR patch end */
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 12, 0)
static int kbase_mem_allocator_shrink(struct shrinker *s,
		struct shrink_control *sc)
{
	if (sc->nr_to_scan == 0)
		return kbase_mem_allocator_count(s, sc);
	else
		return kbase_mem_allocator_scan(s, sc);
}
#endif

mali_error kbase_mem_allocator_init(struct kbase_mem_allocator *const allocator,
		unsigned int max_size, struct kbase_device *kbdev)
{
	KBASE_DEBUG_ASSERT(NULL != allocator);
	KBASE_DEBUG_ASSERT(kbdev);

	/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	{
		int i = 0;
		for ( i = 0; i < MSTAR_MIU_NUM; i++)
		{
			INIT_LIST_HEAD(&allocator->free_list_head[i]);
			atomic_set(&allocator->free_list_size[i], 0);
		}
	}
#else
	INIT_LIST_HEAD(&allocator->free_list_head);
	atomic_set(&allocator->free_list_size, 0);
#endif

	allocator->kbdev = kbdev;

	mutex_init(&allocator->free_list_lock);
	/* MSTAR patch end */
	allocator->free_list_max_size = max_size;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 12, 0)
	allocator->free_list_reclaimer.shrink = kbase_mem_allocator_shrink;
#else
	allocator->free_list_reclaimer.count_objects =
						kbase_mem_allocator_count;
	allocator->free_list_reclaimer.scan_objects = kbase_mem_allocator_scan;
#endif
	allocator->free_list_reclaimer.seeks = DEFAULT_SEEKS;
	/* Kernel versions prior to 3.1 :
	 * struct shrinker does not define batch */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0)
	allocator->free_list_reclaimer.batch = 0;
#endif

	register_shrinker(&allocator->free_list_reclaimer);

	return MALI_ERROR_NONE;
}
KBASE_EXPORT_TEST_API(kbase_mem_allocator_init)

void kbase_mem_allocator_term(struct kbase_mem_allocator *allocator)
{
	KBASE_DEBUG_ASSERT(NULL != allocator);

	unregister_shrinker(&allocator->free_list_reclaimer);
	mutex_lock(&allocator->free_list_lock);
	/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	{
		int i;
		for ( i = 0; i < MSTAR_MIU_NUM; i++) {
			while (!list_empty(&allocator->free_list_head[i])) {
				struct page * p;
				p = list_first_entry(&allocator->free_list_head[i], struct page,
						     lru);
				list_del(&p->lru);
				dma_unmap_page(allocator->kbdev->dev, kbase_dma_addr(p),
					       PAGE_SIZE,
					       DMA_BIDIRECTIONAL);
				ClearPagePrivate(p);
				mali_free_page(p);
			}
			atomic_set(&allocator->free_list_size[i], 0);
		}
	}
#else
	while (!list_empty(&allocator->free_list_head)) {
		struct page *p;

		p = list_first_entry(&allocator->free_list_head, struct page,
				     lru);
		list_del(&p->lru);
		dma_unmap_page(allocator->kbdev->dev, kbase_dma_addr(p),
			       PAGE_SIZE,
			       DMA_BIDIRECTIONAL);
		ClearPagePrivate(p);
		__free_page(p);
	}
	atomic_set(&allocator->free_list_size, 0);
#endif
	/* MSTAR patch end */
	mutex_unlock(&allocator->free_list_lock);
	mutex_destroy(&allocator->free_list_lock);
}
KBASE_EXPORT_TEST_API(kbase_mem_allocator_term)

mali_error kbase_mem_allocator_alloc(struct kbase_mem_allocator *allocator, size_t nr_pages, phys_addr_t *pages)
{
	struct page *p;
	void *mp;
	int i;
	int num_from_free_list;
	struct list_head from_free_list = LIST_HEAD_INIT(from_free_list);
	gfp_t gfp;

	might_sleep();

	KBASE_DEBUG_ASSERT(NULL != allocator);

	/* take from the free list first */
	mutex_lock(&allocator->free_list_lock);
	/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	{
		unsigned long mali_miu = get_mali_alloc_strategy();
		num_from_free_list = MIN(nr_pages, atomic_read(&allocator->free_list_size[mali_miu]));
		atomic_sub(num_from_free_list, &allocator->free_list_size[mali_miu]);
		for (i = 0; i < num_from_free_list; i++)
		{
			BUG_ON(list_empty(&allocator->free_list_head[mali_miu]));
			p = list_first_entry(&allocator->free_list_head[mali_miu], struct page, lru);
			list_move(&p->lru, &from_free_list);
		}
	}
#else
	num_from_free_list = MIN(nr_pages, atomic_read(&allocator->free_list_size));
	atomic_sub(num_from_free_list, &allocator->free_list_size);
	for (i = 0; i < num_from_free_list; i++)
	{
		BUG_ON(list_empty(&allocator->free_list_head));
		p = list_first_entry(&allocator->free_list_head, struct page, lru);
		list_move(&p->lru, &from_free_list);
	}
#endif
	/* MSTAR patch end */
	mutex_unlock(&allocator->free_list_lock);
	i = 0;

	/* Allocate as many pages from the pool of already allocated pages. */
	list_for_each_entry(p, &from_free_list, lru) {
		pages[i] = PFN_PHYS(page_to_pfn(p));
		i++;
	}

	if (i == nr_pages)
		return MALI_ERROR_NONE;

#if defined(CONFIG_ARM) && !defined(CONFIG_HAVE_DMA_ATTRS) && LINUX_VERSION_CODE < KERNEL_VERSION(3, 5, 0)
	/* DMA cache sync fails for HIGHMEM before 3.5 on ARM */
	gfp = GFP_USER;
#else
	gfp = GFP_HIGHUSER;
#endif

	if (current->flags & PF_KTHREAD) {
		/* Don't trigger OOM killer from kernel threads, e.g. when
		 * growing memory on GPU page fault */
		gfp |= __GFP_NORETRY;
	}

	/* If not all pages were sourced from the pool, request new ones. */
	for (; i < nr_pages; i++) {
		dma_addr_t dma_addr;
		/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
		p = mali_alloc_page();
#else
		p = alloc_page(gfp);
#endif
		/* MSTAR patch end */
		if (NULL == p)
			goto err_out_roll_back;
		mp = kmap(p);
		if (NULL == mp) {
			/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
			mali_free_page(p);
#else
			__free_page(p);
#endif
			/* MSTAR patch end */
			goto err_out_roll_back;
		}
		memset(mp, 0x00, PAGE_SIZE); /* instead of __GFP_ZERO, so we can do cache maintenance */
		kunmap(p);

		dma_addr = dma_map_page(allocator->kbdev->dev, p, 0, PAGE_SIZE,
				        DMA_BIDIRECTIONAL);
		if (dma_mapping_error(allocator->kbdev->dev, dma_addr)) {
			__free_page(p);
			goto err_out_roll_back;
		}

		SetPagePrivate(p);
		kbase_set_dma_addr(p, dma_addr);
		pages[i] = PFN_PHYS(page_to_pfn(p));
		BUG_ON(dma_addr != pages[i]);
	}

	return MALI_ERROR_NONE;

err_out_roll_back:
	while (i--) {
		struct page *p;
		p = pfn_to_page(PFN_DOWN(pages[i]));
		pages[i] = (phys_addr_t)0;
		dma_unmap_page(allocator->kbdev->dev, kbase_dma_addr(p),
			       PAGE_SIZE,
			       DMA_BIDIRECTIONAL);
		ClearPagePrivate(p);
		/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
		mali_free_page(p);
#else
		__free_page(p);
#endif
		/* MSTAR patch end */
	}

	return MALI_ERROR_OUT_OF_MEMORY;
}
KBASE_EXPORT_TEST_API(kbase_mem_allocator_alloc)

void kbase_mem_allocator_free(struct kbase_mem_allocator *allocator, size_t nr_pages, phys_addr_t *pages, mali_bool sync_back)
{
	int i = 0;
	/* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	int page_count[MSTAR_MIU_NUM];
	int might_reserve[MSTAR_MIU_NUM];
	struct list_head new_free_list_items[MSTAR_MIU_NUM];

	KBASE_DEBUG_ASSERT(NULL != allocator);

	might_sleep();

	for (i = 0 ; i < MSTAR_MIU_NUM ; i++)
	{
		INIT_LIST_HEAD(&new_free_list_items[i]);
		page_count[i] = 0;
		might_reserve[i] = MAX((int)allocator->free_list_max_size - atomic_read(&allocator->free_list_size[i]), 0);
	}

	for (i = 0 ; i < nr_pages ; i++)
	{
		if (likely(0 != pages[i]))
		{
			struct page * p;
			int miu_id = _get_miu_id(pages[i]);

			BUG_ON(miu_id < 0);
			p = pfn_to_page(PFN_DOWN(pages[i]));
			pages[i] = (phys_addr_t)0;

			if (might_reserve[miu_id] > 0)
			{
				/* Sync back the memory to ensure that future cache
				 * invalidations don't trample on memory.
				 */
				if( sync_back )
					dma_sync_single_for_cpu(allocator->kbdev->dev,
							kbase_dma_addr(p),
							PAGE_SIZE,
							DMA_BIDIRECTIONAL);

				list_add(&p->lru, &new_free_list_items[miu_id]);
				page_count[miu_id]++;
				might_reserve[miu_id]--;
			}
			else
			{
				dma_unmap_page(allocator->kbdev->dev, kbase_dma_addr(p),
		  			       PAGE_SIZE,
			  		       DMA_BIDIRECTIONAL);
			  	ClearPagePrivate(p);
				mali_free_page(p);
			}
		}
	}

	mutex_lock(&allocator->free_list_lock);
	for ( i = 0 ; i < MSTAR_MIU_NUM ; i++ )
	{
		list_splice(&new_free_list_items[i], &allocator->free_list_head[i]);
		atomic_add(page_count[i], &allocator->free_list_size[i]);
	}
	mutex_unlock(&allocator->free_list_lock);
#else
	int page_count = 0;
	int tofree;

	LIST_HEAD(new_free_list_items);

	KBASE_DEBUG_ASSERT(NULL != allocator);

	might_sleep();

	/* Starting by just freeing the overspill.
	* As we do this outside of the lock we might spill too many pages
	* or get too many on the free list, but the max_size is just a ballpark so it is ok
	* providing that tofree doesn't exceed nr_pages
	*/
	tofree = MAX((int)allocator->free_list_max_size - atomic_read(&allocator->free_list_size), 0);
	tofree = nr_pages - MIN(tofree, nr_pages);
	for (; i < tofree; i++) {
		if (likely(0 != pages[i])) {
			struct page *p;

			p = pfn_to_page(PFN_DOWN(pages[i]));
			dma_unmap_page(allocator->kbdev->dev, kbase_dma_addr(p),
				       PAGE_SIZE,
				       DMA_BIDIRECTIONAL);
			ClearPagePrivate(p);
			pages[i] = (phys_addr_t)0;
			__free_page(p);
		}
	}

	for (; i < nr_pages; i++) {
		if (likely(0 != pages[i])) {
			struct page *p;

			p = pfn_to_page(PFN_DOWN(pages[i]));
			pages[i] = (phys_addr_t)0;
			/* Sync back the memory to ensure that future cache
			 * invalidations don't trample on memory.
			 */
			if (sync_back)
				dma_sync_single_for_cpu(allocator->kbdev->dev,
						kbase_dma_addr(p),
						PAGE_SIZE,
						DMA_BIDIRECTIONAL);

			list_add(&p->lru, &new_free_list_items);
			page_count++;
		}
	}
	mutex_lock(&allocator->free_list_lock);
	list_splice(&new_free_list_items, &allocator->free_list_head);
	atomic_add(page_count, &allocator->free_list_size);
	mutex_unlock(&allocator->free_list_lock);
#endif
	/* MSTAR patch end */
}
KBASE_EXPORT_TEST_API(kbase_mem_allocator_free)

