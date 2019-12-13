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

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/delay.h>

#include <mstar/mstar_chip.h>

#ifndef __MHAL_DVFS_H__
#include "mhal_dvfs.h"
#endif

extern void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType);

static MSTAR_DVFS_INFO hMstarDvfsInfo =
{
    .bDvfsInitOk = 0,
    .bCpuClockLevel = CONFIG_DVFS_STATE_INIT,
    .dwOverCounter = 0,
    .dwVidSetting = 0,

    .dwFinalCpuTemperature = 0,
    .dwAvgTempCounterCpu = 0,
    .dwRefTemperature = 0,

    .wCpuClockSetting_High = 0,
    .wCpuClockSetting_Low = 0,

    .dwPresetCpuClock = 0,
    .dwFinalCpuClock = 0,
};

//=================================================================================================
U32 MHalDvfsProc(U32 dwCpuClock)
{
    U32     dwRegisterValue = 0;

    hMstarDvfsInfo.dwPresetCpuClock = dwCpuClock;

    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100500 << 1));
    if(dwRegisterValue == CONFIG_DVFS_ENABLE_PATTERN)
    {
        S32     dwUpperTemperature = 0;
        S32     dwLowerTemperature = 0;
        S32     dwResetTemperature = 0;
#if 0
        if(hMstarDvfsInfo.bDvfsInitOk == 0)
        {
            _s32SAR_Dvfs_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex SAR_DVFS", MSOS_PROCESS_SHARED);
            MS_ASSERT(_s32SAR_Dvfs_Mutex >= 0);
        }

        if (FALSE == MsOS_ObtainMutex(_s32SAR_Dvfs_Mutex, CONFIG_DVFS_MUTEX_WAIT_TIME))
        {
            DVFS_HAL_INFO("[DVFS] Mutex Lock Fail\n");
            return;
        }
#endif
        if(hMstarDvfsInfo.bDvfsInitOk == 0)
        {
            MHalDvfsInit();
        }

        MHalDvfsCpuTemperature();

        //=========================================================
        dwUpperTemperature = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100508 << 1));
        dwUpperTemperature >>= 8;
        if(dwUpperTemperature == 0)
        {
            dwUpperTemperature = CONFIG_DVFS_DEFAULT_UPPER_BOUND;
        }

        dwLowerTemperature = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100508 << 1));
        dwLowerTemperature &= 0xFF;
        if(dwLowerTemperature == 0)
        {
            dwLowerTemperature = CONFIG_DVFS_DEFAULT_LOWER_BOUND;
        }

        dwResetTemperature = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x10050a << 1));
        dwResetTemperature &= 0xFF;
        if(dwResetTemperature == 0)
        {
            dwResetTemperature = CONFIG_DVFS_DEFAULT_RESET_THRESHOLD;
        }

        if((hMstarDvfsInfo.dwFinalCpuTemperature > dwResetTemperature) && (hMstarDvfsInfo.bCpuClockLevel != CONFIG_DVFS_STATE_INIT))
        {
            if(hMstarDvfsInfo.dwOverCounter < CONFIG_DVFS_AVERAGE_COUNT)
            {
                DVFS_HAL_DEBUG("[DVFS] Over Temperature Protection: %d\n", (unsigned int) hMstarDvfsInfo.dwOverCounter);
                hMstarDvfsInfo.dwOverCounter ++;
            }
            else
            {
                //Trigger a WDT Reset
                DVFS_HAL_INFO("[DVFS] Over Temperature Protection\n");
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00300a << 1)) = 0x00;
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x003008 << 1)) = 0x00;
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00300a << 1)) = 0x05;
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x003000 << 1)) = 0x01;

                while(1);
            }
        }
        else if((hMstarDvfsInfo.dwFinalCpuTemperature > dwUpperTemperature) && (hMstarDvfsInfo.bCpuClockLevel == CONFIG_DVFS_STATE_HIGH_SPEED))
        {
            if(hMstarDvfsInfo.dwOverCounter < CONFIG_DVFS_AVERAGE_COUNT)
            {
                hMstarDvfsInfo.dwOverCounter ++;
            }
            else
            {
                DVFS_HAL_INFO("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo.dwFinalCpuTemperature);
                DVFS_HAL_INFO("[DVFS] Change to Lower CPU Clock Setting\n");

                //high to low
                MHalDvfsCpuClockAdjustment(
                                    hMstarDvfsInfo.wCpuClockSetting_Low,
                                    hMstarDvfsInfo.wCpuClockSetting_High,
                                    DVFS_CPU_CLOCK_DOWN
                                    );

                if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                {
                    //Set VID Level
                    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1));
                    dwRegisterValue &= 0xF00F;
                    dwRegisterValue |= (((hMstarDvfsInfo.dwVidSetting - 0x1000) & 0xFF00) >> 4);    //Dowbgrade Voltage Level
                    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) = dwRegisterValue;
                }
                else
                {
                    dwRegisterValue = (hMstarDvfsInfo.dwVidSetting & 0x00FF);
                    if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CPU_POWER_DEFAULT))
                    {
                        MHalDvfsCpuPowerAdjustment(dwRegisterValue - CONFIG_DVFS_CPU_POWER_DROP);
                    }
                    else
                    {
                        MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT - CONFIG_DVFS_CPU_POWER_DROP);
                    }

                    dwRegisterValue = ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 8);
                    if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CORE_POWER_DEFAULT))
                    {
                        MHalDvfsCorePowerAdjustment(dwRegisterValue);
                    }
                    else
                    {
                        MHalDvfsCorePowerAdjustment(CONFIG_DVFS_CORE_POWER_DEFAULT);
                    }
                }

                hMstarDvfsInfo.bCpuClockLevel = CONFIG_DVFS_STATE_LOW_SPEED;
                hMstarDvfsInfo.dwOverCounter = 0;
            }
        }
        else if((hMstarDvfsInfo.dwFinalCpuTemperature < dwLowerTemperature) && (hMstarDvfsInfo.bCpuClockLevel == CONFIG_DVFS_STATE_LOW_SPEED))
        {
            if(hMstarDvfsInfo.dwOverCounter < CONFIG_DVFS_AVERAGE_COUNT)
            {
                hMstarDvfsInfo.dwOverCounter ++;
            }
            else
            {
                DVFS_HAL_INFO("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo.dwFinalCpuTemperature);

                if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                {
                    //Set VID Level
                    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1));
                    dwRegisterValue &= 0xF00F;
                    dwRegisterValue |= ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 4);
                    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) = dwRegisterValue;
                }
                else
                {
                    dwRegisterValue = (hMstarDvfsInfo.dwVidSetting & 0x00FF);
                    if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CPU_POWER_DEFAULT))
                    {
                        MHalDvfsCpuPowerAdjustment(dwRegisterValue);
                    }
                    else
                    {
                        MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
                    }

                    dwRegisterValue = ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 8);
                    if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CORE_POWER_DEFAULT))
                    {
                        MHalDvfsCorePowerAdjustment(dwRegisterValue);
                    }
                    else
                    {
                        MHalDvfsCorePowerAdjustment(CONFIG_DVFS_CORE_POWER_DEFAULT);
                    }
                }

                //low to high
                MHalDvfsCpuClockAdjustment(
                                    hMstarDvfsInfo.wCpuClockSetting_Low,
                                    hMstarDvfsInfo.wCpuClockSetting_High,
                                    DVFS_CPU_CLOCK_UP
                                    );

                DVFS_HAL_INFO("[DVFS] Change to Higher CPU Clock Setting\n");

                hMstarDvfsInfo.bCpuClockLevel = CONFIG_DVFS_STATE_HIGH_SPEED;
                hMstarDvfsInfo.dwOverCounter = 0;
            }

        }
        else
        {
            if(hMstarDvfsInfo.bCpuClockLevel == CONFIG_DVFS_STATE_INIT)
            {
                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x11088a << 1));
                if((dwRegisterValue & 0x0002) != 0)
                {
                    if(hMstarDvfsInfo.dwOverCounter == 0)
                    {
                        DVFS_HAL_DEBUG("[DVFS] Change to Default CPU Clock: %dMHz\n", CONFIG_DVFS_CPU_CLOCK_DEFAULT);
                        MHalDvfsCpuClockAdjustment(
                                            hMstarDvfsInfo.wCpuClockSetting_Low,
                                            hMstarDvfsInfo.wCpuClockSetting_High,
                                            DVFS_CPU_CLOCK_UP
                                            );
                        hMstarDvfsInfo.dwOverCounter ++;
                    }
                }

