//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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

#define HAL_SAR_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
//#include "MsCommon.h"


// Internal Definition

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>

//#include "mdrv_types.h"
#include "mdrv_mstypes.h"
#include "reg_sar.h"
#include "mhal_sar.h"

#ifdef CONFIG_MSTAR_DVFS_ENABLE
#include "mhal_sar_DVFS.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//#define BIT0    BIT(0)
//#define BIT1    BIT(1)
//#define BIT2    BIT(2)
//#define BIT3    BIT(3)
//#define BIT4    BIT(4)
//#define BIT5    BIT(5)
//#define BIT6    BIT(6)
//#define BIT7    BIT(7)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
struct mutex sar_DVFS_mutex = __MUTEX_INITIALIZER(sar_DVFS_mutex);

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32 _gMIO_MapBase = 0;
static MS_U32 _gMIO_MapBase_NPM = 0;
#ifdef CONFIG_MSTAR_DVFS_ENABLE
static MS_S32 _s32SAR_Dvfs_Mutex;
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define HAL_SAR_ERR(x, args...)        //{printf(x, ##args);}
#define HAL_SAR_INFO(x, args...)        //{printf(x, ##args);}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_SAR_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByteMask
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    u8Val = (u8Val & ~(u8Msk)) | ((u8ValIn) & (u8Msk));
    HAL_SAR_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_ReadRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    return (u8Val & u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadByte_NPM
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_SAR_ReadByte_NPM(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase_NPM))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByte_NPM
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByte_NPM(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase_NPM))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByteMask_NPM
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByteMask_NPM(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte_NPM(u32RegAddr);
    u8Val = (u8Val & ~(u8Msk)) | ((u8ValIn) & (u8Msk));
    HAL_SAR_WriteByte_NPM(u32RegAddr, u8Val);
    return TRUE;
}

