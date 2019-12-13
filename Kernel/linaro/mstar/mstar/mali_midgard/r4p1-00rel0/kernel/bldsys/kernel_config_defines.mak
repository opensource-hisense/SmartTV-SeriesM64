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

# fixed ump options
ump_monolithic = 1
ump_pa = 1
ump_ion = 0

# os
ifeq ($(os),linux)
progs_install = bin
libs_install = bin
mem_export_umm = 1
ifneq ($(toolchain_prefix_target),)
export CROSS_COMPILE := $(toolchain_prefix_target)
endif
endif

ifeq ($(os),android)
gralloc = 0
fixed_device_id = 0
mem_export_umm = 1
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

ifneq ($(os),android)
SCONS_CFLAGS += -DCONFIG_KDS
SCONS_CONFIGS += CONFIG_KDS=m CONFIG_MALI_MIDGARD=m
else
SCONS_CONFIGS += CONFIG_MALI_MIDGARD=m
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
SCONS_CFLAGS += -DCONFIG_DEBUG
SCONS_CONFIGS += CONFIG_DEBUG=y
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
ifeq ($(gpu),t76x)
SCONS_CFLAGS += -DCONFIG_MALI_PRODUCT_ID_T76X=y
SCONS_CONFIGS += CONFIG_MALI_PRODUCT_ID_T76X=y
endif
endif

ifeq ($(error_inject),1)
SCONS_CFLAGS += -DCONFIG_MALI_ERROR_INJECTION
SCONS_CONFIGS += CONFIG_MALI_ERROR_INJECTION=y
endif

SCONS_CFLAGS_FAKE = -DCONFIG_MALI_PLATFORM_FAKE
SCONS_CONFIGS_FAKE = CONFIG_MALI_PLATFORM_FAKE=y

ifneq ($(call findtoken, $(platform_config), vexpress rtsm_ve juno vexpress_virtex7_40mhz vexpress_6xvirtex7_10mhz goldfish pbx),)
platform_config_uc := $(call uc,$(platform_config))
SCONS_CFLAGS += -DCONFIG_MALI_PLATFORM_$(platform_config_uc)
SCONS_CONFIGS += CONFIG_MALI_PLATFORM_$(platform_config_uc)=y
else
ifeq ($(platform_config),pandaboard)
SCONS_CFLAGS += -DCONFIG_MALI_PLATFORM_PANDA
SCONS_CONFIGS += CONFIG_MALI_PLATFORM_PANDA=y
else
SCONS_CFLAGS += -DCONFIG_MALI_PLATFORM_THIRDPARTY
SCONS_CONFIGS += CONFIG_MALI_PLATFORM_THIRDPARTY_NAME=y CONFIG_MALI_PLATFORM_THIRDPARTY_NAME=$(platform_config)
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
endif

# MStar build flags
ifeq ($(mstar),1)
SCONS_CFLAGS += -DMSTAR
SCONS_CFLAGS += -D$(call uc, $(mstar_platform))
SCONS_CFLAGS += -D$(call uc, $(project))
SCONS_CFLAGS += -DMALI_MAX_FREQ=$(mali_max_freq)
SCONS_CFLAGS += -DMALI_MIN_FREQ=$(mali_min_freq)
SCONS_CFLAGS += -DMALI_BASE_ADDRESS=$(mali_base_address)
SCONS_CFLAGS += -DMALI_IRQ=$(mali_irq)
SCONS_CFLAGS += -DMSTAR_RIU_ENABLED
SCONS_CFLAGS += -DMSTAR_PM_POLICY_ALWAYS_ON
SCONS_CFLAGS += -DMSTAR_USE_FIXED_DEVID=$(fixed_device_id)
SCONS_CFLAGS += -DMSTAR_UMP_MONOLITHIC=$(ump_monolithic)
SCONS_CFLAGS += -DMSTAR_UMP_IMPORT_PA=$(ump_pa)
SCONS_CFLAGS += -DMSTAR_UMP_IMPORT_ION=$(ump_ion)
SCONS_CFLAGS += -DMSTAR_MEM_EXPORT_UMM=$(mem_export_umm)
SCONS_CFLAGS += -DMSTAR_MIU_NUM=$(miu_num)

SCONS_CONFIGS += MSTAR_PLATFORM_NAME=$(mstar_platform)
SCONS_CONFIGS += MSTAR_UMP_MONOLITHIC=$(ump_monolithic)
SCONS_CONFIGS += MSTAR_UMP_IMPORT_PA=$(ump_pa)
SCONS_CONFIGS += MSTAR_UMP_IMPORT_ION=$(ump_ion)
SCONS_CONFIGS += MSTAR_MEM_EXPORT_UMM=$(mem_export_umm)
endif

kernel_get_configs = $(SCONS_CONFIGS) $(call if_eq, $1, 1, $(SCONS_CONFIGS_FAKE), )
kernel_get_defines = $(SCONS_CFLAGS) $(call if_eq, $1, 1, $(SCONS_CFLAGS_FAKE), )
