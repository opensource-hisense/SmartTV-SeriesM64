#ifndef __eMMC_MIAMI_LINUX__
#define __eMMC_MIAMI_LINUX__

#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/dma-mapping.h>
#include <linux/mmc/host.h>
#include <linux/scatterlist.h>
#include <mstar/mstar_chip.h>
#include <mach/io.h>
#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#endif
#include "chip_int.h"

#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/kthread.h>

#ifdef CONFIG_ARM
#include <mach/io.h>
#endif

#include "chip_int.h"
#include "chip_setup.h"
#if defined(CONFIG_ARM) // ARM: AMBER3, AGATE
#include <mstar/mstar_chip.h>
#endif

//=====================================================
// HW registers
//=====================================================
#define REG_OFFSET_SHIFT_BITS           2

#define REG_FCIE_U16(Reg_Addr)          (*(volatile U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)              ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define REG_FCIE(reg_addr)              REG_FCIE_U16(reg_addr)
#define REG_FCIE_W(reg_addr, val)       REG_FCIE(reg_addr) = (val)
#define REG_FCIE_R(reg_addr, val)       val = REG_FCIE(reg_addr)
#define REG_FCIE_SETBIT(reg_addr, val)  REG_FCIE(reg_addr) |= (val)
#define REG_FCIE_CLRBIT(reg_addr, val)  REG_FCIE(reg_addr) &= ~(val)
#define REG_FCIE_W1C(reg_addr, val)     REG_FCIE_W(reg_addr, REG_FCIE(reg_addr)&(val))

//------------------------------
#define RIU_PM_BASE                     (IO_ADDRESS(0x1F000000))
#define RIU_BASE                        (IO_ADDRESS(0x1F200000))

#define REG_BANK_FCIE0                  0x8980
#define REG_BANK_FCIE1                  (REG_BANK_FCIE0 + 0x20)
#define REG_BANK_FCIE2                  (REG_BANK_FCIE0 + 0x80)
#define REG_BANK_FCIEPOWERSAVEMODE      0x11D80

#define FCIE0_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE1_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE1)
#define FCIE2_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_POWEER_SAVE_MODE_BASE      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIEPOWERSAVEMODE)

#define FCIE_REG_BASE_ADDR              FCIE0_BASE
#define FCIE_CMDFIFO_BASE_ADDR             FCIE1_BASE
#define FCIE_CIFD_BASE_ADDR             FCIE2_BASE

#define FCIE_NC_WBUF_CIFD_BASE          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_NC_RBUF_CIFD_BASE          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2 + 0x20)

#include "eMMC_reg_v5.h"

//--------------------------------clock gen------------------------------------
#define REG_BANK_CLKGEN0                0x0580	// (0x100B - 0x1000) x 80h
#define CLKGEN0_BASE                    GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN0)

#define reg_ckg_fcie                    GET_REG_ADDR(CLKGEN0_BASE, 0x64)
#define BIT_FCIE_CLK_GATING             BIT0
#define BIT_FCIE_CLK_INVERSE            BIT1
#define BIT_CLKGEN_FCIE_MASK            (BIT5|BIT4|BIT3|BIT2)
#define BIT_FCIE_CLK_SRC_SEL            BIT6 // 0: clk_xtal 12M, 1: clk_nfie_p1

//--------------------------------chiptop--------------------------------------
#define REG_BANK_CHIPTOP                0x0F00	// (0x101E - 0x1000) x 80h
#define PAD_CHIPTOP_BASE                GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)

#define reg_chiptop_0x12				GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x12)
#define BIT_TEST_OUT_MODE				(BIT6|BIT5|BIT4)
#define BIT_TEST_IN_MODE				(BIT2|BIT1|BIT0)

#define reg_chiptop_0x50                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x50)
#define BIT_ALL_PAD_IN                  BIT15

#define reg_chiptop_0x5A				GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x5A)
#define BIT_SD_CONFIG					(BIT9|BIT8)


#define reg_chiptop_0x64				GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x64)
#define BIT_CIAD_CONFIG					BIT0
#define BIT_PCMAD_CONFIG				BIT4
#define BIT_PCM2_CTRL_CONFIG			BIT3

