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
///
/// file    mhal_gflip.c
/// @brief  MStar GFLIP DDI HAL LEVEL
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _HAL_GFLIP_C

//=============================================================================
// Include Files
//=============================================================================
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>

#include "mdrv_mstypes.h"
#include "mhal_gflip_reg.h"
#include "mdrv_gflip_st.h"
#include "mhal_gflip.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Debug Macros
//=============================================================================
#define GFLIPHAL_DEBUG
#ifdef GFLIPHAL_DEBUG
    #define GFLIPHAL_PRINT(fmt, args...)      printk("[GFLIP (HAL Driver)][%05d] " fmt, __LINE__, ## args)
    #define GFLIPHAL_ASSERT(_cnd, _fmt, _args...)                   \
                                    if (!(_cnd)) {              \
                                        GFLIPHAL_PRINT(_fmt, ##_args);  \
                                        while(1);               \
                                    }
#else
    #define GFLIPHAL_PRINT(_fmt, _args...)
    #define GFLIPHAL_ASSERT(_cnd, _fmt, _args...)
#endif

//=============================================================================
// Macros
//=============================================================================
#define LO16BIT(x)  (x & 0x0000ffff)
#define HI16BIT(x)  (x >> 16)

#define  HDR_DBG_HAL(x)  //x
#define HDR_DEBUG_MODE   0


static volatile U16 g_u16HdrY2RRatio = 2;
static volatile U16 g_u16Hdr3x3Ratio = 8;

static volatile MS_U8 _u8PreColorPrimaries=0;
static volatile MS_U8 _u8PreTransferCharacteristics=0;

static volatile U8 MaxucTmp;
static volatile U16 MaxHistogram32H;
static volatile U16 _u16Count=0;
static volatile U16 u16PreRegs[18]={0};
static volatile MS_BOOL _bSetGenRead=FALSE;

static volatile MS_BOOL _bSetDLCHDRInit=FALSE;

//=============================================================================
// Local Variables
//=============================================================================
static volatile MS_U16 _u16GflipGOPDst[MAX_GOP_SUPPORT] = { GFLIP_GOP_DST_OP0 };

#if DLC_FUNCTION

static volatile StuDlc_FinetuneParamaters g_DlcParameters;
static volatile StuDlc_HDRinit g_HDRinitParameters;

static volatile WORD g_wLumaHistogram32H[32];

static volatile BOOL g_bSetDLCCurveBoth = 0;//We will set main/sub DLC curve at the same time in 3D side-side ap.
static volatile BOOL g_bSetDlcBleOn = TRUE; //TURE : Enable BLE   ; FALSE : Disnable BLE

static volatile BYTE g_ucHistogramMax, g_ucHistogramMin;
static volatile BYTE g_ucHistogramTotal_H;
static volatile BYTE g_ucHistogramTotal_L;
static volatile BYTE g_ucDlcFlickAlpha;
static volatile BYTE g_ucUpdateCnt;
static volatile BYTE g_ucDlcFastLoop; // for pulse ripple
static volatile BYTE g_ucTmpAvgN_1; // Avg value (N-1)
static volatile BYTE g_ucTmpAvgN; // Avg value (current N)
static volatile BYTE g_ucCurveYAvg[16];
static volatile BYTE g_ucTable[16]; // Final target curve (8 bit)
static volatile BYTE g_pre_ucTable[16]; // Final target curve (8 bit)

static volatile WORD g_wLumiAverageTemp;
static volatile WORD g_uwTable[16]; // Final target curve (10 bit)
static volatile WORD g_uwPre_CurveHistogram[16];
static volatile WORD g_uwPreTable[16]; // New de-Flick
static volatile WORD g_wTmpAvgN_1_x10; // Avg value (N-1) x 10
static volatile WORD g_wTmpAvgN_x10; // Avg value (current N) x 10
static volatile WORD g_wLumiTotalCount;
static volatile WORD g_wDisWinVstart=0, g_wDisWinVend=0 ,g_wDisWinHstart=0, g_wDisWinHend=0 ;
static volatile WORD g_wPIAlpha=0;
static volatile WORD g_wPrePIAlpha=0;
static volatile BYTE g_ucDLCInitBuffer[182];
static volatile DWORD g_uwCurveHistogram[16];

HDR_ToneMappingData HDRtmp; //YT ForHDR
WORD HDR_Curve[18]; //YT ForHDR


void msDlcWriteCurve(BOOL bWindow);

void msWriteByte(DWORD u32Reg, BYTE u8Val ) ;
BYTE msReadByte(DWORD u32Reg ) ;
#endif
//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================
MS_U32 MHal_GFLIP_GetValidGWinIDPerGOPIdx(MS_U32 u32GopIdx, MS_U32 u32GwinIdx);
MS_S32 _MHal_GFLIP_DirectSerialDiff(MS_U16 u16TagID1,  MS_U16 u16TagID2);
MS_BOOL _MHal_GFLIP_SetGopDstClk(MS_U32 u32GopIdx, MS_U16 u16GopDst);

//=============================================================================================
// Local Function
//=============================================================================================

//-------------------------------------------------------------------------------------------------
/// Set GOP Regs.
/// @param  u32GopIdx                  \b IN: The gop idx value
/// @param  u16BankIdx                  \b IN: The gop bank idx value, should be GFLIP_GOP_BANK_IDX_0/GFLIP_GOP_BANK_IDX_1
/// @return The gop bank offset value
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_U8 MHal_GFLIP_GetBankOffset(MS_U32 u32GopIdx, MS_U16 u16BankIdx)
{
    if(u32GopIdx<4)
     return (MS_U8)((GFLIP_GOP_BANKOFFSET * u32GopIdx)  + u16BankIdx);
    else
        return (MS_U8)((GFLIP_GOP_BANKOFFSET * u32GopIdx+2)  + u16BankIdx);
}

//-------------------------------------------------------------------------------------------------
/// Set GOP Regs.
/// @param  u32GopIdx                  \b IN: The gop idx value
/// @param  u16BankIdx                  \b IN: The gop bank idx value, should be GFLIP_GOP_BANK_IDX_0/GFLIP_GOP_BANK_IDX_1
/// @param  u16Addr                  \b IN: The gop reg address
/// @param  u16Val                  \b IN: The gop reg value
/// @param  u16Mask                  \b IN: The gop reg mask
/// @return void
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
void MHal_GFLIP_WriteGopReg(MS_U32 u32GopIdx, MS_U16 u16BankIdx, MS_U16 u16Addr, MS_U16 u16Val, MS_U16 u16Mask)
{
    MS_U16 u16RegVal;

   // GFLIPHAL_ASSERT(((u32GopIdx==GFLIP_GOP_IDX_4G)||(u32GopIdx==GFLIP_GOP_IDX_2G)), "Invalid GopIdx, Currently Only support Gop_4G and Gop_2G!\n");
    if (u32GopIdx < MAX_GOP_SUPPORT)
    {
        u16RegVal = GOP_REG(u16Addr, MHal_GFLIP_GetBankOffset(u32GopIdx, u16BankIdx));

        u16RegVal &= ~u16Mask;
        u16RegVal |= (u16Val & u16Mask);

        GOP_REG(u16Addr, MHal_GFLIP_GetBankOffset(u32GopIdx, u16BankIdx)) = u16RegVal;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get GOP Regs.
/// @param  u32GopIdx                  \b IN: The gop idx value
/// @param  u16BankIdx                  \b IN: The gop bank idx value, should be GFLIP_GOP_BANK_IDX_0/GFLIP_GOP_BANK_IDX_1
/// @param  u16Addr                  \b IN: The gop reg address
/// @param  *u16Val                  \b IN: The return gop reg value
/// @return void
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
void MHal_GFLIP_ReadGopReg(MS_U32 u32GopIdx, MS_U16 u16BankIdx, MS_U16 u16Addr, MS_U16* u16Val)
{
    if (u32GopIdx < MAX_GOP_SUPPORT)
    {
        *u16Val = GOP_REG(u16Addr, MHal_GFLIP_GetBankOffset(u32GopIdx, u16BankIdx));
    }
    else
    {
        *u16Val = 0;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set GOP Regs.
/// @param  u32GopIdx                  \b IN: The gop idx value
/// @param  u32GwinIdx                  \b IN: The gop gwin idx value,
/// @return MS_U32 The valid GWinID for related Gop
/// @attention
/// <b>[GFLIP] <em>GOP0: 0/1/2/3; GOP1: 0/1; GOP2: 1; GOP3: 1</em></b>
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_GFLIP_GetValidGWinIDPerGOPIdx(MS_U32 u32GopIdx, MS_U32 u32GwinIdx)
{
    switch(u32GopIdx)
    {
        case GFLIP_GOP_IDX_2G:
        case GFLIP_GOP_IDX_2GX:
            u32GwinIdx = u32GwinIdx % MAX_GOP_GWIN;
            break;
        case GFLIP_GOP_IDX_1G:
        case GFLIP_GOP_IDX_1GX:
            u32GwinIdx = 0; //only GWin 1 is valid;
            break;
        default:
            u32GwinIdx = 0;
            break;
    }

    return u32GwinIdx;
}

//-------------------------------------------------------------------------------------------------
/// Differ Two TagIDs.
/// @param  u16TagID1                  \b IN: The First TagID
/// @param  u16TagID2                  \b IN: The Second TagID
/// @return MS_S32: The different of two IDs.
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_S32 _MHal_GFLIP_DirectSerialDiff(MS_U16 u16TagID1,  MS_U16 u16TagID2)
{
    if(u16TagID1 < u16TagID2)
    {
        if((u16TagID2-u16TagID1)>0x7FFF)
         {
             return (MS_S32)(0xFFFFUL-u16TagID2+u16TagID1+1);
         }
        else
            return -(MS_S32)(u16TagID2-u16TagID1);
    }
    else
    {
        if((u16TagID1-u16TagID2)>0x7FFF)
         {
             return -(MS_S32)(0xFFFFUL-u16TagID1+u16TagID2+1);
         }
        else
            return (MS_S32)(u16TagID1-u16TagID2);
    }
}

//-------------------------------------------------------------------------------------------------
/// Get Gop Current Dst Plane.
/// @param  u32GopIdx                  \b IN: The gop idx who is the owner
/// @return TRUE/FALSE
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_U16 MHal_GFLIP_GetGopDst(MS_U32 u32GopIdx)
{
    MS_U16 u16RegVal;

    MHal_GFLIP_ReadGopReg(u32GopIdx, GFLIP_GOP_BANK_IDX_0, REG_GOP_CTRL1, &u16RegVal);

    return (u16RegVal & GOP_DST_MASK);
}

//-------------------------------------------------------------------------------------------------
/// Set Gop Dst Plane Clock Src.
/// @param  u16GopDst                  \b IN: The gop Dst Clock Src
/// @return TRUE/FALSE
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL _MHal_GFLIP_SetGopDstClk(MS_U32 u32GopIdx, MS_U16 u16GopDst)
{
    MS_U16 u16RegAddr, u16RegVal, u16RegMask;

    switch(u16GopDst)
    {
        case GFLIP_GOP_DST_IPMAIN:
            switch(u32GopIdx)
            {
                case GFLIP_GOP_IDX_2G:
                    u16RegAddr = GOP_GOPCLK;
                    u16RegVal = CKG_GOPG0_IDCLK2;
                    u16RegMask = CKG_GOPG0_MASK;
                    break;
                case GFLIP_GOP_IDX_2GX:
                    u16RegAddr = GOP_GOPCLK;
                    u16RegVal = CKG_GOPG1_IDCLK2;
                    u16RegMask = CKG_GOPG1_MASK;
                    break;
                case GFLIP_GOP_IDX_1G:
                    u16RegAddr = GOP_GOP2CLK;
                    u16RegVal = CKG_GOPG2_IDCLK2;
                    u16RegMask = CKG_GOPG2_MASK;
                    break;
                case GFLIP_GOP_IDX_1GX:
                    u16RegAddr = GOP_GOP3CLK;
                    u16RegVal = CKG_GOPG3_IDCLK2;
                    u16RegMask = CKG_GOPG3_MASK;
                    break;
                default:
                    return FALSE;
            }

            break;
        case GFLIP_GOP_DST_IPSUB:
            switch(u32GopIdx)
            {
                case GFLIP_GOP_IDX_2G:
                    u16RegAddr = GOP_GOPCLK;
                    u16RegVal = CKG_GOPG0_IDCLK1;
                    u16RegMask = CKG_GOPG0_MASK;
                    break;
                case GFLIP_GOP_IDX_2GX:
                    u16RegAddr = GOP_GOPCLK;
                    u16RegVal = CKG_GOPG1_IDCLK1;
                    u16RegMask = CKG_GOPG1_MASK;
                    break;
                case GFLIP_GOP_IDX_1G:
                    u16RegAddr = GOP_GOP2CLK;
                    u16RegVal = CKG_GOPG2_IDCLK1;
                    u16RegMask = CKG_GOPG2_MASK;
                    break;
                case GFLIP_GOP_IDX_1GX:
                    u16RegAddr = GOP_GOP3CLK;
                    u16RegVal = CKG_GOPG3_IDCLK1;
                    u16RegMask = CKG_GOPG3_MASK;
                    break;
                default:
                    return FALSE;
            }

            break;
        case GFLIP_GOP_DST_OP0:
            switch(u32GopIdx)
            {
                case GFLIP_GOP_IDX_2G:
                    u16RegAddr = GOP_GOPCLK;
                    u16RegVal = CKG_GOPG0_ODCLK;
                    u16RegMask = CKG_GOPG0_MASK;
                    break;
                case GFLIP_GOP_IDX_2GX:
                    u16RegAddr = GOP_GOPCLK;
                    u16RegVal = CKG_GOPG1_ODCLK;
                    u16RegMask = CKG_GOPG1_MASK;
                    break;
                case GFLIP_GOP_IDX_1G:
                    u16RegAddr = GOP_GOP2CLK;
                    u16RegVal = CKG_GOPG2_ODCLK;
                    u16RegMask = CKG_GOPG2_MASK;
                    break;
                case GFLIP_GOP_IDX_1GX:
                    u16RegAddr = GOP_GOP3CLK;
                    u16RegVal = CKG_GOPG3_ODCLK;
                    u16RegMask = CKG_GOPG3_MASK;
                    break;
                default:
                    return FALSE;
            }

            break;
        case GFLIP_GOP_DST_VOP:
        case GFLIP_GOP_DST_VOPSUB:
        default:
            return FALSE;
    }

    CKG_REG(u16RegAddr) = (CKG_REG(u16RegAddr) & ~u16RegMask) | u16RegVal;

    return TRUE;
}

//=============================================================================
// HAL Driver Function
//=============================================================================
//-------------------------------------------------------------------------------------------------
/// Enable/Disable Vsync. Interrupt of gop.
/// @param  u32GopIdx                  \b IN: The gop idx which need enable/disable vsync. interrupt
/// @param bEnable                          \b IN: enable or disable
/// @return TRUE/FALSE
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_IntEnable(MS_U32 u32GopIdx, MS_BOOL bEnable)
{
    MS_U16 u16RegVal;

    if(bEnable)
    {
        u16RegVal = GOP_INTMASK_VS1;
    }
    else
    {
        u16RegVal = GOP_INTMASK_VS0 | GOP_INTMASK_VS1;
    }

    MHal_GFLIP_WriteGopReg(u32GopIdx, GFLIP_GOP_BANK_IDX_0, REG_GOP_INT, u16RegVal, (GOP_INTMASK_VS0 | GOP_INTMASK_VS1));

    return TRUE;
}
MS_BOOL MHal_GFLIP_CheckVSyncInt(MS_U32 u32GopIdx)
{
	printk("\33[0;36m   %s:%d   u32GopIdx = %d,bank = %x,value = %x\33[m \n",__FUNCTION__,__LINE__,u32GopIdx,MHal_GFLIP_GetBankOffset(u32GopIdx, GFLIP_GOP_BANK_IDX_0),GOP_REG(REG_GOP_INT, MHal_GFLIP_GetBankOffset(u32GopIdx, GFLIP_GOP_BANK_IDX_0)) & GOP_INT_VS0);
}

//-------------------------------------------------------------------------------------------------
/// check if the vsync. interrupt
/// @param  void
/// @return MS_BOOL: TRUE/FALSE
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_IsVSyncInt(MS_U32 u32GopIdx)
{
    if(GOP_REG(REG_GOP_INT, MHal_GFLIP_GetBankOffset(u32GopIdx, GFLIP_GOP_BANK_IDX_0)) & GOP_INT_VS0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the Gop Idx who fire the vsync. interrupt
/// @param  void
/// @return MS_U32: the gop idx
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_GFLIP_GetIntGopIdx(void)
{
//have a lot of wrong setting
#if 0
    MS_U16 u16RegVal;

    u16RegVal = (GOP_REG(REG_GOP_BANK_SEL, 0) & GOP_INT_FLAG_MASK) | (GOP_REG(REG_GOP_MUX, 0) & GOPD_INT_FLAG_MASK);

    if(u16RegVal & GOP2G_INT_FLAG)
    {
        return GFLIP_GOP_IDX_2G;
    }
    else if(u16RegVal & GOP2GX_INT_FLAG)
    {
        return GFLIP_GOP_IDX_2GX;
    }
/*
    else if(u16RegVal & GOPD_INT_FLAG)
    {
        return GFLIP_GOP_IDX_DWIN;
    }
*/
    else if(u16RegVal & GOP1G_INT_FLAG)
    {
        return GFLIP_GOP_IDX_1G;
    }
    else if(u16RegVal & GOP1GX_INT_FLAG)
    {
        return GFLIP_GOP_IDX_1GX;
    }
    else
    {
         return GFLIP_GOP_IDX_INVALID;
        //GFLIPHAL_ASSERT(FALSE, "MHal_GFLIP_GetIntGopIdx: unknow GOP Interrupt MASK!\n");
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Check GOP ACK.
/// @param  u32GopIdx                  \b IN: The gop idx
/// @return TRUE: GOP ACK
/// @return FALSE: GOP not ACK
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_IsGOPACK(MS_U32 u32GopIdx)
{
    MS_U16 u16GOPWAck = GOP_REG(REG_GOP_WR_ACK, MHal_GFLIP_GetBankOffset(u32GopIdx, GFLIP_GOP_BANK_IDX_0));
    MS_BOOL bGOPACK = FALSE;
    switch(u32GopIdx)
    {
        case 0:
            if(u16GOPWAck&0x100)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        case 1:
            if(u16GOPWAck&0x200)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        case 2:
            if(u16GOPWAck&0x400)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        case 3:
            if(u16GOPWAck&0x800)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        case 4:
            if(u16GOPWAck&0x1000)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        case 5:
            if(u16GOPWAck&0x2000)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        case 6:
            if(u16GOPWAck&0x4000)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        case 7:
            if(u16GOPWAck&0x8000)
                bGOPACK = FALSE;
            else
                bGOPACK = TRUE;
            break;
        default:
            break;
    }
    //printk("<0>" "@-AK=%u->%x\n", bGOPACK, u16GOPWAck);
    return bGOPACK;
}

//-------------------------------------------------------------------------------------------------
/// Trigger GOP.
/// @param  u32GopBitMask                  \b IN: The gop idx bit mask
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
void MHal_GFLIP_Fire(MS_U32 u32GopBitMask)
{
    MS_U16 i = 0;
    MS_U16 u16GOPWAck = 0;
    MS_U8 u8BankOffset = 0;
    for(i = 0; i < MAX_GOP_SUPPORT; i++)
    {
        if(0 != (u32GopBitMask & (1 << i)))
        {
            u16GOPWAck = GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(i, GFLIP_GOP_BANK_IDX_0));
            u8BankOffset = MHal_GFLIP_GetBankOffset(i, GFLIP_GOP_BANK_IDX_0);
            if((u16GOPWAck & GOP_BK_WR) == 0)
            {
                GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(i, GFLIP_GOP_BANK_IDX_0)) = (u16GOPWAck & (~GOP_BANK_SEL)) | u8BankOffset | GOP_BK_WR;
                GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(i, GFLIP_GOP_BANK_IDX_0)) &= ~GOP_BK_WR;
                GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(i, GFLIP_GOP_BANK_IDX_0)) = (GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(i, GFLIP_GOP_BANK_IDX_0)) & (~GOP_BANK_SEL)) | (u16GOPWAck & GOP_BANK_SEL);//Restore bank
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
/// Kick off Flip to Gop HW.
/// @param  u32GopIdx                  \b IN: The gop idx who is the flip owner
/// @param  u32GwinIdx                 \b IN: The gwin idx who is the flip owner
/// @param  u32MainAddr                \b IN: The flip address for normal 2D display
/// @param  u32SubAddr                 \b IN: The 2nd fb flip address for GOP 3D display
/// @param  bForceWriteIn        \b IN: use forcewrite or bank wr
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_SetFlipToGop(MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_U32 u32MainAddr, MS_U32 u32SubAddr, MS_BOOL bForceWriteIn)
{
    MS_U16 u16GOPWAck = GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(u32GopIdx, GFLIP_GOP_BANK_IDX_0));

    u32GwinIdx = MHal_GFLIP_GetValidGWinIDPerGOPIdx(u32GopIdx, u32GwinIdx);

    MHal_GFLIP_WriteGopReg(u32GopIdx, GFLIP_GOP_BANK_IDX_1, REG_GOP_DRAM_RBLK_STR_L(u32GwinIdx), LO16BIT(u32MainAddr), 0xFFFF);
    MHal_GFLIP_WriteGopReg(u32GopIdx, GFLIP_GOP_BANK_IDX_1, REG_GOP_DRAM_RBLK_STR_H(u32GwinIdx), HI16BIT(u32MainAddr), 0xFFFF);
    if(u32SubAddr != 0)
    {
        MHal_GFLIP_WriteGopReg(u32GopIdx, GFLIP_GOP_BANK_IDX_1, REG_GOP_3DOSD_SUB_RBLK_L(u32GwinIdx), LO16BIT(u32SubAddr), 0xFFFF);
        MHal_GFLIP_WriteGopReg(u32GopIdx, GFLIP_GOP_BANK_IDX_1, REG_GOP_3DOSD_SUB_RBLK_H(u32GwinIdx), HI16BIT(u32SubAddr), 0xFFFF);
    }

    if(bForceWriteIn)
    {
        if((u16GOPWAck & GOP_FWR) == 0)
        {
             GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(u32GopIdx, GFLIP_GOP_BANK_IDX_0)) |= GOP_FWR;
             GOP_REG(REG_GOP_BANK_SEL, MHal_GFLIP_GetBankOffset(u32GopIdx, GFLIP_GOP_BANK_IDX_0)) &= ~GOP_FWR;
        }
    }
    else
    {
        MHal_GFLIP_Fire(1 << u32GopIdx);
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Clear DWinIRQ if It happened.
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_ClearDWINIRQ(GFLIP_DWININT_INFO *pGFlipDWinIntInfo)
{
    MS_U16 u16ValEn, u16ValDe;

    if(!(GOP_REG(REG_GOP_BANK_SEL, 0) & GOPD_INT_FLAG))
    {
        return FALSE;
    }

    u16ValEn = u16ValDe = GOP_REG(REG_GOP_DWIN_INT, GFLIP_GOP_DWIN_BANKID);

    //Check and Clear DWin IRQs:
    if(u16ValEn & GOP_DWIN_INT_WADR)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_WADR;
        u16ValDe |= GOP_DWIN_INTMASK_WADR;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntWADR = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_PROG)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_PROG;
        u16ValDe |= GOP_DWIN_INTMASK_PROG;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntPROG = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_TF)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_TF;
        u16ValDe |= GOP_DWIN_INTMASK_TF;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntTF = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_BF)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_BF;
        u16ValDe |= GOP_DWIN_INTMASK_BF;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntBF = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_VS)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_VS;
        u16ValDe |= GOP_DWIN_INTMASK_VS;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntVS = 0x1;
    }

    //Disable Interrupt:
    GOP_REG(REG_GOP_DWIN_INT, GFLIP_GOP_DWIN_BANKID) = u16ValDe;
    //Enable Interrupt:
    GOP_REG(REG_GOP_DWIN_INT, GFLIP_GOP_DWIN_BANKID) = u16ValEn;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Handle HW Limitation -- Vsync Limitation.
/// @param  u32GopIdx                  \b IN: The gop idx who is the owner
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em>The Limitation is found on U3/T2/T3. When GOP Src Clock is no signal and GOP
///     Vsync is on, Then there will be no chance to Reset GOP Vsync Interrupt. this will leads to
///     system looks like hang(busy handling interrupt always). T2/T3/U3 will Switch GOP Dst
///     to OP to fix the issue and later chips could Reset GOP to clear interrupt Bit.</em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_HandleVsyncLimitation(MS_U32 u32GopIdx)
{
    //Save Current Dst Clock Src:
    _u16GflipGOPDst[u32GopIdx] = MHal_GFLIP_GetGopDst(u32GopIdx);

    //Set Dst Clock Src to OP:
    _MHal_GFLIP_SetGopDstClk(u32GopIdx, GFLIP_GOP_DST_OP0);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Restore HW Limitation -- Vsync Limitation.
/// @param  u32GopIdx                  \b IN: The gop idx who is the owner
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em>The Limitation is found on U3/T2/T3. When GOP Src Clock is no signal and GOP
///     Vsync is on, Then there will be no chance to Reset GOP Vsync Interrupt. this will leads to
///     system looks like hang(busy handling interrupt always). T2/T3/U3 will Switch GOP Dst
///     to OP to fix the issue and later chips could Reset GOP to clear interrupt Bit.</em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_RestoreFromVsyncLimitation(MS_U32 u32GopIdx)
{
    //Restore Dst Clock Src:
    _MHal_GFLIP_SetGopDstClk(u32GopIdx, _u16GflipGOPDst[u32GopIdx]);

    //Restore Interrupt:
    MHal_GFLIP_IntEnable(u32GopIdx, TRUE);

    //Restore Current Dst Clock Src to OP:
    _u16GflipGOPDst[u32GopIdx] = GFLIP_GOP_DST_OP0;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Check If the gived TagId has back or not.
/// @param  u32TagId                  \b IN: The TagId which need to be check
/// @return TRUE: back
/// @return FALSE: not back
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_IsTagIDBack(MS_U16 u16TagId)
{
    MS_U16 u16TagIDHW;
    MS_U32 u32RegVal;

    u16TagIDHW = GE_REG(REG_GE_TAG);

    if(_MHal_GFLIP_DirectSerialDiff(u16TagIDHW, u16TagId) >= 0)
    {    //TagID is back!
          // printk("tagIDHW = %04x\n", tagID_HW);
        return TRUE;
    }

    u32RegVal = GE_REG(REG_GE_STATUS);
    if((u32RegVal & GE_CMDQ2_STATUS) < (16UL<<11)) //still has commands in queue.
    {
        return FALSE;
    }

    if((u32RegVal & GE_CMDQ1_STATUS) < (16UL<<3)) //still has commands in queue.
    {
        return FALSE;
    }

    if(GE_REG(REG_GE_FMT_BLT) & GE_EN_VCMDQ) //still has commands in VCMDQ.
    {
        u32RegVal = GE_REG(REG_GE_VQ_FIFO_STATUS_L);
        u32RegVal |= (GE_REG(REG_GE_VQ_FIFO_STATUS_H)&1)<<16;
        if(u32RegVal)
        {
            return FALSE;
        }
    }

    if(GE_REG(REG_GE_STATUS) & GE_BUSY) //GE still busy
    {
        return FALSE;
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable or Disable VE capture function
/// @param  bEnable                  \b IN: TRUE: Enable, FALSE: Disable
/// @return TRUE: success
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_VECaptureEnable(MS_BOOL bEnable)
{
    if(bEnable)
    {
        VE_REG(REG_VE_CTRL) = VE_REG(REG_VE_CTRL) | BIT_VE_RESET;
        VE_REG(REG_VE_CTRL) = VE_REG(REG_VE_CTRL) | BIT_VE_REG_RELOAD;
        //need delay ???
        VE_REG(REG_VE_CTRL) = VE_REG(REG_VE_CTRL) & ~BIT_VE_REG_RELOAD;
        VE_REG(REG_VE_CTRL) = VE_REG(REG_VE_CTRL) & ~BIT_VE_RESET;
        VE_REG(REG_VE_CTRL) = VE_REG(REG_VE_CTRL) | BIT_VE_ENABLE;
    }
    else
    {
        VE_REG(REG_VE_CTRL) = VE_REG(REG_VE_CTRL) & ~BIT_VE_ENABLE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get the current frame number that the VE HW is writing in VE memory
/// For interlace mode, it is 0~3; for progressive mode, it is 0~2 .
/// @return TRUE: VE frame index of current HW writing
//-------------------------------------------------------------------------------------------------
MS_U8 MHal_GFLIP_GetFrameIdx(void)
{
    return ((VE_REG(REG_VE_COUNTER) & BIT_VE_FIELD_IDX)>>BIT_VE_FIELD_IDX_SHIFT_BITS); //HW is count from 0
}

//-------------------------------------------------------------------------------------------------
/// Check if VE HW is ready to downscaling data to memory(After VE reset)
/// @return TRUE: VE has start to downscaling
/// @return FALSE: VE is not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_CheckVEReady(void)
{
    if(VE_REG(REG_VE_STATE) & BIT_VE_START_DOWNSCALING)
    {
        if(MHal_GFLIP_GetFrameIdx() > 0)
        {
            //VE has finished capture at least one or two frames, take this as VE ready.
            return TRUE;
        }
        else
        {
            //Skip the first frame to avoid return a frame idex of garbage data.
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

#if DLC_FUNCTION
//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msWriteByte(DWORD u32Reg, BYTE u8Val )
{
    if(u32Reg%2)
    {
        REG_WH((u32Reg-1), u8Val);
    }
    else
    {
        REG_WL(u32Reg, u8Val);
    }
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BYTE msReadByte(DWORD u32Reg )
{
    if(u32Reg%2)
    {
        u32Reg = u32Reg-1 ;
        return ((REG_RR(u32Reg) & 0xFF00)>>8);
    }
    else
    {
        return (REG_RR(u32Reg) & 0xFF);
    }
}


//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void MApi_GFLIP_XC_W2BYTE(DWORD u32Reg, WORD u16Val )
{
    REG_W2B(u32Reg, u16Val);
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
WORD MApi_GFLIP_XC_R2BYTE(DWORD u32Reg )
{
    return REG_RR(u32Reg) ;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void MApi_GFLIP_XC_W2BYTEMSK(DWORD u32Reg, WORD u16Val, WORD u16Mask )
{
    WORD u16Data=0 ;
    u16Data = REG_RR(u32Reg);
    u16Data = (u16Data & (0xFFFF-u16Mask))|(u16Val &u16Mask);
    REG_W2B(u32Reg, u16Data);
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
WORD MApi_GFLIP_XC_R2BYTEMSK(DWORD u32Reg, WORD u16Mask )
{
    return (REG_RR(u32Reg) & u16Mask);
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msDlcSetCurve(MS_U8 *_u8DlcCurveInit)
{
    BYTE ucTmp;

    for(ucTmp=0; ucTmp<16; ucTmp++)
    {
        g_DlcParameters.ucLumaCurve[ucTmp]=_u8DlcCurveInit[ucTmp];
        g_DlcParameters.ucLumaCurve2_a[ucTmp]=_u8DlcCurveInit[ucTmp+16];
        g_DlcParameters.ucLumaCurve2_b[ucTmp]=_u8DlcCurveInit[ucTmp+32];
        g_ucDLCInitBuffer[70+ucTmp]=_u8DlcCurveInit[ucTmp];
        g_ucDLCInitBuffer[86+ucTmp]=_u8DlcCurveInit[ucTmp+16];
        g_ucDLCInitBuffer[102+ucTmp]=_u8DlcCurveInit[ucTmp+32];
    }

    for(ucTmp=0; ucTmp<17; ucTmp++)
    {
        g_DlcParameters.ucDlcHistogramLimitCurve[ucTmp]=_u8DlcCurveInit[ucTmp+48];
        g_ucDLCInitBuffer[37+ucTmp]=_u8DlcCurveInit[ucTmp+48];
    }
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msDlcSetBleSlopPoint(MS_U16 *_u16BLESlopPoint)
{
    g_DlcParameters.uwDlcBLESlopPoint_1 = _u16BLESlopPoint[0];
    g_DlcParameters.uwDlcBLESlopPoint_2 = _u16BLESlopPoint[1];
    g_DlcParameters.uwDlcBLESlopPoint_3 = _u16BLESlopPoint[2];
    g_DlcParameters.uwDlcBLESlopPoint_4 = _u16BLESlopPoint[3];
    g_DlcParameters.uwDlcBLESlopPoint_5 = _u16BLESlopPoint[4];
    g_DlcParameters.uwDlcDark_BLE_Slop_Min = _u16BLESlopPoint[5];

    g_ucDLCInitBuffer[7]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_1 &0xFF);
    g_ucDLCInitBuffer[8]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_1>>8) &0xFF);
    g_ucDLCInitBuffer[9]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_2 &0xFF);
    g_ucDLCInitBuffer[10]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_2>>8) &0xFF);
    g_ucDLCInitBuffer[11]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_3 &0xFF);
    g_ucDLCInitBuffer[12]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_3>>8) &0xFF);
    g_ucDLCInitBuffer[13]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_4 &0xFF);
    g_ucDLCInitBuffer[14]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_4>>8) &0xFF);
    g_ucDLCInitBuffer[15]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_5 &0xFF);
    g_ucDLCInitBuffer[16]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_5>>8) &0xFF);
    g_ucDLCInitBuffer[17]=(BYTE)(g_DlcParameters.uwDlcDark_BLE_Slop_Min &0xFF);
    g_ucDLCInitBuffer[18]=(BYTE)((g_DlcParameters.uwDlcDark_BLE_Slop_Min>>8) &0xFF);

}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BOOL msDlcInit(MS_U16 *u16DlcInit )
{
    BYTE ucTmp=0;

    g_DlcParameters.ucDlcPureImageMode     = u16DlcInit[10]; // Compare difference of max and min bright
    g_DlcParameters.ucDlcLevelLimit        = u16DlcInit[11]; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    g_DlcParameters.ucDlcAvgDelta          = u16DlcInit[12]; // n = 0 ~ 50, default value: 12
    g_DlcParameters.ucDlcAvgDeltaStill     = u16DlcInit[13]; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15: enable still curve
    g_DlcParameters.ucDlcFastAlphaBlending = u16DlcInit[14]; // min 17 ~ max 32
    g_DlcParameters.ucDlcYAvgThresholdL    = u16DlcInit[15]; // default value: 0
    g_DlcParameters.ucDlcYAvgThresholdH    = u16DlcInit[16]; // default value: 128
    g_DlcParameters.ucDlcBLEPoint          = u16DlcInit[17]; // n = 24 ~ 64, default value: 48
    g_DlcParameters.ucDlcWLEPoint          = u16DlcInit[18]; // n = 24 ~ 64, default value: 48
    g_DlcParameters.bEnableBLE             = u16DlcInit[19]; // 1: enable; 0: disable
    g_DlcParameters.bEnableWLE             = u16DlcInit[20]; // 1: enable; 0: disable
    g_DlcParameters.ucDlcYAvgThresholdM    = u16DlcInit[21];
    g_DlcParameters.ucDlcCurveMode         = u16DlcInit[22];
    g_DlcParameters.ucDlcCurveModeMixAlpha = u16DlcInit[23];
    g_DlcParameters.ucDlcAlgorithmMode     = u16DlcInit[24];
    g_DlcParameters.ucDlcSepPointH         = u16DlcInit[25];
    g_DlcParameters.ucDlcSepPointL         = u16DlcInit[26];
    g_DlcParameters.uwDlcBleStartPointTH   = u16DlcInit[27];
    g_DlcParameters.uwDlcBleEndPointTH     = u16DlcInit[28];
    g_DlcParameters.ucDlcCurveDiff_L_TH    = u16DlcInit[29];
    g_DlcParameters.ucDlcCurveDiff_H_TH    = u16DlcInit[30];
    g_DlcParameters.uwDlcBLESlopPoint_1    = u16DlcInit[31];
    g_DlcParameters.uwDlcBLESlopPoint_2    = u16DlcInit[32];
    g_DlcParameters.uwDlcBLESlopPoint_3    = u16DlcInit[33];
    g_DlcParameters.uwDlcBLESlopPoint_4    = u16DlcInit[34];
    g_DlcParameters.uwDlcBLESlopPoint_5    = u16DlcInit[35];
    g_DlcParameters.uwDlcDark_BLE_Slop_Min = u16DlcInit[36];
    g_DlcParameters.ucDlcCurveDiffCoringTH = u16DlcInit[37];
    g_DlcParameters.ucDlcAlphaBlendingMin  = u16DlcInit[38];
    g_DlcParameters.ucDlcAlphaBlendingMax  = u16DlcInit[39];
    g_DlcParameters.ucDlcFlicker_alpha     = u16DlcInit[40];
    g_DlcParameters.ucDlcYAVG_L_TH         = u16DlcInit[41];
    g_DlcParameters.ucDlcYAVG_H_TH         = u16DlcInit[42];
    g_DlcParameters.ucDlcDiffBase_L        = u16DlcInit[43];
    g_DlcParameters.ucDlcDiffBase_M        = u16DlcInit[44];
    g_DlcParameters.ucDlcDiffBase_H        = u16DlcInit[45];

    g_ucDLCInitBuffer[1]=(BYTE)(g_DlcParameters.ucDlcSepPointH &0xFF);
    g_ucDLCInitBuffer[2]=(BYTE)(g_DlcParameters.ucDlcSepPointL &0xFF);
    g_ucDLCInitBuffer[3]=(BYTE)(g_DlcParameters.uwDlcBleStartPointTH &0xFF);
    g_ucDLCInitBuffer[4]=(BYTE)((g_DlcParameters.uwDlcBleStartPointTH>>8) &0xFF);
    g_ucDLCInitBuffer[5]=(BYTE)(g_DlcParameters.uwDlcBleEndPointTH &0xFF);
    g_ucDLCInitBuffer[6]=(BYTE)((g_DlcParameters.uwDlcBleEndPointTH>>8) &0xFF);
    g_ucDLCInitBuffer[7]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_1 &0xFF);
    g_ucDLCInitBuffer[8]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_1>>8) &0xFF);
    g_ucDLCInitBuffer[9]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_2 &0xFF);
    g_ucDLCInitBuffer[10]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_2>>8) &0xFF);
    g_ucDLCInitBuffer[11]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_3 &0xFF);
    g_ucDLCInitBuffer[12]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_3>>8) &0xFF);
    g_ucDLCInitBuffer[13]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_4 &0xFF);
    g_ucDLCInitBuffer[14]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_4>>8) &0xFF);
    g_ucDLCInitBuffer[15]=(BYTE)(g_DlcParameters.uwDlcBLESlopPoint_5 &0xFF);
    g_ucDLCInitBuffer[16]=(BYTE)((g_DlcParameters.uwDlcBLESlopPoint_5>>8) &0xFF);
    g_ucDLCInitBuffer[17]=(BYTE)(g_DlcParameters.uwDlcDark_BLE_Slop_Min &0xFF);
    g_ucDLCInitBuffer[18]=(BYTE)((g_DlcParameters.uwDlcDark_BLE_Slop_Min>>8) &0xFF);
    g_ucDLCInitBuffer[19]=g_DlcParameters.ucDlcAlphaBlendingMin;
    g_ucDLCInitBuffer[20]=g_DlcParameters.ucDlcAlphaBlendingMax;
    g_ucDLCInitBuffer[21]=g_DlcParameters.ucDlcFlicker_alpha;
    g_ucDLCInitBuffer[22]=g_DlcParameters.ucDlcYAVG_L_TH;
    g_ucDLCInitBuffer[23]=g_DlcParameters.ucDlcYAVG_H_TH;
    g_ucDLCInitBuffer[24]=g_DlcParameters.ucDlcDiffBase_L;
    g_ucDLCInitBuffer[25]=g_DlcParameters.ucDlcDiffBase_M;
    g_ucDLCInitBuffer[26]=g_DlcParameters.ucDlcDiffBase_H;
    g_ucDLCInitBuffer[27]=g_DlcParameters.ucDlcCurveDiff_L_TH;
    g_ucDLCInitBuffer[28]=g_DlcParameters.ucDlcCurveDiff_H_TH;
    g_ucDLCInitBuffer[29]=g_DlcParameters.ucDlcYAvgThresholdL;
    g_ucDLCInitBuffer[30]=g_DlcParameters.ucDlcYAvgThresholdM;
    g_ucDLCInitBuffer[31]=g_DlcParameters.ucDlcYAvgThresholdH;
    g_ucDLCInitBuffer[32]=g_DlcParameters.ucDlcCurveMode;
    g_ucDLCInitBuffer[33]=g_DlcParameters.ucDlcCurveModeMixAlpha;

    for (ucTmp = 0  ; ucTmp < 32; ucTmp++)
    {
        msWriteByte((REG_SC_BK30_02_L+ucTmp), 0 );
    }

    //Mantis issue of 0232938:Hsense A3 DLC the divisor can't to be equal to zero .
    if((g_DlcParameters.ucDlcYAvgThresholdH-g_DlcParameters.ucDlcYAvgThresholdL)==0)
    {
        DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  (ucDlcYAvgThresholdH - ucDlcYAvgThresholdL)=0 !!!\n", __FUNCTION__,__LINE__));
        return FALSE;
    }

    if(( g_DlcParameters.ucDlcYAvgThresholdM - g_DlcParameters.ucDlcYAvgThresholdL)==0)
    {
        DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  (ucDlcYAvgThresholdM - ucDlcYAvgThresholdL)=0 !!!\n", __FUNCTION__,__LINE__));
        return FALSE;
    }

    if((g_DlcParameters.ucDlcYAvgThresholdH - g_DlcParameters.ucDlcYAvgThresholdM)==0)
    {
        DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  (ucDlcYAvgThresholdH - ucDlcYAvgThresholdM)=0 !!!\n", __FUNCTION__,__LINE__));
        return FALSE;
    }

    if((g_DlcParameters.ucDlcCurveDiff_H_TH - g_DlcParameters.ucDlcCurveDiff_L_TH)==0)
    {
        DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  (ucDlcCurveDiff_H_TH - ucDlcCurveDiff_L_TH)=0 !!!\n", __FUNCTION__,__LINE__));
        return FALSE;
    }

    if(g_DlcParameters.ucDlcBLEPoint==0)
    {
        DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  ucDlcBLEPoint=0 !!!\n", __FUNCTION__,__LINE__));
        return FALSE;
    }

    if(g_DlcParameters.ucDlcWLEPoint==0)
    {
        DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  ucDlcWLEPoint=0 !!!\n", __FUNCTION__,__LINE__));
        return FALSE;
    }

    if(( g_DlcParameters.ucDlcAlphaBlendingMax- g_DlcParameters.ucDlcAlphaBlendingMin)==0)
    {
        DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  (ucDlcAlphaBlendingMax - ucDlcAlphaBlendingMin)=0 !!!\n", __FUNCTION__,__LINE__));
        return FALSE;
    }

    return TRUE ;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msDlcSetBleOnOff(BOOL bSwitch)
{
    g_bSetDlcBleOn= bSwitch ;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BOOL msDlcGetHistogram32Info(MS_U16 *pu16Histogram)
{
    MS_U8 ucTmp;

    for(ucTmp=0; ucTmp<32; ucTmp++)
    {
        pu16Histogram[ucTmp] = g_wLumaHistogram32H[ucTmp];
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
MS_U8 msGetAverageLuminous(void) // = external msGetAverageLuminousValue()
{
    WORD uwTmpAvr;

    // Normalize
    if (g_wLumiTotalCount!=0)
        uwTmpAvr = (256 * (DWORD)g_wLumiAverageTemp + (g_wLumiTotalCount/2)) / g_wLumiTotalCount;
    else
        uwTmpAvr = 0;

    if (uwTmpAvr >= 255)
        uwTmpAvr = 255;
    return (BYTE)uwTmpAvr;
}

MS_U8 msGetAverageLuminousSub(void)
{
    WORD uwTmpAvr=0, uwLumiTotalCount=0, uwLumiAverageTemp=0 ;

    uwLumiTotalCount = msReadByte(REG_ADDR_HISTOGRAM_SUB_TOTAL_COUNT_H);
    uwLumiTotalCount = (uwLumiTotalCount << 8) + (WORD)msReadByte(REG_ADDR_HISTOGRAM_SUB_TOTAL_COUNT_L);

    uwLumiAverageTemp = msReadByte(REG_ADDR_HISTOGRAM_SUB_TOTAL_SUM_H);
    uwLumiAverageTemp = (uwLumiAverageTemp << 8) + (WORD)msReadByte(REG_ADDR_HISTOGRAM_SUB_TOTAL_SUM_L);

    // Normalize
    if (uwLumiTotalCount!=0)
        uwTmpAvr = (256 * (DWORD)uwLumiAverageTemp + (uwLumiTotalCount/2)) / uwLumiTotalCount;
    else
        uwTmpAvr = 0;

    if (uwTmpAvr >= 255)
        uwTmpAvr = 255;

    return (BYTE)uwTmpAvr;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
WORD msGetAverageLuminous_x10(void)
{
    WORD uwTmpAvr;

    // Normalize
    if (g_wLumiTotalCount!=0)
        uwTmpAvr = (256 * 10 * (DWORD)g_wLumiAverageTemp + (g_wLumiTotalCount/2)) / g_wLumiTotalCount;
    else
        uwTmpAvr = 0;

    if (uwTmpAvr >= (256 * 10 - 1))
        uwTmpAvr = (256 * 10 - 1);

    return uwTmpAvr;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BOOL msGetHistogramHandler(BOOL bWindow)
{
    BYTE ucTmp;
    WORD wTmpHistogramSum; // for protect histogram overflow
    BYTE ucHistogramOverflowIndex;
    BYTE ucReturn;

    //When DLC algorithm is not from kernel, force return false.
    if( g_DlcParameters.ucDlcAlgorithmMode != XC_DLC_ALGORITHM_KERNEL )
    {
        return FALSE;
    }

    msDlc_FunctionEnter();

#if 0 //Have double buffer, does not need to wait.
    // ENABLE_FAST_HISTOGRAM_CATCH
    if( !(msReadByte(REG_ADDR_DLC_HANDSHAKE) & _BIT3) )  // Wait until ready
    {
        msDlc_FunctionExit();
        return FALSE;
    }
#endif

    if (g_DlcParameters.ucDlcTimeOut)
    {
        // Get histogram 32 data
        // Total/BW_ref histogram count
        MaxHistogram32H=0;
        for (ucTmp = 0, wTmpHistogramSum = 0, ucHistogramOverflowIndex = 0xFF; ucTmp < 32; ucTmp++)
        {
            g_wLumaHistogram32H[ucTmp] = msReadByte(REG_ADDR_HISTOGRAM_DATA_32+(2*ucTmp)+1);
            g_wLumaHistogram32H[ucTmp] <<= 8;
            g_wLumaHistogram32H[ucTmp] |= msReadByte(REG_ADDR_HISTOGRAM_DATA_32+(2*ucTmp));

            //DLC Bank 0x1A . Check for MSTV_Tool debug.
            if((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0xFF))==0x1A)
            {
                g_ucDLCInitBuffer[118+ucTmp*2]=msReadByte(REG_ADDR_HISTOGRAM_DATA_32+(2*ucTmp));
                g_ucDLCInitBuffer[119+ucTmp*2]=msReadByte(REG_ADDR_HISTOGRAM_DATA_32+(2*ucTmp)+1);
            }

            // protect histogram overflow
            if (g_wLumaHistogram32H[ucTmp] >= 65535)
            {
                ucHistogramOverflowIndex = ucTmp;
            }
            g_wLumaHistogram32H[ucTmp] >>= 1; // protect histogram overflow
            wTmpHistogramSum += g_wLumaHistogram32H[ucTmp];

            if (g_wLumaHistogram32H[ucTmp]>MaxHistogram32H) //YT ForHDR
            {
                MaxHistogram32H=g_wLumaHistogram32H[ucTmp];
                MaxucTmp=ucTmp;
            }

        }

        g_wLumiTotalCount = ((WORD)msReadByte(REG_ADDR_HISTOGRAM_TOTAL_COUNT_H) << 8) + msReadByte(REG_ADDR_HISTOGRAM_TOTAL_COUNT_L);

        // protect histogram overflow
        if (ucHistogramOverflowIndex != 0xFF)
        {
            if (wTmpHistogramSum > g_wLumiTotalCount)
                wTmpHistogramSum = g_wLumiTotalCount;
            g_wLumaHistogram32H[ucHistogramOverflowIndex] += (g_wLumiTotalCount - wTmpHistogramSum);
        }

        g_ucHistogramTotal_L = msReadByte( REG_ADDR_HISTOGRAM_TOTAL_SUM_L );
        g_ucHistogramTotal_H = msReadByte( REG_ADDR_HISTOGRAM_TOTAL_SUM_H );

        // Get MIN. & MAX VALUE
        if( MAIN_WINDOW == bWindow )   // Main window
        {
            g_ucHistogramMin = msReadByte(REG_ADDR_MAIN_MIN_VALUE);
            g_ucHistogramMax = msReadByte(REG_ADDR_MAIN_MAX_VALUE);
        }
        else // Sub window
        {
            g_ucHistogramMin = msReadByte(REG_ADDR_SUB_MIN_VALUE);
            g_ucHistogramMax = msReadByte(REG_ADDR_SUB_MAX_VALUE);
        }

        g_ucTmpAvgN_1 = msGetAverageLuminous();

        //DLC Bank 0x1A . Check for MSTV_Tool debug.
        if((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0xFF))==0x1A)
        {
            g_ucDLCInitBuffer[34]=g_ucHistogramMin;
            g_ucDLCInitBuffer[35]=g_ucHistogramMax;
            g_ucDLCInitBuffer[36]=g_ucTmpAvgN_1;
        }

        g_wTmpAvgN_1_x10 = msGetAverageLuminous_x10();

        g_wLumiAverageTemp = ((WORD)g_ucHistogramTotal_H<<8) + g_ucHistogramTotal_L;
        g_ucTmpAvgN = msGetAverageLuminous();
        g_wTmpAvgN_x10 = msGetAverageLuminous_x10();

        ucReturn = TRUE;
    }
    else
    {
        ucReturn = FALSE;
    }

    // After read histogram, request HW to do histogram
    // Request HW to do histogram
    msWriteByte(REG_ADDR_DLC_HANDSHAKE, msReadByte(REG_ADDR_DLC_HANDSHAKE) & 0xF2);

    g_DlcParameters.ucDlcTimeOut = 150; // 150ms

    msWriteByte(REG_ADDR_DLC_HANDSHAKE, msReadByte(REG_ADDR_DLC_HANDSHAKE) | _BIT2 );

    msDlc_FunctionExit();

    return ucReturn;
}

//=========================================================
//
//=========================================================
void msDlcLumiDiffCtrl(void)
{
    if (g_DlcParameters.ucDlcFlickAlphaStart)
    {
        g_DlcParameters.ucDlcFlickAlphaStart--;
    }

    if (g_DlcParameters.ucDlcSlowEvent)
    {
        g_DlcParameters.ucDlcSlowEvent--;
        g_ucDlcFlickAlpha = 1; // it mean the curve change slowly
        g_ucDlcFastLoop = 0;
    }
    else
    {
        if (g_wTmpAvgN_1_x10 >= g_wTmpAvgN_x10)
        {
            if ((g_ucDlcFastLoop > 0) || ((g_wTmpAvgN_1_x10 - g_wTmpAvgN_x10 ) >= ((WORD)g_DlcParameters.ucDlcAvgDelta * 10)))
            {
                g_ucDlcFlickAlpha = g_DlcParameters.ucDlcFastAlphaBlending; // (32/32) = (32/DLC_DEFLICK_BLEND_FACTOR) // it mean the curve change quickly
                if (g_ucDlcFastLoop == 0)
                {
                    g_ucDlcFastLoop = (g_DlcParameters.ucDlcFastAlphaBlending/2) * 16;
                    if (g_DlcParameters.ucDlcFastAlphaBlending == 32) // 32 / 2 * 16 = 0 overflow
                    {
                        g_ucDlcFastLoop = 255;
                    }
                }
            }
            else if ((g_DlcParameters.ucDlcAvgDeltaStill <= ((WORD)g_DlcParameters.ucDlcAvgDelta * 10)) && (g_DlcParameters.ucDlcAvgDeltaStill != 0) && ((g_wTmpAvgN_1_x10 - g_wTmpAvgN_x10) <= g_DlcParameters.ucDlcAvgDeltaStill)) // if (Yavg is less than g_DlcParameters.ucDlcAvgDeltaStill) & (Fast change is finish) does NOT do curve change
            {
                g_ucDlcFlickAlpha = 0; // (0/32) = (1/DLC_DEFLICK_BLEND_FACTOR)  // it mean the curve will be still
            }
            else // if ((g_ucTmpAvgN_1 - g_ucTmpAvgN) < g_DlcParameters.ucDlcAvgDelta) // if (Yavg is less than g_DlcParameters.ucDlcAvgDelta) & (Fast change is finish) may do curve change slowly
            {
                if ( g_DlcParameters.ucDlcFlickAlphaStart == 0) // chagne source and it's stable
                {
                    g_ucDlcFlickAlpha = 1; // (1/32) = (1/DLC_DEFLICK_BLEND_FACTOR)  // it mean the curve change slowly
                }
                else
                {
                    g_ucDlcFlickAlpha = g_DlcParameters.ucDlcFastAlphaBlending; // (32/32) = (32/DLC_DEFLICK_BLEND_FACTOR) // it mean the curve change quickly
                }
            }
        }
        else if (g_wTmpAvgN_1_x10 < g_wTmpAvgN_x10)
        {
             if ((g_ucDlcFastLoop > 0) || (( g_wTmpAvgN_x10 - g_wTmpAvgN_1_x10 ) >= ((WORD)g_DlcParameters.ucDlcAvgDelta * 10)))
            {
                g_ucDlcFlickAlpha = g_DlcParameters.ucDlcFastAlphaBlending; // (32/32) = (32/DLC_DEFLICK_BLEND_FACTOR)  // it mean the curve change quickly
                if (g_ucDlcFastLoop == 0)
                {
                    g_ucDlcFastLoop = (g_DlcParameters.ucDlcFastAlphaBlending/2) * 16;
                    if (g_DlcParameters.ucDlcFastAlphaBlending == 32) // 32 / 2 * 16 = 0 overflow
                    {
                        g_ucDlcFastLoop = 255;
                    }
                }
            }
            else if ((g_DlcParameters.ucDlcAvgDeltaStill <= ((WORD)g_DlcParameters.ucDlcAvgDelta * 10)) && (g_DlcParameters.ucDlcAvgDeltaStill != 0) && ((g_wTmpAvgN_x10 - g_wTmpAvgN_1_x10) <= g_DlcParameters.ucDlcAvgDeltaStill)) // if (Yavg is less than g_DlcParameters.ucDlcAvgDeltaStill) & (Fast change is finish) does NOT do curve change
            {
                g_ucDlcFlickAlpha = 0; // (0/32) = (1/DLC_DEFLICK_BLEND_FACTOR)  // it mean the curve will be still
            }
            else // if ((g_ucTmpAvgN - g_ucTmpAvgN_1) < g_DlcParameters.ucDlcAvgDelta) // if (Yavg is less than g_DlcParameters.ucDlcAvgDelta) & (Fast change is finish) may do curve change slowly
            {
                if (g_DlcParameters.ucDlcFlickAlphaStart == 0) // chagne source and it's stable
                {
                    g_ucDlcFlickAlpha = 1; // (1/32) = (1/DLC_DEFLICK_BLEND_FACTOR)  // it mean the curve change slowly
                }
                else
                {
                    g_ucDlcFlickAlpha = g_DlcParameters.ucDlcFastAlphaBlending; // (32/32) = (32/DLC_DEFLICK_BLEND_FACTOR)  // it mean the curve change quickly
                }
            }
        }
    }
}

//=========================================================
//
//=========================================================
void msDlcHandler(BOOL bWindow)
{
    BYTE ucTmp,Tmp,sepPoint =0,sepPoint0 =0,sepPoint1 =0,sepPoint2 =0,ucYAvgNormalize=0,belnging_factor;
    WORD ucYAvg=0;
    WORD uwHistogramMax, uwHistogramMax2nd,uwHistogramSlopRatioL, uwHistogramSlopRatioH; // 20110406 Ranma add
    DWORD uwHistogramAreaSum[32];//,uwPre_Histogram_ShiftR[32],uwPre_Histogram_ShiftL[32]; // 20110406 Ranma add
    WORD uwHistogramForBleSum=0;
    WORD uwHistogram_BLE_AreaSum[32];
    WORD BLE_StartPoint=0,BLE_EndPoint=0,BLE_StartPoint_Correction=0,BLE_EndPoint_Correction=0;
    WORD BLE_Slop_400,BLE_Slop_480,BLE_Slop_500,BLE_Slop_600,BLE_Slop_800,Dark_BLE_Slop_Min;
    static WORD Pre_BLE_Slop=0x4000;
    static WORD Pre_g_ucDlcFlickAlpha = 0,Pre_TotalYSum = 0,Pre_BLE_StartPoint_Correction=0xFFF;
    static BYTE ucPre_YAvg =0x00;
    static BOOL bPreSubWindowEnable=FALSE;
    static BYTE ucSubWindowChange=0;
    static WORD uwSubChangeDelayCount=0;
    BOOL bSubWindowEnable = FALSE;
    WORD uwBLE_Blend=0,uwBLE_StartPoint_Blend=0,CurveDiff=0,CurveDiff_Coring=0,CurveDiff_base=0,YAvg_base=0;
    BYTE g_ucDlcFlickAlpha_Max=0,g_ucDlcFlickAlpha_Min=0,YAvg_TH_H,YAvg_TH_L,g_ucDlcFlickAlpha_temp,ucDlcFlickAlpha_Diff=0;
    DWORD dwSumBelowAvg=0, dwSumAboveAvg=0,dwHistogramDlcSlop[17],BLE_Slop=0x400,BLE_Slop_tmp=0x400;
    WORD  Delta_YAvg=0,coeff0,coeff1,coeff2;
    DWORD His_ratio_BelowAvg,His_ratio_AboveAvg;
    DWORD g_uwCurveHistogram0[16],g_uwCurveHistogram1[16],g_uwCurveHistogram2[16];
    DWORD BLE_sum=0;
    WORD Pre_YAvg_base,Diff_L,Diff_M,Diff_H,Pre_CurveDiff_base;
    DWORD HDRAdjustMode=0;  //YT ForHDR

    U8 MaxHisBinValue;
    U16 MidSourceValue;
    U16 MidSourceOffset;
    U16 MidSourceRatio;
    U16 MidTargetValue;
    U16 MidTargetOffset;
    U16 MidTargetRatio;

    msDlc_FunctionEnter();

    msDlcLumiDiffCtrl();
#if 0 //For customer require. Run DLC algorithm when static field .
    if((g_wLumiAverageTemp-Pre_TotalYSum)==0)
    {
        return;
    }
    else
#endif
    {
        ucYAvg = msGetAverageLuminous();

        for (ucTmp=0; ucTmp<32; ucTmp++)
        {
            if(ucTmp==0)
            {
                uwHistogramAreaSum[ucTmp]=g_wLumaHistogram32H[ucTmp];
            }
            else
            {
                uwHistogramAreaSum[ucTmp] = uwHistogramAreaSum[ucTmp-1] + g_wLumaHistogram32H[ucTmp];
            }
        }

        //Mantis issue of 0232938:Hsense A3 DLC uwHistogramAreaSum[31]=0 will code dump, the divisor can't to be equal to zero .
        if(uwHistogramAreaSum[31]==0)
        {
            DLC_DEBUG(printk("\n Error in [Kernel DLC][ %s  , %d ]  uwHistogramAreaSum[31]=0 !!!\n", __FUNCTION__,__LINE__));
            return;
        }


        // ~~~~~~~~~~~~~~~~~~~~~~~~Pure Image Detect  ~~~~~~~~~~~~~~~~~~~~~~~~
        if( ((g_ucHistogramMax-g_ucHistogramMin) <= 10) || (g_ucHistogramMin >= g_ucHistogramMax) )
        {
            g_wPIAlpha = 256;
        }
        else if ((g_ucHistogramMax-g_ucHistogramMin) >= 26)
        {
            g_wPIAlpha = 0;
        }
        else
        {
            g_wPIAlpha = ((26-(g_ucHistogramMax-g_ucHistogramMin))*256+8)/16;
        }

        if(g_wPIAlpha <= 0)
        {
            g_wPIAlpha = 0;
        }
        else if (g_wPIAlpha >= 256)
        {
            g_wPIAlpha = 256;
        }
        else
        {
            g_wPIAlpha = g_wPIAlpha;
        }
        // ~~~~~~~~~~~~~~~~~~~~~~~~Pure Image Detect End   ~~~~~~~~~~~~~~~~~~~~~~~~

        if(ucYAvg >= g_DlcParameters.ucDlcYAvgThresholdH)
        {
            ucYAvgNormalize=g_DlcParameters.ucDlcSepPointL;
        }
        else if(ucYAvg <= g_DlcParameters.ucDlcYAvgThresholdL)
        {
            ucYAvgNormalize=g_DlcParameters.ucDlcSepPointH;
        }
        else
        {
            ucYAvgNormalize=((ucYAvg-g_DlcParameters.ucDlcYAvgThresholdL)*g_DlcParameters.ucDlcSepPointL
                            +(g_DlcParameters.ucDlcYAvgThresholdH-ucYAvg)*g_DlcParameters.ucDlcSepPointH)
                            /(g_DlcParameters.ucDlcYAvgThresholdH-g_DlcParameters.ucDlcYAvgThresholdL);
        }

            for (ucTmp=0; ucTmp<31; ucTmp++)
        {
            if(((ucTmp+1)*8)<ucYAvgNormalize)
            {
                dwSumBelowAvg = uwHistogramAreaSum[ucTmp]
                               +((ucYAvgNormalize-(ucTmp+1)*8)*g_wLumaHistogram32H[(ucTmp+1)]+4)/8;
            }
        }

        dwSumAboveAvg = uwHistogramAreaSum[31] - dwSumBelowAvg ;

        His_ratio_BelowAvg = (4096*dwSumBelowAvg + (uwHistogramAreaSum[31]/2))/uwHistogramAreaSum[31];
        His_ratio_AboveAvg = (4096*dwSumAboveAvg + (uwHistogramAreaSum[31]/2))/uwHistogramAreaSum[31];

        if(His_ratio_BelowAvg > 128)
        {
            His_ratio_BelowAvg = 128;
        }
        if(His_ratio_AboveAvg > 128)
        {
            His_ratio_AboveAvg = 128;
        }

        for (ucTmp=0; ucTmp<32; ucTmp++)
        {
            uwHistogram_BLE_AreaSum[ucTmp]=((1024*uwHistogramAreaSum[ucTmp])+uwHistogramAreaSum[31]/2)/uwHistogramAreaSum[31];

            if(uwHistogram_BLE_AreaSum[ucTmp]>1024)
            {
                uwHistogram_BLE_AreaSum[ucTmp]=1024;
            }
        }
            //uwHistogram_BLE_AreaSum[ucTmp] = histogram Add to normalize 1024

        if ((dwSumBelowAvg == 0)&&(dwSumAboveAvg != 0))
        {
            for (ucTmp = 0; ucTmp < 32; ucTmp++)
            {
                if (ucYAvgNormalize >= (((WORD)ucTmp+1)*8))
                {
                    uwHistogramAreaSum[ucTmp] = (ucTmp+1)*8*4;
                }
                else
                {
                    uwHistogramAreaSum[ucTmp] = ucYAvgNormalize*4 + ((DWORD)uwHistogramAreaSum[ucTmp]*(1024-ucYAvgNormalize*4)+(dwSumAboveAvg/2))/dwSumAboveAvg;
                    if(uwHistogramAreaSum[ucTmp] >= ((ucTmp+1)*32))
                    {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) + (((uwHistogramAreaSum[ucTmp] - ((ucTmp+1)*32))*His_ratio_AboveAvg)/128);
                    }
                    else
                    {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) - (((((ucTmp+1)*32) - uwHistogramAreaSum[ucTmp])*His_ratio_AboveAvg)/128);
                    }
                }
            }
        }
        else if ((dwSumBelowAvg != 0)&&(dwSumAboveAvg == 0))
        {
            for (ucTmp = 0; ucTmp < 32; ucTmp++)
            {
                if (ucYAvgNormalize >= (((WORD)ucTmp+1)*8))
                {
                    uwHistogramAreaSum[ucTmp] = ((DWORD)uwHistogramAreaSum[ucTmp]*ucYAvgNormalize*4+(dwSumBelowAvg/2))/dwSumBelowAvg;
                    if(uwHistogramAreaSum[ucTmp] >= ((ucTmp+1)*32))
                    {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) + (((uwHistogramAreaSum[ucTmp] - ((ucTmp+1)*32))*His_ratio_BelowAvg)/128);
                    }
                    else
                    {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) - (((((ucTmp+1)*32) - uwHistogramAreaSum[ucTmp])*His_ratio_BelowAvg)/128);
                    }
                }
                else
                {
                    uwHistogramAreaSum[ucTmp] = (ucTmp+1)*8*4;
                }
            }
        }
        else if((dwSumBelowAvg == 0)&&(dwSumAboveAvg == 0))
        {
            for (ucTmp = 0; ucTmp < 32; ucTmp++)
            {
                uwHistogramAreaSum[ucTmp] = (ucTmp+1)*8*4;
            }
        }
        else
        {
            for (ucTmp = 0; ucTmp < 32; ucTmp++)
            {
                if (ucYAvgNormalize >= (((WORD)ucTmp+1)*8))
                {
                    uwHistogramAreaSum[ucTmp] = ((DWORD)uwHistogramAreaSum[ucTmp]*ucYAvgNormalize*4+(dwSumBelowAvg/2))/dwSumBelowAvg;
                    if(uwHistogramAreaSum[ucTmp] >= ((ucTmp+1)*32))
                    {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) + (((uwHistogramAreaSum[ucTmp] - ((ucTmp+1)*32))*His_ratio_BelowAvg)/128);
                    }
                    else
                    {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) - (((((ucTmp+1)*32) - uwHistogramAreaSum[ucTmp])*His_ratio_BelowAvg)/128);
                    }
                }
                else
                {
                     uwHistogramAreaSum[ucTmp] = ucYAvgNormalize*4 + ((uwHistogramAreaSum[ucTmp]-dwSumBelowAvg)*(1024-ucYAvgNormalize*4)+(dwSumAboveAvg/2))/dwSumAboveAvg;

                     if(uwHistogramAreaSum[ucTmp] >= ((ucTmp+1)*32))
                     {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) + (((uwHistogramAreaSum[ucTmp] - ((ucTmp+1)*32))*His_ratio_AboveAvg)/128);
                     }
                     else
                     {
                        uwHistogramAreaSum[ucTmp] = ((ucTmp+1)*32) - (((((ucTmp+1)*32) - uwHistogramAreaSum[ucTmp])*His_ratio_AboveAvg)/128);
                     }
                }
            }
        }

            //down sampling to 16 numbers (store to uwHistogramAreaSum[0~15])

        for (ucTmp=0; ucTmp<16; ucTmp++)
        {
            uwHistogramAreaSum[ucTmp] = uwHistogramAreaSum[ucTmp*2]; //
        }

            //histogram_DLC limit by slope
        uwHistogramMax = 0;

        for (ucTmp=0; ucTmp<=16; ucTmp++)
        {
            if (ucTmp == 0)
            {
                dwHistogramDlcSlop[ucTmp] = ((uwHistogramAreaSum[ucTmp] - 0) * 256 + 16) / 64;

                if (dwHistogramDlcSlop[ucTmp] < 257)
                {
                    dwHistogramDlcSlop[ucTmp] = 257;
                }
            }
            else if (ucTmp == 16)
            {
                dwHistogramDlcSlop[ucTmp] = ((1024 - uwHistogramAreaSum[ucTmp-1]) * 256 + 16) / 64;

                if (dwHistogramDlcSlop[ucTmp] < 257)
                {
                    dwHistogramDlcSlop[ucTmp] = 257;
                }
            }
            else
            {
                dwHistogramDlcSlop[ucTmp] = ((uwHistogramAreaSum[ucTmp] - uwHistogramAreaSum[ucTmp-1]) * 256 + 32) / 64; //find slop

                if (dwHistogramDlcSlop[ucTmp] < 257)
                {
                    dwHistogramDlcSlop[ucTmp] = 257; //(only take the slop > 1)
                }
            }

            dwHistogramDlcSlop[ucTmp] = ((g_DlcParameters.ucDlcHistogramLimitCurve[ucTmp]*512)+((dwHistogramDlcSlop[ucTmp]-256)/2)) / (dwHistogramDlcSlop[ucTmp]-256) ; //SlopRatio 8bit 256=1x

            if(dwHistogramDlcSlop[ucTmp] > 0xFFFF)
            {
                dwHistogramDlcSlop[ucTmp] = 0xFFFF;
            }
        }

        sepPoint = (ucYAvgNormalize-8)/16;
        sepPoint0 = sepPoint-1;
        sepPoint1 = sepPoint;
        sepPoint2 = sepPoint+1;

    	if(ucYAvgNormalize < (sepPoint2*16))
    	{
            coeff0 = (((3*((sepPoint2*16)-ucYAvgNormalize)*((sepPoint2*16)-ucYAvgNormalize))+64*((sepPoint2*16)-ucYAvgNormalize)+320));
            coeff1 = ((1408-6*((sepPoint2*16)-ucYAvgNormalize)*((sepPoint2*16)-ucYAvgNormalize)));
            coeff2 = 2048-coeff0-coeff1;
        }
        else
        {
            coeff0 = (((3*(ucYAvgNormalize-(sepPoint2*16))*(ucYAvgNormalize-(sepPoint2*16)))-64*(ucYAvgNormalize-(sepPoint2*16))+320));
            coeff1 = ((1408-6*(ucYAvgNormalize-(sepPoint2*16))*(ucYAvgNormalize-(sepPoint2*16))));
            coeff2 = 2048-coeff0-coeff1;
    	}

        //Calculate for sepPoint0~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    	uwHistogramSlopRatioL = 256;
        uwHistogramSlopRatioH = 256;

        for (ucTmp = 0; ucTmp<= (sepPoint0+1); ucTmp++)
        {
            uwHistogramMax2nd = dwHistogramDlcSlop[ucTmp];

            if (uwHistogramMax2nd < uwHistogramSlopRatioL)
            {
                uwHistogramSlopRatioL = uwHistogramMax2nd;
            }
        }

        for (ucTmp = sepPoint0; ucTmp<=16; ucTmp++)
        {
            uwHistogramMax2nd = dwHistogramDlcSlop[ucTmp];

            if (uwHistogramMax2nd < uwHistogramSlopRatioH)
            {
                uwHistogramSlopRatioH = uwHistogramMax2nd;
            }
        }

        // calcute final target curve for sepPoint0
        for (ucTmp = 0; ucTmp < 16; ucTmp++)
        {
            if (sepPoint0 >= (WORD)ucTmp)
            {
                if(uwHistogramAreaSum[ucTmp]>(ucTmp * 64 + 32))
                {
                    g_uwCurveHistogram0[ucTmp] = (ucTmp * 64 + 32) + (((uwHistogramAreaSum[ucTmp] - (ucTmp * 64 + 32)) * uwHistogramSlopRatioL + 128) / 256 ) ;
                }
                else
                {
                    g_uwCurveHistogram0[ucTmp] = (ucTmp * 64 + 32) - ((((ucTmp * 64 + 32)- uwHistogramAreaSum[ucTmp]) * uwHistogramSlopRatioL + 128) / 256);
                }
            }
            else
            {
                if(uwHistogramAreaSum[ucTmp]>(ucTmp * 64 + 32))
                {
                    g_uwCurveHistogram0[ucTmp] = (ucTmp * 64 + 32) + (((uwHistogramAreaSum[ucTmp] - (ucTmp * 64 + 32)) * uwHistogramSlopRatioH + 128) / 256 ) ;
                }
                else
                {
                    g_uwCurveHistogram0[ucTmp] = (ucTmp * 64 + 32) - ((((ucTmp * 64 + 32)- uwHistogramAreaSum[ucTmp]) * uwHistogramSlopRatioH + 128) / 256);
                }
    	    }
        }

        //Calculate for sepPoint1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    	uwHistogramSlopRatioL = 256;
    	uwHistogramSlopRatioH = 256;

        for (ucTmp = 0; ucTmp<= (sepPoint1+1); ucTmp++)
        {
            uwHistogramMax2nd = dwHistogramDlcSlop[ucTmp];

            if (uwHistogramMax2nd < uwHistogramSlopRatioL)
            {
                uwHistogramSlopRatioL = uwHistogramMax2nd;
            }
        }

        for (ucTmp = sepPoint1; ucTmp<=16; ucTmp++)
        {
            uwHistogramMax2nd = dwHistogramDlcSlop[ucTmp];

            if (uwHistogramMax2nd < uwHistogramSlopRatioH)
            {
                uwHistogramSlopRatioH = uwHistogramMax2nd;
            }
        }

        // calcute final target curve for sepPoint1
        for (ucTmp = 0; ucTmp < 16; ucTmp++)
        {
            if (sepPoint1 >= (WORD)ucTmp)
            {
                if(uwHistogramAreaSum[ucTmp]>(ucTmp * 64 + 32))
                {
                    g_uwCurveHistogram1[ucTmp] = (ucTmp * 64 + 32) + (((uwHistogramAreaSum[ucTmp] - (ucTmp * 64 + 32)) * uwHistogramSlopRatioL + 128) / 256 );
                }
                else
                {
                    g_uwCurveHistogram1[ucTmp] = (ucTmp * 64 + 32) - ((((ucTmp * 64 + 32)- uwHistogramAreaSum[ucTmp]) * uwHistogramSlopRatioL + 128) / 256 );
                }
            }
            else
            {
                if(uwHistogramAreaSum[ucTmp]>(ucTmp * 64 + 32))
                {
                    g_uwCurveHistogram1[ucTmp] = (ucTmp * 64 + 32) + (((uwHistogramAreaSum[ucTmp] - (ucTmp * 64 + 32)) * uwHistogramSlopRatioH + 128) / 256 );
                }
                else
                {
                    g_uwCurveHistogram1[ucTmp] = (ucTmp * 64 + 32) - ((((ucTmp * 64 + 32)- uwHistogramAreaSum[ucTmp]) * uwHistogramSlopRatioH + 128) / 256 );
                }
            }
        }

        //Calculate for sepPoint2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    	uwHistogramSlopRatioL = 256;
    	uwHistogramSlopRatioH = 256;

        for (ucTmp = 0; ucTmp<= (sepPoint2+1); ucTmp++)
        {
            uwHistogramMax2nd = dwHistogramDlcSlop[ucTmp];

            if (uwHistogramMax2nd < uwHistogramSlopRatioL)
            {
                uwHistogramSlopRatioL = uwHistogramMax2nd;
            }
        }

        for (ucTmp = sepPoint2; ucTmp<=16; ucTmp++)
        {
            uwHistogramMax2nd = dwHistogramDlcSlop[ucTmp];

            if (uwHistogramMax2nd < uwHistogramSlopRatioH)
            {
                uwHistogramSlopRatioH = uwHistogramMax2nd;
            }
        }

        // calcute final target curve for sepPoint2
        for (ucTmp = 0; ucTmp < 16; ucTmp++)
        {
            if (sepPoint2 >= (WORD)ucTmp)
            {
                if(uwHistogramAreaSum[ucTmp]>(ucTmp * 64 + 32))
                {
                    g_uwCurveHistogram2[ucTmp] = (ucTmp * 64 + 32) + (((uwHistogramAreaSum[ucTmp] - (ucTmp * 64 + 32)) * uwHistogramSlopRatioL + 128) / 256 );
                }
                else
                {
                    g_uwCurveHistogram2[ucTmp] = (ucTmp * 64 + 32) - ((((ucTmp * 64 + 32)- uwHistogramAreaSum[ucTmp]) * uwHistogramSlopRatioL + 128) / 256 );
                }
            }
            else
            {
                if(uwHistogramAreaSum[ucTmp]>(ucTmp * 64 + 32))
                {
                    g_uwCurveHistogram2[ucTmp] = (ucTmp * 64 + 32) + (((uwHistogramAreaSum[ucTmp] - (ucTmp * 64 + 32)) * uwHistogramSlopRatioH + 128) / 256 );
                }
                else
                {
                    g_uwCurveHistogram2[ucTmp] = (ucTmp * 64 + 32) - ((((ucTmp * 64 + 32)- uwHistogramAreaSum[ucTmp]) * uwHistogramSlopRatioH + 128) / 256);
                }
            }
        }

        for (ucTmp = 0; ucTmp < 16; ucTmp++)
        {
            g_uwCurveHistogram[ucTmp] = (coeff0*g_uwCurveHistogram0[ucTmp]+coeff1*g_uwCurveHistogram1[ucTmp]+coeff2*g_uwCurveHistogram2[ucTmp]+1024)/2048;
        }
        ///////////// Histogram_DLC add Low_Pass_filter /////////////

        for (ucTmp = 0; ucTmp < 16; ucTmp++)
        {
            if (ucTmp == 0)
            {
                g_uwCurveHistogram[ucTmp] = (5*g_uwCurveHistogram[ucTmp] + g_uwCurveHistogram[ucTmp+1]+4)/8;
            }
            else if (ucTmp == 15)
            {
                g_uwCurveHistogram[ucTmp] = (g_uwCurveHistogram[ucTmp-1] + 5*g_uwCurveHistogram[ucTmp] + 2048 + 4)/8;
            }
            else
            {
                g_uwCurveHistogram[ucTmp] = (g_uwCurveHistogram[ucTmp-1] + 6*g_uwCurveHistogram[ucTmp] + g_uwCurveHistogram[ucTmp+1]+4)/8;
            }
        }

    	// Calculate curve by Yavg

        if (ucYAvg <= g_DlcParameters.ucDlcYAvgThresholdL)
        {
            for (ucTmp=0; ucTmp<16; ucTmp++)
            {
                g_ucCurveYAvg[ucTmp] = g_DlcParameters.ucLumaCurve2_a[ucTmp];
            }
        }
        else if (ucYAvg <= g_DlcParameters.ucDlcYAvgThresholdM)
        {
            for (ucTmp=0; ucTmp<16; ucTmp++)
            {
                g_ucCurveYAvg[ucTmp] = (((WORD)g_DlcParameters.ucLumaCurve[ucTmp] * (ucYAvg - g_DlcParameters.ucDlcYAvgThresholdL)) / (g_DlcParameters.ucDlcYAvgThresholdM - g_DlcParameters.ucDlcYAvgThresholdL)) +
                                       (g_DlcParameters.ucLumaCurve2_a[ucTmp] - (((WORD)g_DlcParameters.ucLumaCurve2_a[ucTmp] * (ucYAvg - g_DlcParameters.ucDlcYAvgThresholdL)) / ( g_DlcParameters.ucDlcYAvgThresholdM - g_DlcParameters.ucDlcYAvgThresholdL)));
            }
        }
        else if (ucYAvg <= g_DlcParameters.ucDlcYAvgThresholdH)
        {
            for (ucTmp=0; ucTmp<16; ucTmp++)
            {
                g_ucCurveYAvg[ucTmp] = ((WORD)g_DlcParameters.ucLumaCurve2_b[ucTmp] * (ucYAvg - g_DlcParameters.ucDlcYAvgThresholdM) / (g_DlcParameters.ucDlcYAvgThresholdH - g_DlcParameters.ucDlcYAvgThresholdM)) +
                                       (g_DlcParameters.ucLumaCurve[ucTmp] - ((WORD)g_DlcParameters.ucLumaCurve[ucTmp] * (ucYAvg - g_DlcParameters.ucDlcYAvgThresholdM) / (g_DlcParameters.ucDlcYAvgThresholdH - g_DlcParameters.ucDlcYAvgThresholdM)));
            }
        }
        else // if (ucYAvg > g_DlcParameters.ucDlcYAvgThresholdH)
        {
            for (ucTmp=0; ucTmp<16; ucTmp++)
            {
                g_ucCurveYAvg[ucTmp] = g_DlcParameters.ucLumaCurve2_b[ucTmp];
            }
        }

      	belnging_factor = g_DlcParameters.ucDlcCurveModeMixAlpha;

    	// Decide Target Curve

        Diff_L =g_DlcParameters.ucDlcDiffBase_L;
        Diff_M =g_DlcParameters.ucDlcDiffBase_M;
        Diff_H =g_DlcParameters.ucDlcDiffBase_H;

        if (ucPre_YAvg*4 <= g_DlcParameters.ucDlcYAvgThresholdL*4)
        {
            Pre_YAvg_base = Diff_L*4;
        }
        else if (ucPre_YAvg*4 <= g_DlcParameters.ucDlcYAvgThresholdM*4)
        {
            if(Diff_M >= Diff_L)
            {
                Pre_YAvg_base = Diff_L*4 + (((Diff_M-Diff_L)*(ucPre_YAvg*4-g_DlcParameters.ucDlcYAvgThresholdL*4))/(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
            }
            else
            {
                Pre_YAvg_base = Diff_L*4 - (((Diff_L-Diff_M)*(ucPre_YAvg*4-g_DlcParameters.ucDlcYAvgThresholdL*4))/(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
            }
        }
        else if (ucPre_YAvg*4 <= g_DlcParameters.ucDlcYAvgThresholdH*4)
        {
            if(Diff_H >= Diff_M)
            {
                Pre_YAvg_base = Diff_M*4 + (((Diff_H- Diff_M)*(ucPre_YAvg*4- g_DlcParameters.ucDlcYAvgThresholdM*4))/(g_DlcParameters.ucDlcYAvgThresholdH- g_DlcParameters.ucDlcYAvgThresholdM));
            }
            else
            {
                Pre_YAvg_base = Diff_M*4 - (((Diff_M-Diff_H)*(ucPre_YAvg*4-g_DlcParameters.ucDlcYAvgThresholdM*4))/(g_DlcParameters.ucDlcYAvgThresholdH - g_DlcParameters.ucDlcYAvgThresholdM));
            }
        }
        else
        {
            Pre_YAvg_base = Diff_H*4;
        }

        if (ucYAvg*4 <= g_DlcParameters.ucDlcYAvgThresholdL*4)
        {
            YAvg_base = Diff_L*4;
        }
        else if (ucYAvg*4 <= g_DlcParameters.ucDlcYAvgThresholdM*4)
        {
            if(Diff_M >= Diff_L)
            {
                YAvg_base = Diff_L*4 + (((Diff_M-Diff_L)*( ucYAvg*4-g_DlcParameters.ucDlcYAvgThresholdL*4))/(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
            }
            else
            {
                YAvg_base = Diff_L*4 - (((Diff_L-Diff_M)*( ucYAvg*4-g_DlcParameters.ucDlcYAvgThresholdL*4))/(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
            }
        }
        else if (ucYAvg*4 <= g_DlcParameters.ucDlcYAvgThresholdH*4)
        {
            if(Diff_H >= Diff_M)
            {
                YAvg_base = Diff_M*4 + (((Diff_H- Diff_M)*( ucYAvg*4- g_DlcParameters.ucDlcYAvgThresholdM*4))/(g_DlcParameters.ucDlcYAvgThresholdH- g_DlcParameters.ucDlcYAvgThresholdM));
            }
            else
            {
                YAvg_base = Diff_M*4 - (((Diff_M-Diff_H)*(ucYAvg*4-g_DlcParameters.ucDlcYAvgThresholdM*4))/(g_DlcParameters.ucDlcYAvgThresholdH -g_DlcParameters.ucDlcYAvgThresholdM));
            }
        }
        else
        {
            YAvg_base = Diff_H*4;
        }

        YAvg_base = (Pre_YAvg_base + YAvg_base +1)/2;

        //Mantis issue of 0232938:Hsense A3 DLC the divisor can't to be equal to zero .
        if(YAvg_base==0)
        {
            DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  YAvg_base=0 !!!\n", __FUNCTION__,__LINE__));
            return;
        }

    	if (g_DlcParameters.ucDlcCurveMode == 0 ) //DLC_CURVE_MODE_BY_YAVG)
        {
            CurveDiff = g_DlcParameters.ucDlcCurveDiff_H_TH;
            for (ucTmp=0; ucTmp<16; ucTmp++)
            {
                g_uwTable[ucTmp] = (g_ucCurveYAvg[ucTmp]<<2);
            }
        }
    	else if (g_DlcParameters.ucDlcCurveMode == 1 ) //DLC_CURVE_MODE_BY_HISTOGRAM)
        {
            CurveDiff = g_DlcParameters.ucDlcCurveDiff_H_TH;
            for (ucTmp=0; ucTmp<16; ucTmp++)
            {
                g_uwTable[ucTmp] = g_uwCurveHistogram[ucTmp];
            }
        }
    	else
        {
            CurveDiff = 0;
            CurveDiff_Coring = 0;
            for (ucTmp=0; ucTmp<16; ucTmp++)
            {
                g_uwTable[ucTmp] = (((WORD)(g_ucCurveYAvg[ucTmp]<<2) * belnging_factor) + ((g_uwCurveHistogram[ucTmp]) * (128 - belnging_factor))) / 128;

                if (g_uwPre_CurveHistogram[ucTmp] <= g_DlcParameters.ucDlcYAvgThresholdL*4)
                {
                    Pre_CurveDiff_base = Diff_L*4;
                }
                else if (g_uwPre_CurveHistogram[ucTmp] <= g_DlcParameters.ucDlcYAvgThresholdM*4)
                {
                    if(Diff_M >= Diff_L)
                    {
                        Pre_CurveDiff_base = Diff_L*4 + (((Diff_M-Diff_L)*(g_uwPre_CurveHistogram[ucTmp]-g_DlcParameters.ucDlcYAvgThresholdL*4))/(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
                    }
                    else
                    {
                        Pre_CurveDiff_base = Diff_L*4 - (((Diff_L-Diff_M)*(g_uwPre_CurveHistogram[ucTmp]-g_DlcParameters.ucDlcYAvgThresholdL*4))/(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
                    }
                }
                else if (g_uwPre_CurveHistogram[ucTmp] <= g_DlcParameters.ucDlcYAvgThresholdH*4)
                {
                    if(Diff_H >= Diff_M)
                    {
                        Pre_CurveDiff_base = Diff_M*4 + (((Diff_H- Diff_M)*(g_uwPre_CurveHistogram[ucTmp]- g_DlcParameters.ucDlcYAvgThresholdM*4))/(g_DlcParameters.ucDlcYAvgThresholdH- g_DlcParameters.ucDlcYAvgThresholdM));
                    }
                    else
                    {
                        Pre_CurveDiff_base = Diff_M*4 -(((Diff_M- Diff_H)*(g_uwPre_CurveHistogram[ucTmp]- g_DlcParameters.ucDlcYAvgThresholdM*4))/(g_DlcParameters.ucDlcYAvgThresholdH- g_DlcParameters.ucDlcYAvgThresholdM));
                    }
                }
                else
                {
                    Pre_CurveDiff_base = Diff_H*4;
                }

                if (g_uwCurveHistogram[ucTmp] <= g_DlcParameters.ucDlcYAvgThresholdL*4)
                {
                    CurveDiff_base = Diff_L*4;
                }
                else if (g_uwCurveHistogram[ucTmp] <= g_DlcParameters.ucDlcYAvgThresholdM*4)
                {
                    if(Diff_M >= Diff_L)
                    {
                        CurveDiff_base = Diff_L*4 + (((Diff_M-Diff_L)*(g_uwCurveHistogram[ucTmp]-g_DlcParameters.ucDlcYAvgThresholdL*4))
                                        /(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
                    }
                    else
                    {
                        CurveDiff_base = Diff_L*4 - (((Diff_L-Diff_M)*(g_uwCurveHistogram[ucTmp]-g_DlcParameters.ucDlcYAvgThresholdL*4))
                                        /(g_DlcParameters.ucDlcYAvgThresholdM- g_DlcParameters.ucDlcYAvgThresholdL));
                    }
                }
                else if (g_uwCurveHistogram[ucTmp] <= g_DlcParameters.ucDlcYAvgThresholdH*4)
                {
                    if(Diff_H >= Diff_M)
                    {
                        CurveDiff_base = Diff_M*4 + (((Diff_H- Diff_M)*(g_uwCurveHistogram[ucTmp]- g_DlcParameters.ucDlcYAvgThresholdM*4))
                                        /(g_DlcParameters.ucDlcYAvgThresholdH- g_DlcParameters.ucDlcYAvgThresholdM));
                    }
                    else
                    {
                        CurveDiff_base = Diff_M*4 -(((Diff_M- Diff_H)*(g_uwCurveHistogram[ucTmp]- g_DlcParameters.ucDlcYAvgThresholdM*4))
                                        /(g_DlcParameters.ucDlcYAvgThresholdH- g_DlcParameters.ucDlcYAvgThresholdM));
                    }
                }
                else
                {
                    CurveDiff_base = Diff_H*4;
                }

                CurveDiff_base = (Pre_CurveDiff_base + CurveDiff_base+1)/2;

                //Mantis issue of 0232938:Hsense A3 DLC the divisor can't to be equal to zero .
                if(CurveDiff_base==0)
                {
                    DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  CurveDiff_base=0 !!!\n", __FUNCTION__,__LINE__));
                    return;
                }

                if(ucTmp*16+8 <= ucYAvgNormalize)
                {
                    if (ucTmp == 0)
                    {
                        CurveDiff_Coring = (His_ratio_BelowAvg*(1024-uwHistogram_BLE_AreaSum[1])*abs(g_uwCurveHistogram[ucTmp]-g_uwPre_CurveHistogram[ucTmp]))/(15*CurveDiff_base*128);
                    }
                    else
                    {
                        CurveDiff_Coring = (His_ratio_BelowAvg*(1024-(uwHistogram_BLE_AreaSum[ucTmp*2+1]- uwHistogram_BLE_AreaSum[ucTmp*2-1]))*abs(g_uwCurveHistogram[ucTmp]-g_uwPre_CurveHistogram[ucTmp]))/(15*CurveDiff_base*128);
                    }
                }
                else
                {
                    CurveDiff_Coring = (His_ratio_AboveAvg*(1024-(uwHistogram_BLE_AreaSum[ucTmp*2+1]- uwHistogram_BLE_AreaSum[ucTmp*2-1]))*abs(g_uwCurveHistogram[ucTmp]-g_uwPre_CurveHistogram[ucTmp]))/(15*CurveDiff_base*128);
                }

                CurveDiff_Coring = CurveDiff_Coring/6;

                CurveDiff = CurveDiff + CurveDiff_Coring ;
                g_uwPre_CurveHistogram[ucTmp] = g_uwCurveHistogram[ucTmp];
            }
        }

        if(CurveDiff <= g_DlcParameters.ucDlcCurveDiff_L_TH)
    	{
            g_ucDlcFlickAlpha_Max = g_DlcParameters.ucDlcAlphaBlendingMin;
        }
        else if(CurveDiff >= g_DlcParameters.ucDlcCurveDiff_H_TH)
    	{
            g_ucDlcFlickAlpha_Max = g_DlcParameters.ucDlcAlphaBlendingMax; ;
    	}
        else
        {
            g_ucDlcFlickAlpha_Max = (((g_DlcParameters.ucDlcAlphaBlendingMax-g_DlcParameters.ucDlcAlphaBlendingMin)*(CurveDiff - g_DlcParameters.ucDlcCurveDiff_L_TH))/
                                    (g_DlcParameters.ucDlcCurveDiff_H_TH - g_DlcParameters.ucDlcCurveDiff_L_TH))+g_DlcParameters.ucDlcAlphaBlendingMin;
        }

        Delta_YAvg = (96*4*abs(ucYAvg - ucPre_YAvg)+ YAvg_base/2)/YAvg_base;

        if(Delta_YAvg > 256)
        {
            Delta_YAvg = 256;
        }
        else
        {
            Delta_YAvg = Delta_YAvg;
        }

        YAvg_TH_H = g_DlcParameters.ucDlcYAVG_H_TH;
        YAvg_TH_L = g_DlcParameters.ucDlcYAVG_L_TH;

        if(Delta_YAvg < YAvg_TH_L)
        {
            g_ucDlcFlickAlpha_Min = g_DlcParameters.ucDlcAlphaBlendingMin;
        }
        else if(Delta_YAvg >= YAvg_TH_H)
        {
            g_ucDlcFlickAlpha_Min = g_DlcParameters.ucDlcAlphaBlendingMax;
        }
        else
        {
            g_ucDlcFlickAlpha_Min = ((Delta_YAvg- YAvg_TH_L)*( g_DlcParameters.ucDlcAlphaBlendingMax- g_DlcParameters.ucDlcAlphaBlendingMin)
                                    + ( YAvg_TH_H-YAvg_TH_L)/2)/( YAvg_TH_H-YAvg_TH_L)+g_DlcParameters.ucDlcAlphaBlendingMin;
        }

        ucPre_YAvg = ucYAvg;

        if (g_ucDlcFlickAlpha_Max > g_ucDlcFlickAlpha_Min)
        {
            g_ucDlcFlickAlpha_Max = g_ucDlcFlickAlpha_Max;
            g_ucDlcFlickAlpha_Min = g_ucDlcFlickAlpha_Min;
        }
        else
        {
            g_ucDlcFlickAlpha_temp = g_ucDlcFlickAlpha_Max;
            g_ucDlcFlickAlpha_Max = g_ucDlcFlickAlpha_Min;
            g_ucDlcFlickAlpha_Min = g_ucDlcFlickAlpha_temp;
        }

        g_ucDlcFlickAlpha  = ( g_DlcParameters.ucDlcFlicker_alpha*g_ucDlcFlickAlpha_Max + (256- g_DlcParameters.ucDlcFlicker_alpha)*g_ucDlcFlickAlpha_Min+128)/256;

    }

    if( g_DlcParameters.bEnableBLE )
    {
        BYTE uc_cut,uc_lowbound,uc_highbound,i;

        // combine BLE with software
        // BLE - get cut point
        uc_cut = g_ucHistogramMin;
        uc_highbound = msReadByte( REG_ADDR_BLE_UPPER_BOND );
        uc_lowbound = msReadByte( REG_ADDR_BLE_LOWER_BOND);

        if (uc_cut < uc_lowbound )
        {
            uc_cut = uc_lowbound;
        }
        else if ( uc_cut > uc_highbound )
        {
            uc_cut = uc_highbound;
        }
        else
        {
            uc_cut = uc_cut;
        }

        // BLE - calculation
        for(i=0; i<8; ++i)
        {
            if (g_ucTable[i] < uc_cut)
            {
                g_ucTable[i] = 0;
            }
            else if (g_ucTable[i] > (uc_cut+g_DlcParameters.ucDlcBLEPoint))
            {
                ;
            }
            else
            {
                g_ucTable[i] = ((WORD)(g_ucTable[i]-uc_cut))*(uc_cut+g_DlcParameters.ucDlcBLEPoint)/g_DlcParameters.ucDlcBLEPoint;
            }
        }
    }


    if( g_DlcParameters.bEnableWLE )
    {
        BYTE i,uc_wcut,uc_wlowbound,uc_whighbound;
        // combine BLE with software
        uc_wcut = g_ucHistogramMax;

        uc_whighbound = (msReadByte( REG_ADDR_WLE_UPPER_BOND ) & 0x3F) + 0xC0; // bit offset 0x00~0x3F -> 0xC0~0xFF
        uc_wlowbound = (msReadByte( REG_ADDR_WLE_LOWER_BOND) & 0x3F) + 0xC0; // bit offset 0x00~0x3F -> 0xC0~0xFF

        if ( uc_wcut < uc_wlowbound )
        {
            uc_wcut = uc_wlowbound;
        }
        else if ( uc_wcut > uc_whighbound )
        {
            uc_wcut = uc_whighbound;
        }
        else
        {
            uc_wcut = uc_wcut;
        }

        // WLE - calculation
        for(i=0; i<8; ++i)
        {
            if (g_ucTable[15-i] > uc_wcut)
            {
                g_ucTable[15-i] = 0xFF;
            }
            else if (g_ucTable[15-i] < (uc_wcut-g_DlcParameters.ucDlcWLEPoint))
            {
                ;
            }
            else
            {
                g_ucTable[15-i] = 0xFF - (((WORD)(uc_wcut-g_ucTable[15-i]))*(0xFF-uc_wcut+g_DlcParameters.ucDlcWLEPoint)/g_DlcParameters.ucDlcWLEPoint);
            }
        }
    }

    // Level Limit
    if (g_DlcParameters.ucDlcLevelLimit)
    {
        if ((g_DlcParameters.ucDlcLevelLimit >= 1) && (g_ucTable[15] >= 0xF7))
        {
            g_ucTable[15] = 0xF7;
            if (g_ucTable[14] > 0xF7)
            {
                g_ucTable[14] = 0xF7;
            }

            if (g_ucTable[13] > 0xF7)
            {
                g_ucTable[13] = 0xF7;
            }
        }

        if ((g_DlcParameters.ucDlcLevelLimit >= 2) && (g_ucTable[14] >= 0xE7))
        {
            g_ucTable[14] = 0xE7;
            if (g_ucTable[13] > 0xE7)
            {
                g_ucTable[13] = 0xE7;
            }

            if (g_ucTable[12] > 0xE7)
            {
                g_ucTable[12] = 0xE7;
            }
        }

        if ((g_DlcParameters.ucDlcLevelLimit >= 3) && (g_ucTable[13] >= 0xD7))
        {
            g_ucTable[13] = 0xD7;
            if (g_ucTable[12] > 0xD7)
            {
                g_ucTable[12] = 0xD7;
            }

            if (g_ucTable[11] > 0xD7)
            {
                g_ucTable[11] = 0xD7;
            }
        }

        if ((g_DlcParameters.ucDlcLevelLimit >= 4) && (g_ucTable[12] >= 0xC7))
        {
            g_ucTable[12] = 0xC7;
            if (g_ucTable[11] > 0xC7)
            {
                g_ucTable[11] = 0xC7;
            }

            if (g_ucTable[10] > 0xC7)
            {
                g_ucTable[10] = 0xC7;
            }
        }
    }
#if 0 //For customer require. Run DLC algorithm when static field .
    if((g_wLumiAverageTemp-Pre_TotalYSum)!=0)
#endif
    {

        if ((g_ucDlcFlickAlpha < (5*Pre_g_ucDlcFlickAlpha /8)) && (ucDlcFlickAlpha_Diff < (g_DlcParameters.ucDlcAlphaBlendingMax- g_DlcParameters.ucDlcAlphaBlendingMin)/2))
        {
            g_ucDlcFlickAlpha = 2*(ucDlcFlickAlpha_Diff* g_ucDlcFlickAlpha +(( g_DlcParameters.ucDlcAlphaBlendingMax
                                  - g_DlcParameters.ucDlcAlphaBlendingMin)/2- ucDlcFlickAlpha_Diff)* (5*Pre_g_ucDlcFlickAlpha /8))
                                  /( g_DlcParameters.ucDlcAlphaBlendingMax- g_DlcParameters.ucDlcAlphaBlendingMin);
        }

        Pre_g_ucDlcFlickAlpha = g_ucDlcFlickAlpha;
        ucDlcFlickAlpha_Diff = g_ucDlcFlickAlpha_Max-g_ucDlcFlickAlpha_Min;


         // Write data to luma curve ...
        if (g_HDRinitParameters.bHDREnable==ENABLE)  //YT ForHDR
        {
            MS_U16 u16YAvg;   //YT ForHDR
            u16YAvg = msGetAverageLuminous();//YT ForHDR
#if 0 // Wright Test
            if (msReadByte(REG_3A40)==0x01)
                HDRAdjustMode=1; // YT mode
            else if  (msReadByte(REG_3A40)==0x02)
                HDRAdjustMode=2; //Ian mode
            else
                HDRAdjustMode=0;// Ref Reg.
#endif
            HDRAdjustMode = 0;//MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK1A_3E_L,0xFF); //Ian mode

            HDRtmp.slope = 1582;
            HDRtmp.rolloff = 12288;

#if HDR_DEBUG_MODE
            HDRtmp.smin = MApi_GFLIP_XC_R2BYTE(REG_SC_BK1A_38_L);
            HDRtmp.smax = MApi_GFLIP_XC_R2BYTE(REG_SC_BK1A_39_L);
            HDRtmp.tmin = MApi_GFLIP_XC_R2BYTE(REG_SC_BK1A_3A_L);
            HDRtmp.tmax = MApi_GFLIP_XC_R2BYTE(REG_SC_BK1A_3B_L);
#endif

            MidSourceOffset= g_HDRinitParameters.DLC_HDRToneMappingData.u16MidSourceOffset;
            MidTargetOffset= g_HDRinitParameters.DLC_HDRToneMappingData.u16MidTargetOffset;
            MidSourceRatio= g_HDRinitParameters.DLC_HDRToneMappingData.u16MidSourceRatio;
            MidTargetRatio= g_HDRinitParameters.DLC_HDRToneMappingData.u16MidTargetRatio;
            MaxHisBinValue=MaxucTmp*8;

            if (MidSourceRatio>1)
                MidSourceValue=(100*MaxHisBinValue/MidSourceRatio)+MidSourceOffset;
            else
                MidSourceValue=100*MaxHisBinValue+MidSourceOffset;

            if (MidTargetRatio>1)
                MidTargetValue= (100*MaxHisBinValue/MidTargetRatio)+MidTargetOffset;
            else
                MidTargetValue= (100*MaxHisBinValue)+MidTargetOffset;

            if (MidSourceValue>1023)
                MidSourceValue=1023;
            if (MidTargetValue>1023)
                MidTargetValue=1023;

               /*printf("u = %d ",MaxucTmp);
                printf("m = %d ",MidSourceValue);
                 printf("i = %d ",MidTargetValue);
               printf("\r\n", 0);*/
            if  (HDRAdjustMode==1) //SamMid
            {
                HDRtmp.smed =MidSourceValue;
                HDRtmp.tmed =MidSourceValue;
            }
            else if  (HDRAdjustMode==2) //Ian
            {
                HDRtmp.smed =MidSourceValue;
                HDRtmp.tmed =MidTargetValue;
            }
            else //Reg Reg.
            {

#if HDR_DEBUG_MODE
                HDRtmp.smed =MApi_GFLIP_XC_R2BYTE(REG_SC_BK1A_3C_L);
                HDRtmp.tmed =MApi_GFLIP_XC_R2BYTE(REG_SC_BK1A_3D_L);
#else
                HDRtmp.smed = g_HDRinitParameters.DLC_HDRToneMappingData.u16Smed;
                HDRtmp.tmed = g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmed;
#endif

            }

            if((_u16Count%300)==0)
            {
                //printk("\n  Kernel HDRAdjustMode=%x ; smin=%x , smax=%x ,tmax=%x ,tmin=%x ,smed=%x ,tmed=%x    \n",HDRAdjustMode,HDRtmp.smin,HDRtmp.smax,HDRtmp.tmax,HDRtmp.tmin,HDRtmp.smed,HDRtmp.tmed);
            }

            //MDrv_HDR_ToneMappingCurveGen(HDR_Curve,&HDRtmp,bWindow);

        }
        else
        {

           // ~~~~~~~~~~~~~~~~~~~~~~~~ Sub window open PATCH Start  ~~~~~~~~~~~~~~~~~~~~~~~~
           bSubWindowEnable = ((msReadByte(REG_ADDR_OP_SW_SUB_ENABLE) & BIT(1)) ==  BIT(1));

           if(bPreSubWindowEnable != bSubWindowEnable)
           {
               ucSubWindowChange = 1;
               bPreSubWindowEnable = bSubWindowEnable;
               uwSubChangeDelayCount=0;
           }

           if(ucSubWindowChange > 0)
           {
               uwSubChangeDelayCount++;
               if(((g_ucDlcFlickAlpha > 48) &&(uwSubChangeDelayCount>=100))
                   ||((g_ucDlcFlickAlpha > 24) &&(uwSubChangeDelayCount>=200)))
               {
                   ucSubWindowChange = ucSubWindowChange-1;
               }

               if(ucSubWindowChange == 0)
               {
                   uwSubChangeDelayCount=0;
               }
               else
               {
                   g_ucDlcFlickAlpha = 0;
               }
           }
           // ~~~~~~~~~~~~~~~~~~~~~~~~ Sub window open PATCH End  ~~~~~~~~~~~~~~~~~~~~~~~~

           // ~~~~~~~~~~~~~~~~~~~~~~~~ PI_Alpha IIR Start  ~~~~~~~~~~~~~~~~~~~~~~~~
           if ( g_wPIAlpha >= g_wPrePIAlpha)
           {
               g_wPIAlpha = ((g_ucDlcFlickAlpha * g_wPIAlpha) +((128-g_ucDlcFlickAlpha)*g_wPrePIAlpha)+127)/128;
           }
           else
           {
               g_wPIAlpha = ((g_ucDlcFlickAlpha * g_wPIAlpha) +((128-g_ucDlcFlickAlpha)*g_wPrePIAlpha))/128;
           }

           g_wPrePIAlpha = g_wPIAlpha;
           // ~~~~~~~~~~~~~~~~~~~~~~~~ PI_Alpha IIR End  ~~~~~~~~~~~~~~~~~~~~~~~~

           // Write data to luma curve ...
           msDlcWriteCurve(bWindow);
        }

        if(g_bSetDLCCurveBoth)
        {
          //  msDlcWriteCurve(SUB_WINDOW);
        }

        //Dyanmic BLE Function
        //~~~~~~~~~~~~~~~~~Calculate BLE_StartPoint~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(((ucYAvg*4) + g_DlcParameters.uwDlcBleStartPointTH)>512)
        {
            BLE_StartPoint = ((ucYAvg*4) + g_DlcParameters.uwDlcBleStartPointTH - 512);
        }
        else
        {
            BLE_StartPoint = 0;
        }

        BLE_StartPoint_Correction = (BLE_StartPoint+2)/4;

        if(BLE_StartPoint_Correction >(Pre_BLE_StartPoint_Correction/16))
        {
            if(g_ucDlcFlickAlpha==g_DlcParameters.ucDlcAlphaBlendingMin)
            {
                uwBLE_StartPoint_Blend = (WORD)(((16*(DWORD)BLE_StartPoint_Correction)+(63*(DWORD)Pre_BLE_StartPoint_Correction)+63)/64);
                Pre_BLE_StartPoint_Correction = uwBLE_StartPoint_Blend;
                uwBLE_StartPoint_Blend = (uwBLE_StartPoint_Blend+8)/16;
            }
            else
            {
                uwBLE_StartPoint_Blend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)BLE_StartPoint_Correction) +
                                         ((128 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)(Pre_BLE_StartPoint_Correction/16))+127)/128);
                Pre_BLE_StartPoint_Correction = uwBLE_StartPoint_Blend*16;
            }
        }
        else
        {
            if(g_ucDlcFlickAlpha==g_DlcParameters.ucDlcAlphaBlendingMin)
            {
                uwBLE_StartPoint_Blend = (WORD)(((16*(DWORD)BLE_StartPoint_Correction)+(63*(DWORD)Pre_BLE_StartPoint_Correction))/64);
                Pre_BLE_StartPoint_Correction = uwBLE_StartPoint_Blend;
                uwBLE_StartPoint_Blend = (uwBLE_StartPoint_Blend+8)/16;
            }
            else
            {
                uwBLE_StartPoint_Blend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)BLE_StartPoint_Correction) +
                                         ((128 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)(Pre_BLE_StartPoint_Correction/16)))/128);
                Pre_BLE_StartPoint_Correction = uwBLE_StartPoint_Blend*16;
            }
        }

        //~~~~~~~~~output = uwBLE_StartPoint_Blend & Pre_BLE_StartPoint_Correction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        //~~~~~~~~~~~~~~~~~Calculate  BLE_EndPoint ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        for (ucTmp=0; ucTmp<32; ucTmp++)
        {
            if(ucTmp==0)
            {
                uwHistogramAreaSum[ucTmp]=g_wLumaHistogram32H[ucTmp];
            }
            else
            {
                uwHistogramAreaSum[ucTmp] = uwHistogramAreaSum[ucTmp-1] + g_wLumaHistogram32H[ucTmp];
            }
        }

        ucTmp=0;
        while( uwHistogramAreaSum[ucTmp] < uwHistogramAreaSum[31]/8)
        {
            ucTmp++;
        }

        if(ucTmp==0)
        {
            //Mantis issue of 0264631 :[Konka J3] . Use autotest tool control dc on/off.  DLC divisor can't to be equal to zero .
            if(g_wLumaHistogram32H[0]==0)
            {
                DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  g_wLumaHistogram32H[0]=0 !!!\n", __FUNCTION__,__LINE__));
                return;
            }

            BLE_EndPoint = ((g_uwTable[0]*uwHistogramAreaSum[31])/(16*g_wLumaHistogram32H[0]));
        }
        else
        {
            uwHistogramForBleSum = (uwHistogramAreaSum[31]/8) - uwHistogramAreaSum[ucTmp-1];

            for (Tmp=0; Tmp<30; Tmp++)
            {
                uwHistogramAreaSum[Tmp] = (g_uwTable[Tmp/2]+g_uwTable[(Tmp+1)/2]+1)/2;
            }

            for (Tmp=0; Tmp<(ucTmp-1); Tmp++)
            {
                if(Tmp==0)
                {
                    BLE_sum = g_wLumaHistogram32H[0]*(g_uwTable[0]+1/2);
                }
                else
                {
                    BLE_sum = BLE_sum + g_wLumaHistogram32H[Tmp] * ((uwHistogramAreaSum[Tmp]+uwHistogramAreaSum[Tmp-1]+1)/2);
                }
            }

            //Mantis issue of 0232938:Hsense A3 DLC the divisor can't to be equal to zero .
            if(g_wLumaHistogram32H[ucTmp]==0)
            {
                DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  g_wLumaHistogram32H[%d]=0 !!!\n", __FUNCTION__,__LINE__,ucTmp));
                return;
            }

            BLE_sum = BLE_sum + uwHistogramForBleSum *((uwHistogramAreaSum[ucTmp]+uwHistogramAreaSum[ucTmp-1]+1)/2);

            BLE_EndPoint = (BLE_sum*8+(uwHistogramAreaSum[31]/2))/uwHistogramAreaSum[31];
        }

        BLE_EndPoint = (BLE_EndPoint*g_DlcParameters.uwDlcBleEndPointTH+512)/1024;
        BLE_EndPoint_Correction = BLE_EndPoint;


        //~~~~~~~~~output = BLE_EndPoint Correction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        //~~~~~~~~~Used uwBLE_StartPoint_Blend & BLE_EndPoint_Correction Calculate BLE_Slop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        if(uwBLE_StartPoint_Blend > 0x7F)
        {
            uwBLE_StartPoint_Blend = 0x7F;
        }

        if( uwBLE_StartPoint_Blend*4 > BLE_EndPoint_Correction)
        {
            BLE_Slop = (1024*uwBLE_StartPoint_Blend*4+((uwBLE_StartPoint_Blend*4 - BLE_EndPoint_Correction)/2))
                    /(uwBLE_StartPoint_Blend*4 - BLE_EndPoint_Correction);
        }
        else
        {
            BLE_Slop = 0x7FF;
        }

        if(BLE_Slop > 0x7FF)
        {
            BLE_Slop = 0x7FF;
        }

        if(BLE_Slop < 0x400)
        {
            BLE_Slop = 0x400;
        }

        BLE_Slop_400 = g_DlcParameters.uwDlcBLESlopPoint_1;//(msReadByte(L_BK_DLC(0x38)))<<3;
        BLE_Slop_480 = g_DlcParameters.uwDlcBLESlopPoint_2;//(msReadByte(H_BK_DLC(0x38)))<<3;
        BLE_Slop_500 = g_DlcParameters.uwDlcBLESlopPoint_3;//(msReadByte(L_BK_DLC(0x39)))<<3;
        BLE_Slop_600 = g_DlcParameters.uwDlcBLESlopPoint_4;//(msReadByte(H_BK_DLC(0x39)))<<3;
        BLE_Slop_800 = g_DlcParameters.uwDlcBLESlopPoint_5;//(msReadByte(L_BK_DLC(0x3A)))<<3;
        Dark_BLE_Slop_Min = g_DlcParameters.uwDlcDark_BLE_Slop_Min;//(msReadByte(L_BK_DLC(0x3B)))<<3;

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(BLE_Slop <= 0x480)
        {
            BLE_Slop =((BLE_Slop_480-BLE_Slop_400)*(BLE_Slop-0x400)+64)/128+BLE_Slop_400;
        }
        else if ((BLE_Slop > 0x480) && (BLE_Slop <= 0x500))
    	{
            BLE_Slop =((BLE_Slop_500-BLE_Slop_480)*(BLE_Slop-0x480)+64)/128+BLE_Slop_480;
        }
        else if ((BLE_Slop > 0x500) && (BLE_Slop <= 0x600))
    	{
            BLE_Slop =((BLE_Slop_600-BLE_Slop_500)*(BLE_Slop-0x500)+128)/256+BLE_Slop_500;
    	}
    	else
    	{
            BLE_Slop =((BLE_Slop_800-BLE_Slop_600)*(BLE_Slop-0x600)+256)/512+BLE_Slop_600;
        }

        if (ucYAvg < ((g_DlcParameters.ucDlcYAvgThresholdL+g_DlcParameters.ucDlcYAvgThresholdM)/2))
    	{
            BLE_Slop_tmp =(((Dark_BLE_Slop_Min - BLE_Slop_400)*(((g_DlcParameters.ucDlcYAvgThresholdL+g_DlcParameters.ucDlcYAvgThresholdM)/2)-ucYAvg)/
                          ((g_DlcParameters.ucDlcYAvgThresholdL+g_DlcParameters.ucDlcYAvgThresholdM)/2)) + BLE_Slop_400);

            if(BLE_Slop > BLE_Slop_tmp)
            {
                BLE_Slop = BLE_Slop;
            }
            else
            {
                BLE_Slop = BLE_Slop_tmp;
            }
    	}
        else
        {
            BLE_Slop = BLE_Slop;
        }


        // ~~~~~~~~~~~~~~~~~~~~~~~~ Pure Image Patch  ~~~~~~~~~~~~~~~~~~~~~~~~
        BLE_Slop = (BLE_Slop*(256-g_wPIAlpha)+ 1024*g_wPIAlpha + 128)/256;

        //~~~~~~~~~ output = uwBLE_Blend �]with slow approach�^ ~~~~~~~~~~~~~~~~~~~~~

        if(BLE_Slop >(Pre_BLE_Slop/16))
        {
            if(g_ucDlcFlickAlpha==g_DlcParameters.ucDlcAlphaBlendingMin)
            {
                uwBLE_Blend = (WORD)(((16*(DWORD)BLE_Slop)+(63*(DWORD)Pre_BLE_Slop)+63)/64);
                Pre_BLE_Slop = uwBLE_Blend;
                uwBLE_Blend = (uwBLE_Blend+8)/16;
            }
            else
            {
                uwBLE_Blend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)BLE_Slop) +
                              ((128 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)(Pre_BLE_Slop/16))+127)/128);
                Pre_BLE_Slop = uwBLE_Blend*16;          //14-bits
            }
        }
        else
        {
            if(g_ucDlcFlickAlpha==g_DlcParameters.ucDlcAlphaBlendingMin)
            {
                uwBLE_Blend = (WORD)(((16*(DWORD)BLE_Slop)+(63*(DWORD)Pre_BLE_Slop))/64);
                Pre_BLE_Slop = uwBLE_Blend;
                uwBLE_Blend = (uwBLE_Blend+8)/16;
            }
            else
            {
                uwBLE_Blend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)BLE_Slop) +
                              ((128 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)(Pre_BLE_Slop/16)))/128);
                Pre_BLE_Slop = uwBLE_Blend*16;
            }
        }

        if(g_bSetDlcBleOn)
        {
            msWriteByte(L_BK_DLC(0x10),uwBLE_StartPoint_Blend);
            msWriteByte(H_BK_DLC(0x10),uwBLE_Blend>>3);
            msWriteByte(L_BK_DLC(0x0D),(msReadByte(L_BK_DLC(0x0D))&0xF8)|(uwBLE_Blend & 0x07));
        }
        else
        {
            msWriteByte(L_BK_DLC(0x10),0x00);
            msWriteByte(H_BK_DLC(0x10),0x80);
            msWriteByte(L_BK_DLC(0x0D),msReadByte(L_BK_DLC(0x0D))&0xF8);
        }

    }
    Pre_TotalYSum = g_wLumiAverageTemp;
    msDlc_FunctionExit();
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msDlcWriteCurveLSB(BOOL bWindow, BYTE ucIndex, BYTE ucValue)
{
    DWORD wRegStart;
    BYTE ucData;

    msDlc_FunctionEnter();

    if( MAIN_WINDOW == bWindow )
        wRegStart = REG_ADDR_DLC_DATA_LSB_START_MAIN;
    else
        wRegStart = REG_ADDR_DLC_DATA_LSB_START_SUB;

    switch(ucIndex)
    {
        case 0:
            wRegStart += 0;
            ucData = (msReadByte(wRegStart) & 0xF3) | (ucValue << 2);
            break;
        case 1:
            wRegStart += 0;
            ucData = (msReadByte(wRegStart) & 0xCF) | (ucValue << 4);
            break;
        case 2:
            wRegStart += 0;
            ucData = (msReadByte(wRegStart) & 0x3F) | (ucValue << 6);
            break;
        case 3:
            wRegStart += 1;
            ucData = (msReadByte(wRegStart) & 0xFC) | (ucValue);
            break;
        case 4:
            wRegStart += 1;
            ucData = (msReadByte(wRegStart) & 0xF3) | (ucValue << 2);
            break;
        case 5:
            wRegStart += 1;
            ucData = (msReadByte(wRegStart) & 0xCF) | (ucValue << 4);
            break;
        case 6:
            wRegStart += 1;
            ucData = (msReadByte(wRegStart) & 0x3F) | (ucValue << 6);
            break;
        case 7:
            wRegStart += 2;
            ucData = (msReadByte(wRegStart) & 0xFC) | (ucValue);
            break;
        case 8:
            wRegStart += 2;
            ucData = (msReadByte(wRegStart) & 0xF3) | (ucValue << 2);
            break;
        case 9:
            wRegStart += 2;
            ucData = (msReadByte(wRegStart) & 0xCF) | (ucValue << 4);
            break;
        case 10:
            wRegStart += 2;
            ucData = (msReadByte(wRegStart) & 0x3F) | (ucValue << 6);
            break;
        case 11:
            wRegStart += 3;
            ucData = (msReadByte(wRegStart) & 0xFC) | (ucValue);
            break;
        case 12:
            wRegStart += 3;
            ucData = (msReadByte(wRegStart) & 0xF3) | (ucValue << 2);
            break;
        case 13:
            wRegStart += 3;
            ucData = (msReadByte(wRegStart) & 0xCF) | (ucValue << 4);
            break;
        case 14:
            wRegStart += 3;
            ucData = (msReadByte(wRegStart) & 0x3F) | (ucValue << 6);
            break;
        case 15:
            wRegStart += 4;
            ucData = (msReadByte(wRegStart) & 0xFC) | (ucValue);
            break;
        case 16: // index 16
            wRegStart += 4;
            ucData = (msReadByte(wRegStart) & 0xF3) | (ucValue << 2);
            break;
        case 0xFF: // index n0
        default:
            wRegStart += 0;
            ucData = (msReadByte(wRegStart) & 0xFC) | (ucValue);
            break;
    }

    msWriteByte(wRegStart, ucData);

    msDlc_FunctionExit();
}

