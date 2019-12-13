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
/// file    mdrv_ldm_init.c
/// @brief  local dimming Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#if (defined (CONFIG_HAS_LD) || defined (CONFIG_HAS_LD_MODULE))

#include <linux/module.h>
#include <linux/types.h>
#include <asm/io.h>
#include <mstar/mstar_chip.h>
#include "mst_devid.h"
#include <linux/slab.h>

#include "mdrv_mstypes.h"
#include "mdrv_ldm_io.h"
#include "mdrv_ldm_common.h"
#include "mdrv_ldm_interface.h"
#include "mdrv_ldm_init.h"
#include "mdrv_ldm_parse.h"
#include "mhal_ldm.h"
#include "reg_ldm.h"





#ifndef UNUSED
#define UNUSED(x)  ((x) = (x))
#endif


#define GETMIUOFFSET(addr, Offset) \
    if(0x80000000 > (addr))     \
    {   \
        Offset = MSTAR_MIU0_BUS_BASE; \
    }   \
    else if(0xC0000000 > (addr)) \
    {   \
        Offset = MSTAR_MIU1_BUS_BASE; \
    }
    
/*
#define LD_INI_FILE                 "/config/ldm/ldm.ini"
#define LD_INI_FILE_U               "/mnt/usb/sda1/ldm/ldm.ini"
#define LD_COMPENSATION_BIN_FILE    "/config/ldm/Compensation.bin"
#define LD_COMPENSATION_BIN_FILE_U  "/mnt/usb/sda1/ldm/Compensation.bin"
#define LD_EDGE2D_BIN_FILE          "/config/ldm/Edge2D.bin"
#define LD_EDGE2D_BIN_FILE_U        "/mnt/usb/sda1/ldm/Edge2D.bin"
*/
#define LD_CUS_FILE                 "/config/ldm/"
#define LD_CUS_FILE_U               "/mnt/usb/sda1/ldm/"

#define LD_BIN_LENGTH    0x80000
#define LD_BIN_OFFSET    0x20
#define LDMMAPLENGTH    0x100000        //
#define LDFALIGN        8        //
#define RATIOSHIFT      16        //
#define LDFSHIFT        20        //




ST_DRV_LD_INFO* psDrvLdInfo = NULL;
ST_DRV_LD_PQ_INFO* psDrvLdPQInfo = NULL;

ST_DRV_LD_INFO stDrvLdInfo;
ST_DRV_LD_PQ_INFO stDrvLdPQInfo;


void __iomem * u64Vaddr = 0;  
phys_addr_t ld_addr_base;



static MS_U16 MDrv_LD_PreInitVariable(void)
{
    MS_BOOL bRet = FALSE;
    memset((void *)&stDrvLdInfo,0,sizeof(ST_DRV_LD_INFO));
    memset((void *)&stDrvLdPQInfo,0,sizeof(ST_DRV_LD_PQ_INFO));

    if((0 ==strlen(stCusPath.aCusPath)) || (0 ==strlen(stCusPath.aCusPathU)))
    {
        LD_INFO(" trace! using default ini and bin path \n");
        strncat(stCusPath.aCusPath,LD_CUS_FILE,12);
        strncat(stCusPath.aCusPathU,LD_CUS_FILE_U,18);
    }
	strncat(stCusPath.aCusPath,"ldm.ini",7);
	strncat(stCusPath.aCusPathU,"ldm.ini",7);
    bRet = MDrv_LD_ParseIni(stCusPath.aCusPath, stCusPath.aCusPathU);
    if(FALSE == bRet)
    {
        return E_LD_STATUS_PARAMETER_ERROR;
    }
    
    psDrvLdInfo = &stDrvLdInfo;
    psDrvLdPQInfo = (ST_DRV_LD_PQ_INFO*)&stDrvLdPQInfo;

    return E_LD_STATUS_SUCCESS;
}


