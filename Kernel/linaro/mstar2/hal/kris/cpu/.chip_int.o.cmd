cmd_drivers/mstar2/hal/kris/cpu/chip_int.o := mips-linux-gnu-gcc -Wp,-MD,drivers/mstar2/hal/kris/cpu/.chip_int.o.d  -nostdinc -isystem /opt/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include -Iarch/mips/include/generated  -Iinclude -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi -Iinclude/generated/uapi -include /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0xffffffff80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -fno-peephole2 -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -EL -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/mips-boards/generic/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta -I ../../../../mstar2/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/drivers/mstar2/hal/kris/cpu/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(chip_int)"  -D"KBUILD_MODNAME=KBUILD_STR(chip_int)" -c -o drivers/mstar2/hal/kris/cpu/chip_int.o drivers/mstar2/hal/kris/cpu/chip_int.c

source_drivers/mstar2/hal/kris/cpu/chip_int.o := drivers/mstar2/hal/kris/cpu/chip_int.c

deps_drivers/mstar2/hal/kris/cpu/chip_int.o := \
    $(wildcard include/config/arch/supports/oprofile.h) \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/cpu/mips64.h) \
    $(wildcard include/config/mips/mt/smp.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
    $(wildcard include/config/modules.h) \
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
  include/linux/types.h \
    $(wildcard include/config/mp/debug/tool/changelist.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
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
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/posix_types.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/sgidefs.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/asm-generic/posix_types.h \
  include/linux/irq.h \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/hardirqs/sw/resend.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
    $(wildcard include/config/debug/preempt.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/asm-generic/errno-base.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/const.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /opt/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/kvm/guest.h) \
    $(wildcard include/config/mstar/mips.h) \
    $(wildcard include/config/dma/noncoherent.h) \
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
    $(wildcard include/config/mips/mt/smtc.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta/cpu-feature-overrides.h \
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
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  include/uapi/linux/kernel.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/sysinfo.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/break.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/kdebugd/ftrace.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/irq/cpu.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/bmips.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/xlr.h) \
    $(wildcard include/config/cpu/sb1.h) \
  include/mstar/mpatch_macro.h \
    $(wildcard include/config/mp/platform/arch/general.h) \
    $(wildcard include/config/mp/platform/arch/general/debug.h) \
    $(wildcard include/config/mp/platform/arm.h) \
    $(wildcard include/config/mp/platform/arm/debug.h) \
    $(wildcard include/config/mp/platform/mips.h) \
    $(wildcard include/config/mp/platform/mips/debug.h) \
    $(wildcard include/config/mp/platform/mips/ebase.h) \
    $(wildcard include/config/mp/platform/mips/ebase/debug.h) \
    $(wildcard include/config/mp/platform/pm.h) \
    $(wildcard include/config/mp/platform/pm/debug.h) \
    $(wildcard include/config/mp/platform/arm/errata/775420.h) \
    $(wildcard include/config/mp/platform/arm/errata/775420/debug.h) \
    $(wildcard include/config/mp/platform/arm/errata/835769.h) \
    $(wildcard include/config/mp/platform/arm/errata/835769/debug.h) \
    $(wildcard include/config/mp/platform/arm/errata/834926.h) \
    $(wildcard include/config/mp/platform/arm/errata/834926/debug.h) \
    $(wildcard include/config/mp/platform/arm/errata/834870.h) \
    $(wildcard include/config/mp/platform/arm/errata/834870/debug.h) \
    $(wildcard include/config/mp/platform/arm/errata/832072.h) \
    $(wildcard include/config/mp/platform/arm/errata/832072/debug.h) \
    $(wildcard include/config/mp/platform/arm/errata/821420.h) \
    $(wildcard include/config/mp/platform/arm/errata/821420/debug.h) \
    $(wildcard include/config/mp/platform/arm/64bit/poarting.h) \
    $(wildcard include/config/mp/platform/arm/64bit/poarting/debug.h) \
    $(wildcard include/config/mp/platform/mstar/legancy/intr.h) \
    $(wildcard include/config/mp/platform/mstar/legancy/intr/debug.h) \
    $(wildcard include/config/mp/platform/sw/patch/l2/sram/rma.h) \
    $(wildcard include/config/mp/platform/sw/patch/l2/sram/rma/debug.h) \
    $(wildcard include/config/mp/platform/mips74k/timer.h) \
    $(wildcard include/config/mp/platform/mips74k/timer/debug.h) \
    $(wildcard include/config/mp/platform/arm/mstar/etm.h) \
    $(wildcard include/config/mp/platform/arm/mstar/etm/debug.h) \
    $(wildcard include/config/mp/platform/int/1/to/1/spi.h) \
    $(wildcard include/config/mp/platform/int/1/to/1/spi/debug.h) \
    $(wildcard include/config/mp/platform/arm/pmu.h) \
    $(wildcard include/config/mp/platform/arm/pmu/debug.h) \
    $(wildcard include/config/mp/platform/lzma/bin/compressed.h) \
    $(wildcard include/config/mp/platform/lzma/bin/compressed/debug.h) \
    $(wildcard include/config/mp/platform/cpu/setting.h) \
    $(wildcard include/config/mp/platform/cpu/setting/debug.h) \
    $(wildcard include/config/mp/platform/t/sensor/observation.h) \
    $(wildcard include/config/mp/platform/t/sensor/observation/debug.h) \
    $(wildcard include/config/mp/platform/fixme.h) \
    $(wildcard include/config/mp/platform/fixme/debug.h) \
    $(wildcard include/config/mp/platform/fiq/irq/hyp.h) \
    $(wildcard include/config/mp/platform/fiq/irq/hyp/debug.h) \
    $(wildcard include/config/mp/platform/utopia2/interrupt.h) \
    $(wildcard include/config/mp/platform/utopia2/interrupt/debug.h) \
    $(wildcard include/config/mp/platform/utopia2k/export/symbol.h) \
    $(wildcard include/config/mp/platform/utopia2k/export/symbol/debug.h) \
    $(wildcard include/config/mp/platform/arm/memory/hotplug.h) \
    $(wildcard include/config/mp/platform/arm/memory/hotplug/debug.h) \
    $(wildcard include/config/mp/nand/ubi.h) \
    $(wildcard include/config/mp/nand/ubi/debug.h) \
    $(wildcard include/config/mp/nand/mtd.h) \
    $(wildcard include/config/mp/nand/mtd/debug.h) \
    $(wildcard include/config/mp/nand/ubifs.h) \
    $(wildcard include/config/mp/nand/ubifs/debug.h) \
    $(wildcard include/config/mp/nand/spansion.h) \
    $(wildcard include/config/mp/nand/spansion/debug.h) \
    $(wildcard include/config/mp/nand/bbt.h) \
    $(wildcard include/config/mp/nand/bbt/debug.h) \
    $(wildcard include/config/mp/nand/bbt/size.h) \
    $(wildcard include/config/mp/nand/bbt/size/debug.h) \
    $(wildcard include/config/mp/usb/mstar.h) \
    $(wildcard include/config/mp/usb/mstar/debug.h) \
    $(wildcard include/config/mp/usb/str/patch.h) \
    $(wildcard include/config/mp/usb/str/patch/debug.h) \
    $(wildcard include/config/mp/checkpt/boot.h) \
    $(wildcard include/config/mp/checkpt/boot/debug.h) \
    $(wildcard include/config/mp/compiler/error.h) \
    $(wildcard include/config/mp/compiler/error/debug.h) \
    $(wildcard include/config/mp/android/dummy/mstar/rtc.h) \
    $(wildcard include/config/mp/android/dummy/mstar/rtc/debug.h) \
    $(wildcard include/config/mp/android/mstar/hardcode/lpj.h) \
    $(wildcard include/config/mp/android/mstar/hardcode/lpj/debug.h) \
    $(wildcard include/config/mp/android/mstar/rc/map/define.h) \
    $(wildcard include/config/mp/android/mstar/rc/map/define/debug.h) \
    $(wildcard include/config/mp/android/physical/define/64bit.h) \
    $(wildcard include/config/mp/android/physical/define/64bit/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump.h) \
    $(wildcard include/config/mp/debug/tool/coredump/debug.h) \
    $(wildcard include/config/mp/debug/tool/kdebug.h) \
    $(wildcard include/config/mp/debug/tool/kdebug/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/bootargs/only.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/bootargs/only/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/usb/default.h) \
    $(wildcard include/config/mp/debug/tool/coredump/usb/default/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/backup.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/backup/debug.h) \
    $(wildcard include/config/mp/debug/tool/oprofile.h) \
    $(wildcard include/config/mp/debug/tool/oprofile/debug.h) \
    $(wildcard include/config/mp/debug/tool/vm/used/size/check.h) \
    $(wildcard include/config/mp/debug/tool/vm/used/size/check/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/build/in/usb.h) \
    $(wildcard include/config/mp/debug/tool/coredump/build/in/usb/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/without/compress.h) \
    $(wildcard include/config/mp/debug/tool/coredump/without/compress/debug.h) \
    $(wildcard include/config/mp/debug/tool/changelist/debug.h) \
    $(wildcard include/config/mp/debug/tool/module/check.h) \
    $(wildcard include/config/mp/debug/tool/module/check/debug.h) \
    $(wildcard include/config/mp/debug/tool/memory/usage/monitor.h) \
    $(wildcard include/config/mp/debug/tool/memory/usage/monitor/debug.h) \
    $(wildcard include/config/mp/debug/tool/ramlog.h) \
    $(wildcard include/config/mp/debug/tool/ramlog/debug.h) \
    $(wildcard include/config/mp/mips/l2/cache.h) \
    $(wildcard include/config/mp/mips/l2/cache/debug.h) \
    $(wildcard include/config/mp/mips/highmem/cache/alias/patch.h) \
    $(wildcard include/config/mp/mips/highmem/cache/alias/patch/debug.h) \
    $(wildcard include/config/mp/mips/highmem/memory/present/patch.h) \
    $(wildcard include/config/mp/mips/highmem/memory/present/patch/debug.h) \
    $(wildcard include/config/mp/mips/dma/dma/alloc/coherent/patch.h) \
    $(wildcard include/config/mp/mips/dma/dma/alloc/coherent/patch/debug.h) \
    $(wildcard include/config/mp/smp/startup.h) \
    $(wildcard include/config/mp/smp/startup/debug.h) \
    $(wildcard include/config/mp/uart/serial/8250/riu/base.h) \
    $(wildcard include/config/mp/uart/serial/8250/riu/base/debug.h) \
    $(wildcard include/config/mp/ntfs3g/wrap.h) \
    $(wildcard include/config/mp/ntfs3g/wrap/debug.h) \
    $(wildcard include/config/mp/mstar/str/base.h) \
    $(wildcard include/config/mp/mstar/str/base/debug.h) \
    $(wildcard include/config/mp/ca7/quad/core/patch.h) \
    $(wildcard include/config/mp/ca7/quad/core/patch/debug.h) \
    $(wildcard include/config/mp/security/dm/verity.h) \
    $(wildcard include/config/mp/security/dm/verity/debug.h) \
    $(wildcard include/config/mp/sync/3/1/10/setting/vmalloc/setting.h) \
    $(wildcard include/config/mp/sync/3/1/10/setting/vmalloc/setting/debug.h) \
    $(wildcard include/config/mp/cma/patch/lx/memory/align/to/8k/check.h) \
    $(wildcard include/config/mp/cma/patch/lx/memory/align/to/8k/check/debug.h) \
    $(wildcard include/config/mp/cma/patch/cma/mstar/driver/buffer.h) \
    $(wildcard include/config/mp/cma/patch/cma/mstar/driver/buffer/debug.h) \
    $(wildcard include/config/mp/cma/patch/cma/aggressive/alloc.h) \
    $(wildcard include/config/mp/cma/patch/cma/aggressive/alloc/debug.h) \
    $(wildcard include/config/mp/cma/patch/cma/more/aggressive/alloc.h) \
    $(wildcard include/config/mp/cma/patch/cma/more/aggressive/alloc/debug.h) \
    $(wildcard include/config/mp/cma/patch/cma/uncached/direct/mapping.h) \
    $(wildcard include/config/mp/cma/patch/cma/uncached/direct/mapping/debug.h) \
    $(wildcard include/config/mp/cma/patch/cma/default/buffer/limitted/to/lx0.h) \
    $(wildcard include/config/mp/cma/patch/cma/default/buffer/limitted/to/lx0/debug.h) \
    $(wildcard include/config/mp/cma/patch/force/migration.h) \
    $(wildcard include/config/mp/cma/patch/force/migration/debug.h) \
    $(wildcard include/config/mp/cma/patch/mstar/ion/mtlb.h) \
    $(wildcard include/config/mp/cma/patch/mstar/ion/mtlb/debug.h) \
    $(wildcard include/config/mp/cma/patch/delay/free.h) \
    $(wildcard include/config/mp/cma/patch/delay/free/debug.h) \
    $(wildcard include/config/mp/cma/patch/migration/filter.h) \
    $(wildcard include/config/mp/cma/patch/migration/filter/debug.h) \
    $(wildcard include/config/mp/cma/patch/oom/killer.h) \
    $(wildcard include/config/mp/cma/patch/oom/killer/debug.h) \
    $(wildcard include/config/mp/cma/patch/smaller/socket/buffer.h) \
    $(wildcard include/config/mp/cma/patch/smaller/socket/buffer/debug.h) \
    $(wildcard include/config/mp/cma/patch/do/fork/page/pool.h) \
    $(wildcard include/config/mp/cma/patch/do/fork/page/pool/debug.h) \
    $(wildcard include/config/mp/cma/patch/smaller/slab/page.h) \
    $(wildcard include/config/mp/cma/patch/smaller/slab/page/debug.h) \
    $(wildcard include/config/mp/cma/patch/debug/static/miu/protect.h) \
    $(wildcard include/config/mp/cma/patch/debug/static/miu/protect/debug.h) \
    $(wildcard include/config/mp/global/timer/12mhz/patch.h) \
    $(wildcard include/config/mp/global/timer/12mhz/patch/debug.h) \
    $(wildcard include/config/mp/ion/patch/mstar.h) \
    $(wildcard include/config/mp/ion/patch/mstar/debug.h) \
    $(wildcard include/config/mp/ion/patch/mstar/customer/ioctl.h) \
    $(wildcard include/config/mp/ion/patch/mstar/customer/ioctl/debug.h) \
    $(wildcard include/config/mp/ion/patch/cache/flush/mod.h) \
    $(wildcard include/config/mp/ion/patch/cache/flush/mod/debug.h) \
    $(wildcard include/config/mp/static/timer/clock/source.h) \
    $(wildcard include/config/mp/static/timer/clock/source/debug.h) \
    $(wildcard include/config/mp/sparse/mem/enable/holes/in/zone/check.h) \
    $(wildcard include/config/mp/sparse/mem/enable/holes/in/zone/check/debug.h) \
    $(wildcard include/config/mp/fat/volume/label.h) \
    $(wildcard include/config/mp/fat/volume/label/debug.h) \
    $(wildcard include/config/mp/fat/debug/message/control.h) \
    $(wildcard include/config/mp/fat/debug/message/control/debug.h) \
    $(wildcard include/config/mp/ntfs/ioctl.h) \
    $(wildcard include/config/mp/ntfs/ioctl/debug.h) \
    $(wildcard include/config/mp/ntfs/volume/label.h) \
    $(wildcard include/config/mp/ntfs/volume/label/debug.h) \
    $(wildcard include/config/mp/ntfs/volume/id.h) \
    $(wildcard include/config/mp/ntfs/volume/id/debug.h) \
    $(wildcard include/config/mp/ntfs/read/pages.h) \
    $(wildcard include/config/mp/ntfs/read/pages/debug.h) \
    $(wildcard include/config/mp/ntfs/2tb/plus/partition/support.h) \
    $(wildcard include/config/mp/ntfs/2tb/plus/partition/support/debug.h) \
    $(wildcard include/config/mp/ntfs/page/cache/readahead.h) \
    $(wildcard include/config/mp/ntfs/page/cache/readahead/debug.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/hotplug.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/hotplug/debug.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/immediately/unplug.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/immediately/unplug/debug.h) \
    $(wildcard include/config/mp/ethernet/mstar/icmp/enhance.h) \
    $(wildcard include/config/mp/ethernet/mstar/icmp/enhance/debug.h) \
    $(wildcard include/config/mp/wifi/inc/default/dma/coherent/pool/size.h) \
    $(wildcard include/config/mp/wifi/inc/default/dma/coherent/pool/size/debug.h) \
    $(wildcard include/config/mp/antutu/mstar/hide/sched/policy.h) \
    $(wildcard include/config/mp/antutu/mstar/hide/sched/policy/debug.h) \
    $(wildcard include/config/mp/aarch64/physical/define/64bit.h) \
    $(wildcard include/config/mp/aarch64/physical/define/64bit/debug.h) \
    $(wildcard include/config/mp/new/utopia/32bit.h) \
    $(wildcard include/config/mp/new/utopia/32bit/debug.h) \
    $(wildcard include/config/mp/trustzone/patch.h) \
    $(wildcard include/config/mp/trustzone/patch/debug.h) \
    $(wildcard include/config/mp/binder/kernel/self/patch.h) \
    $(wildcard include/config/mp/binder/kernel/self/patch/debug.h) \
    $(wildcard include/config/mp/cache/drop.h) \
    $(wildcard include/config/mp/cache/drop/debug.h) \
    $(wildcard include/config/mp/fusion/size/address/refine.h) \
    $(wildcard include/config/mp/fusion/size/address/refine/debug.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/processor.h \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/cachectl.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/mips/huge/tlb/support.h) \
    $(wildcard include/config/cpu/micromips.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/prefetch.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/atomic.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  include/linux/gfp.h \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/cma.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/current.h \
  include/asm-generic/current.h \
  include/uapi/linux/wait.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/seqlock.h \
  include/linux/nodemask.h \
    $(wildcard include/config/movable/node.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/page-flags-layout.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/generated/bounds.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/sparsemem.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/page.h \
  include/linux/pfn.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/io.h \
    $(wildcard include/config/pci.h) \
  include/asm-generic/iomap.h \
    $(wildcard include/config/has/ioport.h) \
    $(wildcard include/config/generic/iomap.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/ioremap.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/mutex/spin/on/owner.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
  include/linux/completion.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutree.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/math64.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/uapi/linux/time.h \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/param.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/timex.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/topology.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/irqreturn.h \
  include/linux/irqnr.h \
  include/uapi/linux/irqnr.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/irq.h \
    $(wildcard include/config/i8259.h) \
    $(wildcard include/config/mips/mt/smtc/irqaff.h) \
    $(wildcard include/config/mips/mt/smtc/im/backstop.h) \
  include/linux/irqdomain.h \
    $(wildcard include/config/irq/domain.h) \
    $(wildcard include/config/of/irq.h) \
  include/linux/radix-tree.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mipsmtregs.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta/irq.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/irq.h \
    $(wildcard include/config/irq/cpu/rm7k.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/ptrace.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/isadep.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/ptrace.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/irq_regs.h \
  include/linux/irqdesc.h \
    $(wildcard include/config/irq/preflow/fasteoi.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/sparse/irq.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/hw_irq.h \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/show/fault/trace/info.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/mips.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/uprobes.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/mm/owner.h) \
  include/uapi/linux/sched.h \
  include/linux/capability.h \
  include/uapi/linux/capability.h \
  include/linux/rbtree.h \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/transparent/hugepage.h) \
  include/linux/auxvec.h \
  include/uapi/linux/auxvec.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/auxvec.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  include/linux/uprobes.h \
    $(wildcard include/config/arch/supports/uprobes.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mmu.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cputime.h \
  include/asm-generic/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime_jiffies.h \
  include/linux/sem.h \
  include/uapi/linux/sem.h \
  include/linux/ipc.h \
  include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/uapi/linux/ipc.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/ipcbuf.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/asm-generic/ipcbuf.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/sembuf.h \
  include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  include/uapi/linux/signal.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/signal.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/asm-generic/signal-defs.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/sigcontext.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/sigcontext.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/siginfo.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/uapi/asm-generic/siginfo.h \
  include/linux/pid.h \
  include/linux/proportions.h \
  include/linux/percpu_counter.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/seccomp/filter.h) \
  include/uapi/linux/seccomp.h \
  include/linux/rculist.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/sysctl.h \
  include/uapi/linux/sysctl.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/memcg/kmem.h) \
    $(wildcard include/config/slub/debug.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/hardirq.h \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  include/linux/vtime.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/hardirq.h \
  include/asm-generic/hardirq.h \
  include/linux/irq_cpustat.h \
  include/linux/kernel_stat.h \
  include/linux/random.h \
    $(wildcard include/config/arch/random.h) \
  include/uapi/linux/random.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/ioctl.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  include/generated/uapi/linux/version.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mips-boards/malta.h \
    $(wildcard include/config/reg.h) \
    $(wildcard include/config/devnum.h) \
    $(wildcard include/config/activate.h) \
    $(wildcard include/config/enter.h) \
    $(wildcard include/config/exit.h) \
    $(wildcard include/config/devnum/floppy.h) \
    $(wildcard include/config/activate/enable.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mips-boards/msc01_pci.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/gt64120.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta/mach-gt64120.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mips-boards/maltaint.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mips-boards/piix4.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mips-boards/generic.h \
    $(wildcard include/config/kgdb.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mips-boards/bonito64.h \
    $(wildcard include/config/dis.h) \
  drivers/mstar2/hal/kris/cpu/chip_int.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/setup.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/setup.h \

drivers/mstar2/hal/kris/cpu/chip_int.o: $(deps_drivers/mstar2/hal/kris/cpu/chip_int.o)

$(deps_drivers/mstar2/hal/kris/cpu/chip_int.o):
