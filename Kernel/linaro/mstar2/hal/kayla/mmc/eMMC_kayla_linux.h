#ifndef __eMMC_KAYLA_LINUX__
#define __eMMC_KAYLA_LINUX__

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
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/kthread.h>
#include "chip_int.h"
#include "chip_setup.h"

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
#define RIU_PM_BASE                     0xBF000000
#define RIU_BASE                        0xBF200000

#define REG_BANK_FCIE0                  0x8980
#define REG_BANK_FCIE1                  0x89E0
#define REG_BANK_FCIE2                  0x8A00

#define FCIE0_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE1_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE1)
#define FCIE2_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)

#define FCIE_REG_BASE_ADDR              FCIE0_BASE
#define FCIE_CIFC_BASE_ADDR             FCIE1_BASE
#define FCIE_CIFD_BASE_ADDR             FCIE2_BASE

#include "eMMC_reg.h"

/* clkgen0 */
#define REG_BANK_CLKGEN0                0x0580
#define CLKGEN0_BASE                    GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN0)

#define reg_ckg_fcie                    GET_REG_ADDR(CLKGEN0_BASE, 0x64)

#define BIT_FCIE_CLK_Gate               BIT0
#define BIT_FCIE_CLK_Inverse            BIT1
#define BIT_FCIE_CLK_MASK               (BIT2|BIT3|BIT4|BIT5)
#define BIT_FCIE_CLK_SHIFT              2
#define BIT_FCIE_CLK_SEL                BIT6 // 1: NFIE, 0: 12MHz

#define BIT_FCIE_CLK_12M                0
#define BIT_FCIE_CLK_36M                1
#define BIT_FCIE_CLK_40M                2
#define BIT_FCIE_CLK_5M                 3
#define BIT_FCIE_CLK_13M                4
#define BIT_FCIE_CLK_18M                5
#define BIT_FCIE_CLK_20M                6
#define BIT_FCIE_CLK_27M                7
#define BIT_FCIE_CLK_32M                8
#define BIT_FCIE_CLK_43M                9
#define BIT_FCIE_CLK_300K               14
#define BIT_FCIE_CLK_48M                15

#define eMMC_FCIE_VALID_CLK_CNT         12
extern  U8 gau8_FCIEClkSel[];

/* chiptop */
#define REG_BANK_CHIPTOP                0x0F00
#define PAD_CHIPTOP_BASE                GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)

#define reg_emmc_mode                   GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x08)
#define BIT_EMMC_MODE_MASK              (BIT1|BIT0)
#define BIT_EMMC_MODE1                  (BIT0)
#define BIT_EMMC_MODE2                  (BIT1)
#define BIT_EMMC_MODE3                  (BIT1|BIT0)

#define reg_chiptop_0x24                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x24)
#define reg_chiptop_0x25                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x25)

#define reg_chiptop_0x26                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x26)
#define BIT_GPIO8_PE                    BIT14
#define BIT_GPIO8_PS                    BIT15

#define reg_chiptop_0x27                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x27)
#define BIT_GPIO2_PE                    BIT3
#define BIT_GPIO4_PE                    BIT11

#define reg_chiptop_0x2a                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x2a)
#define reg_chiptop_0x2b                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x2b)
#define reg_chiptop_0x2c                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x2c)

#define reg_emmc_swrstz                 GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x36)
#define BIT_EMMC_SWRSTZ_EN              BIT5

#define reg_all_pad_in                  GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x50)
#define BIT_ALL_PAD_IN                  BIT15

#define eMMC_RST_L()                    {REG_FCIE_SETBIT(reg_emmc_swrstz, BIT_EMMC_SWRSTZ_EN);\
                                         REG_FCIE_SETBIT(FCIE_REG13h, BIT15);\
                                         REG_FCIE_CLRBIT(FCIE_REG13h, BIT14);}
