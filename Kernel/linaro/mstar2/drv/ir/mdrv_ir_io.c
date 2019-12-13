////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvIR.c
/// @brief  IR Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include "MsCommon.h"
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
#include <linux/platform_device.h>
#include <asm/io.h>

#include "mst_devid.h"
#include "mdrv_ir_io.h"
#include "mdrv_ir.h"
#include "mdrv_system.h"
#if LED_BLINK //add for LED blink
    #include <net/sock.h>
    #include <net/netlink.h>
    #include <linux/kernel.h>

    #define NETLINK_LED 26
    #define MAX_MSGSIZE 1024
    int stringlength(char *s);
    void sendnlmsg(char * message);
    int pid;
    int err;
    struct sock *nl_sk = NULL;
    int initflag = 0;
    long repeatkey[KEY_NUM]={0};
    int i=0;
    void nl_io(struct sk_buff *__skb);
    char *str = "1";
    long IRHeadcode0 =0;
    long IRHeadcode1 =0;
#if CVT_SUPPORT_THIRD_IR
    long IR2Headcode0 =0;
    long IR2Headcode1 =0;
#endif
#endif
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define IR_PRINT(fmt, args...)      //printk("IR: [%05d] \n" fmt, __LINE__, ## args)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MOD_IR_DEVICE_COUNT     1

#ifdef CONFIG_MSTAR_DYNAMIC_IR
static DEFINE_MUTEX(get_powerkey_lock);
#endif
//#define IR_DEBUG
#ifdef IR_DEBUG
#define DEBUG_IR(x) (x)
#else
#define DEBUG_IR(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------
static int                      _mod_ir_open (struct inode *inode, struct file *filp);
static int                      _mod_ir_release(struct inode *inode, struct file *filp);
static ssize_t                  _mod_ir_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t                  _mod_ir_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
static unsigned int             _mod_ir_poll(struct file *filp, poll_table *wait);
#ifdef HAVE_UNLOCKED_IOCTL
static long _mod_ir_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#else
static int _mod_ir_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#endif
static int                      _mod_ir_fasync(int fd, struct file *filp, int mode);

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
IRModHandle IRDev=
{
    .s32IRMajor=               MDRV_MAJOR_IR,
    .s32IRMinor=               MDRV_MINOR_IR,
    .cDevice=
    {
        .kobj=                  {.name= MDRV_NAME_IR, },
        .owner  =               THIS_MODULE,
    },
    .IRFop=
    {
        .open=                  _mod_ir_open,
        .release=               _mod_ir_release,
        .read=                  _mod_ir_read,
        .write=                 _mod_ir_write,
        .poll=                  _mod_ir_poll,
        #ifdef HAVE_UNLOCKED_IOCTL
        .unlocked_ioctl = _mod_ir_ioctl,
        #else        
        .ioctl = _mod_ir_ioctl,
        #endif 
        .fasync =	            _mod_ir_fasync,
    },
};

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Initialize IR timing and enable IR.
/// @return None
//-------------------------------------------------------------------------------------------------
static int _mod_ir_open (struct inode *inode, struct file *filp)
{
	IRModHandle *dev;

    IR_PRINT("%s is invoked\n", __FUNCTION__);

    dev = container_of(inode->i_cdev, IRModHandle, cDevice);
	filp->private_data = dev;

    return 0;
}

static int _mod_ir_release(struct inode *inode, struct file *filp)
{
    IR_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

static ssize_t _mod_ir_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
#if defined(CONFIG_MSTAR_GPIO)
#if defined(CONFIG_MSTAR_IR_GPIO_TOGGLE)
	gpio_toggle_trigger();
#endif
#endif
    IR_PRINT("%s is invoked\n", __FUNCTION__);
    return MDrv_IR_Read(filp,buf,count,f_pos);
}

static ssize_t _mod_ir_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    // remove it if it's not required
    IR_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

static unsigned int _mod_ir_poll(struct file *filp, poll_table *wait)
{
    IR_PRINT("%s is invoked\n", __FUNCTION__);
    return MDrv_IR_Poll(filp,wait);
}


static int _mod_ir_fasync(int fd, struct file *filp, int mode)
{
    IR_PRINT("%s is invoked\n", __FUNCTION__);

	return fasync_helper(fd, filp, mode, &IRDev.async_queue);
}

