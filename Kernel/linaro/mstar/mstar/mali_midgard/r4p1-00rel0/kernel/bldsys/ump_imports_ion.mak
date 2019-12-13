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

.PHONY: ump_ion clean_ump_ion

BUILD_TARGET += ump_ion
CLEAN_TARGET += clean_ump_ion

ump_ion_dir = drivers/base/ump/src/imports/ion

ump_ion:
ifeq ($(Q),)
	@echo '[MAKE] $(ump_ion_dir)'
endif
	$(Q)make -C "$(ump_ion_dir)" PLATFORM=$(platform) && cp "$(ump_ion_dir)/ump_ion_import.ko" "$(BIN_DIR)"

clean_ump_ion:
ifeq ($(Q),)
	@echo '[CLEAN] "$(ump_ion_dir)"'
endif
	$(Q)make -C "$(ump_ion_dir)" clean && rm -f "$(BIN_DIR)/ump_ion_import.ko"
