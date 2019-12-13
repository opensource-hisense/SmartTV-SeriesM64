////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

#ifndef _REG_PM_H_
#define _REG_PM_H_

#include <mstar/mstar_chip.h>
#include "mdrv_types.h" 


#if defined(CONFIG_ARM) || defined(CONFIG_MIPS) 
#define PM_RIU_REG_BASE                 0xFD000000 
#elif defined(CONFIG_ARM64) 
extern ptrdiff_t mstar_pm_base;
#define PM_RIU_REG_BASE                 mstar_pm_base 
#endif

#define REG_PM_SLEEP_BASE               (0x0e00UL) 
#define REG_PM_MISC_BASE                (0x2e00UL) 
#define REG_MCU_BASE                    (0x1000UL)
#define REG_MCU_CACHE_BASE              (0x2b00UL)

//------------------------------------------------------------------------------ 
// pm sleep Reg 
//------------------------------------------------------------------------------
#define REG_PM_MCU_CLK                  (REG_PM_SLEEP_BASE + (0x20UL << 1))
#define REG_PM_LOCK                     (REG_PM_SLEEP_BASE + (0x12UL << 1))



//------------------------------------------------------------------------------ 
// pm misc Reg 
//------------------------------------------------------------------------------
#define REG_PM_CPU_SW_RST               (REG_PM_MISC_BASE + (0x29UL << 1))
#define REG_PM_RST_CPU0_PASSWORD        (REG_PM_MISC_BASE + (0x2aUL << 1))


//------------------------------------------------------------------------------ 
// mcu Reg 
//------------------------------------------------------------------------------
#define REG_MCU_SRAM_START_ADDR_H        (REG_MCU_BASE + (0x00UL << 1))
#define REG_MCU_SRAM_END_ADDR_H          (REG_MCU_BASE + (0x01UL << 1))
#define REG_MCU_SRAM_START_ADDR_L        (REG_MCU_BASE + (0x02UL << 1))
#define REG_MCU_SRAM_END_ADDR_L          (REG_MCU_BASE + (0x03UL << 1))

#define REG_MCU_CONFIG                   (REG_MCU_BASE + (0x0cUL << 1))

//------------------------------------------------------------------------------ 
// 8051 cache Reg 
//------------------------------------------------------------------------------
#define REG_MCU_CACHE_CONFIG             (REG_MCU_BASE + (0x50UL << 1))

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


#endif // _REG_PM_H_

