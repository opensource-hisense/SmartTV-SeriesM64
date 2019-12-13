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

#ifndef __DRV_CMA_POOL_ST_H__
#define __DRV_CMA_POOL_ST_H__
#define KERN_CHUNK_NUM 3

#define CMA_FLAG_MAP_VMA    (1<<0)
#define CMA_FLAG_CACHED      (1<<1)
#define CMA_FLAG_VIRT_ADDR   (1<<2)
#define CMA_FLAG_MTLB_POOL  (1<<3)
#define CMA_FLAG_UNCACHED_FOR_TEE (1<<4)

#if defined(CONFIG_ARM) || defined(CONFIG_MIPS)
#ifdef CONFIG_MP_NEW_UTOPIA_32BIT
/* this is for 32-bit kernel and new_utopia(utopia2) */
struct cma_alloc_args {
    u64 offset_in_heap;
    unsigned long cpu_addr;
    unsigned long length;
    u64 align;
    unsigned int heap_id;
    unsigned int flags;
};

struct cma_free_args {
    unsigned int heap_id;
    u64 offset_in_heap;
    unsigned long length;
};

struct cma_heap_info {
    unsigned int heap_id;
    unsigned long flags;

    unsigned int miu;
    u64 bus_addr;
    u64 heap_miu_start_offset;
    unsigned long heap_length;
};
#else
/* this is for 32-bit kernel and old_utopia(utopia2) */
struct cma_alloc_args {
    unsigned long offset_in_heap;
    unsigned long cpu_addr;
    unsigned long length;        
    unsigned long align;
    unsigned int heap_id;
    unsigned int flags;
};

struct cma_free_args {
    unsigned int heap_id;
    unsigned long offset_in_heap;
    unsigned long length;
};

struct cma_heap_info {
    unsigned int heap_id;
    unsigned long flags;

    unsigned int miu;
    unsigned long bus_addr;
    unsigned long heap_miu_start_offset;
    unsigned long heap_length;
};
#endif
#elif defined(CONFIG_ARM64)
/* this is for 64-bit kernel and new_utopia(utopia_muji) */
#if defined(CONFIG_COMPAT)
/* compat size will be as 32-bit utopia */
struct compat_cma_heap_info {
    compat_uint_t heap_id;
    compat_ulong_t flags;

    compat_uint_t miu;
    compat_u64 bus_addr;
    compat_u64 heap_miu_start_offset;
    compat_size_t heap_length;
};
struct compat_cma_alloc_args {
    compat_u64 offset_in_heap;
    compat_ulong_t cpu_addr;
    compat_size_t length;        
    compat_u64 align;
	compat_uint_t heap_id;
    compat_uint_t flags;
};

struct compat_cma_free_args {
    compat_uint_t heap_id;
    compat_u64 offset_in_heap;
    compat_size_t length;
};
#endif

/* this is 64-bit data type, which will be like 64-bit utopia */
struct cma_heap_info {
    u32 heap_id;
    u64 flags;

    u32 miu;
    u64 bus_addr;
    u64 heap_miu_start_offset;
    size_t heap_length;
};

struct cma_alloc_args {
    u64 offset_in_heap;
    u64 cpu_addr;
    size_t length;        
    u64 align;
	u32 heap_id;
    u32 flags;
};

struct cma_free_args {
    u32 heap_id;
    u64 offset_in_heap;
    size_t length;
};
#endif // CONFIG_ARM
#endif

