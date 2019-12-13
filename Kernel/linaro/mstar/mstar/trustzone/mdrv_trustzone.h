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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_trustzone.h
/// @brief  MALLOC Driver Interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TRUSTZONE_H_
#define _DRV_TRUSTZONE_H_

#include "mdrv_trustzone_io.h"
#include "mdrv_trustzone_st.h"
//#include "mdrv_smc.h"
#include <linux/mutex.h>
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SMC_CORE 1

#define CIRC_BUF_SIZE 32
#define CIRC_ITEM_NUM CIRC_BUF_SIZE/4
#define CIRC_NAME_MAX_NUM 32

typedef enum 
{
    TZ_WQ_IRQ=0,
    TZ_WQ_TIMER,
    TZ_WQ_SYS,
    TZ_WQ_H,
    TZ_WQ_M,
    TZ_WQ_L,
    TZ_WQ_NUM
}TZ_WQ;

typedef enum 
{
    TZ_WQ_TYPE_TX,
    TZ_WQ_TYPE_RX
}TZ_WQ_TYPE;

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum {
      TZ_LLV_RES_OK                      //= 0
     ,TZ_LLV_RES_ERR                     //= 1
     ,TZ_LLV_RES_UND                     //= 2
     ,TZ_LLV_RES_MAX
}TZ_LLV_Result;


struct tz_struct{
    volatile unsigned int classID;
    volatile unsigned int cmdID;

    volatile unsigned int cmd0;
    volatile unsigned int cmd1;
    volatile unsigned int cmd2;
    volatile unsigned int cmd3;

    volatile unsigned int* private_data;
};

struct tz_cir_buf{
    unsigned int head;
    unsigned int tail;
    unsigned int buf_num;
    unsigned int buf_ID;
    unsigned int tz_qd_paddr;

};

struct tz_wq_des{
    unsigned int u32PAddr;
    unsigned int cmdSize;
    TZ_WQ  prio;
    TZ_WQ_TYPE type;
    void * handler;
    char name[32];
    struct mutex pmutex;
    struct mutex cmutex;
 
};

struct tz_queue_data{

    unsigned int classID;
    unsigned int cmdID;

    unsigned int cmd0;
    unsigned int cmd1;
    unsigned int cmd2;
    unsigned int cmd3;

    unsigned int crc;
};

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#define TZ_CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))
#define TZ_CIRC_SPACE(head,tail,size) TZ_CIRC_CNT((tail),((head)+1),(size))

//void smc_call(struct tz_struct *tz);
int tz_call(struct tz_struct *tz, TZ_WQ q);
#endif // _DRV_TRUSTZONE_H_