#if 0//Temporarily marked out for compiler warning free
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U16 HAL_SAR_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U32 HAL_SAR_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_SAR_Read2Byte(u32RegAddr) | HAL_SAR_Read2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_SAR_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_SAR_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_SAR_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetIOMapBase
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b u32Base : io map base address
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetIOMapBase(MS_U32 u32Base)
{
    _gMIO_MapBase = u32Base;
    HAL_SAR_INFO("SAR IOMap base:%8lx Reg offset:%4x\n", u32Base, SAR_REG_BASE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIOMapBase
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32 : io map base address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_SAR_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgChannelBound
/// @brief \b Function  \b Description: Configure sar channel upper/lower bound
/// @param <IN>         \b u8Channel : sar channel (0~7), psarBndCfg: sar bound info
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_CfgChannelBound(MS_U8 u8Channel ,HAL_SAR_BndCfg *psarBndCfg)
{
    MS_U16  wChannelAdcValue = 0;

    if(u8Channel >= HAL_SAR_CH_MAXID)
        return FALSE;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH1_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH1_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH1_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH1_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH1_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH1_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH2:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH2_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH2_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH2_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH2_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH2_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH2_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH3:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH3_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH3_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH3_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH3_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH3_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH3_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH4:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH4_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH4_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH4_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH4_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH4_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH4_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH5:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH5_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH5_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH5_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH5_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH5_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH5_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH6:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH6_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH6_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH6_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH6_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH6_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH6_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH7:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH7_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH7_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH7_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH7_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH7_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH7_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH8:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH8_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH8_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH8_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH8_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH8_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH8_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetChannelADC
/// @brief \b Function  \b Description: Get sar channel ADC value
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8: sar ADC value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_SAR_GetChannelADC(MS_U8 u8Channel)
{
    MS_U32  u32Reg = REG_SAR_ADC_CH1_DATA;
    MS_U16  wChannelAdcValue = 0;

    if(u8Channel >= HAL_SAR_CH_MAXID)
        return HAL_SAR_ADC_DATA_MAX;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u32Reg = REG_SAR_ADC_CH1_DATA;
            break;
        case HAL_SAR_CH2:
            u32Reg = REG_SAR_ADC_CH2_DATA;
            break;
        case HAL_SAR_CH3:
            u32Reg = REG_SAR_ADC_CH3_DATA;
            break;
        case HAL_SAR_CH4:
            u32Reg = REG_SAR_ADC_CH4_DATA;
            break;
        case HAL_SAR_CH5:
            u32Reg = REG_SAR_ADC_CH5_DATA;
            break;
        case HAL_SAR_CH6:
            u32Reg = REG_SAR_ADC_CH6_DATA;
            break;
        case HAL_SAR_CH7:
            u32Reg = REG_SAR_ADC_CH7_DATA;
            break;
        case HAL_SAR_CH8:
            u32Reg = REG_SAR_ADC_CH8_DATA;
            break;
    }

    wChannelAdcValue = (HAL_SAR_ReadByte(u32Reg + 1) & _SAR_ADC_OUT_MSK);
    wChannelAdcValue <<= 8;
    wChannelAdcValue |= (HAL_SAR_ReadByte(u32Reg) & _SAR_ADC_OUT_MSK);
    wChannelAdcValue = ((wChannelAdcValue >> 2) & _SAR_ADC_OUT_MSK);

#ifdef CONFIG_MSTAR_DVFS_ENABLE
    SysDvfsProc();
#endif

    return (MS_U8) wChannelAdcValue;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSingleChannel
/// @brief \b Function  \b Description: Configure channel for single channel mode
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSingleChannel(MS_U8 u8Channel)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, u8Channel, _SAR_SINGLE_CH_MSK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgTriggerMode
/// @brief \b Function  \b Description: Configure keypad level(trigger) mode
/// @param <IN>         \b bMode : 0: edge trigger mode, 1: level trigger mode
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgTriggerMode(MS_U8 bMode)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bMode)? _SAR_LEVEL_TRIGGER : 0), _SAR_LEVEL_TRIGGER);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSingleChannelEn
/// @brief \b Function  \b Description: enable single channel mode
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSingleChannelEn(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_SINGLE_CH_EN : 0), _SAR_SINGLE_CH_EN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgDigitOperMode
/// @brief \b Function  \b Description: Configure sar digital operation mode
/// @param <IN>         \b u8Mode : 0: one-shot, 1: freerun
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgDigitOperMode(MS_U8 u8Mode)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((u8Mode)? _SAR_MODE : 0), _SAR_MODE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgDigitPowerdown
/// @brief \b Function  \b Description: Configure sar digital power down
/// @param <IN>         \b bEnable : 0: power up, 1: power down
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgDigitPowerdown(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_PD : 0), _SAR_PD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgStart
/// @brief \b Function  \b Description: Configure sar to trigger start signal
/// @param <IN>         \b bEnable : 0: stop, 1: trigger to start
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgStart(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_START : 0), _SAR_START);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAtopPowerdown
/// @brief \b Function  \b Description: Configure sar atop power down
/// @param <IN>         \b bEnable : 0: power up, 1: power down
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAtopPowerdown(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_ADC_PD : 0), _SAR_ADC_PD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAtopFreeRun
/// @brief \b Function  \b Description: Configure sar atop freerun mode
/// @param <IN>         \b bEnable : 0: controlled by digital, 1: freerun
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAtopFreeRun(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_FREERUN : 0), _SAR_FREERUN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSelection
/// @brief \b Function  \b Description: Configure sar selection
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSelection(MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask=_SAR_SEL;

    u8CtrlData = (bEnable)? u8Mask : 0;
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgHighChannel
/// @brief \b Function  \b Description: Configure to use high sar channel ( 4~7, or 4~5)
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgHighChannel(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_NCH_EN : 0), _SAR_NCH_EN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgClockSamplePeriod
/// @brief \b Function  \b Description: Configure sample period
/// @param <IN>         \b u8ClkSmpPrd :
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgClockSamplePeriod(MS_U8 u8ClkSmpPrd)
{
    HAL_SAR_WriteByteMask(REG_SAR_CKSAMP_PRD, u8ClkSmpPrd, _SAR_CKSAMP_PRD_MSK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAanlogInputSelect
/// @brief \b Function  \b Description: Configure Analog Input/GPIO
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bEnable : 0: GPIO, 1: Analog Input
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAanlogInputSelect(MS_U8 u8Channel, MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH1 : (~_SAR_AISEL_CH1);
            u8Mask=_SAR_AISEL_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH2 : (~_SAR_AISEL_CH2);
            u8Mask=_SAR_AISEL_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH3 : (~_SAR_AISEL_CH3);
            u8Mask=_SAR_AISEL_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH4 : (~_SAR_AISEL_CH4);
            u8Mask=_SAR_AISEL_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH5 : (~_SAR_AISEL_CH5);
            u8Mask=_SAR_AISEL_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_AISEL, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgOutputEnable
/// @brief \b Function  \b Description: Configure output enable for sar channel set as GPIO
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bEnable: 0: output, 1: input
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgOutputEnable(MS_U8 u8Channel, MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH1 : _SAR_OEN_GPIO_CH1;
            u8Mask = _SAR_OEN_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH2 : _SAR_OEN_GPIO_CH2;
            u8Mask = _SAR_OEN_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH3 : _SAR_OEN_GPIO_CH3;
            u8Mask = _SAR_OEN_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH4 : _SAR_OEN_GPIO_CH4;
            u8Mask = _SAR_OEN_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH5 : _SAR_OEN_GPIO_CH5;
            u8Mask = _SAR_OEN_GPIO_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_OEN_GPIO, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetOutput
/// @brief \b Function  \b Description: Set GPIO output value(high/low) for sar channel
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bHighLow  0: low, 1: high
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetOutput(MS_U8 u8Channel, MS_BOOL bHighLow)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH1 : ~_SAR_I_GPIO_CH1;
            u8Mask = _SAR_I_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH2 : ~_SAR_I_GPIO_CH2;
            u8Mask = _SAR_I_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH3 : ~_SAR_I_GPIO_CH3;
            u8Mask = _SAR_I_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH4 : ~_SAR_I_GPIO_CH4;
            u8Mask = _SAR_I_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH5 : ~_SAR_I_GPIO_CH5;
            u8Mask = _SAR_I_GPIO_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_I_GPIO, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetInput
/// @brief \b Function  \b Description: Get GPIO input value(high/low) for sar channel
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bHighLow  0: low, 1: high
/// @param <RET>        \b TRUE: High FALSE: Low
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_GetInput(MS_U8 u8Channel)
{
    MS_U8 u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8Mask = _SAR_C_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8Mask = _SAR_C_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8Mask = _SAR_C_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8Mask = _SAR_C_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8Mask = _SAR_C_GPIO_CH5;
            break;
        default: return FALSE;
    }
    return HAL_SAR_ReadRegBit(REG_SAR_C_GPIO, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntMask
/// @brief \b Function  \b Description: Interrupt mask for sar int
/// @param <IN>         \b bEnable : 0: enable int, 1: disable int
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntMask(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_MASK : 0), _SAR_INT_MASK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntClear
/// @brief \b Function  \b Description: Interrupt clear for sar int
/// @param <IN>         \b bEnable : 0: not clear, 1: clear
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntClear(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_CLR : 0), _SAR_INT_CLR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntForce
/// @brief \b Function  \b Description: Force interrupt for sar int
/// @param <IN>         \b bEnable : 0: not force, 1: force
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntForce(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_FORCE : 0), _SAR_INT_FORCE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIntStatus
/// @brief \b Function  \b Description: get sar interrupt status
/// @param <IN>         \b None : 0: not force, 1: force
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: interrupt comes, FALSE: no interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_GetIntStatus(void)
{
    MS_U8 u8IntStatus;

    u8IntStatus = HAL_SAR_ReadByte(REG_SAR_STATUS0) & _SAR_INT_STATUS;
    return (u8IntStatus)? ENABLE : DISABLE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetIOMapBase_NPM
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b u32Base : io map base address
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetIOMapBase_NPM(MS_U32 u32Base)
{
    _gMIO_MapBase_NPM = u32Base;
    HAL_SAR_INFO("SAR NPM IOMap base:%8lx Reg offset:%4x\n", u32Base);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIOMapBase_NPM
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32 : io map base address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_SAR_GetIOMapBase_NPM(void)
{
    return _gMIO_MapBase_NPM;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetAdcHSyncChannel
/// @brief \b Function  \b Description: Switch HSync signal to SAR High channel
/// @param <IN>         \b u8Channel : ADC HSync channel (0~2)
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_SetAdcHSyncChannel(MS_U8 u8Channel)
{
    MS_U8 u8HSyncCh;

    switch(u8Channel)
    {
        case HAL_SAR_ADC_HSYNC_CH0:
            u8HSyncCh = _SAR2_HSYNC_CH0;
            break;
        case HAL_SAR_ADC_HSYNC_CH1:
            u8HSyncCh = _SAR2_HSYNC_CH1;
            break;
        case HAL_SAR_ADC_HSYNC_CH2:
            u8HSyncCh = _SAR2_HSYNC_CH2;
            break;
        case HAL_SAR_ADC_HSYNC_CH3:
            u8HSyncCh = _SAR2_HSYNC_CH3;
            break;
        default: return FALSE;
    }
    return HAL_SAR_WriteByteMask_NPM(REG_ADC_ATOP_SAR2, u8HSyncCh, _SAR2_HSYNC_MSK);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Interrupt_EN
/// @brief \b Function  \b Description: Switch SAR Interrupt Enable/Disable
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <IN>         \b bEnablel : True: enable interrupt; False: disable interrupt
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_Interrupt_EN(MS_U8 u8Channel, MS_BOOL bEnable)
{
    return HAL_SAR_WriteByteMask(REG_SAR_INT, ((bEnable)? 1<<u8Channel : 0), 1<<u8Channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Wakeup_EN
/// @brief \b Function  \b Description: Switch SAR Interrupt  Wakeup Enable/Disable
/// @param <IN>         \b bEnablel : True: enable interrrupt wakeup; False: disable interrupt wakeup
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_Wakeup_EN(MS_BOOL bEnable)
{
    return HAL_SAR_WriteByteMask(REG_WK_IRQ, ((bEnable)? _W_SAR : 0), _W_SAR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CLR_INT
/// @brief \b Function  \b Description: Switch SAR Clear Interrupt Status
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <OUT>        \b None :
/// @param <RET>        \b  None
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CLR_INT(MS_U8 u8Channel)
{
    HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, 1<<u8Channel, 1<<u8Channel);
    HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, 0, 1<<u8Channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_INT_Status
/// @brief \b Function  \b Description: get sar interrupt status
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: interrupt comes, FALSE: no interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_INT_Status(MS_U8 u8Channel)
{
    return (HAL_SAR_ReadByte(REG_SAR_INT_STATUS) & (1<<u8Channel)) > 0? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SET_LEVEL
/// @brief \b Function  \b Description: set sar level is 2.0V or 3.3V
/// @param <IN>         \b bLevel : 1:3.3V , 0:2.0V
/// @param <OUT>        \b None :
/// @param <RET>        \b Nonw:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SET_LEVEL(MS_BOOL bLevel)
{
    HAL_SAR_WriteByteMask(REG_SAR_TEST0, (bLevel? 1<<6:0) , 1<<6);
}

//=================================================================================================
#ifdef CONFIG_MSTAR_DVFS_ENABLE

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
};

//extern MS_BOOL MApi_SWI2C_SetBusReadMode(MS_U8 u8BusNum, MS_U8 eReadMode);
extern MS_BOOL MApi_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
extern MS_BOOL MApi_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsProc
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsProc(void)
{
    MS_U32  dwRegisterValue = 0;

    dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x100500 << 1));
    if(dwRegisterValue == CONFIG_DVFS_ENABLE_PATTERN)
    {
        MS_S32  dwUpperTemperature = 0;
        MS_S32  dwLowerTemperature = 0;
        MS_S32  dwResetTemperature = 0;

        if(hMstarDvfsInfo.bDvfsInitOk == 0)
        {
            mutex_lock(&sar_DVFS_mutex);
            
        }

       /* if (FALSE == MsOS_ObtainMutex(_s32SAR_Dvfs_Mutex, CONFIG_DVFS_MUTEX_WAIT_TIME))
        {
            DVFS_INFO("[DVFS] Mutex Lock Fail\n");
            return;
        } */

        if(hMstarDvfsInfo.bDvfsInitOk == 0)
        {
            SysDvfsInit();
        }

        SysDvfsCpuTemperature();

        //=========================================================
        dwUpperTemperature = *(volatile MS_U16 *)(_gMIO_MapBase + (0x100508 << 1));
        dwUpperTemperature >>= 8;
        if(dwUpperTemperature == 0)
        {
            dwUpperTemperature = CONFIG_DVFS_DEFAULT_UPPER_BOUND;
        }

        dwLowerTemperature = *(volatile MS_U16 *)(_gMIO_MapBase + (0x100508 << 1));
        dwLowerTemperature &= 0xFF;
        if(dwLowerTemperature == 0)
        {
            dwLowerTemperature = CONFIG_DVFS_DEFAULT_LOWER_BOUND;
        }

        dwResetTemperature = *(volatile MS_U16 *)(_gMIO_MapBase + (0x10050a << 1));
        dwResetTemperature &= 0xFF;
        if(dwResetTemperature == 0)
        {
            dwResetTemperature = CONFIG_DVFS_DEFAULT_RESET_THRESHOLD;
        }

        if((hMstarDvfsInfo.dwFinalCpuTemperature > dwResetTemperature) && (hMstarDvfsInfo.bCpuClockLevel != CONFIG_DVFS_STATE_INIT))
        {
            if(hMstarDvfsInfo.dwOverCounter < CONFIG_DVFS_AVERAGE_COUNT)
            {
                DVFS_DEBUG("[DVFS] Over Temperature Protection: %d\n", (unsigned int) hMstarDvfsInfo.dwOverCounter);
                hMstarDvfsInfo.dwOverCounter ++;
            }
            else
            {
                //Trigger a WDT Reset
                DVFS_INFO("[DVFS] Over Temperature Protection\n");
                *(volatile MS_U16 *)(_gMIO_MapBase + (0x00300a << 1)) = 0x00;
                *(volatile MS_U16 *)(_gMIO_MapBase + (0x003008 << 1)) = 0x00;
                *(volatile MS_U16 *)(_gMIO_MapBase + (0x00300a << 1)) = 0x05;
                *(volatile MS_U16 *)(_gMIO_MapBase + (0x003000 << 1)) = 0x01;

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
                DVFS_INFO("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo.dwFinalCpuTemperature);
                DVFS_INFO("[DVFS] Change to Lower CPU Clock Setting\n");

                //high to low
                SysDvfsCpuClockAdjustment(
                                    hMstarDvfsInfo.wCpuClockSetting_Low,
                                    hMstarDvfsInfo.wCpuClockSetting_High,
                                    DVFS_CPU_CLOCK_DOWN
                                    );

                if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                {
                    //Set VID Level
                    dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1));
                    dwRegisterValue &= 0xF00F;
                    dwRegisterValue |= (((hMstarDvfsInfo.dwVidSetting - 0x1000) & 0xFF00) >> 4);    //Dowbgrade Voltage Level
                    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) = dwRegisterValue;
                }
                else
                {
                    if(((hMstarDvfsInfo.dwVidSetting & 0xFF00) != 0) & ((hMstarDvfsInfo.dwVidSetting & 0x00FF) != 0))
                    {
                        SysDvfsCpuPowerAdjustment((hMstarDvfsInfo.dwVidSetting & 0x00FF) - CONFIG_DVFS_CPU_POWER_DROP);
                    }
                    else
                    {
                        SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT - CONFIG_DVFS_CPU_POWER_DROP);
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
                DVFS_INFO("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo.dwFinalCpuTemperature);

                if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                {
                    //Set VID Level
                    dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1));
                    dwRegisterValue &= 0xF00F;
                    dwRegisterValue |= ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 4);
                    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) = dwRegisterValue;
                }
                else
                {
                    if(((hMstarDvfsInfo.dwVidSetting & 0xFF00) != 0) & ((hMstarDvfsInfo.dwVidSetting & 0x00FF) != 0))
                    {
                        SysDvfsCpuPowerAdjustment(hMstarDvfsInfo.dwVidSetting & 0x00FF);
                    }
                    else
                    {
                        SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
                    }
                }

                //low to high
                SysDvfsCpuClockAdjustment(
                                    hMstarDvfsInfo.wCpuClockSetting_Low,
                                    hMstarDvfsInfo.wCpuClockSetting_High,
                                    DVFS_CPU_CLOCK_UP
                                    );

                DVFS_INFO("[DVFS] Change to Higher CPU Clock Setting\n");

                hMstarDvfsInfo.bCpuClockLevel = CONFIG_DVFS_STATE_HIGH_SPEED;
                hMstarDvfsInfo.dwOverCounter = 0;
            }

        }
        else
        {
            if(hMstarDvfsInfo.bCpuClockLevel == CONFIG_DVFS_STATE_INIT)
            {

////            dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x11088a << 1));
////            if((dwRegisterValue & 0x0002) == 0)
                {
                    hMstarDvfsInfo.dwOverCounter ++;
                    DVFS_DEBUG("[DVFS] Wait for GPU Ready (%d)\n", (unsigned int) hMstarDvfsInfo.dwOverCounter);
                    if(hMstarDvfsInfo.dwOverCounter >= 100)
                    {
                        if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                        {
                            //Set VID Level
                            dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1));
                            dwRegisterValue &= 0xF00F;
                            dwRegisterValue |= ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 4);
                            *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) = dwRegisterValue;
                        }
                        else
                        {
                            if(((hMstarDvfsInfo.dwVidSetting & 0xFF00) != 0) & ((hMstarDvfsInfo.dwVidSetting & 0x00FF) != 0))
                            {
                                SysDvfsCpuPowerAdjustment(hMstarDvfsInfo.dwVidSetting & 0x00FF);
                            }
                            else
                            {
                                SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
                            }
                        }

                        //low to high
                        SysDvfsCpuClockAdjustment(
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

                dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x10050e << 1));
                if(dwRegisterValue != CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT)
                {
                    MS_S32  dwSpecificCpuClock = 0;

                    DVFS_DEBUG("[DVFS] Change to Special CPU Clock: %dMHz\n", (unsigned int) dwRegisterValue);

                    dwSpecificCpuClock = ((2717909 / dwRegisterValue) * 1000);

                    if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0xFF)
                    {
                        //Set VID Level
                        dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1));
                        dwRegisterValue &= 0xF00F;
                        dwRegisterValue |= (((hMstarDvfsInfo.dwVidSetting + 0x1000) & 0xFF00) >> 4);
                        *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) = dwRegisterValue;
                    }
                    else
                    {
                        dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x10050e << 1));
                        if(dwRegisterValue >= CONFIG_DVFS_CPU_CLOCK_MAX)
                        {
                            if(((hMstarDvfsInfo.dwVidSetting & 0xFF00) != 0) & ((hMstarDvfsInfo.dwVidSetting & 0x00FF) != 0))
                            {
                                SysDvfsCpuPowerAdjustment((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 8);
                            }
                            else
                            {
                                SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_MAX);
                            }
                        }
                        else
                        {
                            if(((hMstarDvfsInfo.dwVidSetting & 0xFF00) != 0) & ((hMstarDvfsInfo.dwVidSetting & 0x00FF) != 0))
                            {
                                SysDvfsCpuPowerAdjustment(hMstarDvfsInfo.dwVidSetting & 0x00FF);
                            }
                            else
                            {
                                SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
                            }
                        }
                    }

                    //low to high
                    SysDvfsCpuClockAdjustment(
                                        (dwSpecificCpuClock & 0xFFFF),
                                        ((dwSpecificCpuClock >> 16) & 0xFFFF),
                                        DVFS_CPU_CLOCK_UP
                                        );

                    *(volatile MS_U16 *)(_gMIO_MapBase + (0x10050e << 1)) = CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT;
                }
            }
        }

        //=========================================================
        //Read 25 degree in PM side
        SysDvfsRefTemperature();

        mutex_unlock(&sar_DVFS_mutex);
    }
    else
    {
        //Disable DVFS
        *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb0 << 1)) = 0;
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsInit
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsInit(void)
{
    MS_U32  dwRegisterValue = 0;

    SysDvfsCpuPowerInit();

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

    hMstarDvfsInfo.dwVidSetting = *(volatile MS_U16 *)(_gMIO_MapBase + (0x10050c << 1));
    if((hMstarDvfsInfo.dwVidSetting & 0x00FF) == 0x00FF)
    {
        //Set VID Level
        dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1));
        dwRegisterValue &= 0xF00F;
        dwRegisterValue |= ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 4);
        *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) = dwRegisterValue;

        //Enable VID
        dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x000e38 << 1));
        dwRegisterValue &= ~(0x01 << 10);
        *(volatile MS_U16 *)(_gMIO_MapBase + (0x000e38 << 1)) = dwRegisterValue;
    }
    else
    {
        if(((hMstarDvfsInfo.dwVidSetting & 0xFF00) != 0) & ((hMstarDvfsInfo.dwVidSetting & 0x00FF) != 0))
        {
            SysDvfsCpuPowerAdjustment(hMstarDvfsInfo.dwVidSetting & 0x00FF);
        }
        else
        {
            SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
        }
    }

    //=========================================================
    //Read 25 degree in PM side
    SysDvfsRefTemperature();

