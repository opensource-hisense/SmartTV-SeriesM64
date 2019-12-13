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
/// @file   Mhal_emac.c
/// @brief  EMAC Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/mii.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/pci.h>
#include "mhal_emac.h"


#define MHal_MAX_INT_COUNTER    100
//-------------------------------------------------------------------------------------------------
//  EMAC hardware for Titania
//-------------------------------------------------------------------------------------------------

u32 MHal_EMAC_ReadReg32(struct net_device *ndev, u32 xoffset )
{
    u32 address = ndev->base_addr + xoffset*2;

    u32 xoffsetValueL = *( ( volatile u32* ) address ) & 0x0000FFFF;
    u32 xoffsetValueH = *( ( volatile u32* ) ( address + 4) ) << 0x10;
    return( xoffsetValueH | xoffsetValueL );
}

void MHal_EMAC_WritReg32(struct net_device *ndev ,u32 xoffset, u32 xval )
{
    u32 address = ndev->base_addr + xoffset*2;

    *( ( volatile u32 * ) address ) = ( u32 ) ( xval & 0x0000FFFF );
    *( ( volatile u32 * ) ( address + 4 ) ) = ( u32 ) ( xval >> 0x10 );
}

void MHal_EMAC_WritRam32( u32 uRamAddr, u32 xoffset, u32 xval )
{
    *( ( u32 * ) ( ( char * ) uRamAddr + xoffset ) ) = xval;
}

void MHal_EMAC_Write_SA1_MAC_Address(struct net_device *ndev, u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA1L, w0 );
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA1H, w1 );
}

void MHal_EMAC_Write_SA2_MAC_Address(struct net_device *ndev, u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA2L, w0 );
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA2H, w1 );
}

void MHal_EMAC_Write_SA3_MAC_Address(struct net_device *ndev, u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA3L, w0 );
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA3H, w1 );
}

void MHal_EMAC_Write_SA4_MAC_Address(struct net_device *ndev, u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA4L, w0 );
    MHal_EMAC_WritReg32(ndev, REG_ETH_SA4H, w1 );
}

//-------------------------------------------------------------------------------------------------
//  R/W EMAC register for Titania
//-------------------------------------------------------------------------------------------------

void MHal_EMAC_update_HSH(struct net_device *ndev, u32 mc0, u32 mc1)
{
    MHal_EMAC_WritReg32(ndev, REG_ETH_HSL, mc0 );
    MHal_EMAC_WritReg32(ndev, REG_ETH_HSH, mc1 );
}

//-------------------------------------------------------------------------------------------------
//  Read control register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CTL( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_CTL );
}

//-------------------------------------------------------------------------------------------------
//  Write Network control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CTL(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32(ndev, REG_ETH_CTL, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Network configuration register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CFG( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_CFG );
}

//-------------------------------------------------------------------------------------------------
//  Write Network configuration register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CFG( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32(ndev, REG_ETH_CFG, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RBQP( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_RBQP );
}
//-------------------------------------------------------------------------------------------------
//  Write RBQP
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RBQP(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32(ndev, REG_ETH_RBQP, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Address register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TAR(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32(ndev, REG_ETH_TAR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_TCR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_TCR);
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TCR(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_TCR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Transmit Status Register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TSR(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32(ndev, REG_ETH_TSR, xval );
}

u32 MHal_EMAC_Read_TSR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_TSR );
}

u32 MHal_EMAC_Read_RSR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_RSR );
}

void MHal_EMAC_Write_RSR(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32(ndev, REG_ETH_RSR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt status register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_ISR(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev,  REG_ETH_ISR, xval );
}

u32 MHal_EMAC_Read_ISR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_ISR );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt enable register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IER( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_IER );
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt enable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IER( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_IER, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt disable register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IDR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_IDR );
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt disable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IDR(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_IDR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt mask register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IMR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_IMR );
}

//-------------------------------------------------------------------------------------------------
//  Read PHY maintenance register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_MAN(struct net_device *ndev)
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_MAN );
}

//-------------------------------------------------------------------------------------------------
//  Write PHY maintenance register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_MAN(struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_MAN, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_BUFF( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_BUFF, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_BUFF( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_BUFF );
}

