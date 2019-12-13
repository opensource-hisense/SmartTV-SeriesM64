#include <linux/suspend.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include "chip_setup.h"
#include "pm.h"

#if (CONFIG_MSTAR_PM_ADDR)
#if defined(CONFIG_MP_PLATFORM_MIPS)
#include "mdrv_mstypes.h"
#endif
#endif /* MP_PLATFORM_PM */

#define WAKEUP_SAVE_ADDR 0x80000000

#if (CONFIG_MSTAR_PM_ADDR)
#if defined(CONFIG_MP_PLATFORM_MIPS)
#define KSEG0_BASE  (0x80000000)
#define KSEG1_BASE  (0xA0000000)
extern void enable_cache(MS_BOOL bEnable);
extern MS_BOOL ric_fill_icache(MS_U32 insts[], MS_U32 size);
extern MS_BOOL ric_fill_dcache(MS_U32 insts[], MS_U32 size);
extern unsigned long g_pm_Iaddress, g_pm_Isize;
extern unsigned long g_pm_Daddress, g_pm_Dsize;
extern unsigned long g_pm_run;
#endif /* CONFIG_MP_PLATFORM_MIPS */
#endif /* CONFIG_MSTAR_PM_ADDR */

extern void sleep_save_cpu_registers(void);
extern void sleep_wakeup_first(void);
extern void sleep_restore_cpu_registers(void);
extern void sleep_set_wakeup_save_addr_phy(unsigned long phy_addr, void *virt_addr);
extern void sleep_clear_wakeup_save_addr_phy(unsigned long phy_addr, void *virt_addr);
extern void sleep_prepare_last(unsigned long wakeup_addr_phy);
extern void Chip_L2_cache_wback(unsigned long addr, unsigned long size);
extern void  MDrv_MBX_NotifyPMtoSetPowerOff(void);
extern void  MDrv_MBX_NotifyPMPassword(unsigned char passwd[16]);
extern int get_str_max_cnt(void);
extern void SerPrintf(char *fmt,...);
extern void SerPrintfAtomic(char *fmt,...);
DEFINE_SPINLOCK(ser_printf_lock);
static unsigned char pass_wd[16]={0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,
                                   0x11,0x00,0xFF,0xEE,0xDD,0xCC,0xBB,0xAA};

static u32 mstr_cnt=0;
static int pre_str_max_cnt=0;

void SerPrintChar(char ch)
{
#if CONFIG_EARLY_PRINTK
    extern int prom_putchar(char c);
    prom_putchar(ch);
#endif
}
void SerPrintStr(char *p)
{
    int nLen=strlen(p);
    int i;
    for(i=0;i<nLen;i++)
    {
        if(p[i]=='\n')SerPrintChar('\r');
        SerPrintChar(p[i]);
    }
}
void SerPrintStrAtomic(char *p)
{
    u_long flag;
    spin_lock_irqsave(&ser_printf_lock,flag);
    SerPrintStr(p);
    spin_unlock_irqrestore(&ser_printf_lock,flag);
}
void SerPrintf(char *fmt,...)
{
    char tmpbuf[500];
    int nLen;
    va_list args;
    va_start(args, fmt);
    nLen=vscnprintf(tmpbuf, 500, fmt, args);
    va_end(args);
    if(nLen<=0)
    {
        nLen=0;
    }
    else if(nLen>=500)
    {
        nLen=500-1;
    }
    tmpbuf[nLen]=0;
    SerPrintStr(tmpbuf);
}
void SerPrintfAtomic(char *fmt,...)
{
    char tmpbuf[500];
    int nLen;
    va_list args;
    va_start(args, fmt);
    nLen=vscnprintf(tmpbuf, 500, fmt, args);
    va_end(args);
    if(nLen<=0)
    {
        nLen=0;
    }
    else if(nLen>=500)
    {
        nLen=500-1;
    }
    tmpbuf[nLen]=0;
    SerPrintStrAtomic(tmpbuf);
}
int vSerPrintf(const char *fmt, va_list args)
{
    char tmpbuf[500];
    int nLen;
    nLen=vscnprintf(tmpbuf, 500, fmt, args);
    if(nLen<=0)
    {
        nLen=0;
    }
    else if(nLen>=500)
    {
        nLen=500-1;
    }
    tmpbuf[nLen]=0;
    SerPrintStr(tmpbuf);
    return nLen;
}
int vSerPrintfAtomic(const char *fmt, va_list args)
{
    char tmpbuf[500];
    int nLen;
    nLen=vscnprintf(tmpbuf, 500, fmt, args);
    if(nLen<=0)
    {
        nLen=0;
    }
    else if(nLen>=500)
    {
        nLen=500-1;
    }
    tmpbuf[nLen]=0;
    SerPrintStrAtomic(tmpbuf);
    return nLen;
}
void WriteRegWord(unsigned long addr, unsigned long val)
{
    volatile unsigned long *regaddr=(unsigned long *)addr;
    (*regaddr)=val;
}
unsigned long ReadRegWord(unsigned long addr)
{
    volatile unsigned long *regaddr=(unsigned long *)addr;
    return (*regaddr);
}

