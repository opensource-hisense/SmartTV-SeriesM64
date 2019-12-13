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

#ifndef IR_PROTOCOL_COMMON_H
#define IR_PROTOCOL_COMMON_H

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "mdrv_types.h"
#include "mdrv_ir_st.h"
//#define SUPPORT_MULTI_PROTOCOL
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SHOT_BUF_MAX    100
#define PROTOCOL_SUPPORT_MAX    E_IR_PROTOCOL_MAX-1
#define SHOTLIST_EMPTY()    (_u8ShotHeadIdx==_u8ShotTailIdx)
#define SHOTLIST_FULL()     ((_u8ShotHeadIdx+1)%SHOT_BUF_MAX ==_u8ShotTailIdx)
#define IR_TIME_UPD(time, tolerance)   ((U32)(((double)time)*((double)1+tolerance)))
#define IR_TIME_LOB(time, tolerance)   ((U32)(((double)time)*((double)1-tolerance)))
#define LAST_KEY_PROTOCOL(x)   (x==_eLastKeyProtocol)


#define _GET_IR_PROTOCOL_ENTRY(var, Name) var##Name
#define GET_IR_PROTOCOL_ENTRY(Name) _GET_IR_PROTOCOL_ENTRY(IR_PROTOCOL_ENTRY_,Name)
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_IR_KEY_STATE_PRESS = 0,
    E_IR_KEY_STATE_REPEAT,
    E_IR_KEY_STATE_RELEASE,
} IR_KEY_STATE_e;

typedef enum
{
    E_IR_DECODE_ERR=0,
    E_IR_DECODE_DATA_SHORTAGE,
    E_IR_DECODE_DATA_OK,
} IR_DECODE_STATUS;

typedef BOOL                 drv_ir_protocol_findleadcode(U8 *pu8StartIndex);
typedef IR_DECODE_STATUS     drv_ir_protocol_praseprotocol(U32 *pu32CustCode, U16 *pu16KeyCode, U8 *pu8State, U8 *pu8Reserved);

typedef struct
{
    const char                          *name;          // ir protocol name
    IR_PROCOCOL_TYPE                    etype;          //ir enum value
    drv_ir_protocol_findleadcode        *findleadcode; //check if lead code exist
    drv_ir_protocol_praseprotocol       *parseprotocol;//parse by specific protocol
    U8                                  u8LeadCodeMinCount;//the least shot count to identify lead code
    U32                                 u32Timeout;
} DRV_IR_PROTOCOL_TYPE;
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern IR_PROCOCOL_TYPE _eLastKeyProtocol;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Functions
//-------------------------------------------------------------------------------------------------
U8 _MulProtCommon_GetShotDataSize(void);
BOOL _MulProtCommon_PeekShot(U8 u8Offset, U8 u8Count, U32 *pu32Value, BOOL *pbNegtive);
U8 _MulProtCommon_LSB2MSB(U8 u8OrgData);
void _MulProtCommon_dumpShotData(void);

extern unsigned long _MDrv_IR_GetSystemTime(void);
extern int printk(const char *fmt, ...);
//-------------------------------------------------------------------------------------------------
// Global Functions for Mdrv_ir.c (should not be called in private protocol)
//-------------------------------------------------------------------------------------------------
void _Mdrv_MulProtCommon_ShotDataReset(void);
BOOL _Mdrv_MulProtCommon_AddShot(U32 u32Value, BOOL bNegtive);
#endif

