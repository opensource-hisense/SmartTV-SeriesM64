#
# Makefile for MStar PCMCIA device drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/pcmcia
EXTRA_CFLAGS      +=  -Idrivers/mstar/pcmcia/$(CONFIG_MSTAR_CHIP_NAME)
EXTRA_CFLAGS      +=  -Iinclude/linux
EXTRA_CFLAGS      +=  -Idrivers/media/dvb/dvb-core
# files
obj-$(CONFIG_MSTAR_PCMCIA)             := mdrv-pcmcia.o

mdrv-pcmcia-objs    := mdrv_pcmcia.o drvPCMCIA.o $(CONFIG_MSTAR_CHIP_NAME)/halPCMCIA.o
