/*
 * xHCI host controller driver
 *
 * Copyright (C) 2013 MStar Inc.
 *
 */

#ifndef _XHCI_MSTAR_31023_H
#define _XHCI_MSTAR_31023_H

#include <ehci-mstar-31023.h>

// ----- Don't modify it !----------
#if defined(CONFIG_ARM) || defined(CONFIG_ARM64)
#define XHCI_PA_PATCH   1
#else
#define XHCI_PA_PATCH   0
#endif
#define XHCI_FLUSHPIPE_PATCH  1
//------------------------------

#define XHCI_CHIRP_PATCH  1
#define ENABLE_XHCI_SSC   1

#if (ENABLE_XHCI_SSC)
#if defined(CONFIG_MSTAR_MANHATTAN) || \
	defined(CONFIG_MSTAR_KANO)
#define XHCI_SSC_TX_SYNTH_SET_C0		0x9374
#define XHCI_SSC_TX_SYNTH_SET_C2		0x18
#define XHCI_SSC_TX_SYNTH_STEP_C4		0x7002
#define XHCI_SSC_TX_SYNTH_SPAN_C6		0x04D8
#endif
#endif

#define XHCI_TX_SWING_PATCH  1

//------ for test -----------------
//#define XHCI_CURRENT_SHARE_PATCH 0   //Only for USB3; will cause USB2 chirp handshake fail. 
#define XHCI_ENABLE_DEQ  0
#define XHCI_ENABLE_TESTBUS  0
//--------------------------------

//Inter packet delay setting for all chips
#define XHCI_IPACKET_DELAY_PATCH

#define XHCI_DISABLE_COMPLIANCE
#define XHCI_DISABLE_TESTMODE
#define XHCI_SSDISABLED_PATCH
#define XHCI_HC_RESET_PATCH

#if defined(CONFIG_MSTAR_NAPOLI) || defined(CONFIG_MSTAR_MADISON)
#define MSTAR_LOST_SLOT_PATCH 1
#else
#define MSTAR_LOST_SLOT_PATCH 0
#endif

#define XHCI_TX_ERR_EVENT_PATCH

#define XHCI_ENABLE_PPC

#if defined(CONFIG_MSTAR_MONACO) || \
	defined(CONFIG_MSTAR_MUJI)
#define XHCI_SSC_QUIRK_PATCH
#endif

#if defined(CONFIG_MSTAR_KANO)
#define XHCI_ENABLE_PD_OVERRIDE
#endif

//--------  Setting option  -----------

#if defined(CONFIG_MSTAR_AGATE)
#error "Not support Agate"
#endif

#if !defined(CONFIG_MSTAR_AGATE)
	//Enable it after Agate
	#define XHCI_ENABLE_240MHZ
	#if defined(CONFIG_MSTAR_EIFFEL)
	//Fix it for Eiffel Only.
	#define XHCI_USE_120_PHASE
	#endif
#endif

#if !( defined(CONFIG_MSTAR_AGATE)	 || \
		defined(CONFIG_MSTAR_EDISON)  || \
		defined(CONFIG_MSTAR_KAISER)  || \
		defined(CONFIG_MSTAR_KAISERS) )
		//Enable it after Edison
#define XHCI_ENABLE_LASTDOWNZ
#endif

#if defined(CONFIG_MSTAR_NIKE) || \
    defined(CONFIG_MSTAR_MADISON)
#define XHCI_MIU1_SEL_BIT30
#endif

#if defined(CONFIG_MSTAR_NIKE)
	//Only for Nike.  Don't gate HW to enter loopback mode.
#define XHCI_ALLOW_LOOPBACK_MODE
#endif

#if defined(CONFIG_MSTAR_KAISER) || \
	defined(CONFIG_MSTAR_KAISERS)
#define XHCI_ENABLE_DPDM_SWAP
#endif
//--------------------------------


