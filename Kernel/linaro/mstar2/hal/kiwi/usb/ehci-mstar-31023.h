/*
 * eHCI host controller driver
 *
 * Copyright (C) 2012 MStar Inc.
 *
 *            Warning
 * This file is only for Mstar ARM
 * core chips later than Amber3.
 *     Kiwi 2015/10/26
 *
 * Date: Aug 2012
 */

#ifndef _EHCI_MSTAR_31023_H
#define _EHCI_MSTAR_31023_H

//------ General constant definition ---------------------------------
#define DISABLE	0
#define ENABLE	1

#define BIT0	(1U<<0)
#define BIT1	(1U<<1)
#define BIT2	(1U<<2)
#define BIT3	(1U<<3)
#define BIT4	(1U<<4)
#define BIT5	(1U<<5)
#define BIT6	(1U<<6)
#define BIT7	(1U<<7)
//--------------------------------------------------------------------



//--------------------------------------------------------------------


//------ RIU base addr and bus-addr to phy-addr translation ----------
	#define MSTAR_PM_BASE      0xbf000000
	#define _MSTAR_USB_BASEADR 0xbf200000
	
	#define MIU0_KSEG2_BASE_ADDR 	0x40000000 //MSTAR_MIU0_BUS_BASE
	#define MIU1_BUS_BASE_ADDR	0xA0000000 //MSTAR_MIU1_BUS_BASE
	#define MIU0_LOW_NONCACHE_SIZE	0x10000000

	// general MIPS physical memory map setting
	#define MIU0_PHY_BASE_ADDR	((unsigned long)0x00000000)
	/* MIU0 1.5G*/
	#define MIU0_SIZE		((unsigned long)0x60000000)

	#define MIU1_PHY_BASE_ADDR	((unsigned long)0x60000000)
	/* MIU1 1G*/
	#define MIU1_SIZE		((unsigned long)0x40000000)

	#define _BUS2PA(A)	\
		((A)<(MIU0_KSEG2_BASE_ADDR+MIU0_SIZE)?	\
			(((A)<MIU0_KSEG2_BASE_ADDR)? (A) :\
			((A)-MIU0_KSEG2_BASE_ADDR+MIU0_PHY_BASE_ADDR)):	\
			((((A)>= MIU1_BUS_BASE_ADDR)&&((A)<MIU1_BUS_BASE_ADDR+MIU1_SIZE))?	\
				((A)-MIU1_BUS_BASE_ADDR+MIU1_PHY_BASE_ADDR): \
				(0xFFFFFFFF)))

	//#define DEBUG_BUS_PA_PATCH
	#ifdef DEBUG_BUS_PA_PATCH
		extern unsigned int BUS2PA(unsigned int A);
	#else
		#define BUS2PA(A) _BUS2PA(A)
	#endif

	/* only non-cache map is considered */
	#define PA2BUS(A)	\
		((((A)>=MIU0_PHY_BASE_ADDR)&&((A)<(MIU0_PHY_BASE_ADDR+MIU0_SIZE)))?	\
			(((A)<MIU0_LOW_NONCACHE_SIZE)? (A) :	\
			((A)-MIU0_PHY_BASE_ADDR+MIU0_KSEG2_BASE_ADDR)):	\
			((((A)>= MIU1_PHY_BASE_ADDR)&&((A)<MIU1_PHY_BASE_ADDR+MIU1_SIZE))?	\
				((A)-MIU1_PHY_BASE_ADDR+MIU1_BUS_BASE_ADDR): \
				(0xFFFFFFFF)))

#define BUS_PA_PATCH
//--------------------------------------------------------------------


//------ UTMI, USBC and UHC base address -----------------------------
//---- Port0
#define _MSTAR_UTMI0_BASE	(_MSTAR_USB_BASEADR+(0x3A80*2))
#define _MSTAR_UHC0_BASE	(_MSTAR_USB_BASEADR+(0x2400*2))
#define _MSTAR_USBC0_BASE	(_MSTAR_USB_BASEADR+(0x0700*2))
#define _MSTAR_BC0_BASE		(_MSTAR_USB_BASEADR+(0x20500*2))
//---- Port1
	#define _MSTAR_UTMI1_BASE	(_MSTAR_USB_BASEADR+(0x3A00*2))
	#define _MSTAR_UHC1_BASE	(_MSTAR_USB_BASEADR+(0x0D00*2))
	#define _MSTAR_USBC1_BASE	(_MSTAR_USB_BASEADR+(0x0780*2))
#define _MSTAR_BC1_BASE		(_MSTAR_USB_BASEADR+(0x20540*2))
//--------------------------------------------------------------------


//------ Hardware ECO enable switch ----------------------------------
//---- 1. cross point
#define ENABLE_LS_CROSS_POINT_ECO
#define LS_CROSS_POINT_ECO_OFFSET	(0x04*2)
#define LS_CROSS_POINT_ECO_BITSET	BIT6

//---- 2. power noise
#define ENABLE_PWR_NOISE_ECO

//---- 3. tx/rx reset clock gating cause XIU timeout
#define ENABLE_TX_RX_RESET_CLK_GATING_ECO
#define TX_RX_RESET_CLK_GATING_ECO_OFFSET	(0x04*2)
#define TX_RX_RESET_CLK_GATING_ECO_BITSET	BIT5

//---- 8. fix pv2mi bridge mis-behavior
#define ENABLE_PV2MI_BRIDGE_ECO

//---- 9. change to 55 interface
#define ENABLE_UTMI_55_INTERFACE

//---- 10. 240's phase as 120's clock
/* bit<3> for 240's phase as 120's clock set 1, bit<4> for 240Mhz in mac 0 for faraday 1 for etron */
#define ENABLE_UTMI_240_AS_120_PHASE_ECO

