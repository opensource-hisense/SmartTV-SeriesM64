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

#undef KANO_ENABLE_VOLTAGE_SCALE

/* REG_GPU */
#define REG_GPU_BASE              0x110800

#define REG_GPU_SOFT_RESET        0x00
#define MSK_GPU_SOFT_RESET        0x0001

#define REG_GPU_OUTSDNG_READ      0x60
#define MSK_GPU_OUTSDNG_READ      0x0002

#define REG_GPU_RREQ_RD_LEN       0x61
#define MSK_GPU_RREQ_RD_LEN       0x00e0
#define SHIFT_GPU_RREQ_RD_LEN     5
#define VAL_GPU_RREQ_RD_LEN_48    0x0
#define VAL_GPU_RREQ_RD_LEN_56    0x1
#define VAL_GPU_RREQ_RD_LEN_64    0x2
#define VAL_GPU_RREQ_RD_LEN_68    0x3
#define VAL_GPU_RREQ_RD_LEN_72    0x4
#define VAL_GPU_RREQ_RD_LEN_76    0x5
#define VAL_GPU_RREQ_RD_LEN_80    0x6
#define VAL_GPU_RREQ_RD_LEN_84    0x7

#define REG_GPU_RREQ              0x62
#define MSK_GPU_RREQ_THRD         0x000f
#define MSK_GPU_RREQ_MAX          0xff00

#define REG_GPU_WREQ              0x63
#define MSK_GPU_WREQ_THRD         0x000f
#define MSK_GPU_WREQ_MAX          0xff00

#define REG_GPU_RIU_EN            0x6a
#define MSK_GPU_RIU_EN            0x0001

#define REG_GPU_MIU0_START        0x75
#define REG_GPU_MIU0_END          0x76
#define REG_GPU_MIU1_START        0x77
#define REG_GPU_MIU1_END          0x78

#define REG_GPU_TLB_ENABLE        0x7b
#define MSK_GPU_TLB_ENABLE        0x0004
#define MSK_GPU_64B_WFUL_ON       0x0002

#define REG_GPU_MIU0_TLB_START    0x7c
#define REG_GPU_MIU0_TLB_END      0x7d
#define REG_GPU_MIU1_TLB_START    0x7e
#define REG_GPU_MIU1_TLB_END      0x7f

#define MIU0_START_ADDR           MSTAR_MIU0_BUS_BASE
#define MIU0_START_ADDR_H         ((MIU0_START_ADDR >> 20) & 0xFFFF)
#define MIU0_SIZE                 0x40000000UL
#define MIU0_END_ADDR             (MIU0_START_ADDR+MIU0_SIZE)
#define MIU0_END_ADDR_H           ((MIU0_END_ADDR >> 20) & 0xFFFF)

#define MIU1_START_ADDR           MSTAR_MIU1_BUS_BASE
#define MIU1_START_ADDR_H         ((MIU1_START_ADDR >> 20) & 0xFFFF)
#define MIU1_SIZE                 0x20000000UL
#define MIU1_END_ADDR             (MIU1_START_ADDR+MIU1_SIZE)
#define MIU1_END_ADDR_H           ((MIU1_END_ADDR >> 20) & 0xFFFF)

#define TLB_MIU0_START_ADDR       0xA0000000UL
#define TLB_MIU0_START_ADDR_H     ((TLB_MIU0_START_ADDR >> 20) & 0xFFFF)
#define TLB_MIU0_SIZE             0x20000000UL
#define TLB_MIU0_END_ADDR         (TLB_MIU0_START_ADDR+TLB_MIU0_SIZE)
#define TLB_MIU0_END_ADDR_H       ((TLB_MIU0_END_ADDR >> 20) & 0xFFFF)

#define TLB_MIU1_START_ADDR       0xC0000000UL
#define TLB_MIU1_START_ADDR_H     ((TLB_MIU1_START_ADDR >> 20) & 0xFFFF)
#define TLB_MIU1_SIZE             0x20000000UL
#define TLB_MIU1_END_ADDR         (TLB_MIU1_START_ADDR+TLB_MIU1_SIZE)
#define TLB_MIU1_END_ADDR_H       ((TLB_MIU1_END_ADDR >> 20) & 0xFFFF)

/* REG_GPU_PLL */
#define REG_GPU_PLL_BASE          0x160400

