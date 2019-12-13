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
/// @file   cma_mpool_manager.h
/// @brief  CMA mpool Manager interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CMA_POOL_IO_H__
#define __DRV_CMA_POOL_IO_H__
#include "mdrv_cma_pool_st.h"

//-------------------------------------------------------------------------------------------------
//  IO command
//-------------------------------------------------------------------------------------------------
#define CMA_POOL_IOC_MAGIC   'P'

#define CMA_POOL_IOC_INIT   _IOWR(CMA_POOL_IOC_MAGIC, 0x00, struct cma_heap_info)
#define COMPAT_CMA_POOL_IOC_INIT   _IOWR(CMA_POOL_IOC_MAGIC, 0x00, struct compat_cma_heap_info)
#define CMA_POOL_IOC_ALLOC  _IOWR(CMA_POOL_IOC_MAGIC, 0x02, struct cma_alloc_args)
#define COMPAT_CMA_POOL_IOC_ALLOC  _IOWR(CMA_POOL_IOC_MAGIC, 0x02, struct compat_cma_alloc_args)
#define CMA_POOL_IOC_FREE   _IOWR(CMA_POOL_IOC_MAGIC, 0x03, struct cma_free_args)
#define COMPAT_CMA_POOL_IOC_FREE   _IOWR(CMA_POOL_IOC_MAGIC, 0x03, struct compat_cma_free_args)
#define CMA_POOL_IOC_RELEASE   _IOW(CMA_POOL_IOC_MAGIC, 0x04, unsigned int)


#endif

