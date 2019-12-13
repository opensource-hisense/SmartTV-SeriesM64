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

#include <linux/platform_device.h>
#include <linux/version.h>
#include <asm/pmu.h>
#include "chip_dev.h" /* by chip irq & riu */

struct resource pmu_resources[] = {
    [0] = {
        .start = CHIP_IRQ_PMU0,
        .end   = CHIP_IRQ_PMU0,
        .flags = IORESOURCE_IRQ,
    },
    [1] = {
        .start = CHIP_IRQ_PMU1,
        .end   = CHIP_IRQ_PMU1,
        .flags = IORESOURCE_IRQ,
    },
#ifdef CHIP_IRQ_POMU2
    [2] = {
        .start = CHIP_IRQ_PMU2,
        .end   = CHIP_IRQ_PMU2,
        .flags = IORESOURCE_IRQ,
    },
#endif
#ifdef CHIP_IRQ_POMU3
    [3] = {
        .start = CHIP_IRQ_PMU3,
        .end   = CHIP_IRQ_PMU3,
        .flags = IORESOURCE_IRQ,
    },
#endif
};

struct platform_device chip_device_pmu = {
	.name = "arm-pmu",
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 1, 10)
	.id = -1,
#else
	.id = ARM_PMU_DEVICE_CPU,
#endif
	.num_resources = ARRAY_SIZE(pmu_resources),
	.resource = pmu_resources,
};

struct platform_device Mstar_emac_device = {
	.name = "Mstar-emac",
	.id = 0,
};

struct platform_device Mstar_ir_device = {
	.name = "Mstar-ir",
	.id = 0,
};

struct platform_device Mstar_gmac_device = {
	.name = "Mstar-gmac",
	.id = 0,
};

struct platform_device Mstar_mbx_device = {
	.name = "Mstar-mbx",
	.id = 0,
};

struct platform_device Mstar_gflip_device = {
	.name = "Mstar-gflip",
	.id = 0,
};

struct platform_device Mstar_alsa_device = {
	.name = "Mstar-alsa",
	.id = 0,
};

struct platform_device Mstar_iic_device = {
	.name = "Mstar-iic",
	.id = 0,
};

struct platform_device Mstar_gpio_device = {
	.name = "Mstar-gpio",
	.id = 0,
};

struct platform_device rtc_mstar_device = {
	.name = "rtc-mstar",
	.id	= -1,
};

