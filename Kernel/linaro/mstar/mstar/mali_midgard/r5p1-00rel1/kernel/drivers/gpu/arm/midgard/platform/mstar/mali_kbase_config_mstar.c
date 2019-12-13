/*
 *
 * (C) COPYRIGHT MStar Semiconductor, Inc. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */

#include <mali_kbase.h>
#include <mali_kbase_config.h>
#include "mali_kbase_platform_mstar.h"
#include "mali_kbase_cpu_mstar.h"

#include <chip_int.h>

#ifdef MSTAR_DISABLE_CONFIG_OF
#ifdef CONFIG_OF
#undef CONFIG_OF
#endif
#endif

#ifdef MSTAR_RIU_ENABLED
#define mstar_riu_offset(offset) ((((offset) & 0xFFFF0000) << 1) | ((offset) & 0x0000FFFF))
#endif

/* */
#define KBASE_CPU_SPEED_FUNC                ((uintptr_t)&kbase_get_mstar_cpu_clock_speed)
#define KBASE_GPU_SPEED_FUNC                ((uintptr_t)NULL)
#define KBASE_PLATFORM_FUNCS                ((uintptr_t)&platform_funcs)
#define KBASE_POWER_MANAGEMENT_CALLBACKS    ((uintptr_t)&pm_callbacks)

#define KBASE_JS_SCHEDULING_TICK_NS          1250000000u /* 1.25s */
#define KBASE_JS_SOFT_STOP_TICKS             2           /* 2.5s before soft-stop a job */
#define KBASE_JS_SOFT_STOP_TICKS_CL          1           /* 1.25s before soft-stop a CL job */
#define KBASE_JS_HARD_STOP_TICKS_SS          4           /* 5s before hard-stop */
#define KBASE_JS_HARD_STOP_TICKS_CL          2           /* 2.5s before hard-stop */
#define KBASE_JS_HARD_STOP_TICKS_NSS         1200        /* 1500s before NSS hard-stop */
#define KBASE_JS_RESET_TICKS_SS              6           /* 7.5s before resetting GPU */
#define KBASE_JS_RESET_TICKS_CL              3           /* 7.5s before resetting GPU */
#define KBASE_JS_RESET_TICKS_NSS             1201        /* 1502s before resetting GPU */
#define KBASE_JS_RESET_TIMEOUT_MS            3000        /* 3s before cancelling stuck jobs */

/* platform configs */
#ifndef CONFIG_OF
static kbase_io_resources io_resources =
{
    .job_irq_number = MALI_IRQ,
    .mmu_irq_number = MALI_IRQ,
    .gpu_irq_number = MALI_IRQ,
    .io_memory_region =
    {
        .start  = MALI_BASE_ADDRESS,
#ifdef MSTAR_RIU_ENABLED
        .end    = MALI_BASE_ADDRESS + mstar_riu_offset((4096 * 4) - 1)
#else
        .end    = MALI_BASE_ADDRESS + (4096 * 4) - 1
#endif
    }
};
#endif

static struct kbase_platform_funcs_conf platform_funcs =
{
    .platform_init_func = mstar_platform_init,
    .platform_term_func = mstar_platform_term
};

static struct kbase_pm_callback_conf pm_callbacks =
{
    .power_off_callback             = mstar_platform_power_off,
    .power_on_callback              = mstar_platform_power_on,
    .power_suspend_callback         = mstar_platform_power_suspend,
    .power_resume_callback          = mstar_platform_power_resume,
    .power_runtime_init_callback    = NULL,
    .power_runtime_term_callback    = NULL,
    .power_runtime_off_callback     = NULL,
    .power_runtime_on_callback      = NULL
};

static struct kbase_attribute config_attributes[] =
{
    /* KBASE_CONFIG_ATTR_GPU_IRQ_THROTTLE_TIME_US */
    { KBASE_CONFIG_ATTR_JS_SCHEDULING_TICK_NS,      KBASE_JS_SCHEDULING_TICK_NS         },
    { KBASE_CONFIG_ATTR_JS_SOFT_STOP_TICKS,         KBASE_JS_SOFT_STOP_TICKS            },
    { KBASE_CONFIG_ATTR_JS_SOFT_STOP_TICKS_CL,      KBASE_JS_SOFT_STOP_TICKS_CL         },
    { KBASE_CONFIG_ATTR_JS_HARD_STOP_TICKS_SS,      KBASE_JS_HARD_STOP_TICKS_SS         },
    { KBASE_CONFIG_ATTR_JS_HARD_STOP_TICKS_CL,      KBASE_JS_HARD_STOP_TICKS_CL         },
    { KBASE_CONFIG_ATTR_JS_HARD_STOP_TICKS_NSS,     KBASE_JS_HARD_STOP_TICKS_NSS        },
    /* KBASE_CONFIG_ATTR_JS_CTX_TIMESLICE_NS */
    /* KBASE_CONFIG_ATTR_JS_CFS_CTX_RUNTIME_INIT_SLICES */
    /* KBASE_CONFIG_ATTR_JS_CFS_CTX_RUNTIME_MIN_SLICES */
    { KBASE_CONFIG_ATTR_JS_RESET_TICKS_SS,          KBASE_JS_RESET_TICKS_SS             },
    { KBASE_CONFIG_ATTR_JS_RESET_TICKS_CL,          KBASE_JS_RESET_TICKS_CL             },
    { KBASE_CONFIG_ATTR_JS_RESET_TICKS_NSS,         KBASE_JS_RESET_TICKS_NSS            },
    { KBASE_CONFIG_ATTR_JS_RESET_TIMEOUT_MS,        KBASE_JS_RESET_TIMEOUT_MS           },
    { KBASE_CONFIG_ATTR_POWER_MANAGEMENT_CALLBACKS, KBASE_POWER_MANAGEMENT_CALLBACKS    },
    { KBASE_CONFIG_ATTR_CPU_SPEED_FUNC,             KBASE_CPU_SPEED_FUNC                },
    /* KBASE_CONFIG_ATTR_GPU_SPEED_FUNC */
    { KBASE_CONFIG_ATTR_PLATFORM_FUNCS,             KBASE_PLATFORM_FUNCS                },
    /* KBASE_CONFIG_ATTR_ARID_LIMIT */
    /* KBASE_CONFIG_ATTR_AWID_LIMIT */
    /* KBASE_CONFIG_ATTR_POWER_MANAGEMENT_DVFS_FREQ */
    /* KBASE_CONFIG_ATTR_PM_GPU_POWEROFF_TICK_NS */
    /* KBASE_CONFIG_ATTR_PM_POWEROFF_TICK_SHADER */
    /* KBASE_CONFIG_ATTR_PM_POWEROFF_TICK_GPU */
    { KBASE_CONFIG_ATTR_END,                        0                                   }
};

static struct kbase_platform_config platform_config =
{
    .attributes = config_attributes,
#ifndef CONFIG_OF
    .io_resources = &io_resources
#endif
};

/* platform functions */
struct kbase_platform_config *kbase_get_platform_config(void)
{
    return &platform_config;
}

int kbase_platform_early_init(void)
{
    /* Nothing needed at this stage */
    return 0;
}
