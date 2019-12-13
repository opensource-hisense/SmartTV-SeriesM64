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
/// @file   Mhal_mtlb.c
/// @brief  MTLB Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/atomic.h>
#include <linux/spinlock.h>
#include <linux/memblock.h>
#include <linux/vmalloc.h>
#include <asm/io.h>

#include "mdrv_types.h"
#include "mdrv_system.h"
#include "mhal_mtlb.h"

#include "chip_setup.h"

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macros
//-------------------------------------------------------------------------------------------------
#define Write_TLB_Entry(entry, data)               *(volatile u32*)(entry) = data;           

//-------------------------------------------------------------------------------------------------
//  Local Variable
//-------------------------------------------------------------------------------------------------
static spinlock_t mhal_mtlb_lock = __SPIN_LOCK_UNLOCKED(mhal_mtlb_lock);
static DEFINE_MUTEX(mhal_mtlb_mutex);
static int mtlb_init = 0;
static int mtlb_enable = 0;
static phys_addr_t phys_addr_miu0 = 0, phys_addr_miu1 = 0, phys_addr_miu2 = 0;

static void * virt_addr_miu0 = 0;
static void * virt_addr_miu1 = 0;
static void * virt_addr_miu2 = 0;


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TABLE_SIZE (32*1024)  //etnry size 32K, one entry = one page
#define REVERVE_TABLE_SIZE  (TABLE_SIZE * 4) //128K
//reserve one more page for zero page
#define REVERVE_MEMORY_SIZE  (REVERVE_TABLE_SIZE + 4096) //128K + 4K


//-------------------------------------------------------------------------------------------------
//  MTLB HAL internal function
//-------------------------------------------------------------------------------------------------
static int _getAddrByMiu(u8 u8miu, u32 *miu_bus_base, void ** addr_tlb_table)
{   
    /* check miu, monaco has 3 mius: 0 1 2*/
    if(u8miu > 2)
    {
        MTLB_ERR("invalid miu %d\n", (int)u8miu);       
        return -EINVAL;
    }

    if(u8miu == 0)
    {
       *miu_bus_base = ARM_MIU0_BUS_BASE;
       *addr_tlb_table = virt_addr_miu0;
    }
    else if(u8miu == 1)
    {
       *miu_bus_base = ARM_MIU1_BUS_BASE;
       *addr_tlb_table = virt_addr_miu1;
    }
    else if(u8miu == 2)
    {
       *miu_bus_base = ARM_MIU2_BUS_BASE;
       *addr_tlb_table = virt_addr_miu2;
    }

    if(*addr_tlb_table <= 0)
    {
        MTLB_ERR("error: miu %u, addr_tlb_table <= 0\n", (unsigned int)u8miu);        
        return -EINVAL;    
    }

    return 0;
}
//-------------------------------------------------------------------------------------------------
//  MTLB HAL function
//-------------------------------------------------------------------------------------------------
extern unsigned long lx_mem_addr;
extern unsigned long lx_mem2_addr;
extern unsigned long lx_mem3_addr;

/* get mtlb hardware info */
void MHal_MTLB_Get_HWInfo(mtlb_hardware_info * hwinfo)
{    
    hwinfo->MIU_Has_TLB = false;
    hwinfo->GE_Has_TLB = true;
    hwinfo->GOP_Has_TLB = true;
    
    hwinfo->GE_DEDICAT_TLB_Switchoff_Support = true;
    hwinfo->GOP_DEDICAT_TLB_Switchoff_Support = true;
    
    hwinfo->TLB_GOP_IDS = (E_MTLB_GOP0|E_MTLB_GOP2);
    hwinfo->TLB_Support_Clients = E_MTLB_CLIENT_INVALIDE;    
    hwinfo->GE_Need_CQFlush = false;

    hwinfo->TLB_MAPTO_MIU = 0;
    if(lx_mem_addr != INVALID_PHY_ADDR)
        hwinfo->TLB_MAPTO_MIU |= E_MTLB_MIU_0;
    
    if(lx_mem2_addr != INVALID_PHY_ADDR)
        hwinfo->TLB_MAPTO_MIU |= E_MTLB_MIU_1;
    
    if(lx_mem3_addr != INVALID_PHY_ADDR)
        hwinfo->TLB_MAPTO_MIU |= E_MTLB_MIU_2; 

    hwinfo->tlb_table_addr_miu0 = phys_addr_miu0;
    hwinfo->tlb_table_addr_miu1 = phys_addr_miu1;
    hwinfo->tlb_table_addr_miu2 = phys_addr_miu2;
}

/* get mtlb hardware status */
int MHal_MTLB_Get_HWStatus(mtlb_hardware_status * hwstatus)
{
    /* add code here */
    return 0;
}

