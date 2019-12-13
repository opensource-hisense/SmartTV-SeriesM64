/*
 *
 * (C) COPYRIGHT 2011-2014 ARM Limited. All rights reserved.
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





#include <linux/atomic.h>
#include <linux/mempool.h>
#include <linux/slab.h>

/* raw page handling */
struct kbase_mem_allocator {
	struct kbase_device *kbdev;
    /* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	atomic_t            free_list_size[MSTAR_MIU_NUM];
#else
	atomic_t            free_list_size;
#endif
    /* MSTAR patch end */
	unsigned int        free_list_max_size;
	struct mutex        free_list_lock;
    /* MSTAR patch begin */
#if MSTAR_CONTROL_MIU && defined(CONFIG_MP_ION_PATCH_MSTAR)
	struct list_head    free_list_head[MSTAR_MIU_NUM];
#else
	struct list_head    free_list_head;
#endif
    /* MSTAR patch end */
	struct shrinker     free_list_reclaimer;
};
