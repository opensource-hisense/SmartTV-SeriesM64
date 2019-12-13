////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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
/// file    mdrv_xc_io.c
/// @brief  TEMP Driver Interface for Export
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/poll.h>
#include <linux/version.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/irq.h>
#include <linux/platform_device.h>

//drver header files
#include "mst_devid.h"
#include "mdrv_mstypes.h"
#include "mhal_xc.h"
#include "mdrv_xc.h"
#include "chip_int.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define     MDRV_XC_DEVICE_COUNT           1
#define     MDRV_XC_NAME                           "XC"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//#define     XC_DEBUG_ENABLE
#ifdef      XC_DEBUG_ENABLE
#define     XC_KDBG(_fmt, _args...)        printk(KERN_WARNING _fmt, ## _args)
#define     XC_ASSERT(_con)   do {\
                                                            if (!(_con)) {\
                                                                printk(KERN_CRIT "BUG at %s:%d assert(%s)\n",\
                                                                                                    __FILE__, __LINE__, #_con);\
                                                                BUG();\
                                                            }\
                                                          } while (0)
#else
#define     XC_KDBG(fmt, args...)
#define     XC_ASSERT(arg)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

typedef struct
{
    int s32Major;
    int s32Minor;
    int refCnt;
    struct cdev cdev;
    struct file_operations fops;
}XC_DEV;

typedef struct
{
	int		id;
	bool		id_auto;
}platform_xc_device;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------

static XC_DEV devXC =
{
    .s32Major = MDRV_MAJOR_SCALER,
    .s32Minor = MDRV_MINOR_SCALER,
    .refCnt = 0,

    .cdev =
    {
        .kobj = {.name= MDRV_NAME_SCALER, },
        .owner = THIS_MODULE,
    },
    .fops =
    {


    }

};

static platform_xc_device _devXC =
{
    .id = 5,
    .id_auto = 0,
};

//--------------------------------------------------------------------------------------------------
// IOCtrl functions declaration
//--------------------------------------------------------------------------------------------------
MSYSTEM_STATIC int _MDrv_XC_ModuleInit(void);
MSYSTEM_STATIC void _MDrv_XC_ModuleExit(void);

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

MS_U32 _MDrv_XC_RegisterInterrupt(void);
MS_U32 _MDrv_XC_DeRegisterInterrupt(void);


EXPORT_SYMBOL(_MDrv_XC_RegisterInterrupt);
EXPORT_SYMBOL(_MDrv_XC_DeRegisterInterrupt);

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Module functions
//-------------------------------------------------------------------------------------------------

MS_U32 _MDrv_XC_RegisterInterrupt(void)
{
	int error;

    if(0 > (error=request_irq(E_IRQ_DISP, MDrv_XC_IntHandler, IRQF_SHARED, "scaler", &_devXC)))
    {
        XC_KDBG("[XC] Fail to request IRQ:%d\n", E_IRQ_DISP);
        return EFAULT;
    }
    return 0;
}

MS_U32 _MDrv_XC_DeRegisterInterrupt(void)
{
    XC_KDBG("[XC]  dismiss IRQ:%d\n", E_IRQ_DISP);
    free_irq(E_IRQ_DISP,NULL);

    return 0;
}

int _MDrv_XC_ModuleInit(void)
{
    int         s32Ret;
    dev_t       dev;

    if (devXC.s32Major)
    {
        dev = MKDEV(devXC.s32Major, devXC.s32Minor);
        s32Ret = register_chrdev_region(dev, MDRV_XC_DEVICE_COUNT, MDRV_XC_NAME);
    }
    else
    {
        s32Ret = alloc_chrdev_region(&dev, devXC.s32Minor, MDRV_XC_DEVICE_COUNT, MDRV_XC_NAME);
        devXC.s32Major = MAJOR(dev);
    }

    if (0 > s32Ret)
    {
        XC_KDBG("[XC] Unable to get major= %d  ;s32Ret=%d  ;\n", devXC.s32Major,s32Ret);
        return s32Ret;
    }

    cdev_init(&devXC.cdev ,&devXC.fops);
    if (0 != (s32Ret = cdev_add(&devXC.cdev, dev, MDRV_XC_DEVICE_COUNT)))
    {
        XC_KDBG("[XC] Unable add a character device\n");
        unregister_chrdev_region(dev, MDRV_XC_DEVICE_COUNT);
        return s32Ret;
    }

    /* initial the whole XC Driver */
    if(EFAULT == _MDrv_XC_RegisterInterrupt())
    {
        XC_KDBG("[XC] Startup XC Driver Failed! %d\n", devXC.s32Major);
        cdev_del(&devXC.cdev);
        unregister_chrdev_region(dev, MDRV_XC_DEVICE_COUNT);
	 return -ENOMEM;
    }

    return 0;
}


void _MDrv_XC_ModuleExit(void)
{
    /*de-initial the who XCDriver */
    _MDrv_XC_DeRegisterInterrupt();

    cdev_del(&devXC.cdev);
    unregister_chrdev_region(MKDEV(devXC.s32Major, devXC.s32Minor), MDRV_XC_DEVICE_COUNT);
}


module_init(_MDrv_XC_ModuleInit);
module_exit(_MDrv_XC_ModuleExit);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("XC ioctrl driver");
MODULE_LICENSE("GPL");



