#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/clkdev.h>
#include <asm/smp_twd.h>
#include <asm/mach/time.h>
#include <mach/hardware.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>

#include <mach/io.h>
#include <mach/timex.h>
#include <asm/irq.h>
#include <linux/timer.h>
#include <plat/localtimer.h>
#include "chip_int.h"
#include <plat/sched_clock.h>
#include <linux/version.h>
#include <mstar/mpatch_macro.h>

#if LINUX_VERSION_CODE >  KERNEL_VERSION(3,1,10)
#include <linux/clk-provider.h>
#endif
//------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------

//Mstar PIU Timer
#define TIMER_ENABLE			(0x1)
#define TIMER_TRIG				(0x2)
#define TIMER_INTERRUPT		    (0x100)
#define TIMER_CLEAR			    (0x4)
#define TIMER_CAPTURE       	(0x8)
#define ADDR_TIMER_MAX_LOW 	    (0x2<<2)
#define ADDR_TIMER_MAX_HIGH 	(0x3<<2)
#define PIU_TIMER_FREQ_KHZ	    (12000)

//ARM Global Timer
static int GLB_TIMER_FREQ_KHZ;  // PERICLK = CPUCLK/2
static unsigned int GLB_TIMER_FREQ_HZ;  // PERICLK = CPUCLK/2
static unsigned long interval;
int ORI_FREQ_KHZ;


/*
 * xtal: fixed 12M 
 * mpl: fixed 216M
 * pll, variable, 
 * ex: 1.6G --> 2717908.8 / 1.6 = 0x19EB85
 * bank 0x110C, reg 0x60 = 0xEB85
 *              reg 0x61 = 0x19
 * reverse: 2717908.8 / (reg 0x61 << 16 + reg 0x60) * 1000 = 1600(MHz)
 * --> 2717908800 / (reg 0x61 << 16 + reg 0x60) = 1600(MHz)
 *
 * return unit: MHz
 */
#define XTAL 0x0000 /* 12M */
#define MPL  0x0400 /* 216M */
#define PLL  0x8000 /* freq. depends on setting */
static unsigned int cnt=0;
int query_frequency(void)
{
#ifdef CONFIG_MSTAR_ARM_BD_FPGA
	return 12;
#else
	unsigned short freq_type = reg_readw(0x1F000000 + 0x100B00 * 2 + 0x11 * 4);
	unsigned short freq_low, freq_high;

	switch (freq_type) {
	case XTAL:
		return 12;

	case MPL:
		return 216;

	case PLL:
		freq_low = reg_readw(0x1F000000 + 0x110C00 * 2 + 0x60 * 4);
		freq_high = reg_readw(0x1F000000 + 0x110C00 * 2 + 0x61 * 4);

		return 2717908800U / ((freq_high << 16) + freq_low);			// (2717908800 / 2470000)

	default:
		printk(KERN_ERR "");
		BUG_ON(1);
	}
#endif
}
EXPORT_SYMBOL(query_frequency);

#define USE_GLOBAL_TIMER 1
#if USE_GLOBAL_TIMER
static unsigned long long 	src_timer_cnt;
#else
static unsigned int 			src_timer_cnt;
#endif

static unsigned int evt_timer_cnt;
static unsigned int clksrc_base;
static unsigned int clkevt_base;

