

#if defined(CONFIG_ARM)
	#define A_RIU_PM_BASE                     (IO_ADDRESS(0x1F000000UL))
	#define A_RIU_BASE                        (IO_ADDRESS(0x1F200000UL))
#elif defined(CONFIG_ARM64)
	extern ptrdiff_t   mstar_pm_base;
	#define A_RIU_PM_BASE                     ((uintptr_t)(mstar_pm_base))
	#define A_RIU_BASE                        ((uintptr_t)(mstar_pm_base+0x200000))
#endif

//#define A_FCIE2_0_BANK		GET_CARD_REG_ADDR(A_RIU_BASE, 0x08980) 	// FCIE 0x0113 x 0x80


#define BANK_PM_SLEEP      	GET_CARD_REG_ADDR(A_RIU_PM_BASE, 	0x0700) //		0x000E x 0x80 =
#define BANK_PM_GPIO       	GET_CARD_REG_ADDR(A_RIU_PM_BASE, 	0x0780) //		0x000F x 0x80 =

#define MIU2_BANK			GET_CARD_REG_ADDR(A_RIU_BASE,		0x0300) //		0x0006 x 0x80 =
#define A_CHIPTOP_BANK      GET_CARD_REG_ADDR(A_RIU_BASE, 		0x0F00) // 		0x001E x 0x80 = 0x0F00
#define CLKGEN_BANK			GET_CARD_REG_ADDR(A_RIU_BASE, 		0x0580) //		0x000B x 0x80 = 0x0580
#define RIU_BASE_FCIE       GET_CARD_REG_ADDR(A_RIU_BASE, 	   0x10780) // SDIO 0x020F x 0x80 = 0x10780
#define	REG_BANK_GPIO 		GET_CARD_REG_ADDR(A_RIU_BASE,		0x1580) // GPIO 0x002B x 0x80 =



////////////////////////////////////////////////////////////////////////////////////////////////////
// PM_GPIO BANK 0x000E ?
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// CHIPTOP BANK 0x101E
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIPTOP_0A   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x0A)
#define CHIPTOP_0C   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x0C)
#define CHIPTOP_12   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x12)
#define CHIPTOP_1F   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x1F)
#define CHIPTOP_42   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x42)
#define CHIPTOP_43			GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x43)
#define CHIPTOP_4F			GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x4F)
#define CHIPTOP_50			GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x50)
#define CHIPTOP_52			GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x52)
#define CHIPTOP_5A			GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x5A)
#define CHIPTOP_7B   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x7B)
#define CHIPTOP_5D   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x5D)
#define CHIPTOP_64   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x64)
#define CHIPTOP_6E   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x6E)
#define CHIPTOP_6F   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x6F)
#define CHIPTOP_70   		GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x70)


////////////////////////////////////////////////////////////////////////////////////////////////////
// SDIO BANK 0x120F
////////////////////////////////////////////////////////////////////////////////////////////////////
#define SD_MODE             GET_CARD_REG_ADDR(RIU_BASE_FCIE, 0x10)
#define FCIE_2F             GET_CARD_REG_ADDR(RIU_BASE_FCIE, 0x2F)
#define FCIE_2D             GET_CARD_REG_ADDR(RIU_BASE_FCIE, 0x2D)


////////////////////////////////////////////////////////////////////////////////////////////////////
// MIU2 BANK 0x1006
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU2_7B				GET_CARD_REG_ADDR(MIU2_BANK, 0x7B)


////////////////////////////////////////////////////////////////////////////////////////////////////
// GPIO BANK 0x102B
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIP_GPIO_47		GET_CARD_REG_ADDR(REG_BANK_GPIO, 0x47) // MUJI MST215A power control
#define GPIO158_OEN 		BIT1 // low --> output
#define GPIO158_OUT 		BIT0 // low power on, high power off

#define CHIP_GPIO_52		GET_CARD_REG_ADDR(REG_BANK_GPIO, 0x52) // MUJI MST215A card detect
#define GPIO87_OEN			BIT1 // high --> input
#define GPIO87_IN			BIT2 // low --> insert, high --> remove


////////////////////////////////////////////////////////////////////////////////////////////////////
// CLKGEN BANK 0x100B
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CLKGEN_FCIE			GET_CARD_REG_ADDR(CLKGEN_BANK, 0x64)
#define CLKGEN_SDIO			GET_CARD_REG_ADDR(CLKGEN_BANK, 0x69)

#define   _48M					(0xF<<2)
#define   _43M					(0x5<<2)
#define   _36M					(0x3<<2)
#define   _32M					(0x2<<2)
#define   _27M					(0x1<<2) // 20M
#define   _18M					(0x1<<2) // 20M
#define   _300K					(0xD<<2)

// clock gen for FCIE use only
#define FCIE_CLK_F 48000000
#define FCIE_CLK_5 43200000
#define FCIE_CLK_4 40000000
#define FCIE_CLK_3 36000000
#define FCIE_CLK_2 32000000
#define FCIE_CLK_1 20000000
#define FCIE_CLK_D   300000

#define FCIE_CLK_GATING		BIT0 // 0: gate, 1, open
#define FCIE_CLK_INVERSE	BIT1
#define FCIE_CLK_SOURCE_MSK (BIT2|BIT3|BIT4|BIT5)
#define FCIE_CLK_SOURCE_SEL BIT6 // 0: xtal, 1: clk_fcie_p1


#define MIU2_BASE		GET_CARD_REG_ADDR(A_RIU_BASE,0x300)
#define MIU2_79				  GET_REG_ADDR(MIU2_BANK, 0x79)

#define	SDR50			1
#define	SDR104			2
#define	DDR50			3
//#define 	SDBUS			SDR50

