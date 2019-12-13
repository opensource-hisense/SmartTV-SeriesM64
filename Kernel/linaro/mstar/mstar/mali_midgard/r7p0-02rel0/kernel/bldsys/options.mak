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

android_api_level := jbmr1 jbmr1.1 jbmr2 kitkat lollipop lollipopmr1 future
get_available_profiles := $(foreach f,$(wildcard $(PROFILES_DIR)/*.mak),$(basename $(notdir $(f))))

$(if $(profile),,$(error No profile specified. Valid profiles are: $(get_available_profiles)))

-include $(PROFILES_DIR)/$(profile).mak

########################################################################
# Setup options
########################################################################

# Include runtime debug code
$(call add_enum_option, debug, 0, 0 1)

# Build for a customer release
$(call add_enum_option, release, 0, 0 1)

# Include symbolic debugging information in binaries
$(call add_enum_option, symbols, 1, 0 1)

# Choose instrumentation level (0=none, 1=release, 2=debug)
$(call add_enum_option, instr, 0, 0 1 2)

# Enable forced CPU/GPU cache coherency for ACE enabled platforms (0=normal, 1=ACE, 2=ACE-lite)
$(call add_enum_option, hw_cc, 0, 0 1 2)

# Disable clang frontend (0=do not disable, 1=disable)
$(call add_enum_option, no_clang, 0, 0 1)

# Choose a software trace configuration file for timeline
$(call add_option, timeline, )

# Toggle dumping (0=off, 1=on)
$(call add_enum_option, dump, 0, 0 1)

# Include gator support (0=file, 1=annotate, 2=UDS). Allows Mali profiling via Streamline. Works with Linux/Android plus real hardware
$(call add_enum_option, gator, 0, 0 1 2)

# Enable Mali Instrumentation Packet Encapsulation (0=disabled, 1=enabled). When enabled timeline stream is encapsulated before being sent to the endpoint. Required for the reception of the base timeline stream
$(call add_enum_option, mipe, 0, 0 1)

# Enable KBase timeline stream (0=disabled, 1=enabled). When enabled timeline stream generated by kernel is read and retransmitted by cinstr. Requires Mali Instrumentation Packet Encapsulation to be enabled.
$(call add_enum_option, ktlstream, 0, 0 1)

# Enable HW Counters via UDS
$(call add_enum_option, hwc_stream, 0, 0 1)

# Enable streamline annotation support (0=no, 1=yes)
$(call add_enum_option, streamline_annotate, 0, 0 1)

# Build unit-tests
$(call add_enum_option, unit, 0, 0 1)

# Build benchmark-tests. Enables the user to control the number of used cores and the thread migration policy,
$(call add_enum_option, bench, 0, 0 1)

# Build internal performance tests
$(call add_enum_option, perf, 0, 0 1)

# Build doxygen documentation
$(call add_enum_option, doc, 0, 0 1)

# OBSOLETE - Build driver without cmem syncsets
$(call add_enum_option, no_syncsets, 0, 0 1)

# Build system-tests. You need to build the whole driver when specifying this option.
$(call add_enum_option, system, 0, 0 1)

# Choose architecture
$(call add_enum_option, arch, x86_32, x86_32 x86_64 arm_v7 arm_v8)

# Choose core to optimize for
$(call add_enum_option, core, cortex-a9, cortex-a9 cortex-a15)

# Choose target OS
$(call add_enum_option, os, linux, linux osx android)

# Choose target toolchain
$(call add_enum_option, toolchain, gnu, gnu rvct)

# Prefix to use for target toolchain. Default is "arm-linux-gnueabihf-" for arm_v7, "aarch64-linux-gnu-" for arm_v8 and none for target X86
$(call add_option, toolchain_prefix_target, )

# Prefix to use for host toolchain. Default is "arm-linux-gnueabihf-" for arm and none for target X86
$(call add_option, toolchain_prefix_host, )

# MSTAR patch begin
# Choose target windowing system
$(call add_enum_option, winsys, dummy, 0 fbdev dummy android x11 gbm wayland $(mstar_winsys))
# MSTAR patch end

# Choose a profile [pre-packaged set of default command line options]
$(call add_enum_option, profile, $(profile), $(get_available_profiles))

# List of targets, used by profile files.
$(call add_option, targets, )

# Select an OpenCL platform configuration
$(call add_option, opencl_platform_config, )

# DEPRECATED - Enable UTF trapping of assertion failure errors
$(call add_enum_option, ee, 0, 0 1)

# DEPRECATED - Choose hardware backend (0=dummy model, 1=real model, 2=real hw)
$(call add_enum_option, hw, 0, 0 1 2)

# DEPRECATED - Enable Dummy Model (0=normal, 1=dummy model)
$(call add_enum_option, dummy_model, 0, 0 1)

# kernel=kernel space, user=user space
$(call add_enum_option, backend, user, kernel user)

# 0=targeting mali hw (default), 1=targeting mali model
$(call add_enum_option, mali_model, 0, 0 1)

# Bypass the HW/model, use the dummy model instead (0=normal, 1=bypass mali, 2=backend default)
$(call add_enum_option, no_mali, 2, 0 1 2)

# Build for use on a Guest kernel (0 = kbase expects full control of the GPU (default), 1 = kbase is running in a guest OS and has no access to the MCU subsystem registers)
$(call add_enum_option, virt_guest, 0, 0 1)

# Toggle mcu backend (0=use register interface (default), 1=use mcu and VMIF/HVIF interface)
$(call add_enum_option, mcu, 0, 0 1)

# Enable firmware trace
$(call add_enum_option, fw_trace, 1, 0 1)

# Select register interface size (0=small, 1=large)
$(call add_enum_option, if_size, 0, 0 1)

# Enable bus log integration (0=none (default), 1=enabled)
$(call add_enum_option, buslog, 0, 0 1)

# Choose GPU product id
gpu_list := t60x t62x t76x t72x t88x t86x t83x t82x tMIx tVMx tFRx none
$(call add_enum_option, gpu, t60x, $(gpu_list))

# Enable use of the AARCH64 GPU MMU
$(call add_enum_option, aarch64_mmu, 0, 0 1)

# Choose GPU hardware revision
$(call add_enum_option, hwver, r0p0_eac, r0p0 r0p0_eac r0p1 r0p1_50rel0 r0p2 r0p3 r1p0 r1p1 r2p0 none)

# Enable Bullseye code coverage
$(call add_enum_option, coverage, 0, 0 1)

# Comma separated list of coverage regions used by Bullseye (if coverage=1) - defaults to all regions
$(call add_option, coverage_regions, )

# Enable debug dumping of jobs and framebuffers
$(call add_enum_option, cframe_qa, 0, 0 1 2)

# Enable build for OpenGL ES 1.1, 2.0, and 3.0
$(call add_enum_option, gles, 1, 0 1)

# Enable debug support for OpenGL ES 1.1 and 2.0
$(call add_enum_option, gles_qa, 0, 0 1)

# Enable experimental GLES features
$(call add_enum_option, gles_experimental, 0, 0 1)

# Load GLES inline shaders from source files
$(call add_enum_option, gles_shaders_from_files, 0, 0 1)

#Enable debug versions of internal shaders
$(call add_enum_option, gles_debug_shaders, 0, 0 1)

# Disable advanced optimizations (0=keep default options, 1=disable)
$(call add_enum_option, disable_advanced_opts, 0, 0 1)

# Enable deterministic performance. Used for driver instrumentation and is unsuitable for release builds. (0=disable, 1=enable)
$(call add_enum_option, deterministic_perf, 0, 0 1)

# Enable build for Vulkan
$(call add_enum_option, vulkan, 0, 0 1)

# Enable build for OpenCL
$(call add_enum_option, cl, 1, 0 1)

# Enable experimental OpenCL features
$(call add_enum_option, cl_experimental, 0, 0 1)

# Choose between 32-bit or 64-bit GPU pointers for OpenCL. Setting arch=arm_v8, will override user-supplied value and default to 64
$(call add_enum_option, cl_gpu_ptr_size, 64, 32 64)

# Disable OpenCL profiling command queues, regardless of application options
$(call add_enum_option, cl_disable_profiling, 0, 0 1)

# Disables clcc options for internal use only
$(call add_enum_option, clcc_public_release, 0, 0 1)

# Enable build for EGL
$(call add_enum_option, egl, 1, 0 1)

# Enable build for UMP
$(call add_enum_option, ump, 1, 0 1)

# Enable build of the ION import module for UMP
$(call add_enum_option, ump_ion, 0, 0 1)

# Enable build for base qa
$(call add_list_option, base_qa, , leak resfail use-after-free)

# Enable random delays to aid finding race conditions. The environmental variable MALI_OSU_DELAY_SEED should hold the seed.
$(call add_enum_option, random_delay, 0, 0 1)

# Enable build for GPL compliant driver
$(call add_enum_option, mali_license_is_gpl, 1, 0 1)

# Choose a platform configuration
$(call add_option, platform_config, gpu_default)

# Define at which level of Quality Assurance you want to build CMEM: (0) OFF  (1) Recommended for fault finding  (2) Maximum debugging
$(call add_enum_option, cmem_qa, 0, 0 1 2)

# Define at which level of Quality Assurance you want to build CMPBE: (0) OFF  (1) Recommended for fault finding  (2) Maximum debugging
$(call add_enum_option, cmpbe_qa, 0, 0 1 2)

# Enable ccache for the build.  Requires ccache installed.  Location of the cache is controlled by the CCACHE_DIR environment variable.  [default = $HOME/.ccache]
$(call add_enum_option, ccache, 0, 0 1)

# Set the llvm cache directory.  /arm/projectscratch/mpd/mpdti/prebuilt_llvm
$(call add_option, llvm_cache_dir, )

# Set the llvm cache mode, Off, ReadOnly or ReadWrite.
$(call add_enum_option, llvm_cache_mode, RW, OFF RO RW)

# Enable build with certain flavour.
$(call add_enum_option, flavour, fast, small fast)

# Enable build for thumb.
$(call add_enum_option, thumb, 1, 0 1)

# Override ARM floating-point ABI. Default is hard for Linux, softfp for Android.
$(call add_enum_option, floatabi, softfp, softfp hard)

# Enable optimization build
$(call add_enum_option, optimize, 0, 0 1)

# Enable use of a SIMD ISA (if supported)
$(call add_enum_option, simd, 0, 0 1)

# Enable verbose output
$(call add_enum_option, v, 0, 0 1)
Q = @
ifeq ($(v),1)
Q =
endif

# Include additional instrumentation needed by valgrind
$(call add_enum_option, valgrind, 0, 0 1)

# Build of model used
$(call add_enum_option, model, release, debug release)

# Model options
$(call add_enum_option, model_opts, none, none)

# Installation path for programs
$(call add_option, progs_install, )

# Installation path for libraries
$(call add_option, libs_install, )

# Enable error injection in the Midgar dummy model:0 off, 1 user driven, 2 random
$(call add_enum_option, error_inject, 0, 0 1 2)

# Enable building Android winsys with dma_buf support, instead of default UMP.
$(call add_enum_option, winsys_dma_buf, 0, 0 1)

# Enable KDS asynchronous page flips for X11
$(call add_enum_option, x11_async_pageflip, 0, 0 1)

# Version of Android to build for
$(call add_enum_option, android_version, jbmr1.1, $(android_api_level))

# Value for LOCAL_MODULE_OWNER in Android.mk
$(call add_option, android_module_owner, none)

# Override path for the Gralloc library headers. Will be added to LOCAL_C_INCLUDES in Android.mk
$(call add_option, android_gralloc_path, none)

# Enable building of the Gralloc library
$(call add_enum_option, gralloc, 1, 0 1)

# Use the reference winsys rather than the platform winsys.
$(call add_enum_option, winsys_platform_reference, 0, 0 1)

# Use the reference winsys with 32bit framebuffer config enabled
$(call add_enum_option, arndale_octa_reference_gralloc, 0, 0 1)

# Enable exporting the egl.cfg file for Android builds
$(call add_enum_option, android_egl_cfg, 1, 0 1)

# Enable building of Kernel modules
$(call add_enum_option, kernel_modules, 1, 0 1)

# Force global fragment job dependencies
$(call add_enum_option, force_fragment_order, 0, 0 1)

# Enable usage of cycle counter for profiling
$(call add_enum_option, cycle_counter, 0, 0 1)

# Enable the usage of the Multiple target Frame Buffer Descriptor
$(call add_enum_option, fbd, gpu_default, single multiple gpu_default)

# Force the TLS area to be large enough to contain data for tls_core_num cores
$(call add_enum_option, tls_core_num, 0, 0 1 2 4 6 8 16 32)

# Generate release version of Android.mk
$(call add_enum_option, android_release, 0, 0 1)

# Deprecated.
$(call add_enum_option, gles1, -1, -1 0 1)

# Deprecated.
$(call add_enum_option, gles2, -1, -1 0 1)

# big.Little migration
$(call add_enum_option, bl, none, none cs hmp)

# Enable use of AFBC (1 - native hw support, 2 - simulation)
$(call add_enum_option, afbc, 1, 0 1 2)

# Build with systrace support (1 - debug output, 2 - spammy output )
$(call add_enum_option, systrace, 0, 0 1 2)

# Enable user interaction in the DRM integration suite. This must not be used for automated tests.
$(call add_enum_option, drm_test_interactive, 1, 0 1)

# Sysroot to pick up headers/libs from
$(call add_option, sysroot, )

# Deprecated 
$(call add_enum_option, mmap64_defined, -1, -1 0 1)

# Log GPU events in the system trace buffer
$(call add_enum_option, system_trace, 0, 0 1)

# Use a flat tiler for T83x and T82x
$(call add_enum_option, flat_tiler, 1, 0 1)

pfsver2_gpus := t88x t86x t83x t82x tMIx tVMx tHOx tFRx
# Specify Pixel Format Specifier version to use (0 = use HW default, 1 = v1, 2 = v2). Requires gpu to be one of: $(pfsver2_gpus)
$(call add_enum_option, pfsver, 0, 0 1 2)

# Whether CINSTR should force identity mapping when dumping memory
$(call add_enum_option, force_identity_mapping, 1, 0 1)

# Define SONAME and library symbolic links with library versions. Valid values:
#      0: Don't define SONAME or create library links
#      1: Set SONAME and create library links. The libmali version is derived from the following:
#          1. The MALI_RELEASE_NAME environment variable if set.
#          2. The driver release version, if defined ($(MALI_RELEASE_NAME)).
#          3. The default value "0.9.9"
#      <major>.<minor>.<patch> (e.g. 4.0.2): Set SONAME and create library links. The values provided specify the libmali version
$(call add_option, soversion, 0)

# Path to install pkg-config .pc files. soversion must also be set.
$(call add_option, pkgconfig_install, )

# prefix to define in the generated .pc files. Only applicable when pkgconfig_install is set. Default: /usr/local
$(call add_option, pkgconfig_prefix, /usr/local)

# Build the Mali integration tests
$(call add_enum_option, tests, 1, 0 1)

# Specify kernel backend to use
$(call add_enum_option, kernel_backend, gpu, gpu dummy)

# Enable support for Wayland compositors
$(call add_enum_option, wayland_server, 0, 0 1)

valid_tpi_wayland_backends := fbdev drm
# Specify Wayland backend in use, to enable tests to correctly check the displayed output
$(call add_option, tpi_wayland_backend, 1, $(valid_tpi_wayland_backends))

# Use a DRM allocator for wayland
$(call add_enum_option, drm_allocator, 0, 0 1)

# Specify the allocator to use for wayland and/or GBM
$(call add_option, allocator, )

# Enable/disable index driven vertex shading
$(call add_enum_option, idvs, 1, 0 1)

# Selects version of Bifrost headers
$(call add_enum_option, gpu_arch_version, 0x060005, 0x060005)

# MSTAR patch begin
# MStar options
# The target MStar platform
$(call add_option, mstar_platform, )

# The project name
$(call add_option, project, )

# The maximum frequency of Mali
$(call add_option, mali_max_freq, 0)

# The minimum frequency of Mali
$(call add_option, mali_min_freq, 0)

# Mali IRQ number
$(call add_option, mali_irq, 0)

# The offset of Mali I/O memory region
$(call add_option, mali_reg_offset, 0)

# Use fixed device ID
$(call add_enum_option, fixed_device_id, 1, 0 1)

# Choose EGL extensions
$(call add_list_option, egl_extensions, , $(mstar_egl_extensions))

# Choose DirectFB version
$(call add_enum_option, directfb_version, 1.4.2, 1.4.2)

# Enable API trace
$(call add_enum_option, api_trace, 0, 0 1)
# MSTAR patch end

# Enable frame manager dumping for given frame ranges
ifeq ($(cframe_qa),2)
# Instrumentation is currently needed to get the frame numbers from EGL
ifeq ($(instr),0)
instr := 1
endif
endif

# Float ABI selection for ARM: If left on 'default' pick softfp for Android and hard for everything else
ifeq ($(floatabi),default)
floatabi := hard
ifeq ($(os),android)
floatabi := softfp
endif
endif

# Map product names into internal names (which are used in the codebase)
ifeq ($(gpu),t88x)
gpu := tFRx
endif

# the MCU prototype is currently a t62x r1p0 plus some extra hw
ifeq ($(gpu),tVMx)
gpu := t62x
hwver := r1p0
endif

# Hard-coded values for variables set by obsolete command-line options - these
# can be removed entirely once all build files no longer refer to them.
platform := dummy
