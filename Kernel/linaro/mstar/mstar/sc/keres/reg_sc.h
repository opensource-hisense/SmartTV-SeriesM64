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
/// file    reg_sc.h
/// @brief  Smart Card Module Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SC_H_
#define _REG_SC_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SC_REGBASE1                 0xBF205200
#define SC_REGBASE2                 0xBF205280
#define HW_REGBASE                  0xBF200000

// ------------------------------------------------------------------------------------------------
// UART / SC
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }

#define UART1_READ(addr)            READ_BYTE(SC_REGBASE1 + ((addr)<<2))
#define UART2_READ(addr)            READ_BYTE(SC_REGBASE2 + ((addr)<<2))

#define UART1_WRITE(addr, val)      WRITE_BYTE((SC_REGBASE1 + ((addr)<<2)), (val))
#define UART2_WRITE(addr, val)      WRITE_BYTE((SC_REGBASE2 + ((addr)<<2)), (val))

#define UART1_OR(addr, val)         UART1_WRITE(addr, UART1_READ(addr) | (val))
#define UART1_AND(addr, val)        UART1_WRITE(addr, UART1_READ(addr) & (val))
#define UART1_XOR(addr, val)        UART1_WRITE(addr, UART1_READ(addr) ^ (val))
#define UART2_OR(addr, val)         UART2_WRITE(addr, UART2_READ(addr) | (val))
#define UART2_AND(addr, val)        UART2_WRITE(addr, UART2_READ(addr) & (val))
#define UART2_XOR(addr, val)        UART2_WRITE(addr, UART2_READ(addr) ^ (val))


//
// UART Register List
//
#define UART_RX                             (0x00)                  // In:  Receive buffer (DLAB=0), 16-byte FIFO
#define UART_TX                             (0x00)                  // Out: Transmit buffer (DLAB=0), 16-byte FIFO
#define UART_DLL                            (0x00)                  // Out: Divisor Latch Low (DLAB=1)
#define UART_DLM                            (0x01)                  // Out: Divisor Latch High (DLAB=1)
#define UART_IER                            (0x01)                  // Out: Interrupt Enable Register
#define UART_IIR                            (0x02)                  // In:  Interrupt ID Register
#define UART_FCR                            (0x02)                  // Out: FIFO Control Register
#define UART_EFR                            (0x02)                  // I/O: Extended Features Register
                                                                    // (DLAB=1, 16C660 only)
#define UART_LCR                            (0x03)                  // Out: Line Control Register
#define UART_MCR                            (0x04)                  // Out: Modem Control Register
#define UART_LSR                            (0x05)                  // In:  Line Status Register
#define UART_MSR                            (0x06)                  // In:  Modem Status Register
#define UART_SCR                            (0x07)                  // I/O: Scratch Register
                                             
#define UART_SCCR                           (0x08)                  // Smartcard Control Register
#define UART_SCSR                           (0x09)                  // Smartcard Status Register
                                        
#define UART_SCFC                           (0x0a)                  // Smartcard Fifo Count Register
#define UART_SCFI                           (0x0b)                  // Smartcard Fifo Index Register
#define UART_SCFR                           (0x0c)                  // Smartcard Fifo Read Delay Register
                                        