//-------------------------------------------------------------------------------------------------
//  Read Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RDPTR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_BUFFRDPTR );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RDPTR( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_BUFFRDPTR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_WRPTR( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_BUFFWRPTR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Frames transmitted OK
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_FRA( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_FRA );
}

//-------------------------------------------------------------------------------------------------
//  Single collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SCOL( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_SCOL );
}

//-------------------------------------------------------------------------------------------------
//  Multiple collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_MCOL( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_MCOL );
}

//-------------------------------------------------------------------------------------------------
//  Frames received OK
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_OK( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_OK );
}

//-------------------------------------------------------------------------------------------------
//  Frame check sequence errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SEQE( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_SEQE );
}

//-------------------------------------------------------------------------------------------------
//  Alignment errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ALE( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_ALE );
}

//-------------------------------------------------------------------------------------------------
//  Late collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_LCOL( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_LCOL );
}

//-------------------------------------------------------------------------------------------------
//  Excessive collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ECOL( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_ECOL );
}

//-------------------------------------------------------------------------------------------------
//  Transmit under-run errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_TUE( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_TUE );
}

//-------------------------------------------------------------------------------------------------
//  Carrier sense errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CSE( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_CSE );
}

//-------------------------------------------------------------------------------------------------
//  Receive resource error
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RE( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_RE );
}

//-------------------------------------------------------------------------------------------------
//  Received overrun
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ROVR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_ROVR );
}

//-------------------------------------------------------------------------------------------------
//  Received symbols error
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SE( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_SE );
}

//-------------------------------------------------------------------------------------------------
//  Excessive length errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ELR( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_ETH_ELR );
}

//-------------------------------------------------------------------------------------------------
//  Receive jabbers
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RJB( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_RJB );
}

//-------------------------------------------------------------------------------------------------
//  Undersize frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_USF( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_USF );
}

//-------------------------------------------------------------------------------------------------
//  SQE test errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SQEE( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_SQEE );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 100
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_JULIAN_0100( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32(ndev, REG_EMAC_JULIAN_0100 );
}


