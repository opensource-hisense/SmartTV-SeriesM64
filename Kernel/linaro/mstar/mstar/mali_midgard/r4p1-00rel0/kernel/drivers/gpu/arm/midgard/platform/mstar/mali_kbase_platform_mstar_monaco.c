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

#include <mali_kbase.h>
#include "mali_kbase_platform_mstar.h"

#include <linux/delay.h>
#include "mstar/mstar_chip.h"

/* REG_GPU */
#define REG_GPU_BASE            0x110800

#define REG_GPU_SOFT_RESET      0x00
#define MSK_GPU_SOFT_RESET      0x0001

#define REG_GPU_SRAM            0x43
#define REG_GPU_SRAM_SD_EN      0x0008

#define REG_GPU_PREFETCH        0x5b
#define MSK_GPU_PREFETCH_EN     0x0001
#define MSK_GPU_PREFETCH_BYPASS 0x0002

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
#define MSK_GPU_RREQ_THRD_HIGH  0xff00

#define REG_GPU_WREQ            0x63
#define MSK_GPU_WREQ_THRD       0x000f
#define MSK_GPU_WREQ_THRD_HIGH  0xff00

#define REG_GPU_RIU_EN          0x6a
#define MSK_GPU_RIU_EN          0x1

#define REG_GPU_MIU0_BASE_LOW   0x75
#define REG_GPU_MIU0_BASE_HIGH  0x76
#define REG_GPU_MIU1_BASE_LOW   0x77
#define REG_GPU_MIU1_BASE_HIGH  0x78
#define REG_GPU_MIU2_BASE_LOW   0x6b
#define REG_GPU_MIU2_BASE_HIGH  0x6c

#define MIU0_BASE               (MSTAR_MIU0_BUS_BASE-MSTAR_MIU0_BUS_BASE)
#define MIU1_BASE               (MSTAR_MIU1_BUS_BASE-MSTAR_MIU0_BUS_BASE)
#define MIU2_BASE               (MSTAR_MIU2_BUS_BASE-MSTAR_MIU0_BUS_BASE)

#define MIU0_BASE_LOW           (((MIU0_BASE) >>  0) & 0xffff)
#define MIU0_BASE_HIGH          (((MIU0_BASE) >> 16) & 0xffff)
#define MIU1_BASE_LOW           (((MIU1_BASE) >>  0) & 0xffff)
#define MIU1_BASE_HIGH          (((MIU1_BASE) >> 16) & 0xffff)
#define MIU2_BASE_LOW           (((MIU2_BASE) >>  0) & 0xffff)
#define MIU2_BASE_HIGH          (((MIU2_BASE) >> 16) & 0xffff)

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

/* REG_PM_TOP */
#define REG_PM_TOP_BASE         0x001E00

#define REG_PM_TOP_CHIP_REV     0x01
#define MSK_PM_TOP_CHIP_REV     0xff00
#define SHIFT_PM_TOP_CHIP_REV   8
#define VAL_PM_TOP_CHIP_REV_U01 0
#define VAL_PM_TOP_CHIP_REV_U02 1
#define VAL_PM_TOP_CHIP_REV_U03 2

/* RIU */
#define RIU_MAP                 0xfd000000
#define RIU                     ((volatile unsigned short*)(RIU_MAP))
#define GPU_REG(addr)           RIU[(REG_GPU_BASE) + ((addr) << 1)]
#define GPU_PLL_REG(addr)       RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]
#define CLKGEN1_REG(addr)       RIU[(REG_CLKGEN1_BASE) + ((addr) << 1)]
#define PM_TOP_REG(addr)        RIU[(REG_PM_TOP_BASE) + ((addr) << 1)]

/* */
mali_bool mstar_platform_init(struct kbase_device *kbdev)
{
    mstar_platform_power_on(kbdev);
    return MALI_TRUE;
}
void mstar_platform_term(struct kbase_device *kbdev)
{
    mstar_platform_power_off(kbdev);
}

void mstar_platform_power_off(struct kbase_device *kbdev)
{
    GPU_REG(REG_GPU_SRAM) &= ~REG_GPU_SRAM_SD_EN;
    udelay(1);
    
    /* disable GPU (by enabling clock gating) */
    CLKGEN1_REG(REG_CLKGEN1_GPU) |= MSK_CLKGEN1_GPU_DISABLE;
    udelay(1);
}

