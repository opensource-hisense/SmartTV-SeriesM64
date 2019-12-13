////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mdrv_ldm_algorithm.c
/// @brief  MStar local dimming   file
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////


//=============================================================================
// Include Files
//=============================================================================

#if (defined (CONFIG_HAS_LD) || defined (CONFIG_HAS_LD_MODULE))

#include <linux/types.h>
#include <linux/fs.h>
#include <asm/io.h>

#include "mdrv_mstypes.h"
#include "mdrv_ldm_io.h"
#include "mdrv_ldm_common.h"
#include "mdrv_ldm_interface.h"
#include "mdrv_ldm_algorithm.h"
#include "mdrv_ldm_parse.h"
#include "mhal_ldm.h"
#include "reg_ldm.h"
#include "mdrv_mbx.h"

#include <linux/slab.h>
#include <linux/sched.h>

//=============================================================================
// Compile options
//=============================================================================

extern void Chip_Flush_Memory(void); 

//=============================================================================
// Local Defines
//=============================================================================


//=============================================================================
// Macros
//=============================================================================
#define _MDRV_LDM_ALOG_C

//mmap: #define E_MMAP_ID_LOCAL_DIMMING_LEN                            0x0000100000
#define LDM_LEN 0x100000        //
#define SPI_BUFF_OFFSET           0x80000        //
#define PM_MBX_TIMEOUT          300

//local dimming miu read / write  use va:u64Vaddr, not pa:addr_base_L
#if 1
#define MDrv_LD_MIUReadByte(addr_base_L, offset)           (*((unsigned char  volatile *)(u64Vaddr + (addr_base_L) + (offset))))
#define MDrv_LD_MIURead2Bytes(addr_base_L, offset)           (*((volatile MS_U16 *) (u64Vaddr + (addr_base_L) + (offset))))
#define MDrv_LD_MIUWriteByte(addr_base_L, offset, value)    (*((unsigned char  volatile *)(u64Vaddr + (addr_base_L) + (offset)))) = ((MS_U8)(value))
#define MDrv_LD_MIUWrite2Bytes(addr_base_L, offset, val)  (*((volatile MS_U16*)(u64Vaddr + (addr_base_L) + (offset)))) = (MS_U16)(val)
#else
#define MDrv_LD_MIUReadByte(_BUF_ID, _offset)             (*(volatile unsigned char *) (MsOS_PA2KSEG0(_BUF_ID + _offset) ))
#define MDrv_LD_MIURead2Bytes(_BUF_ID, _offset)           (*(volatile MS_U16 *) (MsOS_PA2KSEG0( _BUF_ID + _offset) ))
#define MDrv_LD_MIUWriteByte(_BUF_ID, _offset, _val)    (*((volatile MS_U8*)(MsOS_PA2KSEG0(_BUF_ID + _offset) ))) = (MS_U8)(_val)
#define MDrv_LD_MIUWrite2Bytes(_BUF_ID, _offset, _val)  (*((volatile MS_U16*)(MsOS_PA2KSEG0(_BUF_ID + _offset) ))) = (MS_U16)(_val)
#endif

//=============================================================================
// Local Variables
//=============================================================================
static DEFINE_MUTEX(Semutex_LD);
static DEFINE_SPINLOCK(spinlock_ld);



static MS_BOOL bReceived = FALSE;


//static void __iomem * u64Vaddr = 0;  
static struct file *mbx_fp = NULL; 

static MS_U8 *pLDF_blocks = NULL;
static MS_U16 *pSD_blocks = NULL;
static MS_U16 *pTF_blocks = NULL;
static MS_U16 *pSF_blocks = NULL;
static MS_U8 *pLDB_blocks = NULL;
static MS_U16 *pSPI_blocks = NULL;
static MS_U16 gu16LedWidth;
static MS_U16 gu16LedHeight;


//=============================================================================
// Global Variables
//=============================================================================
//static phys_addr_t ld_addr_base = 0;
static MS_BOOL bLdOn = FALSE;
static MS_U8 u8LdSuspend = 0;
static struct work_struct *pstWork = NULL;


extern phys_addr_t ld_addr_base;
extern void __iomem * u64Vaddr;  

// *************************programed by initial function***********************************************//
#ifdef LD_SUPPORT_1XN_NX1
extern MS_BOOL s_bMDrvLD_Led_1xN;
extern MS_BOOL s_bMDrvLD_Led_Nx1;
#endif


const U8 Tbl_LD_Gamma_1dot9[256] = {
   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,
   2,   2,   2,   2,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,
   5,   6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,  10,  10,  10,  11,
  11,  12,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,
  19,  19,  20,  21,  21,  22,  22,  23,  24,  24,  25,  25,  26,  27,  27,  28,
  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,  36,  36,  37,  38,  39,  40,
  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  49,  49,  50,  51,  52,  53,
  54,  55,  56,  57,  58,  59,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,
  69,  70,  71,  72,  73,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,
  87,  88,  89,  90,  91,  92,  94,  95,  96,  97,  98, 100, 101, 102, 103, 104,
 106, 107, 108, 109, 111, 112, 113, 115, 116, 117, 119, 120, 121, 123, 124, 125,
 127, 128, 129, 131, 132, 133, 135, 136, 138, 139, 141, 142, 143, 145, 146, 148,
 149, 151, 152, 154, 155, 157, 158, 160, 161, 163, 164, 166, 167, 169, 171, 172,
 174, 175, 177, 178, 180, 182, 183, 185, 187, 188, 190, 191, 193, 195, 196, 198,
 200, 202, 203, 205, 207, 208, 210, 212, 214, 215, 217, 219, 221, 222, 224, 226,
 228, 230, 231, 233, 235, 237, 239, 241, 242, 244, 246, 248, 250, 252, 254, 255
};

const U8 Tbl_LD_Gamma_2dot0[256] = {
   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   4,   4,   4,   4,
   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,
  10,  10,  10,  11,  11,  12,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,
  17,  17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,
  26,  26,  27,  28,  28,  29,  30,  30,  31,  32,  32,  33,  34,  34,  35,  36,
  37,  37,  38,  39,  40,  41,  41,  42,  43,  44,  45,  45,  46,  47,  48,  49,
  50,  51,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,
  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  80,  81,
  82,  83,  84,  85,  86,  88,  89,  90,  91,  92,  94,  95,  96,  97,  98, 100,
 101, 102, 103, 105, 106, 107, 109, 110, 111, 113, 114, 115, 117, 118, 119, 121,
 122, 123, 125, 126, 128, 129, 130, 132, 133, 135, 136, 138, 139, 141, 142, 144,
 145, 147, 148, 150, 151, 153, 154, 156, 157, 159, 161, 162, 164, 165, 167, 169,
 170, 172, 173, 175, 177, 178, 180, 182, 183, 185, 187, 189, 190, 192, 194, 196,
 197, 199, 201, 203, 204, 206, 208, 210, 212, 213, 215, 217, 219, 221, 223, 225,
 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 255
};

const U8 Tbl_LD_Gamma_2dot1[256] = {
   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,
   3,   3,   4,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,
   7,   7,   8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,
  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,
  20,  21,  22,  22,  23,  23,  24,  24,  25,  26,  26,  27,  28,  28,  29,  30,
  30,  31,  32,  32,  33,  34,  34,  35,  36,  37,  37,  38,  39,  40,  41,  41,
  42,  43,  44,  45,  46,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
  56,  57,  58,  59,  60,  61,  62,  63,  64,  66,  67,  68,  69,  70,  71,  72,
  73,  74,  75,  76,  78,  79,  80,  81,  82,  83,  85,  86,  87,  88,  89,  91,
  92,  93,  94,  96,  97,  98, 100, 101, 102, 104, 105, 106, 108, 109, 110, 112,
 113, 115, 116, 118, 119, 120, 122, 123, 125, 126, 128, 129, 131, 132, 134, 136,
 137, 139, 140, 142, 143, 145, 147, 148, 150, 152, 153, 155, 157, 158, 160, 162,
 163, 165, 167, 169, 170, 172, 174, 176, 177, 179, 181, 183, 185, 187, 188, 190,
 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222,
 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 245, 247, 249, 251, 253, 255
};

const U8 Tbl_LD_Gamma_2dot2[256] = {
   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   4,
   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   7,   7,   7,   7,   8,
   8,   8,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  13,  13,  14,  14,
  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  21,  21,  22,  22,
  23,  23,  24,  25,  25,  26,  27,  27,  28,  28,  29,  30,  30,  31,  32,  33,
  33,  34,  35,  35,  36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,
  46,  47,  48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  59,
  60,  61,  62,  63,  64,  65,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,
  77,  78,  80,  81,  82,  83,  84,  85,  87,  88,  89,  90,  91,  93,  94,  95,
  96,  98,  99, 100, 101, 103, 104, 105, 107, 108, 109, 111, 112, 113, 115, 116,
 118, 119, 120, 122, 123, 125, 126, 128, 129, 130, 132, 133, 135, 136, 138, 139,
 141, 143, 144, 146, 147, 149, 150, 152, 154, 155, 157, 158, 160, 162, 163, 165,
 167, 168, 170, 172, 174, 175, 177, 179, 180, 182, 184, 186, 188, 189, 191, 193,
 195, 197, 198, 200, 202, 204, 206, 208, 210, 211, 213, 215, 217, 219, 221, 223,
 225, 227, 229, 231, 233, 235, 237, 239, 241, 243, 245, 247, 249, 251, 253, 255
};

