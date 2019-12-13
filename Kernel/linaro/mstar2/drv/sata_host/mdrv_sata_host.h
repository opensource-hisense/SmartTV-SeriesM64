////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_sata_host.h
/// @author MStar Semiconductor Inc.
/// @brief  SATA Host Driver
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_SATA_HOST_H_
#define _MDRV_SATA_HOST_H_

#define TYPE_XIU 0
#define TYPE_DRAM 1

//#define SATA_CMD_TYPE TYPE_DRAM
#define SATA_CMD_TYPE TYPE_XIU

//#define USE_NCQ //Only Support at TYPE DRAM Mode

enum {
#if defined(USE_NCQ)
    SATA_KA9_HOST_FLAGS = ATA_FLAG_SATA | ATA_FLAG_PIO_DMA |
                      ATA_FLAG_ACPI_SATA | ATA_FLAG_AN | ATA_FLAG_NCQ,
    SATA_KA9_QUEUE_DEPTH    = 31,
#else
    SATA_KA9_HOST_FLAGS = ATA_FLAG_SATA | ATA_FLAG_PIO_DMA |
                      ATA_FLAG_ACPI_SATA | ATA_FLAG_AN,
    SATA_KA9_QUEUE_DEPTH    = 1,
#endif
    SATA_KA9_USED_PRD    = 24,
    SATA_KA9_MAX_PRD    = 24,
    SATA_KA9_CMD_HDR_SIZE   = 0x20,

    SATA_KA9_CMD_DESC_CFIS_SZ   = 64,
    SATA_KA9_CMD_DESC_ACMD_SZ   = 16,
    SATA_KA9_CMD_DESC_RSRVD     = 48,

    SATA_KA9_CMD_DESC_SIZE  = (SATA_KA9_CMD_DESC_CFIS_SZ +
                 SATA_KA9_CMD_DESC_ACMD_SZ +
                 SATA_KA9_CMD_DESC_RSRVD +
                 SATA_KA9_MAX_PRD * 16),

    SATA_KA9_CMD_DESC_OFFSET_TO_PRDT    =
                (SATA_KA9_CMD_DESC_CFIS_SZ +
                 SATA_KA9_CMD_DESC_ACMD_SZ +
                 SATA_KA9_CMD_DESC_RSRVD),
};

typedef struct sata_cmd_header
{
    u8     cmd_fis_len : 5;
    u8     isATA_PI    : 1;
    u8     iswrite     : 1;
    u8     isprefetch  : 1; // enable only PRDT not zero
    u8     issoftreset : 1;
    u8     isbist      : 1;
    u8     isclearok   : 1;
    u8     reserverd   : 1;
    u8     PMPid       : 4;
    u16    PRDTlength  ;
    u32    PRDBytes    ;
    u32    ctba_lbase  ; // 0~6 is reserved
    u32    ctba_hbase  ;

}hal_cmd_header;


typedef struct sata_cmd_h2dfis
{
    u8 u8fis_type       ;
    u8 u8MPM           : 4;
    u8 reserved_0      : 3;
    u8 isclear         : 1;
    u8 ata_cmd          ;
    u8 fearure          ;
    u8 lba_l            ;
    u8 lba_m            ;
    u8 lba_h            ;
    u8 device           ;
    u8 lba_l_exp        ;
    u8 lba_m_exp        ;
    u8 lba_h_exp        ;
    u8 fearure_exp      ;
    u16 u16sector_cnt   ;
    u8 reserved_1       ;
    u8 control          ;
    u32 reserved_2      ;
}hal_cmd_h2dfis;

struct sata_mstar_port_priv {
    void	*cmd_slot;
    void	*cmd_tbl;
    void	*rx_fis;
    dma_addr_t	cmd_slot_dma;
    dma_addr_t	cmd_tbl_dma;
    dma_addr_t	rx_fis_dma;
};

struct sata_mstar_host_priv {
    u32 hba_base;
    u32 port_base;
    u32 misc_base;
};

#endif
