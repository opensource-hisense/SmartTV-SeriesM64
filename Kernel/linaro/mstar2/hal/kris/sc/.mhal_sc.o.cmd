cmd_drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.o := mips-linux-gnu-gcc -Wp,-MD,drivers/mstar2/drv/sc/../../hal/kris/sc/.mhal_sc.o.d  -nostdinc -isystem /opt/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include -Iarch/mips/include/generated  -Iinclude -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/uapi -Iarch/mips/include/generated/uapi -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/uapi -Iinclude/generated/uapi -include /home/mstar/PERFORCE/THEALE/RedLion/3.10.23/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0xffffffff80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -fno-peephole2 -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -EL -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/mips-boards/generic/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-malta -I ../../../../mstar2/include/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/drivers/mstar2/hal/kris/cpu/ -I/home/mstar/PERFORCE/THEALE/RedLion/3.10.23/arch/mips/include/asm/mach-generic -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -Idrivers/mstar2/include -Idrivers/mstar2/drv/sc -Idrivers/mstar2/hal/kris/sc -Iinclude/linux -DRED_LION    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mhal_sc)"  -D"KBUILD_MODNAME=KBUILD_STR(mdrv_sc)" -c -o drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.o drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.c

source_drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.o := drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.c

deps_drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.o := \
  drivers/mstar2/drv/sc/../../hal/kris/sc/reg_sc.h \

drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.o: $(deps_drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.o)

$(deps_drivers/mstar2/drv/sc/../../hal/kris/sc/mhal_sc.o):