#if CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE
    SysDvfsCpuDisplayInit();
#endif

    hMstarDvfsInfo.wCpuClockSetting_Low = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110ca4 << 1));
    hMstarDvfsInfo.wCpuClockSetting_High = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110ca6 << 1));

    hMstarDvfsInfo.bCpuClockLevel = CONFIG_DVFS_STATE_INIT;
    hMstarDvfsInfo.bDvfsInitOk = 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuDisplayInit
/// @brief \b Function  \b Description: Init Test Bus for CPU Clock Counter
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuDisplayInit(void)
{
    MS_U32  dwRegisterValue = 0;

    //=========================================================
    //Init Test Bus for CPU Clock
    dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x101896 << 1));
    dwRegisterValue &= ~(0x07);
    dwRegisterValue |= 0x01;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x101896 << 1)) = dwRegisterValue;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x101eea << 1)) = 0;
    dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x101eea << 1));
    dwRegisterValue &= ~(0x07);
    dwRegisterValue |= 0x04;
    dwRegisterValue &= ~(0x01 << 4);
    dwRegisterValue &= ~(0x01 << 5);
    dwRegisterValue &= ~(0x01 << 6);
    dwRegisterValue |= (0x01 << 14);
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x101eea << 1)) = dwRegisterValue;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x101eee << 1)) = 0x001F;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x101e62 << 1)) = 0;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x101e62 << 1)) = 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuDisplay
/// @brief \b Function  \b Description: Get Value of CPU Clock Counter
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuDisplay(void)
{
    MS_U32  dwRegisterValue = 0;

    dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x101e64 << 1));
    dwRegisterValue *= 12;
    dwRegisterValue /= 1000;
    dwRegisterValue *= 4;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x100502 << 1)) = dwRegisterValue;

    DVFS_DEBUG("[DVFS] Current CPU Clock: %dMHz\n", (unsigned int) dwRegisterValue);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuTemperature
