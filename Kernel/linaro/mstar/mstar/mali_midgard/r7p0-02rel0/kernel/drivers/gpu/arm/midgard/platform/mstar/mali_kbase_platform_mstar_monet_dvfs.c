/*
 *
 * (C) COPYRIGHT 2014-2015 MStar Semiconductor, Inc. All rights reserved.
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

#ifdef CONFIG_MALI_MIDGARD_DVFS

#include <mali_kbase.h>
#include "mali_kbase_platform_mstar.h"

void mstar_platform_dvfs_init(struct kbase_device* kbdev)
{
    /* DVFS not supported in Monet */
    kbdev->dvfs.enable = NULL;
    kbdev->dvfs.disable = NULL;
    kbdev->dvfs.clock_up = NULL;
    kbdev->dvfs.clock_down = NULL;
}

#endif /* CONFIG_MALI_MIDGARD_DVFS */
