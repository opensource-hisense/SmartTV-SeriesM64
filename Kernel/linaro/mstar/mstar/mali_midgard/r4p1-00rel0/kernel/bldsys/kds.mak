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

ifneq ($(os),android)
.PHONY: kds clean_kds

BUILD_TARGET += kds
CLEAN_TARGET += clean_kds

build_kds := 1
ifeq ($(kds_in_kernel),1)
# KDS in kernel. Do not build KDS kernel module but
# still allow for building kds_test module.
build_kds := 0
endif

kds_dir := drivers/base/kds

kds:
ifeq ($(build_kds),1)
ifeq ($(Q),)
	@echo '[MAKE] $(kds_dir)'
endif
	$(Q)make -C "$(kds_dir)" kds && cp "$(kds_dir)/kds.ko" "$(BIN_DIR)"
endif
ifeq ($(unit),1)
	$(Q)make -C "$(kds_dir)" kds_test && cp "$(kds_dir)/kds_test.ko" "$(BIN_DIR)"
endif

clean_kds:
ifeq ($(build_kds),1)
ifeq ($(Q),)
	@echo '[CLEAN] $(kds_dir)'
endif
	$(Q)make -C "$(kds_dir)" clean && rm -f "$(BIN_DIR)/kds.ko" "$(BIN_DIR)/kds_test.ko"
endif

endif
