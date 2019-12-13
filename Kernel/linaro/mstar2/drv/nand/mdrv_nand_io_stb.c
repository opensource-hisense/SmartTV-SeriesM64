#include <linux/device.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/backing-dev.h>
#include <linux/blkpg.h>
#include <linux/vmalloc.h>
#include <linux/cdev.h>
#include <linux/workqueue.h>
#include <linux/mtd/mtd.h>
#include "inc/common/drvNAND.h"

#if defined(NAND_USE_UNFD_NOCS3) && NAND_USE_UNFD_NOCS3

#define MSTAR_NAND_CHAR_MAJOR			284
#define MSTAR_CDEV_NAND_NAME			"nand_stb"
#define CIS_PAGE_SIZE					0x800
#define CIS_SECTOR_SIZE                 0x400
#define CIS_SPARE_SIZE                  0x100

extern struct semaphore					PfModeSem;
extern void nand_lock_fcie(void);
extern void nand_unlock_fcie(void);
char * vbufNand;
char * cur_bufNand;
U8 au8_MainBufSTB[16384] UNFD_ALIGN1;
U8 au8_SpareBufSTB[1280] UNFD_ALIGN1;
U8 au8_nnibufSTB[512] UNFD_ALIGN1;
NAND_FLASH_INFO_t * pNandInfo;

struct cdev cNandDev;

static U32 BFN_ReadSector(U32 u32PageIndex, U8 *u8Buf, U8 *u8Spare, U16 autodis)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err = UNFD_ST_SUCCESS;


    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    u32_Err = NC_ReadSectors( u32PageIndex, 0, u8Buf, u8Spare, 1);
    if( u32_Err != UNFD_ST_SUCCESS)
    {
        printk("NC_WriteSectors error\n");
    }

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    return u32_Err;
}


static U32 BFN_WriteSector(U32 u32PageIndex, U8 *u8Buf, U8 *u8Spare, U16 autodis)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err = UNFD_ST_SUCCESS;


    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    u32_Err = NC_WriteSectors( u32PageIndex, 0, u8Buf, u8Spare, 1);
    if( u32_Err != UNFD_ST_SUCCESS)
    {
        printk("NC_WriteSectors error\n");
    }

    NC_Write_RandomOut(u32PageIndex,0,u8Buf,16);

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    return u32_Err;
}

U32 drvNAND_ReadCISMIUParam(U32 READBLK)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_PageIdx = 0;
    U32 u32_BufOffset = 0;

    ///////////////////////////////////////////////////
    //step 1. Read CISMIU data to dram
    ///////////////////////////////////////////////////

    pNandDrv->u16_PageByteCnt =  CIS_PAGE_SIZE;
    pNandDrv->u16_SpareByteCnt = CIS_SPARE_SIZE;
    pNandDrv->u16_ECCType =      NANDINFO_ECC_TYPE;//CIS_ECC_TYPE;
    NC_ConfigContext();
    NC_ReInit();

    memset(au8_MainBufSTB, 0xff, sizeof(au8_MainBufSTB));

    BFN_ReadSector(u32_PageIdx, au8_MainBufSTB+u32_BufOffset, au8_SpareBufSTB, 1);
    u32_BufOffset += CIS_SECTOR_SIZE;


    for(u32_PageIdx = 10; u32_PageIdx<16; u32_PageIdx++)
    {
        BFN_ReadSector(u32_PageIdx, au8_MainBufSTB+u32_BufOffset, au8_SpareBufSTB, 1);
        u32_BufOffset += CIS_SECTOR_SIZE;
    }

    pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
    pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
    pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
    NC_ConfigContext();
    NC_ReInit();
    pNandDrv->u16_Reg48_Spare &= ~(1 << 12);
    NC_Config();

    return 1;
}


