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
 * @file mstar_platform.h
 * MStar platform specific driver functions
 */

/**
 *         Bus base addr       --> Physical base addr
 *
 * MIU0    MSTAR_MIU0_BUS_BASE --> 0x00000000
 *
 * MIU1    MSTAR_MIU1_BUS_BASE --> (MSTAR_MIU1_BUS_BASE - MSTAR_MIU0_BUS_BASE)
 */
#include <linux/types.h>
#include "mstar/mstar_chip.h"
#if defined(CONFIG_MALI_DVFS)
#include <linux/mali/mali_utgard.h>
#endif

#define PHYS_TO_BUS_ADDRESS_ADJUST MSTAR_MIU0_BUS_BASE

#if (MSTAR_MIU_BOUNDARY_ADDR_SETTING == 1)
/* Setting MIU start/end address to support TLB address mapping in case TLB region is between MIU0 and MIU1
 * The lower 20 bits are always zero and no longer need to be set in register.
 */
#if defined(CONFIG_ARCH_DMA_ADDR_T_64BIT)
/* set MIU physical address to the middle of address mapping */
#define MIU1_PHY_BASE_ADDR_HIGH 0x0800U
#define MSTAR_MIU1_PHY_BASE (MIU1_PHY_BASE_ADDR_HIGH << 20)
#else
#define MSTAR_MIU1_PHY_BASE     (MSTAR_MIU1_BUS_BASE - MSTAR_MIU0_BUS_BASE)
#define MIU1_PHY_BASE_ADDR_HIGH ((MSTAR_MIU1_PHY_BASE >> 20) & 0xffff)
#endif /* if defined(CONFIG_ARCH_DMA_ADDR_T_64BIT) */
#else
#define MSTAR_MIU1_PHY_BASE     (MSTAR_MIU1_BUS_BASE - MSTAR_MIU0_BUS_BASE)
#define MIU1_PHY_BASE_ADDR_LOW  ( MSTAR_MIU1_PHY_BASE        & 0xffff)
#define MIU1_PHY_BASE_ADDR_HIGH ((MSTAR_MIU1_PHY_BASE >> 16) & 0xffff)
#endif /* if (MSTAR_MIU_BOUNDARY_ADDR_SETTING == 1) */

#if defined(CONFIG_ARCH_DMA_ADDR_T_64BIT)
/* We need to compute the correct mapping offset of MIU1 while DMA address support 64 bits */
#define PHYS_TO_BUS_ADDRESS_ADJUST_MIU1 (MSTAR_MIU1_BUS_BASE - MSTAR_MIU1_PHY_BASE)

static __inline__ __attribute__ ((always_inline))void bus_to_phys_address_adjust(dma_addr_t *addr) {
	if (*addr < MSTAR_MIU1_BUS_BASE) {
		*addr -= PHYS_TO_BUS_ADDRESS_ADJUST;
	}
	else {
		/* TODO:Fix mapping strategy, CPU will not use bus address >= 0x2_8000_0000 for now */
		*addr -= PHYS_TO_BUS_ADDRESS_ADJUST_MIU1;
	}
}

static __inline__ __attribute__ ((always_inline))void phys_to_bus_address_adjust(dma_addr_t *addr) {
	if (*addr < MSTAR_MIU1_PHY_BASE) {
		*addr += PHYS_TO_BUS_ADDRESS_ADJUST;
	}
	else {
		*addr += PHYS_TO_BUS_ADDRESS_ADJUST_MIU1;
	}
}
#else
static __inline__ __attribute__ ((always_inline))void bus_to_phys_address_adjust(dma_addr_t *addr) {
	*addr -= PHYS_TO_BUS_ADDRESS_ADJUST;
}

static __inline__ __attribute__ ((always_inline))void phys_to_bus_address_adjust(dma_addr_t *addr) {
	*addr += PHYS_TO_BUS_ADDRESS_ADJUST;
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @brief MStar platform specific setup and initialisation function
 *
 * @return _MALI_OSK_ERR_OK on success otherwise, a suitable _mali_osk_errcode_t error.
 */
void mstar_platform_init(void);

/** @brief MStar platform specific deinitialisation function
 *
 * @return _MALI_OSK_ERR_OK on success otherwise, a suitable _mali_osk_errcode_t error.
 */
void mstar_platform_deinit(void);

/** @brief MStar platform power on function
 *
 */
void mstar_platform_power_on(void);

/** @brief MStar platform power off function
 *
 */
void mstar_platform_power_off(void);

/** @brief MStar platform suspend function
 *
 */
void mstar_platform_suspend(void);

/** @brief MStar platform resume function
 *
 */
void mstar_platform_resume(void);

/** @brief MStar platform runtime suspend function
 *
 */
void mstar_platform_runtime_suspend(void);

/** @brief MStar platform runtime resume function
 *
 */
void mstar_platform_runtime_resume(void);

#if defined(CONFIG_MALI_DVFS)
void mstar_platform_report_gpu_clock_info(struct mali_gpu_clock **data);
int mstar_platform_get_gpu_clock_step(void);
bool mstar_platform_set_gpu_clock_step(int step);
#endif

#ifdef __cplusplus
}
#endif