//              dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x11088a << 1));
//              if((dwRegisterValue & 0x0002) == 0)
                {
                    hMstarDvfsInfo.dwOverCounter ++;
                    //DVFS_HAL_DEBUG("[DVFS] Wait for GPU Ready (%d)\n", (unsigned int) hMstarDvfsInfo.dwOverCounter);
                    //if(hMstarDvfsInfo.dwOverCounter >= 100)
                    {
                        if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                        {
                            //Set VID Level
                            dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1));
                            dwRegisterValue &= 0xF00F;
                            dwRegisterValue |= ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 4);
                            *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) = dwRegisterValue;
                        }
                        else
                        {
                            dwRegisterValue = (hMstarDvfsInfo.dwVidSetting & 0x00FF);
                            if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CPU_POWER_DEFAULT))
                            {
                                MHalDvfsCpuPowerAdjustment(dwRegisterValue);
                            }
                            else
                            {
                                MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
                            }

                            dwRegisterValue = ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 8);
                            if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CORE_POWER_DEFAULT))
                            {
                                MHalDvfsCorePowerAdjustment(dwRegisterValue);
                            }
                            else
                            {
                                MHalDvfsCorePowerAdjustment(CONFIG_DVFS_CORE_POWER_DEFAULT);
                            }
                        }

                        //low to high
                        MHalDvfsCpuClockAdjustment(
                                            hMstarDvfsInfo.wCpuClockSetting_Low,
                                            hMstarDvfsInfo.wCpuClockSetting_High,
                                            DVFS_CPU_CLOCK_UP
                                            );

                        hMstarDvfsInfo.bCpuClockLevel = CONFIG_DVFS_STATE_HIGH_SPEED;
                        hMstarDvfsInfo.dwOverCounter = 0;
                    }
                }
