#
# Makefile for MStar CPU_calibrating device drivers.
#

ifeq ($(MAKE_TYPE), MODULE_STANDALONE)
include $(TOP_DIR)/modules.mk
endif

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/CPU_calibrating
EXTRA_CFLAGS += -Idrivers/mstar/include
EXTRA_CFLAGS += -Idrivers/mstar/dvfs/include
EXTRA_CFLAGS += -Idrivers/mstar/dvfs/hal/$(CONFIG_MSTAR_CHIP_NAME)
EXTRA_CFLAGS += -Idrivers/mstar/dvfs/hal/$(CONFIG_MSTAR_CHIP_NAME)/include

# files
obj-$(CONFIG_CPU_FREQ)	+= mdrv_CPU_calibrating.o



#export header files