MS_U16 MDrv_LD_InitLDF(void)
{
    MS_U32 LDF_mem_size = (((psDrvLdPQInfo->u8LDFWidth*4 - 1) / psDrvLdInfo->u16PackLength) + 1) * psDrvLdInfo->u16PackLength * psDrvLdPQInfo->u8LDFHeight;
    //MHal_LD_SetLocalDimmingEn(TRUE);
    MHal_LD_SetLDFBacklightWidth(psDrvLdPQInfo->u8LDFWidth);
    MHal_LD_SetLDFBacklightHeight(psDrvLdPQInfo->u8LDFHeight);
    
    MHal_LD_SetInputHScalingDownRatio(((MS_U32)psDrvLdPQInfo->u8LDFWidth << LDFSHIFT)/psDrvLdInfo->u16PanelWidth + 1); // W_ledx1024x1024/W_lcd + 1
    MHal_LD_SetInputVScalingDownRatio(((MS_U32)psDrvLdPQInfo->u8LDFHeight << LDFSHIFT)/psDrvLdInfo->u16PanelHeight + 1); // H_ledx1024x1024/H_lcd + 1

    MHal_LD_SetLDFFrameBufBaseAddr_0((psDrvLdInfo->u32BaseAddr) / psDrvLdInfo->u16PackLength);
    LDF_mem_size = ((LDF_mem_size + 0xFF) >> LDFALIGN) << LDFALIGN; // align at 0x100
    MHal_LD_SetLDFFrameBufBaseAddr_1((psDrvLdInfo->u32BaseAddr + LDF_mem_size) / psDrvLdInfo->u16PackLength);

    MHal_LD_SetLDFHLPFEn(TRUE);
    MHal_LD_SetLDFHLPFThreshold(psDrvLdPQInfo->u8NRStrength);
    MHal_LD_SetLDFHLPFStep(0);

    MHal_LD_SetLDFDCMaxAlpha(psDrvLdPQInfo->u8DCMaxAlpha);
    MHal_LD_SetLDFWriteDCMaxEn(DISABLE);
    MHal_LD_SetLDFWriteDCMaxOFEn(ENABLE);
    MHal_LD_SetLDFPixelWhiteModeEn(ENABLE);
    LD_INFO(" trace! MDrv_LD_InitLDF, u8LDFWidth:0x%x, u8LDFHeight:0x%x, u32BaseAddr:0x%x \n",
        psDrvLdPQInfo->u8LDFWidth, psDrvLdPQInfo->u8LDFHeight, (MS_U32)(psDrvLdInfo->u32BaseAddr));

    return E_LD_STATUS_SUCCESS;
}

