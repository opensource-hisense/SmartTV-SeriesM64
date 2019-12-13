#define _CONCAT( a, b )     a##b
#define CONCAT( a, b )      _CONCAT( a, b )

#define BIT0    (0x0001)
#define BIT1    (0x0002)
#define BIT2    (0x0004)
#define BIT3    (0x0008)
#define BIT4    (0x0010)
#define BIT5    (0x0020)
#define BIT6    (0x0040)
#define BIT7    (0x0080)
#define BIT8    (0x0100)
#define BIT9    (0x0200)
#define BIT10   (0x0400)
#define BIT11   (0x0800)
#define BIT12   (0x1000)
#define BIT13   (0x2000)
#define BIT14   (0x4000)
#define BIT15   (0x8000)

#define EN_PCM_GPIO 1
#define EN_NOR_GPIO 0
#define EN_PF_GPIO  1

// Dummy
#define GPIO999_OEN     0, 0
#define GPIO999_OUT     0, 0
#define GPIO999_IN      0, 0

#define GPIO0_PAD PAD_PM_SPI_CZ
#define GPIO0_OEN 0x0e3c, BIT4
#define GPIO0_OUT 0x0e3a, BIT4
#define GPIO0_IN  0x0e3b, BIT4

#define GPIO1_PAD PAD_PM_SPI_CK
#define GPIO1_OEN 0x0e3c, BIT5
#define GPIO1_OUT 0x0e3a, BIT5
#define GPIO1_IN  0x0e3b, BIT5

#define GPIO2_PAD PAD_PM_SPI_DI
#define GPIO2_OEN 0x0e3c, BIT6
#define GPIO2_OUT 0x0e3a, BIT6
#define GPIO2_IN  0x0e3b, BIT6

#define GPIO3_PAD PAD_PM_SPI_DO
#define GPIO3_OEN 0x0e3c, BIT7
#define GPIO3_OUT 0x0e3a, BIT7
#define GPIO3_IN  0x0e3b, BIT7

#define GPIO4_PAD PAD_IRIN
#define GPIO4_OEN 0x0e3c, BIT0
#define GPIO4_OUT 0x0e3a, BIT0
#define GPIO4_IN  0x0e3b, BIT0

#define GPIO5_PAD PAD_CEC
#define GPIO5_OEN 0x0e3c, BIT2
#define GPIO5_OUT 0x0e3a, BIT2
#define GPIO5_IN  0x0e3b, BIT2

#define GPIO6_PAD PAD_GPIO_PM0
#define GPIO6_OEN 0x0e1e, BIT0
#define GPIO6_OUT 0x0e20, BIT0
#define GPIO6_IN  0x0e22, BIT0

#define GPIO7_PAD PAD_GPIO_PM1
#define GPIO7_OEN 0x0e1e, BIT1
#define GPIO7_OUT 0x0e20, BIT1
#define GPIO7_IN  0x0e22, BIT1

#define GPIO8_PAD PAD_GPIO_PM2
#define GPIO8_OEN 0x0e1e, BIT2
#define GPIO8_OUT 0x0e20, BIT2
#define GPIO8_IN  0x0e22, BIT2

#define GPIO9_PAD PAD_GPIO_PM3
#define GPIO9_OEN 0x0e1e, BIT3
#define GPIO9_OUT 0x0e20, BIT3
#define GPIO9_IN  0x0e22, BIT3

#define GPIO10_PAD PAD_GPIO_PM4
#define GPIO10_OEN 0x0e1e, BIT4
#define GPIO10_OUT 0x0e20, BIT4
#define GPIO10_IN  0x0e22, BIT4

#define GPIO11_PAD PAD_GPIO_PM5
#define GPIO11_OEN 0x0e1e, BIT5
#define GPIO11_OUT 0x0e20, BIT5
#define GPIO11_IN  0x0e22, BIT5

#define GPIO12_PAD PAD_GPIO_PM6
#define GPIO12_OEN 0x0e1e, BIT6
#define GPIO12_OUT 0x0e20, BIT6
#define GPIO12_IN  0x0e22, BIT6

#define GPIO13_PAD PAD_GPIO_PM7
#define GPIO13_OEN 0x0e1e, BIT7
#define GPIO13_OUT 0x0e20, BIT7
#define GPIO13_IN  0x0e22, BIT7

#define GPIO14_PAD PAD_GPIO_PM8
#define GPIO14_OEN 0x0e1f, BIT0
#define GPIO14_OUT 0x0e21, BIT0
#define GPIO14_IN  0x0e23, BIT0

#define GPIO15_PAD PAD_GPIO_PM9
#define GPIO15_OEN 0x0e1f, BIT1
#define GPIO15_OUT 0x0e21, BIT1
#define GPIO15_IN  0x0e23, BIT1

#define GPIO16_PAD PAD_GPIO_PM10
#define GPIO16_OEN 0x0e1f, BIT2
#define GPIO16_OUT 0x0e21, BIT2
#define GPIO16_IN  0x0e23, BIT2

#define GPIO17_PAD PAD_GPIO_PM11
#define GPIO17_OEN 0x0e1f, BIT3
#define GPIO17_OUT 0x0e21, BIT3
#define GPIO17_IN  0x0e23, BIT3

#define GPIO18_PAD PAD_GPIO_PM12
#define GPIO18_OEN 0x0e1f, BIT4
#define GPIO18_OUT 0x0e21, BIT4
#define GPIO18_IN  0x0e23, BIT4

#define GPIO19_PAD PAD_HOTPLUGA
#define GPIO19_OEN 0x0e4e, BIT0
#define GPIO19_OUT 0x0e4e, BIT4
#define GPIO19_IN  0x0e4f, BIT0

#define GPIO20_PAD PAD_HOTPLUGB
#define GPIO20_OEN 0x0e4e, BIT1
#define GPIO20_OUT 0x0e4e, BIT5
#define GPIO20_IN  0x0e4f, BIT1

#define GPIO21_PAD PAD_HOTPLUGC
#define GPIO21_OEN 0x0e4e, BIT2
#define GPIO21_OUT 0x0e4e, BIT6
#define GPIO21_IN  0x0e4f, BIT2

#define GPIO22_PAD PAD_HOTPLUGD
#define GPIO22_OEN 0x0e4e, BIT3
#define GPIO22_OUT 0x0e4e, BIT7
#define GPIO22_IN  0x0e4f, BIT3

#define GPIO23_PAD PAD_DDCDA_CK
#define GPIO23_OEN 0x0496, BIT1
#define GPIO23_OUT 0x0496, BIT2
#define GPIO23_IN  0x0496, BIT0

#define GPIO24_PAD PAD_DDCDA_DA
#define GPIO24_OEN 0x0496, BIT5
#define GPIO24_OUT 0x0496, BIT6
#define GPIO24_IN  0x0496, BIT4

#define GPIO25_PAD PAD_DDCDB_CK
#define GPIO25_OEN 0x0497, BIT1
#define GPIO25_OUT 0x0497, BIT2
#define GPIO25_IN  0x0497, BIT0

#define GPIO26_PAD PAD_DDCDB_DA
#define GPIO26_OEN 0x0497, BIT5
#define GPIO26_OUT 0x0497, BIT6
#define GPIO26_IN  0x0497, BIT4

#define GPIO27_PAD PAD_DDCDC_CK
#define GPIO27_OEN 0x0498, BIT1
#define GPIO27_OUT 0x0498, BIT2
#define GPIO27_IN  0x0498, BIT0

#define GPIO28_PAD PAD_DDCDC_DA
#define GPIO28_OEN 0x0498, BIT5
#define GPIO28_OUT 0x0498, BIT6
#define GPIO28_IN  0x0498, BIT4