#define reg_chiptop_0x6E                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x6E)
#define BIT_EMMC_CONFIG_MSK				(BIT7|BIT6)
#define BIT_EMMC_CONFIG_EMMC_MODE_1		BIT6

#define reg_chiptop_0x6F				GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x6F)
#define BIT_NAND_MODE					(BIT7|BIT6)

#define reg_chiptop_0x7B				GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x7B)
#define BIT_SDIO_CONFIG					(BIT5|BIT4)


//--------------------------------emmc pll--------------------------------------
#define REG_BANK_EMMC_PLL               0x11F80	// (0x123F - 0x1000) x 80h
#define EMMC_PLL_BASE                   GET_REG_ADDR(RIU_BASE, REG_BANK_EMMC_PLL)

#define REG_EMMC_PLL_RX01               GET_REG_ADDR(EMMC_PLL_BASE, 0x01)
#define reg_emmcpll_0x02                GET_REG_ADDR(EMMC_PLL_BASE, 0x02)
#define reg_emmcpll_0x03                GET_REG_ADDR(EMMC_PLL_BASE, 0x03)
	#define BIT_SKEW1_MASK				(BIT3|BIT2|BIT1|BIT0)
	#define BIT_SKEW2_MASK				(BIT7|BIT6|BIT5|BIT4)
	#define BIT_SKEW3_MASK				(BIT11|BIT10|BIT9|BIT8)
	#define BIT_SKEW4_MASK				(BIT15|BIT14|BIT13|BIT12)

#define reg_emmcpll_fbdiv               GET_REG_ADDR(EMMC_PLL_BASE, 0x04)
#define reg_emmcpll_pdiv                GET_REG_ADDR(EMMC_PLL_BASE, 0x05)
#define reg_emmc_pll_reset              GET_REG_ADDR(EMMC_PLL_BASE, 0x06)
#define reg_emmc_pll_test               GET_REG_ADDR(EMMC_PLL_BASE, 0x07)

#define reg_emmcpll_0x09                GET_REG_ADDR(EMMC_PLL_BASE, 0x09)

#define reg_ddfset_15_00                GET_REG_ADDR(EMMC_PLL_BASE, 0x18)
#define reg_ddfset_23_16                GET_REG_ADDR(EMMC_PLL_BASE, 0x19)
#define reg_emmc_test                   GET_REG_ADDR(EMMC_PLL_BASE, 0x1A)
#define reg_atop_patch                  GET_REG_ADDR(EMMC_PLL_BASE, 0x1C)
#define BIT_HS200_PATCH                 BIT0
#define BIT_HS_RSP_META_PATCH_HW        BIT2
#define BIT_HS_D0_META_PATCH_HW         BIT4
#define BIT_HS_DIN0_PATCH               BIT5
#define BIT_HS_EMMC_DQS_PATCH           BIT6
#define BIT_HS_RSP_MASK_PATCH           BIT7
#define BIT_DDR_RSP_PATCH               BIT8
#define BIT_ATOP_PATCH_MASK            (BIT0|BIT1|BIT2|BIT4|BIT5|BIT6|BIT7|BIT8)

