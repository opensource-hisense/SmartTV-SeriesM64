////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef IR_FORMAT_H
#define IR_FORMAT_H



//*************************************************************************
// Customer IR Specification parameter define (Please modify them by IR SPEC)
//*************************************************************************
#define IR_MODE_SEL              IR_TYPE_RAWDATA_MODE  // IR_TYPE_SWDECODE_MODE  //  IR_TYPE_RAWDATA_MODE   //IR_TYPE_RAWDATA_MODE    //IR_TYPE_SWDECODE_MODE //IR_TYPE_SWDECODE_MODE   //IR_TYPE_FULLDECODE_MODE
#define IR_FACtORY_SINGLE_KEY_CODE      1
#define IR_CHANNEL_USE_AS_UPDOWN        0
#define IR_VOLUME_USE_AS_LEFTRIGHT      0

#define IR_MODEL_TL20E                  0   // 0:Disable 1:Enable

// IR Header code define
#define IR_HEADER_CODE0                 0x40    // Custom 0
#define IR_HEADER_CODE1_0                 0xBF    // Custom 1
#define IR_HEADER_CODE1_1                 0xBE    // Custom 1


#define IR_FHEADER_CODE0_0              0x40    // Factory 0 //for compal
#define IR_FHEADER_CODE0_1              0x51    // Factory 0 //for compal

#define IR_FHEADER_CODE0_2              0xD0    // Factory 0


#define IR_FHEADER_CODE1_0              0xEE    // Factory 1
#define IR_FHEADER_CODE1_1              0xA1    // Factory 1
#define IR_FHEADER_CODE1_2              0xA2    // Factory 1
#define IR_FHEADER_CODE1_3              0xA3    // Factory 1
#define IR_FHEADER_CODE1_4              0xA4    // Factory 1
#define IR_FHEADER_CODE1_5              0xA5    // Factory 1
#define IR_FHEADER_CODE1_6              0xA6    // Factory 1
#define IR_FHEADER_CODE1_7              0xA7    // Factory 1
#define IR_FHEADER_CODE1_8              0xAE    // Factory 1


#define IR_FHEADER_CODE2_0              0x0F    // Factory 2

#define IR_FHEADER_CODE3_0              0xFF    // Factory 3

// IR Timing define
#define IR_HEADER_CODE_TIME             9000    // us
#define IR_OFF_CODE_TIME                4500    // us
#define IR_OFF_CODE_RP_TIME             2500    // us
#define IR_LOGI_01H_TIME                560     // us
#define IR_LOGI_0_TIME                  1120    // us
#define IR_LOGI_1_TIME                  2240    // us
#define IR_TIMEOUT_CYC                  104500//140000  // us

#define IR_HEADER_CODE_TIME_UB          (20)
#define IR_HEADER_CODE_TIME_LB          (-20)
#define IR_OFF_CODE_TIME_UB             (20)
#define IR_OFF_CODE_TIME_LB             (-20)
#define IR_OFF_CODE_RP_TIME_UB          (20)
#define IR_OFF_CODE_RP_TIME_LB         (-20)
#define IR_LOGI_01H_TIME_UB             (35)
#define IR_LOGI_01H_TIME_LB             (-30)
#define IR_LOGI_0_TIME_UB               (20)
#define IR_LOGI_0_TIME_LB               (-20)
#define IR_LOGI_1_TIME_UB               (20)
#define IR_LOGI_1_TIME_LB               (-20)

// IR Format define
#define IRKEY_DUMY                      0xFFFF
#define CUS22T_IRKEY_DUMY               0xFFFF
#define IRDA_KEY_MAPPING_POWER          IRKEY_POWER
#define IR_RAW_DATA_NUM                 4

#define IR_LEADER_CODE_CHECKING_OPTION  0xBF

typedef enum _IR_CUS22T_FULLDECODE_40B0_TABLE
{
    // 0x40B0XXXX
    //-------------------------------------------------------------------------
    // CABLE/SAT
    CUS22T_IRKEY_CS_POWER                      = 0x12, // K02, POWER

    CUS22T_IRKEY_CS_NUM_1                      = 0x01, // K10, 1
    CUS22T_IRKEY_CS_NUM_2                      = 0x02, // K11, 2
    CUS22T_IRKEY_CS_NUM_3                      = 0x03, // K12, 3
    CUS22T_IRKEY_CS_NUM_4                      = 0x04, // K13, 4
    CUS22T_IRKEY_CS_NUM_5                      = 0x05, // K14, 5
    CUS22T_IRKEY_CS_NUM_6                      = 0x06, // K15, 6
    CUS22T_IRKEY_CS_NUM_7                      = 0x07, // K16, 7
    CUS22T_IRKEY_CS_NUM_8                      = 0x08, // K17, 8
    CUS22T_IRKEY_CS_NUM_9                      = 0x09, // K18, 9
    CUS22T_IRKEY_CS_DASH                       = 0x0A, // K19, -
    CUS22T_IRKEY_CS_NUM_0                      = 0x00, // K20, 0

    CUS22T_IRKEY_CS_CHANNEL_RETURN             = 0x17, // K21, CH RTN
    CUS22T_IRKEY_CS_INFO                       = 0x84, // K24, INFO
    CUS22T_IRKEY_CS_CHANNEL_PLUS               = 0x1B, // K27, CH+
    CUS22T_IRKEY_CS_CHANNEL_MINUS              = 0x1F, // K28, CH-

    CUS22T_IRKEY_CS_PAGE_UP                    = 0x20, // K32, Page up
    CUS22T_IRKEY_CS_PAGE_DOWN                  = 0x21, // K33, Page down
    CUS22T_IRKEY_CS_PAGE_LEFT                  = 0x22, // K34, Page left
    CUS22T_IRKEY_CS_PAGE_RIGHT                 = 0x23, // K35, Page right

    CUS22T_IRKEY_CS_QUICK                      = 0xD3, // K36, QUICK
    CUS22T_IRKEY_CS_EPG                        = 0x85, // K37, GUIDE
    CUS22T_IRKEY_CS_RETURN                     = 0xD9, // K38, BACK
    CUS22T_IRKEY_CS_EXIT                       = 0x58, // K39, EXIT
    CUS22T_IRKEY_CS_UP                         = 0x82, // K40, MENU UP
    CUS22T_IRKEY_CS_DOWN                       = 0x83, // K41, MENU DOWN
    CUS22T_IRKEY_CS_LEFT                       = 0x1D, // K42, MENU LEFT
    CUS22T_IRKEY_CS_RIGHT                      = 0x19, // K43, MENU RIGHT
    CUS22T_IRKEY_CS_SELECT                     = 0x89, // K44, OK

    CUS22T_IRKEY_CS_REW                        = 0x34, // K45, REW
    CUS22T_IRKEY_CS_PLAY                       = 0x30, // K46, PLAY
    CUS22T_IRKEY_CS_FF                         = 0x33, // K47, FF
    CUS22T_IRKEY_CS_SKIP_MINUS                 = 0x37, // K48, SKIP-
    CUS22T_IRKEY_CS_STOP                       = 0x31, // K49, STOP
    CUS22T_IRKEY_CS_PAUSE                      = 0x35, // K50, PAUSE
    CUS22T_IRKEY_CS_SKIP_PLUS                  = 0x36, // K51, SKIP+

    CUS22T_IRKEY_CS_REC                        = 0x38, // K54, REC

    CUS22T_IRKEY_CS_RED                        = 0x73, // K55, RED
    CUS22T_IRKEY_CS_GREEN                      = 0x74, // K56, GREEN
    CUS22T_IRKEY_CS_YELLOW                     = 0x75, // K57, YELLOW
    CUS22T_IRKEY_CS_BLUE                       = 0x76, // K58, BLUE

    CUS22T_IRKEY_CS_MENU                       = 0x80, // K61, MENU/SETUP
    CUS22T_IRKEY_CS_MTS                        = 0x13, // K62, MTS/AUDIO
    CUS22T_IRKEY_CS_CC                         = 0x57, // K63, CC
}_IR_CUS22T_FULLDECODE_40B0_TABLE;

typedef enum _IR_CUS22T_FULLDECODE_40B1_TABLE
{
    // 0x40B1XXXX
    //-------------------------------------------------------------------------
    // BD/DVD
    CUS22T_IRKEY_BD_POWER                      = 0x12, // K02, POWER

    CUS22T_IRKEY_BD_NUM_1                      = 0x01, // K10, 1
    CUS22T_IRKEY_BD_NUM_2                      = 0x02, // K11, 2
    CUS22T_IRKEY_BD_NUM_3                      = 0x03, // K12, 3
    CUS22T_IRKEY_BD_NUM_4                      = 0x04, // K13, 4
    CUS22T_IRKEY_BD_NUM_5                      = 0x05, // K14, 5
    CUS22T_IRKEY_BD_NUM_6                      = 0x06, // K15, 6
    CUS22T_IRKEY_BD_NUM_7                      = 0x07, // K16, 7
    CUS22T_IRKEY_BD_NUM_8                      = 0x08, // K17, 8
    CUS22T_IRKEY_BD_NUM_9                      = 0x09, // K18, 9
    CUS22T_IRKEY_BD_DASH                       = 0x0A, // K19, -
    CUS22T_IRKEY_BD_NUM_0                      = 0x00, // K20, 0

    CUS22T_IRKEY_BD_CHANNEL_RETURN             = 0x17, // K21, CH RTN
    CUS22T_IRKEY_BD_INFO                       = 0x84, // K24, INFO
    CUS22T_IRKEY_BD_CHANNEL_PLUS               = 0x1B, // K27, CH+
    CUS22T_IRKEY_BD_CHANNEL_MINUS              = 0x1F, // K28, CH-

    CUS22T_IRKEY_BD_PAGE_UP                    = 0x20, // K32, Page up
    CUS22T_IRKEY_BD_PAGE_DOWN                  = 0x21, // K33, Page down
    CUS22T_IRKEY_BD_PAGE_LEFT                  = 0x22, // K34, Page left
    CUS22T_IRKEY_BD_PAGE_RIGHT                 = 0x23, // K35, Page right

    CUS22T_IRKEY_BD_QUICK                      = 0xD3, // K36, QUICK
    CUS22T_IRKEY_BD_EPG                        = 0x85, // K37, GUIDE
    CUS22T_IRKEY_BD_RETURN                     = 0xD9, // K38, BACK
    CUS22T_IRKEY_BD_EXIT                       = 0x58, // K39, EXIT
    CUS22T_IRKEY_BD_UP                         = 0x82, // K40, MENU UP
    CUS22T_IRKEY_BD_DOWN                       = 0x83, // K41, MENU DOWN
    CUS22T_IRKEY_BD_LEFT                       = 0x1D, // K42, MENU LEFT
    CUS22T_IRKEY_BD_RIGHT                      = 0x19, // K43, MENU RIGHT
    CUS22T_IRKEY_BD_SELECT                     = 0x89, // K44, OK

    CUS22T_IRKEY_BD_REW                        = 0x34, // K45, REW
    CUS22T_IRKEY_BD_PLAY                       = 0x30, // K46, PLAY
    CUS22T_IRKEY_BD_FF                         = 0x33, // K47, FF
    CUS22T_IRKEY_BD_SKIP_MINUS                 = 0x37, // K48, SKIP-
    CUS22T_IRKEY_BD_STOP                       = 0x31, // K49, STOP
    CUS22T_IRKEY_BD_PAUSE                      = 0x35, // K50, PAUSE
    CUS22T_IRKEY_BD_SKIP_PLUS                  = 0x36, // K51, SKIP+

    CUS22T_IRKEY_BD_REC                        = 0x38, // K54, REC

    CUS22T_IRKEY_BD_RED                        = 0x73, // K55, RED
    CUS22T_IRKEY_BD_GREEN                      = 0x74, // K56, GREEN
    CUS22T_IRKEY_BD_YELLOW                     = 0x75, // K57, YELLOW
    CUS22T_IRKEY_BD_BLUE                       = 0x76, // K58, BLUE

    CUS22T_IRKEY_BD_MENU                       = 0x80, // K61, MENU/SETUP
    CUS22T_IRKEY_BD_MTS                        = 0x13, // K62, MTS/AUDIO
    CUS22T_IRKEY_BD_CC                         = 0x57, // K63, CC
}_IR_CUS22T_FULLDECODE_40B1_TABLE;

typedef enum _IR_CUS22T_FULLDECODE_40B2_TABLE
{
    // 0x40B2XXXX
    //-------------------------------------------------------------------------
    // DVR/VCR
    CUS22T_IRKEY_DV_POWER                      = 0x12, // K02, POWER

    CUS22T_IRKEY_DV_NUM_1                      = 0x01, // K10, 1
    CUS22T_IRKEY_DV_NUM_2                      = 0x02, // K11, 2
    CUS22T_IRKEY_DV_NUM_3                      = 0x03, // K12, 3
    CUS22T_IRKEY_DV_NUM_4                      = 0x04, // K13, 4
    CUS22T_IRKEY_DV_NUM_5                      = 0x05, // K14, 5
    CUS22T_IRKEY_DV_NUM_6                      = 0x06, // K15, 6
    CUS22T_IRKEY_DV_NUM_7                      = 0x07, // K16, 7
    CUS22T_IRKEY_DV_NUM_8                      = 0x08, // K17, 8
    CUS22T_IRKEY_DV_NUM_9                      = 0x09, // K18, 9
    CUS22T_IRKEY_DV_DASH                       = 0x0A, // K19, -
    CUS22T_IRKEY_DV_NUM_0                      = 0x00, // K20, 0

    CUS22T_IRKEY_DV_CHANNEL_RETURN             = 0x17, // K21, CH RTN
    CUS22T_IRKEY_DV_INFO                       = 0x84, // K24, INFO
    CUS22T_IRKEY_DV_CHANNEL_PLUS               = 0x1B, // K27, CH+
    CUS22T_IRKEY_DV_CHANNEL_MINUS              = 0x1F, // K28, CH-

    CUS22T_IRKEY_DV_PAGE_UP                    = 0x20, // K32, Page up
    CUS22T_IRKEY_DV_PAGE_DOWN                  = 0x21, // K33, Page down
    CUS22T_IRKEY_DV_PAGE_LEFT                  = 0x22, // K34, Page left
    CUS22T_IRKEY_DV_PAGE_RIGHT                 = 0x23, // K35, Page right

    CUS22T_IRKEY_DV_QUICK                      = 0xD3, // K36, QUICK
    CUS22T_IRKEY_DV_EPG                        = 0x85, // K37, GUIDE
    CUS22T_IRKEY_DV_RETURN                     = 0xD9, // K38, BACK
    CUS22T_IRKEY_DV_EXIT                       = 0x58, // K39, EXIT
    CUS22T_IRKEY_DV_UP                         = 0x82, // K40, MENU UP
    CUS22T_IRKEY_DV_DOWN                       = 0x83, // K41, MENU DOWN
    CUS22T_IRKEY_DV_LEFT                       = 0x1D, // K42, MENU LEFT
    CUS22T_IRKEY_DV_RIGHT                      = 0x19, // K43, MENU RIGHT
    CUS22T_IRKEY_DV_SELECT                     = 0x89, // K44, OK

    CUS22T_IRKEY_DV_REW                        = 0x34, // K45, REW
    CUS22T_IRKEY_DV_PLAY                       = 0x30, // K46, PLAY
    CUS22T_IRKEY_DV_FF                         = 0x33, // K47, FF
    CUS22T_IRKEY_DV_SKIP_MINUS                 = 0x37, // K48, SKIP-
    CUS22T_IRKEY_DV_STOP                       = 0x31, // K49, STOP
    CUS22T_IRKEY_DV_PAUSE                      = 0x35, // K50, PAUSE
    CUS22T_IRKEY_DV_SKIP_PLUS                  = 0x36, // K51, SKIP+

    CUS22T_IRKEY_DV_REC                        = 0x38, // K54, REC

    CUS22T_IRKEY_DV_RED                        = 0x73, // K55, RED
    CUS22T_IRKEY_DV_GREEN                      = 0x74, // K56, GREEN
    CUS22T_IRKEY_DV_YELLOW                     = 0x75, // K57, YELLOW
    CUS22T_IRKEY_DV_BLUE                       = 0x76, // K58, BLUE

    CUS22T_IRKEY_DV_MENU                       = 0x80, // K61, MENU/SETUP
    CUS22T_IRKEY_DV_MTS                        = 0x13, // K62, MTS/AUDIO
    CUS22T_IRKEY_DV_CC                         = 0x57, // K63, CC
}_IR_CUS22T_FULLDECODE_40B2_TABLE;

typedef enum _IR_CUS22T_FULLDECODE_40B3_TABLE
{
    // 0x40B3XXXX
    //-------------------------------------------------------------------------
    // AUDIO
    CUS22T_IRKEY_AUDIO_INPUT_SOURCE            = 0x0F, // K01, INPUT
    CUS22T_IRKEY_AUDIO_POWER                   = 0x12, // K02, POWER

    CUS22T_IRKEY_AUDIO_NUM_1                   = 0x01, // K10, 1
    CUS22T_IRKEY_AUDIO_NUM_2                   = 0x02, // K11, 2
    CUS22T_IRKEY_AUDIO_NUM_3                   = 0x03, // K12, 3
    CUS22T_IRKEY_AUDIO_NUM_4                   = 0x04, // K13, 4
    CUS22T_IRKEY_AUDIO_NUM_5                   = 0x05, // K14, 5
    CUS22T_IRKEY_AUDIO_NUM_6                   = 0x06, // K15, 6
    CUS22T_IRKEY_AUDIO_NUM_7                   = 0x07, // K16, 7
    CUS22T_IRKEY_AUDIO_NUM_8                   = 0x08, // K17, 8
    CUS22T_IRKEY_AUDIO_NUM_9                   = 0x09, // K18, 9
    CUS22T_IRKEY_AUDIO_DASH                    = 0x0A, // K19, -
    CUS22T_IRKEY_AUDIO_NUM_0                   = 0x00, // K20, 0

    CUS22T_IRKEY_AUDIO_CHANNEL_RETURN          = 0x17, // K21, CH RTN
    CUS22T_IRKEY_AUDIO_VOLUME_PLUS             = 0x1A, // K22, VOL+
    CUS22T_IRKEY_AUDIO_VOLUME_MINUS            = 0x1E, // K23, VOL-

    CUS22T_IRKEY_AUDIO_INFO                    = 0x84, // K24, INFO
    CUS22T_IRKEY_AUDIO_MUTE                    = 0x10, // K25, MUTE

    CUS22T_IRKEY_AUDIO_CHANNEL_PLUS            = 0x1B, // K27, CH+
    CUS22T_IRKEY_AUDIO_CHANNEL_MINUS           = 0x1F, // K28, CH-

    CUS22T_IRKEY_AUDIO_PAGE_UP                 = 0x20, // K32, Page up
    CUS22T_IRKEY_AUDIO_PAGE_DOWN               = 0x21, // K33, Page down
    CUS22T_IRKEY_AUDIO_PAGE_LEFT               = 0x22, // K34, Page left
    CUS22T_IRKEY_AUDIO_PAGE_RIGHT              = 0x23, // K35, Page right

    CUS22T_IRKEY_AUDIO_QUICK                   = 0xD3, // K36, QUICK
    CUS22T_IRKEY_AUDIO_EPG                     = 0x85, // K37, GUIDE
    CUS22T_IRKEY_AUDIO_RETURN                  = 0xD9, // K38, BACK
    CUS22T_IRKEY_AUDIO_EXIT                    = 0x58, // K39, EXIT
    CUS22T_IRKEY_AUDIO_UP                      = 0x82, // K40, MENU UP
    CUS22T_IRKEY_AUDIO_DOWN                    = 0x83, // K41, MENU DOWN
    CUS22T_IRKEY_AUDIO_LEFT                    = 0x1D, // K42, MENU LEFT
    CUS22T_IRKEY_AUDIO_RIGHT                   = 0x19, // K43, MENU RIGHT
    CUS22T_IRKEY_AUDIO_SELECT                  = 0x89, // K44, OK

    CUS22T_IRKEY_AUDIO_REW                     = 0x34, // K45, REW
    CUS22T_IRKEY_AUDIO_PLAY                    = 0x30, // K46, PLAY
    CUS22T_IRKEY_AUDIO_FF                      = 0x33, // K47, FF
    CUS22T_IRKEY_AUDIO_SKIP_MINUS              = 0x37, // K48, SKIP-
    CUS22T_IRKEY_AUDIO_STOP                    = 0x31, // K49, STOP
    CUS22T_IRKEY_AUDIO_PAUSE                   = 0x35, // K50, PAUSE
    CUS22T_IRKEY_AUDIO_SKIP_PLUS               = 0x36, // K51, SKIP+

    CUS22T_IRKEY_AUDIO_REC                     = 0x38, // K54, REC

    CUS22T_IRKEY_AUDIO_RED                     = 0x73, // K55, RED
    CUS22T_IRKEY_AUDIO_GREEN                   = 0x74, // K56, GREEN
    CUS22T_IRKEY_AUDIO_YELLOW                  = 0x75, // K57, YELLOW
    CUS22T_IRKEY_AUDIO_BLUE                    = 0x76, // K58, BLUE

    CUS22T_IRKEY_AUDIO_MENU                    = 0x80, // K61, MENU/SETUP
    CUS22T_IRKEY_AUDIO_MTS                     = 0x13, // K62, MTS/AUDIO
    CUS22T_IRKEY_AUDIO_CC                      = 0x57, // K63, CC
}_IR_CUS22T_FULLDECODE_40B3_TABLE;

