////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2011 MStar Semiconductor, Inc.
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
#ifndef __HAL_SDIO_PLATFORM_H__
#define __HAL_SDIO_PLATFORM_H__

// this file is use only for mooney project 2016.7.15

#include <config.h> // mboot need, kernel don't

#ifdef CONFIG_SD

	// uboot

	#include <common.h>

	#define printk printf

	#define TUNE_NOW		0x01
	#define TUNE_SILENCE	0x02
	#define TUNE_FINISH		0x00


	#define uintptr_t U32

	// use config value to define, for compatible with kernel
	#ifdef CONFIG_MIU0_BUSADDR
	#define MSTAR_MIU0_BUS_BASE CONFIG_MIU0_BUSADDR
	#endif

	#ifdef CONFIG_MIU1_BUSADDR
	#define MSTAR_MIU1_BUS_BASE CONFIG_MIU1_BUSADDR
	#endif

	//#ifdef CONFIG_MIU2_BUSADDR
	//#define MSTAR_MIU2_BUS_BASE CONFIG_MIU2_BUSADDR
	//#endif


	// uboot riu
	//#define RIU_PM_BASE			0x1F000000
	#define RIU_BASE				0x1F000000

#else

	// kernel

	#include <linux/printk.h>
	#include <linux/string.h> // memcpy
	#include <linux/delay.h> // udelay mdelay
	#include <linux/sched.h> // DECLARE_WAIT_QUEUE_HEAD & wait_event_timeout() & TASK_NORMAL

	#include <mstar/mstar_chip.h> // MSTAR_MIU0_BUS_BASE
	#include <linux/irqreturn.h> // irqreturn_t
	#include <linux/mmc/core.h> // struct mmc_command, struct mmc_data, struct mmc_request
	#include <linux/scatterlist.h> // struct scatterlist
	#include "chip_setup.h" // Chip_Clean_Cache_Range_VA_PA()
	#include <chip_int.h> // interrupt vector

	// kernel riu
	//#define RIU_PM_BASE			0xFD000000
	//#define RIU_BASE				0xFD200000

	#if defined(CONFIG_ARM)
		#define RIU_BASE                     (IO_ADDRESS(0x1F000000UL))
	#elif defined(CONFIG_ARM64)
		extern ptrdiff_t   mstar_pm_base;
		#define RIU_BASE                     ((uintptr_t)(mstar_pm_base))
	#endif

	extern unsigned long ulSDIO_CONFIG_NUM;

#endif




#ifdef CONFIG_SD

	// uboot need only

#else
	// kernel need only
	irqreturn_t HalSdio_KernelIrq(int irq, void *devid);

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               PLATFORM FUNCTION DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               DATATYPE DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL    ((void*)0)
#endif

#define U64  unsigned long long
#define U32  unsigned int
#define U16  unsigned short
#define U8   unsigned char
#define S32  signed int


#define BIT00 0x0001
#define BIT01 0x0002
#define BIT02 0x0004
#define BIT03 0x0008
#define BIT04 0x0010
#define BIT05 0x0020
#define BIT06 0x0040
#define BIT07 0x0080
#define BIT08 0x0100
#define BIT09 0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000


#define U32BEND2LEND(X) ( ((X&0x000000FF)<<24) + ((X&0x0000FF00)<<8) + ((X&0x00FF0000)>>8) + ((X&0xFF000000)>>24) )
#define U16BEND2LEND(X) ( ((X&0x00FF)<<8) + ((X&0xFF00)>>8) )


///////////////////////////////////////////////////////////////////////////////////////////////////
// RIU MACRO
////////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_OFFSET_SHIFT_BITS           2
#define GET_REG_ADDR(x, y)              ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define FCIE_RIU_W16(addr,value)	*((volatile U16*)(uintptr_t)(addr)) = (value)
#define FCIE_RIU_R16(addr)			*((volatile U16*)(uintptr_t)(addr))
#define FCIE_RIU_W8(addr, value)	*((volatile U8*)(uintptr_t)(addr)) = (value)
#define FCIE_RIU_R8(addr)			*((volatile U8*)(uintptr_t)(addr))

// read modify write 16 bits register macro
#define FCIE_RIU_16_ON(addr,value) FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)|(value))
#define FCIE_RIU_16_OF(addr,value) FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)&(~(value)))
#define FCIE_RIU_W1C(addr, val)    FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)&(val))

#define FCIE_RIU_8_ON(addr,value) FCIE_RIU_W8(addr, FCIE_RIU_R8(addr)|(value))
#define FCIE_RIU_8_OF(addr,value) FCIE_RIU_W8(addr, FCIE_RIU_R8(addr)&(~(value)))


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU BASE ADDRESS
////////////////////////////////////////////////////////////////////////////////////////////////////



#define FCIE_IP					1
#define SDIO_IP					2
#define ALTERNATIVE_IP			SDIO_IP

#define SDIO_IP_VERIFY

#define RIU_BANK_2_BASE(BANK)		(RIU_BASE+(BANK<<9))
// register num per bank --> 0x80 = 128
// register addr offset  --> 4

