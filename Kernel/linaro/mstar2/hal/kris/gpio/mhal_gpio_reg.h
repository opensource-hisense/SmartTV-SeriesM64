////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_GPIO_H_
#define _REG_GPIO_H_

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------
#define GPIO_UNIT_NUM               145

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_ARM) || defined(CONFIG_MIPS)
#define REG_MIPS_BASE               0xBF000000      //Use 8 bit addressing
#elif defined(CONFIG_ARM64)
extern ptrdiff_t mstar_pm_base;
#define REG_MIPS_BASE           (mstar_pm_base)
#endif

#define REG_ALL_PAD_IN              (0x101ea1UL)      //set all pads (except SPI) as input
#define REG_LVDS_BASE               (0x103200UL)
#define REG_LVDS_BANK               REG_LVDS_BASE

#define PAD_SPDIF_OUT               0
#define PAD_I2S_OUT_MUTE            1
#define PAD_I2S_OUT_BCK             2
#define PAD_I2S_OUT_WS              3
#define PAD_I2S_OUT_MCK             4
#define PAD_I2S_OUT_SDO             5
#define PAD_GPIO0                   6
#define PAD_GPIO1                   7
#define PAD_SM0_CLK                 8
#define PAD_SM0_RST                 9
#define PAD_SM0_C4                  10
#define PAD_SM0_IO                  11
#define PAD_SM0_C8                  12
#define PAD_SM0_CD                  13
#define PAD_SM0_GPIO                14
#define PAD_SM0_PCT                 15
#define PAD_T_GPIO0                 16
#define PAD_T_GPIO1                 17
#define PAD_T_GPIO2                 18
#define PAD_T_GPIO3                 19
#define PAD_T_GPIO4                 20
#define PAD_T_GPIO5                 21
#define PAD_FUART_RX                22
#define PAD_FUART_TX                23
#define PAD_FUART_CTS               24
#define PAD_FUART_RTS               25
#define PAD_GPIO2                   26
#define PAD_GPIO3                   27
#define PAD_GPIO4                   28
#define PAD_GPIO5                   29
#define PAD_GPIO6                   30
#define PAD_GPIO9                   31
#define PAD_GPIO10                  32
#define PAD_GPIO11                  33
#define PAD_GPIO12                  34
#define PAD_GPIO13                  35
#define PAD_GPIO14                  36
#define PAD_GPIO15                  37
#define PAD_GPIO16                  38
#define PAD_CI_A4                   39
#define PAD_CI_A3                   40
#define PAD_CI_A6                   41
#define PAD_CI_A5                   42
#define PAD_CI_A0                   43
#define PAD_CI_A1                   44
#define PAD_CI_A2                   45
#define PAD_CI_A7                   46
#define PAD_CI_A8                   47
#define PAD_CI_A9                   48
#define PAD_GPIO7                   49
#define PAD_GPIO8                   50
#define PAD_CI_A10                  51
#define PAD_CI_A11                  52
#define PAD_CI_A12                  53
#define PAD_CI_A13                  54
#define PAD_CI_A14                  55
#define PAD_CI_CDZ                  56
#define PAD_CI_D3                   57
#define PAD_CI_D4                   58
#define PAD_CI_D5                   59
#define PAD_CI_D6                   60
#define PAD_CI_D7                   61
#define PAD_CI_CEZ                  62
#define PAD_CI_OEZ                  63
#define PAD_CI_IORDZ                64
#define PAD_CI_IOWRZ                65
#define PAD_CI_WEZ                  66
#define PAD_CI_IRQAZ                67
#define PAD_CI_RST                  68
#define PAD_CI_WAITZ                69
#define PAD_CI_REGZ                 70
#define PAD_CI_D0                   71
#define PAD_CI_D1                   72
#define PAD_CI_D2                   73
#define PAD_TS0_SYNC                74
#define PAD_TS0_VLD                 75
#define PAD_TS0_D0                  76
#define PAD_TS0_D1                  77
#define PAD_TS0_D2                  78
#define PAD_TS0_D3                  79
#define PAD_TS0_D4                  80
#define PAD_TS0_D5                  81
#define PAD_TS0_D6                  82
#define PAD_TS0_D7                  83
#define PAD_TS0_CLK                 84
#define PAD_TS1_SYNC                85
#define PAD_TS1_VLD                 86
#define PAD_TS1_D0                  87
#define PAD_TS1_D1                  88
#define PAD_TS1_D2                  89
#define PAD_TS1_D3                  90
#define PAD_TS1_D4                  91
#define PAD_TS1_D5                  92
#define PAD_TS1_D6                  93
#define PAD_TS1_D7                  94
#define PAD_TS1_CLK                 95
#define PAD_I2CM0_SCL               96
#define PAD_I2CM0_SDA               97
#define PAD_I2CM1_SCL               98
#define PAD_I2CM1_SDA               99
#define PAD_PM_IRIN                 100
#define PAD_PM_SPI_CZ               101
#define PAD_PM_SPI_CK               102
#define PAD_PM_SPI_DI               103
#define PAD_PM_SPI_DO               104
#define PAD_PM_SPI_WPZ              105
#define PAD_PM_SPI_HOLDZ            106
#define PAD_PM_SPI_RSTZ             107
#define PAD_PM_GPIO0                108
#define PAD_PM_GPIO1                109
#define PAD_PM_GPIO2                110
#define PAD_PM_GPIO3                111
#define PAD_PM_GPIO4                112
#define PAD_PM_GPIO5                113
#define PAD_PM_GPIO6                114
#define PAD_PM_GPIO7                115
#define PAD_PM_GPIO8                116
#define PAD_PM_GPIO9                117
#define PAD_PM_GPIO10               118
#define PAD_PM_GPIO11               119
#define PAD_PM_GPIO12               120
#define PAD_PM_GPIO13               121
#define PAD_PM_GPIO14               122
#define PAD_PM_GPIO15               123
#define PAD_PM_GPIO16               124
#define PAD_PM_HDMI_CEC             125
#define PAD_PM_HDMI_HPD             126
#define PAD_GT1_MDC                 127
#define PAD_GT1_MDIO                128
#define PAD_GT1_RX_CLK              129
#define PAD_GT1_RX_CTL              130
#define PAD_GT1_RX_D0               131
#define PAD_GT1_RX_D1               132
#define PAD_GT1_RX_D2               133
#define PAD_GT1_RX_D3               134
#define PAD_GT1_TX_CLK              135
#define PAD_GT1_TX_CTL              136
#define PAD_GT1_TX_D0               137
#define PAD_GT1_TX_D1               138
#define PAD_GT1_TX_D2               139
#define PAD_GT1_TX_D3               140
#define PAD_SAR_GPIO0               141
#define PAD_SAR_GPIO1               142
#define PAD_SAR_GPIO2               143
#define PAD_SAR_GPIO3               144





#define GPIO_OEN                    0   //set o to nake output
#define GPIO_ODN                    1

#define IN_HIGH                     1   //input high
#define IN_LOW                      0   //input low

#define OUT_HIGH                    1   //output high
#define OUT_LOW                     0   //output low

#define MHal_GPIO_REG(addr)         (*(volatile U8*)(REG_MIPS_BASE + (((addr) & ~1) << 1) + (addr & 1)))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#endif // _REG_GPIO_H_

