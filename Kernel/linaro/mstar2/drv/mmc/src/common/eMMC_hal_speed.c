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

#include "eMMC.h"

// ==========================================================
U32 eMMC_FCIE_EnableSDRMode(void)
{
    U32 u32_ErrSpeed = eMMC_ST_SUCCESS, u32_ErrWidth = eMMC_ST_SUCCESS;

    // ----------------------------------------
    // may call from any other interface status
    if(DRV_FLAG_SPEED_HS200 == eMMC_SPEED_MODE())
    {
        u32_ErrSpeed = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
    }
    else if(DRV_FLAG_SPEED_HS400 == eMMC_SPEED_MODE())
    {
        u32_ErrSpeed = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
        u32_ErrWidth = eMMC_SetBusWidth(8, 0);
    }
    else
    {
        u32_ErrSpeed = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
        u32_ErrWidth = eMMC_SetBusWidth(8, 0);
    }

    if(eMMC_ST_SUCCESS!=u32_ErrSpeed || eMMC_ST_SUCCESS!=u32_ErrWidth)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh %Xh\n", u32_ErrSpeed, u32_ErrWidth);
        return u32_ErrSpeed ? u32_ErrSpeed : u32_ErrWidth;
    }

    // ----------------------------------------
    // set to normal SDR 48MHz
    eMMC_pads_switch(FCIE_eMMC_SDR);
    eMMC_clock_setting(FCIE_DEFAULT_CLK);

    return eMMC_ST_SUCCESS;
}


// ===========================
#if !(defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP)
void eMMC_DumpDDR48TTable(void)
{
    U16 u16_i;

    eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n  eMMC DDR Timing Table: Cnt:%u CurIdx:%u \n",
        g_eMMCDrv.TimingTable_t.u8_SetCnt, g_eMMCDrv.TimingTable_t.u8_CurSetIdx);

    for(u16_i=0; u16_i<g_eMMCDrv.TimingTable_t.u8_SetCnt; u16_i++)
        eMMC_debug(eMMC_DEBUG_LEVEL,0,"    Set:%u: clk:%02Xh, DQS:%02Xh, Cell:%02Xh \n",
            u16_i, g_eMMCDrv.TimingTable_t.Set[u16_i].u8_Clk,
            g_eMMCDrv.TimingTable_t.Set[u16_i].Param.u8_DQS,
            g_eMMCDrv.TimingTable_t.Set[u16_i].Param.u8_Cell);
}

void eMMC_FCIE_SetDDR48TimingReg(U8 u8_DQS, U8 u8_DelaySel)
{
    #ifndef IP_FCIE_VERSION_5
    REG_FCIE_CLRBIT(FCIE_SM_STS, BIT_DQS_MODE_MASK|BIT_DQS_DELAY_CELL_MASK);

    REG_FCIE_SETBIT(FCIE_SM_STS, u8_DQS<<BIT_DQS_MODE_SHIFT);
    REG_FCIE_SETBIT(FCIE_SM_STS, u8_DelaySel<<BIT_DQS_DELAY_CELL_SHIFT);
    #endif
}

// ===========================
#else // DDR52 (ATOP)
void eMMC_DumpATopTable(void)
{
    U16 u16_i;

    eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n  eMMC ATop Timing Table: Ver:%Xh Cnt:%u CurIdx:%u \n",
        g_eMMCDrv.TimingTable_t.u32_VerNo,
        g_eMMCDrv.TimingTable_t.u8_SetCnt, g_eMMCDrv.TimingTable_t.u8_CurSetIdx);

    for(u16_i=0; u16_i<g_eMMCDrv.TimingTable_t.u8_SetCnt; u16_i++)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL,0,"    Set:%u: \n", u16_i);
        eMMC_debug(eMMC_DEBUG_LEVEL,0,"      Clk: %04Xh, Reg2Ch: %02Xh, Skew4: %02Xh, Result: %08Xh, Idx: %u \n",
                g_eMMCDrv.TimingTable_t.Set[u16_i].u8_Clk,
            g_eMMCDrv.TimingTable_t.Set[u16_i].u8_Reg2Ch,
            g_eMMCDrv.TimingTable_t.Set[u16_i].u8_Skew4,
                g_eMMCDrv.TimingTable_t.Set[u16_i].u32_ScanResult,
                g_eMMCDrv.TimingTable_Ext_t.Set.u8_Skew4Idx);

            #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400    
        eMMC_debug(eMMC_DEBUG_LEVEL,0,"      Skew2: %02Xh, RXDLL: %Xh, RXDLLCnt: %02Xh \n",
                g_eMMCDrv.TimingTable_t.Set[u16_i].u8_Skew2,
                g_eMMCDrv.TimingTable_t.Set[u16_i].u8_Cell,
                g_eMMCDrv.TimingTable_t.Set[u16_i].u8_CellCnt);
            #endif
    }
}