#define GPIO29_PAD PAD_DDCDD_CK
#define GPIO29_OEN 0x0499, BIT1
#define GPIO29_OUT 0x0499, BIT2
#define GPIO29_IN  0x0499, BIT0

#define GPIO30_PAD PAD_DDCDD_DA
#define GPIO30_OEN 0x0499, BIT5
#define GPIO30_OUT 0x0499, BIT6
#define GPIO30_IN  0x0499, BIT4

#define GPIO31_PAD PAD_SAR0
#define GPIO31_OEN 0x1423, BIT0
#define GPIO31_OUT 0x1424, BIT0
#define GPIO31_IN  0x1425, BIT0

#define GPIO32_PAD PAD_SAR1
#define GPIO32_OEN 0x1423, BIT1
#define GPIO32_OUT 0x1424, BIT1
#define GPIO32_IN  0x1425, BIT1

#define GPIO33_PAD PAD_SAR2
#define GPIO33_OEN 0x1423, BIT2
#define GPIO33_OUT 0x1424, BIT2
#define GPIO33_IN  0x1425, BIT2

#define GPIO34_PAD PAD_SAR3
#define GPIO34_OEN 0x1423, BIT3
#define GPIO34_OUT 0x1424, BIT3
#define GPIO34_IN  0x1425, BIT3

#define GPIO35_PAD PAD_SAR4
#define GPIO35_OEN 0x1423, BIT4
#define GPIO35_OUT 0x1424, BIT4
#define GPIO35_IN  0x1425, BIT4

#define GPIO36_PAD PAD_GPIO0
#define GPIO36_OEN 0x101e5C, BIT0
#define GPIO36_OUT 0x101e56, BIT0
#define GPIO36_IN  0x101e50, BIT0

#define GPIO37_PAD PAD_GPIO1
#define GPIO37_OEN 0x101e5C, BIT1
#define GPIO37_OUT 0x101e56, BIT1
#define GPIO37_IN  0x101e50, BIT1

#define GPIO38_PAD PAD_GPIO2
#define GPIO38_OEN 0x101e5C, BIT2
#define GPIO38_OUT 0x101e56, BIT2
#define GPIO38_IN  0x101e50, BIT2

#define GPIO39_PAD PAD_GPIO3
#define GPIO39_OEN 0x101e5C, BIT3
#define GPIO39_OUT 0x101e56, BIT3
#define GPIO39_IN  0x101e50, BIT3

#define GPIO40_PAD PAD_GPIO4
#define GPIO40_OEN 0x101e5C, BIT4
#define GPIO40_OUT 0x101e56, BIT4
#define GPIO40_IN  0x101e50, BIT4

#define GPIO41_PAD PAD_GPIO5
#define GPIO41_OEN 0x101e5C, BIT5
#define GPIO41_OUT 0x101e56, BIT5
#define GPIO41_IN  0x101e50, BIT5

#define GPIO42_PAD PAD_GPIO6
#define GPIO42_OEN 0x101e5C, BIT6
#define GPIO42_OUT 0x101e56, BIT6
#define GPIO42_IN  0x101e50, BIT6

#define GPIO43_PAD PAD_GPIO7
#define GPIO43_OEN 0x101e5C, BIT7
#define GPIO43_OUT 0x101e56, BIT7
#define GPIO43_IN  0x101e50, BIT7

#define GPIO44_PAD PAD_GPIO8
#define GPIO44_OEN 0x101e5D, BIT0
#define GPIO44_OUT 0x101e57, BIT0
#define GPIO44_IN  0x101e51, BIT0

#define GPIO45_PAD PAD_GPIO9
#define GPIO45_OEN 0x101e5D, BIT1
#define GPIO45_OUT 0x101e57, BIT1
#define GPIO45_IN  0x101e51, BIT1

#define GPIO46_PAD PAD_GPIO10
#define GPIO46_OEN 0x101e5D, BIT2
#define GPIO46_OUT 0x101e57, BIT2
#define GPIO46_IN  0x101e51, BIT2

#define GPIO47_PAD PAD_GPIO11
#define GPIO47_OEN 0x101e5D, BIT3
#define GPIO47_OUT 0x101e57, BIT3
#define GPIO47_IN  0x101e51, BIT3

#define GPIO48_PAD PAD_GPIO12
#define GPIO48_OEN 0x101e5D, BIT4
#define GPIO48_OUT 0x101e57, BIT4
#define GPIO48_IN  0x101e51, BIT4

#define GPIO49_PAD PAD_GPIO13
#define GPIO49_OEN 0x101e5D, BIT5
#define GPIO49_OUT 0x101e57, BIT5
#define GPIO49_IN  0x101e51, BIT5

#define GPIO50_PAD PAD_GPIO14
#define GPIO50_OEN 0x101e5D, BIT6 
#define GPIO50_OUT 0x101e57, BIT6
#define GPIO50_IN  0x101e51, BIT6

#define GPIO51_PAD PAD_GPIO15
#define GPIO51_OEN 0x101e5D, BIT7
#define GPIO51_OUT 0x101e57, BIT7
#define GPIO51_IN  0x101e51, BIT7

#define GPIO52_PAD PAD_GPIO16
#define GPIO52_OEN 0x101e5E, BIT0
#define GPIO52_OUT 0x101e58, BIT0
#define GPIO52_IN  0x101e52, BIT0

#define GPIO53_PAD PAD_GPIO17
#define GPIO53_OEN 0x101e5E, BIT1
#define GPIO53_OUT 0x101e58, BIT1
#define GPIO53_IN  0x101e52, BIT1

#define GPIO54_PAD PAD_GPIO18
#define GPIO54_OEN 0x101e5E, BIT2
#define GPIO54_OUT 0x101e58, BIT2
#define GPIO54_IN  0x101e52, BIT2

#define GPIO55_PAD PAD_GPIO19
#define GPIO55_OEN 0x101e5E, BIT3
#define GPIO55_OUT 0x101e58, BIT3
#define GPIO55_IN  0x101e52, BIT3

#define GPIO56_PAD PAD_GPIO20
#define GPIO56_OEN 0x101e5E, BIT4
#define GPIO56_OUT 0x101e58, BIT4
#define GPIO56_IN  0x101e52, BIT4

#define GPIO57_PAD PAD_GPIO21
#define GPIO57_OEN 0x101e5E, BIT5
#define GPIO57_OUT 0x101e58, BIT5
#define GPIO57_IN  0x101e52, BIT5

#define GPIO58_PAD PAD_GPIO22
#define GPIO58_OEN 0x101e5E, BIT6
#define GPIO58_OUT 0x101e58, BIT6
#define GPIO58_IN  0x101e52, BIT6

#define GPIO59_PAD PAD_GPIO23
#define GPIO59_OEN 0x101e5E, BIT7
#define GPIO59_OUT 0x101e58, BIT7
#define GPIO59_IN  0x101e52, BIT7

#define GPIO60_PAD PAD_GPIO24
#define GPIO60_OEN 0x101e5F, BIT0
#define GPIO60_OUT 0x101e59, BIT0
#define GPIO60_IN  0x101e53, BIT0

#define GPIO61_PAD PAD_GPIO25
#define GPIO61_OEN 0x101e5F, BIT1
#define GPIO61_OUT 0x101e59, BIT1
#define GPIO61_IN  0x101e53, BIT1

#define GPIO62_PAD PAD_GPIO26
#define GPIO62_OEN 0x101e5F, BIT2
#define GPIO62_OUT 0x101e59, BIT2
#define GPIO62_IN  0x101e53, BIT2

#define GPIO63_PAD PAD_GPIO27
#define GPIO63_OEN 0x101e5F, BIT3
#define GPIO63_OUT 0x101e59, BIT3
#define GPIO63_IN  0x101e53, BIT3

