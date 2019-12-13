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
/// @file   Mhal_mtlb.c
/// @brief  MTLB Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/printk.h>
#include "MsTypes.h"
#include "mdrv_types.h"
#include "mdrv_miu.h"
#include "regMIU.h"
#include "mhal_miu.h"
#include "mdrv_system.h"


//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define MIU_CLIENT_GP0              \
    MIU_CLIENT_NONE,                \
    MIU_CLIENT_VIVALDI9_DECODER_R,  \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_SECURE_R2_RW,        \
    MIU_CLIENT_AU_R2_RW,            \
    MIU_CLIENT_VD_R2D_RW,           \
    MIU_CLIENT_PM51_RW,             \
    MIU_CLIENT_VD_R2I_R,            \
    MIU_CLIENT_USB_UHC1_RW,         \
    MIU_CLIENT_USB_UHC2_RW,         \
    MIU_CLIENT_USB_UHC0_RW,         \
    MIU_CLIENT_MVD_BBU_RW,          \
    MIU_CLIENT_EMAC_RW,             \
    MIU_CLIENT_BDMA_RW,             \
    MIU_CLIENT_DUMMY

#define MIU_CLIENT_GP1              \
    MIU_CLIENT_VIVALDI9_MAD_RW,     \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_SC_DIPW_RW,          \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_TSP_W,               \
    MIU_CLIENT_TSP_PVR0_W,          \
    MIU_CLIENT_SDIO_RW,             \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_VE_W,                \
    MIU_CLIENT_VE_R,                \
    MIU_CLIENT_DUMMY

#define MIU_CLIENT_GP2              \
    MIU_CLIENT_GE_RW,               \
    MIU_CLIENT_HVD_BBU_R,           \
    MIU_CLIENT_EVD_BBU_R,           \
    MIU_CLIENT_HISPEED_UART_RW,     \
    MIU_CLIENT_MVD_RW,              \
    MIU_CLIENT_G3D_RW,              \
    MIU_CLIENT_GPD_RW,              \
    MIU_CLIENT_MFE0_W,              \
    MIU_CLIENT_MFE1_R,              \
    MIU_CLIENT_NAND_RW,             \
    MIU_CLIENT_JPD720P_RW,          \
    MIU_CLIENT_DSCRMB_RW,           \
    MIU_CLIENT_NJPD_RW,             \
    MIU_CLIENT_TSP_ORZ_W,           \
    MIU_CLIENT_TSP_ORZ_R,           \
    MIU_CLIENT_MIIC0_RW

#define MIU_CLIENT_GP3              \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY

#define MIU_CLIENT_GP4              \
    MIU_CLIENT_MIPS_RW,             \
    MIU_CLIENT_MTF_W,               \
    MIU_CLIENT_GOP0_R,              \
    MIU_CLIENT_GOP1_R,              \
    MIU_CLIENT_GOP2_R,              \
    MIU_CLIENT_GOP3_R,              \
    MIU_CLIENT_SC_OPMAIN_RW,        \
    MIU_CLIENT_SC_IPMAIN_RW,        \
    MIU_CLIENT_SC_IPSUB_RW,         \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_MVOP_128BIT_R,       \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_HVD_RW,              \
    MIU_CLIENT_EVD_RW,              \
    MIU_CLIENT_SECHVD_RW,           \
    MIU_CLIENT_SECEVD_RW

#define MIU1_CLIENT_GP0             \
    MIU_CLIENT_NONE,                \
    MIU_CLIENT_VIVALDI9_DECODER_R,  \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_SECURE_R2_RW,        \
    MIU_CLIENT_AU_R2_RW,            \
    MIU_CLIENT_VD_R2D_RW,           \
    MIU_CLIENT_PM51_RW,             \
    MIU_CLIENT_VD_R2I_R,            \
    MIU_CLIENT_USB_UHC1_RW,         \
    MIU_CLIENT_USB_UHC2_RW,         \
    MIU_CLIENT_USB_UHC0_RW,         \
    MIU_CLIENT_MVD_BBU_RW,          \
    MIU_CLIENT_EMAC_RW,             \
    MIU_CLIENT_BDMA_RW,             \
    MIU_CLIENT_DUMMY