const U8 Tbl_LD_Gamma_2dot3[256] = {
   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
   6,   6,   7,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,  10,  10,  11,
  11,  11,  12,  12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,
  18,  19,  19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  26,  27,
  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,  36,  37,  37,  38,
  39,  40,  41,  41,  42,  43,  44,  45,  46,  46,  47,  48,  49,  50,  51,  52,
  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,
  69,  70,  71,  72,  73,  75,  76,  77,  78,  79,  80,  82,  83,  84,  85,  87,
  88,  89,  90,  92,  93,  94,  96,  97,  98, 100, 101, 102, 104, 105, 106, 108,
 109, 111, 112, 113, 115, 116, 118, 119, 121, 122, 124, 125, 127, 129, 130, 132,
 133, 135, 136, 138, 140, 141, 143, 145, 146, 148, 150, 151, 153, 155, 157, 158,
 160, 162, 164, 165, 167, 169, 171, 173, 175, 176, 178, 180, 182, 184, 186, 188,
 190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220,
 222, 224, 227, 229, 231, 233, 235, 237, 240, 242, 244, 246, 249, 251, 253, 255
};

const U8 Tbl_LD_Gamma_2dot4[256] = {
   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
   2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   5,   5,
   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,   9,   9,   9,
  10,  10,  10,  11,  11,  12,  12,  12,  13,  13,  14,  14,  14,  15,  15,  16,
  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  23,  23,  24,  24,
  25,  26,  26,  27,  27,  28,  29,  29,  30,  31,  32,  32,  33,  34,  34,  35,
  36,  37,  37,  38,  39,  40,  41,  41,  42,  43,  44,  45,  46,  47,  47,  48,
  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,
  65,  66,  67,  68,  70,  71,  72,  73,  74,  75,  77,  78,  79,  80,  81,  83,
  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  98, 100, 101, 102, 104,
 105, 107, 108, 110, 111, 113, 114, 116, 117, 119, 120, 122, 123, 125, 126, 128,
 130, 131, 133, 134, 136, 138, 139, 141, 143, 145, 146, 148, 150, 152, 153, 155,
 157, 159, 161, 162, 164, 166, 168, 170, 172, 174, 176, 177, 179, 181, 183, 185,
 187, 189, 191, 193, 195, 197, 200, 202, 204, 206, 208, 210, 212, 214, 217, 219,
 221, 223, 225, 228, 230, 232, 234, 237, 239, 241, 244, 246, 248, 251, 253, 255
};

const U8 Tbl_LD_Gamma_2dot5[256] = {
   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,
   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,
   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,  13,  14,  14,
  15,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,  22,  22,
  23,  23,  24,  24,  25,  26,  26,  27,  28,  28,  29,  30,  30,  31,  32,  32,
  33,  34,  35,  35,  36,  37,  38,  38,  39,  40,  41,  42,  43,  43,  44,  45,
  46,  47,  48,  49,  50,  51,  52,  53,  53,  54,  55,  56,  57,  58,  60,  61,
  62,  63,  64,  65,  66,  67,  68,  69,  70,  72,  73,  74,  75,  76,  78,  79,
  80,  81,  83,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99, 100,
 101, 103, 104, 106, 107, 109, 110, 112, 113, 115, 116, 118, 120, 121, 123, 124,
 126, 128, 129, 131, 133, 134, 136, 138, 139, 141, 143, 145, 146, 148, 150, 152,
 154, 156, 157, 159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183,
 185, 187, 189, 191, 193, 195, 198, 200, 202, 204, 206, 208, 211, 213, 215, 217,
 220, 222, 224, 227, 229, 231, 234, 236, 238, 241, 243, 246, 248, 251, 253, 255
};
const U8* pTbl_LD_Gamma[16] = {
    NULL, // NULL indicates linear
    Tbl_LD_Gamma_2dot2,
    Tbl_LD_Gamma_2dot1,
    Tbl_LD_Gamma_2dot3,
    Tbl_LD_Gamma_2dot0,
    Tbl_LD_Gamma_2dot4,
    Tbl_LD_Gamma_1dot9,
    Tbl_LD_Gamma_2dot5,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

const U8* pTbl_LD_Remap[16] = {
    NULL, // NULL indicates linear
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};


//=============================================================================
// Local Function Prototypes
//=============================================================================
static MS_S8 MDrv_LD_SendMsg2RT51(phys_addr_t BaseAddr, MS_U8 u8LEDNum);

//=============================================================================
// Local Function
//=============================================================================

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
// Assertion : 0 <= IN_blocks[block_idx] < 0xFF00 ----> 0 <= SPI_blocks[block_idx] < 0xFFFF, 0 <= LDB_blocks[block_idx] < 0xFF
static void MDrv_LD_AlgoPostProcess(const MS_U16* IN_blocks, MS_U16* SPI_blocks, MS_U8* LDB_blocks)
{
    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 n_leds = gu16LedWidth * gu16LedHeight;
    MS_U16 block_idx = 0;
#ifdef LD_MEM_MODE
    MS_BOOL MEM_init = FALSE;
    static MS_U16 MEM_blocks[LD_MAX_BLOCK]; // for measurement
#endif
    MS_U16 backlight = 0;

    MS_U8 led_mod = MHal_LD_GetSWPulseMode();
    MS_U16 led_id = MHal_LD_GetSWPulseId();
    MS_U16 led_str = MHal_LD_GetSWPulseLEDIntensity();
    MS_U16 ldb_str = MHal_LD_GetSWPulseLDBIntensity();

    CHECK_POINTER_V(IN_blocks);
    CHECK_POINTER_V(SPI_blocks);
    CHECK_POINTER_V(LDB_blocks);
    
    // Form SPI signal & LDB data
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        backlight = IN_blocks[block_idx];
        // transform (0, ..., 0xFF00) to (0, ..., 0xFFFF)
        SPI_blocks[block_idx] = backlight + (backlight >> 8);
        // rounding to 8-bit
        LDB_blocks[block_idx] = (backlight + 0x80) >> 8;
    }

#ifdef LD_MEM_MODE
    if (!MEM_init)
    {
        #if 1
        memset(MEM_blocks, 0x00, n_leds*sizeof(MS_U16));
        #else
        for (block_idx = 0; block_idx < n_leds; block_idx++)
            MEM_blocks[block_idx] = 0;
        #endif
        MEM_init = TRUE;
    }
#endif

    if (led_mod == 0x0)
    {
        if (led_id == 0x1) // Left: right LSF, Right: LD on
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % gu16LedWidth >= gu16LedWidth/2)
                {
                    SPI_blocks[block_idx] = (led_str << 8) | led_str;
                    LDB_blocks[block_idx] = led_str;
                }
            }
        }
        else if (led_id == 0x2) // Left: LD off, Right: LD on
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % gu16LedWidth < gu16LedWidth/2)
                {
                    SPI_blocks[block_idx] = (led_str << 8) | led_str;
                    LDB_blocks[block_idx] = led_str;
                }
            }
        }
        else if (led_id == 0x3) // Left: LD on, Right: left LSF
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % gu16LedWidth >= gu16LedWidth/2)
                {
                    SPI_blocks[block_idx] = SPI_blocks[block_idx - gu16LedWidth/2];
                    LDB_blocks[block_idx] = LDB_blocks[block_idx - gu16LedWidth/2];
                }
            }
        }
        else if (led_id == 0x4) // Left: right LSF, Right: LD on
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % gu16LedWidth < gu16LedWidth/2)
                {
                    SPI_blocks[block_idx] = SPI_blocks[block_idx + gu16LedWidth/2];
                    LDB_blocks[block_idx] = LDB_blocks[block_idx + gu16LedWidth/2];
                }
            }
        }
    }
    else if (led_mod == 0xf)     // 22[15:12]=4'b1111  real backlight
    {
        if (led_id == 0xfff)
        {
            #if 0
            memset(SPI_blocks, 0xff, n_leds*sizeof(MS_U16));
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = 0xffff;
            #endif
        }
        else if (led_id < n_leds)
        {
            #if 0
            memset(SPI_blocks, 0x00, n_leds*sizeof(MS_U16));
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = 0;
            #endif
            SPI_blocks[led_id] = 0xffff;
        }
        MHal_LD_SetLSFOutMode(0x1);
    }
    else if (led_mod == 0xe)      // 22[15:12]=4'b1110  digital backlight
    {
        #if 0
        memset(SPI_blocks, 0xff, n_leds*sizeof(MS_U16));
        #else
        for (block_idx = 0; block_idx < n_leds; block_idx++)
            SPI_blocks[block_idx] = 0xffff;
        #endif
        if (led_id == 0xfff)
        {
            #if 0
            memset(LDB_blocks, 0xff, n_leds);
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = 0xff;
            #endif
        }
        else if (led_id < n_leds)
        {
            #if 0
            memset(LDB_blocks, 0x00, n_leds);
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = 0x00;
            #endif
            LDB_blocks[led_id] = 0xff;
        }
        MHal_LD_SetLSFOutMode(0x5);
    }
    else if (led_mod == 0xd)      // 22[15:12]=4'b1101 tunable real backlight
    {
        if (led_id == 0xfff)
        {
            #if 0
            memset(SPI_blocks, ((led_str << 8) | ldb_str), n_leds*sizeof(MS_U16));
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = (led_str << 8) | ldb_str;;
            #endif
        }
        else if (led_id < n_leds)
        {
            #if 0
            memset(SPI_blocks, 0x00, n_leds*sizeof(MS_U16));
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = 0;
            #endif
            SPI_blocks[led_id] = (led_str << 8) | ldb_str;;
        }
    }
    else if (led_mod == 0xc)      // 22[15:12]=4'b1100 tunable digital backlight
    {
        #if 0
        MS_U16 u16Temp = (led_str << 8) | led_str;
        memset(SPI_blocks, u16Temp, n_leds*sizeof(MS_U16));
        #else
        for (block_idx = 0; block_idx < n_leds; block_idx++)
            SPI_blocks[block_idx] = (led_str << 8) | led_str;
        #endif
        if (led_id == 0xfff)
        {
            #if 0
            memset(LDB_blocks, ldb_str, n_leds);
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = ldb_str;
            #endif
        }
        else if (led_id < n_leds)
        {
            #if 0
            memset(LDB_blocks, 0x00, n_leds);
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = 0;
            #endif
            LDB_blocks[led_id] = ldb_str;
        }
    }
