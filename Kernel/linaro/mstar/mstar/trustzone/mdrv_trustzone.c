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
/// file    mdrv_trustzone.c
/// @brief  Trustzone Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <linux/autoconf.h>
//#include <linux/undefconf.h> //unused header file now 
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
#include <asm/io.h>
#include <asm/types.h>
#include <asm/cacheflush.h>

//#include <linux/circ_buf.h>

#include "mdrv_types.h"
#include "mst_platform.h"
#include "mdrv_system.h"
#include <linux/kthread.h>

#include "mst_devid.h"
#include "mdrv_trustzone.h"
#include "mdrv_smc.h"
#include <linux/mutex.h>

#define TZ_DRV_NW_VER_MAIN 1
#define TZ_DRV_NW_VER_SUB  0
#define MOD_TZ_NAME             "tz_mod"
#define MOD_TZ_DEVICE_COUNT 1
#define MAX_REGS								15

#define TZ_DPRINTK(fmt, args...) printk(KERN_WARNING"[TrustZone] %s:%d " fmt,__FUNCTION__,__LINE__,## args)
//#define TZ_DPRINTK(fmt, args...)

//#define CIRC_BUF_SIZE 32
//#define CIRC_ITEM_NUM CIRC_BUF_SIZE/4
//#define CIRC_NAME_MAX_NUM 32

/*
struct tz_cir_buf{
    unsigned int *cmd;
    unsigned int *cmd2;
    int head;
    int tail;
    int buf_num;

    //struct mutex pmutex;
    //struct mutex cmutex;
 
    char name[32];   
};
*/
static int tz_ver_main = TZ_DRV_NW_VER_MAIN;
static int tz_ver_sub  = TZ_DRV_NW_VER_SUB;

static struct class *tz_class;
static struct task_struct *tz_task;
static struct task_struct *tz_rec_thread;
//static struct tz_cir_buf *tz_cir_call=NULL;
static struct tz_wq_des *tz_wq_tx=NULL;
static struct tz_wq_des *tz_wq_rx=NULL;


static DECLARE_WAIT_QUEUE_HEAD(tz_wait);
struct smc_struct *smc;

static DECLARE_WAIT_QUEUE_HEAD(tz_rec_wait);
unsigned int tz_rec_flag=0;

typedef  struct
{
	  unsigned int u32NW_Ver_Main;
	  unsigned int u32NW_Ver_Sub;
	  void *   hook;
	  
}TZ_FileData;

typedef struct
{
    int                         s32TZMajor;
    int                         s32TZMinor;
    void*                       dmaBuf;
    struct cdev                 cDevice;
    struct file_operations      TZFop;
} TZModHandle;

static int                      _MDrv_TZ_Open (struct inode *inode, struct file *filp);
static int                      _MDrv_TZ_Release(struct inode *inode, struct file *filp);
static int                      _MDrv_TZ_MMap(struct file *filp, struct vm_area_struct *vma);
static long                     _MDrv_TZ_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static TZModHandle TZDev=
{
    .s32TZMajor=               MDRV_MAJOR_TZ,
    .s32TZMinor=               MDRV_MINOR_TZ,
    .cDevice=
    {
        .kobj=                  {.name= MOD_TZ_NAME, },
        .owner  =               THIS_MODULE,
    },
    .TZFop=
    {
        .open=                  _MDrv_TZ_Open,
        .release=               _MDrv_TZ_Release,
        .mmap=                  _MDrv_TZ_MMap,
        .unlocked_ioctl =  	_MDrv_TZ_Ioctl,
    },
};


static int _MDrv_TZ_Open(struct inode *inode, struct file *filp)
{
    TZ_FileData *tzData;
    
    tzData = kzalloc(sizeof(*tzData), GFP_KERNEL);
    if (tzData == NULL)
          return -ENOMEM;

    filp->private_data = tzData;
    
    return 0;
}

static int _MDrv_TZ_Release(struct inode *inode, struct file *filp)
{
    TZ_FileData *tzData = filp->private_data ;
    kfree(tzData);

    return 0;
}

static int _MDrv_TZ_MMap(struct file *filp, struct vm_area_struct *vma)
{
	  return 0;
}

