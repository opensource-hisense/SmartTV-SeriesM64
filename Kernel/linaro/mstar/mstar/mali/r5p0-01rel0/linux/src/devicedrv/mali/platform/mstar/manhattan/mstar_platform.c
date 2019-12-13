/*
 * Copyright (C) 2010-2012 MStar Semiconductor, Inc. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mstar_platform.c
 * MStar platform specific driver functions for Manhattan
 */
#include "mstar/mstar_platform.h"
#include <linux/delay.h>
#if defined(CONFIG_MALI_DVFS)
#include "mali_kernel_common.h"
#endif

#define REG_CLKGEN1_BASE        0x103300
#define REG_CKG_GPU             0x20
#define DISABLE_CLK_GPU         0x0001
#if defined(CONFIG_MALI_DVFS)
#define SHFIT_GPU_SRC_SELECT  0x0008
#endif

#define REG_G3D_BASE            0x110800
#define REG_SOFT_RESET          0x00
#define BITMASK_SOFT_RESET      0x0001
#define REG_GPU_SRAM_SD_EN      0x43
#define BITMASK_GPU_SRAM_SD_EN  0x0080
#define REG_GPUPLL_CTRL0_LOW    0x44
#define REG_GPUPLL_CTRL0_HIGH   0x45
#define REG_GPUPLL_CTRL1_LOW    0x46
#define REG_GPUPLL_CTRL1_HIGH   0x47
#define BITMASK_GPU_CLOCK       0x00ff
#define REG_SPLIT_2CH_MD        0x60
#define READ_OUTSTANDING        0x0002
#define REG_FIFO_DEPTH          0x61
#define SHIFT_FIFO_DEPTH        0x5
#define MASK_FIFO_DEPTH         (0x7 << SHIFT_FIFO_DEPTH)
#define REG_G3D_RREQ            0x62
#define REG_G3D_RREQ_THRD       0x000f
#define REG_G3D_WREQ            0x63
#define REG_G3D_WREQ_THRD       0x000f
#define REG_RIU_APB_EN          0x6a
#define BITMASK_RIU_APB_EN      0x0001
#define REG_MIU0_START_ADDR     0x75
#define REG_MIU0_END_ADDR       0x76
#define REG_MIU1_START_ADDR     0x77
#define REG_MIU1_END_ADDR       0x78
#define MIU0_PHY_BASE_ADDR_HIGH 0x0000
#define MIU0_PHY_END_ADDR_HIGH  MIU1_PHY_BASE_ADDR_HIGH
#define MIU1_PHY_END_ADDR_HIGH  0x0fff

/* RIU */
#ifdef MSTAR_MAPPING_32
#define RIU_MAP               0xfd000000
#else
#define RIU_MAP               mstar_pm_base
#endif
#define RIU                   ((volatile unsigned short*)RIU_MAP)
#define CLKGEN1_REG(addr)     RIU[REG_CLKGEN1_BASE + ((addr) << 1)]
#define G3D_REG(addr)         RIU[REG_G3D_BASE + ((addr) << 1)]

/* GPU clock */
extern int mali_gpu_clock;
extern ptrdiff_t mstar_pm_base;

#define GPU_MIN_CLOCK GPU_CLOCK/2
#define GPU_CLOCK_STEP 24
#define GPU_CLOCK_ALIGN(clock) ((clock)/GPU_CLOCK_STEP*GPU_CLOCK_STEP)
static int gpu_min_clock = GPU_CLOCK;
static int gpu_max_clock = GPU_CLOCK;

#if defined(CONFIG_MALI_DVFS)
static struct mali_gpu_clock mali_clock_items;

static void mali_dvfs_init(int min_clock, int max_clock)
{
    u32 size;

    mali_clock_items.num_of_steps = (max_clock - min_clock) / GPU_CLOCK_STEP + 1;

    size = mali_clock_items.num_of_steps * sizeof(*mali_clock_items.item);
    mali_clock_items.item = _mali_osk_valloc(size);
    if (mali_clock_items.item == NULL)
    {
        mali_clock_items.num_of_steps = 0;
    }
    else
    {
        int i = 0;
        int gpu_clock = min_clock;

        _mali_osk_memset(mali_clock_items.item, 0, size);

        for (i = 0; i < mali_clock_items.num_of_steps; ++i)
        {
            mali_clock_items.item[i].clock = gpu_clock; /* MHz */
            /*
             * TODO: Check voltage for each step
             */
            mali_clock_items.item[i].vol = 0; /* voltage */
            gpu_clock += GPU_CLOCK_STEP;
        }
    }
}