void eMMC_FCIE_SetATopTimingReg(U8 u8_SetIdx)
{
    #ifdef IP_FCIE_VERSION_5
    if(g_eMMCDrv.u8_PadType== FCIE_eMMC_DDR)
    {
        REG_FCIE_CLRBIT(reg_emmcpll_0x6c, BIT_DQS_MODE_MASK|BIT_DQS_DELAY_CELL_MASK);
        
        REG_FCIE_SETBIT(reg_emmcpll_0x6c, g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew4<<BIT_DQS_MODE_SHIFT);
        REG_FCIE_SETBIT(reg_emmcpll_0x6c, g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Cell<<BIT_DQS_DELAY_CELL_SHIFT);
    }
    else//HS400,HS200
    {
        if(g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Reg2Ch)
        {
            #ifdef REG_DIG_SKEW4_INV 
            REG_FCIE_SETBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);       //skew4 inverse for muji
            #endif
            #ifdef REG_ANL_SKEW4_INV
            REG_FCIE_SETBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
            #endif
        }
    	else
        {
            #ifdef REG_DIG_SKEW4_INV 
            REG_FCIE_CLRBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);       //skew4 inverse for muji
            #endif
            #ifdef REG_ANL_SKEW4_INV
            REG_FCIE_CLRBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
            #endif
        }
    
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
        REG_FCIE_SETBIT(reg_emmcpll_0x03,
            g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew4<<12);
    
        #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
        eMMC_FCIE_SetDelayLatch(g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Cell);
        
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
        REG_FCIE_SETBIT(reg_emmcpll_0x03,
                g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew2<<4);
        #endif
    }
    #else
    if(g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Reg2Ch)
    {
        REG_FCIE_SETBIT(FCIE_SM_STS, BIT11);
    }
	else
    {
        REG_FCIE_CLRBIT(FCIE_SM_STS, BIT11);
    }

    REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
    REG_FCIE_SETBIT(reg_emmcpll_0x03,
    g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew4<<12);
    #endif
}

void eMMC_FCIE_Apply_Reg(U8 u8_SetIdx)
{
    int i;

    for(i = 0; i < g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt; i ++)
    {
        if(g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_OpCode == 
            REG_OP_W)
        {
            REG_FCIE_W( RIU_BASE + g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u32_RegAddress,  //address = (bank address+ register offset) << 2
                    g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_RegValue);            
        }
        else if(g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_OpCode == 
            REG_OP_CLRBIT)
        {
            REG_FCIE_CLRBIT( RIU_BASE + g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u32_RegAddress,  //address = (bank address+ register offset) << 2
                g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_RegValue);            
        }
        else if(g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_OpCode == 
            REG_OP_SETBIT)
        {
            REG_FCIE_CLRBIT( RIU_BASE + g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u32_RegAddress,  //address = (bank address+ register offset) << 2
                g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_RegMask);
            
            REG_FCIE_SETBIT( RIU_BASE + g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u32_RegAddress,  //address = (bank address+ register offset) << 2
                g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_RegValue);            
        }
        else if(g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_OpCode == 
            0)
           //do nothing here
           ;
        else
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "unknown OpCode 0x%X\n", g_eMMCDrv.TimingTable_G_t.RegSet[u8_SetIdx * g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt + i].u16_OpCode);
            eMMC_die("");
        }
    }
}

void eMMC_DumpGeneralTable(void)
{
    int i, j;
    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "OpCode Address Value Mask\n");

    for(i = 0; i< g_eMMCDrv.TimingTable_G_t.u8_SetCnt; i++)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "==========Set 0x%X==========\n", i);
        
        for(j = 0; j < g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt; j++)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL, 0, "0x%X 0x%08X 0x%04X 0x%04X\n",
                g_eMMCDrv.TimingTable_G_t.RegSet[i*g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt+j].u16_OpCode,                 
                g_eMMCDrv.TimingTable_G_t.RegSet[i*g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt+j].u32_RegAddress, 
                g_eMMCDrv.TimingTable_G_t.RegSet[i*g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt+j].u16_RegValue,
                g_eMMCDrv.TimingTable_G_t.RegSet[i*g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt+j].u16_RegMask);
        }
    }
}

#endif


