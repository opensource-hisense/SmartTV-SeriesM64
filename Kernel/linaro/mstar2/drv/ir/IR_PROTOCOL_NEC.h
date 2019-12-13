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

#ifndef IR_PROTOCOL_NEC_H
#define IR_PROTOCOL_NEC_H

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define IR_NEC_BITS                        (32)

#define IR_NEC_LEADCODE_SHOT_COUNT        (3)
#define IR_NEC_REPEAT_SHOT_COUNT          (4)

#define IR_TIME_NEC_LEADCODE_TOLERENCE       (0.05) //shot time tolerence for lead code, should be more strict
#define IR_TIME_NEC_TOLERENCE                 (0.2) //shot time tolerence


#define IR_TIME_NEC_TIMEOUT                  (110000)    // us

#define IR_TIME_NEC_HEADER_CODE             (9000)    // us
#define IR_TIME_NEC_HEADER_OFF_CODE         (4500)    // us
#define IR_TIME_NEC_LOGI_0                   (1120)    // us
#define IR_TIME_NEC_LOGI_1                   (2250)    // us

#define IR_TIME_NEC_REPEAT_DELAY             (24500)    // us
#define IR_TIME_NEC_REPEAT_SHOT_1            (9000)    // us
#define IR_TIME_NEC_REPEAT_SHOT_2            (2250)    // us
#define IR_TIME_NEC_REPEAT_SHOT_3            (560)    // us

#endif

