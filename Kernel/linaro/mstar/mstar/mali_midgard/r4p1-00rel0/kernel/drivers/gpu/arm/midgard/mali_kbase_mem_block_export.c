/*
 * MStar Software
 *
 * Copyright (c) 2014 MStar Semiconductor, Inc. All rights reserved.
 *
 * All software, firmware and related documentation herein ("MStar Software")
 * are intellectual property of MStar Semiconductor, Inc. ("MStar") and
 * protected by law, including, but not limited to, copyright law and
 * international treaties. Any use, modification, reproduction, retransmission,
 * or republication of all or part of MStar Software is expressly prohibited,
 * unless prior written permission has been granted by MStar.
 *
 * By accessing, browsing and/or using MStar Software, you acknowledge that you
 * have read, understood, and agree, to be bound by below terms ("Terms") and to
 * comply with all applicable laws and regulations:
 *
 * 1. MStar shall retain any and all right, ownership and interest to MStar
 *    Software and any modification/derivatives thereof. No right, ownership, or
 *    interest to MStar Software and any modification/derivatives thereof is
 *    transferred to you under Terms.
 *
 * 2. You understand that MStar Software might include, incorporate or be
 *    supplied together with third party's software and the use of MStar
 *    Software may require additional licenses from third parties. Therefore,
 *    you hereby agree it is your sole responsibility to separately obtain any
 *    and all third party right and license necessary for your use of such third
 *    party's software.
 *
 * 3. MStar Software and any modification/derivatives thereof shall be deemed as
 *    MStar's confidential information and you agree to keep MStar's
 *    confidential information in strictest confidence and not disclose to any
 *    third party.
 *
 * 4. MStar Software is provided on an "AS IS" basis without warranties of any
 *    kind. Any warranties are hereby expressly disclaimed by MStar, including
 *    without limitation, any warranties of merchantability, non-infringement of
 *    intellectual property rights, fitness for a particular purpose, error free
 *    and in conformity with any international standard. You agree to waive any
 *    claim against MStar for any loss, damage, cost or expense that you may
 *    incur related to your use of MStar Software. In no event shall MStar be
 *    liable for any direct, indirect, incidental or consequential damages,
 *    including without limitation, lost of profit or revenues, lost or damage
 *    of data, and unauthorized system use. You agree that this Section 4 shall
 *    still apply without being affected even if MStar Software has been
 *    modified by MStar in accordance with your request or instruction for your
 *    use, except otherwise agreed by both parties in writing.
 *
 * 5. If requested, MStar may from time to time provide technical supports or
 *    services in relation with MStar Software to you for your use of MStar
 *    Software in conjunction with your or your customer's product ("Services").
 *    You understand and agree that, except otherwise agreed by both parties in
 *    writing, Services are provided on an "AS IS" basis and the warranty
 *    disclaimer set forth in Section 4 above shall apply.
 *
 * 6. Nothing contained herein shall be construed as by implication, estoppels
 *    or otherwise: (a) conferring any license or right to use MStar name,
 *    trademark, service mark, symbol or any other identification;
 *    (b) obligating MStar or any of its affiliates to furnish any person,
 *    including without limitation, you and your customers, any assistance of
 *    any kind whatsoever, or any information; or (c) conferring any license or
 *    right under any intellectual property right.
 *
 * 7. These terms shall be governed by and construed in accordance with the laws
 *    of Taiwan, R.O.C., excluding its conflict of law rules. Any and all
 *    dispute arising out hereof or related hereto shall be finally settled by
 *    arbitration referred to the Chinese Arbitration Association, Taipei in
 *    accordance with the ROC Arbitration Law and the Arbitration Rules of the
 *    Association by three (3) arbitrators appointed in accordance with the said
 *    Rules. The place of arbitration shall be in Taipei, Taiwan and the
 *    language shall be English. The arbitration award shall be final and
 *    binding to both parties.
 */


/**
 * @file mali_kbase_mem_block_export.c
 *  Memory Pool Control Interface
 */

#include <linux/dma-buf.h>
#include <linux/highmem.h>
#include <linux/slab.h>

#include "mali_kbase_mem_block_export.h"

#include "mstar/mstar_chip.h"
#define PHYS_TO_BUS_ADDRESS_ADJUST  MSTAR_MIU0_BUS_BASE

