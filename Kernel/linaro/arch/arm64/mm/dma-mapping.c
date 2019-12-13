/*
 * SWIOTLB-based DMA API implementation
 *
 * Copyright (C) 2012 ARM Ltd.
 * Author: Catalin Marinas <catalin.marinas@arm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/gfp.h>
#include <linux/export.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/vmalloc.h>
#include <linux/swiotlb.h>

#include <asm/cacheflush.h>
#include "chip_setup.h"

struct dma_map_ops *dma_ops;
EXPORT_SYMBOL(dma_ops);

static pgprot_t __get_dma_pgprot(struct dma_attrs *attrs, pgprot_t prot,bool coherent)
{
	if (!coherent || dma_get_attr(DMA_ATTR_WRITE_COMBINE, attrs))
		return pgprot_writecombine(prot);
	return prot;
}



//static void *arm64_swiotlb_alloc_coherent(struct device *dev, size_t size,
void *arm64_swiotlb_alloc_coherent(struct device *dev, size_t size,
					  dma_addr_t *dma_handle, gfp_t flags,
					  struct dma_attrs *attrs)
{
	/*if (IS_ENABLED(CONFIG_ZONE_DMA32) &&
	    dev->coherent_dma_mask <= DMA_BIT_MASK(32))
		flags |= GFP_DMA32;
	return swiotlb_alloc_coherent(dev, size, dma_handle, flags);*/
	struct page *page, **map;
	void *ptr, *coherent_ptr;
	int order, i;

	size = PAGE_ALIGN(size);
	order = get_order(size);

	ptr = swiotlb_alloc_coherent(dev, size, dma_handle, flags);
	if (!ptr)
		goto no_mem;
	map = kmalloc(sizeof(struct page *) << order, flags & ~GFP_DMA);
	if (!map)
		goto no_map;

	/* remove any dirty cache lines on the kernel alias */
	__dma_flush_range(ptr, ptr + size);

	/* create a coherent mapping */
	page = virt_to_page(ptr);
	for (i = 0; i < (size >> PAGE_SHIFT); i++)
		map[i] = page + i;
	coherent_ptr = vmap(map, size >> PAGE_SHIFT, VM_MAP,__pgprot(PROT_DEVICE_nGnRnE));
	kfree(map);
	if (!coherent_ptr)
        goto no_map;

	return coherent_ptr;

no_map:
	swiotlb_free_coherent(dev, size, ptr, *dma_handle);
no_mem:
	*dma_handle = ~0;
	return NULL;
}

static void arm64_swiotlb_free_coherent(struct device *dev, size_t size,
					void *vaddr, dma_addr_t dma_handle,
					struct dma_attrs *attrs)
{
	//swiotlb_free_coherent(dev, size, vaddr, dma_handle);
	void *swiotlb_addr = phys_to_virt(dma_to_phys(dev, dma_handle));
	vunmap(vaddr);
	swiotlb_free_coherent(dev, size, swiotlb_addr, dma_handle);
}

static dma_addr_t __swiotlb_map_page(struct device *dev, struct page *page,
										unsigned long offset, size_t size,
					 					enum dma_data_direction dir,
										struct dma_attrs *attrs)
{
	dma_addr_t dev_addr;

	dev_addr = swiotlb_map_page(dev, page, offset, size, dir, attrs);
	__dma_map_area(phys_to_virt(dma_to_phys(dev, dev_addr)), size, dir);
        _chip_flush_miu_pipe();

	return dev_addr;
}

static void __swiotlb_unmap_page(struct device *dev, dma_addr_t dev_addr,
									size_t size, enum dma_data_direction dir,
				 					struct dma_attrs *attrs)
{
	__dma_unmap_area(phys_to_virt(dma_to_phys(dev, dev_addr)), size, dir);
	swiotlb_unmap_page(dev, dev_addr, size, dir, attrs);
}

static int __swiotlb_map_sg_attrs(struct device *dev, struct scatterlist *sgl,
									int nelems, enum dma_data_direction dir,
									struct dma_attrs *attrs)
{
	struct scatterlist *sg;
	int i, ret;

	ret = swiotlb_map_sg_attrs(dev, sgl, nelems, dir, attrs);
	for_each_sg(sgl, sg, ret, i)
		__dma_map_area(phys_to_virt(dma_to_phys(dev, sg->dma_address)),
						sg->length, dir);

        _chip_flush_miu_pipe();
	return ret;
}

static void __swiotlb_unmap_sg_attrs(struct device *dev,
										struct scatterlist *sgl, int nelems,
										enum dma_data_direction dir,
										struct dma_attrs *attrs)
{
	struct scatterlist *sg;
	int i;