static cycle_t timer_read(struct clocksource *cs)
{
#if USE_GLOBAL_TIMER
	src_timer_cnt=PERI_R(GT_LOADER_UP);
	src_timer_cnt=(src_timer_cnt<<32)+PERI_R(GT_LOADER_LOW);
#else
	src_timer_cnt=INREG16(clksrc_base+(0x5<<2));
	src_timer_cnt=(src_timer_cnt<<16)+INREG16(clksrc_base+(0x4<<2));
#endif
	return src_timer_cnt;
}
#if LINUX_VERSION_CODE >  KERNEL_VERSION(3,1,10)
struct clocksource clocksource_timer = {
#else
static struct clocksource clocksource_timer = {
#endif
	.name		= "timer1",
	.rating		= 200,
	.read		= timer_read,
	.mask		= CLOCKSOURCE_MASK(32),
	.shift		= 20,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,1,10)
EXPORT_SYMBOL(clocksource_timer);
#endif

void __init chip_clocksource_init(unsigned int base)
{

	struct clocksource *cs = &clocksource_timer;
	clksrc_base = base;
#if USE_GLOBAL_TIMER
	PERI_W(GT_CONTROL,0x1); //Enable

	/*
	 * calculate the value of mult    
	 * cycle = ( time(ns) * mult ) >> shift
	 * PERICLK = CPUCLK / 2
	 */
	cs->mult = clocksource_hz2mult(GLB_TIMER_FREQ_HZ, cs->shift);
#else
	/* setup timer 1 as free-running clocksource */
	//make sure timer 1 is disable
	CLRREG16(clksrc_base, TIMER_ENABLE);

	//set max period
	OUTREG16(clksrc_base+(0x2<<2),0xffff);
	OUTREG16(clksrc_base+(0x3<<2),0xffff);

	//enable timer 1
	SETREG16(clksrc_base, TIMER_ENABLE);

	/*
	 * TODO: need to double check 
	 * calculate the value of mult
	 * cycle = ( time(ns) * mult ) >> shift
     * Mstar timer => 12Mhz,
	 */
	cs->mult = clocksource_hz2mult(GLB_TIMER_FREQ_HZ, cs->shift);  
#endif

	clocksource_register(cs);
}

/*
 * IRQ handler for the timer
 */
static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = dev_id;

	//printk("t");
	/* clear the interrupt */
	evt_timer_cnt=INREG16(clkevt_base+(0x3<<2));
	OUTREG16(clkevt_base+(0x3<<2),evt_timer_cnt);

	//enable timer
    //SETREG16(clkevt_base, TIMER_TRIG);//default

	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static void timer_set_mode(enum clock_event_mode mode,
	struct clock_event_device *evt)
{
    unsigned short ctl=TIMER_INTERRUPT;
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
	interval = (PIU_TIMER_FREQ_KHZ*1000 / HZ)  ;
	OUTREG16(clkevt_base + ADDR_TIMER_MAX_LOW, (interval &0xffff));
	OUTREG16(clkevt_base + ADDR_TIMER_MAX_HIGH, (interval >>16));
        ctl|=TIMER_ENABLE;
		SETREG16(clkevt_base, ctl);
		break;

	case CLOCK_EVT_MODE_ONESHOT:
		/* period set, and timer enabled in 'next_event' hook */
        ctl|=TIMER_TRIG;
		SETREG16(clkevt_base, ctl);
		break;

	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	default:
		break;
	}
}

static int timer_set_next_event(unsigned long next, 
		struct clock_event_device *evt)
{
	//stop timer
	//OUTREG16(clkevt_base, 0x0);

	//set period
	OUTREG16(clkevt_base + ADDR_TIMER_MAX_LOW, (next &0xffff));
	OUTREG16(clkevt_base + ADDR_TIMER_MAX_HIGH, (next >>16));

	//enable timer
	SETREG16(clkevt_base, TIMER_TRIG|TIMER_INTERRUPT);//default

	return 0;
}

static struct clock_event_device clockevent_timer = {
	.name		= "timer0",
	.shift		= 32,
	.features       = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_mode	= timer_set_mode,
	.set_next_event	= timer_set_next_event,
	.rating		= 300,
	.cpumask	= cpu_all_mask,
};

static struct irqaction timer_irq = {
	.name		= "timer",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= timer_interrupt,
	.dev_id		= &clockevent_timer,
};

void __init chip_clockevents_init(unsigned int base,unsigned int irq)
{
	struct clock_event_device *evt = &clockevent_timer;

	clkevt_base = base;

	evt->irq = irq;
	/* PIU Timer FRE = 12Mhz */
	evt->mult = div_sc(PIU_TIMER_FREQ_KHZ, NSEC_PER_MSEC, evt->shift); 
	evt->max_delta_ns = clockevent_delta2ns(0xffffffff, evt);
	evt->min_delta_ns = clockevent_delta2ns(0xf, evt);

	setup_irq(irq, &timer_irq);
	clockevents_register_device(evt);
}


