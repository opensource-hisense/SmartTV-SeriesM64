/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */
// #include <linux/config.h>
#include <linux/autoconf.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/tty.h>

#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,20)
#include <linux/undefconf.h>
#endif

#ifdef CONFIG_MTD
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#endif

#include <asm/cpu.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mips-boards/generic.h>
#include <asm/mips-boards/prom.h>
#include <asm/dma.h>
#include <asm/time.h>
#include <asm/traps.h>

#include "chip_int.h"
#include "hwreg_kris.h"
#ifdef CONFIG_VT
#include <linux/console.h>
#endif

#include "board/Board.h"
#include "chip_setup.h"

#ifndef ENABLE
#define ENABLE                      1
#endif

#ifndef DISABLE
#define DISABLE                     0
#endif

#ifndef BIT0
#define BIT0	                    0x00000001
#endif

#ifndef BIT1
#define BIT1	                    0x00000002
#endif

#ifndef BIT2
#define BIT2	                    0x00000004
#endif

#ifndef BIT3
#define BIT3	                    0x00000008
#endif

#ifndef BIT4
#define BIT4	                    0x00000010
#endif

#ifndef BIT5
#define BIT5	                    0x00000020
#endif

#ifndef BIT6
#define BIT6	                    0x00000040
#endif

#ifndef BIT7
#define BIT7	                    0x00000080
#endif

#ifndef BIT8
#define BIT8	                    0x00000100
#endif

#ifndef BIT9
#define BIT9	                    0x00000200
#endif

#ifndef BIT10
#define BIT10	                    0x00000400
#endif

#ifndef BIT11
#define BIT11	                    0x00000800
#endif

#ifndef BIT12
#define BIT12	                    0x00001000
#endif

#ifndef BIT13
#define BIT13	                    0x00002000
#endif

#ifndef BIT14
#define BIT14	                    0x00004000
#endif

#ifndef BIT15
#define BIT15  	                    0x00008000
#endif

#ifndef BIT16
#define BIT16                       0x00010000
#endif

#ifndef BIT17
#define BIT17                       0x00020000
#endif

#ifndef BIT18
#define BIT18                       0x00040000
#endif

#ifndef BIT19
#define BIT19                       0x00080000
#endif

#ifndef BIT20
#define BIT20                       0x00100000
#endif

#ifndef BIT21
#define BIT21                       0x00200000
#endif

#ifndef BIT22
#define BIT22                       0x00400000
#endif

#ifndef BIT23
#define BIT23                       0x00800000
#endif

#ifndef BIT24
#define BIT24                       0x01000000
#endif

#ifndef BIT25
#define BIT25                       0x02000000
#endif

#ifndef BIT26
#define BIT26                       0x04000000
#endif

#ifndef BIT27
#define BIT27                       0x08000000
#endif

#ifndef BIT28
#define BIT28                       0x10000000
#endif

#ifndef BIT29
#define BIT29                       0x20000000
#endif

#ifndef BIT30
#define BIT30                       0x40000000
#endif

#ifndef BIT31
#define BIT31                       0x80000000
#endif

const char display_string[] = "        LINUX ON MSTAR     ";

#define USE_ASIC_WITH_ICE 				1
#define DISABLE_MIPS_SYNC_OP            1

// This is for check if flush_all, the size_threahold should be L2_cache size
#define MIPS_L2_WB_INV_TH    			(128*1024) //0x100000    // 1MB
#define MIPS_L2_WB_TH        			(128*1024) //0x100000    // 1MB
#define MIPS_L2_INV_TH       			(128*1024) //0x10000000  // 256MB
/******************************************************************************/
/* Macro for bitwise                                                          */
/******************************************************************************/
#define _BITMASK(loc_msb, loc_lsb)  ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))
#define BITMASK(x)                  _BITMASK(1?x, 0?x)
#define _BIT(x)                     (1<<(x))

// UART_SEL_TYPE
#define UART_SEL_HK51_UART0         0x00
#define UART_SEL_HK51_UART1         0x01
#define UART_SEL_VD51_UART0         0x02
#define UART_SEL_AEON               0x03
#define UART_SEL_TSP                0x04
#define UART_SEL_PIU                0x05
#define UART_SEL_PM51_UART0         0x06

#if defined(CONFIG_MSTAR_MIPS_BD_FPGA)
#define CHIP_UART_CLK_FREQ    12000000
#endif

#if defined(CONFIG_MSTAR_MIPS_BD_GENERIC)
#define CHIP_UART_CLK_FREQ    123000000
#endif

//----------------------------------------------------------
//priority setting: 0:highest priority, 3:lowest priority
//----------------------------------------------------------
//MIU group priority
//----------------------------------------------------------
#define  MIU0_GROUP0_PRIORITY       2
#define  MIU0_GROUP1_PRIORITY       0
#define  MIU0_GROUP2_PRIORITY       1
#define  MIU0_GROUP3_PRIORITY       3

#define  MIU1_GROUP0_PRIORITY       3
#define  MIU1_GROUP1_PRIORITY       2
#define  MIU1_GROUP2_PRIORITY       1
#define  MIU1_GROUP3_PRIORITY       0


#define MIU0_GROUP_PRIORITY         ( ( MIU0_GROUP3_PRIORITY << 6 )| (MIU0_GROUP2_PRIORITY<<4)| (MIU0_GROUP1_PRIORITY<<2) | (MIU0_GROUP0_PRIORITY) )
#define MIU1_GROUP_PRIORITY         ( ( MIU1_GROUP3_PRIORITY << 6 )| (MIU1_GROUP2_PRIORITY<<4)| (MIU1_GROUP1_PRIORITY<<2) | (MIU1_GROUP0_PRIORITY) )