U32 drvNAND_WriteLoader(int LOADERBLOCK, U8* u8LDRPos, int size)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32Row = LOADERBLOCK * pNandDrv->u16_BlkPageCnt;
    U32 u32Spare[256 / sizeof(U32)];
    U32 u32BlkByteCnt = pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt;
    U32 u32AlignedSize;
    U32 v;

    //Aligned to Block byte size
    u32AlignedSize = (size+u32BlkByteCnt-1)&(~(u32BlkByteCnt-1));

    memset(u32Spare, 0xff, sizeof(u32Spare));
    u32Spare[1] = 0x42000000;
    while(u32AlignedSize > 0) {
        printk("[LDR] u32Row=%lu u8LDRPos=0x%08x u8Spare[1]=0x%08x\n", u32Row, (unsigned int)u8LDRPos, (unsigned int)u32Spare[1]);
        if(u32Row % pNandDrv->u16_BlkPageCnt == 0 && u32Row != LOADERBLOCK * pNandDrv->u16_BlkPageCnt)
            u32Spare[1]++;

        memcpy(au8_MainBufSTB, u8LDRPos, pNandDrv->u16_PageByteCnt);

        v = NC_WritePages(u32Row, au8_MainBufSTB, (U8*)u32Spare, 1);
        printk("[LDR] v=%lu\n", v);
        u32AlignedSize -= pNandDrv->u16_PageByteCnt;
        u8LDRPos += pNandDrv->u16_PageByteCnt;
        u32Row++;
    }

    printk("[LDR]end blk:%lu written \n",u32Row/pNandDrv->u16_BlkPageCnt-1);
    return 0;
}

int drvNAND_ReadMIUParam(U32 u32MIUPos)
{
    /*
    int              retlen=0;
    while (1)
    {
        U16 reg = *(U16*)u32MIUPos;

        retlen += 4;

        if (reg == CHIPCFG_CMD_END)
            return retlen;

        u32MIUPos += 4;
    }
    return 0;
    */
     return 0x2000;
}