// --------- ECO option ---------
#if !defined(CONFIG_MSTAR_AGATE)  //exclude Agate. 
#define XHCI_ENABLE_LOOPBACK_ECO
	#if defined(CONFIG_MSTAR_EDISON) || \
		defined(CONFIG_MSTAR_KAISER) || \
		defined(CONFIG_MSTAR_KAISERS)
		//Only for Edison and Kaiser
		#define LOOPBACK_ECO_OFFSET		0xFF*2-1
		#define LOOPBACK_ECO_BIT		BIT5|BIT6
	#elif defined(CONFIG_MSTAR_EIFFEL) || \
			  defined(CONFIG_MSTAR_EINSTEIN) || \
			  defined(CONFIG_MSTAR_NAPOLI) || \
			  defined(CONFIG_MSTAR_NIKE) || \
			  defined(CONFIG_MSTAR_MADISON) || \
			  defined(CONFIG_MSTAR_MONACO) || \
			  defined(CONFIG_MSTAR_MIAMI) || \
			  defined(CONFIG_MSTAR_MUJI) || \
			  defined(CONFIG_MSTAR_MANHATTAN) || \
			  defined(CONFIG_MSTAR_KANO)
		#define LOOPBACK_ECO_OFFSET		0x20*2
		#define LOOPBACK_ECO_BIT		BIT4|BIT5
	#endif
#endif

#if defined(CONFIG_MSTAR_AGATE)
#define ENABLE_KEEPALIVE_ECO
#endif

#if defined(CONFIG_MSTAR_EDISON)  //Only for Edison. 
#define XHCI_ENABLE_HOTRESET_ECO  //re-enable again to prevent from overwitting by sboot PPC function.
#endif

#if defined(CONFIG_MSTAR_EDISON)  //Only for Edison. 
#define XHCI_DISABLE_LS
#define XHCI_ENABLE_LS_UEVENT
#endif
//--------------------------------


//--------  U3 PHY IP  -----------
#if defined(CONFIG_MSTAR_EINSTEIN) || \
	defined(CONFIG_MSTAR_NAPOLI) || \
	defined(CONFIG_MSTAR_MIAMI)
	#define XHCI_PHY_MT28	
#elif defined(CONFIG_MSTAR_MONACO) || \
	defined(CONFIG_MSTAR_MUJI) || \
	defined(CONFIG_MSTAR_MANHATTAN) || \
	defined(CONFIG_MSTAR_KANO)
	#define XHCI_PHY_MS28	
#endif

#if defined(CONFIG_MSTAR_EINSTEIN) || \
	defined(CONFIG_MSTAR_NAPOLI)
#define XHCI_HS_FORCE_CURRENT	
#endif

#ifdef XHCI_PHY_MS28
#define GCR_USB3RX0_RCTRL		(0x08*2)
#define GCR_USB3TX0_RT			(0x10*2)
#define GCR_USB3RX1_RCTRL		(0x08*2)
#define GCR_USB3TX1_RT			(0x10*2)

#if defined(CONFIG_MSTAR_MONACO)
#define USB30RX0_EFUSE_BITOFFSET	0
#define USB30TX0_EFUSE_BITOFFSET	8
#define USB30RX1_EFUSE_BITOFFSET	16
#define USB30TX1_EFUSE_BITOFFSET	24
#else
#define USB30RX0_EFUSE_BITOFFSET	8
#define USB30TX0_EFUSE_BITOFFSET	0
#define USB30RX1_EFUSE_BITOFFSET	24
#define USB30TX1_EFUSE_BITOFFSET	16
#endif

#endif

#if defined(CONFIG_MSTAR_EINSTEIN) || \
	defined(CONFIG_MSTAR_NAPOLI) || \
	defined(CONFIG_MSTAR_MONACO) || \
	defined(CONFIG_MSTAR_MIAMI) || \
	defined(CONFIG_MSTAR_MUJI) || \
	defined(CONFIG_MSTAR_MANHATTAN)
#define XHCI_PHY_EFUSE	
#endif

#if defined(CONFIG_MSTAR_MONACO) || \
	defined(CONFIG_MSTAR_MIAMI) || \
	defined(CONFIG_MSTAR_MUJI) || \
	defined(CONFIG_MSTAR_KANO)
#define XHCI_2PORTS
#endif