#define REG_G3DPLL                0x11
#define MSK_G3DPLL                0xff00

#define REG_SET_0                 0x60
#define REG_SET_1                 0x61

#define REG_SLD                   0x62
#define MSK_SLD                   0x0001

/* REG_CLKGEN2 */
#define REG_CLKGEN2_BASE          0x100a00

#define REG_CKG_GPU               0x3d
#define MSK_DISABLE_CLK           0x0001

/* REG_PM_SLEEP */
#define REG_PM_SLEEP_BASE         0x000e00

#define REG_PD_XTAL_HV            0x3d
#define MSK_PD_XTAL_HV_CLOCK      0x003f

/* REG_ANA_MISC_MPLLTOP */
#define REG_ANA_MISC_MPLLTOP_BASE 0x110B00
#define REG_MPLL                  0x01
#define MSK_MPLL                  0xff00

/* RIU */
#ifdef CONFIG_ARM64
#define RIU_MAP                   mstar_pm_base
#else
#define RIU_MAP                   0xfd000000
#endif
#define RIU                       ((volatile unsigned short*)(RIU_MAP))
#define GPU_REG(addr)             RIU[(REG_GPU_BASE) + ((addr) << 1)]
#define GPU_PLL_REG(addr)         RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]
#define CLKGEN2_REG(addr)         RIU[(REG_CLKGEN2_BASE) + ((addr) << 1)]
#define PM_SLEEP_REG(addr)        RIU[(REG_PM_SLEEP_BASE) + ((addr) << 1)]
#define MPLL_REG(addr)            RIU[(REG_ANA_MISC_MPLLTOP_BASE) + ((addr) << 1)]

#define MHZ_TO_REG(MHZ)           (0x6c000000/MHZ)
#define CLK_REG_TO_MHZ(clk_reg)   (0x6c000000/clk_reg)

int mali_gpu_clock = MALI_MAX_FREQ;

#ifdef MSTAR_ENABLE_DVFS
#define GPU_DVFS_NUM_SCALE 4
#define GPU_DVFS_DEFAULT_SCALE 3
#define GPU_DVFS_MAX_SCALE (GPU_DVFS_NUM_SCALE - 1)
#define GPU_DVFS_SCALE_STEP 1

#ifdef KANO_ENABLE_VOLTAGE_SCALE
#define CONFIG_DVFS_GPU_POWER_I2C_ENABLE        1
#define CONFIG_DVFS_POWER_SWI2C_BUS             0x01
#define CONFIG_DVFS_STR_INIT                    0xFE
#define CONFIG_DVFS_GPU_POWER_SHIFT             94      /* FIXME: exact value to GPU? */
#define CONFIG_DVFS_POWER_SWI2C_GPU_ID          0x80    /* FIXME: exact value to GPU? */
#define CONFIG_DVFS_POWER_SWI2C_ADDR_CPU        ((CONFIG_DVFS_POWER_SWI2C_BUS << 8) | CONFIG_DVFS_POWER_SWI2C_GPU_ID)
#define CONFIG_DVFS_CHIP_ID_PRADA               0x79
#define CONFIG_DVFS_GPU_POWER_SHIFT_PRADA       (69 - 2)
#endif /* #ifdef KANO_ENABLE_VOLTAGE_SCALE */

enum {
    FREQ,
    VOLTAGE
};

/* TODO: define voltage */
static u32 vf_table[GPU_DVFS_NUM_SCALE][2] = {
                                                {MHZ_TO_REG(400), 1.8}, /* Minimum working freq */
                                                {MHZ_TO_REG(450), 2.0},
                                                {MHZ_TO_REG(500), 2.0},
                                                {MHZ_TO_REG(550), 2.2}  /* default */
                                              };

#ifdef KANO_ENABLE_VOLTAGE_SCALE
extern int MDrv_SW_IIC_Write(u8 u8ChIIC, u8 u8SlaveID, u8 u8AddrCnt, u8* pu8Addr, u32 u32BufLen, u8* pu8Buf);
extern int MDrv_SW_IIC_Read(u8 u8ChIIC, u8 u8SlaveID, u8 u8AddrCnt, u8* pu8Addr, u32 u32BufLen, u8* pu8Buf);

