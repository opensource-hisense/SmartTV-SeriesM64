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
#include "IR_PROTOCOL_PANASONIC7051.h"

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static unsigned long long _u64Panasonic7051LastDecodeData=0;
static unsigned long _ulPanasonic7051PreDecodeTime=0;
static unsigned long long _u64Panasonic7051Data=0;
static U8 _u8Panasonic7051Bits=0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _Panasonic7051RAW2Format(unsigned long long u64RawData, U32 *pu32CustCode, U16 *pu16KeyCode, U8 *pu8State, U8 *pu8Reserved)
{
    BOOL bRepeatKey=FALSE;

    if(LAST_KEY_PROTOCOL(E_IR_PROTOCOL_PANASONIC7051))
    {
        if(u64RawData==_u64Panasonic7051LastDecodeData)
        {
            bRepeatKey=TRUE;
        }

        if(bRepeatKey==TRUE)
        {
            *pu8State = E_IR_KEY_STATE_REPEAT;
        }
    }
    else
    {
        *pu8State = E_IR_KEY_STATE_PRESS;
    }
    //printk("state = %d\n",*pu8State);
    *pu32CustCode = (_MulProtCommon_LSB2MSB((u64RawData>>40)&0xFF))<<24 |(_MulProtCommon_LSB2MSB((u64RawData>>32)&0xFF))<<16 | \
        (_MulProtCommon_LSB2MSB((u64RawData>>24)&0xFF))<<8 |_MulProtCommon_LSB2MSB((u64RawData>>16)&0xFF);
    *pu16KeyCode = _MulProtCommon_LSB2MSB((u64RawData>>8)&0xFF) | (_MulProtCommon_LSB2MSB(u64RawData&0xFF)<<8);
   //*pu32CustCode = (u64RawData>>16) & 0xFFFFFFFF;
   //*pu16KeyCode = ((u64RawData>>8)&0xFF)<<8 | (u64RawData&0xFF);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
BOOL _ParseLeadCodePanasonic7051(U8 *pu8StartIndex)
{
    U8 u8Index;
    U32 u32ShotValue[IR_PANASONIC7051_LEADCODE_SHOT_COUNT];
    BOOL bNegshot[IR_PANASONIC7051_LEADCODE_SHOT_COUNT];
    U8 u8Datasize=0;

    u8Datasize = _MulProtCommon_GetShotDataSize();
    if(u8Datasize< IR_PANASONIC7051_LEADCODE_SHOT_COUNT)
    {
        return FALSE;
    }

    for(u8Index=0; u8Index<= u8Datasize-(IR_PANASONIC7051_LEADCODE_SHOT_COUNT); u8Index++)
    {
        if(_MulProtCommon_PeekShot(u8Index, IR_PANASONIC7051_LEADCODE_SHOT_COUNT, u32ShotValue, bNegshot)==TRUE)
        {
            if(bNegshot[0]==TRUE)
            {
                continue;//1st shot must be positive
            }

            if(bNegshot[1]==TRUE && bNegshot[2]==FALSE)//2nd shot must be negtive
            {
                if((u32ShotValue[1] > IR_TIME_LOB(IR_TIME_PANASONIC7051_HEADER_CODE, IR_TIME_PANASONIC7051_LEADCODE_TOLERENCE) &&
                    u32ShotValue[1] < IR_TIME_UPD(IR_TIME_PANASONIC7051_HEADER_CODE, IR_TIME_PANASONIC7051_LEADCODE_TOLERENCE)) &&
                   (u32ShotValue[2] > IR_TIME_LOB(IR_TIME_PANASONIC7051_HEADER_OFF_CODE, IR_TIME_PANASONIC7051_LEADCODE_TOLERENCE) &&
                    u32ShotValue[2] < IR_TIME_UPD(IR_TIME_PANASONIC7051_HEADER_OFF_CODE, IR_TIME_PANASONIC7051_LEADCODE_TOLERENCE)))
                {
                    *pu8StartIndex = u8Index;
                    return TRUE;
                }
            }
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE;

}

IR_DECODE_STATUS _ParsePANASONIC7051(U32 *pu32CustCode, U16 *pu16KeyCode, U8 *pu8State, U8 *pu8Reserved)
{
    U32 u32ShotValue;
    BOOL bNegShot=FALSE;
    BOOL bLastShotN=FALSE;
    U32 u32LastNValue=0;
    U8 u8StartIndex = 0;

    if (_MDrv_IR_GetSystemTime() - _ulPanasonic7051PreDecodeTime > 60)//IR_TIME_UPD(IR_TIME_PANASONIC7051_TIMEOUT, IR_TIME_PANASONIC7051_TOLERENCE)/1000)
    {
        _u64Panasonic7051LastDecodeData=0;
        _u64Panasonic7051Data=0;
        _u8Panasonic7051Bits=0;
    }

    _ulPanasonic7051PreDecodeTime = _MDrv_IR_GetSystemTime();

    if(_MulProtCommon_GetShotDataSize()<IR_PANASONIC7051_LEADCODE_SHOT_COUNT)
    {
        return E_IR_DECODE_DATA_SHORTAGE;
    }

    if(_ParseLeadCodePanasonic7051(&u8StartIndex)==FALSE)
    {
        return E_IR_DECODE_ERR;
    }

    while(_MulProtCommon_PeekShot(u8StartIndex+IR_PANASONIC7051_LEADCODE_SHOT_COUNT+_u8Panasonic7051Bits*2, 1, &u32ShotValue, &bNegShot)==TRUE)//only use peek here
    {
        if(bNegShot==TRUE)//Negtive
        {
            bLastShotN = TRUE;
            u32LastNValue = u32ShotValue;
        }
        else//positive
        {
            if(bLastShotN==TRUE)//A complete logic is: N+P
            {
                if(u32LastNValue+u32ShotValue >= IR_TIME_LOB(IR_TIME_PANASONIC7051_LOGI_0, IR_TIME_PANASONIC7051_TOLERENCE) && u32LastNValue+u32ShotValue <= IR_TIME_UPD(IR_TIME_PANASONIC7051_LOGI_0, IR_TIME_PANASONIC7051_TOLERENCE))//logical 0
                {
                    _u64Panasonic7051Data<<=1;
                    _u64Panasonic7051Data |=0;
                    _u8Panasonic7051Bits+=1;
                }
                else if(u32LastNValue+u32ShotValue >= IR_TIME_LOB(IR_TIME_PANASONIC7051_LOGI_1, IR_TIME_PANASONIC7051_TOLERENCE) && u32LastNValue+u32ShotValue <= IR_TIME_UPD(IR_TIME_PANASONIC7051_LOGI_1, IR_TIME_PANASONIC7051_TOLERENCE))//logical 1
                {
                    _u64Panasonic7051Data <<=1;
                    _u64Panasonic7051Data |=1;
                    _u8Panasonic7051Bits+=1;
                }
                else
                {
                    _u64Panasonic7051LastDecodeData=0;
                    _u64Panasonic7051Data=0;
                    _u8Panasonic7051Bits=0;
                    return E_IR_DECODE_ERR;
                }
            }

            bLastShotN=FALSE;
            u32LastNValue = 0;
        }

        u8StartIndex++;
    }

    if(_u8Panasonic7051Bits==IR_PANASONIC7051_BITS )
    {
        _Panasonic7051RAW2Format(_u64Panasonic7051Data, pu32CustCode, pu16KeyCode, pu8State, pu8Reserved);
        _u64Panasonic7051LastDecodeData = _u64Panasonic7051Data;
        _u64Panasonic7051Data = 0;
        _u8Panasonic7051Bits = 0;
        return E_IR_DECODE_DATA_OK;
    }

    return E_IR_DECODE_DATA_SHORTAGE;
}


DRV_IR_PROTOCOL_TYPE GET_IR_PROTOCOL_ENTRY(E_IR_PROTOCOL_PANASONIC7051) =
{
    .name               = "E_IR_PROTOCOL_PANASONIC7051",
    .etype              = E_IR_PROTOCOL_PANASONIC7051,
    .findleadcode       = _ParseLeadCodePanasonic7051,
    .parseprotocol      = _ParsePANASONIC7051,
    .u8LeadCodeMinCount = IR_PANASONIC7051_LEADCODE_SHOT_COUNT,
    .u32Timeout         = IR_TIME_UPD(IR_TIME_PANASONIC7051_TIMEOUT, IR_TIME_PANASONIC7051_TOLERENCE),
};

