//------------------------------------------------------------------------------
//
// Copyright (c) 2008 MStar Semiconductor, Inc.  All rights reserved.
//
//------------------------------------------------------------------------------
// FILE
//      ms_otg.h
//
// DESCRIPTION
//
// HISTORY
//
//------------------------------------------------------------------------------
#ifndef _UDC_MSTAR_31023_H
#define _UDC_MSTAR_31023_H

/******************************************************************************
 * SW patch config
 ******************************************************************************/
//***** for Cedric C3 patch*****//
#if	defined(CONFIG_ARCH_CEDRIC)
#define NO_DOUBLE_BUFF_CONFIG_PATCH   /// EP3 FIFO RAM size has only 64 bytes; can't use ping-pong buffer   
#endif
#if	defined(CONFIG_ARCH_CEDRIC)
#define DELAY_RECV_PKT_PATCH	//need to wait a SOF time frame to let hw set packet number
#endif

#if defined(CONFIG_MSTAR_CLIPPERS)	
#define DRAM_MORE_THAN_1G_PATCH	//Clippers HW switch miu0/miu1 at 1G, if miu0 > 1G, HW will access wrong place
#endif
/******************************************************************************
 * HW eco config
 ******************************************************************************/
#if defined(CONFIG_MSTAR_KAISER) || defined(CONFIG_MSTAR_KAISERS) || defined(CONFIG_MSTAR_CLIPPERS)
#define AHB_WDATA_CHANGES_WITHOUT_HREADY_ECO	
#endif
#if defined(CONFIG_MSTAR_KAISER) || defined(CONFIG_MSTAR_KAISERS) || defined(CONFIG_MSTAR_CLIPPERS)
#define SHORTPKT_STATUS_CLEAR_ECO
#endif
#if defined(CONFIG_MSTAR_KAISER) || defined(CONFIG_MSTAR_KAISERS) || defined(CONFIG_MSTAR_CLIPPERS)
#define	MONKEY_TEST_ECO	//avoid device can not enter high speed mode
#endif
#if defined(CONFIG_MSTAR_KAISER) || defined(CONFIG_MSTAR_KAISERS) || defined(CONFIG_MSTAR_CLIPPERS)
#define ENABLE_TX_RX_RESET_ECO
#endif
#if defined(CONFIG_MSTAR_CLIPPERS)
#define DMA_RXTX_INT_LAST_DONE_ECO
#endif
#if defined(CONFIG_MSTAR_CLIPPERS)
#define XIU_ACCESS_FAILED_WITH_ECO //(when clk_mcu<120MHz)
#endif

//---- UPLL setting, normally it should be done in sboot
#if defined(CONFIG_MSTAR_KAISERIN)
#define ENABLE_UPLL_SETTING
#endif

//---- change to 55 interface
#if !defined(CONFIG_MSTAR_AMBER3) && \
	!defined(CONFIG_MSTAR_AMETHYST)
	#define ENABLE_UTMI_55_INTERFACE
#endif

/******************************************************************************
 * HW config
 ******************************************************************************/
#if	defined(CONFIG_MSTAR_KAISER) || \
	defined(CONFIG_MSTAR_KAISERS) 
#define MAX_USB_DMA_CHANNEL  1
#else
#define MAX_USB_DMA_CHANNEL  2
#endif

#define MAX_EP_NUMBER	4

static const char ep0name [] = "ep0";
static const char *const ep_name[] = {
    ep0name,                                /* everyone has ep0 */
#if defined(CONFIG_ARCH_CEDRIC)	|| \
	defined(CONFIG_MSTAR_KAISER) || \
	defined(CONFIG_MSTAR_KAISERS)	
    /* msb250x four bidirectional bulk endpoints */
    "ep1in-bulk", "ep2out-bulk", "ep3in-int",
#else
	"ep1in-bulk", "ep2out-bulk", "ep3in-int",
#endif
};
#define MSB250X_ENDPOINTS ARRAY_SIZE(ep_name)

//---USB device switch pad to CID enable
#if defined(CONFIG_MSTAR_CLIPPERS)
#define CID_ENABLE
#endif

//------RIU, UTMI, USBC and OTG base address -----------------------------
#if	defined(CONFIG_ARCH_CEDRIC)
#define RIU_BASE           0xfd200000
#define UTMI_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x3A80) 
#define USBC_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x0700)
#define OTG0_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x51000)
#endif

#if defined(CONFIG_MSTAR_KAISER) || \
	defined(CONFIG_MSTAR_KAISERS) 	
/*---port2 support device---*/
#define RIU_BASE           0xfd200000
#define UTMI_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x2A00)
#define USBC_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x10200)
#define OTG0_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x11700)
#define BC_BASE_ADDR	   GET_UDC_REG_ADDR(RIU_BASE, 0x22F80)
#endif
#if defined(CONFIG_MSTAR_CLIPPERS)
/*---port2 support device---*/
#define RIU_BASE           0xfd200000
#define UTMI_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x3900) 
#define USBC_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x13800)
#define OTG0_BASE_ADDR     GET_UDC_REG_ADDR(RIU_BASE, 0x51000)
#define BC_BASE_ADDR	   GET_UDC_REG_ADDR(RIU_BASE, 0x23640)
#endif
#endif  /* define _UDC_MSTAR_31023_H */