MS_U16 MDrv_LD_InitLDB(void)
{
    MS_U16 u16HRatio = (MS_U16)MIN((((MS_U32)psDrvLdPQInfo->u8LEDWidth)<<RATIOSHIFT) / psDrvLdPQInfo->u8LSFWidth, 0xFFFF);
    MS_U16 u16VRatio = (MS_U16)MIN((((MS_U32)psDrvLdPQInfo->u8LEDHeight)<<RATIOSHIFT) / psDrvLdPQInfo->u8LSFHeight, 0xFFFF);
    MS_U32 LDF_mem_size = (((psDrvLdPQInfo->u8LDFWidth*4 - 1) / psDrvLdInfo->u16PackLength) + 1) * psDrvLdInfo->u16PackLength * psDrvLdPQInfo->u8LDFHeight;
    MS_U32 LDB_mem_size = (((psDrvLdPQInfo->u8LEDWidth - 1) / psDrvLdInfo->u16PackLength) + 1) * psDrvLdInfo->u16PackLength * psDrvLdPQInfo->u8LEDHeight;
    MS_U32 EDGE2D_mem_size = psDrvLdPQInfo->u8LSFWidth * psDrvLdPQInfo->u8LSFHeight * psDrvLdInfo->u16PackLength;

    MHal_LD_SetLDBBacklightWidth(psDrvLdPQInfo->u8LEDWidth);
    MHal_LD_SetLDBBacklightHeight(psDrvLdPQInfo->u8LEDHeight);

    MHal_LD_SetLSFBacklightWidth(psDrvLdPQInfo->u8LSFWidth);
    MHal_LD_SetLSFBacklightHeight(psDrvLdPQInfo->u8LSFHeight);

    MHal_LD_SetFrameWidth(psDrvLdInfo->u16PanelWidth);
    MHal_LD_SetFrameHeight(psDrvLdInfo->u16PanelHeight);

    MHal_LD_SetOutputHScalingUpRatio(((MS_U32)psDrvLdPQInfo->u8LSFWidth<<LDFSHIFT) / (psDrvLdInfo->u16PanelWidth - 1)); // (W_lsf + 1)x(2^20)/(W_lcd - 1)
    MHal_LD_SetOutputVScalingUpRatio(((MS_U32)psDrvLdPQInfo->u8LSFHeight<<LDFSHIFT) / (psDrvLdInfo->u16PanelHeight - 1)); // H_lsfx(2^20)/(H_lcd - 1)
    MHal_LD_SetLSFCBHRatio(u16HRatio); // W_ledx(2^16)/W_lsf
    MHal_LD_SetLSFCBVRatio(u16VRatio); // H_ledx(2^16)/H_lsf

    LDF_mem_size = (LDF_mem_size + 0xFF) >> LDFALIGN << LDFALIGN; // align at 0x100
    LDB_mem_size = (LDB_mem_size + 0xFF) >> LDFALIGN << LDFALIGN; // align at 0x100
    EDGE2D_mem_size = (EDGE2D_mem_size + 0xFF) >> LDFALIGN << LDFALIGN; // align at 0x100

    MHal_LD_SetLDBFrameBufBaseAddr_0((psDrvLdInfo->u32BaseAddr + 2*LDF_mem_size) / psDrvLdInfo->u16PackLength);
    MHal_LD_SetLDBFrameBufBaseAddr_1((psDrvLdInfo->u32BaseAddr + 2*LDF_mem_size) / psDrvLdInfo->u16PackLength);
    MHal_LD_SetEdge2DBufBaseAddr((psDrvLdInfo->u32BaseAddr + 2*LDF_mem_size + LDB_mem_size) / psDrvLdInfo->u16PackLength);

    MHal_LD_SetEdge2DEn(psDrvLdPQInfo->bEdge2DEn);
    MHal_LD_SetEdge2DGainEn(FALSE);
    MHal_LD_SetEdge2DDirectTypeEn(ENABLE);
    MHal_LD_SetEdge2DLocalTypeEn(psDrvLdPQInfo->enLEDType == E_LD_LED_LOCAL_TYPE ? ENABLE : DISABLE);

    MHal_LD_SetLSFHInitialPhase(psDrvLdPQInfo->u32LsfInitialPhase_H); // need fine tune
    MHal_LD_SetLSFVInitialPhase(psDrvLdPQInfo->u32LsfInitialPhase_V); // need fine tune
    MHal_LD_SetLSFHBoundary(ENABLE); // ENABLE -> duplicate
    MHal_LD_SetLSFVBoundary(ENABLE);

    MHal_LD_SetEdgeLevelShift(0); // 0:normal  1:x2  2:x4
    MHal_LD_SetBlendingDitherEn(TRUE);

    MHal_LD_SetCompensationEn(psDrvLdPQInfo->bCompensationEn);
    MHal_LD_SetCompensationBlend(psDrvLdPQInfo->u8CompenGain);
    MHal_LD_SetCompensationDitherEn(TRUE);
    
    LD_INFO(" trace! MDrv_LD_InitLDB, u8LSFWidth:0x%x, u8LSFHeight:0x%x \n",
        psDrvLdPQInfo->u8LSFWidth, psDrvLdPQInfo->u8LSFHeight);

    return E_LD_STATUS_SUCCESS;
}

MS_U16 MDrv_LD_LoadCompensationTable(const MS_U8* pu8CompTable)
{
    MS_U16 u16Lut = 0;
    MS_U16 i = 0;

    MHal_LD_SetCompensationMode(FALSE); // TRUE -> incremental mode
    for (i = 0; i < 256; i++)
    {
        MHal_LD_SetCompensationLUTAddress((MS_U8)i);
        u16Lut = *(pu8CompTable++);
        u16Lut |= (*(pu8CompTable++)) << 8;
        MHal_LD_WriteCompensationLUT(u16Lut);
        MHal_LD_SetCompensationWriteEn(ENABLE);
    }

    return E_LD_STATUS_SUCCESS;
}

