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
// @file   mdrv_ldm_io.h
// @brief  GFlip KMD Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LDM_IO_H
#define _MDRV_LDM_IO_H

//=============================================================================
// Includs
//=============================================================================


//=============================================================================
// Defines
//=============================================================================
//IO Ctrl defines:
#define MDRV_LDM_IOC_INIT_NR        (0)
#define MDRV_LDM_IOC_DEINIT_NR      (MDRV_LDM_IOC_INIT_NR+1)

//#define MDRV_LDM_IOC_MAX_NR              (MDRV_LDM_IOC_DEINIT_NR+1)



// use 'L' as magic number
#define MDRV_LDM_IOC_MAGIC      ('L')
#define MDRV_LDM_IOC_INIT       _IO(MDRV_LDM_IOC_MAGIC, 0)
#define MDRV_LDM_IOC_DEINIT     _IO(MDRV_LDM_IOC_MAGIC, 1)
#define MDRV_LDM_IOC_ENABLE                 _IOW(MDRV_LDM_IOC_MAGIC, 2, MS_U32)
#define MDRV_LDM_IOC_DISABLE                _IOW(MDRV_LDM_IOC_MAGIC, 3, MS_U32)
#define MDRV_LDM_IOC_SUSPEND    _IOW(MDRV_LDM_IOC_MAGIC, 4, MS_U32)
#define MDRV_LDM_IOC_RESUME                 _IOW(MDRV_LDM_IOC_MAGIC, 5, MS_U32)
#define MDRV_LDM_IOC_STRENGTH               _IOW(MDRV_LDM_IOC_MAGIC, 6, MS_U32)
#define MDRV_LDM_IOC_GET_DATA               _IOR(MDRV_LDM_IOC_MAGIC, 7, MS_U32)
#define MDRV_LDM_IOC_DEBUG                  _IOW(MDRV_LDM_IOC_MAGIC, 8, MS_U32)
#define MDRV_LDM_IOC_CUS_PATH               _IOW(MDRV_LDM_IOC_MAGIC, 9, MS_U32)
#define MDRV_LDM_IOC_DEMO_PATTERN           _IOW(MDRV_LDM_IOC_MAGIC, 10, MS_U32)
#define MDRV_LDM_IOC_MAX_NR                 (11)


typedef enum
{
    E_DATA_TYPE_LDF=0x01,
    E_DATA_TYPE_LDB=0x02,
    E_DATA_TYPE_SPI=0x03,
    E_DATA_TYPE_MAX   
}EN_GET_DATA_TYPE;

typedef enum
{
    E_DEBUG_LEVEL_ERROR=0x01,
    E_DEBUG_LEVEL_INFO=0x02,
    E_DEBUG_LEVEL_DEBUG=0x04,
    E_DEBUG_LEVEL_ALL=0x07,
    E_DEBUG_LEVEL_MAX   
}EN_DEBUG_LEVEL;

typedef enum
{
    E_DEMO_PATTERN_LEFT_RIGHT_HALF=0x01,
    E_DEMO_PATTERN_MARQUEE=0x02,
    E_DEMO_PATTERN_LEFT_RIGHT_COLOR_SHELTER=0x03,
    E_DEMO_PATTERN_MAX
}EN_DEMO_PATTERN;

typedef struct
{
    MS_U8 aCusPath[128];
    MS_U8 aCusPathU[128];
}ST_DRV_LD_CUS_PATH;

typedef struct
{
    EN_GET_DATA_TYPE enDataType;
    phys_addr_t addr;
}ST_DRV_LD_GET_DATA;

typedef struct
{
    EN_DEMO_PATTERN enDemoPattern;
    MS_BOOL bOn;
}ST_DRV_LD_DEMO_PATTERN;

#endif //_MDRV_LDM_IO_H