////////////////////////////////////////////////////////////////////////////////
// MIU assignment   0: MIU0, 1: MIU1  2:IP_
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------
//
//----------------------------------------
//group_0
#define  RESERVED_for_MIU_SELF      0
#define  VIVALDI9_DSP_DECODER       0
#define  VIVALDI9_DSP_SE            0
#define  MIPS_W                     0
#define  MIPS_R                     0
#define  MAU0_W                     0
#define  MAU1_R                     0
#define  VD_MHEG5_DCACHE_RW         1
#define  VD_MHEG5_QDMA_RW           1
#define  GE_RW                      0
#define  HVD_RW                     1
#define  HVD_BBU_RW                 1
#define  UHC0_RW                    0
#define  UHC1_RW                    0
#define  PIU_DMA_RW                 2
#define  ADCDVIPLL_RW               0    //co_client with MIU

//group_1
#define  VIVALDI9_MAD               0
#define  VIVALDI9_DMA               0
#define  VIVALDI9_AU_DMA            0
#define  MVOP_RW                    1   //mvop_128
#define  GOP_W                      0
#define  GOP0_R                     0
#define  GOP1_R                     0
#define  GOP2_R                     0
#define  GOP3_R                     0
#define  SC_OP_R                    0
#define  SC_IP_MIAN_W               0
#define  SC_IP_MIAN_R               0
#define  VD_COMB_W                  0
#define  VD_COMB_R                  0
#define  VE_W                       0
#define  VE_R                       0

//backward competiable
#define   MIUSEL_FLAG_DNRB_R        SC_IP_MIAN_R
#define   MIUSEL_FLAG_DNRB_RW       (SC_IP_MIAN_R | SC_IP_MIAN_W)


//group_2
#define  OD_W                      0
#define  OD_R                      0
#define  OD_LSB_W                  0
#define  OD_LSB_R                  0
#define  SC_IP_SUB_W               0
#define  SC_IP_SUB_R               0
#define  DISP_IPATH_NR_RW          0
#define  DISP_IPATH_MR_RW          0
#define  DISP_IPATH_DI_W           0
#define  VD_MHEG5_ICACHE           1
#define  TSP_W                     0
#define  TSP_R                     0
#define  VD_TTX_RW                 0
#define  VD_TTXSL_W                0
#define  TSP_ORZ_W                 0
#define  TSP_ORZ_R                 0

//group_3
#define  MVOP_64_R                 1
#define  M4VE_0_RW                 0
#define  M4VE_ME_R                 0
#define  M4VE_2                    0
#define  MVD_RW                    1
#define  MVD_BBU_RW                1
#define  RVD_RW                    1
#define  RVD_BBU_RW                1
#define  JPD_RW                    0
#define  DSCRMB_RW                 0
#define  STRLD_RW                  0
#define  FUART_RW                  0
#define  EMAC_RW                   0
#define  NAND_RW                   0
#define  MPIF_RW                   2   // MIU assignment   0: MIU0, 1: MIU1  2:IP_ASSIGN
#define  RESERVED_G3D_RW           0

#define  ClientGruop0_in_MIU1     ((( VIVALDI9_DSP_DECODER == 1)? 0x0002 : 0  )| \
                                   (( VIVALDI9_DSP_SE      == 1)? 0x0004 : 0  )| \
                                   (( MIPS_W               == 1)? 0x0008 : 0  )| \
                                   (( MIPS_R               == 1)? 0x0010 : 0  )| \
                                   (( MAU0_W               == 1)? 0x0020 : 0  )| \
                                   (( MAU1_R               == 1)? 0x0040 : 0  )| \
                                   (( VD_MHEG5_DCACHE_RW   == 1)? 0x0080 : 0  )| \
                                   (( VD_MHEG5_QDMA_RW     == 1)? 0x0100 : 0  )| \
                                   (( GE_RW                == 1)? 0x0200 : 0  )| \
                                   (( HVD_RW               == 1)? 0x0400 : 0  )| \
                                   (( HVD_BBU_RW           == 1)? 0x0800 : 0  )| \
                                   (( UHC0_RW              == 1)? 0x1000 : 0  )| \
                                   (( UHC1_RW              == 1)? 0x2000 : 0  )| \
                                   (( PIU_DMA_RW           == 1)? 0x4000 : 0  )| \
                                   (( ADCDVIPLL_RW         == 1)? 0x8000 : 0  )  )

#define  ClientGruop1_in_MIU1     (((VIVALDI9_MAD         == 1)? 0x0001 : 0  )| \
                                   ((VIVALDI9_DMA         == 1)? 0x0002 : 0  )| \
                                   ((VIVALDI9_AU_DMA      == 1)? 0x0004 : 0  )| \
                                   ((MVOP_RW              == 1)? 0x0008 : 0  )| \
                                   ((GOP_W                == 1)? 0x0010 : 0  )| \
                                   ((GOP0_R               == 1)? 0x0020 : 0  )| \
                                   ((GOP1_R               == 1)? 0x0040 : 0  )| \
                                   ((GOP2_R               == 1)? 0x0080 : 0  )| \
                                   ((GOP3_R               == 1)? 0x0100 : 0  )| \
                                   ((SC_OP_R              == 1)? 0x0200 : 0  )| \
                                   ((SC_IP_MIAN_W         == 1)? 0x0400 : 0  )| \
                                   ((SC_IP_MIAN_R         == 1)? 0x0800 : 0  )| \
                                   ((VD_COMB_W            == 1)? 0x1000 : 0  )| \
                                   ((VD_COMB_R            == 1)? 0x2000 : 0  )| \
                                   ((VE_W                 == 1)? 0x4000 : 0  )| \
                                   ((VE_R                 == 1)? 0x8000 : 0  )  )