unsigned long read_wkup_pmu(void)
{
    unsigned long tmp=ReadRegWord(PMU_WAKEUP_ADDR_REGL)&0xFFFF;
    return tmp;
}
void write_wkup_pmu(unsigned long val)
{
    unsigned long tmp=val&0xFFFF;
    WriteRegWord(PMU_WAKEUP_ADDR_REGL,tmp);
}
unsigned long mstar_virt_to_phy(void* virtaddr)
{
    unsigned long rest=0;
    rest=virt_to_phys(virtaddr);
    return rest;
}

void* mstar_phy_to_virt(unsigned long phyaddr )
{
    void *rest=0;
    rest=phys_to_virt(phyaddr);
    return rest;
}

void mstar_sleep_cur_cpu_flush(void)
{
    Chip_L2_cache_wback( 0 , 0xFFFFFFFF );
}

static void mstar_str_notifypmmaxcnt_off(void)
{
    sleep_clear_wakeup_save_addr_phy(mstar_virt_to_phy((void*)WAKEUP_SAVE_ADDR),(void*)WAKEUP_SAVE_ADDR);
    pass_wd[0x0A]=0xFD;
    MDrv_MBX_NotifyPMPassword(pass_wd);
    while(1);
}

#if defined(CONFIG_MSTAR_STR_ACOFF_ON_ERR)
void mstar_str_notifypmerror_off(void)
{
    sleep_clear_wakeup_save_addr_phy(mstar_virt_to_phy((void*)WAKEUP_SAVE_ADDR),(void*)WAKEUP_SAVE_ADDR);
    pass_wd[0x0A]=0xFE;
    MDrv_MBX_NotifyPMPassword(pass_wd);
    while(1);
}
#endif

#if (CONFIG_MSTAR_PM_ADDR)
#if defined(CONFIG_MP_PLATFORM_MIPS)
static void _fill_cache_by_DRAM_addr(unsigned long u32pm_Iaddress, unsigned long u32pm_Isize, unsigned long u32pm_Daddress, unsigned long u32pm_Dsize, unsigned long u32pm_run)
{

    printk("boot : ORIGIN = 0x%x, LENGTH = 0x%x\n", (unsigned int)(u32pm_Iaddress|KSEG0_BASE), (unsigned int)u32pm_Isize);
    printk("data : ORIGIN = 0x%x, LENGTH = 0x%x\n", (unsigned int)(u32pm_Daddress|KSEG0_BASE), (unsigned int)u32pm_Dsize);
    printk("start: 0x%x\n", (unsigned int)(u32pm_run|KSEG0_BASE));

    _dma_cache_wback_inv((unsigned long)__va(u32pm_Iaddress|KSEG0_BASE), 0x10000);

    // disable cache
    enable_cache(0);

    printk("[PM] disable icache\n");

    // fill icache
    ric_fill_icache((void *)(u32pm_Iaddress|KSEG1_BASE), u32pm_Isize);
    ric_fill_dcache((void *)(u32pm_Daddress|KSEG1_BASE), u32pm_Dsize);

    printk("[PM] fill icache end\n");
    // enable cache
    enable_cache(1);

    asm volatile (
        "move $9, %[i0]\n\t" \
        "j $9\n\t" \
        "nop\n\t" \
        : \
        : [i0] "r"(u32pm_run|KSEG0_BASE) \
        : "memory" \
    );

}
#endif
#endif

