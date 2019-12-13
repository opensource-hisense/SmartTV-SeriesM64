////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mdrv_ldm_interrupt.c
/// @brief  MStar localdimming Interrupt Interface
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em>Define the interface which corespond to sysem's interrupt notification</em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (defined (CONFIG_HAS_LD) || defined (CONFIG_HAS_LD_MODULE))

#define _MDRV_LDM_INTERRUPT_C

//=============================================================================
// Include Files
//=============================================================================
#include <linux/interrupt.h>

#include "mdrv_mstypes.h"
#include "mdrv_ldm_interrupt.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================

//=============================================================================
// Debug Macros
//=============================================================================

//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Local Variables
//=============================================================================

//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================

//=============================================================================
// Local Function
//=============================================================================

extern MS_BOOL MDrv_LD_ProcessIRQ(void);
//-------------------------------------------------------------------------------------------------
/// Handle local dimming Interrupt notification handler
/// @param  irq                  \b IN: interrupt number
/// @param  devid                  \b IN: device id
/// @return IRQ_HANDLED
/// @attention
/// <b>[OBAMA] <em></em></b>
//-------------------------------------------------------------------------------------------------
irqreturn_t MDrv_LD_IntHandler(int irq,void *devid)
{
    MDrv_LD_ProcessIRQ();

    return IRQ_HANDLED;
}

#endif
