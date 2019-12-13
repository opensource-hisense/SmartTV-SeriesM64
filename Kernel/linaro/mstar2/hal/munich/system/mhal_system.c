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
/// file    drv_system_io.c
/// @brief  System Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/types.h>

#include "mdrv_types.h"
#include "mst_platform.h"
#include "mhal_system.h"
#if defined(CONFIG_MMC_MSTAR_MMC_EMMC) && CONFIG_MMC_MSTAR_MMC_EMMC
extern bool mstar_mci_exit_checkdone_ForCI(void);
#endif

// NOTE
// PE capability :
// PAD as output
//  PE = 0 / 1, don't care
// PAD as input
//  PE = 0, Hi-Z (input disable)
//  PE = 1, internal pull up
//
// for PAD = PCM_A(output), PE don't care
// for PAD = PCM_D(input/output), PE must be 1

#define PAD_PCM_D_ENABLE() \
    REG_ADDR(REG_CHIP_PCM_PE)   |= REG_CHIP_PCM_PE_MASK;

#define PAD_PCM_A_ENABLE() \
    REG_ADDR(REG_CHIP_PCM_PE1)   |= REG_CHIP_PCM_PE1_MASK;

#define PAD_PCM_D_DISABLE() \
    REG_ADDR(REG_CHIP_PCM_PE)   &= ~REG_CHIP_PCM_PE_MASK;

#define PAD_PCM_A_DISABLE() \
    REG_ADDR(REG_CHIP_PCM_PE1)   &= ~REG_CHIP_PCM_PE1_MASK;

static U16 u16TmpReg[8];

#if defined(CONFIG_MMC_MSTAR_MMC_EMMC) && CONFIG_MMC_MSTAR_MMC_EMMC
BOOL MHal_PCMCIA_WaitEMMCDone(U32 u32loopCnt)
{
    return mstar_mci_exit_checkdone_ForCI();
}
#endif

void MHal_PCMCIA_SetPad(BOOL bRestore)
{
    if(bRestore == FALSE)
    {
        // Switch to PCMCIA
        u16TmpReg[6] = REG_ADDR( REG_CHIP_NAND_MODE ) & REG_CHIP_NAND_MODE_MASK;
        REG_ADDR( REG_CHIP_NAND_MODE ) &= ~REG_CHIP_NAND_MODE_MASK;

        //u16Bypass = REG_ADDR(REG_CHIP_PCM_NAND_BYPASS) & REG_CHIP_PCM_NAND_BYPASS_MASK;
        u16TmpReg[0] = REG_ADDR(REG_CHIP_PCM_NAND_BYPASS) & REG_CHIP_PCM_NAND_BYPASS_MASK;
        REG_ADDR(REG_CHIP_PCM_NAND_BYPASS) |= REG_CHIP_PCM_NAND_BYPASS_ENABLE;

        //u16RegVal3 = REG_ADDR(REG_EMMC_CONFIG) & REG_EMMC_CONFIG_MASK;
        u16TmpReg[2] = REG_ADDR(REG_EMMC_CONFIG) & REG_EMMC_CONFIG_MASK;
        REG_ADDR(REG_EMMC_CONFIG) &= ~REG_EMMC_CONFIG_MASK;
        u16TmpReg[7] = REG_ADDR(REG_EMMC_CONFIG2) & REG_EMMC_CONFIG2_MASK;
        REG_ADDR(REG_EMMC_CONFIG2) &= ~REG_EMMC_CONFIG2_MASK;
        
        //u16RegVal1 = REG_ADDR(REG_CHIP_PCMCFG) & REG_CHIP_PCMCFG_MASK;
        u16TmpReg[3] = REG_ADDR(REG_CHIP_PCMCFG) & REG_CHIP_PCMCFG_MASK;
        REG_ADDR(REG_CHIP_PCMCFG) |= REG_CHIP_PCMCFG_CTRL_EN;

        //u16RegVal4 = REG_ADDR(REG_SD_CONFIG2) & REG_SD_CONFIG2_MASK;
        u16TmpReg[4] = REG_ADDR(REG_SD_CONFIG2) & REG_SD_CONFIG2_MASK;
        REG_ADDR(REG_SD_CONFIG2) &= ~REG_SD_CONFIG2_MASK;

        // disable PCM_A PE for NEOTION CAM issue (mantis id:0365320)
        PAD_PCM_A_DISABLE();
        PAD_PCM_D_ENABLE();
    }
    else
    {
        // restore padmux to original
        PAD_PCM_D_DISABLE();

        REG_ADDR(REG_EMMC_CONFIG) = (REG_ADDR(REG_EMMC_CONFIG) & ~REG_EMMC_CONFIG_MASK) | u16TmpReg[2];
        REG_ADDR(REG_EMMC_CONFIG2) = (REG_ADDR(REG_EMMC_CONFIG2) & ~REG_EMMC_CONFIG2_MASK) | u16TmpReg[7];
        REG_ADDR(REG_CHIP_PCMCFG) = (REG_ADDR(REG_CHIP_PCMCFG) & ~REG_CHIP_PCMCFG_MASK) | u16TmpReg[3];

        REG_ADDR(REG_SD_CONFIG2) = (REG_ADDR(REG_SD_CONFIG2) & ~REG_SD_CONFIG2_MASK) | u16TmpReg[4];

        REG_ADDR(REG_CHIP_PCM_NAND_BYPASS) = 
            (REG_ADDR(REG_CHIP_PCM_NAND_BYPASS) & ~(REG_CHIP_PCM_NAND_BYPASS_MASK)) | u16TmpReg[0];;

        // Switch to Nand
        REG_ADDR( REG_CHIP_NAND_MODE ) |= u16TmpReg[6]; //(BIT0|BIT2);

    }

}
