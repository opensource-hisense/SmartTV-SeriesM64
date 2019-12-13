////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2012 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#include "mstar_mci_v5.h"
#include <linux/string.h>
#include "chip_setup.h"
#include <linux/mmc/mmc.h>
#include "hal_fcie5.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define DRIVER_NAME					"mstar_mci"
#define MCI_RETRY_CNT_OK_CLK_UP     10
#define U32BEND2LEND(X) ( ((X&0x000000FF)<<24) + ((X&0x0000FF00)<<8) + ((X&0x00FF0000)>>8) + ((X&0xFF000000)>>24) )


/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

#define ENABLE_eMMC_ADMA_MODE 1

#if defined(ENABLE_eMMC_ADMA_MODE) && ENABLE_eMMC_ADMA_MODE
static U32	mstar_mci_prepare_descriptors(struct mmc_data * pData);
static void	mstar_mci_setup_descriptor_addr(CMD_RSP_INFO * pCmdRspInfo);
#else
static U32	mstar_mci_setup_dma_info(struct mmc_data * pData, CMD_RSP_INFO * pCmdRspInfo);
#endif

static void mstar_mci_enable(struct mstar_mci_host *pMStarHost_st);
//static  U32 mstar_mci_check_rsp_tran_sts(struct mmc_command * cmd);
//static void mstar_mci_copy_rsp_back(struct mmc_command * cmd);
static  int mstar_mci_pre_dma_transfer(struct mstar_mci_host *host, struct mmc_data *data, struct mstar_mci_host_next *next);
static U32 mstar_mci_send_command(struct mmc_command * pCmd);
//static void mstar_mci_send_cmd_data_tran(struct mmc_command * pCmdDT);
//static void mstar_mci_cmd(void);
//static void mstar_mci_dtc(void);

//static void mstar_mci_dtc_bh(struct work_struct *work);
void mstar_mci_dump_debug_msg(void);
void mstar_mci_hardware_reset(void);
void mstar_mci_normal_resume(void);
void mstar_mci_fast_resume(void);
void mstar_mci_preinit_emmc(void);


//static void mstar_mci_send_command(struct mstar_mci_host *pMStarHost_st, struct mmc_command *cmd, U8 *pu8_str);
//static void mstar_mci_completed_command(struct mmc_command * cmd, u8 u8CpyRsp);
//static void mstar_mci_wait_r1b(struct mmc_command * cmd);
//static void mstar_mci_schedule_tasklet_test(void);

/*****************************************************************************
 * Define Static Global Variables
 ******************************************************************************/
#if defined(DMA_TIME_TEST) && DMA_TIME_TEST
static u32 total_read_dma_len = 0;
static u32 total_read_dma_time = 0;
static u32 total_write_dma_len = 0;
static u32 total_write_dma_time = 0;
#endif
static struct mstar_mci_host *curhost;


/******************************************************************************
 * Functions
 ******************************************************************************/

/*static unsigned long mstar_mci_get_ms(void)
{
    struct timeval tv;
    unsigned long ms;

    do_gettimeofday(&tv);
    ms = tv.tv_usec/1000;
    ms += tv.tv_sec * 1000;

    return ms;
}*/

#if defined(ENABLE_eMMC_ADMA_MODE) && ENABLE_eMMC_ADMA_MODE

#define DEBUG_SG_ELEMENT 0

static U32 mstar_mci_prepare_descriptors(struct mmc_data * pData)
{
	U32 i, u32DescptAddr, u32BusAddr, u32DeviceAddr, u32DmaLeng, u32TotalLength = 0;
	struct scatterlist  *pScatterList = 0;

	//printk("descriptor addr %08Xh", g_ADMAInfo.DescriptorAddr);
	u32DescptAddr = virt_to_phys((void *)g_ADMAInfo.DescriptorAddr); // virtual to bus address
	//printk(" -- virt_to_phys --> %08Xh\n", u32DescptAddr);

	// setup descriptor
	memset(&g_ADMAInfo, 0, sizeof(ADMA_INFO)-4); // clear

	#if defined(DEBUG_SG_ELEMENT) && DEBUG_SG_ELEMENT
	printk("SG[%d] = ", pData->sg_len);
	#endif

	for(i=0; i<pData->sg_len; i++) {

		pScatterList = &(pData->sg[i]);
		u32BusAddr = sg_dma_address(pScatterList);
		u32DmaLeng = sg_dma_len(pScatterList);

		BUG_ON((u32DmaLeng>>9)&0xFFFFF000);
		u32TotalLength += u32DmaLeng;
		#if defined(DEBUG_SG_ELEMENT) && DEBUG_SG_ELEMENT
		printk("%d + ", u32DmaLeng>>9);
		#endif
		//printk("SG[%d] %d sector, BA = %08Xh", i, u32DmaLeng>>9, u32BusAddr);
		if( u32BusAddr >= MSTAR_MIU1_BUS_BASE) {
			u32DeviceAddr = u32BusAddr - MSTAR_MIU1_BUS_BASE;
			g_ADMAInfo.Descriptor[i].adma_miu_sel = 1;
			//printk(" - %08Xh(MIU1) = %08Xh\n", MSTAR_MIU1_BUS_BASE , u32DeviceAddr);
		} else {
			u32DeviceAddr = u32BusAddr - MSTAR_MIU0_BUS_BASE;
			//printk(" - %08Xh(MIU0) = %08Xh\n", MSTAR_MIU0_BUS_BASE , u32DeviceAddr);
		}

		g_ADMAInfo.Descriptor[i].adma_miu_addr = u32DeviceAddr;
		g_ADMAInfo.Descriptor[i].adma_job_cnt = u32DmaLeng>>9;
		g_ADMAInfo.Descriptor[i].adma_length = u32DmaLeng;

	}

	g_ADMAInfo.Descriptor[pData->sg_len-1].adma_end_flag = 1; // must give a end mark

	#if defined(DEBUG_SG_ELEMENT) && DEBUG_SG_ELEMENT
	printk("= %d\n", u32TotalLength>>9);
	#endif

	//MsOS_Dcache_Flush(g_ADMAInfo.Descriptor, sizeof(ADMA_INFO));

	Chip_Clean_Cache_Range_VA_PA(g_ADMAInfo.DescriptorAddr, u32DescptAddr, sizeof(ADMA_INFO));

	//eMMC_dump_mem((U8 *)&g_ADMAInfo, 32); // debug use

	return u32TotalLength;
}

