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
include $(PROFILES_DIR)/linux.mak
include $(PROFILES_DIR)/release.mak

# import platform config
include $(PROFILES_DIR)/kano32.mak

# common config
project=supernova

# kernel driver config
fixed_device_id=1

# user driver config
winsys=directfb
directfb_version=1.4.2

# egl extensions
egl_extensions=egl_swap_buffers_region