#if defined(CONFIG_MSTAR_MONACO) || \
	defined(CONFIG_MSTAR_MIAMI) || \
	defined(CONFIG_MSTAR_MUJI) || \
	defined(CONFIG_MSTAR_MANHATTAN) || \
	defined(CONFIG_MSTAR_KANO)
#define XHCI_COMPANION
#endif

#if defined(XHCI_2PORTS)
#define XHCI_SSDISABLE_POWERDOWN_PATCH
#endif

#if defined(CONFIG_MSTAR_MUJI)
#define XHCI_PHY_ENABLE_RX_LOCK
#endif

#if defined(CONFIG_MSTAR_MUJI)
#define XHCI_PWS_P2
#endif

//--------------------------------

#if defined(CONFIG_ARM64)
	extern ptrdiff_t mstar_pm_base;
	#define _MSTAR_PM_BASE         (mstar_pm_base)
#elif defined(CONFIG_ARM)
	#define _MSTAR_PM_BASE         0xFD000000
#else
	#define _MSTAR_PM_BASE         0xBF000000
#endif

#if defined(CONFIG_MSTAR_EDISON)  || \
    defined(CONFIG_MSTAR_AGATE)   || \
	defined(CONFIG_MSTAR_NIKE)    || \
    defined(CONFIG_MSTAR_MADISON)
#define _MSTAR_U3PHY_DTOP_BASE (_MSTAR_USB_BASEADR+(0x22100*2))
#define _MSTAR_U3PHY_ATOP_BASE (_MSTAR_USB_BASEADR+(0x22200*2))
#define _MSTAR_U3UTMI_BASE     (_MSTAR_USB_BASEADR+(0x22300*2))
//#define _MSTAR_U3INDCTL_BASE   (_MSTAR_USB_BASEADR+(0x22400*2))
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22500*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))
#define _MSTAR_U3BC_BASE       (_MSTAR_USB_BASEADR+(0x23660*2))
#endif

#if defined(CONFIG_MSTAR_EIFFEL) 
#define _MSTAR_U3PHY_DTOP_BASE (_MSTAR_USB_BASEADR+(0x22100*2))
#define _MSTAR_U3PHY_ATOP_BASE (_MSTAR_USB_BASEADR+(0x22200*2))
#define _MSTAR_U3UTMI_BASE     (_MSTAR_USB_BASEADR+(0x22300*2))
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22500*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))
#define _MSTAR_U3BC_BASE       (_MSTAR_USB_BASEADR+(0x236C0*2))
#endif

#if defined(CONFIG_MSTAR_KAISER)  || \
	defined(CONFIG_MSTAR_KAISERS)
#define _MSTAR_U3PHY_DTOP_BASE (_MSTAR_USB_BASEADR+(0x22C00*2))
#define _MSTAR_U3PHY_ATOP_BASE (_MSTAR_USB_BASEADR+(0x22D00*2))
#define _MSTAR_U3UTMI_BASE     (_MSTAR_USB_BASEADR+(0x22B00*2))
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22000*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))
#define _MSTAR_U3BC_BASE       (_MSTAR_USB_BASEADR+(0x22FC0*2))
#endif

#if defined(CONFIG_MSTAR_EINSTEIN) || \
	defined(CONFIG_MSTAR_NAPOLI)
#define _MSTAR_U3PHY_ATOP_M0_BASE (_MSTAR_USB_BASEADR+(0x22100*2))
#define _MSTAR_U3PHY_ATOP_M1_BASE (_MSTAR_USB_BASEADR+(0x22200*2))
#define _MSTAR_U3PHY_DTOP_M0_BASE (_MSTAR_USB_BASEADR+(0x11C00*2))
#define _MSTAR_U3PHY_DTOP_M1_BASE (_MSTAR_USB_BASEADR+(0x11D00*2))
#define _MSTAR_U3UTMI_BASE     (_MSTAR_USB_BASEADR+(0x22300*2))
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22500*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))
#define _MSTAR_U3BC_BASE       (_MSTAR_USB_BASEADR+(0x23680*2))
#endif