// if there is only 1 descriptor, only need to setup once
// need to think about share IP case, setup each time for safe if share IP

static void	mstar_mci_setup_descriptor_addr(CMD_RSP_INFO * pCmdRspInfo)
{
	U32	u32DescptAddr;


	// give descriptor array addr to FCIE
	//u32DescptMiuAddr = eMMC_translate_DMA_address_Ex(g_ADMAInfo.DescriptorAddr, sizeof(ADMA_INFO));

	//printk("mstar_mci_setup_descriptor_addr1 --> %08X\n", g_ADMAInfo.DescriptorAddr);

	u32DescptAddr = virt_to_phys((void *)g_ADMAInfo.DescriptorAddr); // virtual to bus address

	//printk("mstar_mci_setup_descriptor_addr2 --> %08X\n", u32DescptAddr);

	//printk("Descpt Addr: %08Xh --> %08Xh", g_ADMAInfo.DescriptorAddr, u32DescptMiuAddr);

	// bus address to device address
	if( u32DescptAddr >= MSTAR_MIU1_BUS_BASE) {
		u32DescptAddr -= MSTAR_MIU1_BUS_BASE;
		//REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		//printk(" - %08Xh(MIU1) = %08Xh\n", MSTAR_MIU1_BUS_BASE , u32DescptMiuAddr);
	} else {
		u32DescptAddr -= MSTAR_MIU0_BUS_BASE;
		//REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		//printk(" - %08Xh(MIU0) = %08Xh\n", MSTAR_MIU0_BUS_BASE , u32DescptMiuAddr);
	}
	//printk("mstar_mci_setup_descriptor_addr3 --> %08X\n", u32DescptAddr);

	pCmdRspInfo->DataPath = PATH_ADMA;
	pCmdRspInfo->MiuAddr = u32DescptAddr;
	pCmdRspInfo->pBufferAddr = NULL; // DMA don't need this field
	pCmdRspInfo->BlockSize = 512;
	pCmdRspInfo->BlockCount = 1;

}

#else

static U32 mstar_mci_setup_dma_info(struct mmc_data * pData, CMD_RSP_INFO * pCmdRspInfo)
{
	U32 u32BusAddr, u32DeviceAddr, u32DmaLeng = 0;
	struct scatterlist  *pScatterList = 0;

	BUG_ON(pData->sg_len!=1);

	pScatterList = &(pData->sg[0]);
	u32BusAddr = sg_dma_address(pScatterList);
	u32DmaLeng = sg_dma_len(pScatterList);

	BUG_ON((u32DmaLeng>>9)&0xFFFFF000);

	// TODO: move to eMMC_clippers_platform.c, project specific
	//printk("SG[%d] %d sector, BA = %08Xh", u32DmaLeng>>9, u32BusAddr);
	if( u32BusAddr >= MSTAR_MIU1_BUS_BASE) {
		u32DeviceAddr = u32BusAddr - MSTAR_MIU1_BUS_BASE;
		//REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		//printk(" - %08Xh(MIU1) = %08Xh\n", MSTAR_MIU1_BUS_BASE , u32DeviceAddr);
	} else {
		u32DeviceAddr = u32BusAddr - MSTAR_MIU0_BUS_BASE;
		//REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		//printk(" - %08Xh(MIU0) = %08Xh\n", MSTAR_MIU0_BUS_BASE , u32DeviceAddr);
	}

	pCmdRspInfo->DataPath = PATH_DMA;
	pCmdRspInfo->MiuAddr = u32DeviceAddr;
	pCmdRspInfo->pBufferAddr = NULL; // DMA don't need this field
	pCmdRspInfo->BlockSize = 512;
	pCmdRspInfo->BlockCount = u32DmaLeng>>9;

	return u32DmaLeng;
}

#endif




/*static U32 mstar_mci_check_rsp_tran_sts(struct mmc_command * cmd)
{
	unsigned int u32Status;

	u32Status = REG_FCIE(FCIE_SD_STATUS) & BIT_SD_FCIE_ERR_FLAGS; // 0x1F
	if(u32Status)
	{
		if((u32Status & BIT_SD_RSP_CRC_ERR) && !(mmc_resp_type(cmd) & MMC_RSP_CRC))
		{
			cmd->error = 0;
		}
		else
		{
			eMMC_debug(1, 1, "eMMC: STS: %04Xh, CMD:%u, retry:%u\n", u32Status, cmd->opcode, cmd->retries);
			//if(u32Status&BIT_SD_RSP_TIMEOUT) printk("no response\n");
			if(u32Status&BIT_SD_R_CRC_ERR  ) eMMC_debug(1, 1, "read data CRC error\n");
			if(u32Status&BIT_SD_W_FAIL     ) eMMC_debug(1, 1, "no CRC status latched\n");
			if(u32Status&BIT_SD_W_CRC_ERR  ) eMMC_debug(1, 1, "negtive CRC status latched\n");
			if(u32Status&BIT_SD_RSP_CRC_ERR) eMMC_debug(1, 1, "rsp CRC error\n");

			printk("CMD%02d_%08Xh\n", cmd->opcode, cmd->arg);
			printk("Backup CMD%02d_%08Xh\n", curhost->cmd_backup, curhost->arg_backup);

			//printk("gu32BusAddr = %Xh\n", gu32BusAddr);
			//dump_mem((U8 *)phys_to_virt(gu32BusAddr), 0x400);

			if(u32Status & BIT_SD_RSP_TIMEOUT)
			{
				eMMC_debug(1, 0, "no response %02Xh %02Xh %02Xh %02Xh %02Xh\n",
				eMMC_FCIE_CmdRspBufGet(0), eMMC_FCIE_CmdRspBufGet(1),
				eMMC_FCIE_CmdRspBufGet(2), eMMC_FCIE_CmdRspBufGet(3),
				eMMC_FCIE_CmdRspBufGet(4));
				cmd->error = -ETIMEDOUT; // THE_SAME_WARNING_IN_OTHER_HOST_DRV
				eMMC_FCIE_ErrHandler_Stop();
			}
			else if(u32Status & (BIT_SD_RSP_CRC_ERR | BIT_SD_R_CRC_ERR | BIT_SD_W_CRC_ERR | BIT_SD_W_FAIL))
			{
				cmd->error = -EILSEQ; // THE_SAME_WARNING_IN_OTHER_HOST_DRV
				//eMMC_DumpDebugInfo();
				if(cmd->data) {
					eMMC_printf("CMD%02d_%08Xh_%d\n", cmd->opcode, cmd->arg, cmd->data->blocks); // red color
					//eMMC_DebugDumpData(cmd->data);
				}
				eMMC_die("\n");
			}
			else
			{
				cmd->error = -EIO; // THE_SAME_WARNING_IN_OTHER_HOST_DRV
			}
		}
	}
	else
	{
		cmd->error = 0;
	}

	eMMC_FCIE_ClearTranErr();

	return u32Status;
}

static void mstar_mci_copy_rsp_back(struct mmc_command * cmd)
{
	U8 i, u8RspByteNum;
	U8 * pTemp;

	pTemp = (U8*)&(cmd->resp[0]);

	#if defined(PRINT_REQUEST_INFO)&&PRINT_REQUEST_INFO
	printk("\tRSP: %02X ", eMMC_FCIE_CmdRspBufGet(0));
	#endif
	if(cmd->flags&MMC_RSP_136) {
		u8RspByteNum = 16;
	} else {
		u8RspByteNum = 4;
	}
	for(i=0; i<u8RspByteNum; i++) {
		pTemp[ (3-(i%4)) + ((i/4)<<2) ] = eMMC_FCIE_CmdRspBufGet((U8)(i+1));

		if(cmd->opcode == 2) {
			g_eMMCDrv.au8_CID[i] = eMMC_FCIE_CmdRspBufGet((U8)(i+1));
			printk("%02X ", eMMC_FCIE_CmdRspBufGet((U8)(i+1)));
		}

		#if defined(PRINT_REQUEST_INFO)&&PRINT_REQUEST_INFO
		printk("%02X ", eMMC_FCIE_CmdRspBufGet((U8)(i+1)));
		#endif
	}
	#if defined(PRINT_REQUEST_INFO)&&PRINT_REQUEST_INFO
	if( (mmc_resp_type(cmd)==MMC_RSP_R1) || (mmc_resp_type(cmd)==MMC_RSP_R1B) ) { // with card state
		printk("(%d)\n", (cmd->resp[0]&0x1E00)>>9);
	} else {
		printk("\n");
	}
	#endif

	#if 0
	for(i = 0; i < 15; i++)
	{
		pTemp[(3 - (i % 4)) + (4 * (i / 4))] =
			(U8)(REG_FCIE(FCIE1_BASE+(((i+1)/2)*4)) >> (8*((i+1)%2)));
	}
	#endif

	//eMMC_printf("RSP 4 CMD%02d: %08Xh(%d)\n", cmd->opcode, cmd->resp[0], (cmd->resp[0]&0x1E00)>>9);
}*/