//-------------------------------------------------------------------------------------------------
//  Write Julian 100
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0100(struct net_device *ndev,  u32 xval )
{
    MHal_EMAC_WritReg32(ndev, REG_EMAC_JULIAN_0100, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 104
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_JULIAN_0104( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_EMAC_JULIAN_0104 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 104
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0104( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_EMAC_JULIAN_0104, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 108
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_JULIAN_0108( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_EMAC_JULIAN_0108 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 108
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0108( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_EMAC_JULIAN_0108, xval );
}


void MHal_EMAC_Set_Tx_JULIAN_T(struct net_device *ndev, u32 xval)
{
	u32 value;
	value = MHal_EMAC_ReadReg32(ndev, 0x134);
	value &= 0xff0fffff;
	value |= xval << 20;

	MHal_EMAC_WritReg32(ndev, 0x134, value);
}

u32 MHal_EMAC_Get_Tx_FIFO_Threshold(struct net_device *ndev)
{
	return (MHal_EMAC_ReadReg32(ndev, 0x134) & 0x00f00000) >> 20;
}

void MHal_EMAC_Set_Rx_FIFO_Enlarge(struct net_device *ndev, u32 xval)
{
	u32 value;
	value = MHal_EMAC_ReadReg32(ndev, 0x134);
	value &= 0xfcffffff;
	value |= xval << 24;

	MHal_EMAC_WritReg32(ndev, 0x134, value);
}

u32 MHal_EMAC_Get_Rx_FIFO_Enlarge(struct net_device *ndev)
{
	return (MHal_EMAC_ReadReg32(ndev, 0x134) & 0x03000000) >> 24;
}

void MHal_EMAC_Set_Miu_Priority(struct net_device *ndev, u32 xval)
{
	u32 value;

	value = MHal_EMAC_ReadReg32(ndev, 0x100);
	value &= 0xfff7ffff;
	value |= xval << 19;

	MHal_EMAC_WritReg32(ndev, 0x100, value);
}

u32 MHal_EMAC_Get_Miu_Priority(struct net_device *ndev)
{
	return (MHal_EMAC_ReadReg32(ndev, 0x100) & 0x00080000) >> 19;
}

void MHal_EMAC_Set_Tx_Hang_Fix_ECO(struct net_device *ndev, u32 xval)
{
	u32 value;
	value = MHal_EMAC_ReadReg32(ndev, 0x134);
	value &= 0xfffbffff;
	value |= xval << 18;

	MHal_EMAC_WritReg32(ndev, 0x134, value);
}

void MHal_EMAC_Set_MIU_Out_Of_Range_Fix(struct net_device *ndev, u32 xval)
{
	u32 value;
	value = MHal_EMAC_ReadReg32(ndev, 0x134);
	value &= 0xefffffff;
	value |= xval << 28;

	MHal_EMAC_WritReg32(ndev, 0x134, value);
}

void MHal_EMAC_Set_Rx_Tx_Burst16_Mode(struct net_device *ndev, u32 xval)
{
	u32 value;
	value = MHal_EMAC_ReadReg32(ndev, 0x134);
	value &= 0xdfffffff;
	value |= xval << 29;

	MHal_EMAC_WritReg32(ndev, 0x134, value);
}

void MHal_EMAC_Set_Tx_Rx_Req_Priority_Switch(struct net_device *ndev, u32 xval)
{
	u32 value;
	value = MHal_EMAC_ReadReg32(ndev, 0x134);
	value &= 0xfff7ffff;
	value |= xval << 19;

	MHal_EMAC_WritReg32(ndev, 0x134, value);
}

void MHal_EMAC_Set_Rx_Byte_Align_Offset(struct net_device *ndev, u32 xval)
{
	u32 value;
	value = MHal_EMAC_ReadReg32(ndev, 0x134);
	value &= 0xf3ffffff;
	value |= xval << 26;

	MHal_EMAC_WritReg32(ndev, 0x134, value);
}

void MHal_EMAC_Write_Protect(struct net_device *ndev, u32 start_addr, u32 length)
{
    u32 value;

    value = MHal_EMAC_ReadReg32(ndev, 0x11c);
	value &= 0x0000ffff;
	value |= ((start_addr+length) >> 3) << 16;
    MHal_EMAC_WritReg32(ndev, 0x11c, value);

    value = MHal_EMAC_ReadReg32(ndev, 0x120);
	value &= 0x00000000;
	value |= ((start_addr+length) >> 3) >> 16;
    value |= (start_addr >> 3) << 16;
    MHal_EMAC_WritReg32(ndev, 0x120, value);

    value = MHal_EMAC_ReadReg32(ndev, 0x124);
	value &= 0xffff0000;
    value |= (start_addr >> 3) >> 16;
    MHal_EMAC_WritReg32(ndev, 0x124, value);
}

void MHal_EMAC_HW_init(struct net_device *ndev)
{
    MHal_EMAC_Set_Tx_JULIAN_T(ndev, 0x4);
    MHal_EMAC_Set_Rx_FIFO_Enlarge(ndev, 0x2);

    MHal_EMAC_Set_Miu_Priority(ndev, 0x1);

    MHal_EMAC_Set_Tx_Hang_Fix_ECO(ndev, 1);
}

//-------------------------------------------------------------------------------------------------
//  PHY INTERFACE
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Enable the MDIO bit in MAC control register
// When not called from an interrupt-handler, access to the PHY must be
// protected by a spinlock.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_enable_mdi( struct net_device *ndev )
{
    u32 xval;
    xval = MHal_EMAC_Read_CTL(ndev);
    xval |= EMAC_MPE;
    MHal_EMAC_Write_CTL( ndev, xval );
}

//-------------------------------------------------------------------------------------------------
//  Disable the MDIO bit in the MAC control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_disable_mdi( struct net_device *ndev )
{
    u32 xval;
    xval = MHal_EMAC_Read_CTL(ndev);
    xval &= ~EMAC_MPE;
    MHal_EMAC_Write_CTL( ndev, xval );
}

//-------------------------------------------------------------------------------------------------
// Write value to the a PHY register
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_write_phy(struct net_device *ndev, unsigned char phy_addr, unsigned char address, u32 value )
{
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(ndev);
	u32 uRegVal;
    u32 uCTL;
	u32 uRegBase;
	
    if (LocPtr->phytype == PHY_INTERNAL)
    {
	    uRegBase = LocPtr->reg_phy_address; 

        *(volatile unsigned int *)(uRegBase + address*4) = value;
        udelay( 1 );
    }
    else
    {
        uRegVal = 0; 
	    uCTL = 0;
        uRegVal =  ( EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_W) | (( phy_addr & 0x1F ) << PHY_ADDR_OFFSET )
                    | ( address << PHY_REGADDR_OFFSET ) | (value & 0xFFFF);

        uCTL = MHal_EMAC_Read_CTL(ndev);
        MHal_EMAC_enable_mdi(ndev);

        MHal_EMAC_Write_MAN( ndev, uRegVal );
        // Wait until IDLE bit in Network Status register is cleared //
        uRegVal = MHal_EMAC_ReadReg32( ndev, REG_ETH_SR );  //Must read Low 16 bit.
        while ( !( uRegVal & EMAC_IDLE ) )
        {
            uRegVal = MHal_EMAC_ReadReg32( ndev, REG_ETH_SR );
            barrier();
        }
        MHal_EMAC_Write_CTL(ndev,uCTL);
     }
}

//-------------------------------------------------------------------------------------------------
// Read value stored in a PHY register.
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_read_phy( struct net_device *ndev, unsigned char phy_addr, unsigned char address, u32* value )
{ 
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(ndev);
    u32 uRegBase;  
    u32 tempvalue;
    u32 uRegVal;
    u32 uCTL;

    if (LocPtr->phytype == PHY_INTERNAL)
    {
	    uRegBase = LocPtr->reg_phy_address;
        phy_addr =0;

        tempvalue = *(volatile unsigned int *)(uRegBase + 0x04);
        tempvalue |= 0x0004;
        *(volatile unsigned int *)(uRegBase + 0x04) = tempvalue;
        udelay( 1 );
        *value = *(volatile unsigned int *)(uRegBase + address*4);
    }
    else
    {  
        uRegVal = 0;
        uCTL = 0;

        uRegVal = (EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_R)
                | ((phy_addr & 0x1f) << PHY_ADDR_OFFSET) | (address << PHY_REGADDR_OFFSET) | (0) ;

        uCTL = MHal_EMAC_Read_CTL(ndev);
        MHal_EMAC_enable_mdi(ndev);
        MHal_EMAC_Write_MAN(ndev, uRegVal);

        //Wait until IDLE bit in Network Status register is cleared //
        uRegVal = MHal_EMAC_ReadReg32( ndev, REG_ETH_SR );  //Must read Low 16 bit.
        while ( !( uRegVal & EMAC_IDLE ) )
        {
            uRegVal = MHal_EMAC_ReadReg32( ndev, REG_ETH_SR );
            barrier();
        }
        *value = ( MHal_EMAC_Read_MAN( ndev ) & 0x0000ffff );
        MHal_EMAC_Write_CTL( ndev, uCTL );
    }
}

//-------------------------------------------------------------------------------------------------
// Update MAC speed and H/F duplex
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_update_speed_duplex(struct net_device *ndev, u32 uspeed, u32 uduplex )
{
    u32 xval;

    xval = MHal_EMAC_ReadReg32( ndev, REG_ETH_CFG ) & ~( EMAC_SPD | EMAC_FD );

    if ( uspeed == SPEED_100 )
    {
        if ( uduplex == DUPLEX_FULL )    // 100 Full Duplex //
        {
            xval = xval | EMAC_SPD | EMAC_FD;
        }
        else                           // 100 Half Duplex ///
        {
            xval = xval | EMAC_SPD;
        }
    }
    else
    {
        if ( uduplex == DUPLEX_FULL )    //10 Full Duplex //
        {
            xval = xval | EMAC_FD;
        }
        else                           // 10 Half Duplex //
        {
        }
    }
    MHal_EMAC_WritReg32( ndev, REG_ETH_CFG, xval );
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_get_SA1H_addr( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_SA1H );
}

u32 MHal_EMAC_get_SA1L_addr( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_SA1L );
}

u32 MHal_EMAC_get_SA2H_addr( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev, REG_ETH_SA2H );
}

u32 MHal_EMAC_get_SA2L_addr( struct net_device *ndev )
{
    return MHal_EMAC_ReadReg32( ndev,  REG_ETH_SA2L );
}

void MHal_EMAC_Write_SA1H( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_SA1H, xval );
}

void MHal_EMAC_Write_SA1L( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_SA1L, xval );
}

