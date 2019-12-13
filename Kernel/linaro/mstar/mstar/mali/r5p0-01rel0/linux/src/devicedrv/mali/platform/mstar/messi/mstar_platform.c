/*
 *
 * (C) COPYRIGHT MStar Semiconductor, Inc. All rights reserved.
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
 * @file mstar_platform.c
 * MStar platform specific driver functions for Messi
 */
#include "mstar/mstar_platform.h"
#include <linux/delay.h>
#if defined(CONFIG_MALI_DVFS)
#include "mali_kernel_common.h"
#endif

/* REG_GPU */
#define REG_GPU_BASE            0x110800

#define REG_GPU_SOFT_RESET      0x00
#define MSK_GPU_SOFT_RESET      0x1
#define REG_GPU_SRAM_SD_EN      0x43
#define SHIFT_GPU_SRAM_SD_EN    3
#define MSK_GPU_SRAM_SD_EN      (0x1 << SHIFT_GPU_SRAM_SD_EN) /*bit 3*/
#define REG_GPU_OUTSDNG_READ    0x60
#define MSK_GPU_OUTSDNG_READ    (0x1 << 1)

#define REG_GPU_RREQ_RD_LEN     0x61
#define SHIFT_GPU_RREQ_RD_LEN   5
#define MSK_GPU_RREQ_RD_LEN     ((0x8 - 1) << SHIFT_GPU_RREQ_RD_LEN) /* bits [7:5] */
#define VAL_GPU_RREQ_RD_LEN_48  0x0
#define VAL_GPU_RREQ_RD_LEN_56  0x1
#define VAL_GPU_RREQ_RD_LEN_64  0x2 /* default */
#define VAL_GPU_RREQ_RD_LEN_68  0x3
#define VAL_GPU_RREQ_RD_LEN_72  0x4
#define VAL_GPU_RREQ_RD_LEN_76  0x5
#define VAL_GPU_RREQ_RD_LEN_80  0x6
#define VAL_GPU_RREQ_RD_LEN_84  0x7

#define REG_GPU_RREQ            0x62
#define MSK_GPU_RREQ_THRD       0xf /* bits [3:0] */

#define REG_GPU_WREQ            0x63
#define MSK_GPU_WREQ_THRD       0xf /* bits [3:0] */

#define REG_GPU_RIU_EN          0x6a
#define MSK_GPU_RIU_EN          0x1

#define REG_GPU_MIU2_BASE_LOW   0x6b
#define REG_GPU_MIU2_BASE_HIGH  0x6c

#define REG_GPU_MIU0_BASE_LOW   0x75
#define REG_GPU_MIU0_BASE_HIGH  0x76
#define REG_GPU_MIU1_BASE_LOW   0x77
#define REG_GPU_MIU1_BASE_HIGH  0x78
#define REG_GPU_MIU2_BASE_LOW   0x6b
#define REG_GPU_MIU2_BASE_HIGH  0x6c

#define MIU1_BASE_LOW           (((MSTAR_MIU1_BUS_BASE-MSTAR_MIU0_BUS_BASE) >>  0) & 0xffff)
#define MIU1_BASE_HIGH          (((MSTAR_MIU1_BUS_BASE-MSTAR_MIU0_BUS_BASE) >> 16) & 0xffff)
#define MIU2_BASE_LOW           0xffff
#define MIU2_BASE_HIGH          0xffff

/* REG_GPU_PLL */
#define REG_GPU_PLL_BASE        0x160f00

#define REG_GPU_PLL_CTRL0_LOW   0x02
#define REG_GPU_PLL_CTRL0_HIGH  0x03
#define REG_GPU_PLL_CTRL1_LOW   0x04
#define REG_GPU_PLL_CTRL1_HIGH  0x05

/* REG_CLKGEN1 */
#define REG_CLKGEN1_BASE        0x103300

#define REG_CLKGEN1_GPU         0x20
#define MSK_CLKGEN1_GPU_DISABLE 0x0001

#if defined(CONFIG_MALI_DVFS)
#define SHFIT_GPU_SRC_SELECT  0x0008
#endif


/* RIU */
#ifdef CONFIG_ARM64
#define RIU_MAP                 mstar_pm_base
#else
#define RIU_MAP                 0xfd000000
#endif
#define RIU                     ((volatile unsigned short*)(RIU_MAP))
#define GPU_REG(addr)           RIU[(REG_GPU_BASE) + ((addr) << 1)]
#define GPU_PLL_REG(addr)       RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]
#define CLKGEN1_REG(addr)       RIU[(REG_CLKGEN1_BASE) + ((addr) << 1)]

/* GPU clock */
extern int mali_gpu_clock;

#define REG_GPU_DFS_CTRL            0x48
#define MSK_GPU_DFS_CLK_DIV         0x1f
#define REG_GPU_DFS_CLK_UPDATE      0x49
#define VAL_GPU_DFS_DISABLE         0x0
#define GPU_DFS_CLK_NUM_SCALE       0x20
#define VAL_GPU_DFS_CLK_MAX_SCALE   (GPU_DFS_CLK_NUM_SCALE - 1)
#define SHIFT_GPU_DFS_EN            5
#define VAL_GPU_DFS_ENABLE          ( 0x1 << SHIFT_GPU_DFS_EN )
#define VAL_GPU_DFS_CLK_UPDATE      0x1

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