#define  ClientGruop2_in_MIU1     (((OD_W                 == 1)? 0x0001 : 0 )| \
                                   ((OD_R                 == 1)? 0x0002 : 0 )| \
                                   ((OD_LSB_W             == 1)? 0x0004 : 0 )| \
                                   ((OD_LSB_R             == 1)? 0x0008 : 0 )| \
                                   ((SC_IP_SUB_W          == 1)? 0x0010 : 0 )| \
                                   ((SC_IP_SUB_R          == 1)? 0x0020 : 0 )| \
                                   ((DISP_IPATH_NR_RW     == 1)? 0x0040 : 0 )| \
                                   ((DISP_IPATH_MR_RW     == 1)? 0x0080 : 0 )| \
                                   ((DISP_IPATH_DI_W      == 1)? 0x0100 : 0 )| \
                                   ((VD_MHEG5_ICACHE      == 1)? 0x0200 : 0 )| \
                                   ((TSP_W                == 1)? 0x0400 : 0 )| \
                                   ((TSP_R                == 1)? 0x0800 : 0 )| \
                                   ((VD_TTX_RW            == 1)? 0x1000 : 0 )| \
                                   ((VD_TTXSL_W           == 1)? 0x2000 : 0 )| \
                                   ((TSP_ORZ_W            == 1)? 0x4000 : 0 )| \
                                   ((TSP_ORZ_R            == 1)? 0x8000 : 0 ))

#define  ClientGruop3_in_MIU1     (((MVOP_64_R            == 1)? 0x0001 : 0  )| \
                                   ((M4VE_0_RW            == 1)? 0x0002 : 0  )| \
                                   ((M4VE_ME_R            == 1)? 0x0004 : 0  )| \
                                   ((M4VE_2               == 1)? 0x0008 : 0  )| \
                                   ((MVD_RW               == 1)? 0x0010 : 0  )| \
                                   ((MVD_BBU_RW           == 1)? 0x0020 : 0  )| \
                                   ((RVD_RW               == 1)? 0x0040 : 0  )| \
                                   ((RVD_BBU_RW           == 1)? 0x0080 : 0  )| \
                                   ((JPD_RW               == 1)? 0x0100 : 0  )| \
                                   ((DSCRMB_RW            == 1)? 0x0200 : 0  )| \
                                   ((STRLD_RW             == 1)? 0x0400 : 0  )| \
                                   ((FUART_RW             == 1)? 0x0800 : 0  )| \
                                   ((EMAC_RW              == 1)? 0x1000 : 0  )| \
                                   ((NAND_RW              == 1)? 0x2000 : 0  )| \
                                   ((MPIF_RW              == 1)? 0x4000 : 0  )| \
                                   ((RESERVED_G3D_RW      == 1)? 0x8000 : 0  ))
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#define  ClientGruop0_in_IP      (((VIVALDI9_DSP_DECODER == 2)? 0x0002:0 )|\
                                  ((VIVALDI9_DSP_SE      == 2)? 0x0004:0 )| \
                                  ((GE_RW                == 2)? 0x0100:0 )| \
                                  ((UHC0_RW              == 2)? 0x1000:0 )| \
                                  ((UHC1_RW              == 2)? 0x2000:0 )| \
                                  ((PIU_DMA_RW           == 2)? 0x4000:0 ))


#define  ClientGruop1_in_IP     (((VIVALDI9_MAD         == 2)? 0x0001 : 0 )| \
                                 ((VIVALDI9_DMA         == 2)? 0x0002 : 0 )| \
                                 ((VIVALDI9_AU_DMA      == 2)? 0x0004 : 0 )| \
                                 ((SC_OP_R              == 2)? 0x0100 : 0 )| \
                                 ((SC_IP_MIAN_W         == 2)? 0x0200 : 0 )| \
                                 ((SC_IP_MIAN_R         == 2)? 0x0800 : 0 )  )


#define  ClientGruop2_in_IP      ((  SC_IP_SUB_W         == 2)? 0x0010 : 0 | \
                                  (  SC_IP_SUB_R         == 2)? 0x0020 : 0)

#define  ClientGruop3_in_IP      ((  DSCRMB_RW           == 2)? 0x0100 : 0 | \
                                  (  STRLD_RW            == 2)? 0x0200 : 0 | \
                                  (  EMAC_RW             == 2)? 0x1000 : 0 | \
                                  (  NAND_RW             == 2)? 0x2000 : 0 | \
                                  (  MPIF_RW             == 2)? 0x4000 : 0 )
//-----------------------------------------------------------------------------

extern void mips_reboot_setup(void);
extern void mips_time_init(void);
extern void mips_timer_setup(struct irqaction *irq);
extern unsigned long mips_rtc_get_time(void);

static void __init serial_init(void);
static void __init chiptop_init(void);
static void __init pad_config(void);
//static void __init miu_priority_config(void);
//static void __init miu_assignment(void);

//L2 cache
// spinlock for L2/Prefetch buffer
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
static DEFINE_RAW_SPINLOCK(l2prefetch_lock);
#else
spinlock_t l2prefetch_lock = SPIN_LOCK_UNLOCKED;
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,23)
int coherentio = -1;	/* no DMA cache coherency (may be set by user) */
int hw_coherentio;	/* init to 0 => no HW DMA cache coherency (reflects real HW) */
static int __init setcoherentio(char *str)
{
	if (coherentio < 0)
		pr_info("Command line checking done before"
				" plat_setup_iocoherency!!\n");
	if (coherentio == 0)
		pr_info("Command line enabling coherentio"
				" (this will break...)!!\n");

	coherentio = 1;
	pr_info("Hardware DMA cache coherency (command line)\n");
	return 1;
}
__setup("coherentio", setcoherentio);

