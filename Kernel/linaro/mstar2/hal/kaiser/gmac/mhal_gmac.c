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
/// @file   Mhal_gmac.c
/// @brief  GMAC Driver
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
#include "mhal_gmac.h"

//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
struct _MHalBasicConfigGMAC
{
    u8 connected;          // 0:No, 1:Yes
    u8 speed;               // 10:10Mbps, 100:100Mbps
    // ETH_CTL Register:
    u8 wes;             // 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    // ETH_CFG Register:
    u8 duplex;              // 1:Half-duplex, 2:Full-duplex
    u8 cam;                // 0:No CAM, 1:Yes
    u8 rcv_bcast;       // 0:No, 1:Yes
    u8 rlf;                // 0:No, 1:Yes receive long frame(1522)
    // MAC Address:
    u8 sa1[6];              // Specific Addr 1 (MAC Address)
    u8 sa2[6];              // Specific Addr 2
    u8 sa3[6];              // Specific Addr 3
    u8 sa4[6];              // Specific Addr 4
};
typedef struct _MHalBasicConfigGMAC MHalBasicConfigGMAC;

struct _MHalUtilityVarsGMAC
{
    u32 cntChkCableConnect;
    u32 cntChkINTCounter;
    u32 readIdxRBQP;        // Reset = 0x00000000
    u32 rxOneFrameAddr;     // Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")
    u8 flagISR_INT_DONE;
};
typedef struct _MHalUtilityVarsGMAC MHalUtilityVarsGMAC;

MHalBasicConfigGMAC MHalGMACThisBCE;
MHalUtilityVarsGMAC MHalGMACThisUVE;

#define MHal_MAX_INT_COUNTER    100
//-------------------------------------------------------------------------------------------------
//  GMAC hardware for Titania
//-------------------------------------------------------------------------------------------------

/*8-bit RIU address*/
u8 MHal_GMAC_ReadReg8( u32 bank, u32 reg )
{
    u8 val;
    u32 address = GMAC_RIU_REG_BASE + bank*0x100*2;
    address = address + (reg << 1) - (reg & 1);

    val = *( ( volatile u8* ) address );
    return val;
}

void MHal_GMAC_WritReg8( u32 bank, u32 reg, u8 val )
{
    u32 address = GMAC_RIU_REG_BASE + bank*0x100*2;
    address = address + (reg << 1) - (reg & 1);

    *( ( volatile u8* ) address ) = val;
}

u32 MHal_GMAC_ReadReg32( u32 xoffset )
{
    u32 address = GMAC_REG_ADDR_BASE + xoffset*2;

    u32 xoffsetValueL = *( ( volatile u32* ) address ) & 0x0000FFFF;
    u32 xoffsetValueH = *( ( volatile u32* ) ( address + 4) ) << 0x10;
    return( xoffsetValueH | xoffsetValueL );
}

void MHal_GMAC_WritReg32( u32 xoffset, u32 xval )
{
    u32 address = GMAC_REG_ADDR_BASE + xoffset*2;
    *( ( volatile u32 * ) address ) = ( u32 ) ( xval & 0x0000FFFF );
    *( ( volatile u32 * ) ( address + 4 ) ) = ( u32 ) ( xval >> 0x10 );
}

u32 MHal_GMAC_ReadRam32( u32 uRamAddr, u32 xoffset)
{
    return (*( u32 * ) ( ( char * ) uRamAddr + xoffset ) );
}

void MHal_GMAC_WritRam32( u32 uRamAddr, u32 xoffset, u32 xval )
{
    *( ( u32 * ) ( ( char * ) uRamAddr + xoffset ) ) = xval;
}

void MHal_GMAC_Write_SA1_MAC_Address( u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA1L, w0 );
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA1H, w1 );
}

void MHal_GMAC_Write_SA2_MAC_Address( u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA2L, w0 );
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA2H, w1 );
}

void MHal_GMAC_Write_SA3_MAC_Address( u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA3L, w0 );
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA3H, w1 );
}

void MHal_GMAC_Write_SA4_MAC_Address( u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u32 w0 = ( u32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    u32 w1 = ( u32 ) m5 << 8 | m4;
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA4L, w0 );
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA4H, w1 );
}

//-------------------------------------------------------------------------------------------------
//  R/W GMAC register for Titania
//-------------------------------------------------------------------------------------------------

void MHal_GMAC_update_HSH(u32 mc0, u32 mc1)
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_HSL, mc0 );
    MHal_GMAC_WritReg32( GMAC_REG_ETH_HSH, mc1 );
}

//-------------------------------------------------------------------------------------------------
//  Read control register
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_CTL( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_CTL );
}

//-------------------------------------------------------------------------------------------------
//  Write Network control register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_CTL( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_CTL, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Network configuration register
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_CFG( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_CFG );
}

//-------------------------------------------------------------------------------------------------
//  Write Network configuration register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_CFG( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_CFG, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_RBQP( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_RBQP );
}