static MS_IR_ShotInfo stIrShotInfo; //for compile warnning , stack size exceed 1024kb

#ifdef HAVE_UNLOCKED_IOCTL
static long _mod_ir_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#else
static int _mod_ir_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err= 0;
  	int retval = 0;
    MS_IR_DelayTime delaytime;
    MS_IR_LastKeyTime keytime;
    MS_IR_KeyValue keyvalue;
    U8 bEnableIR;
    MS_IR_KeyInfo keyinfo;
    pid_t masterPid;

    IR_PRINT("%s is invoked\n", __FUNCTION__);

    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if ((IR_IOC_MAGIC!= _IOC_TYPE(cmd)) || (_IOC_NR(cmd)> IR_IOC_MAXNR))
    {
        return -ENOTTY;
    }

    /*
     * the direction is a bitmask, and VERIFY_WRITE catches R/W
     * transfers. `Type' is user-oriented, while
     * access_ok is kernel-oriented, so the concept of "read" and
     * "write" is reversed
     */
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
        #if 0
        case IR_IOC:
            IR_PRINT("ioctl: receive IR_IOC\n");
            break;
        case IR_IOCREAD:
            IR_PRINT("ioctl: receive IR_IOCREAD with user pointer 0x%08x 0x%08x\n", (u32)arg, *((u32*)(arg)));
            ptrData = (PMS_IR_KDATA)arg;
            IR_PRINT("IR_IOCREAD (1) -> data1=%d, data2=%d, data3=%d\n", ptrData->data1, ptrData->data2, ptrData->data3);
            ptrData->data1 = 11;
            ptrData->data2 = 22;
            ptrData->data3 = 33;
            IR_PRINT("IR_IOCREAD (2) -> data1=%d, data2=%d, data3=%d\n", ptrData->data1, ptrData->data2, ptrData->data3);
            break;
        case IR_IOCWRITE:
            IR_PRINT("ioctl: receive IR_IOCWRITE with user pointer 0x%08x 0x%08x\n", (u32)arg, *((u32*)(arg)));
            break;
        case IR_IOCRW:
            IR_PRINT("ioctl: receive IR_IOCRW with user pointer 0x%08x 0x%08x\n", (u32)arg, *((u32*)(arg)));
            ptrData = (PMS_IR_KDATA)arg;
            IR_PRINT("IR_IOCREAD (1) -> data1=%d, data2=%d, data3=%d\n", ptrData->data1, ptrData->data2, ptrData->data3);
            ptrData->data1 = 55;
            ptrData->data2 = 66;
            ptrData->data3 = 77;
            IR_PRINT("IR_IOCREAD (2) -> data1=%d, data2=%d, data3=%d\n", ptrData->data1, ptrData->data2, ptrData->data3);
            break;

            break;
        #endif
        case MDRV_IR_INIT:
            MDrv_IR_Init(0);
            IRDev.u32IRFlag |= (IRFLAG_IRENABLE|IRFLAG_HWINITED);
            break;
        case MDRV_IR_SEND_KEY:
	     	printk("Send Key!!!\n");
            IR_PRINT("ioctl: MDRV_IR_SEND_KEY\n");
    	    retval = __get_user(keyinfo.u8Key, &(((MS_IR_KeyInfo __user *)arg)->u8Key));
            retval = __get_user(keyinfo.u8Flag, &(((MS_IR_KeyInfo __user *)arg)->u8Flag));
	     MDrv_IR_SendKey(keyinfo.u8Key, keyinfo.u8Flag);
            break;
        case MDRV_IR_SET_DELAYTIME:
            IR_PRINT("ioctl: MDRV_IR_SET_DELAYTIME\n");
    		retval = __get_user(delaytime.u32_1stDelayTimeMs, &(((MS_IR_DelayTime __user *)arg)->u32_1stDelayTimeMs));
    		retval = __get_user(delaytime.u32_2ndDelayTimeMs, &(((MS_IR_DelayTime __user *)arg)->u32_2ndDelayTimeMs));
            MDrv_IR_SetDelayTime(delaytime.u32_1stDelayTimeMs, delaytime.u32_2ndDelayTimeMs);
            break;
        case MDRV_IR_GET_KEY:
            IR_PRINT("ioctl: MDRV_IR_GET_KEY\n");
            //TBD: need mutex here to protect data
            keyinfo.u8Valid = MDrv_IR_GetKey(&keyinfo.u8Key, &keyinfo.u8System, &keyinfo.u8Flag);
            retval = __put_user(keyinfo.u8Key, &(((MS_IR_KeyInfo __user *)arg)->u8Key));
            retval = __put_user(keyinfo.u8System, &(((MS_IR_KeyInfo __user *)arg)->u8System));
            retval = __put_user(keyinfo.u8Flag, &(((MS_IR_KeyInfo __user *)arg)->u8Flag));
            retval = __put_user(keyinfo.u8Valid, &(((MS_IR_KeyInfo __user *)arg)->u8Valid));
            break;
        case MDRV_IR_GET_LASTKEYTIME:
            IR_PRINT("ioctl: MDRV_IR_GET_LASTKEYTIME\n");
            //retval = __get_user(keytime.time, &(((MS_IR_LastKeyTime __user *)arg)->time));
            keytime.time = MDrv_IR_GetLastKeyTime();
            retval = __put_user(keytime.time, &(((MS_IR_LastKeyTime __user *)arg)->time));
            break;
        case MDRV_IR_PARSE_KEY:
            IR_PRINT("ioctl: MDRV_IR_PARSE_KEY\n");
            retval = __get_user(keyvalue.u8KeyIn, &(((MS_IR_KeyValue __user *)arg)->u8KeyIn));
            keyvalue.u8KeyOut = MDrv_IR_ParseKey(keyvalue.u8KeyIn);
            retval = __put_user(keyvalue.u8KeyOut, &(((MS_IR_KeyValue __user *)arg)->u8KeyOut));
            break;
        case MDRV_IR_TEST:
            IR_PRINT("ioctl: MDRV_IR_TEST\n");
            break;
        case MDRV_IR_ENABLE_IR:
            IR_PRINT("ioctl: MDRV_IR_ENABLE_IR\n");
            retval = __get_user(bEnableIR, (int __user *)arg);
            MDrv_IR_EnableIR(bEnableIR);
            if(bEnableIR){
                IRDev.u32IRFlag |= IRFLAG_IRENABLE;
            }else{
                IRDev.u32IRFlag &= ~IRFLAG_IRENABLE;
            }
            break;
        case MDRV_IR_IS_FANTASY_PROTOCOL_SUPPORTED:
            IR_PRINT("ioctl: MDRV_IR_IS_FANTASY_PROTOCOL_SUPPORTED\n");
            retval = __put_user( MDrv_IR_IsFantasyProtocolSupported(),(BOOL __user *)arg);
            break;

        case MDRV_IR_SET_MASTER_PID:
            retval = __get_user(masterPid, (pid_t __user *)arg);
            MDrv_IR_SetMasterPid(masterPid);
            break;

        case MDRV_IR_GET_MASTER_PID:
            masterPid = MDrv_IR_GetMasterPid();
            retval = __put_user(masterPid, (pid_t __user *)arg);
            break;

        case MDRV_IR_INITCFG:
            {
                MS_IR_InitCfg stInitCfg;

                if(copy_from_user(&stInitCfg, (MS_IR_InitCfg __user *)arg, sizeof(MS_IR_InitCfg)))
                {
                    return EFAULT;
                }
                MDrv_IR_InitCfg((MS_IR_InitCfg*)&stInitCfg);
            }
            break;

        case MDRV_IR_TIMECFG:
            {
                MS_IR_TimeCfg stTimeCfg;

                if(copy_from_user(&stTimeCfg, (MS_IR_TimeCfg __user *)arg, sizeof(MS_IR_TimeCfg)))
                {
                    return EFAULT;
                }
                MDrv_IR_TimeCfg((MS_IR_TimeCfg*)&stTimeCfg);
            }
            break;

        case MDRV_IR_GET_SWSHOT_BUF:

            MDrv_IR_ReadShotBuffer(&stIrShotInfo);
            if (copy_to_user((MS_IR_ShotInfo  __user *) arg, &stIrShotInfo, sizeof(MS_IR_ShotInfo)))
            {
                return -EFAULT;
            }
		break;
		case MDRV_IR_SET_HEADER:
		{
			MS_MultiIR_HeaderInfo stItMulti_HeaderInfo;
			printk("\nioctl:MDRV_IR_SET_HEADER\n");
			if(copy_from_user(&stItMulti_HeaderInfo, (MS_MultiIR_HeaderInfo __user *)arg, sizeof(MS_MultiIR_HeaderInfo)))
			{
				return EFAULT;
			}
			MDrv_IR_SetHeaderCode((MS_MultiIR_HeaderInfo*)&stItMulti_HeaderInfo);
		}
		break;
       case MDRV_IR_SET_PROTOCOL:
            {
                MS_MultiProtocolCfg stProtocolCfg;
                if(copy_from_user(&stProtocolCfg, (MS_MultiProtocolCfg __user *)arg, sizeof(MS_MultiProtocolCfg)))
                {
                    return EFAULT;
                }
                MDrv_IR_SetProtocol((MS_MultiProtocolCfg*)&stProtocolCfg);
            }
            break;
