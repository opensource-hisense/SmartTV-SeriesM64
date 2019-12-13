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

#ifndef __DRV_MCMA_H__
#define __DRV_MCMA_H__
#include "mdrv_cma_pool_io.h"
#include "mdrv_cma_pool_st.h"
#define MIU_BLOCK_NUM 4

typedef enum
{
    MIU_BLOCK_IDLE = 0,
    MIU_BLOCK_BUSY
}MIU_PROTECT_BLOCK_STATUS;

struct cma_buffer{
    int freed;
    int mapped;
    void *cpu_addr;
    struct page * page;
    pid_t pid;
    //bus address
    unsigned long start_pa;
    unsigned long length;
    struct list_head list;
};

struct cma_allocation_list{
    unsigned long min_start;
    unsigned long max_end;
    unsigned long using_count;

    unsigned long freed_count;
    struct list_head list_head;
};

struct MIU_ProtectRanges
{
    unsigned char miu;
    MIU_PROTECT_BLOCK_STATUS miuBlockStatus[MIU_BLOCK_NUM];

    unsigned int krange_num;
    struct list_head list_head;
    struct mutex lock;
};

typedef struct
{
    unsigned char miuBlockIndex;
    unsigned long start_pa;
    unsigned long length;
}MIU_ProtectRange_Record;

typedef struct
{
    MIU_ProtectRange_Record old;
    MIU_ProtectRange_Record new;
}MIU_ProtectRange_pair;

#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
#define CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE  (1<<0)
#endif

struct cma_memory_info{
    char miu;
    unsigned int heap_id;
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
    unsigned int flags;
#endif
    struct device *dev;
    struct cma_allocation_list alloc_list;
    struct MIU_ProtectRanges * pranges;

    struct list_head list_node;
    struct mutex lock;
};

struct heap_proc_info
{
    unsigned int heap_id;
    unsigned long base_phy;
    struct vm_area_struct *vma;
    bool vma_cached;
    struct list_head list_node;
};

typedef struct
{
    unsigned char miuBlockIndex;
    unsigned long start_pa;
    unsigned long length;
    struct list_head list_node;
}MIU_ProtectRange;

typedef struct
{
    pid_t pid;

    unsigned int temp_heap_id;

    struct list_head list_heap_proc_info;
    struct mutex lock;
}filp_private;

typedef struct
{
    unsigned char miu;
    int krange_change_num;
    MIU_ProtectRange_pair pair[KERN_CHUNK_NUM];
}MIU_KERN_PRange_Info_user;
#endif