#ifdef LD_MEM_MODE
    else if (led_mod == 0xb)      // 22[15:12]=4'b1011 set SPI
    {
        if (led_id == 0xfff)
        {
            #if 1
            MS_U16 u16Temp = (led_str << 8) | ldb_str;
            memset(MEM_blocks, u16Temp, n_leds);
            memset(SPI_blocks, u16Temp, n_leds*sizeof(MS_U16));
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = MEM_blocks[block_idx] = (led_str << 8) | ldb_str;
            #endif
        }
        else if (led_id < n_leds)
        {
            #if 1
            memcpy(SPI_blocks, MEM_blocks, n_leds*sizeof(MS_U16));
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = MEM_blocks[block_idx];
            #endif
            SPI_blocks[led_id] = MEM_blocks[led_id] = (led_str << 8) | ldb_str;;
        }
    }
    else if (led_mod == 0xa)      // 22[15:12]=4'b1010 set LDB
    {
        #if 1
        MS_U16 u16Temp = (led_str << 8) | led_str;
        memset(SPI_blocks, u16Temp, n_leds*sizeof(MS_U16));
        #else
        for (block_idx = 0; block_idx < n_leds; block_idx++)
            SPI_blocks[block_idx] = (led_str << 8) | led_str;
        #endif
        if (led_id == 0xfff)
        {
            #if 1
            memset(MEM_blocks, ldb_str, n_leds);
            memset(SPI_blocks, ldb_str, n_leds*sizeof(MS_U16));
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = MEM_blocks[block_idx] = ldb_str;
            #endif
        }
        else if (led_id < n_leds)
        {
            #if 1
            memcpy(LDB_blocks, (MS_U8 *)MEM_blocks, n_leds);
            #else
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = MEM_blocks[block_idx];
            #endif
            LDB_blocks[led_id] = MEM_blocks[led_id] = ldb_str;
        }
    }
#endif

#ifdef LD_SUPPORT_1XN_NX1
    // Map 2xN back to 1xN LED
    if (s_bMDrvLD_Led_1xN)
    {
        for (block_idx = 0; block_idx < n_leds; block_idx += 2)
        {
            if (SPI_blocks[block_idx] < SPI_blocks[block_idx+1])
                SPI_blocks[block_idx] = SPI_blocks[block_idx+1];
            SPI_blocks[block_idx+1] = 0;
            if (LDB_blocks[block_idx] < LDB_blocks[block_idx+1])
                LDB_blocks[block_idx] = LDB_blocks[block_idx+1];
            LDB_blocks[block_idx+1] = 0;
        }
    }
    // Map Nx2 back to Nx1 LED
    if (s_bMDrvLD_Led_Nx1)
    {
        for (block_idx = 0; block_idx < gu16LedWidth; block_idx ++)
        {
            if (SPI_blocks[block_idx] < SPI_blocks[block_idx+gu16LedWidth])
                SPI_blocks[block_idx] = SPI_blocks[block_idx+gu16LedWidth];
            SPI_blocks[block_idx+gu16LedWidth] = 0;
            if (LDB_blocks[block_idx] < LDB_blocks[block_idx+gu16LedWidth])
                LDB_blocks[block_idx] = LDB_blocks[block_idx+gu16LedWidth];
            LDB_blocks[block_idx+gu16LedWidth] = 0;
        }
    }
#endif
}

static void MDrv_LD_AlgoLocalDimmingStrength(MS_U16* BL_blocks)
{
    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 n_leds = gu16LedWidth * gu16LedHeight;
    MS_U16 block_idx = 0;
    MS_U16 bl_val = 0, max_bl_val = 0;

    MS_U32 ld_str = MHal_LD_GetSWLocalDimmingStrength(); // 0 ~ 0xFF
    MS_U16 min_bl_val = MHal_LD_GetSWMinClampValue();

    CHECK_POINTER_V(BL_blocks);

    ld_str = (ld_str > 0x80) ? ld_str+1 : ld_str;
    min_bl_val <<= 8; // 0 ~ 0xFF00

    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        bl_val = BL_blocks[block_idx];
        if (bl_val > max_bl_val)
            max_bl_val = bl_val;
    }
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        bl_val = (BL_blocks[block_idx] * ld_str + max_bl_val * (256 - ld_str) + 0x80) >> 8; // 0 ~ 0xFF00
        if (bl_val < min_bl_val)
            bl_val = min_bl_val;
        BL_blocks[block_idx] = bl_val;
    }
    
}

// Assertion : 0 <= BL_blocks[block_idx] <= 0xFFFF
static void MDrv_LD_AlgoGlobalDimmingStrength(MS_U16* BL_blocks)
{
    static MS_U8 ReEntrance = 0;
    MS_U16 block_idx = 0;
    MS_S8 ret = 0;
    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 n_leds = gu16LedWidth * gu16LedHeight;

    MS_U32 gd_str = MHal_LD_GetSWGlobalDimmingStrength(); // 0 ~ 0xFF

    if(1 == ReEntrance)
    {
        LD_ERROR("warning!  resend! \n");
        return;
    }

    CHECK_POINTER_V(BL_blocks);

    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        BL_blocks[block_idx] = (BL_blocks[block_idx] * gd_str) / 255;
    }
    
    memcpy(pSPI_blocks,BL_blocks,n_leds);
    
    Chip_Flush_Memory();


    bReceived = FALSE;   
    ReEntrance = 1;
    ret = MDrv_LD_SendMsg2RT51((phys_addr_t)(ld_addr_base+SPI_BUFF_OFFSET), n_leds*sizeof(MS_U16));

    ReEntrance = 0;
    if(0 != ret)
    {
        LD_ERROR(" send addr_base to rt51 fail ret:%d\n",ret);
    }
}

// Assertion : 0 <= LDF_blocks[block_idx] <= 0xFF ----> 0 <= SD_blocks[block_idx] <= 0xFF00
static void MDrv_LD_AlgoScalingDown(const MS_U8* LDF_blocks, MS_U16* SD_blocks)
{
    MS_U16 ldf_width = MHal_LD_GetLDFBacklightWidth();
    MS_U16 ldf_height = MHal_LD_GetLDFBacklightHeight();
    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 n_leds = gu16LedWidth * gu16LedHeight;
    MS_U16 hsd_factor = ldf_width / gu16LedWidth;
    MS_U16 vsd_factor = ldf_height / gu16LedHeight;
    MS_U16 LDF_idx = 0, SD_idx = 0, i = 0, j = 0;

    /*
       * modify MS_U16 SortMax_blocks[LD_MAX_BW*LD_MAX_BH];
       *            --> static MS_U16 SortMax_blocks[LD_MAX_BW*LD_MAX_BH]; because:
       * 1. compile warning:warning: the frame size of 2064 bytes is larger than 1536 bytes [-Wframe-larger-than=],
       *     and it's coredump when run:
       */
    //MS_U16 SortMax_blocks[LD_MAX_BW*LD_MAX_BH];
    static MS_U16 SortMax_blocks[LD_MAX_BW*LD_MAX_BH];
    MS_U16 m = 0, n = 0, k = 0;
    MS_U16 u16Max_nth = MHal_LD_GetSWMaxThreshold();
    MS_U32 alpha = MHal_LD_GetLDFDCMaxAlpha();
    MS_U32 block_cur = 0, block_mean = 0, block_max = 0; // 0 ~ 0xFF00

    u16Max_nth = MIN(u16Max_nth, hsd_factor*vsd_factor - 1);

    CHECK_POINTER_V(LDF_blocks);
    CHECK_POINTER_V(SD_blocks);

    #if 0
    memset(SD_blocks, 0x00, n_leds*sizeof(MS_U16));
    #else
    for (SD_idx = 0; SD_idx < n_leds; SD_idx++)
        SD_blocks[SD_idx] = 0;
    #endif

    if (MHal_LD_GetLDFWriteDCMaxOFEn()) // 4 bytes : OF[15:8] OF[7:0] Max DC
    {
        #ifdef LD_SUPPORT_1XN_NX1
        if (s_bMDrvLD_Led_1xN)
        {
            gu16LedWidth = 1;
            hsd_factor = ldf_width;
        }
        if (s_bMDrvLD_Led_Nx1)
        {
            gu16LedHeight = 1;
            vsd_factor = ldf_height;
        }
        #endif

        for (n = 0; n < gu16LedHeight; n++)
        {
            for (m = 0; m < gu16LedWidth; m++)
            {
                SD_idx = m + n*gu16LedWidth;

                for (k = 0; k <= u16Max_nth; k++)
                    SortMax_blocks[k] = 0;

                block_cur = 0;
                for (j = 0; j < vsd_factor; j++)
                {
                    for (i = 0; i < hsd_factor; i++)
                    {
                        LDF_idx = (i + m*hsd_factor) + (j + n*vsd_factor)*ldf_width;

                        block_mean = LDF_blocks[LDF_idx*4];
                        block_max = LDF_blocks[LDF_idx*4 + 1];

                        block_cur += block_mean;

                        if (block_max > SortMax_blocks[u16Max_nth])
                        {
                            for (k = u16Max_nth+1; k > 0; k--)
                            {
                                if (block_max <= SortMax_blocks[k-1])
                                    break;
                                SortMax_blocks[k] = SortMax_blocks[k-1];
                            }
                            SortMax_blocks[k] = block_max;
                        }
                    }
                }
                block_mean = (block_cur << 8) / (hsd_factor * vsd_factor);
                block_max = SortMax_blocks[u16Max_nth] << 8;
                SD_blocks[SD_idx] = (alpha * block_max + (8-alpha) * block_mean + (0x1 << 2)) >> 3;  
            }
        }

        #ifdef LD_SUPPORT_1XN_NX1
        if (s_bMDrvLD_Led_1xN)
        {
            for (n = gu16LedHeight; n > 0; n--)
                SD_blocks[n*2 - 2] = SD_blocks[n*2 - 1] = SD_blocks[n - 1];
        }
        if (s_bMDrvLD_Led_Nx1)
        {
            for (m = 0; m < gu16LedWidth; m++)
                SD_blocks[m + gu16LedWidth] = SD_blocks[m];
        }
        #endif
    }
    else if (MHal_LD_GetLDFWriteDCMaxEn()) // 2 bytes : Max DC
    {
        for (j = 0; j < ldf_height; j++)
        {
            for (i = 0; i < ldf_width; i++)
            {
                LDF_idx = i + j*ldf_width;

                block_mean = LDF_blocks[LDF_idx*2];
                block_mean <<= 8;
                block_max = LDF_blocks[LDF_idx*2 + 1];
                block_max <<= 8;
                block_cur = (alpha * block_max + (8-alpha) * block_mean + (0x1 << 2)) >> 3;

                SD_idx = (i / hsd_factor) + (j / vsd_factor)*gu16LedWidth;
                if (block_cur > SD_blocks[SD_idx])
                    SD_blocks[SD_idx] = block_cur;
            }
        }
    }
    else // 1 bytes : BLEND
    {
        for (j = 0; j < ldf_height; j++)
        {
            for (i = 0; i < ldf_width; i++)
            {
                LDF_idx = i + j*ldf_width;
                block_cur = LDF_blocks[LDF_idx];
                block_cur <<= 8;

                SD_idx = (i / hsd_factor) + (j / vsd_factor)*gu16LedWidth;
                if (block_cur > SD_blocks[SD_idx])
                    SD_blocks[SD_idx] = block_cur;
            }
        }
    }

#ifdef LD_HISTOGRAM
    MS_U16 bin_idx = 0;
    for (bin_idx = 0; bin_idx < 32; bin_idx++)
        HISTOGRAM_bins[bin_idx] = 0;
    for (block_idx = 0; block_idx < n_leds; block_idx++)
        HISTOGRAM_bins[SD_blocks[block_idx] >> 12] ++;
#endif
}