#define MIU1_CLIENT_GP1             \
    MIU_CLIENT_VIVALDI9_MAD_RW,     \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_SC_DIPW_RW,          \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_TSP_W,               \
    MIU_CLIENT_TSP_PVR0_W,          \
    MIU_CLIENT_SDIO_RW,             \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_VE_W,                \
    MIU_CLIENT_VE_R,                \
    MIU_CLIENT_DUMMY

#define MIU1_CLIENT_GP2             \
    MIU_CLIENT_GE_RW,               \
    MIU_CLIENT_HVD_BBU_R,           \
    MIU_CLIENT_EVD_BBU_R,           \
    MIU_CLIENT_HISPEED_UART_RW,     \
    MIU_CLIENT_MVD_RW,              \
    MIU_CLIENT_G3D_RW,              \
    MIU_CLIENT_GPD_RW,              \
    MIU_CLIENT_MFE0_W,              \
    MIU_CLIENT_MFE1_R,              \
    MIU_CLIENT_NAND_RW,             \
    MIU_CLIENT_JPD720P_RW,          \
    MIU_CLIENT_DSCRMB_RW,           \
    MIU_CLIENT_NJPD_RW,             \
    MIU_CLIENT_TSP_ORZ_W,           \
    MIU_CLIENT_TSP_ORZ_R,           \
    MIU_CLIENT_MIIC0_RW

#define MIU1_CLIENT_GP3             \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_DUMMY

#define MIU1_CLIENT_GP4             \
    MIU_CLIENT_MIPS_RW,             \
    MIU_CLIENT_MTF_W,               \
    MIU_CLIENT_GOP0_R,              \
    MIU_CLIENT_GOP1_R,              \
    MIU_CLIENT_GOP2_R,              \
    MIU_CLIENT_GOP3_R,              \
    MIU_CLIENT_SC_OPMAIN_RW,        \
    MIU_CLIENT_SC_IPMAIN_RW,        \
    MIU_CLIENT_SC_IPSUB_RW,         \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_MVOP_128BIT_R,       \
    MIU_CLIENT_DUMMY,               \
    MIU_CLIENT_HVD_RW,              \
    MIU_CLIENT_EVD_RW,              \
    MIU_CLIENT_SECHVD_RW,           \
    MIU_CLIENT_SECEVD_RW

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#define IDNUM_KERNELPROTECT (10) 

const eMIUClientID clientTbl[MIU_MAX_DEVICE][MIU_MAX_TBL_CLIENT] =
{
    {
        MIU_CLIENT_GP0,
        MIU_CLIENT_GP1,
        MIU_CLIENT_GP2,
        MIU_CLIENT_GP3,
        MIU_CLIENT_GP4
    },
    {
        MIU1_CLIENT_GP0,
        MIU1_CLIENT_GP1,
        MIU1_CLIENT_GP2,
        MIU1_CLIENT_GP3,
        MIU1_CLIENT_GP4
    }
};

MS_U8 clientId_KernelProtect[IDNUM_KERNELPROTECT] =
{
    MIU_CLIENT_MIPS_RW, 
    MIU_CLIENT_NAND_RW, 
    MIU_CLIENT_USB_UHC0_RW, 
    MIU_CLIENT_USB_UHC1_RW,
    MIU_CLIENT_USB_UHC2_RW, 
    MIU_CLIENT_G3D_RW, 
    MIU_CLIENT_USB3_RW, 
    MIU_CLIENT_SDIO_RW,
    MIU_CLIENT_SATA_RW, 
    MIU_CLIENT_USB_UHC3_RW
};

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
typedef enum
{
  E_CHIP_MIU_0 = 0,
  E_CHIP_MIU_1,
  E_CHIP_MIU_2,
  E_CHIP_MIU_3,
  E_CHIP_MIU_NUM,
} CHIP_MIU_ID;

//-------------------------------------------------------------------------------------------------
//  Macros
//-------------------------------------------------------------------------------------------------
#define _phy_to_miu_offset(MiuSel, Offset, PhysAddr) if (PhysAddr < ARM_MIU1_BASE_ADDR) \
                                                        {MiuSel = E_CHIP_MIU_0; Offset = PhysAddr;} \
                                                     else \
                                                         {MiuSel = E_CHIP_MIU_1; Offset = PhysAddr - ARM_MIU1_BASE_ADDR;}
                                                 

