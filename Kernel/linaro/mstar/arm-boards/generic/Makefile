#
# Carsten Langgaard, carstenl@mips.com
# Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
#
# This program is free software; you can distribute it and/or modify it
# under the terms of the GNU General Public License (Version 2) as
# published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
#
# Makefile for the MIPS boards generic routines under Linux.
#

obj-y := memory.o chip_arch.o
#ifeq ($(KERNELVERSION), 3.1.10)
ifneq (,$(filter $(KERNELVERSION),3.1.10 3.10.23))
ifeq (,$(filter $(CONFIG_MSTAR_CHIP_NAME),"kaisers"))
obj-y += device.o
endif
endif

#obj-$(CONFIG_MIPS_CMP)	+= amon.o
#
#obj-$(CONFIG_EARLY_PRINTK)	+= console.o
#obj-$(CONFIG_PCI)		+= pci.o
#obj-$(CONFIG_KGDB)		+= gdb_hook.o

EXTRA_CFLAGS += -Werror

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

ifdef CONFIG_MSTAR_PROJECT_NAME
    EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)/$(CONFIG_MSTAR_PROJECT_NAME)
    EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)/$(CONFIG_MSTAR_PROJECT_NAME)/board
endif
ifeq ($(KERNELVERSION),3.8.13)
EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)_3.8
EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)_3.8/board
else
EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)
EXTRA_CFLAGS      +=  -Iarch/arm/arm-boards/$(CONFIG_MSTAR_CHIP_NAME)/board
endif