U32 drvNAND_WriteCISMIUParam(int MIUBLOCK, U32 MIUPOS, NAND_FLASH_INFO_t * pNandInfo)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    int ParamMIUSize;
    int ProgramPage=0;
    U32 u32_Err = UNFD_ST_SUCCESS;

    //Erase CISMIU block
    u32_Err = NC_EraseBlk( MIUBLOCK * pNandDrv->u16_BlkPageCnt );//use first block as test item
    if (u32_Err != UNFD_ST_SUCCESS)
    {
        printk("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
        drvNAND_MarkBadBlk(MIUBLOCK);
        return 0;
    }

    ParamMIUSize = drvNAND_ReadMIUParam(MIUPOS);

    /* Set to CIS setting*/
    pNandDrv->u16_PageByteCnt = CIS_PAGE_SIZE;
    pNandDrv->u16_SpareByteCnt = CIS_SPARE_SIZE;
    pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;//CIS_ECC_TYPE;
    NC_ConfigContext();
    NC_ReInit();

    // copy CIS to write buffer
    memset(au8_MainBufSTB, '\0', 512);
    memcpy(au8_MainBufSTB,pNandInfo,512);
    memset(au8_SpareBufSTB, -1, sizeof(au8_SpareBufSTB));

    // Write CIS data to page0 sector
    u32_Err = BFN_WriteSector(MIUBLOCK * pNandDrv->u16_BlkPageCnt + ProgramPage,
        (U8*)au8_MainBufSTB,
        (U8*)au8_SpareBufSTB,
        1);

    if (u32_Err != UNFD_ST_SUCCESS)
    {
        printk("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
        drvNAND_MarkBadBlk(MIUBLOCK);
        return 0;
    }

    ProgramPage = 10;

    // copy H1, H0, Sig to write buffer
    memset(au8_MainBufSTB, '\0', sizeof(au8_MainBufSTB));
    memcpy(au8_MainBufSTB, (char*)MIUPOS, ParamMIUSize);
    memset(au8_SpareBufSTB, -1, sizeof(au8_SpareBufSTB));

    //write H1, H0, Sig start from page10
    while(ParamMIUSize > 0)
    {
        printk("Write Page, ParamMIUSize left %d program page number %d\n",ParamMIUSize,ProgramPage);
        u32_Err = BFN_WriteSector(MIUBLOCK * pNandDrv->u16_BlkPageCnt+ProgramPage,
            (U8*)au8_MainBufSTB+((ProgramPage-10)*CIS_SECTOR_SIZE),
            (U8*)au8_SpareBufSTB,1);

        if (u32_Err != UNFD_ST_SUCCESS)
        {
            printk("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
        }

        ParamMIUSize -= CIS_SECTOR_SIZE;
        ProgramPage ++;
    }
    printk("totally write %x\n",ProgramPage-10);

    /* Reset NAND driver and FCIE to the original settings */
    pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
    pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
    pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
    NC_ConfigContext();
    NC_ReInit();
    pNandDrv->u16_Reg48_Spare &= ~(1 << 12);
    NC_Config();

    return 1;
}


U32 drvNAND_InitCISMIUParam(int MIUBLOCK, U32 MIUPOS)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    if(!pNandInfo)
    {
        printk("Memory Allocate fail for pNandInfo\n");
        return -1;
    }

    //setup pNandInfo for CIS
    memset(pNandInfo, 0, 512);
    memcpy(pNandInfo->au8_Tag, "MSTARSEMIUNFDCIS", 16);
    pNandInfo->u8_IDByteCnt = pNandDrv->u8_IDByteCnt;
    memset(pNandInfo->au8_ID, 0, NAND_ID_BYTE_CNT);
    memcpy(pNandInfo->au8_ID, pNandDrv->au8_ID, pNandDrv->u8_IDByteCnt);
    pNandInfo->u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
    pNandInfo->u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
    pNandInfo->u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
    pNandInfo->u16_BlkCnt = pNandDrv->u16_BlkCnt;
    pNandInfo->u32_Config = pNandDrv->u32_Config;

    pNandInfo->u16_ECCType = pNandDrv->u16_ECCType;
    pNandInfo->u16_tRC          = pNandDrv->u16_tRC;
    pNandInfo->u8_tRP           = pNandDrv->u8_tRP;
    pNandInfo->u8_tREH          = pNandDrv->u8_tREH;
    pNandInfo->u8_tREA          = pNandDrv->u8_tREA;
    pNandInfo->u8_tRR           = pNandDrv->u8_tRR;
    pNandInfo->u16_tADL         = pNandDrv->u16_tADL;
    pNandInfo->u16_tRHW         = pNandDrv->u16_tRHW;
    pNandInfo->u16_tWHR         = pNandDrv->u16_tWHR;
    pNandInfo->u16_tCCS         = pNandDrv->u16_tCCS;
    pNandInfo->u8_tCS           = pNandDrv->u8_tCS;
    pNandInfo->u16_tWC          = pNandDrv->u16_tWC;
    pNandInfo->u8_tWP           = pNandDrv->u8_tWP;
    pNandInfo->u8_tWH           = pNandDrv->u8_tWH;
    pNandInfo->u16_tCWAW        = pNandDrv->u16_tCWAW;
    pNandInfo->u8_tCLHZ         = pNandDrv->u8_tCLHZ;
    pNandInfo->u8_AddrCycleIdx  = pNandDrv->u8_AddrCycleIdx;
    pNandInfo->u16_tWW          = pNandDrv->u16_tWW;

    pNandInfo->u8_PairPageMapLoc = pNandDrv->u8_PairPageMapLoc;
    pNandInfo->u8_ReadRetryType =   pNandDrv->u8_ReadRetryType;
    pNandInfo->u8_BitflipThreshold = pNandDrv->u16_BitflipThreshold;

    pNandInfo->u32_ChkSum       = drvNAND_CheckSum((U8*) (pNandInfo) + 0x24, 0x32 - 0x24);

    memcpy(pNandInfo->u8_Vendor, pNandDrv->u8_Vendor, 16);
    memcpy(pNandInfo->u8_PartNumber, pNandDrv->u8_PartNumber, 16);

    pNandInfo->u8_Hash0PageIdx = pNandDrv->u8_Hash0PageIdx;
    pNandInfo->u8_Hash1PageIdx = pNandDrv->u8_Hash1PageIdx;
    pNandInfo->u32_BootSize = pNandDrv->u32_BootSize;

    dump_nand_info(pNandInfo);

   // drvNAND_WriteCISMIUParam(MIUBLOCK,MIUPOS,pNandInfo);


    printk("sizeof CIS is %x\n",sizeof(NAND_FLASH_INFO_t));
    return 0;
}


#if 0
static int mstar_nand_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long argp)
{
	unsigned long size;

	nand_debug(UNFD_DEBUG_LEVEL, 1,"%s is invoked\n", __func__);
	size = (cmd & IOCSIZE_MASK) >> IOCSIZE_SHIFT;
	if (cmd & IOC_IN) {
		if (!access_ok(VERIFY_READ, argp, size))
			return -EFAULT;
	}
	if (cmd & IOC_OUT) {
		if (!access_ok(VERIFY_WRITE, argp, size))
			return -EFAULT;
	}

	switch(cmd)
	{

	}
	return 0;
}
#endif

