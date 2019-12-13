////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_mpool.c
/// @brief  Memory Pool Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>  //added
#include <linux/timer.h> //added
#include <linux/device.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <asm/types.h>
#include <asm/cacheflush.h>

#if defined(CONFIG_COMPAT)
#include <linux/compat.h>
#endif

#include "mdrv_mstypes.h"
#include "mdrv_pm.h"
#include "mhal_pm.h"
#include "mdrv_system.h"

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Golbal variable
//-------------------------------------------------------------------------------------------------
unsigned long gPM_DramAddr = 0x0;
unsigned long gPM_DramSize = 0x10000;


//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local function
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Golbal function
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_SetSRAMOffsetForMCU(void)
{
    PM_Result result;

    result = MHal_PM_CopyBin2Sram(gPM_DramAddr);
    if (result != E_PM_OK)
    {
        return result;
    }

    MHal_PM_SetDram2Register(gPM_DramAddr);

    result = MHal_PM_SetSRAMOffsetForMCU();
    return result;
}

PM_Result MDrv_PM_CopyBin2Sram(void)
{
    struct file *fp = NULL;
    mm_segment_t fs;
    loff_t pos;
    char *buf = NULL;

    phys_addr_t *remap_addr = NULL;

    printk("start MDrv_PM_CopyBin2Sram \n");
    buf = (char *)vmalloc(gPM_DramSize);
    if (buf == NULL)
    {
        printk("%s, vmalloc failed\n", __FUNCTION__);
        return E_PM_FAIL;
    }

    memset(buf, 0, gPM_DramSize);

    fp = filp_open("/config/PM.bin", O_RDONLY, 0);
    if (IS_ERR(fp)){
        printk("%s, filp_open failed\n", __FUNCTION__);
        return E_PM_FAIL;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;
    vfs_read(fp, buf, gPM_DramSize, &pos);

    filp_close(fp,NULL);
    set_fs(fs);

    remap_addr = (phys_addr_t *)ioremap(gPM_DramAddr + MIU0_BUS_BASE, gPM_DramSize);
    if (remap_addr == NULL)
    {
        printk("%s, ioremap failed\n", __FUNCTION__);
        return E_PM_FAIL;
    }

    memcpy(remap_addr, buf, gPM_DramSize);

    vfree(buf);
	iounmap(remap_addr);

    return E_PM_OK;
}

//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------



