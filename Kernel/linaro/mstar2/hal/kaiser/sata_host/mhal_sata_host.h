////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mhal_sata_host.h
/// @author MStar Semiconductor Inc.
/// @brief  SATA Host HAL Driver
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_SATA_HOST_H_
#define _MHAL_SATA_HOST_H_

// Local Definition

#define AHCI_P0CLB                  0x18001000
#define AHCI_P0FB                   0x18001100
#define AHCI_CTBA0                  0x18001200

#define SATA_PORT_NUM               1
#define SATA_SDMAP_RIU_BASE         0xFD2A0000
#define SATA_CMD_HEADER_SIZE 		0x400
#define SATA_FIS_SIZE 				0x100

enum {
    /* global controller registers */
    HOST_CAP        = 0x00,        /* host capabilities */
    HOST_CTL        = (0x04 << 1), /* global host control */
    HOST_IRQ_STAT   = (0x08 << 1), /* interrupt status */
    HOST_PORTS_IMPL = (0x0c << 1), /* bitmap of implemented ports */
    HOST_VERSION    = (0x10 << 1), /* AHCI spec. version compliancy */
    HOST_CAP2       = (0x24 << 1), /* host capabilities, extended */

    /* HOST_CTL bits - HOST_CAP, 0x00 */
    HOST_RESET      = (1 << 0),  /* reset controller; self-clear */
    HOST_IRQ_EN     = (1 << 1),  /* global IRQ enable */
    HOST_AHCI_EN    = (1 << 31), /* AHCI enabled */

    /* Registers for each SATA port */
    PORT_LST_ADDR       = 0x00, /* command list DMA addr */
    PORT_LST_ADDR_HI    = (0x04 << 1), /* command list DMA addr hi */
    PORT_FIS_ADDR       = (0x08 << 1), /* FIS rx buf addr */
    PORT_FIS_ADDR_HI    = (0x0c << 1), /* FIS rx buf addr hi */
    PORT_IRQ_STAT       = (0x10 << 1), /* interrupt status */
    PORT_IRQ_MASK       = (0x14 << 1), /* interrupt enable/disable mask */
    PORT_CMD            = (0x18 << 1), /* port command */
    PORT_TFDATA         = (0x20 << 1), /* taskfile data */
    PORT_SIG            = (0x24 << 1), /* device TF signature */
    PORT_SCR_STAT       = (0x28 << 1), /* SATA phy register: SStatus */
    PORT_SCR_CTL        = (0x2c << 1), /* SATA phy register: SControl */
    PORT_SCR_ERR        = (0x30 << 1), /* SATA phy register: SError */
    PORT_SCR_ACT        = (0x34 << 1), /* SATA phy register: SActive */
    PORT_CMD_ISSUE      = (0x38 << 1), /* command issue */
    PORT_SCR_NTF        = (0x3c << 1), /* SATA phy register: SNotification */
    PORT_DMA_CTRL       = (0x70 << 1), /* SATA phy register: SNotification */

    /* PORT_CMD bits */
    PORT_CMD_ASP        = (1 << 27), /* Aggressive Slumber/Partial */
    PORT_CMD_ALPE       = (1 << 26), /* Aggressive Link PM enable */
    PORT_CMD_ATAPI      = (1 << 24), /* Device is ATAPI */
    PORT_CMD_FBSCP      = (1 << 22), /* FBS Capable Port */
    PORT_CMD_PMP        = (1 << 17), /* PMP attached */
    PORT_CMD_LIST_ON    = (1 << 15), /* cmd list DMA engine running */
    PORT_CMD_FIS_ON     = (1 << 14), /* FIS DMA engine running */
    PORT_CMD_FIS_RX     = (1 << 4), /* Enable FIS receive DMA engine */
    PORT_CMD_CLO        = (1 << 3), /* Command list override */
    PORT_CMD_POWER_ON   = (1 << 2), /* Power up device */
    PORT_CMD_SPIN_UP    = (1 << 1), /* Spin up device */
    PORT_CMD_START      = (1 << 0), /* Enable port DMA engine */

