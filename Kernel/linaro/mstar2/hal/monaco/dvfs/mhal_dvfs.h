////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2014 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MHAL_DVFS_H__
#define __MHAL_DVFS_H__

#ifndef __MDRV_TYPES_H__
#include "mdrv_types.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DVFS_HAL_INFO(x, args...)                       {printk(x, ##args);}
#define DVFS_HAL_DEBUG(x, args...)                      //{printk(x, ##args);}

#define CONFIG_REGISTER_BASE_ADDRESS                    0xfd000000

#define CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE            1 //0: Disable Test Bus; 1: Enable Test Bus

#define CONFIG_DVFS_ENABLE_PATTERN                      0x3697
#define CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT           0x2454

#define CONFIG_DVFS_DATA_COUNT                          1
#define CONFIG_DVFS_AVERAGE_COUNT                       5
#define CONFIG_DVFS_DELAY_US                            1
#define CONFIG_DVFS_MUTEX_WAIT_TIME                     50

#define DVFS_CPU_CLOCK_DOWN                             0
#define DVFS_CPU_CLOCK_UP                               1
#define DVFS_CPU_POWER_DOWN                             0
#define DVFS_CPU_POWER_UP                               1

#define CONFIG_DVFS_STATE_LOW_SPEED                     0
#define CONFIG_DVFS_STATE_HIGH_SPEED                    1
#define CONFIG_DVFS_STATE_INIT                          0xFF

#define CONFIG_DVFS_SS_CORNER_CHIP_NORMAL               0x00
#define CONFIG_DVFS_FF_CORNER_CHIP_NORMAL               0x01
#define CONFIG_DVFS_SS_CORNER_CHIP_MAX                  0x02
#define CONFIG_DVFS_FF_CORNER_CHIP_MAX                  0x03
#define CONFIG_DVFS_FF_CORNER_CHIP_MASK                 0x03

#define CONFIG_DVFS_POWER_CONTROL_FREEZE                0
#define CONFIG_DVFS_POWER_CONTROL_NORMAL                1

#define CONFIG_DVFS_POWER_CONTROL_LOWER_BOUND           60
#define CONFIG_DVFS_POWER_CONTROL_UPPER_BOUND           80
#define CONFIG_DVFS_DEFAULT_LOWER_BOUND                 120
#define CONFIG_DVFS_DEFAULT_UPPER_BOUND                 135
#define CONFIG_DVFS_DEFAULT_RESET_THRESHOLD             160

#define CONFIG_DVFS_CPU_POWER_INIT                      110
#define CONFIG_DVFS_CPU_POWER_DEFAULT                   105
#define CONFIG_DVFS_CPU_POWER_DROP                      5

#define CONFIG_DVFS_CPU_POWER_INIT_MAX_EX_LOWER_BOUND   130
#define CONFIG_DVFS_CPU_POWER_INIT_MAX_LOWER_BOUND      135
#define CONFIG_DVFS_CPU_POWER_INIT_L3_LOWER_BOUND       130
#define CONFIG_DVFS_CPU_POWER_INIT_L2_LOWER_BOUND       120
#define CONFIG_DVFS_CPU_POWER_INIT_L1_2_LOWER_BOUND     115
#define CONFIG_DVFS_CPU_POWER_INIT_L1_1_LOWER_BOUND     110
#define CONFIG_DVFS_CPU_POWER_INIT_L0_LOWER_BOUND       110

#define CONFIG_DVFS_CPU_POWER_INIT_MAX_UPPER_BOUND      150
#define CONFIG_DVFS_CPU_POWER_INIT_L3_UPPER_BOUND       140
#define CONFIG_DVFS_CPU_POWER_INIT_L2_UPPER_BOUND       125
#define CONFIG_DVFS_CPU_POWER_INIT_L1_2_UPPER_BOUND     120
#define CONFIG_DVFS_CPU_POWER_INIT_L1_1_UPPER_BOUND     115
#define CONFIG_DVFS_CPU_POWER_INIT_L0_UPPER_BOUND       110     //Minimum

#define CONFIG_DVFS_CPU_POWER_MAX_EX_LOWER_BOUND        130
#define CONFIG_DVFS_CPU_POWER_MAX_LOWER_BOUND           135
#define CONFIG_DVFS_CPU_POWER_L3_LOWER_BOUND            125
#define CONFIG_DVFS_CPU_POWER_L2_LOWER_BOUND            115
#define CONFIG_DVFS_CPU_POWER_L1_2_LOWER_BOUND          110
#define CONFIG_DVFS_CPU_POWER_L1_1_LOWER_BOUND          105
#define CONFIG_DVFS_CPU_POWER_L0_LOWER_BOUND            100
#define CONFIG_DVFS_CPU_POWER_DROP_LOWER_BOUND          5

