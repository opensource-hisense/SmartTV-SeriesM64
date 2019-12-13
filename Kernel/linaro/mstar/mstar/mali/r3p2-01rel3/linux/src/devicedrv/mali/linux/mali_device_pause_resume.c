/**
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2010-2013 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/**
 * @file mali_device_pause_resume.c
 * Implementation of the Mali pause/resume functionality
 */

#include <linux/module.h>
#include <linux/mali/mali_utgard.h>
#include "mali_gp_scheduler.h"
#include "mali_pp_scheduler.h"
#if MSTAR_ADJUST_GPU_CLOCK
#include "mali_platform.h"
#endif

void mali_dev_pause(void)
{
	mali_gp_scheduler_suspend();
	mali_pp_scheduler_suspend();
}

EXPORT_SYMBOL(mali_dev_pause);

void mali_dev_resume(void)
{
	mali_gp_scheduler_resume();
	mali_pp_scheduler_resume();
}

EXPORT_SYMBOL(mali_dev_resume);

#if MSTAR_ADJUST_GPU_CLOCK
void mali_dev_adjust_gpu_clock(void)
{
	mali_dvfs_adjust_gpu_clock();
}

EXPORT_SYMBOL(mali_dev_adjust_gpu_clock);
#endif