    PORT_CMD_ICC_MASK   = (0xf << 28), /* i/f ICC state mask */
    PORT_CMD_ICC_ACTIVE = (0x1 << 28), /* Put i/f in active state */
    PORT_CMD_ICC_PARTIAL    = (0x2 << 28), /* Put i/f in partial state */
    PORT_CMD_ICC_SLUMBER    = (0x6 << 28), /* Put i/f in slumber state */

    /*  Status Error  */
    AHCI_PORT_SERR_RDIE  = (1 << 0),    /* Recovered Data Integrity Error */
    AHCI_PORT_SERR_RCE   = (1 << 1),    /* Recovered Communications Error */
    /* Bit 2 ~ 7 Reserved */
    AHCI_PORT_SERR_TDIE  = (1 << 8),    /* Transient Data Integrity Error */
    AHCI_PORT_SERR_PCDIE = (1 << 9),    /* Persistent    Communication    or    Data    Integrity    Error */
    AHCI_PORT_SERR_PE    = (1 << 10),   /* Protocol  Error */
    AHCI_PORT_SERR_IE    = (1 << 11),   /* Internal  Error */
    /* Bit 15 ~ 12 Reserved */
    AHCI_PORT_SERR_PRDYC = (1 << 16),   /* PhyRdy  Change */
    AHCI_PORT_SERR_PIE   = (1 << 17),   /* Phy  Internal  Error */
    AHCI_PORT_SERR_COMW  = (1 << 18),   /* Comm Wake Detect */
    AHCI_PORT_SERR_TDE   = (1 << 19),   /* 10B  to  8B  Decode  Error  */
    AHCI_PORT_SERR_DE    = (1 << 20),   /* Disparity Error <= Not Use by AHCI  */
    AHCI_PORT_SERR_CRCE  = (1 << 21),   /* CRC Error  */
    AHCI_PORT_SERR_HE    = (1 << 22),   /* Handshake  Error */
    AHCI_PORT_SERR_LSE   = (1 << 23),   /* Link Sequence Error  */
    AHCI_PORT_SERR_TSTE  = (1 << 24),   /* Transport  state  transition  error  */
    AHCI_PORT_SERR_UFIS  = (1 << 25),   /* Unknown FIS Type  */
    AHCI_PORT_SERR_EXC   = (1 << 26),   /* Exchanged :  a  change  in device  presence  has  been  detected */
    /* Bit 31 ~ 27 Reserved */
};

enum {
    E_PORT_SPEED_MASK = (0xF << 4),
    E_PORT_SPEED_NO_RESTRICTION = (0x0 < 4),
    E_PORT_SPEED_GEN1 = (0x1 << 4),
    E_PORT_SPEED_GEN2 = (0x2 << 4),
    E_PORT_SPEED_GEN3 = (0x3 << 4),

    E_PORT_DET_MASK = (0xF << 0),  //Device  Detection  Initialization
    E_PORT_DET_NODEVICE_DETECT = 0x0,
    E_PORT_DET_HW_RESET = 0x1, // Cause HW send initial sequence
    E_PORT_DET_DISABLE_PHY = 0x4, // Put PHY into Offline
    E_PORT_DET_DEVICE_NOEST = 0x1, // not established
    E_PORT_DET_DEVICE_EST = 0x3,  // established
    E_PORT_DET_PHY_OFFLINE = 0x4, // Put PHY into Offline

    DATA_COMPLETE_INTERRUPT = (1 << 31),

    /* PORT_IRQ_{STAT,MASK} bits */
    PORT_IRQ_COLD_PRES	= (1 << 31), /* cold presence detect */
    PORT_IRQ_TF_ERR 	= (1 << 30), /* task file error */
    PORT_IRQ_HBUS_ERR	= (1 << 29), /* host bus fatal error */
    PORT_IRQ_HBUS_DATA_ERR	= (1 << 28), /* host bus data error */
    PORT_IRQ_IF_ERR 	= (1 << 27), /* interface fatal error */
    PORT_IRQ_IF_NONFATAL	= (1 << 26), /* interface non-fatal error */
    PORT_IRQ_OVERFLOW	= (1 << 24), /* xfer exhausted available S/G */
    PORT_IRQ_BAD_PMP	= (1 << 23), /* incorrect port multiplier */

