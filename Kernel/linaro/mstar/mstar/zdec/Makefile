#
# Makefile for MStar ZDEC
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

EXTRA_CFLAGS += -Idrivers/mstar/include
EXTRA_CFLAGS += -Idrivers/mstar/zdec/$(CONFIG_MSTAR_CHIP_NAME)

obj-y        += $(CONFIG_MSTAR_CHIP_NAME)/mhal_zdec.o
obj-y        += mstar_zdec.o

