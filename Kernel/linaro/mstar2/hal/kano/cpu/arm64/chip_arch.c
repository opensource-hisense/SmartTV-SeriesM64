/*------------------------------------------------------------------------------
	Copyright (c) 2008 MStar Semiconductor, Inc.  All rights reserved.
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
    PROJECT: chip

	FILE NAME: arch/arm/mach-chip/chip_arch.c

    DESCRIPTION:
          Power Management Driver

    HISTORY:
         <Date>     <Author>    <Modification Description>
        2008/07/18  Fred Cheng  Add IO tables for ITCM and DTCM
        2008/07/22  Evan Chang  Add SD card init

------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Include Files
------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <mach/hardware.h>
#include <asm/memory.h>
#include <mach/io.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <asm/irq.h>
#include <chip_int.h>
#include <linux/version.h>
#include <linux/dma-mapping.h>
#include <mstar/mstar_chip.h>
#include <asm/cacheflush.h>
#ifdef CONFIG_WRITE_TO_ANDROID_MISC
#include <linux/reboot.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#endif

#ifdef CONFIG_WRITE_TO_ANDROID_MISC_NAND
#include <linux/err.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stringify.h>
#include <linux/namei.h>
#include <linux/stat.h>
#include <linux/miscdevice.h>
#include <linux/log2.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#endif

void __init serial_init(void)
{
#ifdef CONFIG_SERIAL_8250
    struct uart_port s, u, k, w;

    memset(&s, 0, sizeof(s));
    s.type = PORT_16550;
    s.iobase = mstar_pm_base + (0x100980UL << 1); 
    s.irq = E_IRQ_UART0;
#ifdef CONFIG_MSTAR_ARM_BD_FPGA
    s.uartclk = 12000000; //FPGA
#elif defined(CONFIG_MSTAR_ARM_BD_GENERIC)
    s.uartclk = 123000000; //real chip
#endif
    s.iotype = 0;
    s.regshift = 0;
    s.fifosize = 16 ; // use the 8 byte depth FIFO well
    s.line = 0;
    

    if (early_serial_setup(&s) != 0) {
        printk(KERN_ERR "Serial(0) setup failed!\n");
    }

    memset(&u, 0, sizeof(u));
    u.type = PORT_16550;
    u.iobase = mstar_pm_base + (0x110600UL << 1); 
    u.irq = E_IRQEXPL_UART1;
    u.uartclk = 108000000;
    u.iotype = 0;
    u.regshift = 0;
    u.fifosize = 16 ; // use the 8 byte depth FIFO well
    u.line = 1;
    if (early_serial_setup(&u) != 0) {
    	printk(KERN_ERR "Serial piu_uart1 setup failed!\n");
    }

    memset(&k, 0, sizeof(k));
    k.type = PORT_16550;
    k.iobase = mstar_pm_base + (0x110680UL << 1);
    k.irq = E_IRQEXPH_UART2MCU;
    k.uartclk = 123000000;
    k.iotype = 0;
    k.regshift = 0;
    k.fifosize = 16 ; // use the 8 byte depth FIFO well
    k.line = 2;
    if (early_serial_setup(&k) != 0) {
    	printk(KERN_ERR "Serial fuart setup failed!\n");
    }
    
    memset(&w, 0, sizeof(w));
    w.type = PORT_16550;
    w.iobase = 0xFD220C80;
    w.irq = E_IRQEXPL_UART2;
    w.uartclk = 123000000;
    w.iotype = 0;
    w.regshift = 0;
    w.fifosize = 16 ; // use the 8 byte depth FIFO well
    w.line = 3;

    if (early_serial_setup(&w) != 0) {
      printk(KERN_ERR "Serial fuart setup failed!\n");
    }
#endif
}

unsigned int * uncached_buf = NULL;
static int          dummy_read;
dma_addr_t   buf_phy_addr;

/*************************************
*		Mstar chip flush function
*************************************/
#define _BIT(x)                      (1<<(x))
static DEFINE_SPINLOCK(l2prefetch_lock);

inline void _chip_flush_miu_pipe(void)
{
	unsigned long   dwLockFlag = 0;
	unsigned short dwReadData = 0;

	spin_lock_irqsave(&l2prefetch_lock, dwLockFlag);

    //toggle the flush miu pipe fire bit, 0x10188a[0
    *(volatile unsigned short *)(mstar_pm_base + (0x203114)) &= ~(0x0001);
    *(volatile unsigned short *)(mstar_pm_base + (0x203114)) |= 0x0001; 

	// check bits are 0x1018a1[4]~ 0x1018a1[6]
	do
	{
    	dwReadData = *(volatile unsigned short *)(mstar_pm_base + (0x203140)); 
        dwReadData &= (_BIT(12));  //Check Status of Flush Pipe Finish

    } while(dwReadData != (_BIT(12)));	// we need to check 1 bits 0x1018a1[4] miu0 & miu1 all flush done

	spin_unlock_irqrestore(&l2prefetch_lock, dwLockFlag);
}
void Chip_Flush_Miu_Pipe(void)
{
    _chip_flush_miu_pipe();
}
EXPORT_SYMBOL(Chip_Flush_Miu_Pipe);