static long _MDrv_TZ_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	  int         err= 0;
    int         ret= 0;

    //TZ_FileData *tzData = filp->private_data ;
	  
	  /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if (TZ_IOC_MAGIC!= _IOC_TYPE(cmd))
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
    	  TZ_DPRINTK("TZ err\n");
        return -EFAULT;
    }
    
    switch(cmd)
    {
        case TZ_IOC_INFO:
        {
            DrvTZ_Info_t i ;
            i.u32NW_Ver_Main = tz_ver_main;
            i.u32NW_Ver_Sub = tz_ver_sub;
            
            ret = copy_to_user( (void __user *)arg, &i, sizeof(i) );   
        }
        break;
	    case TZ_IOC_Register_Class:
	    {
              // Implement it later.
	    }
	    break;
	    case TZ_IOC_Call:
	    {
                DrvTZ_Args_t i;
		int cpu;
 
                cpu = smp_processor_id();
                if(cpu ==0)
                {
                    TZ_DPRINTK("call from cpu0, reject!\n");
                    break;    
                }                     
                ret= copy_from_user( &i, (void __user *)arg, sizeof(i) );
		//Tz_LLV_Ctrl_SMC(&i.u32Args[0], &i.u32Args[1], 
                              // &i.u32Args[2], &i.u32Args[3]);
		ret = copy_to_user((void __user *)arg, &i, sizeof(i));
	     }
	     break;
	     default:
	        TZ_DPRINTK("Unknown ioctl command %d\n", cmd);
		return -ENOTTY;
	     }
    return 0;
    	  
}


static void __smc_wrapper(void)
{
    unsigned int cpu = smp_processor_id();

    //TZ_DPRINTK("[TZ] cpu = %d before enter secure world cmd1 = %x, cmd2 = %x\n", cpu, smc->cmd1, smc->cmd2);

    if(cpu == SMC_CORE)
    {
       smc_call(smc);
    }
    else
    {
       TZ_DPRINTK("[[Error SMC call CPU = %d!!\n", cpu);
    }

    //TZ_DPRINTK("[TZ] cpu = %d before leave secure world \ncmd1 = %x, cmd2 = %x\n", cpu, smc->cmd1, smc->cmd2);

}


#define TZ_SHM_TEST_PA 0x49f00000
#define TZ_SHM_TEST_SIZE 0x100000

unsigned int tz_shm_va=0;
unsigned int tz_shm_off=TZ_SHM_TEST_PA;


static void tz_shm_init(void)
{
    tz_shm_va = (unsigned int)ioremap_nocache(TZ_SHM_TEST_PA, TZ_SHM_TEST_SIZE);
}

static unsigned int tz_malloc(int size, int flag)
{
    int ret = tz_shm_off;
    tz_shm_off += (size);
    return ret;
}

static unsigned int tz_pa2va(unsigned int paddr)
{
    return paddr-TZ_SHM_TEST_PA+tz_shm_va;
}

static unsigned int tz_va2pa(unsigned int vaddr)
{
    return vaddr+TZ_SHM_TEST_PA-tz_shm_va;
}

static void tz_create_wq(unsigned int *cir_buf_paddr, int size)
{
    struct tz_cir_buf * tz_vcb;

    *cir_buf_paddr = tz_malloc(sizeof(struct tz_cir_buf), GFP_KERNEL);
    tz_vcb = (struct tz_cir_buf *)tz_pa2va(*cir_buf_paddr);
    tz_vcb->tz_qd_paddr = tz_malloc(size* sizeof(struct tz_queue_data), GFP_KERNEL);
    tz_vcb->head = 0;
    tz_vcb->tail = 0;

    //mutex_init(&tz_vcb->cmutex);
    //mutex_init(&tz_vcb->pmutex);

    tz_vcb->buf_num = size/4;

}

static void tz_destroy_wq(struct tz_cir_buf * tz_cb)
{
    kfree(tz_cb);
    //kfree(tz_cb->tz_qd);

    tz_cb->head = 0;  
    tz_cb->tail = 0;
    tz_cb->buf_num = 0;
}

static void tz_notify_securecore(void)
{

}

static int tz_put_cmd(struct tz_struct *tz, unsigned int tz_cb_paddr) 
{
    int head, tail, ret;

    struct tz_cir_buf *tz_vcb=NULL;
    struct tz_queue_data *tz_vqd=NULL;

    //mutex_lock(&tz_cb->pmutex);
    tz_vcb = (struct tz_cir_buf *)tz_pa2va(tz_cb_paddr);
    tz_vqd = (struct tz_queue_data *)tz_pa2va(tz_vcb->tz_qd_paddr);

    head = tz_vcb->head;
    tail = tz_vcb->tail;


    if(TZ_CIRC_SPACE(head, tail, tz_vcb->buf_num) >= 1)
    {
        tz_vqd[head].classID = tz->classID;
        tz_vqd[head].cmdID = tz->cmdID;
        tz_vqd[head].cmd0 = tz->cmd0;
        tz_vqd[head].cmd1 = tz->cmd1;
        tz_vqd[head].cmd2 = tz->cmd2;
        tz_vqd[head].cmd3 = tz->cmd3;
        tz_vcb->head = (head + 1) & (tz_vcb->buf_num - 1);
    }
   
    ret = TZ_CIRC_SPACE(head, tail, tz_vcb->buf_num);

    //mutex_unlock(&tz_cb->pmutex);
    return ret;
}