#define GPIO64_PAD PAD_UART_RX2
#define GPIO64_OEN 0x101e5F, BIT4
#define GPIO64_OUT 0x101e59, BIT4
#define GPIO64_IN  0x101e53, BIT4

#define GPIO65_PAD PAD_UART_TX2
#define GPIO65_OEN 0x101e5F, BIT5
#define GPIO65_OUT 0x101e59, BIT5
#define GPIO65_IN  0x101e53, BIT5

#define GPIO66_PAD PAD_PWM0
#define GPIO66_OEN 0x101e5F, BIT6
#define GPIO66_OUT 0x101e59, BIT6
#define GPIO66_IN  0x101e53, BIT6

#define GPIO67_PAD PAD_PWM1
#define GPIO67_OEN 0x101e5F, BIT7
#define GPIO67_OUT 0x101e59, BIT7
#define GPIO67_IN  0x101e53, BIT7

#define GPIO68_PAD PAD_PWM2
#define GPIO68_OEN 0x101e60, BIT0
#define GPIO68_OUT 0x101e5A, BIT0
#define GPIO68_IN  0x101e54, BIT0

#define GPIO69_PAD PAD_PWM3
#define GPIO69_OEN 0x101e60, BIT1
#define GPIO69_OUT 0x101e5A, BIT1
#define GPIO69_IN  0x101e54, BIT1

#define GPIO70_PAD PAD_PWM4
#define GPIO70_OEN 0x101e60, BIT2
#define GPIO70_OUT 0x101e5A, BIT2
#define GPIO70_IN  0x101e54, BIT2

#define GPIO71_PAD PAD_DDCR_DA
#define GPIO71_OEN 0x101e60, BIT3
#define GPIO71_OUT 0x101e5A, BIT3
#define GPIO71_IN  0x101e54, BIT3

#define GPIO72_PAD PAD_DDCR_CK
#define GPIO72_OEN 0x101e60, BIT4
#define GPIO72_OUT 0x101e5A, BIT4
#define GPIO72_IN  0x101e54, BIT4

#define GPIO73_PAD PAD_TGPIO0
#define GPIO73_OEN 0x101e60, BIT5
#define GPIO73_OUT 0x101e5A, BIT5
#define GPIO73_IN  0x101e54, BIT5

#define GPIO74_PAD PAD_TGPIO1
#define GPIO74_OEN 0x101e60, BIT6
#define GPIO74_OUT 0x101e5A, BIT6
#define GPIO74_IN  0x101e54, BIT6

#define GPIO75_PAD PAD_TGPIO2
#define GPIO75_OEN 0x101e60, BIT7
#define GPIO75_OUT 0x101e5A, BIT7
#define GPIO75_IN  0x101e54, BIT7

#define GPIO76_PAD PAD_TGPIO3
#define GPIO76_OEN 0x101e61, BIT0
#define GPIO76_OUT 0x101e5B, BIT0
#define GPIO76_IN  0x101e55, BIT0

#define GPIO77_PAD PAD_TS0_D0
#define GPIO77_OEN 0x101e68, BIT0
#define GPIO77_OUT 0x101e64, BIT0
#define GPIO77_IN  0x101e62, BIT0

#define GPIO78_PAD PAD_TS0_D1
#define GPIO78_OEN 0x101e68, BIT1
#define GPIO78_OUT 0x101e64, BIT1
#define GPIO78_IN  0x101e62, BIT1

#define GPIO79_PAD PAD_TS0_D2
#define GPIO79_OEN 0x101e68, BIT2
#define GPIO79_OUT 0x101e64, BIT2
#define GPIO79_IN  0x101e62, BIT2

#define GPIO80_PAD PAD_TS0_D3
#define GPIO80_OEN 0x101e68, BIT3
#define GPIO80_OUT 0x101e64, BIT3
#define GPIO80_IN  0x101e62, BIT3

#define GPIO81_PAD PAD_TS0_D4
#define GPIO81_OEN 0x101e68, BIT4
#define GPIO81_OUT 0x101e64, BIT4
#define GPIO81_IN  0x101e62, BIT4

#define GPIO82_PAD PAD_TS0_D5
#define GPIO82_OEN 0x101e68, BIT5
#define GPIO82_OUT 0x101e64, BIT5
#define GPIO82_IN  0x101e62, BIT5

#define GPIO83_PAD PAD_TS0_D6
#define GPIO83_OEN 0x101e68, BIT6
#define GPIO83_OUT 0x101e64, BIT6
#define GPIO83_IN  0x101e62, BIT6

#define GPIO84_PAD PAD_TS0_D7
#define GPIO84_OEN 0x101e68, BIT7
#define GPIO84_OUT 0x101e64, BIT7
#define GPIO84_IN  0x101e62, BIT7

#define GPIO85_PAD PAD_TS0_VLD
#define GPIO85_OEN 0x101e69, BIT0
#define GPIO85_OUT 0x101e65, BIT0
#define GPIO85_IN  0x101e63, BIT0

#define GPIO86_PAD PAD_TS0_SYNC
#define GPIO86_OEN 0x101e69, BIT1
#define GPIO86_OUT 0x101e65, BIT1
#define GPIO86_IN  0x101e63, BIT1

#define GPIO87_PAD PAD_TS0_CLK
#define GPIO87_OEN 0x101e69, BIT2
#define GPIO87_OUT 0x101e65, BIT2
#define GPIO87_IN  0x101e63, BIT2

#define GPIO88_PAD PAD_TS1_D0
#define GPIO88_OEN 0x101e4E, BIT0
#define GPIO88_OUT 0x101e4C, BIT0
#define GPIO88_IN  0x101e4A, BIT0

#define GPIO89_PAD PAD_TS1_D1
#define GPIO89_OEN 0x101e4E, BIT1
#define GPIO89_OUT 0x101e4C, BIT1
#define GPIO89_IN  0x101e4A, BIT1

#define GPIO90_PAD PAD_TS1_D2
#define GPIO90_OEN 0x101e4E, BIT2
#define GPIO90_OUT 0x101e4C, BIT2
#define GPIO90_IN  0x101e4A, BIT2

#define GPIO91_PAD PAD_TS1_D3
#define GPIO91_OEN 0x101e4E, BIT3
#define GPIO91_OUT 0x101e4C, BIT3
#define GPIO91_IN  0x101e4A, BIT3

#define GPIO92_PAD PAD_TS1_D4
#define GPIO92_OEN 0x101e4E, BIT4
#define GPIO92_OUT 0x101e4C, BIT4
#define GPIO92_IN  0x101e4A, BIT4

#define GPIO93_PAD PAD_TS1_D5
#define GPIO93_OEN 0x101e4E, BIT5
#define GPIO93_OUT 0x101e4C, BIT5
#define GPIO93_IN  0x101e4A, BIT5

#define GPIO94_PAD PAD_TS1_D6
#define GPIO94_OEN 0x101e4E, BIT6
#define GPIO94_OUT 0x101e4C, BIT6
#define GPIO94_IN  0x101e4A, BIT6

#define GPIO95_PAD PAD_TS1_D7
#define GPIO95_OEN 0x101e4E, BIT7
#define GPIO95_OUT 0x101e4C, BIT7
#define GPIO95_IN  0x101e4A, BIT7

#define GPIO96_PAD PAD_TS1_VLD
#define GPIO96_OEN 0x101e4F, BIT0
#define GPIO96_OUT 0x101e4D, BIT0
#define GPIO96_IN  0x101e4B, BIT0

