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

BLDSYS_DIR := $(CURDIR)/bldsys
PROFILES_DIR := $(BLDSYS_DIR)/profiles
BIN_DIR := bin

include $(BLDSYS_DIR)/utils.mak
include $(BLDSYS_DIR)/mstar_settings.mak
include $(BLDSYS_DIR)/env.mak
include $(BLDSYS_DIR)/options.mak
include $(BLDSYS_DIR)/kernel_config_defines.mak

.PHONY: all clean setup_bin_dir

all:

include $(BLDSYS_DIR)/kds.mak
include $(BLDSYS_DIR)/ump.mak
include $(BLDSYS_DIR)/kbase.mak

all: setup_bin_dir $(BUILD_TARGET) strip_modules
clean: $(CLEAN_TARGET) clean_bin_dir

setup_bin_dir:
	@mkdir -p "$(BIN_DIR)"

clean_bin_dir:
	-@if [ -d "$(BIN_DIR)" ]; then rmdir "$(BIN_DIR)"; fi

strip_modules:
	@$(CROSS_COMPILE)strip --strip-unneeded $(BIN_DIR)/*.ko

$(BUILD_TARGET): | setup_bin_dir
strip_modules: | $(BUILD_TARGET)
clean_bin_dir: | $(CLEAN_TARGET)