// if failed, must be board issue.
U32 eMMC_FCIE_ChooseSpeedMode(void)
{
    U32 u32_err = eMMC_ST_SUCCESS;

    #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
    if(g_eMMCDrv.u8_ECSD196_DevType & eMMC_DEVTYPE_HS400_1_8V)
    {
        u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);
        if(eMMC_ST_SUCCESS != u32_err)
            return u32_err;
        if(eMMC_GetR1() & eMMC_R1_DEVICE_IS_LOCKED)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL,1,"\neMMC Warn: HS400, but locked\n");
            return u32_err;
        }

        if(eMMC_ST_SUCCESS != eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400) )
        {
            u32_err = eMMC_FCIE_BuildHS200TimingTable(); // DS mode uses HS200 timing table
            if(eMMC_ST_SUCCESS != u32_err)
                return u32_err;

            u32_err = eMMC_FCIE_ResetToHS400(g_eMMCDrv.TimingTable_t.Set[0].u8_Clk);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 fail: %Xh\n", u32_err);
                return u32_err;
            }

            #ifndef DLL_PHASE_DEBUG
            // HS400 use DLL setting for DAT[0-7]
            u32_err = eMMC_FCIE_DetectHS400Timing();
            if(eMMC_ST_SUCCESS != u32_err)
            {
            #endif
                #ifdef CONFIG_MSTAR_MONACO 
                if( (REG_FCIE(reg_chip_version)>>8) == 0x01 )
                {
                    u32_err = eMMC_U02_ECO_Skew4_Adjust_patch();
                }
                else
                #endif    
                {
                    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
                               "eMMC Err: eMMC_FCIE_DetectHS400Timing fail: %Xh\n", u32_err);
                    eMMC_FCIE_ErrHandler_Stop();
                }

            #ifndef DLL_PHASE_DEBUG
            }
            #endif
        }

        eMMC_debug(0,0,"\neMMC: HS400 %uMHz \n", g_eMMCDrv.u32_ClkKHz/1000);
        return u32_err;
    }
    #endif

    #if defined(ENABLE_eMMC_HS200) && ENABLE_eMMC_HS200
    if(g_eMMCDrv.u8_ECSD196_DevType & eMMC_DEVTYPE_HS200_1_8V)
    {
        u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);
        if(eMMC_ST_SUCCESS != u32_err)
            return u32_err;
        if(eMMC_GetR1() & eMMC_R1_DEVICE_IS_LOCKED)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL,1,"\neMMC Warn: HS200, but locked\n");
            return u32_err;
        }

        if(eMMC_ST_SUCCESS != eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS200) )
            u32_err = eMMC_FCIE_BuildHS200TimingTable();

        eMMC_debug(0,0,"\neMMC: HS200 %uMHz \n", g_eMMCDrv.u32_ClkKHz/1000);
        return u32_err;
    }
    #endif

    #if (defined(ENABLE_eMMC_ATOP)&&ENABLE_eMMC_ATOP) || (defined(IF_DETECT_eMMC_DDR_TIMING)&&IF_DETECT_eMMC_DDR_TIMING)
    if(g_eMMCDrv.u8_ECSD196_DevType & eMMC_DEVTYPE_DDR)
    {
        if(eMMC_ST_SUCCESS != eMMC_FCIE_EnableFastMode(FCIE_eMMC_DDR))
            u32_err = eMMC_FCIE_BuildDDRTimingTable();

        eMMC_debug(0,0,"\neMMC: DDR %uMHz \n", g_eMMCDrv.u32_ClkKHz/1000);
        return u32_err;
    }
    #endif

    eMMC_debug(0,0,"\neMMC: SDR %uMHz\n", g_eMMCDrv.u32_ClkKHz/1000);
    return u32_err;
}


void eMMC_FCIE_ApplyTimingSet(U8 u8_Idx)
{
    // make sure a complete outside clock cycle
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    #if !(defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP)
    // DDR48
    eMMC_clock_setting(g_eMMCDrv.TimingTable_t.Set[u8_Idx].u8_Clk);
    eMMC_FCIE_SetDDR48TimingReg(
        g_eMMCDrv.TimingTable_t.Set[u8_Idx].Param.u8_DQS,
        g_eMMCDrv.TimingTable_t.Set[u8_Idx].Param.u8_Cell);
    g_eMMCDrv.TimingTable_t.u8_CurSetIdx = u8_Idx;
    #else
    // HS400 or HS200 or DDR52
    if( g_eMMCDrv.u8_PadType != FCIE_eMMC_HS400 ||g_eMMCDrv.TimingTable_G_t.u8_SetCnt == 0)
    {
        eMMC_clock_setting(g_eMMCDrv.TimingTable_t.Set[u8_Idx].u8_Clk);
        eMMC_FCIE_SetATopTimingReg(u8_Idx);
        g_eMMCDrv.TimingTable_t.u8_CurSetIdx = u8_Idx;
    }
    else
    //apply ext table
    {
        eMMC_clock_setting(g_eMMCDrv.TimingTable_G_t.u32_Clk);
        eMMC_FCIE_Apply_Reg(u8_Idx);
        g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx = u8_Idx;            
    }

    #endif
}


