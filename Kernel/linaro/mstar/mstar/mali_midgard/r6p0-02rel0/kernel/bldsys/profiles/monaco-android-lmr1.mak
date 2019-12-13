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

# import predefined profiles
include $(PROFILES_DIR)/default.mak
include $(PROFILES_DIR)/android.mak
include $(PROFILES_DIR)/release.mak

# import platform config
include $(PROFILES_DIR)/monaco.mak

# common config
project=android_lmr1
android_version=future
ump=0
winsys_dma_buf=1
cl=1
renderscript=0
opencl_platform_config=config_t6xx
enable_dvfs=1

# kernel driver config
mali_irq=E_IRQEXPH_G3D2MCU

# user driver config
android_gralloc_path=hardware/mstar/gralloc2
simd=1