#if LINUX_VERSION_CODE >  KERNEL_VERSION(3,1,10)
#ifdef CONFIG_HAVE_ARM_TWD
static DEFINE_TWD_LOCAL_TIMER(twd_local_timer, chip_MPCORE_TWD, IRQ_LOCALTIMER);

static void __init chip_twd_init(void)
{
    int err = twd_local_timer_register(&twd_local_timer);
    if (err)
        pr_err("twd_local_timer_register failed %d\n", err);
}
#else
#define chip_twd_init()  do {} while(0)
#endif

static int __init chip_init_timer_clocks(void)
{
    struct clk *fclk;
    struct clk *armperi_clk;

    fclk = clk_register_fixed_rate(NULL, "fclk", NULL, CLK_IS_ROOT, GLB_TIMER_FREQ_HZ * 2);

    armperi_clk = clk_register_fixed_rate(NULL, "arm_peri", "fclk", 0, GLB_TIMER_FREQ_HZ);

    /* TODO: use static lookup table? */
    clk_register_clkdev(fclk, "fclk", NULL);
    clk_register_clkdev(armperi_clk, NULL, "smp_twd");

    return 0;
}
#endif

extern u32 SC_MULT;
extern u32 SC_SHIFT;
void __init chip_init_timer(void)
{
	u32 shift,mult;
	unsigned int cpu_freq;

	cpu_freq = query_frequency();
	switch (cpu_freq) {
	case 1100:
		GLB_TIMER_FREQ_HZ = 550183968;
		break;
	case 1000:
		GLB_TIMER_FREQ_HZ = 500167243;
		break;
	case 900:
		GLB_TIMER_FREQ_HZ = 450133952;
		break;
	case 800:
		GLB_TIMER_FREQ_HZ = 400045452;
		break;
	default:
		// PERIPHCLK = CPU Clock / 2  
	// div 2 later,when CONFIG_GENERIC_CLOCKEVENTS
	// clock event will handle this value
		GLB_TIMER_FREQ_HZ = cpu_freq * 1000 / 2;
		printk("Warning! use default formula to get GLB_TIMER_FREQ_HZ\n");
	}

	GLB_TIMER_FREQ_KHZ=(query_frequency()*1000/2); 
  ORI_FREQ_KHZ= GLB_TIMER_FREQ_KHZ *2;
	printk("\033[35mGlobal Timer Frequency = %d MHz\033[m\n", GLB_TIMER_FREQ_HZ/1000/1000);
	printk("\033[35mCPU Clock Frequency = %d Hz\033[m\n", GLB_TIMER_FREQ_HZ * 2);
#ifdef CONFIG_MP_ANTUTU_BENCHMARK_RISE_PERFORMANCE
	  GLB_TIMER_FREQ_KHZ=(1600*1000/2); 
    ORI_FREQ_KHZ= 1600*1000;
    	printk("\033[35mGlobal Timer Frequency = %d MHz\033[m\n", GLB_TIMER_FREQ_KHZ/1000);
	printk("\033[35mCPU Clock Frequency = %d MHz\033[m\n", query_frequency());
#endif // CONFIG_MP_ANTUTU_BENCHMARK_RISE_PERFORMANCE

	

#if LINUX_VERSION_CODE >  KERNEL_VERSION(3,1,10)
	chip_init_timer_clocks();
#endif

	clocks_calc_mult_shift(&mult, &shift,
			GLB_TIMER_FREQ_HZ , NSEC_PER_SEC, 0);
	printk("fre = %d, mult= %d, shift= %d\n",
			GLB_TIMER_FREQ_HZ , mult, shift);
	SC_SHIFT=shift;
	SC_MULT=mult;

	mstar_sched_clock_init((void __iomem *)(PERI_VIRT + 0x200),
			GLB_TIMER_FREQ_HZ);

	chip_clocksource_init(chip_BASE_REG_TIMER1_PA);
	chip_clockevents_init(chip_BASE_REG_TIMER0_PA,E_FIQ_EXTIMER0);

#if LINUX_VERSION_CODE >  KERNEL_VERSION(3,1,10)
	chip_twd_init();
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,23)
struct sys_timer chip_timer = {
    .init = chip_init_timer,
};
#endif