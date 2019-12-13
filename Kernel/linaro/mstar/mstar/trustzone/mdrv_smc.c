#include "mdrv_smc.h"
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/smp_twd.h>


#define TZ_LLV_DPRINTK(fmt, arg...) //printk(KERN_WARNING"%s:%d " fmt,__FUNCTION__,__LINE__,## args)

int in_secure = 0;

void smc_call(struct smc_struct *smc)
{
    volatile unsigned int u32cpsr;
    volatile unsigned int arg1, arg2;

    arg1 = smc->cmd1;
    arg2 = smc->cmd2;

    extern void __iomem *twd_base;
    __raw_writel(0, twd_base + TWD_TIMER_CONTROL);
	in_secure = 1;
   __asm__ volatile (
        ".arch_extension sec\n\t"
        "mrs %0, cpsr\n\t"
        "push {r0, r4-r12, lr}\n\t"
        "mov r1, %1\n\t"
        "mov r2, %2\n\t"
        "mov  r3, sp  \n\t"
        "smc  0       \n\t"
        "mov sp, r3\n\t"
        "pop {r0, r4-r12, lr}\n\t"
        "mov %1, r1\n\t"
        "mov %2, r2\n\t"
        "msr cpsr, %0\n\t"
        : "+&r" (u32cpsr), "+&r" (arg1), "+&r" (arg2)
        :
        : "r0", "r1", "r2", "r3", "memory");
	in_secure = 0;
    __raw_writel(5, twd_base + TWD_TIMER_CONTROL);

    smc->cmd1 = arg1;
    smc->cmd2 = arg2;

}