/// @brief \b Function  \b Description: Get Temperature by T-Sensor in CPU side
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuTemperature(void)
{
    MS_S32  dwCoreChipValue = 0;
    MS_S32  dwChipTempValue = 0;

    //=========================================================
    //Read Chip degree in CPU side
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000e5e << 1)) |= (0x01 << 2);

    *(volatile MS_U16 *)(_gMIO_MapBase + (0x001400 << 1)) = 0x0ab5;         //Channel 5
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) &= ~(0x01 << 8);  //Chip Degree
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) |= (0x01 << 10);  //CPU Side
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) &= ~(0x01 << 9);  //2.0V
    MsOS_DelayTaskUs(CONFIG_DVFS_DELAY_US);

    dwCoreChipValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x00148a << 1));//Channel 5
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) |= (0x01 << 9);   //3.3V
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x001400 << 1)) = 0x0a25;

    hMstarDvfsInfo.dwAvgTempBufferCpu[hMstarDvfsInfo.dwAvgTempCounterCpu] = dwCoreChipValue;
    hMstarDvfsInfo.dwAvgTempCounterCpu ++;
    if(hMstarDvfsInfo.dwAvgTempCounterCpu >= CONFIG_DVFS_AVERAGE_COUNT)
    {
        MS_U32    dwTempCounter = 0;
        MS_U32    dwTempValue = 0;

        DVFS_DEBUG("[DVFS] .................. Start\n");
        DVFS_DEBUG("[DVFS] 25-degree (CPU): 0x%04x\n", (unsigned int) hMstarDvfsInfo.dwRefTemperature);
        DVFS_DEBUG("[DVFS] Chip-degree (CPU):");
        for(dwTempCounter = 0; dwTempCounter < CONFIG_DVFS_AVERAGE_COUNT; dwTempCounter ++)
        {
            //DVFS_DEBUG(" - %d: 0x%04x\n", dwTempCounter, dwAvgTempBufferCpu[dwTempCounter]);
            dwTempValue += hMstarDvfsInfo.dwAvgTempBufferCpu[dwTempCounter];
        }
        dwTempValue /= CONFIG_DVFS_AVERAGE_COUNT;

        DVFS_DEBUG("0x%04x\n", (unsigned int) dwTempValue);

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
        DVFS_DEBUG("[DVFS] Average Temperature (CPU): %d\n", (unsigned int) hMstarDvfsInfo.dwFinalCpuTemperature);
        *(volatile MS_U16 *)(_gMIO_MapBase + (0x100504 << 1)) = hMstarDvfsInfo.dwFinalCpuTemperature;

        hMstarDvfsInfo.dwAvgTempCounterCpu = 0;

#if CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE
        SysDvfsCpuDisplay();
#endif

        DVFS_DEBUG("[DVFS] .................. End\n");
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsRefTemperature
/// @brief \b Function  \b Description: Get Reference Temperature by T-Sensor
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsRefTemperature(void)
{
    //=========================================================
    //Read 25 degree in PM side
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000e5e << 1)) |= (0x01 << 2);

    *(volatile MS_U16 *)(_gMIO_MapBase + (0x001400 << 1)) = 0x0ab7;         //Channel 7
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) |= (0x01 << 8);   //25 degree
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) &= ~(0x01 << 10); //PM Side
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) &= ~(0x01 << 9);  //2.0V
    MsOS_DelayTaskUs(CONFIG_DVFS_DELAY_US);

    hMstarDvfsInfo.dwRefTemperature = *(volatile MS_U16 *)(_gMIO_MapBase + (0x00148e << 1));

    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000ec8 << 1)) |= (0x01 << 9);   //3.3V
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x001400 << 1)) = 0x0a27;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsRefTemperature
/// @brief \b Function  \b Description: Get Reference Temperature by T-Sensor
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuClockAdjustment(MS_U32 dwDvfsUpperBoundCpuClock_Low, MS_U32 dwDvfsUpperBoundCpuClock_High, MS_U32 bCpuClockRisingFlag)
{
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110ca4 << 1)) = dwDvfsUpperBoundCpuClock_Low;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110ca6 << 1)) = dwDvfsUpperBoundCpuClock_High;

    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb0 << 1)) = 0x01;   //switch to LPF control
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110caa << 1)) = 0x06;   //mu[2:0]
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cae << 1)) = 0x08;   //lpf_update_cnt[7:0]

    if(bCpuClockRisingFlag == DVFS_CPU_CLOCK_UP)
    {
        //Set LPF is Low to High
        *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) |= (0x01 << 12);
    }
    else
    {
        //Set LPF is High to Low
        *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) &= ~(0x01 << 12);
    }

    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110ca8 << 1)) = 0x00;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110ca8 << 1)) = 0x01;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuPowerAdjustment
