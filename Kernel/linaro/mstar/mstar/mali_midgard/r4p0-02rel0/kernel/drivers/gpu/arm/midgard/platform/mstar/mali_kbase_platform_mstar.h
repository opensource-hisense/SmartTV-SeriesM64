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

#ifndef _KBASE_PLATFORM_MSTAR_H_
#define _KBASE_PLATFORM_MSTAR_H_

#include <mali_kbase_defs.h>

mali_bool mstar_platform_init(struct kbase_device *kbdev);
void mstar_platform_term(struct kbase_device *kbdev);
void mstar_platform_power_off(struct kbase_device *kbdev);
int mstar_platform_power_on(struct kbase_device *kbdev);
void mstar_platform_power_suspend(struct kbase_device *kbdev);
void mstar_platform_power_resume(struct kbase_device *kbdev);

#endif /* _KBASE_PLATFORM_MSTAR_H_ */
