#
# Makefile for MStar SPI NAND device drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/spinand/
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/include/
XTRA_CFLAGS      +=  -Idrivers/mstar2/include/
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/spinand/
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/spinand/inc/



$(warning $(CONFIG_MSTAR_CHIP_NAME))
$(warning $(EXTRA_CFLAGS))

# specific options
obj-y             += mdrv_spinand.o
#obj-y	            += src/drvSPINAND.o
obj-y               += ../../hal/$(CONFIG_MSTAR_CHIP_NAME)/spinand/mhal_spinand.o

