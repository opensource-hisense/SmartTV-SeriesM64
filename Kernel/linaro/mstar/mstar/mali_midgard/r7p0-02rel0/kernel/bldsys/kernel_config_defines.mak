# Copyright (c) 2014 MStar Semiconductor, Inc. All rights reserved.
#
# This program is free software and is provided to you under the terms of the
# GNU General Public License version 2 as published by the Free Software
# Foundation, and any use by you of this program is subject to the terms
# of such GNU licence.
#
# A copy of the licence is included with the program, and can also be obtained
# from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA  02110-1301, USA.

ifeq ($(mstar),1)
platform_config := mstar

# memory import options
ump_monolithic = 1
ump_pa = 1
ump_ion = 0
umm_import = 1

# os
ifeq ($(os),linux)
progs_install = bin
libs_install = bin
endif

ifneq ($(toolchain_prefix_target),)
export CROSS_COMPILE := $(toolchain_prefix_target)
$(info CROSS_COMPILE is $(CROSS_COMPILE))
endif

ifeq ($(os),android)
gralloc = 0
fixed_device_id = 0
endif

ifeq ($(arch),arm_v7)
ENV = arm
ARCH = arm
else
ifeq ($(arch),arm_v8)
ENV = arm64
ARCH = arm64
endif
endif

endif

SCONS_CFLAGS =
SCONS_CONFIGS =

# GCC 4.7 on AOSP 4.3 contains a defect which didn't allow us to use the "_Pragma" syntax, where an error message
# is returned:
#
# "internal compiler error: unspellable token PRAGMA"
#
# This regression has thus far only been seen on the GCC 4.7 compiler bundled with AOSP 4.3.0.  GCC 4.7
# issue tracked at MIDCOM-4598.
#
# The workaround entails:
# - Overriding the global Android GCC 'Werror=address' flag, by adding the GCC flag 'Wno-address' to
#   local C flags in the Android makefile for the DDK, as is done below.
# - Conditionally compiled use of "_Pragma("..."), so it is _not_ compiled if the workaround is active.
#
# The C code workarounds have a GCC version range bigger than just v4.7, because of a pre-existing workaround,
# presumably to a similar defect.
MALI_GCC_WORKAROUND_MIDCOM_4598 := 0
ifeq ($(toolchains) $(debug) $(os),gnu 1 android)
MALI_GCC_WORKAROUND_MIDCOM_4598 := 1
endif

SCONS_CFLAGS += -DMALI_GCC_WORKAROUND_MIDCOM_4598=$(MALI_GCC_WORKAROUND_MIDCOM_4598)

CONFIG_MALI_MIDGARD ?= m

ifneq ($(os),android)
SCONS_CFLAGS += -DCONFIG_KDS
SCONS_CONFIGS += CONFIG_KDS=m CONFIG_MALI_MIDGARD=$(CONFIG_MALI_MIDGARD)
else
SCONS_CONFIGS += CONFIG_MALI_MIDGARD=$(CONFIG_MALI_MIDGARD)
endif

ifneq ($(filter leak, $(base_qa)),)
SCONS_CFLAGS += -DCONFIG_MALI_QA_LEAK
SCONS_CONFIGS += CONFIG_MALI_QA_LEAK=y
endif

ifneq ($(filter resfail, $(base_qa)),)
SCONS_CFLAGS += -DCONFIG_MALI_QA_RESFAIL
SCONS_CONFIGS += CONFIG_MALI_QA_RESFAIL=y
endif

ifeq ($(debug),1)
SCONS_CFLAGS += -DCONFIG_MALI_DEBUG
SCONS_CONFIGS += CONFIG_MALI_DEBUG=y
endif

ifeq ($(ump),1)
SCONS_CFLAGS += -DCONFIG_UMP
SCONS_CONFIGS += CONFIG_UMP=y
endif

ifeq ($(gator),1)
SCONS_CFLAGS += -DCONFIG_MALI_GATOR_SUPPORT
SCONS_CONFIGS += CONFIG_MALI_GATOR_SUPPORT=y
endif