#define reg_emmcpll_0x1d                GET_REG_ADDR(EMMC_PLL_BASE, 0x1d)
#define reg_emmcpll_0x1e                GET_REG_ADDR(EMMC_PLL_BASE, 0x1e)
#define reg_emmcpll_0x1f                GET_REG_ADDR(EMMC_PLL_BASE, 0x1f)
#define reg_emmcpll_0x20                GET_REG_ADDR(EMMC_PLL_BASE, 0x20)
#define REG_EMMC_PLL_RX30               GET_REG_ADDR(EMMC_PLL_BASE, 0x30)
#define REG_EMMC_PLL_RX32               GET_REG_ADDR(EMMC_PLL_BASE, 0x32)
#define REG_EMMC_PLL_RX33               GET_REG_ADDR(EMMC_PLL_BASE, 0x33)
#define REG_EMMC_PLL_RX34               GET_REG_ADDR(EMMC_PLL_BASE, 0x34)
#define reg_emmcpll_0x63                GET_REG_ADDR(EMMC_PLL_BASE, 0x63)
#define reg_emmcpll_0x68                GET_REG_ADDR(EMMC_PLL_BASE, 0x68)
#define reg_emmcpll_0x69                GET_REG_ADDR(EMMC_PLL_BASE, 0x69)
#define reg_emmcpll_0x6a                GET_REG_ADDR(EMMC_PLL_BASE, 0x6a)
#define reg_emmcpll_0x6b                GET_REG_ADDR(EMMC_PLL_BASE, 0x6b)
#define reg_emmcpll_0x6c                GET_REG_ADDR(EMMC_PLL_BASE, 0x6c)
#define reg_emmcpll_0x6d                GET_REG_ADDR(EMMC_PLL_BASE, 0x6d)
#define reg_emmcpll_0x6f                GET_REG_ADDR(EMMC_PLL_BASE, 0x6f)
#define reg_emmcpll_0x70                GET_REG_ADDR(EMMC_PLL_BASE, 0x70)
#define reg_emmcpll_0x71                GET_REG_ADDR(EMMC_PLL_BASE, 0x71)
#define reg_emmcpll_0x73                GET_REG_ADDR(EMMC_PLL_BASE, 0x73)
#define reg_emmcpll_0x74                GET_REG_ADDR(EMMC_PLL_BASE, 0x74)


#define BIT_TUNE_SHOT_OFFSET_MASK       (BIT4|BIT5|BIT6|BIT7)
#define BIT_TUNE_SHOT_OFFSET_SHIFT      4

//--------------------------------macro define---------------------------------

//--------------------------------clock gen------------------------------------
#define BIT_CLK_XTAL_12M				0x0
#define BIT_FCIE_CLK_20M                0x1
#define BIT_FCIE_CLK_32M				0x2
#define BIT_FCIE_CLK_36M				0x3
#define BIT_FCIE_CLK_40M				0x4
#define BIT_FCIE_CLK_43_2M				0x5
#define BIT_FCIE_CLK_54M				0x6
#define BIT_FCIE_CLK_62M				0x7
#define BIT_FCIE_CLK_72M				0x8
#define BIT_FCIE_CLK_86M				0x9
//										0xA
#define BIT_FCIE_CLK_EMMC_PLL_1X		0xB // 8 bits macro & 32 bit macro HS200
#define BIT_FCIE_CLK_EMMC_PLL_2X		0xC // 32 bit macroDDR & HS400
#define BIT_FCIE_CLK_300K               0xD
#define BIT_CLK_XTAL_24M				0xE
#define BIT_FCIE_CLK_48M                0xF

#define eMMC_PLL_FLAG                   0x80
#define eMMC_PLL_CLK__20M               (0x01|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__27M               (0x02|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__32M               (0x03|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__36M               (0x04|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__40M               (0x05|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__48M               (0x06|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__52M               (0x07|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__62M               (0x08|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__72M               (0x09|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__80M               (0x0A|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__86M               (0x0B|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_100M               (0x0C|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_120M               (0x0D|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_140M               (0x0E|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_160M               (0x0F|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_200M               (0x10|eMMC_PLL_FLAG)



#define eMMC_FCIE_VALID_CLK_CNT         8// FIXME

#define PLL_SKEW4_CNT               9
#define MIN_OK_SKEW_CNT             5

extern  U8 gau8_FCIEClkSel[];
//extern  U8 gau8_eMMCPLLSel_52[];
extern  U8 gau8_eMMCPLLSel_200[]; // for DDR52 or HS200

struct _eMMC_FCIE_ATOP_SET {

    U32 u32_ScanResult;
	U8  u8_Clk;
	U8  u8_Reg2Ch, u8_Skew4;

};
typedef struct _eMMC_FCIE_ATOP_SET eMMC_FCIE_ATOP_SET_t;


#define eMMC_RST_L()
#define eMMC_RST_H()

#define REG_BANK_TIMER1                 0x1800
#define TIMER1_BASE                     GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER1)

#define TIMER1_ENABLE                   GET_REG_ADDR(TIMER1_BASE, 0x20)
#define TIMER1_HIT                      GET_REG_ADDR(TIMER1_BASE, 0x21)
#define TIMER1_MAX_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x22)
#define TIMER1_MAX_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x23)
#define TIMER1_CAP_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x24)
#define TIMER1_CAP_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x25)