typedef enum _IR_CUS22T_FULLDECODE_40B4_TABLE
{
    // 0x40B4XXXX
    //-------------------------------------------------------------------------
    // AUX
    CUS22T_IRKEY_AUX_POWER                     = 0x12, // K02, POWER

    CUS22T_IRKEY_AUX_NUM_1                     = 0x01, // K10, 1
    CUS22T_IRKEY_AUX_NUM_2                     = 0x02, // K11, 2
    CUS22T_IRKEY_AUX_NUM_3                     = 0x03, // K12, 3
    CUS22T_IRKEY_AUX_NUM_4                     = 0x04, // K13, 4
    CUS22T_IRKEY_AUX_NUM_5                     = 0x05, // K14, 5
    CUS22T_IRKEY_AUX_NUM_6                     = 0x06, // K15, 6
    CUS22T_IRKEY_AUX_NUM_7                     = 0x07, // K16, 7
    CUS22T_IRKEY_AUX_NUM_8                     = 0x08, // K17, 8
    CUS22T_IRKEY_AUX_NUM_9                     = 0x09, // K18, 9
    CUS22T_IRKEY_AUX_DASH                      = 0x0A, // K19, -
    CUS22T_IRKEY_AUX_NUM_0                     = 0x00, // K20, 0

    CUS22T_IRKEY_AUX_CHANNEL_RETURN            = 0x17, // K21, CH RTN
    CUS22T_IRKEY_AUX_INFO                      = 0x84, // K24, INFO
    CUS22T_IRKEY_AUX_CHANNEL_PLUS              = 0x1B, // K27, CH+
    CUS22T_IRKEY_AUX_CHANNEL_MINUS             = 0x1F, // K28, CH-

    CUS22T_IRKEY_AUX_PAGE_UP                   = 0x20, // K32, Page up
    CUS22T_IRKEY_AUX_PAGE_DOWN                 = 0x21, // K33, Page down
    CUS22T_IRKEY_AUX_PAGE_LEFT                 = 0x22, // K34, Page left
    CUS22T_IRKEY_AUX_PAGE_RIGHT                = 0x23, // K35, Page right

    CUS22T_IRKEY_AUX_QUICK                     = 0xD3, // K36, QUICK
    CUS22T_IRKEY_AUX_EPG                       = 0x85, // K37, GUIDE
    CUS22T_IRKEY_AUX_RETURN                    = 0xD9, // K38, BACK
    CUS22T_IRKEY_AUX_EXIT                      = 0x58, // K39, EXIT
    CUS22T_IRKEY_AUX_UP                        = 0x82, // K40, MENU UP
    CUS22T_IRKEY_AUX_DOWN                      = 0x83, // K41, MENU DOWN
    CUS22T_IRKEY_AUX_LEFT                      = 0x1D, // K42, MENU LEFT
    CUS22T_IRKEY_AUX_RIGHT                     = 0x19, // K43, MENU RIGHT
    CUS22T_IRKEY_AUX_SELECT                    = 0x89, // K44, OK

    CUS22T_IRKEY_AUX_REW                       = 0x34, // K45, REW
    CUS22T_IRKEY_AUX_PLAY                      = 0x30, // K46, PLAY
    CUS22T_IRKEY_AUX_FF                        = 0x33, // K47, FF
    CUS22T_IRKEY_AUX_SKIP_MINUS                = 0x37, // K48, SKIP-
    CUS22T_IRKEY_AUX_STOP                      = 0x31, // K49, STOP
    CUS22T_IRKEY_AUX_PAUSE                     = 0x35, // K50, PAUSE
    CUS22T_IRKEY_AUX_SKIP_PLUS                 = 0x36, // K51, SKIP+

    CUS22T_IRKEY_AUX_REC                       = 0x38, // K54, REC

    CUS22T_IRKEY_AUX_RED                       = 0x73, // K55, RED
    CUS22T_IRKEY_AUX_GREEN                     = 0x74, // K56, GREEN
    CUS22T_IRKEY_AUX_YELLOW                    = 0x75, // K57, YELLOW
    CUS22T_IRKEY_AUX_BLUE                      = 0x76, // K58, BLUE

    CUS22T_IRKEY_AUX_MENU                      = 0x80, // K61, MENU/SETUP
    CUS22T_IRKEY_AUX_MTS                       = 0x13, // K62, MTS/AUDIO
    CUS22T_IRKEY_AUX_CC                        = 0x57, // K63, CC
}_IR_CUS22T_FULLDECODE_40B4_TABLE;

typedef enum _IR_CUS22T_FULLDECODE_40BE_TABLE
{
    // 0x40BEXXXX
    //-------------------------------------------------------------------------
    // TV
    CUS22T_IRKEY_MEDIA_PLAYER                  = 0x2B, // K26, MEDIA SHARE
    CUS22T_IRKEY_SEARCH                        = 0x25, // K29, SEARCH (NEW)
    CUS22T_IRKEY_HOME                          = 0x34, // K30, HOME
    CUS22T_IRKEY_INTERNET                      = 0x28, // K31, INTERNET

    CUS22T_IRKEY_PAGE_UP                       = 0x20, // K32, Page up
    CUS22T_IRKEY_PAGE_DOWN                     = 0x21, // K33, Page down
    CUS22T_IRKEY_PAGE_LEFT                     = 0x22, // K34, Page left
    CUS22T_IRKEY_PAGE_RIGHT                    = 0x23, // K35, Page right

    CUS22T_IRKEY_NETFLIX                       = 0x2A, // K52, NETFLIX (NEW)
    CUS22T_IRKEY_EMANUAL                       = 0x27, // K53, eManual (NEW)

    CUS22T_IRKEY_3D                            = 0x43, // K59(L7350U), 3D

    //-------------------------------------------------------------------------
    // K55 (RED) +
    CUS22T_IRKEY_MUTE_ON                       = 0x80, // MUTE ON (MUTE+RED)

    //-------------------------------------------------------------------------
    // K56 (GREEN) +
    CUS22T_IRKEY_PICTURE_SIZE_4_3              = 0x54, // NATIVE (NUM2+GREEN)
    CUS22T_IRKEY_PICTURE_SIZE_NORMAL           = 0x55, // NORMAL (NUM7+GREEN)
    CUS22T_IRKEY_PICTURE_SIZE_DOTBYDOT         = 0x56, // DOTBYDOT (NUM8+GREEN)

    CUS22T_IRKEY_HALF_MUTE_ON                  = 0x82, // 1/2 MUTE ON (MUTE+GREEN)

    //-------------------------------------------------------------------------
    // K57 (YELLOW) +
    CUS22T_IRKEY_PICTURE_MODE_AUTOVIEW         = 0x50, // AUTOVIEW (NUM1+YELLOW)
    CUS22T_IRKEY_PICTURE_MODE_CALIBRATED       = 0x58, // CALIBRATED (NUM5+YELLOW)
    CUS22T_IRKEY_PICTURE_MODE_STORE            = 0x51, // STORE (NUM6+YELLOW)
    CUS22T_IRKEY_PICTURE_MODE_GAME             = 0x52, // GAME (NUM7+YELLOW)
    CUS22T_IRKEY_PICTURE_MODE_PC               = 0x53, // PC (NUM8+YELLOW)

    CUS22T_IRKEY_MUTE_OFF                      = 0x81, // MUTE OFF (MUTE+YELLOW)


    //---------------------------------------------------------------------------------
    // MENU
    CUS22T_40BE_POWER                          = 0x12,
    CUS22T_40BE_AUDIO                          = 0x6D,
    CUS22T_40BE_SETUP                          = 0x73,
    CUS22T_40BE_LOAD                           = 0x65,

    // REGZA-LINK
    CUS22T_40BE_ROOT                           = 0x62,
    CUS22T_40BE_CONTENTS                       = 0x75,
    CUS22T_40BE_TOP                            = 0x74,
    CUS22T_40BE_DVD                            = 0x72,

    CUS22T_40BE_3D                             = 0x43,

    //CUS22T_40BE_MUTE_ON                        = 0x80,
    //CUS22T_40BE_MUTE_OFF                       = 0x81,
}_IR_CUS22T_FULLDECODE_40BE_TABLE;

typedef enum _IR_CUS22T_FULLDECODE_40BF_TABLE
{
    // 0x40BFXXXX
    // -- General ------------------------------------------------------------------------
    // TV
    CUS22T_IRKEY_FACTORY_COMMAND               = IRKEY_DUMY-1,   //don't modify for TSB

    CUS22T_IRKEY_INPUT_SOURCE                  = 0x0F, // K01, INPUT
    CUS22T_IRKEY_POWER                         = 0x12, // K02, POWER
    CUS22T_IRKEY_TV_INPUT                      = 0x77, // K04, TV
    CUS22T_IRKEY_FREEZE                         = 0x4C, //freeze

    CUS22T_IRKEY_RECALL                      = 0x1C,//recall

    CUS22T_IRKEY_MODE_CABLE_SAT                = 0x78, // K05, CABLE/SAT (New)
    CUS22T_IRKEY_MODE_BD_DVD                   = 0xD6, // K06, BD/DVD (New)
    CUS22T_IRKEY_MODE_DVR_VCR                  = 0x7A, // K07, DVR/VCR (New)
    CUS22T_IRKEY_MODE_AUDIO                    = 0x7D, // K08, AUDIO (New)
    CUS22T_IRKEY_MODE_AUX                      = 0x7B, // K09, AUX (New)

    CUS22T_IRKEY_NUM_1                         = 0x01, // K10, 1
    CUS22T_IRKEY_NUM_2                         = 0x02, // K11, 2
    CUS22T_IRKEY_NUM_3                         = 0x03, // K12, 3
    CUS22T_IRKEY_NUM_4                         = 0x04, // K13, 4
    CUS22T_IRKEY_NUM_5                         = 0x05, // K14, 5
    CUS22T_IRKEY_NUM_6                         = 0x06, // K15, 6
    CUS22T_IRKEY_NUM_7                         = 0x07, // K16, 7
    CUS22T_IRKEY_NUM_8                         = 0x08, // K17, 8
    CUS22T_IRKEY_NUM_9                         = 0x09, // K18, 9
    CUS22T_IRKEY_DASH                          = 0x0A, // K19, -
    CUS22T_IRKEY_NUM_0                         = 0x00, // K20, 0

    CUS22T_IRKEY_CHANNEL_RETURN                = 0x17, // K21, CH RTN

    CUS22T_IRKEY_VOLUME_PLUS                   = 0x1A, // K22, VOL+
    CUS22T_IRKEY_VOLUME_MINUS                  = 0x1E, // K23, VOL-

    CUS22T_IRKEY_INFO                          = 0x84, // K24, INFO
    CUS22T_IRKEY_MUTE                          = 0x10, // K25, MUTE

    CUS22T_IRKEY_CHANNEL_PLUS                  = 0x1B, // K27, CH+
    CUS22T_IRKEY_CHANNEL_MINUS                 = 0x1F, // K28, CH-

    CUS22T_IRKEY_QUICK                         = 0xD3, // K36, QUICK
    CUS22T_IRKEY_EPG                           = 0x85, // K37, GUIDE
    CUS22T_IRKEY_RETURN                        = 0xD9, // K38, BACK
    CUS22T_IRKEY_EXIT                          = 0x58, // K39, EXIT
    CUS22T_IRKEY_UP                            = 0x82, // K40, MENU UP
    CUS22T_IRKEY_DOWN                          = 0x83, // K41, MENU DOWN
    CUS22T_IRKEY_LEFT                          = 0x1D, // K42, MENU LEFT
    CUS22T_IRKEY_RIGHT                         = 0x19, // K43, MENU RIGHT
    CUS22T_IRKEY_SELECT                        = 0x89, // K44, OK

    CUS22T_IRKEY_REW                           = 0x34, // K45, REW
    CUS22T_IRKEY_PLAY                          = 0x30, // K46, PLAY
    CUS22T_IRKEY_FF                            = 0x33, // K47, FF
    CUS22T_IRKEY_SKIP_MINUS                    = 0x37, // K48, SKIP-
    CUS22T_IRKEY_STOP                          = 0x31, // K49, STOP
    CUS22T_IRKEY_PAUSE                         = 0x35, // K50, PAUSE
    CUS22T_IRKEY_SKIP_PLUS                     = 0x36, // K51, SKIP+

    CUS22T_IRKEY_REC                           = 0x38, // K54, REC
    CUS22T_IRKEY_EJECT                       =0x42,//eject
    CUS22T_IRKEY_FAV                       =0x5D,//eject

    CUS22T_IRKEY_RED                           = 0x73, // K55, RED
    CUS22T_IRKEY_GREEN                         = 0x74, // K56, GREEN
    CUS22T_IRKEY_YELLOW                        = 0x75, // K57, YELLOW
    CUS22T_IRKEY_BLUE                          = 0x76, // K58, BLUE

    CUS22T_IRKEY_ZOOM                          = 0x54, // K59(L4300U), PIC SIZE
    CUS22T_IRKEY_PICTURE                       = 0x2D, // K60, PIC MODE
    CUS22T_IRKEY_MENU                          = 0x80, // K61, MENU/SETUP
    CUS22T_IRKEY_MTS                           = 0x13, // K62, MTS/AUDIO
    CUS22T_IRKEY_CC                            = 0x57, // K63, CC
    CUS22T_IRKEY_SLEEP                         = 0x15, // K64(L7350U) K03(L4300U), SLEEP

    //-------------------------------------------------------------------------
    // K55 (RED) +
    CUS22T_IRKEY_POWER_OFF                     = 0x7F, // POWER OFF (INPUT+RED)
    CUS22T_IRKEY_POWER_ON                      = 0x7E, // POWER ON (POWER+RED)

    CUS22T_IRKEY_INPUT1                        = 0x20, // INPUT 1 (NUM1+RED)
    CUS22T_IRKEY_INPUT2                        = 0x21, // INPUT 2 (NUM2+RED)
    CUS22T_IRKEY_INPUT3                        = 0x22, // INPUT 3 (NUM3+RED)
    CUS22T_IRKEY_INPUT4                        = 0x23, // INPUT 4 (NUM4+RED)
    CUS22T_IRKEY_INPUT5                        = 0x24, // INPUT 5 (NUM5+RED)
    CUS22T_IRKEY_INPUT6                        = 0x25, // INPUT 6 (NUM6+RED)
    CUS22T_IRKEY_INPUT7                        = 0x39, // INPUT 7 (NUM7+RED)
    CUS22T_IRKEY_INPUT8                        = 0x3A, // INPUT 8 (NUM8+RED)
    CUS22T_IRKEY_INPUT9                        = 0x3B, // INPUT 9 (NUM9+RED)
    CUS22T_IRKEY_INPUT10                       = 0x3C, // INPUT 10 (DASH+RED)
    CUS22T_IRKEY_ANT1                          = 0x3D, // ANT1 (NUM0+RED)

    //-------------------------------------------------------------------------
    // K56 (GREEN) +
    CUS22T_IRKEY_PICTURE_SIZE_NATIVE           = 0xD1, // NATIVE (NUM1+GREEN)
    CUS22T_IRKEY_PICTURE_SIZE_NATURAL          = 0xC0,
    CUS22T_IRKEY_PICTURE_SIZE_THEATERWIDE1     = 0xC1, // THEATERWIDE1 (NUM3+GREEN)
    CUS22T_IRKEY_PICTURE_SIZE_THEATERWIDE2     = 0xC2, // THEATERWIDE2 (NUM4+GREEN)
    CUS22T_IRKEY_PICTURE_SIZE_THEATERWIDE3     = 0xC3, // THEATERWIDE3 (NUM5+GREEN)
    CUS22T_IRKEY_PICTURE_SIZE_FULL             = 0xCB, // FULL (NUM6+GREEN)
    CUS22T_IRKEY_PICTURE_SIZE_4_3_P            = 0xCC,

    //-------------------------------------------------------------------------
    // K57 (YELLOW) +
    CUS22T_IRKEY_PICTURE_MODE_DYNAMIC          = 0xCD, // DYNAMIC (NUM2+YELLOW)
    CUS22T_IRKEY_PICTURE_MODE_STANDARD         = 0xCE, // DYNAMIC (NUM3+YELLOW)
    CUS22T_IRKEY_PICTURE_MODE_MOVIE            = 0xCF, // DYNAMIC (NUM4+YELLOW)

    //-------------------------------------------------------------------------
    CUS22T_IRKEY_CHANNEL_LIST                  = CUS22T_IRKEY_DUMY-2,
    CUS22T_IRKEY_CHANNEL_FAV_LIST              = CUS22T_IRKEY_DUMY-3,
    CUS22T_IRKEY_AUDIO                         = CUS22T_IRKEY_DUMY-4, // AD : Audio Description

   // CUS22T_IRKEY_FREEZE                        = CUS22T_IRKEY_DUMY-5,
    CUS22T_IRKEY_CLOCK                         = CUS22T_IRKEY_DUMY-6,

    CUS22T_IRKEY_MIX                           = CUS22T_IRKEY_DUMY-7,
    CUS22T_IRKEY_INDEX                         = CUS22T_IRKEY_DUMY-8,
    //CUS22T_IRKEY_PREVIOUS                    = CUS22T_IRKEY_DUMY,
    //CUS22T_IRKEY_NEXT                        = CUS22T_IRKEY_DUMY,
    //CUS22T_IRKEY_BRIGHTNESS_PLUS             = CUS22T_IRKEY_DUMY,
    //CUS22T_IRKEY_BRIGHTNESS_MINUS            = CUS22T_IRKEY_DUMY,
    CUS22T_IRKEY_FORWARD                       = CUS22T_IRKEY_DUMY-9,
    CUS22T_IRKEY_RECORD                        = CUS22T_IRKEY_DUMY-10,

    CUS22T_IRKEY_TEXT                          = CUS22T_IRKEY_DUMY-11, // TEXT
    CUS22T_IRKEY_SUBTITLE                      = CUS22T_IRKEY_DUMY-12, // SUBTITLE
    CUS22T_IRKEY_TOSHIBA_PLACES                = CUS22T_IRKEY_DUMY-13, // TOSHIBA PLACES

    CUS22T_IRKEY_TTX_MODE                      = CUS22T_IRKEY_DUMY-14, // TEXT SUB PAGE
    CUS22T_IRKEY_RED2                          = CUS22T_IRKEY_DUMY-18,
    CUS22T_IRKEY_GREEN2                        = CUS22T_IRKEY_DUMY-19,
    CUS22T_IRKEY_UPDATE                        = CUS22T_IRKEY_DUMY-20,
    //CUS22T_IRKEY_SUBCODE                     = CUS22T_IRKEY_DUMY,
    //CUS22T_IRKEY_TIME                        = 0x15,
    CUS22T_IRKEY_MEMORY_RECALL                 = CUS22T_IRKEY_DUMY-21,
    CUS22T_IRKEY_SIZE                          = CUS22T_IRKEY_DUMY-22,
    //CUS22T_IRKEY_REVEAL                      = 0x11,
    CUS22T_IRKEY_KEY_DISABLE_KEYPAD            = CUS22T_IRKEY_DUMY-24,
    CUS22T_IRKEY_BACK                          = CUS22T_IRKEY_DUMY-25,

    CUS22T_IRKEY_STILL                         = CUS22T_IRKEY_DUMY-15, //STILL

    //CUS22T_IRKEY_POWER_ON                      = CUS22T_IRKEY_DUMY-16, //POWER ON
    //CUS22T_IRKEY_POWER_OFF                     = CUS22T_IRKEY_DUMY-17, //POWER OFF

    // -- Specific for Vender ------------------------------------------------------------
    //CUS22T_IRKEY_SELECTABLE_PICTURE          = 0x11,
    //CUS22T_IRKEY_FULL_TOP_BOTTOM             = 0x0D,
    CUS22T_IRKEY_SUBPAGE                       = CUS22T_IRKEY_DUMY-27,
    CUS22T_IRKEY_DTV                           = CUS22T_IRKEY_DUMY-28,
    CUS22T_IRKEY_TV                            = CUS22T_IRKEY_DUMY-29,
    CUS22T_IRKEY_SCART                         = CUS22T_IRKEY_DUMY-30,
    CUS22T_IRKEY_HDMI                          = CUS22T_IRKEY_DUMY-31,
    CUS22T_IRKEY_PC                            = CUS22T_IRKEY_DUMY-32,
    CUS22T_IRKEY_COMPONENT                     = CUS22T_IRKEY_DUMY-33,
    CUS22T_IRKEY_AV                            = CUS22T_IRKEY_DUMY-34,
    CUS22T_IRKEY_SV                            = CUS22T_IRKEY_DUMY-35,

    CUS22T_IRKEY_SERVICE_MODE                  = CUS22T_IRKEY_DUMY-36,
    CUS22T_IRKEY_DESIGN_MODE                   = CUS22T_IRKEY_DUMY-37,
    CUS22T_IRKEY_HOTEL_CLONE                   = CUS22T_IRKEY_DUMY-38,

    // Add for compile error
    CUS22T_IRKEY_TV_RADIO                      = CUS22T_IRKEY_DUMY-39,
    CUS22T_IRKEY_NINE_LATTICE                  = CUS22T_IRKEY_DUMY-40,
    CUS22T_IRKEY_CRADRD                        = CUS22T_IRKEY_DUMY-41,
    CUS22T_IRKEY_PIP                           = CUS22T_IRKEY_DUMY-42,
    CUS22T_IRKEY_SUBCODE                       = CUS22T_IRKEY_DUMY-43,

}IR_CUS22T_FULLDECODE_40BF_TABLE;

