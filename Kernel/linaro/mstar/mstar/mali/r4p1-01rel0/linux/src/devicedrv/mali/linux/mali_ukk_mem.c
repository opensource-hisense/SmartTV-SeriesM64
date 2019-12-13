/*
 * Copyright (C) 2010-2014 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <linux/fs.h>       /* file system operations */
#include <asm/uaccess.h>    /* user space access */

#include "mali_ukk.h"
#include "mali_osk.h"
#include "mali_kernel_common.h"
#include "mali_session.h"
#include "mali_ukk_wrappers.h"

/* MSTAR patch begin */
#ifdef MSTAR
#include "mstar/mstar_chip.h"
#define PHYS_TO_BUS_ADDRESS_ADJUST  MSTAR_MIU0_BUS_BASE
#define MSTAR_MIU1_PHY_BASE         (MSTAR_MIU1_BUS_BASE - MSTAR_MIU0_BUS_BASE)
#endif
/* MSTAR patch end */

int mem_write_safe_wrapper(struct mali_session_data *session_data, _mali_uk_mem_write_safe_s __user *uargs)
{
	_mali_uk_mem_write_safe_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_mem_write_safe_s))) {
		return -EFAULT;
	}

	kargs.ctx = (uintptr_t)session_data;

	/* Check if we can access the buffers */
	if (!access_ok(VERIFY_WRITE, kargs.dest, kargs.size)
	    || !access_ok(VERIFY_READ, kargs.src, kargs.size)) {
		return -EINVAL;
	}

	/* Check if size wraps */
	if ((kargs.size + kargs.dest) <= kargs.dest
	    || (kargs.size + kargs.src) <= kargs.src) {
		return -EINVAL;
	}

	err = _mali_ukk_mem_write_safe(&kargs);
	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	if (0 != put_user(kargs.size, &uargs->size)) {
		return -EFAULT;
	}

	return 0;
}

int mem_map_ext_wrapper(struct mali_session_data *session_data, _mali_uk_map_external_mem_s __user *argument)
{
	_mali_uk_map_external_mem_s uk_args;
	_mali_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	MALI_CHECK_NON_NULL(argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if (0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_mali_uk_map_external_mem_s))) {
		return -EFAULT;
	}

	uk_args.ctx = (uintptr_t)session_data;
	err_code = _mali_ukk_map_external_mem(&uk_args);

	if (0 != put_user(uk_args.cookie, &argument->cookie)) {
		if (_MALI_OSK_ERR_OK == err_code) {
			/* Rollback */
			_mali_uk_unmap_external_mem_s uk_args_unmap;

			uk_args_unmap.ctx = (uintptr_t)session_data;
			uk_args_unmap.cookie = uk_args.cookie;
			err_code = _mali_ukk_unmap_external_mem(&uk_args_unmap);
			if (_MALI_OSK_ERR_OK != err_code) {
				MALI_DEBUG_PRINT(4, ("reverting _mali_ukk_unmap_external_mem, as a result of failing put_user(), failed\n"));
			}
		}
		return -EFAULT;
	}

	/* Return the error that _mali_ukk_free_big_block produced */
	return map_errcode(err_code);
}

int mem_unmap_ext_wrapper(struct mali_session_data *session_data, _mali_uk_unmap_external_mem_s __user *argument)
{
	_mali_uk_unmap_external_mem_s uk_args;
	_mali_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	MALI_CHECK_NON_NULL(argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if (0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_mali_uk_unmap_external_mem_s))) {
		return -EFAULT;
	}

	uk_args.ctx = (uintptr_t)session_data;
	err_code = _mali_ukk_unmap_external_mem(&uk_args);

	/* Return the error that _mali_ukk_free_big_block produced */
	return map_errcode(err_code);
}

