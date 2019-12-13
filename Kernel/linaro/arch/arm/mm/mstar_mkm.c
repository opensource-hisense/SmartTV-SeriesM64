#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/swap.h>
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/mman.h>
#include <linux/export.h>
#include <linux/nodemask.h>
#include <linux/initrd.h>
#include <linux/of_fdt.h>
#include <linux/highmem.h>
#include <linux/gfp.h>
#include <linux/memblock.h>
#include <linux/dma-contiguous.h>
#include <linux/sizes.h>
#include <linux/proc_fs.h>
#include <linux/scatterlist.h>

#include <asm/mach-types.h>
#include <asm/memblock.h>
#include <asm/prom.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/tlb.h>
#include <asm/fixmap.h>

#include "mm.h"

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <mstar/mpatch_macro.h>

struct mkpm_area {
	unsigned int start;
	unsigned int end;
};
static struct mkpm_area mkpm_areas[MAX_CMA_AREAS];
static int mkpm_area_num=0;

void mkpm_area_add(phys_addr_t start, unsigned int size)
{
    phys_addr_t end=start+size;
    if(end>arm_lowmem_limit)
        end=arm_lowmem_limit;
    if(start>=end)return;
    mkpm_areas[mkpm_area_num].start=((unsigned int)__va(start));
    mkpm_areas[mkpm_area_num].end=(unsigned int)__va(end);
    mkpm_area_num++;
}

int _modify_kpage_mapping(struct page *pg,int memtype)
{
    unsigned int addr,paddr;
    const struct mem_type *type=get_mem_type(memtype);
    pmd_t *pmd;
    pte_t *pte;
    int ret=0,i;
    if(PageHighMem(pg))
        return -EINVAL;
    paddr=page_to_phys(pg);
    addr=(unsigned int)phys_to_virt(paddr);
    for(i=0;i<mkpm_area_num;i++){
        if(addr>=mkpm_areas[i].start
           && (addr+PAGE_SIZE)<=mkpm_areas[i].end)
            break;
    }
    if(i>=mkpm_area_num)
        return -EINVAL;
    clean_dcache_area((void*)addr,PAGE_SIZE);
    spin_lock(&init_mm.page_table_lock);
    pmd=pmd_off_k(addr);
    if(pmd_none(*pmd) || pmd_bad(*pmd)){
        ret=-EINVAL;
        goto ret_unlock;
    }
    pte=pte_offset_kernel(pmd,addr);
    if(pte_none(*pte)){
        ret=-EINVAL;
        goto ret_unlock;
    }
    set_pte_ext(pte, pfn_pte(__phys_to_pfn(paddr), __pgprot(type->prot_pte)), 0);
    clean_dcache_area(pte,PAGE_SIZE-(((unsigned long)pte)&(PAGE_SIZE-1)));
    flush_tlb_kernel_page(addr);
ret_unlock:
    spin_unlock(&init_mm.page_table_lock);
    return ret;
}

int modify_kpage_mapping_sg(struct sg_table *table,unsigned int type)
{
    struct scatterlist *sg;
	int i,j;
    for_each_sg(table->sgl, sg, table->nents, i) {
	    struct page *page = sg_page(sg);
        int cnt = (sg->offset+sg->length+PAGE_SIZE-1)/PAGE_SIZE;
        for(j=0;j<cnt;j++){
            _modify_kpage_mapping(page+j, type);
        }
    }
    return 0;
}
EXPORT_SYMBOL(modify_kpage_mapping_sg);
int modify_kpage_mapping_conti(struct page *page, int cnt,unsigned int type)
{
	int j;
    for(j=0;j<cnt;j++){
        _modify_kpage_mapping(page+j, type);
    }
    return 0;
}
EXPORT_SYMBOL(modify_kpage_mapping_conti);

