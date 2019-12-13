//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#include "eMMC.h"
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

U32 gu32_eMMCDrvExtFlag = 0;

#if defined(IF_FCIE_SHARE_IP) && IF_FCIE_SHARE_IP
// common for TV linux platform
extern bool ms_sdmmc_card_chg(unsigned int slotNo);
#endif

extern struct platform_device sg_mstar_emmc_device_st;

U32 eMMC_hw_timer_delay(U32 u32us)
{
	#if 0 // udelay not work
    volatile u32 i = 0;

	for (i = 0; i < (u32us>>0); i++)
	{
		#if 1
		volatile int j = 0, tmp;
		for (j = 0; j < 0x38; j++)
		{
			tmp = j;
		}
		#endif
	}
	#else
    U32 u32_i = u32us;

	while(u32_i > 1000)
	{
		mdelay(1);
		u32_i -= 1000;
	}
    udelay(u32_i);
	#endif
	return u32us;
}


U32 eMMC_hw_timer_sleep(U32 u32ms)
{
	U32 u32_i = u32ms;

	while(u32_i > 1000)
	{
	    msleep(1000);
		u32_i -= 1000;
	}

	msleep(u32_i);
    return u32ms;
}


//--------------------------------
// use to performance test
static U32 u32_ms_start;
U32 eMMC_hw_timer_start(void)
{
	u32_ms_start = jiffies_to_usecs(jiffies);
	return eMMC_ST_SUCCESS;
}

U32 eMMC_hw_timer_tick(void)
{
	return jiffies_to_usecs(jiffies) - u32_ms_start;
}


//--------------------------------
void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}


//---------------------------------------
U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    if( u32_DMAAddr >= MSTAR_MIU1_BUS_BASE)
	{
		REG_FCIE_SETBIT( FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		u32_DMAAddr -= MSTAR_MIU1_BUS_BASE;
	}
	else
	{
		REG_FCIE_CLRBIT( FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		u32_DMAAddr -= MSTAR_MIU0_BUS_BASE;
	}

	return u32_DMAAddr;


}

U32 eMMC_DMA_MAP_address(U32 u32_Buffer, U32 u32_ByteCnt, int mode)
{
	dma_addr_t dma_addr;
	#if defined(CONFIG_ENABLE_EMMC_ACP) && CONFIG_ENABLE_EMMC_ACP
	dma_addr = dma_map_single(&sg_mstar_emmc_device_st.dev, (void*)u32_Buffer, u32_ByteCnt, DMA_ACP);
	#else
	if(mode == 0)	//write
	{
		dma_addr = dma_map_single(&sg_mstar_emmc_device_st.dev, (void*)u32_Buffer, u32_ByteCnt, DMA_TO_DEVICE);
	}
	else
	{
		dma_addr = dma_map_single(&sg_mstar_emmc_device_st.dev, (void*)u32_Buffer, u32_ByteCnt, DMA_FROM_DEVICE);
	}
	#endif

	return dma_addr;
}

void eMMC_DMA_UNMAP_address(U32 u32_DMAAddr, U32 u32_ByteCnt, int mode)
{
	#if defined(CONFIG_ENABLE_EMMC_ACP) && CONFIG_ENABLE_EMMC_ACP
	dma_unmap_single(&sg_mstar_emmc_device_st.dev, u32_DMAAddr, u32_ByteCnt, DMA_ACP);

	#else
	if(mode == 0)	//write
	{
		dma_unmap_single(&sg_mstar_emmc_device_st.dev, u32_DMAAddr, u32_ByteCnt, DMA_TO_DEVICE);
	}
	else
	{
		dma_unmap_single(&sg_mstar_emmc_device_st.dev, u32_DMAAddr, u32_ByteCnt, DMA_FROM_DEVICE);
	}
	#endif
}


void eMMC_flush_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    Chip_Clean_Cache_Range_VA_PA(u32_DMAAddr,__pa(u32_DMAAddr), u32_ByteCnt);
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    Chip_Inv_Cache_Range_VA_PA(u32_DMAAddr,__pa(u32_DMAAddr), u32_ByteCnt);
}

void eMMC_flush_miu_pipe(void)
{

}


void eMMC_DumpChiptop(void)
{
	U16 u16_i, u16_reg;

	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n chiptop:");

	for(u16_i=0 ; u16_i<0x80; u16_i++)
	{
		if(0 == u16_i%8)
			eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n%02Xh:| ", u16_i);

		REG_FCIE_R(GET_REG_ADDR(PAD_CHIPTOP_BASE, u16_i), u16_reg);
		eMMC_debug(eMMC_DEBUG_LEVEL, 0, "%04Xh ", u16_reg);

		if((u16_i & 0x7) == 0x7)
			eMMC_debug(eMMC_DEBUG_LEVEL, 0, "| %02Xh ", u16_i);
	}
}

extern U32 gu32_pwrsvr_gpio_enable;
extern U32 gu32_pwrsvr_gpio_addr;
extern U32 gu32_pwrsvr_gpio_bit;
extern U32 gu32_pwrsvr_gpio_trigger;

void eMMC_CheckPowerCut(void)
{
    /* SAR5=ON in set_config will enable this feature */
	extern u8 u8_enable_sar5;
    REG_FCIE_SETBIT(reg_sel_hvdetect, BIT_SEL_HVDETECT);
	if(u8_enable_sar5 == 1)
	{
    	while(((REG_FCIE(reg_vplug_in_pwrgd) >> 14)&0x0001) == 0)
    	{
    		eMMC_debug(0,1,"detect power cut\n");
    		//eMMC_hw_timer_delay(HW_TIMER_DELAY_500ms);
    		while(1);
    	}
    }
    else if(gu32_pwrsvr_gpio_enable)
    {
        if((REG_FCIE(IO_ADDRESS(gu32_pwrsvr_gpio_addr))&(1<<gu32_pwrsvr_gpio_bit))==gu32_pwrsvr_gpio_trigger)
        {
            eMMC_debug(0,1,"detect power cut\n");
            //eMMC_hw_timer_delay(HW_TIMER_DELAY_500ms);
            eMMC_CMD0(0);
            while(1);
        }
    }
}

