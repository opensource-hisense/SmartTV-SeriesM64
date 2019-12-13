//#include "hal_card_common.h"
#include "hal_card_regs5.h"
#include "sd_platform.h"
#include "hal_card_common.h"
#include <linux/delay.h>
#include "hal_sdmmc.h"

#define A_SD_REG_BANK(IP)			GET_CARD_BANK(IP, EV_REG_BANK)
#define A_MIE_FUNC_CTL(IP)			GET_CARD_REG_ADDR(A_SD_REG_BANK(IP), 0x07)
#define A_DDR_MOD_REG(IP)			GET_CARD_REG_ADDR(A_SD_REG_BANK(IP), 0x0F)
#define A_NC_FUNC_CTL(IP)			GET_CARD_REG_ADDR(A_SD_REG_BANK(IP), 0x63)

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_InitSetting
 *     @author jeremy.wang (2011/12/1)
 * Desc: IP Begin Setting before every operation, it's about platform setting.
 *
 * @param eIP : FCIE1/FCIE2/...
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_IPBeginSetting(IPEmType eIP, PADEmType ePAD)
{
	HalFcieSdio_CheckStateMachine(eIP);

	// bring up note, chiptio related register
	FCIE_RIU_16_OF(CHIPTOP_50, BIT15); // all_pad_in
	FCIE_RIU_16_OF(CHIPTOP_7B, BIT04|BIT05);
	FCIE_RIU_16_OF(CHIPTOP_5A, BIT09|BIT08);

	// Bring up note: GPIO_PAD SDR mode
	FCIE_RIU_16_OF(A_DDR_MOD_REG(eIP), BIT7|BIT8|BIT12);
	FCIE_RIU_16_ON(A_DDR_MOD_REG(eIP), BIT13|BIT14);

	if(eIP==0) // SDIO
	{
		if( !(FCIE_RIU_R16(MIU2_7A) & MIU_SELECT_BY_IP))
		{
			printk(LIGHT_RED"SdWarn: MIU select not enable for SDIO IP\n"NONE);
			FCIE_RIU_16_ON(MIU2_7A, MIU_SELECT_BY_IP);	// miu select by IP
		}

		//printk(LIGHT_RED"IP begin setting %d\n"NONE, eIP);

		FCIE_RIU_W16(A_MIE_FUNC_CTL(eIP), BIT_SDIO_MOD);

		#if 1
			// reg_sdio_config = 3 --> MST215A
			// PAD_EMMC_IO13, PAD_EMMC_IO12, PAD_EMMC_IO14, PAD_EMMC_IO15, PAD_EMMC_IO16, PAD_EMMC_IO17
			FCIE_RIU_16_ON(CHIPTOP_7B, BIT04|BIT05);
		#else
			// reg_sdio_config = 1
			// PAD_PCM2_CE_N, PAD_PCM2_IRQA_N, PAD_PCM_A[8], PAD_PCM_A[9], PAD_PCM_A[10], PAD_PCM_A[11]
			FCIE_RIU_16_ON(CHIPTOP_7B, BIT04);
			FCIE_RIU_16_OF(CHIPTOP_7B, BIT05);
		#endif
	}
	else if(eIP==1) // FCIE
	{
		//printk(YELLOW"IP begin setting %d\n"NONE, eIP);

		FCIE_RIU_W16(A_MIE_FUNC_CTL(eIP), BIT_EMMC_EN); // clear BIT_EMMC_ACTIVE by the way

		FCIE_RIU_16_OF(A_NC_FUNC_CTL(eIP), BIT0); // clear for share FICE with nand case

		// reg_sd_config = 2 --> MST215C & 072B
		// PAD_PCM2_RESET, PAD_PCM2_WAIT_N, PAD_PCM_A[4], PAD_PCM_A[5], PAD_PCM_A[6], PAD_PCM_A[7]
		FCIE_RIU_16_OF(CHIPTOP_64, BIT00|BIT03|BIT04);
		FCIE_RIU_16_ON(CHIPTOP_0A, BIT07|BIT06|BIT05|BIT04);
		FCIE_RIU_16_OF(CHIPTOP_6F, BIT07|BIT06);

		FCIE_RIU_16_ON(CHIPTOP_5A, BIT09); // sd_config = 2
		FCIE_RIU_16_OF(CHIPTOP_6E, BIT07|BIT06); // emmc_config = 0

		// emmc pll bank setting
		FCIE_RIU_16_OF(EMMC_PLL_REG_68, BIT00|BIT01); // reg_emmc_en = 0, reg_emmc_ddr_en = 0
		FCIE_RIU_16_OF(EMMC_PLL_REG_6D, BIT00); // reg_ddr_io_mode = 0
		FCIE_RIU_16_OF(EMMC_PLL_REG_70, BIT08);
		FCIE_RIU_W16(  EMMC_PLL_REG_71, 0xFFFF);
		FCIE_RIU_W16(  EMMC_PLL_REG_73, 0xFFFF);
		FCIE_RIU_16_ON(EMMC_PLL_REG_74, BIT15);
	}

}

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_IPEndSetting
 *     @author jeremy.wang (2012/5/3)
 * Desc: IP End Setting after every operation, it's about platform setting.
 *
 * @param eIP :
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_IPEndSetting(IPEmType eIP, PADEmType ePAD)
{
	if(eIP==1) // FCIE, the share IP
	{
		//FCIE_RIU_16_OF(CHIPTOP_5A, BIT8|BIT9); // diable SD pad to prevent eMMC driver send command to SD card
	}
}

