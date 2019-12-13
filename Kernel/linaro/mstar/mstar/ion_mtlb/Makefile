#
# Makefile for MStar ion mtlb device drivers.
#

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/ion_mtlb
EXTRA_CFLAGS      +=  -Idrivers/mstar/ion_mtlb/$(CONFIG_MSTAR_CHIP_NAME)


# files
obj-$(CONFIG_MSTAR_ION_MTLB)	+= mdrv_ion_mtlb.o
obj-$(CONFIG_MSTAR_ION_MTLB)	+= $(CONFIG_MSTAR_CHIP_NAME)/mhal_ion_mtlb.o
