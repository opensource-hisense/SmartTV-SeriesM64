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

#ifndef _MALI_KBASE_MEM_MSTAR_H_
#define _MALI_KBASE_MEM_MSTAR_H_

#include <malisw/mali_malisw.h>
#include "mstar/mstar_chip.h"

#ifndef MONET

#define BUS_TO_MIU_ADDRESS(bus_addr) (bus_addr)
#define MIU_TO_BUS_ADDRESS(miu_addr) (miu_addr)

#else

/* MONET diamond address map:
 * --------------------- --------------------- ---------------------
 * Device Name           ARM Bus Address[39:0] MALI MIU Address
 * ===================== ===================== =====================
 * MIU0     [0000~2048M] 0x20000000~0x9FFFFFFF 0x00000000~0x3FFFFFFF
 * MIU1     [0000~1024M] 0xA0000000~0xDFFFFFFF 0x80000000~0xBFFFFFFF
 * MIU_TLB0 [0000~ 256M] 0xE0000000~0xEFFFFFFF 0x40000000~0x7FFFFFFF
 * MIU_TLB1 [0000~ 256M] 0xF0000000~0xFFFFFFFF 0xC0000000~0xFFFFFFFF
 *
 *                      ARM BUS           MALI MIU
 *       0x20000000 +------------+     +------------+ 0x00000000
 *                  |  MIU0      |     | MIU0       |
 *       0xA0000000 +------------+     +------------+ 0x40000000
 *                  |  MIU1      |     | MIU_TLB0   |
 *       0xE0000000 +------------+  X  +------------+ 0x80000000
 *                  |  MIU_TLB0  |     | MIU1       |
 *       0xF0000000 +------------+     +------------+ 0xC0000000
 *                  |  MIU_TLB1  |     | MIU_TLB1   |
 *                  +------------+     +------------+
 */
static FORCE_INLINE u64 BUS_TO_MIU_ADDRESS(phys_addr_t bus_addr)
{
    /* MIU0/MIU1 */
    if (bus_addr < 0xE0000000ULL) return (bus_addr - 0x20000000ULL);

    /* MIU_TLB0 */
    if ((bus_addr & 0x10000000ULL) == 0) return (bus_addr - 0xA0000000ULL);

    /* MIU_TLB1 */
    return (bus_addr - 0x30000000ULL);
}

static FORCE_INLINE phys_addr_t MIU_TO_BUS_ADDRESS(u64 miu_addr)
{
    /* MIU0 */
    if (miu_addr < 0x40000000ULL) return (miu_addr + 0x20000000ULL);

    /* MIU_TLB0 */
    if (miu_addr < 0x80000000ULL) return (miu_addr + 0xA0000000ULL);

    /* MIU1 */
    if (miu_addr < 0xC0000000ULL) return (miu_addr + 0x20000000ULL);

    /* MIU_TLB1 */
    return (miu_addr + 0x30000000ULL);
}

#endif

#endif	/* _MALI_KBASE_MEM_MSTAR_H_ */
