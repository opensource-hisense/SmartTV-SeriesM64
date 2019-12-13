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

//<MStar Software>

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "IR_PROTOCOL_INSTALL.h"
#ifdef SUPPORT_MULTI_PROTOCOL
#include "IR_PROTOCOL_NONE.c"
#include "IR_PROTOCOL_NEC.c"
#include "IR_PROTOCOL_RC5.c"
#include "IR_PROTOCOL_OCN.c"

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
DRV_IR_PROTOCOL_TYPE* _GetProtocolEntry(IR_PROCOCOL_TYPE eProtocol)
{
    extern DRV_IR_PROTOCOL_TYPE GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_NONE);
    extern DRV_IR_PROTOCOL_TYPE GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_RC5);
    extern DRV_IR_PROTOCOL_TYPE GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_NEC);
    extern DRV_IR_PROTOCOL_TYPE GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_PZ_OCN);

    switch(eProtocol)
    {
        case E_IR_PROTOCOL_NONE:
            return &GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_NONE);
            break;
        case E_IR_PROTOCOL_NEC:
            return &GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_NEC);
            break;
        case E_IR_PROTOCOL_RC5:
            return &GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_RC5);
            break;
        case E_IR_PROTOCOL_PZ_OCN:
            return &GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_PZ_OCN);
            break;
        default:
            printk("not support protocol\n");
    }

    return &GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_NONE);
}
#endif