    PORT_IRQ_PHYRDY 	= (1 << 22), /* PhyRdy changed */
    PORT_IRQ_DEV_ILCK	= (1 << 7), /* device interlock */
    PORT_IRQ_CONNECT	= (1 << 6), /* port connect change status */
    PORT_IRQ_SG_DONE	= (1 << 5), /* descriptor processed */
    PORT_IRQ_UNK_FIS	= (1 << 4), /* unknown FIS rx'd */
    PORT_IRQ_SDB_FIS	= (1 << 3), /* Set Device Bits FIS rx'd */
    PORT_IRQ_DMAS_FIS	= (1 << 2), /* DMA Setup FIS rx'd */
    PORT_IRQ_PIOS_FIS	= (1 << 1), /* PIO Setup FIS rx'd */
    PORT_IRQ_D2H_REG_FIS	= (1 << 0), /* D2H Register FIS rx'd */

    PORT_IRQ_FREEZE		= PORT_IRQ_HBUS_ERR |
				  PORT_IRQ_IF_ERR |
				  PORT_IRQ_CONNECT |
				  PORT_IRQ_PHYRDY |
				  PORT_IRQ_UNK_FIS |
				  PORT_IRQ_BAD_PMP,
    PORT_IRQ_ERROR		= PORT_IRQ_FREEZE |
				  PORT_IRQ_TF_ERR |
				  PORT_IRQ_HBUS_DATA_ERR,
    DEF_PORT_IRQ		= PORT_IRQ_ERROR | PORT_IRQ_SG_DONE |
				  PORT_IRQ_SDB_FIS | PORT_IRQ_DMAS_FIS |
				  PORT_IRQ_PIOS_FIS | PORT_IRQ_D2H_REG_FIS,
};

/*
*          Host Controller MISC Register
*/
enum
{
    SATA_MISC_CFIFO_ADDRL     =((0x10 <<1 )<<1),
    SATA_MISC_CFIFO_ADDRH     =((0x11 <<1 )<<1),
    SATA_MISC_CFIFO_WDATAL    =((0x12 <<1 )<<1),
    SATA_MISC_CFIFO_WDATAH    =((0x13 <<1 )<<1),
    SATA_MISC_CFIFO_RDATAL    =((0x14 <<1 )<<1),
    SATA_MISC_CFIFO_RDATAH    =((0x15 <<1 )<<1),
    SATA_MISC_CFIFO_RORW      =((0x16 <<1 )<<1),
    SATA_MISC_CFIFO_ACCESS    =((0x17 <<1 )<<1),
    SATA_MISC_ACCESS_MODE     =((0x18 <<1 )<<1),
    SATA_MISC_AMBA_MUXRST     =((0x21 <<1 )<<1),
    SATA_MISC_HBA_LADDR       =((0x24 <<1 )<<1),
    SATA_MISC_HBA_HADDR       =((0x25 <<1 )<<1),
    SATA_MISC_CMD_LADDR       =((0x26 <<1 )<<1),
    SATA_MISC_CMD_HADDR       =((0x27 <<1 )<<1),
    SATA_MISC_DATA_ADDR       =((0x28 <<1 )<<1),
    SATA_MISC_ENRELOAD        =((0x29 <<1 )<<1),
    SATA_MISC_AMBA_ARBIT      =((0x2A <<1 )<<1),
    SATA_MISC_AMBA_PGEN       =((0x30 <<1 )<<1),
    SATA_MISC_HOST_SWRST      =((0x50 <<1 )<<1),
    SATA_MISC_HOST_NEAR       =((0x51 <<1 )<<1),
    SATA_MISC_FPGA_EN         =((0x55 <<1 )<<1),
};

void MHal_SATA_HW_Inital(u32 misc_base, u32 port_base, u32 hba_base);
void MHal_SATA_Setup_Port_Implement(u32 misc_base, u32 port_base, u32 hba_base);

#endif
