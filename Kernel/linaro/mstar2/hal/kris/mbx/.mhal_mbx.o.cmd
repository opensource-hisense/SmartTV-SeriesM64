cmd_drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.o := mips-linux-gnu-gcc -Wp,-MD,drivers/mstar2/drv/mbx/../../hal/kris/mbx/.mhal_mbx.o.d  -nostdinc -isystem /opt/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include -Iarch/mips/include/generated  -Iinclude -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi -Iinclude/generated/uapi -include /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0xffffffff80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -fno-peephole2 -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -EL -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/mips-boards/generic/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta -I ../../../../mstar2/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/drivers/mstar2/hal/kris/cpu/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -Idrivers/mstar2/include -Idrivers/mstar2/drv/mbx -Idrivers/mstar2/hal/kris/mbx -DRED_LION    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mhal_mbx)"  -D"KBUILD_MODNAME=KBUILD_STR(mdrv_mbx)" -c -o drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.o drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.c

source_drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.o := drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.c

deps_drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.o := \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /opt/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/linkage.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/mp/debug/tool/changelist.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/types.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/posix_types.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/posix_types.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/sgidefs.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/asm-generic/posix_types.h \
  include/linux/bitops.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/kvm/guest.h) \
    $(wildcard include/config/mstar/mips.h) \
    $(wildcard include/config/dma/noncoherent.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/const.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cpu.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta/cpu-feature-overrides.h \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/cpu/mips64.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/arch_hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/asm-generic/errno-base.h \
  include/uapi/linux/kernel.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/sysinfo.h \
  drivers/mstar2/include/mdrv_mstypes.h \
    $(wildcard include/config/mp/aarch64/physical/define/64bit.h) \
    $(wildcard include/config/mp/android/physical/define/64bit.h) \
  drivers/mstar2/include/mdrv_types.h \
    $(wildcard include/config/mstar/msystem.h) \
  drivers/mstar2/drv/mbx/mdrv_mbx.h \
  drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.h \
  drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx_reg.h \
  drivers/mstar2/include/mdrv_types.h \

drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.o: $(deps_drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.o)

$(deps_drivers/mstar2/drv/mbx/../../hal/kris/mbx/mhal_mbx.o):
