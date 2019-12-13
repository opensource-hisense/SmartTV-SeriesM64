/*
 * Copyright (c) 2010-2011 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * Unless otherwise stipulated in writing, any and all information contained
 * herein regardless in any format shall remain the sole proprietary of
 * MStar Semiconductor Inc. and be kept in strict confidence
 * ("MStar Confidential Information") by the recipient.
 * Any unauthorized act including without limitation unauthorized disclosure,
 * copying, use, reproduction, sale, distribution, modification, disassembling,
 * reverse engineering and compiling of the contents of MStar Confidential
 * Information is unlawful and strictly prohibited. MStar hereby reserves the
 * rights to any and all damages, losses, costs and expenses resulting therefrom.
 *
 * MStar's Audio Driver for ALSA.
 * Author: Darren Chen <darren.chen@mstarsemi.com>
 *
 * Kenya
 *
 */


/*
 * ============================================================================
 * Include Headers
 * ============================================================================
 */
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,1))
#include <linux/module.h>
#endif
#include <asm/io.h>

#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>

#include "mhal_audio.h"


/*
 * ============================================================================
 * Constant Definitions
 * ============================================================================
 */
#define _MAD_CHIP_MODEL    "MAD KENYA"

#define _MAD_PM_MODE_REG_BASE    0xbf000000
#define _MAD_NON_PM_MODE_REG_BASE    0xbf200000
#define _MAD_PHYSICAL_MEM_BASE    _MAD_PM_MODE_REG_BASE
#define _MAD_KERNEL_SPACE_MEMORY_OFFSET    0xA0000000

#define _MAD_BYTES_IN_LINE    16
#define _MAD_ADDR_CONVERTOR    0x1000
#define _MAD_MAILBOX_OFFSET    0x110000

#define _MAD_MAX_CHANNEL    2

#define _MAD_DSP2_DRAM_SIZE    0x0
#define _MAD_DSP2_TO_COMMON_DRAM_OFFSET    _MAD_DSP2_DRAM_SIZE

#define _MAD_PCM_PLAYBACK1_BASE_OFFSET    (0xC6000 + _MAD_DSP2_TO_COMMON_DRAM_OFFSET)
#define _MAD_PCM_PLAYBACK1_BUF_UNIT    4096
#define _MAD_PCM_PLAYBACK1_TOTAL_BUF_SIZE    0x20000 /* 128 KByte */
#define _MAD_PCM_PLAYBACK1_MIN_AVAIL    (_MAD_PCM_PLAYBACK1_TOTAL_BUF_SIZE >> 3) /* The minimal available size should be reserved */
#define _MAD_PCM_PLAYBACK1_HIGH_THRESHOLD    (_MAD_PCM_PLAYBACK1_TOTAL_BUF_SIZE - _MAD_PCM_PLAYBACK1_MIN_AVAIL)
#define _MAD_PCM_PLAYBACK1_BUF_SIZE    (_MAD_PCM_PLAYBACK1_TOTAL_BUF_SIZE >> 1) /* Only half size can be used */
#define _MAD_PCM_PLAYBACK1_PERIOD_SIZE    (_MAD_PCM_PLAYBACK1_BUF_SIZE >> 2)
#define _MAD_PCM_PLAYBACK1_QUEUE_SIZE    100 /* ms */

#define _MAD_READ_BYTE(_reg)    (*(volatile unsigned char*)(_reg))
#define _MAD_READ_WORD(_reg)    (*(volatile unsigned short*)(_reg))
#define _MAD_WRITE_BYTE(_reg, _val)    { (*((volatile unsigned char*)(_reg))) = (unsigned char)(_val); }
#define _MAD_WRITE_WORD(_reg, _val)    { (*((volatile unsigned short*)(_reg))) = (unsigned short)(_val); }
#define _MAD_R1BYTE(u32Addr, u8mask)    (_MAD_READ_BYTE (_MAD_PHYSICAL_MEM_BASE + ((u32Addr) << 1) - ((u32Addr) & 1)) & (u8mask))
#define _MAD_AU_AbsReadByte(u32Reg)    (_MAD_READ_BYTE (_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1) - ((u32Reg) & 1)))
#define _MAD_AU_AbsRead2Byte(u32Reg)    (_MAD_READ_WORD (_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1)) )
#define _MAD_AU_AbsWriteByte(u32Reg, u8Val) \
	do { \
		(_MAD_WRITE_BYTE(_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1) - ((u32Reg) & 1), u8Val)); \
	} while(0)
