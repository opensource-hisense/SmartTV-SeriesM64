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
#define REG_GPU_BASE                0x110800

#define REG_GPU_SOFT_RESET          0x00
#define MSK_GPU_SOFT_RESET          0x0001

#define REG_GPU_SRAM_SD_EN          0x43
#define MSK_GPU_SRAM_SD_EN          0x0008

#define REG_GPU_RREQ_RD_LEN         0x61
#define MSK_GPU_RREQ_RD_LEN         0x00e0
#define SHIFT_GPU_RREQ_RD_LEN       5
#define VAL_GPU_RREQ_RD_LEN_48      0x0
#define VAL_GPU_RREQ_RD_LEN_56      0x1
#define VAL_GPU_RREQ_RD_LEN_64      0x2
#define VAL_GPU_RREQ_RD_LEN_68      0x3
#define VAL_GPU_RREQ_RD_LEN_72      0x4
#define VAL_GPU_RREQ_RD_LEN_76      0x5
#define VAL_GPU_RREQ_RD_LEN_80      0x6
#define VAL_GPU_RREQ_RD_LEN_84      0x7

#define REG_GPU_RIU_EN              0x6a
#define MSK_GPU_RIU_EN              0x0001

#define REG_GPU_MIU2_START          0x6b
#define REG_GPU_MIU2_END            0x6c
#define REG_GPU_MIU0_START          0x75
#define REG_GPU_MIU0_END            0x76
#define REG_GPU_MIU1_START          0x77
#define REG_GPU_MIU1_END            0x78

#define MIU0_START_ADDR             MSTAR_MIU0_BUS_BASE
#define MIU0_START_ADDR_H           ((MIU0_START_ADDR >> 20) & 0xFFFF)
#define MIU0_SIZE                   0x40000000UL
#define MIU0_END_ADDR               (MIU0_START_ADDR + MIU0_SIZE)
#define MIU0_END_ADDR_H             ((MIU0_END_ADDR >> 20) & 0xFFFF)

#define MIU1_START_ADDR             MSTAR_MIU1_BUS_BASE
#define MIU1_START_ADDR_H           ((MIU1_START_ADDR >> 20) & 0xFFFF)
#define MIU1_SIZE                   0x40000000UL
#define MIU1_END_ADDR               (MIU1_START_ADDR + MIU1_SIZE)
#define MIU1_END_ADDR_H             ((MIU1_END_ADDR >> 20) & 0xFFFF)

#define MIU2_START_ADDR             MSTAR_MIU2_BUS_BASE
#define MIU2_START_ADDR_H           ((MIU2_START_ADDR >> 20) & 0xFFFF)
#define MIU2_SIZE                   0x40000000UL
#define MIU2_END_ADDR               (MIU2_START_ADDR + MIU2_SIZE)
#define MIU2_END_ADDR_H             ((MIU2_END_ADDR >> 20) & 0xFFFF)

/* REG_GPU_PLL */
#define REG_GPU_PLL_BASE            0x160f00

#define REG_G3DPLL                  0x11
#define MSK_G3DPLL                  0xff00

#define REG_SET_0                   0x60
#define REG_SET_1                   0x61

#define REG_SLD                     0x62
#define MSK_SLD                     0x0001

/* REG_CLKGEN1 */
#define REG_CLKGEN1_BASE            0x103300

#define REG_CKG_GPU                 0x20
#define MSK_DISABLE_CLK             0x0001

/* REG_CHIPTOP */
#define REG_CHIPTOP_BASE            0x101e00

#define REG_CHIPTOP_DUMMY_0         0x1c
#define MSK_CHIPTOP_DUMMY_0         0x00ff

/* RIU mapping */
#define GPU_REG(addr)               RIU[(REG_GPU_BASE) + ((addr) << 1)]
#define GPU_PLL_REG(addr)           RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]
#define CLKGEN1_REG(addr)           RIU[(REG_CLKGEN1_BASE) + ((addr) << 1)]
#define CHIPTOP_REG(addr)           RIU[(REG_CHIPTOP_BASE) + ((addr) << 1)]

/* helper functions */
static void power_on(void)
{
    CLKGEN1_REG(REG_CKG_GPU) &= ~MSK_DISABLE_CLK;       /* enable GPU (by disabling clock gating) */
    GPU_REG(REG_GPU_SRAM_SD_EN) &= ~MSK_GPU_SRAM_SD_EN; /* enable SRAM (by disabling GPU SRAM power down) */
    udelay(1);
}

static void power_off(void)
{
    GPU_REG(REG_GPU_SRAM_SD_EN) |= MSK_GPU_SRAM_SD_EN;  /* disable SRAM (by enabling GPU SRAM power down) */
    CLKGEN1_REG(REG_CKG_GPU) |= MSK_DISABLE_CLK;        /* disable GPU (by enabling clock gating) */
    udelay(1);
}

static void init_registers(void)
{
#define MHZ_TO_REG(MHZ) (0x6c000000 / MHZ)

    unsigned int clk_reg = MHZ_TO_REG(MALI_MAX_FREQ);

    /* GPU clock */
    GPU_PLL_REG(REG_SET_0) = clk_reg & 0xffff;
    GPU_PLL_REG(REG_SET_1) = (clk_reg >> 16) & 0xffff;
    GPU_PLL_REG(REG_SLD) |= MSK_SLD;
    udelay(10);

    /* Power on MPLL */
    CHIPTOP_REG(REG_CHIPTOP_DUMMY_0) &= ~MSK_CHIPTOP_DUMMY_0;
    udelay(25);

    /* Set MIU addr */
    GPU_REG(REG_GPU_MIU0_START) = MIU0_START_ADDR_H;
    GPU_REG(REG_GPU_MIU0_END)   = MIU0_END_ADDR_H;
    GPU_REG(REG_GPU_MIU1_START) = MIU1_START_ADDR_H;
    GPU_REG(REG_GPU_MIU1_END)   = MIU1_END_ADDR_H;
    GPU_REG(REG_GPU_MIU2_START) = MIU2_START_ADDR_H;
    GPU_REG(REG_GPU_MIU2_END)   = MIU2_END_ADDR_H;

    /* Enable RIU */
#ifdef MSTAR_RIU_ENABLED
    GPU_REG(REG_GPU_RIU_EN) |= MSK_GPU_RIU_EN;
#endif

    /* Set read request length to 64 */
    GPU_REG(REG_GPU_RREQ_RD_LEN) &= ~MSK_GPU_RREQ_RD_LEN;
    GPU_REG(REG_GPU_RREQ_RD_LEN) |= (VAL_GPU_RREQ_RD_LEN_64 << SHIFT_GPU_RREQ_RD_LEN);

    /* Power on G3DPLL */
    GPU_PLL_REG(REG_G3DPLL) &= ~MSK_G3DPLL;
    udelay(1);

    /* Soft reset GPU */
    power_on();
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    GPU_REG(REG_GPU_SOFT_RESET) |= MSK_GPU_SOFT_RESET;
    udelay(1); /* delay for runtime suspend */
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