static void MDrv_LD_AlgoBacklightGamma(MS_U16* BL_blocks, const MS_U8* Table)
{
    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 n_leds = gu16LedWidth * gu16LedHeight;
    MS_U16 block_idx = 0;
    MS_U16 bl_msb = 0, bl_lsb = 0; // 0 ~ 0xFF
    MS_U32 bl_low = 0, bl_high = 0; // 0 ~ 0xFF00

    CHECK_POINTER_V(BL_blocks);
    //CHECK_POINTER_V(Table)   //maybe null

    if(NULL == Table)
    {
        return; //do nothing
    }

    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        bl_msb = BL_blocks[block_idx] >> 8;
        bl_lsb = BL_blocks[block_idx] & 0xFF;
        bl_low = Table[bl_msb];
        bl_low <<= 8;
        bl_high = Table[(bl_msb == 0xFF) ? bl_msb : bl_msb + 1];
        bl_high <<= 8;
        BL_blocks[block_idx] = ((256-bl_lsb)*bl_low + bl_lsb*bl_high + 0x80) >> 8;
    }
}

// Assertion : 0 <= IN_blocks[block_idx] <= 0xFF00 ----> 0 <= OUT_blocks[block_idx] <= 0xFF00
static void MDrv_LD_AlgoTemporalFilter(const MS_U16* IN_blocks, MS_U16* OUT_blocks)
{
    static MS_U16 luma_pre = 0;
    static MS_U16 PRE_blocks[LD_MAX_BLOCK];

    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 n_leds = gu16LedWidth * gu16LedHeight;
    MS_U16 block_idx = 0;
    MS_U32 tf_strength_dn = MHal_LD_GetSWTemporalFilterStrengthDn(); // 0 ~ 0xFF
    MS_U32 tf_strength_up = MHal_LD_GetSWTemporalFilterStrengthUp(); // 0 ~ 0xFF
    MS_U32 luma_low = MHal_LD_GetSWTemporalFilterLowTh(); // 0 ~ 0xFF
    MS_U32 luma_high = MHal_LD_GetSWTemporalFilterHighTh(); // 0 ~ 0xFF
    MS_U32 alpha = 0, alpha_up = 0, alpha_dn = 0; // 0 ~ 0xFF
    MS_U16 max_speed = MHal_LD_GetTempFilterMaxSpeed();
    MS_U16 block_cur = 0, block_pre = 0;

    // get frame average luma
    MS_U32 luma_cur = 0, luma_diff = 0;
    MS_U32 luma_sum = 0;

    CHECK_POINTER_V(IN_blocks);
    CHECK_POINTER_V(OUT_blocks);

   for (block_idx = 0; block_idx < n_leds; block_idx++)
        luma_sum += IN_blocks[block_idx];
    luma_cur = luma_sum / n_leds; // 0 ~ 0xFF00
    luma_diff = (luma_cur > luma_pre) ? luma_cur - luma_pre : luma_pre - luma_cur;
    luma_pre = luma_cur; // 0 ~ 0xFF00

    // get blending alpha based on luma diff
    luma_low <<= 8;
    luma_high <<= 8;
    if (luma_diff >= luma_high)
    {
        alpha_up = 0;
        alpha_dn = 0;
    }
    else if (luma_diff <= luma_low)
    {
        alpha_up = tf_strength_up;
        alpha_dn = tf_strength_dn;
    }
    else
    {
        alpha_up = tf_strength_up * (luma_high - luma_diff) / (luma_high - luma_low);
        alpha_dn = tf_strength_dn * (luma_high - luma_diff) / (luma_high - luma_low);
    }

    max_speed <<= 8;
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        block_cur = IN_blocks[block_idx]; // 0 ~ 0xFF00
        block_pre = PRE_blocks[block_idx]; // 0 ~ 0xFF00

        // 16-bit IIR
        alpha =  (block_cur > block_pre) ? alpha_up : alpha_dn;
        block_cur = (alpha*block_pre + (256-alpha)*block_cur + 0x80) >> 8;
        if (block_cur == block_pre)
        {
            if (block_cur < IN_blocks[block_idx])
                block_cur ++;
            else if (block_cur > IN_blocks[block_idx])
                block_cur --;
        }

        // limit the max speed
        if (max_speed > 0)
        {
            if (block_cur > block_pre && block_cur - block_pre > max_speed)
                block_cur = block_pre + max_speed;
            else if (block_cur < block_pre && block_pre - block_cur > max_speed)
                block_cur = block_pre - max_speed;
        }

        OUT_blocks[block_idx] = block_cur;
        PRE_blocks[block_idx] = block_cur;
    }
}