#define eMMC_RST_H()                    {REG_FCIE_SETBIT(reg_emmc_swrstz, BIT_EMMC_SWRSTZ_EN);\
                                         REG_FCIE_SETBIT(FCIE_REG13h, BIT15);\
                                         REG_FCIE_SETBIT(FCIE_REG13h, BIT14);}

//=====================================================
// API declarations
//=====================================================
extern  U32 eMMC_hw_timer_delay(U32 u32us);
extern  U32 eMMC_hw_timer_sleep(U32 u32ms);

#define eMMC_HW_TIMER_HZ                1000000
#define FCIE_eMMC_DISABLE               0
#define FCIE_eMMC_BYPASS                1
#define FCIE_eMMC_SDR                   2
#define FCIE_eMMC_DDR                   3
#define FCIE_DEFAULT_PAD                FCIE_eMMC_BYPASS

extern  U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type);
extern  U32 eMMC_clock_setting(U16 u16_ClkParam);
extern  U32 eMMC_clock_gating(void);
extern void eMMC_set_WatchDog(U8 u8_IfEnable);
extern void eMMC_reset_WatchDog(void);
extern  U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern  U32 eMMC_DMA_MAP_address(U32 u32_Buffer, U32 u32_ByteCnt, int mode);
extern void eMMC_DMA_UNMAP_address(U32 u32_DMAAddr, U32 u32_ByteCnt, int mode);
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
extern  U32 eMMC_WaitCompleteIntr(uintptr_t u32_RegAddr, U16 u16_WaitEvent, U32 u32_MicroSec);
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
#define eMMC_DrvContext_BLK_0           (eMMC_CIS_BLK_0+6)
#define eMMC_DrvContext_BLK_1           (eMMC_CIS_BLK_0+7)
#define eMMC_ALLRSP_BLK_0               (eMMC_CIS_BLK_0+8)
#define eMMC_ALLRSP_BLK_1               (eMMC_CIS_BLK_0+9)
#define eMMC_BURST_LEN_BLK_0            (eMMC_CIS_BLK_0+10)

#define eMMC_CIS_BLK_END                eMMC_BURST_LEN_BLK_0
// last 1MB in reserved area, use for eMMC test
#define eMMC_TEST_BLK_0                 (eMMC_CIS_BLK_END+1)


//=====================================================
// Driver configs
//=====================================================
#define DRIVER_NAME                     "mstar_mci"
#define eMMC_UPDATE_FIRMWARE            0

#define eMMC_ST_PLAT                    0x80000000

#define IF_IP_VERIFY                    0
#define IF_DETECT_eMMC_DDR_TIMING       0
#define eMMC_IF_TUNING_TTABLE()         (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_TUNING_TTABLE)

#define IF_FCIE_SHARE_IP                1

//------------------------------
#define FICE_BYTE_MODE_ENABLE           1
#define ENABLE_eMMC_INTERRUPT_MODE      1
#define ENABLE_eMMC_RIU_MODE            0

#if ENABLE_eMMC_RIU_MODE
#undef IF_DETECT_eMMC_DDR_TIMING
#define IF_DETECT_eMMC_DDR_TIMING       0
#endif

//------------------------------
#define eMMC_FEATURE_RELIABLE_WRITE     1
#if eMMC_UPDATE_FIRMWARE
#undef  eMMC_FEATURE_RELIABLE_WRITE
#define eMMC_FEATURE_RELIABLE_WRITE     0
#endif

//------------------------------
#define eMMC_RSP_FROM_RAM               1
#define eMMC_BURST_LEN_AUTOCFG          1
#define eMMC_PROFILE_WR                 0
#define eMMC_HOUSEKEEP_THREAD           0

//------------------------------
#define eMMC_SECTOR_BUF_BYTECTN         eMMC_SECTOR_BUF_16KB
extern U8 gau8_eMMC_SectorBuf[];
extern U8 gau8_eMMC_PartInfoBuf[];

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
#define eMMC_DEBUG_LEVEL                eMMC_DEBUG_LEVEL_WARNING
#endif