#define GPIO97_PAD PAD_TS1_SYNC
#define GPIO97_OEN 0x101e4F, BIT1
#define GPIO97_OUT 0x101e4D, BIT1
#define GPIO97_IN  0x101e4B, BIT1

#define GPIO98_PAD PAD_TS1_CLK
#define GPIO98_OEN 0x101e4F, BIT2
#define GPIO98_OUT 0x101e4D, BIT2
#define GPIO98_IN  0x101e4B, BIT2

#define GPIO99_PAD PAD_PCM_A4
#if (EN_PCM_GPIO == 1)
#define GPIO99_OEN 0x101e7C, BIT0
#define GPIO99_OUT 0x101e76, BIT0
#define GPIO99_IN  0x101e70, BIT0
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO99_OEN 0x101eB5, BIT7
#define GPIO99_OUT 0x101eB1, BIT7
#define GPIO99_IN  0x101eB9, BIT7
#endif

#define GPIO100_PAD PAD_PCM_WAIT_N
#define GPIO100_OEN 0x101e7C, BIT1
#define GPIO100_OUT 0x101e76, BIT1
#define GPIO100_IN  0x101e70, BIT1

#define GPIO101_PAD PAD_PCM_A5
#if (EN_PCM_GPIO == 1)
#define GPIO101_OEN 0x101e7C, BIT2
#define GPIO101_OUT 0x101e76, BIT2
#define GPIO101_IN  0x101e70, BIT2
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO101_OEN 0x101eB5, BIT6
#define GPIO101_OUT 0x101eB1, BIT6
#define GPIO101_IN  0x101eB9, BIT6
#endif

#define GPIO102_PAD PAD_PCM_A6
#if (EN_PCM_GPIO == 1)
#define GPIO102_OEN 0x101e7C, BIT3
#define GPIO102_OUT 0x101e76, BIT3
#define GPIO102_IN  0x101e70, BIT3
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO102_OEN 0x101eB5, BIT5
#define GPIO102_OUT 0x101eB1, BIT5
#define GPIO102_IN  0x101eB9, BIT5
#endif

#define GPIO103_PAD PAD_PCM_A7
#if (EN_PCM_GPIO == 1)
#define GPIO103_OEN 0x101e7C, BIT4
#define GPIO103_OUT 0x101e76, BIT4
#define GPIO103_IN  0x101e70, BIT4
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO103_OEN 0x101eB5, BIT4
#define GPIO103_OUT 0x101eB1, BIT4
#define GPIO103_IN  0x101eB9, BIT4
#endif

#define GPIO104_PAD PAD_PCM_A12
#if (EN_PCM_GPIO == 1)
#define GPIO104_OEN 0x101e7C, BIT5
#define GPIO104_OUT 0x101e76, BIT5
#define GPIO104_IN  0x101e70, BIT5
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO104_OEN 0x101eB5, BIT3
#define GPIO104_OUT 0x101eB1, BIT3
#define GPIO104_IN  0x101eB9, BIT3
#endif

#define GPIO105_PAD PAD_PCM_IRQA_N
#define GPIO105_OEN 0x101e7C, BIT6
#define GPIO105_OUT 0x101e76, BIT6
#define GPIO105_IN  0x101e70, BIT6

#define GPIO106_PAD PAD_PCM_A14
#if (EN_PCM_GPIO == 1)
#define GPIO106_OEN 0x101e7C, BIT7
#define GPIO106_OUT 0x101e76, BIT7
#define GPIO106_IN  0x101e70, BIT7
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO106_OEN 0x101eB5, BIT2
#define GPIO106_OUT 0x101eB1, BIT2
#define GPIO106_IN  0x101eB9, BIT2
#endif

#define GPIO107_PAD PAD_PCM_A13
#if (EN_PCM_GPIO == 1)
#define GPIO107_OEN 0x101e7D, BIT0
#define GPIO107_OUT 0x101e77, BIT0
#define GPIO107_IN  0x101e71, BIT0
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO107_OEN 0x101eB5, BIT1
#define GPIO107_OUT 0x101eB1, BIT1
#define GPIO107_IN  0x101eB9, BIT1
#endif

#define GPIO108_PAD PAD_PCM_A8
#if (EN_PCM_GPIO == 1)
#define GPIO108_OEN 0x101e7D, BIT1
#define GPIO108_OUT 0x101e77, BIT1
#define GPIO108_IN  0x101e71, BIT1
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO108_OEN 0x101eB5, BIT0
#define GPIO108_OUT 0x101eB1, BIT0
#define GPIO108_IN  0x101eB9, BIT0
#endif

#define GPIO109_PAD PAD_PCM_IOWR_N
#define GPIO109_OEN 0x101e7D, BIT2
#define GPIO109_OUT 0x101e77, BIT2
#define GPIO109_IN  0x101e71, BIT2

#define GPIO110_PAD PAD_PCM_A9
#if (EN_PCM_GPIO == 1)
#define GPIO110_OEN 0x101e7D, BIT3
#define GPIO110_OUT 0x101e77, BIT3
#define GPIO110_IN  0x101e71, BIT3
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO110_OEN 0x101eB4, BIT7
#define GPIO110_OUT 0x101eB0, BIT7
#define GPIO110_IN  0x101eB8, BIT7
#endif

#define GPIO111_PAD PAD_PCM_IORD_N
#define GPIO111_OEN 0x101e7D, BIT4
#define GPIO111_OUT 0x101e77, BIT4
#define GPIO111_IN  0x101e71, BIT4

#define GPIO112_PAD PAD_PCM_A11
#if (EN_PCM_GPIO == 1)
#define GPIO112_OEN 0x101e7D, BIT5
#define GPIO112_OUT 0x101e77, BIT5
#define GPIO112_IN  0x101e71, BIT5
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO112_OEN 0x101eB4, BIT6
#define GPIO112_OUT 0x101eB0, BIT6
#define GPIO112_IN  0x101eB8, BIT6
#endif

#define GPIO113_PAD PAD_PCM_OE_N
#define GPIO113_OEN 0x101e7D, BIT6
#define GPIO113_OUT 0x101e77, BIT6
#define GPIO113_IN  0x101e71, BIT6

#define GPIO114_PAD PAD_PCM_A10
#if (EN_PCM_GPIO == 1)
#define GPIO114_OEN 0x101e7D, BIT7
#define GPIO114_OUT 0x101e77, BIT7
#define GPIO114_IN  0x101e71, BIT7
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO114_OEN 0x101eB4, BIT5
#define GPIO114_OUT 0x101eB0, BIT5
#define GPIO114_IN  0x101eB8, BIT5
#endif

#define GPIO115_PAD PAD_PCM_CE_N
#define GPIO115_OEN 0x101e7E, BIT0
#define GPIO115_OUT 0x101e78, BIT0
#define GPIO115_IN  0x101e72, BIT0

#define GPIO116_PAD PAD_PCM_D7
#if (EN_PCM_GPIO == 1)
#define GPIO116_OEN 0x101e7e, BIT1
#define GPIO116_OUT 0x101e78, BIT1
#define GPIO116_IN  0x101e72, BIT1
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO116_OEN 0x101eB4, BIT4
#define GPIO116_OUT 0x101eB0, BIT4
#define GPIO116_IN  0x101eB8, BIT4
#endif

#define GPIO117_PAD PAD_PCM_D6
#if (EN_PCM_GPIO == 1)
#define GPIO117_OEN 0x101e7e, BIT3
#define GPIO117_OUT 0x101e78, BIT3
#define GPIO117_IN  0x101e72, BIT3
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO117_OEN 0x101eB4, BIT3
#define GPIO117_OUT 0x101eB0, BIT3
#define GPIO117_IN  0x101eB8, BIT3
#endif