void eMMC_Prepare_Power_Saving_Mode_Queue(void)
{
	REG_FCIE_SETBIT(reg_sel_hvdetect, BIT_SEL_HVDETECT);

    REG_FCIE_SETBIT(reg_pwrgd_int_glirm, BIT_PWRGD_INT_GLIRM_EN);

    REG_FCIE_CLRBIT(reg_pwrgd_int_glirm, BIT_PWEGD_INT_GLIRM_MASK);
    REG_FCIE_SETBIT(reg_pwrgd_int_glirm, (0x1F<<10));

    /* (1) Clear HW Enable */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x40), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x41),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0A);

	/* (2) Clear All Interrupt */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x42), 0xffff);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x43),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x00);

	/* (3) Clear SDE MODE Enable */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x44), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x45),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x10);

	/* (4) Clear SDE CTL Enable */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x46), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x47),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x11);

	/* (5) Reset Start */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x48), 0x4800);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x49),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x30);

	/* (6) Reset End */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x4A), 0x5800);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x4B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x30);

	/* (7) Set "SD_MOD" */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x4C), 0x0051);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x4D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x10);

	/* (8) Enable "csreg_sd_en" */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x4E), 0x0002);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x4F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0A);

	/* (9) Command Content, IDLE */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x50), 0x0040);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x51),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x60);

	/* (10) Command Content, STOP */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x52), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x53),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x61);

	/* (11) Command Content, STOP */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x54), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x55),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x62);

	/* (12) Command Size */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x56), 0x0005);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x57),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0E);

	/* (13) Response Size */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x58), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x59),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0D);

	/* (14) Enable Interrupt, SD_CMD_END */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x5A), 0x0002);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x5B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x01);

	/* (15) Command Enable */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x5C), 0x0004);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x5D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x11);

	/* (16) Wait Interrupt */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x5E), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x5F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WINT);

	/* (17) Clear Interrupt */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x60), 0x0002);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x61),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x00);

    /* (18) Clear HW Enable */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x62), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x63),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0A);

	/* (19) STOP */
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x64), 0x0000);
	REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x65),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_STOP);

    REG_FCIE_W(FCIE_PWR_SAVE_MODE, BIT_POWER_SAVE_MODE_EN | BIT_SD_POWER_SAVE_RST);
}


// [FIXME] -->
void eMMC_DumpPadClk(void)
{
	//----------------------------------------------
    eMMC_debug(0, 0, "\n[clk setting]: %uKHz \n", g_eMMCDrv.u32_ClkKHz);
    eMMC_debug(0, 0, "FCIE CLK(0x%X):0x%04X\n", reg_ckg_fcie, REG_FCIE_U16(reg_ckg_fcie));
    eMMC_debug(0, 0, "reg_ckg_nfie_div4_en (0x%X):0x%04X\n", reg_ckg_nfie_div4_en, REG_FCIE_U16(reg_ckg_nfie_div4_en));
	eMMC_debug(0, 0, "XTAL CLK SEL(0x%X):0x%04X\n", reg_ckg_nfie, REG_FCIE_U16(reg_ckg_nfie));


    //----------------------------------------------
    eMMC_DumpChiptop();

    eMMC_debug(0, 0, "\n[pad setting]: ");
    switch(g_eMMCDrv.u8_PadType)
	{
	case FCIE_eMMC_DDR:
		eMMC_debug(0,0,"DDR\n");  break;
	case FCIE_eMMC_SDR:
		eMMC_debug(0,0,"SDR\n");  break;
	case FCIE_eMMC_BYPASS:
		eMMC_debug(0,0,"BYPASS\n");  break;
	default:
		eMMC_debug(0,0,"eMMC Err: Pad unknown, %Xh\n", g_eMMCDrv.u8_PadType);  eMMC_die("\n");
	}

    eMMC_debug(0, 0, "reg_chiptop_0x50 (0x%08X):0x%04X\n", reg_chiptop_0x50, REG_FCIE_U16(reg_chiptop_0x50));
    eMMC_debug(0, 0, "reg_chiptop_0x43 (0x%08X):0x%04X\n", reg_chiptop_0x43, REG_FCIE_U16(reg_chiptop_0x43));
    eMMC_debug(0, 0, "reg_chiptop_0x5B (0x%08X):0x%04X\n", reg_sd_config,    REG_FCIE_U16(reg_sd_config));
	eMMC_debug(0, 0, "reg_chiptop_0x5D (0x%08X):0x%04X\n", reg_emmc_config, REG_FCIE_U16(reg_emmc_config));

    eMMC_debug(0, 0, "\n");
}

static U16 sgu16_eMMC_Pad=0xFFFF; // initialized by eMMC_platformInit

