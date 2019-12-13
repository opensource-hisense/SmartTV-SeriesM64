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
/// @file   Mdrvl_miu.c
/// @brief  MIU Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "mdrv_miu.h"
#include "mhal_miu.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_Init
/// @brief \b Function  \b Description: parse occupied resource to software structure
/// @param None         \b IN :
/// @param None         \b OUT :
/// @param MS_BOOL      \b RET
/// @param None         \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_Init(void)
{
#if defined(CONFIG_MSTAR_CLIPPERS) || defined(CONFIG_MSTAR_MONACO) || defined(CONFIG_MSTAR_MUJI) || \
    defined(CONFIG_MSTAR_MONET) || defined(CONFIG_MSTAR_NUGGET)
    return HAL_MIU_ParseOccupiedResource();
#else
    return FALSE;
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_GetDefaultClientID_KernelProtect()
/// @brief \b Function \b Description:  Get default client id array pointer for protect kernel
/// @param <RET>           \b     : The pointer of Array of client IDs
////////////////////////////////////////////////////////////////////////////////
MS_U8* MDrv_MIU_GetDefaultClientID_KernelProtect(void)
{
#if defined(CONFIG_MSTAR_CLIPPERS) || defined(CONFIG_MSTAR_MONACO) || defined(CONFIG_MSTAR_MUJI) || \
    defined(CONFIG_MSTAR_MONET) || defined(CONFIG_MSTAR_NUGGET)
    return HAL_MIU_GetDefaultClientID_KernelProtect();
#else
    return NULL;
#endif

 
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_Protect()
/// @brief \b Function \b Description:  Enable/Disable MIU Protection mode
/// @param u8Blockx        \b IN     : MIU Block to protect (0 ~ 3)
/// @param *pu8ProtectId   \b IN     : Allow specified client IDs to write
/// @param u32Start        \b IN     : Starting address(bus address)
/// @param u32End          \b IN     : End address(bus address)
/// @param bSetFlag        \b IN     : Disable or Enable MIU protection
///                                      - -Disable(0)
///                                      - -Enable(1)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_Protect(
                          MS_U8    u8Blockx,
                          MS_U8    *pu8ProtectId,
                          MS_U32   u32BusStart,
                          MS_U32   u32BusEnd,
                          MS_BOOL  bSetFlag
                         )
{
    MS_BOOL Result;

#if defined(CONFIG_MSTAR_CLIPPERS) || defined(CONFIG_MSTAR_MONACO) || defined(CONFIG_MSTAR_MUJI) || \
    defined(CONFIG_MSTAR_MONET) || defined(CONFIG_MSTAR_NUGGET)
    Result = HAL_MIU_Protect(u8Blockx, pu8ProtectId, u32BusStart, u32BusEnd, bSetFlag);
#else
    return FALSE;
#endif

    if(Result == TRUE)
    {
        return TRUE;
    }
    else
    {       
        return FALSE;
    }
}