#define _MAD_AU_AbsWriteMaskByte(u32Reg, u8Mask, u8Val) \
	do { \
		(_MAD_WRITE_BYTE(_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1) - ((u32Reg) & 1), (_MAD_R1BYTE((u32Reg), 0xFF) & ~(u8Mask)) | ((u8Val) & (u8Mask)))); \
	} while(0)
#define _MAD_AU_AbsWrite2Byte(u32Reg, u16Val) \
	do { \
		(_MAD_WRITE_WORD(_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1), u16Val)); \
	} while(0)


/*
 * ============================================================================
 * Forward Declarations
 * ============================================================================
 */
/* Read/Write Register */
#if 0 /* It's not in used for the moment, might be TODO */
static unsigned char _mhal_alsa_read_byte(unsigned int u32RegAddr);
#endif
static unsigned short _mhal_alsa_read_reg(unsigned int u32RegAddr);
#if 0 /* It's not in used for the moment, might be TODO */
static void _mhal_alsa_write_byte(unsigned int u32RegAddr, unsigned char u8Val);
#endif
#if 0 /* It's not in used for the moment, might be TODO */
static void _mhal_alsa_write_mask_byte(unsigned int u32RegAddr, unsigned char u8Mask, unsigned char u8Val);
#endif
static void _mhal_alsa_write_reg(unsigned int u32RegAddr, unsigned short u16Val);
static void _mhal_alsa_write_mask_reg(unsigned int u32RegAddr, unsigned short u16Mask, unsigned short u16Val);
static unsigned int _mhal_alsa_get_device_status(void);

/* PCM Playback1 */
static int _mhal_alsa_pcm_playback1_init(void);
static int _mhal_alsa_pcm_playback1_exit(void);
static int _mhal_alsa_pcm_playback1_start(void);
static int _mhal_alsa_pcm_playback1_stop(void);
static int _mhal_alsa_pcm_playback1_resume(void);
static int _mhal_alsa_pcm_playback1_suspend(void);
static unsigned int _mhal_alsa_pcm_playback1_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_playback1_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_playback1_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_playback1_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_playback1_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_playback1_get_inused_lines(void);
static int _mhal_alsa_pcm_playback1_get_avail_lines(void);


/*
 * ============================================================================
 * Local Variables
 * ============================================================================
 */
/* MStar Audio DSP */
static struct MStar_MAD_Info MStar_MAD;

/* Supported Audio Rates by MStar Audio DSP */
static unsigned int mad_rates[] = {
	48000,
};

/* MStar Audio DSP - PCM Playback1 */
static struct MStar_DMA_Reader_Struct g_pcm_playback1 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_PLAYBACK1_TOTAL_BUF_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.initialized_status = MAD_FALSE,
	.channel_mode = 0,
	.sample_rate = 0,
	.period_size = _MAD_PCM_PLAYBACK1_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_PLAYBACK1_HIGH_THRESHOLD,
	.remain_size = 0,
	.written_size = 0,
};

static struct MStar_MAD_Ops MStar_PCM_Playback1_Ops = {
	.open = _mhal_alsa_pcm_playback1_init,
	.close = _mhal_alsa_pcm_playback1_exit,
	.start = _mhal_alsa_pcm_playback1_start,
	.stop = _mhal_alsa_pcm_playback1_stop,
	.resume = _mhal_alsa_pcm_playback1_resume,
	.suspend = _mhal_alsa_pcm_playback1_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_playback1_write,
	.get = _mhal_alsa_pcm_playback1_get,
	.set = _mhal_alsa_pcm_playback1_set,
};