U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type)
{
	U16 u16_FCIE_Reg10h;

	// avoid possible glitch in clock
	REG_FCIE_R(FCIE_SD_MODE, u16_FCIE_Reg10h);
	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT8|BIT9|BIT10|BIT11|BIT12);
	REG_FCIE_CLRBIT(reg_chiptop_0x43, BIT_PAD_BYPASS_MACRO);
	REG_FCIE_CLRBIT(FCIE_SD_MACRO_REDNT2, BIT_DDR_TIMING_LATCH);
	REG_FCIE_CLRBIT(FCIE_REG_2Dh, BIT_ddr_timing_patch);

    switch(u32_FCIE_IF_Type){
        case FCIE_eMMC_DDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: DDR\n");

            // Let onboot & PCMCIA detemine the pad (reg_chiptop_0x6E) value
 		    REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT8|BIT9);
		    REG_FCIE_SETBIT(FCIE_SD_MACRO_REDNT2, BIT_DDR_TIMING_LATCH);
		    REG_FCIE_SETBIT(FCIE_REG_2Dh, BIT_ddr_timing_patch);
            g_eMMCDrv.u8_PadType = FCIE_eMMC_DDR;
            break;

        case FCIE_eMMC_SDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: SDR\n");

            // Let onboot & PCMCIA detemine the pad (reg_chiptop_0x6E) value
			REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT8);
            g_eMMCDrv.u8_PadType = FCIE_eMMC_SDR;
			break;

        case FCIE_eMMC_BYPASS:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: BYPASS\n");

            // Let onboot & PCMCIA detemine the pad (reg_chiptop_0x6E) value
			REG_FCIE_SETBIT(reg_chiptop_0x43, BIT_PAD_BYPASS_MACRO);	// reg_fcie2marco_sd_bypass
			REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT8|BIT10|BIT11);

            g_eMMCDrv.u8_PadType = FCIE_eMMC_BYPASS;
            break;

        default:
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: unknown interface: %X\n",u32_FCIE_IF_Type);
            REG_FCIE_W(FCIE_SD_MODE, u16_FCIE_Reg10h);
            return eMMC_ST_ERR_INVALID_PARAM;
    }

    // set chiptop
    if(0xFFFF == sgu16_eMMC_Pad)
    {
		REG_FCIE_R(reg_emmc_config, sgu16_eMMC_Pad);
		sgu16_eMMC_Pad &= BIT_EMMC_CFG_MASK;
    }
	REG_FCIE_SETBIT(reg_emmc_config, sgu16_eMMC_Pad);
    REG_FCIE_CLRBIT(reg_chiptop_0x0B, BIT_NAND_CFG_MASK);
    REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN| BIT_NAND_CS1_EN);


	REG_FCIE_W(FCIE_SD_MODE, u16_FCIE_Reg10h);
	return eMMC_ST_SUCCESS;
}

U32 eMMC_clock_setting(U16 u16_ClkParam)
{
	eMMC_PlatformResetPre();

	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_SETBIT(reg_ckg_nfie, BIT_XTAL_CLK_SEL);

	switch(u16_ClkParam)	{
	case BIT_FCIE_CLK_300K:     g_eMMCDrv.u32_ClkKHz = 300;     break;
	case BIT_FCIE_CLK_12M:      g_eMMCDrv.u32_ClkKHz = 12000;   break;
	case BIT_FCIE_CLK_18M:      g_eMMCDrv.u32_ClkKHz = 18000;   break;
    case BIT_FCIE_CLK_27M:      g_eMMCDrv.u32_ClkKHz = 27000;   break;
    case BIT_FCIE_CLK_36M:      g_eMMCDrv.u32_ClkKHz = 36000;   break;
	case BIT_FCIE_CLK_40M:      g_eMMCDrv.u32_ClkKHz = 40000;   break;
    case BIT_FCIE_CLK_48M:      g_eMMCDrv.u32_ClkKHz = 48000;   break;
	default:
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh\n", eMMC_ST_ERR_INVALID_PARAM);
		return eMMC_ST_ERR_INVALID_PARAM;
	}

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
	REG_FCIE_SETBIT(reg_ckg_fcie, u16_ClkParam<<BIT_FCIE_CLK_SHIFT);
	REG_FCIE_SETBIT(reg_ckg_nfie_div4_en, BIT_FCIE_CLK_DIV4_EN);

	eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 1, "clk:%uKHz, Param:%Xh, fcie(%Xh):%Xh, fcie_4X_en_div4_en(%Xh):%Xh\n",
		g_eMMCDrv.u32_ClkKHz, u16_ClkParam,
		reg_ckg_fcie, REG_FCIE_U16(reg_ckg_fcie),
		reg_ckg_nfie_div4_en, REG_FCIE_U16(reg_ckg_nfie_div4_en));

	g_eMMCDrv.u16_ClkRegVal = (U16)u16_ClkParam;
    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}


U32 eMMC_clock_gating(void)
{
	eMMC_PlatformResetPre();
	g_eMMCDrv.u32_ClkKHz = 0;
	REG_FCIE_W(reg_ckg_fcie, BIT_FCIE_CLK_Gate);
	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}


U8 gau8_FCIEClkSel[eMMC_FCIE_VALID_CLK_CNT]={
	BIT_FCIE_CLK_48M,
	BIT_FCIE_CLK_40M,
	BIT_FCIE_CLK_36M,
	BIT_FCIE_CLK_27M,
	BIT_FCIE_CLK_18M,

};

#if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
static char *sgachar_string[]={"  0T","0.5T","  1T","1.5T", "  2T", "2.5T", "  3T", "3.5T"};

static U32 eMMC_FCIE_DetectDDRTiming_Ex(U8 u8_DQS, U8 u8_DelaySel)
{
	U32 u32_SectorAddr;

	u32_SectorAddr = eMMC_TEST_BLK_0;

	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\ndqs:%s[%Xh]  cell:%02Xh \n",
		sgachar_string[u8_DQS], u8_DQS, u8_DelaySel);

	eMMC_FCIE_SetDDR48TimingReg(u8_DQS, u8_DelaySel);
    return eMMCTest_BlkWRC_ProbeTiming(u32_SectorAddr);
}

#define FCIE_DELAY_CELL_ts         300 // 0.3ns
static eMMC_FCIE_DDRT_WINDOW_t sg_DDRTWindow[2];

