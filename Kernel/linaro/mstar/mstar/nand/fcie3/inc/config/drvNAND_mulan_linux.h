#ifndef __UNFD_MULAN_LINUX_H__
#define __UNFD_MULAN_LINUX_H__

#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <asm/io.h>

#define REG(Reg_Addr)                       (*(volatile U16*)(Reg_Addr))
#define REG_OFFSET_SHIFT_BITS               2
#define GET_REG_ADDR(x, y)                  (x+((y)<<REG_OFFSET_SHIFT_BITS))

#define REG_WRITE_UINT16(reg_addr, val)     REG(reg_addr) = val
#define REG_READ_UINT16(reg_addr, val)      val = REG(reg_addr)
#define HAL_WRITE_UINT16(reg_addr, val)     (REG(reg_addr) = val)
#define HAL_READ_UINT16(reg_addr, val)      val = REG(reg_addr)
#define REG_SET_BITS_UINT16(reg_addr, val)  REG(reg_addr) |= (val)
#define REG_CLR_BITS_UINT16(reg_addr, val)  REG(reg_addr) &= ~(val)
#define REG_W1C_BITS_UINT16(reg_addr, val)  REG_WRITE_UINT16(reg_addr, REG(reg_addr)&(val))


//=====================================================
//PM banks Definition
//=====================================================

#define RIU_PM_BASE                         0xBF000000

#define REG_BANK_PM_SLEEP                   (0x700)
#define REG_BANK_TIMER0                     (0x1800)

#define PM_SLEEP_REG_BASE_ADDR              GET_REG_ADDR(RIU_PM_BASE, REG_BANK_PM_SLEEP)
#define TIMER0_REG_BASE_ADDR                GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER0)

//=====================================================
//Non-PM banks Definition
//=====================================================

#define RIU_BASE                            0xBF200000

#define REG_BANK_CLKGEN                     (0x580)
#define REG_BANK_CHIPTOP                    (0xF00)
#define REG_BANK_FCIE0                      (0x8980)
#define REG_BANK_FCIE2                      (0x8A00)
#define REG_BANK_FCIEPOWERSAVEMODE          (0x8A80)
#define REG_BANK_EMMC_PLL                   (0x11F80)
#define REG_BANK_TZPC_NON_PM                (0x11C80)

#define MPLL_CLK_REG_BASE_ADDR              GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN)
#define CHIPTOP_REG_BASE_ADDR               GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)
#define FCIE_REG_BASE_ADDR                  GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE_NC_CIFD_BASE                   GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_NC_WBUF_CIFD_BASE 	GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)	
#define FCIE_NC_RBUF_CIFD_BASE 	GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2 + 0x20)	

#define FCIE_POWEER_SAVE_MODE_BASE          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIEPOWERSAVEMODE)
#define EMMC_PLL_REG_BASE_ADDR              GET_REG_ADDR(RIU_BASE, REG_BANK_EMMC_PLL)

#define FCIE_WRITE_PROTECT_SUPPORT          1
#if defined (FCIE_WRITE_PROTECT_SUPPORT) && FCIE_WRITE_PROTECT_SUPPORT
#define TZPC_NON_PM_REG_BASE_ADDR           GET_REG_ADDR(RIU_BASE, REG_BANK_TZPC_NON_PM)

#define REG_FCIE_WP_START_ROW_ADDR_L        GET_REG_ADDR(TZPC_NON_PM_REG_BASE_ADDR, 0x60)
#define REG_FCIE_WP_START_ROW_ADDR_H        GET_REG_ADDR(TZPC_NON_PM_REG_BASE_ADDR, 0x61)
#define REG_FCIE_WP_END_ROW_ADDR_L          GET_REG_ADDR(TZPC_NON_PM_REG_BASE_ADDR, 0x62)
#define REG_FCIE_WP_END_ROW_ADDR_H          GET_REG_ADDR(TZPC_NON_PM_REG_BASE_ADDR, 0x63)
#define REG_FCIE_WP_ENABLE                  GET_REG_ADDR(TZPC_NON_PM_REG_BASE_ADDR, 0x64)
#define BIT_FCIE_WP_ENABLE                  BIT0

#define FCIE_WRITE_PROTECT_ENABLE()         (REG(REG_FCIE_WP_ENABLE) & BIT_FCIE_WP_ENABLE)
#endif

