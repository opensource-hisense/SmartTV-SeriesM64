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

#ifndef __MDRV_DVFS_H__
#define __MDRV_DVFS_H__

#ifndef __MDRV_TYPES_H__
#include "mdrv_types.h"
#endif

#ifndef __MHAL_DVFS_H__
#include "mhal_dvfs.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DVFS_DRV_INFO(x, args...)               //{printk(x, ##args);}
#define DVFS_DRV_DEBUG(x, args...)              //{printk(x, ##args);}

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
U32  MDrvDvfsProc(U32 dwInputCpuClock);
void MDrvDvfsInit(void);
void MDrvDvfsCpuDisplay(void);
void MDrvDvfsCpuPowerAdjustment(U32 dwCpuPowerVoltage);
void MDrvDvfsCorePowerAdjustment(U32 dwCorePowerVoltage);

#endif