#define GPIO118_PAD PAD_PCM_D5
#if (EN_PCM_GPIO == 1)
#define GPIO118_OEN 0x101e7e, BIT4
#define GPIO118_OUT 0x101e78, BIT4
#define GPIO118_IN  0x101e72, BIT4
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO118_OEN 0x101eB4, BIT2
#define GPIO118_OUT 0x101eB0, BIT2
#define GPIO118_IN  0x101eB8, BIT2
#endif

#define GPIO119_PAD PAD_PCM_D4
#if (EN_PCM_GPIO == 1)
#define GPIO119_OEN 0x101e7e, BIT5
#define GPIO119_OUT 0x101e78, BIT5
#define GPIO119_IN  0x101e72, BIT5
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO119_OEN 0x101eB4, BIT1
#define GPIO119_OUT 0x101eB0, BIT1
#define GPIO119_IN  0x101eB8, BIT1
#endif

#define GPIO120_PAD PAD_PCM_D3
#if (EN_PCM_GPIO == 1)
#define GPIO120_OEN 0x101e7e, BIT6
#define GPIO120_OUT 0x101e78, BIT6
#define GPIO120_IN  0x101e72, BIT6
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO120_OEN 0x101eB4, BIT0
#define GPIO120_OUT 0x101eB0, BIT0
#define GPIO120_IN  0x101eB8, BIT0
#endif

#define GPIO121_PAD PAD_PCM_A3
#if (EN_PCM_GPIO == 1)
#define GPIO121_OEN 0x101e7e, BIT7
#define GPIO121_OUT 0x101e78, BIT7
#define GPIO121_IN  0x101e72, BIT7
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO121_OEN 0x101eB6, BIT6
#define GPIO121_OUT 0x101eB2, BIT6
#define GPIO121_IN  0x101eBA, BIT6
#endif

#define GPIO122_PAD PAD_PCM_A2
#if (EN_PCM_GPIO == 1)
#define GPIO122_OEN 0x101e7F, BIT0
#define GPIO122_OUT 0x101e79, BIT0
#define GPIO122_IN  0x101e73, BIT0
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO122_OEN 0x101eB6, BIT7
#define GPIO122_OUT 0x101eB2, BIT7
#define GPIO122_IN  0x101eBA, BIT7
#endif

#define GPIO123_PAD PAD_PCM_REG_N
#define GPIO123_OEN 0x101e7F, BIT1
#define GPIO123_OUT 0x101e79, BIT1
#define GPIO123_IN  0x101e73, BIT1

#define GPIO124_PAD PAD_PCM_A1
#if (EN_PCM_GPIO == 1)
#define GPIO124_OEN 0x101e7F, BIT2
#define GPIO124_OUT 0x101e79, BIT2
#define GPIO124_IN  0x101e73, BIT2
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO124_OEN 0x101eB7, BIT0
#define GPIO124_OUT 0x101eB3, BIT0
#define GPIO124_IN  0x101eBB, BIT0
#endif

#define GPIO125_PAD PAD_PCM_A0
#if (EN_PCM_GPIO == 1)
#define GPIO125_OEN 0x101e7F, BIT3
#define GPIO125_OUT 0x101e79, BIT3
#define GPIO125_IN  0x101e73, BIT3
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO125_OEN 0x101eB7, BIT1
#define GPIO125_OUT 0x101eB3, BIT1
#define GPIO125_IN  0x101eBB, BIT1
#endif

#define GPIO126_PAD PAD_PCM_D0
#if (EN_PCM_GPIO == 1)
#define GPIO126_OEN 0x101e7F, BIT4
#define GPIO126_OUT 0x101e79, BIT4
#define GPIO126_IN  0x101e73, BIT4
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO126_OEN 0x101eB7, BIT2
#define GPIO126_OUT 0x101eB3, BIT2
#define GPIO126_IN  0x101eBB, BIT2
#endif

#define GPIO127_PAD PAD_PCM_D1
#if (EN_PCM_GPIO == 1)
#define GPIO127_OEN 0x101e7F, BIT5
#define GPIO127_OUT 0x101e79, BIT5
#define GPIO127_IN  0x101e73, BIT5
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO127_OEN 0x101eB7, BIT3
#define GPIO127_OUT 0x101eB3, BIT3
#define GPIO127_IN  0x101eBB, BIT3
#endif

#define GPIO128_PAD PAD_PCM_D2
#if (EN_PCM_GPIO == 1)
#define GPIO128_OEN 0x101e7F, BIT6
#define GPIO128_OUT 0x101e79, BIT6
#define GPIO128_IN  0x101e73, BIT6
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO128_OEN 0x101eB7, BIT4
#define GPIO128_OUT 0x101eB3, BIT4
#define GPIO128_IN  0x101eBB, BIT4
#endif

#define GPIO129_PAD PAD_PCM_RESET
#define GPIO129_OEN 0x101e7F, BIT7
#define GPIO129_OUT 0x101e79, BIT7
#define GPIO129_IN  0x101e73, BIT7

#define GPIO130_PAD PAD_PCM_CD_N
#define GPIO130_OEN 0x101e80, BIT0
#define GPIO130_OUT 0x101e7A, BIT0
#define GPIO130_IN  0x101e74, BIT0

#define GPIO131_PAD PAD_PCM2_CE_N
#define GPIO131_OEN 0x101e81, BIT0
#define GPIO131_OUT 0x101e7B, BIT0
#define GPIO131_IN  0x101e75, BIT0

#define GPIO132_PAD PAD_PCM2_IRQA_N
#define GPIO132_OEN 0x101e81, BIT1
#define GPIO132_OUT 0x101e7B, BIT1
#define GPIO132_IN  0x101e75, BIT1

#define GPIO133_PAD PAD_PCM2_WAIT_N
#define GPIO133_OEN 0x101e81, BIT2
#define GPIO133_OUT 0x101e7B, BIT2
#define GPIO133_IN  0x101e75, BIT2

#define GPIO134_PAD PAD_PCM2_RESET
#define GPIO134_OEN 0x101e81, BIT3
#define GPIO134_OUT 0x101e7B, BIT3
#define GPIO134_IN  0x101e75, BIT3

#define GPIO135_PAD PAD_PCM2_CD_N
#define GPIO135_OEN 0x101e81, BIT4
#define GPIO135_OUT 0x101e7B, BIT4
#define GPIO135_IN  0x101e75, BIT4

#define GPIO136_PAD PAD_PF_AD15
#if (EN_PF_GPIO == 1)
#define GPIO136_OEN 0x101e8C, BIT0
#define GPIO136_OUT 0x101e8A, BIT0
#define GPIO136_IN  0x101e88, BIT0
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO136_OEN 0x101eB6, BIT0
#define GPIO136_OUT 0x101eB2, BIT0
#define GPIO136_IN  0x101eBA, BIT0
#endif

#define GPIO137_PAD PAD_PF_CE0Z
#if (EN_PF_GPIO == 1)
#define GPIO137_OEN 0x101e8C, BIT1
#define GPIO137_OUT 0x101e8A, BIT1
#define GPIO137_IN  0x101e88, BIT1
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO137_OEN 0x101eB6, BIT1
#define GPIO137_OUT 0x101eB2, BIT1
#define GPIO137_IN  0x101eBA, BIT1
#endif

#define GPIO138_PAD PAD_PF_CE1Z
#if (EN_PF_GPIO == 1)
#define GPIO138_OEN 0x101e8C, BIT2
#define GPIO138_OUT 0x101e8A, BIT2
#define GPIO138_IN  0x101e88, BIT2
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO138_OEN 0x101eB6, BIT2
#define GPIO138_OUT 0x101eB2, BIT2
#define GPIO138_IN  0x101eBA, BIT2
#endif