static int tz_get_cmd(struct tz_struct *tz, unsigned int tz_cb_paddr)
{
    int head, tail, ret;

    //mutex_lock(&tz_cb->cmutex);    
    struct tz_cir_buf *tz_vcb=NULL;
    struct tz_queue_data *tz_vqd=NULL;
 
    tz_vcb = (struct tz_cir_buf *)tz_pa2va(tz_cb_paddr);
    tz_vqd = (struct tz_queue_data *)tz_pa2va(tz_vcb->tz_qd_paddr);
     
    head = tz_vcb->head;
    tail = tz_vcb->tail;

    if(TZ_CIRC_CNT(head, tail, tz_vcb->buf_num) >= 1)
    {
        tz->classID = tz_vqd[tail].classID;
        tz->cmdID = tz_vqd[tail].cmdID;
        tz->cmd0 = tz_vqd[tail].cmd0;
        tz->cmd1 = tz_vqd[tail].cmd1;
        tz->cmd2 = tz_vqd[tail].cmd2;
        tz->cmd3 = tz_vqd[tail].cmd3;
        tz_vcb->tail = (tail + 1) & (tz_vcb->buf_num - 1);
    }
    
    ret = TZ_CIRC_CNT(head, tail, tz_vcb->buf_num);     
     
    //mutex_unlock(&tz_cb->cmutex);

    return ret;
}

static int tz_wq_register(struct tz_wq_des *des, unsigned int q_size, TZ_WQ q, char* name, TZ_WQ_TYPE type)
 {
    struct tz_wq_des *st_des=NULL;
    unsigned int buf_paddr;

    st_des = &des[q];
    st_des->cmdSize = q_size;
    st_des->prio = q;
    st_des->type = type;
    memcpy(st_des->name, name, CIRC_NAME_MAX_NUM);
    tz_create_wq(&buf_paddr, q_size);
    st_des->u32PAddr = buf_paddr;

    return 1;
}

static int tz_wq_init(void)
{
    tz_wq_tx =  kmalloc(sizeof(struct tz_wq_des)*TZ_WQ_NUM, GFP_KERNEL);
    tz_wq_rx =  kmalloc(sizeof(struct tz_wq_des)*TZ_WQ_NUM, GFP_KERNEL);

    tz_wq_register(tz_wq_tx, CIRC_BUF_SIZE, TZ_WQ_IRQ, "INTR_TX",TZ_WQ_TYPE_TX);
    tz_wq_register(tz_wq_tx, CIRC_BUF_SIZE, TZ_WQ_TIMER, "TIMER_TX",TZ_WQ_TYPE_TX);

    tz_wq_register(tz_wq_rx, CIRC_BUF_SIZE, TZ_WQ_IRQ, "INTR_RX",TZ_WQ_TYPE_RX);
    tz_wq_register(tz_wq_rx, CIRC_BUF_SIZE, TZ_WQ_TIMER, "TIMER_RX",TZ_WQ_TYPE_RX);


    return 1;
}

static int rx_thread(void *w)
{
    struct tz_struct tz;

    while(1)
    {
        //wait_event_interruptible(tz_rec_wait, tz_rec_flag != 0);
        //tz_rec_flag = 0;

        
        if(tz_get_cmd(&tz, tz_wq_rx[TZ_WQ_IRQ].u32PAddr) != 0)
        {
            printk("Get cmd from normal world!! class=%x, cmdID=%x, \ncmd0=%d, cmd1=%d, cmd2=%d, cmd3=%d\n",
                   tz.classID, tz.cmdID, tz.cmd0, tz.cmd1, tz.cmd2, tz.cmd3);
        }
        schedule();
 }

    return 0;
}

static int __smc_thread(void * w)
{
    TZ_DPRINTK("SMC thread start!!\n\n");

    //volatile unsigned int cmd, val;

    while(1)
    {
       //wait_event_interruptible(tz_wait, smc->smc_flag != 0);
#if 1
       __smc_wrapper(); 
#else
       while(1)
       {
           val = tz_get_cmd( &cmd, tz_cir_call);
 
           if(val == 0)
               break;
 
           printk("get cmd = %d \n", cmd);
       }
#endif
       smc->smc_flag = 0;  // make sure no one modify this outsite.
	   msleep_interruptible(1000);

    }

    return 0;
}