//=====================================================
//FCIE IP Definition
//=====================================================

#define REG50_ECC_CTRL_INIT_VALUE           0

#define UNFD_ST_PLAT                        0x80000000
#define IF_IP_VERIFY                        0 // [CAUTION]: to verify IP and HAL code, defaut 0

#define NC_SEL_FCIE5			1
#if NC_SEL_FCIE5
#include "drvNAND_reg_v5.h"
#else
#error "Error! no FCIE registers selected."
#endif

//DDR & Timing Parameters

#define ENABLE_32BIT_MACRO			1
#define ENABLE_8BIT_MACRO				0

#define DDR_NAND_SUPPORT                          0

#define FCIE_LFSR                           1

#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)
#define NC_SET_DDR_MODE()                   REG_WRITE_UINT16(NC_DDR_CTRL, pNandDrv->u16_Reg58_DDRCtrl);
#define NC_CLR_DDR_MODE()                   REG_CLR_BITS_UINT16(NC_DDR_CTRL, BIT_DDR_MASM);
#else
#define NC_SET_DDR_MODE()
#define NC_CLR_DDR_MODE()
#endif

#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)

#if defined(ENABLE_8BIT_MACRO) && ENABLE_8BIT_MACRO
#define DQS_MODE_0P0T		0
#define DQS_MODE_0P5T		1
#define DQS_MODE_1P0T		2
#define DQS_MODE_1P5T		3
#define DQS_MODE_2P0T		4

#define DQS_MODE_TABLE_CNT	5
#define DQS_MODE_SEARCH_TABLE	{DQS_MODE_1P5T, DQS_MODE_2P0T, DQS_MODE_0P5T, DQS_MODE_1P0T, DQS_MODE_0P0T}

#elif defined(ENABLE_32BIT_MACRO) && ENABLE_32BIT_MACRO
#define DQS_MODE_TABLE_CNT		5
#define DQS_MODE_SEARCH_TABLE 	{0, 1, 2, 3, 4}

#define SKEW_CLK_PHASE_CNT		3

#endif

#define	NC_ONFI_DEFAULT_TRR                 12
#define	NC_ONFI_DEFAULT_TCS                 7
#define NC_ONFI_DEFAULT_TWW                 8
#define NC_ONFI_DEFAULT_TWHR                5
#define NC_ONFI_DEFAULT_TADL                6
#define NC_ONFI_DEFAULT_TCWAW               4
#define	NC_ONFI_DEFAULT_RX40CMD             4
#define	NC_ONFI_DEFAULT_RX40ADR             7
#define	NC_ONFI_DEFAULT_RX56                10

#define	NC_TOGGLE_DEFAULT_TRR               8
#define	NC_TOGGLE_DEFAULT_TCS               6
#define NC_TOGGLE_DEFAULT_TWW               7
#define	NC_TOGGLE_DEFAULT_TWHR              5
#define	NC_TOGGLE_DEFAULT_TADL              7
#define	NC_TOGGLE_DEFAULT_TCWAW             2
#define	NC_TOGGLE_DEFAULT_RX40CMD           4
#define	NC_TOGGLE_DEFAULT_RX40ADR           5
#define	NC_TOGGLE_DEFAULT_RX56              15
#endif

#define	NC_SDR_DEFAULT_TRR                  7
#define	NC_SDR_DEFAULT_TCS                  6
#define NC_SDR_DEFAULT_TWW                  5
#define	NC_SDR_DEFAULT_TWHR                 4
#define	NC_SDR_DEFAULT_TADL                 8
#define	NC_SDR_DEFAULT_TCWAW                2
#define	NC_SDR_DEFAULT_RX40CMD              4
#define	NC_SDR_DEFAULT_RX40ADR              5
#define	NC_SDR_DEFAULT_RX56                 5

#define	NC_INST_DELAY                       1
#define	NC_HWCMD_DELAY                      1
#define	NC_TRR_TCS                          1
#define	NC_TWHR_TCLHZ                       1
#define	NC_TCWAW_TADL                       1

//PAD & SHARE & MISC Definition

#define NAND_PAD_BYPASS_MODE                1
#define NAND_PAD_TOGGLE_MODE                2
#define NAND_PAD_ONFI_SYNC_MODE             3