////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU BANK DEFINITION
////////////////////////////////////////////////////////////////////////////////////////////////////
#define RIU_BANK_PM_SLEPP		0x000EUL
#define RIU_BANK_PMGPIO			0x000FUL
#define RIU_BANK_TIMER1			0x0030UL
#define RIU_BANK_MIU2			0x1006UL
#define RIU_BANK_CLKGEN2		0x100AUL
#define RIU_BANK_CLKGEN0		0x100BUL
#define RIU_BANK_MIU			0x1012UL
#define RIU_BANK_CHIPTOP		0x101EUL
#if (ALTERNATIVE_IP == FCIE_IP)
#define RIU_BANK_SDIO0			0x1113UL
#define RIU_BANK_SDIO1			0x1114UL
#define RIU_BANK_SDIO2			0x1115UL
#elif (ALTERNATIVE_IP == SDIO_IP)
#define RIU_BANK_SDIO0			0x120FUL
#define RIU_BANK_SDIO1			0x1228UL
#define RIU_BANK_SDIO2			0x1229UL
#endif
#define RIU_BANK_CHIP_GPIO		0x102BUL


#define RIU_BASE_PM_SLEEP		RIU_BANK_2_BASE(RIU_BANK_PM_SLEPP)
#define RIU_BASE_PMGPIO			RIU_BANK_2_BASE(RIU_BANK_PMGPIO)
#define RIU_BASE_TIMER1			RIU_BANK_2_BASE(RIU_BANK_TIMER1)
#define RIU_BASE_MIU2			RIU_BANK_2_BASE(RIU_BANK_MIU2)
#define RIU_BASE_CLKGEN2		RIU_BANK_2_BASE(RIU_BANK_CLKGEN2)
#define RIU_BASE_CLKGEN0		RIU_BANK_2_BASE(RIU_BANK_CLKGEN0)
#define RIU_BASE_MIU			RIU_BANK_2_BASE(RIU_BANK_MIU)
#define RIU_BASE_CHIPTOP		RIU_BANK_2_BASE(RIU_BANK_CHIPTOP)
#define RIU_BASE_SDIO0			RIU_BANK_2_BASE(RIU_BANK_SDIO0) // main bank
#define RIU_BASE_SDIO1			RIU_BANK_2_BASE(RIU_BANK_SDIO1) // CIFD + CRC
#define RIU_BASE_SDIO2			RIU_BANK_2_BASE(RIU_BANK_SDIO2) // power save bank
#define RIU_BASE_CHIP_GPIO		RIU_BANK_2_BASE(RIU_BANK_CHIP_GPIO)

////////////////////////////////////////////////////////////////////////////////////////////////////

#define FCIE_REG_BASE_ADDR              (RIU_BASE_SDIO0)
#define FCIE_CMDFIFO_BASE_ADDR          (RIU_BASE_SDIO0 + (0x20<<2)) // CIFC command FIFO

#define FCIE_CIFD_FIFO_W				(RIU_BASE_SDIO1)
#define FCIE_CIFD_FIFO_R				(RIU_BASE_SDIO1 + (0x20<<2))
#define FCIE_CRC_BUF					(RIU_BASE_SDIO1 + (0x40<<2))

#define FCIE_POWER_SAVE_MODE_BASE		(RIU_BASE_SDIO2)

#define SD_USE_FCIE5		1
#define SDIO_D1_INTR_VER	2

#include "hal_reg_sdio.h"

#define RIU_UNIT_SHIFT           2


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_TIMER1 0x0030
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TIMER1_ENABLE                   (RIU_BASE_TIMER1+(0x20<<RIU_UNIT_SHIFT))
#define TIMER1_HIT                      (RIU_BASE_TIMER1+(0x21<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_LOW                  (RIU_BASE_TIMER1+(0x22<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_HIGH                 (RIU_BASE_TIMER1+(0x23<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_LOW                  (RIU_BASE_TIMER1+(0x24<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_HIGH                 (RIU_BASE_TIMER1+(0x25<<RIU_UNIT_SHIFT))


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_MIU2 0x1006
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU2_79							(RIU_BASE_MIU2+(0x79 << RIU_UNIT_SHIFT))
#define MIU_SELECT_BY_SDIO				(BIT10)


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CLKGEN0 0x100B
////////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_CLK_SDIO					(RIU_BASE_CLKGEN0+(0x69<<RIU_UNIT_SHIFT))

#define BIT_SDIO_CLK_GATING				(BIT00)
#define BIT_SDIO_CLK_INVERSE			(BIT01)
#define BIT_CLKGEN_SDIO_MASK			(BIT05|BIT04|BIT03|BIT02)
#define BIT_SDIO_CLK_SRC_SEL			(BIT06) // 0: clk_xtal 12M, 1: clk_nfie_p1