/*
 * ============================================================================
 * Function Implementations
 * ============================================================================
 */
#if 0 /* It's not in used for the moment, might be TODO */
static unsigned char _mhal_alsa_read_byte(unsigned int u32RegAddr)
{
	return (_MAD_AU_AbsReadByte(u32RegAddr+_MAD_MAILBOX_OFFSET));
}
#endif

static unsigned short _mhal_alsa_read_reg(unsigned int u32RegAddr)
{
	return (_MAD_AU_AbsRead2Byte(u32RegAddr+_MAD_MAILBOX_OFFSET));
}

#if 0 /* It's not in used for the moment, might be TODO */
static void _mhal_alsa_write_byte(unsigned int u32RegAddr, unsigned char u8Val)
{
	_MAD_AU_AbsWriteByte((u32RegAddr+_MAD_MAILBOX_OFFSET), u8Val);
}
#endif

#if 0 /* It's not in used for the moment, might be TODO */
static void _mhal_alsa_write_mask_byte(unsigned int u32RegAddr, unsigned char u8Mask, unsigned char u8Val)
{
	_MAD_AU_AbsWriteMaskByte((u32RegAddr+_MAD_MAILBOX_OFFSET), u8Mask, u8Val);
}
#endif

static void _mhal_alsa_write_reg(unsigned int u32RegAddr, unsigned short u16Val)
{
	_MAD_AU_AbsWrite2Byte((u32RegAddr+_MAD_MAILBOX_OFFSET), u16Val);
}

static void _mhal_alsa_write_mask_reg(unsigned int u32RegAddr, unsigned short u16Mask, unsigned short u16Val)
{
	unsigned short u16RegVal;

	u16RegVal = _mhal_alsa_read_reg(u32RegAddr);
	u16RegVal = ((u16RegVal & (~(u16Mask))) | (u16Val & u16Mask));
	_mhal_alsa_write_reg(u32RegAddr, u16RegVal);
}

static unsigned int _mhal_alsa_get_device_status(void)
{
	/* TODO check utopia power down mailbox	*/
	if (_mhal_alsa_read_reg(0x2DE4) == 0x0000) {
		return MAD_FALSE;
	}
	else {
		return MAD_TRUE;
	}
}