//-------------------------------------------------------------------------------------------------
//  Write RBQP
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_RBQP( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_RBQP, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Address register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_TAR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_TAR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_TCR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_TCR);
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Control register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_TCR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_TCR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Transmit Status Register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_TSR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_TSR, xval );
}

u32 MHal_GMAC_Read_TSR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_TSR );
}

void MHal_GMAC_Write_RSR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_RSR, xval );
}

u32 MHal_GMAC_Read_RSR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_RSR );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt status register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_ISR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_ISR, xval );
}

u32 MHal_GMAC_Read_ISR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_ISR );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt enable register
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_IER( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_IER );
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt enable register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_IER( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_IER, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt disable register
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_IDR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_IDR );
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt disable register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_IDR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_IDR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt mask register
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_IMR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_IMR );
}

//-------------------------------------------------------------------------------------------------
//  Read PHY maintenance register
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_MAN( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_MAN );
}

//-------------------------------------------------------------------------------------------------
//  Write PHY maintenance register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_MAN( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_MAN, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_BUFF( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_BUFF, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_BUFF( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_BUFF );
}

//-------------------------------------------------------------------------------------------------
//  Read Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_RDPTR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_BUFFRDPTR );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_RDPTR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_BUFFRDPTR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_WRPTR( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_BUFFWRPTR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Frames transmitted OK
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_FRA( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_FRA );
}

//-------------------------------------------------------------------------------------------------
//  Single collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_SCOL( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SCOL );
}

//-------------------------------------------------------------------------------------------------
//  Multiple collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_MCOL( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_MCOL );
}

//-------------------------------------------------------------------------------------------------
//  Frames received OK
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_OK( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_OK );
}

//-------------------------------------------------------------------------------------------------
//  Frame check sequence errors
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_SEQE( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SEQE );
}

//-------------------------------------------------------------------------------------------------
//  Alignment errors
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_ALE( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_ALE );
}

//-------------------------------------------------------------------------------------------------
//  Late collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_LCOL( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_LCOL );
}

//-------------------------------------------------------------------------------------------------
//  Excessive collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_ECOL( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_ECOL );
}

//-------------------------------------------------------------------------------------------------
//  Transmit under-run errors
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_TUE( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_TUE );
}

//-------------------------------------------------------------------------------------------------
//  Carrier sense errors
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_CSE( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_CSE );
}

//-------------------------------------------------------------------------------------------------
//  Receive resource error
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_RE( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_RE );
}

//-------------------------------------------------------------------------------------------------
//  Received overrun
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_ROVR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_ROVR );
}

//-------------------------------------------------------------------------------------------------
//  Received symbols error
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_SE( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SE );
}

//-------------------------------------------------------------------------------------------------
//  Excessive length errors
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_ELR( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_ELR );
}

//-------------------------------------------------------------------------------------------------
//  Receive jabbers
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_RJB( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_RJB );
}

//-------------------------------------------------------------------------------------------------
//  Undersize frames
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_USF( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_USF );
}