#define _miu_offset_to_phy(MiuSel, Offset, PhysAddr) if (MiuSel == E_CHIP_MIU_0) \
                                                        {PhysAddr = Offset;} \
                                                     else \
                                                         {PhysAddr = Offset + ARM_MIU1_BASE_ADDR;}

#define MIU_HAL_ERR(fmt, args...)   printk(KERN_ERR "miu hal error %s:%d" fmt,__FUNCTION__,__LINE__,## args)

//-------------------------------------------------------------------------------------------------
//  Local Variable
//-------------------------------------------------------------------------------------------------
//static MS_U32 _gMIU_MapBase = 0xBF200000;      //default set to MIPS platfrom
static MS_U32 _gMIU_MapBase = 0xFD200000;   //default set to arm 32bit platfrom

MS_BOOL IDEnables[MIU_MAX_DEVICE][MIU_MAX_PROTECT_BLOCK][MIU_MAX_PROTECT_ID] =
        {
            {{0}, {0}, {0}, {0}},
            {{0}, {0}, {0}, {0}}
        }; //ID enable for protect block 0~3

MS_U8   IDs[MIU_MAX_DEVICE][MIU_MAX_PROTECT_ID] =
        {
            {0}, {0}
        }; //IDs for protection


//-------------------------------------------------------------------------------------------------
//  MTLB HAL internal function
//-------------------------------------------------------------------------------------------------
MS_U32 HAL_MIU_BA2PA(MS_U32 u32BusAddr)
{
    MS_PHYADDR u32PhyAddr = 0x0;
    
    // pa = ba - offset 
	if( (u32BusAddr >= ARM_MIU0_BUS_BASE) && (u32BusAddr < ARM_MIU1_BUS_BASE) )	// MIU0
		u32PhyAddr = u32BusAddr - ARM_MIU0_BUS_BASE + ARM_MIU0_BASE_ADDR;
	else
		u32PhyAddr = u32BusAddr - ARM_MIU1_BUS_BASE + ARM_MIU1_BASE_ADDR;	// MIU1
    
	return u32PhyAddr;
}

MS_S16 HAL_MIU_GetClientInfo(MS_U8 u8MiuDev, eMIUClientID eClientID)
{
    MS_U8 idx;

    if (MIU_MAX_DEVICE <= u8MiuDev)
    {
        MIU_HAL_ERR("Wrong MIU device:%u\n", u8MiuDev);
        return (-1);
    }

    for (idx = 0; idx < MIU_MAX_TBL_CLIENT; idx++)
        if (eClientID == clientTbl[u8MiuDev][idx])
            return idx;
    return (-1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MIU_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIU_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MIU_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIU_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        MIU_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIU_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        MIU_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIU_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

void HAL_MIU_SetProtectID(MS_U32 u32Reg, MS_U8 u8MiuDev, MS_U8 u8ClientID)
{
    MS_S16 sVal = HAL_MIU_GetClientInfo(u8MiuDev, (eMIUClientID)u8ClientID);
    MS_S16 sIDVal;

    if (0 > sVal)
        sVal = 0;

    sIDVal = HAL_MIU_ReadByte(u32Reg);
    sIDVal &= 0xc0;
    sIDVal |= sVal;
    HAL_MIU_WriteByte(u32Reg, sIDVal);
}

MS_BOOL HAL_MIU_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable)
{
    MS_U8 u8Val = HAL_MIU_ReadByte(u32RegAddr);
    if (!u32RegAddr)
    {
        MIU_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_MIU_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_MIU_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

MS_BOOL HAL_MIU_SetGroupID(MS_U8 u8MiuSel, MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_U32 u32RegAddrID, MS_U32 u32RegAddrIDenable)
{
    MS_U32 u32index0, u32index1;
    MS_U8 u8ID;
    MS_U8 u8isfound0, u8isfound1;
    MS_U16 u16idenable;

    //reset IDenables for protect u8Blockx
    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        IDEnables[u8MiuSel][u8Blockx][u32index0] = 0;
    }

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        u8ID = pu8ProtectId[u32index0];

        //Unused ID
        if(u8ID == 0)
           continue;

        u8isfound0 = FALSE;

        for(u32index1 = 0; u32index1 < MIU_MAX_PROTECT_ID; u32index1++)
        {
            if(IDs[u8MiuSel][u32index1] == u8ID)
            {
                //ID reused former setting
                IDEnables[u8MiuSel][u8Blockx][u32index1] = 1;
                u8isfound0 = TRUE;
                break;
            }
        }


        //Need to create new ID in IDs
        if(u8isfound0 != TRUE)
        {
            u8isfound1 = FALSE;

            for(u32index1 = 0; u32index1 < MIU_MAX_PROTECT_ID; u32index1++)
            {
                if(IDs[u8MiuSel][u32index1] == 0)
                {
                    IDs[u8MiuSel][u32index1] = u8ID;
                    IDEnables[u8MiuSel][u8Blockx][u32index1] = 1;
                    u8isfound1 = TRUE;
                    break;
                }
            }

            //ID overflow
            if(u8isfound1 == FALSE)
                return FALSE;
        }
    }

    u16idenable = 0;

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        if(IDEnables[u8MiuSel][u8Blockx][u32index0] == 1)
            u16idenable |= (1<<u32index0);
    }

    HAL_MIU_Write2Byte(u32RegAddrIDenable, u16idenable);

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
         HAL_MIU_SetProtectID(u32RegAddrID + u32index0, (u8MiuSel== E_MIU_1), IDs[u8MiuSel][u32index0]);
    }

    return TRUE;
}