/*
Edge 2D Table Format
Normal: Width = LED Number(HxV) / 4
        Height = Backlight size (HxV)

Special(for Edge-LR panel and odd V size)
        Width = (LED Height+1) x V / 4
        Height = Backlight size (HxV)

H-direction: zero padding to Packet Length (16byte)
*/
MS_U16 MDrv_LD_LoadEdge2DTable(const MS_U8* pu8Edge2DTable)
{
    MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 lsf_width = MHal_LD_GetLSFBacklightWidth();
    MS_U16 lsf_height = MHal_LD_GetLSFBacklightHeight();
    MS_U16 table_width = (((led_width-1)>>1) + 1) * (((led_height-1)>>1) + 1);
    MS_U16 table_height = lsf_width * lsf_height;
    phys_addr_t addr_base = MHal_LD_GetEdge2DBufBaseAddr() * psDrvLdInfo->u16PackLength;
    MS_U32 addr_offset = 0;
    //MS_PHYADDR u32Vaddr = 0;
    MS_U16 i = 0, j = 0;
    MS_U16 pack_length = psDrvLdInfo->u16PackLength;

    if (psDrvLdPQInfo->enLEDType == E_LD_LED_LOCAL_TYPE)
        table_width = 25;

    #if 0
    memcpy((void *)u32Vaddr, (void *)pu8Edge2DTable, table_width*table_height);
    #else
    for (j = 0; j < table_height; j++)
    {
        addr_offset = j * pack_length;
        for (i = 0; i < table_width; i++)
            *((volatile MS_U8*)(u64Vaddr + (addr_base-ld_addr_base) + (addr_offset++))) = *(pu8Edge2DTable++);
            //MDrv_LD_MIUWriteByte(addr_base, addr_offset++, *pu8Edge2DTable++);
    }
    #endif

    return E_LD_STATUS_SUCCESS;
}

MS_U16 MDrv_LD_InitSW(void)
{
    char *buf;

    MHal_LD_SetLEDType((MS_U8)psDrvLdPQInfo->enLEDType);
    MHal_LD_SetLEDBacklightWidth(psDrvLdPQInfo->u8LEDWidth);
    MHal_LD_SetLEDBacklightHeight(psDrvLdPQInfo->u8LEDHeight);

    MHal_LD_SetSWLocalDimmingStrength(psDrvLdPQInfo->u8LDStrength);
    MHal_LD_SetSWGlobalDimmingStrength(psDrvLdPQInfo->u8GDStrength);
    MHal_LD_SetSWBacklightInGamma(psDrvLdPQInfo->u8InGamma);
    MHal_LD_SetSWBacklightOutGamma(psDrvLdPQInfo->u8OutGamma);
    MHal_LD_SetSWMinClampValue(psDrvLdPQInfo->u8BLMinValue);

    MHal_LD_SetSWSpatialFilterStrength(psDrvLdPQInfo->u8SFStrength);
    MHal_LD_SetSWSpatialFilterStrength2(psDrvLdPQInfo->u8SFStrength2);
    MHal_LD_SetSWSpatialFilterStrength3(psDrvLdPQInfo->u8SFStrength3);
    MHal_LD_SetSWSpatialFilterStrength4(psDrvLdPQInfo->u8SFStrength4);
    MHal_LD_SetSWSpatialFilterStrength5(psDrvLdPQInfo->u8SFStrength5);

    MHal_LD_SetSWTemporalFilterStrengthUp(psDrvLdPQInfo->u8TFStrengthUp);
    MHal_LD_SetSWTemporalFilterStrengthDn(psDrvLdPQInfo->u8TFStrengthDn);
    MHal_LD_SetSWTemporalFilterLowTh(psDrvLdPQInfo->u8TFLowThreshold);
    MHal_LD_SetSWTemporalFilterHighTh(psDrvLdPQInfo->u8TFHightThreshold);

    MHal_LD_SetSWMaxThreshold(0x00);  //MaxThreshold initial

    MHal_LD_SetSWPulseMode(0);
    MHal_LD_SetSWPulseId(0);
    MHal_LD_SetSWPulseLEDIntensity(0);
    MHal_LD_SetSWPulseLDBIntensity(0);

    MHal_LD_SetSWReadLDFEn(ENABLE);
    MHal_LD_SetSWAlgorithmEn(ENABLE);
    MHal_LD_SetSWWriteLDBEn(ENABLE);
    MHal_LD_SetSWWriteSPIEn(ENABLE);

    buf = (char *)kzalloc(LD_BIN_LENGTH+1, GFP_KERNEL);
    if(!buf)
    {
        LD_ERROR(" load compensation bin kzalloc fail buf:%p  \n", buf);
		return FALSE;
    }

    //strlen("ldm.ini") is 7, replace "ldm.ini" with "Compensation.bin"
    strncpy((stCusPath.aCusPath+strlen(stCusPath.aCusPath)-7),"Compensation.bin",16);
    strncpy((stCusPath.aCusPathU+strlen(stCusPath.aCusPathU)-7),"Compensation.bin",16);
    if(FALSE == MDrv_LD_ParseBin(buf, LD_BIN_LENGTH, (char *)stCusPath.aCusPath, (char *)stCusPath.aCusPathU))
    {
        kfree((void *)buf);
        LD_ERROR(" load compensation bin fail \n");
        return E_LD_STATUS_PARAMETER_ERROR;
    }

    psDrvLdPQInfo->pu8CompTable = buf + LD_BIN_OFFSET;
    if (psDrvLdPQInfo->bCompensationEn)
    {
        MDrv_LD_LoadCompensationTable(psDrvLdPQInfo->pu8CompTable);
    }

    //strlen("Compensation.bin") is 16, replace "Compensation.bin" with "Edge2D.bin"
    strcpy((stCusPath.aCusPath+strlen(stCusPath.aCusPath)-16),"Edge2D.bin");
    strcpy((stCusPath.aCusPathU+strlen(stCusPath.aCusPathU)-16),"Edge2D.bin");
    if(FALSE == MDrv_LD_ParseBin(buf, LD_BIN_LENGTH, (char *)stCusPath.aCusPath, (char *)stCusPath.aCusPathU))
    {
        kfree((void *)buf);
        LD_ERROR("load Edge2D bin fail \n");
        return E_LD_STATUS_PARAMETER_ERROR;
    }

    psDrvLdPQInfo->pu8Edge2DTable = buf + LD_BIN_OFFSET;
    if (psDrvLdPQInfo->bEdge2DEn)
    {
        MDrv_LD_LoadEdge2DTable(psDrvLdPQInfo->pu8Edge2DTable);
    }
    
    kfree(buf);
    LD_INFO(" trace! MDrv_LD_InitSW, u8LEDWidth:0x%x, u8LEDHeight:0x%x \n",
        psDrvLdPQInfo->u8LEDWidth, psDrvLdPQInfo->u8LEDHeight);

    return E_LD_STATUS_SUCCESS;
}