#define UART_SCMR                           (0x0d)                  // Smartcard Mode Register
#define UART_SCCGT                          (0x0e)                  // Smartcard char guard time - only after Kronus U02
#define UART_SCDBT                          (0x0f)                  // Smartcard debounce time
#define UART_SCCWT_L                        (0x10)                  // Smartcard char waiting time - LoByte
#define UART_SCCWT_H                        (0x11)                  // Smartcard char waiting time - HiByte
#define UART_SCBGT                          (0x12)                  // Smartcard block guard time
#define UART_SCBWT_0                        (0x14)                  // Smartcard block waiting time bit 07:00
#define UART_SCBWT_1                        (0x15)                  // Smartcard block waiting time bit 15:08
#define UART_SCBWT_2                        (0x16)                  // Smartcard block waiting time bit 23:16
#define UART_SCBWT_3                        (0x17)                  // Smartcard block waiting time bit 31:24
#define UART_CTRL2                          (0x18)                  // CGT/CWT/BGT/BWT int mask
#define UART_GWT_INT                        (0x19)                  // CGT/CWT/BGT/BWT int status
#define UART_TX_FIFO_COUNT                  (0x1a)                  // TX FIFO count
#define UART_DEACTIVE_RST                   (0x1b)                  // Set the active time for RST pin
#define UART_DEACTIVE_CLK                   (0x1c)                  // Set the active time for CLK pin
#define UART_DEACTIVE_IO                    (0x1d)                  // Set the active time for IO pin
#define UART_DEACTIVE_VCC                   (0x1e)                  // Set the active time for VCC pin
#define UART_CTRL3                          (0x1f)                  // ctrl3, bit0 use to enable the de-active sequence func
#define UART_ACTIVE_RST                     (0x20)                  // Set the active time for RST pin
#define UART_ACTIVE_CLK                     (0x21)                  // Set the active time for CLK pin
#define UART_ACTIVE_IO                      (0x22)                  // Set the active time for IO pin
#define UART_ACTIVE_VCC                     (0x23)                  // Set the active time for VCC pin
#define UART_CTRL4                          (0x24)                  // control4 reg
#define UART_CTRL5                          (0x25)                  // control5 reg
#define UART_DEBUG4                         (0x26)                  // debug4 reg
#define UART_CTRL6                          (0x27)                  // control6 reg
#define UART_SHORT_CIRCUT_DB_TIME_L         (0x28)                  // short_circuit_debounce_time_low
#define UART_SHORT_CIRCUT_DB_TIME_H         (0x29)                  // short_circuit_debounce_time_high
#define UART_SMC_ATOP_LOW                   (0x2a)                  // reg_test_smc_atop_low
#define UART_SMC_ATOP_HIGH                  (0x2b)                  // reg_test_smc_atop_high
#define UART_CTRL7                          (0x2c)                  // control7 reg
#define UART_ANALOG_STATUS                  (0x2d)                  // analog_status reg
#define UART_CTRL8                          (0x2e)                  // control8 reg
#define UART_CTRL9                          (0x2f)                  // control9 reg

//
// UART_FCR(2)
// FIFO Control Register (16650 only)
//
#define UART_FCR_ENABLE_FIFO        0x01                                // Enable the FIFO
#define UART_FCR_CLEAR_RCVR         0x02                                // Clear the RCVR FIFO
#define UART_FCR_CLEAR_XMIT         0x04                                // Clear the XMIT FIFO
#define UART_FCR_DMA_SELECT         0x08                                // For DMA applications
#define UART_FCR_TRIGGER_MASK       0xC0                                // Mask for the FIFO trigger range
#define UART_FCR_TRIGGER_1          0x00                                // Mask for trigger set at 1
#define UART_FCR_TRIGGER_4          0x40                                // Mask for trigger set at 4
#define UART_FCR_TRIGGER_8          0x80                                // Mask for trigger set at 8
#define UART_FCR_TRIGGER_14         0xC0                                // Mask for trigger set at 14

//
// UART_LCR(3)
// Line Control Register
// Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
// UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
//
#define UART_LCR_WLEN5              0x00                                // Wordlength: 5 bits
#define UART_LCR_WLEN6              0x01                                // Wordlength: 6 bits
#define UART_LCR_WLEN7              0x02                                // Wordlength: 7 bits
#define UART_LCR_WLEN8              0x03                                // Wordlength: 8 bits
#define UART_LCR_STOP1              0x00                                //
#define UART_LCR_STOP2              0x04                                // Stop bits: 0=1 stop bit, 1= 2 stop bits
#define UART_LCR_PARITY             0x08                                // Parity Enable
#define UART_LCR_EPAR               0x10                                // Even parity select
#define UART_LCR_SPAR               0x20                                // Stick parity (?)
#define UART_LCR_SBC                0x40                                // Set break control
#define UART_LCR_DLAB               0x80                                // Divisor latch access bit