#define GPIO139_PAD PAD_PF_OEZ
#if (EN_PF_GPIO == 1)
#define GPIO139_OEN 0x101e8C, BIT3
#define GPIO139_OUT 0x101e8A, BIT3
#define GPIO139_IN  0x101e88, BIT3
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO139_OEN 0x101eB6, BIT3
#define GPIO139_OUT 0x101eB2, BIT3
#define GPIO139_IN  0x101eBA, BIT3
#endif

#define GPIO140_PAD PAD_PF_WEZ
#if (EN_PF_GPIO == 1)
#define GPIO140_OEN 0x101e8C, BIT4
#define GPIO140_OUT 0x101e8A, BIT4
#define GPIO140_IN  0x101e88, BIT4
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO140_OEN 0x101eB6, BIT4
#define GPIO140_OUT 0x101eB2, BIT4
#define GPIO140_IN  0x101eBA, BIT4
#endif

#define GPIO141_PAD PAD_PF_ALE
#if (EN_PF_GPIO == 1)
#define GPIO141_OEN 0x101e8C, BIT5
#define GPIO141_OUT 0x101e8A, BIT5
#define GPIO141_IN  0x101e88, BIT5
#endif

#if (EN_NOR_GPIO == 1)
#define GPIO141_OEN 0x101eB6, BIT5
#define GPIO141_OUT 0x101eB2, BIT5
#define GPIO141_IN  0x101eBA, BIT5
#endif

#define GPIO142_PAD PAD_F_RBZ
#define GPIO142_OEN 0x101e8C, BIT6
#define GPIO142_OUT 0x101e8A, BIT6
#define GPIO142_IN  0x101e88, BIT6

#define GPIO143_PAD PAD_TCON0
#define GPIO143_OEN 0x101e9C, BIT0
#define GPIO143_OUT 0x101e98, BIT0
#define GPIO143_IN  0x101e94, BIT0

#define GPIO144_PAD PAD_TCON1
#define GPIO144_OEN 0x101e9C, BIT1
#define GPIO144_OUT 0x101e98, BIT1
#define GPIO144_IN  0x101e94, BIT1

#define GPIO145_PAD PAD_TCON2
#define GPIO145_OEN 0x101e9C, BIT2
#define GPIO145_OUT 0x101e98, BIT2
#define GPIO145_IN  0x101e94, BIT2

#define GPIO146_PAD PAD_TCON3
#define GPIO146_OEN 0x101e9C, BIT3
#define GPIO146_OUT 0x101e98, BIT3
#define GPIO146_IN  0x101e94, BIT3

#define GPIO147_PAD PAD_TCON4
#define GPIO147_OEN 0x101e9C, BIT4
#define GPIO147_OUT 0x101e98, BIT4
#define GPIO147_IN  0x101e94, BIT4

#define GPIO148_PAD PAD_TCON5
#define GPIO148_OEN 0x101e9C, BIT5
#define GPIO148_OUT 0x101e98, BIT5
#define GPIO148_IN  0x101e94, BIT5

#define GPIO149_PAD PAD_TCON6
#define GPIO149_OEN 0x101e9C, BIT6
#define GPIO149_OUT 0x101e98, BIT6
#define GPIO149_IN  0x101e94, BIT6

#define GPIO150_PAD PAD_TCON7
#define GPIO150_OEN 0x101e9C, BIT7
#define GPIO150_OUT 0x101e98, BIT7
#define GPIO150_IN  0x101e94, BIT7

#define GPIO151_PAD PAD_TCON8
#define GPIO151_OEN 0x101e9D, BIT0
#define GPIO151_OUT 0x101e99, BIT0
#define GPIO151_IN  0x101e95, BIT0

#define GPIO152_PAD PAD_TCON9
#define GPIO152_OEN 0x101e9D, BIT1
#define GPIO152_OUT 0x101e99, BIT1
#define GPIO152_IN  0x101e95, BIT1

#define GPIO153_PAD PAD_TCON10
#define GPIO153_OEN 0x101e9D, BIT2
#define GPIO153_OUT 0x101e99, BIT2
#define GPIO153_IN  0x101e95, BIT2

#define GPIO154_PAD PAD_TCON11
#define GPIO154_OEN 0x101e9D, BIT3
#define GPIO154_OUT 0x101e99, BIT3
#define GPIO154_IN  0x101e95, BIT3

#define GPIO155_PAD PAD_TCON12
#define GPIO155_OEN 0x101e9D, BIT4
#define GPIO155_OUT 0x101e99, BIT4
#define GPIO155_IN  0x101e95, BIT4

#define GPIO156_PAD PAD_TCON13
#define GPIO156_OEN 0x101e9D, BIT5
#define GPIO156_OUT 0x101e99, BIT5
#define GPIO156_IN  0x101e95, BIT5

#define GPIO157_PAD PAD_TCON14
#define GPIO157_OEN 0x101e9D, BIT6
#define GPIO157_OUT 0x101e99, BIT6
#define GPIO157_IN  0x101e95, BIT6

#define GPIO158_PAD PAD_TCON15
#define GPIO158_OEN 0x101e9D, BIT7
#define GPIO158_OUT 0x101e99, BIT7
#define GPIO158_IN  0x101e95, BIT7

#define GPIO159_PAD PAD_TCON16
#define GPIO159_OEN 0x101e9B, BIT0
#define GPIO159_OUT 0x101e9A, BIT0
#define GPIO159_IN  0x101e96, BIT0

#define GPIO160_PAD PAD_TCON17
#define GPIO160_OEN 0x101e9B, BIT1
#define GPIO160_OUT 0x101e9A, BIT1
#define GPIO160_IN  0x101e96, BIT1

#define GPIO161_PAD PAD_TCON18
#define GPIO161_OEN 0x101e9B, BIT2
#define GPIO161_OUT 0x101e9A, BIT2
#define GPIO161_IN  0x101e96, BIT2

#define GPIO162_PAD PAD_TCON19
#define GPIO162_OEN 0x101e9B, BIT3
#define GPIO162_OUT 0x101e9A, BIT3
#define GPIO162_IN  0x101e96, BIT3

#define GPIO163_PAD PAD_TCON20
#define GPIO163_OEN 0x101e9B, BIT4
#define GPIO163_OUT 0x101e9A, BIT4
#define GPIO163_IN  0x101e96, BIT4

#define GPIO164_PAD PAD_TCON21
#define GPIO164_OEN 0x101e9B, BIT5
#define GPIO164_OUT 0x101e9A, BIT5
#define GPIO164_IN  0x101e96, BIT5

#define GPIO165_PAD PAD_ET_COL
#define GPIO165_OEN 0x101e86, BIT0
#define GPIO165_OUT 0x101e84, BIT0
#define GPIO165_IN  0x101e82, BIT0

#define GPIO166_PAD PAD_ET_TXD1
#define GPIO166_OEN 0x101e86, BIT1
#define GPIO166_OUT 0x101e84, BIT1
#define GPIO166_IN  0x101e82, BIT1

#define GPIO167_PAD PAD_ET_TXD0
#define GPIO167_OEN 0x101e86, BIT2
#define GPIO167_OUT 0x101e84, BIT2
#define GPIO167_IN  0x101e82, BIT2

#define GPIO168_PAD PAD_ET_TX_EN
#define GPIO168_OEN 0x101e86, BIT3
#define GPIO168_OUT 0x101e84, BIT3
#define GPIO168_IN  0x101e82, BIT3

#define GPIO169_PAD PAD_ET_TX_CLK
#define GPIO169_OEN 0x101e86, BIT4
#define GPIO169_OUT 0x101e84, BIT4
#define GPIO169_IN  0x101e82, BIT4

