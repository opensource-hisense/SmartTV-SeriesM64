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

//K1: use port 2 instead of port 1
#define SECOND_UTMI_BASE_ADDRESS_START 0xbf205400
#define SECOND_UTMI_BASE_ADDRESS_END 0xbf2054FC
#define SECOND_USB_HOST20_ADDRESS_START 0xbf220600
#define SECOND_USB_HOST20_ADDRESS_END 0xbf2207FC

#if 0
#define THIRD_UTMI_BASE_ADDRESS_START 0xbf205400
#define THIRD_UTMI_BASE_ADDRESS_END 0xbf2054FC
#define THIRD_USB_HOST20_ADDRESS_START 0xbf220600
#define THIRD_USB_HOST20_ADDRESS_END 0xbf2207FC
#endif

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
		.start		= E_IRQ_UHC2,  //E_IRQEXPL_UHC2,
		.end		= E_IRQ_UHC2,  //E_IRQEXPL_UHC2,
		.flags		= IORESOURCE_IRQ,
	},
};


#if 0
static struct resource Third_Mstar_usb_ehci_resources[] = {
	[0] = {
		.start		= THIRD_UTMI_BASE_ADDRESS_START,
		.end		= THIRD_UTMI_BASE_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= THIRD_USB_HOST20_ADDRESS_START,
		.end		= THIRD_USB_HOST20_ADDRESS_END,
		.flags		= IORESOURCE_MEM,
	},
	[2] = {
		.start		= E_IRQ_UHC2,
		.end		= E_IRQ_UHC2,
		.flags		= IORESOURCE_IRQ,
	},
};
#endif


/* The dmamask must be set for EHCI to work */
// keep up the setting to compliant with 2.6.28
//static u64 ehci_dmamask = ~(u32)0;
static u64 ehci_dmamask = 0x7ffffff;	//tony add for limit DMA range

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

#if 0
static struct platform_device Third_Mstar_usb_ehci_device = {
	.name		= "Mstar-ehci-3",
	.id		= 2,
	.dev = {
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0x7ffffff, 	//tony add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Third_Mstar_usb_ehci_resources),
	.resource	= Third_Mstar_usb_ehci_resources,
};
#endif
static struct platform_device Mstar_emac_device = {
	.name		= "Mstar-emac",
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
	
    #if 0
	&Third_Mstar_usb_ehci_device,
    #endif
    &Mstar_emac_device,
    &Mstar_ir_device,
    &Mstar_nand_device,
    &Mstar_mbx_device,
    &Mstar_gflip_device,
};

int Mstar_platform_init(void)
{
        unsigned char chipVer ;

        chipVer = *((unsigned char volatile *)(0xBF203C00+0x60*4+1));
        if (chipVer & 0x1)
        {            
            printk("Kronus 156-pin package, not to enable Mstar-ehci-1\n");
	    Mstar_usb_ehci_device.name = "None";
        }
	return platform_add_devices(Mstar_platform_devices, ARRAY_SIZE(Mstar_platform_devices));
}

arch_initcall(Mstar_platform_init);