void MHal_EMAC_Write_SA2H( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_SA2H, xval );
}

void MHal_EMAC_Write_SA2L( struct net_device *ndev, u32 xval )
{
    MHal_EMAC_WritReg32( ndev, REG_ETH_SA2L, xval );
}

void* MDev_memset( void* s, u32 c, unsigned long count )
{
    char* xs = ( char* ) s;

    while ( count-- )
        *xs++ = c;

    return s;
}

//-------------------------------------------------------------------------------------------------
// Check INT Done
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_CheckINTDone( struct net_device *ndev )
{
    u32 retIntStatus;
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(ndev);

    retIntStatus = MHal_EMAC_Read_ISR(ndev);
    LocPtr->CurUVE.cntChkINTCounter = ( LocPtr->CurUVE.cntChkINTCounter % MHal_MAX_INT_COUNTER );
    LocPtr->CurUVE.cntChkINTCounter ++;

    if ( ( retIntStatus & EMAC_INT_DONE ) ||
         ( LocPtr->CurUVE.cntChkINTCounter == ( MHal_MAX_INT_COUNTER - 1 ) ) )
    {
        LocPtr->CurUVE.flagISR_INT_DONE = 0x01;
        return TRUE;
    }

	return FALSE;
}

//-------------------------------------------------------------------------------------------------
// EMAC Negotiation PHY
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_NegotiationPHY( struct net_device *ndev, int phyaddr)
{
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(ndev);
    // Set PHY --------------------------------------------------------------
    u32 retValue = 0;
    u32 bmsr;

    // IMPORTANT: Get real duplex by negotiation with peer.
    u32 word_ETH_CTL = MHal_EMAC_Read_CTL(ndev);
    MHal_EMAC_Write_CTL(ndev, 0x0000001C | word_ETH_CTL );

    LocPtr->CurBCE.duplex = 1;   // Set default as Half-duplex if negotiation fails.
    retValue = 1;

    LocPtr->CurUVE.flagISR_INT_DONE = 0x00;
    LocPtr->CurUVE.cntChkINTCounter = 0;
    LocPtr->CurUVE.cntChkCableConnect = 0;


    MHal_EMAC_read_phy(ndev, phyaddr, MII_BMSR, &bmsr);
    if ( (bmsr & BMSR_100FULL) || (bmsr & BMSR_10FULL) )
    {
        LocPtr->CurBCE.duplex = 2;
        retValue = 2;
    }
    else
    {
        LocPtr->CurBCE.duplex = 1;
        retValue = 1;
    }

    // NOTE: REG_ETH_CFG must be set according to new ThisBCE.duplex.

    MHal_EMAC_Write_CTL(ndev, word_ETH_CTL );
    // Set PHY --------------------------------------------------------------
    return(retValue);

}