MS_BOOL HAL_MIU_ResetGroupID(MS_U8 u8MiuSel, MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_U32 u32RegAddrID, MS_U32 u32RegAddrIDenable)
{
    MS_U32 u32index0, u32index1;
    MS_U8 u8isIDNoUse;
    MS_U16 u16idenable;

    //reset IDenables for protect u8Blockx
    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        IDEnables[u8MiuSel][u8Blockx][u32index0] = 0;
    }

    u16idenable = 0x0;

    HAL_MIU_Write2Byte(u32RegAddrIDenable, u16idenable);

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        u8isIDNoUse  = FALSE;

        for(u32index1 = 0; u32index1 < MIU_MAX_PROTECT_BLOCK; u32index1++)
        {
            if(IDEnables[u8MiuSel][u32index1][u32index0] == 1)
            {
                //protect ID is still be used
                u8isIDNoUse  = FALSE;
                break;
            }
            u8isIDNoUse  = TRUE;
        }

        if(u8isIDNoUse == TRUE)
            IDs[u8MiuSel][u32index0] = 0;
    }

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
         HAL_MIU_SetProtectID(u32RegAddrID + u32index0, (u8MiuSel== E_MIU_1), IDs[u8MiuSel][u32index0]);
    }

    return TRUE;
}

void HAL_MIU_Write2BytesBit(MS_U32 u32RegOffset, MS_BOOL bEnable, MS_U16 u16Mask)
{
    MS_U16 val = HAL_MIU_Read2Byte(u32RegOffset);
    val = (bEnable) ? (val | u16Mask) : (val & ~u16Mask);
    HAL_MIU_Write2Byte(u32RegOffset, val);
}