static int mstar_mci_get_dma_dir(struct mmc_data *data)
{
	if (data->flags & MMC_DATA_WRITE)
		return DMA_TO_DEVICE;
	else
		return DMA_FROM_DEVICE;
}

static int mstar_mci_pre_dma_transfer(struct mstar_mci_host *pMStarHost_st, struct mmc_data *data, struct mstar_mci_host_next *next)
{
	U32 dma_len;

	/*if (!next && data->host_cookie &&
	data->host_cookie != pMStarHost_st->next_data.mstar_cookie) {
		pr_warning("[%s] invalid cookie: data->host_cookie %d"
			" pMStarHost_st->next_data.mstar_cookie %d\n",
			__func__, data->host_cookie, pMStarHost_st->next_data.mstar_cookie);
		data->host_cookie = 0;
	}*/

	/*if(!next && data->host_cookie != host->next_data.mstar_cookie)
	{
		printk("no cookie map, cmd->opcode = %d\n", host->request->cmd->opcode);
		printk("host_cookie = %d, mstar_cookie = %d\n", data->host_cookie, host->next_data.mstar_cookie);
	}*/

	if (next || (!next && data->host_cookie != pMStarHost_st->next_data.mstar_cookie)) {

		dma_len = (U32)dma_map_sg(mmc_dev(pMStarHost_st->mmc), data->sg, (int)data->sg_len, mstar_mci_get_dma_dir(data));

	} else {

		dma_len = pMStarHost_st->next_data.dma_len;
		pMStarHost_st->next_data.dma_len = 0;
	}

	if (dma_len == 0)
		return -EINVAL;

	if (next) {
		next->dma_len = dma_len;
		data->host_cookie = ++next->mstar_cookie < 0 ? 1 : next->mstar_cookie;
	}
	// else
	//	pMStarHost_st->dma_len = dma_len;

	return 0;
}

#if 0 //defined (ASYNCIO_SUPPORT) && ASYNCIO_SUPPORT

static void mstar_mci_pre_req(struct mmc_host *pMMCHost_st, struct mmc_request *mrq, bool is_first_req)
{
	struct mstar_mci_host *pMStarHost_st = mmc_priv(pMMCHost_st);

	//eMMC_debug(eMMC_DEBUG_LEVEL_LINUX, 1, "\n");

	//mstar_mci_schedule_tasklet_test();

	#if defined(PERF_PROFILE)&&PERF_PROFILE
	if(mrq->cmd->opcode==18) {
		if (pMStarHost_st->profile_fcie.idx_pre_req == 0) {
			eMMC_hw_timer_stop(); eMMC_hw_timer_start();
		}
		if(pMStarHost_st->profile_fcie.idx_pre_req < REC_REQUEST_NUM) {
			pMStarHost_st->profile_fcie.req[pMStarHost_st->profile_fcie.idx_pre_req].pre_request.t_begin = eMMC_TimerGetUs();
		}
	}
	#endif

	if( mrq->data->host_cookie ) {
		mrq->data->host_cookie = 0;
		return;
	}

	//eMMC_GPIO_Debug(114, 1);

	if (mstar_mci_pre_dma_transfer(pMStarHost_st, mrq->data, &pMStarHost_st->next_data))
		mrq->data->host_cookie = 0; // clear cookie when fail

	//eMMC_GPIO_Debug(114, 0);

	#if defined(PERF_PROFILE)&&PERF_PROFILE
	if(mrq->cmd->opcode==18) {
		if(pMStarHost_st->profile_fcie.idx_pre_req < REC_REQUEST_NUM) {
			pMStarHost_st->profile_fcie.req[pMStarHost_st->profile_fcie.idx_pre_req++].pre_request.t_end = eMMC_TimerGetUs();
		}
	}
	#endif

}

