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

makefile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(makefile_path)))

PROFILES_DIR := $(current_dir)/profiles

export MALI_BUILD_EXTMOD := 1
include $(current_dir)/utils.mak
include $(current_dir)/mstar_settings.mak
include $(current_dir)/env.mak
include $(current_dir)/options.mak
include $(current_dir)/kernel_config_defines.mak