#if defined(CONFIG_MALI400_UMP)
int mem_release_ump_wrapper(struct mali_session_data *session_data, _mali_uk_release_ump_mem_s __user *argument)
{
	_mali_uk_release_ump_mem_s uk_args;
	_mali_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	MALI_CHECK_NON_NULL(argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if (0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_mali_uk_release_ump_mem_s))) {
		return -EFAULT;
	}

	uk_args.ctx = (uintptr_t)session_data;
	err_code = _mali_ukk_release_ump_mem(&uk_args);

	/* Return the error that _mali_ukk_free_big_block produced */
	return map_errcode(err_code);
}

int mem_attach_ump_wrapper(struct mali_session_data *session_data, _mali_uk_attach_ump_mem_s __user *argument)
{
	_mali_uk_attach_ump_mem_s uk_args;
	_mali_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	MALI_CHECK_NON_NULL(argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if (0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_mali_uk_attach_ump_mem_s))) {
		return -EFAULT;
	}

	uk_args.ctx = (uintptr_t)session_data;
	err_code = _mali_ukk_attach_ump_mem(&uk_args);

	if (0 != put_user(uk_args.cookie, &argument->cookie)) {
		if (_MALI_OSK_ERR_OK == err_code) {
			/* Rollback */
			_mali_uk_release_ump_mem_s uk_args_unmap;

			uk_args_unmap.ctx = (uintptr_t)session_data;
			uk_args_unmap.cookie = uk_args.cookie;
			err_code = _mali_ukk_release_ump_mem(&uk_args_unmap);
			if (_MALI_OSK_ERR_OK != err_code) {
				MALI_DEBUG_PRINT(4, ("reverting _mali_ukk_attach_mem, as a result of failing put_user(), failed\n"));
			}
		}
		return -EFAULT;
	}

	/* Return the error that _mali_ukk_map_external_ump_mem produced */
	return map_errcode(err_code);
}
#endif /* CONFIG_MALI400_UMP */

int mem_query_mmu_page_table_dump_size_wrapper(struct mali_session_data *session_data, _mali_uk_query_mmu_page_table_dump_size_s __user *uargs)
{
	_mali_uk_query_mmu_page_table_dump_size_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_query_mmu_page_table_dump_size(&kargs);
	if (_MALI_OSK_ERR_OK != err) return map_errcode(err);

	if (0 != put_user(kargs.size, &uargs->size)) return -EFAULT;

	return 0;
}

int mem_dump_mmu_page_table_wrapper(struct mali_session_data *session_data, _mali_uk_dump_mmu_page_table_s __user *uargs)
{
	_mali_uk_dump_mmu_page_table_s kargs;
	_mali_osk_errcode_t err;
	void __user *user_buffer;
	void *buffer = NULL;
	int rc = -EFAULT;

	/* validate input */
	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	/* the session_data pointer was validated by caller */

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_dump_mmu_page_table_s)))
		goto err_exit;

	user_buffer = (void __user *)(uintptr_t)kargs.buffer;
	if (!access_ok(VERIFY_WRITE, user_buffer, kargs.size))
		goto err_exit;

	/* allocate temporary buffer (kernel side) to store mmu page table info */
	if (kargs.size <= 0)
		return -EINVAL;
	/* Allow at most 8MiB buffers, this is more than enough to dump a fully
	 * populated page table. */
	if (kargs.size > SZ_8M)
		return -EINVAL;

	buffer = (void *)(uintptr_t)_mali_osk_valloc(kargs.size);
	if (NULL == buffer) {
		rc = -ENOMEM;
		goto err_exit;
	}

	kargs.ctx = (uintptr_t)session_data;
	kargs.buffer = (uintptr_t)buffer;
	err = _mali_ukk_dump_mmu_page_table(&kargs);
	if (_MALI_OSK_ERR_OK != err) {
		rc = map_errcode(err);
		goto err_exit;
	}

	/* copy mmu page table info back to user space and update pointers */
	if (0 != copy_to_user(user_buffer, buffer, kargs.size))
		goto err_exit;

	kargs.register_writes = kargs.register_writes -
				(uintptr_t)buffer + (uintptr_t)user_buffer;
	kargs.page_table_dump = kargs.page_table_dump -
				(uintptr_t)buffer + (uintptr_t)user_buffer;

	if (0 != copy_to_user(uargs, &kargs, sizeof(kargs)))
		goto err_exit;

	rc = 0;