////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_MIU 0x1012
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU_GROUP2_REQ_MSK				(RIU_BASE_MIU+(0x33 << RIU_UNIT_SHIFT))
#define MIU_SDIO_MSK					BIT10 // 1: mask request from SDIO


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CHIPTOP 0x101E
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIPTOP_43h						(RIU_BASE_CHIPTOP+(0x43 << RIU_UNIT_SHIFT))
#define CHIPTOP_50h						(RIU_BASE_CHIPTOP+(0x50 << RIU_UNIT_SHIFT))
#define CHIPTOP_5Ah						(RIU_BASE_CHIPTOP+(0x5A << RIU_UNIT_SHIFT))
#define CHIPTOP_6Fh						(RIU_BASE_CHIPTOP+(0x6F << RIU_UNIT_SHIFT))
#define CHIPTOP_7Bh						(RIU_BASE_CHIPTOP+(0x7B << RIU_UNIT_SHIFT))

// CHIPTOP_43h
#define REG_SD_PE						(BIT15|BIT14|BIT13|BIT12|BIT11)

// CHIPTOP_50h
#define REG_ALL_PAD_IN					(BIT15)

// CHIPTOP_5Ah
#define REG_SD_MODE_MSK					(BIT09|BIT08) // sd_mode should never use in mooney

// CHIPTOP_6F
#define REG_NAND_MODE_MSK				(BIT07|BIT06)

// CHIPTOP_7Bh
#define REG_SDIO_CONFIG_MSK				(BIT05|BIT04)
#define REG_SDIO_MODE_1					(BIT04) // only sdio_mode = 1


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               EXTERN GLOBAL FUNCTION
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#define SDIO_MODE_GPIO_PAD_BPS			1
#define SDIO_MODE_GPIO_PAD_SDR			2
#define SDIO_MODE_UNKNOWN				7

//#define SDIO_PAD_SDR104				SDIO_MODE_32BITS_MACRO_SDR104
//#define SDIO_PAD_SDR50				SDIO_MODE_32BITS_MACRO_SDR104
//#define SDIO_PAD_DDR50				SDIO_MODE_8BITS_MACRO_DDR
#define SDIO_PAD_SDR25					SDIO_MODE_GPIO_PAD_SDR
#define SDIO_PAD_SDR12					SDIO_MODE_GPIO_PAD_SDR

///////////////////////////////////////////////////////////////////////////////////////////////////
// interrutp vector number
// check mstar2/hal/[project]/cpu/chip_int.h
//       mstar2/hal/[project]/cpu/arm64/chip_int.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#define SDIO_INT_VECTOR E_IRQEXPH_SDIO


///////////////////////////////////////////////////////////////////////////////////////////////////
// new feature or patch different with other project
///////////////////////////////////////////////////////////////////////////////////////////////////
#define SDIO_NEW_R2N_PATCH // R2N mode new patch from manhattan 2015.06.09
#if (ALTERNATIVE_IP == SDIO_IP)
#define SDIO_R3_CRC_PATCH // response CRC false alarm patch 2015.06.09, SDIO has patch only
#endif

#define SDIO_ERRDET 			1

#if (defined SDIO_ERRDET) && (SDIO_ERRDET==1)
#define SDIO_ERRDET_RSP_TO		1
#define SDIO_ERRDET_RSP_CRC_ERR	1
#define SDIO_ERRDET_R_CRC_ERR	1
#define SDIO_ERRDET_W_CRC_STS	1
#define SDIO_ERRDET_R_SBIT_TO	1 // Manhattan fix counter 2 issue
#define SDIO_ERRDET_W_MIU_TO	1 //
#endif

#ifdef CONFIG_SD

	// uboot
	//#define ENABLE_SDIO_INTERRUPT_MODE	1
	//#define ENABLE_SDIO_ADMA			1

#else

	// kernel
	#define ENABLE_SDIO_INTERRUPT_MODE	1
	#define ENABLE_SDIO_ADMA			1

#endif


typedef struct _SKEWER {

	unsigned int u32LatchOKStart;
	unsigned int u32LatchOKEnd;
	unsigned int u32LatchBest;
	unsigned char u8_edge;

} SKEWER;

//#define CONFIG_MIU0_BUSADDR 0x20000000
//#define CONFIG_MIU1_BUSADDR 0xA0000000

U32		HalSdio_SlectBestSkew4(U32 u32_Candidate, SKEWER * pSkewer);
void	HalSdio_SetTuning(U8 u8Type, U8 u8Count);
void	HalSdio_SetTriggerLevel(U8 u8STrigLevel);


void	HalSdio_ResetIP(void);
void	HalSdio_Platform_InitChiptop(void);
U8		HalSdio_GetPadType(void);
void	HalSdio_SwitchPad(unsigned char);
void	HalSdioDelayMs(U32 u32Ms);
void	HalSdioDelayUs(U32 u32Us);
void	HalSdio_DelaySecond(U32 u32Sec);
U32		HalSdio_SetClock(U32 u32Clock);
void	HalSdio_DumpDebugBus(void);
void	HalSdio_GetSBootGPIOConfig(void);
S32		HalSdio_GetCardDetect(void);
S32		HalSdio_GetWriteProtect(void);
void	HalSdio_SetCardPower(U8 u8OnOff);
void	HalSdio_SetDrivingStrength(U16 u16DrivingLevel);
void	HalSdio_MaskMiuReq(U8 u8Mask);


#endif // #ifndef __HAL_SDIO_PLATFORM_H__
