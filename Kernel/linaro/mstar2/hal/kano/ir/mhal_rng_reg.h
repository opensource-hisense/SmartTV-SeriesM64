////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regRNG.h
/// @brief  Random Number Generation Registers Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_RNG_REG_H_
#define _MHAL_RNG_REG_H_

#if defined(CONFIG_ARM) || defined(CONFIG_MIPS)
#define RIU_MAP                    0xFD200000
#elif defined(CONFIG_ARM64)
extern ptrdiff_t mstar_pm_base;
#define RIU_MAP                    (mstar_pm_base + 0x00200000UL)
#endif

#define RIU                               ((unsigned short volatile *) RIU_MAP)
#define REG_MIPS_BASE    (0x1D00UL)
#define MIPS_REG(addr)      RIU[(addr<<1)+REG_MIPS_BASE]
#define REG_RNG_OUT       0x0eUL

#endif