//                else
//                {
//                    hMstarDvfsInfo.dwOverCounter = 0;
//                }
            }
            else
            {
                hMstarDvfsInfo.dwOverCounter = 0;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x10050e << 1));
                if((dwRegisterValue != CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT) || \
                   (hMstarDvfsInfo.dwPresetCpuClock != hMstarDvfsInfo.dwFinalCpuClock))
                {
                    S32  dwSpecificCpuClock = 0;

                    if(dwRegisterValue == CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT)
                    {
                        dwRegisterValue = hMstarDvfsInfo.dwPresetCpuClock;
                    }
                    else
                    {
                        hMstarDvfsInfo.dwPresetCpuClock = dwRegisterValue;
                    }

                    DVFS_HAL_DEBUG("[DVFS] Change to Special CPU Clock: %dMHz\n", (unsigned int) dwRegisterValue);

                    dwSpecificCpuClock = ((2717909 / dwRegisterValue) * 1000);

                    if(dwRegisterValue < CONFIG_DVFS_CPU_CLOCK_MIN)
                    {
                        //low to high
                        MHalDvfsCpuClockAdjustment(
                                            (dwSpecificCpuClock & 0xFFFF),
                                            ((dwSpecificCpuClock >> 16) & 0xFFFF),
                                            DVFS_CPU_CLOCK_UP
                                            );

                        if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                        {
                            //Set VID Level
                            dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1));
                            dwRegisterValue &= 0xF00F;
                            dwRegisterValue |= (((hMstarDvfsInfo.dwVidSetting - 0x1000) & 0xFF00) >> 4);
                            *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) = dwRegisterValue;
                        }
                        else
                        {
                            MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_MIN);
                        }
                    }
                    else
                    {
                        if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                        {
                            //Set VID Level
                            dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1));
                            dwRegisterValue &= 0xF00F;
                            dwRegisterValue |= (((hMstarDvfsInfo.dwVidSetting + 0x1000) & 0xFF00) >> 4);
                            *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) = dwRegisterValue;
                        }
                        else
                        {
                            dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x10050e << 1));
                            if(dwRegisterValue == CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT)
                            {
                               dwRegisterValue = hMstarDvfsInfo.dwPresetCpuClock;
                            }

                            if(dwRegisterValue >= CONFIG_DVFS_CPU_CLOCK_MAX)
                            {
                                dwRegisterValue = (hMstarDvfsInfo.dwVidSetting & 0x00FF);
                                if((dwRegisterValue > 0) && (dwRegisterValue < CONFIG_DVFS_CPU_POWER_DEFAULT))
                                {
                                    MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_MAX_LOWER_BOUND);
                                }
                                else
                                {
                                    MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_MAX_UPPER_BOUND);
                                }
                            }
                            else
                            {
                                dwRegisterValue = (hMstarDvfsInfo.dwVidSetting & 0x00FF);
                                if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CPU_POWER_DEFAULT))
                                {
                                    MHalDvfsCpuPowerAdjustment(dwRegisterValue);
                                }
                                else
                                {
                                    MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
                                }

                                dwRegisterValue = ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 8);
                                if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CORE_POWER_DEFAULT))
                                {
                                    MHalDvfsCorePowerAdjustment(dwRegisterValue);
                                }
                                else
                                {
                                    MHalDvfsCorePowerAdjustment(CONFIG_DVFS_CORE_POWER_DEFAULT);
                                }
                            }
                        }

                        //low to high
                        MHalDvfsCpuClockAdjustment(
                                            (dwSpecificCpuClock & 0xFFFF),
                                            ((dwSpecificCpuClock >> 16) & 0xFFFF),
                                            DVFS_CPU_CLOCK_UP
                                            );
                    }
                    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x10050e << 1)) = CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT;
                }
            }
        }

        //=========================================================
        //Read 25 degree in PM side
        //MHalDvfsRefTemperature();

