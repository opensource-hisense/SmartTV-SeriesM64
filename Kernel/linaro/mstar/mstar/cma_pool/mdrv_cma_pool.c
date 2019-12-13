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
/// @file   mdrv_cma_mpool.c
/// @brief  CMA mpool interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <linux/version.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/debugfs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/dma-contiguous.h>
#include <linux/dma-mapping.h>

#include <linux/delay.h>

#if defined(CONFIG_COMPAT)
#include <linux/compat.h>
#endif

#include <ion.h>
#include "mdrv_types.h"
#include "mst_devid.h"
#include "mdrv_system.h"
#include "mdrv_cma_pool.h"
#include "mdrv_miu.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PHYSICAL_START_INIT     (~0UL)
#define PHYSICAL_END_INIT       0
#define INVALID_MIU          0xFF
#define INVALID_PID          0
#define MAX_ALLOC_TRY        30
#define MOD_CMAPOOL_DEVICE_COUNT     1
#define FREE_FROM_BEGINNING       0x01UL
#define FREE_FROM_END             0x10UL
#define MOD_CMAPOOL_NAME             "cmapool"
#define ION_DEVICE_NAME              "/dev/ion"
#define MIU_PROTECT_ENABLE 1
#define MIU_PROTECT_DISABLE 0

#define CMA_POOL_DUMP 1
#define CMA_POOL_KERNEL_PROTECT_DUMP 1

#define MCMA_BUG_ON(cond)  \
do { \
      if(cond) \
        printk(KERN_ERR "MCMA_BUG in %s @ %d \n", __FUNCTION__, __LINE__); \
      BUG_ON(cond); \
   } while(0)

#define MCMA_CUST_ERR(fmt, args...) printk(KERN_ERR "MCMA error %s:%d " fmt,__FUNCTION__,__LINE__,## args)
#define MCMA_CUST_WARNING(fmt, args...) printk(KERN_ERR "MCMA warning %s:%d " fmt,__FUNCTION__,__LINE__,## args)

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef enum
{
    CMA_ALLOC = 0,
    CMA_FREE
}BUFF_OPS;

typedef struct
{
    int                         s32Major;
    int                         s32Minor;
    struct dentry *              debug_root;
    struct cdev                 cDevice;
    struct file_operations      CMAPoolFop;
} CMAPoolModHandle;

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------
static int CMA_Pool_Open(struct inode *inode, struct file *filp);
static int CMA_Pool_Release(struct inode *inode, struct file *filp);
static int CMA_Pool_Alloc(unsigned int heap_id, unsigned long offset_in_heap, unsigned long * cpu_addr,
    unsigned long length, unsigned long align, unsigned int flags, filp_private * pdev);
static int CMA_Pool_Free(unsigned int heap_id, unsigned long offset_in_heap, unsigned long length, filp_private * pdev);
static long CMA_Pool_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

#if defined(CONFIG_COMPAT)
static long Compat_CMA_Pool_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

static int CMA_Pool_MMap(struct file *filp, struct vm_area_struct *vma);
int _cma_cpu_page_fault_handler(struct vm_area_struct *vma, struct vm_fault *vmf);
void _cma_vma_close(struct vm_area_struct * vma);
static unsigned long CMA_Pool_Mapping(struct heap_proc_info * proc_info, unsigned long start_pa, unsigned long length, filp_private *pdev);
static int CMA_Pool_Unmapping(struct heap_proc_info * proc_info, unsigned long start_pa, unsigned long length, filp_private *pdev);
static void dump_CMA_Pool(struct cma_memory_info * heap_info, bool force_print);
static void free_buffer_list(struct cma_memory_info * heap_info, struct cma_buffer ** release_buf_front , struct cma_buffer ** release_buf_back);
static int deleteKRange(struct cma_memory_info * heap_info, struct cma_buffer * buffer);
static int addKRange(struct cma_memory_info * heap_info, struct cma_buffer * buffer);
static void dumpKRange(struct cma_memory_info * heap_info);
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
export int modify_kpage_mapping_sg(struct sg_table *table,unsigned int type);
export int modify_kpage_mapping_conti(struct page *page, int cnt,unsigned int type);
#endif
//---------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static struct MIU_ProtectRanges glob_miu_kranges[KERN_CHUNK_NUM]; //record kernel protect ranges on 3 MIUs
static DEFINE_MUTEX(heap_info_lock);
static struct list_head heap_info_list; //record heap related info
static unsigned char *kernal_protect_client_id = NULL;
static atomic_t kprotect_enabled = ATOMIC_INIT(1);

static struct class *cmapool_class;
static CMAPoolModHandle CMAPoolDev =
{
    .s32Major=               MDRV_MAJOR_CMAPOOL,
    .s32Minor=               MDRV_MINOR_CMAPOOL,
    .debug_root=             NULL,
    .cDevice=
    {
        .kobj=                  {.name= MOD_CMAPOOL_NAME, },
        .owner  =               THIS_MODULE,
    },
    .CMAPoolFop=
    {
        .open=                  CMA_Pool_Open,
        .release=               CMA_Pool_Release,
        .mmap=                  CMA_Pool_MMap,
        .unlocked_ioctl=        CMA_Pool_ioctl,
#if defined(CONFIG_COMPAT)
		.compat_ioctl 	= 		Compat_CMA_Pool_ioctl,
#endif
    },
};

static struct vm_operations_struct cma_vm_ops =
{
    .close = _cma_vma_close,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
    .fault = _cma_cpu_page_fault_handler
#else
    .nopfn = _cma_cpu_page_fault_handler
#endif
};

