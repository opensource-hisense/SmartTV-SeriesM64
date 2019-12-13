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
/// @file   Mhal_mtlb.h
/// @author MStar Semiconductor Inc.
/// @brief  MTLB Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// Linux Mhal_mtlb.h define start
// -----------------------------------------------------------------------------
#ifndef __DRV_MTLB__
#define __DRV_MTLB__

#ifndef CONFIG_MP_CMA_PATCH_MSTAR_ION_MTLB
#include "mdrv_mtlb.h"
#else
#include "mdrv_ion_mtlb.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Define Enable or Compiler Switches
//-------------------------------------------------------------------------------------------------
#define MTLB_ERR(fmt, args...)   printk(KERN_ERR "mtlb hal error %s:%d" fmt,__FUNCTION__,__LINE__,## args)
#define MTLB_DEBUG(fmt, args...)   printk(KERN_ERR "mtlb hal %s:%d" fmt,__FUNCTION__,__LINE__,## args)

//--------------------------------------------------------------------------------------------------
//  Constant definition
//--------------------------------------------------------------------------------------------------
#define TLB_Entry_BYTE_SIZE             4           /* tlb entry size = 4 bytes */
#define TLB_MAP_PAGE_SIZE               0x1000      /* tlb map per page size = 0x1000 bytes */

#define TLB_TAG_BIT_SHIFT               26
#define TLB_MIU_BIT_SHIFT               19

#if defined(CONFIG_ARM64)
extern ptrdiff_t mstar_pm_base;
#define MTLB_RIU_REG_BASE                   mstar_pm_base
#else
#define MTLB_RIU_REG_BASE                       0xFD000000
#endif

#define MTLB_RIU_BANK                       0x1628

/*8 bits address*/
#define REG_TLB_MODE                        0x00
    #define GE_TLB_FLUSH                        0x0008
#define REG_TLB_ENABLE                      0x02
    #define GE_TLB_ENABLE                        0x0001
#define REG_TLB_TAG                         0x20

#define TLB_GE_TAG_VALUE                          0x28         // (MTLB) GE Tag Value


void MHal_MTLB_Get_HWInfo(mtlb_hardware_info * hwinfo);
int MHal_MTLB_Get_HWStatus(mtlb_hardware_status * hwstatus);
int MHal_MTLB_TableSize(void);
int MHal_MTLB_Init(void);
int MHal_MTLB_Reserve_Memory(void);
int MHal_MTLB_Get_ZeroPage(unsigned long * u32miu0pa, unsigned long * u32miu1pa, unsigned long * u32miu2pa);
int MHal_MTLB_Enable(mtlb_tlbclient_enable * tlb_client_enable);
void MHal_MTLB_Mapping_Start(unsigned long *flag);
void MHal_MTLB_Mapping_End(unsigned long * flag, u8 u8miu, unsigned long start, unsigned long size);
int MHal_MTLB_Mapping(u8 u8miu, u32 u32va, u32 u32pa, bool paInc, u32 u32size);
int MHal_MTLB_Mapping_Address_Support(unsigned long * u32pa, unsigned long nents, unsigned char * miu);
int MHal_MTLB_Dump(u32 *u32va, u32 *u32pa);
#endif
// -----------------------------------------------------------------------------
// Linux Mhal_mtlb.h End
// -----------------------------------------------------------------------------


