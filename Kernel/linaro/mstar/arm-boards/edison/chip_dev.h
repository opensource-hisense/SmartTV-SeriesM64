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

#include "chip_int.h"

#define UTMI_BASE_ADDRESS_START		0xFD207500
#define UTMI_BASE_ADDRESS_END		0xFD2075FC
#define USB_HOST20_ADDRESS_START	0xFD204800
#define USB_HOST20_ADDRESS_END		0xFD2049FC

#define SECOND_UTMI_BASE_ADDRESS_START	0xFD207400
#define SECOND_UTMI_BASE_ADDRESS_END	0xFD2074FC
#define SECOND_USB_HOST20_ADDRESS_START	0xFD201A00
#define SECOND_USB_HOST20_ADDRESS_END	0xFD201BFC

#define THIRD_UTMI_BASE_ADDRESS_START	0xFD207200
#define THIRD_UTMI_BASE_ADDRESS_END		0xFD2072FC
#define THIRD_USB_HOST20_ADDRESS_START	0xFD227200
#define THIRD_USB_HOST20_ADDRESS_END	0xFD2273FC

#define XHCI_ADDRESS_START	0xFD320000
#define XHCI_ADDRESS_END	0xFD327fff
#define U3PHY_ADDRESS_START	0xFD244200
#define U3PHY_ADDRESS_END	0xFD244BFF

#define EHC_PORT0_IRQ E_IRQ_UHC
#define EHC_PORT1_IRQ E_IRQEXPL_UHC1
#define EHC_PORT2_IRQ E_IRQEXPL_UHC2
#define XHC_PORT0_IRQ E_IRQ_UHC30

static u64 ehci_dmamask = 0x7ffffff;
static u64 xhci_dmamask = 0xffffffff; 