void msDlcWriteCurve(BOOL bWindow)
{
    BYTE ucTmp;
    WORD uwBlend;

#if XC_DLC_SET_DLC_CURVE_BOTH_SAME
    WORD uwTmp;
#endif

    msDlc_FunctionEnter();

    // Write data to luma curve ...
    if( MAIN_WINDOW == bWindow )
    {
        for(ucTmp=0; ucTmp<=0x0f; ucTmp++)
        {
            g_uwTable[ucTmp] = (g_wPIAlpha*(ucTmp*64 + 32)+(256-g_wPIAlpha)*g_uwTable[ucTmp]+128)/256;

            if(g_uwTable[ucTmp]>(g_uwPreTable[ucTmp]/16))
            {
                if(g_ucDlcFlickAlpha==g_DlcParameters.ucDlcAlphaBlendingMin)
                {
                    uwBlend = (WORD)(((16*(DWORD)g_uwTable[ucTmp])+(63*(DWORD)g_uwPreTable[ucTmp])+63)/64); //14-bits
                    g_uwPreTable[ucTmp] = uwBlend;                                                              //14-bits
                    uwBlend = (uwBlend+8)/16;                                                                     //10-bits
                }
                else
                {
                    uwBlend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)g_uwTable[ucTmp]) +
                              ((128 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)(g_uwPreTable[ucTmp]/16))+127)/128); //10-bits
                    g_uwPreTable[ucTmp] = uwBlend*16;                                                            //14-bits
                    uwBlend = uwBlend;
                }
            }
            else
            {
                if(g_ucDlcFlickAlpha==g_DlcParameters.ucDlcAlphaBlendingMin)
                {
                    uwBlend = (WORD)(((16*(DWORD)g_uwTable[ucTmp])+(63*(DWORD)g_uwPreTable[ucTmp]))/64);  //14-bits
                    g_uwPreTable[ucTmp] = uwBlend;                                                            //14-bits
                    uwBlend = (uwBlend+8)/16;                                                                   //10-bits
                }
                else
                {
                    uwBlend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)g_uwTable[ucTmp]) +
                              ((128 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)(g_uwPreTable[ucTmp]/16)))/128); //10-bits
                    g_uwPreTable[ucTmp] = uwBlend*16;                                                        //14-bits
                    uwBlend = uwBlend;
                }
            }

            g_uwTable[ucTmp] = uwBlend;
            g_ucTable[ucTmp] = g_uwTable[ucTmp]>>2;

            msWriteByte((REG_ADDR_DLC_DATA_START_MAIN+ucTmp), g_ucTable[ucTmp]);

            //DLC Bank 0x1A . Check for MSTV_Tool debug.
            if((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0xFF))==0x1A)
            {
                g_ucDLCInitBuffer[54+ucTmp]=g_ucTable[ucTmp];
            }

            msDlcWriteCurveLSB(bWindow, ucTmp, g_uwTable[ucTmp]&0x03);

            // set DLC curve index N0 & 16
            if (ucTmp == 0)
            {
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_MAIN, g_uwTable[0]>>2);
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_MAIN + 1, 0x01);
                msDlcWriteCurveLSB(MAIN_WINDOW, 0xFF, g_uwTable[0]&0x03);
            }

            if (ucTmp == 15)
            {
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_MAIN, 255-(g_uwTable[15]>>2));
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_MAIN + 1, 0x01);
                msDlcWriteCurveLSB(MAIN_WINDOW, 16, g_uwTable[15]&0x03);
            }
        }
    }
    else
    {
#if XC_DLC_SET_DLC_CURVE_BOTH_SAME
        //Mantis issue of 0232938:Hsense A3 DLC the divisor can't to be equal to zero .
        if((DLC_DEFLICK_BLEND_FACTOR - g_ucDlcFlickAlpha + 1)==0)
        {
            DLC_DEBUG(printk("\n Error in  [Kernel DLC][ %s  , %d ]  (DLC_DEFLICK_BLEND_FACTOR - g_ucDlcFlickAlpha + 1)=0 !!!\n", __FUNCTION__,__LINE__));
            return;
        }

        for(ucTmp=0; ucTmp<=0x0f; ucTmp++)
        {
            if(g_ucDlcFastLoop || (g_ucDlcFlickAlpha >= (DLC_DEFLICK_BLEND_FACTOR >> 1)))
            {
                if (g_ucDlcFastLoop)
                {
                    g_ucDlcFastLoop--;
                }

                // target value with alpha blending

                uwBlend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)g_ucTable[ucTmp] * 4) +
                          ((32 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)g_uwPreTable[ucTmp])+16)/32);

                if(uwBlend > g_uwPreTable[ucTmp])
                {
                    uwTmp = (uwBlend - g_uwPreTable[ucTmp]) / (DLC_DEFLICK_BLEND_FACTOR - g_ucDlcFlickAlpha + 1);
                    uwBlend = g_uwPreTable[ucTmp] + uwTmp;
                }
                else if(uwBlend < g_uwPreTable[ucTmp])
                {
                    uwTmp = (g_uwPreTable[ucTmp] - uwBlend) / (DLC_DEFLICK_BLEND_FACTOR - g_ucDlcFlickAlpha + 1);
                    uwBlend = g_uwPreTable[ucTmp] - uwTmp;
                }

                g_uwPreTable[ucTmp] = uwBlend;

                g_uwTable[ucTmp] = g_uwPreTable[ucTmp];
                g_ucTable[ucTmp] = g_uwTable[ucTmp]>>2;

                msWriteByte((REG_ADDR_DLC_DATA_START_SUB+ucTmp), g_ucTable[ucTmp]);
                msDlcWriteCurveLSB(bWindow, ucTmp, g_uwTable[ucTmp]&0x03);

                // set DLC curve index N0 & 16
                if (ucTmp == 0)
                {
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_SUB, g_uwTable[0]>>2);
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_SUB + 1, 0x01);
                    msDlcWriteCurveLSB(SUB_WINDOW, 0xFF, g_uwTable[0]&0x03);
                }

                if (ucTmp == 15)
                {
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_SUB, 255-(g_uwTable[15]>>2));
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_SUB + 1, 0x01);
                    msDlcWriteCurveLSB(SUB_WINDOW, 16, g_uwTable[15]&0x03);
                }
            }
            else if (g_ucDlcFlickAlpha == 0)
            {
                g_ucTable[ucTmp] = g_uwPreTable[ucTmp] / 4;
            }
            else if ((ucTmp%4) == (g_ucUpdateCnt%4))
            {
                // target value with alpha blending

                uwBlend = (WORD)((((DWORD) g_ucDlcFlickAlpha * (DWORD)g_ucTable[ucTmp] * 4) +
                          ((32 - (DWORD)g_ucDlcFlickAlpha) * (DWORD)g_uwPreTable[ucTmp])+16)/32);

                if(uwBlend > g_uwPreTable[ucTmp])
                {
                    if((uwBlend - g_uwPreTable[ucTmp]) > 4)
                    {
                        uwBlend = g_uwPreTable[ucTmp] + 4;
                    }
                }
                else if(uwBlend < g_uwPreTable[ucTmp])
                {
                    if((g_uwPreTable[ucTmp] - uwBlend) > 4)
                    {
                        uwBlend = g_uwPreTable[ucTmp] - 4;
                    }
                }

                g_uwPreTable[ucTmp] = uwBlend;

                g_uwTable[ucTmp] = g_uwPreTable[ucTmp];
                g_ucTable[ucTmp] = g_uwTable[ucTmp]>>2;

                msWriteByte((REG_ADDR_DLC_DATA_START_SUB+ucTmp), g_ucTable[ucTmp]);
                msDlcWriteCurveLSB(bWindow, ucTmp, g_uwTable[ucTmp]&0x03);

                // set DLC curve index N0 & 16
                if (ucTmp == 0)
                {
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_SUB, g_uwTable[0]>>2);
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_SUB + 1, 0x01);
                    msDlcWriteCurveLSB(SUB_WINDOW, 0xFF, g_uwTable[0]&0x03);
                }

                if (ucTmp == 15)
                {
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_SUB, 255-(g_uwTable[15]>>2));
                    msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_SUB + 1, 0x01);
                    msDlcWriteCurveLSB(SUB_WINDOW, 16, g_uwTable[15]&0x03);
                }
            }
        }
