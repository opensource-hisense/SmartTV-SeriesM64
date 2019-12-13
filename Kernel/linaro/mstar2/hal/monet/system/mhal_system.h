////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mhal_system.h
/// @brief  System Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "mst_platform.h"

#ifndef _HAL_SYSTEM_H_
#define _HAL_SYSTEM_H_

//Fix me!!Needs to be reivewed!!
#if defined(CONFIG_ARM) || defined(CONFIG_MIPS)
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#elif defined(CONFIG_ARM64)
extern ptrdiff_t mstar_pm_base;
#define REG_ADDR(addr)                         (*((volatile U16*)(mstar_pm_base + 0x00200000 + (addr << 1))))
#endif
#define REG_CKG_AEONTS0                         0x3360UL
#define AEON_CLK_ENABLE                         0x0000UL //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001UL
#define MIU_PROTECT_EN                          0x1230UL
#define MIU_PROTECT3_ID0                        0x1232UL
#define MIU_PROTECT3_START_ADDR_H               0x1234UL
#define MIU_PROTECT3_END_ADDR_H                 0x1236UL
#define MIU_CLI_AEON_RW                         0x0004UL
#define MIU_PROTECT_4                           0x0001UL
#define MBX_AEON_JUDGEMENT                      0x33DEUL
#define MHEG5_CPU_STOP                          0x0FE6UL
#define STOP_AEON                               0x0001UL
#define MHEG5_REG_IOWINORDER                    0x0F80UL
#define REG_AEON_C_FIQ_MASK_L                   0x1148UL
#define REG_AEON_C_FIQ_MASK_H                   0x114AUL
#define REG_AEON_C_IRQ_MASK_L                   0x1168UL
#define REG_AEON_C_IRQ_MASK_H                   0x116AUL
#define REG_MAU0_MIU0_SIZE                      0x1842UL

#define REG_CHIP_NAND_MODE						0x1EDEUL
	#define REG_CHIP_NAND_MODE_MASK				0x00C0UL
	#define REG_CHIP_NAND_MODE_PCMA				0x0080UL
	#define REG_CHIP_NAND_MODE_PCMD				0x0040UL
#define REG_CHIP_PCMCFG                         0x1EC8UL
    #define REG_CHIP_PCMCFG_MASK                0x0030UL
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030UL //AD pads & Ctrl pads enable

#define REG_SD_CONFIG2                          0x1EB4UL
    #define REG_SD_CONFIG2_MASK                 0x0F00UL //reg_sd_config & reg_sd_config2
#define REG_EMMC_CONFIG                         0x1EDCUL
    #define REG_EMMC_CONFIG_MASK                0x00C0UL

#define REG_CHIP_PCM_PE                         0x1E12UL
    #define REG_CHIP_PCM_PE_MASK                0x00FFUL // PCM_D
    #define REG_CHIP_PCM_CTL_MASK               0xFF00UL // PCM_CTRL (except PCM_CD, PCM_RESET)
#define REG_CHIP_PCM_PE1                        0x1E14UL
    #define REG_CHIP_PCM_PE1_MASK               0x00FFUL // PCM_A
#define REG_CHIP_PCM_D_PE                       0x1E16UL
    #define REG_CHIP_PCM_D_PE_MASK              0x0003UL
    
#define REG_CHIP_PCM_NAND_BYPASS                0x1E20UL
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0100UL
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0100UL
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000UL

#if 0
#define ARM_MIU0_BUS_BASE                      0x20000000UL
#define ARM_MIU1_BUS_BASE                      0xA0000000UL
#define ARM_MIU0_BASE                          0x00000000UL
#define ARM_MIU1_BASE                          0x80000000UL

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFFUL
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFFUL
#define ARM_MIU2_BASE                          0xFFFFFFFFUL
#define ARM_MIU3_BASE                          0xFFFFFFFFUL
#endif

void MHal_PCMCIA_SetPad(BOOL bRestore);
BOOL MHal_PCMCIA_WaitEMMCDone(U32 u32loopCnt);

#endif // _HAL_SYSTEM_H_