void eMMC_DumpTimingTable(void)
{
    #if !(defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP)
    eMMC_DumpDDR48TTable();
    #else
    eMMC_DumpATopTable();
    if( g_eMMCDrv.TimingTable_G_t.u8_SetCnt)
        eMMC_DumpGeneralTable();
    
    #endif
}


U32 eMMC_LoadTimingTable(U8 u8_PadType)
{
    U32 u32_err;
    U32 u32_ChkSum, u32_eMMCBlkAddr;

    // --------------------------------------

    switch(u8_PadType)
    {
        #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
        case FCIE_eMMC_HS400:            
            u32_eMMCBlkAddr = eMMC_HS400TABLE_BLK_0;
            break;
        #endif
        #if defined(ENABLE_eMMC_HS200) && ENABLE_eMMC_HS200        
        case FCIE_eMMC_HS200:            
            u32_eMMCBlkAddr = eMMC_HS200TABLE_BLK_0;
            break;
        #endif
        case FCIE_eMMC_DDR:            
            u32_eMMCBlkAddr = eMMC_DDRTABLE_BLK_0;
            break;            
        default:
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: unknown pad type %Xh\n", u8_PadType);            
            return eMMC_ST_ERR_DDRT_NONA;
    }

    // --------------------------------------

    u32_err = eMMC_CMD18(u32_eMMCBlkAddr, gau8_eMMC_SectorBuf, 1);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC WARN: load 1 Table fail, %Xh\n", u32_err);

        u32_err = eMMC_CMD18(u32_eMMCBlkAddr+1, gau8_eMMC_SectorBuf, 1);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: load 2 Tables fail, %Xh\n", u32_err);
            goto LABEL_END_OF_NO_TABLE;
        }
    }

    // --------------------------------------
    memcpy((U8*)&g_eMMCDrv.TimingTable_t, gau8_eMMC_SectorBuf, sizeof(g_eMMCDrv.TimingTable_t));

    u32_ChkSum = eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t)-eMMC_TIMING_TABLE_CHKSUM_OFFSET);
    if(u32_ChkSum != g_eMMCDrv.TimingTable_t.u32_ChkSum)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: ChkSum error, no Table \n");
        printk("u32_ChkSum=%08X\n", u32_ChkSum);
        printk("g_eMMCDrv.TimingTable_t.u32_ChkSum=%08X\n", g_eMMCDrv.TimingTable_t.u32_ChkSum);
        eMMC_dump_mem((U8*)&g_eMMCDrv.TimingTable_t,sizeof(g_eMMCDrv.TimingTable_t));
        u32_err = eMMC_ST_ERR_DDRT_CHKSUM;
        goto LABEL_END_OF_NO_TABLE;
    }

    if(0==u32_ChkSum )
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: no Table \n");
        u32_err = eMMC_ST_ERR_DDRT_NONA;
        goto LABEL_END_OF_NO_TABLE;
    }

    #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
    //read general table if hs400
    if(FCIE_eMMC_HS400 == u8_PadType)
    {
        u32_err = eMMC_CMD18(eMMC_HS400EXTTABLE_BLK_0, gau8_eMMC_SectorBuf, 1);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC WARN: load 1st Gen_TTable fail, %Xh\n", u32_err);

            u32_err = eMMC_CMD18(eMMC_HS400EXTTABLE_BLK_1, gau8_eMMC_SectorBuf, 1);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: load 2nd Gen_TTables fail, %Xh\n", u32_err);                
                g_eMMCDrv.TimingTable_G_t.u8_SetCnt = 0;
                g_eMMCDrv.TimingTable_G_t.u32_ChkSum = g_eMMCDrv.TimingTable_G_t.u32_VerNo = 0;                
                goto LABEL_END_OF_NO_GEN_TABLE;
            }
        }
        memcpy((U8*)&g_eMMCDrv.TimingTable_G_t, gau8_eMMC_SectorBuf, sizeof(g_eMMCDrv.TimingTable_G_t));
        
        u32_ChkSum = eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_G_t.u32_VerNo, 
                        (sizeof(g_eMMCDrv.TimingTable_G_t) - sizeof(U32)/*checksum*/));
        if(u32_ChkSum != g_eMMCDrv.TimingTable_G_t.u32_ChkSum)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: ChkSum error, no Gen_TTable \n");            
            g_eMMCDrv.TimingTable_G_t.u8_SetCnt = 0;
            g_eMMCDrv.TimingTable_G_t.u32_ChkSum = g_eMMCDrv.TimingTable_G_t.u32_VerNo = 0;            
            goto LABEL_END_OF_NO_GEN_TABLE;
        }
        
        if(0==u32_ChkSum)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: no Gen_TTable \n");            
            g_eMMCDrv.TimingTable_G_t.u8_SetCnt = 0;
            g_eMMCDrv.TimingTable_G_t.u32_ChkSum = g_eMMCDrv.TimingTable_G_t.u32_VerNo = 0;            
            goto LABEL_END_OF_NO_GEN_TABLE;
        }
    }    
    LABEL_END_OF_NO_GEN_TABLE:
    #endif

    return eMMC_ST_SUCCESS;

    LABEL_END_OF_NO_TABLE:
    g_eMMCDrv.TimingTable_t.u8_SetCnt = 0;
    #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
    g_eMMCDrv.TimingTable_G_t.u8_SetCnt = 0;
    g_eMMCDrv.TimingTable_G_t.u32_ChkSum = g_eMMCDrv.TimingTable_G_t.u32_VerNo = 0;
    #endif    

    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: load Tables fail, %Xh\n", u32_err);
    return u32_err;

}