#else
        for(ucTmp=0; ucTmp<=0x0f; ucTmp++)
        {
            msWriteByte((REG_ADDR_DLC_DATA_START_SUB+ucTmp), g_ucTable[ucTmp]);
            msDlcWriteCurveLSB(bWindow, ucTmp, g_uwTable[ucTmp]&0x03);

            // set DLC curve index N0 & 16
            if (ucTmp == 0)
            {
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_SUB, g_uwTable[0]>>2);
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_N0_SUB + 1, 0x01);
                msDlcWriteCurveLSB(SUB_WINDOW, 0xFF, g_uwTable[0]&0x03);
            }

            if (ucTmp == 15)
            {
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_SUB, 255-(g_uwTable[15]>>2));
                msWriteByte(REG_ADDR_DLC_DATA_EXTEND_16_SUB + 1, 0x01);
                msDlcWriteCurveLSB(SUB_WINDOW, 16, g_uwTable[15]&0x03);
            }
        }
#endif

    }

    g_ucUpdateCnt++;

    msDlc_FunctionExit();
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BOOL msIsBlackVideoEnable( MS_BOOL bWindow )
{
    MS_BOOL bReturn;

    if( MAIN_WINDOW == bWindow)
    {
        if(msReadByte(REG_ADDR_VOP_SCREEN_CONTROL) & 0x02)
        {
            bReturn = TRUE;
        }
        else
        {
            bReturn = FALSE;
        }
    }
    else
    {
        if(msReadByte(REG_ADDR_VOP_SCREEN_CONTROL) & 0x20)
        {
            bReturn = TRUE;
        }
        else
        {
            bReturn = FALSE;
        }
    }

    return bReturn;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BOOL msDLCWriteMSTVToolBufferData( MS_U8 u8IDValue ,  MS_U8 u8DataByte  )
{
    MS_BOOL bReturn = TRUE;
    MS_U8 ucTmp=0;

    g_DlcParameters.ucDlcSepPointH =g_ucDLCInitBuffer[1];
    g_DlcParameters.ucDlcSepPointL =g_ucDLCInitBuffer[2];
    g_DlcParameters.uwDlcBleStartPointTH =(((WORD)g_ucDLCInitBuffer[4]<<8)&0xFF00)|g_ucDLCInitBuffer[3];
    g_DlcParameters.uwDlcBleEndPointTH =(((WORD)g_ucDLCInitBuffer[6]<<8)&0xFF00)|g_ucDLCInitBuffer[5];
    g_DlcParameters.uwDlcBLESlopPoint_1 =(((WORD)g_ucDLCInitBuffer[8]<<8)&0xFF00)|g_ucDLCInitBuffer[7];
    g_DlcParameters.uwDlcBLESlopPoint_2 =(((WORD)g_ucDLCInitBuffer[10]<<8)&0xFF00)|g_ucDLCInitBuffer[9];
    g_DlcParameters.uwDlcBLESlopPoint_3 =(((WORD)g_ucDLCInitBuffer[12]<<8)&0xFF00)|g_ucDLCInitBuffer[11];
    g_DlcParameters.uwDlcBLESlopPoint_4 =(((WORD)g_ucDLCInitBuffer[14]<<8)&0xFF00)|g_ucDLCInitBuffer[13];
    g_DlcParameters.uwDlcBLESlopPoint_5 =(((WORD)g_ucDLCInitBuffer[16]<<8)&0xFF00)|g_ucDLCInitBuffer[15];
    g_DlcParameters.uwDlcDark_BLE_Slop_Min =(((WORD)g_ucDLCInitBuffer[18]<<8)&0xFF00)|g_ucDLCInitBuffer[17];

    if((g_ucDLCInitBuffer[19]<=128)&&(g_ucDLCInitBuffer[19]>=1)) // 1~128
    {
        g_DlcParameters.ucDlcAlphaBlendingMin=g_ucDLCInitBuffer[19];
    }
    else
    {
        printk("{DLC Tool ERROR]  DlcAlphaBlendingMin= %d   value is error!!!     (1~128) \n",g_ucDLCInitBuffer[19]);
        g_ucDLCInitBuffer[19]=g_DlcParameters.ucDlcAlphaBlendingMin;
        bReturn = FALSE;
    }

    if((g_ucDLCInitBuffer[20]<=128)&&(g_ucDLCInitBuffer[20]>=1)) // 1~128
    {
        g_DlcParameters.ucDlcAlphaBlendingMax=g_ucDLCInitBuffer[20];
    }
    else
    {
        printk("{DLC Tool ERROR]  DlcAlphaBlendingMax= %d  value is error!!!    (1~128) \n",g_ucDLCInitBuffer[20]);
        g_ucDLCInitBuffer[20]=g_DlcParameters.ucDlcAlphaBlendingMax;
        bReturn = FALSE;
    }

    g_DlcParameters.ucDlcFlicker_alpha=g_ucDLCInitBuffer[21];

    if(g_ucDLCInitBuffer[22]>=48) //  48~255
    {
        g_DlcParameters.ucDlcYAVG_L_TH=g_ucDLCInitBuffer[22];
    }
    else
    {
        printk("{DLC Tool ERROR]  DlcYAVG_L_TH= %d  value is error!!!    (48~255) \n",g_ucDLCInitBuffer[22]);
        g_ucDLCInitBuffer[22]=g_DlcParameters.ucDlcYAVG_L_TH;
        bReturn = FALSE;
    }

    if(g_ucDLCInitBuffer[23]>=48) //  48~255
    {
        g_DlcParameters.ucDlcYAVG_H_TH=g_ucDLCInitBuffer[23];
    }
    else
    {
        printk("{DLC Tool ERROR]  DlcYAVG_H_TH= %d  value is error!!!    (48~255) \n",g_ucDLCInitBuffer[23]);
        g_ucDLCInitBuffer[23]=g_DlcParameters.ucDlcYAVG_H_TH;
        bReturn = FALSE;
    }

    g_DlcParameters.ucDlcDiffBase_L=g_ucDLCInitBuffer[24];
    g_DlcParameters.ucDlcDiffBase_M=g_ucDLCInitBuffer[25];
    g_DlcParameters.ucDlcDiffBase_H=g_ucDLCInitBuffer[26];

    if(g_ucDLCInitBuffer[27]>=48) //  48~255
    {
        g_DlcParameters.ucDlcCurveDiff_L_TH=g_ucDLCInitBuffer[27];
    }
    else
    {
        printk("{DLC Tool ERROR]  DlcCurveDiff_L_TH= %d value is error!!!    (48~255) \n",g_ucDLCInitBuffer[27]);
        g_ucDLCInitBuffer[27]=g_DlcParameters.ucDlcCurveDiff_L_TH;
        bReturn = FALSE;
    }

    if(g_ucDLCInitBuffer[28]>=48) //  48~255
    {
        g_DlcParameters.ucDlcCurveDiff_H_TH=g_ucDLCInitBuffer[28];
    }
    else
    {
        printk("{DLC Tool ERROR]  DlcCurveDiff_H_TH= %d value is error!!!    (48~255) \n",g_ucDLCInitBuffer[28]);
        g_ucDLCInitBuffer[28]=g_DlcParameters.ucDlcCurveDiff_H_TH;
        bReturn = FALSE;
    }

    g_DlcParameters.ucDlcYAvgThresholdL=g_ucDLCInitBuffer[29];
    g_DlcParameters.ucDlcYAvgThresholdM=g_ucDLCInitBuffer[30];
    g_DlcParameters.ucDlcYAvgThresholdH=g_ucDLCInitBuffer[31];

    if(g_ucDLCInitBuffer[32]<=2) //0~2
    {
        g_DlcParameters.ucDlcCurveMode=g_ucDLCInitBuffer[32];
    }
    else
    {
        printk("{DLC Tool ERROR]  DlcCurveMode= %d value is error!!!    (0~2) \n",g_ucDLCInitBuffer[32]);
        g_ucDLCInitBuffer[32]=g_DlcParameters.ucDlcCurveMode;
        bReturn = FALSE;
    }

    g_DlcParameters.ucDlcCurveModeMixAlpha=g_ucDLCInitBuffer[33];

    for(ucTmp=0; ucTmp<16; ucTmp++)
    {
        g_DlcParameters.ucLumaCurve[ucTmp]=g_ucDLCInitBuffer[70+ucTmp];
        g_DlcParameters.ucLumaCurve2_a[ucTmp]=g_ucDLCInitBuffer[86+ucTmp];
        g_DlcParameters.ucLumaCurve2_b[ucTmp]=g_ucDLCInitBuffer[102+ucTmp];
    }

    for(ucTmp=0; ucTmp<17; ucTmp++)
    {
        g_DlcParameters.ucDlcHistogramLimitCurve[ucTmp]=g_ucDLCInitBuffer[37+ucTmp];
    }

    return bReturn;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
MS_U32 msDLCRegTrans(MS_U8 u8Value)
{
    MS_U32 u32Ret = 0xFFFF;

    switch(u8Value)
    {
    case 0:
        u32Ret = REG_SC_BK30_06_L;
        break;
    case 1:
        u32Ret = REG_SC_BK30_06_L + 1;
        break;
    case 2:
        u32Ret = REG_SC_BK30_06_L + 3;
        break;
    case 3:
        u32Ret = REG_SC_BK30_06_L + 4;
        break;
    case 4:
        u32Ret = REG_SC_BK30_06_L + 5;
        break;
    case 5:
        u32Ret = REG_SC_BK30_06_L + 7;
        break;
    case 6:
        u32Ret = REG_SC_BK30_06_L + 8;
        break;
    case 7:
        u32Ret = REG_SC_BK30_06_L + 9;
        break;
    case 8:
        u32Ret = REG_SC_BK30_06_L + 10;
        break;
    case 9:
        u32Ret = REG_SC_BK30_06_L + 11;
        break;
    case 10:
        u32Ret = REG_SC_BK30_06_L + 12;
        break;
    case 11:
        u32Ret = REG_SC_BK30_06_L + 14;
        break;
    case 12:
        u32Ret = REG_SC_BK30_06_L + 15;
        break;
    case 13:
        u32Ret = REG_SC_BK30_06_L + 16;
        break;
    case 14:
        u32Ret = REG_SC_BK30_06_L + 17;
        break;
    case 15:
        u32Ret = REG_SC_BK30_06_L + 18;
        break;
    default:
        u32Ret = 0xFFFF;
        //MS_ASSERT(0);
        break;
    }

    return u32Ret;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BOOL msDLCMSTVToolReadWriteValue( void )
{
    MS_BOOL bReturn=FALSE;
    BYTE u8IDValue=0;
    BYTE u8DataByte=0;
    BYTE ucTmp=0;

    //Check R/C bit
    if ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_03_L, BIT(0))==BIT(0) )
    {
        u8IDValue=(BYTE)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_02_L, 0xFF);
        u8DataByte=(BYTE)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_04_L, 0xFF);

        if((u8DataByte>16)|(u8IDValue>181)|((u8IDValue+u8DataByte)>182))
        {
            printk("[DLC tool ERROR] Read   IDValue=%d ,   DataByte=%d   return FALSE     \n",u8IDValue,u8DataByte);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_03_L, 0xFE, 0xFF);
            return FALSE;
        }

        //MSTV Tool want to read the data
        if( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_03_L, BIT(8))==BIT(8) )
        {
            for (ucTmp = 0  ; ucTmp < u8DataByte; ucTmp++)
            {
                printk("[DLC tool Read Data]  g_ucDLCInitBuffer[%d]=0x%x \n",(u8IDValue+ucTmp),g_ucDLCInitBuffer[u8IDValue+ucTmp]);

                msWriteByte( msDLCRegTrans(ucTmp), g_ucDLCInitBuffer[u8IDValue+ucTmp] );
            }
            printk("[DLC tool Read Data]  Finish     \n");
        }
        else//MSTV Tool want to write the data
        {
            if((u8IDValue>=118)||(( u8IDValue>=34)&&(u8IDValue<=36))||(( u8IDValue>=54)&&(u8IDValue<=69)))
            {
                printk("[DLC tool ERROR] Read only data    IDValue=%d ,   DataByte=%d   return FALSE     \n",u8IDValue,u8DataByte);
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_03_L, 0xFE, 0xFF);
                //Read Only Data
                return FALSE;
            }

            for (ucTmp = 0  ; ucTmp < u8DataByte; ucTmp++)
            {
                g_ucDLCInitBuffer[u8IDValue+ucTmp] =msReadByte(msDLCRegTrans(ucTmp)) ;

                printk("[DLC tool Write Data]   _ucDLCInitBuffer[%d]=%x \n",(u8IDValue+ucTmp),g_ucDLCInitBuffer[u8IDValue+ucTmp]);
            }

            msDLCWriteMSTVToolBufferData(u8IDValue ,u8DataByte);
            printk("[DLC tool Write Data]  Finish     \n");
        }

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_03_L, 0, 0xFF);
    }
    bReturn = TRUE;

    return bReturn;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msDlcSetYGain(MS_U8 u8YGain, BOOL bWindow)
{
    if(MAIN_WINDOW == bWindow)
    {
        msWriteByte(REG_ADDR_DLC_MAIN_Y_GAIN, u8YGain);
    }
    else
    {
        msWriteByte(REG_ADDR_DLC_SUB_Y_GAIN, u8YGain);
    }
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
MS_U8 msDlcGetYGain(BOOL bWindow)
{
    MS_U8 ucYGain=0;

    if(MAIN_WINDOW == bWindow)
    {
        ucYGain = msReadByte(REG_ADDR_DLC_MAIN_Y_GAIN);
    }
    else
    {
        ucYGain = msReadByte(REG_ADDR_DLC_SUB_Y_GAIN);
    }
    return ucYGain;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msDlcSetCGain(MS_U8 u8CGain, BOOL bWindow)
{
    if(MAIN_WINDOW == bWindow)
    {
        msWriteByte(REG_ADDR_DLC_MAIN_C_GAIN, u8CGain);
    }
    else
    {
        msWriteByte(REG_ADDR_DLC_SUB_C_GAIN, u8CGain);
    }
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
MS_U8 msDlcGetCGain(BOOL bWindow)
{
    MS_U8 ucCGain=0;

    if(MAIN_WINDOW == bWindow)
    {
        ucCGain = msReadByte(REG_ADDR_DLC_MAIN_C_GAIN);
    }
    else
    {
        ucCGain = msReadByte(REG_ADDR_DLC_SUB_C_GAIN);
    }
    return ucCGain;
}


#endif

BOOL msHDRInit(MS_U16 *u16HDRInit , MS_U16 u16HDRInitSize)
{
    MS_U16 i = 0, j = 0;

    // Please check the i need less than u16HDRInitSize.
    g_HDRinitParameters.bHDREnable     = u16HDRInit[i++];   // 0
    g_HDRinitParameters.u16HDRFunctionSelect     = u16HDRInit[i++];

    g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8ColorPrimaries            = u16HDRInit[i++]; // 2
    g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8TransferCharacteristics   = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8MatrixCoefficients         = u16HDRInit[i++];

    g_HDRinitParameters.DLC_HDRToneMappingData.u16Smin     = u16HDRInit[i++]; // 5
    g_HDRinitParameters.DLC_HDRToneMappingData.u16Smed     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16Smax     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmin     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmed     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmax     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16MidSourceOffset     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16MidTargetOffset     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16MidSourceRatio     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRToneMappingData.u16MidTargetRatio     = u16HDRInit[i++];

    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tRx     = u16HDRInit[i++];    // 15
    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tRy     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tGx     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tGy     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tBx     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tBy     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tWx     = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRGamutMappingData.u16tWy     = u16HDRInit[i++];

    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u8EOTF = u16HDRInit[i++]; // 23
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Rx = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Ry = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Gx = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Gy = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Bx = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16By = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Wx = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Wy = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Lmax = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Lmin = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16MaxCLL = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16MaxFALL = u16HDRInit[i++];

    g_HDRinitParameters.DLC_HDRCustomerDlcCurve.bFixHdrCurve = u16HDRInit[i++];  // 36
    g_HDRinitParameters.DLC_HDRCustomerDlcCurve.u16DlcCurveSize = u16HDRInit[i++];
    if (g_HDRinitParameters.DLC_HDRCustomerDlcCurve.pucDlcCurve != NULL)
    {
        kfree(g_HDRinitParameters.DLC_HDRCustomerDlcCurve.pucDlcCurve);
        g_HDRinitParameters.DLC_HDRCustomerDlcCurve.pucDlcCurve = NULL;
    }
    if (g_HDRinitParameters.DLC_HDRCustomerDlcCurve.u16DlcCurveSize > 0)
    {
        g_HDRinitParameters.DLC_HDRCustomerDlcCurve.pucDlcCurve = (MS_U8*)kmalloc(g_HDRinitParameters.DLC_HDRCustomerDlcCurve.u16DlcCurveSize, GFP_ATOMIC);
        for ( j = 0 ; j < g_HDRinitParameters.DLC_HDRCustomerDlcCurve.u16DlcCurveSize ; j++ )
        {
            g_HDRinitParameters.DLC_HDRCustomerDlcCurve.pucDlcCurve[j] = u16HDRInit[i++];
        }
    }

    g_HDRinitParameters.DLC_HDRCustomerColorPrimaries.bCustomerEnable = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRCustomerColorPrimaries.u16sWx = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRCustomerColorPrimaries.u16sWy = u16HDRInit[i++];

    g_HDRinitParameters.DLC_HDRHdmiTxAviInfoFrame.PixelFormat = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRHdmiTxAviInfoFrame.Colorimetry = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRHdmiTxAviInfoFrame.ExtendedColorimetry = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRHdmiTxAviInfoFrame.RgbQuantizationRange = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRHdmiTxAviInfoFrame.YccQuantizationRange = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRHdmiTxAviInfoFrame.StaticMetadataDescriptorID = u16HDRInit[i++];

    g_HDRinitParameters.DLC_HDRMetadataSeiContentLightLevel.bUsed = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataSeiContentLightLevel.bContentLightLevelEnabled = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataSeiContentLightLevel.u16MaxContentLightLevel = u16HDRInit[i++];
    g_HDRinitParameters.DLC_HDRMetadataSeiContentLightLevel.u16MaxPicAverageLightLevel = u16HDRInit[i++];

    HDR_DBG_HAL(printk("\n  Kernel  bHDREnable=%d    \n",g_HDRinitParameters.bHDREnable ));
    HDR_DBG_HAL(printk("\n  Kernel  u16HDRFunctionSelect=%d    \n",g_HDRinitParameters.u16HDRFunctionSelect ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataMpegVUI.u8ColorPrimaries =%d    \n",g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8ColorPrimaries ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataMpegVUI.u8TransferCharacteristics=%d    \n",g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8TransferCharacteristics ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataMpegVUI.u8MatrixCoefficients=%d    \n",g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8MatrixCoefficients ));

    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16Smin=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16Smin ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16Smed=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16Smed ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16Smax=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16Smax ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16Tmin=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmin ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16Tmed=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmed ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16Tmax=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmax ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16MidSourceOffset=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16MidSourceOffset ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16MidTargetOffset=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16MidTargetOffset ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16MidSourceRatio=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16MidSourceRatio ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRToneMappingData.u16MidTargetRatio=%d    \n",g_HDRinitParameters.DLC_HDRToneMappingData.u16MidTargetRatio ));

    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tRx=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tRx ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tRy=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tRy ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tGx=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tGx));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tGy=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tGy ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tBx=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tBx ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tBy=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tBy ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tWx=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tWx ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRGamutMappingData.u16tWy=%d    \n",g_HDRinitParameters.DLC_HDRGamutMappingData.u16tWy ));


    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u8EOTF=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u8EOTF ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Rx=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Rx ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Ry=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Ry ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Gx=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Gx ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Gy=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Gy ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Bx=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Bx ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16By=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16By ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Wx=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Wx ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Wy=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Wy ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Lmax=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Lmax) );
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16Lmin=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16Lmin ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16MaxCLL=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16MaxCLL ));
    HDR_DBG_HAL(printk("\n  Kernel  DLC_HDRMetadataHdmiTxInfoFrame.u16MaxFALL=%d    \n",g_HDRinitParameters.DLC_HDRMetadataHdmiTxInfoFrame.u16MaxFALL  ));

    HDR_DBG_HAL(printk("\n  Kernel  bFixHdrCurve=%d    \n",g_HDRinitParameters.DLC_HDRCustomerDlcCurve.bFixHdrCurve ));
    if (g_HDRinitParameters.DLC_HDRCustomerDlcCurve.bFixHdrCurve == TRUE &&
        g_HDRinitParameters.DLC_HDRCustomerDlcCurve.pucDlcCurve != NULL &&
        g_HDRinitParameters.DLC_HDRCustomerDlcCurve.u16DlcCurveSize > 0)
    {
        HDR_DBG_HAL(printk("\n  Fix Hdr Curve is :  \n"));
        for ( j = 0 ; j < g_HDRinitParameters.DLC_HDRCustomerDlcCurve.u16DlcCurveSize ; j++ )
        {
            HDR_DBG_HAL(printk("%d   ",g_HDRinitParameters.DLC_HDRCustomerDlcCurve.pucDlcCurve[j]));
        }
        HDR_DBG_HAL(printk("\n"));
    }

    HDR_DBG_HAL(printk("\n  Kernel  use customer color primaries =%d    \n", g_HDRinitParameters.DLC_HDRCustomerColorPrimaries.bCustomerEnable ));
    if (g_HDRinitParameters.DLC_HDRCustomerColorPrimaries.bCustomerEnable == TRUE)
    {
        HDR_DBG_HAL(printk("\n  Kernel  customer color primaries Wx =%d    \n",g_HDRinitParameters.DLC_HDRCustomerColorPrimaries.u16sWx));
        HDR_DBG_HAL(printk("\n  Kernel  customer color primaries Wy =%d    \n",g_HDRinitParameters.DLC_HDRCustomerColorPrimaries.u16sWy));
    }

