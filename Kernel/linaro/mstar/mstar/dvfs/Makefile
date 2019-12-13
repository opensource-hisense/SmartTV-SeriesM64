#//========================================================
#// Makefile for MStar DVFS Mechanism
#//========================================================

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

EXTRA_CFLAGS += -Idrivers/mstar/include
EXTRA_CFLAGS += -Idrivers/mstar/dvfs/include
EXTRA_CFLAGS += -Idrivers/mstar/dvfs/hal/$(CONFIG_MSTAR_CHIP_NAME)
EXTRA_CFLAGS += -Idrivers/mstar/dvfs/hal/$(CONFIG_MSTAR_CHIP_NAME)/include
EXTRA_CFLAGS += -Iinclude/linux

#//obj-$(CONFIG_MSTAR_DVFS) := mdrv_dvfs.o hal/$(CONFIG_MSTAR_CHIP_NAME)/mhal_dvfs.o
obj-$(CONFIG_CPU_FREQ) += mdrv_dvfs.o 
obj-$(CONFIG_CPU_FREQ) += hal/$(CONFIG_MSTAR_CHIP_NAME)/mhal_dvfs.o