static void mstar_mci_post_req(struct mmc_host *pMMCHost_st, struct mmc_request *mrq, int err)
{
	struct mstar_mci_host *pMStarHost_st = mmc_priv(pMMCHost_st);
	struct mmc_data *pData = mrq->data;

	#if defined(PERF_PROFILE)&&PERF_PROFILE
	if(mrq->cmd->opcode==18) {
		if(pMStarHost_st->profile_fcie.idx_post_req < REC_REQUEST_NUM) {
			pMStarHost_st->profile_fcie.req[pMStarHost_st->profile_fcie.idx_post_req].post_request.t_begin = eMMC_TimerGetUs();
		}
	}
	#endif

	//eMMC_debug(eMMC_DEBUG_LEVEL_LINUX, 1, "\n");

	//mstar_mci_schedule_tasklet_test();
	//eMMC_GPIO_Debug(155, 1);

	if (pData->host_cookie) {
		dma_unmap_sg(mmc_dev(pMStarHost_st->mmc), pData->sg, (int)pData->sg_len, mstar_mci_get_dma_dir(pData));
	}

	//eMMC_GPIO_Debug(155, 0);

	pData->host_cookie = 0;

	#if defined(PERF_PROFILE)&&PERF_PROFILE
	if(mrq->cmd->opcode==18) {
		if(pMStarHost_st->profile_fcie.idx_post_req < REC_REQUEST_NUM) {
			pMStarHost_st->profile_fcie.req[pMStarHost_st->profile_fcie.idx_post_req++].post_request.t_end = eMMC_TimerGetUs();
		}
	}
	#endif
}

#endif // ASYNCIO_SUPPORT

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,13)
static void mstar_mci_reset(struct mmc_host *pMMCHost_st)
{
    //struct mstar_mci_host *pMStarHost_st = mmc_priv(mmc);

    eMMC_debug(1, 1, "\n");

    // reset eMMC with hardware pin
}
#endif

#if defined(PERF_PROFILE)&&PERF_PROFILE

void mstar_mci_log_irq_time_stamp(void)
{

    if(curhost->req_dtc->cmd->opcode==18) {
        if(curhost->profile_fcie.idx_request < REC_REQUEST_NUM) {
            curhost->profile_fcie.req[curhost->profile_fcie.idx_request].transfer.t_irq_happen = eMMC_TimerGetUs();
        }
    }

}

EXPORT_SYMBOL(mstar_mci_log_irq_time_stamp);

#endif

// do the reinit & retry here...

static U32 mstar_mci_send_command_ex(CMD_RSP_INFO * pCmdRspInfo)
{
	U32 u32_err = eMMC_ST_SUCCESS;
	U8 u8CmdIdx;
	U8 u8Retry = 3;
	U8 u8ReadWriteCmd = 0;
	static U8 u8CurrentSpeed = 0xFF;
	static U8 u8SuccessTime = 0;

	//static U8 u8ErrorRetryTest = 1;
	//static U8 u8TempCounter = 0;


	u8CmdIdx = pCmdRspInfo->Command.Cmd.Index & (~_START_TXMIT);

	if( u8CmdIdx==17 || u8CmdIdx==18 || u8CmdIdx==24 || u8CmdIdx==25 )
	{
		u8ReadWriteCmd = 1;
	}

	if(u8ReadWriteCmd) // read write command
	{
		if( u8SuccessTime==MCI_RETRY_CNT_OK_CLK_UP )
		{
			eMMC_debug(1, 0, "\33[1;31meMMC: restore speed\33[m\n");
			eMMC_FCIE_EnableFastMode(u8CurrentSpeed); // restore speed
			u8CurrentSpeed = 0xFF;
			u8SuccessTime = 0;
		}

		AccessAgain:

	    u32_err = HalFcie_SendCommand(pCmdRspInfo);

		#if 0 // error retry test use only
		if( u8CmdIdx==18 && u8ErrorRetryTest )
		{
			u8TempCounter++;
			if(u8TempCounter==3)
			{
				u32_err = 1;
				u8ErrorRetryTest = 0;
			}
		}
		#endif

		if(u32_err) // fail
		{
			if(u8Retry--)
			{
				u8CurrentSpeed = g_eMMCDrv.u8_PadType; // back up current speed

				// reset IP & reinit eMMC to high speed

			    printk("\33[1;31meMMC Warn: retry access\33[m\n");

				eMMC_FCIE_ErrHandler_ReInit();

				goto AccessAgain;
			}
			else
			{
				eMMC_FCIE_ErrHandler_Stop();
			}
		}
		else // success
		{
			if(u8Retry!=3) // success after retry
			{
				printk("\33[1;31meMMC: retry access OK\33[m\n");
			}
			else // success without retry
			{
				if( u8CurrentSpeed!=0xFF && u8Retry==3 ) // record success times for restore speed
				{
					u8SuccessTime++;
				}
			}
		}
	}
	else // others command
	{
		ReSendCommand:

	    u32_err = HalFcie_SendCommand(pCmdRspInfo);

		#if 0 // error retry test use only
		if( u8ErrorRetryTest && u8CmdIdx==6 )
		{
			u32_err = 1;
			u8ErrorRetryTest = 0;
		}
		#endif

		if(u32_err)
		{
			if(u8Retry--)
			{
				printk("eMMC Warn: retry fail CMD%d\n", u8CmdIdx);
				goto ReSendCommand;
			}
			else
			{
				eMMC_FCIE_ErrHandler_Stop();
			}
		}
		else if(u8Retry!=3)
		{
			printk("eMMC: retry cmd OK\n");
		}
	}

	return u32_err;
}