U32 eMMC_FCIE_DetectDDRTiming(void)
{
	U8  u8_dqs, u8_delay_sel, u8_i;
	U8  u8_dqs_prev=0xFF, u8_delay_sel_prev=0;
	U8  au8_DQSRegVal[8]={0,1,2,3,4,5,6,7}; // 0T, 0.5T, 1T, 1.5T, 2T, 2.5T, 3T, 3.5T
	U8  au8_DQS_10T[8]={0,5,10,15,20,25,30,35}; // 0T, 0.5T, 1T, 1.5T, 2T, 2.5T, 3T, 3.5T
	U8  u8_delay_Sel_max;
	U32 u32_ts;
	U32 u32_err;
	eMMC_FCIE_DDRT_WINDOW_t *pWindow = &sg_DDRTWindow[0];

	g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // just to turn off some log

	//--------------------------------------------------
	sg_DDRTWindow[0].u8_Cnt = 0;
	sg_DDRTWindow[0].aParam[0].u8_DQS = 0;
	sg_DDRTWindow[0].aParam[1].u8_DQS = 0;
    sg_DDRTWindow[1].u8_Cnt = 0;
	sg_DDRTWindow[1].aParam[0].u8_DQS = 0;
	sg_DDRTWindow[1].aParam[1].u8_DQS = 0;
	for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT; u8_i++)
	{
		sg_DDRTWindow[0].au8_DQSValidCellCnt[u8_i]=0;
		sg_DDRTWindow[1].au8_DQSValidCellCnt[u8_i]=0;
	}

	//--------------------------------------------------
	// calculate delay_Sel_max
	u32_ts = 1000*1000*1000 / g_eMMCDrv.u32_ClkKHz;
	u32_ts >>= 2; // for 4X's 1T

    //--------------------------------------------------
    // no need to try DQS of no delay
	//for(u8_dqs=0; u8_dqs<=(BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT); u8_dqs++)
	for(u8_dqs=1; u8_dqs<=(BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT); u8_dqs++)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\n---------------------------\n");

		if(u8_dqs < (BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT))
		    u8_delay_Sel_max =
		        (au8_DQS_10T[u8_dqs+1]-au8_DQS_10T[u8_dqs])
		        *u32_ts/(FCIE_DELAY_CELL_ts*10);
		else
			u8_delay_Sel_max = (BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT);

		if(u8_delay_Sel_max > (BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT))
		{
			u8_delay_Sel_max = (BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT);
			eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"max delay cell: %u\n", u8_delay_Sel_max);
			//eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"invalid, not try\n\n");
			//continue;
		}
		else
			eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"max delay cell: %u\n\n", u8_delay_Sel_max);

		sg_DDRTWindow[0].au8_DQSTryCellCnt[u8_dqs] = u8_delay_Sel_max;
		sg_DDRTWindow[1].au8_DQSTryCellCnt[u8_dqs] = u8_delay_Sel_max;

		//for(u8_delay_sel=0; u8_delay_sel<=(BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT); u8_delay_sel++)
		for(u8_delay_sel=0; u8_delay_sel<=u8_delay_Sel_max; u8_delay_sel++)
		{
			u32_err = eMMC_FCIE_DetectDDRTiming_Ex(au8_DQSRegVal[u8_dqs], u8_delay_sel);
			if(eMMC_ST_SUCCESS == u32_err)
			{
				eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"  PASS\n\n");
				pWindow->u8_Cnt++;
				if(0 == pWindow->aParam[0].u8_DQS) // save the window head
				{
					pWindow->aParam[0].u8_DQS = u8_dqs; // dqs uses index
					pWindow->aParam[0].u8_Cell = u8_delay_sel;
				}
				pWindow->au8_DQSValidCellCnt[u8_dqs]++;

				u8_dqs_prev = u8_dqs;
				u8_delay_sel_prev = u8_delay_sel;
			}
			else
			{   // save the window tail
				if(0xFF != u8_dqs_prev)
				{
					pWindow->aParam[1].u8_DQS = u8_dqs_prev; // dqs uses index
					pWindow->aParam[1].u8_Cell = u8_delay_sel_prev;
				}
				u8_dqs_prev = 0xFF;

				// discard & re-use the window having less PASS cnt
				pWindow =
				    (sg_DDRTWindow[0].u8_Cnt < sg_DDRTWindow[1].u8_Cnt) ?
				    &sg_DDRTWindow[0] : &sg_DDRTWindow[1];
				pWindow->u8_Cnt = 0;
				pWindow->aParam[0].u8_DQS = 0;
				pWindow->aParam[1].u8_DQS = 0;
				for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT; u8_i++)
					pWindow->au8_DQSValidCellCnt[u8_i]=0;
			}
		}
	}

	// for the case of last try is ok
	if(0xFF != u8_dqs_prev)
	{
		pWindow->aParam[1].u8_DQS = u8_dqs_prev; // dqs uses index
		pWindow->aParam[1].u8_Cell = u8_delay_sel_prev;
	}

	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE;

	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\n W0, Cnt:%Xh, [%Xh %Xh], [%Xh %Xh]\n",
		sg_DDRTWindow[0].u8_Cnt,
		sg_DDRTWindow[0].aParam[0].u8_DQS, sg_DDRTWindow[0].aParam[0].u8_Cell,
		sg_DDRTWindow[0].aParam[1].u8_DQS, sg_DDRTWindow[0].aParam[1].u8_Cell);
	for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT; u8_i++)
		eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"DQSValidCellCnt[%u]:%u \n",
			u8_i, sg_DDRTWindow[0].au8_DQSValidCellCnt[u8_i]);

	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\n W1, Cnt:%Xh, [%Xh %Xh], [%Xh %Xh]\n",
		sg_DDRTWindow[1].u8_Cnt,
		sg_DDRTWindow[1].aParam[0].u8_DQS, sg_DDRTWindow[1].aParam[0].u8_Cell,
		sg_DDRTWindow[1].aParam[1].u8_DQS, sg_DDRTWindow[1].aParam[1].u8_Cell);
	for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT; u8_i++)
		eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"DQSValidCellCnt[%u]:%u \n",
			u8_i, sg_DDRTWindow[1].au8_DQSValidCellCnt[u8_i]);

	if(sg_DDRTWindow[0].u8_Cnt || sg_DDRTWindow[1].u8_Cnt)
		return eMMC_ST_SUCCESS;
	else
		return eMMC_ST_ERR_NO_OK_DDR_PARAM;

}