//-------------------------------------------------------------------------------------------------
//  SQE test errors
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_SQEE( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SQEE );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 100
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_JULIAN_0100( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_JULIAN_0100 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 100
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_JULIAN_0100( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_JULIAN_0100, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 104
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_JULIAN_0104( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_JULIAN_0104 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 104
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_JULIAN_0104( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_JULIAN_0104, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 108
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_JULIAN_0108( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_JULIAN_0108 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 108
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_JULIAN_0108( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_JULIAN_0108, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 414
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_JULIAN_0414( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_JULIAN_0414 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 414
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_JULIAN_0414( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_JULIAN_0414, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 418
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Write_JULIAN_0418( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_JULIAN_0418, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 418
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_Read_JULIAN_0418( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_JULIAN_0418 );
}

void MHal_GMAC_Set_Tx_JULIAN_T(u32 xval)
{
	u32 value;
	value = MHal_GMAC_ReadReg32(0x134);
	value &= 0xff0fffff;
	value |= xval << 20;

	MHal_GMAC_WritReg32(0x134, value);
}

void MHal_GMAC_Set_TEST(u32 xval)
{
	u32 value = 0xffffffff;
	int i=0;

	for(i = 0x100; i< 0x160;i+=4){
		MHal_GMAC_WritReg32(i, value);
		}

}

u32 MHal_GMAC_Get_Tx_FIFO_Threshold(void)
{
	return (MHal_GMAC_ReadReg32(0x134) & 0x00f00000) >> 20;
}

void MHal_GMAC_Set_Rx_FIFO_Enlarge(u32 xval)
{
	u32 value;
	value = MHal_GMAC_ReadReg32(0x134);
	value &= 0xfcffffff;
	value |= xval << 24;

	MHal_GMAC_WritReg32(0x134, value);
}

u32 MHal_GMAC_Get_Rx_FIFO_Enlarge(void)
{
	return (MHal_GMAC_ReadReg32(0x134) & 0x03000000) >> 24;
}

void MHal_GMAC_Set_Miu_Priority(u32 xval)
{
	u32 value;

	value = MHal_GMAC_ReadReg32(0x100);
	value &= 0xfff7ffff;
	value |= xval << 19;

	MHal_GMAC_WritReg32(0x100, value);
}

u32 MHal_GMAC_Get_Miu_Priority(void)
{
	return (MHal_GMAC_ReadReg32(0x100) & 0x00080000) >> 19;
}

void MHal_GMAC_Set_Tx_Hang_Fix_ECO(u32 xval)
{
	u32 value;
	value = MHal_GMAC_ReadReg32(0x134);
	value &= 0xfffbffff;
	value |= xval << 18;

	MHal_GMAC_WritReg32(0x134, value);
}

void MHal_GMAC_Set_MIU_Out_Of_Range_Fix(u32 xval)
{
	u32 value;
	value = MHal_GMAC_ReadReg32(0x134);
	value &= 0xefffffff;
	value |= xval << 28;

	MHal_GMAC_WritReg32(0x134, value);
}

void MHal_GMAC_Set_Rx_Tx_Burst16_Mode(u32 xval)
{
	u32 value;
	value = MHal_GMAC_ReadReg32(0x134);
	value &= 0xdfffffff;
	value |= xval << 29;

	MHal_GMAC_WritReg32(0x134, value);
}

void MHal_GMAC_Set_Tx_Rx_Req_Priority_Switch(u32 xval)
{
	u32 value;
	value = MHal_GMAC_ReadReg32(0x134);
	value &= 0xfff7ffff;
	value |= xval << 19;

	MHal_GMAC_WritReg32(0x134, value);
}

void MHal_GMAC_Set_Rx_Byte_Align_Offset(u32 xval)
{
	u32 value;
	value = MHal_GMAC_ReadReg32(0x134);
	value &= 0xf3ffffff;
	value |= xval << 26;

	MHal_GMAC_WritReg32(0x134, value);
}

void MHal_GMAC_Write_Protect(u32 start_addr, u32 length)
{
    u32 value;

    value = MHal_GMAC_ReadReg32(0x11c);
	value &= 0x0000ffff;
	value |= ((start_addr+length) >> 4) << 16;
    MHal_GMAC_WritReg32(0x11c, value);

    value = MHal_GMAC_ReadReg32(0x120);
	value &= 0x00000000;
	value |= ((start_addr+length) >> 4) >> 16;
    value |= (start_addr >> 4) << 16;
    MHal_GMAC_WritReg32(0x120, value);

    value = MHal_GMAC_ReadReg32(0x124);
	value &= 0xffff0000;
    value |= (start_addr >> 4) >> 16;
    MHal_GMAC_WritReg32(0x124, value);

    value = MHal_GMAC_ReadReg32(0x100);
	value |= 0x00000040;
    MHal_GMAC_WritReg32(0x100, value);
}

void MHal_GMAC_Set_Miu_Highway(u32 xval)
{
    u32 value;
    value = MHal_GMAC_ReadReg32(0x134);
    value &= 0xbfffffff;
    value |= xval << 30;

    MHal_GMAC_WritReg32(0x134, value);
}

void MHal_GMAC_HW_init(void)
{
    MHal_GMAC_Set_Miu_Priority(1);
    MHal_GMAC_Set_Tx_JULIAN_T(4);
    MHal_GMAC_Set_Rx_Tx_Burst16_Mode(1);
    MHal_GMAC_Set_Rx_FIFO_Enlarge(2);
    MHal_GMAC_Set_Tx_Hang_Fix_ECO(1);
    MHal_GMAC_Set_MIU_Out_Of_Range_Fix(1);
    #ifdef GMAC_RX_BYTE_ALIGN_OFFSET
    MHal_GMAC_Set_Rx_Byte_Align_Offset(2);
    #endif
    MHal_GMAC_WritReg32(GMAC_REG_JULIAN_0138, MHal_GMAC_ReadReg32(GMAC_REG_JULIAN_0138) | 0x00000001);
//    MHal_GMAC_Set_Miu_Highway(1);
}

//-------------------------------------------------------------------------------------------------
//  PHY INTERFACE
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Enable the MDIO bit in MAC control register
// When not called from an interrupt-handler, access to the PHY must be
// protected by a spinlock.
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_enable_mdi( void )
{
    u32 xval;
    xval = MHal_GMAC_Read_CTL();
    xval |= GMAC_MPE;
    MHal_GMAC_Write_CTL( xval );
}

//-------------------------------------------------------------------------------------------------
//  Disable the MDIO bit in the MAC control register
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_disable_mdi( void )
{
    u32 xval;
    xval = MHal_GMAC_Read_CTL();
    xval &= ~GMAC_MPE;
    MHal_GMAC_Write_CTL( xval );
}

//-------------------------------------------------------------------------------------------------
// Write value to the a PHY register
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_write_phy( unsigned char phy_addr, unsigned char address, u32 value )
{
#ifdef CONFIG_GMAC_ETHERNET_ALBANY
    u32 uRegBase = GMAC_INTERNEL_PHY_REG_BASE;

    if(phy_addr >= 32) return; // invalid phy address
    phy_addr =0;

    *(volatile unsigned int *)(uRegBase + address*4) = value; 
    udelay( 1 );
#else
    u32 uRegVal = 0, uCTL = 0;
    if(phy_addr >= 32) return; // invalid phy address
    uRegVal =  ( GMAC_HIGH | GMAC_CODE_802_3 | GMAC_RW_W) | (( phy_addr & 0x1F ) << GMAC_PHY_ADDR_OFFSET )
                | ( address << GMAC_PHY_REGADDR_OFFSET ) | (value & 0xFFFF);

    uCTL = MHal_GMAC_Read_CTL();
    MHal_GMAC_enable_mdi();

    MHal_GMAC_Write_MAN( uRegVal );
    // Wait until IDLE bit in Network Status register is cleared //
    uRegVal = MHal_GMAC_ReadReg32( GMAC_REG_ETH_SR );  //Must read Low 16 bit.
    while ( !( uRegVal & GMAC_IDLE ) )
    {
        uRegVal = MHal_GMAC_ReadReg32( GMAC_REG_ETH_SR );
        barrier();
    }
    MHal_GMAC_Write_CTL(uCTL);
#endif
}
//-------------------------------------------------------------------------------------------------
// Read value stored in a PHY register.
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_read_phy( unsigned char phy_addr, unsigned char address, u32* value )
{
#ifdef CONFIG_GMAC_ETHERNET_ALBANY
    u32 uRegBase  = GMAC_INTERNEL_PHY_REG_BASE;
    u32 tempvalue ;

    if(phy_addr >= 32) return; // invalid phy address
    phy_addr =0;

    tempvalue = *(volatile unsigned int *)(GMAC_INTERNEL_PHY_REG_BASE + 0x04);
    tempvalue |= 0x0004;
    *(volatile unsigned int *)(GMAC_INTERNEL_PHY_REG_BASE + 0x04) = tempvalue;
    udelay( 1 );
    *value = *(volatile unsigned int *)(uRegBase + address*4); 
#else
    u32 uRegVal = 0, uCTL = 0;

    if(phy_addr >= 32) return; // invalid phy address
    uRegVal = (GMAC_HIGH | GMAC_CODE_802_3 | GMAC_RW_R)
            | ((phy_addr & 0x1f) << GMAC_PHY_ADDR_OFFSET) | (address << GMAC_PHY_REGADDR_OFFSET) | (0) ;

    uCTL = MHal_GMAC_Read_CTL();
    MHal_GMAC_enable_mdi();
    MHal_GMAC_Write_MAN(uRegVal);

    //Wait until IDLE bit in Network Status register is cleared //
    uRegVal = MHal_GMAC_ReadReg32( GMAC_REG_ETH_SR );  //Must read Low 16 bit.
    while ( !( uRegVal & GMAC_IDLE ) )
    {
        uRegVal = MHal_GMAC_ReadReg32( GMAC_REG_ETH_SR );
        barrier();
    }
    *value = ( MHal_GMAC_Read_MAN() & 0x0000ffff );
    MHal_GMAC_Write_CTL(uCTL);
#endif
}

//-------------------------------------------------------------------------------------------------
// Update MAC speed and H/F duplex
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_update_speed_duplex( u32 uspeed, u32 uduplex )
{
    u32 xval;
    u8 uRegVal;

#ifdef CONFIG_MSTAR_GMAC_RMII_DOWNGRADE_MODE
    return;
#endif

    xval = MHal_GMAC_ReadReg32( GMAC_REG_ETH_CFG ) & ~( GMAC_SPD | GMAC_FD );

    if(uspeed == SPEED_1000)
    {
        /* Disable reg_rgmii_slow */
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd6);
        uRegVal &= 0x7f;
        MHal_GMAC_WritReg8(0x000e, 0xd6, uRegVal);

        /* Set reg_xmii_type as 1G */
        uRegVal = MHal_GMAC_ReadReg8(0x121f, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x00;
        MHal_GMAC_WritReg8(0x121f, 0x60, uRegVal);

        if ( uduplex == DUPLEX_FULL )    // 100 Full Duplex //
        {
            xval = xval | GMAC_FD;
        }
    }
    else if ( uspeed == SPEED_100 )
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd6);
        uRegVal |= 0x80;
        MHal_GMAC_WritReg8(0x000e, 0xd6, uRegVal);

        /* Set reg_rgmii10_100 as 100M*/
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd7);
        uRegVal |= 0x01;
        MHal_GMAC_WritReg8(0x000e, 0xd7, uRegVal);

        /* Set reg_xmii_type as 10M/100M */
        uRegVal = MHal_GMAC_ReadReg8(0x121f, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x01;
        MHal_GMAC_WritReg8(0x121f, 0x60, uRegVal);

        if ( uduplex == DUPLEX_FULL )    // 100 Full Duplex //
        {
            xval = xval | GMAC_SPD | GMAC_FD;
        }
        else                           // 100 Half Duplex ///
        {
            xval = xval | GMAC_SPD;
        }
    }
    else
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd6);
        uRegVal |= 0x80;
        MHal_GMAC_WritReg8(0x000e, 0xd6, uRegVal);

        /* Enable reg_rgmii10_100 as 10M*/
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd7);
        uRegVal &= 0xfe;
        MHal_GMAC_WritReg8(0x000e, 0xd7, uRegVal);

        /* Set reg_xmii_type as 10M/100M */
        uRegVal = MHal_GMAC_ReadReg8(0x121f, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x01;
        MHal_GMAC_WritReg8(0x121f, 0x60, uRegVal);

        if ( uduplex == DUPLEX_FULL )    //10 Full Duplex //
        {
            xval = xval | GMAC_FD;
        }
        else                           // 10 Half Duplex //
        {
        }
    }
    MHal_GMAC_WritReg32( GMAC_REG_ETH_CFG, xval );
}

u8 MHal_GMAC_CalcMACHash( u8 m0, u8 m1, u8 m2, u8 m3, u8 m4, u8 m5 )
{
    u8 hashIdx0 = ( m0&0x01 ) ^ ( ( m0&0x40 ) >> 6 ) ^ ( ( m1&0x10 ) >> 4 ) ^ ( ( m2&0x04 ) >> 2 )
                 ^ ( m3&0x01 ) ^ ( ( m3&0x40 ) >> 6 ) ^ ( ( m4&0x10 ) >> 4 ) ^ ( ( m5&0x04 ) >> 2 );

     u8 hashIdx1 = ( m0&0x02 ) ^ ( ( m0&0x80 ) >> 6 ) ^ ( ( m1&0x20 ) >> 4 ) ^ ( ( m2&0x08 ) >> 2 )
                ^ ( m3&0x02 ) ^ ( ( m3&0x80 ) >> 6 ) ^ ( ( m4&0x20 ) >> 4 ) ^ ( ( m5&0x08 ) >> 2 );

    u8 hashIdx2 = ( m0&0x04 ) ^ ( ( m1&0x01 ) << 2 ) ^ ( ( m1&0x40 ) >> 4 ) ^ ( ( m2&0x10 ) >> 2 )
                ^ ( m3&0x04 ) ^ ( ( m4&0x01 ) << 2 ) ^ ( ( m4&0x40 ) >> 4 ) ^ ( ( m5&0x10 ) >> 2 );

    u8 hashIdx3 = ( m0&0x08 ) ^ ( ( m1&0x02 ) << 2 ) ^ ( ( m1&0x80 ) >> 4 ) ^ ( ( m2&0x20 ) >> 2 )
                ^ ( m3&0x08 ) ^ ( ( m4&0x02 ) << 2 ) ^ ( ( m4&0x80 ) >> 4 ) ^ ( ( m5&0x20 ) >> 2 );

    u8 hashIdx4 = ( m0&0x10 ) ^ ( ( m1&0x04 ) << 2 ) ^ ( ( m2&0x01 ) << 4 ) ^ ( ( m2&0x40 ) >> 2 )
                ^ ( m3&0x10 ) ^ ( ( m4&0x04 ) << 2 ) ^ ( ( m5&0x01 ) << 4 ) ^ ( ( m5&0x40 ) >> 2 );

    u8 hashIdx5 = ( m0&0x20 ) ^ ( ( m1&0x08 ) << 2 ) ^ ( ( m2&0x02 ) << 4 ) ^ ( ( m2&0x80 ) >> 2 )
                ^ ( m3&0x20 ) ^ ( ( m4&0x08 ) << 2 ) ^ ( ( m5&0x02 ) << 4 ) ^ ( ( m5&0x80 ) >> 2 );

    return( hashIdx0 | hashIdx1 | hashIdx2 | hashIdx3 | hashIdx4 | hashIdx5 );
}

//-------------------------------------------------------------------------------------------------
//Initialize and enable the PHY interrupt when link-state changes
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_enable_phyirq( void )
{
#if 0

#endif
}

//-------------------------------------------------------------------------------------------------
// Disable the PHY interrupt
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_disable_phyirq( void )
{
#if 0

#endif
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

u32 MHal_GMAC_get_SA1H_addr( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SA1H );
}

u32 MHal_GMAC_get_SA1L_addr( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SA1L );
}

u32 MHal_GMAC_get_SA2H_addr( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SA2H );
}

u32 MHal_GMAC_get_SA2L_addr( void )
{
    return MHal_GMAC_ReadReg32( GMAC_REG_ETH_SA2L );
}

void MHal_GMAC_Write_SA1H( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA1H, xval );
}

void MHal_GMAC_Write_SA1L( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA1L, xval );
}

