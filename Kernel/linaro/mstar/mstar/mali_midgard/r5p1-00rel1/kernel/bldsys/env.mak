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

export MALI_RELEASE_NAME ?= r5p1-00rel1-CL$(get_version_info)
export UMP_RELEASE_NAME ?= r5p1-00rel1-CL$(get_version_info)
export HOME ?= $(shell pwd)
# MSTAR patch begin
# MStar: default to arm
export ARCH ?= arm
# MStar: default to arm-none-linux-gnueabi-
export CROSS_COMPILE ?= arm-none-linux-gnueabi-
# MSTAR patch end
export PKG_CONFIG ?= pkg-config

mali_release_name := $(MALI_RELEASE_NAME)

$(if $(KDIR),,$(error KDIR must be specified))