MS_S8 MDrv_LD_Init(void)
{
    MS_U16 flag = 0;
    phys_addr_t u64Addr_Offset = 0;
    MS_U16 U16Ret = FALSE;
    
    U16Ret = MDrv_LD_PreInitVariable();
    if(E_LD_STATUS_SUCCESS != U16Ret)
    {
        LD_ERROR("error! parse customer file fail  \n");
        return -1;
    }

    psDrvLdInfo->u16PackLength = MHAL_LD_PACKLENGTH;    

    //ld_addr_base = MHal_LD_GetLDFFrameBufBaseAddr_L0();
    ld_addr_base = psDrvLdInfo->u32BaseAddr;
    if(!ld_addr_base)
    {
        LD_ERROR("error!  addr_base_LDF null \n");
        return -1;
    }

    GETMIUOFFSET(ld_addr_base, u64Addr_Offset);
    u64Vaddr = ioremap_cached(ld_addr_base+u64Addr_Offset, LDMMAPLENGTH);
    if((NULL == u64Vaddr) || ((void*)((size_t)-1) == u64Vaddr))
    {
        LD_ERROR("error! ioremap_cached paddr:0x%x,length:%d\n", (MS_U32)(ld_addr_base+u64Addr_Offset),LDMMAPLENGTH);
        return -1;
    }

    flag = MDrv_LD_InitLDF();
    if (flag != E_LD_STATUS_SUCCESS)
    {        
        LD_ERROR("flag[%d] \n", flag);
        return E_LD_STATUS_FAIL;
    }

    flag = MDrv_LD_InitLDB();
    if (flag != E_LD_STATUS_SUCCESS)
    {        
        LD_ERROR("flag[%d] \n", flag);
        return E_LD_STATUS_FAIL;
    }
    
    flag = MDrv_LD_InitSW();
    if (flag != E_LD_STATUS_SUCCESS)
    {        
        LD_ERROR("flag[%d] \n", flag);
        return E_LD_STATUS_FAIL;
    }

    LD_INFO("  done \n");
    return E_LD_STATUS_SUCCESS;
}


#endif

