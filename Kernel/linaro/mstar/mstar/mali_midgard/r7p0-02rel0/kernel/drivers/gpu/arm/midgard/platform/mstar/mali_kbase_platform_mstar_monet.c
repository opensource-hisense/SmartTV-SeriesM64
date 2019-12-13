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

#include <mali_kbase.h>
#include "mali_kbase_platform_mstar.h"

#include <linux/delay.h>
#include "mstar/mstar_chip.h"

/* REG_GPU */
#define REG_GPU_BASE            0x110800

#define REG_GPU_SOFT_RESET      0x00
#define MSK_GPU_SOFT_RESET      0x0001

#define REG_GPU_OUTSDNG_READ    0x60
#define MSK_GPU_OUTSDNG_READ    0x0002

#define REG_GPU_RREQ_RD_LEN     0x61
#define MSK_GPU_RREQ_RD_LEN     0x00e0
#define SHIFT_GPU_RREQ_RD_LEN   5
#define VAL_GPU_RREQ_RD_LEN_48  0x0
#define VAL_GPU_RREQ_RD_LEN_56  0x1
#define VAL_GPU_RREQ_RD_LEN_64  0x2
#define VAL_GPU_RREQ_RD_LEN_68  0x3
#define VAL_GPU_RREQ_RD_LEN_72  0x4
#define VAL_GPU_RREQ_RD_LEN_76  0x5
#define VAL_GPU_RREQ_RD_LEN_80  0x6
#define VAL_GPU_RREQ_RD_LEN_84  0x7

#define REG_GPU_RREQ            0x62
#define MSK_GPU_RREQ_THRD       0x000f
#define MSK_GPU_RREQ_MAX        0xff00

#define REG_GPU_WREQ            0x63
#define MSK_GPU_WREQ_THRD       0x000f
#define MSK_GPU_WREQ_MAX        0xff00

#define REG_GPU_RIU_EN          0x6a
#define MSK_GPU_RIU_EN          0x1

#define REG_GPU_MIU2_BASE_LOW   0x6B
#define REG_GPU_MIU2_BASE_HIGH  0x6C

#define REG_GPU_MIU0_BASE_LOW   0x75
#define REG_GPU_MIU0_BASE_HIGH  0x76
#define REG_GPU_MIU1_BASE_LOW   0x77
#define REG_GPU_MIU1_BASE_HIGH  0x78

#define MIU1_BASE_LOW           (((MSTAR_MIU1_BUS_BASE - MSTAR_MIU0_BUS_BASE) >>  0) & 0xffff)
#define MIU1_BASE_HIGH          (((MSTAR_MIU1_BUS_BASE - MSTAR_MIU0_BUS_BASE) >> 16) & 0xffff)
/* XXX: Registers of MIU2 base addr are used for MIU selection in HW RTL code.                                        */
/*      (The reason is that the related HW design is copied fron Monaco which has 3 MIUs)                             */
/*      But HW defaule value is 0x40000000, which make all the address >= 0x40000000 will be treated as MIU2 request. */
/*      So we have to set it to 0xffffffff to prevent HW from issuing MIU2 request.                                   */
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

/* RIU mapping */
#define GPU_REG(addr)           RIU[(REG_GPU_BASE) + ((addr) << 1)]
#define GPU_PLL_REG(addr)       RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]
#define CLKGEN1_REG(addr)       RIU[(REG_CLKGEN1_BASE) + ((addr) << 1)]

/* helper functions */
static void power_on(void)
{
    /* enable GPU (by disabling clock gating) */
    CLKGEN1_REG(REG_CLKGEN1_GPU) &= ~MSK_CLKGEN1_GPU_DISABLE;
    udelay(1);
}

static void power_off(void)
{
    /* disable GPU (by enabling clock gating) */
    CLKGEN1_REG(REG_CLKGEN1_GPU) |= MSK_CLKGEN1_GPU_DISABLE;
    udelay(1);
}

static void init_registers(void)
{
#define FREQ_STEP   24  /* 24 MHz */

    /* GPU clock */
    GPU_PLL_REG(REG_GPU_PLL_CTRL0_HIGH) = 0x0080;
    GPU_PLL_REG(REG_GPU_PLL_CTRL1_LOW)  = 0x1000 | (MALI_MAX_FREQ/FREQ_STEP);   /* default to max freq */
    udelay(10);

    /* read/write request */
    GPU_REG(REG_GPU_OUTSDNG_READ) |= MSK_GPU_OUTSDNG_READ;  /* enable read by outstanding order*/
    GPU_REG(REG_GPU_RREQ) &= ~MSK_GPU_RREQ_THRD;            /* reg_g3d_rreq_thrd = 0x0 */
    GPU_REG(REG_GPU_RREQ) |= MSK_GPU_RREQ_MAX;
    GPU_REG(REG_GPU_WREQ) &= ~MSK_GPU_WREQ_THRD;            /* reg_g3d_wreq_thrd = 0x0 */
    GPU_REG(REG_GPU_WREQ) |= MSK_GPU_WREQ_MAX;

    GPU_REG(REG_GPU_RREQ_RD_LEN) &= ~MSK_GPU_RREQ_RD_LEN;   /* outstanding read length */
    GPU_REG(REG_GPU_RREQ_RD_LEN) |= VAL_GPU_RREQ_RD_LEN_64 << SHIFT_GPU_RREQ_RD_LEN;    /* TODO: use other values to see if performance improves */

    /* MIU base addresses */
    GPU_REG(REG_GPU_MIU1_BASE_LOW)  = MIU1_BASE_LOW;
    GPU_REG(REG_GPU_MIU1_BASE_HIGH) = MIU1_BASE_HIGH;
    GPU_REG(REG_GPU_MIU2_BASE_LOW)  = MIU2_BASE_LOW;
    GPU_REG(REG_GPU_MIU2_BASE_HIGH) = MIU2_BASE_HIGH;

    /* enable RIU access */
#ifdef MSTAR_RIU_ENABLED
    GPU_REG(REG_GPU_RIU_EN) |= MSK_GPU_RIU_EN;
#endif

    /* reset Mali */
    power_on();
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    GPU_REG(REG_GPU_SOFT_RESET) |= MSK_GPU_SOFT_RESET;
    udelay(1); /*delay for run-time suspend*/
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    udelay(1);
    power_off();
}

/* platform functions */
int mstar_platform_init(struct kbase_device *kbdev)
{
    /* platform specific initialization */
    init_registers();

#ifdef CONFIG_MALI_MIDGARD_DVFS
    mstar_platform_dvfs_init(kbdev);
#endif

    return 0;
}

void mstar_platform_term(struct kbase_device *kbdev)
{
    /* platform specific termination */
}

/* PM callbacks */
void mstar_pm_off(struct kbase_device *kbdev)
{
    power_off();
}

int mstar_pm_on(struct kbase_device *kbdev)
{
    power_on();

    /* If the GPU state has been lost then this function must return 1, otherwise it should return 0. */
    return 1; /* TODO: check if the GPU state has been lost */
}

void mstar_pm_suspend(struct kbase_device *kbdev)
{
    power_off();
}

void mstar_pm_resume(struct kbase_device *kbdev)
{
    init_registers();
    power_on();
}