static int __init setnocoherentio(char *str)
{
	if (coherentio < 0)
		pr_info("Command line checking done before"
				" plat_setup_iocoherency!!\n");
	if (coherentio == 1)
		pr_info("Command line disabling coherentio\n");

	coherentio = 0;
	pr_info("Software DMA cache coherency (command line)\n");
	return 1;
}
__setup("nocoherentio", setnocoherentio);
#endif

const char *get_system_type(void)
{
	return "MStar Chip";
}

extern int MDrv_SYS_GetMMAP(int type, unsigned int *addr, unsigned int *len) ;

static int __init chip_setup(void)
{
    printk("Begin chip_setup\n");
    chiptop_init();
    pad_config();
	serial_init();
	mips_reboot_setup();
	return 0;
}

void __init plat_mem_setup(void)
{
    chip_setup();
}

extern void Uart16550IntrruptEnable(void);
extern void Uart16550Put(unsigned char);
static void __init serial_init(void)
{
#ifdef CONFIG_SERIAL_8250
	struct uart_port s;

	memset(&s, 0, sizeof(s));

    //Uart16550IntrruptEnable();
	//while(1) Uart16550Put('*');
	s.type = PORT_16550;
	s.iobase = (0xBF000000 + (0x100980 << 1));
	// s.irq = E_IRQ_UART;
	s.irq = E_IRQ_UART0;

	s.uartclk   = CHIP_UART_CLK_FREQ;
	s.iotype = 0;
	s.regshift = 0;
	s.fifosize = 16 ; // use the 8 byte depth FIFO well

	if (early_serial_setup(&s) != 0)
    {
		printk(KERN_ERR "Serial setup failed!\n");
	}
#endif

    {
        //    Uart2_early_Init();
        struct uart_port sUartInfo;
        memset(&sUartInfo, 0, sizeof(sUartInfo));
        sUartInfo.type = PORT_16550;
        sUartInfo.iobase = 0xBF220C00;
        sUartInfo.irq = E_IRQEXPL_UART1;
        sUartInfo.uartclk = CHIP_UART_CLK_FREQ;
        sUartInfo.iotype = 0;
        sUartInfo.regshift = 0;
        sUartInfo.fifosize = 16;
        sUartInfo.line = 1;

        if(early_serial_setup(&sUartInfo) != 0)
        {
            printk(KERN_ERR "[ERROR] PIU UART1 Init Failed\n");
        }
    }
}

#define _MapBase_nonPM 0xbf200000
#define _MapBase_PM    0xbf000000

static void __init chiptop_init(void)
{

}

// The following macros are defined just for easy porting from 8051 code
#define MDrv_WriteByte(addr, value)     _RV1(addr, value)
#define MDrv_Write2Byte(addr, value)    _RV2(addr, value)

#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

#define RIU     ((unsigned short volatile *) u32MapBase)
#define RIU8    ((unsigned char  volatile *) u32MapBase)

#define MDrv_WriteWordRegBit( u32Reg, bEnable, u16Mask )                                     \
    MST_MACRO_START                                                                     \
    RIU[u32Reg] = (bEnable) ? (RIU[u32Reg] |  (u16Mask)) :                           \
                                (RIU[u32Reg] & ~(u16Mask));                            \
    MST_MACRO_END

static void __init pad_config(void)
{
}

#define    RIUBASE_MIU                 0x101200    //0x101200 ~ 0x1012ff
#define    RIUBASE_MIU2                0x100600    //0x100600 ~ 0x1006ff


static int GOP0_MIU=0;
static int __init GOP0_MIU_setup(char *str)
{
    if( str != NULL )
    {
        sscanf(str,"%d",&GOP0_MIU);
    }

    return 0;
}
early_param("MS_GOP0_MIU", GOP0_MIU_setup);

#define UART_REG(addr)          (*(volatile unsigned int*)((addr)))
#define CHIP_UART_BASE        (0xBF000000 + (0x100980 << 1))

#define CHIP_UART_RX          (0x00*4 + CHIP_UART_BASE)
#define CHIP_UART_TX          (0x00*4 + CHIP_UART_BASE)
#define CHIP_UART_DLL         (0x00*4 + CHIP_UART_BASE)
#define CHIP_UART_DLM         (0x02*4 + CHIP_UART_BASE)
#define CHIP_UART_IER         (0x02*4 + CHIP_UART_BASE)
#define CHIP_UART_IIR         (0x04*4 + CHIP_UART_BASE)
#define CHIP_UART_FCR         (0x04*4 + CHIP_UART_BASE)
#define CHIP_UART_LCR         (0x06*4 + CHIP_UART_BASE)
#define CHIP_UART_MCR         (0x08*4 + CHIP_UART_BASE)
#define CHIP_UART_LSR         (0x0A*4 + CHIP_UART_BASE)
#define CHIP_UART_MSR         (0x0C*4 + CHIP_UART_BASE)

//Baud rate
#ifdef CONFIG_MSTAR_MIPS_BD_FPGA
#define CHIP_UART_BAUD_RATE  38400
#else
#define CHIP_UART_BAUD_RATE  115200
#endif

//Clock source
// 12000000, 14318180 , 123000000 , 144000000 , 160000000 , 172800000 , 108000000

