////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mstar_ion.c
/// @brief  mstar ion driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/memblock.h>
#include <linux/dma-contiguous.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include "ion_customer.h"
#include "../ion.h"
#include "../ion_priv.h"
#include "mdrv_types.h"
#include "mdrv_system.h"
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/dma-mapping.h>
#include <linux/highmem.h>

#include "../ion_mstar_cma_heap.h"
#include "compat_ion_customer.h"
static int num_heaps;
static struct ion_device *idev;
static struct ion_heap **heaps;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define ION_COMPAT_STR  "mstar,ion-mstar"
#define ION_SYSTEM_HEAP_NAME  "ion_system_heap"

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
extern struct CMA_BootArgs_Config cma_config[MAX_CMA_AREAS];
extern struct device mstar_cma_device[MAX_CMA_AREAS];
extern int mstar_driver_boot_cma_buffer_num;
#else
int mstar_driver_boot_cma_buffer_num = 0;
#endif

static struct ion_heap* s_mali_current_heap = NULL;
static struct ion_heap* mali_heaps[3] = {NULL};		//mali heaps: [0] mali miu 0 heap
static int s_current_mali_alloc_strategy = -1;
int mali_alloc_strategy = 0;

#define CMABITMAP_OUT_PATH "/mnt/usb/sda1/"
#define CMABITMAP_OUT_FILE "cmabitmap_"
#define CMARUNLIST_PROC_NAME "cmarunlist_"
#define CMABITMAP_PROC_NAME "cmabitmap_"
char cmarunlist_proc_name[15][16];
char cmabitmap_proc_name[15][16];

/**
 * These heaps are listed in the order they will be allocated.
 * Don't swap the order unless you know what you are doing!
 */
struct ion_platform_heap mstar_default_heaps[] = {
		/*
		 * type means the heap_type of ion, which will decide the alloc/free methods, map[user/kernel], ...
		 * id is used to specify different usage, each id will have a unique heap, we can alloc buffer with a heap_id
		 * cma heap is added by boot args, don't add cma id here manually
		 */

		{
			.id    = ION_SYSTEM_HEAP_ID,
			.type   = ION_HEAP_TYPE_SYSTEM,
			.name   = ION_SYSTEM_HEAP_NAME,
		},
};

static struct ion_platform_data ion_pdata;

static struct platform_device ion_dev = {
	.name = "ion-mstar",
	.id = 1,
	.dev = { .platform_data = &ion_pdata },
};

static struct platform_device *common_ion_devices[] __initdata = {
	&ion_dev,
};

//for kernel mali driver usage
void mali_attch_heap(void)
{
	int i;
	struct mstar_cma_heap_private *mcma_private = NULL;

	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &ion_pdata.heaps[i];
		
		if(heap_data->id == ION_MALI_MIUO_HEAP_ID)
			mali_heaps[0] = heaps[i];
		else if(heap_data->id == ION_MALI_MIU1_HEAP_ID)
			mali_heaps[1] = heaps[i];
		else if(heap_data->id == ION_MALI_MIU2_HEAP_ID)
			mali_heaps[2] = heaps[i];
	}
}

struct ion_heap *find_heap_by_page(struct page* page)
{
	int i;

	for (i = 0; i < 3; i++) {
		if(mali_heaps[i] && in_cma_range(mali_heaps[i],page))
			return mali_heaps[i];
	}
	return NULL;
}

struct page* mali_alloc_page(void)
{
	struct page *page;

	if(s_current_mali_alloc_strategy != mali_alloc_strategy){
		BUG_ON(mali_alloc_strategy < 0 || mali_alloc_strategy >= 3);
		s_mali_current_heap = mali_heaps[mali_alloc_strategy];
		s_current_mali_alloc_strategy = mali_alloc_strategy;
	}

	if(s_mali_current_heap){
		page = __mstar_get_discrete(s_mali_current_heap);
		if(page)
			clear_highpage(page);//printk("alloc page pfn=%x \n ",page_to_pfn(page));
		else
			printk("alloc null page \n");
	}
	else
		page = alloc_page(GFP_HIGHUSER | __GFP_ZERO | __GFP_REPEAT | __GFP_NOWARN | __GFP_COLD);

	return page;
}
EXPORT_SYMBOL(mali_alloc_page);

