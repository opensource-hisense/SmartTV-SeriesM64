//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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
/// file    regMIU.h
/// @brief  MIU Control Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_MIU_H_
#define _REG_MIU_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define BITS_RANGE(range)                       (BIT(((1)?range)+1) - BIT((0)?range))
#define BITS_RANGE_VAL(x, range)                ((x & BITS_RANGE(range)) >> ((0)?range))

#define MIU_REG_BASE                            (0x1200UL)
#define MIU1_REG_BASE                           (0x0600UL)
#define MIU2_REG_BASE                           (0x62000UL)
#define PM_REG_BASE                             (0x1E00UL)
#define MIU_ATOP_BASE                           (0x10D00UL)
#define MIU1_ATOP_BASE                          (0x61600UL)
#define MIU2_ATOP_BASE                          (0x62100UL)
#define CHIP_TOP_BASE                           (0x1E00UL)
#define MIU_ARB_REG_BASE                        (0x61500UL)
#define MIU1_ARB_REG_BASE                       (0x62200UL)
#define MIU2_ARB_REG_BASE                       (0x62300UL)

#define PM_CHIP_REVISION                        (PM_REG_BASE+0x03UL) // 0x1E03
#define DDR_FREQ_SET_0                          (MIU_REG_BASE+0x20UL) // 0x1220
#define DDR_FREQ_SET_1                          (MIU_REG_BASE+0x21UL) //0x1221
#define DDR_FREQ_DIV_1                          (MIU_REG_BASE+0x25UL) //0x1225
#define DDR_FREQ_INPUT_DIV_2                    (MIU_REG_BASE+0x26UL) //0x1226
#define DDR_FREQ_LOOP_DIV_2                     (MIU_REG_BASE+0x27UL) //0x1227
#define DDR_CLK_SELECT                          (MIU_REG_BASE+0x3eUL) //0x123E
#define DDR_FREQ_STATUS                         (MIU_REG_BASE+0x3fUL) //0x123F

#define MIU_RQ0L_MASK                           (MIU_REG_BASE+0x46UL)
#define MIU_RQ0H_MASK                           (MIU_REG_BASE+0x47UL)
#define MIU_RQ1L_MASK                           (MIU_REG_BASE+0x66UL)
#define MIU_RQ1H_MASK                           (MIU_REG_BASE+0x67UL)
#define MIU_RQ2L_MASK                           (MIU_REG_BASE+0x86UL)
#define MIU_RQ2H_MASK                           (MIU_REG_BASE+0x87UL)
#define MIU_RQX_MASK(Reg, Group)                (Reg = (Group < 4 )?  (MIU_REG_BASE + 0x46UL + 0x20UL*Group) : (MIU_ARB_REG_BASE + 0x06UL + 0x20UL * (Group - 4)))
#define MIU_RQX_HPMASK(Reg, Group)              (Reg = (Group < 4 )?  (MIU_REG_BASE + 0x48UL + 0x20UL*Group) : (MIU_ARB_REG_BASE + 0x08UL + 0x20UL * (Group - 4)))

#define MIU_PROTECT_EN_INTERNAL                 (MIU_REG_BASE+0xD2UL)
#define MIU_PROTECT_DDR_SIZE                    (MIU_REG_BASE+0xD3UL)
#define MIU_PROTECT_DDR_SIZE_MASK               BITS_RANGE(11:8)
#define MIU_PROTECT_DDR_32MB                    (0x50UL)
#define MIU_PROTECT_DDR_64MB                    (0x60UL)
#define MIU_PROTECT_DDR_128MB                   (0x70UL)
#define MIU_PROTECT_DDR_256MB                   (0x80UL)
#define MIU_PROTECT_DDR_512MB                   (0x90UL)
#define MIU_PROTECT_DDR_1024MB                  (0xA0UL)
#define MIU_PROTECT_DDR_2048MB                  (0xB0UL)


