///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_ldm_io.c
// @brief  LDM KMD Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Include Files
//=============================================================================
#if (defined (CONFIG_HAS_LD) || defined (CONFIG_HAS_LD_MODULE))
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/string.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#include "mdrv_mstypes.h"
#include "chip_int.h"
#include "mdrv_ldm_common.h"
#include "mdrv_ldm_io.h"
#include "mdrv_ldm_init.h"
#include "mdrv_ldm_algorithm.h"
#include "mdrv_ldm_interrupt.h"
#include "mst_devid.h"


//=============================================================================
// Macros
//=============================================================================

#define     MDRV_LDM_DEVICE_COUNT           1
//#define     MDRV_LDM_NAME                   "LDM"


//-------------------------------------------------------------------------------------------------
// declaration
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LD_Init(void);


//--------------------------------------------------------------------------------------------------
// IOCtrl functions declaration
//--------------------------------------------------------------------------------------------------
static int          _mod_ld_open (struct inode *inode, struct file *filp);
static int          _mod_ld_release(struct inode *inode, struct file *filp);
//static ssize_t      _mod_ld_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
//static ssize_t      _mod_ld_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

#if defined(CONFIG_COMPAT)
static long Compat_mod_ld_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static long                 _mod_ld_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#else
static int                  _mod_ld_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#endif


//=============================================================================
// Local Variables: Device handler
//=============================================================================
static struct class *mld_class;

EN_DEBUG_LEVEL enDbgLevel = E_DEBUG_LEVEL_ERROR;//EN_DEBUG_LEVEL_ALL;
ST_DRV_LD_CUS_PATH stCusPath;

typedef struct
{
    int s32Major;
    int s32Minor;
    int refCnt;
    int refIndex;
    struct cdev cdev;
    struct file_operations fops;
}ST_LDM_DEV;

ST_LDM_DEV devLDM =
{
    .s32Major = MDRV_MAJOR_LDM,
    .s32Minor = MDRV_MINOR_LDM,
    .refCnt = 0,
    .refIndex =0,
    .cdev =
    {
        .kobj = {.name= MDRV_NAME_LDM, },
        .owner = THIS_MODULE,
    },
    .fops =
    {
        .open =                     _mod_ld_open,
        .release =                  _mod_ld_release,
        .unlocked_ioctl =           _mod_ld_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl =                 Compat_mod_ld_ioctl,
#endif
    }
};

static struct platform_device ldm_dev = {
    .name       = "Mstar-localdimming",
    .id     = 0,
};

//static struct platform_device *ldm_devices[] __initdata = {
//	&ldm_dev,
//};


//=============================================================================
// Local Function Prototypes
//=============================================================================


//-------------------------------------------------------------------------------------------------
// IOCtrl Driver functions
//-------------------------------------------------------------------------------------------------


MS_U32 MDrv_LD_RegisterInterrupt(void)
{
    if(0 != (request_irq(E_IRQ_DISP, MDrv_LD_IntHandler, IRQF_SHARED, "local dimming", &ldm_dev)))
    {
        LD_ERROR("Fail to request IRQ \n");
        return EFAULT;
    }

    return 0;
}