#define MACRO_TYPE_8BIT			1
#define MACRO_TYPE_32BIT			2

#define IF_FCIE_SHARE_PINS                  0 // 1: need to nand_pads_switch at HAL's functions.
#define IF_FCIE_SHARE_CLK                   0 // 1: need to nand_clock_setting at HAL's functions.
#define IF_FCIE_SHARE_IP                    1 // A3 might use SD

#define ENABLE_NAND_INTERRUPT_MODE          1

#define ENABLE_NAND_POWER_SAVING_MODE       1
#define ENABLE_NAND_POWER_SAVING_DEGLITCH   0

#define NAND_DRIVER_ROM_VERSION             0 // to save code size
#define AUTO_FORMAT_FTL                     0

#define ENABLE_CUS_READ_ENHANCEMENT         0

#define __VER_UNFD_FTL__                    0

//FCIE5 Definition

#define SPARE_DMA_ADDR_AUTO_INC 		0 //spare would increase its address when dma

//=====================================================
// Nand Driver configs
//=====================================================
#define NAND_BUF_USE_STACK                  0
#define NAND_ENV_FPGA                       1
#define NAND_ENV_ASIC                       2
#ifdef __FPGA_MODE__
#define NAND_DRIVER_ENV                     NAND_ENV_FPGA
#else
#define NAND_DRIVER_ENV                     NAND_ENV_ASIC
#endif

#define UNFD_CACHE_LINE                     0x80
//=====================================================
// tool-chain attributes
//=====================================================
#define UNFD_PACK0
#define UNFD_PACK1                          __attribute__((__packed__))
#define UNFD_ALIGN0
#define UNFD_ALIGN1                         __attribute__((aligned(UNFD_CACHE_LINE)))

//=====================================================
// debug option
//=====================================================
#define NAND_TEST_IN_DESIGN                 0      /* [CAUTION] */

#ifndef NAND_DEBUG_MSG
#define NAND_DEBUG_MSG                      1
#endif

/* Define trace levels. */
#define UNFD_DEBUG_LEVEL_ERROR              (1)    /* Error condition debug messages. */
#define UNFD_DEBUG_LEVEL_WARNING            (2)    /* Warning condition debug messages. */
#define UNFD_DEBUG_LEVEL_HIGH               (3)    /* Debug messages (high debugging). */
#define UNFD_DEBUG_LEVEL_MEDIUM             (4)    /* Debug messages. */
#define UNFD_DEBUG_LEVEL_LOW                (5)    /* Debug messages (low debugging). */

/* Higer debug level means more verbose */
#ifndef UNFD_DEBUG_LEVEL
#define UNFD_DEBUG_LEVEL                    UNFD_DEBUG_LEVEL_WARNING
#endif