/*------------------------------------------------------------------------------
    Function: mstar_pm_enter

    Description:
        Actually enter sleep state
    Input: (The arguments were used by caller to input data.)
        state - suspend state (not used)
    Output: (The arguments were used by caller to receive data.)
        None.
    Return:
        0
    Remark:
        None.
-------------------------------------------------------------------------------*/
static int mstar_pm_enter(suspend_state_t state)
{
    void *pWakeup=0;
    __asm__ volatile (
        "la $8, MSTAR_WAKEUP_ENTRY \n"
        "sw $8, %0\n"
        :"=m"(pWakeup)::"t0"
        );
    if(pre_str_max_cnt!=get_str_max_cnt())
    {
        pre_str_max_cnt=get_str_max_cnt();
        mstr_cnt=0;
    }
    mstr_cnt++;
    sleep_save_cpu_registers();
    sleep_set_wakeup_save_addr_phy(mstar_virt_to_phy((void*)WAKEUP_SAVE_ADDR),(void*)WAKEUP_SAVE_ADDR);
    sleep_prepare_last(mstar_virt_to_phy(pWakeup));
    SerPrintf("\nMStar STR waiting power off...\n");
    __asm__ volatile (
        "nop\n"
        :::"v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7","t8","t9",\
           "s0","s1","s2","s3","s4","s5","s6","s7"
        );
#if 1
    // pass different password to do ac onoff
    if(get_str_max_cnt()>0 &&mstr_cnt>=get_str_max_cnt())
    {
        SerPrintf("Max Cnt Ac off...\n");
        mstar_str_notifypmmaxcnt_off();
    }
    else
    {
        MDrv_MBX_NotifyPMtoSetPowerOff();
    }

#if (CONFIG_MSTAR_PM_ADDR)
#if defined(CONFIG_MP_PLATFORM_MIPS)
    unsigned long x = (unsigned long)pWakeup;
    WriteRegWord((0xBF000000 + (0x103380 << 1)), x);
    WriteRegWord((0xBF000000 + (0x103382 << 1)), (x >> 16));
    _fill_cache_by_DRAM_addr(g_pm_Iaddress, g_pm_Isize, g_pm_Daddress, g_pm_Dsize, g_pm_run);
#endif
#endif

    __asm__ volatile(
        "WAIT_SLEEP:\n"
        "nop\n"
        "b WAIT_SLEEP\n"
        "nop\n"
        );
#endif
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "MSTAR_WAKEUP_ENTRY:\n"
        "jal SerPrintChar\n"
        "li  $4, 'K'\n"
        "jal ensure_environment\n"
        "nop\n"
        "jal SerPrintChar\n"
        "li  $4, 'N'\n"
        "jal sleep_wakeup_first\n"
        "nop\n"
        "jal SerPrintChar\n"
        "li  $4, 'L'\n"
        "jal sleep_restore_cpu_registers\n"
        "nop\n"
        ".set pop\n"
        :::"v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7","t8","t9",\
           "s0","s1","s2","s3","s4","s5","s6","s7"
    );
    SerPrintf("\nMStar STR Resuming...\n");
    sleep_clear_wakeup_save_addr_phy(mstar_virt_to_phy((void*)WAKEUP_SAVE_ADDR),(void*)WAKEUP_SAVE_ADDR);
    return 0;
}

static struct platform_suspend_ops mstar_pm_ops =
{
    .enter      = mstar_pm_enter,
    .valid      = suspend_valid_only_mem,
};


/*------------------------------------------------------------------------------
    Function: mstar_pm_init

    Description:
        init function of power management
    Input: (The arguments were used by caller to input data.)
        None.
    Output: (The arguments were used by caller to receive data.)
        None.
    Return:
        0
    Remark:
        None.
-------------------------------------------------------------------------------*/
static int __init mstar_pm_init(void)
{
    /* set operation function of suspend */
    suspend_set_ops(&mstar_pm_ops);
    return 0;
}

__initcall(mstar_pm_init);
