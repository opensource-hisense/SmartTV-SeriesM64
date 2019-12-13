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

#include <linux/delay.h>
#include "mstar/mstar_chip.h"

#undef KANO_ENABLE_VOLTAGE_SCALE

/* REG_GPU_PLL */
#define REG_GPU_PLL_BASE            0x160400

#define REG_SET_0                   0x60
#define REG_SET_1                   0x61
#define REG_SLD                     0x62
#define MSK_SLD                     0x0001

/* RIU mapping */
#define GPU_PLL_REG(addr)           RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]

/* DVFS */
#define MHZ_TO_REG(MHZ)             (0x6c000000 / MHZ)

#define GPU_DVFS_NUM_SCALES         4
#define GPU_DVFS_DEFAULT_SCALE      3
#define GPU_DVFS_MAX_SCALE          (GPU_DVFS_NUM_SCALES - 1)
#define GPU_DVFS_SCALE_STEP         1
#define GPU_MIN_FREQ                300

#ifdef KANO_ENABLE_VOLTAGE_SCALE
#define CONFIG_DVFS_GPU_POWER_I2C_ENABLE    1
#define CONFIG_DVFS_POWER_SWI2C_BUS         0x01
#define CONFIG_DVFS_STR_INIT                0xFE
#define CONFIG_DVFS_GPU_POWER_SHIFT         94      /* FIXME: exact value to GPU? */
#define CONFIG_DVFS_POWER_SWI2C_GPU_ID      0x80    /* FIXME: exact value to GPU? */
#define CONFIG_DVFS_POWER_SWI2C_ADDR_CPU    ((CONFIG_DVFS_POWER_SWI2C_BUS << 8) | CONFIG_DVFS_POWER_SWI2C_GPU_ID)
#define CONFIG_DVFS_CHIP_ID_PRADA           0x79
#define CONFIG_DVFS_GPU_POWER_SHIFT_PRADA   (69 - 2)
#endif

enum
{
    FREQ,
    VOLTAGE
};

static u32 vf_table[GPU_DVFS_NUM_SCALES][2] =
{
    { MHZ_TO_REG(400), 1.8 }, /* minimum working frequency */
    { MHZ_TO_REG(450), 2.0 },
    { MHZ_TO_REG(500), 2.0 },
    { MHZ_TO_REG(550), 2.2 }  /* default */
};

#ifdef KANO_ENABLE_VOLTAGE_SCALE
extern int MDrv_SW_IIC_Write(u8 u8ChIIC, u8 u8SlaveID, u8 u8AddrCnt, u8* pu8Addr, u32 u32BufLen, u8* pu8Buf);
extern int MDrv_SW_IIC_Read(u8 u8ChIIC, u8 u8SlaveID, u8 u8AddrCnt, u8* pu8Addr, u32 u32BufLen, u8* pu8Buf);

static int mstar_gpu_dynamic_power_init(void)
{
    int init_status = true;

#if CONFIG_DVFS_CPU_POWER_I2C_ENABLE
    u8 targetRegAddress[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    u8 targetData[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    targetRegAddress[0] = 0x53;
    targetRegAddress[1] = 0x45;
    targetRegAddress[2] = 0x52;
    targetRegAddress[3] = 0x44;
    targetRegAddress[4] = 0x42;

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 5, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_Enter_I2C Failed\n");
        init_status = true;
    }

    targetRegAddress[0] = 0x7F;

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_USE_CFG Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x7D;

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_OUT_NO_DELAY Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x50;

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_AD_BYTE_EN0 Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x55;

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_DA_BYTE_EN1 Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x35;

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 1, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] I2C_USE Failed\n");
        init_status = false;
    }

    targetRegAddress[0] = 0x10;
    targetRegAddress[1] = 0xc0;

    if (MDrv_SW_IIC_Read(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 2, targetRegAddress, 2, targetData) != -1)
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

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS, CONFIG_DVFS_POWER_SWI2C_GPU_ID, 4, targetRegAddress, 0, targetData) == -1)
    {
        printk("[ERROR] Software I2C Write Failed\n");
        init_status = false;
    }
#endif

#if 0
    SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);

    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100510 << 1)) = CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT;
#endif
#endif /* CONFIG_DVFS_CPU_POWER_I2C_ENABLE */

    return init_status;
}