#ifdef CONFIG_MSTAR_DYNAMIC_IR
        case MDRV_IR_GET_POWERKEY:
       {
         unsigned int index, key;
         IR_PRINT("%s : ioctl ----> MDRV_IR_GET_POWERKEY\n", __FUNCTION__);

         mutex_lock(&get_powerkey_lock);

         if(unlikely(copy_from_user(&index, (unsigned int __user *)arg, sizeof(int))))
         {
             IR_PRINT("ioctl: copy_from_user error\n");
             return EFAULT;
         }
         IR_PRINT("ioctl: index = 0x%x\n", index);
         key = getPowerKeyAndHeadCode(index);
        if(key != 0xDEADBEAF)
             copy_to_user((unsigned int __user *)arg, &key, sizeof(int));
                                                                                                                                                                                                                                                mutex_unlock(&get_powerkey_lock);
                                                                                                                                                                                                                                               }
                                                                                                                                                                                                                                               break;

#endif
	case MDRV_IR_SET_MULTLICUSTOMER:
	    {
		MS_IR_MutliCustomerCode  stMutliCustomerCfg;
		if(copy_from_user(&stMutliCustomerCfg, (MS_IR_MutliCustomerCode __user *)arg, sizeof(MS_IR_MutliCustomerCode)))
                {
                    return EFAULT;
                }
				
		stMutliCustomerCfg.bResult = MDrv_IR_SetMultiCustomerCode(stMutliCustomerCfg.CustomerCode, stMutliCustomerCfg.IR_Protocol, stMutliCustomerCfg.Enable);

		if (copy_to_user((MS_IR_MutliCustomerCode  __user *) arg, &stMutliCustomerCfg, sizeof(MS_IR_MutliCustomerCode)))
            	{	
                	return -EFAULT;
            	}
	     }
	     break;		
        default:
            IR_PRINT("ioctl: unknown command\n");
            return -ENOTTY;
    }

    return 0;
}