#if defined(CONFIG_MSTAR_MONACO) || \
	defined(CONFIG_MSTAR_MUJI) || \
	defined(CONFIG_MSTAR_KANO)
#define _MSTAR_U3PHY_P0_DTOP_BASE (_MSTAR_USB_BASEADR+(0x11C00*2))
#define _MSTAR_U3PHY_P0_ATOP_BASE (_MSTAR_USB_BASEADR+(0x22100*2))
#define _MSTAR_U3PHY_P1_DTOP_BASE (_MSTAR_USB_BASEADR+(0x53300*2))
#define _MSTAR_U3PHY_P1_ATOP_BASE (_MSTAR_USB_BASEADR+(0x53500*2))
#define _MSTAR_U3UTMI_BASE     0
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22500*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))
#define _MSTAR_U3BC_BASE       0
#endif

#if defined(CONFIG_MSTAR_MIAMI)
#define _MSTAR_U3PHY_P0_ATOP_M0_BASE (_MSTAR_USB_BASEADR+(0x22100*2))
#define _MSTAR_U3PHY_P0_ATOP_M1_BASE (_MSTAR_USB_BASEADR+(0x22200*2))
#define _MSTAR_U3PHY_P0_DTOP_M0_BASE (_MSTAR_USB_BASEADR+(0x11C00*2))
#define _MSTAR_U3PHY_P0_DTOP_M1_BASE (_MSTAR_USB_BASEADR+(0x11D00*2))
#define _MSTAR_U3PHY_P1_ATOP_M0_BASE (_MSTAR_USB_BASEADR+(0x53500*2))
#define _MSTAR_U3PHY_P1_ATOP_M1_BASE (_MSTAR_USB_BASEADR+(0x53600*2))
#define _MSTAR_U3PHY_P1_DTOP_M0_BASE (_MSTAR_USB_BASEADR+(0x53300*2))
#define _MSTAR_U3PHY_P1_DTOP_M1_BASE (_MSTAR_USB_BASEADR+(0x53400*2))

#define _MSTAR_U3UTMI_BASE     0
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22500*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))
#define _MSTAR_U3BC_BASE       0
#endif

#if defined(CONFIG_MSTAR_MANHATTAN)
#define _MSTAR_U3PHY_DTOP_BASE (_MSTAR_USB_BASEADR+(0x11C00*2))
#define _MSTAR_U3PHY_ATOP_BASE (_MSTAR_USB_BASEADR+(0x22100*2))
#define _MSTAR_U3UTMI_BASE     0
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22500*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))
#define _MSTAR_U3BC_BASE       (_MSTAR_USB_BASEADR+(0x23680*2))
#endif

#if !defined(XHCI_COMPANION)

#if defined(CONFIG_MSTAR_AGATE) || \
	defined(CONFIG_MSTAR_EDISON) || \
	defined(CONFIG_MSTAR_KANO)
	#define XHC_HSPORT_OFFSET	0x420
	#define XHC_SSPORT_OFFSET	0x440
#else
	#define XHC_HSPORT_OFFSET	0x420
	#define XHC_SSPORT_OFFSET	0x430
#endif

#endif

#if defined(XHCI_PHY_EFUSE)
	#if defined(CONFIG_MSTAR_EINSTEIN) || \
		defined(CONFIG_MSTAR_NAPOLI) || \
		defined(CONFIG_MSTAR_MIAMI)
		#define _MSTAR_EFUSE_BASE	(_MSTAR_PM_BASE+(0x2000*2))
		#define XHC_EFUSE_OFFSET	0x26	//bank5E
		#define XHC_EFUSE_FSM		1		//use FSM1
	#endif
	#if defined(CONFIG_MSTAR_MONACO) || \
		defined(CONFIG_MSTAR_MUJI) || \
		defined(CONFIG_MSTAR_MANHATTAN) || \
		defined(CONFIG_MSTAR_KANO)
		#define RTERM_XHC_BANK		(0x50*4)	//bank 0x50
	#endif
#endif

#endif	/* _XHCI_MSTAR_31023_H */