void Hal_CARD_InitPADPin(PADEmType ePAD, BOOL_T bTwoCard)
{
	//printk("switch pad hal %d\n", ePAD);

	if(ePAD==1) // SDIO coneect to PAD_SDIO_IO0 ~ PAD_SDIO_IO5
	{
		//printk(LIGHT_RED"switch pad hal for SDIO connect to PAD_SDIO...\n"NONE);
	}
	else if(ePAD==0) // FCIE connect to PAD_SD_CLK, PAD_SD_CMD, PAD_SD_DX
	{
		//printk(YELLOW"switch pad hal for FCIE connect to PAD_SD...\n"NONE);
	}
}

void Hal_CARD_InitGPIO(GPIOEmType eGPIO, BOOL_T bEnable)
{
	//printk(CYAN"Init GPIO %d\n"NONE, eGPIO);

	if( eGPIO==EV_GPIO1 ) // for SDIO
    {
		//printk(LIGHT_RED"SDIO Init GPIO\n"NONE);
		if (bEnable)
		{
			FCIE_RIU_16_ON(CHIP_GPIO_52, GPIO87_OEN); // set oen to high for input
		}
		else
		{
		}
    }
	else if( eGPIO==EV_GPIO2 )
	{
		//printk(YELLOW"FCIE Init GPIO\n"NONE);
		if (bEnable)
		{
			FCIE_RIU_16_ON(PMGPIO_REG_07, PMGPIO7_OEN); // set oen to high for input
		}
		else
		{
		}
	}
}

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_GetGPIOState
 *     @author jeremy.wang (2012/5/22)
 * Desc:
 *
 * @param eGPIO :
 *
 * @return BOOL_T  :
 ----------------------------------------------------------------------------------------------------------*/
BOOL_T Hal_CARD_GetGPIOState(GPIOEmType eGPIO)
{
	if( eGPIO==EV_GPIO1 ) // SDIO
	{
		//printk(LIGHT_RED"SDIO Get GPIO\n"NONE);
		if (FCIE_RIU_R16(CHIP_GPIO_52) & GPIO87_IN)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else if( eGPIO==EV_GPIO2 )
	{
		//printk(YELLOW"FCIE Get GPIO\n"NONE);
		if (FCIE_RIU_R16(PMGPIO_REG_07) & PMGPIO7_IN)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	return TRUE;
}
/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_SetGPIOIntAttr
 *     @author jeremy.wang (2012/5/22)
 * Desc:
 *
 * @param eGPIO :
 * @param eGPIOOPT :
 ----------------------------------------------------------------------------------------------------------*/
void Hal_CARD_SetGPIOIntAttr(GPIOEmType eGPIO, GPIOOptEmType eGPIOOPT)
{
	//printk(CYAN"Set GPIO int attr %d\n"NONE, eGPIO);
}

/*----------------------------------------------------------------------------------------------------------
 *
 * Function: Hal_CARD_GPIOIntFilter
 *     @author jeremy.wang (2012/5/9)
 * Desc:
 *
 * @param eGPIO :
 *
 * @return BOOL_T  :
 ----------------------------------------------------------------------------------------------------------*/

BOOL_T Hal_CARD_GPIOIntFilter(GPIOEmType eGPIO)
{
    BOOL_T bRet=FALSE;
    return bRet;
}


void ClockBeginSetting(void)
{
	FCIE_RIU_16_OF(CLKGEN_SDIO, 0x3F);
	FCIE_RIU_16_ON(CLKGEN_SDIO, BIT06);
}


void HalCard_SetCardPower(U8_T u8OnOff)
{
	FCIE_RIU_16_OF(CHIP_GPIO_47, GPIO158_OEN); // active low

	if(u8OnOff)
	{
		FCIE_RIU_16_OF(CHIP_GPIO_47, GPIO158_OUT); // power on
	}
	else
	{
		FCIE_RIU_16_ON(CHIP_GPIO_47, GPIO158_OEN); // power off
	}
}