struct mpool_mem_block_pdata {
    phys_addr_t base;
};

static struct sg_table *mpool_mem_block_map(struct dma_buf_attachment *attach,
        enum dma_data_direction direction)
{
    struct mpool_mem_block_pdata *pdata = attach->dmabuf->priv;
    unsigned long pfn = PFN_DOWN(pdata->base);
    struct page *page = pfn_to_page(pfn);
    struct sg_table *table;
    int ret;

    table = kzalloc(sizeof(*table), GFP_KERNEL);
    if (!table)
        return ERR_PTR(-ENOMEM);

    ret = sg_alloc_table(table, 1, GFP_KERNEL);
    if (ret < 0)
        goto err;

    sg_set_page(table->sgl, page, attach->dmabuf->size, 0);
    /* XXX: in sparse memory model, it's possible that pfn_to_page(page_to_pfn(page)) != page) */
    sg_dma_address(table->sgl) = pdata->base;
    /* sg_dma_address(table->sgl) = sg_phys(table->sgl); */
    return table;

err:
    kfree(table);
    return ERR_PTR(ret);
}

static void mpool_mem_block_unmap(struct dma_buf_attachment *attach,
        struct sg_table *table, enum dma_data_direction direction)
{
    sg_free_table(table);
    kfree(table);
}

static void mpool_mem_block_release(struct dma_buf *buf)
{
    struct mpool_mem_block_pdata *pdata = buf->priv;
    kfree(pdata);
}

static void *mpool_mem_block_do_kmap(struct dma_buf *buf, unsigned long pgoffset,
        bool atomic)
{
    struct mpool_mem_block_pdata *pdata = buf->priv;
    unsigned long pfn = PFN_DOWN(pdata->base) + pgoffset;
    struct page *page = pfn_to_page(pfn);

    if (atomic)
        return kmap_atomic(page);
    else
        return kmap(page);
}

static void *mpool_mem_block_kmap_atomic(struct dma_buf *buf,
        unsigned long pgoffset)
{
    return mpool_mem_block_do_kmap(buf, pgoffset, true);
}

static void mpool_mem_block_kunmap_atomic(struct dma_buf *buf,
        unsigned long pgoffset, void *vaddr)
{
    kunmap_atomic(vaddr);
}

static void *mpool_mem_block_kmap(struct dma_buf *buf, unsigned long pgoffset)
{
    return mpool_mem_block_do_kmap(buf, pgoffset, false);
}

static void mpool_mem_block_kunmap(struct dma_buf *buf, unsigned long pgoffset,
        void *vaddr)
{
    kunmap(vaddr);
}

static int mpool_mem_block_mmap(struct dma_buf *buf, struct vm_area_struct *vma)
{
    struct mpool_mem_block_pdata *pdata = buf->priv;

    return remap_pfn_range(vma, vma->vm_start, PFN_DOWN(pdata->base),
            vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

static struct dma_buf_ops mpool_mem_block_ops = {
    .map_dma_buf = mpool_mem_block_map,
    .unmap_dma_buf = mpool_mem_block_unmap,
    .release = mpool_mem_block_release,
    .kmap_atomic = mpool_mem_block_kmap_atomic,
    .kunmap_atomic = mpool_mem_block_kunmap_atomic,
    .kmap = mpool_mem_block_kmap,
    .kunmap = mpool_mem_block_kunmap,
    .mmap = mpool_mem_block_mmap,
};

int mem_block_export_dma_buf(phys_addr_t base, u32 size, bool is_bus_address)
{
    struct mpool_mem_block_pdata *pdata;
    struct dma_buf *buf;
    int fd = 0;

    if (PAGE_ALIGN(base) != base || PAGE_ALIGN(size) != size)
        return -EINVAL;

    if (!is_bus_address)
        base += PHYS_TO_BUS_ADDRESS_ADJUST;

    pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
    if (!pdata)
        return -ENOMEM;

    pdata->base = base;
    buf = dma_buf_export(pdata, &mpool_mem_block_ops, size, O_RDWR);
    if (IS_ERR(buf))
    {
        kfree(pdata);
        return PTR_ERR(buf);
    }

    fd = dma_buf_fd(buf, O_CLOEXEC);
    if (fd < 0)
        dma_buf_put(buf);

    return fd;
}