MS_U32 MDrv_LD_DeRegisterInterrupt(void)
{
    free_irq(E_IRQ_DISP,&ldm_dev);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
// IOCtrl Driver interface functions
//-------------------------------------------------------------------------------------------------
static int _mod_ld_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int _mod_ld_release(struct inode *inode, struct file *filp)
{
    return 0;
}

#if defined(CONFIG_COMPAT)
static long Compat_mod_ld_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return filp->f_op->unlocked_ioctl(filp, cmd,(unsigned long)compat_ptr(arg));
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static long _mod_ld_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#else
static int _mod_ld_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err= 0;
	int ret= 0;

    if ((MDRV_LDM_IOC_MAGIC!= _IOC_TYPE(cmd)) || (_IOC_NR(cmd)> MDRV_LDM_IOC_MAX_NR))
    {
        return -ENOTTY;
    }

    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    }
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    }
    
    if (err)
    {
        return -EFAULT;
    }

    switch(cmd)
{
        case MDRV_LDM_IOC_INIT:
        {
            err = MDrv_LD_Init();
            if(E_LD_STATUS_SUCCESS != err)
            {
                LD_ERROR(" MDrv_LD_Init FAIL \n");
                return -1;
            }
            err = MDrv_LD_Setup();
            if(E_LD_STATUS_SUCCESS != err)
            {
                LD_ERROR(" MDrv_LD_Setup FAIL \n");
                return -1;
            }
            break;
        }

        case MDRV_LDM_IOC_DEINIT:
        {
            memset(&stCusPath,0,sizeof(ST_DRV_LD_CUS_PATH));
            MDrv_LD_DeRegisterInterrupt();
            MDrv_LD_CleanupModule();
            break;
        }
        
        case MDRV_LDM_IOC_ENABLE:
        {
            MDrv_LDEnable(TRUE,0);
            break;
        }

        case MDRV_LDM_IOC_DISABLE:
        {
            int luma = 0;

            memset(&stCusPath,0,sizeof(ST_DRV_LD_CUS_PATH));
            ret = copy_from_user(&luma, (int*)arg, sizeof(luma)) ;
            MDrv_LDEnable(FALSE,luma);
            break;
        }

        case MDRV_LDM_IOC_SUSPEND:
        {
            MDrv_LD_Suspend();
            break ;
        }
            
        case MDRV_LDM_IOC_RESUME:
        {
            MDrv_LD_Resume();
            break;
        }
                        
        case MDRV_LDM_IOC_STRENGTH:
        {
            int strength = 0;

            ret = copy_from_user(&strength, (int*)arg, sizeof(strength)) ;
            MDrv_LD_SetStrength(strength);
            break ;
        }
        
        case MDRV_LDM_IOC_GET_DATA:
        {
            ST_DRV_LD_GET_DATA stData;
            ret = copy_from_user(&stData, (ST_DRV_LD_GET_DATA*)arg, sizeof(stData));
            stData.addr = MDrv_LD_GetDataAddr(stData.enDataType);
            if(0 == stData.addr)
            {
                LD_ERROR(" MDrv_LD_GetDataAddr FAIL, addr:0 \n");
                return -1;
            }
            ret = copy_to_user( (void *)arg, &stData, sizeof(stData));
            break ;
        }
        
        case MDRV_LDM_IOC_DEBUG:
        {
            int dbg = 0;

            ret = copy_from_user(&dbg, (int*)arg, sizeof(dbg)) ;
            LD_INFO(" dbg:%d \n", dbg);
            enDbgLevel = dbg;
            break ;
        }
        
        case MDRV_LDM_IOC_CUS_PATH:
        {
            ST_DRV_LD_CUS_PATH stPath;

            ret = copy_from_user(&stPath, (ST_DRV_LD_CUS_PATH*)arg, sizeof(stPath));
            LD_INFO(" path:%s, pathU:%s \n", stPath.aCusPath, stPath.aCusPathU);
            strcpy( stCusPath.aCusPath, stPath.aCusPath);
            strcpy( stCusPath.aCusPathU, stPath.aCusPathU);
            break ;
        }
        
        case MDRV_LDM_IOC_DEMO_PATTERN:
        {
            ST_DRV_LD_DEMO_PATTERN stPattern;
            ret = copy_from_user(&stPattern, (ST_DRV_LD_DEMO_PATTERN*)arg, sizeof(stPattern));
            MDrv_LD_SetDemoPattern(stPattern.enDemoPattern, stPattern.bOn);
            break ;
        }
        
        default:
            LD_ERROR("Unknown ioctl command %d\n", cmd);
            return -ENOTTY;
    }
    
    if (ret)
    {
        return EFAULT;
    }

    return 0;
}

static int Mstar_ld_drv_suspend(struct platform_device *dev, pm_message_t state)
{
    //disable_irq(E_IRQ_GOP);   //share gop interrupt, disable in gflip
    MDrv_LD_Suspend();
    return 0;
}
static int Mstar_ld_drv_resume(struct platform_device *dev)
{
    MDrv_LD_Resume();
    //enable_irq(E_IRQ_GOP);
    return 0;
}