static U32 mstar_mci_send_command(struct mmc_command * pCmd)
{
	U32 u32_err = eMMC_ST_SUCCESS;
    CMD_RSP_INFO CmdRspInfo= {0};
	U8 * pTemp;
	U8 u8RspByteNum;
	U8 u8_i;
	//U32 u32BusAddr, u32DeviceAddr, u32DmaLeng = 0;
	//struct scatterlist  *pScatterList = 0;
	U8 u8_retry = 0;

	CmdRspInfo.Command.Cmd.Index = pCmd->opcode | 0x40;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(pCmd->arg);

	switch(mmc_cmd_type(pCmd)) {
		case MMC_CMD_ADTC:
			//printk("MMC_CMD_ADTC, ");
			CmdRspInfo.CmdType = CMD_TYPE_ADTC;
			break;
		case MMC_CMD_AC:
			//printk("MMC_CMD_AC, ");
			CmdRspInfo.CmdType = CMD_TYPE_AC;
			break;
		case MMC_CMD_BC:
			//printk("MMC_CMD_BC, ");
			CmdRspInfo.CmdType = CMD_TYPE_BC;
			break;
		case MMC_CMD_BCR:
			//printk("MMC_CMD_BCR, ");
			CmdRspInfo.CmdType = CMD_TYPE_BC;
			break;
		default:
			printk("cmd->flags = %08Xh, ", pCmd->flags);
			break;
	}

	switch(mmc_resp_type(pCmd)) {
		case MMC_RSP_R1:
		//case MMC_RSP_R5:
		//case MMC_RSP_R6:
		//case MMC_RSP_R7:
			CmdRspInfo.RspType = RSP_TYPE_R1;
			break;
		case MMC_RSP_NONE:
			CmdRspInfo.RspType = RSP_TYPE_NO;
			break;

		case MMC_RSP_R1B:
			CmdRspInfo.RspType = RSP_TYPE_R1b;
			break;
		case MMC_RSP_R2:
			CmdRspInfo.RspType = RSP_TYPE_R2;
			break;
		case MMC_RSP_R3:
		//case MMC_RSP_R4:
			CmdRspInfo.RspType = RSP_TYPE_R3;
			break;
		default:
			printk("eMMC Err: not support response type");
			break;
	}

	if(pCmd->data) // command with data transfer
	{
		switch(pCmd->data->flags) {
			case MMC_DATA_WRITE:
				//printk("MMC_DATA_WRITE, ");
				CmdRspInfo.ReadWriteDir = DIR_W;
				break;
			case MMC_DATA_READ:
				//printk("MMC_DATA_READ, ");
				CmdRspInfo.ReadWriteDir = DIR_R;
				break;
			case MMC_DATA_STREAM:
			default:
				printk("eMMC Err: MMC_DATA_STREAM, ");
				printk("eMMC Err: data->flags = %08Xh, ", pCmd->data->flags);
				break;
		}

		switch(g_eMMCDrv.u8_BUS_WIDTH)
		{
			case BUS_WIDTH_1:
				CmdRspInfo.BusWidth = BUS_1_BIT;
				//printk("1 bits\n");
				break;
			case BUS_WIDTH_4:
				CmdRspInfo.BusWidth = BUS_4_BITS;
				//printk("4 bits\n");
				break;
			case BUS_WIDTH_8:
				CmdRspInfo.BusWidth = BUS_8_BITS;
				//printk("8 bits\n");
				break;
		}

		#if defined(ENABLE_eMMC_ADMA_MODE) && ENABLE_eMMC_ADMA_MODE
			pCmd->data->bytes_xfered = mstar_mci_prepare_descriptors(pCmd->data);
			mstar_mci_setup_descriptor_addr(&CmdRspInfo);
		#else
			pCmd->data->bytes_xfered = mstar_mci_setup_dma_info(pCmd->data, &CmdRspInfo);
		#endif

	}


    u32_err = mstar_mci_send_command_ex(&CmdRspInfo); // do all the job here . . . . .

	if(u32_err)
	{
		pCmd->error = -EIO;

		if(pCmd->data) // command with data transfer
		{
			pCmd->data->bytes_xfered = 0;
		}

	    #if 0
		if(u8_retry_cmd < eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT && 0==eMMC_IF_TUNING_TTABLE())
		{
			u8_retry_cmd++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC WARN: CMD18 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_SEND_CMD;
		}
        #endif
		eMMC_debug(1, 0, "eMMC Err: request fail retry:%u, %Xh \n", u8_retry, u32_err);
		//eMMC_FCIE_ErrHandler_Stop();
		goto ErrorHandle;
	}
	else
	{
		pCmd->error = 0;

		pTemp = (U8*)&(pCmd->resp[0]);
		u8RspByteNum = (pCmd->flags&MMC_RSP_136)?16:4;
		for(u8_i=0; u8_i<u8RspByteNum; u8_i++) {
			pTemp[ (3-(u8_i%4)) + ((u8_i/4)<<2) ] = eMMC_FCIE_CmdRspBufGet((U8)(u8_i+1));
		}
	}

	return eMMC_ST_SUCCESS;

ErrorHandle:

	return u32_err;

}



static void mstar_mci_request(struct mmc_host *pMMCHost_st, struct mmc_request *mrq)
{
	struct mstar_mci_host *pMStarHost_st;

	//printk("\33[1;31m request %d \33[m\n", __LINE__);

	if (!pMMCHost_st)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: pMMCHost_st is NULL\n");
		return;
	}
	if (!mrq)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mrq is NULL\n");
		return;
	}

	eMMC_LockFCIE(NULL);

	pMStarHost_st = mmc_priv(pMMCHost_st);

	if( (pMMCHost_st->caps&MMC_CAP_CMD23) && (mrq->sbc) ) {
		mstar_mci_send_command(mrq->sbc); // CMD23
	}

	if(mrq->data){
		// dma_map_sg in non async io config
		mstar_mci_pre_dma_transfer(pMStarHost_st, mrq->data, NULL);
	}

	// skip SDIO / SD related command
	if( mrq->cmd->opcode==52 ||  mrq->cmd->opcode==5 || mrq->cmd->opcode==55 || (mrq->cmd->opcode==8 && mrq->data == NULL) ) {
		mrq->cmd->error = -EIO;
		mmc_request_done(pMMCHost_st, mrq); // call back to notify request done
		return;
	}

	mstar_mci_send_command(mrq->cmd); // simple command or data transfer command

	// dma_ummap_sg in non async io config
	if(mrq->data){
		if(!mrq->data->host_cookie) {
			dma_unmap_sg(mmc_dev(pMStarHost_st->mmc), mrq->data->sg, (int)mrq->data->sg_len, mstar_mci_get_dma_dir(mrq->data));
		}
	}

	if( (!(pMMCHost_st->caps&MMC_CAP_CMD23)) && (mrq->stop)) {
		mstar_mci_send_command(mrq->stop); // CMD12
	}

	eMMC_UnlockFCIE(NULL);

	mmc_request_done(pMMCHost_st, mrq); // call back to notify request done

}

//#define DBG_SET_IOS

