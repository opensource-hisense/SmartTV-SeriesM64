/*
 *
 * (C) COPYRIGHT ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */





#include <linux/ump.h>
#include <linux/dma-mapping.h>
#include <linux/ion.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

/* MSTAR patch begin */
#ifdef MSTAR
#include "mstar/mstar_chip.h"
#define PHYS_TO_BUS_ADDRESS_ADJUST  MSTAR_MIU0_BUS_BASE
#endif
/* MSTAR patch end */

struct ion_wrapping_info
{
	struct ion_client *   ion_client;
	struct ion_handle *   ion_handle;
	int                   num_phys_blocks;
	struct scatterlist *  sglist;
};

static struct ion_device * ion_device_get(void)
{
	/* < Customer to provide implementation >
	 * Return a pointer to the global ion_device on the system
	 */
	return NULL;
}

static unsigned int ion_heap_mask_get(void)
{
	/* < Customer to provide implementation >
	 * Return the heap mask to use with UMP clients.
	 * Suspect this can be kept as 0 as we won't allocate memory, just import.
	 */
	return 0;
}

static int import_ion_client_create(void** const custom_session_data)
{
	struct ion_client ** ion_client;

	ion_client = (struct ion_client**)custom_session_data;
	*ion_client = ion_client_create(ion_device_get(), ion_heap_mask_get(), "ump");

	return PTR_RET(*ion_client);
}


static void import_ion_client_destroy(void* custom_session_data)
{
	struct ion_client * ion_client;

	ion_client = (struct ion_client*)custom_session_data;
	BUG_ON(!ion_client);

	ion_client_destroy(ion_client);
}


static void import_ion_final_release_callback(const ump_dd_handle handle, void * info)
{
	struct ion_wrapping_info * ion_info;

	BUG_ON(!info);

	(void)handle;
	ion_info = (struct ion_wrapping_info*)info;

	dma_unmap_sg(NULL, ion_info->sglist, ion_info->num_phys_blocks, DMA_BIDIRECTIONAL);
	ion_unmap_dma(ion_info->ion_client, ion_info->ion_handle);
	ion_free(ion_info->ion_client, ion_info->ion_handle);
	kfree(ion_info);
	module_put(THIS_MODULE);
}

/* MSTAR patch begin */
#if MSTAR_UMP_IMPORT_PA
static ump_dd_handle import_ion_import(void * custom_session_data, void * pfd, uint32_t size, ump_alloc_flags flags)
#else
static ump_dd_handle import_ion_import(void * custom_session_data, void * pfd, ump_alloc_flags flags)
#endif
/* MSTAR patch end */
{
	int fd;
	ump_dd_handle ump_handle;
	struct scatterlist * sg;
	int num_dma_blocks;
	ump_dd_physical_block_64 * phys_blocks;
	unsigned long i;

	struct ion_wrapping_info * ion_info;

	BUG_ON(!custom_session_data);
	BUG_ON(!pfd);

	/* MSTAR patch begin */
	ion_info = kzalloc(sizeof(*ion_info), GFP_KERNEL);
	/* MSTAR patch end */
	if (NULL == ion_info)
	{
		return UMP_DD_INVALID_MEMORY_HANDLE;
	}

	ion_info->ion_client = (struct ion_client*)custom_session_data;

	if (get_user(fd, (int*)pfd))
	{
		goto out;
	}

	ion_info->ion_handle = ion_import_fd(ion_info->ion_client, fd);

	if (IS_ERR_OR_NULL(ion_info->ion_handle))
	{
		goto out;
	}

	ion_info->sglist = ion_map_dma(ion_info->ion_client, ion_info->ion_handle);
	if (IS_ERR_OR_NULL(ion_info->sglist))
	{
		goto ion_dma_map_failed;
	}

	sg = ion_info->sglist;
	while (sg)
	{
		ion_info->num_phys_blocks++;
		sg = sg_next(sg);
	}

	num_dma_blocks = dma_map_sg(NULL, ion_info->sglist, ion_info->num_phys_blocks, DMA_BIDIRECTIONAL);

	if (0 == num_dma_blocks)
	{
		goto linux_dma_map_failed;
	}

	phys_blocks = vmalloc(num_dma_blocks * sizeof(*phys_blocks));
	if (NULL == phys_blocks)
	{
		goto vmalloc_failed;
	}

	for_each_sg(ion_info->sglist, sg, num_dma_blocks, i)
	{
		phys_blocks[i].addr = sg_phys(sg);
		phys_blocks[i].size = sg_dma_len(sg);

		/* MSTAR patch begin */
#ifdef MSTAR
		phys_blocks[i].addr += PHYS_TO_BUS_ADDRESS_ADJUST;
#endif
		/* MSTAR patch end */
	}

	ump_handle = ump_dd_create_from_phys_blocks_64(phys_blocks, num_dma_blocks, flags, NULL, import_ion_final_release_callback, ion_info);

	vfree(phys_blocks);

	if (ump_handle != UMP_DD_INVALID_MEMORY_HANDLE)
	{
		/*
		 * As we have a final release callback installed
		 * we must keep the module locked until
		 * the callback has been triggered
		 * */
		__module_get(THIS_MODULE);
		return ump_handle;
	}

	/* failed*/
vmalloc_failed:
	dma_unmap_sg(NULL, ion_info->sglist, ion_info->num_phys_blocks, DMA_BIDIRECTIONAL);
linux_dma_map_failed:
	ion_unmap_dma(ion_info->ion_client, ion_info->ion_handle);
ion_dma_map_failed:
	ion_free(ion_info->ion_client, ion_info->ion_handle);
out:
	kfree(ion_info);
	return UMP_DD_INVALID_MEMORY_HANDLE;
}

struct ump_import_handler import_handler_ion =
{
	.linux_module =  THIS_MODULE,
	.session_begin = import_ion_client_create,
	.session_end =   import_ion_client_destroy,
	.import =        import_ion_import
};

/* MSTAR patch begin */
#if MSTAR_UMP_MONOLITHIC
int __init import_ion_initialize_module(void)
#else
static int __init import_ion_initialize_module(void)
#endif
/* MSTAR patch end */
{
	/* register with UMP */
	return ump_import_module_register(UMP_EXTERNAL_MEM_TYPE_ION, &import_handler_ion);
}

/* MSTAR patch begin */
#if MSTAR_UMP_MONOLITHIC
void __exit import_ion_cleanup_module(void)
#else
static void __exit import_ion_cleanup_module(void)
#endif
/* MSTAR patch end */
{
	/* unregister import handler */
	ump_import_module_unregister(UMP_EXTERNAL_MEM_TYPE_ION);
}

/* Setup init and exit functions for this module */
/* MSTAR patch begin */
#if !MSTAR_UMP_MONOLITHIC
module_init(import_ion_initialize_module);
module_exit(import_ion_cleanup_module);
#endif
/* MSTAR patch end */

/* And some module information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ARM Ltd.");
MODULE_VERSION("1.0");
