/*
 *  arch/arm/mach-vexpress/include/mach/memory.h
 *
 *  Copyright (C) 2003 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * The macros below define a section size of 256MB and a non-linear virtual to
 * physical mapping:
 *
 * 512MB @ 0x40000000 -> PAGE_OFFSET
 * 256MB @ 0xA0000000 -> PAGE_OFFSET + 0x20000000
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#define PHYS_OFFSET		UL(CONFIG_MEMORY_START_ADDRESS)

#ifdef CONFIG_SPARSEMEM

#ifndef __ASSEMBLY__
extern unsigned long lx_mem_addr;
extern unsigned long lx_mem_size;
extern unsigned long lx_mem2_addr;
extern unsigned long lx_mem2_size;
extern unsigned long lx_mem3_addr;
extern unsigned long lx_mem_left_size;
extern unsigned long lx_mem2_left_size;
extern unsigned long vmalloc_end_check;
extern unsigned long miu01_adj;
#endif

#define VMALLOC_END_CHECK vmalloc_end_check

#define MAX_PHYSMEM_BITS        32
#define SECTION_SIZE_BITS       29

/* bank page offsets */
#define PAGE_OFFSET1    (PAGE_OFFSET + lx_mem_size)
#define PAGE_OFFSET1_LEFT    (PAGE_OFFSET + lx_mem_left_size)

/* page offset of LX_MEM3*/
#define PAGE_OFFSET2    (PAGE_OFFSET1 + lx_mem2_size)
#define PAGE_OFFSET2_LEFT    (PAGE_OFFSET1_LEFT + lx_mem2_left_size)

#define __phys_to_virt(phys)                                            \
        ((phys) >= lx_mem2_addr ? ( (phys) >= lx_mem3_addr ?( lx_mem3_addr > ( lx_mem2_addr + lx_mem2_size)?(phys) - lx_mem3_addr + PAGE_OFFSET2 : (phys) - lx_mem2_addr + PAGE_OFFSET1 ): ((phys) - lx_mem2_addr + PAGE_OFFSET1) ):    \
         (phys) - PHYS_OFFSET + PAGE_OFFSET)

/* this only convert lowmem (virt <= lowmem_limit) && (virt <= vmalloc_min) */
#define __virt_to_phys(virt)                                            \
         ( (virt) > VMALLOC_END_CHECK ? 0 : \
		 ((virt) >= PAGE_OFFSET1_LEFT ?  ( (virt) >= PAGE_OFFSET2_LEFT? ((virt) - PAGE_OFFSET2_LEFT + lx_mem3_addr) : ((virt) - PAGE_OFFSET1_LEFT + lx_mem2_addr) ): \
          (virt) - PAGE_OFFSET + PHYS_OFFSET ) )


#endif
#endif
