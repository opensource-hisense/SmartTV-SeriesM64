//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/version.h>
#include <linux/slab.h>
#include "mdrv_spinand_command.h"
#include "mdrv_spinand.h"
#include "mhal_spinand.h"

//==========================================================================
// Global Variable
//==========================================================================
U8 u8MagicData[] = {0x4D, 0x53, 0x54, 0x41, 0x52, 0x53, 0x45, 0x4D, 0x49, 0x55, 0x53, 0x46, 0x44, 0x43, 0x49, 0x53};

SPINAND_FLASH_INFO_t gtSpiNandInfoTable[]=
{    //u8_IDByteCnt           au8_ID                u16_SpareByteCnt   u16_PageByteCnt   u16_BlkPageCnt   u16_BlkCnt  u16_SectorByteCnt u8PlaneCnt  u8WrapConfig bRIURead u8CLKConfig u8_UBOOTPBA u8_BL0PBA u8_BL1PBA u8_HashPBA  eReadMode                            eWriteMode
    {2, {MID_GD    , 0xF4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 64, 2048, 64, 4096, 512, 0, 0, FALSE, 108, 0, 0, 0, {{0,0},{0,0},{0,0}}, E_SPINAND_QUAD_MODE_IO, E_SPINAND_QUAD_MODE_IO},
    {2, {MID_GD    , 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 64, 2048, 64, 1024, 512, 0, 0, FALSE, 108, 0, 0, 0, {{0,0},{0,0},{0,0}}, E_SPINAND_QUAD_MODE_IO, E_SPINAND_QUAD_MODE_IO},
    {2, {MID_GD    , 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 64, 2048, 64, 1024, 512, 0, 0, FALSE, 108, 0, 0, 0, {{0,0},{0,0},{0,0}}, E_SPINAND_QUAD_MODE_IO, E_SPINAND_QUAD_MODE_IO},
    {2, {MID_MICRON, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 64, 2048, 64, 1024, 512, 2, 0, FALSE, 108, 0, 0, 0, {{0,0},{0,0},{0,0}}, E_SPINAND_QUAD_MODE   , E_SPINAND_SINGLE_MODE },
    {2, {MID_MICRON, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 64, 2048, 64, 2048, 512, 2, 0, FALSE, 50 , 0, 0, 0, {{0,0},{0,0},{0,0}}, E_SPINAND_QUAD_MODE   , E_SPINAND_SINGLE_MODE },
    {2, {MID_MICRON, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 64, 2048, 64, 4096, 512, 2, 0, FALSE, 50 , 0, 0, 0, {{0,0},{0,0},{0,0}}, E_SPINAND_QUAD_MODE   , E_SPINAND_SINGLE_MODE },
    {2, {0x00      , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  0,     0, 0,    0,   0, 0, 0, FALSE, 50 , 0, 0, 0, {{0,0},{0,0},{0,0}}, E_SPINAND_SINGLE_MODE , E_SPINAND_SINGLE_MODE },
};

SPI_NAND_DRIVER_t gtSpiNandDrv;
SPINAND_FLASH_INFO_t _gtSpinandInfo;
struct device *spi_nand_dev = NULL;

#if defined(SPINAND_MEASURE_PERFORMANCE) && SPINAND_MEASURE_PERFORMANCE
#include <linux/proc_fs.h>
uint64_t	u64_TotalWriteBytes;
uint64_t	u64_TotalReadBytes;
struct proc_dir_entry * writefile;
const char procfs_name[] = "StorageBytes";
#endif


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static int _s32SPINAND_Mutex;
static struct mstar_spinand_info *info;
static U32 u32_curRow = 0;

/* These really don't belong here, as they are specific to the NAND Model */
static uint8_t scan_ff_pattern[] = {0xff};

#define DRIVER_NAME "ms-spinand"

#define CACHE_LINE	0x10

#define MS_ASSERT(a)
//serial flash mutex wait time
#define SPINAND_MUTEX_WAIT_TIME    3000
// For linux, thread sync is handled by mtd. So, these functions are empty.
#define MSOS_PROCESS_PRIVATE    0x00000000
#define MSOS_PROCESS_SHARED     0x00000001
static spinlock_t _gtSpiNANDLock;

/* struct nand_bbt_descr - bad block table descriptor */
static struct nand_bbt_descr spi_nand_bbt_descr = {
    .options = NAND_BBT_2BIT | NAND_BBT_LASTBLOCK | NAND_BBT_VERSION | NAND_BBT_CREATE | NAND_BBT_WRITE,
    .offs = 0,
    .len = 1,
    .pattern = scan_ff_pattern
};

static struct nand_ecclayout spi_nand_oobinfo = {
    .eccbytes = 32,
    .eccpos = {8, 9, 10, 11, 12, 13, 14, 15,
        24, 25, 26, 27, 28, 29, 30, 31,
        40, 41, 42, 43, 44, 45, 46, 47,
        56, 57, 58, 59, 60, 61, 62, 63},
    .oobavail = 30,
    .oobfree = {
        {2, 6},
        {16, 8},
        {32, 8},
        {48, 8},
        {0, 0}
    },
};

static uint8_t bbt_pattern[] = {'B', 'b', 't', '0' };
static uint8_t mirror_pattern[] = {'1', 't', 'b', 'B' };

static struct nand_bbt_descr spi_nand_bbt_main_descr = {
    .options		= NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE |
    NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
    .offs			= 1,
    .len			= 3,
    .veroffs		= 4,
    .maxblocks		= NAND_BBT_BLOCK_NUM,
    .pattern		= bbt_pattern
};

static struct nand_bbt_descr spi_nand_bbt_mirror_descr = {
    .options		= NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE |
    NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
    .offs			= 1,
    .len			= 3,
    .veroffs		= 4,
    .maxblocks		= NAND_BBT_BLOCK_NUM,
    .pattern		= mirror_pattern
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
extern int add_mtd_partitions(struct mtd_info *, const struct mtd_partition *, int);
#endif

#if defined(CONFIG_MTD_CMDLINE_PARTS)
extern int parse_cmdline_partitions(struct mtd_info *master, struct mtd_partition **pparts, char *);
#endif

unsigned char MsOS_In_Interrupt (void)
{
    return FALSE;
}

int MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName, unsigned int u32Flag)
{
    spin_lock_init(&_gtSpiNANDLock);
    return 1;
}

unsigned char MsOS_DeleteMutex (int s32MutexId)
{
    return TRUE;
}

unsigned char MsOS_ObtainMutex (int s32MutexId, unsigned int u32WaitMs)
{
    spin_lock_irq(&_gtSpiNANDLock);
    return TRUE;
}

unsigned char MsOS_ReleaseMutex (int s32MutexId)
{
    spin_unlock_irq(&_gtSpiNANDLock);
    return TRUE;
}


//==========================================================================
// Local Functions:  SPI NAND Driver Function
//==========================================================================
static BOOL _MDrv_SPINAND_GET_INFO(void)
{
    U32 u32Ret;
    U8 u8Spare[16];
    U8 u8Data[512];
    U8 u8Idx, u8Status;
    U8 *u8SrcAddr, *u8DstAddr;
    U16 u16PageIndex;
    u8SrcAddr = u8Data;
    u8DstAddr = (U8*)(&_gtSpinandInfo);

    // if ecc error read back up block(block2 ,4, 6, 8)
    for (u8Idx = 0; u8Idx < 10; u8Idx+=2)
    {
        u16PageIndex = 64 * u8Idx;
        //read data to cache first
        u32Ret = HAL_SPINAND_RFC(u16PageIndex, &u8Status);
        if (u32Ret != ERR_SPINAND_SUCCESS || u8Status & ECC_STATUS_ERR)
            continue;

        // Read SPINand Data
        u32Ret = HAL_SPINAND_Read (0, 512, u8SrcAddr);
        if (u32Ret != ERR_SPINAND_SUCCESS)
            continue;

        // Read SPINand Spare Data
        u32Ret = HAL_SPINAND_Read(2048, 16, u8Spare);
        if (u32Ret != ERR_SPINAND_SUCCESS)
            continue;

        if (memcmp((const void *) u8SrcAddr, (const void *) u8MagicData, sizeof(u8MagicData)) != 0)
            continue;

        u8SrcAddr += sizeof(u8MagicData);
        memcpy(u8DstAddr, u8SrcAddr, sizeof(SPINAND_FLASH_INFO_t));

        return TRUE;
    }

    return FALSE;
}

BOOL MDrv_SPINAND_Init(SPINAND_FLASH_INFO_t *tSpinandInfo)
{
#define SPINAND_ID_SIZE 2
    U8 u8ID[SPINAND_ID_SIZE];
    U32 u32Index;
    U32 u32Ret;
    BOOL bReFind = FALSE;
    // 1. HAL init
    _u8SPINANDDbgLevel = E_SPINAND_DBGLV_DEBUG;

MDrv_SPINAND_Init_Detect_ID:

    u32Ret = HAL_SPINAND_Init();
    if (u32Ret != ERR_SPINAND_SUCCESS)
    {
        printk(KERN_ERR"Init SPI NAND fail!!!!\r\n");
        tSpinandInfo->au8_ID[0] = 0xFF;
        tSpinandInfo->au8_ID[1] = 0xFF;
        tSpinandInfo->u8_IDByteCnt = 2;
        return FALSE;
    }

    _s32SPINAND_Mutex= MsOS_CreateMutex(E_MSOS_FIFO, "Mutex SPINAND", MSOS_PROCESS_SHARED);
    MS_ASSERT(_s32SPINAND_Mutex >= 0);

    if (!_MDrv_SPINAND_GET_INFO())
    {
        u32Ret = HAL_SPINAND_ReadID(SPINAND_ID_SIZE, u8ID);
        if (u32Ret != ERR_SPINAND_SUCCESS)
        {
            printk(KERN_ERR"Can't not Detect SPINAND Device!!!!\r\n");
            tSpinandInfo->au8_ID[0] = 0xFF;
            tSpinandInfo->au8_ID[1] = 0xFF;
            tSpinandInfo->u8_IDByteCnt = 2;
            return FALSE;
        }
        printk(KERN_INFO"MID =%x, DID =%x \r\n",u8ID[0], u8ID[1]);

        for (u32Index = 0; gtSpiNandInfoTable[u32Index].au8_ID[0] != 0; u32Index++)
        {
            if (gtSpiNandInfoTable[u32Index].au8_ID[0] == u8ID[0] &&
                    gtSpiNandInfoTable[u32Index].au8_ID[1] == u8ID[1])
            {
                printk(KERN_INFO"SPINAND Device DETECT\r\n");
                memcpy(tSpinandInfo, &gtSpiNandInfoTable[u32Index], sizeof(SPINAND_FLASH_INFO_t));
                memcpy(&_gtSpinandInfo, &gtSpiNandInfoTable[u32Index], sizeof(SPINAND_FLASH_INFO_t));
                break;
            }
        }
        if ((!gtSpiNandInfoTable[u32Index].au8_ID[0]) && (!gtSpiNandInfoTable[u32Index].au8_ID[1]))
        {
            printk(KERN_ERR"Can't not Detect SPINAND Device!!!!\r\n");
            tSpinandInfo->au8_ID[0] = u8ID[0];
            tSpinandInfo->au8_ID[1] = u8ID[1];
            tSpinandInfo->u8_IDByteCnt = 2;
            if (!bReFind)
            {
                bReFind = TRUE;
                HAL_SPINAND_CSCONFIG();
                goto MDrv_SPINAND_Init_Detect_ID;
            }
            return FALSE;
        }
    }
    else
    {
        memcpy(tSpinandInfo, &_gtSpinandInfo, sizeof(SPINAND_FLASH_INFO_t));
    }
    HAL_SPINAND_SetCKG(CLKCFG);
    return TRUE;
}

BOOL MDrv_SPINAND_ForceInit(SPINAND_FLASH_INFO_t *tSpinandInfo)
{
    memcpy(&_gtSpinandInfo, tSpinandInfo, sizeof(SPINAND_FLASH_INFO_t));
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
// Read SPINAND Data
// @param u32_PageIdx : page index of read data in specific block
// @return TRUE : succeed
// @return FALSE : fail
// @note : If Enable ISP engine, the XIU mode does not work
//-------------------------------------------------------------------------------------------------

U32 MDrv_SPINAND_Read(U32 u32_PageIdx, U8 *u8Data, U8 *pu8_SpareBuf)
{
    U8 u8Status;
    U32 u32Ret = ERR_SPINAND_SUCCESS;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    if (FALSE == MsOS_ObtainMutex(_s32SPINAND_Mutex, SPINAND_MUTEX_WAIT_TIME))
    {
        printk("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    //read data to cache first
    u32Ret = HAL_SPINAND_RFC(u32_PageIdx, &u8Status);
    if (u32Ret != ERR_SPINAND_SUCCESS)
    {
        MsOS_ReleaseMutex(_s32SPINAND_Mutex);
        return u32Ret;
    }

    //DEBUG_SPINAND(E_SPINAND_DBGLV_DEBUG, printk("u8Status %x\r\n", u8Status));

    HAL_SPINAND_PLANE_HANDLER(u32_PageIdx);

    // Read SPINand Data
    u32Ret = HAL_SPINAND_Read (0x0, PAGE_SIZE, u8Data);
    if (u32Ret != ERR_SPINAND_SUCCESS)
    {
        MsOS_ReleaseMutex(_s32SPINAND_Mutex);
        return u32Ret;
    }

    //printk("READ SPARE DATA\r\n");
    // Read SPINand Spare Data
    u32Ret= HAL_SPINAND_Read(PAGE_SIZE, SPARE_SIZE, pu8_SpareBuf);
    if (u32Ret == ERR_SPINAND_SUCCESS) {
        if (u8Status & ECC_STATUS_ERR)
            u32Ret = ERR_SPINAND_ECC_ERROR;
        else if (u8Status & ECC_STATUS_BITFLIP)
            u32Ret = ERR_SPINAND_ECC_BITFLIP;
    }

#if defined(SPINAND_MEASURE_PERFORMANCE) && SPINAND_MEASURE_PERFORMANCE
    u64_TotalReadBytes+= _gtSpinandInfo.u16_PageByteCnt;
#endif

    MsOS_ReleaseMutex(_s32SPINAND_Mutex);

    return u32Ret;
}

//-------------------------------------------------------------------------------------------------
// Read SPINAND Data From Random column address
// @param u32_PageIdx : page index of read data in specific block
// @return TRUE : succeed
// @return FALSE : fail
// @note : If Enable ISP engine, the XIU mode does not work
//-------------------------------------------------------------------------------------------------
U32 MDrv_SPINAND_Read_RandomIn(U32 u32_PageIdx, U32 u32_Column, U32 u32_Byte, U8 *u8Data)
{
    U8 u8Status;
    U32 u32Ret = ERR_SPINAND_SUCCESS;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    if (FALSE == MsOS_ObtainMutex(_s32SPINAND_Mutex, SPINAND_MUTEX_WAIT_TIME))
    {
        printk("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    //read data to cache first
    u32Ret = HAL_SPINAND_RFC(u32_PageIdx, &u8Status);
    if (u32Ret != ERR_SPINAND_SUCCESS)
    {
        MsOS_ReleaseMutex(_s32SPINAND_Mutex);
        return u32Ret;
    }

    //DEBUG_SPINAND(E_SPINAND_DBGLV_DEBUG, printk("u8Status %x\r\n", u8Status));

    HAL_SPINAND_PLANE_HANDLER(u32_PageIdx);

    // Read SPINand Data
    u32Ret = HAL_SPINAND_Read (u32_Column, u32_Byte, u8Data);
    if (u32Ret != ERR_SPINAND_SUCCESS)
    {
        MsOS_ReleaseMutex(_s32SPINAND_Mutex);
        return u32Ret;
    }

    if (u32Ret == ERR_SPINAND_SUCCESS) {
        if (u8Status & ECC_STATUS_ERR)
            u32Ret = ERR_SPINAND_ECC_ERROR;
        else if (u8Status & ECC_STATUS_BITFLIP)
            u32Ret = ERR_SPINAND_ECC_BITFLIP;
    }

#if defined(SPINAND_MEASURE_PERFORMANCE) && SPINAND_MEASURE_PERFORMANCE
    u64_TotalReadBytes += u32_Byte;
#endif

    MsOS_ReleaseMutex(_s32SPINAND_Mutex);
    return u32Ret;
}

U32 MDrv_SPINAND_SetMode(SPINAND_MODE eMode)
{
    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    if (FALSE == MsOS_ObtainMutex(_s32SPINAND_Mutex, SPINAND_MUTEX_WAIT_TIME))
    {
        printk("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }
    U32 u32Ret=HAL_SPINAND_SetMode(eMode);

    MsOS_ReleaseMutex(_s32SPINAND_Mutex);
    return u32Ret;
}

U32 MDrv_SPINAND_Write(U32 u32_PageIdx, U8 *u8Data, U8 *pu8_SpareBuf)
{
    U32 u32Ret;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    if (FALSE == MsOS_ObtainMutex(_s32SPINAND_Mutex, SPINAND_MUTEX_WAIT_TIME))
    {
        printk("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

#if defined(SPINAND_MEASURE_PERFORMANCE) && SPINAND_MEASURE_PERFORMANCE
    u64_TotalWriteBytes += _gtSpinandInfo.u16_PageByteCnt;
#endif
    u32Ret=HAL_SPINAND_Write(u32_PageIdx, u8Data, pu8_SpareBuf);

    MsOS_ReleaseMutex(_s32SPINAND_Mutex);

    return u32Ret;
}

U8 MDrv_SPINAND_ReadID(U16 u16Size, U8 *u8Data)
{
    U8 u8Ret;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    if (FALSE == MsOS_ObtainMutex(_s32SPINAND_Mutex, SPINAND_MUTEX_WAIT_TIME))
    {
        printk("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    u8Ret=HAL_SPINAND_ReadID(u16Size, u8Data);

    MsOS_ReleaseMutex(_s32SPINAND_Mutex);
    return u8Ret;
}

U32 MDrv_SPINAND_BLOCK_ERASE(U32 u32_PageIdx)
{
    U32 u32Ret;
    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    if (FALSE == MsOS_ObtainMutex(_s32SPINAND_Mutex, SPINAND_MUTEX_WAIT_TIME))
    {
        printk("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    u32Ret=HAL_SPINAND_BLOCKERASE(u32_PageIdx);

    MsOS_ReleaseMutex(_s32SPINAND_Mutex);
    return u32Ret;
}

void MDrv_SPINAND_Device(struct device *dev)
{
    spi_nand_dev = dev;
}

void SpiNandMain(unsigned int dwSramAddress, unsigned int dwSramSize)
{
    U8 u8Data[2];
    MDrv_SPINAND_ReadID(2,u8Data);
}

U32 MDrv_SPINAND_WriteProtect(BOOL bEnable)
{
    return HAL_SPINAND_WriteProtect(bEnable);
}

BOOL MDrv_SPINAND_IsActive(void)
{
    return HAL_SPINAND_IsActive();
}


//==========================================================================
// Global Function  for structure  nand_chip and nand_ecc_ctrl
//==========================================================================

#if 0
static __inline void dump_mem_line(unsigned char *buf, int cnt)
{
#if 1
    printk(KERN_NOTICE" 0x%08lx: " \
           "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \n" \
           , (U32)buf,
           buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],buf[12],buf[13],buf[14],buf[15]
          );
#else
    int i;

    printk(KERN_NOTICE" 0x%08lx: ", (U32)buf);
    for (i= 0; i < cnt; i++)
        printk(KERN_NOTICE"%02X ", buf[i]);

    printk(KERN_NOTICE" | ");

    for (i = 0; i < cnt; i++)
        printk(KERN_NOTICE"%c", (buf[i] >= 32 && buf[i] < 128) ? buf[i] : '.');

    printk(KERN_NOTICE"\n");
#endif
}

void dump_mem(unsigned char *buf, int cnt)
{
    int i;

    for (i= 0; i < cnt; i+= 16)
        dump_mem_line(buf + i, 16);
}
#endif

#if defined(SPINAND_MEASURE_PERFORMANCE) && SPINAND_MEASURE_PERFORMANCE
int procfile_read(char* buffer, char ** buffer_location, off_t offset,
                  int buffer_length, int *eof, void *data)
{
    int ret;
    if (offset > 0)
        ret = 0;
    else
        ret = sprintf(buffer,"TotalWriteBytes %lld GB %lld MB\nTotalReadBytes %lld GB %lld MB\n",
                      u64_TotalWriteBytes/1024/1024/1024, (u64_TotalWriteBytes/1024/1024) % 1024,
                      u64_TotalReadBytes/1024/1024/1024, (u64_TotalReadBytes/1024/1024) % 1024);
    return ret;
}
#endif

uint8_t	spi_nand_read_byte(struct mtd_info *mtd)
{
    spi_nand_msg("");

    return gtSpiNandDrv.pu8_sparebuf[gtSpiNandDrv.u32_column++];
}

u16 spi_nand_read_word(struct mtd_info *mtd)
{
    u16 u16_word;

    spi_nand_msg("");
    u16_word = ((u16)gtSpiNandDrv.pu8_sparebuf[gtSpiNandDrv.u32_column] | ((u16)gtSpiNandDrv.pu8_sparebuf[gtSpiNandDrv.u32_column+1]<<8));
    gtSpiNandDrv.u32_column += 2;

    return u16_word;
}

void spi_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
    spi_nand_msg("not support");
}

void spi_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
    spi_nand_msg("not support");
}

int spi_nand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
    spi_nand_msg("not support");

    return 0;
}

void spi_nand_select_chip(struct mtd_info *mtd, int chip)
{
    spi_nand_msg("");
}

void spi_nand_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
    spi_nand_msg("not support");
}

int spi_nand_dev_ready(struct mtd_info *mtd)
{
    spi_nand_msg("not support");

    return 1;
}

void spi_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
    U32 ret;

    switch (command) {
    case NAND_CMD_STATUS:
        spi_nand_msg("%02Xh", command);
        gtSpiNandDrv.pu8_sparebuf[0] = NAND_STATUS_READY|NAND_STATUS_TRUE_READY;
        gtSpiNandDrv.u32_column = 0;
        break;

    case NAND_CMD_READOOB:
        spi_nand_msg("%02Xh", command);
        MDrv_SPINAND_Read(page_addr, (U8 *)gtSpiNandDrv.pu8_pagebuf, (U8 *)gtSpiNandDrv.pu8_sparebuf);
        gtSpiNandDrv.u32_column = column;
        break;


    case NAND_CMD_ERASE2:
        spi_nand_msg("%02Xh", command);
        break;

    case NAND_CMD_ERASE1:
        spi_nand_msg("NAND_CMD_ERASE1, page_addr: 0x%X\n", page_addr);
        gtSpiNandDrv.u8_status = NAND_STATUS_READY|NAND_STATUS_TRUE_READY;
        ret = MDrv_SPINAND_BLOCK_ERASE(page_addr);
        if (ret != ERR_SPINAND_SUCCESS)
        {
            spi_nand_err("MDrv_SPINAND_Erase=%ld", ret);
            gtSpiNandDrv.u8_status |= NAND_STATUS_FAIL;
        }

        break;

    case NAND_CMD_READ0:
        u32_curRow = page_addr;
        spi_nand_msg("%02Xh", command);
        break;

    default:
        spi_nand_err("unsupported command %02Xh", command);
        break;
    }

    return;
}

int spi_nand_waitfunc(struct mtd_info *mtd, struct nand_chip *this)
{
    spi_nand_msg("");

    return (int)gtSpiNandDrv.u8_status;
}

int spi_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
                        uint32_t offset, int data_len, const uint8_t *buf,
                        int oob_required, int page, int cached, int raw)
{
    U32 ret;

    spi_nand_msg("0x%X", page);

    ret = MDrv_SPINAND_Write(page, (U8 *)buf, (U8 *)chip->oob_poi);
    if (ret != ERR_SPINAND_SUCCESS)
    {
        spi_nand_err("MDrv_SPINAND_Write=%ld", ret);
        return -EIO;
    }

    return 0;
}

void spi_nand_ecc_hwctl(struct mtd_info *mtd, int mode)
{
    spi_nand_msg("not support");
}

int spi_nand_ecc_calculate(struct mtd_info *mtd, const uint8_t *dat, uint8_t *ecc_code)
{
    spi_nand_msg("not support");

    return 0;
}

int spi_nand_ecc_correct(struct mtd_info *mtd, uint8_t *dat, uint8_t *read_ecc, uint8_t *calc_ecc)
{
    spi_nand_msg("not support");

    return 0;
}

int spi_nand_ecc_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
                               uint8_t *buf, int oob_required, int page)
{
    U32 ret;
    U8 *u8_DmaBuf = buf;

    spi_nand_msg("0x%X", page);

#if defined(CONFIG_MIPS)
    if ( ((U32)buf) >= 0xC0000000 || ((U32)buf) % CACHE_LINE )
#elif defined(CONFIG_ARM)
    if (!virt_addr_valid((U32)buf) || !virt_addr_valid((U32)buf + (U32)mtd->writesize - 1) || ((U32)buf) % CACHE_LINE )
#endif
    {
        spi_nand_msg("Receive Virtual Mem:%08lXh", (U32)buf);
        u8_DmaBuf = gtSpiNandDrv.pu8_pagebuf;
    }

    ret = MDrv_SPINAND_Read(page, (U8 *)u8_DmaBuf, (U8 *)chip->oob_poi);
    if (ret != ERR_SPINAND_SUCCESS && ret != ERR_SPINAND_ECC_BITFLIP)
    {
        spi_nand_err("MDrv_SPINAND_Read=%ld", ret);
    }

    if (u8_DmaBuf != buf)
    {
        memcpy((void *) buf, (const void *) u8_DmaBuf, mtd->writesize);
    }
    if (ret == ERR_SPINAND_ECC_BITFLIP)
    {
        mtd->ecc_stats.corrected += 1;
    }
    return 0;
}

int spi_nand_ecc_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
                                const uint8_t *buf, int oob_required)
{
    spi_nand_msg("not support");
    return 0;
}

int spi_nand_ecc_read_page(struct mtd_info *mtd, struct nand_chip *chip,
                           uint8_t *buf, int oob_required, int page)
{
    U32 ret;
    U8 *u8_DmaBuf = buf;

    spi_nand_msg("0x%X", page);

#if defined(CONFIG_MIPS)
    if ( ((U32)buf) >= 0xC0000000 || ((U32)buf) % CACHE_LINE )
#elif defined(CONFIG_ARM)
    if (!virt_addr_valid((U32)buf) || !virt_addr_valid((U32)buf + (U32)mtd->writesize - 1) || ((U32)buf) % CACHE_LINE )
#endif
    {
        spi_nand_msg("Receive Virtual Mem:%08lXh", (U32)buf);
        u8_DmaBuf = gtSpiNandDrv.pu8_pagebuf;
    }

    ret = MDrv_SPINAND_Read(page, (U8 *)u8_DmaBuf, (U8 *)chip->oob_poi);
    if (ret != ERR_SPINAND_SUCCESS && ret != ERR_SPINAND_ECC_BITFLIP)
    {
        spi_nand_err("MDrv_SPINAND_Read=%ld", ret);
        mtd->ecc_stats.failed++;
    }
    if (ret == ERR_SPINAND_ECC_BITFLIP)
    {
        mtd->ecc_stats.corrected += 1;
    }
    if (u8_DmaBuf != buf)
    {
        memcpy((void *) buf, (const void *) u8_DmaBuf, mtd->writesize);
    }

    return 0;
}

int spi_nand_ecc_read_subpage(struct mtd_info *mtd, struct nand_chip *chip, uint32_t offs, uint32_t len, uint8_t *buf)
{
    U32 ret = 0;
    U8 *u8_DmaBuf;

    buf += offs;
    u8_DmaBuf = buf;

#if defined(CONFIG_MIPS)
    if ( ((U32)buf) >= 0xC0000000 || ((U32)buf) % CACHE_LINE )
#elif defined(CONFIG_ARM)
    if (!virt_addr_valid((U32)buf) || !virt_addr_valid((U32)buf + (U32)mtd->writesize - 1) || ((U32)buf) % CACHE_LINE )
#endif
    {
        spi_nand_msg("Receive Virtual Mem:%08lXh", (U32)buf);
        u8_DmaBuf = gtSpiNandDrv.pu8_pagebuf;
    }

    ret = MDrv_SPINAND_Read_RandomIn(u32_curRow, offs, len, u8_DmaBuf);

    if (ret == ERR_SPINAND_SUCCESS)
    {
        //printk("read subpage %ld success\n", u32_curRow);
    }
    else if (ret == ERR_SPINAND_ECC_ERROR)
        mtd->ecc_stats.failed++;
    else if (ret == ERR_SPINAND_ECC_BITFLIP)
        mtd->ecc_stats.corrected += 1;
    else
        return -ret;

    if (u8_DmaBuf != buf)
    {
        memcpy((void *) buf, (const void *) u8_DmaBuf, len);
    }

    return 0;
}

int spi_nand_ecc_write_page(struct mtd_info *mtd, struct nand_chip *chip,
                            const uint8_t *buf, int oob_required)
{
    spi_nand_msg("not support");
    return 0;
}

int spi_nand_ecc_read_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
    U32 ret;
    spi_nand_msg("0x%X", page);

    ret = MDrv_SPINAND_Read(page, (U8 *)gtSpiNandDrv.pu8_pagebuf, (U8 *)chip->oob_poi);
    if (ret != ERR_SPINAND_SUCCESS && ret != ERR_SPINAND_ECC_BITFLIP)
    {
        spi_nand_err("MDrv_SPINAND_Read=%ld", ret);
    }

    return 0;
}

int spi_nand_ecc_write_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
    U32 ret;

    spi_nand_msg("0x%X", page);

    memset((void *)gtSpiNandDrv.pu8_pagebuf, 0xFF, mtd->writesize);
    ret = MDrv_SPINAND_Write(page, (U8 *)gtSpiNandDrv.pu8_pagebuf, (U8 *)chip->oob_poi);
    if (ret != ERR_SPINAND_SUCCESS)
    {
        spi_nand_err("MDrv_SPINAND_Write=%ld", ret);
        return -EIO;
    }

    return 0;
}

static U32 CheckSum(U8 *pu8_Data, U16 u16_ByteCnt)
{
    U32 u32_Sum = 0;

    while (u16_ByteCnt--)
        u32_Sum += *pu8_Data++;

    return u32_Sum;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,1)
static int __init mstar_spinand_probe(struct platform_device *pdev)
#else
static int __devinit mstar_spinand_probe(struct platform_device *pdev)
#endif
{
    U8 u8_i;
    U32 u32_ret;
    SPI_NAND_PARTITION_INFO_t *ptPartInfo;
    struct nand_chip *nand;
    struct mtd_info* mtd;
    int err = 0;

    /* Allocate memory for MTD device structure and private data */
    info = kzalloc(sizeof(struct mstar_spinand_info), GFP_KERNEL);
    if (!info)
    {
        spi_nand_err("Allocate Mstar spi nand info fail\n");
        return -ENOMEM;
    }
    platform_set_drvdata(pdev, info);

    /* Get pointer to private data */
    info->pdev = pdev;
    nand = &info->nand;
    mtd = &info->mtd;

    /* Initialize structures */
    mtd->priv = nand;

    MDrv_SPINAND_Device(&pdev->dev);
    if (MDrv_SPINAND_Init(&(gtSpiNandDrv.tSpinandInfo)) != TRUE)
    {
        spi_nand_err("MDrv_SPINAND_Init fail");
        return -ENODEV;
    }

    gtSpiNandDrv.u8_status = NAND_STATUS_READY|NAND_STATUS_TRUE_READY;
    gtSpiNandDrv.u32_column = 0;
    gtSpiNandDrv.pu8_pagebuf = kmalloc(gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt, GFP_KERNEL);
    gtSpiNandDrv.pu8_sparebuf = kmalloc(gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt, GFP_KERNEL);
    if (!gtSpiNandDrv.pu8_pagebuf || !gtSpiNandDrv.pu8_sparebuf)
    {
        spi_nand_err("Can not alloc memory for page/spare buffer");
        return -ENOMEM;
    }

    ptPartInfo = (SPI_NAND_PARTITION_INFO_t *)gtSpiNandDrv.pu8_pagebuf;
    for (u8_i=0 ; u8_i<10 ; u8_i+=2)
    {
        u32_ret = MDrv_SPINAND_Read(u8_i*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
        if (u32_ret == ERR_SPINAND_SUCCESS)
        {
            if (memcmp((const void *) gtSpiNandDrv.pu8_pagebuf, SPINAND_FLASH_INFO_TAG, 16) == 0)
            {
                u32_ret = MDrv_SPINAND_Read(u8_i*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt+1, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
                if (u32_ret == ERR_SPINAND_SUCCESS)
                {
                    if (ptPartInfo->u32_ChkSum == CheckSum((u8*)&(ptPartInfo->u16_SpareByteCnt), 0x200 - 0x04))
                        break;
                }
            }
        }
    }
    if (u8_i == 10)
    {
        spi_nand_warn("CIS doesn't contain part info\r\n");
        gtSpiNandDrv.u8_HasPNI = 0;
    }
    else
    {
        spi_nand_msg("CIS contains part info\r\n");
        gtSpiNandDrv.u8_HasPNI = 1;
        memcpy((void *)&gtSpiNandDrv.tPartInfo, (const void *) ptPartInfo, 0x200);
    }

    /* please refer to include/linux/nand.h for more info. */
    nand->read_byte = spi_nand_read_byte;
    nand->read_word = spi_nand_read_word;
    nand->write_buf = spi_nand_write_buf;
    nand->read_buf = spi_nand_read_buf;
    nand->select_chip = spi_nand_select_chip;
    nand->cmd_ctrl = spi_nand_cmd_ctrl;
    nand->dev_ready = spi_nand_dev_ready;
    nand->cmdfunc = spi_nand_cmdfunc;
    nand->waitfunc = spi_nand_waitfunc;
    nand->write_page = spi_nand_write_page;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,1)
    nand->bbt_options = NAND_BBT_USE_FLASH;
#else
    nand->options = NAND_USE_FLASH_BBT;
#endif

    nand->chip_delay = 0;
    nand->badblock_pattern = &spi_nand_bbt_descr; //using default badblock pattern.
    nand->bbt_td = &spi_nand_bbt_main_descr;
    nand->bbt_md = &spi_nand_bbt_mirror_descr;

    nand->ecc.mode = NAND_ECC_HW;
    nand->ecc.size = gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
    nand->ecc.bytes = (gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt>>1);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,1)
    nand->ecc.strength                  = 4;
#endif
    nand->ecc.layout =  &spi_nand_oobinfo;

    nand->ecc.hwctl = spi_nand_ecc_hwctl;
    nand->ecc.calculate = spi_nand_ecc_calculate;
    nand->ecc.correct = spi_nand_ecc_correct;
    nand->ecc.read_page_raw = spi_nand_ecc_read_page_raw;
    nand->ecc.write_page_raw = spi_nand_ecc_write_page_raw;
    nand->ecc.read_page = spi_nand_ecc_read_page;
    nand->ecc.read_subpage = spi_nand_ecc_read_subpage;
    nand->ecc.write_page = spi_nand_ecc_write_page;
    nand->ecc.read_oob = spi_nand_ecc_read_oob;
    nand->ecc.write_oob = spi_nand_ecc_write_oob;

    nand->options |= NAND_IS_SPI;

    if ((err = nand_scan(mtd, 1)) != 0)
    {
        spi_nand_err("can't register SPI NAND\n");
        return -ENOMEM;
    }

#ifdef CONFIG_MTD_CMDLINE_PARTS
    {
        int mtd_parts_nb = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,1)
        mtd_parts_nb = parse_cmdline_partitions(mtd, &info->parts, NULL);
#else
        mtd_parts_nb = parse_cmdline_partitions(mtd, &info->parts, "nand");
#endif

        if (mtd_parts_nb > 0)
            add_mtd_partitions(mtd, info->parts, mtd_parts_nb);
        else
            add_mtd_partitions(mtd, NULL, 0);
    }
#else
    add_mtd_partitions(mtd, NULL, 0);
#endif

    platform_set_drvdata(pdev, &info->mtd);

#if defined(SPINAND_MEASURE_PERFORMANCE) && SPINAND_MEASURE_PERFORMANCE
    u64_TotalWriteBytes = u64_TotalReadBytes = 0;
    writefile = create_proc_entry (procfs_name, 0644, NULL);
    if (writefile == NULL)
        printk(KERN_CRIT"Error: Can not initialize /proc/%s\n", procfs_name);
    else
    {
        writefile->read_proc = procfile_read;
        writefile->mode      = S_IFREG | S_IRUGO;
        writefile->uid       = 0;
        writefile->gid      = 0;
        writefile->size      = 0x10;
    }
#endif

    return 0;
}

static int mstar_spinand_remove(struct platform_device *pdev)
{
    platform_set_drvdata(pdev, NULL);

    /* Release NAND device, its internal structures and partitions */
    nand_release(&info->mtd);
    kfree(info);
    kfree(gtSpiNandDrv.pu8_pagebuf);
    kfree(gtSpiNandDrv.pu8_sparebuf);

    return 0;
}

static struct platform_device mstar_spinand_deivce_st =
{
    .name = DRIVER_NAME,
    .id = 0,
    .resource = NULL,
    .num_resources = 0,
};

static struct platform_driver mstar_spinand_driver = {
    .probe 		= mstar_spinand_probe,
    .remove		= mstar_spinand_remove,
    .driver = {
        .name 	= DRIVER_NAME,
        .owner	= THIS_MODULE,
    },
};

static int __init mstar_spinand_init(void)
{
    int err = 0;

    if (MDrv_SPINAND_IsActive() == 0)
    {
        pr_info("%s device not found\n", DRIVER_NAME);
        return -ENODEV;
    }

    pr_info("%s driver initializing\n", DRIVER_NAME);

    err = platform_device_register(&mstar_spinand_deivce_st);
    if (err < 0)
        spi_nand_err("SPI NAND Err: platform device register fail %d\n", err);

    return platform_driver_register(&mstar_spinand_driver);
}

static void __exit mstar_spinand_exit(void)
{
    platform_driver_unregister(&mstar_spinand_driver);
}

module_init(mstar_spinand_init);
module_exit(mstar_spinand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MStar");
MODULE_DESCRIPTION("MStar MTD SPI NAND driver");