int mstar_gpu_dynamic_power_init(void)
{
    int init_status = true;
#if CONFIG_DVFS_CPU_POWER_I2C_ENABLE
    u8  targetRegAddress[5] =
        {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        };
    u8  targetData[5] =
        {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        };

    targetRegAddress[0] = 0x53;
    targetRegAddress[1] = 0x45;
    targetRegAddress[2] = 0x52;
    targetRegAddress[3] = 0x44;
    targetRegAddress[4] = 0x42;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 5, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_Enter_I2C Failed\n");
        init_status = true;
    }

    targetRegAddress[0] = 0x7F;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_USE_CFG Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x7D;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_OUT_NO_DELAY Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x50;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_AD_BYTE_EN0 Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x55;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_DA_BYTE_EN1 Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x35;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_USE Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x10;
    targetRegAddress[1] = 0xc0;
    if(MDrv_SW_IIC_Read(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 2, targetRegAddress, 2, targetData) != -1)
    {
        printk("[INFO] MStar Power IC Chip ID: %x%x\n", (unsigned int) targetData[0], (unsigned int) targetData[1]);
        power_chip_id = (unsigned int) targetData[1];
    }
    else
    {
        init_status = false;
    }

#ifdef CONFIG_DVFS_CHIP_ID_PRADO
    //Set OTP Level
    targetRegAddress[0] = 0x10;
    targetRegAddress[1] = (0x05 << 1);
    targetRegAddress[2] = 0x40;
    targetRegAddress[3] = 0x00;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 4, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] Software I2C Write Failed\n");
        init_status = false;
    }

    //Set Password
    targetRegAddress[0] = 0x10;
    targetRegAddress[1] = (0x0C << 1);
    targetRegAddress[2] = 0xbe;
    targetRegAddress[3] = 0xaf;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 4, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] Software I2C Write Failed\n");
        init_status = false;
    }
#endif /* #ifdef CONFIG_DVFS_CHIP_ID_PRADO */
//    SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);

    //*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100510 << 1)) = CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT;
#endif /* #if CONFIG_DVFS_CPU_POWER_I2C_ENABLE */

    return init_status;

}

void mstar_voltage_adjust(u32 target_voltage)
{
    u32 registerValue = 0;
    u8 targetRegAddress[5] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    u8 targetData[5] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
#ifdef CONFIG_DVFS_CHIP_ID_PRADO
    //Set Core Voltage
    registerValue = (target_voltage - CONFIG_DVFS_GPU_POWER_SHIFT_PRADO);
    targetRegAddress[0] = 0x10;
    targetRegAddress[1] = (0x06 << 1);
    targetRegAddress[2] = 0x10;
    targetRegAddress[3] = registerValue;
    if(MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID,
            4, targetRegAddress, 0, targetData) == true/* true */) {
        printk("[Mali DVFS] Adj Voltage to: %u, reg %X\n", target_voltage, registerValue);
    }
#endif /* #ifdef CONFIG_DVFS_CHIP_ID_PRADO */

}

#endif /* #ifdef KANO_ENABLE_VOLTAGE_SCALE */

void mstar_lpf_freq_adjust(struct kbase_device *kbdev,
        u32 clk_reg)
{
    if(!kbdev->dev)
        return;

    /* dev_info(kbdev->dev, "%s: target:%x\n", __func__, clk_reg); */

    GPU_PLL_REG(REG_SET_0) = clk_reg & 0xffff;
    GPU_PLL_REG(REG_SET_1) = (clk_reg >> 16) & 0xffff;
    GPU_PLL_REG(REG_SLD) |= MSK_SLD;
    udelay(1);

    mali_gpu_clock = CLK_REG_TO_MHZ(clk_reg);
}

bool mstar_platform_kano_dvfs_enable(struct kbase_device *kbdev)
{
    if (!kbdev)
        return false;

    kbdev->dvfs_enable = 1;

    return true;
}

