////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
/// file    mdrv_pm_io.c
/// @brief  PM I/O Control Interface
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
#include <linux/time.h> 
#include <linux/timer.h>
#include <linux/platform_device.h>
#include <asm/io.h>

#include "mdrv_pm_io.h"
#include "mst_devid.h"



//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------
static int _MDrv_PM_io_open(struct inode *inode, struct file *filp);
static int _MDrv_PM_io_release(struct inode *inode, struct file *filp);
static int _MDrv_PM_io_ioctl(struct inode *inode, struct file *filp, unsigned long u32Cmd, unsigned long u32Arg);

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    int s32Major;
    int s32Minor;
    struct cdev cdev;
    struct file_operations fops;
} PM_DEV;

PM_DEV PMDev=
{
    .s32Major=               MDRV_MAJOR_PM,
    .s32Minor=               MDRV_MINOR_PM,
    .cdev=
    {
        .kobj=                  {.name= MDRV_NAME_PM, },
        .owner  =               THIS_MODULE,
    },
    .fops=
    {
        .open=                  _MDrv_PM_io_open,
        .release=               _MDrv_PM_io_release,
        #ifdef HAVE_UNLOCKED_IOCTL
        .unlocked_ioctl =       _MDrv_PM_io_ioctl,
        #else
        .ioctl =                _MDrv_PM_io_ioctl,
        #endif
    },
};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static int _MDrv_PM_SetDramAddr(struct file *filp, unsigned long arg)
{

    return 0;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// IOCtrl Driver interface functions
//-------------------------------------------------------------------------------------------------
static int _MDrv_PM_io_open(struct inode *inode, struct file *filp)
{
    printk("Inside open \n");
    return 0;
}

//-------------------------------------------------------------------------------------------------
static int _MDrv_PM_io_release(struct inode *inode, struct file *filp)
{
    printk("Inside close \n");
    return 0;
}

//-------------------------------------------------------------------------------------------------
static int _MDrv_PM_io_ioctl(struct inode *inode, struct file *filp, unsigned long u32Cmd, unsigned long u32Arg)
{
    int retval;
    switch (u32Cmd)
    {
        case IOCTL_PM_SET_DRAM_ADDRESS:
            retval = _MDrv_PM_SetDramAddr(filp, u32Arg);
            break;
        default:
            return -ENOTTY;

    }

    return 0;
}