U32 eMMC_FCIE_BuildDDRTimingTable(void)
{
	U8  au8_DQSRegVal[8]={0,1,2,3,4,5,6,7}; // 0T, 0.5T, 1T, 1.5T, 2T, 2.5T, 3T, 3.5T
	U8  u8_i, u8_ClkIdx, u8_SetIdx, u8_tmp, u8_DqsIdx = 0, u8_CellBase;
	U32 u32_err, u32_ret=eMMC_ST_ERR_NO_OK_DDR_PARAM;
	eMMC_FCIE_DDRT_WINDOW_t *pWindow;

	eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC Info: building DDR table, please wait... \n");
	memset((void*)&g_eMMCDrv.TimingTable_t, '\0', sizeof(g_eMMCDrv.TimingTable_t));
	u8_SetIdx = 0;

	if(eMMC_IF_NORMAL_SDR()){
		//eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC WARN: SDR mode can't detect DDR timing\n");
		u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_DDR);
		if(eMMC_ST_SUCCESS != u32_err)
		{
		    printk("eMMC Err: set DDR IF fail: %X\n", u32_err);
		    return u32_err;
		}
	}
	for(u8_ClkIdx=0; u8_ClkIdx<eMMC_FCIE_VALID_CLK_CNT; u8_ClkIdx++)
	{

		eMMC_debug(eMMC_DEBUG_LEVEL,0,"=================================\n");
		eMMC_clock_setting(gau8_FCIEClkSel[u8_ClkIdx]);
		eMMC_debug(eMMC_DEBUG_LEVEL,0,"=================================\n");

		// ---------------------------
		// search and set the Windows
		u32_err = eMMC_FCIE_DetectDDRTiming();

		// ---------------------------
		// set the Table
		if(eMMC_ST_SUCCESS == u32_err)
		{
			#if 0
			// before 12MHz, should at least 2 clk is ok for DDR
			if(eMMC_FCIE_DDRT_SET_CNT-1 == u8_SetIdx &&
				BIT_FCIE_CLK_12M == gau8_FCIEClkSel[u8_ClkIdx])
				u32_ret = eMMC_ST_SUCCESS;
			#endif

			g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Clk = gau8_FCIEClkSel[u8_ClkIdx];

			// ---------------------------
		 	// select Window
			pWindow = NULL;

			// pick up the Window of Cell=0 case
			if(sg_DDRTWindow[0].aParam[0].u8_DQS != sg_DDRTWindow[0].aParam[1].u8_DQS &&
			   sg_DDRTWindow[1].aParam[0].u8_DQS == sg_DDRTWindow[1].aParam[1].u8_DQS)
			   pWindow = &sg_DDRTWindow[0];
			else if(sg_DDRTWindow[0].aParam[0].u8_DQS == sg_DDRTWindow[0].aParam[1].u8_DQS &&
			   sg_DDRTWindow[1].aParam[0].u8_DQS != sg_DDRTWindow[1].aParam[1].u8_DQS)
			   pWindow = &sg_DDRTWindow[1];
			else if(sg_DDRTWindow[0].aParam[0].u8_DQS != sg_DDRTWindow[0].aParam[1].u8_DQS &&
			   sg_DDRTWindow[1].aParam[0].u8_DQS != sg_DDRTWindow[1].aParam[1].u8_DQS)
			   pWindow =
				    (sg_DDRTWindow[0].u8_Cnt > sg_DDRTWindow[1].u8_Cnt) ?
				    &sg_DDRTWindow[0] : &sg_DDRTWindow[1];

			// ---------------------------
			if(NULL != pWindow)
			{

				// pick up the DQS having max valid cell
				u8_tmp = 0;
				for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MODE_SHIFT; u8_i++)
				{
					if(u8_tmp <= pWindow->au8_DQSValidCellCnt[u8_i]){
						u8_tmp = pWindow->au8_DQSValidCellCnt[u8_i];
						u8_DqsIdx = u8_i;
					}
				}
				if(0 != u8_DqsIdx) // do not use 0T, this should be always TRUE
				{
					if(pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] >= 7 &&
						pWindow->au8_DQSValidCellCnt[u8_DqsIdx] >= 7)
					{
						g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_DQS = au8_DQSRegVal[u8_DqsIdx];
						g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_Cell = 0; // nice
					}
					else
					{
						u8_tmp = (pWindow->au8_DQSValidCellCnt[u8_DqsIdx] +
							pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1]) / 2;


						if(u8_tmp < pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1])
						{
							g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_DQS =
								au8_DQSRegVal[u8_DqsIdx-1];

							u8_CellBase = pWindow->au8_DQSTryCellCnt[u8_DqsIdx-1] - pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1];
							g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_Cell =
								u8_CellBase + pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] +
								pWindow->au8_DQSValidCellCnt[u8_DqsIdx] - u8_tmp;
						}
						else
						{   g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_DQS =
								au8_DQSRegVal[u8_DqsIdx];
						    g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_Cell =
								(pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] +
								pWindow->au8_DQSValidCellCnt[u8_DqsIdx]) / 2;
						}
					}
				}
			}

			// ---------------------------
            // or, pick up the Window of large PASS Cnt
			else //if(NULL == pWindow)
			{
			    pWindow =
				    (sg_DDRTWindow[0].u8_Cnt > sg_DDRTWindow[1].u8_Cnt) ?
				    &sg_DDRTWindow[0] : &sg_DDRTWindow[1];

				g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_DQS = au8_DQSRegVal[pWindow->aParam[0].u8_DQS];
				g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].Param.u8_Cell =
					(pWindow->aParam[0].u8_Cell + pWindow->aParam[1].u8_Cell)/2;
			}

			#if 0
			// ---------------------------
			// use 12M for Set.Min
			if(eMMC_FCIE_DDRT_SET_CNT-2 == u8_SetIdx)
				u8_ClkIdx = eMMC_FCIE_VALID_CLK_CNT-2;
			#else
			if(FCIE_SLOW_CLK == g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Clk)
				g_eMMCDrv.TimingTable_t.u8_SetCnt = u8_SetIdx + 1;
			#endif

			u8_SetIdx++;
		}
	}

	// dump DDRT Set
	u8_tmp = eMMC_FCIE_VALID_CLK_CNT > eMMC_FCIE_DDRT_SET_CNT ?
	    eMMC_FCIE_DDRT_SET_CNT : eMMC_FCIE_VALID_CLK_CNT;
	u8_tmp = u8_tmp > g_eMMCDrv.TimingTable_t.u8_SetCnt ?
		g_eMMCDrv.TimingTable_t.u8_SetCnt : u8_tmp;

	eMMC_DumpTimingTable();

	// ======================================================
	// CAUTION: expect 48MHz can have valid DDRT parameter
	if(g_eMMCDrv.TimingTable_t.Set[eMMC_TIMING_SET_MAX].u8_Clk == gau8_FCIEClkSel[0]
		&& 0 != g_eMMCDrv.TimingTable_t.u8_SetCnt)
		u32_ret = eMMC_ST_SUCCESS;
	else if(g_eMMCDrv.TimingTable_t.Set[eMMC_TIMING_SET_MAX].u8_Clk != gau8_FCIEClkSel[0])
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: max clk can NOT run DDR\n");
	else if(0 == g_eMMCDrv.TimingTable_t.u8_SetCnt)
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: min clk can NOT run DDR\n");
	// ======================================================

	// ---------------------------
	// save DDRT Table
	if(eMMC_ST_SUCCESS == u32_ret)
	{

		g_eMMCDrv.TimingTable_t.u32_ChkSum =
		    eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t)-4);
		memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t));

		eMMC_FCIE_ApplyTimingSet(eMMC_TIMING_SET_MAX);

		u32_err = eMMC_CMD24(eMMC_DDRTABLE_BLK_0, gau8_eMMC_SectorBuf);
	    u32_ret = eMMC_CMD24(eMMC_DDRTABLE_BLK_1, gau8_eMMC_SectorBuf);
	    if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
	    {
		    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC ERROR: %Xh %Xh\n",
			    u32_err, u32_ret);
		    return eMMC_ST_ERR_SAVE_DDRT_FAIL;
	    }
	}
	else
	{
		gau8_eMMC_SectorBuf[0] = (U8)~(eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t))>>24);
		gau8_eMMC_SectorBuf[1] = (U8)~(eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t))>>16);
		gau8_eMMC_SectorBuf[2] = (U8)~(eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t))>> 8);
		gau8_eMMC_SectorBuf[3] = (U8)~(eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t))>> 0);

		u32_ret = eMMC_ST_ERR_BUILD_DDRT;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: fail! please call CAE or RD for helps, %Xh \n",
			u32_ret);
		eMMC_ResetReadyFlag();
		eMMC_Init();
		eMMC_FCIE_EnableSDRMode();

	}

	return u32_ret;
}