/*8-bit RIU address*/
u8 MHal_MTLB_ReadReg8( u32 u32bank, u32 u32reg )
{
    u8 val;
    u32 address = MTLB_RIU_REG_BASE + u32bank*0x100*2;
    address = address + (u32reg << 1) - (u32reg & 1);

    val = *( ( volatile u8* ) address );
    return val;
}

void MHal_MTLB_WritReg8( u32 u32bank, u32 u32reg, u8 u8val )
{
    u32 address = MTLB_RIU_REG_BASE + u32bank*0x100*2;
    address = address + (u32reg << 1) - (u32reg & 1);

    *( ( volatile u8* ) address ) = u8val;
}

void MHal_MTLB_WritReg8Bit( u32 u32bank, u32 u32reg, bool bEnable, u8 u8Mask )
{
    u8 val = MHal_MTLB_ReadReg8( u32bank, u32reg );
    val = (bEnable) ? (val | u8Mask) : (val & ~u8Mask );
    MHal_MTLB_WritReg8( u32bank, u32reg, val );
}

extern phys_addr_t arm_lowmem_limit;
static void * _mtlb_phys_to_virtul(phys_addr_t  phys_addr)
{
    void * virt_addr = 0;
    pgprot_t pgprot;
    int npages = PAGE_ALIGN(REVERVE_MEMORY_SIZE) / PAGE_SIZE;
    struct page **pages = vmalloc(sizeof(struct page *) * npages);
    struct page **tmp = pages;
    struct page *page = NULL;
    int i = 0;

    //if in low memory range, kernel already map it
    if(phys_addr < arm_lowmem_limit)
    {
        virt_addr = (void *)(__phys_to_virt(phys_addr));
    }
    else  // in high memroy
    {
        pgprot = pgprot_writecombine(PAGE_KERNEL);
        page = __pfn_to_page(__phys_to_pfn(phys_addr));

        for(i=0; i < npages; ++i)
        {
            *(tmp++) = page++;
        }

        virt_addr = vmap(pages, npages, VM_MAP, pgprot);
	    vfree(pages);
    }

    return virt_addr;    
}


/*
  * do MTLB basic init
  */
int MHal_MTLB_Init(void)
{    
    int ret = 0;
   
    mutex_lock(&mhal_mtlb_mutex);
    if(mtlb_init > 0)
        goto MHAL_MTLB_INIT_DONE;
    
    if(phys_addr_miu0 > 0)
    {  
        
        virt_addr_miu0 = _mtlb_phys_to_virtul(phys_addr_miu0);
        if(virt_addr_miu0 == NULL)
        {
            ret = -ENOMEM;
            goto MHAL_MTLB_INIT_DONE;
        }
        //MTLB_DEBUG("phys_addr_miu0 %08x , virt_addr_miu0 %p\n", phys_addr_miu0, virt_addr_miu0);
        
        memset((void *)((unsigned long)virt_addr_miu0+REVERVE_TABLE_SIZE), 0, PAGE_SIZE);
    }

    if(phys_addr_miu1 > 0)
    {        
        virt_addr_miu1= _mtlb_phys_to_virtul(phys_addr_miu1);
        if(virt_addr_miu1 == NULL)
        {
            ret = -ENOMEM;
            goto MHAL_MTLB_INIT_DONE;
        }        
        //MTLB_DEBUG("phys_addr_miu1 %08x , virt_addr_miu1 %p\n", phys_addr_miu1, virt_addr_miu1);
        
        memset((void *)((unsigned long)virt_addr_miu1+REVERVE_TABLE_SIZE), 0, PAGE_SIZE);
    }

    if(phys_addr_miu2 > 0)
    {
        virt_addr_miu2= _mtlb_phys_to_virtul(phys_addr_miu2);
        if(virt_addr_miu2 == NULL)
        {
            ret = -ENOMEM;
            goto MHAL_MTLB_INIT_DONE;
        }        
        //MTLB_DEBUG("phys_addr_miu2 %08x , virt_addr_miu2 %p\n", phys_addr_miu2, virt_addr_miu2);
        
        memset((void *)((unsigned long)virt_addr_miu2+REVERVE_TABLE_SIZE), 0, PAGE_SIZE);
    }

    mtlb_init = 1;

MHAL_MTLB_INIT_DONE:  
    mutex_unlock(&mhal_mtlb_mutex);
    return ret;   
}

/*
  * get mtlb table size
  */
int MHal_MTLB_TableSize(void)
{
    return TABLE_SIZE;
}

