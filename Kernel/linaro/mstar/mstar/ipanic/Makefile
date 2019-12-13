#
# Makefile for MStar ipanic drivers.
#

# general options
EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/ipanic

# specific options
EXTRA_CFLAGS      +=  -Iinclude/linux


ipanic-y        := ipanic-utils.o ipanic-emmc.o dump-process.o
obj-$(CONFIG_IPANIC)        += ipanic.o