static void mstar_mci_set_ios(struct mmc_host *pMMCHost_st, struct mmc_ios *ios)
{
	/* Define Local Variables */
	struct mstar_mci_host *pMStarHost_st = mmc_priv(pMMCHost_st);
	static unsigned char last_bus_width = 0xFF;
	static unsigned int last_clock = 0xFFFFFFFF;
	static unsigned char last_timing = 0xFF;

	//printk("\33[1;36m mstar_mci_set_ios() \33[m \n");

	if (!pMMCHost_st)
	{
		eMMC_debug(1, 1, "eMMC Err: mmc is NULL \n");
		return;
	}

	if (!ios)
	{
		eMMC_debug(1, 1, "eMMC Err: ios is NULL \n");
		return;
	}

	eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0, "eMMC: clock: %u, bus_width %Xh \n", ios->clock, ios->bus_width);

	eMMC_LockFCIE(NULL);

	// ----------------------------------
	if(ios->clock!=last_clock) {

		last_clock = ios->clock;

		#ifdef DBG_SET_IOS
		printk("\33[1;36mset clock to %d \33[m \n", ios->clock);
		#endif

		if (ios->clock == 0)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0, "eMMC Warn: disable clk \n");
			eMMC_clock_gating();
		}
		else
		{
			if(ios->clock <= 400000)
			{
				eMMC_clock_setting(FCIE_SLOWEST_CLK);
			}
			else if(ios->clock <= 26000000)
			{
				eMMC_clock_setting(FCIE_DEFO_SPEED_CLK);
			}
			else
			{
				eMMC_clock_setting(FCIE_HIGH_SPEED_CLK);
				eMMC_pads_switch(EMMC_HIGH_SPEED_MODE);
			}
		}
	}

	// ----------------------------------
	if(ios->bus_width!=last_bus_width) {
		last_bus_width = ios->bus_width;

		switch(ios->bus_width) {
			case MMC_BUS_WIDTH_1:
				g_eMMCDrv.u8_BUS_WIDTH = BUS_WIDTH_1;
				break;
			case MMC_BUS_WIDTH_4:
				g_eMMCDrv.u8_BUS_WIDTH = BUS_WIDTH_4;
				break;
			case MMC_BUS_WIDTH_8:
				g_eMMCDrv.u8_BUS_WIDTH = BUS_WIDTH_8;
				// load timing tuning parameter here...

				break;
		}
		//printk("\33[1;36mset bus_width to %d bit(s)\33[m \n", 1<<ios->bus_width);
	}


	// ----------------------------------
	if(ios->timing!=last_timing) {

		last_timing = ios->timing;

		#ifdef DBG_SET_IOS
		printk("\33[1;36mset timing to (%d) ", ios->timing);
		switch(ios->timing) {
			case MMC_TIMING_LEGACY:		printk("default speed\n\33[m");	break; // 0
			case MMC_TIMING_MMC_HS:		printk("high speed\n\33[m");	break; // 1
			case MMC_TIMING_UHS_DDR50:	printk("DDR52\n\33[m");			break; // 7
			case MMC_TIMING_MMC_HS200:	printk("HS200\n\33[m");			break; // 8
			case MMC_TIMING_MMC_HS400:	printk("HS400\n\33[m");			break; // 9
		}
		#endif

		switch(pMStarHost_st->mmc->ios.timing)
		{
			case MMC_TIMING_LEGACY:
				eMMC_pads_switch(EMMC_DEFO_SPEED_MODE);
				break;

			case MMC_TIMING_MMC_HS:
				//eMMC_clock_setting(FCIE_HIGH_SPEED_CLK);
				//eMMC_pads_switch(EMMC_HIGH_SPEED_MODE);
				break;

			case MMC_TIMING_UHS_DDR50:
				g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_DDR_MODE;
				eMMC_pads_switch(EMMC_DDR52_MODE);
				break;

			case MMC_TIMING_MMC_HS200:
				g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_SPEED_HS200;
				eMMC_pads_switch(EMMC_HS200_MODE);
				break;
			case MMC_TIMING_MMC_HS400:
				g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_SPEED_HS400;
				eMMC_pads_switch(EMMC_HS400_MODE);
				break;
		}
	}

	eMMC_UnlockFCIE(NULL);

}

static s32 mstar_mci_get_ro(struct mmc_host *pMMCHost_st)
{
	s32 read_only = 0;

	if(!pMMCHost_st)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mmc is NULL \n");
		read_only = -EINVAL;
	}

	return read_only;
}

#if 0
int mstar_mci_check_D0_status(void)
{
    u16 u16Reg = 0;

    u16Reg = REG_FCIE(FCIE_SD_STATUS);

    if(u16Reg & BIT_SD_D0 )
        return 1;
    else
        return 0;
}

//EXPORT_SYMBOL(mstar_mci_check_D0_status);
#endif

static void mstar_mci_enable(struct mstar_mci_host *pMStarHost_st)
{
	u32 u32_err;

	u32_err = eMMC_FCIE_Init();

	if(eMMC_ST_SUCCESS != u32_err)
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1, "eMMC Err: eMMC_FCIE_Init fail: %Xh \n", u32_err);
}

static void mstar_mci_disable(void)
{
	u32 u32_err;

	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");

	eMMC_clock_setting(FCIE_DEFAULT_CLK); // enable clk

	u32_err = eMMC_FCIE_Reset();
	if(eMMC_ST_SUCCESS != u32_err)
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1, "eMMC Err: eMMC_FCIE_Reset fail: %Xh\n", u32_err);

	eMMC_clock_gating();
}

#ifdef CONFIG_DEBUG_FS

//#if defined(MMC_SPEED_TEST) && MMC_SPEED_TEST
static int mstar_mci_perf_show(struct seq_file *seq, void *v)
{
    #if defined(PERF_PROFILE)&&PERF_PROFILE

    U32 i;

    for(i=0; i < REC_REQUEST_NUM; i++) {
        if(!curhost->profile_fcie.req[i].sector.length) {
            printk("break at %d\n", i);
            break;
        }
        printk(", %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
                                            curhost->profile_fcie.req[i].sector.addr,
                                            curhost->profile_fcie.req[i].sector.length,

                                            curhost->profile_fcie.req[i].pre_request.t_begin,
                                            curhost->profile_fcie.req[i].pre_request.t_end,

                                            curhost->profile_fcie.req[i].request.t_begin,
                                            curhost->profile_fcie.req[i].transfer.t_start_dma,
                                            curhost->profile_fcie.req[i].transfer.t_bh_run,
                                            curhost->profile_fcie.req[i].transfer.t_irq_happen,
                                            curhost->profile_fcie.req[i].request.t_end,

                                            curhost->profile_fcie.req[i].post_request.t_begin,
                                            curhost->profile_fcie.req[i].post_request.t_end  );
    }

    __memzero(&curhost->profile_fcie, sizeof(struct profile_hcd));

    #endif

    return 0;
}