bool mstar_platform_kano_dvfs_disable(struct kbase_device *kbdev)
{
    u32 current_dvfs_scale;
    u32 default_freq_reg, default_voltage_reg;

    if(!kbdev)
        return false;

    current_dvfs_scale = kbdev->dvfs.current_dvfs_scale;
    default_freq_reg = vf_table[GPU_DVFS_DEFAULT_SCALE][FREQ];
    default_voltage_reg = vf_table[GPU_DVFS_DEFAULT_SCALE][VOLTAGE];

    /* restore frequency and voltage to default */
    if (current_dvfs_scale > GPU_DVFS_DEFAULT_SCALE) {
        /* decrease frequency first */
        mstar_lpf_freq_adjust(kbdev, default_freq_reg);
#ifdef KANO_ENABLE_VOLTAGE_SCALE
        mstar_voltage_adjust(default_voltage_reg);
#endif
    } else {
        /* increase voltage first */
#ifdef KANO_ENABLE_VOLTAGE_SCALE
        mstar_voltage_adjust(default_voltage_reg);
#endif
        mstar_lpf_freq_adjust(kbdev, default_freq_reg);
    }

    kbdev->dvfs_enable = 0;
    kbdev->dvfs.current_dvfs_scale = GPU_DVFS_MAX_SCALE;
    /*
    dev_info(kbdev->dev, "%s: disable DVFS, adjust GPU Freq scale from %u -> %u scale\n",
            __func__, current_dvfs_scale, GPU_DVFS_DEFAULT_SCALE);
    */
    return false;
}

bool mstar_platform_kano_clockup(struct kbase_device *kbdev)
{

    u16 current_dvfs_scale;
    u16 target_dvfs_scale;
    u32 target_freq_reg, target_voltage_reg;

    if (!kbdev)
        return false;

    current_dvfs_scale = kbdev->dvfs.current_dvfs_scale;
    target_dvfs_scale = current_dvfs_scale + GPU_DVFS_SCALE_STEP;

    if (current_dvfs_scale >= GPU_DVFS_MAX_SCALE)
        return false;

    /* frequency and voltage lookup */
    target_freq_reg = vf_table[target_dvfs_scale][FREQ];
    target_voltage_reg = vf_table[target_dvfs_scale][VOLTAGE];

#ifdef KANO_ENABLE_VOLTAGE_SCALE
    /* Increase to target voltage first */
    mstar_voltage_adjust(target_voltage_reg);
#endif
    /* then perform GPU clockup */
    mstar_lpf_freq_adjust(kbdev, target_freq_reg);

    kbdev->dvfs.current_dvfs_scale = target_dvfs_scale;

    /*
    dev_info(kbdev->dev, "%s: clockup GPU Freq from %u -> %u scale\n",
            __func__, current_dvfs_scale, target_dvfs_scale);
    */
    return true;
}

bool mstar_platform_kano_clockdown(struct kbase_device *kbdev)
{
    u16 current_dvfs_scale;
    u16 target_dvfs_scale;
    u32 target_freq_reg;
    u32 target_voltage_reg;

    if (!kbdev)
        return false;

    current_dvfs_scale = kbdev->dvfs.current_dvfs_scale;

    if(current_dvfs_scale == 0)
        return true;

    if (current_dvfs_scale < GPU_DVFS_SCALE_STEP)
        target_dvfs_scale = 0;
    else
        target_dvfs_scale = current_dvfs_scale - GPU_DVFS_SCALE_STEP;

    /* frequency and voltage lookup */
    target_freq_reg = vf_table[target_dvfs_scale][FREQ];
    target_voltage_reg = vf_table[target_dvfs_scale][VOLTAGE];

    /* Perform GPU clockdown */
    mstar_lpf_freq_adjust(kbdev, target_freq_reg);
#ifdef KANO_ENABLE_VOLTAGE_SCALE
    /* then decrease voltage level */
    mstar_voltage_adjust(target_voltage_reg);
#endif
    kbdev->dvfs.current_dvfs_scale = target_dvfs_scale;

    /*
    dev_info(kbdev->dev, "%s: clockdown GPU Freq from %u -> %u scale\n",
            __func__, current_dvfs_scale, target_dvfs_scale);
    */
    return true;
}

void mstar_platform_kano_dvfs_init(struct kbase_device *kbdev)
{
#ifdef KANO_ENABLE_VOLTAGE_ENABLE
    mstar_platform_gpu_dynamic_power_init();
#endif

    dev_info(kbdev->dev, "%s: register DVFS callbacks\n" ,__func__);

    kbdev->dvfs.current_dvfs_scale = GPU_DVFS_DEFAULT_SCALE;
    kbdev->dvfs.max_dvfs_scale = GPU_DVFS_MAX_SCALE;
    kbdev->dvfs.enable = mstar_platform_kano_dvfs_enable;
    kbdev->dvfs.disable = mstar_platform_kano_dvfs_disable;
    kbdev->dvfs.gpu_clock_up = mstar_platform_kano_clockup;
    kbdev->dvfs.gpu_clock_down = mstar_platform_kano_clockdown;

    mstar_platform_kano_dvfs_enable(kbdev);
}
#endif /* #ifdef MSTAR_ENABLE_DVFS */