void mali_free_page(struct page* page)
{
	int ret;
	struct ion_heap* heap;
	if(s_mali_current_heap){
		ret = __mstar_free_one_page(s_mali_current_heap,page);
		if(ret){	//current heap changed, get the heap the page alloc from
			heap = find_heap_by_page(page);
			if(heap)
				__mstar_free_one_page(heap,page);
			else
				__free_page(page);
		}
	}
	else{
		//case 1: alloc with current_heap !NULL, but when free, current_heap changed to NULL
		//case 2: alloc with current_heap NULL, free with current_heap NULL
		//can not judge which case unless do find_heap_by_page
		heap = find_heap_by_page(page);
		if(heap)
			__mstar_free_one_page(heap,page);
		else
			__free_page(page);
	}
}
EXPORT_SYMBOL(mali_free_page);

inline unsigned long get_mali_alloc_strategy(void)
{
	return mali_alloc_strategy;
}
EXPORT_SYMBOL(get_mali_alloc_strategy);


inline void set_mali_alloc_strategy(unsigned long mali)
{
	mali_alloc_strategy = mali;
}
EXPORT_SYMBOL(set_mali_alloc_strategy);

extern struct cma *dma_contiguous_default_area;
void get_cma_status(char *info)
{
	int i;
	struct ion_heap *heap;
	for (i = 0; i < num_heaps; i++) {
		int cma_status[4] = {0};
		char cma_info[64] = {0};
		char name[16] = {0};
		heap = heaps[i];
		if(heap->type != ION_HEAP_TYPE_MSTAR_CMA && heap->type != ION_HEAP_TYPE_SYSTEM)
			continue;

		if(heap->type == ION_HEAP_TYPE_MSTAR_CMA)
		{
			get_cma_heap_info(heap, cma_status, name);
			sprintf(cma_info,"%s (%dkb %dkb %dkb %dkb) ", name, cma_status[0]*4, cma_status[1]*4, cma_status[2]*4, cma_status[3]*4);		// we use kb instead of page_cnt
		}
		else
		{
			get_system_heap_info(dma_contiguous_default_area, cma_status);
			sprintf(cma_info,"%s (%dkb %dkb %dkb %dkb) ", " DEFAULT_CMA_BUFFER", cma_status[0]*4, cma_status[1]*4, cma_status[2]*4, cma_status[3]*4);
		}
		strcat(info,cma_info);
	}
}

//for mali end

#ifdef CONFIG_PROC_FS
static int cmarunlist_proc_show(struct seq_file *m, void *v)
{
	int frst_bit = 0, last_bit = 0, this_bit = 0;
	struct cma *cma = (struct cma *)m->private;

	if(!cma)
		return 0;

	mutex_lock(&cma->lock);
	
	frst_bit = bitmap_find_next_zero_area(cma->bitmap, cma->count, 0, 1, 0);
	if (this_bit >= cma->count){
		goto out;
	}
	this_bit = frst_bit;
	last_bit = frst_bit;
		
	while(1){
		this_bit = bitmap_find_next_zero_area(cma->bitmap, cma->count, this_bit + 1, 1, 0);
		if (this_bit >= cma->count){
			printk("free pages: %d~%d, page num = %d\n", frst_bit, last_bit, last_bit - frst_bit + 1);
			break;
		}
		if(this_bit > last_bit + 1){
			printk("free pages: %d~%d, page num = %d\n", frst_bit, last_bit, last_bit - frst_bit + 1);
			frst_bit = this_bit;
		}
		last_bit = this_bit;
	}
out:
	mutex_unlock(&cma->lock);
	return 0;
}

static int cmarunlist_proc_open(struct inode *inode, struct file *file)
{
	int n = 0, ret=0;
	unsigned long freebits = 0;
	struct ion_heap *heap = NULL;
	struct ion_mstar_cma_heap *mheap = NULL;
	struct cma *cma;
	const char *filename = file->f_path.dentry->d_name.name + strlen(CMARUNLIST_PROC_NAME);

	for(n = 0; n < MAX_CMA_AREAS; n++){
		if(heaps[n] && strcmp(heaps[n]->name, filename) == 0)
			heap = heaps[n];
	}
	if(!heap)
		return 0;
	
	mheap = to_mstar_cma_heap(heap);
	cma = mheap->mstar_cma->cma;
	if(cma){
		freebits = get_free_bit_count(cma, 0, cma->count);
		printk("cma %s, total page %lu, free page %lu\n", heap->name, cma->count, freebits);
		ret = single_open(file, cmarunlist_proc_show, cma);
	}
	return ret;
}