//-------------------------------------------------------------------------------------------------
//  MTLB HAL function
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_GetDefaultClientID_KernelProtect()
/// @brief \b Function \b Description:  Get default client id array pointer for protect kernel
/// @param <RET>           \b     : The pointer of Array of client IDs
////////////////////////////////////////////////////////////////////////////////
MS_U8* HAL_MIU_GetDefaultClientID_KernelProtect()
{
     if(IDNUM_KERNELPROTECT > 0)
         return  (MS_U8 *)&clientId_KernelProtect[0];

     return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Protect()
/// @brief \b Function \b Description:  Enable/Disable MIU Protection mode
/// @param u8Blockx        \b IN     : MIU Block to protect (0 ~ 4)
/// @param *pu8ProtectId   \b IN     : Allow specified client IDs to write
/// @param u32Start        \b IN     : Starting bus address
/// @param u32End          \b IN     : End bus address
/// @param bSetFlag        \b IN     : Disable or Enable MIU protection
///                                      - -Disable(0)
///                                      - -Enable(1)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_Protect(
                          MS_U8    u8Blockx,
                          MS_U8    *pu8ProtectId,
                          MS_U32   u32BusStart,
                          MS_U32   u32BusEnd,
                          MS_BOOL  bSetFlag
                         )
{
    MS_U32 u32RegAddr;
    MS_U32 u32Reg;
    MS_U32 u32RegAddrStar;
    MS_U32 u32RegAddrMSB;
    MS_U32 u32RegAddrIDenable;
    MS_U32 u32MiuProtectEn;
    MS_U16 u16Data;
    MS_U16 u16Data1;
    MS_U8  u8Data;
    MS_U8  u8MiuSel;
    MS_U32 u32Start, u32End;

    u32Start = HAL_MIU_BA2PA(u32BusStart);
    u32End = HAL_MIU_BA2PA(u32BusEnd);

    u8MiuSel = E_MIU_0;
    // Incorrect Block ID
    if(u8Blockx > 3)
    {
        MIU_HAL_ERR("Err: Out of the number of protect device\n");
        return false;
    }
    else if(((u32Start & ((1 << MIU_PAGE_SHIFT) -1)) != 0) || ((u32End & ((1 << MIU_PAGE_SHIFT) -1)) != 0))
    {
        MIU_HAL_ERR("Err: Protected address should be aligned to 8KB\n");
        return false;
    }
    else if(u32Start >= u32End)
    {
        MIU_HAL_ERR("Err: Start address is equal to or more than end address\n");
        return false;
    }

    //write_enable
    u8Data = 1 << u8Blockx;

    if( u32Start < ARM_MIU1_BASE_ADDR) 
    {
        u32MiuProtectEn = MIU_PROTECT_EN_INTERNAL;
        u32Reg = MIU_REG_BASE;

        switch (u8Blockx)
        {
            case E_MIU_BLOCK_0:
                u32RegAddr = MIU_PROTECT0_ID0;
                u32RegAddrMSB = MIU_PROTECT0_MSB;
                u32RegAddrStar = MIU_PROTECT0_START;
                u32RegAddrIDenable = MIU_PROTECT0_ID_ENABLE;
                break;
            case E_MIU_BLOCK_1:
                u32RegAddr = MIU_PROTECT0_ID0;
                u32RegAddrMSB = MIU_PROTECT1_MSB;
                u32RegAddrStar = MIU_PROTECT1_START;
                u32RegAddrIDenable = MIU_PROTECT1_ID_ENABLE;
                break;
            case E_MIU_BLOCK_2:
                u32RegAddr = MIU_PROTECT0_ID0;
                u32RegAddrMSB = MIU_PROTECT2_MSB;
                u32RegAddrStar = MIU_PROTECT2_START;
                u32RegAddrIDenable = MIU_PROTECT2_ID_ENABLE;
                break;
            case E_MIU_BLOCK_3:
                u32RegAddr = MIU_PROTECT0_ID0;
                u32RegAddrMSB = MIU_PROTECT3_MSB;
                u32RegAddrStar = MIU_PROTECT3_START;
                u32RegAddrIDenable = MIU_PROTECT3_ID_ENABLE;
                break;
            default:
                return false;
        }
    }
    else
    {
        u32MiuProtectEn = MIU1_PROTECT_EN;
        u32Reg = MIU1_REG_BASE;

        switch (u8Blockx)
        {
            case E_MIU_BLOCK_0:
                u32RegAddr = MIU1_PROTECT0_ID0;
                u32RegAddrMSB = MIU1_PROTECT0_MSB;
                u32RegAddrStar = MIU1_PROTECT0_START;
                u32RegAddrIDenable = MIU1_PROTECT0_ID_ENABLE;
                break;
                break;
            case E_MIU_BLOCK_1:
                u32RegAddr = MIU1_PROTECT0_ID0;
                u32RegAddrMSB = MIU1_PROTECT1_MSB;
                u32RegAddrStar = MIU1_PROTECT1_START;
                u32RegAddrIDenable = MIU1_PROTECT1_ID_ENABLE;
                break;
            case E_MIU_BLOCK_2:
                u32RegAddr = MIU1_PROTECT0_ID0;
                u32RegAddrMSB = MIU1_PROTECT2_MSB;
                u32RegAddrStar = MIU1_PROTECT2_START;
                u32RegAddrIDenable = MIU1_PROTECT2_ID_ENABLE;
                break;
            case E_MIU_BLOCK_3:
                u32RegAddr = MIU1_PROTECT0_ID0;
                u32RegAddrMSB = MIU1_PROTECT3_MSB;
                u32RegAddrStar = MIU1_PROTECT3_START;
                u32RegAddrIDenable = MIU1_PROTECT3_ID_ENABLE;
                break;
            default:
                return false;
        }
        u32Start-=ARM_MIU1_BASE_ADDR; 
        u32End-=ARM_MIU1_BASE_ADDR;;
        u8MiuSel = E_MIU_1;
    }

    // Disable MIU protect
    HAL_MIU_WriteRegBit(u32MiuProtectEn,u8Data,DISABLE);

    if ( bSetFlag )
    {
        // Set Protect IDs
        if(HAL_MIU_SetGroupID(u8MiuSel, u8Blockx, pu8ProtectId, u32RegAddr, u32RegAddrIDenable) == FALSE)
        {
            return FALSE;
        }

        // Set BIT29,30 of start/end address
        u16Data1 = HAL_MIU_Read2Byte(u32RegAddrMSB);
        u16Data1 &= ~(0x0003 << (u8Blockx * 4));
        u16Data1 |= (MS_U16)((u32Start >> 29) << (u8Blockx * 4));
        u16Data1 &= ~(0x0003 << (u8Blockx * 4 + 2));
        u16Data1 |= (MS_U16)(((u32End -1) >> 29) << (u8Blockx * 4 + 2));
        HAL_MIU_Write2Byte(u32RegAddrMSB, u16Data1);

        // Start Address
        u16Data = (MS_U16)(u32Start >> MIU_PAGE_SHIFT);   //8k/unit
        HAL_MIU_Write2Byte(u32RegAddrStar , u16Data);

        // End Address
        u16Data = (MS_U16)((u32End >> MIU_PAGE_SHIFT)-1);   //8k/unit;
        HAL_MIU_Write2Byte(u32RegAddrStar + 2, u16Data);

        // Enable MIU protect
        HAL_MIU_WriteRegBit(u32MiuProtectEn, u8Data, ENABLE);
    }
    else
    {
        // Reset Protect IDs
        HAL_MIU_ResetGroupID(u8MiuSel, u8Blockx, pu8ProtectId, u32RegAddr, u32RegAddrIDenable);
    }

    // clear log
    HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, TRUE, REG_MIU_PROTECT_LOG_CLR);
    HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, FALSE, REG_MIU_PROTECT_LOG_CLR);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_ParseOccupiedResource
