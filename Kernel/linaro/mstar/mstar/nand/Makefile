#
# Makefile for MStar NAND device drivers.
#

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/nand/$(CONFIG_MSTAR_NAND_IP)
EXTRA_CFLAGS      +=  -Idrivers/mstar/nand/$(CONFIG_MSTAR_NAND_IP)/inc/api
EXTRA_CFLAGS      +=  -Idrivers/mstar/nand/$(CONFIG_MSTAR_NAND_IP)/inc/common
EXTRA_CFLAGS      +=  -Idrivers/mstar/nand/$(CONFIG_MSTAR_NAND_IP)/inc/config

# specific options
obj-y             += mdrv_nand_io_stb.o
obj-y             += mdrv_nand.o mdrv_nand_io.o $(CONFIG_MSTAR_NAND_IP)/
obj-y             += unfdblk.o
