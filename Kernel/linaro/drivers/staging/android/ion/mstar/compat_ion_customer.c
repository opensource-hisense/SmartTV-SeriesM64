////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
/// @file   compat_ion_customer.h
/// @brief  compat ion customer command driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/compat.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/err.h>
#include "ion_customer.h"
#include "../../uapi/ion_customer.h"
#include "../ion.h"
#include "../ion_priv.h"
#include "mdrv_types.h"
#include "mdrv_system.h"

#include "compat_ion_customer.h"

struct compat_mtlb_hardware_info{
    compat_int_t MIU_Has_TLB;  //MIU has TLB for some ip internally
    compat_int_t GE_Has_TLB;   //GE has dedicated TLB
    compat_int_t GOP_Has_TLB;  //GOP has dedicated TLB

    compat_uint_t TLB_GOP_IDS; //which GOP can access memory by TLB
    compat_uint_t TLB_Support_Clients; //tlb support clients except GE&GOP

    compat_int_t GE_DEDICAT_TLB_Switchoff_Support; //ge its dedicated TLB support on-off switch
    compat_int_t GOP_DEDICAT_TLB_Switchoff_Support; //gop its dedicated TLB support on-off switch

    compat_int_t GE_Need_CQFlush; //when ge its dedicated TLB on-off switch, whether it need flush command queue

    compat_int_t TLB_MAPTO_MIU; //which miu has kernel memory mtlb space may be map to

    compat_ulong_t tlb_table_addr_miu0;  //the offset of tlb  table in MIU0
    compat_ulong_t tlb_table_addr_miu1;  //the offset of tlb  table in MIU1
    compat_ulong_t tlb_table_addr_miu2;  //the offset of tlb  table in MIU2
} ;

struct compat_ion_cust_alloc_data{
    //input parameters
    compat_int_t handle;
    compat_int_t client;
    compat_int_t addrType;

    //output parameters
    compat_ulong_t mapaddr;
    unsigned char miu;
    compat_ulong_t miu_offset;

    unsigned char tlb_table_change;
} ;



#define COMPAT_MTLB_CUST_IOC_GET_HWINO             _IOR(MTLB_CUST_IOC_MAGIC, 0x00, struct compat_mtlb_hardware_info)
#define COMPAT_MTLB_CUST_IOC_GET_TABLESIZE         _IOR(MTLB_CUST_IOC_MAGIC, 0x02, compat_ulong_t)
#define COMPAT_MTLB_CUST_IOC_LOCK                  _IOWR(MTLB_CUST_IOC_MAGIC, 0x05, struct compat_ion_cust_alloc_data)
#define COMPAT_MTLB_CUST_IOC_UNLOCK                _IOWR(MTLB_CUST_IOC_MAGIC, 0x06, struct compat_ion_cust_alloc_data)

static int compat_get_mtlb_hardware_info(
    struct compat_mtlb_hardware_info __user *data32,
			mtlb_hardware_info __user *data)
{
	compat_ulong_t ul;
    compat_uint_t ui;
	compat_int_t i;
	int err;

    err = get_user(i,&data32->MIU_Has_TLB);
    err |= put_user(i,&data->MIU_Has_TLB);
    err |= get_user(i,&data32->GE_Has_TLB);
    err |= put_user(i,&data->GE_Has_TLB);
    err |= get_user(i,&data32->GOP_Has_TLB);
    err |= put_user(i,&data->GOP_Has_TLB);
    err |= get_user(ui,&data32->TLB_GOP_IDS);
    err |= put_user(ui,&data->TLB_GOP_IDS);
    err |= get_user(ui,&data32->TLB_Support_Clients);
    err |= put_user(ui,&data->TLB_Support_Clients);
    err |= get_user(i,&data32->GE_DEDICAT_TLB_Switchoff_Support);
    err |= put_user(i,&data->GE_DEDICAT_TLB_Switchoff_Support);
    err |= get_user(i,&data32->GOP_DEDICAT_TLB_Switchoff_Support);
    err |= put_user(i,&data->GOP_DEDICAT_TLB_Switchoff_Support);
    err |= get_user(i,&data32->GE_Need_CQFlush);
    err |= put_user(i,&data->GE_Need_CQFlush);
    err |= get_user(i,&data32->TLB_MAPTO_MIU);
    err |= put_user(i,&data->TLB_MAPTO_MIU);
    err |= get_user(ul,&data32->tlb_table_addr_miu0);
    err |= put_user(ul,&data->tlb_table_addr_miu0);
    err |= get_user(ul,&data32->tlb_table_addr_miu1);
    err |= put_user(ul,&data->tlb_table_addr_miu1);
    err |= get_user(ul,&data32->tlb_table_addr_miu2);
    err |= put_user(ul,&data->tlb_table_addr_miu2);
    return err;
}
static int compat_put_mtlb_hardware_info(
    struct compat_mtlb_hardware_info __user *data32,
			mtlb_hardware_info __user *data)
{
	compat_ulong_t ul;
    compat_uint_t ui;
	compat_int_t i;
	int err;

    err = get_user(i,&data->MIU_Has_TLB);
    err |= put_user(i,&data32->MIU_Has_TLB);
    err |= get_user(i,&data->GE_Has_TLB);
    err |= put_user(i,&data32->GE_Has_TLB);
    err |= get_user(i,&data->GOP_Has_TLB);
    err |= put_user(i,&data32->GOP_Has_TLB);
    err |= get_user(ui,&data->TLB_GOP_IDS);
    err |= put_user(ui,&data32->TLB_GOP_IDS);
    err |= get_user(ui,&data->TLB_Support_Clients);
    err |= put_user(ui,&data32->TLB_Support_Clients);
    err |= get_user(i,&data->GE_DEDICAT_TLB_Switchoff_Support);
    err |= put_user(i,&data32->GE_DEDICAT_TLB_Switchoff_Support);
    err |= get_user(i,&data->GOP_DEDICAT_TLB_Switchoff_Support);
    err |= put_user(i,&data32->GOP_DEDICAT_TLB_Switchoff_Support);
    err |= get_user(i,&data->GE_Need_CQFlush);
    err |= put_user(i,&data32->GE_Need_CQFlush);
    err |= get_user(i,&data->TLB_MAPTO_MIU);
    err |= put_user(i,&data32->TLB_MAPTO_MIU);
    err |= get_user(ul,&data->tlb_table_addr_miu0);
    err |= put_user(ul,&data32->tlb_table_addr_miu0);
    err |= get_user(ul,&data->tlb_table_addr_miu1);
    err |= put_user(ul,&data32->tlb_table_addr_miu1);
    err |= get_user(ul,&data->tlb_table_addr_miu2);
    err |= put_user(ul,&data32->tlb_table_addr_miu2);
    return err;
}
static int compat_put_table_size(
    compat_ulong_t __user *data32,
			unsigned long __user *data)
{
    compat_ulong_t ul;
	int err;
    err = get_user(ul,data);
    err |= put_user(ul,data32);
    return err;
}

