#
# Makefile for MStar SAR device drivers.
#

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif


# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/sar
EXTRA_CFLAGS      +=  -Idrivers/mstar/sar/$(CONFIG_MSTAR_CHIP_NAME)

# files
obj-$(CONFIG_MSTAR_SAR)	+= mdrv-sar.o
mdrv-sar-objs			+= mdrv_sar.o
mdrv-sar-objs			+= $(CONFIG_MSTAR_CHIP_NAME)/mhal_sar.o
