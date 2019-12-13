#
# Makefile for MStar device drivers.
#

obj-$(CONFIG_MSTAR_IOMAP)                           += drv/iomap/
#obj-$(CONFIG_MSTAR_MSYSTEM)                        += drv/msystem/
#obj-$(CONFIG_MSTAR_FW)                             += drv/firmware/
obj-$(CONFIG_MSTAR_MPOOL)                           += drv/mpool/
obj-$(CONFIG_MSTAR_IIC)                             += drv/iic/
obj-$(CONFIG_MSTAR_GPIO)                            += drv/gpio/
obj-$(CONFIG_MSTAR_RTC)                             += drv/rtc/
#obj-$(CONFIG_MSTAR_HW_SEM)                         += drv/hw_sem/
obj-$(CONFIG_MSTAR_EMAC)                            += drv/emac/
obj-$(CONFIG_MSTAR_SYSTEM)                          += drv/system/
obj-$(CONFIG_MSTAR_IR)                              += drv/ir/
#obj-$(CONFIG_MSTAR_SOFTWARE_IR)                    += drv/software_ir/
obj-$(CONFIG_MSTAR_NAND)                            += drv/nand/
#obj-$(CONFIG_MSTAR_SPINAND)                        += drv/spinand/
obj-$(CONFIG_MSTAR_SPI_NAND)                        += drv/spinand/
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifeq ($(CONFIG_MSTAR_CHIP_NAME),$(filter $(CONFIG_MSTAR_CHIP_NAME),clippers eiffel einstein kaiser kano kayla kirin kiwi kentucky kratos kris madison maldives manhattan miami monaco monet muji munich napoli ))
obj-$(CONFIG_MSTAR_SDMMC)                           += drv/sdmmc/
else
obj-$(CONFIG_MSTAR_FCIE_HOST)                       += drv/fcie/
obj-$(CONFIG_MSTAR_SDIO_HOST)                       += drv/sdio/
endif
obj-$(CONFIG_MMC_MSTAR_MMC_EMMC)                    += drv/mmc/
obj-$(CONFIG_MSTAR_MBX)                             += drv/mbx/
obj-$(CONFIG_MSTAR_FUSION)                          += drv/fusion/
obj-$(CONFIG_MSTAR_FUSION_8_10)                     += drv/fusion_8_10/
obj-$(CONFIG_MSTAR_FUSION_9_03)                     += drv/fusion_9_03/
#obj-$(CONFIG_MSTAR_ONENAND)                        += drv/onenand/
obj-$(CONFIG_MSTAR_GFLIP)                           += drv/gflip/
#obj-$(CONFIG_MSTAR_RFID)                           += drv/rfid/
obj-$(CONFIG_MSTAR_SPI_FLASH)                       += drv/flash/
#obj-$(CONFIG_MSTAR_VPOOL)                          += drv/vid_pre_alloc_mem/
#obj-$(CONFIG_MSTAR_MSGLOG)                         += drv/msglog/
#obj-$(CONFIG_MSTAR_MSGIO)                          += drv/msgio/
#obj-$(CONFIG_MSTAR_TSP)                            += drv/tsp/
obj-$(CONFIG_MSTAR_SC)                              += drv/sc/
obj-$(CONFIG_MSTAR_XC)                              += drv/xc/
#obj-$(CONFIG_MSTAR_G3D)                            += drv/g3d/
#obj-$(CONFIG_MSTAR_MPIF)                           += drv/mpif/
obj-$(CONFIG_MSTAR_ALSA_AUDIO_DRIVER_DRV)           += drv/alsa_audio_driver/
obj-$(CONFIG_MSTAR_SEMUTEX)                         += drv/semutex/
obj-$(CONFIG_MSTAR_GMAC)                            += drv/gmac/
obj-$(CONFIG_TZ_SUPPORT)                            += drv/trustzone/
obj-$(CONFIG_XZRAM)                                 += drv/xzram/
#obj-$(CONFIG_MSTAR_FB)                             += drv/mstar_fb/
obj-$(CONFIG_MSTAR_SATA_HOST)                       += drv/sata_host/
obj-$(CONFIG_MSTAR_RAMLOG)                          += drv/ramlog/
#obj-$(CONFIG_MSTAR_MTLB)                           += drv/mtlb/
#obj-$(CONFIG_MSTAR_EMAC2)                          += drv/emac2/
#obj-$(CONFIG_MSTAR_PM_ADDR)                        += drv/pm_addr/
#obj-$(CONFIG_MSTAR_CPU_setting)                    += drv/CPU_setting/
obj-$(CONFIG_MSTAR_CPU_calibrating)                 += drv/CPU_calibrating/
obj-$(CONFIG_MSTAR_DVFS)                            += drv/dvfs/
obj-$(CONFIG_MALI400)                               += drv/mali/
#obj-$(CONFIG_UMP)                                  += drv/ump/
obj-$(CONFIG_MSTAR_ZDEC)                            += drv/zdec/
#obj-y                                              += uarttp/
obj-$(CONFIG_MSTAR_MTLB)                            += drv/mtlb/
obj-$(CONFIG_MSTAR_CMAPOOL)                         += drv/cma_pool/
obj-$(CONFIG_MSTAR_MIU)                             += drv/miu/
obj-$(CONFIG_IPANIC)                                += drv/ipanic/
ifneq ($(CONFIG_MSTAR_UTOPIA2K),)
  obj-$(CONFIG_MSTAR_UTOPIA2K)                      += utopia2k/build/
endif
obj-$(CONFIG_MSTAR_SOFTWARE_IR_MODULE)              += drv/software_ir/
obj-$(CONFIG_MSTAR_ENABLE_ENV_HANDLER)              += drv/env_handler/
#obj-$(CONFIG_MSTAR_TUNER)                          += drv/tuner/
obj-$(CONFIG_MSTAR_PM_ADDR)                         += drv/pm_addr/
obj-$(CONFIG_MSTAR_UTOPIA2K_STR)                     += drv/utopia2k_str/
obj-$(CONFIG_MSTAR_PM)                              += drv/pm/
obj-$(CONFIG_HAS_LD)                         		+= drv/ldm/
obj-$(CONFIG_MSTAR_CPU_IDLE)                                += drv/cpuidle/