#define MIU_PROTECT0_ID0                        (MIU_REG_BASE+0x2EUL)
#define MIU_BW_REQUEST                          (MIU_REG_BASE+0x1AUL)
#define MIU_BW_RESULT                           (MIU_REG_BASE+0x1CUL)
#define MIU_PROTECT0_ID_ENABLE                  (MIU_REG_BASE+0x20UL)
#define MIU_PROTECT1_ID_ENABLE                  (MIU_REG_BASE+0x22UL)
#define MIU_PROTECT2_ID_ENABLE                  (MIU_REG_BASE+0x24UL)
#define MIU_PROTECT3_ID_ENABLE                  (MIU_REG_BASE+0x26UL)
#define MIU_PROTECT0_MSB                        (MIU_REG_BASE+0xD0UL)
#define MIU_PROTECT0_START                      (MIU_REG_BASE+0xC0UL)
#define MIU_PROTECT1_START                      (MIU_REG_BASE+0xC4UL)
#define MIU_PROTECT2_START                      (MIU_REG_BASE+0xC8UL)
#define MIU_PROTECT3_START                      (MIU_REG_BASE+0xCCUL)
#define REG_MIU_PROTECT_LOADDR                  (0x6DUL << 1)	//0xDE
#define REG_MIU_PROTECT_HIADDR                  (0x6EUL << 1)	//0xDE
#define REG_MIU_GROUP_PRIORITY                  (0x7FUL << 1)
#define REG_MIU_PROTECT_STATUS                  (0x6FUL << 1)	//0xDE

// MIU selection registers
#define REG_MIU_SEL0                            (MIU_REG_BASE+0xf0UL)  //0x12F0
#define REG_MIU_SEL1                            (MIU_REG_BASE+0xf2UL)  //0x12F1
#define REG_MIU_SEL2                            (MIU_REG_BASE+0xf4UL)  //0x12F2
#define REG_MIU_SEL3                            (MIU_REG_BASE+0xf6UL)  //0x12F3
#define REG_MIU_SELX(x)                         (0xF0UL+x*2)

//MIU1
#define MIU1_PROTECT_EN                          (MIU1_REG_BASE+0xD2UL)
#define MIU1_PROTECT_DDR_SIZE                    (MIU1_REG_BASE+0xD3UL)
#define MIU1_PROTECT0_ID0                        (MIU1_REG_BASE+0x2EUL)
#define MIU1_BW_REQUEST                          (MIU1_REG_BASE+0x1AUL)
#define MIU1_BW_RESULT                           (MIU1_REG_BASE+0x1CUL)
#define MIU1_PROTECT0_ID_ENABLE                  (MIU1_REG_BASE+0x20UL)
#define MIU1_PROTECT1_ID_ENABLE                  (MIU1_REG_BASE+0x22UL)
#define MIU1_PROTECT2_ID_ENABLE                  (MIU1_REG_BASE+0x24UL)
#define MIU1_PROTECT3_ID_ENABLE                  (MIU1_REG_BASE+0x26UL)
#define MIU1_PROTECT0_MSB                        (MIU1_REG_BASE+0xD0UL)
#define MIU1_PROTECT0_START                      (MIU1_REG_BASE+0xC0UL)
#define MIU1_PROTECT1_START                      (MIU1_REG_BASE+0xC4UL)
#define MIU1_PROTECT2_START                      (MIU1_REG_BASE+0xC8UL)
#define MIU1_PROTECT3_START                      (MIU1_REG_BASE+0xCCUL)
#define MIU1_RQX_MASK(Reg, Group)                (Reg = (Group < 4 )?  (MIU1_REG_BASE + 0x46UL + 0x20UL*Group) : (MIU1_ARB_REG_BASE + 0x06UL + 0x20UL * (Group - 4)))
#define MIU1_RQX_HPMASK(Reg, Group)              (Reg = (Group < 4 )?  (MIU1_REG_BASE + 0x48UL + 0x20UL*Group) : (MIU1_ARB_REG_BASE + 0x08UL + 0x20UL * (Group - 4)))

