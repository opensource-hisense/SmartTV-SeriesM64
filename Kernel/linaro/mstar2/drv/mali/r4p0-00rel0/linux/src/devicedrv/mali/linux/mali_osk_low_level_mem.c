/*
 * Copyright (C) 2010-2013 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_osk_low_level_mem.c
 * Implementation of the OS abstraction layer for the kernel device driver
 */

#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/slab.h>

#include "mali_kernel_common.h"
#include "mali_osk.h"
#include "mali_ukk.h"

void _mali_osk_mem_barrier( void )
{
	mb();
}

void _mali_osk_write_mem_barrier( void )
{
	wmb();
}

mali_io_address _mali_osk_mem_mapioregion( u32 phys, u32 size, const char *description )
{
	return (mali_io_address)ioremap_nocache(phys, size);
}

void _mali_osk_mem_unmapioregion( u32 phys, u32 size, mali_io_address virt )
{
	iounmap((void*)virt);
}

_mali_osk_errcode_t inline _mali_osk_mem_reqregion( u32 phys, u32 size, const char *description )
{
#if MALI_LICENSE_IS_GPL
	return _MALI_OSK_ERR_OK; /* GPL driver gets the mem region for the resources registered automatically */
#else
	return ((NULL == request_mem_region(phys, size, description)) ? _MALI_OSK_ERR_NOMEM : _MALI_OSK_ERR_OK);
#endif
}

void inline _mali_osk_mem_unreqregion( u32 phys, u32 size )
{
#if !MALI_LICENSE_IS_GPL
	release_mem_region(phys, size);
#endif
}

void inline _mali_osk_mem_iowrite32_relaxed( volatile mali_io_address addr, u32 offset, u32 val )
{
	__raw_writel(cpu_to_le32(val),((u8*)addr) + offset);
}

u32 inline _mali_osk_mem_ioread32( volatile mali_io_address addr, u32 offset )
{
	return ioread32(((u8*)addr) + offset);
}

void inline _mali_osk_mem_iowrite32( volatile mali_io_address addr, u32 offset, u32 val )
{
	iowrite32(val, ((u8*)addr) + offset);
}

void _mali_osk_cache_flushall( void )
{
	/** @note Cached memory is not currently supported in this implementation */
}

void _mali_osk_cache_ensure_uncached_range_flushed( void *uncached_mapping, u32 offset, u32 size )
{
	_mali_osk_write_mem_barrier();
}

u32 _mali_osk_mem_write_safe(void *dest, const void *src, u32 size)
{
#define MALI_MEM_SAFE_COPY_BLOCK_SIZE 4096
	u32 retval = 0;
	void *temp_buf;

	temp_buf = kmalloc(MALI_MEM_SAFE_COPY_BLOCK_SIZE, GFP_KERNEL);
	if (NULL != temp_buf) {
		u32 bytes_left_to_copy = size;
		u32 i;
		for (i = 0; i < size; i += MALI_MEM_SAFE_COPY_BLOCK_SIZE) {
			u32 size_to_copy;
			u32 size_copied;
			u32 bytes_left;

			if (bytes_left_to_copy > MALI_MEM_SAFE_COPY_BLOCK_SIZE) {
				size_to_copy = MALI_MEM_SAFE_COPY_BLOCK_SIZE;
			} else {
				size_to_copy = bytes_left_to_copy;
			}

			bytes_left = copy_from_user(temp_buf, ((char*)src) + i, size_to_copy);
			size_copied = size_to_copy - bytes_left;

			bytes_left = copy_to_user(((char*)dest) + i, temp_buf, size_copied);
			size_copied -= bytes_left;

			bytes_left_to_copy -= size_copied;
			retval += size_copied;

			if (size_copied != size_to_copy) {
				break; /* Early out, we was not able to copy this entire block */
			}
		}

		kfree(temp_buf);
	}

	return retval;
}

_mali_osk_errcode_t _mali_ukk_mem_write_safe(_mali_uk_mem_write_safe_s *args)
{
	MALI_DEBUG_ASSERT_POINTER(args);

	if (NULL == args->ctx) {
		return _MALI_OSK_ERR_INVALID_ARGS;
	}

	/* Return number of bytes actually copied */
	args->size = _mali_osk_mem_write_safe(args->dest, args->src, args->size);
	return _MALI_OSK_ERR_OK;
}

#ifdef MSTAR_RIU_ENABLED

#if (MSTAR_RIU_ADDRESS_TYPE == 32)
u32 inline _mstar_riu_read(volatile mstar_riu_address base, u32 offset)
{
    return *(volatile u32*)(base + offset);
}

void inline _mstar_riu_write(volatile mstar_riu_address base, u32 offset, u32 value)
{
    *(volatile u32*)(base + offset) = value;
}

#elif (MSTAR_RIU_ADDRESS_TYPE == 16)
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static DEFINE_SPINLOCK(_mstar_riu_lock);
#else
static spinlock_t _mstar_riu_lock = SPIN_LOCK_UNLOCKED;
#endif

u32 inline _mstar_riu_read_no_lock(volatile mstar_riu_address base, u32 offset)
{
	u32 value = 0;

	base += offset;

	value = (u32)(*base);
	rmb();
	value |= (u32)(*(base + 2)) << 16;

	return value;
}

void inline _mstar_riu_write_no_lock(volatile mstar_riu_address base, u32 offset, u32 value)
{
	base += offset;

	*base = (u16)(value & 0xFFFF);
	wmb();
	*(base + 2) = (u16)(value >> 16);
}

u32 inline _mstar_riu_read(volatile mstar_riu_address base, u32 offset)
{
	u32 value = 0;
	unsigned long flags;

	spin_lock_irqsave(&_mstar_riu_lock, flags);
	value = _mstar_riu_read_no_lock(base, offset);
	spin_unlock_irqrestore(&_mstar_riu_lock, flags);

	return value;
}

void inline _mstar_riu_write(volatile mstar_riu_address base, u32 offset, u32 value)
{
	unsigned long flags;

	spin_lock_irqsave(&_mstar_riu_lock, flags);
	_mstar_riu_write_no_lock(base, offset, value);
	spin_unlock_irqrestore(&_mstar_riu_lock, flags);
}

#else
#error "No supported RIU address type defined"
#endif

#endif /* MSTAR_RIU_ENABLED */
