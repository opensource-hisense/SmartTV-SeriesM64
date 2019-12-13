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
/// @file   drvSystem.h
/// @brief  System Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "mdrv_system_st.h"
#include "mst_platform.h"

#ifndef _HAL_SYSTEM_H_
#define _HAL_SYSTEM_H_

   #define DEFINE_IN_MSTAR_CHIP_H 1
   #include <include/mstar/mstar_chip.h>

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SYS_BOARD_NAME_MAX          32                                  ///< Maximum length of board name
#define SYS_PLATFORM_NAME_MAX       32                                  ///< Maximum length of playform name

#if 1
#define REG_SW_RESET_CPU_AEON                   0x1086
    //---------------------------------------------
    // definition for REG_SW_RESET_CPU_AEON   //reg[0x1086]
    #define AEON_SW_RESET                           BIT0

#define AEON_SPI_ADDR0                          0x0FFE

#if defined(CONFIG_MSTAR_TITANIA2)
#define REG_ADDR(addr)              (*((volatile U16*)(0xBF800000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x1E26
#define AEON_CLK_ENABLE                         0x00
#define AEON_CLK_DISABLE                        0x40
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0006
    #define MIU_PROTECT_4                       0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x2B40
#define REG_AEON_C_FIQ_MASK_H                   0x2B42
#define REG_AEON_C_IRQ_MASK_L                   0x2B58
#define REG_AEON_C_IRQ_MASK_H                   0x2B5A

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x0010

#define MIPS_MIU0_BUS_BASE                      0x00000000

#elif defined(CONFIG_MSTAR_EUCLID)
#define REG_ADDR(addr)              (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x1E24
#define AEON_CLK_ENABLE                         0x0000
#define AEON_CLK_DISABLE                        0x4000
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0006
    #define MIU_PROTECT_4                       0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x11940
#define REG_AEON_C_FIQ_MASK_H                   0x11942
#define REG_AEON_C_IRQ_MASK_L                   0x11958
#define REG_AEON_C_IRQ_MASK_H                   0x1195A

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x0030

#define MIPS_MIU0_BUS_BASE                      0x00000000

#elif defined(CONFIG_MSTAR_TITANIA3) || defined(CONFIG_MSTAR_TITANIA10)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)              (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x6948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x0010

#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x40000000

#elif defined(CONFIG_MSTAR_URANUS4)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)              (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1E0C
	#define REG_CHIP_NAND_MODE_MASK				0x000F

#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x40000000

#elif defined(CONFIG_MSTAR_TITANIA4)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)              (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x0010

#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x40000000