static void mali_dvfs_deinit(void)
{
    if (mali_clock_items.item)
    {
        _mali_osk_vfree(mali_clock_items.item);

        mali_clock_items.item = NULL;
        mali_clock_items.num_of_steps = 0;
    }
}
#endif

static void get_gpu_clock(int* min_clock, int* max_clock)
{
    if (min_clock)
    {
        *min_clock = GPU_CLOCK_ALIGN(GPU_MIN_CLOCK);
    }

    if (max_clock)
    {
        int gpu_clock = GPU_CLOCK;

        *max_clock = GPU_CLOCK_ALIGN((gpu_clock < GPU_MIN_CLOCK) ? GPU_MIN_CLOCK : gpu_clock);
    }
}

/* platform functions */
void mstar_platform_init(void)
{
    get_gpu_clock(&gpu_min_clock, &gpu_max_clock);
#if defined(CONFIG_MALI_DVFS)
    mali_dvfs_init(gpu_min_clock, gpu_max_clock);
#endif
    mali_gpu_clock = gpu_max_clock;
    mstar_platform_power_on();
}

void mstar_platform_deinit(void)
{
#if defined(CONFIG_MALI_DVFS)
    mali_dvfs_deinit();
#endif
    mstar_platform_power_off();
}

static void update_fifo_depth(void)
{
    int fifo_depth;

#if defined(CONFIG_MALI_DVFS)
    /* set read request length if necessary */
    if (gpu_max_clock == mali_gpu_clock)
    {
        /* set read request length to 48 for performance improvement only when GPU clock is 396 MHz */
        fifo_depth = 0x2;
    }
    else
    {
        /* set read request length to 32 */
        fifo_depth = 0x0;
    }
#else
    /* For improve performance, set FIFO depth to 48                   */
    fifo_depth = 0x2;
#endif

    /* Actually, inside the hardware design, the FIFO depth is only 32 */
    G3D_REG(REG_FIFO_DEPTH) = (G3D_REG(REG_FIFO_DEPTH) & ~MASK_FIFO_DEPTH)
        | ((fifo_depth << SHIFT_FIFO_DEPTH) & MASK_FIFO_DEPTH);
    udelay(100);
}

void mstar_platform_power_on(void)
{
    /* set GPU clock: must before power on */
    G3D_REG(REG_GPUPLL_CTRL0_LOW)  = 0x0000;
    G3D_REG(REG_GPUPLL_CTRL0_HIGH) = 0x0080;
    G3D_REG(REG_GPUPLL_CTRL1_LOW)  = (G3D_REG(REG_GPUPLL_CTRL1_LOW) & ~BITMASK_GPU_CLOCK) | (mali_gpu_clock/ GPU_CLOCK_STEP);
    G3D_REG(REG_GPUPLL_CTRL1_HIGH) = 0x0001;
    udelay(100);

    /* enable read by outstanding order*/
    G3D_REG(REG_SPLIT_2CH_MD) |= READ_OUTSTANDING;

    /* reg_g3d_rreq_thrd = 0x0 */
    G3D_REG(REG_G3D_RREQ) &= ~REG_G3D_RREQ_THRD;

    /* reg_g3d_wreq_thrd = 0x0 */
    G3D_REG(REG_G3D_WREQ) &= ~REG_G3D_WREQ_THRD;

    /* Set MIU0 base and end address */
    G3D_REG(REG_MIU0_START_ADDR) = MIU0_PHY_BASE_ADDR_HIGH;
    G3D_REG(REG_MIU0_END_ADDR) = MIU0_PHY_END_ADDR_HIGH;
    udelay(100);

    /* Set MIU1 base and end address */
    G3D_REG(REG_MIU1_START_ADDR) = MIU1_PHY_BASE_ADDR_HIGH;
    G3D_REG(REG_MIU1_END_ADDR) = MIU1_PHY_END_ADDR_HIGH;
    udelay(100);

    /* enable RIU access */
#ifdef MSTAR_RIU_ENABLED
    G3D_REG(REG_RIU_APB_EN) |= BITMASK_RIU_APB_EN;
    udelay(100);
#endif

    update_fifo_depth();
    udelay(100);

    /* disable GPU clock gating */
    CLKGEN1_REG(REG_CKG_GPU) &= ~DISABLE_CLK_GPU;
    udelay(100);

    /* power up SRAM */
    G3D_REG(REG_GPU_SRAM_SD_EN) &= ~BITMASK_GPU_SRAM_SD_EN;
    udelay(100);

    /* reset mali */
    G3D_REG(REG_SOFT_RESET) &= ~BITMASK_SOFT_RESET;
    G3D_REG(REG_SOFT_RESET) |= BITMASK_SOFT_RESET;
    udelay(100); /*delay for run-time suspend*/
    G3D_REG(REG_SOFT_RESET) &= ~BITMASK_SOFT_RESET;
    udelay(100);
}

