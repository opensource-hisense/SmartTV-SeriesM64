dtb-$(CONFIG_ARCH_VEXPRESS) += $(subst ",,$(CONFIG_MSTAR_CHIP_NAME)_squashfs).dtb
dtb-$(CONFIG_ARCH_VEXPRESS) += $(subst ",,$(CONFIG_MSTAR_CHIP_NAME)_ext4).dtb
dtb-$(CONFIG_ARCH_VEXPRESS) += $(subst ",,$(CONFIG_MSTAR_CHIP_NAME)_ramdisk).dtb

targets += dtbs
targets += $(dtb-y)

dtbs: $(addprefix $(obj)/, $(dtb-y))

clean-files := *.dtb
