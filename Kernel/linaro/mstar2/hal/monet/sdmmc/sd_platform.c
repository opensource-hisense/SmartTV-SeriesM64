//#include "hal_card_common.h"
#include "hal_card_regs5.h"
#include "sd_platform.h"
#include "hal_card_common.h"
#include <linux/delay.h>
#include "hal_sdmmc.h"

#define A_SD_REG_BANK(IP)			GET_CARD_BANK(IP, EV_REG_BANK)
#define A_DDR_MOD_REG(IP)			GET_CARD_REG_ADDR(A_SD_REG_BANK(IP), 0x0F)

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

	#if 1
		// reg_sdio_config = 3 --> MST215A
		// PAD_EMMC_IO13, PAD_EMMC_IO12, PAD_EMMC_IO14, PAD_EMMC_IO15, PAD_EMMC_IO16, PAD_EMMC_IO17
		FCIE_RIU_16_ON(CHIPTOP_7B, BIT04|BIT05);
	#else
		// reg_sdio_config = 1 --> MST215C / 072B
		// PAD_PCM2_CE_N, PAD_PCM2_IRQA_N, PAD_PCM_A[8], PAD_PCM_A[9], PAD_PCM_A[10], PAD_PCM_A[11]
		FCIE_RIU_16_ON(CHIPTOP_7B, BIT04);
		FCIE_RIU_16_OF(CHIPTOP_7B, BIT05);
	#endif
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
		//printk(YELLOW"switch pad hal for SDIO connect to PAD_SDIO...\n"NONE);

		// Bring up note: GPIO_PAD SDR mode
		FCIE_RIU_16_OF(FCIE_MIE_FUNC_CTL, BIT_EMMC_EN|BIT_SD_EN);
	  	FCIE_RIU_16_ON(FCIE_MIE_FUNC_CTL, BIT_SDIO_MOD);
	   	FCIE_RIU_16_OF(FCIE_DDR_MODE, BIT07|BIT08|BIT12);
		FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT13|BIT14);
	}
	else if(ePAD==0) // FCIE connect to PAD_SD_CLK, PAD_SD_CMD, PAD_SD_DX
	{
		printk(YELLOW"switch pad hal for FCIE connect to PAD_SD...\n"NONE);
	}
}

void Hal_CARD_InitGPIO(GPIOEmType eGPIO, BOOL_T bEnable)
{
	//printk(CYAN"Init GPIO %d\n"NONE, eGPIO);

	if( eGPIO==EV_GPIO1 ) // for FCIE, PAD_SD_XXX
    {
		//printk(YELLOW"SDIO Init GPIO\n"NONE);
		if (bEnable)
		{
			FCIE_RIU_16_ON(CHIP_GPIO_52, GPIO87_OEN); // set oen to high for input
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
		//printk(YELLOW"SDIO Get GPIO\n"NONE);
		if (FCIE_RIU_R16(CHIP_GPIO_52) & GPIO87_IN)
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