void mstar_platform_power_off(void)
{
    /* power down SRAM */
    G3D_REG(REG_GPU_SRAM_SD_EN) |= BITMASK_GPU_SRAM_SD_EN;
    udelay(100);

    /* enable GPU clock gating */
    CLKGEN1_REG(REG_CKG_GPU) |= DISABLE_CLK_GPU;
    udelay(100);
}

void mstar_platform_suspend(void)
{
    mstar_platform_power_off(); /* just power off */
}

void mstar_platform_resume(void)
{
    mstar_platform_power_on();  /* just power on */
}

void mstar_platform_runtime_suspend(void)
{
    mstar_platform_power_off();
}

void mstar_platform_runtime_resume(void)
{
    /* disable GPU clock gating */
    CLKGEN1_REG(REG_CKG_GPU) &= ~DISABLE_CLK_GPU;
    udelay(100);

    /* power up SRAM */
    G3D_REG(REG_GPU_SRAM_SD_EN) &= ~BITMASK_GPU_SRAM_SD_EN;
    udelay(100);
}

#if defined(CONFIG_MALI_DVFS)
static bool mstar_platform_adjust_gpu_clock(void)
{
    MALI_DEBUG_PRINT(4, ("mstar_platform_adjust_gpu_clock() called, mali_gpu_clock = %d\n", mali_gpu_clock));

    if ((mali_gpu_clock < gpu_min_clock) || (gpu_max_clock < mali_gpu_clock))
    {
        return false;
    }

    /* set PLL source to MPLL */
    CLKGEN1_REG(REG_CKG_GPU) |= SHFIT_GPU_SRC_SELECT;

    update_fifo_depth();

    /* set GPU clock */
    G3D_REG(REG_GPUPLL_CTRL1_LOW) = mali_gpu_clock / GPU_CLOCK_STEP;
    /* set delay time 5us to make G3DPLL stable */
    udelay(5);

    /* set PLL source back to G3D PLL */
    CLKGEN1_REG(REG_CKG_GPU) &= ~(SHFIT_GPU_SRC_SELECT);

    return true;
}

void mstar_platform_report_gpu_clock_info(struct mali_gpu_clock **data)
{
    MALI_DEBUG_PRINT(4, ("mstar_platform_report_gpu_clock_info() called\n"));

    *data = &mali_clock_items;
}

int mstar_platform_get_gpu_clock_step(void)
{
    int step = 0;
    int clock = 0;
    int num_of_steps = mali_clock_items.num_of_steps;

    MALI_DEBUG_PRINT(4, ("mstar_platform_get_gpu_clock_step() called\n"));

    for (step = 0; step < num_of_steps; step++)
    {
        clock = mali_clock_items.item[step].clock;
        if ( mali_gpu_clock == clock )
        {
            break;
        }
    }

    return step;
}

/*
 * ToDo: check mali_gpu_linux_ddk_integration_guide.pdf
 * The set_freq function is protected by a spinlock and cannot sleep.  
 * Use a work queue to set the required frequency.
 * The function parameter is the frequency step, not the real clock value.
 */
bool mstar_platform_set_gpu_clock_step(int step)
{
    unsigned int gpu_clock_bak;

    MALI_DEBUG_PRINT(4, ("mstar_platform_set_gpu_clock_step() called, step = %d\n", step));

    if (step >= mali_clock_items.num_of_steps)
    {
        return false;
    }

    gpu_clock_bak = mali_gpu_clock;
    mali_gpu_clock = mali_clock_items.item[step].clock;
    if ( mstar_platform_adjust_gpu_clock() == false )
    {
        mali_gpu_clock = gpu_clock_bak;
        return false;
    }

    return true;
}
#endif
