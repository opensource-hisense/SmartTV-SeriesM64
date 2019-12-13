/*------------------------------------------------------------------------------
	Copyright (c) 2008 MStar Semiconductor, Inc.  All rights reserved.
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
    PROJECT: CHIP

	FILE NAME: include/asm-arm/arch-CHIP/io.h

    DESCRIPTION:
          Head file of IO table definition

    HISTORY:
         <Date>     <Author>    <Modification Description>
        2008/07/18  Fred Cheng  Modify to add ITCM and DTCM'd IO tables

------------------------------------------------------------------------------*/

#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

/*------------------------------------------------------------------------------
    Constant
-------------------------------------------------------------------------------*/
/* max of IO Space */
#define IO_SPACE_LIMIT 0xffffffff

/* Constants of AGATE RIU */
#define IO_VIRT         0xfd000000  // Define VM start address
#define IO_PHYS         0x1f000000  // Define bus start address
#define IO_OFFSET       ((IO_VIRT) - (IO_PHYS))
#ifndef CONFIG_OC_ETM
#define IO_SIZE         0x00400000
#else
#define IO_SIZE         0x00f20000
#endif

#define io_p2v(pa)      ((pa) + IO_OFFSET)
#define io_v2p(va)      ((va) - IO_OFFSET)
#define IO_ADDRESS(x)   io_p2v(x)

/* Constants of AGATE periphral  */
#define PERI_VIRT         ((IO_VIRT) + (IO_SIZE))
#define PERI_PHYS         0x16000000
#define PERI_SIZE         0x8000
#define PERI_OFFSET       ((PERI_VIRT) - (PERI_PHYS))
#define peri_p2v(pa)      ((pa) + PERI_OFFSET)
#define peri_v2p(va)      ((va) - PERI_OFFSET)
#define PERI_ADDRESS(x)   peri_p2v(x)

/******************************************************
       CA9 GIC 
*******************************************************/

#define GIC_DIST_SET_EANBLE	0x1100 + PT_BASE


/******************************************************
       CA9 Private Timer
*******************************************************/
#define FLAG_EVENT		(0x00000001)
#define FLAG_IT_ENABLE		(0x00000004)
#define FLAG_AUTO_RELOAD	(0x00000002)
#define FLAG_TIMER_ENABLE	(0x00000001)
#define FLAG_TIMER_PRESCALAR    (0x00000000)

#define PT_BASE                PERI_PHYS
#define PT_STATUS		(0x60c + PT_BASE)
#define PT_CONTROL		(0x608 + PT_BASE)
#define PT_COUNTER		(0x604 + PT_BASE)
#define PT_LOADER		(0x600 + PT_BASE)

#define GT_STATUS		(0x20c + PT_BASE)
#define GT_CONTROL		(0x208 + PT_BASE)
#define GT_LOADER_UP	(0x204 + PT_BASE)
#define GT_LOADER_LOW	(0x200 + PT_BASE)

#define PERI_W(a,v)		(*(volatile unsigned int *)PERI_ADDRESS(a)=v)
#define PERI_R(a)		(*(volatile unsigned int *)PERI_ADDRESS(a))

/* Constants of CHIP L2_CACHE  */
#define L2_CACHE_VIRT         ((PERI_VIRT) + (PERI_SIZE))
#define L2_CACHE_PHYS         0x15000000
#define L2_CACHE_SIZE         0x1000
#define L2_CACHE_OFFSET       ((L2_CACHE_VIRT) - (L2_CACHE_PHYS))
#define L2_CACHE_p2v(pa)      ((pa) + L2_CACHE_OFFSET)
#define L2_CACHE_v2p(va)      ((va) - L2_CACHE_OFFSET)
#define L2_CACHE_ADDRESS(x)   L2_CACHE_p2v(x)

/* Constants of CHIP L2 Linefill & Prefetch */
#define L2_LINEFILL             1
#define L2_PREFETCH             1
#define PREFETCH_CTL_REG        0xF60