#elif defined(CONFIG_MSTAR_TITANIA8) || \
	  defined(CONFIG_MSTAR_TITANIA12) || \
      defined(CONFIG_MSTAR_AMBER2)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)              (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0040  //Board 116A
	#define REG_CHIP_NAND_MODE_PCMD				0x0080

#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x60000000

#elif defined(CONFIG_MSTAR_AMBER5)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080  //Board 116A
	#define REG_CHIP_NAND_MODE_PCMD				0x0040

#define REG_SD_CONFIG2                          0x1EB4
    #define REG_SD_CONFIG2_MASK                 0x0020 //reg_sd_config & reg_sd_config2
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x0030
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030 //AD pads & Ctrl pads enable
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00F0

#define MIPS_MIU0_BUS_BASE                      0x40000000
#define MIPS_MIU1_BUS_BASE                      0x60000000

#elif defined(CONFIG_MSTAR_TITANIA9) || \
      defined(CONFIG_MSTAR_TITANIA13) || \
      defined(CONFIG_MSTAR_AMBER6) || \
      defined(CONFIG_MSTAR_AMBER7) || \
      defined(CONFIG_MSTAR_AMETHYST) || \
      defined(CONFIG_MSTAR_EMERALD) || \
      defined(CONFIG_MSTAR_NUGGET) || \
	  defined(CONFIG_MSTAR_NIKON) || \
	  defined(CONFIG_MSTAR_MILAN)
#define REG_ADDR(addr)                          (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
 #define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

    #if (defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_TITANIA13))
#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x0010
	#elif defined(CONFIG_MSTAR_EMERALD)
    #define REG_CHIP_NAND_MODE						0x1EA0
    	#define REG_CHIP_NAND_MODE_MASK				0x0300
    	#define REG_CHIP_NAND_MODE_PCMA				0x0200
    	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PCMCFG                         0x1E9E
        #define REG_CHIP_PCMCFG_MASK                0xC000
        #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable
    #define REG_SD_CONFIG2                          0x1EAE
        #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2

    #define REG_CHIP_PCM_PE                         0x1E16
        #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
        #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
    #define REG_CHIP_PCM_PE1                        0x1E18
        #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
    #define REG_CHIP_PCM_D_PE                       0x1E1A
        #define REG_CHIP_PCM_D_PE_MASK              0x0003

	#elif defined(CONFIG_MSTAR_NUGGET)
    #define REG_CHIP_NAND_MODE						0x1EA0
    	#define REG_CHIP_NAND_MODE_MASK				0x0380
//    	#define REG_CHIP_NAND_MODE_PCMA				0x0200
//    	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PCMCFG                         0x1E9E
        #define REG_CHIP_PCMCFG_MASK                0xC000
        #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable
    #define REG_SD_CONFIG2                          0x1EAE
        #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2

    #define REG_CHIP_PCM_PE                         0x1E16
        #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
        #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
    #define REG_CHIP_PCM_PE1                        0x1E18
        #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
    #define REG_CHIP_PCM_D_PE                       0x1E1A
        #define REG_CHIP_PCM_D_PE_MASK              0x0003

	#elif defined(CONFIG_MSTAR_NIKON)
    #define REG_CHIP_NAND_MODE						0x1EA0
    	#define REG_CHIP_NAND_MODE_MASK				0x0380
//    	#define REG_CHIP_NAND_MODE_PCMA				0x0200
//    	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PCMCFG                         0x1E9E
        #define REG_CHIP_PCMCFG_MASK                0xC000
        #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable
    #define REG_SD_CONFIG2                          0x1EAE
        #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2

    #define REG_CHIP_PCM_PE                         0x1E16
        #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
        #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
    #define REG_CHIP_PCM_PE1                        0x1E18
        #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
    #define REG_CHIP_PCM_D_PE                       0x1E1A
        #define REG_CHIP_PCM_D_PE_MASK              0x0003
		
	#elif defined(CONFIG_MSTAR_MILAN)
    #define REG_CHIP_NAND_MODE						0x1EA0
    	#define REG_CHIP_NAND_MODE_MASK				0x0380
//    	#define REG_CHIP_NAND_MODE_PCMA				0x0200
//    	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PCMCFG                         0x1E9E
        #define REG_CHIP_PCMCFG_MASK                0xC000
        #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable
    #define REG_SD_CONFIG2                          0x1EAE
        #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2

    #define REG_CHIP_PCM_PE                         0x1E16
        #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
        #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
    #define REG_CHIP_PCM_PE1                        0x1E18
        #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
    #define REG_CHIP_PCM_D_PE                       0x1E1A
        #define REG_CHIP_PCM_D_PE_MASK              0x0003

    #else
#define REG_CHIP_NAND_MODE						0x1EA0
	#define REG_CHIP_NAND_MODE_MASK				0x0300
    #define REG_CHIP_NAND_MODE_PCMA				0x0200
	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PF_MODE			        0x1EDE
    #define REG_CHIP_PF_MODE_MASK				0x0010
    #endif

    #define MIPS_MIU0_BUS_BASE                      0x00000000
    #define MIPS_MIU1_BUS_BASE                      0x60000000

#elif defined(CONFIG_MSTAR_TITANIA11)
#define REG_ADDR(addr)                          (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0040
	#define REG_CHIP_NAND_MODE_PCMD				0x0080


#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x40000000
#elif defined(CONFIG_MSTAR_JANUS2)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)              (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0


#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x60000000

#elif defined(CONFIG_MSTAR_KRONUS)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                          (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1E0C
	#define REG_CHIP_NAND_MODE_MASK				0x000F


#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x40000000

#elif defined(CONFIG_MSTAR_KAISERIN) || defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KELTIC)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                          (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
    #define MIU_CLI_AEON_RW                     0x0005
    #define MIU_PROTECT_4                       0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
    #define STOP_AEON                           0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE			0x1E0C
#define REG_CHIP_NAND_MODE_MASK			0x000F

#if defined(CONFIG_MSTAR_KAISERIN) || defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KELTIC)
   #include <include/mstar/mstar_chip.h>
   #define MIPS_MIU0_BUS_BASE                   MSTAR_MIU0_BUS_BASE
   #define MIPS_MIU1_BUS_BASE                   MSTAR_MIU1_BUS_BASE
#else
   #define MIPS_MIU0_BUS_BASE                   0x00000000
   #define MIPS_MIU1_BUS_BASE                   0x60000000
#endif

#elif defined(CONFIG_MSTAR_AMBER3)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080  //Board 116A
	#define REG_CHIP_NAND_MODE_PCMD				0x0040
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x0030
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030 //AD pads & Ctrl pads enable
#define REG_SD_CONFIG                           0x1EB4
    #define REG_SD_CONFIG_MASK                  0x0020
#define REG_SD_CONFIG2                          0x1E38 //U02 added
    #define REG_SD_CONFIG2_MASK                 0x1000
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00F0

#define ARM_MIU0_BUS_BASE                      0x40000000
#define ARM_MIU1_BUS_BASE                      0xA0000000
#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x60000000

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                     0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                     0xFFFFFFFF

#elif defined(CONFIG_MSTAR_EAGLE)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1E16
	#define REG_CHIP_NAND_MODE_MASK				0x7000
	#define REG_CHIP_NAND_MODE_PCMA				0x2000
	#define REG_CHIP_NAND_MODE_PCMD				0x1000

#define REG_CHIP_PCMCFG                         0x1E9E
    #define REG_CHIP_PCMCFG_MASK                0xC000
    #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable

#define REG_EMMC_CONFIG                         0x1EBA
    #define REG_EMMC_CONFIG_MASK                0xC000

#define REG_CHIP_PCM_PE                         0x1E82
    #define REG_CHIP_PCM_PE_MASK                0xFFFF
#define REG_CHIP_PCM_PE1                        0x1E84
    #define REG_CHIP_PCM_PE1_MASK               0x03FF
#define REG_CHIP_PCM_D_PE                       0x1E18
    #define REG_CHIP_PCM_D_PE_MASK              0x00FF

#define REG_CHIP_PCM_NAND_BYPASS                0x1E86
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0002
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0002
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define ARM_MIU0_BUS_BASE                      0x40000000
#define ARM_MIU1_BUS_BASE                      0xA0000000
#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x80000000

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                     0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                     0xFFFFFFFF

#elif defined(CONFIG_MSTAR_AGATE)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080  //Board 116A
	#define REG_CHIP_NAND_MODE_PCMD				0x0040
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x0030
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030 //AD pads & Ctrl pads enable
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00F0

#define REG_CHIP_PCM_PE                         0x1E1C
    #define REG_CHIP_PCM_PE_MASK                0xFFFF
#define REG_CHIP_PCM_PE1                        0x1E1E
    #define REG_CHIP_PCM_PE1_MASK               0x03FF
#define REG_CHIP_PCM_D_PE                       0x1E1A
    #define REG_CHIP_PCM_D_PE_MASK              0x00FF

#define REG_CHIP_PCM_NAND_BYPASS                0x1E20
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define ARM_MIU0_BUS_BASE                      0x40000000
#define ARM_MIU1_BUS_BASE                      0xA0000000
#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x80000000

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                     0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                     0xFFFFFFFF

#elif defined(CONFIG_MSTAR_EDISON)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080  //Board 116A
	#define REG_CHIP_NAND_MODE_PCMD				0x0040
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x0030
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030 //AD pads & Ctrl pads enable

#define REG_SD_CONFIG2                          0x1EB4
    #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00C0

#define REG_CHIP_PCM_NAND_BYPASS                0x1E20
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define REG_CHIP_PCM_PE                         0x1E12
    #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
    #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
#define REG_CHIP_PCM_PE1                        0x1E14
    #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
#define REG_CHIP_PCM_D_PE                       0x1E16
    #define REG_CHIP_PCM_D_PE_MASK              0x0003

#define ARM_MIU0_BUS_BASE                      0x40000000
#define ARM_MIU1_BUS_BASE                      0xA0000000
#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x80000000

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                     0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                     0xFFFFFFFF

#elif defined(CONFIG_MSTAR_EINSTEIN) || defined(CONFIG_MSTAR_NAPOLI)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080  //Board 116A
	#define REG_CHIP_NAND_MODE_PCMD				0x0040
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x0030
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030 //AD pads & Ctrl pads enable

#define REG_SD_CONFIG2                          0x1EB4
    #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00C0

#define REG_CHIP_PCM_NAND_BYPASS                0x1E20
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define REG_CHIP_PCM_PE                         0x1E12
    #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
    #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
#define REG_CHIP_PCM_PE1                        0x1E14
    #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
#define REG_CHIP_PCM_D_PE                       0x1E16
    #define REG_CHIP_PCM_D_PE_MASK              0x0003
    
#define ARM_MIU0_BUS_BASE                      0x20000000
#define ARM_MIU1_BUS_BASE                      0xA0000000
#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x80000000

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                     0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                     0xFFFFFFFF

#elif defined(CONFIG_MSTAR_EINSTEIN3) || \
      defined(CONFIG_MSTAR_MONACO)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080  //Board 116A
	#define REG_CHIP_NAND_MODE_PCMD				0x0040
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x0030
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030 //AD pads & Ctrl pads enable

#define REG_SD_CONFIG2                          0x1EB4
    #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00C0

#define REG_CHIP_PCM_NAND_BYPASS                0x1E20
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define REG_CHIP_PCM_PE                         0x1E12
    #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
    #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
#define REG_CHIP_PCM_PE1                        0x1E14
    #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
#define REG_CHIP_PCM_D_PE                       0x1E16
    #define REG_CHIP_PCM_D_PE_MASK              0x0003
    
#define ARM_MIU0_BUS_BASE                      0x20000000
#define ARM_MIU1_BUS_BASE                      0xA0000000
#define ARM_MIU2_BUS_BASE                      0xE0000000
#define ARM_MIU3_BUS_BASE                      0x60000000

#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x80000000
#define ARM_MIU2_BASE_ADDR                     0xc0000000
#define ARM_MIU3_BASE_ADDR                     0xe0000000

#elif defined(CONFIG_MSTAR_EIFFEL)
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x01C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080
	#define REG_CHIP_NAND_MODE_PCMD				0x0040
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x0030
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0030

#define REG_SD_CONFIG2                          0x1EB4
    #define REG_SD_CONFIG2_MASK                 0x0F00
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00C0

#define REG_CHIP_PCM_NAND_BYPASS                0x1E20
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define REG_CHIP_PCM_PE                         0x1E12
    #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
    #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
#define REG_CHIP_PCM_PE1                        0x1E14
    #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
#define REG_CHIP_PCM_D_PE                       0x1E16
    #define REG_CHIP_PCM_D_PE_MASK              0x0003

#define ARM_MIU0_BUS_BASE                       0x20000000
#define ARM_MIU1_BUS_BASE                       0xA0000000
#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x80000000

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                     0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                     0xFFFFFFFF

#elif defined(CONFIG_MSTAR_NIKE) || \
      defined(CONFIG_MSTAR_CLIPPERS) || \
      defined(CONFIG_MSTAR_MADISON)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1E16
	#define REG_CHIP_NAND_MODE_MASK				0x7000
	#define REG_CHIP_NAND_MODE_PCMA				0x2000
	#define REG_CHIP_NAND_MODE_PCMD				0x1000
#define REG_CHIP_PCMCFG                         0x1E9E
    #define REG_CHIP_PCMCFG_MASK                0xC000
    #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable

#define REG_SD_CONFIG2                          0x1EB6
    #define REG_SD_CONFIG2_MASK                 0x0F00
#define REG_EMMC_CONFIG                         0x1EBA
    #define REG_EMMC_CONFIG_MASK                0xC000

#define REG_CHIP_PCM_PE                         0x1E82
    #define REG_CHIP_PCM_PE_MASK                0xFFFF
#define REG_CHIP_PCM_PE1                        0x1E84
    #define REG_CHIP_PCM_PE1_MASK               0x03FF
#define REG_CHIP_PCM_D_PE                       0x1E18 // PAD_PCM_A[7:0] PE Control
    #define REG_CHIP_PCM_D_PE_MASK              0x00FF

#define REG_CHIP_PCM_NAND_BYPASS                0x1E86
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0002
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0002
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define ARM_MIU0_BUS_BASE                       0x20000000
#define ARM_MIU1_BUS_BASE                       0xA0000000
#define ARM_MIU0_BASE_ADDR                      0x00000000
#define ARM_MIU1_BASE_ADDR                      0x80000000

#define ARM_MIU2_BUS_BASE                       0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                       0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                      0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                      0xFFFFFFFF

#elif defined(CONFIG_MSTAR_MIAMI)
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_NAND_MODE						0x1EDE
	#define REG_CHIP_NAND_MODE_MASK				0x00C0
	#define REG_CHIP_NAND_MODE_PCMA				0x0080
	#define REG_CHIP_NAND_MODE_PCMD				0x0040
#define REG_CHIP_PCMCFG                         0x1EC8
    #define REG_CHIP_PCMCFG_MASK                0x00F0
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0050 //AD pads & Ctrl pads enable

#define REG_SD_CONFIG2                          0x1EB4
    #define REG_SD_CONFIG2_MASK                 0x0F00
#define REG_EMMC_CONFIG                         0x1EDC
    #define REG_EMMC_CONFIG_MASK                0x00C0

#define REG_CHIP_PCM_PE                         0x1E12
    #define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
    #define REG_CHIP_PCM_CTL_MASK               0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
#define REG_CHIP_PCM_PE1                        0x1E14
    #define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
#define REG_CHIP_PCM_D_PE                       0x1E16
    #define REG_CHIP_PCM_D_PE_MASK              0x0003

#define REG_CHIP_PCM_NAND_BYPASS                0x1E20
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0100
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define ARM_MIU0_BUS_BASE                       0x20000000
#define ARM_MIU1_BUS_BASE                       0xA0000000
#define ARM_MIU0_BASE_ADDR                      0x00000000
#define ARM_MIU1_BASE_ADDR                      0x80000000

#define ARM_MIU2_BUS_BASE                       0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                       0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                      0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                      0xFFFFFFFF

#elif defined(CONFIG_MSTAR_KAISER) || \
      defined(CONFIG_MSTAR_KAISERS)
//Fix me!!Needs to be reivewed!!
#define REG_ADDR(addr)                         (*((volatile U16*)(0xFD200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x3360
#define AEON_CLK_ENABLE                         0x0000 //CLK 216MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x1230
#define MIU_PROTECT3_ID0                        0x1232
#define MIU_PROTECT3_START_ADDR_H               0x1234
#define MIU_PROTECT3_END_ADDR_H                 0x1236
#define MIU_CLI_AEON_RW                         0x0004
#define MIU_PROTECT_4                           0x0001
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                               0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1148
#define REG_AEON_C_FIQ_MASK_H                   0x114A
#define REG_AEON_C_IRQ_MASK_L                   0x1168
#define REG_AEON_C_IRQ_MASK_H                   0x116A
#define REG_MAU0_MIU0_SIZE                      0x1842

#define REG_CHIP_PCMCFG                         0x1E0E
    #define REG_CHIP_PCMCFG_MASK                0x0003
    #define REG_CHIP_PCMCFG_CTRL_EN             0x0001 //AD pads & Ctrl pads enable

#define REG_SD_CONFIG2                          0x1E0C
    #define REG_SD_CONFIG2_MASK                 0xC000 //reg_sd_mode

#define ARM_MIU0_BUS_BASE                      0x20000000
#define ARM_MIU1_BUS_BASE                      0xA0000000
#define ARM_MIU0_BASE_ADDR                     0x00000000
#define ARM_MIU1_BASE_ADDR                     0x80000000

#define ARM_MIU2_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU3_BUS_BASE                      0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR                     0xFFFFFFFF
#define ARM_MIU3_BASE_ADDR                     0xFFFFFFFF

#endif

#if defined(CONFIG_MSTAR_EUCLID) || \
    defined(CONFIG_MSTAR_URANUS4) || \
    defined(CONFIG_MSTAR_TITANIA2) || \
    defined(CONFIG_MSTAR_TITANIA4) || \
    defined(CONFIG_MSTAR_KRONUS) || \
    defined(CONFIG_MSTAR_KAISERIN) || \
    defined(CONFIG_MSTAR_KAPPA) || \
    defined(CONFIG_MSTAR_KELTIC) || \
    defined(CONFIG_MSTAR_TITANIA11) || \
    defined(CONFIG_MSTAR_AMBER2)

    #define MS_MIU_INTERVAL 0x08000000

#elif defined(CONFIG_MSTAR_TITANIA3) || defined(CONFIG_MSTAR_TITANIA10)

    #define MS_MIU_INTERVAL 0x10000000

#elif defined(CONFIG_MSTAR_TITANIA8) || \
       defined(CONFIG_MSTAR_TITANIA12) || \
       defined(CONFIG_MSTAR_TITANIA9) || \
       defined(CONFIG_MSTAR_TITANIA13) || \
       defined(CONFIG_MSTAR_AMBER5) || \
       defined(CONFIG_MSTAR_AMBER6) || \
       defined(CONFIG_MSTAR_AMBER7) || \
       defined(CONFIG_MSTAR_AMETHYST) || \
       defined(CONFIG_MSTAR_JANUS2) || \
       defined(CONFIG_MSTAR_EMERALD) || \
       defined(CONFIG_MSTAR_NUGGET) || \
	   defined(CONFIG_MSTAR_NIKON) || \
	   defined(CONFIG_MSTAR_MILAN)

    #define MS_MIU_INTERVAL 0x20000000

#elif defined(CONFIG_MSTAR_AMBER3) || \
      defined(CONFIG_MSTAR_EAGLE) || \
      defined(CONFIG_MSTAR_AGATE) || \
      defined(CONFIG_MSTAR_EDISON) || \
      defined(CONFIG_MSTAR_EINSTEIN) || \
      defined(CONFIG_MSTAR_NAPOLI) || \
      defined(CONFIG_MSTAR_EINSTEIN3) || \
      defined(CONFIG_MSTAR_EIFFEL) || \
      defined(CONFIG_MSTAR_NIKE) || \
      defined(CONFIG_MSTAR_KAPPA) || \
      defined(CONFIG_MSTAR_KELTIC) || \
      defined(CONFIG_MSTAR_KAISER) || \
	  defined(CONFIG_MSTAR_KAISERS)

    #define MS_MIU_INTERVAL 0x60000000

#elif defined(CONFIG_MSTAR_MADISON) || \
      defined(CONFIG_MSTAR_CLIPPERS) || \
      defined(CONFIG_MSTAR_MIAMI) || \
      defined(CONFIG_MSTAR_MONACO)

    #define MS_MIU_INTERVAL 0x80000000

#elif defined(DEFINE_IN_MSTAR_CHIP_H)

#else
     #error unknown chip!!
#endif

#define AEON_REG_CTRL                           0x0FF0

    //---------------------------------------------
    // definition for AEON_REG_CTRL   //reg[0x0FF0]
    #define AEON_CTRL_EN                            BIT0
    #define AEON_CTRL_RST                           BIT1
    #define AEON_DWB_SWAP                           BIT3

#define REG_PCMCIA_PCM_MEM_IO_CMD           0x00
#define REG_PCMCIA_ADDR0                    0x02
#define REG_PCMCIA_ADDR1                    0x03
#define REG_PCMCIA_WRITE_DATA               0x04
#define REG_PCMCIA_FIRE_READ_DATA_CLEAR     0x06
#define REG_PCMCIA_READ_DATA                0x08
#define REG_PCMCIA_READ_DATA_DONE_BUS_IDLE  0x09
#define REG_PCMCIA_INT_MASK_CLEAR           0x0A
#define REG_PCMCIA_INT_MASK_CLEAR1          0x0B
#define REG_PCMCIA_STAT_INT_RAW_INT         0x0E
#define REG_PCMCIA_STAT_INT_RAW_INT1        0x0F
#define REG_PCMCIA_MODULE_VCC_OOB           0x10
#define PCMCIA_ATTRIBMEMORY_READ            0x03
#define PCMCIA_ATTRIBMEMORY_WRITE           0x04
#define PCMCIA_IO_READ                      0x05
#define PCMCIA_IO_WRITE                     0x06
#define REG_PCMCIA_BASE                     0x3440
#define PCMCIA_FIRE_COMMAND         BIT0
#define PCMCIA_CLEAN_STATE_RD_DONE  BIT1
#define PCMCIA_STATE_RD_DONE        BIT0
#define PCMCIA_STATE_BUS_IDLE       BIT1
#define PCMCIA_MAX_DETECT_COUNT     1
#define PCMCIA_MAX_POLLING_COUNT    20000
#define MAX_PCMCIA_CONFIGS              6       //!< The maximum number of configurations supported by a PCMCIA card
#define MAX_PCMCIA_STRLEN               (20)    //!< The maximum name of vendor/manufacturer/info strings
#define MAX_CIS_SIZE                    0x100   //!< The maximum size of a CIS, that is understood by this driver
#define PCMCIA_HW_TIMEOUT               3000
#define PCMCIA_HW_MAX_RETRY_COUNT       10      //  PCMCIA hardware register maximum access times
#define PCMCIA_IRQ_ENABLE               1       // FIXME_ALEC

#endif

void MHal_PCMCIA_SetPad(BOOL bRestore);
BOOL MHal_PCMCIA_WaitEMMCDone(U32 u32loopCnt);

#endif /* _HAL_SYSTEM_H_ */
