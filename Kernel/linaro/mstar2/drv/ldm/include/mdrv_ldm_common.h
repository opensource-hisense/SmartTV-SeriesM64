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
/// @file   mdrv_ldm_common.h
/// @brief  MStar local dimming Interface header file
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LD_COMMON_H
#define _MDRV_LD_COMMON_H


#if defined(__cplusplus)
extern "C" {
#endif

//=============================================================================
// Defines & Macros
//=============================================================================


//=============================================================================
// Debug Macros
//=============================================================================


#define CHECK_POINTER_I(ptr)   \
    if((NULL == ptr) || ((void*)((size_t)-1) == ptr))  \
    {   \
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__); \
        return -1;    \
    }

#define CHECK_POINTER_V(ptr)   \
    if((NULL == ptr) || ((void*)((size_t)-1) == ptr))  \
    {   \
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__); \
        return;    \
    }

#define LD_ERROR(msg...)\
    do{\
        if(enDbgLevel&E_DEBUG_LEVEL_ERROR) \
        { \
            printk("\033[0;31m[LDM ERROR] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printk(msg);\
        } \
    }while(0)
    
#define LD_INFO(msg...)\
    do{\
        if(enDbgLevel&E_DEBUG_LEVEL_INFO) \
        { \
            printk("\033[0;33m[LDM INFO] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printk(msg);\
        } \
    }while(0)


#define LD_DEBUG(msg...)\
    do{\
        if(enDbgLevel&E_DEBUG_LEVEL_DEBUG) \
        { \
            printk("\033[0;34m[LDM DEBUG] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printk(msg);\
        } \
    }while(0)


#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))

//=============================================================================
// Type and Structure Declaration
//=============================================================================
typedef enum
{
    E_LD_LED_EDGE_TB_TYPE = 0, // deprecated
    E_LD_LED_EDGE_LR_TYPE = 1, // deprecated
    E_LD_LED_DIRECT_TYPE  = 2,
    E_LD_LED_LOCAL_TYPE   = 3,  // new mode supporting 32x32 LED
    E_LD_LED_TYPE_NUM,
    E_LD_LED_TYPE_MAX = E_LD_LED_TYPE_NUM,
} EN_LD_TYPE;

/*
typedef enum
{
    EN_DEBUG_LEVEL_ERROR=0x01,
    EN_DEBUG_LEVEL_INFO=0x02,
    EN_DEBUG_LEVEL_DEBUG=0x04,
    EN_DEBUG_LEVEL_ALL=0x07,
    EN_DEBUG_LEVEL_MAX   
}EN_DEBUG_LEVEL;
*/



//=============================================================================
// Variable
//=============================================================================


//=============================================================================
// Function
//=============================================================================


#if defined(__cplusplus)
}
#endif


#endif //_MDRV_LD_COMMON_H