//add for fixing compile warning
void CPU_Cache_Flush_All(void *p)
{
	__cpuc_flush_kern_all();
}

void Chip_Flush_Cache_All(void)
{
	unsigned long flags;

	local_irq_save(flags);

	//Clean & Inv All L1
	__cpuc_flush_kern_all();

	local_irq_restore(flags);

	smp_call_function(CPU_Cache_Flush_All, NULL, 1);

	local_irq_save(flags);

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Clean&Inv L2 by Way
		outer_cache.flush_all();
	}
#endif
	//Clean L1  & Inv L1
	//dmac_flush_range(u32Addr,u32Addr + u32Size );

#ifndef CONFIG_OUTER_CACHE
	_chip_flush_miu_pipe();
#endif

	local_irq_restore(flags);
}
EXPORT_SYMBOL(Chip_Flush_Cache_All);

void Chip_Flush_Cache_Range(unsigned long u32Addr, unsigned long u32Size)
{
	if(  u32Addr == (unsigned long) NULL )
        {
                printk("u32Addr is invalid\n");
                return;
        }
	//Clean L1 & Inv L1
	dmac_flush_range((const unsigned long)u32Addr,(const unsigned long)(u32Addr + u32Size));

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		if(!virt_addr_valid(u32Addr) || !virt_addr_valid(u32Addr+ u32Size - 1))
			//Clean&Inv L2 by Way
			outer_cache.flush_all();
		else
			//Clean&Inv L2 by Range
			outer_cache.flush_range(__pa(u32Addr) ,__pa(u32Addr)+ u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE //flush miu pipe for L2 disabled case
	_chip_flush_miu_pipe();
#endif
}
EXPORT_SYMBOL(Chip_Flush_Cache_Range);

void Chip_Flush_Cache_Range_VA_PA(unsigned long u32VAddr,unsigned long u32PAddr,unsigned long u32Size)
{
        if(  u32VAddr == (unsigned long) NULL )
        {
                printk("u32VAddr is invalid\n");
                return;
        }
	//Clean & Invalid L1
	dmac_flush_range((const unsigned long)u32VAddr,(const unsigned long)(u32VAddr + u32Size));

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Clean&Inv L2 by range
		outer_cache.flush_range(u32PAddr,u32PAddr + u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE
	_chip_flush_miu_pipe();
#endif
}
EXPORT_SYMBOL(Chip_Flush_Cache_Range_VA_PA);

void Chip_Flush_Memory(void)
{
    #ifdef CONFIG_OUTER_CACHE
	if(outer_cache.sync)
		outer_cache.sync();
	else
	#endif
    {
		extern ptrdiff_t mstar_pm_base;
        //flush store buffer
        *uncached_buf= *(volatile unsigned short *)(mstar_pm_base + (0x101274 << 1));
        smp_mb();
        dummy_read = *uncached_buf;
		_chip_flush_miu_pipe();
	}
}
EXPORT_SYMBOL(Chip_Flush_Memory);

void Chip_Inv_Cache_Range(unsigned long u32Addr, unsigned long u32Size)
{
        if(  u32Addr == (unsigned long) NULL )
        {
                printk("u32Addr is invalid\n");
                return;
        }
#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{

		if(!virt_addr_valid(u32Addr) || !virt_addr_valid(u32Addr+ u32Size - 1))
			printk(KERN_DEBUG "Input VA can't be converted to PA\n");
		else
			//Inv L2 by range
			outer_cache.inv_range(__pa(u32Addr) , __pa(u32Addr) + u32Size);
	}
#endif
	//Inv L1
	dmac_map_area((const void *)u32Addr,(size_t)u32Size,2);
}
EXPORT_SYMBOL(Chip_Inv_Cache_Range);

void Chip_Flush_Cache_All_Single(void)
{
	unsigned long flags;

	local_irq_save(flags);

	//Clean & Inv All L1
	__cpuc_flush_kern_all();

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()&&outer_cache.flush_all) //check if L2 is enabled
	{
		//Clean&Inv L2 by Way
		outer_cache.flush_all();
	}
#endif
	//Clean L1  & Inv L1
	//dmac_flush_range(u32Addr,u32Addr + u32Size );

#ifndef CONFIG_OUTER_CACHE
	_chip_flush_miu_pipe();
#endif

	local_irq_restore(flags);
}
EXPORT_SYMBOL(Chip_Flush_Cache_All_Single);