//Divisor
#define CHIP_UART_DIVISOR    (CHIP_UART_CLK_FREQ/(16 * CHIP_UART_BAUD_RATE))

#define UART_LSR_DR			        0x01	                            // Receiver data ready
#define UART_LSR_OE			        0x02	                            // Overrun error indicator
#define UART_LSR_PE			        0x04	                            // Parity error indicator
#define UART_LSR_FE			        0x08	                            // Frame error indicator
#define UART_LSR_BI			        0x10	                            // Break interrupt indicator
#define UART_LSR_THRE		        0x20	                            // Transmit-hold-register empty
#define UART_LSR_TEMT		        0x40	                            // Transmitter empty

int chip_putc(int c)
{
    unsigned char u8Reg;

    do
	{
        u8Reg = UART_REG(CHIP_UART_LSR) & 0xff;
        if ((u8Reg & UART_LSR_THRE) == UART_LSR_THRE)
        {
            break;
        }
    }while(1);

    if(c=='\n')
    {
        UART_REG(CHIP_UART_TX)='\n';
        do
		{
            u8Reg = UART_REG(CHIP_UART_LSR) & 0xff;
            if ((u8Reg & UART_LSR_THRE) == UART_LSR_THRE)
            {
                break;
            }
        }while(1);
        UART_REG(CHIP_UART_TX)='\r';
    }
    else
    {
        UART_REG(CHIP_UART_TX) = c & 0xffff;
    }
    do
	{
        u8Reg = UART_REG(CHIP_UART_LSR) & 0xff;
        if ((u8Reg & UART_LSR_THRE) == UART_LSR_THRE)
        {
            break;
        }
    }while(1);
    return 1;
}

void uart_clk_scan(void)
{
    int i;
    unsigned long clk[] =
	{
		12000000,
		43200000,
		14318180,
		123000000,
		144000000,
		160000000,
		172800000,
		108000000,
		623001600
	};

    for(i=0;i<sizeof(clk)/sizeof(unsigned long);i++)
    {
        unsigned short divisor= (clk[i]/(16 * CHIP_UART_BAUD_RATE));
        //UART1 INIT:

        //Disable all UART interrupts
        UART_REG(CHIP_UART_IER) = 0x00;

        //Reset receiver and transmitr
        UART_REG(CHIP_UART_FCR) = 0x07;

        //Set 8 bit char, 1 stop bit, no parity
        UART_REG(CHIP_UART_LCR) = 0x03;

        UART_REG(CHIP_UART_LCR) = 0x83;
        UART_REG(CHIP_UART_DLM) = (divisor >> 8) & 0x00ff;
        UART_REG(CHIP_UART_DLL) = divisor & 0x00ff;
        UART_REG(CHIP_UART_LCR) = 0x03;

        UART_REG(CHIP_UART_TX) = '0'+ i;
    }
}

void pool_getc (void)
{
	  int nflag ;
    char u8Ch;
    nflag = 0 ;
    do
	{
        if ( ((UART_REG(CHIP_UART_LSR) & UART_LSR_DR) == UART_LSR_DR) || nflag )
        {
            break;
        }
    } while (1);

    u8Ch = (char)UART_REG(CHIP_UART_RX);      //get char
    chip_putc(u8Ch);
    return ;
}

#define USE_UART0 1
#define USE_ICE 0

void init_chip_uart(void)
{
#ifdef CONFIG_MSTAR_MIPS_BD_FPGA
    //FOR_FPGA_UART
    //bit 12 UART Enable, bit 11 UART RX Enable
    *(volatile volatile unsigned int*)(0xBF000000 + (0x000E12 << 1)) |= 0x0800;

    // Switch UART0 to PIU UART 0
    // Clear UART_SEL0
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101EA6 << 1)) &= ~0x000F;

    // UART_SEL0 --> PIU UART0
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101EA6 << 1)) |= 0x0004;

    // UART mode
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101EDC << 1)) &= ~0x0F00;

    //enable reg_uart_rx_enable;
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101E02 << 1)) |= (1<<10);
#else
    *(volatile volatile unsigned int*)(0xBF000000 + (0x100B26 << 1)) = 0x0C01;

///// Switch PAD ISP//////////////////////////////////////////////////////////
#if (USE_UART0 == 1)
    *(volatile volatile unsigned int*)(0xBF000000 + (0x000E12 << 1)) |= 0x0800;
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101EA6 << 1)) &= ~0x000F;
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101EA6 << 1)) |= 0x0004; //Select UART 0 to PIU UART 0
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101E02 << 1)) |= 0x0400;
#else
////Switch PAD RS232//////////////////////////////////////////////////
//   Select UART2 source
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101EA6 << 1)) = 0x0400;  //Select UART 1 to PIU UART 0
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101EA8 << 1)) = 0x0000;  //Select UART 4 to N.A
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101E04 << 1)) |= 0x0800;
    *(volatile volatile unsigned int*)(0xBF000000 + (0x101E04 << 1)) &= 0xFBFF;
//// Switch PAD RS232 end///////////////////////////////////////////////////
#endif  //#if (USE_UART0==0)
#endif//CONFIG_MSTAR_MIPS_BD_FPGA

    //Disable all UART interrupts
    UART_REG(CHIP_UART_IER) = 0x00;

    //Reset receiver and transmitr
    UART_REG(CHIP_UART_FCR) = 0x07;

    //Set 8 bit char, 1 stop bit, no parity
    UART_REG(CHIP_UART_LCR) = 0x03;

    //Set baud rate

    UART_REG(CHIP_UART_LCR) |= 0x80;
    UART_REG(CHIP_UART_DLM) = (CHIP_UART_DIVISOR >> 8) & 0x00ff;
    UART_REG(CHIP_UART_DLL) = CHIP_UART_DIVISOR & 0x00ff;
    UART_REG(CHIP_UART_LCR) &= ~(0x80) ;

    chip_putc('H');
    chip_putc('e');
    chip_putc('l');
    chip_putc('l');
    chip_putc('o');
    chip_putc(' ');
    chip_putc('U');
    chip_putc('A');
    chip_putc('R');
    chip_putc('T');
    chip_putc('\n');
}