//        MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
    }
    else
    {
        //Disable DVFS
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb0 << 1)) = 0;
    }

    hMstarDvfsInfo.dwFinalCpuClock = hMstarDvfsInfo.dwPresetCpuClock;

    return hMstarDvfsInfo.dwFinalCpuClock;
}

//=================================================================================================
void MHalDvfsInit(void)
{
//  U32  dwRegisterValue = 0;
//  MHalDvfsCpuPowerInit();
//  MHalDvfsCorePowerInit();

    //Init Basic Register Settings
    //- 0x1005_00[15:0] = 0x3697    //Enable
    //- 0x1005_02[15:0] = 900       //Current CPU Clock
    //- 0x1005_04[15:0] = 25        //Current Temperature (CPU)
    //- 0x1005_06[15:0] = 25        //Current Temperature (PM)
    //- 0x1005_08[15:8] = 130       //Upper Bound of T-sensor
    //- 0x1005_08[7:0] = 120        //Lower Bound of T-sensor
    //- 0x1005_0a[15:0] = 150       //Upper Bound of Global Reset
    //- 0x1005_0c[15:12] = 2        //Current Port
    //- 0x1005_0c[11:8] = 3         //Maximum Port
    //- 0x1005_0c[7:0] = 120        //Current Voltage
    //- 0x1005_0e[15:0] = 0x2454    //Dynamic CPU Clock Adjustment

    //=========================================================
    //SIDD = Bank1[21:12] = 0x00200E[5:0] + 0x00200C[15:12]

    hMstarDvfsInfo.dwVidSetting = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x10050c << 1));

#if 0
    if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0x00FF)
    {
        //Set VID Level
        dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1));
        dwRegisterValue &= 0xF00F;
        dwRegisterValue |= ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 4);
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) = dwRegisterValue;

        //Enable VID
        dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000e38 << 1));
        dwRegisterValue &= ~(0x01 << 10);
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000e38 << 1)) = dwRegisterValue;
    }
    else
    {
        dwRegisterValue = (hMstarDvfsInfo.dwVidSetting & 0x00FF);
        if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CPU_POWER_DEFAULT))
        {
            MHalDvfsCpuPowerAdjustment(dwRegisterValue);
        }
        else
        {
            MHalDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
        }

        dwRegisterValue = ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 8);
        if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CORE_POWER_DEFAULT))
        {
            MHalDvfsCorePowerAdjustment(dwRegisterValue);
        }
        else
        {
            MHalDvfsCorePowerAdjustment(CONFIG_DVFS_CORE_POWER_DEFAULT);
        }
    }