//--------------------------------sar5----------------------------
#define reg_sel_hvdetect             	GET_REG_ADDR(RIU_PM_BASE+0x1C00, 0x28)
#define BIT_SEL_HVDETECT                BIT4
#define reg_vplug_in_pwrgd             	GET_REG_ADDR(RIU_PM_BASE+0x1C00, 0x62)
//--------------------------------power saving mode----------------------------
#define REG_BANK_PM_SLEEP               (0x700)
#define PM_SLEEP_REG_BASE_ADDR          GET_REG_ADDR(RIU_PM_BASE, REG_BANK_PM_SLEEP)
#define reg_pwrgd_int_glirm             GET_REG_ADDR(PM_SLEEP_REG_BASE_ADDR, 0x61)
#define BIT_PWRGD_INT_GLIRM_EN          BIT9
#define BIT_PWEGD_INT_GLIRM_MASK        (BIT15|BIT14|BIT13|BIT12|BIT11|BIT10)
//=====================================================
// API declarations
//=====================================================
extern  U32 eMMC_hw_timer_delay(U32 u32us);
extern  U32 eMMC_hw_timer_sleep(U32 u32ms);
#define eMMC_HW_TIMER_MHZ               12000000//12MHz  [FIXME]


////////////////////////////////////////////////////////////////////////////////////////////////////

// define what latch method (mode) fcie has
// implement switch pad function with below cases

#define FCIE_MODE_GPIO_PAD_DEFO_SPEED		0
//#define FCIE_MODE_GPIO_PAD_HIGH_SPEED		1 // Brian said not use this pad for eMMC, no feedback clock in real chip case
#define FCIE_MODE_8BITS_MACRO_HIGH_SPEED	2
#define FCIE_MODE_8BITS_MACRO_DDR52			3
#define FCIE_MODE_32BITS_MACRO_DDR52		4
#define FCIE_MODE_32BITS_MACRO_HS200		5
#define FCIE_MODE_32BITS_MACRO_HS400_DS		6 // data strobe
#define FCIE_MODE_32BITS_MACRO_HS400_SKEW4	7

// define what latch method (mode) use for latch eMMC data
// switch FCIE mode when driver (kernel) change eMMC speed

#define EMMC_DEFO_SPEED_MODE				FCIE_MODE_GPIO_PAD_DEFO_SPEED
#define EMMC_HIGH_SPEED_MODE				FCIE_MODE_8BITS_MACRO_HIGH_SPEED
#define EMMC_DDR52_MODE						FCIE_MODE_32BITS_MACRO_DDR52 // FCIE_MODE_8BITS_MACRO_DDR52
#define EMMC_HS200_MODE						FCIE_MODE_32BITS_MACRO_HS200
#define EMMC_HS400_MODE						FCIE_MODE_32BITS_MACRO_HS400_DS // FCIE_MODE_32BITS_MACRO_HS400_SKEW4

// define what speed we want this chip/project run

#define ENABLE_eMMC_DDR52		            1
#define ENABLE_eMMC_HS200		            1
#define ENABLE_eMMC_HS400		            1

// for special project config use

#if defined(CONFIG_MMC_MSTAR_EMMC_FORCE_DDR52) && CONFIG_MMC_MSTAR_EMMC_FORCE_DDR52
#undef  ENABLE_eMMC_HS200
#define ENABLE_eMMC_HS200		            0
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////