/// @brief \b Function  \b Description: Parse occupied resource to software structure
/// @return             \b 0: Fail 1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_ParseOccupiedResource(void)
{
    MS_U8  u8MiuSel;
    MS_U8  u8Blockx;
    MS_U8  u8ClientID;
    MS_U16 u16idenable;
    MS_U32 u32index;
    MS_U32 u32RegAddr;
    MS_U32 u32RegAddrIDenable;

    for(u8MiuSel = E_MIU_0; u8MiuSel < MIU_MAX_DEVICE; u8MiuSel++)
    {
        for(u8Blockx = E_MIU_BLOCK_0; u8Blockx < E_MIU_BLOCK_NUM; u8Blockx++)
        {
            if(u8MiuSel == E_MIU_0)
            {
                u32RegAddr = MIU_PROTECT0_ID0;

                switch (u8Blockx)
                {
                    case E_MIU_BLOCK_0:
                        u32RegAddrIDenable = MIU_PROTECT0_ID_ENABLE;
                        break;
                    case E_MIU_BLOCK_1:
                        u32RegAddrIDenable = MIU_PROTECT1_ID_ENABLE;
                        break;
                    case E_MIU_BLOCK_2:
                        u32RegAddrIDenable = MIU_PROTECT2_ID_ENABLE;
                        break;
                    case E_MIU_BLOCK_3:
                        u32RegAddrIDenable = MIU_PROTECT3_ID_ENABLE;
                        break;
                    default:
                        return false;
                }
            }// if(u8MiuSel == E_MIU_0)
            else
            {
                u32RegAddr = MIU1_PROTECT0_ID0;

                switch (u8Blockx)
                {
                case E_MIU_BLOCK_0:
                     u32RegAddrIDenable = MIU1_PROTECT0_ID_ENABLE;
                     break;
                 case E_MIU_BLOCK_1:
                     u32RegAddrIDenable = MIU1_PROTECT1_ID_ENABLE;
                     break;
                 case E_MIU_BLOCK_2:
                     u32RegAddrIDenable = MIU1_PROTECT2_ID_ENABLE;
                     break;
                 case E_MIU_BLOCK_3:
                     u32RegAddrIDenable = MIU1_PROTECT3_ID_ENABLE;
                     break;
                 default:
                    return false;
                }
            }// if(u8MiuSel == E_MIU_0)

            u16idenable = HAL_MIU_Read2Byte(u32RegAddrIDenable);
            for(u32index = 0; u32index < MIU_MAX_PROTECT_ID; u32index++)
            {
                IDEnables[u8MiuSel][u8Blockx][u32index] = ((u16idenable >> u32index) & 0x1)? 1: 0;
            }
        }//for(u8Blockx = E_MIU_BLOCK_0; u8Blockx < E_MIU_BLOCK_NUM; u8Blockx++)

        for(u32index = 0; u32index < MIU_MAX_PROTECT_ID; u32index++)
        {
            u8ClientID = HAL_MIU_ReadByte(u32RegAddr + u32index);
            IDs[u8MiuSel][u32index] = clientTbl[u8MiuSel][u8ClientID];
        }
    }//for(u8MiuSel = E_MIU_0; u8MiuSel < E_MIU_NUM; u8MiuSel++)

    return TRUE;
}

