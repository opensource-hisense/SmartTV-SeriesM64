////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_utopia2k_str_io.h
/// @brief  UTOPIA2K STR interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_UTOPIA2K_STR_IO_H__
#define __DRV_UTOPIA2K_STR_IO_H__

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

#include "MsTypes.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

enum utopia2k_str_power_mode {
    UTOPIA2K_STR_POWER_SUSPEND = 1,
    UTOPIA2K_STR_POWER_RESUME = 2,
    UTOPIA2K_STR_POWER_MAX,
};

typedef int (*FUtopiaSTR)(int u32PowerState, void* pModule);

////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

int mdrv_utopia2k_str_setup_function_ptr(void* pModuleTmp, FUtopiaSTR fpSTR);
int mdrv_utopia2k_str_wait_condition(const char* name, MS_U32 mode, MS_U32 stage);
int mdrv_utopia2k_str_send_condition(const char* name, MS_U32 mode, MS_U32 stage);

#endif