/// @brief \b Function  \b Description: Update Output Voltage Level in External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuPowerAdjustment(MS_U32 dwCpuPowerVoltage)
{
    MS_U32    dwRegisterValue = 0;

    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

    byTargetRegAddress[0] = 0x10;
    byTargetRegAddress[1] = (0x06 << 1);
    byTargetRegAddress[2] = 0x00;

    dwRegisterValue = (dwCpuPowerVoltage - CONFIG_DVFS_CPU_POWER_SHIFT);
    dwRegisterValue = (((dwRegisterValue * 1000000) / 1.07) / 1000000);
    byTargetRegAddress[3] = dwRegisterValue;

    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 4, byTargetRegAddress, 0, byTargetData) == TRUE)
    {
        DVFS_INFO("[INFO] Change to Voltage: %d mv (0x%X)\n", (unsigned int) dwCpuPowerVoltage, (unsigned int) dwRegisterValue);
    }
    else
    {
        DVFS_INFO("[ERROR] Software I2C Write Failed\n");
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuPowerInit
/// @brief \b Function  \b Description: The Init Flow of  External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuPowerInit(void)
{
    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

//  MApi_SWI2C_SetBusReadMode(CONFIG_DVFS_POWER_SWI2C_BUS, 2);

    byTargetRegAddress[0] = 0x53;
    byTargetRegAddress[1] = 0x45;
    byTargetRegAddress[2] = 0x52;
    byTargetRegAddress[3] = 0x44;
    byTargetRegAddress[4] = 0x42;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 5, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("[ERROR] I2C_Enter_I2C Failed\n");
    }

    byTargetRegAddress[0] = 0x7F;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("[ERROR] I2C_USE_CFG Failed\n");
    }

    byTargetRegAddress[0] = 0x7D;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("[ERROR] I2C_OUT_NO_DELAY Failed\n");
    }

    byTargetRegAddress[0] = 0x50;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("[ERROR] I2C_AD_BYTE_EN0 Failed\n");
    }

    byTargetRegAddress[0] = 0x55;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("[ERROR] I2C_DA_BYTE_EN1 Failed\n");
    }

    byTargetRegAddress[0] = 0x35;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("[ERROR] I2C_USE Failed\n");
    }

    byTargetRegAddress[0] = 0x10;
    byTargetRegAddress[1] = 0xc0;
    if(MApi_SWI2C_ReadBytes(CONFIG_DVFS_POWER_SWI2C_ADDRESS, 2, byTargetRegAddress, 2, byTargetData) == TRUE)
    {
        DVFS_DEBUG("[INFO] MStar Power IC Chip ID: %x%x\n", (unsigned int) byTargetData[0], (unsigned int) byTargetData[1]);
    }
}

#endif