#if defined(eMMC_DEBUG_MSG) && eMMC_DEBUG_MSG
#define eMMC_printf(fmt, arg...)        printk(KERN_ERR fmt, ##arg)
#define eMMC_debug(dbg_lv, tag, str, ...)\
	do {\
		if (dbg_lv > eMMC_DEBUG_LEVEL)\
			break;\
		else if(eMMC_IF_TUNING_TTABLE())\
			break;\
		else if(eMMC_IF_DISABLE_LOG())\
			break;\
		else {\
			if (tag)\
				eMMC_printf("[ %s() Ln.%u ] ", __FUNCTION__, __LINE__);\
		\
			eMMC_printf(str, ##__VA_ARGS__);\
		}\
	} while(0)
#else /* eMMC_DEBUG_MSG */
#define eMMC_printf(...)
#define eMMC_debug(enable, tag, str, ...)   do{}while(0)
#endif /* eMMC_DEBUG_MSG */

#define eMMC_die(str)\
    {eMMC_printf("eMMC Die: %s() Ln.%u, %s \n", __FUNCTION__, __LINE__, str);\
     panic("\n");}

#define eMMC_stop()\
    while(1)    eMMC_reset_WatchDog();

#define REG_BANK_TIMER1                 0x1810
#define TIMER1_BASE                     GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER1)

#define TIMER1_ENABLE                   GET_REG_ADDR(TIMER1_BASE, 0x00)
#define TIMER1_HIT                      GET_REG_ADDR(TIMER1_BASE, 0x01)
#define TIMER1_MAX_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x02)
#define TIMER1_MAX_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x03)
#define TIMER1_CAP_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x04)
#define TIMER1_CAP_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x05)


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
#define FCIE_SLOW_CLK                   BIT_FCIE_CLK_12M
#define FCIE_DEFAULT_CLK                BIT_FCIE_CLK_48M

//=====================================================
// transfer DMA Address
//=====================================================
#define MIU_BUS_WIDTH_BITS              3 // Need to confirm
/*
 * Important:
 * The following buffers should be large enough for a whole eMMC block
 */
// FIXME, this is only for verifing IP
#define DMA_W_ADDR                      0x80C00000
#define DMA_R_ADDR                      0x80D00000
#define DMA_W_SPARE_ADDR                0x80E00000
#define DMA_R_SPARE_ADDR                0x80E80000
#define DMA_BAD_BLK_BUF                 0x80F00000

#define MIU_CHECK_LAST_DONE             1

//=====================================================
// misc
//=====================================================
//#define BIG_ENDIAN
#define LITTLE_ENDIAN

#if 0 //(defined(BIT_DQS_MODE_MASK) && (BIT_DQS_MODE_MASK != (BIT12|BIT13|BIT14)))

#undef BIT_DQS_MODE_MASK
#undef BIT_DQS_MODE_2T
#undef BIT_DQS_MODE_1_5T
#undef BIT_DQS_MODE_2_5T
#undef BIT_DQS_MODE_1T

#define BIT_DQS_MODE_MASK               (BIT12|BIT13|BIT14)
#define BIT_DQS_MODE_0T                 (0 << BIT_DQS_MODE_SHIFT)
#define BIT_DQS_MODE_0_5T               (1 << BIT_DQS_MODE_SHIFT)
#define BIT_DQS_MODE_1T                 (2 << BIT_DQS_MODE_SHIFT)
#define BIT_DQS_MODE_1_5T               (3 << BIT_DQS_MODE_SHIFT)
#define BIT_DQS_MODE_2T                 (4 << BIT_DQS_MODE_SHIFT)
#define BIT_DQS_MODE_2_5T               (5 << BIT_DQS_MODE_SHIFT)
#define BIT_DQS_MODE_3T                 (6 << BIT_DQS_MODE_SHIFT)
#define BIT_DQS_MODE_3_5T               (7 << BIT_DQS_MODE_SHIFT)

#endif

#define E_IRQ_NFIE                      E_IRQEXPH_NFIE

#endif /* __eMMC_KAYLA_LINUX__ */