static const struct file_operations cmarunlist_proc_fops = {
	.open		= cmarunlist_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int cmabitmap_proc_show(struct seq_file *m, void *v)
{
	int ret;
	loff_t pos = 0;
	struct ion_heap *heap = (struct ion_heap *)m->private;
	struct ion_mstar_cma_heap *mheap = to_mstar_cma_heap(heap);
	struct cma *cma = mheap->mstar_cma->cma;
	struct file *filp = NULL;
	char filename[64];
	
	memset(filename, 0, 64);
	sprintf(filename, "%s%s%s", CMABITMAP_OUT_PATH, CMABITMAP_OUT_FILE, heap->name);
	filp =  filp_open(filename, O_RDWR | O_TRUNC | O_CREAT, 0777);
	if(!filp || IS_ERR(filp)){
		printk("%s, open file %s failed, error %ld\n", __func__, filename, PTR_ERR(filp));
		ret = PTR_ERR(filp);
		goto out;
	}
	set_fs(get_ds());
	ret = vfs_write(filp, (char __user *)cma->bitmap, cma->count / 8, &pos);
	
out:
	mutex_unlock(&cma->lock);
	if(!PTR_ERR(filp))
		filp_close(filp, NULL);
	return ret;
}

static int cmabitmap_proc_open(struct inode *inode, struct file *file)
{
	int n = 0, ret=0; 
	unsigned long freebits = 0;
	struct ion_heap *heap = NULL;
	struct ion_mstar_cma_heap *mheap = NULL;
	struct cma *cma = NULL;
	const char *filename = file->f_path.dentry->d_name.name + strlen(CMABITMAP_PROC_NAME);
	
	for(n = 0; n < MAX_CMA_AREAS; n++){
		if(heaps[n] && strcmp(heaps[n]->name, filename) == 0)
			heap = heaps[n];
	}
	if(!heap)
		return ret;
	
	mheap = to_mstar_cma_heap(heap);
	cma = mheap->mstar_cma->cma;
	if(cma){
		freebits = get_free_bit_count(cma, 0, cma->count);
		printk("cma %s, total page %lu, free page %lu\n", heap->name, cma->count, freebits);
		return single_open(file, cmabitmap_proc_show, heap);
	}
	return ret;
}

static const struct file_operations cmabitmap_proc_fops = {
	.open		= cmabitmap_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#endif 


static int mstar_ion_probe(struct platform_device *pdev)
{
	struct ion_platform_data *pdata = pdev->dev.platform_data;
	int err;
	int i;

	num_heaps = pdata->nr;
	printk("\033[31mFunction = %s, Line = %d, doing mstar_ion_probe for %s, having %d heaps\033[m\n", __PRETTY_FUNCTION__, __LINE__, pdev->name, num_heaps); // joe.liu

	heaps = kzalloc(sizeof(struct ion_heap *) * pdata->nr, GFP_KERNEL);


#ifdef CONFIG_MP_ION_PATCH_MSTAR_CUSTOMER_IOCTL
#ifdef CONFIG_COMPAT
    idev = ion_device_create(compat_mstar_custom_ioctl);
#else
	idev = ion_device_create(mstar_custom_ioctl);
#endif
#else
	idev = ion_device_create(NULL);
#endif

	if (IS_ERR_OR_NULL(idev)) {
		kfree(heaps);
		return PTR_ERR(idev);
	}

	/* create the heaps as specified in the mstar_default_heaps[] and parsed from bootargs */
	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &pdata->heaps[i];
		printk("\033[35mFunction = %s, Line = %d, create heap %s, for type %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, heap_data->name, heap_data->type); // joe.liu

		heaps[i] = ion_heap_create(heap_data);
		if (IS_ERR_OR_NULL(heaps[i])) {
			err = PTR_ERR(heaps[i]);
			goto err;
		}
		ion_device_add_heap(idev, heaps[i]);

#ifdef CONFIG_PROC_FS		
		if(heaps[i]->type == ION_HEAP_TYPE_MSTAR_CMA){
			//create cma runlist proc file	
			memset(cmarunlist_proc_name[i], 0, 16);
			sprintf(cmarunlist_proc_name[i], "%s%s", CMARUNLIST_PROC_NAME, heaps[i]->name);
			proc_create(cmarunlist_proc_name[i], 0, NULL, &cmarunlist_proc_fops);

			//create cma bimap proc file
			memset(cmabitmap_proc_name[i], 0, 16);
			sprintf(cmabitmap_proc_name[i], "%s%s", CMABITMAP_PROC_NAME, heaps[i]->name);
			proc_create(cmabitmap_proc_name[i], 0, NULL, &cmabitmap_proc_fops);

		}
#endif
	}
	mali_attch_heap();

	platform_set_drvdata(pdev, idev);

	return 0;
err:
	for (i = 0; i < num_heaps; i++) {
		if (heaps[i])
			ion_heap_destroy(heaps[i]);
	}
	kfree(heaps);
	return err;
}

static int mstar_ion_remove(struct platform_device *pdev)
{
	struct ion_device *idev = platform_get_drvdata(pdev);
	int i;

	ion_device_destroy(idev);
	for (i = 0; i < num_heaps; i++)
		ion_heap_destroy(heaps[i]);
	kfree(heaps);
	return 0;
}

static struct of_device_id mstar_ion_match_table[] = {
	{.compatible = ION_COMPAT_STR},
	{},
};

static struct platform_driver mstar_ion_driver = {
	.probe = mstar_ion_probe,
	.remove = mstar_ion_remove,
	.driver = {
		.name = "ion-mstar",
		.of_match_table = mstar_ion_match_table,
	},
};

static int __init mstar_ion_init(void)
{
	int index = 0;
	struct ion_platform_heap *heaps_info = NULL;

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
	struct CMA_BootArgs_Config *heapconfig = NULL;
	int add_heap_nr = 0;
	struct mstar_cma_heap_private *mcma_private = NULL;
#endif

	printk("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu

	// mstar_default_heaps is default(we set it in kernel code), mstar_driver_boot_cma_buffer_num is set in bootargs
	printk("\033[35mFunction = %s, Line = %d, default_heap: %d, mstar_driver heap: %d\033[m\n", __PRETTY_FUNCTION__, __LINE__,  ARRAY_SIZE(mstar_default_heaps), mstar_driver_boot_cma_buffer_num); // joe.liu
	ion_pdata.nr = ARRAY_SIZE(mstar_default_heaps) + mstar_driver_boot_cma_buffer_num;
	heaps_info = kzalloc(sizeof(struct ion_platform_heap) * ion_pdata.nr, GFP_KERNEL);
	if(!heaps_info)
		return -ENOMEM;

	for(index = 0; index < ARRAY_SIZE(mstar_default_heaps); ++index)
	{
		heaps_info[index].id = mstar_default_heaps[index].id;
		heaps_info[index].type= mstar_default_heaps[index].type;
		heaps_info[index].name = mstar_default_heaps[index].name;
	}

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
	/*add heap id of cma type by parsing cma bootargs(stored in cma_config)*/
	index = ARRAY_SIZE(mstar_default_heaps);
	heapconfig = &cma_config[0];

	while(add_heap_nr < mstar_driver_boot_cma_buffer_num)
	{
		BUG_ON(heapconfig->size == 0);
		BUG_ON( (heapconfig->pool_id < ION_CMA_HEAP_ID_START) || (heapconfig->pool_id >= ION_HEAP_ID_RESERVED) );

		heaps_info[index].id = heapconfig->pool_id;
		heaps_info[index].type= ION_HEAP_TYPE_MSTAR_CMA;
		heaps_info[index].name = heapconfig->name;
		mcma_private = kzalloc(sizeof(struct mstar_cma_heap_private),GFP_KERNEL);
		if(!mcma_private)
			goto err_ret;
		mcma_private->cma_dev = &mstar_cma_device[add_heap_nr];
		
		//FIXME: judge by pool_id ?
		if((heapconfig->pool_id >= ION_MALI_MIUO_HEAP_ID)
		   && (heapconfig->pool_id <= ION_MALI_MIU2_HEAP_ID))
			mcma_private->flag = DESCRETE_CMA;
		else  //VDEC XC DIP heap id
			mcma_private->flag = CONTINUOUS_ONLY_CMA;

		heaps_info[index].priv = mcma_private;
		heapconfig++;
		add_heap_nr++;
		index++;
	}
#endif

	ion_pdata.heaps = heaps_info;

	platform_add_devices(common_ion_devices, ARRAY_SIZE(common_ion_devices));	// add platform_device first, for binding platform_device and platform_driver
	return platform_driver_register(&mstar_ion_driver);

err_ret:
	for(; index >= 0; index --){
		mcma_private = heaps_info[index].priv;
		kfree(mcma_private);
	}
	return 0;
}

static void __exit mstar_ion_exit(void)
{
	int i;
	struct ion_platform_heap *heaps_info;
	heaps_info = ion_pdata.heaps;
	for(i = 0 ; i < mstar_driver_boot_cma_buffer_num ; i++){
		kfree(heaps_info[i + ARRAY_SIZE(mstar_default_heaps)].priv);
	}
	kfree(ion_pdata.heaps);
	platform_device_unregister(common_ion_devices[0]);
	platform_driver_unregister(&mstar_ion_driver);
}

subsys_initcall(mstar_ion_init);
module_exit(mstar_ion_exit);

