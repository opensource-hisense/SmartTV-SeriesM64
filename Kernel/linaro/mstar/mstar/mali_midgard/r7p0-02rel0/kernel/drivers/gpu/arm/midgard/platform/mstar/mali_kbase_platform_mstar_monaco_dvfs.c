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

/* REG_GPU */
#define REG_GPU_BASE            0x110800

#define REG_GPU_DFS_CTRL        0x48
#define VAL_GPU_DFS_DISABLE     0x0
#define MSK_GPU_DFS_CLK_DIV     0x1f
#define SHIFT_GPU_DFS_EN        5
#define VAL_GPU_DFS_ENABLE      (0x1 << SHIFT_GPU_DFS_EN)

#define REG_GPU_DFS_CLK_UPDATE  0x49
#define VAL_GPU_DFS_CLK_UPDATE  0x1

/* RIU mapping */
#define GPU_REG(addr)           RIU[(REG_GPU_BASE) + ((addr) << 1)]

/* frequency scales */
#define GPU_DFS_NUM_CLK_SCALES  0x20
#define GPU_DFS_CLK_STEP        ((MALI_MAX_FREQ) / (GPU_DFS_NUM_CLK_SCALES))
#define GPU_DFS_MAX_CLK_SCALE   (GPU_DFS_NUM_CLK_SCALES - 1)
#define GPU_DFS_MIN_CLK_SCALE   ((((MALI_MIN_FREQ) + (GPU_DFS_CLK_STEP) - 1) / (GPU_DFS_CLK_STEP)) - 1)
#define GPU_DFS_CLK_SCALE_STEP  0x1

/* DVFS callbacks */
static bool dvfs_enable(struct kbase_device* kbdev)
{
    if (!kbdev)
    {
        return false;
    }

    if (MALI_MIN_FREQ == MALI_MAX_FREQ)
    {
        printk(KERN_ERR "[%s] MALI_MIN_FREQ == MALI_MAX_FREQ, DVFS is disabled\n", __func__);
        return false;
    }

    if (MALI_MIN_FREQ == 0)
    {
        printk(KERN_ERR "[%s] MALI_MIN_FREQ == 0, DVFS is disabled\n", __func__);
        return false;
    }

    /* enable DVFS */
    kbdev->dvfs.current_scale = GPU_DFS_MAX_CLK_SCALE;
    kbdev->dvfs.enabled = 1;
    GPU_REG(REG_GPU_DFS_CTRL) = VAL_GPU_DFS_ENABLE | (kbdev->dvfs.current_scale & MSK_GPU_DFS_CLK_DIV);
    GPU_REG(REG_GPU_DFS_CLK_UPDATE) = VAL_GPU_DFS_CLK_UPDATE;

    printk(KERN_ERR "[%s] DVFS is enabled\n", __func__);

    return true;
}

static bool dvfs_disable(struct kbase_device* kbdev)
{
    if (!kbdev)
    {
        return false;
    }

    /* disable DVFS */
    kbdev->dvfs.current_scale = GPU_DFS_MAX_CLK_SCALE;
    kbdev->dvfs.enabled = 0;
    GPU_REG(REG_GPU_DFS_CTRL) = VAL_GPU_DFS_DISABLE;
    GPU_REG(REG_GPU_DFS_CLK_UPDATE) = VAL_GPU_DFS_CLK_UPDATE;

    printk(KERN_ERR "[%s] DVFS is disabled\n", __func__);

    return true;
}

static bool dvfs_clock_up(struct kbase_device* kbdev)
{
    u32 current_scale = kbdev->dvfs.current_scale;
    u32 new_scale;

    if (!kbdev)
    {
        return false;
    }

    if (current_scale >= GPU_DFS_MAX_CLK_SCALE)
    {
       return false;
    }

    new_scale = current_scale + GPU_DFS_CLK_SCALE_STEP;

    if (new_scale > GPU_DFS_MAX_CLK_SCALE)
    {
        new_scale = GPU_DFS_MAX_CLK_SCALE;
    }

    /* clock up */
    kbdev->dvfs.current_scale = new_scale;
    GPU_REG(REG_GPU_DFS_CTRL) = (GPU_REG(REG_GPU_DFS_CTRL) & ~MSK_GPU_DFS_CLK_DIV) | (new_scale & MSK_GPU_DFS_CLK_DIV);
    GPU_REG(REG_GPU_DFS_CLK_UPDATE) = VAL_GPU_DFS_CLK_UPDATE;

    printk(KERN_ERR "[%s] GPU frequency up: %u -> %u MHz\n",
           __func__,
           MALI_MAX_FREQ / GPU_DFS_NUM_CLK_SCALES * (current_scale + 1),
           ((MALI_MAX_FREQ / GPU_DFS_NUM_CLK_SCALES ) * (new_scale + 1 )));

    return true;
}

static bool dvfs_clock_down(struct kbase_device* kbdev)
{
    u32 current_scale = kbdev->dvfs.current_scale;
    s32 new_scale;

    if (!kbdev)
    {
        return false;
    }

    if (current_scale <= GPU_DFS_MIN_CLK_SCALE)
    {
        return false;
    }

    new_scale = (s32)current_scale - GPU_DFS_CLK_SCALE_STEP;

    if (new_scale < GPU_DFS_MIN_CLK_SCALE)
    {
        new_scale = GPU_DFS_MIN_CLK_SCALE;
    }

    /* clock down */
    kbdev->dvfs.current_scale = (u32)new_scale
    GPU_REG(REG_GPU_DFS_CTRL) = (GPU_REG(REG_GPU_DFS_CTRL) & ~MSK_GPU_DFS_CLK_DIV) | (new_scale & MSK_GPU_DFS_CLK_DIV);
    GPU_REG(REG_GPU_DFS_CLK_UPDATE) = VAL_GPU_DFS_CLK_UPDATE;

    printk(KERN_ERR "[%s] GPU frequency down: %u -> %u MHz\n",
           __func__,
           MALI_MAX_FREQ / GPU_DFS_NUM_CLK_SCALES * (current_scale + 1),
           ((MALI_MAX_FREQ / GPU_DFS_NUM_CLK_SCALES ) * (new_scale + 1)));

    return true;
}

/* */
void mstar_platform_dvfs_init(struct kbase_device* kbdev)
{
    kbdev->dvfs.enable = dvfs_enable;
    kbdev->dvfs.disable = dvfs_disable;
    kbdev->dvfs.clock_up = dvfs_clock_up;
    kbdev->dvfs.clock_down = dvfs_clock_down;

    dvfs_enable(kbdev);
}

#endif /* CONFIG_MALI_MIDGARD_DVFS */