inline void _chip_flush_miu_pipe(void)
{
    unsigned long flag=0;

#if DISABLE_MIPS_SYNC_OP
    unsigned int    dwReadData = 0;
    spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

    //Flush Pipe Data
    dwReadData = *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1));
	dwReadData &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    dwReadData |= 0x02;    //Mode 2: Flush Pipe Data

    *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) = dwReadData;
	*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) |= BIT4;	// cache op_fire

    do
    {
        dwReadData = *(volatile unsigned int *)(0xbf203104);			// check if mode_2 operation is done (0x101882)
        dwReadData &= _BIT(9);											// if BIT_10 is 1, mode_2 flush done

    } while(dwReadData == 0);

    spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
#else
    spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

    asm volatile (
    "sync;"
    );

    spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
#endif
}

//L2 cache write back invalid
void Chip_L2_cache_wback_inv( unsigned long addr, unsigned long size)
{
    unsigned long flag = 0;
    volatile unsigned int temp = 0;

    if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) != 0)	// L2 cache buffer is disabled
    {
        // flush pipe
		_chip_flush_miu_pipe();
    }
    else
    {
		if(size == 0)
        {
            printk("[ERROR] L2 Cache: Invalid Parameters\n");
        }
        else
        {
			if(size >= MIPS_L2_INV_TH)	// if size >= 128KB, invalid all
            {
            	spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

                //Flush All and Invalid Cache
 				temp = *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1));
 				temp &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
 				temp |= 0x04;    //Mode 4: Flush All and Invalid Cache

				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) = temp;
                *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) |= BIT4;

                do
                {
                	temp = *(volatile unsigned int *)(0xbf000000+0x203104); // check if mode_4 operation is done (0x101882)
                	temp &= _BIT(10);										// if BIT_10 is 1, mode_4 flush done
            	}while(temp == 0);
            	spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
            }
        	else // range flush
            {
            	spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

            	*(volatile unsigned int *)(0xbf000000+0x203110) = (addr) & 0xFFE0; 					// H44 range start Lo(the bit_0 to bit_4 can not be changed)
            	*(volatile unsigned int *)(0xbf000000+0x203114) = ((addr>>16)) & 0xFFFF; 			// H45 range start hi
            	*(volatile unsigned int *)(0xbf000000+0x203118) = ((addr+size-1)) & 0xFFE0; 		// H46 range start Lo(the bit_0 to bit_4 can not be changed)
            	*(volatile unsigned int *)(0xbf000000+0x20311C) = ((addr+size-1)>>16) & 0xFFFF; 	// H47 range start hi

                //Flush and Invalid Range
				temp = *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1));
 				temp &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
 				temp |= 0x06;    //Mode 6: Flush Range and Invalid Cache

 				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) = temp;	// enable mode_6
 				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) |= BIT4;	// cache_op fire

                do
                {
                	temp = *(volatile unsigned int *)(0xbf000000+0x203104); // check if mode_6 operation is done (0x101882)
                	temp &= _BIT(11);										// if BIT_11 is 1, mode_6 flush done
                	//temp &= _BIT(10);										// if use mips sync op, all mode will change to mode 4
            	}while(temp == 0);

            	spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
            }
        }

        // flush MIU pipe
        _chip_flush_miu_pipe();
    }
}

// L2 cache write back keep valid
void Chip_L2_cache_wback( unsigned long addr, unsigned long size)
{
    unsigned long flag = 0;
    volatile unsigned int temp = 0;

    if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) != 0)	// L2 cache buffer is disabled
    {
        // flush pipe
        _chip_flush_miu_pipe();
    }
    else
    {
		if(size == 0)
        {
            printk("[ERROR] L2 Cache: Invalid Parameters\n");
        }
        else
        {
        	if(size >= MIPS_L2_WB_TH)	// if size >= 256KB, flush all
            {
            	spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

				//Flush all and keep valid (pipe data and cache dirty data)
 				temp = *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1));
 				temp &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
 				temp |= 0x03;    //Mode 3: Flush all and keep valid (pipe data and cache dirty data)

				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) = temp;
                *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) |= BIT4;

                do
                {
                	temp = *(volatile unsigned int *)(0xbf000000+0x203104); // check if mode_4 operation is done (0x101882)
                	temp &= _BIT(10);										// if BIT_10 is 1, mode_3 flush done
            	}while(temp == 0);
            	spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
            }
        	else // range flush
            {
            	spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

            	*(volatile unsigned int *)(0xbf000000+0x203110) = (addr) & 0xFFE0; 					// H44 range start Lo(the bit_0 to bit_4 can not be changed)
            	*(volatile unsigned int *)(0xbf000000+0x203114) = ((addr>>16)) & 0xFFFF; 			// H45 range start hi
            	*(volatile unsigned int *)(0xbf000000+0x203118) = ((addr+size-1)) & 0xFFE0; 		// H46 range start Lo(the bit_0 to bit_4 can not be changed)
            	*(volatile unsigned int *)(0xbf000000+0x20311C) = ((addr+size-1)>>16) & 0xFFFF; 	// H47 range start hi

				//Flush and Invalid Range
				temp = *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1));
 				temp &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
 				temp |= 0x05;    //Mode 5: Flush Range and Invalid Cache

 				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) = temp;	// enable mode_5
 				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) |= BIT4;	// cache_op fire

                do
                {
                	temp = *(volatile unsigned int *)(0xbf000000+0x203104); // check if mode_5 operation is done (0x101882)
                	temp &= _BIT(11);										// if BIT_11 is 1, mode_5 flush done
                	//temp &= _BIT(10);										// if use mips sync op, all mode will change to mode 4
            	}while(temp == 0);

            	spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
            }
        }

        // flush MIU pipe
        _chip_flush_miu_pipe();
    }
}