//
// UART_LSR(5)
// Line Status Register
//
#define UART_LSR_DR                 0x01                                // Receiver data ready
#define UART_LSR_OE                 0x02                                // Overrun error indicator
#define UART_LSR_PE                 0x04                                // Parity error indicator
#define UART_LSR_FE                 0x08                                // Frame error indicator
#define UART_LSR_BI                 0x10                                // Break interrupt indicator
#define UART_LSR_THRE               0x20                                // Transmit-hold-register empty
#define UART_LSR_TEMT               0x40                                // Transmitter empty

//
// UART_IIR(2)
// Interrupt Identification Register
//
#define UART_IIR_NO_INT             0x01                                // No interrupts pending
#define UART_IIR_ID                 0x0E                                // Mask for the interrupt ID

#define UART_IIR_MSI                0x00                                // Modem status interrupt
#define UART_IIR_THRI               0x02                                // Transmitter holding register empty
#define UART_IIR_TOI                0x0c                                // Receive time out interrupt
#define UART_IIR_RDI                0x04                                // Receiver data interrupt
#define UART_IIR_RLSI               0x06                                // Receiver line status interrupt

//
// UART_IER(1)
// Interrupt Enable Register
//
#define UART_IER_RDI                0x01                                // Enable receiver data available interrupt
#define UART_IER_THRI               0x02                                // Enable Transmitter holding reg empty int
#define UART_IER_RLSI               0x04                                // Enable receiver line status interrupt
#define UART_IER_MSI                0x08                                // Enable Modem status interrupt

//
// UART_MCR(4)
// Modem Control Register
//
#define UART_MCR_DTR                0x01                                // DTR complement
#define UART_MCR_RTS                0x02                                // RTS complement
#define UART_MCR_OUT1               0x04                                // Out1 complement
#define UART_MCR_OUT2               0x08                                // Out2 complement
#define UART_MCR_LOOP               0x10                                // Enable loopback test mode

#define UART_MCR_FAST               0x20                                // Slow / Fast baud rate mode

//
// UART_MSR(6)
// Modem Status Register
//
#define UART_MSR_ANY_DELTA          0x0F                                // Any of the delta bits!
#define UART_MSR_DCTS               0x01                                // Delta CTS
#define UART_MSR_DDSR               0x02                                // Delta DSR
#define UART_MSR_TERI               0x04                                // Trailing edge ring indicator
#define UART_MSR_DDCD               0x08                                // Delta DCD
#define UART_MSR_CTS                0x10                                // Clear to Send
#define UART_MSR_DSR                0x20                                // Data Set Ready
#define UART_MSR_RI                 0x40                                // Ring Indicator
#define UART_MSR_DCD                0x80                                // Data Carrier Detect

//
// UART_EFR(2, UART_LCR_DLAB)
// These are the definitions for the Extended Features Register
// (StarTech 16C660 only, when DLAB=1)
//
#define UART_EFR_ENI                0x10                                // Enhanced Interrupt
#define UART_EFR_SCD                0x20                                // Special character detect
#define UART_EFR_RTS                0x40                                // RTS flow control
#define UART_EFR_CTS                0x80                                // CTS flow control

//
// UART_SCCR(8)
// SmartCard Control Register
//
#define UART_SCCR_MASK_CARDIN       0x01                                // Smartcard card in interrupt mask
#define UART_SCCR_MASK_CARDOUT      0x02                                // Smartcard card out interrupt mask
#define UART_SCCR_TX_BINV           0x04                                // Smartcard Tx bit invert
#define UART_SCCR_TX_BSWAP          0x08                                // Smartcard Tx bit swap
#define UART_SCCR_RST               0x10                                // Smartcard reset 0->1, UART Rx enable 1
#define UART_SCCR_RX_BINV           0x20                                // Smartcard Rx bit inverse
#define UART_SCCR_RX_BSWAP          0x40                                // Smartcard Rx bit swap