static ssize_t mstar_nand_write(struct file *filp, const char __user *buf, size_t count,loff_t *ppos)
{
    unsigned int offset = 0;
    int LdrSize;
    U8* bootstrap;
    loff_t fpos = *ppos;
    char * inputBuf;
    char * writeBuf;

	nand_debug(UNFD_DEBUG_LEVEL, 1, "+++\n");
	nand_debug(UNFD_DEBUG_LEVEL, 1, "count(%d) fpos(%lld) filp->f_pos(%lld)\n", count, fpos, filp->f_pos);

    if(count > 0x100000)
    {
		return -EFAULT;
    }

	if(copy_from_user(vbufNand, buf, count))
	{
		return -EFAULT;
	}

    nand_lock_fcie();

    inputBuf = vbufNand;

    if(count == 0x100000) //update all CISMIU Block
    {
        bootstrap = (char*)kmalloc(0x2000,GFP_KERNEL);

        //put HASH1  (4 sectors)
        memcpy(bootstrap+offset, (char*)inputBuf+0x2000, CIS_SECTOR_SIZE*4);
        offset += CIS_SECTOR_SIZE*4;

        //put HASH0  (1 sectors)
        memcpy(bootstrap+offset, (char*)inputBuf+0x0, CIS_SECTOR_SIZE);
        offset += CIS_SECTOR_SIZE;

        //put SIG      (1 sectors)
        memcpy(bootstrap+offset, (char*)inputBuf+0x1D00, 0x300);
        offset += CIS_SECTOR_SIZE;

        LdrSize = (bootstrap[0x0FFC]<<0)
                 |(bootstrap[0x0FFD]<<8)
                 |(bootstrap[0x0FFE]<<16)
                 |(bootstrap[0x0FFF]<<24);

        //write CISMIU to block 0
        drvNAND_WriteCISMIUParam(0,(U32)bootstrap,pNandInfo);

        kfree(bootstrap);

    	nand_debug(UNFD_DEBUG_LEVEL, 1, "Loader Size = %d\n", LdrSize);

        //point to LDR start address
        writeBuf = inputBuf + 0x3000;

        //write LDR to block 1
        drvNAND_WriteLoader(1, writeBuf, LdrSize);
    }
    else
    {
        //half of vbuf is for the final data writing to NAND
        writeBuf = vbufNand+0x100000;

        //read block
        drvNAND_ReadCISMIUParam(0);

        //backup read block to half of vbuffer (CIS+H1+H0+Sig)
        memcpy(writeBuf, au8_MainBufSTB, 0x2000);

        //update the target write region
        memcpy(writeBuf + fpos, inputBuf, count);

        //write to block
        drvNAND_WriteCISMIUParam(0, (U32)(writeBuf+0x400), pNandInfo);

    }

    nand_unlock_fcie();

    nand_debug(UNFD_DEBUG_LEVEL, 1, "---\n");

	return count;
}