err_exit:
	if (buffer) _mali_osk_vfree(buffer);
	return rc;
}

/* MSTAR patch begin */
#ifdef MSTAR
int mem_get_cpu_usage_adjust_wrapper(struct mali_session_data *session_data, _mali_uk_get_cpu_usage_adjust __user *uargs)
{
	_mali_uk_get_cpu_usage_adjust kargs;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	kargs.ctx = (uintptr_t)session_data;
	kargs.value = PHYS_TO_BUS_ADDRESS_ADJUST;

	if (0 != put_user(kargs.value, &uargs->value))
	{
		return -EFAULT;
	}

	return 0;
}

int mem_get_miu1_base_wrapper(struct mali_session_data *session_data, _mali_uk_get_miu1_base __user *uargs)
{
	_mali_uk_get_miu1_base kargs;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	kargs.ctx = (uintptr_t)session_data;
	kargs.addr = MSTAR_MIU1_PHY_BASE;

	if (0 != put_user(kargs.addr, &uargs->addr))
	{
		return -EFAULT;
	}

	return 0;
}
#endif

#ifdef MSTAR_DUMP_MEMORY_USAGE
int mem_set_dump_notification_wrapper(_mali_uk_memory_usage_dumped_s __user *arg)
{
	struct mali_session_data *session, *tmp;
	_mali_osk_notification_t * notification;

	mali_session_lock();
	MALI_SESSION_FOREACH(session, tmp, link)
	{
		notification = _mali_osk_notification_create(_MALI_NOTIFCATION_MEMORY_USAGE_DUMPED, sizeof(_mali_uk_memory_usage_dumped_s));

		if (NULL == notification)
		{
			MALI_PRINT_ERROR( ("Failed to create notification object\n"));
			mali_session_unlock();
			return -ENOMEM;
		}

		if (0 != copy_from_user(notification->result_buffer, arg, sizeof(_mali_uk_memory_usage_dumped_s)))
		{
			_mali_osk_notification_delete(notification);
			mali_session_unlock();
			return -EFAULT;
		}

		mali_session_send_notification(session, notification);

		MALI_DEBUG_PRINT(4, ("Mali memory usage dump notification is sent (pid: %d)\n", ((_mali_uk_memory_usage_dumped_s*)notification->result_buffer)->pid));
	}

	mali_session_unlock();

	return 0;
}
#endif
#if MSTAR_STATISTIC_MALI_MIU_USAGE
char *mali_paged_allocated_miu(void)
{
	struct mali_session_data *session, *tmp;
	char *buffer, *ptr,tmp_str[96];
	int i, buffer_len, buffer_size = 0;

	buffer = NULL;

	mali_session_lock();
	MALI_SESSION_FOREACH(session, tmp, link)
	{
		ptr = buffer;
		buffer_size += 96;
		buffer = _mali_osk_malloc(buffer_size);

		if (buffer != NULL)
		{
			memset(buffer, 0 , buffer_size);
			if (ptr != NULL)
			{
				buffer_len = strlen(ptr);
				memcpy(buffer,ptr,buffer_len);
			}

			for (i = 0 ; i < MSTAR_MIU_NUM ;i++)
			{
				sprintf(tmp_str, "%10u ", session->num_pages_allocated_miu[i] * _MALI_OSK_MALI_PAGE_SIZE);
				strcat(buffer, tmp_str);
			}

			sprintf(tmp_str, "(%u)\n", session->pid);
			strcat(buffer,tmp_str);
		}
		else
		{
			MALI_PRINT_ERROR(("_mali_osk_malloc failed\n"));

			if (ptr != NULL) _mali_osk_free(ptr);

			mali_session_unlock();
			return buffer;
		}

		if (ptr != NULL) _mali_osk_free(ptr);
	}
	mali_session_unlock();

	return buffer;
}
#endif

/* MSTAR patch end */