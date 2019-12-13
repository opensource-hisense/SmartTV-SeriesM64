
#include <linux/cpuidle.h>
#include <linux/module.h>
#include <asm/system_misc.h>
#include <asm/proc-fns.h>


static int mstar_enter_idle_state(struct cpuidle_device *dev,
				struct cpuidle_driver *drv, int idx)
{
       cpu_do_idle();
	return 0;
}




static struct cpuidle_driver mstar_idle_driver = {
	.name             = "mstar_cpuidle",
	.owner            = THIS_MODULE,
	.states[0] = {
		.enter            = mstar_enter_idle_state,
		.exit_latency     = 2,
		.target_residency = 1,
		.flags            = CPUIDLE_FLAG_TIME_VALID,
		.name             = "WFI",
		.desc             = "ARM WFI",
	},
	.state_count = 1,
};


static int __init mstar_idle_init(void)
{

	return cpuidle_register(&mstar_idle_driver, NULL);

}

device_initcall(mstar_idle_init);