typedef enum _IR_CUS22T_FACTORY_40BE_TABLE
{
    // 0x40BEXXXX
    CUS22T_F_40BE_PC_INPUT_SELECTION        = 0x57,     //0x40BE57A8
}IR_CUS22T_FACTORY_40BE_TABLE;

typedef enum _IR_CUS22T_FACTORY_40BF_TABLE
{
    // 0x40BFXXXX
    CUS22T_F_40BF_ATV                       = 0x2F,
    CUS22T_F_40BF_DTV                       = 0x30,
    CUS22T_F_40BF_VIDEO1                    = 0x31,
    CUS22T_F_40BF_VIDEO2                    = 0x32,
    CUS22T_F_40BF_VIDEO3                    = 0x33,
    CUS22T_F_40BF_VIDEO4                    = 0x34,
    CUS22T_F_40BF_VIDEO5                    = 0x35,
    CUS22T_F_40BF_VIDEO6                    = 0x36,
    CUS22T_F_40BF_VIDEO7                    = 0x37,
    CUS22T_F_40BF_HDMI1                     = 0x38,
    CUS22T_F_40BF_HDMI2                     = 0x39,
    CUS22T_F_40BF_HDMI3                     = 0x3A,
    CUS22T_F_40BF_HDMI4                     = 0x3B,
    CUS22T_F_40BF_PC                        = 0x3C,
    CUS22T_F_40BF_PIP_ON_OFF                = 0x51,
    CUS22T_F_40BF_POWER_OFF                 = 0x7F,
    CUS22T_F_40BF_TUNE_DOWN_SEARCH          = 0xA4,
    CUS22T_F_40BF_TUNE_UP_SEARCH            = 0xA5,
    CUS22T_F_40BF_LNB_POWER_ON_OFF          = 0xA9,     //DTV
    CUS22T_F_40BF_MEM                       = 0xAB,
    CUS22T_F_40BF_COLOR_SYS_CHECK           = 0xE6,
    CUS22T_F_40BF_SOUND_SYS_CHECK           = 0xE7,
    CUS22T_F_40BF_B_G_RRESET                = 0xE9,     //DTV
    CUS22T_F_40BF_I_PRESET                  = 0xEA,     //DTV
    CUS22T_F_40BF_AUDYSSEY                  = 0xEE,
    CUS22T_F_40BF_VXT2_PEAK                 = 0xEF,
    CUS22T_F_40BF_TUNER_STATUS_READ         = 0xF0,
    CUS22T_F_40BF_START_PE_MEASUREMENT      = 0xF1,
    CUS22T_F_40BF_STOP_PE_MEASUREMENT       = 0xF2,
    CUS22T_F_40BF_3D_TIMMING                = 0xF3,
    CUS22T_F_40BF_UART                      = 0xF4,
    CUS22T_F_40BF_E_D_MODE                  = 0xFE,
}IR_CUS22T_FACTORY_40BF_TABLE;

typedef enum _IR_CUS22T_FACTORY_40E1_TABLE
{
    // 0x40E1XXXX
    CUS22T_F_40E1_EEPROM_DISPLAY_DATA       = 0x00,
}IR_CUS22T_FACTORY_40E1_TABLE;

typedef enum _IR_CUS22T_FACTORY_40E2_TABLE
{
    // 0x40E2XXXX
    CUS22T_F_40E2_CONTRAST                  = 0x00,
    CUS22T_F_40E2_BRIGHT                    = 0x01,
    CUS22T_F_40E2_COLOR                     = 0x02,
    CUS22T_F_40E2_TINT                      = 0x03,
    CUS22T_F_40E2_SHARPNESS                 = 0x04,
    CUS22T_F_40E2_VOLUME                    = 0x10,
    CUS22T_F_40E2_BASS                      = 0x11,
    CUS22T_F_40E2_TREBLE                    = 0x12,
    CUS22T_F_40E2_BALANCE                   = 0x13,
    CUS22T_F_40E2_RCUT                      = 0x30,
    CUS22T_F_40E2_GCUT                      = 0x31,
    CUS22T_F_40E2_BCUT                      = 0x32,
    CUS22T_F_40E2_RDRV                      = 0x33,
    CUS22T_F_40E2_BDRV                      = 0x34,
}IR_CUS22T_FACTORY_40E2_TABLE;

typedef enum _IR_CUS22T_FACTORY_40E3_TABLE
{
    // 0x40E3XXXX
    CUS22T_F_40E3_RCUT                      = 0x30,
    CUS22T_F_40E3_GCUT                      = 0x31,
    CUS22T_F_40E3_BCUT                      = 0x32,
    CUS22T_F_40E3_RDRV_GDRV                 = 0x33,
    CUS22T_F_40E3_BDRV                      = 0x34,
}IR_CUS22T_FACTORY_40E3_TABLE;

typedef enum _IR_CUS22T_FACTORY_40EA_TABLE
{
    // 0x40EAXXXX
    CUS22T_F_40EA_INTERNAL_PATTERN_OFF      = 0x00,
    CUS22T_F_40EA_RED_RASTER                = 0x01,
    CUS22T_F_40EA_GREEN_RASTER              = 0x02,
    CUS22T_F_40EA_BLUE_RASTER               = 0x03,
    CUS22T_F_40EA_BLACK_RASTER              = 0x04,
    CUS22T_F_40EA_WHITE_RASTER              = 0x05,
    CUS22T_F_40EA_AGING_MODE_1              = 0x30,
    CUS22T_F_40EA_AGINE_MODE_2              = 0x31,
}IR_CUS22T_FACTORY_40EA_TABLE;

typedef enum _IR_CUS22T_FACTORY_40EB_TABLE
{
    // 0x40EBXXXX
    CUS22T_F_40EB_DIRECT_CH_00              = 0x00,
    CUS22T_F_40EB_DIRECT_CH_01              = 0x01,
    CUS22T_F_40EB_DIRECT_CH_02              = 0x02,
    CUS22T_F_40EB_DIRECT_CH_03              = 0x03,
    CUS22T_F_40EB_DIRECT_CH_04              = 0x04,
    CUS22T_F_40EB_DIRECT_CH_05              = 0x05,
    CUS22T_F_40EB_DIRECT_CH_06              = 0x06,
    CUS22T_F_40EB_DIRECT_CH_07              = 0x07,
    CUS22T_F_40EB_DIRECT_CH_08              = 0x08,
    CUS22T_F_40EB_DIRECT_CH_09              = 0x09,
    CUS22T_F_40EB_DIRECT_CH_0A              = 0x0A,
    CUS22T_F_40EB_DIRECT_CH_0B              = 0x0B,
    CUS22T_F_40EB_DIRECT_CH_0C              = 0x0C,
    CUS22T_F_40EB_DIRECT_CH_0D              = 0x0D,
    CUS22T_F_40EB_DIRECT_CH_0E              = 0x0E,
    CUS22T_F_40EB_DIRECT_CH_0F              = 0x0F,
    CUS22T_F_40EB_DIRECT_CH_10              = 0x10,
    CUS22T_F_40EB_DIRECT_CH_11              = 0x11,
    CUS22T_F_40EB_DIRECT_CH_12              = 0x12,
    CUS22T_F_40EB_DIRECT_CH_13              = 0x13,
    CUS22T_F_40EB_DIRECT_CH_14              = 0x14,
    CUS22T_F_40EB_DIRECT_CH_15              = 0x15,
    CUS22T_F_40EB_DIRECT_CH_16              = 0x16,
    CUS22T_F_40EB_DIRECT_CH_17              = 0x17,
    CUS22T_F_40EB_DIRECT_CH_18              = 0x18,
    CUS22T_F_40EB_DIRECT_CH_19              = 0x19,
    CUS22T_F_40EB_DIRECT_CH_1A              = 0x1A,
    CUS22T_F_40EB_DIRECT_CH_1B              = 0x1B,
    CUS22T_F_40EB_DIRECT_CH_1C              = 0x1C,
    CUS22T_F_40EB_DIRECT_CH_1D              = 0x1D,
    CUS22T_F_40EB_DIRECT_CH_1E              = 0x1E,
    CUS22T_F_40EB_DIRECT_CH_1F              = 0x1F,
    CUS22T_F_40EB_DIRECT_CH_20              = 0x20,
    CUS22T_F_40EB_DIRECT_CH_21              = 0x21,
    CUS22T_F_40EB_DIRECT_CH_22              = 0x22,
    CUS22T_F_40EB_DIRECT_CH_23              = 0x23,
    CUS22T_F_40EB_DIRECT_CH_24              = 0x24,
    CUS22T_F_40EB_DIRECT_CH_25              = 0x25,
    CUS22T_F_40EB_DIRECT_CH_26              = 0x26,
    CUS22T_F_40EB_DIRECT_CH_27              = 0x27,
    CUS22T_F_40EB_DIRECT_CH_28              = 0x28,
    CUS22T_F_40EB_DIRECT_CH_29              = 0x29,
    CUS22T_F_40EB_DIRECT_CH_2A              = 0x2A,
    CUS22T_F_40EB_DIRECT_CH_2B              = 0x2B,
    CUS22T_F_40EB_DIRECT_CH_2C              = 0x2C,
    CUS22T_F_40EB_DIRECT_CH_2D              = 0x2D,
    CUS22T_F_40EB_DIRECT_CH_2E              = 0x2E,
    CUS22T_F_40EB_DIRECT_CH_2F              = 0x2F,
    CUS22T_F_40EB_DIRECT_CH_30              = 0x30,
    CUS22T_F_40EB_DIRECT_CH_31              = 0x31,
    CUS22T_F_40EB_DIRECT_CH_32              = 0x32,
    CUS22T_F_40EB_DIRECT_CH_33              = 0x33,
    CUS22T_F_40EB_DIRECT_CH_34              = 0x34,
    CUS22T_F_40EB_DIRECT_CH_35              = 0x35,
    CUS22T_F_40EB_DIRECT_CH_36              = 0x36,
    CUS22T_F_40EB_DIRECT_CH_37              = 0x37,
    CUS22T_F_40EB_DIRECT_CH_38              = 0x38,
    CUS22T_F_40EB_DIRECT_CH_39              = 0x39,
    CUS22T_F_40EB_DIRECT_CH_3A              = 0x3A,
    CUS22T_F_40EB_DIRECT_CH_3B              = 0x3B,
    CUS22T_F_40EB_DIRECT_CH_3C              = 0x3C,
    CUS22T_F_40EB_DIRECT_CH_3D              = 0x3D,
    CUS22T_F_40EB_DIRECT_CH_3E              = 0x3E,
    CUS22T_F_40EB_DIRECT_CH_3F              = 0x3F,
    CUS22T_F_40EB_DIRECT_CH_40              = 0x40,
    CUS22T_F_40EB_DIRECT_CH_41              = 0x41,
    CUS22T_F_40EB_DIRECT_CH_42              = 0x42,
    CUS22T_F_40EB_DIRECT_CH_43              = 0x43,
    CUS22T_F_40EB_DIRECT_CH_44              = 0x44,
    CUS22T_F_40EB_DIRECT_CH_45              = 0x45,
    CUS22T_F_40EB_DIRECT_CH_46              = 0x46,
    CUS22T_F_40EB_DIRECT_CH_47              = 0x47,
    CUS22T_F_40EB_DIRECT_CH_48              = 0x48,
    CUS22T_F_40EB_DIRECT_CH_49              = 0x49,
    CUS22T_F_40EB_DIRECT_CH_4A              = 0x4A,
    CUS22T_F_40EB_DIRECT_CH_4B              = 0x4B,
    CUS22T_F_40EB_DIRECT_CH_4C              = 0x4C,
    CUS22T_F_40EB_DIRECT_CH_4D              = 0x4D,
    CUS22T_F_40EB_DIRECT_CH_4E              = 0x4E,
    CUS22T_F_40EB_DIRECT_CH_4F              = 0x4F,
    CUS22T_F_40EB_DIRECT_CH_50              = 0x50,
    CUS22T_F_40EB_DIRECT_CH_51              = 0x51,
    CUS22T_F_40EB_DIRECT_CH_52              = 0x52,
    CUS22T_F_40EB_DIRECT_CH_53              = 0x53,
    CUS22T_F_40EB_DIRECT_CH_54              = 0x54,
    CUS22T_F_40EB_DIRECT_CH_55              = 0x55,
    CUS22T_F_40EB_DIRECT_CH_56              = 0x56,
    CUS22T_F_40EB_DIRECT_CH_57              = 0x57,
    CUS22T_F_40EB_DIRECT_CH_58              = 0x58,
    CUS22T_F_40EB_DIRECT_CH_59              = 0x59,
    CUS22T_F_40EB_DIRECT_CH_5A              = 0x5A,
    CUS22T_F_40EB_DIRECT_CH_5B              = 0x5B,
    CUS22T_F_40EB_DIRECT_CH_5C              = 0x5C,
    CUS22T_F_40EB_DIRECT_CH_5D              = 0x5D,
    CUS22T_F_40EB_DIRECT_CH_5E              = 0x5E,
    CUS22T_F_40EB_DIRECT_CH_5F              = 0x5F,
    CUS22T_F_40EB_DIRECT_CH_60              = 0x60,
    CUS22T_F_40EB_DIRECT_CH_61              = 0x61,
    CUS22T_F_40EB_DIRECT_CH_62              = 0x62,
    CUS22T_F_40EB_DIRECT_CH_63              = 0x63,
    CUS22T_F_40EB_DIRECT_CH_64              = 0x64,
    CUS22T_F_40EB_DIRECT_RF                 = 0x80,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT1         = 0x81,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT2         = 0x82,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT3         = 0x83,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT4         = 0x84,     //DTV
    CUS22T_F_40EB_DIRECT_VIDEO_EXT5         = 0x85,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT6         = 0x86,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT7         = 0x87,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT8         = 0x88,
    CUS22T_F_40EB_DIRECT_VIDEO_EXT9         = 0x89,
    CUS22T_F_40EB_SIDE_SHARED_AUDEO_EXT2    = 0xF3,
    CUS22T_F_40EB_SIDE_SHARED_AUDIO_EXT3    = 0xF4,
}IR_CUS22T_FACTORY_40EB_TABLE;

