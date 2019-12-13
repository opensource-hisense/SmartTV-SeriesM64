#
# Makefile for MStar eMMC host drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

# general options
EXTRA_CFLAGS	+= -Idrivers/mstar/mmc/host/eMMCDrv/inc/api/
EXTRA_CFLAGS	+= -Idrivers/mstar/mmc/host/eMMCDrv/inc/common/
EXTRA_CFLAGS	+= -Idrivers/mstar/mmc/host/eMMCDrv/inc/config/
EXTRA_CFLAGS	+= -Werror

# specific options
EXTRA_LDFLAGS	+= -Tdrivers/mstar/mmc/mmc.lds

# files
##ifdef CONFIG_MSTAR_AMBER3
##obj-y               += host/mstar_mci_a3.o 
##else
#obj-y               += host/mstar_mci.o 
#obj-y	            += host/eMMCDrv/src/api/eMMC_prg.o
#obj-y	            += host/eMMCDrv/src/common/eMMC_hal.o
#obj-y	            += host/eMMCDrv/src/common/eMMC_hal_speed.o
#obj-y	            += host/eMMCDrv/src/common/eMMC_ip_verify.o
#obj-y	            += host/eMMCDrv/src/common/eMMC_utl.o
#obj-y	            += host/eMMCDrv/src/config/eMMC_platform.o
##endif

obj-y		+= host/eMMCDrv/src/api/eMMC_prg.o

ifeq ($(CONFIG_MSTAR_CHIP_NAME),$(filter $(CONFIG_MSTAR_CHIP_NAME), miami))
	obj-y	+= host/eMMCDrv/src/common/hal_fcie5.o
	obj-y	+= host/eMMCDrv/src/common/eMMC_hal_v5.o
	obj-y	+= host/mstar_mci_v5.o
else
	obj-y	+= host/eMMCDrv/src/common/eMMC_hal.o
	obj-y	+= host/eMMCDrv/src/common/eMMC_hal_speed.o
	obj-y	+= host/mstar_mci.o 
endif

obj-y		+= host/eMMCDrv/src/common/eMMC_ip_verify.o
obj-y		+= host/eMMCDrv/src/common/eMMC_utl.o
obj-y		+= host/eMMCDrv/src/config/eMMC_platform.o