static struct resource Mstar_usb_ehci_resources[] = {
	[0] = {
		.start = UTMI_BASE_ADDRESS_START,
		.end   = UTMI_BASE_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = USB_HOST20_ADDRESS_START,
		.end   = USB_HOST20_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = EHC_PORT0_IRQ,
		.end   = EHC_PORT0_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device Mstar_usb_ehci_device = {
	.name = "Mstar-ehci-1",
	.id = 0,
	.dev = {
		.dma_mask = &ehci_dmamask,
		.coherent_dma_mask = 0xffffffff,
	},
	.num_resources = ARRAY_SIZE(Mstar_usb_ehci_resources),
	.resource = Mstar_usb_ehci_resources,
};

static struct resource Second_Mstar_usb_ehci_resources[] = {
	[0] = {
		.start = SECOND_UTMI_BASE_ADDRESS_START,
		.end   = SECOND_UTMI_BASE_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = SECOND_USB_HOST20_ADDRESS_START,
		.end   = SECOND_USB_HOST20_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = EHC_PORT1_IRQ,
		.end   = EHC_PORT1_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device Second_Mstar_usb_ehci_device = {
	.name = "Mstar-ehci-2",
	.id = 1,
	.dev = {
		.dma_mask = &ehci_dmamask,
		.coherent_dma_mask = 0xffffffff, /* tony add for limit DMA range */
	},
	.num_resources = ARRAY_SIZE(Second_Mstar_usb_ehci_resources),
	.resource = Second_Mstar_usb_ehci_resources,
};

#ifdef THIRD_USB_HOST20_ADDRESS_START
static struct resource Third_Mstar_usb_ehci_resources[] = {
    [0] = {
        .start = THIRD_UTMI_BASE_ADDRESS_START,
        .end   = THIRD_UTMI_BASE_ADDRESS_END,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = THIRD_USB_HOST20_ADDRESS_START,
        .end   = THIRD_USB_HOST20_ADDRESS_END,
        .flags = IORESOURCE_MEM,
    },
    [2] = {
        .start = EHC_PORT2_IRQ,
        .end   = EHC_PORT2_IRQ,
        .flags = IORESOURCE_IRQ,
    },
};

struct platform_device Third_Mstar_usb_ehci_device = {
    .name = "Mstar-ehci-3",
    .id = 2,
    .dev = {
        .dma_mask = &ehci_dmamask,
        .coherent_dma_mask = 0xffffffff,
    },
    .num_resources = ARRAY_SIZE(Third_Mstar_usb_ehci_resources),
    .resource = Third_Mstar_usb_ehci_resources,
};
#endif

#ifdef FOURTH_USB_HOST20_ADDRESS_START
static struct resource Fourth_Mstar_usb_ehci_resources[] = {
	[0] = {
		.start = FOURTH_UTMI_BASE_ADDRESS_START,
		.end   = FOURTH_UTMI_BASE_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = FOURTH_USB_HOST20_ADDRESS_START,
		.end   = FOURTH_USB_HOST20_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = EHC_PORT3_IRQ,
		.end   = EHC_PORT3_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device Fourth_Mstar_usb_ehci_device = {
    .name = "Mstar-ehci-4",
    .id = 3,
    .dev = {
        .dma_mask = &ehci_dmamask,
        .coherent_dma_mask = 0xffffffff,
    },
    .num_resources = ARRAY_SIZE(Fourth_Mstar_usb_ehci_resources),
    .resource = Fourth_Mstar_usb_ehci_resources,
};
#endif

#ifdef XHCI_ADDRESS_START
static struct resource Mstar_usb_xhci_resources[] = {
	[0] = {
		.start = U3PHY_ADDRESS_START,
		.end   = U3PHY_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = XHCI_ADDRESS_START,
		.end   = XHCI_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = XHC_PORT0_IRQ,
		.end   = XHC_PORT0_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device Mstar_usb_xhci_device = {
	.name = "Mstar-xhci-1",
	.id = 0,
	.dev = {
		.dma_mask = &xhci_dmamask,
		.coherent_dma_mask = 0xffffffff,
	},
	.num_resources = ARRAY_SIZE(Mstar_usb_xhci_resources),
	.resource = Mstar_usb_xhci_resources,
};
#endif

#ifdef SATA_GHC_0_ADDRESS_START
static struct resource satahost_resources[] = {
	[0] = {
		.start = SATA_GHC_0_ADDRESS_START,
		.end   = SATA_GHC_0_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = SATA_GHC_1_ADDRESS_START,
		.end   = SATA_GHC_1_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = SATA_MISC_ADDRESS_START,
		.end   = SATA_MISC_ADDRESS_END,
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = E_IRQEXPL_SATA,
		.end   = E_IRQEXPL_SATA,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device chip_device_satahost = {
	.name = "Mstar-sata",
	.id = 0,
	.dev = {
		.dma_mask = &sata_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.num_resources = ARRAY_SIZE(satahost_resources),
	.resource = satahost_resources,
};
#endif

struct platform_device Mstar_mspi_device = {
    .name = "mspi_spi",
    .id = 0,
};

struct platform_device Mstar_isp_device = {
    .name = "isp_spi",
    .id = 1,
};
#if defined(CONFIG_USB_GADGET_MSTAR)
static struct resource ms_udc_device_resource[] =
{
    [0] = {
        .start = UDC_ADDRESS_START,
        .end   = UDC_ADDRESS_END,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = USB_IRQ_UDC,
        .end   = USB_IRQ_UDC,
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
#endif
#if defined(CONFIG_USB_MS_OTG)
/******* USB OTG ********/
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
#endif
