
#ifndef _MDRV_MPOOL_ST_H_
#define _MDRV_MPOOL_ST_H_


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// MPOOL_IOC_INFO 

#if defined(CONFIG_ARM) || defined(CONFIG_MIPS)
#ifdef CONFIG_MP_NEW_UTOPIA_32BIT
typedef struct
{
    u64                u32Addr;
    u64                u32Size;
    u64                u32Interval;
    unsigned char      u8MiuSel;
} DrvMPool_Info_t;


typedef struct
{
    size_t             u32AddrVirt;
    u64                u32AddrPhys;
    u64                u32Size;
} DrvMPool_Flush_Info_t;

typedef struct
{
    u64                LX_MEM_ADDR;
    u64                LX_MEM_LENGTH;
    u64                LX_MEM2_ADDR;
    u64                LX_MEM2_LENGTH;
    u64                EMAC_ADDR;
    u64                EMAC_LENGTH;
    u64                DRAM_ADDR;
    u64                DRAM_LENGTH;
} De_Sys_Info_t;

typedef struct
{
    u64                u32lxAddr;
    u64                u32lxSize;
    u64                u32lx2Addr;
    u64                u32lx2Size;
} DrvMPool_Kernel_Info_t;

typedef struct{
    unsigned int       ASID;
    unsigned int       global;
    u64                u32AddrVirt;
    unsigned int       rwx;
    unsigned int       mask;
} DrvMPool_Watchpt_Info_t;

typedef struct{
    unsigned int       wvr0;
    unsigned int       wvr1;
    unsigned int       wcr0;
    unsigned int       wcr1;
} DrvMPool_Wcvr_Info_t;
#else
typedef struct
{
    unsigned int                u32Addr;
    unsigned int                u32Size;
    unsigned int                u32Interval;
    unsigned char               u8MiuSel;
} DrvMPool_Info_t;

typedef struct
{
    unsigned int                u32AddrVirt;
    unsigned int                u32AddrPhys;
    unsigned int                u32Size;
} DrvMPool_Flush_Info_t;

typedef struct
{
    unsigned int                LX_MEM_ADDR;
    unsigned int                LX_MEM_LENGTH;
    unsigned int                LX_MEM2_ADDR;
    unsigned int                LX_MEM2_LENGTH;
    unsigned int                EMAC_ADDR;
    unsigned int                EMAC_LENGTH;
    unsigned int                DRAM_ADDR;
    unsigned int                DRAM_LENGTH;
} De_Sys_Info_t;

typedef struct
{
    unsigned int                u32lxAddr;
    unsigned int                u32lxSize;
    unsigned int                u32lx2Addr;
    unsigned int                u32lx2Size;
} DrvMPool_Kernel_Info_t;

typedef struct{
    unsigned int                ASID;
    unsigned int                global;
    unsigned int                u32AddrVirt;
    unsigned int                rwx;
    unsigned int                mask;
} DrvMPool_Watchpt_Info_t;

typedef struct{
    unsigned int                wvr0;
    unsigned int                wvr1;
    unsigned int                wcr0;
    unsigned int                wcr1;
} DrvMPool_Wcvr_Info_t;
#endif //CONFIG_MP_NEW_UTOPIA_32BIT
#elif defined(CONFIG_ARM64) //64bit kernel and 64bit physical
#if defined(CONFIG_COMPAT)
typedef struct
{
    compat_size_t                u32AddrVirt;
    compat_u64                   u32AddrPhys;
    compat_u64                   u32Size;
} DrvMPool_Flush_Info_t32;
#endif

typedef struct
{
    u64                u32Addr;
    u64                u32Size;
    u64                u32Interval;
    unsigned char      u8MiuSel;
} DrvMPool_Info_t;


typedef struct
{
    size_t             u32AddrVirt;
    u64                u32AddrPhys;
    u64                u32Size;
} DrvMPool_Flush_Info_t;

typedef struct
{
    u64                LX_MEM_ADDR;
    u64                LX_MEM_LENGTH;
    u64                LX_MEM2_ADDR;
    u64                LX_MEM2_LENGTH;
    u64                EMAC_ADDR;
    u64                EMAC_LENGTH;
    u64                DRAM_ADDR;
    u64                DRAM_LENGTH;
} De_Sys_Info_t;

typedef struct
{
    u64                u32lxAddr;
    u64                u32lxSize;
    u64                u32lx2Addr;
    u64                u32lx2Size;
} DrvMPool_Kernel_Info_t;

typedef struct{
    unsigned int       ASID;
    unsigned int       global;
    u64                u32AddrVirt;
    unsigned int       rwx;
    unsigned int       mask;
} DrvMPool_Watchpt_Info_t;

typedef struct{
    unsigned int       wvr0;
    unsigned int       wvr1;
    unsigned int       wcr0;
    unsigned int       wcr1;
} DrvMPool_Wcvr_Info_t;
#endif //CONFIG_ARM

#endif // _MDRV_MPOOL_ST_H_

