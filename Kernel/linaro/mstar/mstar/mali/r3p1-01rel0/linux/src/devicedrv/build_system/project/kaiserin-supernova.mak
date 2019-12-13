#
# Copyright (C) 2010-2012 MStar Semiconductor, Inc. All rights reserved.
# 
# This program is free software and is provided to you under the terms of the GNU General Public License version 2
# as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
# 
# A copy of the licence is included with the program, and can also be obtained from Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

include $(if $(KBUILD_EXTMOD),$(KBUILD_EXTMOD)/,)../build_system/platform/kaiserin.mak

# Mali build options
USING_UMP = 0

BUILD ?= debug

# toolchain
export CROSS_COMPILE ?= mips-linux-gnu-

# project build flags
CONFIG_BUILDFLAGS := -DHIGH_MEMORY_NOT_SUPPORTED

# IRQ
MALI_IRQ := E_IRQEXPH_GPU

# device ID
USING_FIXED_DEVID = 1