#endif
    //=========================================================
    //Read 25 degree in PM side
    MHalDvfsRefTemperature();

#if CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE
    MHalDvfsCpuDisplayInit();
#endif

    hMstarDvfsInfo.wCpuClockSetting_Low = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110ca4 << 1));
    hMstarDvfsInfo.wCpuClockSetting_High = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110ca6 << 1));

    hMstarDvfsInfo.bCpuClockLevel = CONFIG_DVFS_STATE_INIT;
    hMstarDvfsInfo.bDvfsInitOk = 1;
}

//=================================================================================================
void MHalDvfsCpuDisplayInit(void)
{
    U32  dwRegisterValue = 0;

    //=========================================================
    //Init Test Bus for CPU Clock
    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101896 << 1));
    dwRegisterValue &= ~(0x07);
    dwRegisterValue |= 0x01;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101896 << 1)) = dwRegisterValue;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101eea << 1)) = 0;
    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101eea << 1));
    dwRegisterValue &= ~(0x07);
    dwRegisterValue |= 0x04;
    dwRegisterValue &= ~(0x01 << 4);
    dwRegisterValue &= ~(0x01 << 5);
    dwRegisterValue &= ~(0x01 << 6);
    dwRegisterValue |= (0x01 << 14);
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101eea << 1)) = dwRegisterValue;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101eee << 1)) = 0x001F;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101e62 << 1)) = 0;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101e62 << 1)) = 1;
}

//=================================================================================================
void MHalDvfsCpuDisplay(void)
{
    U32     dwRegisterValue = 0;

    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x101e64 << 1));
    dwRegisterValue *= 12;
    dwRegisterValue /= 1000;
    dwRegisterValue *= 4;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100502 << 1)) = dwRegisterValue;

    DVFS_HAL_DEBUG("[DVFS] Current CPU Clock: %dMHz\n", (unsigned int) dwRegisterValue);
}

//=================================================================================================
void MHalDvfsCpuTemperature(void)
{
    S32     dwCoreChipValue = 0;
    S32     dwChipTempValue = 0;

    //=========================================================
    //Read Chip degree in CPU side
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000e5e << 1)) |= (0x01 << 2);

    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001400 << 1)) = 0x0ab5;         //Channel 5
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) &= ~(0x01 << 8);  //Chip Degree
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) |= (0x01 << 10);  //CPU Side
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) &= ~(0x01 << 9);  //2.0V
    //MsOS_DelayTaskUs(CONFIG_DVFS_DELAY_US);
    mdelay(CONFIG_DVFS_DELAY_US);

    dwCoreChipValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00148a << 1));//Channel 5
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) |= (0x01 << 9);   //3.3V
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001400 << 1)) = 0x0a25;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000e5e << 1)) &= ~(0x01 << 2);

    hMstarDvfsInfo.dwAvgTempBufferCpu[hMstarDvfsInfo.dwAvgTempCounterCpu] = dwCoreChipValue;
    hMstarDvfsInfo.dwAvgTempCounterCpu ++;
    if(hMstarDvfsInfo.dwAvgTempCounterCpu >= CONFIG_DVFS_DATA_COUNT)
    {
        U32    dwTempCounter = 0;
        U32    dwTempValue = 0;

        DVFS_HAL_DEBUG("[DVFS] .................. Start\n");
        DVFS_HAL_DEBUG("[DVFS] 25-degree (CPU): 0x%04x\n", (unsigned int) hMstarDvfsInfo.dwRefTemperature);
        DVFS_HAL_DEBUG("[DVFS] Chip-degree (CPU):");
        for(dwTempCounter = 0; dwTempCounter < CONFIG_DVFS_DATA_COUNT; dwTempCounter ++)
        {
            //DVFS_HAL_DEBUG(" - %d: 0x%04x\n", dwTempCounter, dwAvgTempBufferCpu[dwTempCounter]);
            dwTempValue += hMstarDvfsInfo.dwAvgTempBufferCpu[dwTempCounter];
        }
        dwTempValue /= CONFIG_DVFS_DATA_COUNT;

        DVFS_HAL_DEBUG("0x%04x\n", (unsigned int) dwTempValue);

        //dwChipTempValue = ((((((dwRefTemperature - dwTempValue) * 1000) * 2000) / 1024) / 1.45) + 28000);
        if(hMstarDvfsInfo.dwRefTemperature >= dwTempValue)
        {
            dwChipTempValue = (((hMstarDvfsInfo.dwRefTemperature - dwTempValue) * 1347) + 28000);
        }
        else
        {
            dwChipTempValue = ((dwTempValue - hMstarDvfsInfo.dwRefTemperature) * 1347);
            dwChipTempValue = (28000 - dwChipTempValue);
        }

        hMstarDvfsInfo.dwFinalCpuTemperature = (dwChipTempValue / 1000);
        DVFS_HAL_DEBUG("[DVFS] Average Temperature (CPU): %d\n", (unsigned int) hMstarDvfsInfo.dwFinalCpuTemperature);
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100504 << 1)) = hMstarDvfsInfo.dwFinalCpuTemperature;

        hMstarDvfsInfo.dwAvgTempCounterCpu = 0;

