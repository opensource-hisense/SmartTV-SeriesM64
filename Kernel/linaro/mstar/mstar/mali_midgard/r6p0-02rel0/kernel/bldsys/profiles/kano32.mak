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

mstar_platform=kano

# gpu core
gpu=t82x
hwver=r1p0

# cpu core
arch=arm_v7
# TBD
core=cortex-a9 # temporarily specify a9 instead of a12 before the toolchain supports it
simd=1
miu_num=2
toolchain_prefix_target=arm-none-linux-gnueabi-
kernel_arch=arm

# kernel driver config
mali_irq=E_IRQHYPL_G3D2MCU_DFT

# config
mali_max_freq=550 # in MHz
mali_min_freq=550 # in MHz
mali_base_address=0xfd360000