extern  U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type);
extern  U32 eMMC_clock_setting(U16 u16_ClkParam);
extern  U32 eMMC_clock_gating(void);
extern  U32 eMMC_pll_BusIO_Setting(void);
extern void eMMC_set_WatchDog(U8 u8_IfEnable);
extern void eMMC_reset_WatchDog(void);
extern  U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern  U32 eMMC_DMA_MAP_address(U32 u32_Buffer, U32 u32_ByteCnt, int mode);
extern  void eMMC_DMA_UNMAP_address(U32 u32_DMAAddr, U32 u32_ByteCnt, int mode);
extern void eMMC_flush_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_Invalidate_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_flush_miu_pipe(void);
extern  U32 eMMC_PlatformResetPre(void);
extern  U32 eMMC_PlatformResetPost(void);
extern  U32 eMMC_PlatformInit(void);
extern  U32 eMMC_PlatformDeinit(void);
extern  U32 eMMC_CheckIfMemCorrupt(void);
extern void eMMC_DumpPadClk(void);
#define eMMC_BOOT_PART_W                BIT0
#define eMMC_BOOT_PART_R                BIT1
extern  U32 eMMC_BootPartitionHandler_WR(U8 *pDataBuf, U16 u16_PartType, U32 u32_StartSector, U32 u32_SectorCnt, U8 u8_OP);
extern  U32 eMMC_BootPartitionHandler_E(U16 u16_PartType);
extern  U32 eMMC_hw_timer_start(void);
extern  U32 eMMC_hw_timer_tick(void);
extern  irqreturn_t eMMC_FCIE_IRQ(int irq, void *dummy); // [FIXME]
extern  U32 eMMC_WaitCompleteIntr(U32 u32_RegAddr, U16 u16_WaitEvent, U32 u32_MicroSec);
extern  struct mutex FCIE3_mutex;
extern  void eMMC_LockFCIE(U8 *pu8_str);
extern  void eMMC_UnlockFCIE(U8 *pu8_str);
extern int  mstar_mci_Housekeep(void *pData);
extern U32  mstar_SD_CardChange(void);


//=====================================================
// partitions config
//=====================================================
// every blk is 512 bytes (reserve 2MB-64KB for internal use)
#define eMMC_DRV_RESERVED_BLK_CNT       ((0x200000-0x10000)/0x200)

#define eMMC_CIS_NNI_BLK_CNT            2
#define eMMC_CIS_PNI_BLK_CNT            2
#define eMMC_TEST_BLK_CNT               (0x100000/0x200) // 1MB

#define eMMC_CIS_BLK_0                  (64*1024/512) // from 64KB
#define eMMC_NNI_BLK_0                  (eMMC_CIS_BLK_0+0)
#define eMMC_NNI_BLK_1                  (eMMC_CIS_BLK_0+1)
#define eMMC_PNI_BLK_0                  (eMMC_CIS_BLK_0+2)
#define eMMC_PNI_BLK_1                  (eMMC_CIS_BLK_0+3)
#define eMMC_DDRTABLE_BLK_0             (eMMC_CIS_BLK_0+4)
#define eMMC_DDRTABLE_BLK_1             (eMMC_CIS_BLK_0+5)
#define eMMC_HS200TABLE_BLK_0           (eMMC_CIS_BLK_0+6)
#define eMMC_HS200TABLE_BLK_1           (eMMC_CIS_BLK_0+7)
#define eMMC_HS400TABLE_BLK_0           (eMMC_CIS_BLK_0+8)
#define eMMC_HS400TABLE_BLK_1           (eMMC_CIS_BLK_0+9)
#define eMMC_DrvContext_BLK_0           (eMMC_CIS_BLK_0+10)
#define eMMC_DrvContext_BLK_1           (eMMC_CIS_BLK_0+11)
#define eMMC_ALLRSP_BLK_0               (eMMC_CIS_BLK_0+12)
#define eMMC_ALLRSP_BLK_1               (eMMC_CIS_BLK_0+13)
#define eMMC_BURST_LEN_BLK_0            (eMMC_CIS_BLK_0+14)

#define eMMC_CIS_BLK_END                eMMC_BURST_LEN_BLK_0
// last 1MB in reserved area, use for eMMC test
#define eMMC_TEST_BLK_0                 (eMMC_CIS_BLK_END+1)


//=====================================================
// Driver configs
//=====================================================
#define DRIVER_NAME                     "mstar_mci"
#define eMMC_UPDATE_FIRMWARE            0

#define eMMC_ST_PLAT                    0x80000000
// [CAUTION]: to verify IP and HAL code, defaut 0
#define IF_IP_VERIFY                    0 // [FIXME] -->
// [CAUTION]: to detect DDR timiing parameters, only for DL

#define eMMC_IF_TUNING_TTABLE()           (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_TUNING_TTABLE)

// need to eMMC_pads_switch
// need to eMMC_clock_setting
#define IF_FCIE_SHARE_IP                0

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
#define ENABLE_EMMC_ASYNC_IO            0
#define ENABLE_EMMC_PRE_DEFINED_BLK     0
#endif