static int compat_get_ion_cust_alloc_data(
    struct compat_ion_cust_alloc_data __user *data32,
			ion_cust_alloc_data __user *data)
{
    compat_ulong_t ul;
    compat_int_t i;
    unsigned char uc;
	int err;

    err = get_user(i,&data32->handle);
    err |= put_user(i,&data->handle);
    err = get_user(i,&data32->client);
    err |= put_user(i,&data->client);
    err = get_user(i,&data32->addrType);
    err |= put_user(i,&data->addrType);
    err = get_user(ul,&data32->mapaddr);
    err |= put_user(ul,&data->mapaddr);
    err = get_user(uc,&data32->miu);
    err |= put_user(uc,&data->miu);
    err = get_user(ul,&data32->miu_offset);
    err |= put_user(ul,&data->miu_offset);
    err = get_user(uc,&data32->tlb_table_change);
    err |= put_user(uc,&data->tlb_table_change);
    return err;
}

static int compat_put_ion_cust_alloc_data(
    struct compat_ion_cust_alloc_data __user *data32,
			ion_cust_alloc_data __user *data)
{
    compat_ulong_t ul;
    compat_int_t i;
    unsigned char uc;
	int err;

    err = get_user(i,&data->handle);
    err |= put_user(i,&data32->handle);
    err = get_user(i,&data->client);
    err |= put_user(i,&data32->client);
    err = get_user(i,&data->addrType);
    err |= put_user(i,&data32->addrType);
    err = get_user(ul,&data->mapaddr);
    err |= put_user(ul,&data32->mapaddr);
    err = get_user(uc,&data->miu);
    err |= put_user(uc,&data32->miu);
    err = get_user(ul,&data->miu_offset);
    err |= put_user(ul,&data32->miu_offset);
    err = get_user(uc,&data->tlb_table_change);
    err |= put_user(uc,&data32->tlb_table_change);
    return err;
}


long compat_mstar_custom_ioctl (struct ion_client *client, unsigned int cmd, unsigned long arg)
{
    long ret;
	if (!is_compat_task()) {
		return mstar_custom_ioctl(client, cmd, arg);
	}
    switch(cmd){
    case COMPAT_MTLB_CUST_IOC_GET_HWINO:
    {
        struct compat_mtlb_hardware_info __user *data32;
		mtlb_hardware_info __user *data;
		int err;

		data32 = compat_ptr(arg);
		data = compat_alloc_user_space(sizeof(*data));
		if (data == NULL)
			return -EFAULT;

		ret = mstar_custom_ioctl(client, MTLB_CUST_IOC_GET_HWINO,
							(unsigned long)data);
		err = compat_put_mtlb_hardware_info(data32, data);
		return ret ? ret : err;
    }
    case COMPAT_MTLB_CUST_IOC_GET_TABLESIZE:
    {
        compat_ulong_t __user *data32;
		unsigned long __user *data;
		int err;

		data32 = compat_ptr(arg);
		data = compat_alloc_user_space(sizeof(*data));
		if (data == NULL)
			return -EFAULT;

		ret = mstar_custom_ioctl(client, MTLB_CUST_IOC_GET_TABLESIZE,
							(unsigned long)data);
		err = compat_put_table_size(data32, data);
		return ret ? ret : err;
    }
    case COMPAT_MTLB_CUST_IOC_LOCK:
    case COMPAT_MTLB_CUST_IOC_UNLOCK:
    {
        struct compat_ion_cust_alloc_data __user *data32;
		ion_cust_alloc_data __user *data;
		int err;

		data32 = compat_ptr(arg);
		data = compat_alloc_user_space(sizeof(*data));
		if (data == NULL)
			return -EFAULT;

		err = compat_get_ion_cust_alloc_data(data32, data);
		if (err)
			return err;
		ret = mstar_custom_ioctl(client, (cmd==COMPAT_MTLB_CUST_IOC_LOCK)?(MTLB_CUST_IOC_LOCK):(MTLB_CUST_IOC_UNLOCK),
							(unsigned long)data);
		err = compat_put_ion_cust_alloc_data(data32, data);
		return ret ? ret : err;
    }
    case MTLB_CUST_IOC_GET_HWSTATUS:
    case MTLB_CUST_IOC_INIT:
    case MTLB_CUST_IOC_ENABLE:
    case MTLB_CUST_IOC_IMPORT:
        return mstar_custom_ioctl(client, cmd, arg);
    default:
		return -ENOIOCTLCMD;
    }
}