typedef enum _IR_CUS22T_FACTORY_40EE_TABLE
{
    // 0x40EEXXXX
    CUS22T_F_40EE_ALL_CH                    = 0x00,
    CUS22T_F_40EE_M_MODE                    = 0x01,
    CUS22T_F_40EE_INITIALIZATION            = 0x02,
    CUS22T_F_40EE_WLAN_TEST_FULL            = 0x04,
    CUS22T_F_40EE_PICTURE_MODE_DYNAMIC      = 0x07,
    CUS22T_F_40EE_LED_MANUAL_CK             = 0x0A,
    CUS22T_F_40EE_M_OSD_OFF                 = 0xF1,
    CUS22T_F_40EE_DLTV_FACTORY_SIGNAL_SETTING   = 0x0B, //DTV
    CUS22T_F_40EE_TJP_FACTORY_SIGNAL_SETTING= 0x0C,
    CUS22T_F_40EE_3D_ON_SIDE_BY_SIDE        = 0x11,
    CUS22T_F_40EE_REVIC_CHECK               = 0x12,
    CUS22T_F_40EE_PLAYBACK_4K_ON_SD         = 0x13,
    CUS22T_F_40EE_CI_FACTORY_TEST           = 0x15,     //DTV
    CUS22T_F_40EE_DATA_1_UP                 = 0x1C,
    CUS22T_F_40EE_DATA_1_DN                 = 0x1D,
    CUS22T_F_40EE_DATA_4_UP                 = 0x1E,
    CUS22T_F_40EE_DATA_4_DN                 = 0x1F,
    CUS22T_F_40EE_2D_3D_CONVERSION          = 0x24,
    CUS22T_F_40EE_AGING_MODE_1              = 0x30,
    CUS22T_F_40EE_AGING_MODE_2              = 0x31,
    CUS22T_F_40EE_AUDIO_BALANCE_TOGGLE      = 0x39,
    CUS22T_F_40EE_LED_CHECK                 = 0x3C,
    CUS22T_F_40EE_4_3_STRETCH_TOGGLE        = 0x3D,
    CUS22T_F_40EE_IR_BLASTER_CHECK          = 0x41,
    CUS22T_F_40EE_POLARIZATION_H_V_TOGGLE   = 0x51,     //DTV
    CUS22T_F_40EE_DNR_SELECT                = 0x52,
    CUS22T_F_40EE_AUTO_BRIGHTNESS_SENSOR    = 0x53,
    CUS22T_F_40EE_AUTO_BRIGHTNESS_SENSOR_DETECT_VALUE_DISPLAY   = 0x54,
    CUS22T_F_40EE_EDID_DATA_DOWNLOAD        = 0x57,
    CUS22T_F_40EE_STABLE_SOUND_ON_OFF       = 0x5D,
    CUS22T_F_40EE_CLEAR_SCAN_100_ON_OFF     = 0x5F,
    CUS22T_F_40EE_RESOLUTION_PLUS_ON_OFF    = 0x60,     //DTV
    CUS22T_F_40EE_CLEAR_SCAN_200_PRO_ON_OFF = 0x63,
    CUS22T_F_40EE_KEY_WRITING_VIA_USB_MEMORY = 0x66,
    CUS22T_F_40EE_LVDS_OUTPUT_REVERSE       = 0x6B,     //DTV
    CUS22T_F_40EE_DVBC_TUNNING_RUSSIA       = 0x6E,
    CUS22T_F_40EE_MEDIA_PLAYER_MOVIE        = 0x70,
    CUS22T_F_40EE_BASE_BOOST_ON_OFF         = 0x73,
    CUS22T_F_40EE_DVB_C_MANUAL_TUNING       = 0x76,     //DTV
    CUS22T_F_40EE_RUSSIA_PRESET             = 0x79,     //DTV
    CUS22T_F_40EE_BLACK_WHITE_LEVEL         = 0x7A,
    CUS22T_F_40EE_NEWZEALAND_PRESET         = 0x7C,
    CUS22T_F_40EE_DLNA_TEST                 = 0x7F,
    CUS22T_F_40EE_FRANCE_PRESET             = 0x80,     //DTV
    CUS22T_F_40EE_VORDIC_PRESET             = 0x81,     //DTV
    CUS22T_F_40EE_MAC_CHK                   = 0x83,     //DTV
    CUS22T_F_40EE_FACTORY_TUNING            = 0x84,     //DTV
    CUS22T_F_40EE_FACTORY_SETUP             = 0x85,     //DTV
    CUS22T_F_40EE_COLOR_TEMPERATURE         = 0x86,
    CUS22T_F_40EE_PEQ_SETTING               = 0x8D,
    CUS22T_F_40EE_SPEAKER_CHECK             = 0x98,     //SPEAKER
    CUS22T_F_40EE_HDMI_ARC_OUTPUT_ENABLE    = 0x99,     //DTV
    CUS22T_F_40EE_LOCAL_DIMMING_CHECK       = 0x9B,
    CUS22T_F_40EE_BLACK_LIGHT_CHECK         = 0x9D,
    CUS22T_F_40EE_DEFAULT_MAC_IP            = 0x9F,
    CUS22T_F_40EE_MAC_ADDRESS_DELETE        = 0xA0,
    CUS22T_F_40EE_FACTORY_CLASSIS_SETTING   = 0xA2,     //DTV
    CUS22T_F_40EE_ADC_ADJUSTMENT            = 0xA3,
    CUS22T_F_40EE_WRITE_PROTECT             = 0xA5,
    CUS22T_F_40EE_VGA_EDID_DOWNLOAD         = 0xAB,
    CUS22T_F_40EE_TEMP_IO_CHECK             = 0xAC,
    CUS22T_F_40EE_DUAL_1                    = 0xAD,
    CUS22T_F_40EE_MONO_MODE                 = 0xAE,
    CUS22T_F_40EE_STEREO_MODE               = 0xAF,
    CUS22T_F_40EE_DUAL_2                    = 0xB0,
    CUS22T_F_40EE_DEFAULT_IP_ADDRESS        = 0xB5,
    CUS22T_F_40EE_HDMI_2_AUDIO              = 0xB8,
    CUS22T_F_40EE_SHIPOUT_SETTING           = 0xB9,
    CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_ESTEC = 0xBA,
    CUS22T_F_40EE_HDMI_2_AUDIO_SELECT       = 0xBF,
    CUS22T_F_40EE_COMPAL_CHANNEL_PRESET     = 0xC0,
    CUS22T_F_40EE_FULL_POWER_MODE           = 0xC1,
    CUS22T_F_CLEAR_POWER_ON_TIME            = 0xC2,
    CUS22T_F_40EE_S_MODE                    = 0xC3,
    CUS22T_F_40EE_SET_REGION_TO_USA         = 0xC4,
    CUS22T_F_40EE_SET_REGION_TO_CANADA      = 0xC5,
    CUS22T_F_40EE_SET_REGION_TO_MEXICO      = 0xC6,
    CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_CM = 0xC7,
    CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_IEC = 0xC8,
    CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_GW = 0xC9,
    CUS22T_F_40EE_SET_STORE_TO_NONE         = 0xCA,
    CUS22T_F_40EE_SET_STORE_TO_BEST_BUY     = 0xCB,
    CUS22T_F_40EE_SIDE_PANEL_SELECT         = 0xCC,
    CUS22T_F_40EE_PICTURE_SIZE_POSITION     = 0xCD,
    CUS22T_F_40EE_CLONE_COPY                = 0xCE,
    CUS22T_F_40EE_FULL_STANDBY              = 0xCF,
    CUS22T_F_40EE_SET_STORE_TO_COSTCO       = 0xD0,
    CUS22T_F_40EE_SET_STORE_TO_WALMART      = 0xD1,
    CUS22T_F_40EE_MPEG_NR                   = 0xD2,
    CUS22T_F_40EE_HDMI_ARC_OUTPUT_DISABLE   = 0xD3,     //DTV
    CUS22T_F_40EE_DVB_S_CHANNEL_PRESET      = 0xD4,     //DTV
    CUS22T_F_40EE_DISEQC_SWITCHING          = 0xD5,     //DTV
    CUS22T_F_40EE_SIGNAL_BOOSTER_ON_OFF     = 0xD8,
    CUS22T_F_40EE_PICTURE_RESET             = 0xD9,
    CUS22T_F_40EE_22KHZ_TONE_ON_OFF         = 0xDA,     //DTV
    CUS22T_F_40EE_DIGITAL_AUDIO_OUT         = 0xE6,
    CUS22T_F_40EE_SERIAL_DEBUG_ON           = 0xE7,
    CUS22T_F_40EE_CHANNEL_22                = 0xE8,     //DTV
    CUS22T_F_40EE_CHANNEL_67                = 0xE9,     //DTV
    CUS22T_F_40EE_DIGITAL_SIGNAL_SEARCH     = 0xEA,     //DTV
    CUS22T_F_40EE_CHANNEL_37                = 0xEB,     //DTV T2
    CUS22T_F_40EE_CHANNEL_45                = 0xEC,     //DTV
}IR_CUS22T_FACTORY_40EE_TABLE;

typedef enum _IR_CUS22T_FACTORY_40F0_TABLE
{
    // 0x40F0XXXX
    CUS22T_F_40F0_EEPROM_MEM_DATE_UPDATE    = 0x00,
}IR_CUS22T_FACTORY_40F0_TABLE;

typedef enum _IR_CUS22T_FACTORY_40F1_TABLE
{
    // 0x40F1XXXX
    CUS22T_F_40F1_EEPROM_MEM_DATE_UPDATE    = 0x00,
}IR_CUS22T_FACTORY_40F1_TABLE;

typedef enum _IR_CUS22T_FACTORY_40F2_TABLE
{
    // 0x40F2XXXX
    CUS22T_F_40F2_EEPROM_MEM_DATE_UPDATE    = 0x00,
}IR_CUS22T_FACTORY_40F2_TABLE;

typedef enum _IR_CUS22T_FACTORY_CFXX_TABLE
{
    // 0xCXXXXXXX
    CUS22T_F_CFXX_WHITE_WINDOW_ON           = 0x00,
    CUS22T_F_CFXX_70IRE_BRIGHTNESS          = 0xA9,
}IR_CUS22T_FACTORY_CFXX_TABLE;

