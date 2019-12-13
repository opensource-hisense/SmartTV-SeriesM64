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
#include <linux/module.h>
#include <linux/string.h>
#include <linux/platform_device.h>

#ifndef __MDRV_DVFS_H__
#include "mdrv_dvfs.h"
#endif

DEFINE_MUTEX(MDrvDvfsMutex);

//=================================================================================================
U32 MDrvDvfsProc(U32 dwInputCpuClock)
{
    U32 dwOutputCpuClock = 0;

    mutex_lock(&MDrvDvfsMutex);
    dwOutputCpuClock = MHalDvfsProc(dwInputCpuClock / 1000);
    mutex_unlock(&MDrvDvfsMutex);

    return (dwOutputCpuClock * 1000);
}

//=================================================================================================
void MDrvDvfsInit(void)
{
    MHalDvfsCpuDisplayInit();
    MHalDvfsCpuTemperature();
    MHalDvfsRefTemperature();
    MHalDvfsCpuPowerInit();
    MHalDvfsCorePowerInit();
}

//=================================================================================================
void MDrvDvfsCpuDisplay(void)
{
    MHalDvfsCpuDisplay();
}

//=================================================================================================
void MDrvDvfsCpuPowerAdjustment(U32 dwCpuPowerVoltage)
{
    MHalDvfsCpuPowerAdjustment(dwCpuPowerVoltage);
}

//=================================================================================================
void MDrvDvfsCorePowerAdjustment(U32 dwCorePowerVoltage)
{
    MHalDvfsCorePowerAdjustment(dwCorePowerVoltage);
}

//=================================================================================================
static int MDrvDvfsProbe(struct platform_device *pdev)
{
    int wReturnValue = 0;

    if(!(pdev->name) || \
       strcmp(pdev->name, "Mstar_DVFS") || \
       pdev->id != 0)
    {
        wReturnValue = -ENXIO;
    }

    MDrvDvfsInit();

    return wReturnValue;
}

//=================================================================================================
static int MDrvDvfsRemove(struct platform_device *pdev)
{
    return 0;
}

//=================================================================================================
static int MDrvDvfsSuspend(struct platform_device *dev, pm_message_t state)
{
    return 0;
}

//=================================================================================================
static int MDrvDvfsResume(struct platform_device *dev)
{
    return 0;
}

//=================================================================================================
static struct platform_driver MstarDvfsDriver =
{
    .probe      = MDrvDvfsProbe,
    .remove     = MDrvDvfsRemove,
    .suspend    = MDrvDvfsSuspend,
    .resume     = MDrvDvfsResume,

    .driver =
    {
        .name   = "Mstar_DVFS",
        .owner  = THIS_MODULE,
    }
};

//=================================================================================================
static int __init MDrvDvfsModuleInit(void)
{
    return (int) platform_driver_register(&MstarDvfsDriver);
}

//=================================================================================================
static void __exit MDrvDvfsModuleExit(void)
{
    platform_driver_unregister(&MstarDvfsDriver);
}

//=================================================================================================
module_init(MDrvDvfsModuleInit);
module_exit(MDrvDvfsModuleExit);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("Mstar DVFS Driver");
MODULE_LICENSE("GPL");

//=================================================================================================
