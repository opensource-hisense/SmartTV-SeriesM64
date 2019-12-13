////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <linux/kernel.h>
#include "mdrv_mstypes.h"
#include "reg_sc.h"
#include "mhal_sc.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_SC_GetIntTxLevelAndGWT(MS_U8 u8SCID, HAL_SC_TX_LEVEL_GWT_INT *pstTxLevelGWT)
{    
    //init
    pstTxLevelGWT->bTxLevelInt = FALSE;
    pstTxLevelGWT->bCWT_RxFail = FALSE;
    pstTxLevelGWT->bCWT_TxFail = FALSE;
    pstTxLevelGWT->bCGT_RxFail = FALSE;
    pstTxLevelGWT->bCGT_TxFail = FALSE;
    pstTxLevelGWT->bBGT_Fail = FALSE;
    pstTxLevelGWT->bBWT_Fail = FALSE;

    //no support
    return FALSE;
}

void HAL_SC_ClearIntTxLevelAndGWT(MS_U8 u8SCID)
{
    //no support
}

MS_BOOL HAL_SC_IsPendingINT(MS_U8 u8SCID, MS_U8 IIRReg)
{
    MS_U16 u16DevID=0;
    u16DevID = READ_WORD((HW_REGBASE-0x200000)+REG_PMTOP);
    if (u16DevID == 0x0087) // kratos dev id
    {
        return TRUE;
    }
    else
    {
        if (!(IIRReg & UART_IIR_NO_INT))
        {
            return TRUE;
        }
    }
    return FALSE;
}

MS_U8 HAL_SC_GetLsr(MS_U8 u8SCID)
{
    MS_U8 u8Data;

    u8Data = SC_READ(u8SCID,UART_LSR);

    return u8Data;
}

