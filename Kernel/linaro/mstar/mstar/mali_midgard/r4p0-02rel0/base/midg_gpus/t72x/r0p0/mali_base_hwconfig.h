/*
 * Copyright:
 * ----------------------------------------------------------------------------
 * This confidential and proprietary software may be used only as authorized
 * by a licensing agreement from ARM Limited.
 *      (C) COPYRIGHT 2013-2014 ARM Limited, ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorized copies and
 * copies may only be made to the extent permitted by a licensing agreement
 * from ARM Limited.
 * ----------------------------------------------------------------------------
 */

#ifndef _BASE_HWCONFIG_H_
#define _BASE_HWCONFIG_H_

/* ----------------------------------------------------------------------------
   OBSOLETE ISSUES - WORKAROUNDS STILL EXIST IN CODEBASE, BUT NOT NEEDED
---------------------------------------------------------------------------- */
/* None */

/* ----------------------------------------------------------------------------
   ACTIVE ISSUES
---------------------------------------------------------------------------- */
/* The current version of the model doesn't support Soft-Stop */
#define BASE_HW_ISSUE_5736  0

/* Need way to guarantee that all previously-translated memory accesses are commited */
#define BASE_HW_ISSUE_6367  0

/* Result swizzling doesn't work for GRDESC/GRDESC_DER */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_6398  0

/* Unaligned load stores crossing 128 bit boundaries will fail */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_6402  1

/* On job complete with non-done the cache is not flushed */
#define BASE_HW_ISSUE_6787  0

/* WLS allocation does not respect the Instances field in the Thread Storage Descriptor */
#define BASE_HW_ISSUE_7027  0

/* The clamp integer coordinate flag bit of the sampler descriptor is reserved */
#define BASE_HW_ISSUE_7144  0

/* TEX_INDEX LOD is always use converted */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_8073  0

/* Write of PRFCNT_CONFIG_MODE_MANUAL to PRFCNT_CONFIG causes a instr. dump if PRFCNT_TILER_EN is enabled */
#define BASE_HW_ISSUE_8186  0

/* Do not set .skip flag on the GRDESC, GRDESC_DER, DELTA, MOV, and NOP texturing instructions */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_8215  0

/* TIB: Reports faults from a vtile which has not yet been allocated */
#define BASE_HW_ISSUE_8245  0

/* WLMA memory goes wrong when run on shader cores other than core 0. */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_8250  0

/** Hierz doesn't work when stenciling is enabled */
#define BASE_HW_ISSUE_8260  0

/* Livelock in L0 icache */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_8280  0

/** uTLB deadlock could occur when writing to an invalid page at the same time as access to a valid page in the same uTLB cache line */
#define BASE_HW_ISSUE_8316  0

/* HT: TERMINATE for RUN command ignored if previous LOAD_DESCRIPTOR is still executing */
#define BASE_HW_ISSUE_8394  0

/* CSE : Sends a TERMINATED response for a task that should not be terminated */
/* (Note that PRLAM-8379 also uses this workaround) */
#define BASE_HW_ISSUE_8401  0

/* Repeatedly Soft-stopping a job chain consisting of (Vertex Shader, Cache Flush, Tiler) jobs causes 0x58 error on tiler job. */
#define BASE_HW_ISSUE_8408  0

/* Compute job hangs: disable the Pause buffer in the LS pipe.  */
/* BASE_HW_ISSUE_8443 implemented at dynamically for r0p0-00dev15 */

/* Stencil test enable 1->0 sticks */
#define BASE_HW_ISSUE_8456  0

/* Tiler heap issue using FBOs or multiple processes using the tiler simultaneously */
/* (Note that PRLAM-9049 also uses this work-around) */
#define BASE_HW_ISSUE_8564  0

/* Livelock issue using atomic instructions (particularly when using atomic_cmpxchg as a spinlock) */
#define BASE_HW_ISSUE_8791  0

/* Fused jobs are not supported (for various reasons) */
/* Jobs with relaxed dependencies do not support soft-stop */
/* (Note that PRLAM-8803, PRLAM-8393, PRLAM-8559, PRLAM-8601 & PRLAM-8607 all use this work-around) */
#define BASE_HW_ISSUE_8803  1

/* Blend shader output is wrong for certain formats */
#define BASE_HW_ISSUE_8833  0

/* Occlusion queries can create false 0 result in boolean and counter modes */
#define BASE_HW_ISSUE_8879  0

/* Output has half intensity with blend shaders enabled on 8xMSAA. */
#define BASE_HW_ISSUE_8896  0

/* 8xMSAA does not work with CRC */
#define BASE_HW_ISSUE_8975  0

/* Boolean occlusion queries don't work properly due to sdc issue. */
#define BASE_HW_ISSUE_8986  0

/* Change in RMUs in use causes problems related with the core's SDC */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_8987  0

/* Occlusion query result is not updated if color writes are disabled. */
#define BASE_HW_ISSUE_9010  0

/* Problem with number of work registers in the RSD if set to 0 */
#define BASE_HW_ISSUE_9275  0

/* Translate load/store moves into decode instruction */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_9418  0

/* Incorrect coverage mask for 8xMSAA */
#define BASE_HW_ISSUE_9423  0

/* Compute endpoint has a 4-deep queue of tasks, meaning a soft stop won't complete until all 4 tasks have completed */
#define BASE_HW_ISSUE_9435  1