//MIU2
#define MIU2_PROTECT_EN                          (MIU2_REG_BASE+0xD2)
#define MIU2_PROTECT_DDR_SIZE                    (MIU2_REG_BASE+0xD3)
#define MIU2_PROTECT0_ID0                        (MIU2_REG_BASE+0x2E)
#define MIU2_BW_REQUEST                          (MIU2_REG_BASE+0x1A)
#define MIU2_BW_RESULT                           (MIU2_REG_BASE+0x1C)
#define MIU2_PROTECT0_ID_ENABLE                  (MIU2_REG_BASE+0x20)
#define MIU2_PROTECT1_ID_ENABLE                  (MIU2_REG_BASE+0x22)
#define MIU2_PROTECT2_ID_ENABLE                  (MIU2_REG_BASE+0x24)
#define MIU2_PROTECT3_ID_ENABLE                  (MIU2_REG_BASE+0x26)
#define MIU2_PROTECT0_MSB                        (MIU2_REG_BASE+0xD0)
#define MIU2_PROTECT0_START                      (MIU2_REG_BASE+0xC0)
#define MIU2_PROTECT1_START                      (MIU2_REG_BASE+0xC4)
#define MIU2_PROTECT2_START                      (MIU2_REG_BASE+0xC8)
#define MIU2_PROTECT3_START                      (MIU2_REG_BASE+0xCC)
#define MIU2_RQX_MASK(Reg, Group)                (Reg = (Group < 4 )?  (MIU2_REG_BASE + 0x46UL + 0x20UL*Group) : (MIU2_ARB_REG_BASE + 0x06UL + 0x20UL * (Group - 4)))
#define MIU2_RQX_HPMASK(Reg, Group)              (Reg = (Group < 4 )?  (MIU2_REG_BASE + 0x48UL + 0x20UL*Group) : (MIU2_ARB_REG_BASE + 0x08UL + 0x20UL * (Group - 4)))


#define REG_MIU_I64_MODE                         (BIT7)
#define REG_MIU_INIT_DONE                        (BIT15)

//Protection Status
#define REG_MIU_PROTECT_LOG_CLR                 (BIT0)
#define REG_MIU_PROTECT_IRQ_MASK                (BIT1)
#define REG_MIU_PROTECT_HIT_FALG                (BIT4)
#define REG_MIU_PROTECT_HIT_ID                  14:8
#define REG_MIU_PROTECT_HIT_NO                  7:5

// MIU Scramble
#define REG_MIU_SCRAMBLE_EN                     (MIU_REG_BASE+0x06UL)

//MIU Bus Width
#define REG_MI64_FORCE                          (CHIP_TOP_BASE+0x40UL) 

//-------------------------------------------------------------------------------------------------
//MAU
//
//-------------------------------------------------------------------------------------------------
#define    RIUBASE_MAU0                           0x1840UL
#define    RIUBASE_MAU1                           0x1860UL


//-------------------------------------------------------------------------------------------------
//  MIU ATOP registers
//-------------------------------------------------------------------------------------------------
#define MIU_DDFSTEP                             (0x28UL)//0x110D28
#define MIU_SSC_EN                              (0x29UL)//0x110D29
#define MIU_DDFSPAN                             (0x2AUL)//0x110D2A
#define MIU_DDFSET                              (0x30UL)
//#define MIU_PLL_INPUT_DIV_2ND                 (0x34UL) // no this reg in Einstein
#define MIU_PLL_LOOP_DIV_2ND                    (0x34UL)
//xxx_div_first
#define MIU_PLLCTRL                             (0x36UL)
#define MIU_DDRPLL_DIV_FIRST                    (0x37UL)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_MIU_H_

