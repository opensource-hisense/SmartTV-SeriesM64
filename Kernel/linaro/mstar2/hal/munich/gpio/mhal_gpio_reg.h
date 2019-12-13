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
#define GPIO_UNIT_NUM               254

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define REG_MIPS_BASE               0xFD000000      //Use 8 bit addressing

#define REG_ALL_PAD_IN              (0x101ea1)      //set all pads (except SPI) as input
#define REG_LVDS_BASE               (0x103200)
#define REG_LVDS_BANK               REG_LVDS_BASE

#define PAD_PM_SPI_CZ                	0
#define PAD_PM_SPI_CK	1
#define PAD_PM_SPI_DI	2
#define PAD_PM_SPI_DO	3
#define PAD_PM_SPI_HOLDN	4
#define PAD_PM_SPI_WPN	5
#define PAD_VID0	6
#define PAD_VID1	7
#define PAD_IRIN	8
#define PAD_CEC	9
#define PAD_GPIO_PM0	10
#define PAD_GPIO_PM1	11
#define PAD_GPIO_PM2	12
#define PAD_GPIO_PM3	13
#define PAD_GPIO_PM4	14
#define PAD_GPIO_PM5	15
#define PAD_GPIO_PM6	16
#define PAD_GPIO_PM7	17
#define PAD_GPIO_PM8	18
#define PAD_GPIO_PM9	19
#define PAD_GPIO_PM10	20
#define PAD_GPIO_PM11	21
#define PAD_GPIO_PM12	22
#define PAD_GPIO_PM13	23
#define PAD_GPIO_PM14	24
#define PAD_GPIO_PM15	25
#define PAD_HOTPLUGA	26
#define PAD_HOTPLUGB	27
#define PAD_HOTPLUGC	28
#define PAD_HOTPLUGD	29
#define PAD_DDCDA_CK	30
#define PAD_DDCDA_DA	31
#define PAD_DDCDB_CK	32
#define PAD_DDCDB_DA	33
#define PAD_DDCDC_CK	34
#define PAD_DDCDC_DA	35
#define PAD_DDCDD_CK	36
#define PAD_DDCDD_DA	37
#define PAD_SAR0	38
#define PAD_SAR1	39
#define PAD_SAR2	40
#define PAD_SAR3	41
#define PAD_SAR4	42
#define PAD_PWM_PM	43
#define PAD_DDCA_CK	44
#define PAD_DDCA_DA	45
#define PAD_GPIO0	46
#define PAD_GPIO1	47
#define PAD_GPIO2	48
#define PAD_GPIO3	49
#define PAD_GPIO4	50
#define PAD_GPIO5	51
#define PAD_GPIO6	52
#define PAD_GPIO7	53
#define PAD_GPIO8	54
#define PAD_GPIO9	55
#define PAD_GPIO10	56
#define PAD_GPIO11	57
#define PAD_GPIO12	58
#define PAD_GPIO13	59
#define PAD_GPIO14	60
#define PAD_GPIO15	61
#define PAD_GPIO16	62
#define PAD_GPIO17	63
#define PAD_GPIO18	64
#define PAD_GPIO19	65
#define PAD_GPIO20	66
#define PAD_GPIO21	67
#define PAD_GPIO22	68
#define PAD_GPIO23	69
#define PAD_GPIO24	70
#define PAD_GPIO25	71
#define PAD_GPIO26	72
#define PAD_GPIO27	73
#define PAD_UART_RX2	74
#define PAD_UART_TX2	75
#define PAD_PWM0	76
#define PAD_PWM1	77
#define PAD_PWM2	78
#define PAD_PWM3	79
#define PAD_PWM4	80
#define PAD_DDCR_DA	81
#define PAD_DDCR_CK	82
#define PAD_TGPIO0	83
#define PAD_TGPIO1	84
#define PAD_TGPIO2	85
#define PAD_TGPIO3	86
#define PAD_TS0_D0	87
#define PAD_TS0_D1	88
#define PAD_TS0_D2	89
#define PAD_TS0_D3	90
#define PAD_TS0_D4	91
#define PAD_TS0_D5	92
#define PAD_TS0_D6	93
#define PAD_TS0_D7	94
#define PAD_TS0_VLD	95
#define PAD_TS0_SYNC	96
#define PAD_TS0_CLK	97
#define PAD_TS1_D0	98
#define PAD_TS1_D1	99
#define PAD_TS1_D2	100
#define PAD_TS1_D3	101
#define PAD_TS1_D4	102
#define PAD_TS1_D5	103
#define PAD_TS1_D6	104
#define PAD_TS1_D7	105
#define PAD_TS1_VLD	106
#define PAD_TS1_SYNC	107
#define PAD_TS1_CLK	108
#define PAD_PCM_A4	109
#define PAD_PCM_WAIT_N	110
#define PAD_PCM_A5	111
#define PAD_PCM_A6	112
#define PAD_PCM_A7	113
#define PAD_PCM_A12	114
#define PAD_PCM_IRQA_N	115
#define PAD_PCM_A14	116
#define PAD_PCM_A13	117
#define PAD_PCM_A8	118
#define PAD_PCM_IOWR_N	119
#define PAD_PCM_A9	120
#define PAD_PCM_IORD_N	121
#define PAD_PCM_A11	122
#define PAD_PCM_OE_N	123
#define PAD_PCM_A10	124
#define PAD_PCM_CE_N	125
#define PAD_PCM_D7	126
#define PAD_PCM_D6	127
#define PAD_PCM_D5	128
#define PAD_PCM_D4	129
#define PAD_PCM_D3	130
#define PAD_PCM_A3	131
#define PAD_PCM_A2	132
#define PAD_PCM_REG_N	133
#define PAD_PCM_A1	134
#define PAD_PCM_A0	135
#define PAD_PCM_D0	136
#define PAD_PCM_D1	137
#define PAD_PCM_D2	138
#define PAD_PCM_RESET	139
#define PAD_PCM_CD_N	140
#define PAD_PCM2_CE_N	141
#define PAD_PCM2_IRQA_N	142
#define PAD_PCM2_WAIT_N	143
#define PAD_PCM2_RESET	144
#define PAD_PCM2_CD_N	145
#define PAD_EMMC_IO0	146
#define PAD_EMMC_IO1	147
#define PAD_EMMC_IO2	148
#define PAD_EMMC_IO3	149
#define PAD_EMMC_IO4	150
#define PAD_EMMC_IO5	151
#define PAD_EMMC_IO6	152
#define PAD_EMMC_IO7	153
#define PAD_EMMC_IO8	154
#define PAD_EMMC_IO9	155
#define PAD_EMMC_IO10	156
#define PAD_EMMC_IO11	157
#define PAD_EMMC_IO12	158
#define PAD_EMMC_IO13	159
#define PAD_EMMC_IO14	160
#define PAD_EMMC_IO15	161
#define PAD_EMMC_IO16	162
#define PAD_EMMC_IO17	163
#define PAD_VSYNC_LIKE	164
#define PAD_I2S_IN_WS	165
#define PAD_I2S_IN_BCK	166
#define PAD_I2S_IN_SD	167
#define PAD_SPDIF_IN	168
#define PAD_SPDIF_OUT	169
#define PAD_I2S_OUT_MCK	170
#define PAD_I2S_OUT_WS	171
#define PAD_I2S_OUT_BCK	172
#define PAD_I2S_OUT_SD	173
#define PAD_I2S_OUT_SD1	174
#define PAD_I2S_OUT_SD2	175
#define PAD_I2S_OUT_SD3	176
#define PAD_B_ODD0	177
#define PAD_B_ODD1	178
#define PAD_B_ODD2	179
#define PAD_B_ODD3	180
#define PAD_B_ODD4	181
#define PAD_B_ODD5	182
#define PAD_B_ODD6	183
#define PAD_B_ODD7	184
#define PAD_G_ODD0	185
#define PAD_G_ODD1	186
#define PAD_G_ODD2	187
#define PAD_G_ODD3	188
#define PAD_G_ODD4	189
#define PAD_G_ODD5	190
#define PAD_G_ODD6	191
#define PAD_G_ODD7	192
#define PAD_R_ODD0	193
#define PAD_R_ODD1	194
#define PAD_R_ODD2	195
#define PAD_R_ODD3	196
#define PAD_R_ODD4	197
#define PAD_R_ODD5	198
#define PAD_R_ODD6	199
#define PAD_R_ODD7	200
#define PAD_LCK	201
#define PAD_LDE	202
#define PAD_LHSYNC	203
#define PAD_LVSYNC	204
#define PAD_PCM_WE_N	205
#define PAD_SPI1_CK	206
#define PAD_SPI1_DI	207
#define PAD_SPI2_CK	208
#define PAD_SPI2_DI	209
#define PAD_TCON0	210
#define PAD_TCON1	211
#define PAD_TCON2	212
#define PAD_TCON3	213
#define PAD_TCON4	214
#define PAD_TCON5	215
#define PAD_TCON6       	216
#define PAD_TCON7	217
#define PAD_TS2_D0	218
#define PAD_TS2_D1	219
#define PAD_TS2_D2	220
#define PAD_TS2_D3	221
#define PAD_TS2_D4	222
#define PAD_TS2_D5	223
#define PAD_TS2_D6	224
#define PAD_TS2_D7	225
#define PAD_TS2_CLK	226
#define PAD_TS2_SYNC	227
#define PAD_TS2_VLD	228
#define PAD_LINEIN_L1	229
#define PAD_LINEIN_R1	230
#define PAD_LINEIN_L2	231
#define PAD_LINEIN_R2	232
#define PAD_LINEIN_L3	233
#define PAD_LINEIN_R3	234
#define PAD_LINEIN_L4	235
#define PAD_LINEIN_R4	236
#define PAD_LINEIN_L5	237
#define PAD_LINEIN_R5	238
#define PADA_HSYNC0	239
#define PADA_VSYNC0	240
#define PADA_HSYNC1	241
#define PADA_VSYNC1	242
#define PADA_HSYNC2	243
#define PADA_VSYNC2	244
#define PADA_RIN0P	245
#define PADA_RIN1P	246
#define PADA_GIN0P	247
#define PADA_GIN1P	248
#define PADA_BIN0P	249
#define PADA_BIN1P	250
#define PADA_GIN0M	251
#define PADA_GIN1M	252
#define PAD_ARC0	253


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