int tz_call(struct tz_struct *tz, TZ_WQ q)
{
    while(1)
    {
        //printk("Put cmd from normal world!! class=%x, cmdID=%x, \ncmd0=%d, cmd1=%d, cmd2=%d, cmd3=%d\n",
          //         tz->classID, tz->cmdID, tz->cmd0, tz->cmd1, tz->cmd2, tz->cmd3);
        if(tz_put_cmd(tz, tz_wq_tx[q].u32PAddr)!=0)
            break;

        //printk("cmd quque full\n");
        schedule();
    }
      
    //down_write(&smc->smc_lock);
 
    //smc->smc_flag = 1;
    //wake_up_interruptible(&tz_wait);

    //tz_rec_flag = 1;
    //wake_up_interruptible(&tz_rec_wait);

    /*while(smc->smc_flag) 
    {
        schedule();
    }*/
      
    //up_write(&smc->smc_lock);

    return 0;
}

int __init mod_TZ_DRV_Init(void)
{
    int s32Ret;
    dev_t dev;

    TZ_DPRINTK("TZ support Ver.1.3\n");

    smc = kmalloc(sizeof(struct smc_struct), GFP_KERNEL);
 
    init_rwsem(&smc->smc_lock);
    tz_shm_init();
    tz_wq_init();

    //printk("OOO tz_cir_call = %x\n", tz_cir_call);

    if(!smc)
    {
        TZ_DPRINTK("smc malloc fail!!\n");
        return -1;
    }

    tz_class = class_create(THIS_MODULE, "trustzone");

    if (IS_ERR(tz_class))
    {
        return PTR_ERR(tz_class);
    }

    if (TZDev.s32TZMajor)
    {
        dev = MKDEV(TZDev.s32TZMajor, TZDev.s32TZMinor);
        s32Ret = register_chrdev_region(dev, MOD_TZ_DEVICE_COUNT, MOD_TZ_NAME);
    }
    else
    {
        s32Ret = alloc_chrdev_region(&dev, TZDev.s32TZMinor, MOD_TZ_DEVICE_COUNT, MOD_TZ_NAME);
        TZDev.s32TZMajor = MAJOR(dev);
    }

    if ( 0 > s32Ret)
    {
        TZ_DPRINTK("Unable to get major %d\n", TZDev.s32TZMajor);
        class_destroy(tz_class);
        return s32Ret;
    }

    cdev_init(&TZDev.cDevice, &TZDev.TZFop);
    if (0!= (s32Ret= cdev_add(&TZDev.cDevice, dev, MOD_TZ_DEVICE_COUNT)))
    {
        TZ_DPRINTK("Unable add a character device\n");
        unregister_chrdev_region(dev, MOD_TZ_DEVICE_COUNT);
        class_destroy(tz_class);
        return s32Ret;
    }

    device_create(tz_class, NULL, dev, NULL, MOD_TZ_NAME);

    down_write(&smc->smc_lock);
    smc->smc_flag = 0;
    smc->cmd1 = 0xdeadbee1;
    smc->cmd2 = 0xdeadbee2;
    up_write(&smc->smc_lock);

    tz_task = kthread_create(__smc_thread, NULL, "SMC handle");

    kthread_bind(tz_task, 1); //bind the thread on processor 1

    if (!IS_ERR(tz_task))
        wake_up_process(tz_task);
    else
    {
        TZ_DPRINTK("smc fail !!\n");
        return -1;
    }
    
    tz_rec_thread = kthread_create(rx_thread, NULL, "RX handle");
 
    if (!IS_ERR(tz_rec_thread))
        wake_up_process(tz_rec_thread);
    else
    {
        TZ_DPRINTK("create receive thread fail !!\n");
        return -1;
    }

    return 0;	
}

void __exit mod_TZ_DRV_Exit(void)
{
    cdev_del(&TZDev.cDevice);
    unregister_chrdev_region(MKDEV(TZDev.s32TZMajor, TZDev.s32TZMinor), MOD_TZ_DEVICE_COUNT);

    device_destroy(tz_class, MKDEV(TZDev.s32TZMajor, TZDev.s32TZMinor));
    class_destroy(tz_class);	
    kfree(smc);
}

module_init(mod_TZ_DRV_Init);
module_exit(mod_TZ_DRV_Exit);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("TZ driver");
MODULE_LICENSE("GPL");