static int mod_ir_init(void)
{
    int ret;
    dev_t dev;

    IR_PRINT("%s is invoked\n", __FUNCTION__);

    if (IRDev.s32IRMajor)
    {
        dev = MKDEV(IRDev.s32IRMajor, IRDev.s32IRMinor);
        ret = register_chrdev_region(dev, MOD_IR_DEVICE_COUNT, MDRV_NAME_IR);
    }
    else
    {
        ret = alloc_chrdev_region(&dev, IRDev.s32IRMinor, MOD_IR_DEVICE_COUNT, MDRV_NAME_IR);
        IRDev.s32IRMajor = MAJOR(dev);
    }

    if ( 0 > ret)
    {
        IR_PRINT("Unable to get major %d\n", IRDev.s32IRMajor);
        return ret;
    }

    cdev_init(&IRDev.cDevice, &IRDev.IRFop);
    if (0!= (ret= cdev_add(&IRDev.cDevice, dev, MOD_IR_DEVICE_COUNT)))
    {
        IR_PRINT("Unable add a character device\n");
        unregister_chrdev_region(dev, MOD_IR_DEVICE_COUNT);
        return ret;
    }

#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
    MDrv_IR_Input_Init();
    MDrv_IR_Init(0);
    IRDev.u32IRFlag |= (IRFLAG_IRENABLE|IRFLAG_HWINITED);
#endif

    return 0;
}