//L2 cache invalid function
void Chip_L2_cache_inv( unsigned long addr, unsigned long size)
{
    unsigned long flag = 0;
    volatile unsigned int temp = 0;

    if(((*(volatile unsigned int *)(0xbf203100)) & BIT1) != 0)	// L2 cache buffer is disabled
    {
        // flush pipe
        _chip_flush_miu_pipe();
    }
    else
    {
		if(size == 0)
        {
            printk("[ERROR] L2 Cache: Invalid Parameters\n");
        }
        else
        {
			if(size >= MIPS_L2_INV_TH)	// if size >= 128KB, invalid all
            {
            	spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

                //Flush All and Invalid Cache
 				temp = *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1));
 				temp &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
 				temp |= 0x04;    //Mode 4: Flush All and Invalid Cache

				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) = temp;
                *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) |= BIT4;

                do
                {
                	temp = *(volatile unsigned int *)(0xbf000000+0x203104); // check if mode_4 operation is done (0x101882)
                	temp &= _BIT(10);										// if BIT_10 is 1, mode_4 flush done
            	}while(temp == 0);
            	spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
            }
            else // range invalid
            {
            	spin_lock_irqsave(((spinlock_t*)&l2prefetch_lock), flag);

            	*(volatile unsigned int *)(0xbf000000+0x203110) = (addr) & 0xFFE0; 					// H44 range start Lo(the bit_0 to bit_4 can not be changed)
            	*(volatile unsigned int *)(0xbf000000+0x203114) = ((addr>>16)) & 0xFFFF; 			// H45 range start hi
            	*(volatile unsigned int *)(0xbf000000+0x203118) = ((addr+size-1)) & 0xFFE0; 		// H46 range start Lo(the bit_0 to bit_4 can not be changed)
            	*(volatile unsigned int *)(0xbf000000+0x20311C) = ((addr+size-1)>>16) & 0xFFFF; 	// H47 range start hi

                //Flush and Invalid Range
				temp = *(volatile unsigned int *)(0xBF000000 + (0x101882 << 1));
 				temp &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
 				temp |= 0x06;    //Mode 6: Flush Range and Invalid Cache

 				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) = temp;	// enable mode_6
 				*(volatile unsigned int *)(0xBF000000 + (0x101882 << 1)) |= BIT4;	// cache_op fire

                do
                {
                	temp = *(volatile unsigned int *)(0xbf000000+0x203104); // check if mode_6 operation is done (0x101882)
                	temp &= _BIT(11);										// if BIT_11 is 1, mode_6 flush done
                	//temp &= _BIT(10);										// if use mips sync op, all mode will change to mode 4
            	}while(temp == 0);

            	spin_unlock_irqrestore((spinlock_t*)&l2prefetch_lock, flag);
            }
        }

        // flush MIU pipe
        _chip_flush_miu_pipe();
    }
}

void Chip_Flush_Memory_Range(unsigned long pAddress , unsigned long  size)
{
#if defined(CONFIG_MSTAR_MIPS)
    volatile unsigned int temp=0;
    if (*(volatile unsigned int*)(0xbf000000+0x203100) & BIT2) // Force All Request Cacheable
    {
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        Chip_L2_cache_wback( pAddress , size );
    }
    else
    {
        temp=*(volatile unsigned int *)(0xbf000000+0x201600) ; // dummy read
        // flush MIU pipe
        _chip_flush_miu_pipe();
    }
#endif
}

//flush miu pipe
void Chip_Flush_Memory(void)
{
#if defined(CONFIG_MSTAR_MIPS)
    Chip_Flush_Memory_Range(0, MIPS_L2_WB_TH);
#endif
}

void Chip_Read_Memory_Range(unsigned long pAddress , unsigned long  size)
{

#if defined(CONFIG_MSTAR_MIPS)
        Chip_L2_cache_inv(pAddress, size);
#endif
}

void Chip_Read_Memory(void)
{
#if defined(CONFIG_MSTAR_MIPS)
    Chip_Read_Memory_Range(0, MIPS_L2_INV_TH); //32KB
#endif
}

void Chip_Query_L2_Config(void)
{
    unsigned int CacheLineSize=0;

	// L2 cache buffer enable
    if ((*(volatile unsigned int *)(0xbf000000+0x203100)) & BIT1)
    {
        printk("L2 Cache Buffer Disabled\n");
        return;
    }
    else
    {
        printk("L2 Cache Buffer Enabled\n");
    }

    // H40 L2 cache line size 00: 32-bytes    01: 64-bytes
    CacheLineSize = ((*(volatile unsigned int *)(0xbf000000+0x203100)) & BIT4) >> 4;
    switch (CacheLineSize)
    {
        case 0:
            CacheLineSize = 32;
            break;
        case 1:
            CacheLineSize = 64;
            break;
        default:
			printk("\033[35mFunction = %s, Line = %d, un-known Cache Line Size\033[m\n", __PRETTY_FUNCTION__, __LINE__);
            break;
    }
    printk("Line Size: %d bytes", CacheLineSize);

	// WT/WB
    if (*(volatile unsigned int *)(0xbf000000+0x203124) & BIT2)  // H49 enable write through
    {
        printk(", write through");
    }
    else
    {
        printk(", write back");
    }

    if (*(volatile unsigned int*)(0xbf000000+0x203100) & BIT2) // H40
    {
        printk("(All Request Cacheable)\n");
    }
    else
    {
        printk("(Only Cache)\n");
    }
}