static inline void phy_to_MiuOffset(unsigned long phy_addr, unsigned int *miu, unsigned long *offset)
{
    *miu = INVALID_MIU;

	if(phy_addr >= MIU3_BUS_BASE)
	{
		*miu = 3; 
		*offset = phy_addr - MIU3_BUS_BASE;
	}
    else if(phy_addr >= MIU2_BUS_BASE)
    {
        *miu = 2; 
        *offset = phy_addr - MIU2_BUS_BASE;
    }
    else if(phy_addr >= MIU1_BUS_BASE)
    {
        *miu = 1;
        *offset = phy_addr - MIU1_BUS_BASE;
    }
    else if(phy_addr >= MIU0_BUS_BASE)
    {
        *miu = 0;
        *offset = phy_addr - MIU0_BUS_BASE;
    }
	else
		printk("\033[35mFunction = %s, Line = %d, Error, Unknown MIU, for phy_addr is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, phy_addr);
}

int _cma_cpu_page_fault_handler(struct vm_area_struct *vma, struct vm_fault *vmf)
{
    void __user * address;
    address = vmf->virtual_address;
    MCMA_CUST_ERR("vmf->virtual_address = %lX \n",(unsigned long)address);
    return VM_FAULT_SIGBUS;
}

void _cma_vma_close(struct vm_area_struct * vma)
{      
}

static int CMA_Pool_Open(struct inode *inode, struct file *filp)
{
    filp_private * pdev =  kzalloc(sizeof(filp_private), GFP_KERNEL);
    if (pdev == NULL)
          return -ENOMEM;
    pdev->pid = task_pid_nr(current->group_leader);
    INIT_LIST_HEAD(&pdev->list_heap_proc_info);
    mutex_init(&pdev->lock);
    filp->private_data = pdev;
    return 0;
}

static inline struct cma_memory_info * find_heap_info(unsigned int heap_id)
{
    struct cma_memory_info *heap_info = NULL;
    bool find = false;

    mutex_lock(&heap_info_lock);
    list_for_each_entry(heap_info, &heap_info_list, list_node)
    {
        if(heap_id == heap_info->heap_id)
        {
            find = true;
            break;
        }
    }
    mutex_unlock(&heap_info_lock);

    if(find)
        return heap_info;
    else
        return NULL;
}

static inline struct heap_proc_info * find_heap_proc_info(unsigned int heap_id, filp_private * pdev)
{
    struct heap_proc_info * proc_info = NULL;
    bool find = false;

    mutex_lock(&pdev->lock);
    list_for_each_entry(proc_info, &pdev->list_heap_proc_info, list_node)
    {
        if(proc_info->heap_id == heap_id)
        {        
            find = true;
            break;
        }
    }
    mutex_unlock(&pdev->lock);

    if(find)
        return proc_info;
    else
        return NULL;
}

/* release a specified heap with heap_id, all allocated_buffers will be freed */
static int CMA_Pool_Release_Heap(unsigned int heap_id, filp_private * pdev)
{
   	pid_t pid = pdev->pid;
   	struct cma_memory_info *heap_info = NULL;
   	struct cma_buffer *buffer_node, *next;
   	struct cma_buffer *release_buf_front = NULL, * release_buf_back = NULL;
   	struct cma_allocation_list *alloc_list = NULL;
   	struct heap_proc_info *proc_info = NULL;


   	heap_info = find_heap_info(heap_id);
   	if(!heap_info)
       	return -EINVAL;

	printk("\033[31mFunction = %s, Line = %d, pid is %d, current->pid is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, pid, current->pid);
	printk("\033[31mRelease for heap_id %u\033[m\n", heap_id);

   	mutex_lock(&heap_info->lock);
   	alloc_list = &heap_info->alloc_list;
   	list_for_each_entry_safe(buffer_node, next, &alloc_list->list_head, list)
   	{
      	if(buffer_node->pid == pid)
      	{
          	if(buffer_node->freed == 0)
        	{
				printk("\033[35mFunction = %s, Line = %d, ready to freed buffer_start: 0x%lX, length: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, buffer_node->start_pa, buffer_node->length);
             	buffer_node->freed = 1;
             	buffer_node->pid = INVALID_PID;
             	heap_info->alloc_list.using_count--;
             	heap_info->alloc_list.freed_count++;
          	}
      	}
   	}

   	free_buffer_list(heap_info, &release_buf_front, &release_buf_back);
   	if(release_buf_front)
   	{
		printk("\033[35mFunction = %s, Line = %d, release_buf_front\033[m\n", __PRETTY_FUNCTION__, __LINE__);
      	addKRange(heap_info, release_buf_front);
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
        if(heap_info->flags & CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE){
            modify_kpage_mapping_conti(release_buf_front->page, release_buf_front->length >> PAGE_SHIFT,MT_MEMORY);
        }
#endif
     	dma_release_from_contiguous(heap_info->dev, release_buf_front->page, release_buf_front->length >> PAGE_SHIFT);
      	kfree(release_buf_front);
   	}
   
   	if(release_buf_back)
   	{
		printk("\033[35mFunction = %s, Line = %d, release_buf_back\033[m\n", __PRETTY_FUNCTION__, __LINE__);
      	addKRange(heap_info, release_buf_back);
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
        if(heap_info->flags & CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE){
            modify_kpage_mapping_conti(release_buf_back->page, release_buf_back->length >> PAGE_SHIFT,MT_MEMORY);
        }
#endif
      	dma_release_from_contiguous(heap_info->dev, release_buf_back->page, release_buf_back->length >> PAGE_SHIFT);
      	kfree(release_buf_back);
   	}

   	dump_CMA_Pool(heap_info, true);
   	dumpKRange(heap_info);
   	mutex_unlock(&heap_info->lock);

   	proc_info = find_heap_proc_info(heap_id, pdev);
    mutex_lock(&pdev->lock);
   	if(proc_info != NULL)
   	{
       	printk(KERN_ERR "%s: %d delete proc_info\n", __FUNCTION__, __LINE__);
       	list_del(&proc_info->list_node);
       	kfree(proc_info);
   	}
    mutex_unlock(&pdev->lock);
   
   	return 0;
}

static int CMA_Pool_Release(struct inode *inode, struct file *filp)
{
	filp_private *pdev = (filp_private *)filp->private_data;
	pid_t pid = pdev->pid;
   	struct cma_memory_info *heap_info = NULL, *next_heap = NULL;
   	struct cma_buffer *buffer_node, *next;
   	struct cma_buffer *release_buf_front = NULL, *release_buf_back = NULL;
   	struct cma_allocation_list *alloc_list = NULL;
   	struct heap_proc_info *proc_info = NULL, *proc_n = NULL;

	printk("\033[31mFunction = %s, Line = %d, pid is %d, current->pid is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, pid, current->pid);
	printk("\033[31mFunction = %s, Line = %d, [%s] do CMA_Pool_Release for /dev/cmapool close\033[m\n", __PRETTY_FUNCTION__, __LINE__, current->comm);

   	mutex_lock(&heap_info_lock);
	list_for_each_entry_safe(heap_info, next_heap, &heap_info_list, list_node)	// search for all heap_info(each heap_id has a heap_info)
	{
		printk("\033[35mFunction = %s, Line = %d, check heap_id %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, heap_info->heap_id);
      	dump_CMA_Pool(heap_info, true);
		
		alloc_list = &heap_info->alloc_list;	
		list_for_each_entry_safe(buffer_node, next, &alloc_list->list_head, list)	// search this heap's alloc_list, find each buffer for this pid, mark as freed(only free this pid's allocation)
		{
 			if(buffer_node->pid == pid)
			{
				if(buffer_node->freed == 0)
				{
					printk("\033[35mFunction = %s, Line = %d, ready to freed buffer_start: 0x%lX, length: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, buffer_node->start_pa, buffer_node->length);
					buffer_node->freed = 1;
					buffer_node->pid = INVALID_PID;
					heap_info->alloc_list.using_count--;
					heap_info->alloc_list.freed_count++;
				}
			}
		}

 		free_buffer_list(heap_info, &release_buf_front, &release_buf_back);			// for this heap, check if having free buffer
      	if(release_buf_front)
      	{
			printk("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__);
         	addKRange(heap_info, release_buf_front);
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
        if(heap_info->flags & CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE){
            modify_kpage_mapping_conti(release_buf_front->page, release_buf_front->length >>PAGE_SHIFT,MT_MEMORY);
        }
#endif
         	dma_release_from_contiguous(heap_info->dev, release_buf_front->page, release_buf_front->length >> PAGE_SHIFT);
         	kfree(release_buf_front);
      	}

      	if(release_buf_back)
      	{
			printk("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__);
         	addKRange(heap_info, release_buf_back);
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
        if(heap_info->flags & CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE){
            modify_kpage_mapping_conti(release_buf_back->page, release_buf_back->length>>PAGE_SHIFT,MT_MEMORY);
        }
#endif
         	dma_release_from_contiguous(heap_info->dev, release_buf_back->page, release_buf_back->length >> PAGE_SHIFT);
         	kfree(release_buf_back);
      	}

      	dump_CMA_Pool(heap_info, true);
      	dumpKRange(heap_info);
   	}
   	mutex_unlock(&heap_info_lock);

  	mutex_lock(&pdev->lock);
   	list_for_each_entry_safe(proc_info, proc_n, &pdev->list_heap_proc_info, list_node)
   	{
       	printk(KERN_ERR "%s: %d delete proc_info\n", __FUNCTION__, __LINE__);
       	list_del(&proc_info->list_node);       
       	kfree(proc_info);
   	}
   	mutex_unlock(&pdev->lock);

   	kfree(pdev);
   
   	return 0;
}


static int CMA_Pool_Init(unsigned int heap_id, unsigned long flags, unsigned int *miu, unsigned long *bus_addr, 
    unsigned long *heap_start_offset, unsigned long *heap_length, filp_private *pdev)
{
    int ret = 0;
    unsigned long pfn = 0, offset = 0;
    unsigned int miu_index = INVALID_MIU;
    struct file *fp = NULL;
    struct device * dev = NULL;
    struct ion_client *client;
    struct cma_memory_info *heap_info;				// for a heap, created once here, and never be freed
    struct heap_proc_info* proc_info = NULL;		// for a process, handling the vma setting, freed when fd_close
    bool isNew_heap_proc_info = false;
    
#ifdef CMA_POOL_KERNEL_PROTECT_DUMP   
    printk(KERN_ERR "\n%s:%d heapid %u\n", __FUNCTION__,__LINE__, heap_id);
#endif

#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
    if((flags&CMA_FLAG_UNCACHED_FOR_TEE)
        &&(flags&(CMA_FLAG_MAP_VMA|CMA_FLAG_CACHED|CMA_FLAG_VIRT_ADDR))){
        MCMA_CUST_ERR("TEE memory can't be mapped to cached\n");
        return -EINVAL;
    }
#endif

    proc_info = find_heap_proc_info(heap_id, pdev);
    if(!proc_info)
    {
		printk("\033[35mFunction = %s, Line = %d, create proc_info\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        proc_info = (struct heap_proc_info *)kzalloc(sizeof(struct heap_proc_info), GFP_KERNEL);
        if(!proc_info)
        {
            MCMA_CUST_ERR("no memory\n");
            return -ENOMEM;           
        }
		
        proc_info->vma = NULL;		
        INIT_LIST_HEAD(&proc_info->list_node);
        isNew_heap_proc_info = true;
    }

    heap_info = find_heap_info(heap_id);	// heap_info will be created here for only once, never be cleared
    if(heap_info)
    {
		printk("\033[35mFunction = %s, Line = %d, already having heap_info, do not init again\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        *miu = heap_info->miu;        
        *heap_length = heap_info->dev->cma_area->count * PAGE_SIZE;        
        pdev->temp_heap_id = heap_id;
        proc_info->heap_id = heap_id;
        proc_info->base_phy = PFN_PHYS(heap_info->dev->cma_area->base_pfn);       
        if(flags & CMA_FLAG_CACHED)
            proc_info->vma_cached = true;

		phy_to_MiuOffset(proc_info->base_phy, &miu_index, &offset);
        *heap_start_offset = offset;
        *bus_addr = proc_info->base_phy;

        if(isNew_heap_proc_info)
        {
            mutex_lock(&pdev->lock);
            list_add(&proc_info->list_node, &pdev->list_heap_proc_info);
            mutex_unlock(&pdev->lock);        
        }
        
        dumpKRange(heap_info);
        return 0;
    }

	printk("\033[35mFunction = %s, Line = %d, create heap_info\033[m\n", __PRETTY_FUNCTION__, __LINE__);
    fp = filp_open(ION_DEVICE_NAME, O_RDONLY | O_NONBLOCK, 0);
    if(IS_ERR(fp))
    {
        MCMA_CUST_ERR("open ion device %s fail\n", ION_DEVICE_NAME);
        return -EINVAL;
    }
    
    client = (struct ion_client *)fp->private_data;
    dev = ion_get_heap_dev(client, (1 << heap_id));
    if(dev == NULL)
    {
        MCMA_CUST_ERR("not find the cma device for heap id %d\n", heap_id);
        ret = -ENXIO;
        goto CLOSE_ION_FP;    
    }
    heap_info = (struct cma_memory_info *)kzalloc(sizeof(struct cma_memory_info), GFP_KERNEL);
    if(!heap_info)
    {
        MCMA_CUST_ERR("out of memory!\n");
        ret = -ENOMEM;
        goto CLOSE_ION_FP;  
    }	

    pfn = dev->cma_area->base_pfn;	
    MCMA_BUG_ON(!pfn_valid(pfn));	

    if(flags &CMA_FLAG_CACHED)
        proc_info->vma_cached = true;    
    proc_info->heap_id = heap_id;	
    proc_info->base_phy = PFN_PHYS(pfn);	
    
    pdev->temp_heap_id = heap_id;    
    phy_to_MiuOffset(proc_info->base_phy, &miu_index, &offset);
    heap_info->miu = miu_index;
    MCMA_BUG_ON(heap_info->miu >= 3);
    heap_info->heap_id = heap_id;
    heap_info->dev = dev;
    heap_info->alloc_list.min_start = PHYSICAL_START_INIT;
    heap_info->alloc_list.max_end = PHYSICAL_END_INIT;
    heap_info->alloc_list.using_count = 0;
    heap_info->alloc_list.freed_count = 0;	
    INIT_LIST_HEAD(&heap_info->alloc_list.list_head);	

    heap_info->pranges = &glob_miu_kranges[(int)(heap_info->miu)];
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
    if(flags&CMA_FLAG_UNCACHED_FOR_TEE){
        heap_info->flags |= CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE;
    }
#endif
    INIT_LIST_HEAD(&heap_info->list_node);	
    mutex_init(&heap_info->lock);
    
    mutex_lock(&heap_info_lock);
    list_add(&heap_info->list_node, &heap_info_list);
    mutex_unlock(&heap_info_lock);

    *miu = heap_info->miu;
    *heap_length = dev->cma_area->count * PAGE_SIZE;
    *heap_start_offset = offset;
    *bus_addr = proc_info->base_phy;    

    if(isNew_heap_proc_info)
    {        
        mutex_lock(&pdev->lock);		
        list_add(&proc_info->list_node, &pdev->list_heap_proc_info);
        mutex_unlock(&pdev->lock);
    }	
    dumpKRange(heap_info);

CLOSE_ION_FP:
    filp_close(fp, NULL);
    return ret;
}

//before call this API, lock heap info mutex firstly
static struct cma_buffer * SplitBuffer(unsigned long start_pa, unsigned long length, struct cma_buffer *find, 
        struct cma_memory_info *heap_info, BUFF_OPS ops)
{
    struct cma_buffer *buffer_node = NULL;
    struct cma_allocation_list * alloc_list = &heap_info->alloc_list;
    MCMA_BUG_ON(!find || start_pa < find->start_pa || ((start_pa + length) > (find->start_pa + find->length)));
    if(ops == CMA_ALLOC)
        MCMA_BUG_ON(find->freed == 0);
    else
        MCMA_BUG_ON(find->freed == 1);

    //at begginning of found buffer
    if(start_pa == find->start_pa)
    {
        if(length == find->length)
        {
           /* bufffer_node(start_pa, length)  =   find
            *  |------------------------------------|
            */
            buffer_node = find;
            if(ops == CMA_ALLOC)
            {
                buffer_node->freed = 0;
                alloc_list->freed_count--;
                alloc_list->using_count++;
            }
            else
            {
                alloc_list->freed_count++;
                alloc_list->using_count--;
                buffer_node->freed = 1;
                buffer_node->cpu_addr = 0;
                buffer_node->pid = INVALID_PID;
            }
        }
        else
        {
           /* bufffer_node(start_pa, length)  find
            *  |----------------------------|--------|
            */
            buffer_node = (struct cma_buffer *)kzalloc(sizeof(struct cma_buffer), GFP_KERNEL);
            MCMA_BUG_ON(!buffer_node);
            buffer_node->start_pa = find->start_pa;	// buffer_noda specify the freed buffer
            buffer_node->length = length;
            buffer_node->page = find->page;
			buffer_node->cpu_addr = find->cpu_addr;
            INIT_LIST_HEAD(&buffer_node->list);

            find->start_pa = buffer_node->start_pa + buffer_node->length;	// adjust the freed buffer
            find->length -= buffer_node->length;
            find->page = buffer_node->page + (buffer_node->length >> PAGE_SHIFT);   
			find->cpu_addr = buffer_node->cpu_addr + buffer_node->length;
            list_add(&buffer_node->list, find->list.prev); //insert new node before find node (list_add(source, to))

            if(ops == CMA_ALLOC)
            {
                buffer_node->freed = 0;
                alloc_list->using_count++;
            }
            else
            {
                alloc_list->freed_count++;
                buffer_node->freed = 1;
                buffer_node->cpu_addr = 0;
                buffer_node->pid = INVALID_PID;
            }
        }
    }
    else if((start_pa+length) == (find->start_pa+find->length)) //at end of found buffer
    {
        /* find         bufffer_node(start_pa, length)    
         *  |-------|----------------------------|
         */
        find->length -= length;
        buffer_node = (struct cma_buffer *)kzalloc(sizeof(struct cma_buffer), GFP_KERNEL);
        MCMA_BUG_ON(!buffer_node);       
        buffer_node->start_pa = find->start_pa + find->length;
        buffer_node->length = length;                
        buffer_node->page = find->page + (find->length>>PAGE_SHIFT);
		buffer_node->cpu_addr = find->cpu_addr + find->length;
        INIT_LIST_HEAD(&buffer_node->list);        
        list_add(&buffer_node->list, &find->list); //insert new node after find node        

        if(ops == CMA_ALLOC)
        {
            buffer_node->freed = 0;        
            alloc_list->using_count++;        
        }
        else
        {   
            alloc_list->freed_count++;
            buffer_node->freed = 1;
            buffer_node->cpu_addr = 0;
            buffer_node->pid = INVALID_PID;
        }
    }
    else //in the middle(into 2 find buffers and one ready_to_be_allocated buffer)
    {
        /*    find   bufffer_node(start_pa, length) buffer_node1
         *  |-------|----------------------------|-------------|
         */
        struct cma_buffer *buffer_node1 = NULL;
        unsigned long total_len = find->length;
    
        find->length = start_pa - find->start_pa;	// leftest buffer
        
        buffer_node = (struct cma_buffer *)kzalloc(sizeof(struct cma_buffer), GFP_KERNEL);	// ready_to_be_allocated buffer
        MCMA_BUG_ON(!buffer_node);      
        buffer_node->start_pa = find->start_pa + find->length;
        buffer_node->length = length;
        buffer_node->page = find->page + (find->length >> PAGE_SHIFT);
		buffer_node->cpu_addr = find->cpu_addr + find->length;
        INIT_LIST_HEAD(&buffer_node->list);
        list_add(&buffer_node->list, &find->list); //insert new node after find node        

        buffer_node1 = (struct cma_buffer *)kzalloc(sizeof(struct cma_buffer), GFP_KERNEL);	// rightest buffer
        MCMA_BUG_ON(!buffer_node1);
        buffer_node1->start_pa = buffer_node->start_pa + buffer_node->length;
        buffer_node1->length = total_len - buffer_node->length - find->length;
        buffer_node1->page = buffer_node->page + (buffer_node->length >> PAGE_SHIFT);         
        buffer_node1->cpu_addr = buffer_node->cpu_addr + buffer_node->length;
        INIT_LIST_HEAD(&buffer_node1->list);
        list_add(&buffer_node1->list, &buffer_node->list); //insert new node after find node

        if(ops == CMA_ALLOC)
        {
            buffer_node->freed = 0;
            alloc_list->using_count++;
            buffer_node1->freed = 1;
            alloc_list->freed_count++;
        }
        else
        {
            alloc_list->freed_count++;
            buffer_node1->freed = 0;
            alloc_list->using_count++;

            buffer_node->freed = 1;
            buffer_node->cpu_addr = 0;
            buffer_node->pid = INVALID_PID;
        }
    } 
        
    return buffer_node;
}

//before call this API, lock heap info mutex firstly
static struct cma_buffer * _alloc_from_freelist(struct cma_memory_info * heap_info, unsigned long start_pa, 
size_t length, unsigned int flags)
{
    struct cma_buffer *buffer_node, *find = NULL;
    struct cma_allocation_list * alloc_list = &heap_info->alloc_list;


    if(alloc_list->freed_count <= 0)
        return NULL;
    
    //find the start address in free list
    list_for_each_entry(buffer_node, &alloc_list->list_head, list)
    {
        //not freed buffer
        if(buffer_node->freed == 0)
            continue;

		// check if this allocation_range is located at this freed buffer 
        if((buffer_node->start_pa<=start_pa) 
            && ((start_pa - buffer_node->start_pa + length) <= buffer_node->length))                
        {
            find = buffer_node;
            break;
        }
    }

    if(!find)
    {
        buffer_node = NULL;
    }
    else
    {
        buffer_node = SplitBuffer(start_pa, length, find, heap_info, CMA_ALLOC);	// split the free buffer to "ready_to_be_allocated buffer" and "free_buffer"
    }

    return buffer_node;
}

int idleBlockIndx(struct MIU_ProtectRanges * pranges)
{ 
    int index = 0;
    
    for(index = 0; index < MIU_BLOCK_NUM; ++index)
    {
        if(pranges->miuBlockStatus[index] == MIU_BLOCK_IDLE)
        {
            return index;
        }
    }

    return -1;
}

static void dumpKRange(struct cma_memory_info * heap_info)
{
#ifdef CMA_POOL_KERNEL_PROTECT_DUMP
    struct MIU_ProtectRanges *pranges = heap_info->pranges;
    MIU_ProtectRange *range = NULL;
    
    mutex_lock(&pranges->lock);
    list_for_each_entry(range, &pranges->list_head, list_node)
    {
        printk(KERN_ERR "range start 0x%lX to 0x%lX, length 0x%lX miublock_index %d\n",
            range->start_pa, (range->start_pa+range->length), range->length, range->miuBlockIndex);
    }
    mutex_unlock(&pranges->lock);
#endif
}

static bool _miu_kernel_protect(unsigned char miuBlockIndex, unsigned char *pu8ProtectId,
    unsigned long start, unsigned long end, int flag)
{
    bool ret = true;

    if(atomic_read(&kprotect_enabled) > 0)
        ret = MDrv_MIU_Protect(miuBlockIndex, kernal_protect_client_id, start, end, flag);
    else
        printk(KERN_ERR "ignore kernel protect\n");

    return ret;
}

//when alloc from cma heap, call this API to deleteKRange of this allocted buffer
static int deleteKRange(struct cma_memory_info * heap_info, struct cma_buffer * buffer)
{
    struct MIU_ProtectRanges * pranges = heap_info->pranges;
    MIU_ProtectRange * range,  * r_front = NULL, * r_back= NULL;
    MIU_ProtectRange old;
    unsigned long r_front_len = 0, r_back_len = 0;
    int miuBlockIndex = -1;
    bool find = false, protect_ret = false;
    int ret = 0;

    if(buffer->length == 0)
        return 0;
    
    /* 
         * kernel protect range( before allocate buffer)
         *
         * |--------------------------------|
         *
         * kernel protect range(buffer location in this range, after buffer allocated)
         *  r_front        allocated buffer    r_back
         *
         * |------|=============|-------|
         * 
         * case: r_front = 0; r_back = 0; r_front=r_back=0;
         */
    mutex_lock(&pranges->lock);
    list_for_each_entry(range, &pranges->list_head, list_node)
    {
        if((buffer->start_pa >= range->start_pa) 
            && ((buffer->start_pa+buffer->length) <= (range->start_pa+range->length)))
        {
            find = true;
            old.start_pa = range->start_pa;
            old.length = range->length;
            old.miuBlockIndex = range->miuBlockIndex;                
            break;
        }
    }

    if(!find)
    {
       ret = -EINVAL;
       MCMA_CUST_ERR("not find the buffer: start_pa %lx length %lu\n", buffer->start_pa, buffer->length);
       goto DELETE_KRANGE_DONE;
    }

    r_front_len = buffer->start_pa - range->start_pa;
    r_back_len = range->start_pa + range->length - (buffer->start_pa + buffer->length);

    if((r_front_len != 0) && (r_back_len != 0))
    {
        miuBlockIndex = idleBlockIndx(pranges);
        if(miuBlockIndex < 0)
        {
           ret = -ENXIO;
           MCMA_CUST_ERR("no idle miu protect block in miu %d\n", (int)heap_info->miu);
           goto DELETE_KRANGE_DONE;
        }
        
        r_back = (MIU_ProtectRange *)kzalloc(sizeof(MIU_ProtectRange), GFP_KERNEL);
        if(!r_back)
        {
           ret = -ENOMEM;
           printk(KERN_ERR "no memory\n");
           goto DELETE_KRANGE_DONE;        
        }

        r_front = range;
        r_front->length = r_front_len;
        
        r_back->start_pa = buffer->start_pa + buffer->length;
        r_back->length = r_back_len;
        r_back->miuBlockIndex = miuBlockIndex;
        INIT_LIST_HEAD(&r_back->list_node);
        list_add(&r_back->list_node, &r_front->list_node);
        pranges->krange_num++;        
    }
    else if(r_front_len != 0) //and (r_back_len == 0)
    {
        r_front = range;
        r_front->length = r_front_len;
    }
    else if(r_back_len != 0) //and (r_front_len == 0)
    {
        r_back = range;
        r_back->start_pa = buffer->start_pa + buffer->length;
        r_back->length = r_back_len;
    }
    else //((r_front_len == 0) && (r_back_len == 0))
    {
        list_del(&range->list_node);
        kfree(range);
        pranges->krange_num--;
    }
        
    protect_ret = _miu_kernel_protect(old.miuBlockIndex, kernal_protect_client_id, old.start_pa, 
        old.start_pa + old.length, MIU_PROTECT_DISABLE);
    MCMA_BUG_ON(!protect_ret);
    pranges->miuBlockStatus[old.miuBlockIndex] = MIU_BLOCK_IDLE;
    
    if(r_front)
    {
        protect_ret = _miu_kernel_protect(r_front->miuBlockIndex, kernal_protect_client_id, 
            r_front->start_pa, r_front->start_pa+r_front->length, MIU_PROTECT_ENABLE);
        MCMA_BUG_ON(!protect_ret);
        pranges->miuBlockStatus[r_front->miuBlockIndex] = MIU_BLOCK_BUSY;
    }

    if(r_back)
    {
        protect_ret = _miu_kernel_protect(r_back->miuBlockIndex, kernal_protect_client_id, 
            r_back->start_pa, r_back->start_pa+r_back->length, MIU_PROTECT_ENABLE);
        MCMA_BUG_ON(!protect_ret);
        pranges->miuBlockStatus[r_back->miuBlockIndex] = MIU_BLOCK_BUSY;    
    }  

DELETE_KRANGE_DONE:    
    mutex_unlock(&pranges->lock);
    return ret;
}

// when free to cma heap, call this API to add KRange of this allocted buffer
static int addKRange(struct cma_memory_info *heap_info, struct cma_buffer *buffer)
{
    struct MIU_ProtectRanges *pranges = heap_info->pranges;
    MIU_ProtectRange *r_prev = NULL, *r_next= NULL;
    MIU_ProtectRange *range;   
    int miuBlockIndex = -1;
    bool protect_ret = false;
    int ret = 0;

    if(buffer->length == 0)
        return 0;

    /* 
         * kernel protect range (before freed buffer)
         *      r_prev       allocated buffer     r_next
         * |-------------|====================|------------|
         *
         * kernel protect range(freed buffer location in this range)
         *   r_prev   freed buffer    r_next
         * |--------|?-------------?|-------|
         * 
    */
    mutex_lock(&pranges->lock);
    list_for_each_entry(range, &pranges->list_head, list_node)	// find this miu all kernel_protect setting(range)
    {
        if((range->start_pa + range->length) <= buffer->start_pa)
        {
			//printk("\033[35mFunction = %s, Line = %d, find r_prev form 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, range->start_pa, (range->start_pa + range->length));
            r_prev = range;
            continue;	// should be continue, we are going to find a nearest one k_range before this buffer
        }
    }

    if(r_prev)	// find a kernel_protect range before this buffer
    {
        if(!list_is_last(&r_prev->list_node,&pranges->list_head))
        {
            r_next = container_of(r_prev->list_node.next, MIU_ProtectRange, list_node);		// if prev_krange is not the last one, the next one krange will be r_next
			//printk("\033[35mFunction = %s, Line = %d, find r_next form 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, r_next->start_pa, (r_next->start_pa + r_next->length));
        }
    }
    else		// no kernel_protect range before this buffer ==> all k_range is behind this buffer
    {
        if(list_empty(&pranges->list_head))
            r_next = NULL;
        else
            r_next = list_first_entry(&pranges->list_head, MIU_ProtectRange, list_node);	// r_next will be first krange 
    }

    //till now, find the prev range and next range of buffer freed
    if(r_prev && r_next)
    {        
        if(((r_prev->start_pa + r_prev->length) == buffer->start_pa)
            && ((buffer->start_pa + buffer->length) == r_next->start_pa))	// the buffer is just the hole between r_prev and r_next
        {
			// disable r_prev
            protect_ret = _miu_kernel_protect(r_prev->miuBlockIndex, kernal_protect_client_id, 
                r_prev->start_pa, r_prev->start_pa + r_prev->length, MIU_PROTECT_DISABLE);
            MCMA_BUG_ON(!protect_ret);

			// disable r_next
            protect_ret = _miu_kernel_protect(r_next->miuBlockIndex, kernal_protect_client_id, 
                r_next->start_pa, r_next->start_pa + r_next->length, MIU_PROTECT_DISABLE);
            MCMA_BUG_ON(!protect_ret);
            pranges->miuBlockStatus[r_next->miuBlockIndex] = MIU_BLOCK_IDLE;	// mark a k_range is available

            r_prev->length += (r_next->length + buffer->length);				// extend the r_prev length, and protect it
            protect_ret = _miu_kernel_protect(r_prev->miuBlockIndex, kernal_protect_client_id, 
                r_prev->start_pa, r_prev->start_pa + r_prev->length, MIU_PROTECT_ENABLE);
            MCMA_BUG_ON(!protect_ret);

            list_del(&r_next->list_node);
            kfree(r_next);
            pranges->krange_num--;
            
            goto ADD_KRANGE_DONE;
        }
    }
    
    if(r_prev)
    {
        if((r_prev->start_pa + r_prev->length) == buffer->start_pa)
        {
            protect_ret = _miu_kernel_protect(r_prev->miuBlockIndex, kernal_protect_client_id, 
                r_prev->start_pa, r_prev->start_pa + r_prev->length, MIU_PROTECT_DISABLE);
            MCMA_BUG_ON(!protect_ret);

            r_prev->length += buffer->length;
            protect_ret = _miu_kernel_protect(r_prev->miuBlockIndex, kernal_protect_client_id, 
                r_prev->start_pa, r_prev->start_pa + r_prev->length, MIU_PROTECT_ENABLE);
            MCMA_BUG_ON(!protect_ret);  

            goto ADD_KRANGE_DONE;
        }
    }

    if(r_next)
    {
        if((buffer->start_pa + buffer->length) == r_next->start_pa)
        {
			protect_ret = _miu_kernel_protect(r_next->miuBlockIndex, kernal_protect_client_id, 
                r_next->start_pa, r_next->start_pa + r_next->length, MIU_PROTECT_DISABLE);
            MCMA_BUG_ON(!protect_ret);

            r_next->start_pa = buffer->start_pa;
            r_next->length += buffer->length;
            protect_ret = _miu_kernel_protect(r_next->miuBlockIndex, kernal_protect_client_id, 
                r_next->start_pa, r_next->start_pa + r_next->length, MIU_PROTECT_ENABLE);
            MCMA_BUG_ON(!protect_ret);

            goto ADD_KRANGE_DONE;
        }
    }

	// use a new k_range for this buffer
    miuBlockIndex = idleBlockIndx(pranges);
    if(miuBlockIndex < 0)
    {
       ret = -ENXIO;
       MCMA_CUST_ERR("no idle miu protect block in miu %d\n", (int)heap_info->miu);
       goto ADD_KRANGE_DONE;
    }
	printk("\033[35mFunction = %s, Line = %d, use a new k_range for this buffer, miu_protect %d for 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, miuBlockIndex, buffer->start_pa, (buffer->start_pa+buffer->length));

    range = (MIU_ProtectRange *)kzalloc(sizeof(MIU_ProtectRange), GFP_KERNEL);
    if(!range)
    {
       ret = -ENOMEM;
       printk(KERN_ERR "no memory\n");
       goto ADD_KRANGE_DONE;        
    }
    range->start_pa = buffer->start_pa;
    range->length = buffer->length;
    range->miuBlockIndex = miuBlockIndex;
    INIT_LIST_HEAD(&range->list_node);
    if(r_prev)
        list_add(&range->list_node, &r_prev->list_node);
    else
        list_add(&range->list_node, &pranges->list_head);
    
    protect_ret = _miu_kernel_protect(range->miuBlockIndex, kernal_protect_client_id, 
        range->start_pa, range->start_pa + range->length, MIU_PROTECT_ENABLE);
    MCMA_BUG_ON(!protect_ret);
    pranges->miuBlockStatus[range->miuBlockIndex] = MIU_BLOCK_BUSY;
    pranges->krange_num++;

ADD_KRANGE_DONE:    
    mutex_unlock(&pranges->lock);
    return ret;
}

extern void drop_slab(void);
extern void drop_pagecache_sb(struct super_block *sb, void *unused);
#define SIZE_8K (8192)
static int CMA_Pool_Alloc(unsigned int heap_id, unsigned long offset_in_heap, unsigned long *cpu_addr,
    unsigned long length, unsigned long align, unsigned int flags, filp_private *pdev)
{
    int ret = 0, try_num = 0;
    bool alloc_at_front = false;
    struct page * page = NULL;
    unsigned long phy_addr = 0, start_pa = 0;
    struct cma_memory_info * heap_info = NULL;
    struct cma_buffer *buffer = NULL, *buffer_hole = NULL;
    unsigned long length_buffer_hole = 0;
    bool find_in_freelist = false;
    struct cma * cma_area = NULL;
    struct heap_proc_info * proc_info = NULL;

    if(!IS_ALIGNED(offset_in_heap, SIZE_8K) || !IS_ALIGNED(length, SIZE_8K))
    {
       printk(KERN_ERR "input parameter not align to 8K, offset_in_heap %lx, length %lx\n",
        offset_in_heap, length);
       return -EINVAL;
    }

    proc_info = find_heap_proc_info(heap_id, pdev);
    if(!proc_info)
    {
        MCMA_CUST_ERR("has not find heap_id %u its heap proc info\n", heap_id);
        return -EINVAL;
    }

    heap_info = find_heap_info(heap_id);
    if(!heap_info)
    {
        MCMA_CUST_ERR("not find the heap: heap id %u\n", heap_id);
        return -EINVAL;
    }

    if(!proc_info->vma && (flags & CMA_FLAG_VIRT_ADDR))
    {
        MCMA_CUST_ERR("invalid parameter: no vma but need virtual address\n");
        return -EINVAL;
    }
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
    if( (heap_info->flags &CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE)
        && (flags&CMA_FLAG_VIRT_ADDR) ){
        MCMA_CUST_ERR("TEE memory can't have virtal address\n");
        return -EINVAL;
    }
#endif

#ifdef CMA_POOL_DUMP
    printk(KERN_ERR "\n%s:%d heapid %u\n", __FUNCTION__,__LINE__, heap_id);
#endif

    cma_area = heap_info->dev->cma_area;
    start_pa = offset_in_heap + proc_info->base_phy;	// the start_allocation_addr

    mutex_lock(&heap_info->lock);

    //find firstly in free list(buffer means a cma_alloc_result => from allocation_start to allocation_end, and mark if freed or not)
    if(heap_info->alloc_list.freed_count > 0)
    {
		buffer = _alloc_from_freelist(heap_info, start_pa, length, flags);
    }

    //if not find in free list, get from cma heap directly
    if(!buffer)
    {
        phy_addr = start_pa;
        //check if input parameters out of cma heap range
        if(((phy_addr + length) > PFN_PHYS(cma_area->base_pfn + cma_area->count))
            || (phy_addr < PFN_PHYS(cma_area->base_pfn)))
        {
			ret = -EINVAL;
            MCMA_CUST_ERR("invalid start address or length:  heap id %u, offset_in_heap 0x%lX ,max end 0x%lX, base_phy 0x%lX\n",
                           heap_id, offset_in_heap, heap_info->alloc_list.max_end, proc_info->base_phy);
            goto ALLOCAT_BUFF_FAIL;
        }

        //if not find in freelist previously, here it [start, length] should not locate in the middle of allocation_list 
        if (heap_info->alloc_list.min_start != PHYSICAL_START_INIT)
        {
            if (((phy_addr >= heap_info->alloc_list.min_start) && (phy_addr < heap_info->alloc_list.max_end))
                || (((phy_addr+length) > heap_info->alloc_list.min_start) && ((phy_addr+length) <= heap_info->alloc_list.max_end)))
            {
                ret = -EINVAL;         
                MCMA_CUST_ERR("invalid start address or length:  heap id %u, min_start 0x%lX , max_end 0x%lX, phy_addr 0x%lX, length 0x%lX  \"%s\" \n\n",
                           heap_id, heap_info->alloc_list.min_start, heap_info->alloc_list.max_end, 
                           phy_addr, length, pid_task(find_get_pid(pdev->pid), PIDTYPE_PID)->comm);
                dump_CMA_Pool(heap_info, true);
                goto ALLOCAT_BUFF_FAIL;
            }

            if(phy_addr < heap_info->alloc_list.min_start)
                alloc_at_front = true;

            /*
                      *   new buffer       hole   min_start                                   max_end
                      * |xxxxxxxxx|---------|======================|
            */
            if ((phy_addr+length) < heap_info->alloc_list.min_start)	// font hole
            {
                length_buffer_hole = heap_info->alloc_list.min_start - (phy_addr + length);
                length = heap_info->alloc_list.min_start - phy_addr;	// adjust allocation length to cover the hole
                
                buffer_hole = (struct cma_buffer *)kzalloc(sizeof(struct cma_buffer), GFP_KERNEL);
                MCMA_BUG_ON(!buffer_hole);
                INIT_LIST_HEAD(&buffer_hole->list);

                buffer_hole->freed = 1;									// set this addition cma_buffer(hole) to be freed
                buffer_hole->start_pa = phy_addr + length - length_buffer_hole;	// hole_start_pa
                buffer_hole->length = length_buffer_hole;						// hole_length

				// this hole(buffer_hole) will be allocated, but no one can use this(allcated in new buffer, the new_buffer will be prolonged to cover hole)
            }
            else if(phy_addr > heap_info->alloc_list.max_end)
            {
            /*
                      * min_start           max_end    hole        new buffer      
                      * |======================|---------------|xxxxxxxxxxxxxx|
            */
                length_buffer_hole = phy_addr - heap_info->alloc_list.max_end;	// hole_length
                length += length_buffer_hole;									// adjust length to cover this hole
                phy_addr = heap_info->alloc_list.max_end;						// adjust alloc_start_addr to hole_start_addr

                buffer_hole = (struct cma_buffer *)kzalloc(sizeof(struct cma_buffer), GFP_KERNEL);
                MCMA_BUG_ON(!buffer_hole);
                INIT_LIST_HEAD(&buffer_hole->list);

                buffer_hole->freed = 1;
                buffer_hole->start_pa = heap_info->alloc_list.max_end;
                buffer_hole->length = length_buffer_hole;                
            }
        }

ALLOC_RETRY:
		printk("\033[35mFunction = %s, Line = %d, mips_check page_to_pfn: ori phy_addr is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, phy_addr);
        page = dma_alloc_from_contiguous_addr(heap_info->dev, phy_addr, length>>PAGE_SHIFT, 1);
        if(!page)
        {
            if(try_num < MAX_ALLOC_TRY)
            {
                try_num++;
                iterate_supers(drop_pagecache_sb, NULL);
                drop_slab();
                msleep(1000);
                goto ALLOC_RETRY;
            }
           
            MCMA_CUST_ERR("Fail to allocate buffer\n");
			printk("\033[35mFunction = %s, Line = %d, Error, Please Check WHY dma_alloc_at_from_contiguous failed (maybe no memory left)!! \033[m\n", __PRETTY_FUNCTION__, __LINE__);
            ret = -ENOMEM;
            goto ALLOCAT_BUFF_FAIL;
        }
#if defined(CONFIG_ARM)
        phy_addr = pfn_to_dma(heap_info->dev, page_to_pfn(page));
#else
        phy_addr = (dma_addr_t)((page_to_pfn(page)) << PAGE_SHIFT);     // page to pfn to bus_addr
		printk("\033[35mFunction = %s, Line = %d, mips_check page_to_pfn: after phy_addr is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, phy_addr);
#endif
        buffer = (struct cma_buffer *)kzalloc(sizeof(struct cma_buffer), GFP_KERNEL);
        MCMA_BUG_ON(!buffer);
        buffer->start_pa = phy_addr;
        buffer->length = length;
        buffer->page = page;
        INIT_LIST_HEAD(&buffer->list);
        buffer->freed = 0;

        //insert the buffer node into alloc_list(be in order)
        if(alloc_at_front)
            list_add(&buffer->list, &heap_info->alloc_list.list_head);
        else
            list_add_tail(&buffer->list, &heap_info->alloc_list.list_head);
        heap_info->alloc_list.using_count++;
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
        if(heap_info->flags & CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE){
            modify_kpage_mapping_conti(page, length>>PAGE_SHIFT,MT_MEMORY_NONCACHED);
        }
#endif
    }// if not find in free list, get from cma heap directly
    else
    {
        find_in_freelist = true;
    }
    
    buffer->pid = task_pid_nr(current->group_leader);
    
    // find in freelist, so alloc_list max_end and min_start does't need to be changed
	// here to update max_end and min_start addr
    if(!find_in_freelist)
    {
        if(buffer->start_pa < heap_info->alloc_list.min_start)
            heap_info->alloc_list.min_start = buffer->start_pa;
        if((buffer->start_pa+buffer->length) > heap_info->alloc_list.max_end)
            heap_info->alloc_list.max_end = buffer->start_pa + buffer->length;
    }

    if(!find_in_freelist)
    {
        ret = deleteKRange(heap_info, buffer);	// this buffer will contain hole_buffer(if hole_buffer exists)
        if(ret)
        {
            MCMA_CUST_ERR("update kernal range fail\n");
            MCMA_CUST_ERR("update kernal range fail\n");
            MCMA_CUST_ERR("update kernal range fail\n");
            MCMA_CUST_ERR("update kernal range fail\n");
            MCMA_CUST_ERR("update kernal range fail\n");
            MCMA_CUST_ERR("update kernal range fail\n");
            MCMA_CUST_ERR("update kernal range fail\n");
            MCMA_CUST_ERR("update kernal range fail\n");
            goto ALLOCAT_BUFF_FAIL;
        }

        if(length_buffer_hole > 0)	// this means we have holes during this allocation, we will reserve this hole(cma_alloc, done by above dma_alloc_from_contiguous_addr()), but we do not mapping this hole(CMA_Pool_Mapping)
        {
            if (alloc_at_front)
            {
				/* adjust the real addr and length for buffer and buffer_hole */
                buffer->length -= length_buffer_hole;
                buffer_hole->page = buffer->page + (buffer->length >> PAGE_SHIFT);	// not sure if page can use this
                list_add(&buffer_hole->list, &buffer->list);	// add to buffer lists

				heap_info->alloc_list.freed_count++;            // add this for allocating freed hole_buffer
            }
            else
            {
				/* adjust the real addr and length for buffer and buffer_hole */
                buffer_hole->page = buffer->page;

                buffer->start_pa += buffer_hole->length;
				buffer->page = buffer->page + (buffer_hole->length >> PAGE_SHIFT);
                buffer->length -= buffer_hole->length;
                list_add(&buffer_hole->list, buffer->list.prev);	// add to buffer lists
				
				heap_info->alloc_list.freed_count++;            // add this for allocating freed hole_buffer
            }
        }
    }

ALLOCAT_BUFF_FAIL:    
    if(!ret && (flags & CMA_FLAG_VIRT_ADDR))
    {
		printk("\033[35mFunction = %s, Line = %d, mapping from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, buffer->start_pa, (buffer->start_pa+buffer->length));
		unsigned long va = CMA_Pool_Mapping(proc_info, buffer->start_pa, buffer->length, pdev);
        if(va < 0)
        {
            MCMA_CUST_ERR("CMA_Pool_Mapping fail: start_pa %lu, length %lu\n", buffer->start_pa, buffer->length);
            return ret;
        }        
        *cpu_addr = va;
        buffer->cpu_addr = (void *)va;
        buffer->mapped = 1;
    }
    dump_CMA_Pool(heap_info, true);
    dumpKRange(heap_info);
    mutex_unlock(&heap_info->lock);
    
    return ret;
}

//before call this API, lock heap info mutex firstly
static void free_buffer_list(struct cma_memory_info *heap_info, struct cma_buffer **release_buf_front , struct cma_buffer **release_buf_back)
{
    struct cma_allocation_list *buffer_list = &heap_info->alloc_list;
    struct cma_buffer *buffer = NULL, *next = NULL;
    struct cma_buffer *front = NULL, *back = NULL;
    bool min_max_change = false;

    *release_buf_front = *release_buf_back = NULL;
    if(list_empty(&buffer_list->list_head))
        return;
  
	list_for_each_entry_safe(buffer, next, &buffer_list->list_head, list)	// from this heap_info, search every buffer
    {
		if((buffer->freed == 1) && (next->freed == 1))						// in current case, buffers must be neighbor, so, merge contiguous freed buffers
       	{
           	if(buffer->start_pa + buffer->length == next->start_pa)
           	{
				printk("\033[35mFunction = %s, Line = %d, buffer_start: 0x%lX, length: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, buffer->start_pa, buffer->length);
				printk("\033[35mFunction = %s, Line = %d, next buffer_start: 0x%lX, length: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, next->start_pa, next->length);
				printk("\033[35mFunction = %s, Line = %d, merge it\033[m\n", __PRETTY_FUNCTION__, __LINE__);
               	list_del(&buffer->list);
               	next->start_pa = buffer->start_pa;               
               	next->page = buffer->page;               
               	next->length += buffer->length;
               	buffer_list->freed_count--;
               	kfree(buffer);
           	}
       	}
    }

    /* case A: freed buffer locate at beginning of alloc list */
    if(list_empty(&buffer_list->list_head))
	{
		printk("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        goto UPDATE_MIN_MAX;
	}
    
    front = list_entry(heap_info->alloc_list.list_head.next, struct cma_buffer, list);	// check if 1st buffer is free? we can only free 1st or last_one buffer because kernel protect limitation
    if(front->freed == 1)
    {
		printk("\033[35mFunction = %s, Line = %d, free front\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        list_del(&front->list);		// remove form list
        buffer_list->freed_count--;
        min_max_change = true;
        *release_buf_front = front;    
    }

    /*case B: freed buffer locate at end of kernel range*/
    if(list_empty(&buffer_list->list_head))
        goto UPDATE_MIN_MAX;
    
    back = list_entry(heap_info->alloc_list.list_head.prev, struct cma_buffer, list);	// check if last_one buffer is free? we can only free 1st or last_one buffer because kernel protect limitation
    if(back->freed == 1)
    {
		printk("\033[35mFunction = %s, Line = %d, free back\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        list_del(&back->list); 
        buffer_list->freed_count--;
        min_max_change = true;
        *release_buf_back = back; 
    }

UPDATE_MIN_MAX:  
    if(list_empty(&buffer_list->list_head))	// empty, no buffer
    {
		printk("\033[35mFunction = %s, Line = %d, list is empty\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        buffer_list->min_start = PHYSICAL_START_INIT;
        buffer_list->max_end = PHYSICAL_END_INIT;
    }
    else if(min_max_change)
    {
		printk("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        buffer = list_entry(heap_info->alloc_list.list_head.next, struct cma_buffer, list);
        buffer_list->min_start = buffer->start_pa;
        buffer = list_entry(heap_info->alloc_list.list_head.prev, struct cma_buffer, list);
        buffer_list->max_end = buffer->start_pa + buffer->length;        
    }
}

static struct cma_buffer * find_cma_buffer(struct cma_memory_info * heap_info, unsigned long start_pa, unsigned long length)
{
    struct cma_allocation_list * alloc_list = &heap_info->alloc_list;
    struct cma_buffer * buffer = NULL;
        
    mutex_lock(&heap_info->lock);
    if(list_empty(&alloc_list->list_head))
        goto FIND_CMA_BUFF_DONE;

    list_for_each_entry(buffer, &alloc_list->list_head, list)
    {
        if((start_pa >= buffer->start_pa) && ((start_pa + length) <= (buffer->start_pa + buffer->length)))
        {            
            break;
        }
    }

FIND_CMA_BUFF_DONE:
    mutex_unlock(&heap_info->lock);
    return buffer;
}

static int CMA_Pool_Free(unsigned int heap_id, unsigned long offset_in_heap, unsigned long length, filp_private * pdev)
{
    int ret = 0;
    struct cma_memory_info * heap_info = NULL;
    struct cma_buffer * release_buf_front = NULL, * release_buf_back = NULL, * find = NULL;
    struct heap_proc_info * proc_info = NULL;
    unsigned long start_pa = 0;

    heap_info = find_heap_info(heap_id);
    if(!heap_info)
        return -EINVAL;

    proc_info = find_heap_proc_info(heap_id, pdev);
    if(!proc_info)
    {
        MCMA_CUST_ERR("has not find heap_id %u its heap proc info\n", heap_id);
        return -EINVAL;
    }
    
#ifdef CMA_POOL_DUMP
    printk(KERN_ERR "\n%s:%d heapid %u\n", __FUNCTION__,__LINE__, heap_id);
#endif

    start_pa = offset_in_heap + proc_info->base_phy;
    find = find_cma_buffer(heap_info, start_pa, length);
    MCMA_BUG_ON(!find);

    if(find->freed)
    {
        MCMA_CUST_WARNING("buffer already freed [offset %llX length %llX] heap_id %u\n", (u64)offset_in_heap, (u64)length, heap_id);
        return -EFAULT;
    }

    if(find->pid != task_pid_nr(current->group_leader))
    {
        MCMA_CUST_ERR("buffer pid %d current pid %d\n", find->pid, task_pid_nr(current->group_leader));
        return -EPERM;
    }

    if(find->mapped)
    {
        ret = CMA_Pool_Unmapping(proc_info, start_pa, length, pdev);
        if(ret)
        {
            MCMA_CUST_ERR("CMA_Pool_Unmapping fail: start_pa %llX, length %llX\n", (u64)start_pa, (u64)length);
            return ret;
        }
    }

    mutex_lock(&heap_info->lock);
    MCMA_BUG_ON(heap_info->alloc_list.using_count == 0);

    SplitBuffer(start_pa, length, find, heap_info, CMA_FREE);
    //be careful, update kernel protect, then free memory to kernel
    free_buffer_list(heap_info, &release_buf_front, &release_buf_back);
    if(release_buf_front)
    {
#if 0
		printk("\033[35mFunction = %s, Line = %d, ori: release from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, start_pa, (start_pa+length));
		printk("\033[35mFunction = %s, Line = %d, after: release from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, 
				page_to_pfn((release_buf_front->page)) << PAGE_SHIFT, ((page_to_pfn((release_buf_front->page)) << PAGE_SHIFT) + release_buf_front->length));
#endif
		ret = addKRange(heap_info, release_buf_front);
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
        if(heap_info->flags & CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE){
            modify_kpage_mapping_conti(release_buf_front->page, release_buf_front->length >> PAGE_SHIFT,MT_MEMORY);
        }
#endif
        dma_release_from_contiguous(heap_info->dev, release_buf_front->page, release_buf_front->length >> PAGE_SHIFT);
        kfree(release_buf_front);
    }
  
    if(release_buf_back)
    {
#if 0
		printk("\033[35mFunction = %s, Line = %d, ori: release from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, start_pa, (start_pa+length));
		printk("\033[35mFunction = %s, Line = %d, after: release from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, 
				page_to_pfn((release_buf_back->page)) << PAGE_SHIFT, ((page_to_pfn((release_buf_back->page)) << PAGE_SHIFT) + release_buf_back->length));
#endif
        ret = addKRange(heap_info, release_buf_back);
#ifdef CONFIG_MP_CMA_PATCH_CMA_UNCACHED_DIRECT_MAPPING
        if(heap_info->flags & CMA_MEMINFO_FLG_DIRECT_MAPPING_UNCACHE){
            modify_kpage_mapping_conti(release_buf_back->page, release_buf_back->length >> PAGE_SHIFT,MT_MEMORY);
        }
#endif
        dma_release_from_contiguous(heap_info->dev, release_buf_back->page, release_buf_back->length >> PAGE_SHIFT);
        kfree(release_buf_back);
    }
    
    dump_CMA_Pool(heap_info, true);
    mutex_unlock(&heap_info->lock);
    dumpKRange(heap_info);

    return ret;
}


static unsigned long CMA_Pool_Mapping(struct heap_proc_info * proc_info, unsigned long start_pa, unsigned long length, filp_private *pdev)
{
    struct vm_area_struct *vma = NULL;
    unsigned long vaddr = 0, pfn = 0, ret_addr;
    struct mm_struct *mm = NULL;
    int err = -EFAULT;

    if((start_pa & (PAGE_SIZE -1)) || (length & (PAGE_SIZE -1)))
    {
        MCMA_CUST_ERR("start_pa %lu, length %lu\n", start_pa, length);
        return -EINVAL;
    } 

    vma = proc_info->vma;
    if(start_pa - proc_info->base_phy + length > (vma->vm_end - vma->vm_start))
    {
        MCMA_CUST_ERR("input length %lu, vma length %lu\n", start_pa - proc_info->base_phy + length, 
            vma->vm_end - vma->vm_start);

        MCMA_CUST_ERR("start_pa %lx, base_phy %lx, length %lx, vm_start %lx vm_end %lx\n", start_pa, 
            proc_info->base_phy, length, vma->vm_start, vma->vm_end);
        
        return -EINVAL;    
    }
    vaddr = vma->vm_start + (start_pa - proc_info->base_phy);
#if defined(CONFIG_ARM)
    pfn = __phys_to_pfn(start_pa);
#else
	pfn = ((unsigned long)((start_pa) >> PAGE_SHIFT));
#endif
    ret_addr = vaddr;

    if (vma)
        mm = vma->vm_mm;
    else
        mm = NULL;

    if (mm) 
        down_write(&mm->mmap_sem);

    while(length)
    {
        err = vm_insert_pfn(vma, vaddr, pfn);
        vaddr += PAGE_SIZE;
        pfn++;
        length -= PAGE_SIZE;
    }

    if (mm)        
        up_write(&mm->mmap_sem);    

    return ret_addr;
}

static int CMA_Pool_Unmapping(struct heap_proc_info * proc_info, unsigned long start_pa, unsigned long length, filp_private *pdev)
{
    struct vm_area_struct *vma = NULL;    
    unsigned long vaddr = 0, pfn = 0, ret_addr;
    struct mm_struct *mm = NULL;
    int err = 0;

    if((start_pa & (PAGE_SIZE -1)) || (length & (PAGE_SIZE -1)))
    {
        MCMA_CUST_ERR("start_pa %lu, length %lu\n", start_pa, length);
        return -EINVAL;
    }

    vma = proc_info->vma;
    if(start_pa - proc_info->base_phy + length > (vma->vm_end - vma->vm_start))
    {
        MCMA_CUST_ERR("input length %lu, vma length %lu\n", start_pa - proc_info->base_phy + length, 
            vma->vm_end - vma->vm_start);
        return -EINVAL;    
    }
    vaddr = vma->vm_start + (start_pa - proc_info->base_phy);
#if defined(CONFIG_ARM)
    pfn = __phys_to_pfn(start_pa);
#else
	pfn = ((unsigned long)((start_pa) >> PAGE_SHIFT));
#endif
    ret_addr = vaddr;

    if (vma)
        mm = vma->vm_mm;
    else
        mm = NULL;

    if (mm) 
        down_write(&mm->mmap_sem); 

    if(zap_vma_ptes(vma,(uintptr_t)vaddr, length)) 
    {
        MCMA_CUST_ERR("!!! zap_vma_ptes failed start=0x%lX size = 0x%lX by pid:%d\n", vaddr, length, (int)current->pid);
        err = -EFAULT;
    }
    
    if (mm)        
        up_write(&mm->mmap_sem);    

    return err;

}

#if defined(CONFIG_COMPAT)
static long Compat_CMA_Pool_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	long ret;

	if (!filp->f_op || !filp->f_op->unlocked_ioctl)
		return -ENOTTY;

    switch (cmd)
	{
    	case COMPAT_CMA_POOL_IOC_INIT:
    	{
			compat_uint_t uint;
			compat_ulong_t ulong;
			compat_u64 u64;
			compat_size_t usize;
			
			struct compat_cma_heap_info __user *data32;
			struct cma_heap_info __user *data;
			int	err = 0;
			
			data32 = compat_ptr(arg);						// use 32-bit variable type to point the data(compat is used for 32-bit app and 64-bit kernel, the arg is 32-bit!!
			data = compat_alloc_user_space(sizeof(*data));	// alloc a memory in user space
			if (data == NULL)
				return -EFAULT;
			
			/* 
			 * copy 32_bit data to 64_bit data
			 * we only wanna change data_size for flags, but we still need to copy all data from data32(user data) to data
			 */
			err = get_user(uint, &data32->heap_id);
			err |= put_user(uint, &data->heap_id);
			
			err = get_user(ulong, &data32->flags);
			err |= put_user(ulong, &data->flags);
			
			err = get_user(uint, &data32->miu);
			err |= put_user(uint, &data->miu);
			
			err = get_user(u64, &data32->bus_addr);
			err |= put_user(u64, &data->bus_addr);
			
			err = get_user(u64, &data32->heap_miu_start_offset);
			err |= put_user(u64, &data->heap_miu_start_offset);
			
			err = get_user(usize, &data32->heap_length);
			err |= put_user(usize, &data->heap_length);
			if(err)
				return err;

			/* call the responding 64_bit ioctl */
			ret = filp->f_op->unlocked_ioctl(filp, CMA_POOL_IOC_INIT, (unsigned long)data);

			/* 
			 * copy 64_bit data to 32_bit data for return to 32_bit app
			 */
			err = get_user(uint, &data->heap_id);
			err |= put_user(uint, &data32->heap_id);
			
			err = get_user(ulong, &data->flags);
			err |= put_user(ulong, &data32->flags);
			
			err = get_user(uint, &data->miu);
			err |= put_user(uint, &data32->miu);
			
			err = get_user(u64, &data->bus_addr);
			err |= put_user(u64, &data32->bus_addr);
			
			err = get_user(u64, &data->heap_miu_start_offset);
			err |= put_user(u64, &data32->heap_miu_start_offset);
			
			err = get_user(usize, &data->heap_length);
			err |= put_user(usize, &data32->heap_length);
			
			return ret ? ret : err;
    	}
    	case COMPAT_CMA_POOL_IOC_ALLOC: // alloc a buffer
    	{
			compat_uint_t uint;
			compat_ulong_t ulong;
			compat_u64 u64;
			compat_size_t usize;

			struct compat_cma_alloc_args __user *data32;
			struct cma_alloc_args __user *data;
			int	err = 0;
			
			data32 = compat_ptr(arg);						// use 32-bit variable type to point the data(compat is used for 32-bit app and 64-bit kernel, the arg is 32-bit!!
			data = compat_alloc_user_space(sizeof(*data));	// alloc a memory in user space
			if (data == NULL)
				return -EFAULT;

			/* 
			 * copy 32_bit data to 64_bit data
			 * we only wanna change data_size for flags, but we still need to copy all data from data32(user data) to data
			 */
			err = get_user(u64, &data32->offset_in_heap);
			err |= put_user(u64, &data->offset_in_heap);
			
			err = get_user(ulong, &data32->cpu_addr);
			err |= put_user(ulong, &data->cpu_addr);
			
			err = get_user(usize, &data32->length);
			err |= put_user(usize, &data->length);
			
			err = get_user(u64, &data32->align);
			err |= put_user(u64, &data->align);
			
			err = get_user(uint, &data32->heap_id);
			err |= put_user(uint, &data->heap_id);
			
			err = get_user(uint, &data32->flags);
			err |= put_user(uint, &data->flags);
			if(err)
				return err;

			/* call the responding 64_bit ioctl */
			ret = filp->f_op->unlocked_ioctl(filp, CMA_POOL_IOC_ALLOC, (unsigned long)data);
			
			/* 
			 * copy 64_bit data to 32_bit data for return to 32_bit app
			 */
			err = get_user(u64, &data->offset_in_heap);
			err |= put_user(u64, &data32->offset_in_heap);
			
			err = get_user(ulong, &data->cpu_addr);
			err |= put_user(ulong, &data32->cpu_addr);
			
			err = get_user(usize, &data->length);
			err |= put_user(usize, &data32->length);
			
			err = get_user(u64, &data->align);
			err |= put_user(u64, &data32->align);
			
			err = get_user(uint, &data->heap_id);
			err |= put_user(uint, &data32->heap_id);
			
			err = get_user(uint, &data->flags);
			err |= put_user(uint, &data32->flags);
			
			return ret ? ret : err;
    	}
    	case COMPAT_CMA_POOL_IOC_FREE: // free a buffer
    	{
			compat_uint_t uint;
			compat_u64 u64;
			compat_size_t usize;

			struct compat_cma_free_args __user *data32;
			struct cma_free_args __user *data;
			int	err = 0;
			
			data32 = compat_ptr(arg);						// use 32-bit variable type to point the data(compat is used for 32-bit app and 64-bit kernel, the arg is 32-bit!!
			data = compat_alloc_user_space(sizeof(*data));	// alloc a memory in user space
			if (data == NULL)
				return -EFAULT;
			
			/* 
			 * copy 32_bit data to 64_bit data
			 * we only wanna change data_size for flags, but we still need to copy all data from data32(user data) to data
			 */
			err = get_user(uint, &data32->heap_id);
			err |= put_user(uint, &data->heap_id);
			
			err = get_user(u64, &data32->offset_in_heap);
			err |= put_user(u64, &data->offset_in_heap);
			
			err = get_user(usize, &data32->length);
			err |= put_user(usize, &data->length);
			if(err)
				return err;

			/* call the responding 64_bit ioctl */
			return filp->f_op->unlocked_ioctl(filp, CMA_POOL_IOC_FREE, (unsigned long)data);
    	}
    	case CMA_POOL_IOC_RELEASE: // free heap
    	{
			return filp->f_op->unlocked_ioctl(filp, cmd, (unsigned long)compat_ptr(arg));
    	}
    	default:
			printk("\033[35mFunction = %s, Line = %d, ERROR!!! CAN NOT FIND CMD_TYPE\033[m\n", __PRETTY_FUNCTION__, __LINE__);
       		return -ENOTTY;
    }
}
#endif	

static long CMA_Pool_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    unsigned int dir;

    union {
        struct cma_heap_info heap_info;
        struct cma_alloc_args allocation;
        struct cma_free_args free; 
        unsigned int heap_id;
    } data;

    dir = _IOC_DIR(cmd);

    if (_IOC_SIZE(cmd) > sizeof(data))
        return -EINVAL;

	printk("\033[35mFunction = %s, Line = %d, copy_from_user: %d bytes\033[m\n", __PRETTY_FUNCTION__, __LINE__, _IOC_SIZE(cmd));
    if (dir & _IOC_WRITE)
        if (copy_from_user(&data, (void __user *)arg, _IOC_SIZE(cmd)))
            return -EFAULT;

    switch (cmd) {
    case CMA_POOL_IOC_INIT:
    {
        ret = CMA_Pool_Init(data.heap_info.heap_id, data.heap_info.flags, &data.heap_info.miu, 
                    &data.heap_info.bus_addr, &data.heap_info.heap_miu_start_offset, 
                    &data.heap_info.heap_length, (filp_private *)filp->private_data);
        if(ret)
            return -EFAULT;

        if(copy_to_user((void __user *)arg, &data, _IOC_SIZE(cmd))) 
        {
            MCMA_CUST_ERR("copy to user error\n");
            ret = -EFAULT;       
        }

        break;
    }
    case CMA_POOL_IOC_ALLOC: // alloc a buffer
    {
        ret = CMA_Pool_Alloc(data.allocation.heap_id, data.allocation.offset_in_heap, 
            &data.allocation.cpu_addr, data.allocation.length, data.allocation.align, 
            data.allocation.flags, (filp_private *)filp->private_data);      
        
	   	/* it seems we need to do copy_to_user for data.allocation.cpu_addr(va) */
        if(ret)
		{
			printk("\033[35mFunction = %s, Line = %d, CMA_Pool_Alloc Failed, the ret is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, ret);
            return -EFAULT;
		}
        break;
    }
    case CMA_POOL_IOC_FREE: // free a buffer
    {
        ret = CMA_Pool_Free(data.free.heap_id, data.free.offset_in_heap, data.free.length, 
            (filp_private *)filp->private_data);        
        if(ret)
            return -EFAULT;
        
        break;
    }
    case CMA_POOL_IOC_RELEASE: // free heap
    {
        ret = CMA_Pool_Release_Heap(data.heap_id, (filp_private *)filp->private_data);
        if(ret)
            return -EFAULT;

        break;
    }
    default:
        return -ENOTTY;
    }

    if (dir & _IOC_READ) 
    {
        if (copy_to_user((void __user *)arg, &data, _IOC_SIZE(cmd))) 
        {
            MCMA_CUST_ERR("copy to user fail\n");
            return -EFAULT;
        }
    }

    return ret;
}

static int CMA_Pool_MMap(struct file *filp, struct vm_area_struct *vma)
{
    filp_private * pdev = (filp_private *)filp->private_data;
    struct heap_proc_info* heap_proc_info = NULL;

    
    if (NULL == vma)  
        return -EINVAL;

    heap_proc_info = find_heap_proc_info(pdev->temp_heap_id, pdev);
    if(heap_proc_info == NULL)
    {
        MCMA_CUST_ERR("not find heap id %u proc info!!!\n", pdev->temp_heap_id);
        return -EINVAL;
    }

    vma->vm_flags |= VM_IO;
    vma->vm_flags |= VM_PFNMAP;

    //printk(KERN_ERR "CMA_Pool_MMap vma-start %lx vma-end %lx\n", vma->vm_start, vma->vm_end);
    if(!heap_proc_info->vma_cached)
    {
#if (defined(MIPS))
        vma->vm_page_prot.pgprot = (vma->vm_page_prot.pgprot & ~_CACHE_MASK) | _CACHE_UNCACHED; /* MIPS's equivalents of 'pgprot_writecombine' */
#else
        vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
#endif
    } 
    
    vma->vm_ops = &cma_vm_ops;
    heap_proc_info->vma = vma;

    return 0;
}


static int _insertKRange(int miu_index, unsigned long lx_addr, unsigned long lx_length)
{   
    MIU_ProtectRange *krange = NULL;

    if(!list_empty(&glob_miu_kranges[miu_index].list_head))
    {
        krange= list_entry(glob_miu_kranges[miu_index].list_head.prev, MIU_ProtectRange, list_node);
        if((krange->start_pa+krange->length) == lx_addr)
        {
            _miu_kernel_protect(krange->miuBlockIndex, kernal_protect_client_id, 
                krange->start_pa, krange->start_pa+krange->length, MIU_PROTECT_DISABLE);

            krange->length += lx_length;
            _miu_kernel_protect(krange->miuBlockIndex, kernal_protect_client_id, 
                krange->start_pa, krange->start_pa+krange->length, MIU_PROTECT_ENABLE);    
            return 0;
        }
    }
    return -1;
}

static void init_glob_miu_kranges(void)
{
    unsigned long offset = 0;
    MIU_ProtectRange *krange = NULL;
    int i = 0, miu_index = 0;
    
    for(i = 0; i < KERN_CHUNK_NUM; ++i)
    {
        glob_miu_kranges[i].miu = i;
        memset(glob_miu_kranges[i].miuBlockStatus, 0, sizeof(unsigned char)*MIU_BLOCK_NUM);
        glob_miu_kranges[i].krange_num = 0;
        mutex_init(&glob_miu_kranges[i].lock);
        INIT_LIST_HEAD(&glob_miu_kranges[i].list_head);
    }

    if(lx_mem_size != INVALID_PHY_ADDR)
    {
        phy_to_MiuOffset(PHYS_OFFSET, &miu_index, &offset);
        
        krange = (MIU_ProtectRange *)kzalloc(sizeof(MIU_ProtectRange), GFP_KERNEL);
        MCMA_BUG_ON(!krange);
        INIT_LIST_HEAD(&krange->list_node);
        krange->start_pa = PHYS_OFFSET;
        krange->length = lx_mem_size;
        krange->miuBlockIndex = glob_miu_kranges[miu_index].krange_num;	// use miu_index's kernel protect
        // kernel protect block index start with 0
        
		printk("\033[35mFunction = %s, Line = %d, [INIT] for LX0 kprotect: from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, krange->start_pa, krange->start_pa+krange->length);
		_miu_kernel_protect(krange->miuBlockIndex, kernal_protect_client_id, 
            krange->start_pa, krange->start_pa+krange->length, MIU_PROTECT_ENABLE);        
        glob_miu_kranges[miu_index].miuBlockStatus[krange->miuBlockIndex] = MIU_BLOCK_BUSY;

        glob_miu_kranges[miu_index].krange_num++;						// next miu_index's kernel protect id 
        list_add_tail(&krange->list_node, &glob_miu_kranges[miu_index].list_head);
    }    
   
    if(lx_mem2_size != INVALID_PHY_ADDR)
    {
        phy_to_MiuOffset(lx_mem2_addr, &miu_index, &offset);
        
        if(_insertKRange(miu_index, lx_mem2_addr, lx_mem2_size))
        {            
            krange = (MIU_ProtectRange *)kzalloc(sizeof(MIU_ProtectRange), GFP_KERNEL);
            MCMA_BUG_ON(!krange);
            INIT_LIST_HEAD(&krange->list_node);
            krange->start_pa = lx_mem2_addr;
            krange->length = lx_mem2_size;
            krange->miuBlockIndex = glob_miu_kranges[miu_index].krange_num;
            //kernel protect block index start with 0
			
			printk("\033[35mFunction = %s, Line = %d, [INIT] for LX1 kprotect: from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, krange->start_pa, krange->start_pa+krange->length);
            _miu_kernel_protect(krange->miuBlockIndex, kernal_protect_client_id, 
                krange->start_pa, krange->start_pa+krange->length, MIU_PROTECT_ENABLE);        
            glob_miu_kranges[miu_index].miuBlockStatus[krange->miuBlockIndex] = MIU_BLOCK_BUSY;

            glob_miu_kranges[miu_index].krange_num++;
            list_add_tail(&krange->list_node, &glob_miu_kranges[miu_index].list_head);
        }
    }

    if(lx_mem3_size != INVALID_PHY_ADDR) 
    {      
        phy_to_MiuOffset(lx_mem3_addr, &miu_index, &offset);

        if(_insertKRange(miu_index, lx_mem3_addr, lx_mem3_size))
        {           
            krange = (MIU_ProtectRange *)kzalloc(sizeof(MIU_ProtectRange), GFP_KERNEL);
            MCMA_BUG_ON(!krange);
            INIT_LIST_HEAD(&krange->list_node);
            krange->start_pa = lx_mem3_addr;
            krange->length = lx_mem3_size;
            krange->miuBlockIndex = glob_miu_kranges[miu_index].krange_num;
            //kernel protect block index start with 0
            _miu_kernel_protect(krange->miuBlockIndex, kernal_protect_client_id, 
                krange->start_pa, krange->start_pa+krange->length, MIU_PROTECT_ENABLE);        
            glob_miu_kranges[miu_index].miuBlockStatus[krange->miuBlockIndex] = MIU_BLOCK_BUSY; 

            glob_miu_kranges[miu_index].krange_num++;
           
            list_add_tail(&krange->list_node, &glob_miu_kranges[miu_index].list_head);
        }
    }       
}

static void dump_CMA_Pool(struct cma_memory_info * heap_info, bool force_print)
{
    struct cma_allocation_list *buffer_list = NULL;
    struct cma_buffer *buffer = NULL;
    struct cma *cma_area = NULL;

    if(!force_print)
    {
#ifndef CMA_POOL_DUMP
        return;
#endif
    }

    cma_area = heap_info->dev->cma_area;
    buffer_list = &heap_info->alloc_list;

    printk(KERN_ERR "dump heap_id %u information: min_start 0x%lX, max_end 0x%lX\n", heap_info->heap_id,
        buffer_list->min_start, buffer_list->max_end);
    list_for_each_entry(buffer, &buffer_list->list_head, list)
    {
        printk(KERN_ERR "allocated info: cpu address 0x%lX, phy_addr 0x%lX, length 0x%lX, pid %lu, freed %d, mapped %d\n", 
            (unsigned long)buffer->cpu_addr, buffer->start_pa, buffer->length, 
            (unsigned long)buffer->pid, buffer->freed, buffer->mapped);
    }
}

static ssize_t kprotect_enable(struct file *file, const char __user *user_buf, size_t size, loff_t *ppos)
{
    int len = 2;
    char buf[2];

    if(size > 2)
	    len = 2;

    if(__copy_from_user(buf, user_buf, len))
    {        
	    return -EFAULT;
    }

    if(buf[0]=='0')
    {
	    atomic_set(&kprotect_enabled, 0);
    }
    else if(buf[0]=='1')
    {
	    atomic_set(&kprotect_enabled, 1);
    }

    if(atomic_read(&kprotect_enabled) > 0)
        printk(KERN_ERR "kernel protect enabled\n");
    else
       printk(KERN_ERR "kernel protect disabled\n");
    
    return size;
}

int kprotect_status(struct seq_file *m, void *v)
{
    int kprotect_value;
    char buf[2];

	kprotect_value = atomic_read(&kprotect_enabled);
    if(kprotect_value == 0)
        buf[0] = '0';
    else
        buf[0] = '1';
    
    //seq_write(m, (const void *)buf, 1);
    if(kprotect_value > 0)
        printk(KERN_ERR "kernel protect enabled\n");
    else
        printk(KERN_ERR "kernel protect disabled\n");

    return 0;
}

static int miu_kernel_protect_open(struct inode *inode, struct file *file)
{
	return single_open(file, kprotect_status, inode->i_private);
}

static const struct file_operations kprotect_fops = {
	.owner		= THIS_MODULE,
	.open		= miu_kernel_protect_open,
	.read		= seq_read,
	.write		= kprotect_enable,
 	.llseek		= seq_lseek,	
	.release	= single_release,
};

MSYSTEM_STATIC int __init mod_cmapool_init(void)
{
    int s32Ret;
    dev_t dev;
    struct dentry * read_file = NULL;

    cmapool_class = class_create(THIS_MODULE, "cmapool");
    if (IS_ERR(cmapool_class))
    {
        return PTR_ERR(cmapool_class);
    }

    if (CMAPoolDev.s32Major)
    {
        dev = MKDEV(CMAPoolDev.s32Major, CMAPoolDev.s32Minor);
        s32Ret = register_chrdev_region(dev, MOD_CMAPOOL_DEVICE_COUNT, MOD_CMAPOOL_NAME);
    }
    else
    {
        s32Ret = alloc_chrdev_region(&dev, CMAPoolDev.s32Minor, MOD_CMAPOOL_DEVICE_COUNT, MOD_CMAPOOL_NAME);
        CMAPoolDev.s32Major = MAJOR(dev);
    }

    if (0 > s32Ret)
    {
        printk(KERN_ERR "Unable to get major %d\n", CMAPoolDev.s32Major);
        class_destroy(cmapool_class);
        return s32Ret;
    }

    cdev_init(&CMAPoolDev.cDevice, &CMAPoolDev.CMAPoolFop);
    if (0 != (s32Ret= cdev_add(&CMAPoolDev.cDevice, dev, MOD_CMAPOOL_DEVICE_COUNT)))
    {
        printk(KERN_ERR "Unable add a character device\n");
        unregister_chrdev_region(dev, MOD_CMAPOOL_DEVICE_COUNT);
        class_destroy(cmapool_class);
        return s32Ret;
    }
    device_create(cmapool_class, NULL, dev, NULL, MOD_CMAPOOL_NAME);

    MDrv_MIU_Init();
    kernal_protect_client_id = MDrv_MIU_GetDefaultClientID_KernelProtect();
    init_glob_miu_kranges();		// to set kernel_protect for each lxmem
    INIT_LIST_HEAD(&heap_info_list);

    CMAPoolDev.debug_root = debugfs_create_dir("cma_pool", NULL);
	if (!CMAPoolDev.debug_root) 
    {
		printk(KERN_ERR "cma pool: failed to create debugfs root directory.\n");
	}
    else
    {
        read_file = debugfs_create_file("kprotect_state", 0664, CMAPoolDev.debug_root,
						NULL, &kprotect_fops);
        if (!read_file)
            printk(KERN_ERR "cma pool: failed to create debugfs file kprotect_state\n");
    }

    return 0;
}


MSYSTEM_STATIC void __exit mod_cmapool_exit(void)
{
    cdev_del(&CMAPoolDev.cDevice);
    debugfs_remove_recursive(CMAPoolDev.debug_root);
    unregister_chrdev_region(MKDEV(CMAPoolDev.s32Major, CMAPoolDev.s32Minor), MOD_CMAPOOL_DEVICE_COUNT);

    device_destroy(cmapool_class, MKDEV(CMAPoolDev.s32Major, CMAPoolDev.s32Minor));
    class_destroy(cmapool_class);
}
#ifdef CONFIG_MSTAR_CMAPOOL
module_init(mod_cmapool_init);
module_exit(mod_cmapool_exit);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("CMAPOOL driver");
MODULE_LICENSE("GPL");
#endif