static int mstar_mci_perf_open(struct inode *inode, struct file *file)
{
	return single_open(file, mstar_mci_perf_show, inode->i_private);
}

static const struct file_operations mstar_mci_fops_perf = {
	.owner		= THIS_MODULE,
	.open		= mstar_mci_perf_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
//#endif

static void mstar_mci_debugfs_attach(struct mstar_mci_host *pMStarHost_st)
{
	struct device *dev = mmc_dev(pMStarHost_st->mmc);

	pMStarHost_st->debug_root = debugfs_create_dir(dev_name(dev), NULL);

	if (IS_ERR(pMStarHost_st->debug_root)) {
		dev_err(dev, "failed to create debugfs root\n");
		return;
	}

	// #if defined(MMC_SPEED_TEST) && MMC_SPEED_TEST
	pMStarHost_st->debug_perf = debugfs_create_file("fcie_performance", 0444,
					       pMStarHost_st->debug_root, pMStarHost_st,
					       &mstar_mci_fops_perf);

	if (IS_ERR(pMStarHost_st->debug_perf))
		dev_err(dev, "failed to create debug regs file\n");
	//#endif
}

static void mstar_mci_debugfs_remove(struct mstar_mci_host *pMStarHost_st)
{
	#if defined(MMC_SPEED_TEST) && MMC_SPEED_TEST
		debugfs_remove(pMStarHost_st->debug_perf);
	#endif
	debugfs_remove(pMStarHost_st->debug_root);
}

#else

static inline void mstar_mci_debugfs_attach(struct mstar_mci_host *pMStarHost_st) { }
static inline void mstar_mci_debugfs_remove(struct mstar_mci_host *pMStarHost_st) { }

#endif

/* MSTAR Multimedia Card Interface Operations */
static const struct mmc_host_ops mstar_mci_ops =
{
	//.pre_req =  mstar_mci_pre_req,

	//.post_req = mstar_mci_post_req,

#if 0 // defined (HPI_SUPPORT) && HPI_SUPPORT
	.abort_req = mstar_mci_abort_req,
#endif

    #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,13)
	.hw_reset = mstar_mci_reset,
	#endif

	.request =	mstar_mci_request,

	.set_ios =	mstar_mci_set_ios,

	.get_ro =	mstar_mci_get_ro,
};

static s32 mstar_mci_probe(struct platform_device *dev)
{
	struct mmc_host *pMMCHost_st = 0;
	struct mstar_mci_host *pMStarHost_st = 0;
	s32 s32_ret = 0;

	//eMMC_debug(eMMC_DEBUG_LEVEL_HIGH, 0,
	//		 "\33[1;31meMMC test built at %s on %s\33[m\n", __TIME__, __DATE__);

	eMMC_LockFCIE(NULL);

	#if defined(ENABLE_eMMC_ADMA_MODE) && ENABLE_eMMC_ADMA_MODE
		g_ADMAInfo.DescriptorAddr = (U32)g_ADMAInfo.Descriptor;
		//printk("MIU0 BASE = %Xh, ", MSTAR_MIU0_BUS_BASE);
		//printk("MIU1 BASE = %Xh\n", MSTAR_MIU1_BUS_BASE);
	#endif

	eMMC_PlatformInit();

	//printk("platform_device pointer = %Xh\n", (U32)dev);

	mstar_mci_enable(pMStarHost_st);


	eMMC_UnlockFCIE(NULL);

	if (!dev) {
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: dev is NULL \n");
		s32_ret = -EINVAL;
		goto LABEL_END;
	}

	//printk("pMMCHost_st = %Xh, ", (U32)pMMCHost_st);

	pMMCHost_st = mmc_alloc_host(sizeof(struct mstar_mci_host), &dev->dev);
	if (!pMMCHost_st)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mmc_alloc_host fail \n");
		s32_ret = -ENOMEM;
		goto LABEL_END;
	}

	//printk("--> %Xh\n", (U32)pMMCHost_st);

	//printk("\33[1;31m %d \33[m\n", __LINE__);

	pMMCHost_st->ops = &mstar_mci_ops;
	pMMCHost_st->f_min = CLK_300KHz;
	pMMCHost_st->f_max = CLK_200MHz;
	pMMCHost_st->ocr_avail =
		MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31 |
		MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;

	pMMCHost_st->max_blk_count = BIT_SD_JOB_BLK_CNT_MASK; // 4095
	pMMCHost_st->max_blk_size = 512; /* sector */
	pMMCHost_st->max_req_size = pMMCHost_st->max_blk_count * pMMCHost_st->max_blk_size;
	pMMCHost_st->max_seg_size = pMMCHost_st->max_req_size;



#if defined(ENABLE_eMMC_ADMA_MODE) && ENABLE_eMMC_ADMA_MODE
	pMMCHost_st->max_segs           = 512;
	//printk("\33[1;35mEnable ADMA\33[m\n");
#else
	pMMCHost_st->max_segs           = 1;
#endif


	pMMCHost_st->caps =	MMC_CAP_8_BIT_DATA | MMC_CAP_MMC_HIGHSPEED | MMC_CAP_NONREMOVABLE | MMC_CAP_ERASE;

	//pMMCHost_st->caps |= MMC_CAP_CMD23;

	//pMMCHost_st->caps |= MMC_CAP_1_8V_DDR | MMC_CAP_UHS_DDR50;	// DDR52
	//pMMCHost_st->caps2 |= MMC_CAP2_HS200_1_8V_SDR;				// HS200
	//pMMCHost_st->caps2 |= MMC_CAP2_HS400_1_8V_DDR;				// HS400

	//---------------------------------------------------------

	pMStarHost_st                   = mmc_priv(pMMCHost_st);
	pMStarHost_st->mmc              = pMMCHost_st;
	curhost                         = pMStarHost_st;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
	pMStarHost_st->next_data.mstar_cookie = 1;
#endif

#if defined (ASYNCIO_SUPPORT) && ASYNCIO_SUPPORT
	INIT_WORK(&pMStarHost_st->workqueue_dtc, mstar_mci_dtc_bh);
#endif

	pMStarHost_st->irq_lock = __SPIN_LOCK_UNLOCKED(pMStarHost_st->irq_lock);

	//sema_init(&pMStarHost_st->sem_aborted_req_go_through, 0);

	//pMStarHost_st->abort_success = 0;

	//pMStarHost_st->SpeedUpeMMCResume = 0;

	//---------------------------------------------------------

	//printk("\33[1;31m %d \33[m\n", __LINE__);

	mmc_add_host(pMMCHost_st);

	//printk("\33[1;31m %d \33[m\n", __LINE__);

#ifdef CONFIG_DEBUG_FS
	mstar_mci_debugfs_attach(pMStarHost_st);
#endif

	platform_set_drvdata(dev, pMMCHost_st);

#if (defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE) || \
	(defined(ENABLE_FCIE_HW_BUSY_CHECK)&&ENABLE_FCIE_HW_BUSY_CHECK)
	//s32_ret = request_irq(E_IRQ_NFIE, eMMC_FCIE_IRQ, 0, DRIVER_NAME, pMStarHost_st);
	s32_ret = request_irq(E_IRQ_NFIE, eMMC_FCIE_IRQ, IRQF_DISABLED, DRIVER_NAME, pMStarHost_st);
	if (s32_ret)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: request_irq fail \n");
		mmc_free_host(pMMCHost_st);
		goto LABEL_END;
	}