#if 1
    HDRtmp.smin = g_HDRinitParameters.DLC_HDRToneMappingData.u16Smin ;
    HDRtmp.smax = g_HDRinitParameters.DLC_HDRToneMappingData.u16Smax;
    HDRtmp.tmax = g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmax;
    HDRtmp.tmin = g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmin;
    HDRtmp.smed = g_HDRinitParameters.DLC_HDRToneMappingData.u16Smed;
    HDRtmp.tmed = g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmed;

    g_u16HdrY2RRatio = 2;
    g_u16Hdr3x3Ratio = 8;

#if HDR_DEBUG_MODE
    MApi_GFLIP_XC_W2BYTE(REG_SC_BK1A_38_L, HDRtmp.smin);
    MApi_GFLIP_XC_W2BYTE(REG_SC_BK1A_39_L, HDRtmp.smax);
    MApi_GFLIP_XC_W2BYTE(REG_SC_BK1A_3A_L, HDRtmp.tmin);
    MApi_GFLIP_XC_W2BYTE(REG_SC_BK1A_3B_L, HDRtmp.tmax);
    MApi_GFLIP_XC_W2BYTE(REG_SC_BK1A_3C_L, HDRtmp.smed);
    MApi_GFLIP_XC_W2BYTE(REG_SC_BK1A_3D_L, HDRtmp.tmed);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK1A_3E_L, 0,0xFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK1A_3E_L, 0,0xFF00);
    MApi_GFLIP_XC_W2BYTE(REG_SC_BK1A_3F_L, 0x208);
#endif
#endif
    return TRUE ;
}

// Reference code for Wright
void MDrv_HDR_Main(void)
{

}