static unsigned long _find_in_range(phys_addr_t start, phys_addr_t end, phys_addr_t size, phys_addr_t alignfactor)
{
    phys_addr_t ret = 0;

    if((arm_lowmem_limit > start) && (arm_lowmem_limit < end))
    {
        ret = memblock_find_in_range(start, arm_lowmem_limit, size, alignfactor);
        if(ret > 0)
            return ret;

        ret = memblock_find_in_range(arm_lowmem_limit, end, size, alignfactor);
        if(ret > 0)
            return ret;      
    }
    else if(end > start)
    {
        ret = memblock_find_in_range(start, end, size, alignfactor);
        if(ret > 0)
            return ret;
    }

    return ret;
}

int MHal_MTLB_Reserve_Memory(void)
{
    //reseve memory in MIU0
    if(lx_mem_addr != INVALID_PHY_ADDR)
    {
        phys_addr_miu0 = _find_in_range(ARM_MIU0_BUS_BASE, ARM_MIU1_BUS_BASE, REVERVE_MEMORY_SIZE, PAGE_SIZE);
        if(phys_addr_miu0 > 0)
        {
            if(memblock_is_region_reserved(phys_addr_miu0, REVERVE_MEMORY_SIZE) || 
                memblock_reserve(phys_addr_miu0, REVERVE_MEMORY_SIZE) < 0)
            {
                phys_addr_miu0 = 0;
                return -EINVAL;
            }
        }
    }

    //reseve memory in MIU1
    if(lx_mem2_addr != INVALID_PHY_ADDR)
    {
        phys_addr_miu1 = _find_in_range(ARM_MIU1_BUS_BASE, ARM_MIU2_BUS_BASE, REVERVE_MEMORY_SIZE, PAGE_SIZE);
        if(phys_addr_miu1 > 0)
        {
            if(memblock_is_region_reserved(phys_addr_miu1, REVERVE_MEMORY_SIZE) || 
                memblock_reserve(phys_addr_miu1, REVERVE_MEMORY_SIZE) < 0)

            {
                memblock_free(phys_addr_miu0, REVERVE_MEMORY_SIZE);
                phys_addr_miu0 = phys_addr_miu1 = phys_addr_miu2 = 0;
                return -EINVAL;
            }
        }
    }

    //reseve memory in MIU2
    if(lx_mem3_addr != INVALID_PHY_ADDR)
    {
        phys_addr_miu2 = _find_in_range(ARM_MIU2_BUS_BASE, ARM_MIU3_BUS_BASE, REVERVE_MEMORY_SIZE, PAGE_SIZE);
        if(phys_addr_miu2 > 0)
        {
            if(memblock_is_region_reserved(phys_addr_miu2, REVERVE_MEMORY_SIZE) || 
                memblock_reserve(phys_addr_miu2, REVERVE_MEMORY_SIZE) < 0)

            {
                memblock_free(phys_addr_miu0, REVERVE_MEMORY_SIZE);
                memblock_free(phys_addr_miu1, REVERVE_MEMORY_SIZE);
                phys_addr_miu0 = phys_addr_miu1 = phys_addr_miu2 = 0;
                return -EINVAL;
            }
        }
    }

    return 0;
}

int MHal_MTLB_Get_ZeroPage(unsigned long * u32miu0pa, unsigned long * u32miu1pa, unsigned long * u32miu2pa)
{
    *u32miu0pa = *u32miu1pa =  *u32miu2pa = 0;

    if(phys_addr_miu0 > 0)
    {
        *u32miu0pa = phys_addr_miu0 + REVERVE_TABLE_SIZE;
    }

    if(phys_addr_miu1 > 0)
    {
        *u32miu1pa = phys_addr_miu1 + REVERVE_TABLE_SIZE;
    }

    if(phys_addr_miu2 > 0)
    {
        *u32miu2pa = phys_addr_miu2 + REVERVE_TABLE_SIZE;
    }

    return 0;
}

/*
  * only enable TLB writer, and utopia will control GOP/GE enable 
  */
int MHal_MTLB_Enable(mtlb_tlbclient_enable * tlb_client_enable)
{        
    mutex_lock(&mhal_mtlb_mutex);

    if(mtlb_enable > 0)
        goto MHAL_MTLB_ENABLE_DONE;
    
    /* Set TLB Tag */
    MHal_MTLB_WritReg8(MTLB_RIU_BANK, REG_TLB_TAG, TLB_GE_TAG_VALUE);
    /*Enable TLB write*/
    MHal_MTLB_WritReg8Bit(MTLB_RIU_BANK, REG_TLB_ENABLE, 1, GE_TLB_ENABLE);    
    mtlb_enable = 1;

MHAL_MTLB_ENABLE_DONE:
    mutex_unlock(&mhal_mtlb_mutex);
    return 0;
}

