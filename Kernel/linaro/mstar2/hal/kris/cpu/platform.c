////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//#include <linux/config.h>
#include <linux/autoconf.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/platform_device.h>

#include "chip_int.h"

#define UTMI_BASE_ADDRESS_START 0xbf207500
#define UTMI_BASE_ADDRESS_END 0xbf2075FC
#define USB_HOST20_ADDRESS_START 0xbf204800
#define USB_HOST20_ADDRESS_END 0xbf2049FC
//tony add for 2st EHCI
#define SECOND_UTMI_BASE_ADDRESS_START 0xbf207400
#define SECOND_UTMI_BASE_ADDRESS_END 0xbf2074FC
#define SECOND_USB_HOST20_ADDRESS_START 0xbf201A00
#define SECOND_USB_HOST20_ADDRESS_END 0xbf201BFC
// First EMAC
#define EMAC_BASE_REG_ADDRESS_START 0xBF204000
#define EMAC_BASE_REG_ADDRESS_END 0xBF2047FF
#define EMAC_BASE_PHY_ADDRESS_START 0xBF243000
#define EMAC_BASE_PHY_ADDRESS_END 0xBF2434FF

// Second EMAC
#define SECOND_EMAC_BASE_ADDRESS_START 0xBF243600
#define SECOND_EMAC_BASE_ADDRESS_END 0xBF243FFF

static struct resource Mstar_usb_ehci_resources[] = {
	[0] = {
		.start		= UTMI_BASE_ADDRESS_START,
		.end		= UTMI_BASE_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= USB_HOST20_ADDRESS_START,
		.end		= USB_HOST20_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
	[2] = {
		.start		= E_IRQ_UHC,
		.end		= E_IRQ_UHC,
		.flags		= IORESOURCE_IRQ,
	},
};
//tony add for 2st EHCI
static struct resource Second_Mstar_usb_ehci_resources[] = {
	[0] = {
		.start		= SECOND_UTMI_BASE_ADDRESS_START,
		.end		= SECOND_UTMI_BASE_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= SECOND_USB_HOST20_ADDRESS_START,
		.end		= SECOND_USB_HOST20_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
	[2] = {
		.start		= E_IRQEXPL_UHC1,
		.end		= E_IRQEXPL_UHC1,
		.flags		= IORESOURCE_IRQ,
	},
};

/* The dmamask must be set for EHCI to work */
static u64 ehci_dmamask = ~(u32)0;

static struct platform_device Mstar_usb_ehci_device = {
	.name		= "Mstar-ehci-1",
	.id		= 0,
	.dev = {
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0x7ffffff, //tony add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Mstar_usb_ehci_resources),
	.resource	= Mstar_usb_ehci_resources,
};
//tony add for 2st EHCI
static struct platform_device Second_Mstar_usb_ehci_device = {
	.name		= "Mstar-ehci-2",
	.id		= 1,
	.dev = {
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0x7ffffff, 	//tony add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Second_Mstar_usb_ehci_resources),
	.resource	= Second_Mstar_usb_ehci_resources,
};

/*
static struct resource emac2_resource_0[] = {
	[0] = {
		.start		= EMAC_BASE_REG_ADDRESS_START,
		.end		= EMAC_BASE_REG_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
    [1] = {
		.start		= EMAC_BASE_PHY_ADDRESS_START,
		.end		= EMAC_BASE_PHY_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
	[2] = {
		.start		= E_IRQ_EMAC,
		.end		= E_IRQ_EMAC,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource emac2_resource_1[] = {
    [0] = {
        .start      = SECOND_EMAC_BASE_ADDRESS_START,
        .end        = SECOND_EMAC_BASE_ADDRESS_END,
        .flags      = IORESOURCE_MEM,
    },
    [1] = {
        .start      = E_IRQ_SECEMAC,
        .end        = E_IRQ_SECEMAC,
        .flags      = IORESOURCE_IRQ,
    },
};
*/

/*
static struct platform_device Mstar_emac_device_0 = {
	.name		= "Mstar-emac2",
	.id		= 0,
    .num_resources	= ARRAY_SIZE(emac2_resource_0),
	.resource	= emac2_resource_0,
};

static struct platform_device Mstar_emac_device_1 = {
	.name		= "Mstar-emac2",
	.id		= 1,
    .num_resources	= ARRAY_SIZE(emac2_resource_1),
	.resource	= emac2_resource_1,
};
*/

static struct platform_device Mstar_gmac_device = {
    .name       = "Mstar-gmac",
	.id		= 0,
};

static struct platform_device Mstar_ir_device = {
	.name		= "Mstar-ir",
	.id		= 0,
};

static struct platform_device Mstar_nand_device = {
	.name		= "Mstar-nand",
	.id		= 0,
};

static struct platform_device Mstar_mbx_device = {
	.name		= "Mstar-mbx",
	.id		= 0,
};

static struct platform_device Mstar_gflip_device = {
	.name		= "Mstar-gflip",
	.id		= 0,
};

//static struct platform_device *Mstar_platform_devices[] __initdata = {
static struct platform_device *Mstar_platform_devices[] = {
	&Mstar_usb_ehci_device,
	&Second_Mstar_usb_ehci_device,	 //tony add for 2st EHCI
    &Mstar_gmac_device,
    &Mstar_ir_device,
    &Mstar_nand_device,
    &Mstar_mbx_device,
    &Mstar_gflip_device,
};

int Mstar_platform_init(void)
{
	return platform_add_devices(Mstar_platform_devices, ARRAY_SIZE(Mstar_platform_devices));
}

arch_initcall(Mstar_platform_init);