//-------------------------------------------------------------------------------------------------
// EMAC Hardware register set
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// EMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_timer_callback( struct net_device *ndev, unsigned long value )
{
    u32 uRegVal;
    uRegVal = MHal_EMAC_Read_IER(ndev);
    uRegVal |= ( EMAC_INT_RCOM );
    MHal_EMAC_Write_IER( ndev, uRegVal );
}

//-------------------------------------------------------------------------------------------------
// EMAC clock on/off
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Power_On_Clk( struct net_device *dev )
{
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(dev);
	struct platform_device	*pdev = LocPtr->pdev;

    u16 trimvalue = *((volatile u16*)0xB0011F28); // PHY ADC trim value, used to fill in EPHY register
    u16 package_is_qfp = *((volatile u16*)0xBF203D80) & (0x1 << 8);
    *( ( u32 * ) ( ( char * ) 0xBF206600 +  0x32*4) ) = 0x0400;
    *( ( u32 * ) ( ( char * ) 0xBF206600 +  0x33*4) ) = 0x0004;
    *( ( u32 * ) ( ( char * ) 0xBF206600 +  0x34*4) ) = 0x0000;

    //emac_clk gen
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x60*4) ) = 0x0000;
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x61*4) ) = 0x0000;
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x62*4) ) = 0x0000;

    //chiptop [15] allpad_in
    *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x50*4 ) ) = 0; // All pad in should be 0, due to here are several output pin

    //chiptop [10:8] et_mode
    // et_mode used to select PM pad and NON_PM pad, should be 0 when BGA pkg, should be 1 when QFP package
    if (package_is_qfp)
    {
        // et_mode
        *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x7*4 ) ) &= (~0x0700);
        *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x7*4 ) ) |= 0x0100;

        // SDIO mode
        *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x6*4 ) ) &= ~0x0070;
    }

	if (pdev->id == 0)
    {

    // Albany related register
    #define REG_8(reg) (*(volatile u8*)( 0xBF000000 + reg * 2 - reg % 2))

    REG_8(0x32b4) = 0x56;//wriu    0x0032b4    0x56    // gain shift
    REG_8(0x324f) = 0x02;//wriu    0x00324f    0x02    // det max
    REG_8(0x3251) = 0x01;//wriu    0x003251    0x01    // det min
    REG_8(0x3277) = 0x18;//wriu    0x003277    0x18    // snr len
    REG_8(0x3243) = 0x15;//wriu    0x003243    0x15    // snr k value change back

    REG_8(0x32FC) = 0x00;//wriu    0x0032fc    0x00    // Power-on LDO
    REG_8(0x32B7) = 0x07;//wriu    0x0032b7    0x07    // Power-on ADC
    REG_8(0x32CB) = 0x11;//wriu    0x0032cb    0x11    // Power-on BGAP
    REG_8(0x32CC) = 0x80;//wriu    0x0032cc    0x80    // Power-on ADCPL
    REG_8(0x32CD) = 0xD2;//wriu    0x0032cd    0xd1    // Power-on ADCPL (is d2 for 55lp, not d1)
    REG_8(0x32D4) = 0x00;//wriu    0x0032d4    0x00    // Power-on LPF_OP
    REG_8(0x32B9) = 0x40;//wriu    0x0032b9    0x40    // Power-on LPF
    REG_8(0x32BB) = 0x05;//wriu    0x0032bb    0x05    // Power-on REF
    REG_8(0x32B0) = 0x02;//wriu -w 0x0032b0    0x6002  // Power-on TX
    REG_8(0x32B1) = 0x60;

    printk(KERN_INFO "[ALBANY] TRIM VALUE = %08x\n", trimvalue);
    printk(KERN_INFO "[ALBANY] REG VALUE = %04x %04x %04x\n",REG_8(0x3168), REG_8(0x3169), REG_8(0x3160));

    REG_8(0x3168) = trimvalue & 0xFF;
    REG_8(0x3169) = (trimvalue >> 8) & 0xFF;

    REG_8(0x3160) |= 1 << 2; // efuse key 1
    REG_8(0x3169) |= 1 << 7; // efuse key 2

    printk(KERN_INFO "[ALBANY] REG VALUE = %04x %04x %04x\n",REG_8(0x3168), REG_8(0x3169), REG_8(0x3160));

    // DAC settings for buggy link partner
    // ltp
    REG_8(0x00311e)=0x00; //wriu 0x00311e 0x00
    REG_8(0x00311f)=0x98; //wriu 0x00311f 0x98
    REG_8(0x003120)=0x84; //wriu 0x003120 0x84
    REG_8(0x003121)=0x84; //wriu 0x003121 0x84
    REG_8(0x003122)=0x88; //wriu 0x003122 0x88
    REG_8(0x003123)=0x88; //wriu 0x003123 0x88
    REG_8(0x003124)=0x8c; //wriu 0x003124 0x8c
    REG_8(0x003125)=0x8c; //wriu 0x003125 0x8c
    REG_8(0x003126)=0x98; //wriu 0x003126 0x98
    REG_8(0x003127)=0xc8; //wriu 0x003127 0xc8
    REG_8(0x003128)=0x00; //wriu 0x003128 0x00
    REG_8(0x003129)=0x00; //wriu 0x003129 0x00
    REG_8(0x00312a)=0x00; //wriu 0x00312a 0x00
    REG_8(0x00312b)=0x10; //wriu 0x00312b 0x10
    REG_8(0x00312c)=0x00; //wriu 0x00312c 0x00
    // tin
    REG_8(0x003188)=0x3c; //wriu 0x003188 0x3c
    REG_8(0x003189)=0x3c; //wriu 0x003189 0x3c
    REG_8(0x00318a)=0x3c; //wriu 0x00318a 0x3c
    REG_8(0x00318b)=0x3c; //wriu 0x00318b 0x3c
    REG_8(0x00318c)=0x30; //wriu 0x00318c 0x30
    REG_8(0x00318d)=0x3c; //wriu 0x00318d 0x3c
    REG_8(0x00318e)=0x7c; //wriu 0x00318e 0x7c
    REG_8(0x00318f)=0x68; //wriu 0x00318f 0x68
    REG_8(0x003190)=0x34; //wriu 0x003190 0x34
    REG_8(0x003191)=0x78; //wriu 0x003191 0x78
    REG_8(0x003192)=0x94; //wriu 0x003192 0x94
    REG_8(0x003193)=0xd4; //wriu 0x003193 0xd4
    REG_8(0x003194)=0xa0; //wriu 0x003194 0xa0
    REG_8(0x003195)=0x84; //wriu 0x003195 0x84
    REG_8(0x003196)=0xc8; //wriu 0x003196 0xc8
    REG_8(0x003197)=0xe4; //wriu 0x003197 0xe4
    REG_8(0x003198)=0xc8; //wriu 0x003198 0xc8
    REG_8(0x003199)=0xc8; //wriu 0x003199 0xc8
    REG_8(0x00319a)=0xe8; //wriu 0x00319a 0xe8
    REG_8(0x00319b)=0xc8; //wriu 0x00319b 0xc8
    REG_8(0x00319c)=0x3c; //wriu 0x00319c 0x3c
    REG_8(0x00319d)=0x3c; //wriu 0x00319d 0x3c
    REG_8(0x00319e)=0x3c; //wriu 0x00319e 0x3c
    REG_8(0x00319f)=0x3c; //wriu 0x00319f 0x3c
    REG_8(0x0031a0)=0x30; //wriu 0x0031a0 0x30
    REG_8(0x0031a1)=0x24; //wriu 0x0031a1 0x24
    REG_8(0x0031a2)=0x7c; //wriu 0x0031a2 0x7c
    REG_8(0x0031a3)=0x70; //wriu 0x0031a3 0x70
    REG_8(0x0031a4)=0x20; //wriu 0x0031a4 0x20
    REG_8(0x0031a5)=0x64; //wriu 0x0031a5 0x64
    REG_8(0x0031a6)=0xa0; //wriu 0x0031a6 0xa0
    REG_8(0x0031a7)=0xd4; //wriu 0x0031a7 0xd4
    REG_8(0x0031a8)=0x98; //wriu 0x0031a8 0x98
    REG_8(0x0031a9)=0x84; //wriu 0x0031a9 0x84
    REG_8(0x0031aa)=0xc8; //wriu 0x0031aa 0xc8
    REG_8(0x0031ab)=0xd0; //wriu 0x0031ab 0xd0
    REG_8(0x0031ac)=0xc8; //wriu 0x0031ac 0xc8
    REG_8(0x0031ad)=0xc8; //wriu 0x0031ad 0xc8
    REG_8(0x0031ae)=0xc8; //wriu 0x0031ae 0xc8
    REG_8(0x0031af)=0xc8; //wriu 0x0031af 0xc8
    
    }
    else if (pdev->id == 1)
    {
        //chiptop [5] PAD_TS1_SYNC
        *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x4*4 ) ) &= (~0x020); //wriu 0x101e08 0x0020        
    }


}

void MHal_EMAC_Power_Off_Clk( struct net_device *ndev )
{
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(ndev);
	
    //emac_clk gen
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x60*4) ) = 0x0101;
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x61*4) ) = 0x0101;
    if (LocPtr->pdev->id == 0)
    {
        REG_8(0x32bb) = 0xc5;
        REG_8(0x32cb) = 0x13;
        REG_8(0x32d4) = 0x20;
        REG_8(0x32b7) = 0x17;
        REG_8(0x0032b0) = 0x03;
        REG_8(0x0032b1) = 0x70;
        REG_8(0x0032fc) = 0x03;
        REG_8(0x0032cc) = 0x90;
    }
}