void MHal_GMAC_Write_SA2H( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA2H, xval );
}

void MHal_GMAC_Write_SA2L( u32 xval )
{
    MHal_GMAC_WritReg32( GMAC_REG_ETH_SA2L, xval );
}

void* MDev_GMAC_memset( void* s, u32 c, unsigned long count )
{
    char* xs = ( char* ) s;

    while ( count-- )
        *xs++ = c;

    return s;
}

//-------------------------------------------------------------------------------------------------
// Check INT Done
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_CheckINTDone( void )
{
    u32 retIntStatus;
    retIntStatus = MHal_GMAC_Read_ISR();
    MHalGMACThisUVE.cntChkINTCounter = ( MHalGMACThisUVE.cntChkINTCounter %
                                     MHal_MAX_INT_COUNTER );
    MHalGMACThisUVE.cntChkINTCounter ++;
    if ( ( retIntStatus & GMAC_INT_DONE ) ||
         ( MHalGMACThisUVE.cntChkINTCounter == ( MHal_MAX_INT_COUNTER - 1 ) ) )
    {
        MHalGMACThisUVE.flagISR_INT_DONE = 0x01;
        return TRUE;
    }
    return FALSE;
}

extern unsigned char gmac_phyaddr;
//-------------------------------------------------------------------------------------------------
// MAC cable connection detection
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_CableConnection( void )
{
    u32 retValue = 0;
    u32 word_ETH_MAN = 0x00000000;
    u32 word_ETH_CTL = MHal_GMAC_Read_CTL();

    MHal_GMAC_Write_CTL( 0x00000010 | word_ETH_CTL );
    MHalGMACThisUVE.flagISR_INT_DONE = 0x00;
    MHalGMACThisUVE.cntChkINTCounter = 0;
    MHal_GMAC_read_phy(gmac_phyaddr, MII_BMSR, &word_ETH_MAN);

    if ( word_ETH_MAN & BMSR_LSTATUS )
    {
        retValue = 1;
    }
    else
    {
        retValue = 0;
    }
    MHal_GMAC_Write_CTL( word_ETH_CTL );
    return(retValue);
}

