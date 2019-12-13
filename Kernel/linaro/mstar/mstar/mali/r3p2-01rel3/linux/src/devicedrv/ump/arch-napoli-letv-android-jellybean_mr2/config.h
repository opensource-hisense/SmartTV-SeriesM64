/*
 * Copyright (C) 2010-2012 MStar Semiconductor, Inc. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __ARCH_CONFIG_H__
#define __ARCH_CONFIG_H__

#define ARCH_UMP_BACKEND_DEFAULT            1           /* OS_MEMORY */
#define ARCH_UMP_MEMORY_ADDRESS_DEFAULT     0x00000000  /* only required for the dedicated memory allocator */
#define ARCH_UMP_MEMORY_SIZE_DEFAULT        (512 << 20) /* 1024MB */

#endif /* __ARCH_CONFIG_H__ */
