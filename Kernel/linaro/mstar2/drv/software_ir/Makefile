#
# Makefile for the kernel IrDA char device drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

CHIP    = titania

EXTRA_CFLAGS += -Idrivers/mstar2/include
EXTRA_CFLAGS += -Idrivers/mstar2/dev/ir
EXTRA_CFLAGS += -Idrivers/mstar2/dev/ir/$(CONFIG_MSTAR_CHIP_NAME)
EXTRA_CFLAGS += -Iinclude/linux
EXTRA_CFLAGS += -Idrivers/mstar2/dev/software_ir


obj-$(CONFIG_MSTAR_IR) += mdrv-software-ir.o
mdrv-software-ir-objs		+= mdrv_software_ir.o
mdrv-software-ir-objs           += mdrv_software_ir_io.o
