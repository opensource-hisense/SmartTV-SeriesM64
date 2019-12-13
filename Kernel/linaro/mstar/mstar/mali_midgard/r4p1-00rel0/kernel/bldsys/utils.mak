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

uc = $(strip $(shell echo $1 | tr a-z A-Z))

empty := 
space := $(empty) $(empty)
findtoken = $(strip $(findstring $(space)$(strip $1)$(space), $(strip $2) ))

define add_option
$(eval $1 ?= $2)\
$(eval cur_val_ := $(strip $($(strip $1))))\
$(eval $1 := $(cur_val_))
endef

define add_enum_option
$(call add_option, $1, $2)\
$(if $(cur_val_),$(if $(call findtoken,$(cur_val_),$3),,\
    $(error Invalid value for option $(strip $1): $(cur_val_). Valid values are: $(strip $3))))
endef

define add_list_option
$(call add_option, $1, $2)\
$(if $(cur_val_),$(foreach v,$2,,$(if $(call findtoken,$(v),$3),,\
    $(error Invalid value for option $(strip $1): $(cur_val_). Valid values are: $(strip $3)))))
endef

kds_in_kernel = $(shell grep -q '^[\ ]*CONFIG_KDS[\ ]*=[\ ]*y' '$(KDIR)/.config' && echo -n 1 || echo -n 0)

get_version_info = $(strip $(shell cd $(BLDSYS_DIR); python -c 'import version_info; print version_info.REVISION'))

if_eq = $(if $(call findtoken,$(strip $1),$(strip $2)),$(strip $3),$(strip $4))
