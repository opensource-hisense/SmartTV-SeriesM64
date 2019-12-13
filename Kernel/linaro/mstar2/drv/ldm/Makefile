#
# Makefile for MStar localdimming device drivers.
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
EXTRA_CFLAGS      += -Idrivers/mstar2/drv/ldm/include
EXTRA_CFLAGS      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/ldm/
EXTRA_CFLAGS 	  += -Idrivers/mstar2/drv/mbx


EXTRA_CFLAGS      +=  -DRED_LION


# build moudle
ifeq ($(CONFIG_HAS_LD),m)
obj-m += mdrv_ldm.o
mdrv_ldm-objs +=	mdrv_ldm_init.o
mdrv_ldm-objs +=	mdrv_ldm_io.o
mdrv_ldm-objs +=	mdrv_ldm_parse.o
mdrv_ldm-objs +=	mdrv_ldm_algorithm.o
mdrv_ldm-objs +=	mdrv_ldm_interrupt.o
mdrv_ldm-objs += ../../hal/$(CONFIG_MSTAR_CHIP_NAME)/ldm/mhal_ldm.o
endif

# build-in
ifeq ($(CONFIG_HAS_LD),y)
obj-$(CONFIG_HAS_LD)	+= mdrv_ldm_init.o mdrv_ldm_io.o mdrv_ldm_parse.o mdrv_ldm_algorithm.o mdrv_ldm_interrupt.o
obj-$(CONFIG_HAS_LD)	+= ../../hal/$(CONFIG_MSTAR_CHIP_NAME)/ldm/mhal_ldm.o
endif