void mstar_platform_power_off(void)
{
    /* power down SRAM */
    GPU_REG(REG_GPU_SRAM_SD_EN) |= MSK_GPU_SRAM_SD_EN;
    udelay(10);
    /* disable GPU (by enabling clock gating) */
    CLKGEN1_REG(REG_CLKGEN1_GPU) |= MSK_CLKGEN1_GPU_DISABLE;
    udelay(1);
}

/* power on the GPU, the power on sequence is implemented according to bring up template
 * gpu_bringup_messi_20150717.xslx, thus legacy comments based on hardware pattern
 * are deleted. */
void mstar_platform_power_on(void)
{
    /* GPU clock */
    GPU_PLL_REG(REG_GPU_PLL_CTRL1_LOW)  = 0x1000 | (GPU_CLOCK/GPU_CLOCK_STEP);   /* default to max freq */
    GPU_PLL_REG(REG_GPU_PLL_CTRL0_HIGH) = 0x0080; /* update settings */
    udelay(10);

    /* read/write request */
    GPU_REG(REG_GPU_OUTSDNG_READ) |= MSK_GPU_OUTSDNG_READ;  /* enable read by outstanding order*/
    GPU_REG(REG_GPU_RREQ) &= ~MSK_GPU_RREQ_THRD;            /* reg_g3d_rreq_thrd = 0x0 */
    /* GPU_REG(REG_GPU_RREQ) |= MSK_GPU_RREQ_MAX; */        /* there's no instruction to do this*/
    GPU_REG(REG_GPU_WREQ) &= ~MSK_GPU_WREQ_THRD;            /* reg_g3d_wreq_thrd = 0x0 */
    /* GPU_REG(REG_GPU_WREQ) |= MSK_GPU_WREQ_MAX; */

    /* MIU base addresses
     * MIU0 default to 0x0000_0000
     * MIU1 0x8000_0000
     * there are actually no MIU2, but we have to fill 0xffff_ffff in
     * reg_miu2_base to avoid an existing hw bug.
     */
    GPU_REG(REG_GPU_MIU1_BASE_LOW)  = MIU1_BASE_LOW;
    GPU_REG(REG_GPU_MIU1_BASE_HIGH) = MIU1_BASE_HIGH;
    GPU_REG(REG_GPU_MIU2_BASE_LOW)  = MIU2_BASE_LOW;
    GPU_REG(REG_GPU_MIU2_BASE_HIGH) = MIU2_BASE_HIGH;

#ifdef MSTAR_RIU_ENABLED
    GPU_REG(REG_GPU_RIU_EN) |= MSK_GPU_RIU_EN;
#endif

    /* set read quest length to 64 */
    GPU_REG(REG_GPU_RREQ_RD_LEN) = VAL_GPU_RREQ_RD_LEN_64;

    /* enable GPU clock */
    CLKGEN1_REG(REG_CLKGEN1_GPU) &= ~MSK_CLKGEN1_GPU_DISABLE;
    udelay(10);

    /* reset Mali */
    /* disable MALI GPU soft reset */
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    /* enable MALI GPU soft reset */
    GPU_REG(REG_GPU_SOFT_RESET) |= MSK_GPU_SOFT_RESET;
    udelay(10);

    /* disable MALI GPU soft reset */
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    udelay(1);

}

void mstar_platform_suspend(void)
{
    mstar_platform_power_off();    /* just power off */
}

void mstar_platform_resume(void)
{
    mstar_platform_power_on();     /* just power on */
}

void mstar_platform_runtime_suspend(void)
{
    mstar_platform_power_off();
}

void mstar_platform_runtime_resume(void)
{
    /* disable GPU clock gating */
    CLKGEN1_REG(REG_CLKGEN1_GPU) &= ~MSK_CLKGEN1_GPU_DISABLE;
    udelay(10);

    /* power up SRAM */
    GPU_REG(REG_GPU_SRAM_SD_EN) &= ~MSK_GPU_SRAM_SD_EN;
    udelay(10);
}

#if defined(CONFIG_MALI_DVFS)
static bool mstar_platform_adjust_gpu_clock(void)
{
    u16 freq_scale;

    MALI_DEBUG_PRINT(4, ("mstar_platform_adjust_gpu_clock() called, mali_gpu_clock = %d\n", mali_gpu_clock));

    if ((mali_gpu_clock < gpu_min_clock) || (gpu_max_clock < mali_gpu_clock))
    {
        return false;
    }

    freq_scale = GPU_CLOCK_ALIGN(mali_gpu_clock) / GPU_CLOCK_STEP;

    /* enable DFS */
    GPU_REG(REG_GPU_DFS_CTRL) = VAL_GPU_DFS_ENABLE | VAL_GPU_DFS_CLK_MAX_SCALE ;
    GPU_REG(REG_GPU_DFS_CLK_UPDATE) = VAL_GPU_DFS_CLK_UPDATE;

    /* adjust clock */
    GPU_REG(REG_GPU_DFS_CTRL) = (GPU_REG(REG_GPU_DFS_CTRL) & ~MSK_GPU_DFS_CLK_DIV) | freq_scale ;

    /* Update GPU clock */
    GPU_REG(REG_GPU_DFS_CLK_UPDATE) = VAL_GPU_DFS_CLK_UPDATE;

    /* disable DFS */
    GPU_REG(REG_GPU_DFS_CTRL) = VAL_GPU_DFS_DISABLE; 
    GPU_REG(REG_GPU_DFS_CLK_UPDATE) = VAL_GPU_DFS_CLK_UPDATE;

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
