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