void Chip_Clean_Cache_Range(unsigned long u32Addr, unsigned long u32Size)
{
        if(  u32Addr == (unsigned long) NULL )
        {
                printk("u32Addr is invalid\n");
                return;
        }
	//Clean L1
	dmac_map_area((const void *)u32Addr,(size_t)u32Size,1);

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		if(!virt_addr_valid(u32Addr) || !virt_addr_valid(u32Addr+ u32Size - 1))
			//Clean L2 by Way
			outer_cache.clean_all();
		else
			//Clean L2 by Range
			outer_cache.clean_range( __pa(u32Addr),__pa(u32Addr) + u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE //flush miu pipe for L2 disabled case
	_chip_flush_miu_pipe();
#endif
}
EXPORT_SYMBOL(Chip_Clean_Cache_Range);

//need to be modified
void Chip_L2_cache_wback_inv( unsigned long addr, unsigned long size)
{
#ifdef CONFIG_OUTER_CACHE //flush miu pipe for L2 disabled case
	outer_cache.flush_all();
#endif
}
EXPORT_SYMBOL(Chip_L2_cache_wback_inv);

//need to be modified
void Chip_L2_cache_wback( unsigned long addr, unsigned long size)
{
	//Flush L2 by Way, change to by Addr later
#ifdef CONFIG_OUTER_CACHE //flush miu pipe for L2 disabled case
	outer_cache.clean_all();
#endif
}
EXPORT_SYMBOL(Chip_L2_cache_wback);

//need to be modified
void Chip_L2_cache_inv( unsigned long addr, unsigned long size)
{
	//Inv L2 by Way, change to by Addr later
#ifdef CONFIG_OUTER_CACHE //flush miu pipe for L2 disabled case
	outer_cache.inv_all();
#endif
}
EXPORT_SYMBOL(Chip_L2_cache_inv);

void Chip_Flush_Memory_Range(unsigned long pAddress , unsigned long  size)
{
    #ifdef CONFIG_OUTER_CACHE
	if(outer_cache.sync)
		outer_cache.sync();
	else
    #endif
    {
        //flush store buffer
        *uncached_buf= *(volatile unsigned short *)(mstar_pm_base + (0x101274 << 1));
        smp_mb();
        dummy_read = *uncached_buf;
		_chip_flush_miu_pipe();
	}
}
EXPORT_SYMBOL(Chip_Flush_Memory_Range);

void Chip_Read_Memory(void)
{
    #ifdef CONFIG_OUTER_CACHE
	if(outer_cache.sync)
		outer_cache.sync();
	else
	#endif
    {
        //flush store buffer
        *uncached_buf= *(volatile unsigned short *)(mstar_pm_base + (0x101274 << 1));
        smp_mb();
        dummy_read = *uncached_buf;
		_chip_flush_miu_pipe();
	}
}
EXPORT_SYMBOL(Chip_Read_Memory);

void Chip_Read_Memory_Range(unsigned long pAddress , unsigned long  size)
{
    #ifdef CONFIG_OUTER_CACHE
	if(outer_cache.sync)
		outer_cache.sync();
	else
	#endif
    {
        //flush store buffer
        *uncached_buf= *(volatile unsigned short *)(mstar_pm_base + (0x101274 << 1));
        smp_mb();
        dummy_read = *uncached_buf;
		_chip_flush_miu_pipe();
	}
}
EXPORT_SYMBOL(Chip_Read_Memory_Range);

void Chip_Clean_Cache_Range_VA_PA(unsigned long u32VAddr,unsigned long u32PAddr,unsigned long u32Size)
{
        if(  u32VAddr == (unsigned long) NULL )
        {
                printk("u32VAddr is invalid\n");
                return;
        }
	//Clean L1
	dmac_map_area((const void *)u32VAddr,(size_t)u32Size,1);

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Clean L2 by Way
		outer_cache.clean_range(u32PAddr,u32PAddr + u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE
	_chip_flush_miu_pipe();
#endif
}
EXPORT_SYMBOL(Chip_Clean_Cache_Range_VA_PA);

void Chip_Inv_Cache_Range_VA_PA(unsigned long u32VAddr,unsigned long u32PAddr,unsigned long u32Size)
{
        if(  u32VAddr == (unsigned long) NULL )
        {
                printk("u32VAddr is invalid\n");
                return;
        }

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Inv L2 by range
		outer_cache.inv_range( u32PAddr ,u32PAddr + u32Size );
	}
#endif
	//Inv L1
	dmac_map_area((const void *)u32VAddr,(size_t)u32Size,2);
}
EXPORT_SYMBOL(Chip_Inv_Cache_Range_VA_PA);