#endif

LABEL_END:

	if(s32_ret) eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: %d\n", s32_ret);

	return 0;
}

static s32 __exit mstar_mci_remove(struct platform_device *dev)
{
    /* Define Local Variables */
    struct mmc_host *pMMCHost_st = platform_get_drvdata(dev);
    struct mstar_mci_host *pMStarHost_st = mmc_priv(pMMCHost_st);
    s32 s32_ret = 0;

    eMMC_debug(1, 1, "\n");

    eMMC_LockFCIE(NULL);

    if (!dev)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: dev is NULL\n");
        s32_ret = -EINVAL;
        goto LABEL_END;
    }

    if (!pMMCHost_st)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: mmc is NULL\n");
        s32_ret= -1;
        goto LABEL_END;
    }

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, remove +\n");

    #ifdef CONFIG_DEBUG_FS
    mstar_mci_debugfs_remove(pMStarHost_st);
    #endif

    mmc_remove_host(pMMCHost_st);

    mstar_mci_disable();

    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    free_irq(E_IRQ_NFIE, pMStarHost_st);
    #endif

    mmc_free_host(pMMCHost_st);
    platform_set_drvdata(dev, NULL);

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, remove -\n");

    LABEL_END:

	eMMC_UnlockFCIE(NULL);

    return s32_ret;
}


#ifdef CONFIG_PM

void mstar_mci_hardware_reset(void)
{
	eMMC_RST_L();
	eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
	eMMC_RST_H();
	eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
}

void mstar_mci_normal_resume(void)
{
	eMMC_LockFCIE(NULL);

	eMMC_PlatformInit();

	mstar_mci_enable(curhost);

	mstar_mci_hardware_reset();

	eMMC_UnlockFCIE(NULL);
}

static s32 mstar_mci_suspend(struct platform_device *dev, pm_message_t state)
{
    /* Define Local Variables */
    struct mmc_host *pMMCHost_st = platform_get_drvdata(dev);
    s32 ret = 0;

    if (pMMCHost_st)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, suspend +\n");
        ret = mmc_suspend_host(pMMCHost_st);
    }

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, suspend -, %Xh\n", ret);

    return ret;
}

static s32 mstar_mci_resume(struct platform_device *dev)
{
	struct mmc_host *pMMCHost_st = platform_get_drvdata(dev);
	s32 ret = 0;

	if(!curhost->SpeedUpeMMCResume) {

		mstar_mci_normal_resume();
	}

	if (pMMCHost_st)
	{
		ret = mmc_resume_host(pMMCHost_st);
	}

	return ret;
}

#endif  /* End ifdef CONFIG_PM */

/******************************************************************************
 * Define Static Global Variables
 ******************************************************************************/
static struct platform_driver mstar_mci_driver =
{
    .probe = mstar_mci_probe,
    .remove = __exit_p(mstar_mci_remove),

    #ifdef CONFIG_PM
    .suspend = mstar_mci_suspend,
    .resume = mstar_mci_resume,
    #endif

    .driver  =
    {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    },
};

u64 mstar_dma_mask = BLK_BOUNCE_ANY; // to access high memory directly without bounce

struct platform_device mstar_mci_device =
{
    .name =	DRIVER_NAME,
    .id = 0,
    .resource =	NULL,
    .num_resources = 0,
	.dev = {
		.dma_mask = &mstar_dma_mask,
	},
};


/******************************************************************************
 * Init & Exit Modules
 ******************************************************************************/
static s32 __init mstar_mci_init(void)
{
    int err = 0;
    //eMMC_debug(1 ,1,"\n");

    if((err = platform_device_register(&mstar_mci_device)) < 0)
        eMMC_debug(1 ,1,"eMMC Err: platform_driver_register fail, %Xh\n", err);

    if((err = platform_driver_register(&mstar_mci_driver)) < 0)
        eMMC_debug(1 ,1,"eMMC Err: platform_driver_register fail, %Xh\n", err);

    return err;
}

static void __exit mstar_mci_exit(void)
{
    platform_driver_unregister(&mstar_mci_driver);
}

void mstar_mci_dump_debug_msg(void)
{
	printk("\n[%s]\n", __FUNCTION__);

	//eMMC_DumpDebugInfo();

	if(curhost->req_cmd) {
		eMMC_printf("\33[1;31m Last CMD: CMD%02d_%08Xh\33[m\n", curhost->req_cmd->cmd->opcode, curhost->req_cmd->cmd->arg);
	}
	if(curhost->req_dtc) {
		eMMC_printf("\33[1;31m Last DTC: CMD%02d_%08Xh\33[m\n", curhost->req_dtc->cmd->opcode, curhost->req_dtc->cmd->arg);
		//eMMC_DebugDumpData(curhost->req_dtc->data);
	}
}

subsys_initcall(mstar_mci_init);
//rootfs_initcall(mstar_mci_init);
module_exit(mstar_mci_exit);

MODULE_LICENSE("Proprietary");
MODULE_DESCRIPTION("Mstar Multimedia Card Interface driver");
MODULE_AUTHOR("Hill.Sung/Hungda.Wang");