// set eMMC device & pad registers (no macro timing registers, since also involved in tuning procedure)
U32 eMMC_FCIE_EnableFastMode_Ex(U8 u8_PadType)
{
    U32 u32_err=eMMC_ST_SUCCESS;

    switch(u8_PadType)
    {
        case FCIE_eMMC_DDR:
            u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
            if(eMMC_ST_SUCCESS!=u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: enable HighSpeed fail: %Xh\n", u32_err);
                return u32_err;
            }
            u32_err = eMMC_SetBusWidth(8, 1);
            if(eMMC_ST_SUCCESS!=u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: enable DDR fail: %Xh\n", u32_err);
                return u32_err;
            }

            if(0==g_eMMCDrv.TimingTable_t.u8_SetCnt)
                #if defined(ENABLE_eMMC_ATOP)&&ENABLE_eMMC_ATOP
                eMMC_clock_setting(gau8_eMMCPLLSel_52[0]);
                #else
                eMMC_clock_setting(gau8_FCIEClkSel[0]);
                #endif
            break;

        #if (defined(ENABLE_eMMC_HS200)&&ENABLE_eMMC_HS200) || \
            (defined(ENABLE_eMMC_HS400)&&ENABLE_eMMC_HS400)
        case FCIE_eMMC_HS200:
            u32_err = eMMC_SetBusWidth(8, 0); // disable DDR
            if(eMMC_ST_SUCCESS!=u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: HS200 disable DDR fail: %Xh\n", u32_err);
                return u32_err;
            }
            u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HS200);
            if(eMMC_ST_SUCCESS!=u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: enable HS200 fail: %Xh\n", u32_err);
                return u32_err;
            }
            if(0==g_eMMCDrv.TimingTable_t.u8_SetCnt)
                eMMC_clock_setting(gau8_eMMCPLLSel_200[0]);

            break;
        #endif

        #if defined(ENABLE_eMMC_HS400)&&ENABLE_eMMC_HS400
        case FCIE_eMMC_HS400:
            u32_err = eMMC_SetBusWidth(8, 1); // enable DDR
            if(eMMC_ST_SUCCESS!=u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: HS400 enable DDR fail: %Xh\n", u32_err);
                return u32_err;
            }
            u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HS400);
            if(eMMC_ST_SUCCESS!=u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: enable HS400 fail: %Xh\n", u32_err);
                return u32_err;
            }
            if(0==g_eMMCDrv.TimingTable_t.u8_SetCnt)
                eMMC_clock_setting(gau8_eMMCPLLSel_200[0]);
            break;
        #endif
    }

    // --------------------------------------
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
            "eMMC Err: set ECSD fail: %Xh\n", u32_err);
        return u32_err;
    }

    #ifndef IP_FCIE_VERSION_5
    g_eMMCDrv.u16_Reg10_Mode &= ~BIT_SD_DATA_SYNC;
    #endif

    eMMC_pads_switch(u8_PadType);
    #if defined(ENABLE_eMMC_HS400)&&ENABLE_eMMC_HS400
	if(g_eMMCDrv.TimingTable_G_t.u8_SetCnt && u8_PadType == FCIE_eMMC_HS400)
		eMMC_FCIE_ApplyTimingSet(g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx);
        else
    #endif
    if(g_eMMCDrv.TimingTable_t.u8_SetCnt)
        eMMC_FCIE_ApplyTimingSet(eMMC_TIMING_SET_MAX);


    return u32_err;
}


