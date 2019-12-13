/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to 1 if you are compiling for ARM. */
#define ARCH_ARM 1

/* Define to 1 if you are compiling for MIPS. */
/* #undef ARCH_MIPS */

/* Define to 1 if you are compiling for PowerPC. */
/* #undef ARCH_PPC */

/* Define to 1 if you are compiling for SH4. */
/* #undef ARCH_SH4 */

/* Define to 1 if you are compiling for ix86. */
/* #undef ARCH_X86 */

/* Define to 1 if you are compiling for AMD64. */
/* #undef ARCH_X86_64 */

/* Define to 1 if build for porject OBAMA. */
#define DFB_BUILD_FOR_PRJ_OBAMA 0

/* Dithering to use when rendering to RGB16 surfaces */
#define DFB_DITHER565 0

/* Advanced dithering, uses large dither table */
#define DFB_DITHER_ADVANCED 2

/* Simple dithering, uses small dither table */
#define DFB_DITHER_SIMPLE 1

/* Define to 1 if Video4Linux 2 is supported. */
/* #undef DFB_HAVE_V4L2 */

/* Only possible to 1 in case Obama & MstarIR on. */
#define DFB_HISENSE_MOUSE 0

/* Define to 0 if build for porject OBAMA. */
#define DFB_MSTAR_HOST_IR 1

/* Define to 1 if smooth scaling code should be built. */
#define DFB_SMOOTH_SCALING 0

/* The DirectFB version */
#define DIRECTFB_VERSION "1.4.2"

/* Default is 0, define 1 to enable it if necessary. */
#define FIXME_SC_2 0

/* Default is 0, define 1 to enable it if necessary. */
#define FIXME_SC_3 0

/* Define to 1 if you have the <asm/page.h> header file. */
/* #undef HAVE_ASM_PAGE_H */

/* Define to 1 if you have the declaration of `PTHREAD_MUTEX_RECURSIVE', and
   to 0 if you don't. */
#define HAVE_DECL_PTHREAD_MUTEX_RECURSIVE 1

/* Define to 1 if you have the declaration of
   `PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP', and to 0 if you don't. */
#define HAVE_DECL_PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if inb, outb and iopl are available. */
/* #undef HAVE_INB_OUTB_IOPL */

/* Define to 1 if struct input_absinfo is defined in linux/input.h. */
#define HAVE_INPUT_ABSINFO 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `c_r' library (-lc_r). */
/* #undef HAVE_LIBC_R */

/* Define to 1 if you have the `dl' library (-ldl). */
#define HAVE_LIBDL 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
#define HAVE_LIBPTHREAD 1

/* Define to 1 if you have the <linux/compiler.h> header file. */
/* #undef HAVE_LINUX_COMPILER_H */

/* Define to 1 if you have the <linux/unistd.h> header file. */
#define HAVE_LINUX_UNISTD_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/io.h> header file. */
/* #undef HAVE_SYSIO */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 0 if disable hw decode gif */
#define HW_SUPPORT_GIF_DECODER 0

/* Define to 0 if disable hw decode jpeg */
#define HW_SUPPORT_JPEG_DECODER 1

/* Define to 0 if disable hw decode png */
#define HW_SUPPORT_PNG_DECODER 1

/* Define to 1 if Linux Input driver should use FBDev system module for
   console keymap queries. */
/* #undef LINUX_INPUT_USE_FBDEV */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define to 0 if disable mstar special blit acceleration */
#define PATCH_MSTAR_GE_BLEND_OPT 1

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if arm assembly is available. */
/* #undef USE_ARMASM */

/* Define to 0 if disable cursor */
#define USE_CURSOR 1

/* Define to 0 if disable dynamic link */
#define USE_DYNAMIC_LINK 1

/* Define to 1 if GLX has been selected or detected */
/* #undef USE_GLX */

/* Define to 1 if compiling on KallistiOS. */
/* #undef USE_KOS */

/* Define to 1 if MMX assembly is available. */
/* #undef USE_MMX */

/* Define to 0 if disable mstar using cma */
#define USE_MSTAR_CMA 0

/* Define to 1 if ppc assembly is available. */
/* #undef USE_PPCASM */

/* Define to 0 if disable size optimization */
#define USE_SIZE_OPTIMIZATION 0

/* Define to 1 if SSE assembly is available. */
/* #undef USE_SSE */

/* Define to 1 to build with sysfs support. */
/* #undef USE_SYSFS */

/* Define to 1 to build with zlib compression. */
#define USE_ZLIB 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif
