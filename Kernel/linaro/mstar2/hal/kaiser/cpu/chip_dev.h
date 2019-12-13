////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (??MStar Confidential Information??) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#include <linux/dma-mapping.h> 
#include "chip_int.h"

#define UTMI_BASE_ADDRESS_START		0xFD207500
#define UTMI_BASE_ADDRESS_END		0xFD2075FC
#define USB_HOST20_ADDRESS_START	0xFD204800
#define USB_HOST20_ADDRESS_END		0xFD2049FC

#define SECOND_UTMI_BASE_ADDRESS_START	0xFD207400
#define SECOND_UTMI_BASE_ADDRESS_END	0xFD2074FC
#define SECOND_USB_HOST20_ADDRESS_START	0xFD201A00
#define SECOND_USB_HOST20_ADDRESS_END	0xFD201BFC

#define THIRD_UTMI_BASE_ADDRESS_START	0xFD205400
#define THIRD_UTMI_BASE_ADDRESS_END	    0xFD2054FC
#define THIRD_USB_HOST20_ADDRESS_START	0xFD220600
#define THIRD_USB_HOST20_ADDRESS_END	0xFD2207FC

#define XHCI_ADDRESS_START	0xFD320000
#define XHCI_ADDRESS_END	0xFD327fff
#define U3PHY_ADDRESS_START	0xFD245600
#define U3PHY_ADDRESS_END	0xFD245FFF

#define EHC_PORT0_IRQ E_IRQ_UHC
#define EHC_PORT1_IRQ E_IRQEXPL_UHC1
#define EHC_PORT2_IRQ E_IRQ_UHC_2
#define XHC_PORT0_IRQ E_IRQ_UHC30

static u64 ehci_dmamask = ~(u32)0;
static u64 xhci_dmamask = 0xffffffff; 

/* for stb devices */
#define SATA_GHC_0_ADDRESS_START	(0xFD000000 + (0x122400 << 1))
#define SATA_GHC_0_ADDRESS_END		(0xFD000000 + (0x1224FE << 1))
#define SATA_GHC_1_ADDRESS_START	(0xFD000000 + (0x122500 << 1))
#define SATA_GHC_1_ADDRESS_END		(0xFD000000 + (0x1225FE << 1))
#define SATA_MISC_ADDRESS_START		(0xFD000000 + (0x122600 << 1))
#define SATA_MISC_ADDRESS_END		(0xFD000000 + (0x1226FE << 1))

static u64 sata_dmamask = DMA_BIT_MASK(32);
