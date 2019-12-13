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

ifeq ($(ump),1)
.PHONY: ump clean_ump

BUILD_TARGET += ump
CLEAN_TARGET += clean_ump

ump_dir = drivers/base/ump/src

ump:
ifeq ($(Q),)
	@echo '[MAKE] $(ump_dir)'
endif
ifeq ($(unit),1)
	$(Q)make -C "$(ump_dir)" MALI_UNIT_TEST=$(unit) PLATFORM=$(platform) SCONS_CONFIGS="$(kernel_get_configs)" SCONS_CFLAGS="$(kernel_get_defines)" && cp "$(ump_dir)/ump.ko" "$(BIN_DIR)"
else
	$(Q)make -C "$(ump_dir)" PLATFORM=$(platform) SCONS_CONFIGS="$(kernel_get_configs)" SCONS_CFLAGS="$(kernel_get_defines)" && cp "$(ump_dir)/ump.ko" "$(BIN_DIR)" 
endif

clean_ump:
ifeq ($(Q),)
	@echo '[CLEAN] $(ump_dir)'
endif
	$(Q)make -C "$(ump_dir)" clean && rm -f "$(BIN_DIR)/ump.ko"

# MSTAR patch begin
ifeq ($(ump_monolithic),0)
-include ump_imports_*.mak
endif
# MSTAR patch end

endif
