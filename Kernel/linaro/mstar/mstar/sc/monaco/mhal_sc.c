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

MS_U8 HAL_SC_GetLsr(MS_U8 u8SCID)
{
    MS_U8 u8Data;

    // Since some bitfield of LSR is "read clear", and we found these bitfield data will be cleared before RIU read back for some chips (ex: kirin)
    // So we need to enable LCR DLAB to make these bitfield can not be cleared based on HW design
    SC_OR(u8SCID,UART_LCR, UART_LCR_DLAB);                                     
    u8Data = SC_READ(u8SCID,UART_LSR);
    SC_AND(u8SCID,UART_LCR, ~(UART_LCR_DLAB)); 

    _HAL_SC_ClearLsr(u8SCID);

    return u8Data;
}