#define GPIO170_PAD PAD_ET_RXD0
#define GPIO170_OEN 0x101e86, BIT5
#define GPIO170_OUT 0x101e84, BIT5
#define GPIO170_IN  0x101e82, BIT5

#define GPIO171_PAD PAD_ET_RXD1
#define GPIO171_OEN 0x101e86, BIT6
#define GPIO171_OUT 0x101e84, BIT6
#define GPIO171_IN  0x101e82, BIT6

#define GPIO172_PAD PAD_ET_MDC
#define GPIO172_OEN 0x101e86, BIT7
#define GPIO172_OUT 0x101e84, BIT7
#define GPIO172_IN  0x101e82, BIT7

#define GPIO173_PAD PAD_ET_MDIO
#define GPIO173_OEN 0x101e87, BIT0
#define GPIO173_OUT 0x101e85, BIT0
#define GPIO173_IN  0x101e83, BIT0

#define GPIO174_PAD PAD_I2S_IN_WS
#define GPIO174_OEN 0x101e6E, BIT0
#define GPIO174_OUT 0x101e6C, BIT0
#define GPIO174_IN  0x101e6A, BIT0

#define GPIO175_PAD PAD_I2S_IN_BCK
#define GPIO175_OEN 0x101e6E, BIT1
#define GPIO175_OUT 0x101e6C, BIT1
#define GPIO175_IN  0x101e6A, BIT1

#define GPIO176_PAD PAD_I2S_IN_SD
#define GPIO176_OEN 0x101e6E, BIT2
#define GPIO176_OUT 0x101e6C, BIT2
#define GPIO176_IN  0x101e6A, BIT2

#define GPIO177_PAD PAD_SPDIF_IN
#define GPIO177_OEN 0x101e6E, BIT3
#define GPIO177_OUT 0x101e6C, BIT3
#define GPIO177_IN  0x101e6A, BIT3

#define GPIO178_PAD PAD_SPDIF_OUT
#define GPIO178_OEN 0x101e6E, BIT4
#define GPIO178_OUT 0x101e6C, BIT4
#define GPIO178_IN  0x101e6A, BIT4

#define GPIO179_PAD PAD_I2S_OUT_MCK
#define GPIO179_OEN 0x101e6E, BIT5
#define GPIO179_OUT 0x101e6C, BIT5
#define GPIO179_IN  0x101e6A, BIT5

#define GPIO180_PAD PAD_I2S_OUT_WS
#define GPIO180_OEN 0x101e6E, BIT6
#define GPIO180_OUT 0x101e6C, BIT6
#define GPIO180_IN  0x101e6A, BIT6

#define GPIO181_PAD PAD_I2S_OUT_BCK
#define GPIO181_OEN 0x101e6E, BIT7
#define GPIO181_OUT 0x101e6C, BIT7
#define GPIO181_IN  0x101e6A, BIT7

#define GPIO182_PAD PAD_I2S_OUT_SD
#define GPIO182_OEN 0x101e6F, BIT0
#define GPIO182_OUT 0x101e6D, BIT0
#define GPIO182_IN  0x101e6B, BIT0

#define GPIO183_PAD PAD_I2S_OUT_SD1
#define GPIO183_OEN 0x101e6F, BIT1
#define GPIO183_OUT 0x101e6D, BIT1
#define GPIO183_IN  0x101e6B, BIT1

#define GPIO184_PAD PAD_I2S_OUT_SD2
#define GPIO184_OEN 0x101e6F, BIT2
#define GPIO184_OUT 0x101e6D, BIT2
#define GPIO184_IN  0x101e6B, BIT2

#define GPIO185_PAD PAD_I2S_OUT_SD3
#define GPIO185_OEN 0x101e6F, BIT3
#define GPIO185_OUT 0x101e6D, BIT3
#define GPIO185_IN  0x101e6B, BIT3

#define GPIO186_PAD PAD_B_ODD0
#define GPIO186_OEN 0x1032fc, BIT3
#define GPIO186_OUT 0x1032fc, BIT7
#define GPIO186_IN  0x1032fe, BIT3

#define GPIO187_PAD PAD_B_ODD1
#define GPIO187_OEN 0x1032fc, BIT2
#define GPIO187_OUT 0x1032fc, BIT6
#define GPIO187_IN  0x1032fe, BIT2

#define GPIO188_PAD PAD_B_ODD2
#define GPIO188_OEN 0x1032fc, BIT1
#define GPIO188_OUT 0x1032fc, BIT5
#define GPIO188_IN  0x1032fe, BIT1

#define GPIO189_PAD PAD_B_ODD3
#define GPIO189_OEN 0x1032fc, BIT0
#define GPIO189_OUT 0x1032fc, BIT4
#define GPIO189_IN  0x1032fe, BIT0

#define GPIO190_PAD PAD_B_ODD4
#define GPIO190_OEN 0x1032a5, BIT7
#define GPIO190_OUT 0x1032a1, BIT7
#define GPIO190_IN  0x1032ad, BIT7

#define GPIO191_PAD PAD_B_ODD5
#define GPIO191_OEN 0x1032a5, BIT6
#define GPIO191_OUT 0x1032a1, BIT6
#define GPIO191_IN  0x1032ad, BIT6

#define GPIO192_PAD PAD_B_ODD6
#define GPIO192_OEN 0x1032a5, BIT5
#define GPIO192_OUT 0x1032a1, BIT5
#define GPIO192_IN  0x1032ad, BIT5

#define GPIO193_PAD PAD_B_ODD7
#define GPIO193_OEN 0x1032a5, BIT4
#define GPIO193_OUT 0x1032a1, BIT4
#define GPIO193_IN  0x1032ad, BIT4

#define GPIO194_PAD PAD_G_ODD0
#define GPIO194_OEN 0x1032a5, BIT3
#define GPIO194_OUT 0x1032a1, BIT3
#define GPIO194_IN  0x1032ad, BIT3

#define GPIO195_PAD PAD_G_ODD1
#define GPIO195_OEN 0x1032a5, BIT2
#define GPIO195_OUT 0x1032a1, BIT2
#define GPIO195_IN  0x1032ad, BIT2

#define GPIO196_PAD PAD_G_ODD2
#define GPIO196_OEN 0x1032a5, BIT1
#define GPIO196_OUT 0x1032a1, BIT1
#define GPIO196_IN  0x1032ad, BIT1

#define GPIO197_PAD PAD_G_ODD3
#define GPIO197_OEN 0x1032a5, BIT0
#define GPIO197_OUT 0x1032a1, BIT0
#define GPIO197_IN  0x1032ad, BIT0

#define GPIO198_PAD PAD_G_ODD4
#define GPIO198_OEN 0x1032a4, BIT7
#define GPIO198_OUT 0x1032a0, BIT7
#define GPIO198_IN  0x1032ac, BIT7

#define GPIO199_PAD PAD_G_ODD5
#define GPIO199_OEN 0x1032a4, BIT6
#define GPIO199_OUT 0x1032a0, BIT6
#define GPIO199_IN  0x1032ac, BIT6

#define GPIO200_PAD PAD_G_ODD6
#define GPIO200_OEN 0x1032a4, BIT5
#define GPIO200_OUT 0x1032a0, BIT5
#define GPIO200_IN  0x1032ac, BIT5

#define GPIO201_PAD PAD_G_ODD7
#define GPIO201_OEN 0x1032a4, BIT4
#define GPIO201_OUT 0x1032a0, BIT4
#define GPIO201_IN  0x1032ac, BIT4

#define GPIO202_PAD PAD_R_ODD0
#define GPIO202_OEN 0x1032a4, BIT3
#define GPIO202_OUT 0x1032a0, BIT3
#define GPIO202_IN  0x1032ac, BIT3

