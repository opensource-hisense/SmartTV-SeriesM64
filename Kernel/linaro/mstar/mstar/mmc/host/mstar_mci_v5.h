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

#ifndef MSTAR_MCI_H
#define MSTAR_MCI_H

#include "eMMC.h"

/******************************************************************************
* Function define for this driver
******************************************************************************/

/******************************************************************************
* Register Address Base
******************************************************************************/
#define CLK_300KHz                   300*1000
#define CLK__52MHz               52*1000*1000
#define CLK_200MHz              200*1000*1000

#define eMMC_GENERIC_WAIT_TIME  (HW_TIMER_DELAY_1s*3) // 3 sec
#define eMMC_ADMA_WAIT_TIME  (HW_TIMER_DELAY_1s*6) // 6 sec

/******************************************************************************
* Low level type for this driver
******************************************************************************/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
struct mstar_mci_host_next
{
    unsigned int                dma_len;
    s32                         mstar_cookie;
};
#endif

enum mstar_request_progress {

    REQ_PGS_RECIEVE_REQ = 0,
    REQ_PGS_SEND_CMD,
    REQ_PGS_DMA,
    REQ_PGS_SEND_STOP,
    REQ_PGS_CHECK_ABORT,
    REQ_PGS_FINISH,
};

struct time_stamp_request
{
    U32 t_begin;
    U32 t_end;
};

struct time_stamp_transfer
{
    U32 t_start_dma;
    U32 t_irq_happen;
    U32 t_bh_run;
};

struct sector_info
{
    U32 addr;
    U32 length;
};

struct profile_request
{
    struct sector_info          sector;

    struct time_stamp_request   pre_request;
    struct time_stamp_request   request;
    struct time_stamp_request   post_request;

    struct time_stamp_transfer  transfer;
};

#define REC_REQUEST_NUM 500

struct profile_hcd
{
    struct profile_request      req[REC_REQUEST_NUM];
    U32                         idx_pre_req;
    U32                         idx_request;
    U32                         idx_post_req;
};

struct mstar_mci_host
{
	struct mmc_host             *mmc;
	//struct mmc_command          *cmd;
	struct mmc_request          *req_cmd;
	struct mmc_request          *req_dtc;

	U32				sd_mod;

#ifdef CONFIG_DEBUG_FS
	struct dentry               *debug_root;
	struct dentry               *debug_perf;
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
	struct mstar_mci_host_next  next_data;
#endif

	spinlock_t                  irq_lock; /* Prevent races with irq handler */

	enum mstar_request_progress req_progress;
	struct semaphore            sem_aborted_req_go_through;

	u8                          abort_success;
	u8                          abort_in_process;
	U8				SpeedUpeMMCResume;
	u8				cmd_backup;
	u32				arg_backup;
	// workqueue
	struct work_struct          workqueue_dtc;

	// tasklet
	//struct tasklet_struct       fcie_tasklet;
	//struct tasklet_struct       tasklet_test;

	//u32                         mie_event;

#if defined (CHECK_ABORT_TIMING) && CHECK_ABORT_TIMING
	U32 TimeUsRequest;
	U32 TimeUsAbort;
	U32 TimeUsLag;
#endif

#if defined(PERF_PROFILE)&&PERF_PROFILE
	struct profile_hcd          profile_fcie;
#endif


}; /* struct mstar_mci_host*/

#endif