//-------------------------------------------------------------------------------------------------
// GMAC Negotiation PHY
//-------------------------------------------------------------------------------------------------
u32 MHal_GMAC_NegotiationPHY( void )
{
    // Set PHY --------------------------------------------------------------
    u32 retValue = 0;
    u32 bmsr;

    // IMPORTANT: Get real duplex by negotiation with peer.
    u32 word_ETH_CTL = MHal_GMAC_Read_CTL();
    MHal_GMAC_Write_CTL( 0x0000001C | word_ETH_CTL );

    MHalGMACThisBCE.duplex = 1;   // Set default as Half-duplex if negotiation fails.
    retValue = 1;
    
    MHalGMACThisUVE.flagISR_INT_DONE = 0x00;
    MHalGMACThisUVE.cntChkINTCounter = 0;
    MHalGMACThisUVE.cntChkCableConnect = 0;
 

    MHal_GMAC_read_phy(gmac_phyaddr, MII_BMSR, &bmsr);
    if ( (bmsr & BMSR_100FULL) || (bmsr & BMSR_10FULL) )
    {
       MHalGMACThisBCE.duplex = 2;
       retValue = 2;
    }
    else
    {
        MHalGMACThisBCE.duplex = 1;
        retValue = 1;
    }

    // NOTE: REG_ETH_CFG must be set according to new ThisGMACBCE.duplex.

    MHal_GMAC_Write_CTL( word_ETH_CTL );
    // Set PHY --------------------------------------------------------------
    return(retValue);
}