//------------------------------
// DDR48, DDR52, HS200, HS400
#define IF_DETECT_eMMC_DDR_TIMING           0 // 8 bit macro
#define ENABLE_eMMC_ATOP		            1 // 32 bit macro



#define FICE_BYTE_MODE_ENABLE               1 // always 1
#define ENABLE_eMMC_INTERRUPT_MODE			1
#define ENABLE_eMMC_RIU_MODE				0 // for debug cache issue

#define ENABLE_EMMC_POWER_SAVING_MODE   0

#if ENABLE_eMMC_RIU_MODE
#undef IF_DETECT_eMMC_DDR_TIMING
#define IF_DETECT_eMMC_DDR_TIMING       0 // RIU mode can NOT use DDR
#endif
// <-- [FIXME]

//------------------------------
#define eMMC_FEATURE_RELIABLE_WRITE     1
#if eMMC_UPDATE_FIRMWARE
#undef  eMMC_FEATURE_RELIABLE_WRITE
#define eMMC_FEATURE_RELIABLE_WRITE     0
#endif

//------------------------------
#if defined(CONFIG_MMC_MSTAR_tSD) && CONFIG_MMC_MSTAR_tSD
#define eMMC_RSP_FROM_RAM                   0
#else
#define eMMC_RSP_FROM_RAM                   0
#endif
#define eMMC_BURST_LEN_AUTOCFG              0 // asked by ¬õÀs¤j
#define eMMC_PROFILE_WR                     0
#define eMMC_HOUSEKEEP_THREAD               0

//------------------------------
#define eMMC_SECTOR_BUF_BYTECTN         eMMC_SECTOR_BUF_16KB
extern U8 gau8_eMMC_SectorBuf[];
extern U8 gau8_eMMC_PartInfoBuf[];

//------------------------------
// Boot Partition:
//   [FIXME]: if platform has ROM code like G2P
//------------------------------
//	No Need in A3
#define BL_BLK_OFFSET                   0
#define BL_BLK_CNT                      (0xF200/0x200)
#define OTP_BLK_OFFSET                  BL_BLK_CNT
#define OTP_BLK_CNT                     (0x8000/0x200)
#define SecInfo_BLK_OFFSET              (BL_BLK_CNT+OTP_BLK_CNT)
#define SecInfo_BLK_CNT                 (0x1000/0x200)
#define BOOT_PART_TOTAL_CNT             (BL_BLK_CNT+OTP_BLK_CNT+SecInfo_BLK_CNT)
// <-- [FIXME]

#define eMMC_CACHE_LINE                 0x80 // [FIXME]

//=====================================================
// tool-chain attributes
//===================================================== [FIXME] -->
#define eMMC_PACK0
#define eMMC_PACK1                      __attribute__((__packed__))
#define eMMC_ALIGN0
#define eMMC_ALIGN1                     __attribute__((aligned(eMMC_CACHE_LINE)))
// <-- [FIXME]

//=====================================================
// debug option
//=====================================================
#define eMMC_TEST_IN_DESIGN             0 // [FIXME]: set 1 to verify HW timer

#ifndef eMMC_DEBUG_MSG
#define eMMC_DEBUG_MSG                  1
#endif

/* Define trace levels. */
#define eMMC_DEBUG_LEVEL_ERROR          (1)    /* Error condition debug messages. */
#define eMMC_DEBUG_LEVEL_WARNING        (2)    /* Warning condition debug messages. */
#define eMMC_DEBUG_LEVEL_HIGH           (3)    /* Debug messages (high debugging). */
#define eMMC_DEBUG_LEVEL_MEDIUM         (4)    /* Debug messages. */
#define eMMC_DEBUG_LEVEL_LOW            (5)    /* Debug messages (low debugging). */

/* Higer debug level means more verbose */
#ifndef eMMC_DEBUG_LEVEL
#define eMMC_DEBUG_LEVEL                eMMC_DEBUG_LEVEL_HIGH
#endif