#endif // IF_DETECT_eMMC_DDR_TIMING


//---------------------------------------
#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE

static DECLARE_WAIT_QUEUE_HEAD(fcie_wait);
static volatile U32 fcie_int = 0;

#define eMMC_IRQ_DEBUG    1

irqreturn_t eMMC_FCIE_IRQ(int irq, void *dummy)
{
	volatile u16 u16_Events;

	if((REG_FCIE(FCIE_REG16h) & BIT_EMMC_ACTIVE) != BIT_EMMC_ACTIVE)
	{
		return IRQ_NONE;
	}

	// one time enable one bit
	u16_Events = REG_FCIE(FCIE_MIE_EVENT) & REG_FCIE(FCIE_MIE_INT_EN);

	if(u16_Events & BIT_MIU_LAST_DONE)
    {
        REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);

        fcie_int = 1;
        wake_up(&fcie_wait);
		return IRQ_HANDLED;
    }
    else if(u16_Events & BIT_CARD_DMA_END)
    {
        REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_CARD_DMA_END);

        fcie_int = 1;
        wake_up(&fcie_wait);
		return IRQ_HANDLED;
    }
	else if(u16_Events & BIT_SD_CMD_END)
	{
		REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_SD_CMD_END);

		fcie_int = 1;
		wake_up(&fcie_wait);
		return IRQ_HANDLED;
	}
	#if defined(ENABLE_FCIE_HW_BUSY_CHECK)&&ENABLE_FCIE_HW_BUSY_CHECK
	else if(u16_Events & BIT_SD_BUSY_END)
	{
        REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_SD_BUSY_END);
		REG_FCIE_CLRBIT(FCIE_SD_CTRL, BIT_SD_BUSY_DET_ON);
		fcie_int = 1;
		wake_up(&fcie_wait);
		return IRQ_HANDLED;
	}
    #endif