//---- 14. HS connection fail problem (Gate into VFALL state)
#define ENABLE_HS_CONNECTION_FAIL_INTO_VFALL_ECO

//---- 15. Enable UHC Preamble ECO function
#define ENABLE_UHC_PREAMBLE_ECO

//---- 16. Don't close RUN bit when device disconnect
#define ENABLE_UHC_RUN_BIT_ALWAYS_ON_ECO

//---- 19. Not yet support MIU lower bound address subtraction ECO (for chips which use ENABLE_USB_NEW_MIU_SLE)
#define DISABLE_MIU_LOW_BOUND_ADDR_SUBTRACT_ECO
//--------------------------------------------------------------------


//------ Software patch enable switch --------------------------------
//---- 1. flush MIU pipe
#define _USB_T3_WBTIMEOUT_PATCH 1

//---- 2. data structure (qtd ,...) must be 128-byte aligment
#define _USB_128_ALIGMENT 1

//---- 3. tx/rx reset clock gating cause XIU timeout
#define _USB_XIU_TIMEOUT_PATCH 0

//---- 4. short packet lose interrupt without IOC
#define _USB_SHORT_PACKET_LOSE_INT_PATCH 0

//---- 5. QH blocking in MDATA condition, split zero-size data
#define _USB_SPLIT_MDATA_BLOCKING_PATCH 1

//---- 6. DM always keep high issue
#if !defined(ENABLE_HS_DM_KEEP_HIGH_ECO) // if without ECO solution, use SW patch.
	#define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH 0
#else
	#define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH 0
#endif

//---- 7. clear port eanble when device disconnect while bus reset
#define _USB_CLEAR_PORT_ENABLE_AFTER_FAIL_RESET_PATCH 1

//---- 8. mstar host only supports "Throttle Mode" in split translation
#define _USB_TURN_ON_TT_THROTTLE_MODE_PATCH 1

//---- 9. lower squelch level to cover weak cable link
#define _USB_ANALOG_RX_SQUELCH_PATCH 0

//---- 10. high speed reset chirp patch
//#define _USB_HS_CHIRP_PATCH 1

//---- 11. friendly customer patch
#define _USB_FRIENDLY_CUSTOMER_PATCH 1

//---- 12. enabe PVCI i_miwcplt wait for mi2uh_last_done_z
#define _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH 1

//--------------------------------------------------------------------


//------ Reduce EOF1 to 12us for performance imporvement -------------
/* Enlarge EOP1 from 12us to 16us for babble prvention under hub case.
 * However, we keep the "old config name". 20130121
 */
#define ENABLE_12US_EOF1
//--------------------------------------------------------------------

#define _UTMI_PWR_SAV_MODE_ENABLE 1

//---- Setting PV2MI bridge read/write burst size to minimum
#define _USB_MINI_PV2MI_BURST_SIZE 1

//--------------------------------------------------------------------
#define _USB_UTMI_DPDM_SWAP_P0 0
#define _USB_UTMI_DPDM_SWAP_P1 0

//------ Titania3_series_start_ehc flag ------------------------------
// Use low word as flag
#define EHCFLAG_NONE			0x0
#define EHCFLAG_DPDM_SWAP		0x1
#define EHCFLAG_TESTPKG			0x2
#define EHCFLAG_DOUBLE_DATARATE		0x4
#define EHCFLAG_ENABLE_OTG		0x8

// Use high word as data
#define EHCFLAG_DDR_MASK		0xF0000000
#define EHCFLAG_DDR_x15			0x10000000 //480MHz x1.5
#define EHCFLAG_DDR_x18			0x20000000 //480MHz x1.8
//--------------------------------------------------------------------

//------ UTMI disconnect level parameters ---------------------------------
// 0x00: 550mv, 0x20: 575, 0x40: 600, 0x60: 625
#define UTMI_DISCON_LEVEL_2A	(0x60)
	
//------ UTMI eye diagram parameters ---------------------------------
#if 0
	// for 40nm
	#define UTMI_EYE_SETTING_2C	(0x98)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x10)
	#define UTMI_EYE_SETTING_2F	(0x01)
#elif 0
	// for 40nm after Agate, use 55nm setting7
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x30)
	#define UTMI_EYE_SETTING_2F	(0x81)
#elif 0
	// for 40nm after Agate, use 55nm setting6
	#define UTMI_EYE_SETTING_2C	(0x10)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x30)
	#define UTMI_EYE_SETTING_2F	(0x81)
#elif 0
	// for 40nm after Agate, use 55nm setting5
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x30)
	#define UTMI_EYE_SETTING_2F	(0x81)
#elif 0
	// for 40nm after Agate, use 55nm setting4
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
#elif 0
	// for 40nm after Agate, use 55nm setting3
	#define UTMI_EYE_SETTING_2C	(0x10)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
#elif 0
	// for 40nm after Agate, use 55nm setting2
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
#else
	// for 40nm after Agate, use 55nm setting1, the default
	#define UTMI_EYE_SETTING_2C	(0x10)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
#endif

//------ eFuse setting ---------------------------------
//#define MSTAR_EFUSE_RTERM
#define MSTAR_EFUSE_BASE	(MSTAR_PM_BASE+(0x2000*2))
#define EFUSE_REG_ADDR		(0x50*2)	//8 bit offset 0x50
#define EFUSE_REG_READ		(0x50*2)	//8 bit offset 0x50
#define EFUSE_REG_DATA		(0x58*2)	//8 bit offset 0x58
#define RTERM_BANK		(0x4F*4)	//bank 0x4F
#define EFUSE_READ_TRIG		0x2000  // (1<<13)
#define RTERM_VALID_BIT 	(1<<4)

#endif	/* _EHCI_MSTAR_31023_H */