/* HT: Tiler returns TERMINATED for command that hasn't been terminated */
#define BASE_HW_ISSUE_9510  0

/* Livelock issue using atomic_cmpxchg */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_9566  0

/* Occasionally the GPU will issue multiple page faults for the same address before the MMU page table has been read by the GPU */
#define BASE_HW_ISSUE_9630  0

/* RA DCD load request to SDC returns invalid load ignore causing colour buffer mismatch */
#define BASE_HW_ISSUE_10327 0

/* Occlusion query result may be updated prematurely when fragment shader alters coverage */
#define BASE_HW_ISSUE_10410 0

/* TEXGRD doesn't honor Sampler Descriptor LOD clamps nor bias */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_10471 1

/* MAG / MIN filter selection happens after image descriptor clamps were applied */
#define BASE_HW_ISSUE_10472 0

/* GPU interprets sampler and image descriptor pointer array sizes as one bigger than they are defined in midg structures */
#define BASE_HW_ISSUE_10487 0

/* ld_special 0x1n applies SRGB conversion */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_10607 0

/* LD_SPECIAL instruction reads incorrect RAW tile buffer value when internal tib format is R10G10B10A2 */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_10632 0

/* MMU TLB invalidation hazards */
#define BASE_HW_ISSUE_10649 1

/* Indexed format 95 cannot be used with a component swizzle of "set to 1" when sampled as integer texture */
#define BASE_HW_ISSUE_10682 0

/* sometimes HW doesn't invalidate cached VPDs when it has to */
#define BASE_HW_ISSUE_10684 1

/* Chicken bit on (t67x_r1p0 and t72x) to work for a HW workaround in compiler */
#define BASE_HW_ISSUE_10797 1

/* soft-stopping fragment jobs, might fail with TILE_RANGE_ERROR */
#define BASE_HW_ISSUE_10817 0

/* Fragment frontend heuristic bias to force early-z required */
#define BASE_HW_ISSUE_10821 1

/* Intermittent missing interrupt on job completion */
#define BASE_HW_ISSUE_10883 1

/* Depth bounds incorrectly normalized in hierz depth bounds test */
#define BASE_HW_ISSUE_10931 1

/* Incorrect cubemap sampling */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_10946 1

/* Soft-stopping fragment jobs might fail with TILE_RANGE_ERROR (similar to issue 10817) and can use BASE_HW_ISSUE_10817 workaround  */
#define BASE_HW_ISSUE_10959 0

/* Soft-stopped fragment shader job can restart with out-of-bound restart index  */
#define BASE_HW_ISSUE_10969 0

/* Instanced arrays conformance fail, workaround by unrolling */
#define BASE_HW_ISSUE_10984 0

/* TEX_INDEX lod selection (immediate , register) not working with 8.8 encoding for levels > 1 */
/* NOTE: compiler workaround: keep in sync with _essl_hwrev_needs_workaround() */
#define BASE_HW_ISSUE_10995 0

/* LD_SPECIAL instruction reads incorrect RAW tile buffer value when internal tib format is RGB565 or RGBA5551 */
#define BASE_HW_ISSUE_11012 0

/* Race condition can cause tile list corruption */
#define BASE_HW_ISSUE_11020 0

/* Write buffer can cause tile list corruption */
#define BASE_HW_ISSUE_11024 0

/* T76X hw issues */

/* Partial 16xMSAA support */
#define BASE_HW_ISSUE_T76X_26   0

/* Forward pixel kill doesn't work with MRT */
#define BASE_HW_ISSUE_T76X_2121 0

/* CRC not working with MFBD and more than one render target */
#define BASE_HW_ISSUE_T76X_2315 0

/* Some indexed formats not supported for MFBD preload. */
#define BASE_HW_ISSUE_T76X_2686 0

/* Must disable CRC if the tile output size is 8 bytes or less. */
#define BASE_HW_ISSUE_T76X_2712 0

/* DBD clean pixel enable bit is reserved */
#define BASE_HW_ISSUE_T76X_2772 0

/* AFBC is not supported for T76X beta. */
#define BASE_HW_ISSUE_T76X_2906 0

/* Prevent MMU deadlock for T76X beta. */
#define BASE_HW_ISSUE_T76X_3285 0

/* Clear encoder state for a hard stopped fragment job which is AFBC 
 * encoded by soft resetting the GPU. Only for T76X r0p0 and r0p1
 */
#define BASE_HW_ISSUE_T76X_3542 0

/* Do not use 8xMSAA with 16x8 pixel tile size or 16xMSAA with 8x8 pixel
 * tile size when no D/S-attachment is present.
 */
#define BASE_HW_ISSUE_T76X_3556 0

/* For cases forcing early-zs when discard is enabled (valid if zs are read only),
 * T76X is more power efficient using reads-from-tilebuffer, whereas T6xx cores
 * are more efficient using blend-uses-destination. Note that this is difference
 * in hardware, but not an errata.
 */
#define BASE_HW_ISSUE_T76X_3700 0

/* ST_TILEBUFFER is not supported */
#define BASE_HW_ISSUE_T76X_3759 0

/* Preload ignores any size or bounding box restrictions of the output image. */
#define BASE_HW_ISSUE_T76X_3793 0

#endif /* _BASE_HWCONFIG_H_ */