//-------------------------------------------------------------------------------------------------
// GMAC Hardware register set
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// GMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_timer_callback( unsigned long value )
{
    u32 uRegVal;
    uRegVal = MHal_GMAC_Read_IER();
    uRegVal |= ( GMAC_INT_RCOM );
    MHal_GMAC_Write_IER( uRegVal );
}

//-------------------------------------------------------------------------------------------------
// GMAC clock on/off
//-------------------------------------------------------------------------------------------------
void MHal_GMAC_Power_On_Clk( void )
{
    u8 uRegVal;

#ifdef CONFIG_MSTAR_GMAC_RMII_DOWNGRADE_MODE
    // Set to 100M Reg_gt_xmii_type:0x000e,0x6b[2:0]=3'b110
    uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xD6);
    uRegVal &= 0xF8;
    uRegVal |= 0x6;
    MHal_GMAC_WritReg8(0x000e, 0xD6, uRegVal);

    // Set to 100M Reg_xmii_type : 0x121f,0x30[1:0]='b11
    uRegVal = MHal_GMAC_ReadReg8(0x121f, 0x60);
    uRegVal |= 0x3;
    MHal_GMAC_WritReg8(0x121f, 0x60, uRegVal);

    // Set to 100M Reg_rmii:0x121c,0x00[1]=1
    uRegVal = MHal_GMAC_ReadReg8(0x121c, 0x00);
    uRegVal |= 0x2;
    MHal_GMAC_WritReg8(0x121c, 0x00, uRegVal);

    // Clock, Ahb: 0x1033,0x32[0]=0
    uRegVal = MHal_GMAC_ReadReg8(0x1033, 0x64);
    uRegVal &= 0xFE;
    MHal_GMAC_WritReg8(0x1033, 0x64, uRegVal);

    // Clock, TX  : 0x1033,0x33[2:0]=3'b100
    uRegVal = MHal_GMAC_ReadReg8(0x1033, 0x66);
    uRegVal &= 0xF8;
    uRegVal |= 0x4;
    MHal_GMAC_WritReg8(0x1033, 0x66, uRegVal);

    // Clock, TX: 0x000e,0x6c[0]=0, [3]=1 [5]=0  (RX)
    uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xD8);
    uRegVal &= 0xD6;
    uRegVal |= 0x8;
    MHal_GMAC_WritReg8(0x000e, 0xD8, uRegVal);

    // Clock, RX  : 0x1033,0x32[10:8]=3'b100
    uRegVal = MHal_GMAC_ReadReg8(0x1033, 0x65);
    uRegVal &= 0xF8;
    uRegVal |= 0x4;
    MHal_GMAC_WritReg8(0x1033, 0x65, uRegVal);

    // Clock: TX_REF:0x1033,0x34[0]=0
    uRegVal = MHal_GMAC_ReadReg8(0x1033, 0x68);
    uRegVal &= 0xFE;
    MHal_GMAC_WritReg8(0x1033, 0x68, uRegVal);

    // Clock: RX_REF:0x1033,0x33[8]=0
    uRegVal = MHal_GMAC_ReadReg8(0x1033, 0x67);
    uRegVal &= 0xFE;
    MHal_GMAC_WritReg8(0x1033, 0x67, uRegVal);

    // Enable IOs : 0x000e,0x7a[14:0]='h7fff
    uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xF4);
    uRegVal = 0xFF;
    MHal_GMAC_WritReg8(0x000e, 0xF4, uRegVal);
    uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xF5);
    uRegVal = 0x7F;
    MHal_GMAC_WritReg8(0x000e, 0xF5, uRegVal);
