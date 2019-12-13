
#define A_RIU_PM_BASE   	(0x1F000000)

#define MIU2_BANK	       	 GET_CARD_REG_ADDR(A_RIU_BASE, 0x300)
#define A_CLKGEN_BANK       GET_CARD_REG_ADDR(A_RIU_BASE, 0x0580)
#define A_CHIPTOP_BANK      GET_CARD_REG_ADDR(A_RIU_BASE, 0x0F00)

#define PMGPIO_BANK       GET_CARD_REG_ADDR(A_RIU_PM_BASE, 0x700)
#define PMGPIO_OE			 GET_CARD_REG_ADDR(PMGPIO_BANK,0x0F)
#define PMGPIO_MASK		 GET_CARD_REG_ADDR(PMGPIO_BANK,0)
#define PMGPIO_RAWST		 GET_CARD_REG_ADDR(PMGPIO_BANK,0x0C)
#define PMGPIO_FINALST		GET_CARD_REG_ADDR(PMGPIO_BANK,0x0A)
#define PMGPIO_CLR			GET_CARD_REG_ADDR(PMGPIO_BANK,0x04)
#define PMGPIO_POL			GET_CARD_REG_ADDR(PMGPIO_BANK,0x06)

#define CHIPTOP_0C   		  GET_CARD_REG_ADDR (A_CHIPTOP_BANK, 0x0c )
#define CHIPTOP_12   		GET_CARD_REG_ADDR   (A_CHIPTOP_BANK, 0x12 )
#define CHIPTOP_1F   		GET_CARD_REG_ADDR   (A_CHIPTOP_BANK, 0x1F )
#define CHIPTOP_42   		GET_CARD_REG_ADDR   (A_CHIPTOP_BANK, 0x42 )
#define CHIPTOP_43   		  GET_CARD_REG_ADDR (A_CHIPTOP_BANK, 0x43 )
#define CHIPTOP_4F   		 GET_CARD_REG_ADDR  (A_CHIPTOP_BANK, 0x4F )
#define CHIPTOP_50   		 GET_CARD_REG_ADDR  (A_CHIPTOP_BANK, 0x50 )
#define CHIPTOP_5B   		   GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x5B )
#define CHIPTOP_5D   		   GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x5D )

#define CHIPTOP_64   		   GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x64 )
#define CHIPTOP_6F   		   GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x6F )
#define CHIPTOP_70   		   GET_CARD_REG_ADDR(A_CHIPTOP_BANK, 0x70 )

#define RIU_BASE_FCIE           GET_CARD_REG_ADDR(A_RIU_BASE, 0x10780)
#define SD_MODE                 	GET_CARD_REG_ADDR(RIU_BASE_FCIE,0x10)
#define FCIE_2F                 	GET_CARD_REG_ADDR(RIU_BASE_FCIE,0x2F)
#define FCIE_2D                 	GET_CARD_REG_ADDR(RIU_BASE_FCIE,0x2D)

#define MIU2_7B				  GET_CARD_REG_ADDR(MIU2_BANK, 0x7B)






#define BIT_GPIO_IN         BIT02
#define BIT_GPIO_FIQ_MASK   BIT04
#define BIT_GPIO_FIQ_CLR    BIT06
#define BIT_GPIO_FIQ_POL    BIT07
#define BIT_GPIO_FIQ_FINAL  BIT08

#define   CLOCK_GEN_REG		0x69
#define   _48M					(15<<2)
#define   _43M					(6<<2)
#define   _36M					(4<<2)
#define   _32M					(3<<2)
#define   _27M					(2<<2)
#define   _18M					(0<<2)
#define   _300K				(13<<2)