#if CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE
        MHalDvfsCpuDisplay();
#endif

        DVFS_HAL_DEBUG("[DVFS] .................. End\n");
    }
}

//=================================================================================================
void MHalDvfsRefTemperature(void)
{
    //=========================================================
    //Read 25 degree in PM side
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000e5e << 1)) |= (0x01 << 2);

    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001400 << 1)) = 0x0ab7;         //Channel 7
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) |= (0x01 << 8);   //25 degree
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) &= ~(0x01 << 10); //PM Side
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) &= ~(0x01 << 9);  //2.0V
    //MsOS_DelayTaskUs(CONFIG_DVFS_DELAY_US);
    mdelay(CONFIG_DVFS_DELAY_US);

    hMstarDvfsInfo.dwRefTemperature = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00148e << 1));

    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000ec8 << 1)) |= (0x01 << 9);   //3.3V
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001400 << 1)) = 0x0a27;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x000e5e << 1)) &= ~(0x01 << 2);
}

//=================================================================================================
void MHalDvfsCpuClockAdjustment(U32 dwDvfsUpperBoundCpuClock_Low, U32 dwDvfsUpperBoundCpuClock_High, U32 bCpuClockRisingFlag)
{
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110ca4 << 1)) = dwDvfsUpperBoundCpuClock_Low;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110ca6 << 1)) = dwDvfsUpperBoundCpuClock_High;

    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb0 << 1)) = 0x01;   //switch to LPF control
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110caa << 1)) = 0x06;   //mu[2:0]
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cae << 1)) = 0x08;   //lpf_update_cnt[7:0]

    if(bCpuClockRisingFlag == DVFS_CPU_CLOCK_UP)
    {
        //Set LPF is Low to High
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) |= (0x01 << 12);
    }
    else
    {
        //Set LPF is High to Low
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110cb2 << 1)) &= ~(0x01 << 12);
    }

    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110ca8 << 1)) = 0x00;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x110ca8 << 1)) = 0x01;

    hMstarDvfsInfo.dwPresetCpuClock = (2717909000UL / ((dwDvfsUpperBoundCpuClock_High << 16) | dwDvfsUpperBoundCpuClock_Low));
}

//=================================================================================================
void MHalDvfsCpuPowerInit(void)
{
    DVFS_HAL_DEBUG("[DVFS] %s: %d\n", __FUNCTION__, __LINE__);
}

//=================================================================================================
void MHalDvfsCpuPowerAdjustment(U32 dwCpuPowerVoltage)
{
    DVFS_HAL_DEBUG("[DVFS] %s: %d\n", __FUNCTION__, __LINE__);
    MDrvDvfsVoltageSetup(0, dwCpuPowerVoltage, 0);
}

//=================================================================================================
void MHalDvfsCorePowerInit(void)
{
    DVFS_HAL_DEBUG("[DVFS] %s: %d\n", __FUNCTION__, __LINE__);
}

//=================================================================================================
void MHalDvfsCorePowerAdjustment(U32 dwCorePowerVoltage)
{
    DVFS_HAL_DEBUG("[DVFS] %s: %d\n", __FUNCTION__, __LINE__);
    MDrvDvfsVoltageSetup(0, dwCorePowerVoltage, 1);
}

//=================================================================================================
