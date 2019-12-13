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

ifneq ($(MALI_BUILD_EXTMOD),1)

makefile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(makefile_path)))

profile = $(MALI_PROFILE)

KDIR := dummy
PROFILES_DIR := $(current_dir)/profiles
CONFIG_MALI_MIDGARD ?= y

include $(current_dir)/utils.mak
include $(current_dir)/mstar_settings.mak
include $(current_dir)/env.mak
include $(current_dir)/options.mak
toolchain_prefix_target=
include $(current_dir)/kernel_config_defines.mak

mock_test := 0
ifneq ($(wildcard $(src)/tests/internal/src/mock),)
ifeq ($(unit),1)
mock_test := 1
endif
endif

# Fake platform is a transient solution for GPL drivers running in kernel that does not provide configuration via platform data.
# For such kernels fake_platform_device should be set to 1. For kernels providing platform data fake_platform_device should be set to 0.
fake_platform_device := 1

android := $(call if_eq, $(os), android, 1, 0)
kernel_test := $(call if_eq, $(unit), 1, 1, 0)

PLATFORM=$(platform)
MALI_ERROR_INJECT_ON=$(error_inject)
MALI_ANDROID=$(android)
MALI_KERNEL_TEST_API=$(kernel_test)
MALI_UNIT_TEST=$(unit)
MALI_RELEASE_NAME=$(mali_release_name)
MALI_MOCK_TEST=$(mock_test)
MALI_CUSTOMER_RELEASE=$(release)
MALI_INSTRUMENTATION_LEVEL=$(instr)
MALI_COVERAGE=$(coverage)
SCONS_CONFIGS:=$(call kernel_get_configs,$(fake_platform_device))
SCONS_CFLAGS:=$(call kernel_get_defines,$(fake_platform_device))

define newline


endef

empty :=
space := $(empty) $(empty)
$(eval $(subst $(space),$(newline),$(SCONS_CONFIGS)))

EXTRA_CFLAGS=-I$(CURDIR)/../../../../include $(SCONS_CFLAGS)

endif