#if eMMC_IRQ_DEBUG
	if(0==fcie_int)
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: Int St:%Xh, En:%Xh, Evt:%Xh \n",
		    REG_FCIE(FCIE_MIE_EVENT), REG_FCIE(FCIE_MIE_INT_EN), u16_Events);
#endif

	return IRQ_NONE;

}


U32 eMMC_WaitCompleteIntr(uintptr_t u32_RegAddr, U16 u16_WaitEvent, U32 u32_MicroSec)
{
	U32 u32_i=0;

    #if eMMC_IRQ_DEBUG
	U32 u32_isr_tmp[2];
	unsigned long long u64_jiffies_tmp, u64_jiffies_now;
	struct timeval time_st;
	time_t sec_tmp;
	suseconds_t us_tmp;

	u32_isr_tmp[0] = fcie_int;
	do_gettimeofday(&time_st);
	sec_tmp = time_st.tv_sec;
	us_tmp = time_st.tv_usec;
	u64_jiffies_tmp = jiffies_64;
    #endif

	//----------------------------------------
	if(wait_event_timeout(fcie_wait, (fcie_int == 1), usecs_to_jiffies(u32_MicroSec)) == 0)
    {
	    #if eMMC_IRQ_DEBUG
		u32_isr_tmp[1] = fcie_int;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
			"eMMC Warn: int timeout, WaitEvt:%Xh, NowEvt:%Xh, IntEn:%Xh, ISR:%u->%u->%u \n",
			u16_WaitEvent, REG_FCIE(FCIE_MIE_EVENT), REG_FCIE(FCIE_MIE_INT_EN),
			u32_isr_tmp[0], u32_isr_tmp[1], fcie_int);

		do_gettimeofday(&time_st);
		u64_jiffies_now = jiffies_64;
	    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
		    " PassTime: %lu s, %lu us, %llu jiffies.  WaitTime: %u us, %lu jiffies, HZ:%u.\n",
		    time_st.tv_sec-sec_tmp, time_st.tv_usec-us_tmp, u64_jiffies_now-u64_jiffies_tmp,
		    u32_MicroSec, usecs_to_jiffies(u32_MicroSec), HZ);
	    #else
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
			"eMMC Warn: int timeout, WaitEvt:%Xh, NowEvt:%Xh, IntEn:%Xh \n",
			u16_WaitEvent, REG_FCIE(FCIE_MIE_EVENT), REG_FCIE(FCIE_MIE_INT_EN));
	    #endif

		// switch to polling
        for(u32_i=0; u32_i<u32_MicroSec; u32_i++)
        {
            if((REG_FCIE(u32_RegAddr) & u16_WaitEvent) == u16_WaitEvent )
                break;

            eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        }

		if(u32_i == u32_MicroSec)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: events lose, WaitEvent: %Xh \n", u16_WaitEvent);
			eMMC_DumpDriverStatus();  eMMC_DumpPadClk();
		    eMMC_FCIE_DumpRegisters();eMMC_FCIE_DumpDebugBus();
			return eMMC_ST_ERR_INT_TO;
		}
		else
		{	REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, u16_WaitEvent);
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: but polling ok: %Xh \n", REG_FCIE(u32_RegAddr));
		}

    }

	//----------------------------------------
	if(u16_WaitEvent & BIT_MIU_LAST_DONE)
	{
		for(u32_i=0; u32_i<TIME_WAIT_1_BLK_END; u32_i++)
		{
			if(REG_FCIE(u32_RegAddr) & BIT_CARD_DMA_END)
				break; // should be very fase
			eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
		}

		if(TIME_WAIT_1_BLK_END == u32_i)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: no CARD_DMA_END\n");
			#if 0
			eMMC_FCIE_ErrHandler_Stop();
			#else
			eMMC_DumpDriverStatus();  eMMC_DumpPadClk();
		    eMMC_FCIE_DumpRegisters();eMMC_FCIE_DumpDebugBus();
			return eMMC_ST_ERR_INT_TO;
			#endif
		}
	}

	//----------------------------------------
	fcie_int = 0;
	return eMMC_ST_SUCCESS;

}


#endif


int mstar_mci_Housekeep(void *pData)
{
	#if !(defined(eMMC_HOUSEKEEP_THREAD) && eMMC_HOUSEKEEP_THREAD)
    return 0;
	#endif



	while(1)
	{

		if(kthread_should_stop())
			break;
	}

	return 0;
}


int mstar_mci_exit_checkdone_Ex(void)
{
    U16 u16Reg = 0;

    u16Reg = REG_FCIE(FCIE_SD_STATUS);

    if(u16Reg & BIT_SD_D0)
        return 1;
    else
        return 0;
}

bool mstar_mci_exit_checkdone_ForCI(void)
{
	return true;
}

EXPORT_SYMBOL(mstar_mci_exit_checkdone_ForCI);

#if (defined(IF_FCIE_SHARE_IP) && IF_FCIE_SHARE_IP)
bool mstar_mci_exit_checkdone_ForSD(void)
{
	U32 u32_Cnt=0;

    // nike SD_mode_x does not share pins
	//if(BIT_eMMC_CFG_MODE2 == (REG_FCIE(reg_chiptop_0x5D)&BIT_eMMC_CFG_MASK))
	//	return TRUE;
    REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_CLK_EN); // output clock
	while(mstar_mci_exit_checkdone_Ex() == 0) /* card busy */
	{
		if ((++u32_Cnt) >= HW_TIMER_DELAY_1s*10)
		{
			REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN); // not output clock
			eMMC_debug(0,1,"eMMC Info: SD check -> D0 busy\n");
			return false;
		}

		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);

		if(0 == (u32_Cnt%HW_TIMER_DELAY_100us))
		{
			msleep(1);
			u32_Cnt += HW_TIMER_DELAY_1ms;
		}
	}

	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN); // not output clock
	return true;
}
EXPORT_SYMBOL(mstar_mci_exit_checkdone_ForSD);
#endif

