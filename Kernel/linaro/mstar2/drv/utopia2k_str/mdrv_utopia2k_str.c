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
/// @file   mdrv_utopia2k_str.c
/// @brief  Utopia2K STR interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <linux/timer.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/autoconf.h>
#include <linux/async.h>
#include <linux/slab.h>
#include <linux/sched.h>

#include "mdrv_utopia2k_str_io.h"

#define UTOPIA2K_STR_SEFL_TEST 1

#define UTOPIA2K_STR_NAME "Mstar-utopia2k-str"

#define utopia2k_str_dbg(fmt, ...)              \
do {                                    \
    if (utopia2k_str_debug_enabled) \
        pr_info(fmt, ##__VA_ARGS__);                \
} while (0)

static ASYNC_DOMAIN_EXCLUSIVE(condition_domain);
static spinlock_t lock;

struct utopia2k_str_module {
    struct list_head list;
    struct task_struct *p;
    struct timer_list timer;
    FUtopiaSTR fpSTR;
    int mode;
    void *data;
};

struct dts_post_condition {
    struct list_head list;
    char name[];
};

struct list_head utopia2k_str_head;

struct list_head dts_post_condition_head;

#ifdef CONFIG_OF
static const char *power_mode[] = {
    "",
    "suspend",
    "resume"
};
#endif

bool utopia2k_str_debug_enabled = 0;

static int __init utopia2k_str_debug_enable(char *str)
{
    utopia2k_str_debug_enabled = 1;
    return 1;
}
__setup("utopia2k_str_debug", utopia2k_str_debug_enable);

static void utopia2k_str_module_timeout(unsigned long data)
{
    struct utopia2k_str_module *um = (struct utopia2k_str_module *)data;
    show_stack(um->p, NULL);
}

static ktime_t utopia2k_str_time_debug_start(struct utopia2k_str_module *module)
{
    ktime_t calltime = ktime_set(0, 0);

    utopia2k_str_dbg("[module %i] calling [%pf].\n", task_pid_nr(current), module->fpSTR);
    calltime = ktime_get();

    return calltime;
}

static void utopia2k_str_time_debug_report(ktime_t calltime, struct utopia2k_str_module *module)
{
    ktime_t delta, rettime;

    rettime = ktime_get();
    delta = ktime_sub(rettime, calltime);
    utopia2k_str_dbg("[module %i] call [%pf] return after [%Ld] usecs.\n",
            task_pid_nr(current), module->fpSTR,
            (unsigned long long)ktime_to_ns(delta) >> 10);
}

int mdrv_utopia2k_str_setup_function_ptr(void* pModuleTmp, FUtopiaSTR fpSTR)
{
    struct utopia2k_str_module *um;
    um = kmalloc(sizeof(struct utopia2k_str_module), GFP_KERNEL);
    if (!um) {
        return -ENOMEM;
    }
    INIT_LIST_HEAD(&um->list);
    list_add(&um->list, &utopia2k_str_head);
    um->fpSTR = fpSTR;
    um->data = pModuleTmp;
    um->p = NULL;
    return 0;
}
EXPORT_SYMBOL(mdrv_utopia2k_str_setup_function_ptr);

int mdrv_utopia2k_str_wait_condition(const char* name, MS_U32 mode, MS_U32 stage)
{
    int ret = 0;
#ifdef CONFIG_OF
    char node_path[128] = {0};
    struct device_node *stage_node;
    struct property *prop;

    int i = 0;
    size_t l = 0, total = 0;
    const char *p;

    struct dts_post_condition *pc;
    int cnt = 0;

    int prop_cnt;

    if (mode <= 0 || mode >= UTOPIA2K_STR_POWER_MAX) {
        printk("error!! no such mode [%d].\n", mode);
        return -1;
    }

    sprintf(node_path, "/%s/%s/%s/stage%d", UTOPIA2K_STR_NAME, name, power_mode[mode], stage);

    utopia_dbg("module %i in wait %s start...\n", task_pid_nr(current), node_path);

    stage_node = of_find_node_by_path(node_path);
    if (!stage_node) {
        return -1;
    }

    prop = of_find_property(stage_node, "pre-condition", NULL);

    prop_cnt = of_property_count_strings(stage_node, "pre-condition");

    if (prop_cnt <= 0) {
        ret = -1;
        goto wait_out_np;
    }

    p = prop->value;

    while(true) {
        for (i = 0, p = prop->value, total = 0; total < prop->length; total += l, p += l, i++) {
            l = strlen(p) + 1;
            // for the following case:
            // pre-condition = "xc_suspend_stage0", "", "oo_suspend_stage0";
            if (1 == l) {
                cnt++;
                continue;
            }
            spin_lock(&lock);
            list_for_each_entry(pc, &dts_post_condition_head, list) {
                if (pc && pc->name) {
                    if (!strncmp(pc->name, p, l)) {
                        cnt++;
                        break;
                    }
                }
            }
            spin_unlock(&lock);
        }
        if (cnt == prop_cnt) {
            break;
        }
        cnt = 0;
        schedule_timeout_interruptible(HZ / 1000);
    }

wait_out_np:

    of_node_put(stage_node);

    utopia_dbg("module %i in wait %s end.\n", task_pid_nr(current), node_path);
#endif
    return ret;
}
EXPORT_SYMBOL(mdrv_utopia2k_str_wait_condition);

int mdrv_utopia2k_str_send_condition(const char* name, MS_U32 mode, MS_U32 stage)
{
    int ret = 0;
#ifdef CONFIG_OF
    char node_path[128] = {0};
    struct device_node *stage_node;
    struct property *prop;

    int i = 0;
    size_t l = 0, total = 0;
    const char *p;

    struct dts_post_condition *pc;

    if (mode <= 0 || mode >= UTOPIA2K_STR_POWER_MAX) {
        printk("error!! no such mode [%d].\n", mode);
        return -1;
    }

    sprintf(node_path, "/%s/%s/%s/stage%d", UTOPIA2K_STR_NAME, name, power_mode[mode], stage);

    utopia_dbg("module %i in send %s start...\n", task_pid_nr(current), node_path);
    
    stage_node = of_find_node_by_path(node_path);
    if (!stage_node) {
        return -1;
    }

    prop = of_find_property(stage_node, "post-condition", NULL);

    if (!prop)
        goto send_out_np;
    if (!prop->value)
        goto send_out_np;
    if (strnlen(prop->value, prop->length) >= prop->length)
        goto send_out_np;

    p = prop->value;
    
    for (i = 0; total < prop->length; total += l, p += l, i++) {
        l = strlen(p) + 1;
        // for the following case:
        // post-condition = "xc_suspend_stage0", "", "oo_suspend_stage0";
        if (1 == l) {
            continue;
        }
        pc = kmalloc(sizeof(struct dts_post_condition) + l, GFP_KERNEL);
        if (!pc) {
            ret = -ENOMEM;
            break;
        }
        strncpy(pc->name, p, l);
        INIT_LIST_HEAD(&pc->list);
        spin_lock(&lock);
        list_add(&pc->list, &dts_post_condition_head);
        spin_unlock(&lock);
    }

send_out_np:

    of_node_put(stage_node);

    utopia_dbg("module %i in send %s end.\n", task_pid_nr(current), node_path);
#endif
    return ret;
}
EXPORT_SYMBOL(mdrv_utopia2k_str_send_condition);


#if UTOPIA2K_STR_SEFL_TEST
int vdecStr(int u32PowerState, void* pModule)
{
    mdrv_utopia2k_str_wait_condition("vdec", u32PowerState, 0);
    schedule_timeout_interruptible(HZ);
    mdrv_utopia2k_str_send_condition("vdec", u32PowerState, 0);
    return 0;
}

int xcStr(int u32PowerState, void* pModule)
{
    mdrv_utopia2k_str_wait_condition("xc", u32PowerState, 0);
    schedule_timeout_interruptible(HZ/2);
    mdrv_utopia2k_str_send_condition("xc", u32PowerState, 0);
    return 0;
}

int ooStr(int u32PowerState, void* pModule)
{
    mdrv_utopia2k_str_wait_condition("oo", u32PowerState, 0);
    schedule_timeout_interruptible(HZ);
    mdrv_utopia2k_str_send_condition("oo", u32PowerState, 0);
    return 0;
}
#endif

static void mdrv_utopia2k_str_async_scheduler(void *data, async_cookie_t cookie)
{
    struct utopia2k_str_module *module = (struct utopia2k_str_module *)data;
    ktime_t starttime;

    if (module && module->fpSTR != NULL) {
        /* 2s timeout */
        module->p = current;
        init_timer(&module->timer);
        module->timer.function = utopia2k_str_module_timeout;
        module->timer.data = (unsigned long)module;
        mod_timer(&module->timer, jiffies + 2 * HZ);
        /* debug module time start */
        starttime = utopia2k_str_time_debug_start(module);
        /* module entry */
        module->fpSTR(module->mode, module->data);
        /* debug module time end */
        utopia2k_str_time_debug_report(starttime, module);
        del_timer_sync(&module->timer);
    }
}

static int mstar_utopia2k_str_drv_suspend(struct platform_device *dev, pm_message_t state)
{
    struct utopia2k_str_module *module;
    struct dts_post_condition *pc, *n;

    list_for_each_entry(module, &utopia2k_str_head, list) {
        if (module && module->fpSTR != NULL) {
            module->mode = UTOPIA2K_STR_POWER_SUSPEND;
            async_schedule_domain(mdrv_utopia2k_str_async_scheduler, module, &condition_domain);
        }
    }
    async_synchronize_full_domain(&condition_domain);

    list_for_each_entry_safe(pc, n, &dts_post_condition_head, list) {
        list_del(&pc->list);
        kfree(pc);
    }

    printk("-----------utopia2k str suspend-----------\n");

    return 0;
}
static int mstar_utopia2k_str_drv_resume(struct platform_device *dev)
{
    struct utopia2k_str_module *module;
    struct dts_post_condition *pc, *n;

    list_for_each_entry(module, &utopia2k_str_head, list) {
        if (module && module->fpSTR != NULL) {
            module->mode = UTOPIA2K_STR_POWER_RESUME;
            async_schedule_domain(mdrv_utopia2k_str_async_scheduler, module, &condition_domain);
        }
    }
    async_synchronize_full_domain(&condition_domain);

    list_for_each_entry_safe(pc, n, &dts_post_condition_head, list) {
        list_del(&pc->list);
        kfree(pc);
    }

    printk("-----------utopia2k str resume-----------\n");
    return 0;
}

static int mstar_utopia2k_str_drv_probe(struct platform_device *pdev)
{
    return 0;
}

static int mstar_utopia2k_str_drv_remove(struct platform_device *pdev)
{
    return 0;
}
#if defined (CONFIG_ARM64)
static struct of_device_id mstar_utopia2k_str_of_device_ids[] = {
         {.compatible = UTOPIA2K_STR_NAME},
         {},
};
#endif
static struct platform_driver Mstar_utopia2k_str_driver = {
    .probe      = mstar_utopia2k_str_drv_probe,
    .remove     = mstar_utopia2k_str_drv_remove,
    .suspend    = mstar_utopia2k_str_drv_suspend,
    .resume     = mstar_utopia2k_str_drv_resume,

    .driver = {
#if defined(CONFIG_ARM64)
        .of_match_table = mstar_utopia2k_str_of_device_ids,
#endif
        .name   = UTOPIA2K_STR_NAME,
        .owner  = THIS_MODULE,
    }
};

#ifndef CONFIG_OF
static struct platform_device Mstar_utopia2k_str_device = {
    .name   = UTOPIA2K_STR_NAME,
    .id     = 0,
};
#endif
static int __init mstar_utopia2k_str_init(void)
{
#ifndef CONFIG_OF
    platform_device_register(&Mstar_utopia2k_str_device);
#endif
    platform_driver_register(&Mstar_utopia2k_str_driver);
    
    INIT_LIST_HEAD(&dts_post_condition_head);
    INIT_LIST_HEAD(&utopia2k_str_head);
    spin_lock_init(&lock);

#if UTOPIA2K_STR_SEFL_TEST
    mdrv_utopia2k_str_setup_function_ptr(NULL, ooStr);
    mdrv_utopia2k_str_setup_function_ptr(NULL, xcStr);
    mdrv_utopia2k_str_setup_function_ptr(NULL, vdecStr);
#endif

    return 0;
}

static void __exit mstar_utopia2k_str_exit(void)
{
    struct utopia2k_str_module *module;
    list_for_each_entry(module, &utopia2k_str_head, list) {
        list_del(&module->list);
        kfree(module);
    }
    platform_driver_unregister(&Mstar_utopia2k_str_driver);
#ifndef CONFIG_OF
    platform_device_unregister(&Mstar_utopia2k_str_device);
#endif
}

module_init(mstar_utopia2k_str_init);
module_exit(mstar_utopia2k_str_exit);

MODULE_DESCRIPTION("Mstar utopia2k STR Device Driver");
MODULE_LICENSE("GPL");

