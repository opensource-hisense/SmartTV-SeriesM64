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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file    mdrv_ldm_init.h
/// @brief  local dimming Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LD_INIT_H_
#define _MDRV_LD_INIT_H_


/// Define PNL local dimming return type
typedef enum
{
    E_LD_STATUS_SUCCESS = 0,
    E_LD_STATUS_FAIL = 1,
    E_LD_STATUS_NOT_SUPPORTED,
    E_LD_STATUS_PARAMETER_ERROR,
    E_LD_OBTAIN_MUTEX_FAIL,            ///< obtain mutex timeout when calling this function
    E_LD_OBTAIN_MMIO_FAIL,            ///< obtain mutex timeout when calling this function
} EN_LD_RETURN;


typedef struct
{
    MS_U16 u16PanelWidth;
    MS_U16 u16PanelHeight;
    MS_U16 u16PackLength;
    phys_addr_t u32BaseAddr;
}ST_DRV_LD_INFO;


typedef struct
{
    EN_LD_TYPE enLEDType; // 0/1/2/3

    // PANEL
    MS_U8 u8LDFWidth, u8LDFHeight; // Statistic width & height  MAX=32*32
    MS_U8 u8LEDWidth, u8LEDHeight; // LED width & height  MAX=W*H=64 if Edge2d enable, MAX=128*72 otherwise
    MS_U8 u8LSFWidth, u8LSFHeight; // LSF width & height  MAX=128*72
    MS_BOOL bCompensationEn; // Compensation enable
    const MS_U8* pu8CompTable; // Compensation table  SIZE=256x2
    MS_BOOL bEdge2DEn; // Edge2D enable
    const MS_U8* pu8Edge2DTable; // Edge2D table  SIZE=(u8LEDWidth*u8LEDHeight)*(u8LDFWidth*u8LDFHeight)

    // PQ HW
    MS_U32 u32LsfInitialPhase_H, u32LsfInitialPhase_V;
    MS_U8 u8DCMaxAlpha; // statistics : higher -> prefer max, lower -> prefer average
    MS_U8 u8CompenGain; // pixel compensation tuner : higher -> prefer compensated pixel, lower -> prefer original pixel

    // PQ SW
    MS_U8 u8GDStrength; // global dimming strength
    MS_U8 u8LDStrength; // local dimming strength
    MS_U8 u8TFStrengthUp; // temporal filter strength up
    MS_U8 u8TFStrengthDn; // temporal filter strength down
    MS_U8 u8TFLowThreshold; // temporal filter low threshold
    MS_U8 u8TFHightThreshold; // temporal filter high threshold
    MS_U8 u8SFStrength; // spatial filter strength
    MS_U8 u8SFStrength2; // spatial filter strength 2
    MS_U8 u8SFStrength3; // spatial filter strength 3
    MS_U8 u8SFStrength4; // spatial filter strength 4
    MS_U8 u8SFStrength5; // spatial filter strength 5
    MS_U8 u8InGamma; // gamma at SW begin
    MS_U8 u8OutGamma; // gamma at SW end
    MS_U8 u8BLMinValue; // min clamp value
    MS_U8 u8NRStrength; // de-noise strength
}ST_DRV_LD_PQ_INFO;

#endif

