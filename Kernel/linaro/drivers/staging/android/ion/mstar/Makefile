EXTRA_CFLAGS      +=  -Idrivers/mstar2/include
EXTRA_CFLAGS      +=  -Idrivers/mstar2/drv/mtlb

EXTRA_CFLAGS      +=  -Idrivers/mstar/include
EXTRA_CFLAGS      +=  -Idrivers/mstar/ion_mtlb

obj-y += mstar_ion.o
obj-$(CONFIG_MP_ION_PATCH_MSTAR_CUSTOMER_IOCTL) += ion_customer.o

ifdef CONFIG_COMPAT
obj-$(CONFIG_MP_ION_PATCH_MSTAR_CUSTOMER_IOCTL) += compat_ion_customer.o
endif