U32 eMMC_FCIE_EnableFastMode(U8 u8_PadType)
{
    U32 u32_err;

    if(u8_PadType == g_eMMCDrv.u8_PadType)
        return eMMC_ST_SUCCESS;

    u32_err = eMMC_LoadTimingTable(u8_PadType);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: no Timing Table, %Xh\n", u32_err);
        return u32_err;
    }
    #ifndef IP_FCIE_VERSION_5
    #if defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP
    {
        U16 u16_OldClkRegVal = g_eMMCDrv.u16_ClkRegVal;
        // --------------------------------
        // ATOP: must init emmpll before pad
        eMMC_clock_setting(g_eMMCDrv.TimingTable_t.Set[0].u8_Clk);
        // --------------------------------
        eMMC_clock_setting(u16_OldClkRegVal);
    }
    #endif
    #endif
    u32_err = eMMC_FCIE_EnableFastMode_Ex(u8_PadType);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: EnableDDRMode_Ex fail, %Xh\n", u32_err);
        eMMC_die("");
        return u32_err;
    }


    return eMMC_ST_SUCCESS;
}



#if defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP

static const U8 hs200_tunning_pattern_64[64] = {
    0xFF, 0x0F, 0xFF, 0x00, 0xFF, 0xCC, 0xC3, 0xCC, 0xC3, 0x3C, 0xCC, 0xFF, 0xFE, 0xFF, 0xFE, 0xEF,
    0xFF, 0xDF, 0xFF, 0xDD, 0xFF, 0xFB, 0xFF, 0xFB, 0xBF, 0xFF, 0x7F, 0xFF, 0x77, 0xF7, 0xBD, 0xEF,
    0xFF, 0xF0, 0xFF, 0xF0, 0x0F, 0xFC, 0xCC, 0x3C, 0xCC, 0x33, 0xCC, 0xCF, 0xFF, 0xEF, 0xFF, 0xEE,
    0xFF, 0xFD, 0xFF, 0xFD, 0xDF, 0xFF, 0xBF, 0xFF, 0xBB, 0xFF, 0xF7, 0xFF, 0xF7, 0x7F, 0x7B, 0xDE
};

static const U8 hs200_tunning_pattern_128[128] = {
    0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0x33, 0xCC, 0xCC,
    0xCC, 0x33, 0x33, 0xCC, 0xCC, 0xCC, 0xFF, 0xFF,
    0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE, 0xFF,
    0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD, 0xDD,
    0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF, 0xBB,
    0xBB, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0xFF, 0xFF,
    0x77, 0x77, 0xFF, 0x77, 0xBB, 0xDD, 0xEE, 0xFF,
    0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
    0x00, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0x33, 0xCC,
    0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0xCC, 0xFF,
    0xFF, 0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE,
    0xFF, 0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD,
    0xDD, 0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF,
    0xBB, 0xBB, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0xFF,
    0xFF, 0x77, 0x77, 0xFF, 0x77, 0xBB, 0xDD, 0xEE
};

