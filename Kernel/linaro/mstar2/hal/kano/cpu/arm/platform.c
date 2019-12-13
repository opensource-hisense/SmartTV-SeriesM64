////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2012 MStar Semiconductor, Inc.
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

//#include <linux/config.h>
#include <generated/autoconf.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/platform_device.h>

#include <mach/irqs.h>
#include "chip_int.h"

#if  defined(CONFIG_USB_XHCI_HCD) || defined(CONFIG_USB_XHCI_HCD_MODULE) 
#define ENABLE_XHC
#endif

static struct resource Mstar_usb_ehci_resources[] = 
{
	[2] = 
	{
		.start		= E_IRQ_UHC,
		.end		= E_IRQ_UHC,
		.flags		= IORESOURCE_IRQ,
	},
};

//add for 2st EHCI
static struct resource Second_Mstar_usb_ehci_resources[] = 
{
	[2] = 
	{
		.start		= E_IRQEXPL_UHC1,
		.end		= E_IRQEXPL_UHC1,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource Third_Mstar_usb_ehci_resources[] = 
{
	[2] = 
	{
		.start		= E_IRQEXPL_UHC2P2,
		.end		= E_IRQEXPL_UHC2P2,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource Fourth_Mstar_usb_ehci_resources[] = 
{
	[2] = 
	{
		.start		= E_IRQHYPL_USB30_HS_UHC,
		.end		= E_IRQHYPL_USB30_HS_UHC,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource Fifth_Mstar_usb_ehci_resources[] = 
{
	[2] = 
	{
		.start		= E_IRQHYPL_USB30_HS1_UHC,
		.end		= E_IRQHYPL_USB30_HS1_UHC,
		.flags		= IORESOURCE_IRQ,
	},
};

/* The dmamask must be set for EHCI to work */
static u64 ehci_dmamask = ~(u32)0;

static struct platform_device Mstar_usb_ehci_device = 
{
	.name           = "Mstar-ehci-1",
	.id             = 0,
	.dev = 
	{
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0xffffffff, //add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Mstar_usb_ehci_resources),
	.resource	= Mstar_usb_ehci_resources,
};

//tony add for 2st EHCI
static struct platform_device Second_Mstar_usb_ehci_device = 
{
	.name		= "Mstar-ehci-2",
	.id		= 1,
	.dev = 
	{
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0xffffffff,    //add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Second_Mstar_usb_ehci_resources),
	.resource	= Second_Mstar_usb_ehci_resources,
};

static struct platform_device Third_Mstar_usb_ehci_device = 
{
	.name		= "Mstar-ehci-3",
	.id		= 2,
	.dev = 
	{
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0xffffffff,    //add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Third_Mstar_usb_ehci_resources),
	.resource	= Third_Mstar_usb_ehci_resources,
};

static struct platform_device Fourth_Mstar_usb_ehci_device = 
{
	.name		= "Mstar-ehci-4",
	.id		= 3,
	.dev = 
	{
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0xffffffff,    //add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Fourth_Mstar_usb_ehci_resources),
	.resource	= Fourth_Mstar_usb_ehci_resources,
};

static struct platform_device Fifth_Mstar_usb_ehci_device = 
{
	.name		= "Mstar-ehci-5",
	.id		= 4,
	.dev = 
	{
		.dma_mask		= &ehci_dmamask,
		.coherent_dma_mask	= 0xffffffff,    //add for limit DMA range
	},
	.num_resources	= ARRAY_SIZE(Fifth_Mstar_usb_ehci_resources),
	.resource	= Fifth_Mstar_usb_ehci_resources,
};

#ifdef ENABLE_XHC
//-----------------------------------------
//   xHCI platform device
//-----------------------------------------
static struct resource Mstar_usb_xhci_resources[] = {
	[2] = {
		.start		= E_IRQ_USB30,
		.end		= E_IRQ_USB30,
		.flags		= IORESOURCE_IRQ,
	},
};

static u64 xhci_dmamask = 0xffffffff;

static struct platform_device Mstar_usb_xhci_device = {
	.name		= "Mstar-xhci-1",
	.id		= 0,
	.dev = {
		.dma_mask		= &xhci_dmamask,
		.coherent_dma_mask	= 0xffffffff, 
	},
	.num_resources	= ARRAY_SIZE(Mstar_usb_xhci_resources),
	.resource	= Mstar_usb_xhci_resources,

};
//---------------------------------------------------------------
#endif

static struct platform_device *Mstar_platform_devices[] = {
	&Mstar_usb_ehci_device,
	&Second_Mstar_usb_ehci_device,
	&Third_Mstar_usb_ehci_device,
	&Fourth_Mstar_usb_ehci_device,
	&Fifth_Mstar_usb_ehci_device,
#ifdef ENABLE_XHC
	&Mstar_usb_xhci_device,    
#endif
};

int Mstar_ehc_platform_init(void)
{
	return platform_add_devices(Mstar_platform_devices, ARRAY_SIZE(Mstar_platform_devices));
}

#if defined(CONFIG_USB_MSB250X)
#define INT_MS_OTG	E_IRQHYPL_IRQ_OTG
#define UDC_ADDRESS_START	(0xFD000000 + (0x111700 << 1))
#define UDC_ADDRESS_END		(0xFD000000 + (0x1117FE << 1))
static struct resource ms_udc_device_resource[] =
{
    [0] = {
        .start = UDC_ADDRESS_START,
        .end   = UDC_ADDRESS_END,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = INT_MS_OTG,
        .end   = INT_MS_OTG,
        .name  = "udc-int",
        .flags = IORESOURCE_IRQ,
    }
};

struct platform_device ms_udc_device =
{
    .name             = "msb250x_udc",
    .id               = -1,
    .num_resources    = ARRAY_SIZE(ms_udc_device_resource), 
	.resource         = ms_udc_device_resource,		  
};


static struct platform_device *Mstar_udc_devices[] = {
	&ms_udc_device,
};

int Mstar_udc_platform_init(void)
{
	return platform_add_devices(Mstar_udc_devices, ARRAY_SIZE(Mstar_udc_devices));
}

#endif

#if defined(CONFIG_USB_MS_OTG)
#define OTG_USBC_ADDRESS_START	 (0xFD000000 + (0x100700 << 1))
#define OTG_USBC_ADDRESS_END	 (0xFD000000 + (0x10077E << 1))
#define OTG_MOTG_ADDRESS_START	 (0xFD000000 + (0x111700 << 1))
#define OTG_MOTG_ADDRESS_END	 (0xFD000000 + (0x1117FE << 1))
#define OTG_HOST20_ADDRESS_START	(0xFD000000 + (0x102400 << 1))
#define OTG_HOST20_ADDRESS_END		(0xFD000000 + (0x1024FE << 1))
#define OTG_UTMI_ADDRESS_START	(0xFD000000 + (0x103A80 << 1))
#define OTG_UTMI_ADDRESS_END	(0xFD000000 + (0x103B7E << 1))
#define USB_IRQ_OTG	E_IRQ_USB
#define OTG_IRQ_UHC E_IRQ_UHC


static struct resource ms_otg_device_resource[] =
{
	[0] = {
		.start = OTG_USBC_ADDRESS_START,
		.end   = OTG_USBC_ADDRESS_END,
		.name  = "usbc-base",
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = OTG_HOST20_ADDRESS_START,
		.end   = OTG_HOST20_ADDRESS_END,
		.name  = "uhc-base",
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = OTG_MOTG_ADDRESS_START,
		.end   = OTG_MOTG_ADDRESS_END,
		.name  = "motg-base",
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = USB_IRQ_OTG,	
		.end   = USB_IRQ_OTG,			
		.name  = "usb-int",
		.flags = IORESOURCE_IRQ,
	},
	[4] = {
		.start = OTG_UTMI_ADDRESS_START,
		.end   = OTG_UTMI_ADDRESS_END,
		.name  = "utmi-base",
		.flags = IORESOURCE_MEM,
	},
	[5] = {
		.start = OTG_IRQ_UHC,	
		.end   = OTG_IRQ_UHC,		
		.name  = "uhc-int",
		.flags = IORESOURCE_IRQ,
	}
};

struct platform_device Mstar_otg_device =
{
	.name             = "mstar-otg",
	.id               = -1,
	.num_resources    = ARRAY_SIZE(ms_otg_device_resource),
	.resource         = ms_otg_device_resource,
};

static struct platform_device *Mstar_otg_devices[] = {
	&Mstar_otg_device,
};

int Mstar_otg_platform_init(void)
{
	return platform_add_devices(Mstar_otg_devices, ARRAY_SIZE(Mstar_otg_devices));
}
#endif