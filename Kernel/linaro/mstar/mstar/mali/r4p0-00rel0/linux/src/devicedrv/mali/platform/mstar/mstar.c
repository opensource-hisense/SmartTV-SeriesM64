/*
 * Copyright (C) 2010-2012 MStar Semiconductor, Inc. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_platform.c
 * Platform specific Mali driver functions for a MStar platform
 */
#include <linux/platform_device.h>
#include <linux/version.h>
#include <linux/pm.h>
#ifdef CONFIG_PM_RUNTIME
#include <linux/pm_runtime.h>
#endif
#include <linux/mali/mali_utgard.h>
#include "mali_kernel_common.h"
#include <linux/dma-mapping.h>

#include "mstar_platform.h"
#include <chip_int.h>

static void mali_platform_device_release(struct device *device);
void mali_gpu_utilization_callback(struct mali_gpu_utilization_data *data);

unsigned long mali_gpu_utilization = 0;

/* Mali GPU resources: all cores share the same IRQ */
static struct resource mali_gpu_resources[] =
{
#if defined(MALI400)
#if (NUM_PP == 1)
	MALI_GPU_RESOURCES_MALI400_MP1(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 2)
	MALI_GPU_RESOURCES_MALI400_MP2(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 3)
	MALI_GPU_RESOURCES_MALI400_MP3(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 4)
	MALI_GPU_RESOURCES_MALI400_MP4(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#else
	#error "invalid NUM_PP for MALI-400"
#endif
#elif defined(MALI450)
#if (NUM_PP == 1)
	MALI_GPU_RESOURCES_MALI450_MP1(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 2)
	MALI_GPU_RESOURCES_MALI450_MP2(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 3)
	MALI_GPU_RESOURCES_MALI450_MP3(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 4)
	MALI_GPU_RESOURCES_MALI450_MP4(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 6)
	MALI_GPU_RESOURCES_MALI450_MP6(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#elif (NUM_PP == 8)
	MALI_GPU_RESOURCES_MALI450_MP8(GPU_BASE_ADDRESS, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ, MALI_IRQ)
#else
#error "invalid NUM_PP for MALI-450"
#endif
#else
#error "unknown GPU"
#endif
};

static struct mali_gpu_device_data mali_gpu_data =
{
#if defined(DEDICATED_MEM_START) && defined(DEDICATED_MEM_SIZE)
	.dedicated_mem_start = DEDICATED_MEM_START,
	.dedicated_mem_size = DEDICATED_MEM_SIZE,
#endif

#ifdef SHARED_MEM_SIZE
	.shared_mem_size = SHARED_MEM_SIZE,
#endif

#if defined(FB_START) && defined(FB_SIZE)
	.fb_start = FB_START,
	.fb_size = FB_SIZE,
#endif

	.max_job_runtime = 60000,      /* 60 seconds */
	.utilization_interval = 1000,  /* 1000ms */
	.utilization_callback = mali_gpu_utilization_callback,

	/*
	.pmu_switch_delay
	.pmu_domain_config
	.set_freq_callback
	*/
};

static struct platform_device mali_gpu_device =
{
	.name = MALI_GPU_NAME_UTGARD,
	.id = 0,
	.dev.release = mali_platform_device_release,
	.dev.coherent_dma_mask = DMA_BIT_MASK(32),
	.dev.platform_data = &mali_gpu_data,
};

int mali_platform_device_register(void)
{
	int err = -1;
	int num_pp_cores = 0;

	MALI_DEBUG_PRINT(4, ("mali_platform_device_register() called\n"));

	mstar_platform_init();

	/* Connect the resources to the device */
#if defined(MALI400)
	MALI_DEBUG_PRINT(4, ("Registering Mali-400 MP%d device\n", NUM_PP));
#elif defined(MALI450)
	MALI_DEBUG_PRINT(4, ("Registering Mali-450 MP%d device\n", NUM_PP));
#else
	#error "unknown GPU"
#endif

	num_pp_cores = NUM_PP;
	mali_gpu_device.num_resources = ARRAY_SIZE(mali_gpu_resources);
	mali_gpu_device.resource = mali_gpu_resources;

	/* Register the platform device */
	err = platform_device_register(&mali_gpu_device);

	if (0 == err)
	{
#ifdef CONFIG_PM_RUNTIME
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37))
		pm_runtime_set_autosuspend_delay(&(mali_gpu_device.dev), 1000);
		pm_runtime_use_autosuspend(&(mali_gpu_device.dev));
#endif
		pm_runtime_enable(&(mali_gpu_device.dev));
#endif
		MALI_DEBUG_ASSERT(0 < num_pp_cores);

		return 0;
	}

	return err;
}

void mali_platform_device_unregister(void)
{
	MALI_DEBUG_PRINT(4, ("mali_platform_device_unregister() called\n"));

	platform_device_unregister(&mali_gpu_device);
	mstar_platform_deinit();
	platform_device_put(&mali_gpu_device);
}

static void mali_platform_device_release(struct device *device)
{
	MALI_DEBUG_PRINT(4, ("mali_platform_device_release() called\n"));
}

void mali_gpu_utilization_callback(struct mali_gpu_utilization_data *data)
{
	mali_gpu_utilization = data->utilization_gpu;
}