// read out from RIU then compare pattern is slow
extern U32 eMMC_WaitGetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt);
U32 eMMC_CMD21_CIFD(void)
{
    U32 u32_err = 0;
    U16 u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN, u16_reg;
    U16 u16_mode;
    #ifdef IP_FCIE_VERSION_5
    U16 u16_i;
    #endif
    U8 i;

    eMMC_FCIE_ClearEvents();

    if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_4) {
        #ifndef IP_FCIE_VERSION_5
        REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | 64); // 64 bytes tuning pattern
        #else
        REG_FCIE_W(FCIE_BLK_SIZE,  64); // 64 bytes tuning pattern
        REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, 64 & 0xFFFF);
        REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, 64 >> 16);
        #endif
    } else if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_8) {
        #ifndef IP_FCIE_VERSION_5
        REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | 128); // 128 bytes tuning pattern
        #else
        REG_FCIE_W(FCIE_BLK_SIZE,  128); // 128 bytes tuning pattern
        REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, 128 & 0xFFFF);
        REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, 128 >> 16);
        #endif
    } else {
        u32_err = eMMC_ST_ERR_CMD21_ONE_BIT;
        eMMC_debug(0, 0, "eMMC Warn: g_eMMCDrv.u8_BUS_WIDTH = %02Xh\n", g_eMMCDrv.u8_BUS_WIDTH);
        goto ErrorHandle;
    }

    if(!(g_eMMCDrv.u8_ECSD196_DevType & BIT4)) {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: eMMC not support HS200 1.8V\n");
        u32_err = eMMC_ST_ERR_CMD21_NO_HS200_1_8V;
        goto ErrorHandle;
    }

    REG_FCIE_W(FCIE_JOB_BL_CNT, 1);

    u16_mode = BIT_SD_DATA_CIFD | g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

    u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, 0, 21, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err) {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: CMD55 send CMD fail: %08Xh\n", u32_err);
        goto ErrorHandle;
    }

    #ifndef IP_FCIE_VERSION_5
    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_SD_DATA_END, TIME_WAIT_1_BLK_END);
    #else
    //read for data
    for (u16_i=0; u16_i< (128 >> 6); u16_i++)
    {   // read data
        u32_err = eMMC_WaitGetCIFD((U8*)((uintptr_t)gau8_eMMC_SectorBuf + (u16_i << 6)), 0x40);
        if(u32_err != eMMC_ST_SUCCESS)
        {
            eMMC_FCIE_ErrHandler_Stop();
            goto ErrorHandle;
        }
    }

    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_1_BLK_END);
    #endif
    if(u32_err) {
        goto ErrorHandle;
    }

    // check status
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

    if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR)) {

        u32_err = eMMC_ST_ERR_CMD21_NO_RSP;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: CMD21 no Rsp, SD_STS: %04Xh \n", u16_reg);
        goto ErrorHandle;

    } else if(u16_reg &BIT_SD_R_CRC_ERR) {

        u32_err = eMMC_ST_ERR_CMD21_DATA_CRC;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: CMD21 data CRC err, SD_STS: %04Xh \n", u16_reg);
        goto ErrorHandle;

    }else {
        if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_4)
        {
            if(memcmp((void*)hs200_tunning_pattern_64, (void*)gau8_eMMC_SectorBuf, 64)) {
                eMMC_debug(0, 0, "tuning pattern 4bit width:");
                for(i=0; i<64; i++) {
                    if(i%16==0) eMMC_debug(0, 0, "\n\t");
                    eMMC_debug(0, 0, "%02X ", gau8_eMMC_SectorBuf[i]);
                }
                eMMC_debug(0, 0, "\n");
                u32_err = eMMC_ST_ERR_CMD21_DATA_CMP;
                goto ErrorHandle;
            }
        }
        else if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_8)
        {
            if(memcmp((void*)hs200_tunning_pattern_128, (void*)gau8_eMMC_SectorBuf, 128)) {
                eMMC_debug(0, 0, "tuning pattern 8bit width:");
                for(i=0; i<128; i++) {
                    if(i%16==0) eMMC_debug(0, 0, "\n\t");
                    eMMC_debug(0, 0, "%02X ", gau8_eMMC_SectorBuf[i]);
                }
                eMMC_debug(0, 0, "\n");
                u32_err = eMMC_ST_ERR_CMD21_DATA_CMP;
                goto ErrorHandle;
            }
        }
    }
ErrorHandle:

    #ifndef IP_FCIE_VERSION_5
    REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | eMMC_SECTOR_512BYTE); // restore anyway...
    #else
    REG_FCIE_W(FCIE_BLK_SIZE,  eMMC_SECTOR_512BYTE);
    #endif

    if(u32_err)
        eMMC_debug(0, 0, "eMMC err: %08Xh\n", u32_err);

    return u32_err;

}