#if defined(NAND_DEBUG_MSG) && NAND_DEBUG_MSG
#define nand_print_tmt
#define nand_printf                         printk
#define nand_debug(dbg_lv, tag, str, ...)           \
    do {                                            \
        if (dbg_lv > UNFD_DEBUG_LEVEL)              \
            break;                                  \
        else {                                      \
            if (tag)                                \
            {                                       \
                nand_printf(KERN_ERR"[%s]\t",__func__);     \
            }                                       \
                                                    \
            nand_printf(KERN_ERR str, ##__VA_ARGS__);        \
        }                                           \
    } while(0)
#else /* NAND_DEBUG_MSG */
#define nand_printf(...)
#define nand_debug(enable, tag, str, ...)   {}
#endif /* NAND_DEBUG_MSG */

static __inline void nand_assert(int condition)
{
    if (!condition) {
        panic("%s\n UNFD Assert(%d)\n", __func__, __LINE__);
    }
}

#define nand_die()      \
    do {                \
        nand_assert(0); \
    } while(0);

#define nand_stop() \
    while(1)  nand_reset_WatchDog();

//=====================================================
// HW Timer for Delay
//=====================================================
#define TIMER0_ENABLE                       GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x20)
#define TIMER0_HIT                          GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x21)
#define TIMER0_MAX_LOW                      GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x22)
#define TIMER0_MAX_HIGH                     GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x23)
#define TIMER0_CAP_LOW                      GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x24)
#define TIMER0_CAP_HIGH                     GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x25)

#define HW_TIMER_DELAY_1us                  1
#define HW_TIMER_DELAY_10us                 10
#define HW_TIMER_DELAY_100us                100
#define HW_TIMER_DELAY_1ms                  (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms                  (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms                 (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms                (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms                (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s                   (1000 * HW_TIMER_DELAY_1ms)

extern void delay_us(unsigned us);
extern U32  nand_hw_timer_delay(U32 u32usTick);

//=====================================================
// Pads Switch
//=====================================================
#define REG_NAND_MODE_MASK					(BIT9|BIT8|BIT7)
#define NAND_MODE1							(BIT7)				//PCM_D
#define NAND_MODE2							(BIT8)				//PCM_A
#define NAND_MODE3							(BIT8|BIT7)			//NAND_AD
#define NAND_MODE4							(BIT9)				//NAND_AD + DQS

#define reg_pcm_a_pe                        GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x0C)
#define reg_pcm_d_pe                        GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x0B)

#define reg_allpad_in           			GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)
#define reg_nf_en                           GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)
#define reg_emmc_config                     GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x6E)
#define reg_sd_config                       GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x57)

#define reg_emmcpll_rx60		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x60)
#define reg_emmcpll_rx63		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x63)
#define reg_emmcpll_rx6d		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x6d)
#define reg_emmcpll_rx70		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x70)
#define reg_emmcpll_rx71		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x71)
#define reg_emmcpll_rx72		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x72)
#define reg_emmcpll_rx73		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x73)
#define reg_emmcpll_rx74		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x74)

//@FIXME
#if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
#define REG_NAND_DQS_UL                     GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x0F)
#define BIT_DQS_PULL_H                      BIT8
#define NC_DQS_PULL_H()                     REG_SET_BITS_UINT16(REG_NAND_DQS_UL, BIT_DQS_PULL_H)
#define NC_DQS_PULL_L()                     REG_CLR_BITS_UINT16(REG_NAND_DQS_UL, BIT_DQS_PULL_H)
#else
#define NC_DQS_PULL_H()
#define NC_DQS_PULL_L()
#endif

extern U32 nand_pads_switch(U32 u32EnableFCIE);
extern U32 nand_check_DDR_pad(void);

//=====================================================
// set FCIE clock
//=====================================================
#define DECIDE_CLOCK_BY_NAND			1

#define NFIE_CLK_MASK                       (BIT13|BIT12|BIT11|BIT10)
#define NFIE_CLK_3M                         (0<<10)
#define NFIE_CLK_12M                        (1<<10)
#define NFIE_CLK_15_5M                      (2<<10)
#define NFIE_CLK_18M                        (3<<10)
#define NFIE_CLK_21_5M                      (4<<10)
#define NFIE_CLK_27M                        (5<<10)
#define NFIE_CLK_36M                        (6<<10)
#define NFIE_CLK_40M                        (7<<10)
#define NFIE_CLK_48M                        (8<<10)
#define NFIE_CLK_54M                        (9<<10)
#define NFIE_CLK_60M                        (10<<10)
#define NFIE_CLK_86_25M                     (11<<10)
#define NFIE_CLK_75K                        (12<<10)
#define NFIE_CLK_80M                        (13<<10)

#define NFIE_CLK_TABLE_CNT	13
#define NFIE_CLK_TABLE	{	 NFIE_CLK_3M, NFIE_CLK_12M, NFIE_CLK_15_5M, \
							NFIE_CLK_18M, NFIE_CLK_21_5M, NFIE_CLK_27M, NFIE_CLK_36M, \
							NFIE_CLK_40M, NFIE_CLK_48M, NFIE_CLK_54M, NFIE_CLK_60M, \
							NFIE_CLK_80M, NFIE_CLK_86_25M}

#define NFIE_CLK_TABLE_STR	{	"3M", "12M", "15.5M",\
							"18M", "21.5M", "27M", "36M", \
							"40M", "48M", "54M", "60M", \
							"80M", "86.25M"}

#define NFIE_3M_VALUE		3000000
#define NFIE_12M_VALUE		12000000
#define NFIE_15_5M_VALUE	15500000
#define NFIE_18M_VALUE		18000000
#define NFIE_21_5M_VALUE	21500000
#define NFIE_27M_VALUE		27000000
#define NFIE_36M_VALUE		36000000
#define NFIE_40M_VALUE		40000000
#define NFIE_48M_VALUE		48000000
#define NFIE_54M_VALUE		54000000
#define NFIE_60M_VALUE		60000000
#define NFIE_80M_VALUE		80000000
#define NFIE_86_25M_VALUE	86250000