unsigned int Chip_Query_MIPS_CLK(void)
{
#if 0
     unsigned int    dwCpuSpeed = 0;

    dwCpuSpeed = (*(volatile unsigned int*)(0xbf000000 + (0x110ac2 << 1)) & 0x000000FF);
    dwCpuSpeed <<= 16;
    dwCpuSpeed |= (*(volatile unsigned int*)(0xbf000000 + (0x110ac0 << 1)) & 0x0000FFFF);
    dwCpuSpeed = (((4320000 / ((dwCpuSpeed * 100) / 0x80000)) * 24) / 100)*1000000;

    return dwCpuSpeed;
#endif
    /* Kiwi update(from kratos)
    * MIPS_PLL(3f is default value)
    * wriu -w 0x110c34 0x003f   // PLL Freq = 0x3f*12 = 756 (MHz), get the value of 0x110c34 and * 12 (need to * 1000000)
    */
    unsigned int u32Count, u32_pll_freq = 0;
    u32_pll_freq = *(volatile unsigned int *)(0xBF221868);
    u32Count = (u32_pll_freq * 12) * 1000000;

    return u32Count;

    }

inline void* Chip_mphy_cachevirt( unsigned long mphyaddr )
{
#if defined(CONFIG_MSTAR_MIPS)
    if (mphyaddr < HIGH_MEM_PHY_BASE)
    {
        return (void*)(mphyaddr | 0x80000000);
    }
    else
    {
        return (void*)(NULL);
    }
#endif
}

inline void* Chip_mphy_noncachevirt( unsigned long mphyaddr )
{
#if defined(CONFIG_MSTAR_MIPS)
    if (mphyaddr < HIGH_MEM_PHY_BASE)
    {
        return (void*)(mphyaddr | 0xA0000000);
    }
    #if (MIU_DRAM_LEN0 == 0x20000000)
    else if (mphyaddr < MIU_INTERVAL)
    {
        return (void*)((mphyaddr -(MIU_INTERVAL>>1) ) |0xC0000000);
    }
    #endif
    else if ((MIU_INTERVAL <= mphyaddr) &&

    #if (MIU_DRAM_LEN1 == 0x20000000)
                (mphyaddr < (MIU_INTERVAL + (MIU_DRAM_LEN1>>1))))
    {
        return (void*)((mphyaddr -MIU_INTERVAL) | 0xD0000000);
    }
    #else
                (mphyaddr < (MIU_INTERVAL + MIU_DRAM_LEN1)))
    {
        return (void*)((mphyaddr - MIU_INTERVAL) | 0xD0000000);
    }
    #endif
    else
    {
        return (void*)(NULL);
    }
#endif
}

inline unsigned long Chip_mphy_bus( unsigned long phyaddress )
{
#if defined(CONFIG_MSTAR_MIPS)
    if (phyaddress >= (MIU_INTERVAL << 1))
    {
        return -1;
    }
    else if (phyaddress >= MIU_INTERVAL)
    {
        return (MIU1_BUS_BASE + phyaddress - MIU_INTERVAL);
    }
    #if (MIU_DRAM_LEN0 == 0x20000000 )
    else if (phyaddress >= HIGH_MEM_PHY_BASE)
    {
        return ( HIGH_MEM_BUS_BASE+phyaddress-HIGH_MEM_PHY_BASE );
    }
    #endif
    else
    {
        return phyaddress;
    }
#endif
}

inline unsigned long Chip_bus_mphy( unsigned long busaddress )
{
#if defined(CONFIG_MSTAR_MIPS)
    if (busaddress >= HIGH_MEM_MAX_BUS_ADDR)
    {
        return -1;
    }
    else if (busaddress >= MIU1_BUS_BASE)
    {
        return (MIU_INTERVAL + busaddress - MIU1_BUS_BASE);
    }
    #if (MIU_DRAM_LEN0 == 0x20000000 )
    else if (busaddress >= HIGH_MEM_BUS_BASE)
    {
        return (HIGH_MEM_PHY_BASE +busaddress- HIGH_MEM_BUS_BASE);
    }
    #endif
    else
{
        return busaddress;
    }
#endif
}

void arch_enable_MsWDT_reset(unsigned long sec)
{
    *(volatile int*)(0xBF000000 + (0x300A << 1)) = 0x02dc;
    *(volatile int*)(0xBF000000 + (0x3008 << 1)) = 0x6700;
}

void arch_disable_MsWDT_reset(void)
{
    *(volatile int*)(0xBF000000+(0x300A<<1)) = 0x0;
    *(volatile int*)(0xBF000000+(0x3008<<1)) = 0x0;  // reg_top_sw_rst   
}

EXPORT_SYMBOL(Chip_L2_cache_wback_inv);
EXPORT_SYMBOL(Chip_L2_cache_wback);
EXPORT_SYMBOL(Chip_L2_cache_inv);

EXPORT_SYMBOL(Chip_Flush_Memory);
EXPORT_SYMBOL(Chip_Flush_Memory_Range);
EXPORT_SYMBOL(Chip_Read_Memory);
EXPORT_SYMBOL(Chip_Read_Memory_Range);
