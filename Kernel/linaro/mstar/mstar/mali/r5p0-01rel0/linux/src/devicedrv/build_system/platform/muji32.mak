#
# Copyright (C) 2010-2015 MStar Semiconductor, Inc. All rights reserved.
#
# This program is free software and is provided to you under the terms of the GNU General Public License version 2
# as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
#
# A copy of the licence is included with the program, and can also be obtained from Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

# Muji platform options
ARCH=arm
TARGET_PLATFORM=mstar
TARGET_GPU=MALI450
MSTAR_PLATFORM=muji
USING_RIU=1
RIU_ADDRESS_TYPE=32
MSTAR_MIU_HIGH_WAY=1

# GPU config
#GPU_BASE_ADDRESS:=$(if $(findstring 1,$(USING_RIU)),0xfd340000,0x1f800000)
GPU_CLOCK=600
NUM_PP=4

# Linux kernel config checking
CHECK_CONFIG_CHIP_NAME=1