#define GET_HIT_BLOCK(regval)       BITS_RANGE_VAL(regval, REG_MIU_PROTECT_HIT_NO)
#define GET_HIT_CLIENT(regval)      BITS_RANGE_VAL(regval, REG_MIU_PROTECT_HIT_ID)

MS_BOOL HAL_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo)
{
    MS_U16 ret = 0;
    MS_U16 loaddr = 0;
    MS_U16 hiaddr = 0;
    MS_U32 u32Address = 0;
    MS_U32 u32Reg = (u8MiuDev) ? MIU1_REG_BASE : MIU_REG_BASE;

    if (!pInfo)
        return FALSE;

    ret = HAL_MIU_Read2Byte(u32Reg+REG_MIU_PROTECT_STATUS);
    loaddr = HAL_MIU_Read2Byte(u32Reg+REG_MIU_PROTECT_LOADDR);
    hiaddr = HAL_MIU_Read2Byte(u32Reg+REG_MIU_PROTECT_HIADDR);

    pInfo->bHit = false;
    
    if (REG_MIU_PROTECT_HIT_FALG & ret)
    {
        pInfo->bHit = TRUE;

        pInfo->u8Block = (MS_U8)GET_HIT_BLOCK(ret);
        pInfo->u8Group = (MS_U8)(GET_HIT_CLIENT(ret) >> 4);
        pInfo->u8ClientID = (MS_U8)(GET_HIT_CLIENT(ret) & 0x0F);
        u32Address = (MS_U32)((hiaddr << 16) | loaddr) ;
        u32Address = u32Address * MIU_PROTECT_ADDRESS_UNIT;

        printk(KERN_ERR "MIU%u Block:%u Group:%u ClientID:%u Hitted_Address:0x%lx<->0x%lx\n", u8MiuDev,
        pInfo->u8Block, pInfo->u8Group, pInfo->u8ClientID, u32Address, u32Address + MIU_PROTECT_ADDRESS_UNIT - 1);

        //clear log
        HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, TRUE, REG_MIU_PROTECT_LOG_CLR);
        HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, FALSE, REG_MIU_PROTECT_LOG_CLR);
    }

    return TRUE;
}