//
// UART_SCSR(9)
// Smartcard Status Register
//
#define UART_SCSR_CLK               0x01                                // Smartcard clock out
#define UART_SCSR_INT_CARDIN        0x02                                // Smartcard card in interrupt
#define UART_SCSR_INT_CARDOUT       0x04                                // Smartcard card out interrupt
#define UART_SCSR_DETECT            0x08                                // Smartcard detection status
#define UART_SCSR_BGT_MASK      0x20                                // Smartcard BGT interrupt mask
#define UART_SCSR_CWT_MASK      0x40                              // Smartcard CWT interrupt mask
#define UART_SCSR_CGT_MASK      0x80                                // Smartcard CGT interrupt mask


//
// UART_SCFC(10), UART_SCFI(11), UART_SCFR(12)
// Smartcard Fifo Register
//
#define UART_SCFC_MASK              0x07
#define UART_SCFI_MASK              0x0F
#define UART_SCFR_MASK              0x07


//
// UART_SCFR(12)
// Smartcard Fifo Read Delay Register
//
#define UART_SCFR_DELAY_MASK        0x03
#define UART_SCFR_V_HIGH            0x04
#define UART_SCFR_V_ENABLE          0x08                                // Vcc = (Vcc_high ^ (Vcc_en & UART_SCSR_INT_CARDOUT))
#define UART_SCFR_STOP_BIT_CNT_REACH_NDS_FLC_EN 0x80                                           
//
// UART_SCMR(13)
// SMart Mode Register
//
#define UART_SCMR_RETRY_MASK        0x1F
#define UART_SCMR_SMARTCARD         0x20
#define UART_SCMR_PARITYCHK         0x40
#define UART_SCMR_FLOWCTRL                  0x80								// control the parity check error and prevent from data overflow

//
// UART_CTRL2(0x18)
//
#define UART_CTRL2_FLAG_CLEAR           0x01
#define UART_CTRL2_CGWT_MASK            0x02                        // Smartcard CGWT interrupt mask
#define UART_CTRL2_BGWT_MASK            0x04                        // Smartcard BGWT interrupt mask
#define UART_CTRL2_TX_LEVEL_MASK        0x08                        // Smartcard TX level interrupt mask
#define UART_CTRL2_TX_LEVEL             0x30                        // Smartcard TX level interrupt
#define UART_CTRL2_NDS_FLC_EN           0x80                        // Smartcard NDS flow control enable
#define UART_CTRL2_TX_LEVEL_5_TO_4      0x00                        // TX level to 5->4 trigger int mode
#define UART_CTRL2_TX_LEVEL_9_TO_8      0x10                        // TX level to 9->8 trigger int mode
#define UART_CTRL2_TX_LEVEL_17_TO_16    0x20                        // TX level to 17->16 trigger int mode
#define UART_CTRL2_TX_LEVEL_31_TO_30    0x30                        // TX level to 31->30 trigger int mode

//
// UART_GWT_INT(0x19)
//
#define UART_GWT_BWT_FAIL               0x01                               
#define UART_GWT_BGT_FAIL               0x02
#define UART_GWT_CWT_RX_FAIL            0x04
#define UART_GWT_CWT_TX_FAIL            0x08
#define UART_GWT_CGT_RX_FAIL            0x10
#define UART_GWT_CGT_TX_FAIL            0x20
#define UART_GWT_TX_LEVEL_INT           0x40


//
// UART_CTRL3(0x1F)
//
#define UART_DEACTIVE_SEQ_EN            0x01                        // Smartcard deactive sequence enable
#define UART_NDS_FLC_BLK_DATA_CTRL      0x80                        // Tell rx module DO NOT received any data during NDS flow control