typedef enum _IR_CUS22T_FACTORY_D0XX_TABLE
{
    // 0xDXXXXXXX
    CUS22T_F_D0XX_WHITE_WINDOW_OFF          = 0x00,
}IR_CUS22T_FACTORY_D0XX_TABLE;

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE || IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
typedef enum _IR_CUS22T_SWDECODE_TABLE
{
    // Customer Header
    CUS22T_SW_IRKEY_40B0_COMMAND            = 0X40B00000,
    CUS22T_SW_IRKEY_40B1_COMMAND            = 0X40B10000,
    CUS22T_SW_IRKEY_40B2_COMMAND            = 0X40B20000,
    CUS22T_SW_IRKEY_40B3_COMMAND            = 0X40B30000,
    CUS22T_SW_IRKEY_40B4_COMMAND            = 0X40B40000,
    CUS22T_SW_IRKEY_40BE_COMMAND            = 0X40BE0000,
    CUS22T_SW_IRKEY_40BF_COMMAND            = 0X40BF0000,
    // Factury Header
    CUS22T_SW_IRKEY_40E1_COMMAND            = 0X40E10000,
    CUS22T_SW_IRKEY_40E2_COMMAND            = 0X40E20000,
    CUS22T_SW_IRKEY_40E3_COMMAND            = 0X40E30000,
    CUS22T_SW_IRKEY_40EA_COMMAND            = 0X40EA0000,
    CUS22T_SW_IRKEY_40EB_COMMAND            = 0X40EB0000,
    CUS22T_SW_IRKEY_40EE_COMMAND            = 0X40EE0000,
    CUS22T_SW_IRKEY_40F0_COMMAND            = 0X40F00000,
    CUS22T_SW_IRKEY_40F1_COMMAND            = 0X40F10000,
    CUS22T_SW_IRKEY_40F2_COMMAND            = 0X40F20000,
    //Compal Factory
    CUS22T_SW_IRKEY_51A1_COMMAND            = 0X51A10000,
    CUS22T_SW_IRKEY_51A2_COMMAND            = 0X51A20000,
    CUS22T_SW_IRKEY_51A3_COMMAND            = 0X51A30000,
    CUS22T_SW_IRKEY_51A4_COMMAND            = 0X51A40000,
    CUS22T_SW_IRKEY_51A5_COMMAND            = 0X51A50000,
    CUS22T_SW_IRKEY_51A6_COMMAND            = 0X51A60000,
    CUS22T_SW_IRKEY_51A7_COMMAND            = 0X51A70000,
    CUS22T_SW_IRKEY_51AE_COMMAND            = 0X51AE0000,
    //
    CUS22T_SW_IRKEY_CF00_COMMAND            = 0XCF000FFF,
    CUS22T_SW_IRKEY_D000_COMMAND            = 0XD0000000,

    // -- General ------------------------------------------------------------------------
    CUS22T_SW_IRKEY_CHANNEL_LIST            = CUS22T_IRKEY_DUMY-2,
    CUS22T_SW_IRKEY_CHANNEL_FAV_LIST        = CUS22T_IRKEY_DUMY-3,
    CUS22T_SW_IRKEY_CHANNEL_RETURN          = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_CHANNEL_RETURN << 8)), // PREVIOUS
    CUS22T_SW_IRKEY_CHANNEL_PLUS            = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_CHANNEL_PLUS << 8)),
    CUS22T_SW_IRKEY_CHANNEL_MINUS           = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_CHANNEL_MINUS << 8)),

    CUS22T_SW_IRKEY_AUDIO                   = CUS22T_IRKEY_DUMY-4,
    CUS22T_SW_IRKEY_VOLUME_PLUS             = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_VOLUME_PLUS << 8)),
    CUS22T_SW_IRKEY_VOLUME_MINUS            = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_VOLUME_MINUS << 8)),

    CUS22T_SW_IRKEY_UP                      = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_UP << 8)),
    CUS22T_SW_IRKEY_POWER                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_POWER << 8)),
    CUS22T_SW_IRKEY_EXIT                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_EXIT << 8)),
    CUS22T_SW_IRKEY_MENU                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MENU << 8)),
    CUS22T_SW_IRKEY_DOWN                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_DOWN << 8)),
    CUS22T_SW_IRKEY_LEFT                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_LEFT << 8)),
    CUS22T_SW_IRKEY_SELECT                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_SELECT << 8)), // OK
    CUS22T_SW_IRKEY_RIGHT                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_RIGHT << 8)),

    CUS22T_SW_IRKEY_NUM_0                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_0 << 8)),
    CUS22T_SW_IRKEY_NUM_1                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_1 << 8)),
    CUS22T_SW_IRKEY_NUM_2                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_2 << 8)),
    CUS22T_SW_IRKEY_NUM_3                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_3 << 8)),
    CUS22T_SW_IRKEY_NUM_4                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_4 << 8)),
    CUS22T_SW_IRKEY_NUM_5                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_5 << 8)),
    CUS22T_SW_IRKEY_NUM_6                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_6 << 8)),
    CUS22T_SW_IRKEY_NUM_7                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_7 << 8)),
    CUS22T_SW_IRKEY_NUM_8                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_8 << 8)),
    CUS22T_SW_IRKEY_NUM_9                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_NUM_9 << 8)),

    CUS22T_SW_IRKEY_MUTE                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MUTE << 8)),
    CUS22T_SW_IRKEY_PAGE_UP                 = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PAGE_UP << 8)),
    CUS22T_SW_IRKEY_PAGE_DOWN               = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PAGE_DOWN << 8)),
    CUS22T_SW_IRKEY_PAGE_LEFT               = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PAGE_LEFT << 8)),
    CUS22T_SW_IRKEY_PAGE_RIGHT              = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PAGE_RIGHT << 8)),
    CUS22T_SW_IRKEY_CLOCK                   = CUS22T_IRKEY_DUMY-6,

    CUS22T_SW_IRKEY_QUICK                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_QUICK << 8)),

    CUS22T_SW_IRKEY_INFO                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INFO << 8)),
    CUS22T_SW_IRKEY_RED                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_RED << 8)),
    CUS22T_SW_IRKEY_GREEN                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_GREEN << 8)),
    CUS22T_SW_IRKEY_YELLOW                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_YELLOW << 8)),
    CUS22T_SW_IRKEY_BLUE                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_BLUE << 8)),
    CUS22T_SW_IRKEY_MTS                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MTS << 8)),
    CUS22T_SW_IRKEY_CC                      = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_CC << 8)),
    CUS22T_SW_IRKEY_INPUT_SOURCE            = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT_SOURCE << 8)),
    CUS22T_SW_IRKEY_PICTURE                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE << 8)),
    CUS22T_SW_IRKEY_ZOOM                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_ZOOM << 8)),
    CUS22T_SW_IRKEY_DASH                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_DASH << 8)),
    CUS22T_SW_IRKEY_SLEEP                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_SLEEP << 8)),
    CUS22T_SW_IRKEY_EPG                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_EPG << 8)),
    CUS22T_SW_IRKEY_FREEZE                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_FREEZE<< 8)),

    CUS22T_SW_IRKEY_MIX                     = CUS22T_IRKEY_DUMY-7,
    CUS22T_SW_IRKEY_INDEX                   = CUS22T_IRKEY_DUMY-8,
    //CUS22T_SW_IRKEY_PREVIOUS              = CUS22T_IRKEY_DUMY,
    //CUS22T_SW_IRKEY_NEXT                  = CUS22T_IRKEY_DUMY,
    //CUS22T_SW_IRKEY_BRIGHTNESS_PLUS       = CUS22T_IRKEY_DUMY,
    //CUS22T_SW_IRKEY_BRIGHTNESS_MINUS      = CUS22T_IRKEY_DUMY,
    CUS22T_SW_IRKEY_FORWARD                 = CUS22T_IRKEY_DUMY-9,
    CUS22T_SW_IRKEY_RECORD                  = CUS22T_IRKEY_DUMY-10,

    CUS22T_SW_IRKEY_TEXT                    = CUS22T_IRKEY_DUMY-11, // TEXT
    CUS22T_SW_IRKEY_MEDIA_PLAYER            = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_MEDIA_PLAYER << 8)),// MEDIA PLAYER
    CUS22T_SW_IRKEY_SUBTITLE                = CUS22T_IRKEY_DUMY-12, // SUBTITLE
    CUS22T_SW_IRKEY_TOSHIBA_PLACES          = CUS22T_IRKEY_DUMY-13, // TOSHIBA PLACES

    CUS22T_SW_IRKEY_REW                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_REW << 8)),         // REW/HOLD
    CUS22T_SW_IRKEY_PLAY                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PLAY << 8)),        // PLAY
    CUS22T_SW_IRKEY_FF                      = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_FF << 8)),          // FF/FULL-TOP-BOTTOM
    CUS22T_SW_IRKEY_SKIP_MINUS              = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_SKIP_MINUS << 8)),  // SKIP-/REVEAL
    CUS22T_SW_IRKEY_STOP                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_STOP << 8)),        // STOP
    CUS22T_SW_IRKEY_PAUSE                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PAUSE << 8)),       // AUSE/STEP/STILL
    CUS22T_SW_IRKEY_SKIP_PLUS               = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_SKIP_PLUS << 8)),   // SKIP+/TIME/TIME/TXCL

    CUS22T_SW_IRKEY_TTX_MODE                = CUS22T_IRKEY_DUMY-14, // TEXT SUB PAGE
    CUS22T_SW_IRKEY_RED2                    = CUS22T_IRKEY_DUMY-18,
    CUS22T_SW_IRKEY_GREEN2                  = CUS22T_IRKEY_DUMY-19,
    CUS22T_SW_IRKEY_UPDATE                  = CUS22T_IRKEY_DUMY-20,
    //CUS22T_SW_IRKEY_SUBCODE               = CUS22T_IRKEY_DUMY,
    //CUS22T_SW_IRKEY_TIME                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_TIME << 8)),
    CUS22T_SW_IRKEY_MEMORY_RECALL           = CUS22T_IRKEY_DUMY-21,
    CUS22T_SW_IRKEY_SIZE                    = CUS22T_IRKEY_DUMY-22,
    //CUS22T_SW_IRKEY_REVEAL                = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_REVEAL << 8)),
    CUS22T_SW_IRKEY_RETURN                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_RETURN << 8)),
    CUS22T_SW_IRKEY_TV_INPUT                = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_TV_INPUT << 8)),
    CUS22T_SW_IRKEY_KEY_DISABLE_KEYPAD      = CUS22T_IRKEY_DUMY-24,
    CUS22T_SW_IRKEY_BACK                    = CUS22T_IRKEY_DUMY-25,

    CUS22T_SW_IRKEY_STILL                   = CUS22T_IRKEY_DUMY-15, //STILL
    CUS22T_SW_IRKEY_REC                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_REC << 8)), //REC
    CUS22T_SW_IRKEY_EJECT                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_EJECT << 8)), //EJECT
    CUS22T_SW_IRKEY_FAV                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_FAV << 8)), //FAV
    CUS22T_SW_IRKEY_RECALL                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_RECALL << 8)), //RECALL

    CUS22T_SW_IRKEY_SUBPAGE                 = CUS22T_IRKEY_DUMY-27,
    CUS22T_SW_IRKEY_DTV                     = CUS22T_IRKEY_DUMY-28,
    CUS22T_SW_IRKEY_TV                      = CUS22T_IRKEY_DUMY-29,
    CUS22T_SW_IRKEY_SCART                   = CUS22T_IRKEY_DUMY-30,
    CUS22T_SW_IRKEY_HDMI                    = CUS22T_IRKEY_DUMY-31,
    CUS22T_SW_IRKEY_PC                      = CUS22T_IRKEY_DUMY-32,
    CUS22T_SW_IRKEY_COMPONENT               = CUS22T_IRKEY_DUMY-33,
    CUS22T_SW_IRKEY_AV                      = CUS22T_IRKEY_DUMY-34,
    CUS22T_SW_IRKEY_SV                      = CUS22T_IRKEY_DUMY-35,

    CUS22T_SW_IRKEY_SERVICE_MODE            = CUS22T_IRKEY_DUMY-36,
    CUS22T_SW_IRKEY_DESIGN_MODE             = CUS22T_IRKEY_DUMY-37,
    CUS22T_SW_IRKEY_HOTEL_CLONE             = CUS22T_IRKEY_DUMY-38,

    // Add for compile error
    CUS22T_SW_IRKEY_TV_RADIO                = CUS22T_IRKEY_DUMY-39,
    CUS22T_SW_IRKEY_NINE_LATTICE            = CUS22T_IRKEY_DUMY-40,
    CUS22T_SW_IRKEY_CRADRD                  = CUS22T_IRKEY_DUMY-41,
    CUS22T_SW_IRKEY_PIP                     = CUS22T_IRKEY_DUMY-42,
    CUS22T_SW_IRKEY_SUBCODE                 = CUS22T_IRKEY_DUMY-43,

    CUS22T_SW_IRKEY_3D                      = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_3D << 8)),

    CUS22T_SW_IRKEY_MUTE_ON                 = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_MUTE_ON << 8)),     //MUTE ON
    CUS22T_SW_IRKEY_MUTE_OFF                = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_MUTE_OFF << 8)),     //MUTE OFF

    // -- REGZA Link ------------------------------------------------------------------------
    CUS22T_SW_R_40BE_POWER                  = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_POWER << 8)),
    CUS22T_SW_R_40BE_AUDIO                  = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_AUDIO << 8)),
    CUS22T_SW_R_40BE_SETUP                  = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_SETUP << 8)),
    CUS22T_SW_R_40BE_LOAD                   = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_LOAD << 8)),
    // MENU
    CUS22T_SW_R_40BE_ROOT                   = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_ROOT << 8)),
    CUS22T_SW_R_40BE_CONTENTS               = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_CONTENTS << 8)),
    CUS22T_SW_R_40BE_TOP                    = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_TOP << 8)),
    CUS22T_SW_R_40BE_DVD                    = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_40BE_DVD << 8)),

    // -- Factory ------------------------------------------------------------------------
    // 0x40BEXXXX
    CUS22T_SW_F_40BE_PC_INPUT_SELECTION     = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_F_40BE_PC_INPUT_SELECTION << 8)),
    // 0x40BFXXXX
    CUS22T_SW_F_40BF_ATV                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_ATV << 8)),
    CUS22T_SW_F_40BF_DTV                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_DTV << 8)),
    CUS22T_SW_F_40BF_VIDEO1                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VIDEO1 << 8)),
    CUS22T_SW_F_40BF_VIDEO2                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VIDEO2 << 8)),
    CUS22T_SW_F_40BF_VIDEO3                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VIDEO3 << 8)),
    CUS22T_SW_F_40BF_VIDEO4                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VIDEO4 << 8)),
    CUS22T_SW_F_40BF_VIDEO5                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VIDEO5 << 8)),
    CUS22T_SW_F_40BF_VIDEO6                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VIDEO6 << 8)),
    CUS22T_SW_F_40BF_VIDEO7                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VIDEO7 << 8)),
    CUS22T_SW_F_40BF_HDMI1                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_HDMI1 << 8)),
    CUS22T_SW_F_40BF_HDMI2                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_HDMI2 << 8)),
    CUS22T_SW_F_40BF_HDMI3                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_HDMI3 << 8)),
    CUS22T_SW_F_40BF_HDMI4                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_HDMI4 << 8)),
    CUS22T_SW_F_40BF_PC                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_PC << 8)),
    CUS22T_SW_F_40BF_PIP_ON_OFF             = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_PIP_ON_OFF << 8)),
    CUS22T_SW_F_40BF_POWER_OFF              = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_POWER_OFF << 8)),
    CUS22T_SW_F_40BF_TUNE_DOWN_SEARCH       = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_TUNE_DOWN_SEARCH << 8)),
    CUS22T_SW_F_40BF_TUNE_UP_SEARCH         = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_TUNE_UP_SEARCH << 8)),
    CUS22T_SW_F_40BF_LNB_POWER_ON_OFF       = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_LNB_POWER_ON_OFF << 8)),    //DTV
    CUS22T_SW_F_40BF_MEM                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_MEM << 8)),
    CUS22T_SW_F_40BF_COLOR_SYS_CHECK        = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_COLOR_SYS_CHECK << 8)),
    CUS22T_SW_F_40BF_SOUND_SYS_CHECK        = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_SOUND_SYS_CHECK << 8)),
    CUS22T_SW_F_40BF_B_G_RRESET             = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_B_G_RRESET << 8)),          //DTV
    CUS22T_SW_F_40BF_I_PRESET               = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_I_PRESET << 8)),            //DTV
    CUS22T_SW_F_40BF_AUDYSSEY               = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_AUDYSSEY << 8)),
    CUS22T_SW_F_40BF_VXT2_PEAK              = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_VXT2_PEAK << 8)),
    CUS22T_SW_F_40BF_TUNER_STATUS_READ      = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_TUNER_STATUS_READ << 8)),
    CUS22T_SW_F_40BF_START_PE_MEASUREMENT   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_START_PE_MEASUREMENT << 8)),
    CUS22T_SW_F_40BF_STOP_PE_MEASUREMENT    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_STOP_PE_MEASUREMENT << 8)),
    CUS22T_SW_F_40BF_3D_TIMMING             = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_3D_TIMMING << 8)),
    CUS22T_SW_F_40BF_UART                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_UART << 8)),
    CUS22T_SW_F_40BF_E_D_MODE               = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_F_40BF_E_D_MODE << 8)),
    // 0x40E1XXXX

    // 0x40E2XXXX
    CUS22T_SW_F_40E2_CONTRAST                  = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_CONTRAST << 8)),
    CUS22T_SW_F_40E2_BRIGHT                    = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_BRIGHT << 8)),
    CUS22T_SW_F_40E2_COLOR                     = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_COLOR << 8)),
    CUS22T_SW_F_40E2_TINT                      = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_TINT << 8)),
    CUS22T_SW_F_40E2_SHARPNESS                 = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_SHARPNESS << 8)),
    CUS22T_SW_F_40E2_VOLUME                    = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_VOLUME << 8)),
    CUS22T_SW_F_40E2_BASS                      = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_BASS << 8)),
    CUS22T_SW_F_40E2_TREBLE                    = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_TREBLE << 8)),
    CUS22T_SW_F_40E2_BALANCE                   = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_BALANCE << 8)),
    CUS22T_SW_F_40E2_RCUT                      = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_RCUT << 8)),
    CUS22T_SW_F_40E2_GCUT                      = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_GCUT << 8)),
    CUS22T_SW_F_40E2_BCUT                      = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_BCUT << 8)),
    CUS22T_SW_F_40E2_RDRV                      = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_RDRV << 8)),
    CUS22T_SW_F_40E2_BDRV                      = (CUS22T_SW_IRKEY_40E2_COMMAND | (CUS22T_F_40E2_BDRV << 8)),
    // 0x40E3XXXX
    CUS22T_SW_F_40E3_RCUT                      = (CUS22T_SW_IRKEY_40E3_COMMAND | (CUS22T_F_40E3_RCUT << 8)),
    CUS22T_SW_F_40E3_GCUT                      = (CUS22T_SW_IRKEY_40E3_COMMAND | (CUS22T_F_40E3_GCUT << 8)),
    CUS22T_SW_F_40E3_BCUT                      = (CUS22T_SW_IRKEY_40E3_COMMAND | (CUS22T_F_40E3_BCUT << 8)),
    CUS22T_SW_F_40E3_RDRV_GDRV                 = (CUS22T_SW_IRKEY_40E3_COMMAND | (CUS22T_F_40E3_RDRV_GDRV << 8)),
    CUS22T_SW_F_40E3_BDRV                      = (CUS22T_SW_IRKEY_40E3_COMMAND | (CUS22T_F_40E3_BDRV << 8)),
    // 0x40EAXXXX
    CUS22T_SW_F_40EA_INTERNAL_PATTERN_OFF      = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_INTERNAL_PATTERN_OFF << 8)),
    CUS22T_SW_F_40EA_RED_RASTER                = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_RED_RASTER << 8)),
    CUS22T_SW_F_40EA_GREEN_RASTER              = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_GREEN_RASTER << 8)),
    CUS22T_SW_F_40EA_BLUE_RASTER               = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_BLUE_RASTER << 8)),
    CUS22T_SW_F_40EA_BLACK_RASTER              = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_BLACK_RASTER << 8)),
    CUS22T_SW_F_40EA_WHITE_RASTER              = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_WHITE_RASTER << 8)),
    CUS22T_SW_F_40EA_AGING_MODE_1              = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_AGING_MODE_1 << 8)),
    CUS22T_SW_F_40EA_AGINE_MODE_2              = (CUS22T_SW_IRKEY_40EA_COMMAND | (CUS22T_F_40EA_AGINE_MODE_2 << 8)),
    // 0x40EBXXXX
    CUS22T_SW_F_40EB_DIRECT_CH_00              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_00 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_01              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_01 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_02              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_02 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_03              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_03 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_04              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_04 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_05              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_05 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_06              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_06 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_07              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_07 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_08              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_08 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_09              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_09 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_0A              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_0A << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_0B              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_0B << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_0C              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_0C << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_0D              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_0D << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_0E              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_0E << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_0F              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_0F << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_10              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_10 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_11              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_11 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_12              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_12 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_13              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_13 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_14              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_14 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_15              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_15 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_16              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_16 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_17              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_17 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_18              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_18 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_19              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_19 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_1A              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_1A << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_1B              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_1B << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_1C              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_1C << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_1D              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_1D << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_1E              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_1E << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_1F              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_1F << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_20              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_20 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_21              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_21 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_22              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_22 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_23              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_23 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_24              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_24 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_25              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_25 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_26              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_26 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_27              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_27 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_28              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_28 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_29              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_29 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_2A              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_2A << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_2B              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_2B << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_2C              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_2C << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_2D              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_2D << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_2E              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_2E << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_2F              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_2F << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_30              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_30 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_31              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_31 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_32              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_32 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_33              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_33 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_34              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_34 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_35              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_35 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_36              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_36 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_37              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_37 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_38              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_38 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_39              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_39 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_3A              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_3A << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_3B              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_3B << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_3C              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_3C << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_3D              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_3D << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_3E              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_3E << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_3F              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_3F << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_40              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_40 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_41              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_41 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_42              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_42 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_43              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_43 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_44              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_44 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_45              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_45 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_46              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_46 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_47              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_47 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_48              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_48 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_49              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_49 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_4A              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_4A << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_4B              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_4B << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_4C              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_4C << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_4D              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_4D << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_4E              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_4E << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_4F              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_4F << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_50              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_50 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_51              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_51 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_52              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_52 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_53              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_53 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_54              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_54 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_55              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_55 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_56              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_56 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_57              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_57 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_58              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_58 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_59              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_59 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_5A              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_5A << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_5B              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_5B << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_5C              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_5C << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_5D              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_5D << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_5E              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_5E << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_5F              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_5F << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_60              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_60 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_61              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_61 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_62              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_62 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_63              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_63 << 8)),
    CUS22T_SW_F_40EB_DIRECT_CH_64              = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_CH_64 << 8)),
    CUS22T_SW_F_40EB_DIRECT_RF                 = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_RF << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT1         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT1 << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT2         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT2 << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT3         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT3 << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT4         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT4 << 8)),       //DTV
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT5         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT5 << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT6         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT6 << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT7         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT7 << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT8         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT8 << 8)),
    CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT9         = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_DIRECT_VIDEO_EXT9 << 8)),
    CUS22T_SW_F_40EB_SIDE_SHARED_AUDEO_EXT2     = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_SIDE_SHARED_AUDEO_EXT2 << 8)),
    CUS22T_SW_F_40EB_SIDE_SHARED_AUDIO_EXT3     = (CUS22T_SW_IRKEY_40EB_COMMAND | (CUS22T_F_40EB_SIDE_SHARED_AUDIO_EXT3 << 8)),
    // 0x40EEXXXX
    CUS22T_SW_F_40EE_ALL_CH                    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_ALL_CH << 8)),
    CUS22T_SW_F_40EE_M_MODE                    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_M_MODE << 8)),
    CUS22T_SW_F_40EE_INITIALIZATION            = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_INITIALIZATION << 8)),
    CUS22T_SW_F_40EE_WLAN_TEST_FULL            = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_WLAN_TEST_FULL << 8)),
    CUS22T_SW_F_40EE_PICTURE_MODE_DYNAMIC      = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_PICTURE_MODE_DYNAMIC << 8)),
    CUS22T_SW_F_40EE_LED_MANUAL_CK             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_LED_MANUAL_CK << 8)),
    CUS22T_SW_F_40EE_M_OSD_OFF                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_M_OSD_OFF << 8)),
    CUS22T_SW_F_40EE_DLTV_FACTORY_SIGNAL_SETTING    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DLTV_FACTORY_SIGNAL_SETTING << 8)),     //DTV
    CUS22T_SW_F_40EE_TJP_FACTORY_SIGNAL_SETTING= (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_TJP_FACTORY_SIGNAL_SETTING << 8)),
    CUS22T_SW_F_40EE_3D_ON_SIDE_BY_SIDE        = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_3D_ON_SIDE_BY_SIDE << 8)),
    CUS22T_SW_F_40EE_REVIC_CHECK               = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_REVIC_CHECK << 8)),
    CUS22T_SW_F_40EE_PLAYBACK_4K_ON_SD         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_PLAYBACK_4K_ON_SD << 8)),
    CUS22T_SW_F_40EE_CI_FACTORY_TEST           = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CI_FACTORY_TEST << 8)),         //DTV
    CUS22T_SW_F_40EE_DATA_1_UP                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DATA_1_UP << 8)),
    CUS22T_SW_F_40EE_DATA_1_DN                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DATA_1_DN << 8)),
    CUS22T_SW_F_40EE_DATA_4_UP                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DATA_4_UP << 8)),
    CUS22T_SW_F_40EE_DATA_4_DN                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DATA_4_DN << 8)),
    CUS22T_SW_F_40EE_2D_3D_CONVERSION          = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_2D_3D_CONVERSION << 8)),
    CUS22T_SW_F_40EE_AGING_MODE_1              = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_AGING_MODE_1 << 8)),
    CUS22T_SW_F_40EE_AGING_MODE_2              = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_AGING_MODE_2 << 8)),
    CUS22T_SW_F_40EE_AUDIO_BALANCE_TOGGLE      = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_AUDIO_BALANCE_TOGGLE << 8)),
    CUS22T_SW_F_40EE_LED_CHECK                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_LED_CHECK << 8)),
    CUS22T_SW_F_40EE_4_3_STRETCH_TOGGLE        = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_4_3_STRETCH_TOGGLE << 8)),
    CUS22T_SW_F_40EE_IR_BLASTER_CHECK          = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_IR_BLASTER_CHECK << 8)),
    CUS22T_SW_F_40EE_POLARIZATION_H_V_TOGGLE   = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_POLARIZATION_H_V_TOGGLE << 8)), //DTV
    CUS22T_SW_F_40EE_DNR_SELECT                = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DNR_SELECT << 8)),
    CUS22T_SW_F_40EE_AUTO_BRIGHTNESS_SENSOR    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_AUTO_BRIGHTNESS_SENSOR << 8)),
    CUS22T_SW_F_40EE_AUTO_BRIGHTNESS_SENSOR_DETECT_VALUE_DISPLAY   = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_AUTO_BRIGHTNESS_SENSOR_DETECT_VALUE_DISPLAY << 8)),
    CUS22T_SW_F_40EE_EDID_DATA_DOWNLOAD        = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_EDID_DATA_DOWNLOAD << 8)),
    CUS22T_SW_F_40EE_STABLE_SOUND_ON_OFF       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_STABLE_SOUND_ON_OFF << 8)),
    CUS22T_SW_F_40EE_CLEAR_SCAN_100_ON_OFF     = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CLEAR_SCAN_100_ON_OFF << 8)),
    CUS22T_SW_F_40EE_RESOLUTION_PLUS_ON_OFF    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_RESOLUTION_PLUS_ON_OFF << 8)),  //DTV
    CUS22T_SW_F_40EE_CLEAR_SCAN_200_PRO_ON_OFF = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CLEAR_SCAN_200_PRO_ON_OFF << 8)),
    CUS22T_SW_F_40EE_KEY_WRITING_VIA_USB_MEMORY = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_KEY_WRITING_VIA_USB_MEMORY << 8)),
    CUS22T_SW_F_40EE_LVDS_OUTPUT_REVERSE       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_LVDS_OUTPUT_REVERSE << 8)),
    CUS22T_SW_F_40EE_DVBC_TUNNING_RUSSIA       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DVBC_TUNNING_RUSSIA << 8)),
    CUS22T_SW_F_40EE_MEDIA_PLAYER_MOVIE        = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_MEDIA_PLAYER_MOVIE << 8)),
    CUS22T_SW_F_40EE_BASE_BOOST_ON_OFF         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_BASE_BOOST_ON_OFF << 8)),
    CUS22T_SW_F_40EE_DVB_C_MANUAL_TUNING       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DVB_C_MANUAL_TUNING << 8)),     //DTV
    CUS22T_SW_F_40EE_RUSSIA_PRESET             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_RUSSIA_PRESET << 8)),           //DTV
    CUS22T_SW_F_40EE_BLACK_WHITE_LEVEL         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_BLACK_WHITE_LEVEL << 8)),
    CUS22T_SW_F_40EE_NEWZEALAND_PRESET         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_NEWZEALAND_PRESET << 8)),
    CUS22T_SW_F_40EE_DLNA_TEST                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DLNA_TEST << 8)),
    CUS22T_SW_F_40EE_FRANCE_PRESET             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_FRANCE_PRESET << 8)),           //DTV
    CUS22T_SW_F_40EE_VORDIC_PRESET             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_VORDIC_PRESET << 8)),           //DTV
    CUS22T_SW_F_40EE_MAC_CHK                   = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_MAC_CHK << 8)),                 //DTV
    CUS22T_SW_F_40EE_FACTORY_TUNING            = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_FACTORY_TUNING << 8)),          //DTV
    CUS22T_SW_F_40EE_FACTORY_SETUP             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_FACTORY_SETUP << 8)),           //DTV
    CUS22T_SW_F_40EE_COLOR_TEMPERATURE         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_COLOR_TEMPERATURE << 8)),
    CUS22T_SW_F_40EE_HDMI_ARC_OUTPUT_ENABLE    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_HDMI_ARC_OUTPUT_ENABLE << 8)),  //DTV
    CUS22T_SW_F_40EE_LOCAL_DIMMING_CHECK       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_LOCAL_DIMMING_CHECK << 8)),  //DTV
    CUS22T_SW_F_40EE_BLACK_LIGHT_CHECK         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_BLACK_LIGHT_CHECK << 8)),
    CUS22T_SW_F_40EE_DEFAULT_MAC_IP            = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DEFAULT_MAC_IP << 8)),
    CUS22T_SW_F_40EE_MAC_ADDRESS_DELETE        = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_MAC_ADDRESS_DELETE << 8)),
    CUS22T_SW_F_40EE_FACTORY_CLASSIS_SETTING   = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_FACTORY_CLASSIS_SETTING << 8)), //DTV
    CUS22T_SW_F_40EE_ADC_ADJUSTMENT            = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_ADC_ADJUSTMENT << 8)),           //DTV
    CUS22T_SW_F_40EE_WRITE_PROTECT             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_WRITE_PROTECT << 8)),
    CUS22T_SW_F_40EE_VGA_EDID_DOWNLOAD          = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_VGA_EDID_DOWNLOAD << 8)),
    CUS22T_SW_F_40EE_TEMP_IO_CHECK              = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_TEMP_IO_CHECK << 8)),
    CUS22T_SW_F_40EE_DUAL_1                    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DUAL_1 << 8)),
    CUS22T_SW_F_40EE_MONO_MODE                 = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_MONO_MODE << 8)),
    CUS22T_SW_F_40EE_STEREO_MODE               = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_STEREO_MODE << 8)),
    CUS22T_SW_F_40EE_DUAL_2                    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DUAL_2 << 8)),
    CUS22T_SW_F_40EE_DEFAULT_IP_ADDRESS        = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DEFAULT_IP_ADDRESS << 8)),
    CUS22T_SW_F_40EE_HDMI_2_AUDIO              = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_HDMI_2_AUDIO << 8)),
    CUS22T_SW_F_40EE_SHIPOUT_SETTING           = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SHIPOUT_SETTING << 8)),
    CUS22T_SW_F_40EE_SELECT_SPEAKER_TYPE_TO_ESTEC = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_ESTEC << 8)),
    CUS22T_SW_F_40EE_HDMI_2_AUDIO_SELECT       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_HDMI_2_AUDIO_SELECT << 8)),
    CUS22T_SW_F_40EE_COMPAL_CHANNEL_PRESET     = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_COMPAL_CHANNEL_PRESET << 8)),
    CUS22T_SW_F_40EE_FULL_POWER_MODE           = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_FULL_POWER_MODE << 8)),
    CUS22T_SW_F_40EE_CLEAR_POWER_ON_TIME       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_CLEAR_POWER_ON_TIME << 8)),
    CUS22T_SW_F_40EE_S_MODE                    = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_S_MODE << 8)),
    CUS22T_SW_F_40EE_SET_REGION_TO_USA         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SET_REGION_TO_USA << 8)),
    CUS22T_SW_F_40EE_SET_REGION_TO_CANADA      = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SET_REGION_TO_CANADA << 8)),
    CUS22T_SW_F_40EE_SET_REGION_TO_MEXICO      = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SET_REGION_TO_MEXICO << 8)),
    CUS22T_SW_F_40EE_SELECT_SPEAKER_TYPE_TO_CM = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_CM << 8)),
    CUS22T_SW_F_40EE_SELECT_SPEAKER_TYPE_TO_IEC = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_IEC << 8)),
    CUS22T_SW_F_40EE_SELECT_SPEAKER_TYPE_TO_GW = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SELECT_SPEAKER_TYPE_TO_GW << 8)),
    CUS22T_SW_F_40EE_SET_STORE_TO_NONE         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SET_STORE_TO_NONE << 8)),
    CUS22T_SW_F_40EE_SET_STORE_TO_BEST_BUY     = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SET_STORE_TO_BEST_BUY << 8)),
    CUS22T_SW_F_40EE_SIDE_PANEL_SELECT         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SIDE_PANEL_SELECT << 8)),
    CUS22T_SW_F_40EE_PICTURE_SIZE_POSITION     = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_PICTURE_SIZE_POSITION << 8)),
    CUS22T_SW_F_40EE_CLONE_COPY                = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CLONE_COPY << 8)),
    CUS22T_SW_F_40EE_FULL_STANDBY              = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_FULL_STANDBY << 8)),
    CUS22T_SW_F_40EE_SET_STORE_TO_COSTCO       = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SET_STORE_TO_COSTCO << 8)),
    CUS22T_SW_F_40EE_SET_STORE_TO_WALMART      = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SET_STORE_TO_WALMART << 8)),
    CUS22T_SW_F_40EE_MPEG_NR                   = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_MPEG_NR << 8)),
    CUS22T_SW_F_40EE_HDMI_ARC_OUTPUT_DISABLE   = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_HDMI_ARC_OUTPUT_DISABLE << 8)), //DTV
    CUS22T_SW_F_40EE_DVB_S_CHANNEL_PRESET      = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DVB_S_CHANNEL_PRESET << 8)),    //DTV
    CUS22T_SW_F_40EE_DISEQC_SWITCHING          = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DISEQC_SWITCHING << 8)),        //DTV
    CUS22T_SW_F_40EE_SIGNAL_BOOSTER_ON_OFF     = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SIGNAL_BOOSTER_ON_OFF << 8)),
    CUS22T_SW_F_40EE_PICTURE_RESET             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_PICTURE_RESET << 8)),
    CUS22T_SW_F_40EE_22KHZ_TONE_ON_OFF         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_22KHZ_TONE_ON_OFF << 8)),       //DTV
    CUS22T_SW_F_40EE_DIGITAL_AUDIO_OUT         = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DIGITAL_AUDIO_OUT << 8)),
    CUS22T_SW_F_40EE_SERIAL_DEBUG_ON           = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SERIAL_DEBUG_ON << 8)),
    CUS22T_SW_F_40EE_CHANNEL_22                = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CHANNEL_22 << 8)),              //DTV
    CUS22T_SW_F_40EE_CHANNEL_67                = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CHANNEL_67 << 8)),              //DTV
    CUS22T_SW_F_40EE_DIGITAL_SIGNAL_SEARCH     = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_DIGITAL_SIGNAL_SEARCH << 8)),   //DTV
    CUS22T_SW_F_40EE_CHANNEL_37                = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CHANNEL_37 << 8)),              //DTV T2
    CUS22T_SW_F_40EE_CHANNEL_45                = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_CHANNEL_45 << 8)),                 //DTV
    CUS22T_SW_F_40EE_PEQ_SETTING               = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_PEQ_SETTING << 8)),
    CUS22T_SW_F_40EE_SPEAKER_CHECK             = (CUS22T_SW_IRKEY_40EE_COMMAND | (CUS22T_F_40EE_SPEAKER_CHECK << 8)),
    // 0x40F0XXXX
    CUS22T_SW_F_40F0_EEPROM_MEM_DATE_UPDATE    = (CUS22T_SW_IRKEY_40F0_COMMAND | (CUS22T_F_40F0_EEPROM_MEM_DATE_UPDATE << 8)),
    // 0x40F1XXXX
    CUS22T_SW_F_40F1_EEPROM_MEM_DATE_UPDATE    = (CUS22T_SW_IRKEY_40F1_COMMAND | (CUS22T_F_40F1_EEPROM_MEM_DATE_UPDATE << 8)),
    // 0x40F2XXXX
    CUS22T_SW_F_40F2_EEPROM_MEM_DATE_UPDATE    = (CUS22T_SW_IRKEY_40F2_COMMAND | (CUS22T_F_40F2_EEPROM_MEM_DATE_UPDATE << 8)),
    // 0xCXXXXXXX
    CUS22T_SW_F_CFXX_WHITE_WINDOW_ON           = (CUS22T_SW_IRKEY_CF00_COMMAND | (CUS22T_F_CFXX_WHITE_WINDOW_ON << 16)),
    CUS22T_SW_F_CFXX_70IRE_BRIGHTNESS          = (CUS22T_SW_IRKEY_CF00_COMMAND | (CUS22T_F_CFXX_70IRE_BRIGHTNESS << 16)),
    // 0xDXXXXXXX
    CUS22T_SW_F_D0XX_WHITE_WINDOW_OFF          = (CUS22T_SW_IRKEY_D000_COMMAND | (CUS22T_F_D0XX_WHITE_WINDOW_OFF << 8)),

    // TM13 NA Model Key Code
    // 0x40B0XXXX
    CUS22T_SW_IRKEY_CS_POWER                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_POWER << 8)),
    CUS22T_SW_IRKEY_CS_NUM_1                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_1 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_2                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_2 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_3                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_3 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_4                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_4 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_5                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_5 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_6                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_6 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_7                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_7 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_8                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_8 << 8)),
    CUS22T_SW_IRKEY_CS_NUM_9                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_9 << 8)),
    CUS22T_SW_IRKEY_CS_DASH                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_DASH << 8)),
    CUS22T_SW_IRKEY_CS_NUM_0                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_NUM_0 << 8)),
    CUS22T_SW_IRKEY_CS_CHANNEL_RETURN          = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_CHANNEL_RETURN << 8)),
    CUS22T_SW_IRKEY_CS_INFO                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_INFO << 8)),
    CUS22T_SW_IRKEY_CS_CHANNEL_PLUS            = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_CHANNEL_PLUS << 8)),
    CUS22T_SW_IRKEY_CS_CHANNEL_MINUS           = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_CHANNEL_MINUS << 8)),
    CUS22T_SW_IRKEY_CS_PAGE_UP                 = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_PAGE_UP << 8)),
    CUS22T_SW_IRKEY_CS_PAGE_DOWN               = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_PAGE_DOWN << 8)),
    CUS22T_SW_IRKEY_CS_PAGE_LEFT               = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_PAGE_LEFT << 8)),
    CUS22T_SW_IRKEY_CS_PAGE_RIGHT              = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_PAGE_RIGHT << 8)),
    CUS22T_SW_IRKEY_CS_QUICK                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_QUICK << 8)),
    CUS22T_SW_IRKEY_CS_EPG                     = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_EPG << 8)),
    CUS22T_SW_IRKEY_CS_RETURN                  = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_RETURN << 8)),
    CUS22T_SW_IRKEY_CS_EXIT                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_EXIT << 8)),
    CUS22T_SW_IRKEY_CS_UP                      = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_UP << 8)),
    CUS22T_SW_IRKEY_CS_DOWN                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_DOWN << 8)),
    CUS22T_SW_IRKEY_CS_LEFT                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_LEFT << 8)),
    CUS22T_SW_IRKEY_CS_RIGHT                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_RIGHT << 8)),
    CUS22T_SW_IRKEY_CS_SELECT                  = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_SELECT << 8)),
    CUS22T_SW_IRKEY_CS_REW                     = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_REW << 8)),
    CUS22T_SW_IRKEY_CS_PLAY                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_PLAY << 8)),
    CUS22T_SW_IRKEY_CS_FF                      = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_FF << 8)),
    CUS22T_SW_IRKEY_CS_SKIP_MINUS              = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_SKIP_MINUS << 8)),
    CUS22T_SW_IRKEY_CS_STOP                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_STOP << 8)),
    CUS22T_SW_IRKEY_CS_PAUSE                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_PAUSE << 8)),
    CUS22T_SW_IRKEY_CS_SKIP_PLUS               = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_SKIP_PLUS << 8)),
    CUS22T_SW_IRKEY_CS_REC                     = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_REC << 8)),
    CUS22T_SW_IRKEY_CS_RED                     = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_RED << 8)),
    CUS22T_SW_IRKEY_CS_GREEN                   = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_GREEN << 8)),
    CUS22T_SW_IRKEY_CS_YELLOW                  = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_YELLOW << 8)),
    CUS22T_SW_IRKEY_CS_BLUE                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_BLUE << 8)),
    CUS22T_SW_IRKEY_CS_MENU                    = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_MENU << 8)),
    CUS22T_SW_IRKEY_CS_MTS                     = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_MTS << 8)),
    CUS22T_SW_IRKEY_CS_CC                      = (CUS22T_SW_IRKEY_40B0_COMMAND | (CUS22T_IRKEY_CS_CC << 8)),

    // 0x40B1XXXX
    CUS22T_SW_IRKEY_BD_POWER                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_POWER << 8)),
    CUS22T_SW_IRKEY_BD_NUM_1                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_1 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_2                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_2 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_3                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_3 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_4                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_4 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_5                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_5 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_6                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_6 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_7                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_7 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_8                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_8 << 8)),
    CUS22T_SW_IRKEY_BD_NUM_9                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_9 << 8)),
    CUS22T_SW_IRKEY_BD_DASH                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_DASH << 8)),
    CUS22T_SW_IRKEY_BD_NUM_0                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_NUM_0 << 8)),
    CUS22T_SW_IRKEY_BD_CHANNEL_RETURN          = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_CHANNEL_RETURN << 8)),
    CUS22T_SW_IRKEY_BD_INFO                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_INFO << 8)),
    CUS22T_SW_IRKEY_BD_CHANNEL_PLUS            = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_CHANNEL_PLUS << 8)),
    CUS22T_SW_IRKEY_BD_CHANNEL_MINUS           = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_CHANNEL_MINUS << 8)),
    CUS22T_SW_IRKEY_BD_PAGE_UP                 = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_PAGE_UP << 8)),
    CUS22T_SW_IRKEY_BD_PAGE_DOWN               = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_PAGE_DOWN << 8)),
    CUS22T_SW_IRKEY_BD_PAGE_LEFT               = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_PAGE_LEFT << 8)),
    CUS22T_SW_IRKEY_BD_PAGE_RIGHT              = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_PAGE_RIGHT << 8)),
    CUS22T_SW_IRKEY_BD_QUICK                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_QUICK << 8)),
    CUS22T_SW_IRKEY_BD_EPG                     = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_EPG << 8)),
    CUS22T_SW_IRKEY_BD_RETURN                  = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_RETURN << 8)),
    CUS22T_SW_IRKEY_BD_EXIT                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_EXIT << 8)),
    CUS22T_SW_IRKEY_BD_UP                      = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_UP << 8)),
    CUS22T_SW_IRKEY_BD_DOWN                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_DOWN << 8)),
    CUS22T_SW_IRKEY_BD_LEFT                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_LEFT << 8)),
    CUS22T_SW_IRKEY_BD_RIGHT                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_RIGHT << 8)),
    CUS22T_SW_IRKEY_BD_SELECT                  = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_SELECT << 8)),
    CUS22T_SW_IRKEY_BD_REW                     = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_REW << 8)),
    CUS22T_SW_IRKEY_BD_PLAY                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_PLAY << 8)),
    CUS22T_SW_IRKEY_BD_FF                      = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_FF << 8)),
    CUS22T_SW_IRKEY_BD_SKIP_MINUS              = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_SKIP_MINUS << 8)),
    CUS22T_SW_IRKEY_BD_STOP                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_STOP << 8)),
    CUS22T_SW_IRKEY_BD_PAUSE                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_PAUSE << 8)),
    CUS22T_SW_IRKEY_BD_SKIP_PLUS               = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_SKIP_PLUS << 8)),
    CUS22T_SW_IRKEY_BD_REC                     = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_REC << 8)),
    CUS22T_SW_IRKEY_BD_RED                     = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_RED << 8)),
    CUS22T_SW_IRKEY_BD_GREEN                   = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_GREEN << 8)),
    CUS22T_SW_IRKEY_BD_YELLOW                  = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_YELLOW << 8)),
    CUS22T_SW_IRKEY_BD_BLUE                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_BLUE << 8)),
    CUS22T_SW_IRKEY_BD_MENU                    = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_MENU << 8)),
    CUS22T_SW_IRKEY_BD_MTS                     = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_MTS << 8)),
    CUS22T_SW_IRKEY_BD_CC                      = (CUS22T_SW_IRKEY_40B1_COMMAND | (CUS22T_IRKEY_BD_CC << 8)),

    // 0x40B2XXXX
    CUS22T_SW_IRKEY_DV_POWER                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_POWER << 8)),
    CUS22T_SW_IRKEY_DV_NUM_1                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_1 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_2                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_2 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_3                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_3 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_4                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_4 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_5                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_5 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_6                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_6 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_7                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_7 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_8                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_8 << 8)),
    CUS22T_SW_IRKEY_DV_NUM_9                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_9 << 8)),
    CUS22T_SW_IRKEY_DV_DASH                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_DASH << 8)),
    CUS22T_SW_IRKEY_DV_NUM_0                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_NUM_0 << 8)),
    CUS22T_SW_IRKEY_DV_CHANNEL_RETURN          = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_CHANNEL_RETURN << 8)),
    CUS22T_SW_IRKEY_DV_INFO                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_INFO << 8)),
    CUS22T_SW_IRKEY_DV_CHANNEL_PLUS            = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_CHANNEL_PLUS << 8)),
    CUS22T_SW_IRKEY_DV_CHANNEL_MINUS           = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_CHANNEL_MINUS << 8)),
    CUS22T_SW_IRKEY_DV_PAGE_UP                 = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_PAGE_UP << 8)),
    CUS22T_SW_IRKEY_DV_PAGE_DOWN               = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_PAGE_DOWN << 8)),
    CUS22T_SW_IRKEY_DV_PAGE_LEFT               = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_PAGE_LEFT << 8)),
    CUS22T_SW_IRKEY_DV_PAGE_RIGHT              = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_PAGE_RIGHT << 8)),
    CUS22T_SW_IRKEY_DV_QUICK                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_QUICK << 8)),
    CUS22T_SW_IRKEY_DV_EPG                     = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_EPG << 8)),
    CUS22T_SW_IRKEY_DV_RETURN                  = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_RETURN << 8)),
    CUS22T_SW_IRKEY_DV_EXIT                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_EXIT << 8)),
    CUS22T_SW_IRKEY_DV_UP                      = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_UP << 8)),
    CUS22T_SW_IRKEY_DV_DOWN                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_DOWN << 8)),
    CUS22T_SW_IRKEY_DV_LEFT                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_LEFT << 8)),
    CUS22T_SW_IRKEY_DV_RIGHT                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_RIGHT << 8)),
    CUS22T_SW_IRKEY_DV_SELECT                  = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_SELECT << 8)),
    CUS22T_SW_IRKEY_DV_REW                     = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_REW << 8)),
    CUS22T_SW_IRKEY_DV_PLAY                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_PLAY << 8)),
    CUS22T_SW_IRKEY_DV_FF                      = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_FF << 8)),
    CUS22T_SW_IRKEY_DV_SKIP_MINUS              = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_SKIP_MINUS << 8)),
    CUS22T_SW_IRKEY_DV_STOP                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_STOP << 8)),
    CUS22T_SW_IRKEY_DV_PAUSE                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_PAUSE << 8)),
    CUS22T_SW_IRKEY_DV_SKIP_PLUS               = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_SKIP_PLUS << 8)),
    CUS22T_SW_IRKEY_DV_REC                     = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_REC << 8)),
    CUS22T_SW_IRKEY_DV_RED                     = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_RED << 8)),
    CUS22T_SW_IRKEY_DV_GREEN                   = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_GREEN << 8)),
    CUS22T_SW_IRKEY_DV_YELLOW                  = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_YELLOW << 8)),
    CUS22T_SW_IRKEY_DV_BLUE                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_BLUE << 8)),
    CUS22T_SW_IRKEY_DV_MENU                    = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_MENU << 8)),
    CUS22T_SW_IRKEY_DV_MTS                     = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_MTS << 8)),
    CUS22T_SW_IRKEY_DV_CC                      = (CUS22T_SW_IRKEY_40B2_COMMAND | (CUS22T_IRKEY_DV_CC << 8)),

    // 0x40B3XXXX
    CUS22T_SW_IRKEY_AUDIO_INPUT_SOURCE         = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_INPUT_SOURCE << 8)),
    CUS22T_SW_IRKEY_AUDIO_POWER                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_POWER << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_1                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_1 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_2                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_2 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_3                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_3 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_4                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_4 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_5                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_5 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_6                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_6 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_7                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_7 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_8                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_8 << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_9                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_9 << 8)),
    CUS22T_SW_IRKEY_AUDIO_DASH                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_DASH << 8)),
    CUS22T_SW_IRKEY_AUDIO_NUM_0                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_NUM_0 << 8)),
    CUS22T_SW_IRKEY_AUDIO_CHANNEL_RETURN       = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_CHANNEL_RETURN << 8)),
    CUS22T_SW_IRKEY_AUDIO_VOLUME_PLUS          = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_VOLUME_PLUS << 8)),
    CUS22T_SW_IRKEY_AUDIO_VOLUME_MINUS         = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_VOLUME_MINUS << 8)),
    CUS22T_SW_IRKEY_AUDIO_INFO                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_INFO << 8)),
    CUS22T_SW_IRKEY_AUDIO_MUTE                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_MUTE << 8)),
    CUS22T_SW_IRKEY_AUDIO_CHANNEL_PLUS         = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_CHANNEL_PLUS << 8)),
    CUS22T_SW_IRKEY_AUDIO_CHANNEL_MINUS        = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_CHANNEL_MINUS << 8)),
    CUS22T_SW_IRKEY_AUDIO_PAGE_UP              = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_PAGE_UP << 8)),
    CUS22T_SW_IRKEY_AUDIO_PAGE_DOWN            = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_PAGE_DOWN << 8)),
    CUS22T_SW_IRKEY_AUDIO_PAGE_LEFT            = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_PAGE_LEFT << 8)),
    CUS22T_SW_IRKEY_AUDIO_PAGE_RIGHT           = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_PAGE_RIGHT << 8)),
    CUS22T_SW_IRKEY_AUDIO_QUICK                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_QUICK << 8)),
    CUS22T_SW_IRKEY_AUDIO_EPG                  = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_EPG << 8)),
    CUS22T_SW_IRKEY_AUDIO_RETURN               = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_RETURN << 8)),
    CUS22T_SW_IRKEY_AUDIO_EXIT                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_EXIT << 8)),
    CUS22T_SW_IRKEY_AUDIO_UP                   = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_UP << 8)),
    CUS22T_SW_IRKEY_AUDIO_DOWN                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_DOWN << 8)),
    CUS22T_SW_IRKEY_AUDIO_LEFT                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_LEFT << 8)),
    CUS22T_SW_IRKEY_AUDIO_RIGHT                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_RIGHT << 8)),
    CUS22T_SW_IRKEY_AUDIO_SELECT               = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_SELECT << 8)),
    CUS22T_SW_IRKEY_AUDIO_REW                  = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_REW << 8)),
    CUS22T_SW_IRKEY_AUDIO_PLAY                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_PLAY << 8)),
    CUS22T_SW_IRKEY_AUDIO_FF                   = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_FF << 8)),
    CUS22T_SW_IRKEY_AUDIO_SKIP_MINUS           = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_SKIP_MINUS << 8)),
    CUS22T_SW_IRKEY_AUDIO_STOP                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_STOP << 8)),
    CUS22T_SW_IRKEY_AUDIO_PAUSE                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_PAUSE << 8)),
    CUS22T_SW_IRKEY_AUDIO_SKIP_PLUS            = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_SKIP_PLUS << 8)),
    CUS22T_SW_IRKEY_AUDIO_REC                  = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_REC << 8)),
    CUS22T_SW_IRKEY_AUDIO_RED                  = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_RED << 8)),
    CUS22T_SW_IRKEY_AUDIO_GREEN                = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_GREEN << 8)),
    CUS22T_SW_IRKEY_AUDIO_YELLOW               = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_YELLOW << 8)),
    CUS22T_SW_IRKEY_AUDIO_BLUE                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_BLUE << 8)),
    CUS22T_SW_IRKEY_AUDIO_MENU                 = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_MENU << 8)),
    CUS22T_SW_IRKEY_AUDIO_MTS                  = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_MTS << 8)),
    CUS22T_SW_IRKEY_AUDIO_CC                   = (CUS22T_SW_IRKEY_40B3_COMMAND | (CUS22T_IRKEY_AUDIO_CC << 8)),

    // 0x40B4XXXX
    CUS22T_SW_IRKEY_AUX_POWER                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_POWER << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_1                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_1 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_2                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_2 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_3                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_3 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_4                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_4 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_5                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_5 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_6                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_6 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_7                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_7 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_8                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_8 << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_9                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_9 << 8)),
    CUS22T_SW_IRKEY_AUX_DASH                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_DASH << 8)),
    CUS22T_SW_IRKEY_AUX_NUM_0                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_NUM_0 << 8)),
    CUS22T_SW_IRKEY_AUX_CHANNEL_RETURN         = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_CHANNEL_RETURN << 8)),
    CUS22T_SW_IRKEY_AUX_INFO                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_INFO << 8)),
    CUS22T_SW_IRKEY_AUX_CHANNEL_PLUS           = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_CHANNEL_PLUS << 8)),
    CUS22T_SW_IRKEY_AUX_CHANNEL_MINUS          = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_CHANNEL_MINUS << 8)),
    CUS22T_SW_IRKEY_AUX_PAGE_UP                = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_PAGE_UP << 8)),
    CUS22T_SW_IRKEY_AUX_PAGE_DOWN              = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_PAGE_DOWN << 8)),
    CUS22T_SW_IRKEY_AUX_PAGE_LEFT              = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_PAGE_LEFT << 8)),
    CUS22T_SW_IRKEY_AUX_PAGE_RIGHT             = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_PAGE_RIGHT << 8)),
    CUS22T_SW_IRKEY_AUX_QUICK                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_QUICK << 8)),
    CUS22T_SW_IRKEY_AUX_EPG                    = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_EPG << 8)),
    CUS22T_SW_IRKEY_AUX_RETURN                 = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_RETURN << 8)),
    CUS22T_SW_IRKEY_AUX_EXIT                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_EXIT << 8)),
    CUS22T_SW_IRKEY_AUX_UP                     = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_UP << 8)),
    CUS22T_SW_IRKEY_AUX_DOWN                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_DOWN << 8)),
    CUS22T_SW_IRKEY_AUX_LEFT                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_LEFT << 8)),
    CUS22T_SW_IRKEY_AUX_RIGHT                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_RIGHT << 8)),
    CUS22T_SW_IRKEY_AUX_SELECT                 = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_SELECT << 8)),
    CUS22T_SW_IRKEY_AUX_REW                    = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_REW << 8)),
    CUS22T_SW_IRKEY_AUX_PLAY                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_PLAY << 8)),
    CUS22T_SW_IRKEY_AUX_FF                     = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_FF << 8)),
    CUS22T_SW_IRKEY_AUX_SKIP_MINUS             = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_SKIP_MINUS << 8)),
    CUS22T_SW_IRKEY_AUX_STOP                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_STOP << 8)),
    CUS22T_SW_IRKEY_AUX_PAUSE                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_PAUSE << 8)),
    CUS22T_SW_IRKEY_AUX_SKIP_PLUS              = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_SKIP_PLUS << 8)),
    CUS22T_SW_IRKEY_AUX_REC                    = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_REC << 8)),
    CUS22T_SW_IRKEY_AUX_RED                    = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_RED << 8)),
    CUS22T_SW_IRKEY_AUX_GREEN                  = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_GREEN << 8)),
    CUS22T_SW_IRKEY_AUX_YELLOW                 = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_YELLOW << 8)),
    CUS22T_SW_IRKEY_AUX_BLUE                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_BLUE << 8)),
    CUS22T_SW_IRKEY_AUX_MENU                   = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_MENU << 8)),
    CUS22T_SW_IRKEY_AUX_MTS                    = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_MTS << 8)),
    CUS22T_SW_IRKEY_AUX_CC                     = (CUS22T_SW_IRKEY_40B4_COMMAND | (CUS22T_IRKEY_AUX_CC << 8)),

    // 0x40BEXXXX
    CUS22T_SW_IRKEY_SEARCH                     = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_SEARCH << 8)),
    CUS22T_SW_IRKEY_HOME                       = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_HOME << 8)),
    CUS22T_SW_IRKEY_INTERNET                   = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_INTERNET << 8)),
    CUS22T_SW_IRKEY_NETFLIX                    = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_NETFLIX << 8)),
    CUS22T_SW_IRKEY_EMANUAL                    = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_EMANUAL << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_4_3           = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_4_3 << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_NORMAL        = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_NORMAL << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_DOTBYDOT      = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_DOTBYDOT << 8)),
    CUS22T_SW_IRKEY_HALF_MUTE_ON               = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_HALF_MUTE_ON << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_AUTOVIEW      = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_AUTOVIEW << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_CALIBRATED    = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_CALIBRATED << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_STORE         = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_STORE << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_GAME          = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_GAME << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_PC            = (CUS22T_SW_IRKEY_40BE_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_PC << 8)),

    // 0x40BFXXXX
    CUS22T_SW_IRKEY_MODE_CABLE_SAT             = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MODE_CABLE_SAT << 8)),
    CUS22T_SW_IRKEY_MODE_BD_DVD                = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MODE_BD_DVD << 8)),
    CUS22T_SW_IRKEY_MODE_DVR_VCR               = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MODE_DVR_VCR << 8)),
    CUS22T_SW_IRKEY_MODE_AUDIO                 = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MODE_AUDIO << 8)),
    CUS22T_SW_IRKEY_MODE_AUX                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_MODE_AUX << 8)),
    CUS22T_SW_IRKEY_POWER_OFF                  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_POWER_OFF << 8)),
    CUS22T_SW_IRKEY_POWER_ON                   = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_POWER_ON << 8)),
    CUS22T_SW_IRKEY_INPUT1                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT1 << 8)),
    CUS22T_SW_IRKEY_INPUT2                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT2 << 8)),
    CUS22T_SW_IRKEY_INPUT3                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT3 << 8)),
    CUS22T_SW_IRKEY_INPUT4                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT4 << 8)),
    CUS22T_SW_IRKEY_INPUT5                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT5 << 8)),
    CUS22T_SW_IRKEY_INPUT6                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT6 << 8)),
    CUS22T_SW_IRKEY_INPUT7                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT7 << 8)),
    CUS22T_SW_IRKEY_INPUT8                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT8 << 8)),
    CUS22T_SW_IRKEY_INPUT9                     = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT9 << 8)),
    CUS22T_SW_IRKEY_INPUT10                    = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_INPUT10 << 8)),
    CUS22T_SW_IRKEY_ANT1                       = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_ANT1 << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_NATIVE        = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_NATIVE << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_THEATERWIDE1  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_THEATERWIDE1 << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_THEATERWIDE2  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_THEATERWIDE2 << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_THEATERWIDE3  = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_THEATERWIDE3 << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_FULL          = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_FULL << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_DYNAMIC       = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_DYNAMIC << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_STANDARD      = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_STANDARD << 8)),
    CUS22T_SW_IRKEY_PICTURE_MODE_MOVIE         = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_MODE_MOVIE << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_NATURAL       = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_NATURAL << 8)),
    CUS22T_SW_IRKEY_PICTURE_SIZE_4_3_P         = (CUS22T_SW_IRKEY_40BF_COMMAND | (CUS22T_IRKEY_PICTURE_SIZE_4_3_P << 8))
}IR_CUS22T_SWDECODE_TABLE;
#endif

