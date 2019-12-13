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

#ifndef IR_PROTOCOL_PANASONIC7051_H
#define IR_PROTOCOL_PANASONIC7051_H

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define IR_PANASONIC7051_CUSTOM_BITS                (32)
#define IR_PANASONIC7051_KEYCODE_BITS               (16)
#define IR_PANASONIC7051_BITS                       (IR_PANASONIC7051_CUSTOM_BITS+IR_PANASONIC7051_KEYCODE_BITS) //customer: 32 + Keycode:16

#define IR_PANASONIC7051_LEADCODE_SHOT_COUNT        (3)

#define IR_TIME_PANASONIC7051_LEADCODE_TOLERENCE    (0.1) //shot time tolerence for lead code, should be more strict
#define IR_TIME_PANASONIC7051_TOLERENCE             (0.2) //shot time tolerence


#define IR_TIME_PANASONIC7051_TIMEOUT               (113000)    // us

#define IR_TIME_PANASONIC7051_HEADER_CODE           (3640)    // us
#define IR_TIME_PANASONIC7051_HEADER_OFF_CODE       (1800)    // us
#define IR_TIME_PANASONIC7051_LOGI_0                (760)    // us
#define IR_TIME_PANASONIC7051_LOGI_1                (1730)    // us

#endif
