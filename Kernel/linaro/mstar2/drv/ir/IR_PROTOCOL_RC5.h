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

#ifndef IR_PROTOCOL_RC5_H
#define IR_PROTOCOL_RC5_H

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define IR_RC5_BITS                          (14)

#define IR_RC5_LEADCODE_SHOT_COUNT         (2)

#define IR_TIME_RC5_LEADCODE_TOLERENCE     (0.1) //shot time tolerence for lead code, should be more strict
#define IR_TIME_RC5_TOLERENCE               (0.15) //shot time tolerence


#define IR_TIME_RC5_FIRST_SHOT              (25000)    // us

#define IR_TIME_RC5_HALF_BIT                (889)    // us
#define IR_TIME_RC5_ONE_BIT                 (1772)    // us

#define IR_TIME_RC5_TIMEOUT                 (114000)
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//BOOL _ParseLeadCodeRc5(U8 *pu8StartIndex);
//IR_DECODE_STATUS _MDrv_ParseRC5(unsigned long long *pu64RawData);

#endif