ifeq ($(no_mali),1)
SCONS_CFLAGS += -DCONFIG_MALI_NO_MALI
SCONS_CONFIGS += CONFIG_MALI_NO_MALI=y
ifneq ($(filter t76x tFRx t86x,$(gpu)),)
SCONS_CFLAGS += -DCONFIG_MALI_PRODUCT_ID_T76X=y
SCONS_CONFIGS += CONFIG_MALI_PRODUCT_ID_T76X=y
endif
endif

ifeq ($(mcu),1)
SCONS_CFLAGS += -DCONFIG_MALI_MCU
SCONS_CONFIGS += CONFIG_MALI_MCU=y
ifeq ($(virt_guest),0)
SCONS_CFLAGS += -DCONFIG_MALI_VIRTUALIZATION_HOST
SCONS_CONFIGS += CONFIG_MALI_VIRTUALIZATION_HOST=y
endif
ifeq ($(virt_guest),1)
SCONS_CFLAGS += -DCONFIG_MALI_VIRTUALIZATION_GUEST
SCONS_CONFIGS += CONFIG_MALI_VIRTUALIZATION_GUEST=y
endif
endif

ifeq ($(if_size),1)
SCONS_CFLAGS += -DCONFIG_MALI_IF_LARGE
SCONS_CONFIGS += CONFIG_MALI_IF_LARGE=y
endif

ifeq ($(aarch64_mmu),1)
SCONS_CFLAGS += -DCONFIG_MALI_GPU_MMU_AARCH64
SCONS_CONFIGS += CONFIG_MALI_GPU_MMU_AARCH64=y
endif

ifeq ($(error_inject),1)
SCONS_CFLAGS += -DCONFIG_MALI_ERROR_INJECTION
SCONS_CONFIGS += CONFIG_MALI_ERROR_INJECTION=y
endif

ifeq ($(system_trace),1)
SCONS_CFLAGS += -DCONFIG_MALI_SYSTEM_TRACE
SCONS_CONFIGS += CONFIG_MALI_SYSTEM_TRACE=y
endif

SCONS_CFLAGS_FAKE = -DCONFIG_MALI_PLATFORM_FAKE
SCONS_CONFIGS_FAKE = CONFIG_MALI_PLATFORM_FAKE=y

ifneq ($(call findtoken, $(platform_config), vexpress rtsm_ve juno vexpress_6xvirtex7_10mhz goldfish a7_kipling pbx),)
platform_config_uc := $(call uc,$(platform_config))
SCONS_CFLAGS += -DCONFIG_MALI_PLATFORM_$(platform_config_uc)
SCONS_CONFIGS += CONFIG_MALI_PLATFORM_$(platform_config_uc)=y
else
ifeq ($(platform_config),pandaboard)
SCONS_CFLAGS += -DCONFIG_MALI_PLATFORM_PANDA
SCONS_CONFIGS += CONFIG_MALI_PLATFORM_PANDA=y
else
ifeq ($(platform_config),devicetree)
SCONS_CFLAGS += -DCONFIG_MALI_DEVFREQ -DCONFIG_MALI_PLATFORM_DEVICETREE
SCONS_CONFIGS += CONFIG_MALI_DEVFREQ=y CONFIG_MALI_PLATFORM_DEVICETREE=y
else
SCONS_CFLAGS += -DCONFIG_MALI_PLATFORM_THIRDPARTY
SCONS_CONFIGS += CONFIG_MALI_PLATFORM_THIRDPARTY=y CONFIG_MALI_PLATFORM_THIRDPARTY_NAME=$(platform_config)
endif
endif
endif

ifeq ($(error_inject),1)
SCONS_CFLAGS += -DCONFIG_MALI_ERROR_INJECT
SCONS_CONFIGS += CONFIG_MALI_ERROR_INJECT=y
endif
ifeq ($(error_inject),2)
SCONS_CFLAGS += -DCONFIG_MALI_ERROR_INJECT -DCONFIG_MALI_ERROR_INJECT_RANDOM
SCONS_CONFIGS += CONFIG_MALI_ERROR_INJECT=y CONFIG_MALI_ERROR_INJECT_RANDOM=y
endif