#else

    /* Enable HW patch */
    uRegVal = MHal_GMAC_ReadReg8(0x121c, 0x38);
    uRegVal |= 0xc2;
    MHal_GMAC_WritReg8(0x121c, 0x38, uRegVal);

    uRegVal = MHal_GMAC_ReadReg8(0x121c, 0x39);
    uRegVal |= 0x02;
    MHal_GMAC_WritReg8(0x121c, 0x39, uRegVal);

    if (*(volatile unsigned int *)(0xFD203D9C) >= 0x0101) // K3S
    {
        // K3S, need to reference doc
        /* Enable TX delay */
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd9);
        uRegVal |= 0x04;
        MHal_GMAC_WritReg8(0x000e, 0xd9, uRegVal);

        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd9);
        uRegVal &= 0xFD;
        MHal_GMAC_WritReg8(0x000e, 0xd9, uRegVal);
    }

    
    /*
    MHal_GMAC_WritReg8(0x121b, 0x34, 0x07);
    MHal_GMAC_WritReg8(0x121b, 0x35, 0);
    
    MHal_GMAC_WritReg8(0x121b, 0x36, 0xfe);
    MHal_GMAC_WritReg8(0x121b, 0x37, 0x50);
    
    MHal_GMAC_WritReg8(0x121b, 0x34, 0xa4);
    MHal_GMAC_WritReg8(0x121b, 0x35, 0);
    
    MHal_GMAC_WritReg8(0x121b, 0x36, 0xfa);
    MHal_GMAC_WritReg8(0x121b, 0x37, 0x50);

    MHal_GMAC_WritReg8(0x121b, 0x34, 0x91);
    if (*(volatile unsigned int *)(0xFD203D9C) == 0x0101) // K3S
    {
        MHal_GMAC_WritReg8(0x121b, 0x35, 0xad);
    }
    else
    {
        MHal_GMAC_WritReg8(0x121b, 0x35, 0xbd);
    }
    
    MHal_GMAC_WritReg8(0x121b, 0x36, 0xf2);
    MHal_GMAC_WritReg8(0x121b, 0x37, 0x50);

    MHal_GMAC_WritReg8(0x121b, 0x34, 0);
    MHal_GMAC_WritReg8(0x121b, 0x35, 0);
    
    MHal_GMAC_WritReg8(0x121b, 0x36, 0xfe);
    MHal_GMAC_WritReg8(0x121b, 0x37, 0x50);
    */

    /* Set gmac ahb clock to 172MHZ */
    MHal_GMAC_WritReg8(0x1033, 0x64, 0x10);

    /* Enable gmac tx clock */
    uRegVal = MHal_GMAC_ReadReg8(0x1033, 0x66);
    uRegVal &= 0xfe;
    MHal_GMAC_WritReg8(0x1033, 0x66, uRegVal);

    if (*(volatile unsigned int *)(0xFD203D9C) >= 0x0101) // K3S
    {
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd8);
        uRegVal &= 0xce;
        MHal_GMAC_WritReg8(0x000e, 0xd8, uRegVal);

        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd9);
        uRegVal |= 0x80;
        MHal_GMAC_WritReg8(0x000e, 0xd9, uRegVal);
    }
    else
    {
        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd8);
        uRegVal &= 0xde;
        MHal_GMAC_WritReg8(0x000e, 0xd8, uRegVal);

        uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd8);
        uRegVal |= 0x10;
        MHal_GMAC_WritReg8(0x000e, 0xd8, uRegVal);
    }


    //uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd8);
    //uRegVal &= 0xdf;
    //MHal_GMAC_WritReg8(0x000e, 0xd8, uRegVal);

    /* Enable gmac rx clock */
    uRegVal = MHal_GMAC_ReadReg8(0x1033, 0x65);
    uRegVal &= 0xfe;
    MHal_GMAC_WritReg8(0x1033, 0x65, uRegVal);

    /* Set gt_mode = 1 */
    MHal_GMAC_WritReg8(0x000e, 0xf4, 0xff);

    uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xf5);
    uRegVal &= 0x80;
    uRegVal |= 0x7f;
    MHal_GMAC_WritReg8(0x000e, 0xf5, uRegVal);

    /* Digital synthesizer */
    MHal_GMAC_WritReg8(0x100b, 0xc6, 0x00);
    MHal_GMAC_WritReg8(0x110c, 0x68, 0x08);
    MHal_GMAC_WritReg8(0x110c, 0x69, 0x80);
    MHal_GMAC_WritReg8(0x110c, 0x6a, 0x83);
    MHal_GMAC_WritReg8(0x110c, 0x6b, 0x06);
    MHal_GMAC_WritReg8(0x110c, 0x70, 0x6a);
    MHal_GMAC_WritReg8(0x110c, 0x71, 0xbc);
    MHal_GMAC_WritReg8(0x110c, 0x72, 0x14);
    MHal_GMAC_WritReg8(0x110c, 0x73, 0x00);

    uRegVal = MHal_GMAC_ReadReg8(0x000e, 0xd9);
    uRegVal |= 0x80;
    MHal_GMAC_WritReg8(0x000e, 0xd9, uRegVal);

    MHal_GMAC_WritReg8(0x110c, 0x60, 0x19);
    MHal_GMAC_WritReg8(0x110c, 0x61, 0x00);
    MHal_GMAC_WritReg8(0x110c, 0x62, 0x00);
    MHal_GMAC_WritReg8(0x110c, 0x63, 0xaf);
    MHal_GMAC_WritReg8(0x110c, 0x64, 0x0a);
    MHal_GMAC_WritReg8(0x110c, 0x65, 0x00);
    MHal_GMAC_WritReg8(0x110c, 0x66, 0x07);
    MHal_GMAC_WritReg8(0x110c, 0x67, 0x00);

    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x0*4) ) =  (*( ( u32 * ) ( ( char * ) 0xFD243600 +  0x0*4) ) | 0x0010);
    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x1*4) ) =  (*( ( u32 * ) ( ( char * ) 0xFD243600 +  0x1*4) ));

    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x34*2) ) = 0x0007;
    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x36*2) ) = 0x50fe;
    mdelay(50);
    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x34*2) ) = 0x00a4;
    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x36*2) ) = 0x50fa;
    mdelay(50);
    if (*(volatile unsigned int *)(0xFD203D9C) == 0x0101) // K3S
    {
        *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x34*2) ) = 0xad91;
    }
    else
    {
        *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x34*2) ) = 0xbd91;
    }

    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x36*2) ) = 0x50f2;
    mdelay(50);
    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x34*2) ) = 0x0000;
    *( ( u32 * ) ( ( char * ) 0xFD243600 +  0x36*2) ) = 0x50fe;

    /* chiptop [15] allpad_in */
    uRegVal = MHal_GMAC_ReadReg8(0x101e, 0xa1);
    uRegVal &= 0x7F;
    MHal_GMAC_WritReg8(0x101e, 0xa1, uRegVal);

    /* chiptop pad_top [9:8]*/
    uRegVal = MHal_GMAC_ReadReg8(0x101e, 0xf8);
    uRegVal &= 0x01;
    MHal_GMAC_WritReg8(0x101e, 0xf8, uRegVal);
#endif
}

void MHal_GMAC_Power_Off_Clk( void )
{
}