//
// UART_CTRL5(0x25)
//
#define UART_CTRL5_PAD_MASK             0x10                        // Block all pads output to Smart Card
#define UART_CTRL5_PAD_RELEASE          0x20                        // Make all pads output to Smart Card without block
#define UART_CTRL5_ACTIVE_SEQ_EN        0x01                        // Smartcard active sequence enable
#define UART_CTRL5_AUTO_SEQ_CTRL        0x08                        // Smartcard auto sequence enable


//
// UART_CTRL6(0x27)
//
#define UART_CTRL6_SHORT_CIRCUIT_AUTO_DEACTIVE          0x80        // Automatical to deactivate while short circuit
#define UART_CTRL6_SHORT_CIRCUIT_MASK                   0x40
#define UART_CTRL6_SHORT_CIRCUIT_DISABLE                0x20        // Disable short circuit automatical deactivation
#define UART_CTRL6_GCR_SMC_3P3_GPIO_EN                  0x10        // Config as general GPIO or Smart Card mode
#define UART_CTRL6_LDO_3V_5V_EN                         0x08        // Enable 5v/3v mode
#define UART_CTRL6_PD_SMC_LDO                           0x04        // Power down Smart Card internal analog circuit


//
// UART_CTRL7(0x2c)
//
#define UART_CTRL7_PD_SMC_PAD           0x40            // Power down Smart Card PAD function
#define UART_CTRL7_ACT_PULLUP_EN        0x80            // Increasing I/O pad raising time

//
// UART_ANALOG_STATUS (0x2d)
//
#define UART_ANALOG_STATUS_SMC_LDO_5V_EN                0x20        // 5v enable status
#define UART_ANALOG_STATUS_PD_SMC_LDO                   0x10        // Power down Smart Card status 
#define UART_ANALOG_STATUS_PD_SMC_PAD                   0x08        // Power down Smart Card pad func 
#define UART_ANALOG_STATUS_GCR_SMC_VL_SEL               0x04        // 
#define UART_ANALOG_STATUS_LDO_3V_TO_5V_DONE            0x02        // 3v to 5v is done   
#define UART_ANALOG_STATUS_LDO_5V_TO_3V_DONE            0x01        // 5v to 3v is done

//
// UART_CTRL8 (0x2e)
//
#define UART_CTRL8_ANALOG_DC_CARD_RESET                 0x04        // analog circuit donot care card reset
#define UART_CTRL8_CWT_EXT_EN                           0x02        // Extend CWT enable
#define UART_CTRL8_OCP_INTRF_MASK                       0x01        //

//
// UART_CTRL9 (0x2f)
//
#define UART_CTRL9_CWT_EXT                              0xFF        // Extend CWT


// ------------------------------------------------------------------------------------------------
// TOP

#if defined(READ_WORD) && defined(WRITE_WORD)
#define HW_READ(addr)               READ_WORD(HW_REGBASE + ((addr)<<2))
#define HW_WRITE(addr, val)         WRITE_WORD((HW_REGBASE + ((addr)<<2)), (val))
#else // HAL / HWTEST
#define HW_READ(addr)               (*(volatile MS_U16*)(HW_REGBASE + ((addr)<<2)))
#define HW_WRITE(addr, val)         { (*((volatile MS_U16*)(HW_REGBASE + ((addr)<<2)))) = (MS_U16)(val); }
#endif

#define REG_TOP_SM0                 (0x0F00+0x0C)
    #define TOP_SM0_OPEN                            BITS(2:0,7)
    #define TOP_SM0_C48                             BITS(5:4,3)
    #define TOP_SM0_EN                              BITS(10:8,1)

#define REG_TOP_SM1                 (0x0F00+0x0D)
    #define TOP_SM1_OPEN                            BITS(1:0,3)
    #define TOP_SM1_C48                             BITS(5:4,3)
    #define TOP_SM1_EN                              BITS(10:8,2)

#define REG_TOP_OCP                 (0x0F00+0x0E)
    #define TOP_OCP0_EN                             BITS(1:0,3)
    #define TOP_OCP1_EN                             BITS(5:4,3)