#if 0
U32 mstar_SD_CardChange(void)
{
	return 0;
}
#endif

//---------------------------------------

#if (defined(IF_FCIE_SHARE_PINS) && IF_FCIE_SHARE_PINS) || \
    (defined(IF_FCIE_SHARE_IP) && IF_FCIE_SHARE_IP)
extern struct semaphore	PfModeSem;
#endif

extern bool ms_sdmmc_wait_d0_for_emmc(void);
#if IF_FCIE_SHARE_IP && defined(CONFIG_MSTAR_SDMMC)
static U16 sgu16_SD_Pad;
#endif

void eMMC_LockFCIE(U8 *pu8_str)
{
	//eMMC_debug(0,1,"%s 1\n", pu8_str);
	#if (defined(IF_FCIE_SHARE_PINS) && IF_FCIE_SHARE_PINS) || \
        (defined(IF_FCIE_SHARE_IP) && IF_FCIE_SHARE_IP)
	down(&PfModeSem);
    #endif
	//eMMC_debug(0,1,"%s 2\n", pu8_str);

	#if IF_FCIE_SHARE_IP && defined(CONFIG_MSTAR_SDMMC)
	if(false == ms_sdmmc_wait_d0_for_emmc())
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: SD keep D0 low \n");
		eMMC_FCIE_ErrHandler_Stop();
	}

	eMMC_clock_setting(g_eMMCDrv.u16_ClkRegVal);
	eMMC_pads_switch(g_eMMCDrv.u8_PadType);

	// backup sd_config/sd_config2, and clear them
	REG_FCIE_R(reg_sd_config, sgu16_SD_Pad);
	sgu16_SD_Pad &= BIT_SD_CFG_MASK;
	REG_FCIE_CLRBIT(reg_sd_config, BIT_SD_CFG_MASK);
	#else
    if( (g_eMMCDrv.u8_PadType==FCIE_eMMC_DDR) || (g_eMMCDrv.u8_PadType==FCIE_eMMC_SDR) )
    {
        REG_FCIE_CLRBIT(reg_chiptop_0x43, BIT_PAD_BYPASS_MACRO);
    }
	REG_FCIE_CLRBIT(reg_sd_config, BIT_SD_CFG_MASK);
	#endif

    REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_CLK_EN); // output clock
	REG_FCIE_SETBIT(FCIE_REG16h, BIT_EMMC_ACTIVE);
}


void eMMC_UnlockFCIE(U8 *pu8_str)
{
	//eMMC_debug(0,1,"%s 1\n", pu8_str);
	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN); // not output clock
	REG_FCIE_CLRBIT(FCIE_REG16h, BIT_EMMC_ACTIVE);

	#if IF_FCIE_SHARE_IP && defined(CONFIG_MSTAR_SDMMC)
	// restore sd_config/sd_config2
    REG_FCIE_SETBIT(reg_sd_config, sgu16_SD_Pad);
	#endif

    #if (defined(IF_FCIE_SHARE_PINS) && IF_FCIE_SHARE_PINS) || \
        (defined(IF_FCIE_SHARE_IP) && IF_FCIE_SHARE_IP)
	up(&PfModeSem);
    #endif
	//eMMC_debug(0,1,"%s 2\n", pu8_str);
}

//---------------------------------------

U32 eMMC_PlatformResetPre(void)
{
	/**((volatile unsigned short *)(0x25007DCC))|=0x02;	// emi mask
	*((volatile unsigned short *)(0x25007C18))|=0x02;	// imi0 mask
	*((volatile unsigned short *)(0x25007C58))|=0x02;	// imi1 mask
	*/
	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	/**((volatile unsigned short *)(0x25007DCC))&=(~0x02);	// emi unmask
	*((volatile unsigned short *)(0x25007C18))&=(~0x02);	// imi0 unmask
	*((volatile unsigned short *)(0x25007C58))&=(~0x02);	// imi1 unmask
	*/
    #if defined(ENABLE_EMMC_POWER_SAVING_MODE) && ENABLE_EMMC_POWER_SAVING_MODE
	eMMC_Prepare_Power_Saving_Mode_Queue();
    #endif


	REG_FCIE_CLRBIT(FCIE_REG_2Dh, BIT_csreg_miu_wp_last2_sel);
	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
	static U8 initialized = 0;

	if(initialized == 0)
	{
		REG_FCIE_R(reg_emmc_config, sgu16_eMMC_Pad);
		sgu16_eMMC_Pad &= BIT_EMMC_CFG_MASK;
		if(sgu16_eMMC_Pad!=BIT_EMMC_MODE1)
		{
			eMMC_die("\n");
		}
		initialized = 1;
	}

	eMMC_pads_switch(FCIE_DEFAULT_PAD);
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformDeinit(void)
{
	return eMMC_ST_SUCCESS;
}

// --------------------------------------------
static U32 sgu32_MemGuard0 = 0xA55A;
eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
static U32 sgu32_MemGuard1 = 0x1289;

eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

U32 eMMC_CheckIfMemCorrupt(void)
{
	if(0xA55A != sgu32_MemGuard0 || 0x1289 != sgu32_MemGuard1)
		return eMMC_ST_ERR_MEM_CORRUPT;

	return eMMC_ST_SUCCESS;
}
// <-- [FIXME]

#endif