#define CONFIG_DVFS_CPU_POWER_MAX_UPPER_BOUND           145
#define CONFIG_DVFS_CPU_POWER_L3_UPPER_BOUND            140
#define CONFIG_DVFS_CPU_POWER_L2_UPPER_BOUND            125
#define CONFIG_DVFS_CPU_POWER_L1_2_UPPER_BOUND          120
#define CONFIG_DVFS_CPU_POWER_L1_1_UPPER_BOUND          115
#define CONFIG_DVFS_CPU_POWER_L0_UPPER_BOUND            105
#define CONFIG_DVFS_CPU_POWER_DROP_UPPER_BOUND          5

#define CONFIG_DVFS_CORE_POWER_INIT                     105
#define CONFIG_DVFS_CORE_POWER_DEFAULT                  100
#define CONFIG_DVFS_CORE_POWER_MAX                      100
#define CONFIG_DVFS_CORE_POWER_MIN                      95
#define CONFIG_DVFS_CORE_POWER_DROP                     0

#define CONFIG_DVFS_CPU_CLOCK_DEFAULT                   1008
#define CONFIG_DVFS_CPU_CLOCK_MIN                       900
#define CONFIG_DVFS_CPU_CLOCK_L0                        900
#define CONFIG_DVFS_CPU_CLOCK_L1                        1100
#define CONFIG_DVFS_CPU_CLOCK_L1_1                      1200    //for IR Boost
#define CONFIG_DVFS_CPU_CLOCK_L1_2                      1300    //for IR Boost
#define CONFIG_DVFS_CPU_CLOCK_L2                        1400
#define CONFIG_DVFS_CPU_CLOCK_L3                        1700
#define CONFIG_DVFS_CPU_CLOCK_MAX                       1800
#define CONFIG_DVFS_CPU_CLOCK_PROTECTION                850

#define CONFIG_DVFS_CPU_CLOCK_FREEZE_MODE               100

#define CONFIG_DVFS_CPU_POWER_PROTECTION_UPPER_BOUND    100
#define CONFIG_DVFS_CPU_POWER_PROTECTION_LOWER_BOUND    95

#define CONFIG_DVFS_T_SENSOR_SHIFT                      32000 //(27000 + 15000)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    U32     bDvfsInitOk;
    U32     bCpuClockLevel;
    U32     bPowerControlLevel;
    U32     bPowerControlChange;
    U32     dwOverCounter;
    U32     dwVidSetting;
    U32     bOverTemperatureFlag;
    U32     bSystemResumeFlag;

    S32     dwFinalCpuTemperature;
    U32     dwAvgTempBufferCpu[CONFIG_DVFS_DATA_COUNT];
    U32     dwAvgTempCounterCpu;
    U32     dwRefTemperature;

    U16     wCpuClockSetting_High;
    U16     wCpuClockSetting_Low;
    U16     wProtectCpuClockSetting_High;
    U16     wProtectCpuClockSetting_Low;

    U32     dwTemperatureCounter;
    U32     dwValidCpuClockByTemperature;

    U32     dwPresetCpuClock;
    U32     dwFinalCpuClock;

} MSTAR_DVFS_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
U32  MHalDvfsProc(U32 dwCpuClock);
void MHalDvfsInit(void);
void MHalDvfsCpuDisplayInit(void);
void MHalDvfsCpuDisplay(void);
void MHalDvfsCpuTemperature(void);
void MHalDvfsRefTemperature(void);
void MHalDvfsCpuClockAdjustment(U32 dwDvfsUpperBoundCpuClock_Low, U32 dwDvfsUpperBoundCpuClock_High, U32 bCpuClockRisingFlag);
void MHalDvfsPowerControlStateUpdate(void);
void MHalDvfsCpuPowerInit(void);
void MHalDvfsCpuPowerAdjustment(U32 dwCpuPowerVoltage);
void MHalDvfsCorePowerInit(void);
void MHalDvfsCorePowerAdjustment(U32 dwCorePowerVoltage);
U32  MHalDvfsQueryCpuClockByTemperature(void);

#endif
