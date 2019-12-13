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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mdrv_mtlb.h
/// @brief  MTLB Driver Interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// Linux Mhal_mtlb.h define start
// -----------------------------------------------------------------------------
#ifndef __DRV_MTLB_H__
#define __DRV_MTLB_H__
#include <linux/types.h>
#include "mdrv_mtlb_io.h"
#include "mdrv_mtlb_st.h"

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

/* get mtlb hardware info */
void MDrv_MTLB_Get_HWInfo(mtlb_hardware_info * hwinfo);

/* get mtlb hardware status */
int MDrv_MTLB_Get_HWStatus(mtlb_hardware_status * hwstatus);

/* get mtlb table size */
int MDrv_MTLB_TableSize(void);

/* do MTLB basic init */
int MDrv_MTLB_Init(void);

/* allocate MTLB entry memory */
int MDrv_MTLB_Reserve_Memory(void);

/* get zero page reserved in hal layer */
int MDrv_MTLB_Get_ZeroPage(unsigned long * u32miu0pa, unsigned long * u32miu1pa, unsigned long * u32miu2pa);

/* enable GE or GOP MTLB */
int MDrv_MTLB_Enable(mtlb_tlbclient_enable * tlbclient_enable);

/* check the pa array(memory access one time by MTLB) is support by MTLB hardware */
int MDrv_MTLB_Mapping_Address_Support(unsigned long * u32pa, unsigned long nents, unsigned char * miu);

/* do some work for mapping start */
void MDrv_MTLB_Mapping_Start(unsigned long *flag);

/* mapping work end */
void MDrv_MTLB_Mapping_End(unsigned long *flag, u8 u8miu, unsigned long start,  unsigned long size);


/* create mtbl mapping for specified va & pa*/
int MDrv_MTLB_Mapping(u8 u8miu, u32 u32va, u32 u32pa, bool paInc, u32 u32size);

/* dump MTLB info*/
int MDrv_MTLB_Dump(u32 *u32va, u32 *u32pa);
#endif


