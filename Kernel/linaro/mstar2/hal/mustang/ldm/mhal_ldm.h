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
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_LD_H_
#define _HAL_LD_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "mdrv_types.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#ifdef _HALLD_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif


////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
// h01[0] reg_local_dimming_en
void MHal_LD_SetLocalDimmingEn(BOOL bEn);
BOOL MHal_LD_GetLocalDimmingEn(void);
// h03[4:0] reg_bl_width
void MHal_LD_SetLDFBacklightWidth(U8 u8Width);
U8 MHal_LD_GetLDFBacklightWidth(void);
// h03[9:5] reg_bl_height
void MHal_LD_SetLDFBacklightHeight(U8 u8Height);
U16 MHal_LD_GetLDFBacklightHeight(void);
// h04[25:0] reg_baseaddr0_l
void MHal_LD_SetLDFFrameBufBaseAddr_0(U32 u32Addr);
U32 MHal_LD_GetLDFFrameBufBaseAddr_0(void);
// h06[25:0] reg_baseaddr1_l
void MHal_LD_SetLDFFrameBufBaseAddr_1(U32 u32Addr);
U32 MHal_LD_GetLDFFrameBufBaseAddr_1(void);
// h04[26:0] reg_baseaddr0_l
U32 MHal_LD_GetLDFFrameBufBaseAddr_L0(void);
// h06[26:0] reg_baseaddr1_l
U32 MHal_LD_GetLDFFrameBufBaseAddr_L1(void);
// h08[26:0] reg_baseaddr0_r
U32 MHal_LD_GetLDFFrameBufBaseAddr_R0(void);
// h0a[26:0] reg_baseaddr1_r
U32 MHal_LD_GetLDFFrameBufBaseAddr_R1(void);
// h19[19:0] reg_ratio_h_in
void MHal_LD_SetInputHScalingDownRatio(U32 u32Ratio);
U32 MHal_LD_GetInputHScalingDownRatio(void);
// h1B[19:0] reg_ratio_v_in
void MHal_LD_SetInputVScalingDownRatio(U32 u32Ratio);
U32 MHal_LD_GetInputVScalingDownRatio(void);
// h1d[7:0] reg_pel_hlpf_thrd
void MHal_LD_SetLDFHLPFThreshold(U8 u8Th);
U8 MHal_LD_GetLDFHLPFThreshold(void);
// h1d[12] reg_pel_hlpf_en
void MHal_LD_SetLDFHLPFEn(BOOL bEn);
BOOL MHal_LD_GetLDFHLPFEn(void);
// h1d[13] reg_pel_hlpf_step
void MHal_LD_SetLDFHLPFStep(BOOL bEn);
BOOL MHal_LD_GetLDFHLPFStep(void);
// h1f[3:0] reg_alpha
void MHal_LD_SetLDFDCMaxAlpha(U8 u8Alpha);
U8 MHal_LD_GetLDFDCMaxAlpha(void);
// h1f[7] reg_pel_white_mod_en
void MHal_LD_SetLDFPixelWhiteModeEn(BOOL bEn);
BOOL MHal_LD_GetLDFPixelWhiteModeEn(void);
// h1f[8] reg_write_dc_max_en
void MHal_LD_SetLDFWriteDCMaxEn(BOOL bEn);
BOOL MHal_LD_GetLDFWriteDCMaxEn(void);;
// h1f[9] reg_write_dc_max_of_en
void MHal_LD_SetLDFWriteDCMaxOFEn(BOOL bEn);
BOOL MHal_LD_GetLDFWriteDCMaxOFEn(void);
// h22[11:0] reg_dummy0
void MHal_LD_SetSWPulseId(U16 u16Id);
U16 MHal_LD_GetSWPulseId(void);
// h22[15:12] reg_dummy0
void MHal_LD_SetSWPulseMode(U8 u8Mode);
U8 MHal_LD_GetSWPulseMode(void);
// h23[7:0] reg_dummy1
void MHal_LD_SetSWPulseLEDIntensity(U8 u8Intensity);
U8 MHal_LD_GetSWPulseLEDIntensity(void);
// h23[15:8] reg_dummy1
void MHal_LD_SetSWPulseLDBIntensity(U8 u8Intensity);
U8 MHal_LD_GetSWPulseLDBIntensity(void);
// h24[7:0] reg_dummy2
void MHal_LD_SetSWSpatialFilterStrength(U8 u8Str);
U8 MHal_LD_GetSWSpatialFilterStrength(void);
// h24[15:8] reg_dummy2
void MHal_LD_SetSWTemporalFilterStrengthDn(U8 u8Str);
U8 MHal_LD_GetSWTemporalFilterStrengthDn(void);
// h25[7:0] reg_dummy3
void MHal_LD_SetSWLocalDimmingStrength(U8 u8Str);
U8 MHal_LD_GetSWLocalDimmingStrength(void);
// h25[15:8] reg_dummy3
void MHal_LD_SetSWGlobalDimmingStrength(U8 u8Str);
U8 MHal_LD_GetSWGlobalDimmingStrength(void);
//  h28[7:0] reg_dummy2
void MHal_LD_SetSWMaxThreshold(U8 u8Th);
U8 MHal_LD_GetSWMaxThreshold(void);
// h30[19:0] reg_ratio_h_out
void MHal_LD_SetOutputHScalingUpRatio(U32 u32Ratio);
U32 MHal_LD_GetOutputHScalingUpRatio(void);
// h32[19:0] reg_ratio_v_out
void MHal_LD_SetOutputVScalingUpRatio(U32 u32Ratio);
U32 MHal_LD_GetOutputVScalingUpRatio(void);
// h33[15:0] reg_ratio_h_lsf_cbmode
void MHal_LD_SetLSFCBHRatio(U16 u16Ratio);
U16 MHal_LD_GetLSFCBHRatio(void);
// h34[15:0] reg_ratio_v_lsf_cbmode
void MHal_LD_SetLSFCBVRatio(U16 u16Ratio);
U16 MHal_LD_GetLSFCBVRatio(void);
// h37[0] reg_edge2d_gain_en
void MHal_LD_SetEdge2DGainEn(BOOL bEn);
BOOL MHal_LD_GetEdge2DGainEn(void);
// h37[1] reg_edge2d_en
void MHal_LD_SetEdge2DEn(BOOL bEn);
BOOL MHal_LD_GetEdge2DEn(void);
// h37[2] reg_edge2d_direct_type_en
void MHal_LD_SetEdge2DDirectTypeEn(BOOL bEn);
BOOL MHal_LD_GetEdge2DDirectTypeEn(void);
// h37[8] reg_new_edge2d_en
void MHal_LD_SetEdge2DLocalTypeEn(BOOL bEn);
BOOL MHal_LD_GetEdge2DLocalTypeEn(void);
// h38[19:0] reg_lsf_h_init
void MHal_LD_SetLSFHInitialPhase(U32 u32Phase);
U32 MHal_LD_GetLSFHInitialPhase(void);
// h3a[19:0] reg_lsf_v_init
void MHal_LD_SetLSFVInitialPhase(U32 u32Phase);
U32 MHal_LD_GetLSFVInitialPhase(void);
// h3c[0] reg_lsf_h_shift
void MHal_LD_SetLSFHBoundary(BOOL bEn);
BOOL MHal_LD_GetLSFHBoundary(void);
// h3c[1] reg_lsf_v_shift
void MHal_LD_SetLSFVBoundary(BOOL bEn);
BOOL MHal_LD_GetLSFVBoundary(void);
// h3c[3:2] reg_edge_level_shift
void MHal_LD_SetEdgeLevelShift(U8 u8Mode);
U8 MHal_LD_GetEdgeLevelShift(void);
// h3c[8:4] reg_cmp_blend
void MHal_LD_SetCompensationBlend(U8 u8Alpha);
U8 MHal_LD_GetCompensationBlend(void);
// h3c[10] reg_bld_dither_en
void MHal_LD_SetBlendingDitherEn(BOOL bEn);
BOOL MHal_LD_GetBlendingDitherEn(void);
// h3c[13] reg_cmp_dither_en
void MHal_LD_SetCompensationDitherEn(BOOL bEn);
BOOL MHal_LD_GetCompensationDitherEn(void);
// h3d[11:0] reg_comp_lut
void MHal_LD_WriteCompensationLUT(U16 u16Lut);
// h3e[11:0] reg_r_comp_lut (readonly)
U16 MHal_LD_ReadCompensationLUT(void);
// h3f[7:0] reg_comp_addr
void MHal_LD_SetCompensationLUTAddress(U8 u8Addr);
U8 MHal_LD_GetCompensationLUTAddress(void);
// h3f[10] reg_comp_wd
void MHal_LD_SetCompensationWriteEn(BOOL bEn);
BOOL MHal_LD_GetCompensationWriteEn(void);
// h3f[11] reg_comp_rd
void MHal_LD_SetCompensationReadEn(BOOL bEn);
BOOL MHal_LD_GetCompensationReadEn(void);
// h3f[12] reg_comp_mod, 1:incremental
void MHal_LD_SetCompensationMode(U8 bMode);
BOOL MHal_LD_GetCompensationMode(void);
// h40[4] reg_read_addr_idx (readonly)
BOOL MHal_LD_GetCurFrameIdx(void);
// h41[7:0] reg_dummy7
void MHal_LD_SetLEDBacklightWidth(U8 u8Width);
U8 MHal_LD_GetLEDBacklightWidth(void);
// h41[15:8] reg_dummy7
void MHal_LD_SetLEDBacklightHeight(U8 u8Height);
U8 MHal_LD_GetLEDBacklightHeight(void);
// h43[15:8] reg_dummy9
void MHal_LD_SetSWTemporalFilterStrengthUp(U8 u8Str);
U8 MHal_LD_GetSWTemporalFilterStrengthUp(void);
// h44[3] reg_dummy10
void MHal_LD_SetSWWriteSPIEn(BOOL bEn);
BOOL MHal_LD_GetSWWriteSPIEn(void);
// h44[7] reg_dummy10
void MHal_LD_SetSWWriteLDBEn(BOOL bEn);
BOOL MHal_LD_GetSWWriteLDBEn(void);
// h44[12] reg_dummy10
void MHal_LD_SetSWAlgorithmEn(BOOL bEn);
BOOL MHal_LD_GetSWAlgorithmEn(void);
// h44[13] reg_dummy10
void MHal_LD_SetSWReadLDFEn(BOOL bEn);
BOOL MHal_LD_GetSWReadLDFEn(void);
// h44[15:14] reg_dummy10
void MHal_LD_SetLEDType(U8 eType);
U8 MHal_LD_GetLEDType(void);
// h46[6] reg_comp_en
void MHal_LD_SetCompensationEn(BOOL bEn);
BOOL MHal_LD_GetCompensationEn(void);
// h4c[6:0] reg_bl_width
void MHal_LD_SetLSFBacklightWidth(U8 u8Width);
U8 MHal_LD_GetLSFBacklightWidth(void);
// h4c[14:8] reg_bl_height
void MHal_LD_SetLSFBacklightHeight(U8 u8Height);
U8 MHal_LD_GetLSFBacklightHeight(void);
// h4D[25:0] reg_baseaddr0_l
void MHal_LD_SetLDBFrameBufBaseAddr_0(U32 u32Addr);
U32 MHal_LD_GetLDBFrameBufBaseAddr_0(void);
// h4F[25:0] reg_baseaddr1_l
void MHal_LD_SetLDBFrameBufBaseAddr_1(U32 u32Addr);
U32 MHal_LD_GetLDBFrameBufBaseAddr_1(void);
// h4D[26:0] reg_baseaddr0_l
U32 MHal_LD_GetLDBFrameBufBaseAddr_L0(void);
// h4F[26:0] reg_baseaddr1_l
U32 MHal_LD_GetLDBFrameBufBaseAddr_L1(void);
// h51[26:0] reg_baseaddr0_r
U32 MHal_LD_GetLDBFrameBufBaseAddr_R0(void);
// h53[26:0] reg_baseaddr1_r
U32 MHal_LD_GetLDBFrameBufBaseAddr_R1(void);
// h57[7:0] reg_dummy5
void MHal_LD_SetSWTemporalFilterLowTh(U8 u8Th);
U8 MHal_LD_GetSWTemporalFilterLowTh(void);
// h57[15:8] reg_dummy5
void MHal_LD_SetSWTemporalFilterHighTh(U8 u8Th);
U8 MHal_LD_GetSWTemporalFilterHighTh(void);
// h58[7:0] reg_dummy6
void MHal_LD_SetSWSpatialFilterStrength2(U8 u8Dist);
U8 MHal_LD_GetSWSpatialFilterStrength2(void);
// h58[15:8] reg_dummy6
void MHal_LD_SetSWSpatialFilterStrength3(U8 u8Dist);
U8 MHal_LD_GetSWSpatialFilterStrength3(void);
// 
void MHal_LD_SetSWSpatialFilterStrength4(U8 u8Dist);
U8 MHal_LD_GetSWSpatialFilterStrength4(void);
// 
void MHal_LD_SetSWSpatialFilterStrength5(U8 u8Dist);
U8 MHal_LD_GetSWSpatialFilterStrength5(void);
// h63[4:0] reg_bl_width_led
void MHal_LD_SetLDBBacklightWidth(U8 u8Width);
U8 MHal_LD_GetLDBBacklightWidth(void);
// h63[9:5] reg_bl_height_led
void MHal_LD_SetLDBBacklightHeight(U8 u8Height);
U8 MHal_LD_GetLDBBacklightHeight(void);
// h70[12:0] reg_frm_width
void MHal_LD_SetFrameWidth(U16 u16Width);
U16 MHal_LD_GetFrameWidth(void);
// h71[11:0] reg_frm_height
void MHal_LD_SetFrameHeight(U16 u16Height);
U16 MHal_LD_GetFrameHeight(void);
// h72[3:1] reg_lsf_out_mod
void MHal_LD_SetLSFOutMode(U8 u8Mode);
U8 MHal_LD_GetLSFOutMode(void);
// h76[7:0] reg_dummy0
void MHal_LD_SetSWMinClampValue(U8 u8Val);
U8 MHal_LD_GetSWMinClampValue(void);
// h76[11:8] reg_dummy0
void MHal_LD_SetSWBacklightInGamma(U8 u8Type);
U8 MHal_LD_GetSWBacklightInGamma(void);
// h76[15:12] reg_dummy0
void MHal_LD_SetSWBacklightOutGamma(U8 u8Type);
U8 MHal_LD_GetSWBacklightOutGamma(void);
// h79[26:0] reg_edge_2d_baseaddr
void MHal_LD_SetEdge2DBufBaseAddr(U32 u32Addr);
U32 MHal_LD_GetEdge2DBufBaseAddr(void);
BOOL MHal_LD_GetLinearEn(void);
U16 MHal_LD_GetTempFilterMaxSpeed(void);

#undef INTERFACE

#endif  //_HAL_LD_H_