static ssize_t mstar_nand_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    nand_lock_fcie();

    if(count>drvNAND_ReadMIUParam(0))
    {
        nand_unlock_fcie();
        return -EFAULT;
    }
	nand_debug(UNFD_DEBUG_LEVEL, 1, "count(%d) *f_pos(%lld) filp->f_pos(%lld)\n", count, *f_pos, filp->f_pos);

    drvNAND_ReadCISMIUParam(0);

    nand_unlock_fcie();

    if(copy_to_user(buf, au8_MainBufSTB , count))
    {
        return -EFAULT;
    }

    return count;
}

static loff_t mstar_nand_seek(struct file *filp, loff_t offset, int whence)
{
    loff_t newpos;

    // remove it if it's not required
	nand_debug(UNFD_DEBUG_LEVEL, 1, "\n");
    printk("whence = %d\n", whence);
    printk("offset = %lld\n", offset);

    switch(whence) {
      case 0: /* SEEK_SET */
        newpos = offset;
        break;
#if 0
      case 1: /* SEEK_CUR */
        newpos = filp->f_pos + offset;
        break;

      case 2: /* SEEK_END */
        newpos = dev->size + offset;
        break;
#endif
      default: /* can't happen */
        return -EINVAL;
    }
    if (newpos < 0) return -EINVAL;
    filp->f_pos = newpos;
    return newpos;
}


static int mstar_nand_open (struct inode *inode, struct file *filp)
{
	nand_debug(UNFD_DEBUG_LEVEL, 1, "\n");
    pNandInfo = (NAND_FLASH_INFO_t* )kmalloc(512, GFP_KERNEL);

	vbufNand= vmalloc(0x200000);
	if(!vbufNand)
		return -ENOMEM;
	cur_bufNand = vbufNand;

    nand_lock_fcie();

    drvNAND_InitCISMIUParam(0, 0);

    nand_unlock_fcie();

    return 0;
}

static int mstar_nand_close(struct inode *inode, struct file *filp)
{
	int err = 0;

    if(pNandInfo)
        kfree(pNandInfo);

	if(vbufNand)
		vfree(vbufNand);

	return err;
}

static const struct file_operations mstar_nand_fops = {
	.owner		= THIS_MODULE,
	.write		= mstar_nand_write,
	.read		= mstar_nand_read,
	.llseek     = mstar_nand_seek,
//	.ioctl		= mstar_nand_ioctl,
	.open		= mstar_nand_open,
	.release	= mstar_nand_close,
};

static int __init init_mstar_nand_char(void)
{
	int ret;
	dev_t dev;

	if(NC_CheckStorageType() == 0)
            return 0;

	nand_debug(UNFD_DEBUG_LEVEL, 1, "\n");
	dev = MKDEV(MSTAR_NAND_CHAR_MAJOR, 0);
	ret = register_chrdev_region(dev, 1, MSTAR_CDEV_NAND_NAME);
	if (ret < 0) {
		pr_notice("Can't allocate major number %d for "
				"Memory Technology Devices.\n", MSTAR_NAND_CHAR_MAJOR);
		return ret;
	}

	//add cdev
	cdev_init(&cNandDev, &mstar_nand_fops);
	cNandDev.owner = THIS_MODULE;
	cNandDev.ops = &mstar_nand_fops;

	ret = cdev_add(&cNandDev, dev, 1);
	if(ret != 0)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Unable to add a char deivce\n");
		unregister_chrdev_region(dev, 1);
	}
    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "init_mstar_nand_char\n");

	return ret;
}


static void __exit cleanup_mstar_nand_char(void)
{
	cdev_del(&cNandDev);
	unregister_chrdev_region( MKDEV(MSTAR_NAND_CHAR_MAJOR, 0), 1);
}

module_init(init_mstar_nand_char);
module_exit(cleanup_mstar_nand_char);
MODULE_ALIAS_CHARDEV_MAJOR(MSTAR_NAND_CHAR_MAJOR);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mstar");
MODULE_DESCRIPTION("Direct access to mstar stb nand hidden partition");
MODULE_ALIAS_CHARDEV_MAJOR(MSTAR_NAND_CHAR_MAJOR);

#endif