#if defined(eMMC_DEBUG_MSG) && eMMC_DEBUG_MSG
#define eMMC_printf(fmt, arg...)  printk(KERN_ERR fmt, ##arg)
#define eMMC_debug(dbg_lv, tag, str, ...)	         \
	do {	                                         \
		if (dbg_lv > eMMC_DEBUG_LEVEL)				 \
			break;									 \
		else if(eMMC_IF_TUNING_TTABLE())             \
			break;                                   \
		else if(eMMC_IF_DISABLE_LOG())               \
			break;                                   \
		else {										    \
			if (tag)								    \
				eMMC_printf("[ %s() Ln.%u ] ", __FUNCTION__, __LINE__);	\
													\
			eMMC_printf(str, ##__VA_ARGS__);		\
		} \
	} while(0)
#else /* eMMC_DEBUG_MSG */
#define eMMC_printf(...)
#define eMMC_debug(enable, tag, str, ...)	do{}while(0)
#endif /* eMMC_DEBUG_MSG */

#define eMMC_die(str) {eMMC_printf("eMMC Die: %s() Ln.%u, %s \n", __FUNCTION__, __LINE__, str); \
	                   panic("\n");}

#define eMMC_stop() \
	while(1)  eMMC_reset_WatchDog();

#define REG_BANK_TIMER1                 0x1800
#define TIMER1_BASE                     GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER1)

#define TIMER1_ENABLE                   GET_REG_ADDR(TIMER1_BASE, 0x20)
#define TIMER1_HIT                      GET_REG_ADDR(TIMER1_BASE, 0x21)
#define TIMER1_MAX_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x22)
#define TIMER1_MAX_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x23)
#define TIMER1_CAP_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x24)
#define TIMER1_CAP_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x25)


//=====================================================
// unit for HW Timer delay (unit of us)
//=====================================================
#define HW_TIMER_DELAY_1us              1
#define HW_TIMER_DELAY_5us              5
#define HW_TIMER_DELAY_10us             10
#define HW_TIMER_DELAY_100us            100
#define HW_TIMER_DELAY_500us            500
#define HW_TIMER_DELAY_1ms              (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms              (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms             (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms            (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms            (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s               (1000 * HW_TIMER_DELAY_1ms)

//=====================================================
// set FCIE clock
//=====================================================
#define FCIE_SLOWEST_CLK                BIT_FCIE_CLK_300K
#define FCIE_DEFO_SPEED_CLK			BIT_CLK_XTAL_12M
#define FCIE_HIGH_SPEED_CLK			eMMC_PLL_CLK_200M

// for backward compatible
#define FCIE_SLOW_CLK				FCIE_DEFO_SPEED_CLK
#define FCIE_DEFAULT_CLK			FCIE_DEFO_SPEED_CLK

//=====================================================
// transfer DMA Address
//=====================================================
#define MIU_BUS_WIDTH_BITS              3 // Need to confirm
/*
 * Important:
 * The following buffers should be large enough for a whole eMMC block
 */
// FIXME, this is only for verifing IP
#define DMA_W_ADDR                      0x40C00000
#define DMA_R_ADDR                      0x40D00000
#define DMA_W_SPARE_ADDR                0x40E00000
#define DMA_R_SPARE_ADDR                0x40E80000
#define DMA_BAD_BLK_BUF                 0x40F00000

#define MIU_CHECK_LAST_DONE             1

//=====================================================
// misc
//=====================================================
//#define BIG_ENDIAN
#define LITTLE_ENDIAN

//#if (defined(BIT_DQS_MODE_MASK) && (BIT_DQS_MODE_MASK != (BIT12|BIT13|BIT14)))

#undef BIT_DQS_MODE_MASK
#undef BIT_DQS_MODE_2T
#undef BIT_DQS_MODE_1_5T
#undef BIT_DQS_MODE_2_5T
#undef BIT_DQS_MODE_1T

#define BIT_DQS_MDOE_SHIFT              0

#define BIT_DQS_MODE_MASK               (BIT12|BIT13|BIT14)
#define BIT_DQS_MODE_0T                 (0 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_0_5T               (1 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1T                 (2 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1_5T               (3 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_2T                 (4 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_2_5T               (5 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_3T                 (6 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_3_5T               (7 << BIT_DQS_MDOE_SHIFT)

//#endif

#endif /* __eMMC_G2P_UBOOT__ */