static int Mstar_ld_drv_probe(struct platform_device *pdev)
{    
    dev_t  dev;
    int s32Ret=0;
    struct device *psdev = NULL;

    LD_INFO(" trace! entrance \n");
    if( !(pdev->name) || strcmp(pdev->name,"Mstar-localdimming")|| pdev->id!=0)
    {
        s32Ret = -ENXIO;
    }


    if( !(pdev->name) || strcmp(pdev->name,"Mstar-localdimming")
        || pdev->id!=0)
    {
        s32Ret = -ENXIO;
    }

    mld_class = class_create(THIS_MODULE, MDRV_NAME_LDM);
    if (IS_ERR(mld_class))
    {
        LD_ERROR(" Unable create class \n");
        return PTR_ERR(mld_class);
    }

    if(devLDM.s32Major)
    {
        dev = MKDEV(devLDM.s32Major, devLDM.s32Minor);
        s32Ret = register_chrdev_region(dev, MDRV_LDM_DEVICE_COUNT, MDRV_NAME_LDM);
    }
    else
    {
        s32Ret = alloc_chrdev_region(&dev, devLDM.s32Minor, MDRV_LDM_DEVICE_COUNT, MDRV_NAME_LDM);
        devLDM.s32Major = MAJOR(dev);
    }

    if (0 > s32Ret)
    {
        LD_ERROR(" Unable to get major %d\n", devLDM.s32Major);
        class_destroy(mld_class);
        return s32Ret;
    }

    cdev_init(&devLDM.cdev, &devLDM.fops);
    if (0 != (s32Ret= cdev_add(&devLDM.cdev, dev, MDRV_LDM_DEVICE_COUNT)))
    {
        LD_ERROR(" Unable add a character device\n");
        unregister_chrdev_region(dev, MDRV_LDM_DEVICE_COUNT);
        class_destroy(mld_class);
        return s32Ret;
    }
    
    psdev = device_create(mld_class, NULL, dev, NULL, MDRV_NAME_LDM);
    if (IS_ERR(psdev))
    {
        LD_ERROR(" Unable create localdimming inode \n");
        return PTR_ERR(psdev);
    }
    
    LD_INFO(" trace! MDrv_LD_RegisterInterrupt \n");    
    if(EFAULT == MDrv_LD_RegisterInterrupt())
    {
        LD_ERROR(" Startup LDM Driver Failed! %d\n", devLDM.s32Major);
        cdev_del(&devLDM.cdev);
        unregister_chrdev_region(dev, MDRV_LDM_DEVICE_COUNT);
     return -ENOMEM;
    }

    if(!s32Ret)
    {
        pdev->dev.platform_data=&devLDM;
    }
/*
    LD_INFO(" trace! MDrv_LD_Init \n");
    if(E_LD_STATUS_SUCCESS != (s32Ret = MDrv_LD_Init()))
    {
        LD_ERROR(" Unable init device\n");
    }
    
    LD_INFO(" trace! MDrv_LD_Setup \n");
    if(E_LD_STATUS_SUCCESS != (s32Ret = MDrv_LD_Setup()))
    {
        LD_ERROR(" Unable init device\n");
    }
*/
    return s32Ret;
}

static int Mstar_ld_drv_remove(struct platform_device *pdev)
{
    if( !(pdev->name) || strcmp(pdev->name,"Mstar-localdimming") || pdev->id!=0)
    {
        return -ENXIO;
    }

    if( !(pdev->name) || strcmp(pdev->name,"Mstar-localdimming")
        || pdev->id!=0)
    {
        return -ENXIO;
    }

    MDrv_LD_DeRegisterInterrupt();
    MDrv_LD_CleanupModule();

    cdev_del(&devLDM.cdev);
    unregister_chrdev_region(MKDEV(devLDM.s32Major, devLDM.s32Minor), MDRV_LDM_DEVICE_COUNT);
    device_destroy(mld_class, MKDEV(devLDM.s32Major, devLDM.s32Minor));
    class_destroy(mld_class);
    pdev->dev.platform_data=NULL;

    return 0;
}

#if defined (CONFIG_ARM64)
static struct of_device_id mstar_ldm_of_device_ids[] = {
         {.compatible = "mstar-localdimming"},
         {},
};
#endif

static struct platform_driver Mstar_ldm_driver = {
	.probe 		= Mstar_ld_drv_probe,
	.remove 	= Mstar_ld_drv_remove,
    .suspend    = Mstar_ld_drv_suspend,
    .resume     = Mstar_ld_drv_resume,

	.driver = {
#if defined(CONFIG_ARM64)
	    .of_match_table = mstar_ldm_of_device_ids,
#endif
		.name	= "Mstar-localdimming",
        .owner  = THIS_MODULE,
	}
};

//-------------------------------------------------------------------------------------------------
// Module functions
//-------------------------------------------------------------------------------------------------
MSYSTEM_STATIC int __init mod_ld_init(void)
{
    int retval=0;
    platform_device_register(&ldm_dev);
	//platform_add_devices(ldm_devices, ARRAY_SIZE(ldm_devices));	
    retval = platform_driver_register(&Mstar_ldm_driver);
    return retval;
}

MSYSTEM_STATIC void __exit mod_ld_exit(void)
{
	platform_device_unregister(&ldm_dev);
    platform_driver_unregister(&Mstar_ldm_driver);
}


#if defined(CONFIG_MSTAR_MSYSTEM) || defined(CONFIG_MSTAR_MSYSTEM_MODULE)

#else//#if defined(CONFIG_MSTAR_MSYSTEM) || defined(CONFIG_MSTAR_MSYSTEM_MODULE)
module_init(mod_ld_init);
module_exit(mod_ld_exit);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("local dimming ioctrl driver");
MODULE_LICENSE("GPL");
#endif//#if defined(CONFIG_MSTAR_MSYSTEM) || defined(CONFIG_MSTAR_MSYSTEM_MODULE)
#endif

