////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mdrv_ldm_algorithm.h
/// @brief  MStar local dimming Interface header file
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LD_ALGO_H
#define _MDRV_LD_ALGO_H

#ifdef _MDRV_LDM_INTERRUPT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if defined(__cplusplus)
extern "C" {
#endif

//=============================================================================
// Defines & Macros
//=============================================================================
#define LD_MAX_BW       (32)//(32)
#define LD_MAX_BH       (32)//(32)
#define LD_MAX_BLOCK    (10*1024)//(1024)


#define BUFF_LENGTH       64


//=============================================================================
// Type and Structure Declaration
//=============================================================================

typedef enum
{
    //Aeon-->51 Command Index
    E_PM_CMDIDX_INIT = 0x01,
    E_PM_CMDIDX_GET_STATUS = 0x02,
    E_PM_CMDIDX_GET_LIBVER = 0x03,
    E_PM_CMDIDX_POWER_DOWN = 0x04,
    E_PM_CMDIDX_RTC_INIT = 0x05,
    E_PM_CMDIDX_RTC_SETCOUNTER = 0x06,
    E_PM_CMDIDX_RTC_GETCOUNTER = 0x07,
    E_PM_CMDIDX_RTC_SETMATCHCOUNTER = 0x08,
    E_PM_CMDIDX_RTC_GETMATCHCOUNTER = 0x09,
    E_PM_CMDIDX_GET_INFO = 0x0A,
    E_PM_CMDIDX_GET_MASTERKEY = 0x0B,
    E_PM_CMDIDX_GET_DEVICEID = 0x0C,
    E_PM_CMDIDX_GET_CHIPID = 0x0D,
    E_PM_CMDIDX_PWM_INIT =0x0F,
    E_PM_CMDIDX_PWM_CONFIG =0x10,
    E_PM_CMDIDX_PWM_IRRECORD_RECEIVED_SETCALLBACK     = 0x11,
    E_PM_CMDIDX_PWM_IRRECORD_RECEIVED_REQUIRE        = 0x12,
    E_PM_CMDIDX_PWM_IRRECORD_RECEIVED_COMPLETE      =  0x13,
    E_PM_CMDIDX_PWM_IRRECORD_TRANSMIT               =  0x14,
    E_PM_CMDIDX_PWM_IRRECORD_RECEIVED_EXIT           =  0x15,
    E_PM_CMDIDX_LD_SEND =0x16,

    E_PM_CMDIDX_GLOBAL_CHIP_RESET = 0x21,


    E_PM_CMDIDX_ACK_51ToAEON = 0x30,
    E_PM_CMDIDX_ACK_51ToARM = 0x31,

    //51->Aeon Command Index
    E_PM_CMDIDX_ACK_AEONTo51 = 0xA0,

} EN_PM_CmdIndex;


//=============================================================================
// Variable
//=============================================================================


//=============================================================================
// Function
//=============================================================================
INTERFACE MS_S8 MDrv_LD_Setup(void);
INTERFACE void MDrv_LD_CleanupModule(void);
INTERFACE void MDrv_LD_Suspend(void);
INTERFACE void MDrv_LD_Resume(void);
INTERFACE void MDrv_LD_SetStrength(MS_U8 u8LDStrength); 
INTERFACE phys_addr_t MDrv_LD_GetDataAddr(EN_GET_DATA_TYPE type);
INTERFACE void MDrv_LDEnable(MS_BOOL bOn, MS_U8 u8Luma);
INTERFACE void MDrv_LD_SetDemoPattern(MS_U8 u8Pattern, MS_BOOL bOn);




#if defined(__cplusplus)
}
#endif


#endif //_MDRV_LD_ALGO_H