// Assertion : 0 <= IN_blocks[block_idx] <= 0xFF00 ----> 0 <= OUT_blocks[block_idx] <= 0xFF00
static void MDrv_LD_AlgoSpatialFilter(const MS_U16* IN_blocks, MS_U16* OUT_blocks)
{
    MS_U16 block_idx = 0, neighbor_idx = 0;
    MS_U32 i = 0, j = 0, k = 0, p = 0, q = 0;
    MS_U16 sf_str[5];
    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 n_leds = gu16LedWidth * gu16LedHeight;
    EN_LD_TYPE led_type = (EN_LD_TYPE)MHal_LD_GetLEDType();

    CHECK_POINTER_V(IN_blocks);
    CHECK_POINTER_V(OUT_blocks);

    #if 0
    memcpy(u16OUT_blocks, u16IN_blocks, n_leds*sizeof(MS_U16));
    #else
    for (block_idx = 0; block_idx < n_leds; block_idx++)
        OUT_blocks[block_idx] = IN_blocks[block_idx];
    #endif
    sf_str[0] = MHal_LD_GetSWSpatialFilterStrength();
    sf_str[1] = MHal_LD_GetSWSpatialFilterStrength2();
    sf_str[2] = MHal_LD_GetSWSpatialFilterStrength3();
    sf_str[3] = MHal_LD_GetSWSpatialFilterStrength4();
    sf_str[4] = MHal_LD_GetSWSpatialFilterStrength5();

    /*
       * modify if (MHal_LD_GetLinearEn()) -> if(0) because:
       * 1. using nolinear style default, now
       * 2. compile warning:warning: the frame size of 21504 bytes is larger than 1536 bytes [-Wframe-larger-than=],
       *     and it's coredump when run:
       *      [   82.639403] WARNING: at kernel/rcutree.c:621 rcu_irq_enter+0x9c/0xa8()
       *      [   82.639422] Modules linked in: mdrv_ldm rtkm(O) mtprealloc(PO) bnep rfcomm hidp bluetooth uhid rc_mstar_tv mali(O)
       *      [   82.639429] CPU: 1 PID: 0 Comm: swapper/1 Tainted: P           O 3.10.40 #163
       *      [   82.639431] Call trace:
       *      [   82.639438] [<ffffffc00008bc58>] (dump_backtrace+0x0/0x148)
       *      [   82.639441] [<ffffffc00008bc58>] dump_backtrace+0x0/0x148
       *      [   82.639445] [<ffffffc00008c698>] (show_stack+0x10/0x1c)
       *      [   82.639448] [<ffffffc00008c698>] show_stack+0x10/0x1c
       *      [   82.639455] [<ffffffc0007ac8a4>] (dump_stack+0x1c/0x28)
       *      [   82.639458] [<ffffffc0007ac8a4>] dump_stack+0x1c/0x28
       *      [   82.639464] [<ffffffc0000a18b4>] (warn_slowpath_common+0x78/0x9c)
       *      [   82.639468] [<ffffffc0000a18b4>] warn_slowpath_common+0x78/0x9c
       *      [   82.639471] [<ffffffc0000a1998>] (warn_slowpath_null+0x14/0x20)
       *      [   82.639475] [<ffffffc0000a1998>] warn_slowpath_null+0x14/0x20
       *      [   82.639478] [<ffffffc00011d39c>] (rcu_irq_enter+0x98/0xa8)
       */
    //if (MHal_LD_GetLinearEn()) // linear style
    if (0) // linear style
    {
        MS_U32 block_sum = 0;
        if (1) // anti-dumbness
        {
            // Make sure (sf_str[0] + 2*sf_str[1] + 2*sf_str[2] + 2*sf_str[3] + 2*sf_str[4] == 256)
            MS_U16 str_left = 256;
            str_left -= sf_str[0];
            if ((str_left >> 1) < sf_str[1])
                sf_str[1] = str_left >> 1;
            str_left -= (sf_str[1] << 1);
            if ((str_left >> 1) < sf_str[2])
                sf_str[2] = str_left >> 1;
            str_left -= (sf_str[2] << 1);
            if ((str_left >> 1) < sf_str[3])
                sf_str[3] = str_left;
            str_left -= (sf_str[3] << 1);
            if ((str_left >> 1) < sf_str[4])
                sf_str[4] = str_left >> 1;
            str_left -= (sf_str[4] << 1);
            sf_str[0] += str_left;
        }

        if (led_type == E_LD_LED_EDGE_TB_TYPE)
        {
            MS_U8 nk = (sf_str[4] > 0) ? 5 : (sf_str[3] > 0) ? 4 : (sf_str[2] > 0) ? 3 : (sf_str[1] > 0) ? 2 : 1;
            for (j = 0; j < 2; j++) // led_height == 2
            {
                for (i = 0; i < gu16LedWidth; i++)
                {
                    block_idx = i + gu16LedWidth*j;
                    block_sum = IN_blocks[block_idx] * sf_str[0];
                    for (k = 1; k < nk; k++)
                    {
                        p = MAX(i - k, 0); // left
                        neighbor_idx = p + gu16LedWidth*j;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];

                        p = MIN(i + k, gu16LedWidth-1); // right
                        neighbor_idx = p + gu16LedWidth*j;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];
                    }
                    OUT_blocks[block_idx] = MAX(IN_blocks[block_idx], (block_sum + 0x80) >> 8);
                }
            }
        }
        else if (led_type == E_LD_LED_EDGE_LR_TYPE)
        {
            MS_U8 nk = (sf_str[4] > 0) ? 5 : (sf_str[3] > 0) ? 4 : (sf_str[2] > 0) ? 3 : (sf_str[1] > 0) ? 2 : 1;
            for (i = 0; i < 2; i++) // led_width == 2
            {
                for (j = 0; j < gu16LedHeight; j++)
                {
                    block_idx = i + gu16LedWidth*j;
                    block_sum = IN_blocks[block_idx] * sf_str[0];
                    for (k = 1; k < nk; k++)
                    {
                        q = MAX(j - k, 0); // up
                        neighbor_idx = i + gu16LedWidth*q;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];

                        q = MIN(j + k, gu16LedHeight-1); // down
                        neighbor_idx = i + gu16LedWidth*q;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];
                    }
                    OUT_blocks[block_idx] = MAX(IN_blocks[block_idx], (block_sum + 0x80) >> 8);
                }
            }
        }
        else// if (led_type == E_LD_LED_DIRECT_TYPE)
        {
            MS_U16 TMP_blocks[LD_MAX_BLOCK];
            MS_U8 nk = (sf_str[4] > 0) ? 5 : (sf_str[3] > 0) ? 4 : (sf_str[2] > 0) ? 3 : (sf_str[1] > 0) ? 2 : 1;

            // horizontal filtering
            for (j = 0; j < gu16LedHeight; j++)
            {
                for (i = 0; i < gu16LedWidth; i++)
                {
                    block_idx = i + gu16LedWidth*j;
                    block_sum = IN_blocks[block_idx] * sf_str[0];
                    for (k = 1; k < nk; k++)
                    {
                        p = MAX(i - k, 0); // left
                        neighbor_idx = p + gu16LedWidth*j;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];

                        p = MIN(i + k, gu16LedWidth-1); // right
                        neighbor_idx = p + gu16LedWidth*j;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];
                    }
                    TMP_blocks[block_idx] = MAX(IN_blocks[block_idx], (block_sum + 0x80) >> 8);
                }
            }

            // vertical filtering
            for (i = 0; i < gu16LedWidth; i++)
            {
                for (j = 0; j < gu16LedHeight; j++)
                {
                    block_idx = i + gu16LedWidth*j;
                    block_sum = TMP_blocks[block_idx] * sf_str[0];
                    for (k = 1; k < nk; k++)
                    {
                        q = MAX(j - k, 0); // up
                        neighbor_idx = i + gu16LedWidth*q;
                        block_sum += (MS_U32)TMP_blocks[neighbor_idx] * sf_str[k];

                        q = MIN(j + k, gu16LedHeight-1); // down
                        neighbor_idx = i + gu16LedWidth*q;
                        block_sum += (MS_U32)TMP_blocks[neighbor_idx] * sf_str[k];
                    }
                    OUT_blocks[block_idx] = MAX(TMP_blocks[block_idx], (block_sum + 0x80) >> 8);
                }
            }
        }
    }
    else // nonlinear style
    {
        // direct type
        const MS_S32 N1_direct = 8; // dist = 1.0
        const MS_S32 xoffset1_direct[8]  = { -1,  0,  1,  0, -1,  1,  1, -1 };
        const MS_S32 yoffset1_direct[8]  = {  0, -1,  0,  1, -1, -1,  1,  1 };
        const MS_S32 N2_direct = 16; // dist = 2.0
        const MS_S32 xoffset2_direct[16] = { -2,  0,  2,  0, -2, -1,  1,  2,  2,  1, -1, -2, -2,  2,  2, -2 };
        const MS_S32 yoffset2_direct[16] = {  0, -2,  0,  2, -1, -2, -2, -1,  1,  2,  2,  1, -2, -2,  2,  2 };
        const MS_S32 N3_direct = 24; // dist = 3.0
        const MS_S32 xoffset3_direct[24] = { -3,  0,  3,  0, -3, -1,  1,  3,  3,  1, -1, -3, -3, -2,  2,  3,  3,  2, -2, -3, -3,  3,  3, -3 };
        const MS_S32 yoffset3_direct[24] = {  0, -3,  0,  3, -1, -3, -3, -1,  1,  3,  3,  1, -2, -3, -3, -2,  2,  3,  3,  2, -3, -3,  3,  3 };
        const MS_S32 N4_direct = 32; // dist = 4.0
        const MS_S32 xoffset4_direct[32]  = { -4,  0,  4,  0, -4, -1,  1,  4,  4,  1, -1, -4, -4, -2,  2,  4,  4,  2, -2, -4, -4, -3,  3,  4,  4,  3, -3, -4, -4,  4,  4, -4 };
        const MS_S32 yoffset4_direct[32]  = {  0, -4,  0,  4, -1, -4, -4, -1,  1,  4,  4,  1, -2, -4, -4, -2,  2,  4,  4,  2, -3, -4, -4, -3,  3,  4,  4,  3, -4, -4,  4,  4 };
        const MS_S32 N5_direct = 40; // dist = 5.0
        const MS_S32 xoffset5_direct[40]  = { -5,  0,  5,  0, -5, -1,  1,  5,  5,  1, -1, -5, -5, -2,  2,  5,  5,  2, -2, -5, -5, -3,  3,  5,  5,  3, -3, -5, -5, -4,  4,  5,  5,  4, -4, -5, -5,  5,  5, -5 };
        const MS_S32 yoffset5_direct[40]  = {  0, -5,  0,  5, -1, -5, -5, -1,  1,  5,  5,  1, -2, -5, -5, -2,  2,  5,  5,  2, -3, -5, -5, -3,  3,  5,  5,  3, -4, -5, -5, -4,  4,  5,  5,  4, -5, -5,  5,  5 };

        // left-right type
        const MS_S32 N1_LR = 2; // dist = 1.0
        const MS_S32 xoffset1_LR[2] = {  0,  0 };
        const MS_S32 yoffset1_LR[2] = {  1, -1 };
        const MS_S32 N2_LR = 2; // dist = 2.0
        const MS_S32 xoffset2_LR[2] = {  0,  0 };
        const MS_S32 yoffset2_LR[2] = {  2, -2 };
        const MS_S32 N3_LR = 2; // dist = 3.0
        const MS_S32 xoffset3_LR[2] = {  0,  0 };
        const MS_S32 yoffset3_LR[2] = {  3, -3 };
        const MS_S32 N4_LR = 2; // dist = 4.0
        const MS_S32 xoffset4_LR[2] = {  0,  0 };
        const MS_S32 yoffset4_LR[2] = {  4, -4 };
        const MS_S32 N5_LR = 2; // dist = 5.0
        const MS_S32 xoffset5_LR[2] = {  0,  0 };
        const MS_S32 yoffset5_LR[2] = {  5, -5 };

        // top-bottom type
        const MS_S32 N1_TB = 2; // dist = 1.0
        const MS_S32 xoffset1_TB[2] = {  1, -1 };
        const MS_S32 yoffset1_TB[2] = {  0,  0 };
        const MS_S32 N2_TB = 2; // dist = 2.0
        const MS_S32 xoffset2_TB[2] = {  2, -2 };
        const MS_S32 yoffset2_TB[2] = {  0,  0 };
        const MS_S32 N3_TB = 2; // dist = 3.0
        const MS_S32 xoffset3_TB[2] = {  3, -3 };
        const MS_S32 yoffset3_TB[2] = {  0,  0 };
        const MS_S32 N4_TB = 2; // dist = 4.0
        const MS_S32 xoffset4_TB[2] = {  4, -4 };
        const MS_S32 yoffset4_TB[2] = {  0,  0 };
        const MS_S32 N5_TB = 2; // dist = 5.0
        const MS_S32 xoffset5_TB[2] = {  5, -5 };
        const MS_S32 yoffset5_TB[2] = {  0,  0 };

        MS_S32 N1 = 0, N2 = 0, N3 = 0, N4 = 0, N5 = 0;
        const MS_S32 *xoffset1 = NULL, *xoffset2 = NULL, *xoffset3 = NULL, *xoffset4 = NULL, *xoffset5 = NULL;
        const MS_S32 *yoffset1 = NULL, *yoffset2 = NULL, *yoffset3 = NULL, *yoffset4 = NULL, *yoffset5 = NULL;
        if (led_type == E_LD_LED_EDGE_TB_TYPE)
        {
           N1 = N1_TB; N2 = N2_TB; N3 = N3_TB;
            xoffset1 = xoffset1_TB; xoffset2 = xoffset2_TB; xoffset3 = xoffset3_TB;
            yoffset1 = yoffset1_TB; yoffset2 = yoffset2_TB; yoffset3 = yoffset3_TB;
            N4 = N4_TB; N5 = N5_TB;
            xoffset4 = xoffset4_TB; xoffset5 = xoffset5_TB;
            yoffset4 = yoffset4_TB; yoffset5 = yoffset5_TB;
        }
        else if (led_type == E_LD_LED_EDGE_LR_TYPE)
        {
            N1 = N1_LR; N2 = N2_LR; N3 = N3_LR;
            xoffset1 = xoffset1_LR; xoffset2 = xoffset2_LR; xoffset3 = xoffset3_LR;
            yoffset1 = yoffset1_LR; yoffset2 = yoffset2_LR; yoffset3 = yoffset3_LR;
            N4 = N4_LR; N5 = N5_LR;
            xoffset4 = xoffset4_LR; xoffset5 = xoffset5_LR;
            yoffset4 = yoffset4_LR; yoffset5 = yoffset5_LR;
        }
        else// if (led_type == E_LD_LED_DIRECT_TYPE)
        {
            N1 = N1_direct; N2 = N2_direct; N3 = N3_direct;
            xoffset1 = xoffset1_direct; xoffset2 = xoffset2_direct; xoffset3 = xoffset3_direct;
            yoffset1 = yoffset1_direct; yoffset2 = yoffset2_direct; yoffset3 = yoffset3_direct;
            N4 = N4_direct; N5 = N5_direct;
            xoffset4 = xoffset4_direct; xoffset5 = xoffset5_direct;
            yoffset4 = yoffset4_direct; yoffset5 = yoffset5_direct;
        }

        for (k = 0; k < 5; k++)
            sf_str[k] <<= 8; // 0 ~ 0xFF00

        for (j = 0; j < gu16LedHeight; j++)
        {
            for (i = 0; i < gu16LedWidth; i++)
            {
                block_idx = i + gu16LedWidth*j;

                // 3x3 non-linear filter
                if (sf_str[0] < 0xFF00)
                {
                    for (k = 0; k < N1; k++)
                    {
                        q = j + yoffset1[k];
                        if (q < 0 || q >= gu16LedHeight) continue;
                        p = i + xoffset1[k];
                        if (p < 0 || p >= gu16LedWidth) continue;

                        neighbor_idx = p + gu16LedWidth*q;
                        if (sf_str[0] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[0];
                    }
                }

                // 5x5 non-linear filter
                if (sf_str[1] < 0xFF00)
                {
                    for (k = 0; k < N2; k++)
                    {
                        q = j + yoffset2[k];
                        if (q < 0 || q >= gu16LedHeight) continue;
                        p = i + xoffset2[k];
                        if (p < 0 || p >= gu16LedWidth) continue;

                        neighbor_idx = p + gu16LedWidth*q;
                        if (sf_str[1] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[1];
                    }
                }

                // 7x7 non-linear filter
                if (sf_str[2] < 0xFF00)
                {
                    for (k = 0; k < N3; k++)
                    {
                        q = j + yoffset3[k];
                        if (q < 0 || q >= gu16LedHeight) continue;
                        p = i + xoffset3[k];
                        if (p < 0 || p >= gu16LedWidth) continue;

                        neighbor_idx = p + gu16LedWidth*q;
                        if (sf_str[2] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[2];
                    }
                }

                // 9x9 non-linear filter
                if (sf_str[3] < 0xFF00)
                {
                    for (k = 0; k < N4; k++)
                    {
                        q = j + yoffset4[k];
                        if (q < 0 || q >= gu16LedHeight) continue;
                        p = i + xoffset4[k];
                        if (p < 0 || p >= gu16LedWidth) continue;

                        neighbor_idx = p + gu16LedWidth*q;
                        if (sf_str[3] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[3];
                    }
                }

                // 11x11 non-linear filter
                if (sf_str[4] < 0xFF00)
                {
                    for (k = 0; k < N5; k++)
                    {
                        q = j + yoffset5[k];
                        if (q < 0 || q >= gu16LedHeight) continue;
                        p = i + xoffset5[k];
                        if (p < 0 || p >= gu16LedWidth) continue;

                        neighbor_idx = p + gu16LedWidth*q;
                        if (sf_str[4] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[4];
                    }
                }
           }
        }
    }
}


static void MDrv_LD_LoadLDFFrameStatistics(MS_U8* LDF_blocks)
{
    MS_U16 ldf_width = MHal_LD_GetLDFBacklightWidth();
    MS_U16 ldf_height = MHal_LD_GetLDFBacklightHeight();
    phys_addr_t addr_base_L = MHal_LD_GetLDFFrameBufBaseAddr_L0();
    MS_U32 addr_offset_L = 0;
    MS_U16 pack_length = 0;
    MS_U16 i = 0, j = 0;
    
    CHECK_POINTER_V(LDF_blocks);

#ifdef LD_SW_PINGPONG_MODE
    MS_BOOL bFrameIdx = MDrv_LD_ReadRegBit(REG_2E04, BIT0);
    phys_addr_t addr_base_R = 0;
    if (MDrv_LD_ReadRegBit(REG_2E05, BIT0) && bFrameIdx)
    {
        addr_base_L = MHal_LD_GetLDFFrameBufBaseAddr_L1();
        addr_base_R = MHal_LD_GetLDFFrameBufBaseAddr_R1();
    }
#else
    if (MHal_LD_GetCurFrameIdx() == 1)
        addr_base_L = MHal_LD_GetLDFFrameBufBaseAddr_L1();
#endif

    if(!addr_base_L)
    {
        LD_ERROR("error!  addr_base_L null \n");
        return;
    }
    
    if (MHal_LD_GetLDFWriteDCMaxOFEn()) // 4 bytes : OF[15:8] OF[7:0] Max DC
    {
        ldf_width *= 4;
    }
    else if (MHal_LD_GetLDFWriteDCMaxEn()) // 2 bytes : Max DC
    {
        ldf_width *= 2;
    }
    //else // 1 bytes : BLEND
    //    ldf_width *= 1;
    
    #if 0
    memcpy(LDF_blocks, (void *)u64Vaddr, addr_offset_L);
    #else
    if (ldf_width > 0)
        pack_length = (((ldf_width - 1) / MHAL_LD_PACKLENGTH) + 1) * MHAL_LD_PACKLENGTH;

    //    1 2 3 4 5 6
    //    7 8 9 a b c
    //    d e f g h i
    //    j k l m n o
    for (j = 0; j < ldf_height; j++)
    {
        addr_offset_L = j * pack_length;
        for (i = 0; i < ldf_width; i++)
        {
            *(LDF_blocks++) = MDrv_LD_MIUReadByte(addr_base_L-ld_addr_base, addr_offset_L++);
        }
    }
    #endif
    //Chip_Flush_Memory();

#ifdef LD_SW_PINGPONG_MODE
    MDrv_LD_WriteRegBit(REG_2E04, !bFrameIdx, BIT0);
#endif
}


static void MDrv_LD_OutputLDBFrameBacklight(const MS_U8* LDB_blocks)
{
    phys_addr_t addr_base_L = MHal_LD_GetLDBFrameBufBaseAddr_L0();
#ifdef LD_SW_PINGPONG_MODE
    MS_BOOL bFrameIdx = MDrv_LD_ReadRegBit(REG_2E80, BIT0);
    phys_addr_t addr_base_R = MHal_LD_GetLDBFrameBufBaseAddr_R0();
    if (MDrv_LD_ReadRegBit(REG_2E81, BIT4) && !bFrameIdx)
    {
        addr_base_L = MHal_LD_GetLDBFrameBufBaseAddr_L1();
        addr_base_R = MHal_LD_GetLDBFrameBufBaseAddr_R1();
    }
#endif

    //MS_U16 led_width = MHal_LD_GetLEDBacklightWidth();
    //MS_U16 led_height = MHal_LD_GetLEDBacklightHeight();
    MS_U16 led_half_width = (gu16LedWidth + 1) >> 1;
    MS_U16 led_half_height = (gu16LedHeight + 1) >> 1;
    MS_U16 block_idx = 0;

    MS_U16 ldb_width = MHal_LD_GetLDBBacklightWidth();
    //MS_U16 ldb_height = MHal_LD_Get_LDBBacklightHeight();

    MS_U32 addr_offset = 0;
    MS_U16 pack_length = 0;
    MS_U16 i = 0, j = 0;

    CHECK_POINTER_V(LDB_blocks);

    if(!addr_base_L)
    {
        LD_ERROR("error!  addr_base_L null \n");
        return;
    }

    if (gu16LedWidth > 0)
        pack_length = (((ldb_width - 1) / MHAL_LD_PACKLENGTH) + 1) * MHAL_LD_PACKLENGTH;

    if (MHal_LD_GetEdge2DEn())
    {
        block_idx = 0;
        addr_offset = 0;

        // top-left quater
        #if 0
        memcpy((void *)u64Vaddr, LDB_blocks, addr_offset_L);
        #else
        for (j = 0; j < led_half_height; j++)
        {
            for (i = 0; i < led_half_width; i++)
            {
                MDrv_LD_MIUWriteByte(addr_base_L-ld_addr_base, addr_offset, LDB_blocks[i + j*gu16LedWidth]);
                addr_offset ++;
                if (block_idx % ldb_width == ldb_width - 1)
                    addr_offset += pack_length - ldb_width;
                block_idx ++;
            }
        }
        #endif
        // top-left quater
        #if 0
        addr_offset += addr_offset_L;
        addr_offset_L = gu16LedWidth*led_half_height;
        memcpy((void *)(u64Vaddr+addr_offset), LDB_blocks, addr_offset_L);
        #else
        for (j = 0; j < led_half_height; j++)
        {
            for (i = led_half_width; i < gu16LedWidth; i++)
            {
                MDrv_LD_MIUWriteByte(addr_base_L-ld_addr_base, addr_offset, LDB_blocks[i + j*gu16LedWidth]);
                addr_offset ++;
                if (block_idx % ldb_width == ldb_width - 1)
                    addr_offset += pack_length - ldb_width;
                block_idx ++;
            }
        }
        #endif
        // bottom-left quater
        #if 0
        addr_offset += addr_offset_L;
        addr_offset_L = led_half_width*gu16LedHeight;
        memcpy((void *)(u64Vaddr+addr_offset), LDB_blocks, addr_offset_L);
        #else
        for (j = led_half_height; j < gu16LedHeight; j++)
        {
            for (i = 0; i < led_half_width; i++)
            {
                MDrv_LD_MIUWriteByte(addr_base_L-ld_addr_base, addr_offset, LDB_blocks[i + j*gu16LedWidth]);
                addr_offset ++;
                if (block_idx % ldb_width == ldb_width - 1)
                    addr_offset += pack_length - ldb_width;
                block_idx ++;
            }
        }
        #endif
        // bottom-left quater
        #if 0
        addr_offset += addr_offset_L;
        addr_offset_L = gu16LedWidth*gu16LedHeight;
        memcpy((void *)(u64Vaddr+addr_offset), LDB_blocks, addr_offset_L);
        #else
        for (j = led_half_height; j < gu16LedHeight; j++)
        {
            for (i = led_half_width; i < gu16LedWidth; i++)
            {
                MDrv_LD_MIUWriteByte(addr_base_L-ld_addr_base, addr_offset, LDB_blocks[i + j*gu16LedWidth]);
                addr_offset ++;
                if (block_idx % ldb_width == ldb_width - 1)
                    addr_offset += pack_length - ldb_width;
                block_idx ++;
            }
        }
        #endif
    }
    else
    {
        #if 0
        memcpy((void *)u64Vaddr, LDB_blocks, addr_offset_L);
        #else
        for (j = 0; j < gu16LedHeight; j++)
        {
            addr_offset = j*pack_length;
            for (i = 0; i < ldb_width; i++)
                MDrv_LD_MIUWriteByte(addr_base_L-ld_addr_base, addr_offset++, *LDB_blocks++);
        }
        #endif
    }
#ifdef LD_SW_PINGPONG_MODE
    MDrv_LD_WriteRegBit(REG_2E80, !bFrameIdx, BIT0);
#endif
}


void MDrv_LD_Main(void)
{
    MS_U8 in_gamma = 0;
    MS_U8 out_gamma = 0;

    in_gamma = MHal_LD_GetSWBacklightInGamma();
    out_gamma = MHal_LD_GetSWBacklightOutGamma();

    if((NULL == pLDF_blocks) || (NULL == pSD_blocks) || (NULL == pTF_blocks) ||
        (NULL == pSF_blocks) || (NULL == pLDB_blocks) || (NULL == pSPI_blocks) ||
        (FALSE == bLdOn))
    {
        LD_ERROR("error!  init failed\n");
        return ;
    }
    

    // SW input
    if (MHal_LD_GetSWReadLDFEn())
        MDrv_LD_LoadLDFFrameStatistics(pLDF_blocks);

    // SW algorithm begin
    if (MHal_LD_GetSWAlgorithmEn())
    {
        gu16LedWidth = MHal_LD_GetLEDBacklightWidth();
        gu16LedHeight = MHal_LD_GetLEDBacklightHeight();

        MDrv_LD_AlgoScalingDown(pLDF_blocks, pSD_blocks);       // LDF -> SD
        MDrv_LD_AlgoBacklightGamma(pSD_blocks, pTbl_LD_Gamma[in_gamma]);
        //return ;
        MDrv_LD_AlgoSpatialFilter(pSD_blocks, pSF_blocks);     // SD -> SF
        MDrv_LD_AlgoTemporalFilter(pSF_blocks, pTF_blocks);      // TF -> TF
        //return ;
        MDrv_LD_AlgoLocalDimmingStrength(pTF_blocks);
        //return ;
        MDrv_LD_AlgoBacklightGamma(pTF_blocks, pTbl_LD_Remap[out_gamma]);
        //return ;
        MDrv_LD_AlgoPostProcess(pTF_blocks, pSPI_blocks, pLDB_blocks);      // process SPI & LDB
        MDrv_LD_AlgoGlobalDimmingStrength(pSPI_blocks);
    }
    // SW algorithm end

    // SW output
    if (MHal_LD_GetSWWriteLDBEn())
        MDrv_LD_OutputLDBFrameBacklight(pLDB_blocks); // digital backlight

   // printk("MBB MDrv_LD_Main take  %lu jiff, %lu ms \n",jiffies, jiffies_to_msecs(jiffies));
}

static int MDrv_LD_RecHandler(void)
{
    MBX_Result enMbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg stMbxCommand;
    MS_U8 u8Ack = 0;

    if(TRUE != bReceived)
        return E_MBX_SUCCESS;
    
    // Waiting for message done
    memset((void*)&stMbxCommand, 0x00, sizeof(MBX_Msg));
    spin_lock_irq(&spinlock_ld);
    enMbxResult = MDrv_MBX_RecvMsg((MS_U64)mbx_fp, E_MBX_CLASS_PM_NOWAIT, &stMbxCommand, 1/*PM_MBX_TIMEOUT*/, MBX_CHECK_NORMAL_MSG,0);
    spin_unlock_irq(&spinlock_ld);

    LD_INFO(" MDrv_LD_RecHandler  rec enMbxResult:%d \n", enMbxResult);  
   //check result
    if (enMbxResult == E_MBX_SUCCESS)
    {
        if ((stMbxCommand.u8Ctrl != 0) && (stMbxCommand.u8Ctrl != 1))
        {
           enMbxResult = E_MBX_ERR_NOT_IMPLEMENTED;
           LD_ERROR("Not Implemented!\n");
        }

        switch (stMbxCommand.u8Index)
        {
           case E_PM_CMDIDX_ACK_51ToARM:
               u8Ack = (MS_U8)stMbxCommand.u8Parameters[0];
               break;
           default:
               break;
        }

    }
    else if (E_MBX_ERR_TIME_OUT == enMbxResult)
    {
       LD_ERROR("Handler receive timeout! nMbxResult:%d \n", enMbxResult);
    }
    else  //(E_MBX_SUCCESS != enMbxResult)
    {
       LD_ERROR("Handler receive error nMbxResult:%d \n", enMbxResult);
    }

    return enMbxResult;
}

static MS_S8 MDrv_LD_SendMsg2RT51(phys_addr_t BaseAddr, MS_U8 u8LEDNum)
{
    MBX_Result enMbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg stMbxCommand;
    MS_BOOL bIs64bits = FALSE;

    //send msg to PM
    memset((void*)&stMbxCommand, 0, sizeof(MBX_Msg));
    stMbxCommand.eRoleID = E_MBX_ROLE_PM;
    stMbxCommand.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    stMbxCommand.u8Ctrl = 0;
    stMbxCommand.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
    stMbxCommand.u8Index = E_PM_CMDIDX_LD_SEND;
    
    bIs64bits = (8 == sizeof(phys_addr_t))?TRUE:FALSE;
    if(TRUE == bIs64bits)
    {
        stMbxCommand.u8ParameterCount = 10;
        stMbxCommand.u8Parameters[0] =  64; //64-bit
        stMbxCommand.u8Parameters[1] =  (MS_U8)(((MS_U64)BaseAddr)>>56);
        stMbxCommand.u8Parameters[2] =  (MS_U8)(((MS_U64)BaseAddr)>>48);
        stMbxCommand.u8Parameters[3] =  (MS_U8)(((MS_U64)BaseAddr)>>40);
        stMbxCommand.u8Parameters[4] =  (MS_U8)(((MS_U64)BaseAddr)>>32);
        stMbxCommand.u8Parameters[5] =  (MS_U8)(((MS_U64)BaseAddr)>>24);
        stMbxCommand.u8Parameters[6] =  (MS_U8)(((MS_U64)BaseAddr)>>16);
        stMbxCommand.u8Parameters[7] =  (MS_U8)(((MS_U64)BaseAddr)>>8);
        stMbxCommand.u8Parameters[8] =  (MS_U8)(((MS_U64)BaseAddr)&0xFF);
        stMbxCommand.u8Parameters[9] =  u8LEDNum; //led numbers
    }
    else
    {
        stMbxCommand.u8ParameterCount = 6;
        stMbxCommand.u8Parameters[0] =  32; //32-bit
        stMbxCommand.u8Parameters[1] =  (MS_U8)(BaseAddr>>24);
        stMbxCommand.u8Parameters[2] =  (MS_U8)(BaseAddr>>16);
        stMbxCommand.u8Parameters[3] =  (MS_U8)(BaseAddr>>8);
        stMbxCommand.u8Parameters[4] =  (MS_U8)(BaseAddr&0xFF);
        stMbxCommand.u8Parameters[5] =  u8LEDNum; //led numbers
    }
    enMbxResult = MDrv_MBX_SendMsg((MS_U64)mbx_fp, &stMbxCommand, 0);
    if(enDbgLevel&E_DEBUG_LEVEL_INFO)
    {
        static MS_U32 i;
        if(0 == ++i%33)
            LD_INFO(" MDrv_LD_SendMsg2RT51  send enMbxResult:%d \n", enMbxResult);  
    }
    
    return enMbxResult;    
}

static MS_S8 MDrv_LD_SetupMbx(void)
{
    MBX_Result enMbxResult = E_MBX_UNKNOW_ERROR;
    //bReceived = FALSE;
    mbx_fp = filp_open("/dev/msmailbox",O_RDWR,0);

    if(IS_ERR(mbx_fp))
    {
        mbx_fp = NULL;
        LD_ERROR("!! OPEN fail  \n");
        return -1;
    }

    enMbxResult = MDrv_MBX_Init((MS_U64)mbx_fp, E_MBX_CPU_MIPS, E_MBX_ROLE_HK, PM_MBX_TIMEOUT);
    if( E_MBX_SUCCESS != enMbxResult)
    {
        LD_ERROR("!! MDrv_MBX_Init fail nMbxResult:%d \n", enMbxResult);  
        return -1;
    }
    else
    {
        extern int MDrv_MBX_NotifyMsgRecCbFunc(void *);
        #define PM_MBX_QUEUESIZE      8

	    MDrv_MBX_Enable((MS_U64)mbx_fp, TRUE);
        MDrv_MBX_NotifyMsgRecCbFunc(MDrv_LD_RecHandler);

        enMbxResult = MDrv_MBX_RegisterMSG((MS_U64)mbx_fp, E_MBX_CLASS_PM_NOWAIT, PM_MBX_QUEUESIZE);
        if( E_MBX_SUCCESS != enMbxResult)
        {
            LD_ERROR("!!MDrv_MBX_RegisterMSG fail nMbxResult:%d \n", enMbxResult);
        }

        return enMbxResult;
    }
}

void MDrv_LD_Suspend(void)
{
    LD_INFO(" entrance \n");
    u8LdSuspend = 1;
    MHal_LD_SetLocalDimmingEn(FALSE);
}

void MDrv_LD_Resume(void)
{
    u8LdSuspend = 0;
    MHal_LD_SetLocalDimmingEn(TRUE);
}

MS_BOOL MDrv_LD_ProcessIRQ(void)
{
    if ((pstWork)&&(TRUE == bLdOn)&&(0 == u8LdSuspend))
    {
        schedule_work(pstWork);
    }

    return TRUE;
}

static void do_ld(struct work_struct *work)
{
	MDrv_LD_Main();
}

void MDrv_LDEnable(MS_BOOL bOn, MS_U8 u8Luma)
{
    MS_S8 ret = 0;
    bLdOn = bOn;
    
    if(FALSE == bOn)
    {
        MS_U16 n_leds = gu16LedWidth * gu16LedHeight;
        
        LD_INFO(" disable, u8Luma:%d \n", u8Luma);
        MHal_LD_SetLocalDimmingEn(FALSE);
        memset(pSPI_blocks,u8Luma,n_leds);
        Chip_Flush_Memory();
        
        bReceived = FALSE;   
        ret = MDrv_LD_SendMsg2RT51((phys_addr_t)(ld_addr_base+SPI_BUFF_OFFSET), n_leds*sizeof(MS_U16));
        if(0 != ret)
        {
            LD_ERROR(" send addr_base to rt51 fail ret:%d\n",ret);
        }
    }
    else
    {
        LD_INFO(" enable \n");
        MHal_LD_SetLocalDimmingEn(TRUE);
    }
}

MS_S8 MDrv_LD_Setup(void)
{
    MS_S32 err = 0;  
    
    if(pLDF_blocks)
    {
        LD_ERROR("error!  re-entrance! \n");
        return -1;
    }

    mutex_lock(&Semutex_LD);

    pSPI_blocks = (MS_U16*)(u64Vaddr+SPI_BUFF_OFFSET); //0x3e70 0000 + 0x80000
    pLDF_blocks = (MS_U8*)(pSPI_blocks+LD_MAX_BLOCK); //SPI + 0X2800
    pSD_blocks = (MS_U16*)(pLDF_blocks+LD_MAX_BLOCK); //LDF_blocks + 0X2800
    pSF_blocks = (MS_U16*)(pSD_blocks+LD_MAX_BLOCK);  //SD_blocks + 0X2800
    pTF_blocks = (MS_U16*)(pSF_blocks+LD_MAX_BLOCK);  //SF_blocks + 0X2800
    pLDB_blocks = (MS_U8*)(pTF_blocks+LD_MAX_BLOCK);  //TF_blocks + 0X2800
    
    err = MDrv_LD_SetupMbx();
    if(0 != err)
    {
        LD_ERROR("error!  MDrv_LD_SetupMbx fail \n");
        mutex_unlock(&Semutex_LD);
        return -1;
    }
    
    pstWork = kmalloc(sizeof(*pstWork), GFP_ATOMIC);
    if (pstWork) 
    {
        INIT_WORK(pstWork, do_ld);
    }

    if(enDbgLevel&E_DEBUG_LEVEL_INFO)
    {
        memset(((void *)u64Vaddr),0x15,32);
        LD_INFO(" test! LD_VA_BASE 0x:%p, contents 0x:%x \n", u64Vaddr,*(MS_U8*)u64Vaddr);
        memset(((void *)(u64Vaddr+0x400/(sizeof(MS_U8)))),0x24,32);
        LD_INFO(" (LD_VA_BASE+0x400) 0x:%p, contents 0x:%x \n",(u64Vaddr+0x400/(sizeof(MS_U8))),*(MS_U8*)(u64Vaddr+0x400/(sizeof(MS_U8))));
        memset(((void *)(u64Vaddr+0x4000/(sizeof(MS_U8)))),0x30,32);
        LD_INFO(" (LD_VA_BASE+0x4000) 0x:%p, contents 0x:%x \n",(u64Vaddr+0x4000/(sizeof(MS_U8))),*(MS_U8*)(u64Vaddr+0x4000/(sizeof(MS_U8))));
        memset(((void *)(u64Vaddr+0x8000/(sizeof(MS_U8)))),0x44,32);
        LD_INFO(" (LD_VA_BASE+0x8000) 0x:%p, contents 0x:%x \n",(u64Vaddr+0x8000/(sizeof(MS_U8))),*(MS_U8*)(u64Vaddr+0x8000/(sizeof(MS_U8))));
        memset(((void *)(u64Vaddr+SPI_BUFF_OFFSET/(sizeof(MS_U8)))),0x55,32);
        LD_INFO(" (LD_VA_BASE+SPI_BUFF_OFFSET) 0x:%p, contents 0x:%x \n",(u64Vaddr+SPI_BUFF_OFFSET/(sizeof(MS_U8))), *(MS_U8*)(u64Vaddr+SPI_BUFF_OFFSET/(sizeof(MS_U8))));
        LD_INFO(" (LD_VA_BASE+SPI_BUFF_OFFSET) 0x:%x, (u64Vaddr+0x4000) 0x:%x \n", MDrv_LD_MIUReadByte(0x80000, 0), MDrv_LD_MIUReadByte(0x4000, 0));
    }
    
    memset(((void *)pSPI_blocks),0x00,LDM_LEN/2);    //follow spi to the end of buffer
    Chip_Flush_Memory();

    mutex_unlock(&Semutex_LD);
    
    LD_INFO("  done \n");
    return 0;
}

void MDrv_LD_SetStrength(MS_U8 u8LDStrength)  
{
    LD_INFO(" u8LDStrength:%d \n", u8LDStrength);
    MHal_LD_SetSWLocalDimmingStrength(u8LDStrength);
}

phys_addr_t MDrv_LD_GetDataAddr(EN_GET_DATA_TYPE type)  
{
    phys_addr_t uAddr = 0;
    
    switch(type)
    {           
        case E_DATA_TYPE_LDF:
        {
            uAddr = MHal_LD_GetLDFFrameBufBaseAddr_L0();
            LD_INFO(" ldf uAddr:0x%x \n", uAddr);
            break ;
        }
        case E_DATA_TYPE_LDB:
        {
            uAddr = MHal_LD_GetLDBFrameBufBaseAddr_0();
            LD_INFO(" ldb uAddr:0x%x \n", uAddr);
            break ;
        }
        case E_DATA_TYPE_SPI:
        {
            uAddr = MHal_LD_GetLDFFrameBufBaseAddr_L0();
            uAddr += SPI_BUFF_OFFSET;
            LD_INFO(" spi uAddr:0x%x \n", uAddr);
            break ;
        }                
        
        default:
            LD_ERROR("Unknown type %d\n", type);
            return 0;
    }
    
    return uAddr;
}

void MDrv_LD_SetDemoPattern(MS_U8 u8Pattern, MS_BOOL bOn)
{    
    LD_INFO(" demo Pattern:%d \n", u8Pattern);
    //bOn to backup
    
    switch(u8Pattern)
    {           
        case E_DEMO_PATTERN_LEFT_RIGHT_HALF:
        {
            LD_INFO(" E_DEMO_PATTERN_LEFT_RIGHT_HALF \n");
            break ;
        }
        case E_DEMO_PATTERN_MARQUEE:
        {
            LD_INFO(" E_DEMO_PATTERN_LEFT_RIGHT_HALF \n");
            break ;
        }
        case E_DEMO_PATTERN_LEFT_RIGHT_COLOR_SHELTER:
        {
            LD_INFO(" E_DEMO_PATTERN_LEFT_RIGHT_COLOR_SHELTER \n");
            break ;
        }    
        
        default:
            LD_ERROR("Unknown pattern %d\n", u8Pattern);
    }
}

void MDrv_LD_CleanupModule(void)  
{    
    spin_lock_irq(&spinlock_ld);
    LD_INFO(" entrance \n");

    MHal_LD_SetLocalDimmingEn(FALSE);
    if(NULL != pstWork)
    {
        kfree(pstWork);
        pstWork = NULL;
    }
    
    if(NULL != pLDF_blocks)
    {
        pLDF_blocks = NULL;
    }
    if(NULL != pSD_blocks)
    {
        pSD_blocks = NULL;
    }
    if(NULL != pTF_blocks)
    {
        pTF_blocks = NULL;
    }
    if(NULL != pSF_blocks)
    {
        pSF_blocks = NULL;
    }
    if(NULL != pLDB_blocks)
    {
        pLDB_blocks = NULL;
    }

    if(NULL != mbx_fp)
    {
        filp_close(mbx_fp, NULL);
        mbx_fp = NULL;
    }

    iounmap(u64Vaddr);
    u64Vaddr = 0;
    spin_unlock_irq(&spinlock_ld);
    bLdOn = FALSE;
    u8LdSuspend = 0;
}  

#endif

