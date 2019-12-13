#
# Makefile for MStar mtlb device drivers.
#

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/include/sys/common
EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/miu
EXTRA_CFLAGS      +=  -Idrivers/mstar/miu/$(CONFIG_MSTAR_CHIP_NAME)/
EXTRA_CFLAGS      +=  -Iinclude/linux
# specific options
EXTRA_CFLAGS      +=


# files
obj-$(CONFIG_MSTAR_MIU)             := mdrv-miu.o

mdrv-miu-objs    := mdrv_miu.o
mdrv-miu-objs    += $(CONFIG_MSTAR_CHIP_NAME)/mhal_miu.o