typedef enum _IrCommandType
{
    // General Command
    // 0x40BFXXXX
    IRKEY_TV_RADIO          = 0x0C,
    IRKEY_CHANNEL_LIST      = 0x10,
    IRKEY_CHANNEL_FAV_LIST  = 0x08,
    IRKEY_CHANNEL_RETURN    = 0x5C,
    IRKEY_CHANNEL_PLUS      = 0x1F,
    IRKEY_CHANNEL_MINUS     = 0x19,

    IRKEY_AUDIO             = 0x44, //0x62
    IRKEY_VOLUME_PLUS       = 0x16,
    IRKEY_VOLUME_MINUS      = 0x15,

    IRKEY_UP                = 0x52,
    IRKEY_POWER             = 0x46,
    IRKEY_EXIT              = 0x1B,
    IRKEY_MENU              = 0x07,
    IRKEY_DOWN              = 0x13,
    IRKEY_LEFT              = 0x06,
    IRKEY_SELECT            = 0x0F,
    IRKEY_RIGHT             = 0x1A,

    IRKEY_NUM_0             = 0x50,
    IRKEY_NUM_1             = 0x49,
    IRKEY_NUM_2             = 0x55,
    IRKEY_NUM_3             = 0x59,
    IRKEY_NUM_4             = 0x4D,
    IRKEY_NUM_5             = 0x51,
    IRKEY_NUM_6             = 0x5D,
    IRKEY_NUM_7             = 0x48,
    IRKEY_NUM_8             = 0x54,
    IRKEY_NUM_9             = 0x58,

    IRKEY_MUTE              = 0x5A,
    IRKEY_PAGE_UP           = 0x03,
    IRKEY_PAGE_DOWN         = 0x05,
    IRKEY_CLOCK             = 0x5F,

    IRKEY_INFO              = 0x14,
    IRKEY_RED               = 0x47,
    IRKEY_GREEN             = 0x4B,
    IRKEY_YELLOW            = 0x57,
    IRKEY_BLUE              = 0x5B,
    IRKEY_MTS               = 0x41,
    IRKEY_NINE_LATTICE      = IRKEY_DUMY,
    IRKEY_TTX               = 0x0A,
    IRKEY_CC                = 0x09,
    IRKEY_INPUT_SOURCE      = 0x04,
    IRKEY_CRADRD            = IRKEY_DUMY-1,
    IRKEY_PICTURE           = 0x56, //0x40,
    IRKEY_ZOOM              = 0x4C,
    IRKEY_DASH              = 0x0B,
    IRKEY_SLEEP             = 0x45,
    IRKEY_EPG               = 0x4A,
    IRKEY_PIP               = 0x40,
    IRKEY_FREEZE        = 0x4E,

    IRKEY_MIX               = 0x1C,
    IRKEY_INDEX             = 0x18,
    IRKEY_HOLD              = 0x00,
    IRKEY_PREVIOUS          = 0x02,//0x0E,
    IRKEY_NEXT              = 0x1E,//0x12,
    IRKEY_BACKWARD          = 0x0E,//0x02,
    IRKEY_FORWARD           = 0x12,//0x1E,
    IRKEY_PLAY              = 0x01,
    IRKEY_RECORD            = 0x0D,
    IRKEY_STOP              = 0x11,
    IRKEY_PAUSE             = 0x1D,
    IRKEY_EJECT             =0x7A,
    IRKEY_RECALL           =0x7B,
    IRKEY_HOME              = 0x7C,
    IRKEY_NETFLIX       = 0x7D,
    IRKEY_FAV             =0x7E,

    IRKEY_SIZE              = 0x43,
    IRKEY_REVEAL            = 0x4F,
    IRKEY_SUBCODE           = 0x53,

    IRKEY_QUICK             = 0x60,
    IRKEY_RETURN            = 0x61,
    IRKEY_MEDIA_PLAYER      = 0x62,
    IRKEY_TV_INPUT          = 0x63,
    IRKEY_TOSHIBA_PLACES    = 0X64,
    IRKEY_PAGE_LEFT         = 0x65,
    IRKEY_PAGE_RIGHT        = 0x66,


    IRKEY_STILL             = 0x68,
    IRKEY_REC               = 0x69,

    IRKEY_POWER_ON          = 0x89,
    IRKEY_POWER_OFF         = 0x90,

    // 0x40BEXXXX
    IRKEY_3D                = 0x70,
    //REGZA Link
    R_40BE_POWER            = 0x71,
    R_40BE_AUDIO            = 0x72,
    R_40BE_SETUP            = 0x73,
    R_40BE_LOAD             = 0x74,
    // MENU
    R_40BE_ROOT             = 0x75,
    R_40BE_CONTENTS         = 0x76,
    R_40BE_TOP              = 0x77,
    R_40BE_DVD              = 0x78,
    //MUTE
    IRKEY_MUTE_ON           = 0x91,
    IRKEY_MUTE_OFF          = 0x92,


    // Factory Command
#if (IR_FACtORY_SINGLE_KEY_CODE == 1)
    IRKEY_FACTORY           = 0x79,
#endif
    // 0x40BFXXXX
    F_40BF_PIP_ON_OFF       = 0x80,
    F_40BF_TUNE_DOWN_SEARCH = 0x81,
    F_40BF_TUNE_UP_SEARCH   = 0x82,
    F_40BF_MEM              = 0x83,
    F_40BF_COLOR_SYS_CHECK  = 0x84,
    F_40BF_SOUND_SYS_CHECK  = 0x85,
    F_40BF_E_D_MODE         = 0x86,
    // 0x40E2XXXX
    F_40E2_CONTRAST         = 0x87,
    F_40E2_BRIGHT           = 0x88,
    F_40E2_COLOR            = 0x89,
    F_40E2_TINT             = 0x8A,
    F_40E2_SHARPNESS        = 0x8B,
    F_40E2_VOLUME           = 0x8C,
    F_40E2_BASS             = 0x8D,
    F_40E2_TREBLE           = 0x8E,
    F_40E2_BALANCE          = 0x8F,
    F_40E2_RCUT             = 0x90,
    F_40E2_GCUT             = 0x91,
    F_40E2_BCUT             = 0x92,
    F_40E2_RDRV             = 0x93,
    F_40E2_BDRV             = 0x94,
    // 0x40E3XXXX
    F_40E3_RCUT             = 0x95,
    F_40E3_GCUT             = 0x96,
    F_40E3_BCUT             = 0x97,
    F_40E3_RDRV_GDRV        = 0x98,
    F_40E3_BDRV             = 0x99,
    // 0x40EAXXXX
    F_40EA_INTERNAL_PATTERN_OFF = 0x9A,
    F_40EA_RED_RASTER           = 0x9B,
    F_40EA_GREEN_RASTER         = 0x9C,
    F_40EA_BLUE_RASTER          = 0x9D,
    F_40EA_BLACK_RASTER         = 0x9E,
    F_40EA_WHITE_RASTER         = 0x9F,
    F_40EA_AGING_MODE_1         = 0xA0,
    F_40EA_AGINE_MODE_2         = 0xA1,
    // 0x40EBXXXX
    F_40EB_DIRECT_CH            = 0xA2, // ??
    F_40EB_DIRECT_RF            = 0xA3,
    F_40EB_DIRECT_VIDEO_EXT1    = 0xA4,
    F_40EB_DIRECT_VIDEO_EXT2    = 0xA5,
    F_40EB_DIRECT_VIDEO_EXT3    = 0xA6,
    F_40EB_DIRECT_VIDEO_EXT5    = 0xA7,
    F_40EB_DIRECT_VIDEO_EXT6    = 0xA8,
    F_40EB_DIRECT_VIDEO_EXT7    = 0xA9,
    F_40EB_DIRECT_VIDEO_EXT8    = 0xAA,
    F_40EB_DIRECT_VIDEO_EXT9    = 0xAB,
    F_40EB_SIDE_SHARED_AUDEO_EXT2   = 0xAC,
    F_40EB_SIDE_SHARED_AUDIO_EXT3   = 0xAD,
    // 0x40EEXXXX
    F_40EE_ALL_CH               = 0xAE, //
    F_40EE_M_MODE               = 0xAF,
    F_40EE_INITIALIZATION       = 0xB0,
    F_40EE_PICTURE_MODE_DYNAMIC = 0xB1,
    F_40EE_M_OSD_OFF            = 0xB2,
    F_40EE_TJP_FACTORY_SIGNAL_SETTING   = 0xB3,
    F_40EE_3D_ON_SIDE_BY_SIDE   = 0xB4,
    F_40EE_DATA_1_UP            = 0xB5,
    F_40EE_DATA_1_DN            = 0xB6,
    F_40EE_DATA_4_UP            = 0xB7,
    F_40EE_DATA_4_DN            = 0xB8,
    F_40EE_2D_3D_CONVERSION     = 0xB9,
    F_40EE_AUDIO_BALANCE_TOGGLE = 0xBA,
    F_40EE_LED_CHECK            = 0xBB,
    F_40EE_4_3_STRETCH_TOGGLE   = 0xBC,
    F_40EE_DNR_SELECT           = 0xBD,
    F_40EE_AUTO_BRIGHTNESS_SENSOR   = 0xBE,
    F_40EE_AUTO_BRIGHTNESS_SENSOR_DETECT_VALUE_DISPLAY  = 0xBF,
    F_40EE_EDID_DATA_DOWNLOAD   = 0xC0,
    F_40EE_STABLE_SOUND_ON_OFF  = 0xC1,
    F_40EE_CLEAR_SCAN_100_ON_OFF= 0xC2,
    F_40EE_CLEAR_SCAN_200_PRO_ON_OFF= 0xC3,
    F_40EE_MEDIA_PLAYER_MOVIE   = 0xC4,
    F_40EE_BASE_BOOST_ON_OFF    = 0xC5,
    F_40EE_BLACK_WHITE_LEVEL    = 0xC6,
    F_40EE_NEWZEALAND_PRESET    = 0xC7,
    F_40EE_DLNA_TEST            = 0xC8,
    F_40EE_COLOR_TEMPERATURE    = 0xC9,
    F_40EE_BLACK_LIGHT_CHECK    = 0xCA,
    F_40EE_DEFAULT_MAC_IP       = 0xCB,
    F_40EE_MAC_ADDRESS_DELETE   = 0xCC,
    F_40EE_ADC_ADJUSTMENT       = 0xCD,
    F_40EE_DUAL_1               = 0xCE,
    F_40EE_MONO_MODE            = 0xCF,
    F_40EE_STEREO_MODE          = 0xD0,
    F_40EE_DUAL_2               = 0xD1,
    F_40EE_HDMI_2_AUDIO         = 0xD2,
    F_40EE_HDMI_2_AUDIO_SELECT  = 0xD3,
    F_40EE_S_MODE               = 0xD4,
    F_40EE_SIDE_PANEL_SELECT    = 0xD5,
    F_40EE_PICTURE_SIZE_POSITION= 0xD6,
    F_40EE_MPEG_NR              = 0xD7,
    F_40EE_SIGNAL_BOOSTER_ON_OFF= 0xD8,
    F_40EE_PICTURE_RESET        = 0xD9,
    F_40EE_DIGITAL_AUDIO_OUT    = 0xDA,
    // 0x40F0XXXX
    F_40F0_EEPROM_MEM_DATE_UPDATE   = 0xDB,
    // 0x40F1XXXX
    F_40F1_EEPROM_MEM_DATE_UPDATE   = 0xDC,
    // 0x40F2XXXX
    F_40F2_EEPROM_MEM_DATE_UPDATE   = 0xDD,
    // 0xCXXXXXXX
    //F_CFXX_WHITE_WINDOW_ON      = 0xDE,
    // 0xDXXXXXXX
    //F_D0XX_WHITE_WINDOW_ON      = 0xDF,

    // TM13 NA Model Key Code
    // 0x40B0XXXX
    IRKEY_CS_POWER                      = 0x0100,
    IRKEY_CS_NUM_1                      = 0x0101,
    IRKEY_CS_NUM_2                      = 0x0102,
    IRKEY_CS_NUM_3                      = 0x0103,
    IRKEY_CS_NUM_4                      = 0x0104,
    IRKEY_CS_NUM_5                      = 0x0105,
    IRKEY_CS_NUM_6                      = 0x0106,
    IRKEY_CS_NUM_7                      = 0x0107,
    IRKEY_CS_NUM_8                      = 0x0108,
    IRKEY_CS_NUM_9                      = 0x0109,
    IRKEY_CS_DASH                       = 0x010A,
    IRKEY_CS_NUM_0                      = 0x010B,
    IRKEY_CS_CHANNEL_RETURN             = 0x010C,
    IRKEY_CS_INFO                       = 0x010D,
    IRKEY_CS_CHANNEL_PLUS               = 0x010E,
    IRKEY_CS_CHANNEL_MINUS              = 0x010F,
    IRKEY_CS_PAGE_UP                    = 0x0110,
    IRKEY_CS_PAGE_DOWN                  = 0x0111,
    IRKEY_CS_PAGE_LEFT                  = 0x0112,
    IRKEY_CS_PAGE_RIGHT                 = 0x0113,
    IRKEY_CS_QUICK                      = 0x0114,
    IRKEY_CS_EPG                        = 0x0115,
    IRKEY_CS_RETURN                     = 0x0116,
    IRKEY_CS_EXIT                       = 0x0117,
    IRKEY_CS_UP                         = 0x0118,
    IRKEY_CS_DOWN                       = 0x0119,
    IRKEY_CS_LEFT                       = 0x011A,
    IRKEY_CS_RIGHT                      = 0x011B,
    IRKEY_CS_SELECT                     = 0x011C,
    IRKEY_CS_REW                        = 0x011D,
    IRKEY_CS_PLAY                       = 0x011E,
    IRKEY_CS_FF                         = 0x011F,
    IRKEY_CS_SKIP_MINUS                 = 0x0120,
    IRKEY_CS_STOP                       = 0x0121,
    IRKEY_CS_PAUSE                      = 0x0122,
    IRKEY_CS_SKIP_PLUS                  = 0x0123,
    IRKEY_CS_REC                        = 0x0124,
    IRKEY_CS_RED                        = 0x0125,
    IRKEY_CS_GREEN                      = 0x0126,
    IRKEY_CS_YELLOW                     = 0x0127,
    IRKEY_CS_BLUE                       = 0x0128,
    IRKEY_CS_MENU                       = 0x0129,
    IRKEY_CS_MTS                        = 0x012A,
    IRKEY_CS_CC                         = 0x012B,

    // 0x40B1XXXX
    IRKEY_BD_POWER                      = 0x012C,
    IRKEY_BD_NUM_1                      = 0x012D,
    IRKEY_BD_NUM_2                      = 0x012E,
    IRKEY_BD_NUM_3                      = 0x012F,
    IRKEY_BD_NUM_4                      = 0x0130,
    IRKEY_BD_NUM_5                      = 0x0131,
    IRKEY_BD_NUM_6                      = 0x0132,
    IRKEY_BD_NUM_7                      = 0x0133,
    IRKEY_BD_NUM_8                      = 0x0134,
    IRKEY_BD_NUM_9                      = 0x0135,
    IRKEY_BD_DASH                       = 0x0136,
    IRKEY_BD_NUM_0                      = 0x0137,
    IRKEY_BD_CHANNEL_RETURN             = 0x0138,
    IRKEY_BD_INFO                       = 0x0139,
    IRKEY_BD_CHANNEL_PLUS               = 0x013A,
    IRKEY_BD_CHANNEL_MINUS              = 0x013B,
    IRKEY_BD_PAGE_UP                    = 0x013C,
    IRKEY_BD_PAGE_DOWN                  = 0x013D,
    IRKEY_BD_PAGE_LEFT                  = 0x013E,
    IRKEY_BD_PAGE_RIGHT                 = 0x013F,
    IRKEY_BD_QUICK                      = 0x0140,
    IRKEY_BD_EPG                        = 0x0141,
    IRKEY_BD_RETURN                     = 0x0142,
    IRKEY_BD_EXIT                       = 0x0143,
    IRKEY_BD_UP                         = 0x0144,
    IRKEY_BD_DOWN                       = 0x0145,
    IRKEY_BD_LEFT                       = 0x0146,
    IRKEY_BD_RIGHT                      = 0x0147,
    IRKEY_BD_SELECT                     = 0x0148,
    IRKEY_BD_REW                        = 0x0149,
    IRKEY_BD_PLAY                       = 0x014A,
    IRKEY_BD_FF                         = 0x014B,
    IRKEY_BD_SKIP_MINUS                 = 0x014C,
    IRKEY_BD_STOP                       = 0x014D,
    IRKEY_BD_PAUSE                      = 0x014E,
    IRKEY_BD_SKIP_PLUS                  = 0x014F,
    IRKEY_BD_REC                        = 0x0150,
    IRKEY_BD_RED                        = 0x0151,
    IRKEY_BD_GREEN                      = 0x0152,
    IRKEY_BD_YELLOW                     = 0x0153,
    IRKEY_BD_BLUE                       = 0x0154,
    IRKEY_BD_MENU                       = 0x0155,
    IRKEY_BD_MTS                        = 0x0156,
    IRKEY_BD_CC                         = 0x0157,

    // 0x40B2XXXX
    IRKEY_DV_POWER                      = 0x0158,
    IRKEY_DV_NUM_1                      = 0x0159,
    IRKEY_DV_NUM_2                      = 0x015A,
    IRKEY_DV_NUM_3                      = 0x015B,
    IRKEY_DV_NUM_4                      = 0x015C,
    IRKEY_DV_NUM_5                      = 0x015D,
    IRKEY_DV_NUM_6                      = 0x015E,
    IRKEY_DV_NUM_7                      = 0x015F,
    IRKEY_DV_NUM_8                      = 0x0160,
    IRKEY_DV_NUM_9                      = 0x0161,
    IRKEY_DV_DASH                       = 0x0162,
    IRKEY_DV_NUM_0                      = 0x0163,
    IRKEY_DV_CHANNEL_RETURN             = 0x0164,
    IRKEY_DV_INFO                       = 0x0165,
    IRKEY_DV_CHANNEL_PLUS               = 0x0166,
    IRKEY_DV_CHANNEL_MINUS              = 0x0167,
    IRKEY_DV_PAGE_UP                    = 0x0168,
    IRKEY_DV_PAGE_DOWN                  = 0x0169,
    IRKEY_DV_PAGE_LEFT                  = 0x016A,
    IRKEY_DV_PAGE_RIGHT                 = 0x016B,
    IRKEY_DV_QUICK                      = 0x016C,
    IRKEY_DV_EPG                        = 0x016D,
    IRKEY_DV_RETURN                     = 0x016E,
    IRKEY_DV_EXIT                       = 0x016F,
    IRKEY_DV_UP                         = 0x0170,
    IRKEY_DV_DOWN                       = 0x0171,
    IRKEY_DV_LEFT                       = 0x0172,
    IRKEY_DV_RIGHT                      = 0x0173,
    IRKEY_DV_SELECT                     = 0x0174,
    IRKEY_DV_REW                        = 0x0175,
    IRKEY_DV_PLAY                       = 0x0176,
    IRKEY_DV_FF                         = 0x0177,
    IRKEY_DV_SKIP_MINUS                 = 0x0178,
    IRKEY_DV_STOP                       = 0x0179,
    IRKEY_DV_PAUSE                      = 0x017A,
    IRKEY_DV_SKIP_PLUS                  = 0x017B,
    IRKEY_DV_REC                        = 0x017C,
    IRKEY_DV_RED                        = 0x017D,
    IRKEY_DV_GREEN                      = 0x017E,
    IRKEY_DV_YELLOW                     = 0x017F,
    IRKEY_DV_BLUE                       = 0x0180,
    IRKEY_DV_MENU                       = 0x0181,
    IRKEY_DV_MTS                        = 0x0182,
    IRKEY_DV_CC                         = 0x0183,

    // 0x40B3XXXX
    IRKEY_AUDIO_INPUT_SOURCE            = 0x0184,
    IRKEY_AUDIO_POWER                   = 0x0185,
    IRKEY_AUDIO_NUM_1                   = 0x0186,
    IRKEY_AUDIO_NUM_2                   = 0x0187,
    IRKEY_AUDIO_NUM_3                   = 0x0188,
    IRKEY_AUDIO_NUM_4                   = 0x0189,
    IRKEY_AUDIO_NUM_5                   = 0x018A,
    IRKEY_AUDIO_NUM_6                   = 0x018B,
    IRKEY_AUDIO_NUM_7                   = 0x018C,
    IRKEY_AUDIO_NUM_8                   = 0x018D,
    IRKEY_AUDIO_NUM_9                   = 0x018E,
    IRKEY_AUDIO_DASH                    = 0x018F,
    IRKEY_AUDIO_NUM_0                   = 0x0190,
    IRKEY_AUDIO_CHANNEL_RETURN          = 0x0191,
    IRKEY_AUDIO_VOLUME_PLUS             = 0x0192,
    IRKEY_AUDIO_VOLUME_MINUS            = 0x0193,
    IRKEY_AUDIO_INFO                    = 0x0194,
    IRKEY_AUDIO_MUTE                    = 0x0195,
    IRKEY_AUDIO_CHANNEL_PLUS            = 0x0196,
    IRKEY_AUDIO_CHANNEL_MINUS           = 0x0197,
    IRKEY_AUDIO_PAGE_UP                 = 0x0198,
    IRKEY_AUDIO_PAGE_DOWN               = 0x0199,
    IRKEY_AUDIO_PAGE_LEFT               = 0x019A,
    IRKEY_AUDIO_PAGE_RIGHT              = 0x019B,
    IRKEY_AUDIO_QUICK                   = 0x019C,
    IRKEY_AUDIO_EPG                     = 0x019D,
    IRKEY_AUDIO_RETURN                  = 0x019E,
    IRKEY_AUDIO_EXIT                    = 0x019F,
    IRKEY_AUDIO_UP                      = 0x01A0,
    IRKEY_AUDIO_DOWN                    = 0x01A1,
    IRKEY_AUDIO_LEFT                    = 0x01A2,
    IRKEY_AUDIO_RIGHT                   = 0x01A3,
    IRKEY_AUDIO_SELECT                  = 0x01A4,
    IRKEY_AUDIO_REW                     = 0x01A5,
    IRKEY_AUDIO_PLAY                    = 0x01A6,
    IRKEY_AUDIO_FF                      = 0x01A7,
    IRKEY_AUDIO_SKIP_MINUS              = 0x01A8,
    IRKEY_AUDIO_STOP                    = 0x01A9,
    IRKEY_AUDIO_PAUSE                   = 0x01AA,
    IRKEY_AUDIO_SKIP_PLUS               = 0x01AB,
    IRKEY_AUDIO_REC                     = 0x01AC,
    IRKEY_AUDIO_RED                     = 0x01AD,
    IRKEY_AUDIO_GREEN                   = 0x01AE,
    IRKEY_AUDIO_YELLOW                  = 0x01AF,
    IRKEY_AUDIO_BLUE                    = 0x01B0,
    IRKEY_AUDIO_MENU                    = 0x01B1,
    IRKEY_AUDIO_MTS                     = 0x01B2,
    IRKEY_AUDIO_CC                      = 0x01B3,

    // 0x40B4XXXX
    IRKEY_AUX_POWER                     = 0x01B4,
    IRKEY_AUX_NUM_1                     = 0x01B5,
    IRKEY_AUX_NUM_2                     = 0x01B6,
    IRKEY_AUX_NUM_3                     = 0x01B7,
    IRKEY_AUX_NUM_4                     = 0x01B8,
    IRKEY_AUX_NUM_5                     = 0x01B9,
    IRKEY_AUX_NUM_6                     = 0x01BA,
    IRKEY_AUX_NUM_7                     = 0x01BB,
    IRKEY_AUX_NUM_8                     = 0x01BC,
    IRKEY_AUX_NUM_9                     = 0x01BD,
    IRKEY_AUX_DASH                      = 0x01BE,
    IRKEY_AUX_NUM_0                     = 0x01BF,
    IRKEY_AUX_CHANNEL_RETURN            = 0x01C0,
    IRKEY_AUX_INFO                      = 0x01C1,
    IRKEY_AUX_CHANNEL_PLUS              = 0x01C2,
    IRKEY_AUX_CHANNEL_MINUS             = 0x01C3,
    IRKEY_AUX_PAGE_UP                   = 0x01C4,
    IRKEY_AUX_PAGE_DOWN                 = 0x01C5,
    IRKEY_AUX_PAGE_LEFT                 = 0x01C6,
    IRKEY_AUX_PAGE_RIGHT                = 0x01C7,
    IRKEY_AUX_QUICK                     = 0x01C8,
    IRKEY_AUX_EPG                       = 0x01C9,
    IRKEY_AUX_RETURN                    = 0x01CA,
    IRKEY_AUX_EXIT                      = 0x01CB,
    IRKEY_AUX_UP                        = 0x01CC,
    IRKEY_AUX_DOWN                      = 0x01CD,
    IRKEY_AUX_LEFT                      = 0x01CE,
    IRKEY_AUX_RIGHT                     = 0x01CF,
    IRKEY_AUX_SELECT                    = 0x01D0,
    IRKEY_AUX_REW                       = 0x01D1,
    IRKEY_AUX_PLAY                      = 0x01D2,
    IRKEY_AUX_FF                        = 0x01D3,
    IRKEY_AUX_SKIP_MINUS                = 0x01D4,
    IRKEY_AUX_STOP                      = 0x01D5,
    IRKEY_AUX_PAUSE                     = 0x01D6,
    IRKEY_AUX_SKIP_PLUS                 = 0x01D7,
    IRKEY_AUX_REC                       = 0x01D8,
    IRKEY_AUX_RED                       = 0x01D9,
    IRKEY_AUX_GREEN                     = 0x01DA,
    IRKEY_AUX_YELLOW                    = 0x01DB,
    IRKEY_AUX_BLUE                      = 0x01DC,
    IRKEY_AUX_MENU                      = 0x01DD,
    IRKEY_AUX_MTS                       = 0x01DE,
    IRKEY_AUX_CC                        = 0x01DF,

    // 0x40BEXXXX
    IRKEY_SEARCH                        = 0x01E0,
   // IRKEY_HOME                          = 0x01E1,
    IRKEY_INTERNET                      = 0x01E2,
    //IRKEY_NETFLIX                       = 0x01E3,
    IRKEY_EMANUAL                       = 0x01E4,
    IRKEY_PICTURE_SIZE_4_3              = 0xE8,
    IRKEY_PICTURE_SIZE_NORMAL           = 0xE9,
    IRKEY_PICTURE_SIZE_DOTBYDOT         = 0xEA,
    IRKEY_HALF_MUTE_ON                  = 0xEB,
    IRKEY_PICTURE_MODE_AUTOVIEW         = 0xEC,
    IRKEY_PICTURE_MODE_CALIBRATED       = 0xED,
    IRKEY_PICTURE_MODE_STORE            = 0xEE,
    IRKEY_PICTURE_MODE_GAME             = 0xEF,
    IRKEY_PICTURE_MODE_PC               = 0xF0,

    // 0x40BFXXXX
    IRKEY_MODE_CABLE_SAT                = 0x01EE,
    IRKEY_MODE_BD_DVD                   = 0x01EF,
    IRKEY_MODE_DVR_VCR                  = 0x01F0,
    IRKEY_MODE_AUDIO                    = 0x01F1,
    IRKEY_MODE_AUX                      = 0x01F2,
    IRKEY_INPUT1                        = 0xF1,
    IRKEY_INPUT2                        = 0xF2,
    IRKEY_INPUT3                        = 0xF3,
    IRKEY_INPUT4                        = 0xF4,
    IRKEY_INPUT5                        = 0xF5,
    IRKEY_INPUT6                        = 0xF6,
    IRKEY_INPUT7                        = 0xF7,
    IRKEY_INPUT8                        = 0xF8,
    IRKEY_INPUT9                        = 0xF9,
    IRKEY_INPUT10                       = 0xFA,
    IRKEY_ANT1                          = 0xFB,
    IRKEY_PICTURE_SIZE_NATIVE           = 0xE0,
    IRKEY_PICTURE_SIZE_THEATERWIDE1     = 0xE1,
    IRKEY_PICTURE_SIZE_THEATERWIDE2     = 0xE2,
    IRKEY_PICTURE_SIZE_THEATERWIDE3     = 0xE3,
    IRKEY_PICTURE_SIZE_FULL             = 0xE4,
    IRKEY_PICTURE_MODE_DYNAMIC          = 0xE5,
    IRKEY_PICTURE_MODE_STANDARD         = 0xE6,
    IRKEY_PICTURE_MODE_MOVIE            = 0xE7,
    IRKEY_PICTURE_SIZE_NATURAL          = 0xDE,
    IRKEY_PICTURE_SIZE_4_3_P            = 0xDF
}IrCommandType;