/* Initiate PCM Playback1 */
static int _mhal_alsa_pcm_playback1_init(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	unsigned int audio_pcm_dmaRdr_base_pa = 0;
	unsigned int audio_pcm_dmaRdr_base_ba = 0;
	unsigned int audio_pcm_dmaRdr_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Playback1 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info.status != E_RESUME)) {
		audio_pcm_dmaRdr_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2DE4) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_PLAYBACK1_BASE_OFFSET);
		audio_pcm_dmaRdr_base_pa += ((((_mhal_alsa_read_reg(0x2D1C) >> 16) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_dmaRdr_base_ba = audio_pcm_dmaRdr_base_pa;

		if (audio_pcm_dmaRdr_base_pa >= 0xA0000000) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback address!\n");
			return -EFAULT;
		}
		else if ((audio_pcm_dmaRdr_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		audio_pcm_dmaRdr_base_va = audio_pcm_dmaRdr_base_ba + _MAD_KERNEL_SPACE_MEMORY_OFFSET;

		pcm_playback->str_mode_info.physical_addr = audio_pcm_dmaRdr_base_pa;
		pcm_playback->str_mode_info.bus_addr = audio_pcm_dmaRdr_base_ba;
		pcm_playback->str_mode_info.virtual_addr = audio_pcm_dmaRdr_base_va;

		pcm_playback->initialized_status = MAD_TRUE;
	}
	else {
		audio_pcm_dmaRdr_base_pa = pcm_playback->str_mode_info.physical_addr;
		audio_pcm_dmaRdr_base_ba = pcm_playback->str_mode_info.bus_addr;
		audio_pcm_dmaRdr_base_va = pcm_playback->str_mode_info.virtual_addr;
	}

	/* init PCM playback1 buffer address */
	pcm_playback->buffer.addr = (unsigned char *)audio_pcm_dmaRdr_base_va;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Playback1 buffer start address = 0x%08X \n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Playback1 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* reset PCM playback1 engine */
	_mhal_alsa_write_mask_reg(0x2AAC, 0xFFFF, 0x0001);

	/* clear PCM playback1 write pointer */
	_mhal_alsa_write_mask_reg(0x2AAA, 0xFFFF, 0x0000);

	/* reset remain size */
	pcm_playback->remain_size = 0;

	/* reset written size */
	pcm_playback->written_size = 0;

	/* clear PCM playback1 pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit PCM Playback1 */
static int _mhal_alsa_pcm_playback1_exit(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Playback1 engine\n");

	/* reset PCM playback1 engine */
	_mhal_alsa_write_mask_reg(0x2AAC, 0xFFFF, 0x0001);

	/* clear PCM playback1 write pointer */
	_mhal_alsa_write_mask_reg(0x2AAA, 0xFFFF, 0x0000);

	if (pcm_playback->buffer.addr) {
		iounmap((void *)pcm_playback->buffer.addr);
		pcm_playback->buffer.addr = 0;
	}

	pcm_playback->str_mode_info.status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Playback1 */
static int _mhal_alsa_pcm_playback1_start(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Playback1 engine\n");

	/* start PCM playback1 engine */
	_mhal_alsa_write_mask_reg(0x2AAC, 0xFFFF, 0x0002);

	pcm_playback->str_mode_info.status = E_START;

	return 0;
}

/* Stop PCM Playback1 */
static int _mhal_alsa_pcm_playback1_stop(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	MAD_PRINT(KERN_INFO "Stop MStar PCM Playback1 engine\n");

	/* reset PCM playback1 engine */
	_mhal_alsa_write_mask_reg(0x2AAC, 0xFFFF, 0x0001);

	/* clear PCM playback1 write pointer */
	_mhal_alsa_write_mask_reg(0x2AAA, 0xFFFF, 0x0000);

	/* reset Write Pointer */
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;

	/* reset remain size */
	pcm_playback->remain_size = 0;

	/* reset written size */
	pcm_playback->written_size = 0;

	return 0;
}

/* Resume PCM Playback1 */
static int _mhal_alsa_pcm_playback1_resume(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Playback1 engine\n");

	pcm_playback->str_mode_info.status = E_RESUME;

	return 0;
}

/* Suspend PCM Playback1 */
static int _mhal_alsa_pcm_playback1_suspend(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Playback1 engine\n");

	pcm_playback->str_mode_info.status = E_SUSPEND;

	return 0;
}

/* Write PCM to PCM Playback1 */
static unsigned int _mhal_alsa_pcm_playback1_write(void *buffer, unsigned int bytes)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int w_ptr_offset = 0;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_playback1_get_inused_lines() * _MAD_BYTES_IN_LINE;

	if (inused_bytes == 0) {
		MAD_PRINT(KERN_INFO "***** PCM Playback1 Buffer empty !! ***** \n");
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Playback1 Buffer full !! ***** \n");
		return 0;
	}

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* flush MIU */
	Chip_Flush_Memory();

	/* update PCM playback1 write pointer */
	w_ptr_offset = pcm_playback->buffer.w_ptr - pcm_playback->buffer.addr;
	_mhal_alsa_write_mask_reg(0x2AAA, 0xFFFF, (w_ptr_offset / _MAD_BYTES_IN_LINE));
	pcm_playback->written_size += (copy_size - pcm_playback->remain_size);

	/* ensure write pointer can be applied */
	mdelay(1);

	return bytes;
}

/* Get information from PCM Playback1 */
static int _mhal_alsa_pcm_playback1_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Playback1\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(mad_rates) / sizeof(mad_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&mad_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
 			*param = _mhal_alsa_pcm_playback1_get_inused_lines() * _MAD_BYTES_IN_LINE;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
 			*param = _mhal_alsa_pcm_playback1_get_avail_lines() * _MAD_BYTES_IN_LINE;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_CONSUMED_BYTES:
		{
 			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
 			int inused_bytes = 0;
 			int consumed_bytes = 0;

 			inused_bytes = _mhal_alsa_pcm_playback1_get_inused_lines() * _MAD_BYTES_IN_LINE;
			consumed_bytes = pcm_playback->written_size - inused_bytes;
			*param = consumed_bytes;
			pcm_playback->written_size = inused_bytes;
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;

 			*param = pcm_playback->str_mode_info.status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Playback1 */
static int _mhal_alsa_pcm_playback1_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Playback1\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_playback1_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_playback1_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Playback1 buffer level */
static int _mhal_alsa_pcm_playback1_get_inused_lines(void)
{
	int inused_lines = 0;

	inused_lines = _mhal_alsa_read_reg(0x2A7E);

	return inused_lines;
}

/* Get PCM Playback1 avail bytes */
static int _mhal_alsa_pcm_playback1_get_avail_lines(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	int inused_lines = 0;
	int avail_lines = 0;

	inused_lines = _mhal_alsa_pcm_playback1_get_inused_lines();
	avail_lines = (pcm_playback->buffer.size / _MAD_BYTES_IN_LINE) - inused_lines;
	if (avail_lines < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused lines %d!\n", inused_lines);
		avail_lines = 0;
	}

	return avail_lines;
}

/* Set smaple rate to PCM Playback1 */
static int _mhal_alsa_pcm_playback1_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;

	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	/*
	 * Becuase this PCM I/O doesn't support configurable sample rate setting,
	 * there is nothing to do here.
	 */

	return 0;
}

/* Set channel mode to PCM Playback1 */
static int _mhal_alsa_pcm_playback1_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback1;
	unsigned int buffer_size = 0;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	buffer_size = _MAD_PCM_PLAYBACK1_TOTAL_BUF_SIZE >> 1;
	pcm_playback->period_size = buffer_size >> 2;
	pcm_playback->buffer.size = buffer_size;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}


static int __init _mhal_alsa_init(void)
{
	int err = 0;

	MAD_PRINT(KERN_INFO "Initiate MStar ALSA core driver\n");

	memset(&MStar_MAD, 0x00, sizeof(struct MStar_MAD_Info));

	memcpy(MStar_MAD.name, _MAD_CHIP_MODEL, sizeof(MStar_MAD.name));
	sprintf(MStar_MAD.version, "%d.%d.%d", _MAD_ALSA_HAL_VERSION_MAJOR, _MAD_ALSA_HAL_VERSION_MINOR, _MAD_ALSA_HAL_VERSION_REVISION);

	/* Hook Playback Operators */
	MStar_MAD.playback_pcm_ops[1] = &MStar_PCM_Playback1_Ops;

	err = _mdrv_alsa_hook_device(&MStar_MAD);
	if (err < 0) {
		MAD_PRINT(KERN_ERR "Error(%d)! fail to hook PCM operators\n", err);
		return err;
	}

	return 0;
}

static void __exit _mhal_alsa_exit(void)
{
	int err = 0;

	MAD_PRINT(KERN_INFO "Exit MStar ALSA core driver\n");

	err = _mdrv_alsa_unhook_device();
	if (err < 0) {
		MAD_PRINT(KERN_ERR "Error(%d)! fail to unhook PCM operators\n", err);
		return;
	}

	return;
}

/*
 * ============================================================================
 * Module Information
 * ============================================================================
 */
module_init(_mhal_alsa_init);
module_exit(_mhal_alsa_exit);

MODULE_AUTHOR("MStar Semiconductor, Inc.");
MODULE_DESCRIPTION("MStar ALSA Driver - HAL Layer");
MODULE_SUPPORTED_DEVICE("MAD DEVICE");
MODULE_LICENSE("Proprietary");