#define REG_TOP_ALLPAD_IN           (0x0F00+0x50)
    #define TOP_ALLPAD_IN_EN                        BIT(15)

#define REG_TOP_CKG_UART_SRC        (0x0580+0x13)
    #define TOP_CKG_UART_SRC_DFT                    0
    #define TOP_CKG_UART0_SRC_CLK                   BIT(0)              // DFT_LIVE / CLK_UART
    #define TOP_CKG_UART1_SRC_CLK                   BIT(1)
    #define TOP_CKG_UART2_SRC_CLK                   BIT(2)

#define REG_TOP_CKG_UART_CLK        (0x0580+0x14)
    #define TOP_CKG_UART1_MASK                      BMASK(4:0)
    #define TOP_CKG_UART1_CLK_DIS                   BIT(0)
    #define TOP_CKG_UART1_CLK_INV                   BIT(1)
    #define TOP_CKG_UART1_CLK_MASK                  BMASK(4:2)
    #define TOP_CKG_UART1_CLK_170M                  BITS(4:2, 0)
    #define TOP_CKG_UART1_CLK_160M                  BITS(4:2, 1)
    #define TOP_CKG_UART1_CLK_144M                  BITS(4:2, 2)
    #define TOP_CKG_UART1_CLK_123M                  BITS(4:2, 3)
    #define TOP_CKG_UART1_CLK_108M                  BITS(4:2, 4)
    #define TOP_CKG_UART1_CLK_PLL                   BITS(4:2, 5)
    #define TOP_CKG_UART1_CLK_PLL_D2                BITS(4:2, 6)

    #define TOP_CKG_UART2_MASK                      BMASK(12:8)
    #define TOP_CKG_UART2_CLK_DIS                   BIT(8)
    #define TOP_CKG_UART2_CLK_INV                   BIT(9)
    #define TOP_CKG_UART2_CLK_MASK                  BMASK(12:10)
    #define TOP_CKG_UART2_CLK_170M                  BITS(12:10, 0)
    #define TOP_CKG_UART2_CLK_160M                  BITS(12:10, 1)
    #define TOP_CKG_UART2_CLK_144M                  BITS(12:10, 2)
    #define TOP_CKG_UART2_CLK_123M                  BITS(12:10, 3)
    #define TOP_CKG_UART2_CLK_108M                  BITS(12:10, 4)
    #define TOP_CKG_UART2_CLK_PLL                   BITS(12:10, 5)
    #define TOP_CKG_UART2_CLK_PLL_D2                BITS(12:10, 6)

#define REG_TOP_CKG_SM_CA           (0x1980+0x25)
    #define TOP_CKG_SM_CA0_MASK                     BMASK(3:0)
    #define TOP_CKG_SM_CA0_DIS                      BIT(0)
    #define TOP_CKG_SM_CA0_INV                      BIT(1)
    #define TOP_CKG_SM_CA0_CLK_MASK                 BMASK(3:2)
    #define TOP_CKG_SM_CA0_CLK_27M_D6               BITS(3:2, 0)        // 4.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D2               BITS(3:2, 1)        // 13.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D4               BITS(3:2, 2)        // 6.75MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D8               BITS(3:2, 3)        // 3.375MHz
    #define TOP_CKG_SM_CA1_MASK                     BMASK(7:4)
    #define TOP_CKG_SM_CA1_DIS                      BIT(4)
    #define TOP_CKG_SM_CA1_INV                      BIT(5)
    #define TOP_CKG_SM_CA1_CLK_MASK                 BMASK(7:6)
    #define TOP_CKG_SM_CA1_CLK_27M_D6               BITS(7:6, 0)        // 4.5MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D2               BITS(7:6, 1)        // 13.5MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D4               BITS(7:6, 2)        // 6.75MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D8               BITS(7:6, 3)        // 3.375MHz


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_SC_H_