#define DOUBLE_LINEFILL_ENABLE  0x40000000  //[30]
#define I_PREFETCH_ENABLE       0x20000000  //[29]     
#define D_PREFETCH_ENABLE       0x10000000  //[28]
#define LINEFILL_WRAP_DISABLE   0x08000000  //[27]
#define PREFETCH_OFFSET         0x00000000  //[4:0] ,only support 0-7,15,23,31 

#define L2_CACHE_write(v,a)         (*(volatile unsigned int *)L2_CACHE_ADDRESS(a) = (v))
#define L2_CACHE_read(a)            (*(volatile unsigned int *)L2_CACHE_ADDRESS(a))

/*------------------------------------------------------------------------------
    Macro
-------------------------------------------------------------------------------*/
/* macro for __iomem */
static inline void __iomem *__io(unsigned long addr)
{
	return (void __iomem *)addr;
}
#define __io(a)	__io(a)
#define __mem_pci(a) (a)

/* read register by byte */
#define reg_readb(a) (*(volatile unsigned char *)IO_ADDRESS(a))

/* read register by word */
#define reg_readw(a) (*(volatile unsigned short *)IO_ADDRESS(a))

/* read register by long */
#define reg_readl(a) (*(volatile unsigned int *)IO_ADDRESS(a))

/* write register by byte */
#define reg_writeb(v,a) (*(volatile unsigned char *)IO_ADDRESS(a) = (v))

/* write register by word */
#define reg_writew(v,a) (*(volatile unsigned short *)IO_ADDRESS(a) = (v))

/* write register by long */
#define reg_writel(v,a) (*(volatile unsigned int *)IO_ADDRESS(a) = (v))


//------------------------------------------------------------------------------
//
//  Macros:  INREGx/OUTREGx/SETREGx/CLRREGx
//
//  This macros encapsulates basic I/O operations.
//  Memory address space operation is used on all platforms.
//
#define INREG8(x)           reg_readb(x)
#define OUTREG8(x, y)       reg_writeb((u8)(y), x)
#define SETREG8(x, y)       OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)       OUTREG8(x, INREG8(x)&~(y))

#define INREG16(x)          reg_readw(x)
#define OUTREG16(x, y)      reg_writew((u16)(y), x)
#define SETREG16(x, y)      OUTREG16(x, INREG16(x)|(y))
#define CLRREG16(x, y)      OUTREG16(x, INREG16(x)&~(y))

#define INREG32(x)          reg_readl(x)
#define OUTREG32(x, y)      reg_writel((u32)(y), x)
#define SETREG32(x, y)      OUTREG32(x, INREG32(x)|(y))
#define CLRREG32(x, y)      OUTREG32(x, INREG32(x)&~(y))

#define BIT_0                   (0x01 << 0)
#define BIT_1                   (0x01 << 1)
#define BIT_2                   (0x01 << 2)
#define BIT_3                   (0x01 << 3)
#define BIT_4                   (0x01 << 4)
#define BIT_5                   (0x01 << 5)
#define BIT_6                   (0x01 << 6)
#define BIT_7                   (0x01 << 7)
#define BIT_8                   (0x01 << 8)
#define BIT_9                   (0x01 << 9)
#define BIT_10                  (0x01 << 10)
#define BIT_11                  (0x01 << 11)
#define BIT_12                  (0x01 << 12)
#define BIT_13                  (0x01 << 13)
#define BIT_14                  (0x01 << 14)
#define BIT_15                  (0x01 << 15)


//------------------------------------------------------------------------------
//
//  Function: read_chip_revision
//
//  This inline function returns the chip revision (for drivers)
//
static inline u32 read_chip_revision(void)
{
	u32 u32_rev = *((volatile u32*)(0xA0003C00+(0x67<<2)));
    return ((u32_rev & 0xff00)>>8);
}




#endif /* __ASM_ARM_ARCH_IO_H */

