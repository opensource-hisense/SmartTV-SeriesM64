cmd_drivers/mstar2/hal/kris/cpu/platform.o := mips-linux-gnu-gcc -Wp,-MD,drivers/mstar2/hal/kris/cpu/.platform.o.d  -nostdinc -isystem /opt/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include -Iarch/mips/include/generated  -Iinclude -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi -Iinclude/generated/uapi -include /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0xffffffff80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -fno-peephole2 -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -EL -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/mips-boards/generic/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta -I ../../../../mstar2/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/drivers/mstar2/hal/kris/cpu/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(platform)"  -D"KBUILD_MODNAME=KBUILD_STR(platform)" -c -o drivers/mstar2/hal/kris/cpu/platform.o drivers/mstar2/hal/kris/cpu/platform.c

source_drivers/mstar2/hal/kris/cpu/platform.o := drivers/mstar2/hal/kris/cpu/platform.c

deps_drivers/mstar2/hal/kris/cpu/platform.o := \
  include/linux/autoconf.h \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/scsi/dma.h) \
    $(wildcard include/config/kernel/gzip.h) \
    $(wildcard include/config/crc32.h) \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/sys/has/early/printk.h) \
    $(wildcard include/config/mstar/gflip.h) \
    $(wildcard include/config/scsi/logging.h) \
    $(wildcard include/config/network/filesystems.h) \
    $(wildcard include/config/module/force/unload.h) \
    $(wildcard include/config/ppp/sync/tty.h) \
    $(wildcard include/config/arch/suspend/possible.h) \
    $(wildcard include/config/mp/compiler/error.h) \
    $(wildcard include/config/ssb/possible.h) \
    $(wildcard include/config/mp/usb/mstar.h) \
    $(wildcard include/config/mtd/cmdline/parts.h) \
    $(wildcard include/config/mp/smp/startup.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/arch/flatmem/enable.h) \
    $(wildcard include/config/crypto/manager/disable/tests.h) \
    $(wildcard include/config/have/kernel/lzma.h) \
    $(wildcard include/config/mp/fusion/size/address/refine.h) \
    $(wildcard include/config/arch/want/ipc/parse/version.h) \
    $(wildcard include/config/generic/smp/idle/thread.h) \
    $(wildcard include/config/kdrv/mstar2.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/mp/cma/patch/cma/aggressive/alloc.h) \
    $(wildcard include/config/default/security/dac.h) \
    $(wildcard include/config/bql.h) \
    $(wildcard include/config/have/kernel/bzip2.h) \
    $(wildcard include/config/default/tcp/cong.h) \
    $(wildcard include/config/uevent/helper/path.h) \
    $(wildcard include/config/mp/android/mstar/rc/map/define.h) \
    $(wildcard include/config/android/binder/ipc.h) \
    $(wildcard include/config/arch/sparsemem/enable.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/default/message/loglevel.h) \
    $(wildcard include/config/connector.h) \
    $(wildcard include/config/crypto/rng2.h) \
    $(wildcard include/config/mstar/chip/name.h) \
    $(wildcard include/config/msdos/fs.h) \
    $(wildcard include/config/can.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/serial/8250.h) \
    $(wildcard include/config/lzo/decompress.h) \
    $(wildcard include/config/usb/ehci/root/hub/tt.h) \
    $(wildcard include/config/iommu/support.h) \
    $(wildcard include/config/usb.h) \
    $(wildcard include/config/ethernet.h) \
    $(wildcard include/config/sched/omit/frame/pointer.h) \
    $(wildcard include/config/branch/profile/none.h) \
    $(wildcard include/config/mips/mt/disabled.h) \
    $(wildcard include/config/have/dma/contiguous.h) \
    $(wildcard include/config/dql.h) \
    $(wildcard include/config/framebuffer/console.h) \
    $(wildcard include/config/coredump.h) \
    $(wildcard include/config/usb/serial/generic.h) \
    $(wildcard include/config/bcma/possible.h) \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/sync.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/fb/sys/fillrect.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/mtd/cfi/i2.h) \
    $(wildcard include/config/mstar/mips/mmap/128mb/128mb.h) \
    $(wildcard include/config/bounce.h) \
    $(wildcard include/config/mp/ion/patch/cache/flush/mod.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/mtd.h) \
    $(wildcard include/config/have/arch/jump/label.h) \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/nls/codepage/437.h) \
    $(wildcard include/config/mtd/nand/ids.h) \
    $(wildcard include/config/mtd/ubi/wl/threshold.h) \
    $(wildcard include/config/mp/nand/mtd.h) \
    $(wildcard include/config/fb/sys/imageblit.h) \
    $(wildcard include/config/binfmt/elf/comp.h) \
    $(wildcard include/config/mtd/nand/platform.h) \
    $(wildcard include/config/pss/info.h) \
    $(wildcard include/config/zlib/inflate.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/stacktrace/support.h) \
    $(wildcard include/config/emmc/partition.h) \
    $(wildcard include/config/lockd.h) \
    $(wildcard include/config/mp/platform/mips/ebase.h) \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/has/wakelock.h) \
    $(wildcard include/config/usb/storage.h) \
    $(wildcard include/config/net/vendor/broadcom.h) \
    $(wildcard include/config/standalone.h) \
    $(wildcard include/config/mips.h) \
    $(wildcard include/config/ashmem.h) \
    $(wildcard include/config/have/ide.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/init/env/arg/limit.h) \
    $(wildcard include/config/mp/cma/patch/migration/filter.h) \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/context/switch/tracer.h) \
    $(wildcard include/config/mp/nand/ubi.h) \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/mtd/redboot/parts.h) \
    $(wildcard include/config/ppp/deflate.h) \
    $(wildcard include/config/vt.h) \
    $(wildcard include/config/gmac/ethernet/albany.h) \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/cpu/r4k/fpu.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/nls.h) \
    $(wildcard include/config/mstar/mips/bd/generic.h) \
    $(wildcard include/config/pppoe.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/irq/work.h) \
    $(wildcard include/config/fb/sys/copyarea.h) \
    $(wildcard include/config/usb/common.h) \
    $(wildcard include/config/mp/ntfs3g/wrap.h) \
    $(wildcard include/config/mstar/mips.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/net/cadence.h) \
    $(wildcard include/config/mp/antutu/mstar/hide/sched/policy.h) \
    $(wildcard include/config/sys/supports/little/endian.h) \
    $(wildcard include/config/show/pc/lr/info.h) \
    $(wildcard include/config/nls/iso8859/1.h) \
    $(wildcard include/config/crypto/workqueue.h) \
    $(wildcard include/config/usb/ehci/hcd.h) \
    $(wildcard include/config/ppp/mppe.h) \
    $(wildcard include/config/can/calc/bittiming.h) \
    $(wildcard include/config/netdevices.h) \
    $(wildcard include/config/have/context/tracking.h) \
    $(wildcard include/config/eventfd.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/defconfig/list.h) \
    $(wildcard include/config/list.h) \
    $(wildcard include/config/serial/8250/console.h) \
    $(wildcard include/config/usb/announce/new/devices.h) \
    $(wildcard include/config/proc/page/monitor.h) \
    $(wildcard include/config/rcu/fanout/leaf.h) \
    $(wildcard include/config/net/vendor/seeq.h) \
    $(wildcard include/config/inet/xfrm/mode/beet.h) \
    $(wildcard include/config/mp/platform/lzma/bin/compressed.h) \
    $(wildcard include/config/mp/nand/bbt.h) \
    $(wildcard include/config/crypto/deflate.h) \
    $(wildcard include/config/have/dynamic/ftrace.h) \
    $(wildcard include/config/cevt/r4k.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/fat/fs.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/mtd/redboot/directory/block.h) \
    $(wildcard include/config/generic/clockevents.h) \
    $(wildcard include/config/mp/fat/debug/message/control.h) \
    $(wildcard include/config/dummy/console.h) \
    $(wildcard include/config/module/force/load.h) \
    $(wildcard include/config/nls/ascii.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/font/autoselect.h) \
    $(wildcard include/config/mp/platform/mips74k/timer.h) \
    $(wildcard include/config/hardware/watchpoints.h) \
    $(wildcard include/config/inet/xfrm/mode/transport.h) \
    $(wildcard include/config/elfcore.h) \
    $(wildcard include/config/have/kvm.h) \
    $(wildcard include/config/have/generic/hardirqs.h) \
    $(wildcard include/config/binfmt/elf.h) \
    $(wildcard include/config/scsi/proc/fs.h) \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/sched/tracer.h) \
    $(wildcard include/config/mp/ntfs/page/cache/readahead.h) \
    $(wildcard include/config/mstar/nand.h) \
    $(wildcard include/config/vd/memfree.h) \
    $(wildcard include/config/usb/serial.h) \
    $(wildcard include/config/mp/debug/tool/coredump/build/in/usb.h) \
    $(wildcard include/config/crc16.h) \
    $(wildcard include/config/generic/calibrate/delay.h) \
    $(wildcard include/config/hz/periodic.h) \
    $(wildcard include/config/broken/on/smp.h) \
    $(wildcard include/config/tmpfs.h) \
    $(wildcard include/config/anon/inodes.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/cpu/supports/32bit/kernel.h) \
    $(wildcard include/config/mstar/ir.h) \
    $(wildcard include/config/serial/core/console.h) \
    $(wildcard include/config/usb/hid.h) \
    $(wildcard include/config/sys/has/cpu/mips32/r2.h) \
    $(wildcard include/config/lpj/manual/setting.h) \
    $(wildcard include/config/mp/uart/serial/8250/riu/base.h) \
    $(wildcard include/config/ubifs/fs.h) \
    $(wildcard include/config/android.h) \
    $(wildcard include/config/mp/mips/highmem/memory/present/patch.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/crypto/pcomp2.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/hotplug.h) \
    $(wildcard include/config/have/debug/kmemleak.h) \
    $(wildcard include/config/mp/sparse/mem/enable/holes/in/zone/check.h) \
    $(wildcard include/config/mp/ntfs/volume/id.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/arch/hibernation/possible.h) \
    $(wildcard include/config/unix.h) \
    $(wildcard include/config/mp/platform/arch/general.h) \
    $(wildcard include/config/crypto/hash2.h) \
    $(wildcard include/config/default/hostname.h) \
    $(wildcard include/config/nfs/fs.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/crypto/algapi.h) \
    $(wildcard include/config/net/vendor/wiznet.h) \
    $(wildcard include/config/fonts.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/mtd/cfi/i1.h) \
    $(wildcard include/config/ubifs/fs/zlib.h) \
    $(wildcard include/config/nfs/common.h) \
    $(wildcard include/config/mp/nand/ubifs.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/arch/has/atomic64/dec/if/positive.h) \
    $(wildcard include/config/crypto/hash.h) \
    $(wildcard include/config/efi/partition.h) \
    $(wildcard include/config/trace/clock.h) \
    $(wildcard include/config/mstar/kernel/panic/do/wdt/reset.h) \
    $(wildcard include/config/log/buf/shift.h) \
    $(wildcard include/config/hz/1000.h) \
    $(wildcard include/config/net/vendor/8390.h) \
    $(wildcard include/config/virt/to/bus.h) \
    $(wildcard include/config/vfat/fs.h) \
    $(wildcard include/config/mstar/kris.h) \
    $(wildcard include/config/blk/dev/sr.h) \
    $(wildcard include/config/mp/cma/patch/cma/more/aggressive/alloc.h) \
    $(wildcard include/config/wakelock.h) \
    $(wildcard include/config/trad/signals.h) \
    $(wildcard include/config/print/module/addr.h) \
    $(wildcard include/config/suspend.h) \
    $(wildcard include/config/cross/memory/attach.h) \
    $(wildcard include/config/mtd/nand/ecc.h) \
    $(wildcard include/config/serial/8250/runtime/uarts.h) \
    $(wildcard include/config/fs/mbcache.h) \
    $(wildcard include/config/mstar/semutex.h) \
    $(wildcard include/config/generic/tracer.h) \
    $(wildcard include/config/have/function/tracer.h) \
    $(wildcard include/config/crypto/manager2.h) \
    $(wildcard include/config/serial/8250/deprecated/options.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/mtd/ubi.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/ppp/multilink.h) \
    $(wildcard include/config/mp/mstar/str/base/debug.h) \
    $(wildcard include/config/binfmt/script.h) \
    $(wildcard include/config/tick/cpu/accounting.h) \
    $(wildcard include/config/relay.h) \
    $(wildcard include/config/crypto/ecb.h) \
    $(wildcard include/config/debug/fs.h) \
    $(wildcard include/config/base/full.h) \
    $(wildcard include/config/zlib/deflate.h) \
    $(wildcard include/config/sunrpc.h) \
    $(wildcard include/config/mstar/mpool.h) \
    $(wildcard include/config/arch/discard/memblock.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/sw/sync/user.h) \
    $(wildcard include/config/generic/atomic64.h) \
    $(wildcard include/config/mii.h) \
    $(wildcard include/config/net/core.h) \
    $(wildcard include/config/uidgid/converted.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/crypto/sha1.h) \
    $(wildcard include/config/csrc/r4k.h) \
    $(wildcard include/config/mstar/gmac.h) \
    $(wildcard include/config/lockd/v4.h) \
    $(wildcard include/config/mtd/ubi/bitflips.h) \
    $(wildcard include/config/mp/ntfs/ioctl.h) \
    $(wildcard include/config/has/iomem.h) \
    $(wildcard include/config/ring/buffer/allow/swap.h) \
    $(wildcard include/config/pppopns.h) \
    $(wildcard include/config/gpio/devres.h) \
    $(wildcard include/config/generic/irq/probe.h) \
    $(wildcard include/config/sys/supports/zboot.h) \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/mtd/map/bank/width/1.h) \
    $(wildcard include/config/mstar/mbx.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/crypto/lzo.h) \
    $(wildcard include/config/partition/advanced.h) \
    $(wildcard include/config/mp/nand/bbt/size.h) \
    $(wildcard include/config/have/net/dsa.h) \
    $(wildcard include/config/cpu/supports/highmem.h) \
    $(wildcard include/config/net.h) \
    $(wildcard include/config/input/evdev.h) \
    $(wildcard include/config/ext2/fs.h) \
    $(wildcard include/config/arch/binfmt/elf/randomize/pie.h) \
    $(wildcard include/config/packet.h) \
    $(wildcard include/config/mali400/version/none.h) \
    $(wildcard include/config/nfs/v3.h) \
    $(wildcard include/config/nls/codepage/949.h) \
    $(wildcard include/config/nop/tracer.h) \
    $(wildcard include/config/inet.h) \
    $(wildcard include/config/prevent/firmware/build.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/rtc/lib.h) \
    $(wildcard include/config/have/kprobes.h) \
    $(wildcard include/config/crypto/aes.h) \
    $(wildcard include/config/net/vendor/smsc.h) \
    $(wildcard include/config/mp/platform/arm/64bit/poarting.h) \
    $(wildcard include/config/clone/backwards.h) \
    $(wildcard include/config/mstar/sc.h) \
    $(wildcard include/config/inet/xfrm/mode/tunnel.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/lockdep/support.h) \
    $(wildcard include/config/mp/ntfs/read/pages.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/usb/arch/has/ehci.h) \
    $(wildcard include/config/mtd/blkdevs.h) \
    $(wildcard include/config/sysctl/syscall.h) \
    $(wildcard include/config/mp/cma/patch/cma/mstar/driver/buffer.h) \
    $(wildcard include/config/need/dma/map/state.h) \
    $(wildcard include/config/android/paranoid/network.h) \
    $(wildcard include/config/mp/fat/volume/label.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/sparsemem/static.h) \
    $(wildcard include/config/mstar/iomap.h) \
    $(wildcard include/config/tty.h) \
    $(wildcard include/config/cpu/generic/dump/tlb.h) \
    $(wildcard include/config/have/kernel/gzip.h) \
    $(wildcard include/config/net/vendor/i825xx.h) \
    $(wildcard include/config/skip/checking/memory/while/fork.h) \
    $(wildcard include/config/need/per/cpu/km.h) \
    $(wildcard include/config/address/filter.h) \
    $(wildcard include/config/mp/mips/dma/dma/alloc/coherent/patch.h) \
    $(wildcard include/config/android/timed/output.h) \
    $(wildcard include/config/generic/io.h) \
    $(wildcard include/config/have/ftrace/mcount/record.h) \
    $(wildcard include/config/inet/tcp/diag.h) \
    $(wildcard include/config/hw/console.h) \
    $(wildcard include/config/devmem.h) \
    $(wildcard include/config/iosched/noop.h) \
    $(wildcard include/config/cpu/r4k/cache/tlb.h) \
    $(wildcard include/config/mstar/kernel/fault/do/wdt/reset.h) \
    $(wildcard include/config/localversion.h) \
    $(wildcard include/config/crypto.h) \
    $(wildcard include/config/mstar/fusion.h) \
    $(wildcard include/config/mstar/system.h) \
    $(wildcard include/config/default/mmap/min/addr.h) \
    $(wildcard include/config/mstar/chip/type.h) \
    $(wildcard include/config/fb/virtual.h) \
    $(wildcard include/config/sys/supports/big/endian.h) \
    $(wildcard include/config/have/dma/api/debug.h) \
    $(wildcard include/config/separate/printk/from/user.h) \
    $(wildcard include/config/usb/arch/has/hcd.h) \
    $(wildcard include/config/generic/irq/show.h) \
    $(wildcard include/config/mstar/ir/fantasy/mode.h) \
    $(wildcard include/config/panic/on/oops/value.h) \
    $(wildcard include/config/scsi/mod.h) \
    $(wildcard include/config/net/vendor/micrel.h) \
    $(wildcard include/config/mp/ntfs/volume/label.h) \
    $(wildcard include/config/infinite/oops/display.h) \
    $(wildcard include/config/oprofile.h) \
    $(wildcard include/config/serial/core.h) \
    $(wildcard include/config/buildtime/extable/sort.h) \
    $(wildcard include/config/embedded.h) \
    $(wildcard include/config/have/kretprobes.h) \
    $(wildcard include/config/ppp/filter.h) \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/mips/boards/gen.h) \
    $(wildcard include/config/scsi.h) \
    $(wildcard include/config/hid.h) \
    $(wildcard include/config/font/8x16.h) \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/squashfs/fragment/cache/size.h) \
    $(wildcard include/config/net/vendor/marvell.h) \
    $(wildcard include/config/ubifs/fs/advanced/compr.h) \
    $(wildcard include/config/misc/filesystems.h) \
    $(wildcard include/config/ftrace.h) \
    $(wildcard include/config/net/vendor/stmicro.h) \
    $(wildcard include/config/rcu/cpu/stall/timeout.h) \
    $(wildcard include/config/input/ff/memless.h) \
    $(wildcard include/config/profiling.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/crypto/arc4.h) \
    $(wildcard include/config/boot/elf32.h) \
    $(wildcard include/config/mstar/chip.h) \
    $(wildcard include/config/slhc.h) \
    $(wildcard include/config/crypto/manager.h) \
    $(wildcard include/config/ppp/bsdcomp.h) \
    $(wildcard include/config/mtd/nand.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/expert.h) \
    $(wildcard include/config/wireless.h) \
    $(wildcard include/config/wext/proc.h) \
    $(wildcard include/config/squashfs.h) \
    $(wildcard include/config/perf/use/vmalloc.h) \
    $(wildcard include/config/fat/default/iocharset.h) \
    $(wildcard include/config/dump/use/regset.h) \
    $(wildcard include/config/frame/warn.h) \
    $(wildcard include/config/hid/generic.h) \
    $(wildcard include/config/generic/hweight.h) \
    $(wildcard include/config/sys/supports/highmem.h) \
    $(wildcard include/config/mp/debug/tool/changelist.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/lzo/compress.h) \
    $(wildcard include/config/stacktrace.h) \
    $(wildcard include/config/pppolac.h) \
    $(wildcard include/config/has/ioport.h) \
    $(wildcard include/config/hz.h) \
    $(wildcard include/config/squashfs/zlib.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/serial/8250/nr/uarts.h) \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/mtd/ubi/beb/limit.h) \
    $(wildcard include/config/default/iosched.h) \
    $(wildcard include/config/mp/wifi/inc/default/dma/coherent/pool/size.h) \
    $(wildcard include/config/nlattr.h) \
    $(wildcard include/config/tcp/cong/cubic.h) \
    $(wildcard include/config/suspend/freezer.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/have/function/trace/mcount/test.h) \
    $(wildcard include/config/fb/sys/fops.h) \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/crc32/sliceby4.h) \
    $(wildcard include/config/mtd/complex/mappings.h) \
    $(wildcard include/config/fb.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/msdos/partition.h) \
    $(wildcard include/config/have/oprofile.h) \
    $(wildcard include/config/have/generic/dma/coherent.h) \
    $(wildcard include/config/arch/have/custom/gpio/h.h) \
    $(wildcard include/config/dump/range/based/on/register.h) \
    $(wildcard include/config/have/arch/kgdb.h) \
    $(wildcard include/config/sys/supports/multithreading.h) \
    $(wildcard include/config/zone/dma/flag.h) \
    $(wildcard include/config/net/vendor/intel.h) \
    $(wildcard include/config/mp/debug/tool/coredump.h) \
    $(wildcard include/config/mtd/map/bank/width/2.h) \
    $(wildcard include/config/mp/platform/mips.h) \
    $(wildcard include/config/ip/multicast.h) \
    $(wildcard include/config/generic/cmos/update.h) \
    $(wildcard include/config/default/security.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/tick/oneshot.h) \
    $(wildcard include/config/sw/sync.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/redlion/debug.h) \
    $(wildcard include/config/mtd/ubi/delay/init.h) \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/default/noop.h) \
    $(wildcard include/config/have/function/graph/tracer.h) \
    $(wildcard include/config/mips/l1/cache/shift.h) \
    $(wildcard include/config/base/small.h) \
    $(wildcard include/config/show/fault/trace/info.h) \
    $(wildcard include/config/crypto/blkcipher2.h) \
    $(wildcard include/config/devpts/multiple/instances.h) \
    $(wildcard include/config/nfs/v2.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/mtd/block.h) \
    $(wildcard include/config/wext/priv.h) \
    $(wildcard include/config/sys/supports/arbit/hz.h) \
    $(wildcard include/config/scsi/lowlevel.h) \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/vmallocused/plus.h) \
    $(wildcard include/config/ext2/fs/xattr.h) \
    $(wildcard include/config/mtd/oops.h) \
    $(wildcard include/config/debug/stackoverflow.h) \
    $(wildcard include/config/net/vendor/natsemi.h) \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/mali400/platform/none.h) \
    $(wildcard include/config/mtd/block2mtd.h) \
    $(wildcard include/config/have/c/recordmcount.h) \
    $(wildcard include/config/have/perf/events.h) \
    $(wildcard include/config/mp/binder/kernel/self/patch.h) \
    $(wildcard include/config/ppp/async.h) \
    $(wildcard include/config/can/gw.h) \
    $(wildcard include/config/cramfs.h) \
    $(wildcard include/config/lpj/value.h) \
    $(wildcard include/config/mp/debug/tool/coredump/usb/default.h) \
    $(wildcard include/config/mp/mips/l2/cache.h) \
    $(wildcard include/config/mstar/master0/only.h) \
    $(wildcard include/config/usb/ehci/tt/newsched.h) \
    $(wildcard include/config/fat/default/codepage.h) \
    $(wildcard include/config/tracing/support.h) \
    $(wildcard include/config/unix98/ptys.h) \
    $(wildcard include/config/printk/time.h) \
    $(wildcard include/config/ppp.h) \
    $(wildcard include/config/have/kernel/lzo.h) \
    $(wildcard include/config/ms/usb/miu1/patch.h) \
    $(wildcard include/config/inet/diag.h) \
    $(wildcard include/config/mstar/dump/reg/lr.h) \
    $(wildcard include/config/elf/core.h) \
    $(wildcard include/config/mp/mstar/str/base.h) \
    $(wildcard include/config/dma/noncoherent.h) \
    $(wildcard include/config/mp/nand/spansion.h) \
    $(wildcard include/config/usb/support.h) \
    $(wildcard include/config/staging.h) \
    $(wildcard include/config/inet/lro.h) \
    $(wildcard include/config/vt/console.h) \
    $(wildcard include/config/mp/cma/patch/lx/memory/align/to/8k/check.h) \
    $(wildcard include/config/posix/mqueue/sysctl.h) \
    $(wildcard include/config/mp/usb/str/patch.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/irq/cpu.h) \
    $(wildcard include/config/binary/printf.h) \
    $(wildcard include/config/generic/clockevents/build.h) \
    $(wildcard include/config/tracer/max/trace.h) \
    $(wildcard include/config/sys/supports/32bit/kernel.h) \
    $(wildcard include/config/sysvipc/sysctl.h) \
    $(wildcard include/config/cross/compile.h) \
    $(wildcard include/config/nls/utf8.h) \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/can/dev.h) \
    $(wildcard include/config/scsi/multi/lun.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/crc/ccitt.h) \
    $(wildcard include/config/blk/dev/sd.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/average.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/tracer/snapshot.h) \
    $(wildcard include/config/rcu/fanout.h) \
    $(wildcard include/config/bitreverse.h) \
    $(wildcard include/config/tracer/snapshot/per/cpu/swap.h) \
    $(wildcard include/config/crypto/blkcipher.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/mp/checkpt/boot.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/immediately/unplug.h) \
    $(wildcard include/config/mp/mips/highmem/cache/alias/patch.h) \
    $(wildcard include/config/mtd/map/bank/width/4.h) \
    $(wildcard include/config/cpu/little/endian.h) \
    $(wildcard include/config/net/activity/stats.h) \
    $(wildcard include/config/nls/default.h) \
    $(wildcard include/config/crypto/aead2.h) \
    $(wildcard include/config/crypto/algapi2.h) \
    $(wildcard include/config/ubifs/fs/lzo.h) \
    $(wildcard include/config/have/memblock.h) \
    $(wildcard include/config/input.h) \
    $(wildcard include/config/proc/sysctl.h) \
    $(wildcard include/config/mmu.h) \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/acpi.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/ioport.h \
    $(wildcard include/config/memory/hotremove.h) \
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
  include/linux/kobject.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/const.h \
  include/linux/sysfs.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/errno.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/asm-generic/errno-base.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/kobject_ns.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/atomic.h \
  include/linux/irqflags.h \
    $(wildcard include/config/kdebugd/ftrace.h) \
  include/linux/typecheck.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
  include/linux/stringify.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/bmips.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/xlr.h) \
    $(wildcard include/config/cpu/sb1.h) \
  include/mstar/mpatch_macro.h \
    $(wildcard include/config/mp/platform/arch/general/debug.h) \
    $(wildcard include/config/mp/platform/arm.h) \
    $(wildcard include/config/mp/platform/arm/debug.h) \
    $(wildcard include/config/mp/platform/mips/debug.h) \
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
    $(wildcard include/config/mp/platform/arm/64bit/poarting/debug.h) \
    $(wildcard include/config/mp/platform/mstar/legancy/intr.h) \
    $(wildcard include/config/mp/platform/mstar/legancy/intr/debug.h) \
    $(wildcard include/config/mp/platform/sw/patch/l2/sram/rma.h) \
    $(wildcard include/config/mp/platform/sw/patch/l2/sram/rma/debug.h) \
    $(wildcard include/config/mp/platform/mips74k/timer/debug.h) \
    $(wildcard include/config/mp/platform/arm/mstar/etm.h) \
    $(wildcard include/config/mp/platform/arm/mstar/etm/debug.h) \
    $(wildcard include/config/mp/platform/int/1/to/1/spi.h) \
    $(wildcard include/config/mp/platform/int/1/to/1/spi/debug.h) \
    $(wildcard include/config/mp/platform/arm/pmu.h) \
    $(wildcard include/config/mp/platform/arm/pmu/debug.h) \
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
    $(wildcard include/config/mp/nand/ubi/debug.h) \
    $(wildcard include/config/mp/nand/mtd/debug.h) \
    $(wildcard include/config/mp/nand/ubifs/debug.h) \
    $(wildcard include/config/mp/nand/spansion/debug.h) \
    $(wildcard include/config/mp/nand/bbt/debug.h) \
    $(wildcard include/config/mp/nand/bbt/size/debug.h) \
    $(wildcard include/config/mp/usb/mstar/debug.h) \
    $(wildcard include/config/mp/usb/str/patch/debug.h) \
    $(wildcard include/config/mp/checkpt/boot/debug.h) \
    $(wildcard include/config/mp/compiler/error/debug.h) \
    $(wildcard include/config/mp/android/dummy/mstar/rtc.h) \
    $(wildcard include/config/mp/android/dummy/mstar/rtc/debug.h) \
    $(wildcard include/config/mp/android/mstar/hardcode/lpj.h) \
    $(wildcard include/config/mp/android/mstar/hardcode/lpj/debug.h) \
    $(wildcard include/config/mp/android/mstar/rc/map/define/debug.h) \
    $(wildcard include/config/mp/android/physical/define/64bit.h) \
    $(wildcard include/config/mp/android/physical/define/64bit/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/debug.h) \
    $(wildcard include/config/mp/debug/tool/kdebug.h) \
    $(wildcard include/config/mp/debug/tool/kdebug/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/bootargs/only.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/bootargs/only/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/usb/default/debug.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/backup.h) \
    $(wildcard include/config/mp/debug/tool/coredump/path/backup/debug.h) \
    $(wildcard include/config/mp/debug/tool/oprofile.h) \
    $(wildcard include/config/mp/debug/tool/oprofile/debug.h) \
    $(wildcard include/config/mp/debug/tool/vm/used/size/check.h) \
    $(wildcard include/config/mp/debug/tool/vm/used/size/check/debug.h) \
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
    $(wildcard include/config/mp/mips/l2/cache/debug.h) \
    $(wildcard include/config/mp/mips/highmem/cache/alias/patch/debug.h) \
    $(wildcard include/config/mp/mips/highmem/memory/present/patch/debug.h) \
    $(wildcard include/config/mp/mips/dma/dma/alloc/coherent/patch/debug.h) \
    $(wildcard include/config/mp/smp/startup/debug.h) \
    $(wildcard include/config/mp/uart/serial/8250/riu/base/debug.h) \
    $(wildcard include/config/mp/ntfs3g/wrap/debug.h) \
    $(wildcard include/config/mp/ca7/quad/core/patch.h) \
    $(wildcard include/config/mp/ca7/quad/core/patch/debug.h) \
    $(wildcard include/config/mp/security/dm/verity.h) \
    $(wildcard include/config/mp/security/dm/verity/debug.h) \
    $(wildcard include/config/mp/sync/3/1/10/setting/vmalloc/setting.h) \
    $(wildcard include/config/mp/sync/3/1/10/setting/vmalloc/setting/debug.h) \
    $(wildcard include/config/mp/cma/patch/lx/memory/align/to/8k/check/debug.h) \
    $(wildcard include/config/mp/cma/patch/cma/mstar/driver/buffer/debug.h) \
    $(wildcard include/config/mp/cma/patch/cma/aggressive/alloc/debug.h) \
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
    $(wildcard include/config/mp/ion/patch/cache/flush/mod/debug.h) \
    $(wildcard include/config/mp/static/timer/clock/source.h) \
    $(wildcard include/config/mp/static/timer/clock/source/debug.h) \
    $(wildcard include/config/mp/sparse/mem/enable/holes/in/zone/check/debug.h) \
    $(wildcard include/config/mp/fat/volume/label/debug.h) \
    $(wildcard include/config/mp/fat/debug/message/control/debug.h) \
    $(wildcard include/config/mp/ntfs/ioctl/debug.h) \
    $(wildcard include/config/mp/ntfs/volume/label/debug.h) \
    $(wildcard include/config/mp/ntfs/volume/id/debug.h) \
    $(wildcard include/config/mp/ntfs/read/pages/debug.h) \
    $(wildcard include/config/mp/ntfs/2tb/plus/partition/support.h) \
    $(wildcard include/config/mp/ntfs/2tb/plus/partition/support/debug.h) \
    $(wildcard include/config/mp/ntfs/page/cache/readahead/debug.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/hotplug/debug.h) \
    $(wildcard include/config/mp/scsi/mstar/sd/card/immediately/unplug/debug.h) \
    $(wildcard include/config/mp/ethernet/mstar/icmp/enhance.h) \
    $(wildcard include/config/mp/ethernet/mstar/icmp/enhance/debug.h) \
    $(wildcard include/config/mp/wifi/inc/default/dma/coherent/pool/size/debug.h) \
    $(wildcard include/config/mp/antutu/mstar/hide/sched/policy/debug.h) \
    $(wildcard include/config/mp/aarch64/physical/define/64bit.h) \
    $(wildcard include/config/mp/aarch64/physical/define/64bit/debug.h) \
    $(wildcard include/config/mp/new/utopia/32bit.h) \
    $(wildcard include/config/mp/new/utopia/32bit/debug.h) \
    $(wildcard include/config/mp/trustzone/patch.h) \
    $(wildcard include/config/mp/trustzone/patch/debug.h) \
    $(wildcard include/config/mp/binder/kernel/self/patch/debug.h) \
    $(wildcard include/config/mp/cache/drop.h) \
    $(wildcard include/config/mp/cache/drop/debug.h) \
    $(wildcard include/config/mp/fusion/size/address/refine/debug.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/kvm/guest.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cpu.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cache.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta/cpu-feature-overrides.h \
    $(wildcard include/config/cpu/mips64.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/cmpxchg.h \
  include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/bug.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/break.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /opt/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/bitops.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
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
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/linux/string.h \
  include/uapi/linux/string.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  include/uapi/linux/kernel.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi/linux/sysinfo.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/processor.h \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
  include/linux/bitmap.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/cachectl.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/mips/huge/tlb/support.h) \
    $(wildcard include/config/cpu/micromips.h) \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/prefetch.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  include/linux/kref.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/mutex/spin/on/owner.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/wait.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/current.h \
  include/asm-generic/current.h \
  include/uapi/linux/wait.h \
  include/linux/klist.h \
  include/linux/pinctrl/devinfo.h \
  include/linux/pm.h \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/seqlock.h \
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
  include/uapi/asm-generic/param.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/timex.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/completion.h \
  include/linux/ratelimit.h \
  include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/device.h \
  include/linux/pm_wakeup.h \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/platform_device.h \
    $(wildcard include/config/hibernate/callbacks.h) \
  include/linux/mod_devicetable.h \
  include/linux/uuid.h \
  include/uapi/linux/uuid.h \
  drivers/mstar2/hal/kris/cpu/chip_int.h \

drivers/mstar2/hal/kris/cpu/platform.o: $(deps_drivers/mstar2/hal/kris/cpu/platform.o)

$(deps_drivers/mstar2/hal/kris/cpu/platform.o):