/* */
int mstar_platform_init(struct kbase_device *kbdev)
{

    mstar_platform_power_on(kbdev);
#ifdef MSTAR_ENABLE_DVFS
    mstar_platform_kano_dvfs_init(kbdev);
#endif

    return 0;
}
void mstar_platform_term(struct kbase_device *kbdev)
{
    mstar_platform_power_off(kbdev);
}

void mstar_platform_power_off(struct kbase_device *kbdev)
{
    /* disable GPU (by enabling clock gating) */
    CLKGEN2_REG(REG_CKG_GPU) |= MSK_DISABLE_CLK;
    udelay(1);
}

int mstar_platform_power_on(struct kbase_device *kbdev)
{
    unsigned int clk_reg = MHZ_TO_REG(MALI_MAX_FREQ);

    /* Turn on XTali */
    PM_SLEEP_REG(REG_PD_XTAL_HV) &= ~MSK_PD_XTAL_HV_CLOCK;

    /* GPU clock */
    GPU_PLL_REG(REG_SET_0) = clk_reg & 0xffff;
    GPU_PLL_REG(REG_SET_1) = (clk_reg >> 16) & 0xffff;
    GPU_PLL_REG(REG_SLD) |= MSK_SLD;
    udelay(10);

    /* Power on MPLL */
    MPLL_REG(REG_MPLL) &= ~MSK_MPLL;
    udelay(1);

    /* Set MIU addr */
    GPU_REG(REG_GPU_MIU0_START) = MIU0_START_ADDR_H;
    GPU_REG(REG_GPU_MIU0_END)   = MIU0_END_ADDR_H;
    GPU_REG(REG_GPU_MIU1_START) = MIU1_START_ADDR_H;
    GPU_REG(REG_GPU_MIU1_END)   = MIU1_END_ADDR_H;

    /* Disable TLB and disable 64-bit full write func */
    GPU_REG(REG_GPU_TLB_ENABLE) &= ~(MSK_GPU_TLB_ENABLE | MSK_GPU_64B_WFUL_ON);

    /* Set MIU TLB addr */
    GPU_REG(REG_GPU_MIU0_TLB_START) = TLB_MIU0_START_ADDR_H;
    GPU_REG(REG_GPU_MIU0_TLB_END)   = TLB_MIU0_END_ADDR_H;
    GPU_REG(REG_GPU_MIU1_TLB_START) = TLB_MIU1_START_ADDR_H;
    GPU_REG(REG_GPU_MIU1_TLB_END)   = TLB_MIU1_END_ADDR_H;

    /* Enable RIU */
    GPU_REG(REG_GPU_RIU_EN) |= MSK_GPU_RIU_EN;

    /* Set read request length to 64 */
    GPU_REG(REG_GPU_RREQ_RD_LEN) &= ~MSK_GPU_RREQ_RD_LEN;
    GPU_REG(REG_GPU_RREQ_RD_LEN) |= (VAL_GPU_RREQ_RD_LEN_64 << SHIFT_GPU_RREQ_RD_LEN);

    /* Power on G3DPLL */
    GPU_PLL_REG(REG_G3DPLL) &= ~MSK_G3DPLL;
    udelay(1);

    /* Enable GPU clock */
    CLKGEN2_REG(REG_CKG_GPU) &= ~MSK_DISABLE_CLK;
    udelay(1);

    /* Soft reset GPU */
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    GPU_REG(REG_GPU_SOFT_RESET) |= MSK_GPU_SOFT_RESET;
    udelay(1); /*delay for run-time suspend*/
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    udelay(1);

    return 0;
}

void mstar_platform_power_suspend(struct kbase_device *kbdev)
{
    mstar_platform_power_off(kbdev);    /* just power off */
}

void mstar_platform_power_resume(struct kbase_device *kbdev)
{
    mstar_platform_power_on(kbdev);     /* just power on */
}
int mstar_platform_runtime_resume(struct kbase_device *kbdev)
{
    /* Enable GPU clock */
    CLKGEN2_REG(REG_CKG_GPU) &= ~MSK_DISABLE_CLK;
    udelay(1);
    return 0;
}