//*************************************************************************

//*************************************************************************
// IR system parameter define for H/W setting (Please don't modify them)
//*************************************************************************
#define IR_CKDIV_NUM                        ((XTAL_CLOCK_FREQ + 500000UL) / 1000000UL)
#define IR_CKDIV_NUM_BOOT                   XTAL_CLOCK_FREQ //13
#define IR_CLK_BOOT                         (XTAL_CLOCK_FREQ / 1000000.0)
#define IR_CLK                              IR_CLK_BOOT

#define irGetMinCnt_BOOT(time, tolerance)   ((u32)(((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1-tolerance)))
#define irGetMaxCnt_BOOT(time, tolerance)   ((u32)(((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1+tolerance)))
#define irGetMinCnt(time, tolerance)        ((u32)(((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1-tolerance)))
#define irGetMaxCnt(time, tolerance)        ((u32)(((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1+tolerance)))

#define irGetCnt_BOOT(time)                 ((u32)((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))+0x300000UL)
#define irGetCnt(time)                      ((u32)((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))+0x300000UL)

// 12Mhz
#define IR_RP_TIMEOUT_BOOT      irGetCnt_BOOT(IR_TIMEOUT_CYC)
#define IR_HDC_UPB_BOOT         irGetMaxCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB_BOOT         irGetMinCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB_BOOT         irGetMaxCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB_BOOT         irGetMinCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB_BOOT      irGetMaxCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB_BOOT      irGetMinCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB_BOOT       irGetMaxCnt_BOOT(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB_BOOT       irGetMinCnt_BOOT(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_1_TIME, 0.2)

// 90Mhz
#define IR_RP_TIMEOUT           irGetCnt(IR_TIMEOUT_CYC)
#define IR_HDC_UPB              irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB              irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB              irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB              irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB           irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB           irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB            irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB            irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB              irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB              irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB              irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB              irGetMinCnt(IR_LOGI_1_TIME, 0.2)

// Power off mode
#define PM_IR_TH_VAL            (PM_TH0_VAL & 0x0F)
// PM_IR_TH_GRID specify the time (in us) for each threshold bit.
// So PM_IR_TH_GRID = (1/12M) * (2^14) * 1000000 = (2^14) / 12
#define PM_IR_TH_GRID           (16384/IR_CLK_BOOT)
#define PM_IR_HEADER_CODE_TIME  (IR_HEADER_CODE_TIME-(0x0F-PM_IR_TH_VAL)*PM_IR_TH_GRID)
#define PM_IR_HDC_UPB_BOOT      irGetMaxCnt_BOOT(PM_IR_HEADER_CODE_TIME, 0.6)
#define PM_IR_HDC_LOB_BOOT      irGetMinCnt_BOOT(PM_IR_HEADER_CODE_TIME, 0.6)
//*************************************************************************

#endif
