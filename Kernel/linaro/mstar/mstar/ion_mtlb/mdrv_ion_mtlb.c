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
/// @file   Mdrvl_mtlb.c
/// @brief  MTLB Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#ifndef CONFIG_MP_CMA_PATCH_MSTAR_ION_MTLB
#include "mdrv_mtlb.h"
#include "mhal_mtlb.h"
#else
#include "mdrv_ion_mtlb.h"
#include "mhal_ion_mtlb.h"
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MTLB_DPRINTK(fmt, args...) printk(KERN_ERR "%s:%d " fmt,__FUNCTION__,__LINE__,## args)


/* get mtlb hardware info */
void MDrv_MTLB_Get_HWInfo(mtlb_hardware_info * hwinfo)
{
    MHal_MTLB_Get_HWInfo(hwinfo);
}

/* get mtlb hardware status */
int MDrv_MTLB_Get_HWStatus(mtlb_hardware_status * hwstatus)
{
	return MHal_MTLB_Get_HWStatus(hwstatus);
}

/*
  * get mtlb table size
  */
int MDrv_MTLB_TableSize()
{
	return MHal_MTLB_TableSize();    
}

/*
  * do MTLB basic init
  */
int MDrv_MTLB_Init(void)
{
    return MHal_MTLB_Init();	
}

/*
  *    reserve memory for MTLB entry
  */
int MDrv_MTLB_Reserve_Memory(void)
{
    return MHal_MTLB_Reserve_Memory();
}

/*
  *  get zero page reserved in hal layer
  */
int MDrv_MTLB_Get_ZeroPage(unsigned long * u32miu0pa, unsigned long * u32miu1pa, unsigned long * u32miu2pa)
{
    return MHal_MTLB_Get_ZeroPage(u32miu0pa, u32miu1pa, u32miu2pa);
}

/*
  * enable GE or GOP MTLB
  */
int MDrv_MTLB_Enable(mtlb_tlbclient_enable * tlb_client_enable)
{
    return MHal_MTLB_Enable(tlb_client_enable);
}

/*
  *  check the pa array(memory access one time by MTLB) is support by MTLB hardware
  */
int MDrv_MTLB_Mapping_Address_Support(unsigned long * u32pa, unsigned long nents, unsigned char * miu)
{
    return MHal_MTLB_Mapping_Address_Support(u32pa, nents, miu);
}

/*
  * do some work for mapping start
  */
void MDrv_MTLB_Mapping_Start(unsigned long *flag)
{
    MHal_MTLB_Mapping_Start(flag);
}

/*
  * mapping work end
  */
void MDrv_MTLB_Mapping_End(unsigned long *flag, u8 u8miu, unsigned long start,  unsigned long size)
{    
    MHal_MTLB_Mapping_End(flag, u8miu, start, size);
}

/*
  * map <va, pa> array in MTLB
  */
int MDrv_MTLB_Mapping(u8 u8miu, u32 u32va, u32 u32pa, bool paInc, u32 u32size)
{
    return MHal_MTLB_Mapping(u8miu, u32va, u32pa, paInc, u32size);
}

int MDrv_MTLB_Dump(u32 *u32va, u32 *u32pa)
{
    return MHal_MTLB_Dump(u32va, u32pa);
}