#define GPIO203_PAD PAD_R_ODD1
#define GPIO203_OEN 0x1032a4, BIT2
#define GPIO203_OUT 0x1032a0, BIT2
#define GPIO203_IN  0x1032ac, BIT2

#define GPIO204_PAD PAD_R_ODD2
#define GPIO204_OEN 0x1032a4, BIT1
#define GPIO204_OUT 0x1032a0, BIT1
#define GPIO204_IN  0x1032ac, BIT1

#define GPIO205_PAD PAD_R_ODD3
#define GPIO205_OEN 0x1032a4, BIT0
#define GPIO205_OUT 0x1032a0, BIT0
#define GPIO205_IN  0x1032ac, BIT0

#define GPIO206_PAD PAD_R_ODD4
#define GPIO206_OEN 0x1032a3, BIT7
#define GPIO206_OUT 0x10329f, BIT7
#define GPIO206_IN  0x1032ab, BIT7

#define GPIO207_PAD PAD_R_ODD5
#define GPIO207_OEN 0x1032a3, BIT6
#define GPIO207_OUT 0x10329f, BIT6
#define GPIO207_IN  0x1032ab, BIT6

#define GPIO208_PAD PAD_R_ODD6
#define GPIO208_OEN 0x1032a3, BIT5
#define GPIO208_OUT 0x10329f, BIT5
#define GPIO208_IN  0x1032ab, BIT5

#define GPIO209_PAD PAD_R_ODD7
#define GPIO209_OEN 0x1032a3, BIT4
#define GPIO209_OUT 0x10329f, BIT4
#define GPIO209_IN  0x1032ab, BIT4

#define GPIO210_PAD PAD_mini_LVDS0
#define GPIO210_OEN 0x1032a3, BIT3
#define GPIO210_OUT 0x10329f, BIT3
#define GPIO210_IN  0x1032ab, BIT3

#define GPIO211_PAD PAD_mini_LVDS1
#define GPIO211_OEN 0x1032a3, BIT2
#define GPIO211_OUT 0x10329f, BIT2
#define GPIO211_IN  0x1032ab, BIT2

#define GPIO212_PAD PAD_mini_LVDS2
#define GPIO212_OEN 0x1032a3, BIT1
#define GPIO212_OUT 0x10329f, BIT1
#define GPIO212_IN  0x1032ab, BIT1

#define GPIO213_PAD PAD_mini_LVDS3
#define GPIO213_OEN 0x1032a3, BIT0
#define GPIO213_OUT 0x10329f, BIT0
#define GPIO213_IN  0x1032ab, BIT0

#define GPIO214_PAD PAD_mini_LVDS4
#define GPIO214_OEN 0x1032a2, BIT7
#define GPIO214_OUT 0x10329e, BIT7
#define GPIO214_IN  0x1032aa, BIT7

#define GPIO215_PAD PAD_mini_LVDS5
#define GPIO215_OEN 0x1032a2, BIT6
#define GPIO215_OUT 0x10329e, BIT6
#define GPIO215_IN  0x1032aa, BIT6

#define GPIO216_PAD PAD_mini_LVDS6
#define GPIO216_OEN 0x1032a2, BIT5
#define GPIO216_OUT 0x10329e, BIT5
#define GPIO216_IN  0x1032aa, BIT5

#define GPIO217_PAD PAD_mini_LVDS7
#define GPIO217_OEN 0x1032a2, BIT4
#define GPIO217_OUT 0x10329e, BIT4
#define GPIO217_IN  0x1032aa, BIT4

#define GPIO218_PAD PAD_LCK
#define GPIO218_OEN 0x1032a2, BIT3
#define GPIO218_OUT 0x10329e, BIT3
#define GPIO218_IN  0x1032aa, BIT3

#define GPIO219_PAD PAD_LDE
#define GPIO219_OEN 0x1032a2, BIT2
#define GPIO219_OUT 0x10329e, BIT2
#define GPIO219_IN  0x1032aa, BIT2

#define GPIO220_PAD PAD_LHSYNC
#define GPIO220_OEN 0x1032a2, BIT1
#define GPIO220_OUT 0x10329e, BIT1
#define GPIO220_IN  0x1032aa, BIT1

#define GPIO221_PAD PAD_LVSYNC
#define GPIO221_OEN 0x1032a2, BIT0
#define GPIO221_OUT 0x10329e, BIT0
#define GPIO221_IN  0x1032aa, BIT0

#define GPIO222_PAD PAD_PCM_WE_N
#define GPIO222_OEN 0x101e7E, BIT2
#define GPIO222_OUT 0x101e78, BIT2
#define GPIO222_IN  0x101e72, BIT0

#define GPIO223_PAD PAD_ONE_RPN
#define GPIO223_OEN 0x101e8c, BIT7
#define GPIO223_OUT 0x101e8a, BIT7
#define GPIO223_IN  0x101e88, BIT7

#define GPIO224_PAD PAD_ONE_RDY
#define GPIO224_OEN 0x101e8d, BIT0
#define GPIO224_OUT 0x101e8b, BIT0
#define GPIO224_IN  0x101e89, BIT0

#define GPIO225_PAD PAD_PWM_PM
#define GPIO225_OEN 0x0e3c, BIT1
#define GPIO225_OUT 0x0e3a, BIT1
#define GPIO225_IN  0x0e3b, BIT1

#define GPIO226_PAD PAD_AV_LNK
#define GPIO226_OEN 0x0e3c, BIT3
#define GPIO226_OUT 0x0e3a, BIT3
#define GPIO226_IN  0x0e3b, BIT3

#define GPIO227_PAD PAD_DDCA_CK
#define GPIO227_OEN 0x000494, BIT1
#define GPIO227_OUT 0x000494, BIT2
#define GPIO227_IN  0x000494, BIT0

#define GPIO228_PAD PAD_DDCA_DA
#define GPIO228_OEN 0x000494, BIT5
#define GPIO228_OUT 0x000494, BIT6
#define GPIO228_IN  0x000494, BIT4


//----------------------------------------------------------------------------
// GPIO dispatcher
//----------------------------------------------------------------------------

#define GPIO_NUM(pin)       CONCAT(GPIO_PAD_, pin)

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define RIU_MAP 0xBF000000


#define RIU     ((unsigned short volatile *) RIU_MAP)
#define RIU8    ((unsigned char  volatile *) RIU_MAP)

#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

#define MDrv_ReadRegBit( u32Reg, u8Mask )                                               \
        (RIU8[(u32Reg) * 2 - ((u32Reg) & 1)] & (u8Mask))

#define MDrv_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    MST_MACRO_START                                                                     \
    MS_U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                   \
    RIU8[u32Reg8] = (bEnable) ? (RIU8[u32Reg8] |  (u8Mask)) :                           \
                                (RIU8[u32Reg8] & ~(u8Mask));                            \
    MST_MACRO_END

#define MDrv_WriteByte( u32Reg, u8Val )                                                 \
    MST_MACRO_START                                                                     \
    RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;                                      \
    MST_MACRO_END

#define MDrv_ReadByte( u32Reg )                                                         \
        (RIU8[(u32Reg) * 2 - ((u32Reg) & 1)])

#define MDrv_Write2Byte( u32Reg, u16Val )                                               \
    MST_MACRO_START                                                                     \
    if ((u32Reg) & 0x01)                                                                \
    {                                                                                   \
        RIU8[((u32Reg) * 2) - 1] = (MS_U8)((u16Val));                                   \
        RIU8[((u32Reg) + 1) * 2] = (MS_U8)((u16Val) >> 8);                              \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        RIU[u32Reg] = u16Val;                                                           \
    }                                                                                   \
    MST_MACRO_END

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


