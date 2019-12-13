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

.PHONY: ump_pa clean_ump_pa

BUILD_TARGET += ump_pa
CLEAN_TARGET += clean_ump_pa

ump_pa_dir = drivers/base/ump/src/imports/pa 

ump_pa:
ifeq ($(Q),)
	@echo '[MAKE] $(ump_pa_dir)'
endif
	$(Q)make -C "$(ump_pa_dir)" PLATFORM=$(platform) && cp "$(ump_pa_dir)/ump_pa_import.ko" "$(BIN_DIR)"

clean_ump_pa:
ifeq ($(Q),)
	@echo '[CLEAN] $(ump_pa_dir)'
endif
	$(Q)make -C "$(ump_pa_dir)" clean && rm -f "$(BIN_DIR)/ump_pa_import.ko"
