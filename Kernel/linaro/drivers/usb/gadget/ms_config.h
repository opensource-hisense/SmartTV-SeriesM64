//------------------------------------------------------------------------------
//
// Copyright (c) 2008 MStar Semiconductor, Inc.  All rights reserved.
//
//------------------------------------------------------------------------------
// FILE
//      ms_config.h
//
// DESCRIPTION
//
// HISTORY
//
//------------------------------------------------------------------------------
#ifndef _MS_CONFIG_H_
#define _MS_CONFIG_H_

#include <mach/hardware.h>
#include "msb250x_udc_reg.h"

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

//#define MSB250X_BASE_REG_RIU_PA		0xfd200000
//#define usbcRegAddress   			(MSB250X_BASE_REG_RIU_PA+0x0700*2)
//#define otgRegAddress   			(MSB250X_BASE_REG_RIU_PA+0x51000*2)
//#define utmiRegAddress   			(MSB250X_BASE_REG_RIU_PA+0x3A80*2)

#define OffShift        					1
#define otgNumEPDefs    				4
#define SCSI_BLOCK_NUM  			5000
#define SCSI_BLOCK_SIZE 			512
#define EVB_Board
#define MASS_BUFFER_SIZE 			(4 * 1024)
#define MAX_DMA_CHANNEL  			1

#define MASS_BUFFER_SIZE2 			0x10000
#define MASS_TRN_LEN 				8
#define Enable_Read_Write_Test
#define Force_Host_Mode
#define UVC_BULK_MODE

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  /* _MS_CONFIG_H_ */

