#
# Makefile for MStar XC device drivers.
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif


# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/xc/drv
EXTRA_CFLAGS      +=  -Idrivers/mstar/xc/drv/$(CONFIG_MSTAR_CHIP_NAME)

EXTRA_CFLAGS      +=  -DRED_LION

# specific options
EXTRA_CFLAGS      +=

# files
obj-$(CONFIG_MSTAR_GFLIP)	+= mdrv-xc.o
mdrv-xc-objs		+= mdrv_xc_io.o drv/mdrv_xc.o 
mdrv-xc-objs		+= drv/$(CONFIG_MSTAR_CHIP_NAME)/mhal_xc.o