static void mod_ir_exit(void)
{
    IR_PRINT("%s is invoked\n", __FUNCTION__);

    cdev_del(&IRDev.cDevice);
    unregister_chrdev_region(MKDEV(IRDev.s32IRMajor, IRDev.s32IRMinor), MOD_IR_DEVICE_COUNT);

#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
    MDrv_IR_Input_Exit();
#endif
}

static int mstar_ir_drv_suspend(struct platform_device *dev, pm_message_t state)
{
    IRModHandle *pIRHandle=(IRModHandle *)(dev->dev.platform_data);
    if(pIRHandle && (pIRHandle->u32IRFlag&IRFLAG_HWINITED))
    {
        if(pIRHandle->u32IRFlag & IRFLAG_IRENABLE)
        {
            MDrv_IR_EnableIR(0);
        }
    }
    return 0;
}
static int mstar_ir_drv_resume(struct platform_device *dev)
{
    IRModHandle *pIRHandle=(IRModHandle *)(dev->dev.platform_data);
    if(pIRHandle && (pIRHandle->u32IRFlag&IRFLAG_HWINITED))
    {
        MDrv_IR_Init(1);
        if(pIRHandle->u32IRFlag & IRFLAG_IRENABLE)
        {
            MDrv_IR_EnableIR(1);
        }
    }
    return 0;
}

static int mstar_ir_drv_probe(struct platform_device *pdev)
{
    int retval=0;
    if( !(pdev->name) || strcmp(pdev->name,"Mstar-ir")
        || pdev->id!=0)
    {
        retval = -ENXIO;
    }
    IRDev.u32IRFlag = 0;
    retval = mod_ir_init();
    if(!retval)
    {
        pdev->dev.platform_data=&IRDev;
    }
	return retval;
}

static int mstar_ir_drv_remove(struct platform_device *pdev)
{
    if( !(pdev->name) || strcmp(pdev->name,"Mstar-ir")
        || pdev->id!=0)
    {
        return -1;
    }
    mod_ir_exit();
    IRDev.u32IRFlag = 0;
    pdev->dev.platform_data=NULL;
    return 0;
}


static struct platform_driver Mstar_ir_driver = {
	.probe 		= mstar_ir_drv_probe,
	.remove 	= mstar_ir_drv_remove,
    .suspend    = mstar_ir_drv_suspend,
    .resume     = mstar_ir_drv_resume,

	.driver = {
		.name	= "Mstar-ir",
        .owner  = THIS_MODULE,
	}
};


static int __init mstar_ir_drv_init_module(void)
{
    int retval=0;
    retval = platform_driver_register(&Mstar_ir_driver);
#if LED_BLINK//add for LED blink
     struct netlink_kernel_cfg cfg={
       .groups = 1,
       .input = nl_io,
       .cb_mutex = NULL,
   };
    // nl_sk = netlink_kernel_create(&init_net, NETLINK_LED, cfg);
   nl_sk = netlink_kernel_create(&init_net, NETLINK_LED, &cfg);

    if(!nl_sk)
   {
        printk(KERN_ERR "my_net_link: create netlink socket error.\n");
        return 1;
    }

    //printk(KERN_ERR "my_net_link: create netlink socket ok.\n");
#endif
    return retval;
}