static void adjust_voltage(u32 target_voltage)
{
    u32 registerValue = 0;
    u8 targetRegAddress[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    u8 targetData[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

#ifdef CONFIG_DVFS_CHIP_ID_PRADO
    /* Set Core Voltage */
    registerValue = (target_voltage - CONFIG_DVFS_GPU_POWER_SHIFT_PRADO);
    targetRegAddress[0] = 0x10;
    targetRegAddress[1] = (0x06 << 1);
    targetRegAddress[2] = 0x10;
    targetRegAddress[3] = registerValue;

    if (MDrv_SW_IIC_Write(CONFIG_DVFS_POWER_SWI2C_BUS,
                          CONFIG_DVFS_POWER_SWI2C_GPU_ID,
                          4,
                          targetRegAddress,
                          0,
                          targetData) == true)
    {
        printk("[Mali DVFS] Adj Voltage to: %u, reg %X\n", target_voltage, registerValue);
    }
#endif
}
#endif /* KANO_ENABLE_VOLTAGE_SCALE */

static void adjust_frequency(struct kbase_device* kbdev, u32 clk_reg)
{
    if (!kbdev->dev)
    {
        return;
    }

    /* dev_info(kbdev->dev, "%s: target:%x\n", __func__, clk_reg); */

    /* GPU clock */
    GPU_PLL_REG(REG_SET_0) = clk_reg & 0xffff;
    GPU_PLL_REG(REG_SET_1) = (clk_reg >> 16) & 0xffff;
    GPU_PLL_REG(REG_SLD) |= MSK_SLD;
    udelay(1);
}

/* DVFS callbacks */
static bool dvfs_enable(struct kbase_device* kbdev)
{
    if (!kbdev)
    {
        return false;
    }

    kbdev->dvfs.enabled = 1;
    return true;
}

static bool dvfs_disable(struct kbase_device* kbdev)
{
    u32 current_scale;
    u32 default_freq_reg;
    u32 default_voltage_reg;

    if (!kbdev)
    {
        return false;
    }

    current_scale = kbdev->dvfs.current_scale;
    default_freq_reg = vf_table[GPU_DVFS_DEFAULT_SCALE][FREQ];
    default_voltage_reg = vf_table[GPU_DVFS_DEFAULT_SCALE][VOLTAGE];

    /* restore frequency and voltage to default */
    if (current_scale > GPU_DVFS_DEFAULT_SCALE)
    {
        /* decrease frequency first */
        adjust_frequency(kbdev, default_freq_reg);
#ifdef KANO_ENABLE_VOLTAGE_SCALE
        adjust_voltage(default_voltage_reg);
#endif
    }
    else
    {
        /* increase voltage first */
#ifdef KANO_ENABLE_VOLTAGE_SCALE
        adjust_voltage(default_voltage_reg);
#endif
        adjust_frequency(kbdev, default_freq_reg);
    }

    kbdev->dvfs.enabled = 0;
    kbdev->dvfs.current_scale = GPU_DVFS_MAX_SCALE;

#if 0
    dev_info(kbdev->dev,
             "%s: disable DVFS, adjust GPU Freq scale from %u -> %u scale\n",
             __func__, current_scale, GPU_DVFS_DEFAULT_SCALE);
#endif

    return false;
}

static bool dvfs_clock_up(struct kbase_device* kbdev)
{
    u32 current_scale;
    u32 target_scale;
    u32 target_freq_reg;
    u32 target_voltage_reg;

    if (!kbdev)
    {
        return false;
    }

    current_scale = kbdev->dvfs.current_scale;
    target_scale = current_scale + GPU_DVFS_SCALE_STEP;

    if (current_scale >= GPU_DVFS_MAX_SCALE)
    {
        return false;
    }

    /* frequency and voltage lookup */
    target_freq_reg = vf_table[target_scale][FREQ];
    target_voltage_reg = vf_table[target_scale][VOLTAGE];

#ifdef KANO_ENABLE_VOLTAGE_SCALE
    /* Increase to target voltage first */
    adjust_voltage(target_voltage_reg);
#endif
    /* then perform GPU clockup */
    adjust_frequency(kbdev, target_freq_reg);

    kbdev->dvfs.current_scale = target_scale;

#if 0
    dev_info(kbdev->dev, "%s: clockup GPU Freq from %u -> %u scale\n", __func__, current_scale, target_scale);
#endif

    return true;
}

static bool dvfs_clock_down(struct kbase_device* kbdev)
{
    u32 current_scale;
    u32 target_scale;
    u32 target_freq_reg;
    u32 target_voltage_reg;

    if (!kbdev)
    {
        return false;
    }

    current_scale = kbdev->dvfs.current_scale;

    if (current_scale == 0)
    {
        return true;
    }

    if (current_scale < GPU_DVFS_SCALE_STEP)
    {
        target_scale = 0;
    }
    else
    {
        target_scale = current_scale - GPU_DVFS_SCALE_STEP;
    }

    /* frequency and voltage lookup */
    target_freq_reg = vf_table[target_scale][FREQ];
    target_voltage_reg = vf_table[target_scale][VOLTAGE];

    /* Perform GPU clockdown */
    adjust_frequency(kbdev, target_freq_reg);
#ifdef KANO_ENABLE_VOLTAGE_SCALE
    /* then decrease voltage level */
    adjust_voltage(target_voltage_reg);
#endif

    kbdev->dvfs.current_scale = target_scale;

#if 0
    dev_info(kbdev->dev, "%s: clockdown GPU Freq from %u -> %u scale\n", __func__, current_scale, target_scale);
#endif

    return true;
}

/* */
void mstar_platform_dvfs_init(struct kbase_device* kbdev)
{
#ifdef KANO_ENABLE_VOLTAGE_ENABLE
    mstar_platform_gpu_dynamic_power_init();
#endif

    dev_info(kbdev->dev, "[%s] register DVFS callbacks\n" ,__func__);

    kbdev->dvfs.current_scale = GPU_DVFS_DEFAULT_SCALE;
    kbdev->dvfs.enable = dvfs_enable;
    kbdev->dvfs.disable = dvfs_disable;
    kbdev->dvfs.clock_up = dvfs_clock_up;
    kbdev->dvfs.clock_down = dvfs_clock_down;

    dvfs_enable(kbdev);
}

#endif /* CONFIG_MALI_MIDGARD_DVFS */