	for_each_sg(sgl, sg, nelems, i)
		__dma_unmap_area(phys_to_virt(dma_to_phys(dev, sg->dma_address)),
							sg->length, dir);
	swiotlb_unmap_sg_attrs(dev, sgl, nelems, dir, attrs);
}

static void __swiotlb_sync_single_for_cpu(struct device *dev,
											dma_addr_t dev_addr, size_t size,
											enum dma_data_direction dir)
{
	__dma_unmap_area(phys_to_virt(dma_to_phys(dev, dev_addr)), size, dir);
	swiotlb_sync_single_for_cpu(dev, dev_addr, size, dir);
}

static void __swiotlb_sync_single_for_device(struct device *dev,
												dma_addr_t dev_addr, size_t size,
												enum dma_data_direction dir)
{
	swiotlb_sync_single_for_device(dev, dev_addr, size, dir);
	__dma_map_area(phys_to_virt(dma_to_phys(dev, dev_addr)), size, dir);
        _chip_flush_miu_pipe();
}

static void __swiotlb_sync_sg_for_cpu(struct device *dev,
										struct scatterlist *sgl, int nelems,
										enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	for_each_sg(sgl, sg, nelems, i)
		__dma_unmap_area(phys_to_virt(dma_to_phys(dev, sg->dma_address)),
							sg->length, dir);
	swiotlb_sync_sg_for_cpu(dev, sgl, nelems, dir);
}

static void __swiotlb_sync_sg_for_device(struct device *dev,
											struct scatterlist *sgl, int nelems,
											enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	swiotlb_sync_sg_for_device(dev, sgl, nelems, dir);
	for_each_sg(sgl, sg, nelems, i)
		__dma_map_area(phys_to_virt(dma_to_phys(dev, sg->dma_address)),
						sg->length, dir);
        _chip_flush_miu_pipe();
}

/* vma->vm_page_prot must be set appropriately before calling this function */
static int __dma_common_mmap(struct device *dev, struct vm_area_struct *vma,
								void *cpu_addr, dma_addr_t dma_addr, size_t size)
{
	int ret = -ENXIO;
	unsigned long nr_vma_pages = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
	unsigned long nr_pages = PAGE_ALIGN(size) >> PAGE_SHIFT;
	unsigned long pfn = dma_to_phys(dev, dma_addr) >> PAGE_SHIFT;
	unsigned long off = vma->vm_pgoff;

	if (dma_mmap_from_coherent(dev, vma, cpu_addr, size, &ret))
		return ret;

	if (off < nr_pages && nr_vma_pages <= (nr_pages - off)) {
		ret = remap_pfn_range(vma, vma->vm_start,
								pfn + off,
								vma->vm_end - vma->vm_start,
								vma->vm_page_prot);
	}

	return ret;
}

static int __swiotlb_mmap_noncoherent(struct device *dev,
										struct vm_area_struct *vma,
										void *cpu_addr, dma_addr_t dma_addr, size_t size,
										struct dma_attrs *attrs)
{
	vma->vm_page_prot = __get_dma_pgprot(attrs, vma->vm_page_prot, false);
	return __dma_common_mmap(dev, vma, cpu_addr, dma_addr, size);
}

static int __swiotlb_mmap_coherent(struct device *dev,
										struct vm_area_struct *vma,
										void *cpu_addr, dma_addr_t dma_addr, size_t size,
										struct dma_attrs *attrs)
{
	/* Just use whatever page_prot attributes were specified */
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	return __dma_common_mmap(dev, vma, cpu_addr, dma_addr, size);
}

static struct dma_map_ops arm64_swiotlb_dma_ops = {
	.alloc = arm64_swiotlb_alloc_coherent,
	.free = arm64_swiotlb_free_coherent,
	.mmap = __swiotlb_mmap_coherent,
	.map_page = __swiotlb_map_page,
	.unmap_page = __swiotlb_unmap_page,
	.map_sg = __swiotlb_map_sg_attrs,
	.unmap_sg = __swiotlb_unmap_sg_attrs,
	.sync_single_for_cpu = __swiotlb_sync_single_for_cpu,
	.sync_single_for_device = __swiotlb_sync_single_for_device,
	.sync_sg_for_cpu = __swiotlb_sync_sg_for_cpu,
	.sync_sg_for_device = __swiotlb_sync_sg_for_device,
	.dma_supported = swiotlb_dma_supported,
	.mapping_error = swiotlb_dma_mapping_error,
};

void __init arm64_swiotlb_init(void)
{
	dma_ops = &arm64_swiotlb_dma_ops;
	swiotlb_init(1);
}

#define PREALLOC_DMA_DEBUG_ENTRIES	4096

static int __init dma_debug_do_init(void)
{
	dma_debug_init(PREALLOC_DMA_DEBUG_ENTRIES);
	return 0;
}
fs_initcall(dma_debug_do_init);