ifneq ($(instr),0)
SCONS_CFLAGS += -DCONFIG_MALI_TRACE_TIMELINE
SCONS_CONFIGS += CONFIG_MALI_TRACE_TIMELINE=y
ifneq ($(filter 0 2, $(gator)),)
ifneq ($(ktlstream),0)
SCONS_CFLAGS += -DCONFIG_MALI_MIPE_ENABLED
SCONS_CONFIGS += CONFIG_MALI_MIPE_ENABLED=y
endif
endif
endif

# =========================================================== to be deleted
# Force Fully coherent
#ifeq ($(hw_cc),1)
# treat all gpu memory as coherent
#SCONS_CFLAGS += '-DCONFIG_MALI_CACHE_COHERENT '
#SCONS_CONFIGS += 'CONFIG_MALI_CACHE_COHERENT=y '
# use coherent kernel gpu memory
#SCONS_CFLAGS += '-DCONFIG_MALI_COH_KERN '
#SCONS_CONFIGS += 'CONFIG_MALI_COH_KERN=y '
# use coherent page tables support
# Temporariy disabled due to hardware issue
#SCONS_CFLAGS += '-DCONFIG_MALI_COH_PAGES '
#SCONS_CONFIGS += 'DCONFIG_MALI_COH_PAGES=y '
#endif
# ===========================================================

# This is purely for hardware validation of coherency.
# Expect defines and related code to be removed mid-long term.
ifeq ($(hw_cc),1)
# Default to ACE mode
MALI_FORCE_ACELITE := 0
# User space is coherent with GPU ( CPU cache maintenance disabled )
MALI_COH_USER := 1
# All allocations are marked as system coherent
MALI_GPU_OUTER := 1
# All allocations are marked as cpu cached
MALI_CACHED_CPU := 1
# GPU is cohernet with CPU ( GPU cache maintenance disabled )
MALI_COH_GPU := 1
# Kernel driver is coherent with GPU (Kbase cache maintenance disabled)
MALI_COH_KERN := 1
# GPU page tables are coherent with CPU
MALI_COH_PAGES := 1
else
ifeq ($(hw_cc),2)
# Force ACE-Lite even if ACE supported
MALI_FORCE_ACELITE := 1
# GPU is cohernet with CPU ( GPU cache maintenance disabled )
MALI_COH_USER := 1
# All allocations are marked as system coherent
MALI_GPU_OUTER := 1
# All allocations are marked cpu cached as required
MALI_CACHED_CPU := 0
# GPU cache maintenance is necessary
MALI_COH_GPU := 0
# Kernel driver is not coherent with GPU (Kbase cache maintenance enabled)
MALI_COH_KERN := 0
# GPU page tables are coherent with CPU
MALI_COH_PAGES := 1
else
MALI_FORCE_ACELITE := 0
MALI_COH_USER := 0
MALI_GPU_OUTER := 0
MALI_CACHED_CPU := 0
MALI_COH_GPU := 0
MALI_COH_KERN := 0
MALI_COH_PAGES := 0
endif
endif

# **** Begin coherency validation settings ****
# Set kernel configuration required for hw validation of coherency.
# Expect to be removed mid-long term.
ifeq ($(MALI_COH_USER),1)
SCONS_CFLAGS += -DCONFIG_MALI_COH_USER
SCONS_CONFIGS += CONFIG_MALI_COH_USER=y
endif
ifeq ($(MALI_GPU_OUTER),1)
SCONS_CFLAGS += -DCONFIG_MALI_GPU_OUTER
SCONS_CONFIGS += CONFIG_MALI_GPU_OUTER=y
endif
ifeq ($(MALI_CACHED_CPU),1)
SCONS_CFLAGS += -DCONFIG_MALI_CACHED_CPU
SCONS_CONFIGS += CONFIG_MALI_CACHED_CPU=y
endif
ifeq ($(MALI_FORCE_ACELITE),1)
SCONS_CFLAGS += -DCONFIG_MALI_FORCE_ACELITE
SCONS_CONFIGS += CONFIG_MALI_FORCE_ACELITE=y
endif
ifeq ($(MALI_COH_GPU),1)
SCONS_CFLAGS += -DCONFIG_MALI_COH_GPU
SCONS_CONFIGS += CONFIG_MALI_COH_GPU=y
endif
ifeq ($(MALI_COH_KERN),1)
SCONS_CFLAGS += -DCONFIG_MALI_COH_KERN
SCONS_CONFIGS += CONFIG_MALI_COH_KERN=y
endif
ifeq ($(MALI_COH_PAGES),1)
SCONS_CFLAGS += -DCONFIG_MALI_COH_PAGES
SCONS_CONFIGS += CONFIG_MALI_COH_PAGES=y
endif
# **** End coherency validation settings****