int mstar_platform_power_on(struct kbase_device *kbdev)
{
#define FREQ_STEP   24  /* 24 MHz */

    u32 chip_rev;

    /* GPU clock
     *
     *  `RIU_W( (`RIUBASE_GPU_PLL>>1)+ 7'h2, 2'b11, 16'h0000);
     *  `RIU_W( (`RIUBASE_GPU_PLL>>1)+ 7'h3, 2'b11, 16'h0080);  // LOOPDIV_FIRST = 1         504/24 = 21 = 'h15
     *  `RIU_W( (`RIUBASE_GPU_PLL>>1)+ 7'h4, 2'b11, 16'h1015);  // 504MHz, each step = 24MHz
     *  `RIU_W( (`RIUBASE_GPU_PLL>>1)+ 7'h5, 2'b11, 16'h0001);
     */
    GPU_PLL_REG(REG_GPU_PLL_CTRL0_LOW)  = 0x0000;
    GPU_PLL_REG(REG_GPU_PLL_CTRL0_HIGH) = 0x0080;
    GPU_PLL_REG(REG_GPU_PLL_CTRL1_LOW)  = 0x1000 | (MALI_MAX_FREQ/FREQ_STEP);   /* default to max freq */
    GPU_PLL_REG(REG_GPU_PLL_CTRL1_HIGH) = 0x0001;
    udelay(10);

    /* prefetch */
    /* TODO: enable prefetch (disabled for bring-up first) */
    GPU_REG(REG_GPU_PREFETCH) &= ~MSK_GPU_PREFETCH_EN;
    GPU_REG(REG_GPU_PREFETCH) |= MSK_GPU_PREFETCH_BYPASS;

    /* read/write request */
    GPU_REG(REG_GPU_OUTSDNG_READ) |= MSK_GPU_OUTSDNG_READ;  /* enable read by outstanding order*/
    GPU_REG(REG_GPU_RREQ) &= ~MSK_GPU_RREQ_THRD;            /* reg_g3d_rreq_thrd = 0x0 */
    GPU_REG(REG_GPU_RREQ) |= MSK_GPU_RREQ_THRD_HIGH;
    GPU_REG(REG_GPU_WREQ) &= ~MSK_GPU_WREQ_THRD;            /* reg_g3d_wreq_thrd = 0x0 */
    GPU_REG(REG_GPU_WREQ) |= MSK_GPU_WREQ_THRD_HIGH;

    GPU_REG(REG_GPU_RREQ_RD_LEN) &= ~MSK_GPU_RREQ_RD_LEN;   /* outstanding read length */
    GPU_REG(REG_GPU_RREQ_RD_LEN) |= VAL_GPU_RREQ_RD_LEN_64 << SHIFT_GPU_RREQ_RD_LEN;    /* TODO: use other values to see if performance improves */

    /* MIU base addresses */
    GPU_REG(REG_GPU_MIU0_BASE_LOW)  = MIU0_BASE_LOW;
    GPU_REG(REG_GPU_MIU0_BASE_HIGH) = MIU0_BASE_HIGH;
    GPU_REG(REG_GPU_MIU1_BASE_LOW)  = MIU1_BASE_LOW;
    GPU_REG(REG_GPU_MIU1_BASE_HIGH) = MIU1_BASE_HIGH;
    GPU_REG(REG_GPU_MIU2_BASE_LOW)  = MIU2_BASE_LOW;
    GPU_REG(REG_GPU_MIU2_BASE_HIGH) = MIU2_BASE_HIGH;

    /* enable RIU access
     *
     *  `RIU_W( (`RIUBASE_GPU>>1)+ 7'h6a, 2'b01, 16'h0001);
     */
#ifdef MSTAR_RIU_ENABLED
    GPU_REG(REG_GPU_RIU_EN) |= MSK_GPU_RIU_EN;
#endif

    GPU_REG(REG_GPU_SRAM) |= REG_GPU_SRAM_SD_EN;
    udelay(1);

    /* enable GPU (by disabling clock gating)
     *
     *  `RIU_W( (`RIUBASE_CLKGEN1>>1)+ 7'h20, 2'b11, 16'h0000);
     */
    CLKGEN1_REG(REG_CLKGEN1_GPU) &= ~MSK_CLKGEN1_GPU_DISABLE;
    udelay(1);

    /* reset Mali */
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    GPU_REG(REG_GPU_SOFT_RESET) |= MSK_GPU_SOFT_RESET;
    udelay(1); /*delay for run-time suspend*/
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    udelay(1);

    /* get chip revision */
    chip_rev = (PM_TOP_REG(REG_PM_TOP_CHIP_REV) & MSK_PM_TOP_CHIP_REV) >> SHIFT_PM_TOP_CHIP_REV;

    if (VAL_PM_TOP_CHIP_REV_U03 <= chip_rev)
    {
        kbdev->gpu_props.props.core_props.is_monaco_u03 = 1;
    }
    else
    {
        kbdev->gpu_props.props.core_props.is_monaco_u03 = 0;
    }

    return 1;
}

void mstar_platform_power_suspend(struct kbase_device *kbdev)
{
    mstar_platform_power_off(kbdev);    /* just power off */
}

void mstar_platform_power_resume(struct kbase_device *kbdev)
{
    mstar_platform_power_on(kbdev);     /* just power on */
}