/*
  *  check the pa array(memory access one time by MTLB) is support by MTLB hardware
  */
int MHal_MTLB_Mapping_Address_Support(unsigned long * u32pa, unsigned long nents, unsigned char * miu)
{
    unsigned long i = 0, min = 0, max = 0;
 

    if(nents <= 0)
        return -EINVAL;

    if(u32pa[0] >= ARM_MIU2_BUS_BASE)
    {
        *miu = 2;
        min = ARM_MIU2_BUS_BASE;
        max = (-1UL);

        return -EINVAL;
        //todo: tlbtable reserve for miu2
    }
    else if(u32pa[0] >= ARM_MIU1_BUS_BASE)
    {
        *miu = 1;
        min = ARM_MIU1_BUS_BASE;
        max = ARM_MIU2_BUS_BASE;
    }
    else if(u32pa[0] >= ARM_MIU0_BUS_BASE)
    {
        *miu = 0;
        min = ARM_MIU0_BUS_BASE;
        max = ARM_MIU1_BUS_BASE;
    }    
    else
        return -EINVAL;    

    for(i = 1; i < nents; ++i)
    {
        if(u32pa[i] < min || u32pa[i] >= max)
            return -EINVAL;            
    }

    return 0;
}

void MHal_MTLB_Mapping_Start(unsigned long * flag)
{
    spin_lock_irqsave(&mhal_mtlb_lock, *flag);
}

void MHal_MTLB_Mapping_End(unsigned long * flag, u8 u8miu, unsigned long start, unsigned long size)
{
    u32 miu_bus_base = 0, u32header = 0;
    void * virt_addr_tlbtable = 0;
    
    if(_getAddrByMiu(u8miu, &miu_bus_base, &virt_addr_tlbtable))
    {        
        MTLB_ERR("flush cache fail\n"); 
        return;
    }

    u32header = (unsigned long)virt_addr_tlbtable + start * TLB_Entry_BYTE_SIZE;

    /* Flush cache and miu pipe to push data to sram*/
    Chip_Flush_Cache_Range(u32header, size * TLB_Entry_BYTE_SIZE);
    spin_unlock_irqrestore(&mhal_mtlb_lock, *flag);
}


/*  only update the tlb table which with pa locate at same miu
  *  
  */
int MHal_MTLB_Mapping(u8 u8miu, u32 u32va, u32 u32pa, bool paInc, u32 u32size)
{
    u32 u32header = 0;
    u32 u32data = 0, entry_pa = 0;
    u32 miu_bus_base = 0;
    void * virt_addr_tlbtable = 0;
   
    /* check if mtlb enable or not*/
    if(mtlb_enable == 0)
    {
        MTLB_ERR("MHal mtlb not enabled\n");        
        return -EPERM;
    }
        
    /* Virtual address and physical address should be align to 0x1000 */
    if((u32va & (0x1000-1)) || (u32pa & (0x1000-1)))
    {
        MTLB_ERR("u32va = 0x%x, u32pa = 0x%x, which should align to 0x1000\n", u32va, u32pa);        
        return -EINVAL;
    }

    /* Size should be be align to 0x1000 */
    if(u32size & (0x1000-1))
    {
        MTLB_ERR("u32size = 0x%x, which should align to 0x1000\n", u32size);        
        return -EINVAL;
    }

    if(_getAddrByMiu(u8miu, &miu_bus_base, &virt_addr_tlbtable))      
        return -EINVAL;

    mb();
    while(u32size != 0)
    {
        u32header = (unsigned long)virt_addr_tlbtable + (u32va>>12) * TLB_Entry_BYTE_SIZE;
        entry_pa = (u32pa-miu_bus_base)/TLB_MAP_PAGE_SIZE;
        u32data = (TLB_GE_TAG_VALUE<<TLB_TAG_BIT_SHIFT) | (u8miu<<TLB_MIU_BIT_SHIFT) | entry_pa;
        
        if(u32header != 0)
        {
            //FIXME: maybe we don't need below mb()
            mb();            
            Write_TLB_Entry(u32header, u32data); 
            mb();
        }
        else
        {
            MTLB_ERR("ERROR: Get wrong TLB entry address \n");
            return -EINVAL;
        }

        if(paInc)
        {
            u32pa += 0x1000;
        }
        
        u32va += 0x1000;
        u32size -= 0x1000;
    }
    mb();
    
    return 0;
}

int MHal_MTLB_Dump(u32 *u32va, u32 *u32pa)
{
    MTLB_ERR("MHal_MTLB_Dump \n");
    MTLB_ERR("addr_miu0=0x%x , addr_miu1=0x%x\n", phys_addr_miu0, phys_addr_miu1);
    return 0;
}