// use memcmp to confirm tuning pattern
U32 eMMC_CMD21_MIU(void)
{
    U32 u32_err = 0;
    U16 u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;
    U16 u16_reg;
    U16 u16_mode;
    U32 u32_DataDMAAddr, u32_DataMapAddr = 0;
    U8 i;

    #ifndef IP_FCIE_VERSION_5
    REG_FCIE_W(FCIE_TOGGLE_CNT, TOGGLE_CNT_128_CLK_R);
    REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
    REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
    eMMC_hw_timer_delay(TIME_WAIT_FCIE_RST_TOGGLE_CNT); // Brian needs 2T
    REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
    #endif

    eMMC_FCIE_ClearEvents();

    if(!(g_eMMCDrv.u8_ECSD196_DevType & BIT4)) {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: eMMC not support HS200 1.8V\n");
        u32_err = eMMC_ST_ERR_CMD21_NO_HS200_1_8V;
        goto ErrorHandle;
    }

    u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH; // edit for MIU

    // add for MIU start
    REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
    u32_DataMapAddr = eMMC_DMA_MAP_address((uintptr_t)gau8_eMMC_SectorBuf,eMMC_SECTOR_512BYTE,1);
    u32_DataDMAAddr = eMMC_translate_DMA_address_Ex(u32_DataMapAddr, eMMC_SECTOR_512BYTE);

    #ifndef IP_FCIE_VERSION_5
    #if FICE_BYTE_MODE_ENABLE
    REG_FCIE_W(FCIE_SDIO_ADDR0, u32_DataDMAAddr & 0xFFFF);
    REG_FCIE_W(FCIE_SDIO_ADDR1, u32_DataDMAAddr >> 16);
    #else
    REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_DataDMAAddr>>MIU_BUS_WIDTH_BITS)&0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_DataDMAAddr>>MIU_BUS_WIDTH_BITS)>>16);
    #endif
    REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
    u32_err = eMMC_FCIE_FifoClkRdy(0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: CMD21 wait FIFOClk fail: %08Xh\n", u32_err);
        eMMC_FCIE_ErrHandler_Stop();
        goto ErrorHandle;
    }
    REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);
    #else
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_DataDMAAddr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_DataDMAAddr >> 16);
    #endif

    if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_4) {
        #ifndef IP_FCIE_VERSION_5
        REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | 64);
        #else
        REG_FCIE_W(FCIE_BLK_SIZE,  64); // 64 bytes tuning pattern
        REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, 64 & 0xFFFF);
        REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, 64 >> 16);
        #endif
    } else if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_8) {
        #ifndef IP_FCIE_VERSION_5
        REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | 128);
        #else
        REG_FCIE_W(FCIE_BLK_SIZE,  128); // 128 bytes tuning pattern
        REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, 128 & 0xFFFF);
        REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, 128 >> 16);
        #endif
    } else {
        u32_err = eMMC_ST_ERR_CMD21_ONE_BIT;
        eMMC_debug(1, 0, "eMMC Warn: g_eMMCDrv.u8_BUS_WIDTH = %02Xh\n", g_eMMCDrv.u8_BUS_WIDTH);
        goto ErrorHandle;
    }

    // add for MIU end
    u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, 0, 21, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err) {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: CMD21 send CMD fail: %08Xh\n", u32_err);
        goto ErrorHandle;
    }
    #ifndef IP_FCIE_VERSION_5
    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_W(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
    #endif
    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_MIU_LAST_DONE|BIT_CARD_DMA_END, HW_TIMER_DELAY_100ms); // edit for MIU    
    #else
    u32_err = eMMC_FCIE_PollingEvents(FCIE_MIE_EVENT, BIT_DMA_END, HW_TIMER_DELAY_100ms);
    #endif

    if(u32_err) {
        goto ErrorHandle;
    }

    // check status
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

    if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR)) {

        u32_err = eMMC_ST_ERR_CMD21_NO_RSP;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: CMD21 no Rsp/CRC err, SD_STS: %04Xh \n", u16_reg);
        goto ErrorHandle;

    } else if(u16_reg &BIT_SD_R_CRC_ERR) {

        u32_err = eMMC_ST_ERR_CMD21_DATA_CRC;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Warn: CMD21 data CRC err, SD_STS: %04Xh \n", u16_reg);
        goto ErrorHandle;

    } else {
        if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_4)
        {
            if(memcmp((void*)hs200_tunning_pattern_64, (void*)gau8_eMMC_SectorBuf, 64)) {
                eMMC_debug(0, 0, "tuning pattern 4bit width:");
                for(i=0; i<64; i++) {
                    if(i%16==0) eMMC_debug(0, 0, "\n\t");
                    eMMC_debug(0, 0, "%02X ", gau8_eMMC_SectorBuf[i]);
                }
                eMMC_debug(0, 0, "\n");
                u32_err = eMMC_ST_ERR_CMD21_DATA_CMP;
                goto ErrorHandle;
            }
        }
        else if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_8)
        {
            if(memcmp((void*)hs200_tunning_pattern_128, (void*)gau8_eMMC_SectorBuf, 128)) {
                eMMC_debug(0, 0, "tuning pattern 8bit width:");
                for(i=0; i<128; i++) {
                    if(i%16==0) eMMC_debug(0, 0, "\n\t");
                    eMMC_debug(0, 0, "%02X ", gau8_eMMC_SectorBuf[i]);
                }
                eMMC_debug(0, 0, "\n");
                u32_err = eMMC_ST_ERR_CMD21_DATA_CMP;
                goto ErrorHandle;
            }
        }
    }

ErrorHandle:

    eMMC_DMA_UNMAP_address(u32_DataMapAddr, eMMC_SECTOR_512BYTE, 1);
    #ifndef IP_FCIE_VERSION_5
    REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
    REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | eMMC_SECTOR_512BYTE);
    #else
    REG_FCIE_W(FCIE_BLK_SIZE,  eMMC_SECTOR_512BYTE);
    #endif

    if(u32_err)
        eMMC_debug(0, 1, "eMMC err: %08Xh\n", u32_err);

    return u32_err;
}

// eMMC CMD21 adtc, R1, fix 128 clock, for HS200 only
//  64 bytes in 4 bits mode
// 128 bytes in 8 bits mode

U32 eMMC_CMD21(void) // send tuning block
{
    #if defined(ENABLE_eMMC_RIU_MODE)&&ENABLE_eMMC_RIU_MODE
    return eMMC_CMD21_CIFD(); // slow
    #else
    return eMMC_CMD21_MIU(); // fast
    #endif
}

#endif// ENABLE_eMMC_ATOP