static void __exit mstar_ir_drv_exit_module(void)
{
    platform_driver_unregister(&Mstar_ir_driver);
#if LED_BLINK//add for LED blink
    if(nl_sk != NULL)
    {
        sock_release(nl_sk->sk_socket);
    }
    printk("my_net_link: self module exited\n");
#endif
}
#if LED_BLINK//add for LED blink
void sendnlmsg(char *message)
{
    struct sk_buff *skb_1;
    struct nlmsghdr *nlh;
    int len = NLMSG_SPACE(MAX_MSGSIZE);
    int slen = 0;
    if(!message || !nl_sk)
    {
        return ;
    }
    skb_1 = alloc_skb(len,GFP_KERNEL);
    if(!skb_1)
    {
        printk(KERN_ERR "my_net_link:alloc_skb_1 error\n");
    }
    slen = stringlength(message);
    nlh = nlmsg_put(skb_1,0,0,0,MAX_MSGSIZE,0);

    NETLINK_CB(skb_1).portid = 0;
    NETLINK_CB(skb_1).dst_group = 0;

    message[slen]= '\0';
    memcpy(NLMSG_DATA(nlh),message,slen+1);
    //printk("my_net_link:send message '%s'.\n",(char *)NLMSG_DATA(nlh));

    netlink_unicast(nl_sk,skb_1,pid,MSG_DONTWAIT);

}
int stringlength(char *s)
{
    int slen = 0;
    for(; *s; s++)
    {
        slen++;
    }
    return slen;
}

void nl_io(struct sk_buff *__skb)
{
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    char str[100]={0};
    skb = skb_get (__skb);
    if(skb->len >= NLMSG_SPACE(0))
    {
        nlh = nlmsg_hdr(skb);
        memcpy(str, NLMSG_DATA(nlh), sizeof(str));
       printk(KERN_ERR "[Kernel]:Message received:%s\n",str);

       if(str[0]=='I' && str[1]=='R' && str[2]=='H' && str[3]=='e')//IRHeadcode
       {
           int k=0;
           for(k = 0;k<strlen(str);k++)
           {
               if(str[k] == ':')
               {
                   char str_tmp[10]={0};
                   int j=0;
                   #if 1
                   k++;
                   while(str[k]!=',')
                   {
                       str_tmp[j]=str[k];
                       j++;
                       k++;
                   }
                   k--;
                   strict_strtol(str_tmp,10, &IRHeadcode0);
                   #else
                   strict_strtol(&str[k+1],10, &IRHeadcode0);
                   #endif
                   //IRHeadcode0 = simple_strtol(str_tmp,NULL,16);
                   printk(KERN_ERR "[Kernel]:Message received:%ld,i=%d,str_tmp=%s\n",IRHeadcode0,k,str_tmp);
                   continue;
               }
               else if(str[k] == ',')
               {
                   strict_strtol(&str[k+1],10, &IRHeadcode1);
                   printk(KERN_ERR "[Kernel]:Message received:%ld,i=%d\n",IRHeadcode1,k);
                   MDrv_IR_Init(0);
                   break;
               }
           }
       }
    #if CVT_SUPPORT_THIRD_IR
        if(str[0]=='I' && str[1]=='R' && str[2]=='2' && str[3]=='H' && str[4]=='e')//IRHeadcode
        {
            int k=0;
            for(k = 0;k<strlen(str);k++)
            {
                if(str[k] == ':')
                {
                    char str_tmp[10]={0};
                    int j=0;
                    #if 1
                    k++;
                    while(str[k]!=',')
                    {
                        str_tmp[j]=str[k];
                        j++;
                        k++;
                    }
                    k--;
                    strict_strtol(str_tmp,10, &IR2Headcode0);
                    #else
                    strict_strtol(&str[k+1],10, &IRHeadcode0);
                    #endif
                    //IRHeadcode0 = simple_strtol(str_tmp,NULL,16);
                    //printk(KERN_ERR "[Kernel]:Message received:%ld,i=%d,str_tmp=%s\n",IRHeadcode0,k,str_tmp);
                    continue;
                }
                else if(str[k] == ',')
                {
                    strict_strtol(&str[k+1],10, &IR2Headcode1);
                    //printk(KERN_ERR "[Kernel]:Message received:%ld,i=%d\n",IRHeadcode1,k);
                    MDrv_IR_Init(0);
                    break;
                }
            }
        }
    #endif
        else//Use for repeat key
        {
            strict_strtol(str,16, &repeatkey[i++]);
            if(i >= KEY_NUM)
                i=0;
        }

        //printk(KERN_ERR"[Kernel]:Message received:%ld\n",repeatkey[i-1]) ;
        pid = nlh->nlmsg_pid;
        initflag = 1;
    }
}
#endif

module_init(mstar_ir_drv_init_module);
module_exit(mstar_ir_drv_exit_module);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("IR driver");
MODULE_LICENSE("GPL");