#define NFIE_CLK_VALUE_TABLE	{	NFIE_3M_VALUE, NFIE_12M_VALUE, NFIE_15_5M_VALUE, NFIE_18M_VALUE, \
							NFIE_21_5M_VALUE, NFIE_27M_VALUE, NFIE_36M_VALUE, NFIE_40M_VALUE, \
							NFIE_48M_VALUE, NFIE_54M_VALUE, NFIE_60M_VALUE, NFIE_80M_VALUE, \
							NFIE_86_25M_VALUE}


/*Define 1 cycle Time for each clock note: define value must be the (real value -1)*/
#define NFIE_1T_3M			333
#define NFIE_1T_12M			83
#define NFIE_1T_15_5M		64
#define NFIE_1T_18M			55
#define NFIE_1T_21_5M		46
#define NFIE_1T_27M			37
#define NFIE_1T_36M			27
#define NFIE_1T_40M			25
#define NFIE_1T_48M			20
#define NFIE_1T_54M			18
#define NFIE_1T_60M			16
#define NFIE_1T_80M			12
#define NFIE_1T_86_25M		11


#define NFIE_1T_TABLE		{	NFIE_1T_3M, NFIE_1T_12M, NFIE_1T_15_5M, \
							NFIE_1T_18M, NFIE_1T_21_5M, NFIE_1T_27M, NFIE_1T_36M, \
							NFIE_1T_40M, NFIE_1T_48M, NFIE_1T_54M, NFIE_1T_60M, \
							NFIE_1T_80M, NFIE_1T_86_25M}

#define DUTY_CYCLE_PATCH                    0 // 1: to enlarge low width for tREA's worst case of 25ns
#if DUTY_CYCLE_PATCH
#define FCIE3_SW_DEFAULT_CLK                NFIE_1T_86_25M
#define FCIE_REG41_VAL                      ((2<<9)|(2<<3)) // RE,WR pulse, Low:High=3:1
#define REG57_ECO_FIX_INIT_VALUE            0
#else
#define FCIE3_SW_DEFAULT_CLK                NFIE_CLK_54M
#define FCIE_REG41_VAL                      0               // RE,WR pulse, Low:High=1:1
#define REG57_ECO_FIX_INIT_VALUE            BIT_NC_LATCH_DATA_1_5_T	// delay 1.0T
#endif
#define FCIE3_SW_SLOWEST_CLK                NFIE_CLK_12M

#define NAND_SEQ_ACC_TIME_TOL               16 //in unit of ns

#define reg_ckg_fcie				GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x31)
#define reg_clk_nfie_div4_en        GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x65)
#define BIT_CLK_DIV4_ENABLE         BIT14

extern U32  nand_clock_setting(U32 u32ClkParam);

extern void nand_DumpPadClk(void);

extern U32 NC_CheckStorageType(void);

extern U32 NC_CheckBlankPage(U8 * pu8_Buffer);

extern U32 nand_translate_Spare_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int mode);

extern void nand_flush_cache_post_read(U32 u32_DMAAddr, U32 u32_ByteCnt);
//=====================================================
// transfer DMA Address
//=====================================================
#define MIU_BUS_WIDTH_BITS                  3 // Need to confirm

#define MIU_CHECK_LAST_DONE                 1

//=====================================================
// misc
//=====================================================
//#define BIG_ENDIAN
#define LITTLE_ENDIAN

typedef struct NAND_DRIVER_PLATFORM_DEPENDENT
{
    U8 *pu8_PageSpareBuf;
    U8 *pu8_PageDataBuf;
    U32	u32_DMAAddrOffset;
    U32	u32_RAMBufferOffset;
    U32	u32_RAMBufferLen;

}NAND_DRIVER_PLAT_CTX, *P_NAND_DRIVER_PLAT;

#define NANDINFO_ECC_TYPE                   ECC_TYPE_40BIT1KB
#define malloc(x)                           kmalloc(x, GFP_KERNEL)
#define free                                kfree

#endif
