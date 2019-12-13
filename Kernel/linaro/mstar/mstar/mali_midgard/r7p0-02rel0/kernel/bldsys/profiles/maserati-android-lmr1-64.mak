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

# import default profiles
include $(PROFILES_DIR)/default.mak
include $(PROFILES_DIR)/release.mak

# import platform profiles
include $(PROFILES_DIR)/maserati.mak
include $(PROFILES_DIR)/android.mak
include $(PROFILES_DIR)/arm64.mak

# project config
project=android_lmr1
android_version=lollipopmr1
android_gralloc_path=hardware/mstar/gralloc2