SCONS_CFLAGS += -DCONFIG_MALI_BACKEND=$(kernel_backend)
SCONS_CONFIGS += CONFIG_MALI_BACKEND=$(kernel_backend)

ifeq ($(buslog),1)
SCONS_CFLAGS += -DCONFIG_MALI_FPGA_BUS_LOGGER
SCONS_CONFIGS += CONFIG_MALI_FPGA_BUS_LOGGER=y
endif

# MStar build flags
ifeq ($(mstar),1)
SCONS_CFLAGS += -DMSTAR
SCONS_CFLAGS += -D$(call uc, $(mstar_platform))
SCONS_CFLAGS += -D$(call uc, $(project))
SCONS_CFLAGS += -DMALI_MAX_FREQ=$(mali_max_freq)
SCONS_CFLAGS += -DMALI_MIN_FREQ=$(mali_min_freq)
SCONS_CFLAGS += -DMALI_IRQ=$(mali_irq)
SCONS_CFLAGS += -DMALI_REG_OFFSET=$(mali_reg_offset)
SCONS_CFLAGS += -DMSTAR_RIU_ENABLED
SCONS_CFLAGS += -DMSTAR_PM_CALLBACKS
SCONS_CFLAGS += -DMSTAR_PM_DVFS_PERIOD=500 # 500 ms
SCONS_CFLAGS += -DMSTAR_PM_GPU_POWEROFF_TICK_NS=1000000000 # 1 sec
SCONS_CFLAGS += -DMSTAR_USE_FIXED_DEVID=$(fixed_device_id)
SCONS_CFLAGS += -DMSTAR_UMP_MONOLITHIC=$(ump_monolithic)
SCONS_CFLAGS += -DMSTAR_UMP_IMPORT_PA=$(ump_pa)
SCONS_CFLAGS += -DMSTAR_UMP_IMPORT_ION=$(ump_ion)
SCONS_CFLAGS += -DMSTAR_UMM_IMPORT=$(umm_import)
SCONS_CFLAGS += -DMSTAR_DISABLE_CONFIG_OF
# XXX: kernel enables CONFIG_NEED_SG_DMA_LENGTH, but dma_length is not set correctly in kernel.
#      we disable this config internally in mali driver to use length instead of dma_length.
SCONS_CFLAGS += -DMSTAR_DISABLE_CONFIG_NEED_SG_DMA_LENGTH
SCONS_CFLAGS += -DMSTAR_GPU_UTILIZATION
SCONS_CFLAGS += -DMSTAR_MEMORY_USAGE
SCONS_CFLAGS += -DCONFIG_MALI_MIDGARD_DVFS

SCONS_CONFIGS += MSTAR_PLATFORM_NAME=$(mstar_platform)
SCONS_CONFIGS += MSTAR_UMP_MONOLITHIC=$(ump_monolithic)
SCONS_CONFIGS += MSTAR_UMP_IMPORT_PA=$(ump_pa)
SCONS_CONFIGS += MSTAR_UMP_IMPORT_ION=$(ump_ion)
SCONS_CONFIGS += MSTAR_UMM_IMPORT=$(umm_import)
endif

kernel_get_configs = $(SCONS_CONFIGS) $(call if_eq, $1, 1, $(SCONS_CONFIGS_FAKE), )
kernel_get_defines = $(SCONS_CFLAGS) $(call if_eq, $1, 1, $(SCONS_CFLAGS_FAKE), )
