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

mstar_platform=monet

# gpu core
gpu=t72x
hwver=r1p1

# cpu core
arch=arm_v7
core=cortex-a9

# config
mali_max_freq=504 # in MHz
mali_min_freq=504 # in MHz
mali_irq=E_IRQEXPH_G3D2MCU
mali_reg_offset=0x00340000

afbc=0 # not supported in T720
