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

#ifndef IR_PROTOCOL_OCN_H
#define IR_PROTOCOL_OCN_H

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define IR_OCN_CUSTOM_BITS                (24)
#define IR_OCN_KEYCODE_BITS               (16)
#define IR_OCN_BITS                        (IR_OCN_CUSTOM_BITS+IR_OCN_KEYCODE_BITS) //customer: 24 + Keycode:16

#define IR_OCN_LEADCODE_SHOT_COUNT        (3)
#define IR_OCN_REPEAT_SHOT_COUNT          (4)

#define IR_TIME_OCN_LEADCODE_TOLERENCE       (0.05) //shot time tolerence for lead code, should be more strict
#define IR_TIME_OCN_TOLERENCE                 (0.2)   //shot time tolerence


#define IR_TIME_OCN_TIMEOUT                  (108000)    // us

#define IR_TIME_OCN_HEADER_CODE             (3640)    // us
#define IR_TIME_OCN_HEADER_OFF_CODE         (1800)    // us
#define IR_TIME_OCN_LOGI_0                   (1120)    // us
#define IR_TIME_OCN_LOGI_1                   (1680)    // us

#define IR_TIME_OCN_REPEAT_DELAY            (54000)    // us
#define IR_TIME_OCN_REPEAT_SHOT_1           (3640)
#define IR_TIME_OCN_REPEAT_SHOT_2           (3640)
#define IR_TIME_OCN_REPEAT_SHOT_3           (560)

#endif

