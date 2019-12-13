#include <linux/fs.h>
#include <linux/hugetlb.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/quicklist.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/ctype.h>

#include <asm/setup.h>
#include <asm/cacheflush.h>
#include <linux/delay.h>

#include <include/mstar/mstar_chip.h>
#include <linux/spinlock.h>

#include <linux/module.h>
#include <linux/cpufreq.h>
#include <linux/smp.h>
#include <linux/io.h>
#include <linux/timer.h>

#include "./include/mach/hardware.h"
#include "./include/mach/platform.h"
#include <asm/mach-types.h>
#include <asm/hardware/icst.h>

#include "chip_dvfs_calibrating.h"

#ifdef CONFIG_MSTAR_DVFS
#ifndef __MDRV_DVFS_H__
#include "mdrv_dvfs.h"
#endif
#endif

extern void change_interval(unsigned int old_freq, unsigned int new_freq);
static atomic_t proc_is_open = ATOMIC_INIT(0);
static DEFINE_SPINLOCK(set_freq_lock);
static struct cpufreq_driver integrator_driver;
unsigned int current_frequency = 0;
unsigned int echo_calibrating_freq = 0;
unsigned int register_frequency = 0;
int echo_calibrating = 0;

#define MAX_DMSG_WRITE_BUFFER	64

#define CM_ID  	IO_ADDRESS(INTEGRATOR_HDR_ID)
#define CM_OSC	IO_ADDRESS(INTEGRATOR_HDR_OSC)
#define CM_STAT IO_ADDRESS(INTEGRATOR_HDR_STAT)
#define CM_LOCK IO_ADDRESS(INTEGRATOR_HDR_LOCK)

#define FREQ_CHECK_LINK_TIME (HZ)

static atomic_t on_demand_handshake_is_open = ATOMIC_INIT(0);
DECLARE_WAIT_QUEUE_HEAD(DVFS_on_demand_event_waitqueue);
DECLARE_WAIT_QUEUE_HEAD(DVFS_on_demand_event_waitqueue_userspace_return);
EXPORT_SYMBOL(DVFS_on_demand_event_waitqueue);
EXPORT_SYMBOL(DVFS_on_demand_event_waitqueue_userspace_return);
unsigned int start_userspace_ondemand_handshake = 0;
unsigned int ready_to_change_cpufreq = 0;
unsigned int ready_to_change_voltage = 0;
unsigned int ready_to_change_voltage_type = 0;
unsigned int change_cnt = 0;
unsigned int finished_change_cnt = 0;
int voltage_change_result = 0;

static struct timer_list Check_Freq_timer;

extern unsigned int query_frequency(void);
int get_freq(void)
{
	return query_frequency() * 1000;
}	

/*
 * Validate the speed policy.
 */
static int integrator_verify_policy(struct cpufreq_policy *policy)
{
	struct icst_vco vco;
	cpufreq_verify_within_limits(policy, 
				     policy->cpuinfo.min_freq, 
				     policy->cpuinfo.max_freq);

	//printk("\033[31mFunction = %s, Line = %d, policy min %d, policy max %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, policy->min, policy->max);
	//printk("\033[31mFunction = %s, Line = %d, cpuinfo min %d, cpuinfo max %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, policy->cpuinfo.min_freq, policy->cpuinfo.max_freq);

	//cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq, policy->cpuinfo.max_freq);

	return 0;
}

static int integrator_set_target(struct cpufreq_policy *policy,
				 unsigned int target_freq,
				 unsigned int relation)
{
	cpumask_t cpus_allowed;
	int cpu = policy->cpu;
	struct icst_vco vco;
	struct cpufreq_freqs freqs;
	u_int cm_osc;

	/*
	 * Save this threads cpus_allowed mask.
	 */
	cpus_allowed = current->cpus_allowed;

	/*
	 * Bind to the specified CPU.  When this call returns,
	 * we should be running on the right CPU.
	 */
	set_cpus_allowed(current, cpumask_of_cpu(cpu));
	BUG_ON(cpu != smp_processor_id());

	/* get current setting */
	/*cm_osc = __raw_readl(CM_OSC);

	if (machine_is_integrator()) {
		vco.s = (cm_osc >> 8) & 7;
	} else if (machine_is_cintegrator()) {
		vco.s = 1;
	}
	vco.v = cm_osc & 255;
	vco.r = 22;
	freqs.old = icst_hz(&cclk_params, vco) / 1000; */
 	 freqs.old = get_freq(); //  johnson
  
	/* icst_hz_to_vco rounds down -- so we need the next
	 * larger freq in case of CPUFREQ_RELATION_L.
	 */
	if (relation == CPUFREQ_RELATION_L)
		target_freq += 999;
	if (target_freq > policy->max)
		target_freq = policy->max;
	//vco = icst_hz_to_vco(&cclk_params, target_freq * 1000);
	//freqs.new = icst_hz(&cclk_params, vco) / 1000;
	freqs.new = get_freq(); //johnson

	freqs.cpu = policy->cpu;

	if (freqs.old == freqs.new) {
		set_cpus_allowed(current, cpus_allowed);
		return 0;
	}

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
   //set_freq(freqs.new/1000); //johnson
	/*cm_osc = __raw_readl(CM_OSC);

	if (machine_is_integrator()) {
		cm_osc &= 0xfffff800;
		cm_osc |= vco.s << 8;
	} else if (machine_is_cintegrator()) {
		cm_osc &= 0xffffff00;
	}
	cm_osc |= vco.v;

	__raw_writel(0xa05f, CM_LOCK);
	__raw_writel(cm_osc, CM_OSC);
	__raw_writel(0, CM_LOCK); */

	/*
	 * Restore the CPUs allowed mask.
	 */
	set_cpus_allowed(current, cpus_allowed);

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	return 0;
}

static unsigned int integrator_get(unsigned int cpu)
{
	cpumask_t cpus_allowed;
	unsigned int current_freq;
	u_int cm_osc;
	struct icst_vco vco;
	
	cpus_allowed = current->cpus_allowed;
	set_cpus_allowed(current, cpumask_of_cpu(cpu));
	BUG_ON(cpu != smp_processor_id());
	
	current_freq = get_freq();

	set_cpus_allowed(current, cpus_allowed);

	return current_freq;
}

static int integrator_cpufreq_init(struct cpufreq_policy *policy)
{
	/* set default policy and cpuinfo */
	policy->cpuinfo.max_freq = policy->max = MAX_CPU_FREQ;		/* Max is 1450 MHz, we set to 1400 MHz */
	policy->cpuinfo.min_freq = policy->min = MIN_CPU_FREQ;		/* Low is  950 MHz, we set to 1000 MHz */
	policy->cpuinfo.transition_latency = TRANSITION_LATENCY; 	/* 1 ms, assumed */
	policy->cur = integrator_get(policy->cpu);
	printk("\033[35mFunction = %s, Line = %d, integrator_get return %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, policy->cur);
	return 0;
}

typedef struct _IO_CPU_calibrating_INFO
{
	char* MESSAGE_BUFF;
	char CPUID;
	char  MESSAGE_LEN;
	int MID;	
}IO_CPU_calibrating_INFO;

static int CPU_calibrating_proc_ioctl(struct file *filp, unsigned int cmd, IO_CPU_calibrating_INFO* message_buf)
{
	IO_CPU_calibrating_INFO* bb = message_buf;
	char usr_buf[256];

	if (copy_from_user(usr_buf, bb->MESSAGE_BUFF, bb->MESSAGE_LEN))
	{
		printk("setgreq_proc_ioctl error\n");
			return -EFAULT;
		}
			
	return 0;
}

extern int ORI_FREQ_KHZ;
static ssize_t CPU_calibrating_proc_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[MAX_DMSG_WRITE_BUFFER];
	long idx;
	int i = 0; 
	struct cpufreq_freqs freqs;
 
	if (!count)
		return count;

	if (count >= MAX_DMSG_WRITE_BUFFER)
		count = MAX_DMSG_WRITE_BUFFER - 1;

	/*
	 * Prevent Tainted Scalar Warning:
	 * Buffer can't be tainted because:
	 * 1. The count never exceeds MAX_DMSG_WRITE_BUFFER i.e. buffer size.
	 * 2. copy_from_user returns 0 in case of correct copy.
	 *So, we don't need to sanitize buffer.
	 *
	 */
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

	if (buffer[0] == '/')
		idx = 3;
	else if (strict_strtol(buffer, 0, &idx) != 0)  //Force change str to decimal conversion (str, base, *converted_num), base is hex, decimal, or ...
		return -EINVAL;

	register_frequency = idx;
	echo_calibrating_freq = idx;
	echo_calibrating++;
#ifdef MSTAR_CHIP_ARM_CORE_NUM
    for(i = 0; i < MSTAR_CHIP_ARM_CORE_NUM; i++)
#else
	for(i = 0;i < CONFIG_NR_CPUS; i++)
#endif	
	{   
		freqs.cpu = i;
		freqs.old = current_frequency;
		freqs.new = idx;
		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

		cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
		
	}
	current_frequency = idx;
	change_interval(freqs.old,freqs.new);	
	
	if(echo_calibrating % 2 == 0)
	{
		printk("\033[31mFunction = %s, Line = %d, special task is over, reset echo_calibrating_freq\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		echo_calibrating_freq = 0;
	}
	else
		printk("\033[31mFunction = %s, Line = %d, special job is working now..., force using cpu_freq: %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, echo_calibrating_freq);

	return count;
}

void write_cpufreq_to_RIU(unsigned int new_scaling_cur_freq)
{
	// write cpu_freq to RIU(busybox devmem 0x1F200A1C 16 0x0578 ==> write 0x0578 to 0x1F200A1C, 0x0578 is 1400(1.4 GHz))
	printk("\033[31mFunction = %s, Line = %d, write cpu_freq to RIU: 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)(new_scaling_cur_freq/1000)); // joe.liu
	reg_writew((new_scaling_cur_freq/1000), 0x1F200A1C);
}
	
unsigned int wait_for_voltage(unsigned int cpufreq_from_cpuload)
{
#ifndef CONFIG_MSTAR_DVFS
	if(start_userspace_ondemand_handshake == 1)
	{
		unsigned int old_voltage = ready_to_change_voltage;

		ready_to_change_cpufreq = cpufreq_from_cpuload;
		ready_to_change_voltage = 120; // temp, need to add safe check
		ready_to_change_voltage_type = 0; // temp

		// the adjusted cpufreq_from_cpuload and ready_to_change_voltage is as old one ==> do nothing
		if( (ready_to_change_voltage == old_voltage) && (cpufreq_from_cpuload == current_frequency) )
			return current_frequency;
	
		change_cnt++;

		// if cpufreq_from_cpuload or voltage is changed, then need to wakeup read waitqueue
		wake_up_interruptible(&DVFS_on_demand_event_waitqueue);
		printk("\033[35mFunction = %s, change_cnt is %d, finished_change_cnt is %d\033[m\n", __PRETTY_FUNCTION__, change_cnt, finished_change_cnt); // joe.liu

		// waiting for voltage change in userspace finished
		wait_event_interruptible_timeout(DVFS_on_demand_event_waitqueue_userspace_return, finished_change_cnt==change_cnt , MAX_SCHEDULE_TIMEOUT);

		if(voltage_change_result == 0) // pass in userspace voltage change
			printk("\033[35mFunction = %s, Line = %d, finish No-%d DVFS_ONDEMAND\033[m\n", __PRETTY_FUNCTION__, __LINE__, finished_change_cnt); // joe.liu
	}
#endif
	return cpufreq_from_cpuload;
}

#ifdef CONFIG_MSTAR_DVFS
void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType);
void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType)
{
    if(start_userspace_ondemand_handshake == 1)
    {
    	ready_to_change_cpufreq = dwCpuClock;
    	ready_to_change_voltage = dwVoltage;
    	ready_to_change_voltage_type = dwVoltageType;
    	change_cnt++;
    	
    	mdelay(100);

    	wake_up_interruptible(&DVFS_on_demand_event_waitqueue);
    	printk("\033[35m[INFO] Data Exchange Count to User Space: %d\033[m\n", change_cnt);
    	printk("\033[35m[INFO] Voltage: %d\033[m\n", dwVoltage);
    	printk("\033[35m[INFO] VoltageType: %d\033[m\n", dwVoltageType);
    	
    	// waiting for voltage change in userspace finished
    	wait_event_interruptible_timeout(DVFS_on_demand_event_waitqueue_userspace_return, finished_change_cnt==change_cnt , MAX_SCHEDULE_TIMEOUT);
    	printk("\033[35m[INFO] Data Exchange Count from User Space: %d\033[m\n", finished_change_cnt);
    }
}
EXPORT_SYMBOL(MDrvDvfsVoltageSetup);
#endif

static int integrator_set_target_on_demand(struct cpufreq_policy *policy, unsigned int target_freq, unsigned int relation)
{
	if(!start_userspace_ondemand_handshake)
	{
		return 0;
	}
	else
	{
		int cpu = policy->cpu;
		cpumask_t cpus_allowed;

	// only cpu0 is allowed to adhjust cpufreq(due to no mutex for now)
	if(smp_processor_id() != 0)
		return 0;

		if(echo_calibrating % 2 == 1)
		{
			if(echo_calibrating_freq != 0)
				target_freq = echo_calibrating_freq;
			
			//printk("\033[31mFunction = %s, Line = %d, special job is working now..., force using cpu_freq: %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, echo_calibrating_freq);
	}

	/*
	 * Save this threads cpus_allowed mask.
	 */
	cpus_allowed = current->cpus_allowed;

	/*
	 * Bind to the specified CPU.  When this call returns,
	 * we should be running on the right CPU.
	 */
	set_cpus_allowed(current, cpumask_of_cpu(cpu));
	BUG_ON(cpu != smp_processor_id());
	
	if(target_freq > policy->max)
		target_freq = policy->max;
	if(target_freq < policy->min)
		target_freq = policy->min;

		/*
		 * set all cpus to new cpu_freq(scaling_cur_freq), and set the timer
		 * always let wait_for_voltage to do voltage_change, adjust target_freq according to Temperature, write adjusted target_freq to RIU, then use new target_freq to set timer, jiffes
		 */

		/* Step 1: rise voltage first, wait for voltage change(accroding to target_freq) */
#ifdef CONFIG_MSTAR_DVFS
        target_freq = MDrvDvfsProc(target_freq);
#else
		target_freq = wait_for_voltage(target_freq);
#endif

		/* Step 2: check if newly adjusted target_freq is equal to current_frequency
		 *         I think this should be done in wait_for_voltage, before voltage change, and write to RIU
		 */
		if(current_frequency == target_freq)
			return 0;

		if(voltage_change_result == 1) // failed in userspace voltage change
			return 0;

		/* Step 3: update timer, cpu_freq(RIU), and scaling_cur_freq for all cpus */
		change_cpus_timer(__FUNCTION__, target_freq);

#ifdef CONFIG_MSTAR_DVFS
//		MDrvDvfsCpuClockSetup(target_freq);	// wirte target_freq to RIU
#else
		write_cpufreq_to_RIU(target_freq);
#endif

		/*
		 * Restore the CPUs allowed mask.
		 */
		set_cpus_allowed(current, cpus_allowed);
	
		return 0;
	}
}

void change_cpus_timer(char *caller, unsigned int target_freq)
{
	int i = 0;
	struct cpufreq_policy *other_cpu_policy;
	struct cpufreq_freqs freqs;

	spin_lock(&set_freq_lock);
	//register_frequency = target_freq; 
	printk("\033[36mcaller: %s\033[m\n", caller);
    for(i = 0; i < CONFIG_NR_CPUS; i++)
	{
		freqs.cpu = i;
		other_cpu_policy = cpufreq_cpu_get(i);

		if(!other_cpu_policy) // to avoid some cpus have not had its policy
		{
			printk("\033[35mFunction = %s, Line = %d, cpu%d do: [cpu%d] policy is NULL\033[m\n", __PRETTY_FUNCTION__, __LINE__, smp_processor_id(), i);
			continue;
		}
		if( (other_cpu_policy->cur < other_cpu_policy->min) || (other_cpu_policy->cur > other_cpu_policy->max) ) // to avoid some cpus have wrong scaling setting
		{
			printk("\033[35mFunction = %s, Line = %d, cpu%d do: [cpu%d] scaling_cur_freq is %d, not allowed\033[m\n", __PRETTY_FUNCTION__, __LINE__, smp_processor_id(), i, other_cpu_policy->cur);
			continue;
		}

		if(other_cpu_policy->cur == target_freq)
			continue;

		freqs.old = other_cpu_policy->cur;
		freqs.new = target_freq;

		if(i == 0)
		printk("\033[35mFunction = %s, Line = %d, cpu%d do: [cpu%d] adjust cpufreq for %d to %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, smp_processor_id(), i, freqs.old, freqs.new);
		
		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
		cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	}

	change_interval(current_frequency, target_freq);
	current_frequency = target_freq;
	spin_unlock(&set_freq_lock);
}

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
static struct cpufreq_driver integrator_driver = {
	.verify		= integrator_verify_policy,
	.target		= integrator_set_target_on_demand,
	.get		= integrator_get,
	.init		= integrator_cpufreq_init,
	.name		= "ondemand",
};
#else
static struct cpufreq_driver integrator_driver = {
	.verify		= integrator_verify_policy,
	.target		= integrator_set_target,
	.get		= integrator_get,
	.init		= integrator_cpufreq_init,
	.name		= "integrator",
};
#endif

static int CPU_calibrating_proc_open(struct inode *inode, struct file *file)
{
	if (atomic_read(&proc_is_open))
		return -EACCES;

	atomic_set(&proc_is_open, 1);
	return 0;	
}

static int CPU_calibrating_proc_release(struct inode *inode, struct file * file)
{

	WARN_ON(!atomic_read(&proc_is_open));
	atomic_set(&proc_is_open, 0);
	return 0;
}

static int on_demand_handshake_proc_open(struct inode *inode, struct file *file)
{
	printk("\033[35mFunction = %s, Line = %d, set start_userspace_ondemand_handshake to be 1\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
	start_userspace_ondemand_handshake = 1;

	if(atomic_read(&on_demand_handshake_is_open))
		return -EACCES;

	atomic_set(&on_demand_handshake_is_open, 1);
	return 0;
}

static int on_demand_handshake_proc_release(struct inode *inode, struct file * file)
{
	printk("\033[35mFunction = %s, Line = %d, set start_userspace_ondemand_handshake to be 0\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
	start_userspace_ondemand_handshake = 0;

	WARN_ON(!atomic_read(&on_demand_handshake_is_open));
	atomic_set(&on_demand_handshake_is_open, 0);
	return 0;
}

ssize_t on_demand_handshake_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	ON_Demand_From_Userspace from_user_data;

	if(!start_userspace_ondemand_handshake)
	{
		printk("\033[35mFunction = %s, Line = %d, Have not got fd\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return  -EFAULT;
	}
	else
	{
		if(!count)
		{
			printk("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
			return count;
		}

		if(count >= MAX_DMSG_WRITE_BUFFER)
			count = MAX_DMSG_WRITE_BUFFER - 1;

		copy_from_user(&from_user_data, buf, sizeof(from_user_data));
		finished_change_cnt = from_user_data.from_userspace_finished_change_cnt;
		voltage_change_result = from_user_data.from_userspace_voltage_change_result;

		printk("\033[33m[result is %d] input: %d, wake_up wait_queue for write_down\033[m\n", voltage_change_result, finished_change_cnt);
		wake_up_interruptible(&DVFS_on_demand_event_waitqueue_userspace_return);	// to wake_up a wait_queue waiting for voltage change

		return count;
	}
}

ssize_t on_demand_handshake_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	ON_Demand_To_Userspace to_userspace_data;
	int err;

	if(!start_userspace_ondemand_handshake)
	{
		printk("\033[35mFunction = %s, Line = %d, Have not got fd, start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, start_userspace_ondemand_handshake);
		return  -EFAULT;
	}
	else
	{
		DECLARE_WAITQUEUE(wait, current);
		allow_signal(SIGKILL);
		add_wait_queue(&DVFS_on_demand_event_waitqueue, &wait);

		printk("[%s] on_demand_handshake: sleeping\n", current->comm);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		if(signal_pending(current))
		{
			printk("on_demand_handshake: received a SIGKILL signal\n");

			set_current_state(TASK_RUNNING);
			remove_wait_queue(&DVFS_on_demand_event_waitqueue, &wait);

			to_userspace_data.to_userspace_cpufreq = 0;
			to_userspace_data.to_userspace_voltage = 0;
			to_userspace_data.to_userspace_voltage_type = 0;
			to_userspace_data.to_userspace_change_cnt = 0;

			err = copy_to_user(buf, &to_userspace_data, sizeof(to_userspace_data));

			*ppos += sizeof(to_userspace_data);

			return sizeof(to_userspace_data);	// userspace should add error handling, if all values are 0, re-do read
		}
		set_current_state(TASK_RUNNING);
		remove_wait_queue(&DVFS_on_demand_event_waitqueue, &wait);
		printk("on_demand_handshake: [%s] woken up, copy cpu_voltage:%d cpu_voltage_type:%d change_cnt: %d to userspace\n", current->comm, ready_to_change_voltage, ready_to_change_voltage_type, change_cnt);

		to_userspace_data.to_userspace_cpufreq = ready_to_change_cpufreq;
		to_userspace_data.to_userspace_voltage = ready_to_change_voltage;
		to_userspace_data.to_userspace_voltage_type = ready_to_change_voltage_type;
		to_userspace_data.to_userspace_change_cnt = change_cnt;

		err = copy_to_user((void *)buf, &to_userspace_data, sizeof(to_userspace_data));

		*ppos += sizeof(to_userspace_data);

		return sizeof(to_userspace_data);
	}
}

const struct file_operations proc_CPU_calibrating_operations = {
	.write      = CPU_calibrating_proc_write,
	.open       = CPU_calibrating_proc_open,
	.release    = CPU_calibrating_proc_release,
	.unlocked_ioctl  = CPU_calibrating_proc_ioctl,
};

const struct file_operations proc_on_demand_handshake_operations = {
	.open       = on_demand_handshake_proc_open,
	.write      = on_demand_handshake_proc_write,
	.read		= on_demand_handshake_proc_read,
	.release    = on_demand_handshake_proc_release,
};

static void Mdrv_CPU_Freq_Check_callback(unsigned long value)
{
	int freq,i;
	struct cpufreq_freqs freqs;

	if(*(volatile u32 *)(0xfd200a00) == 0x3697)
   	{
   		freq = *(volatile u32 *)(0xfd200a04);
   		freq = freq * 1000;
   		register_frequency = freq;

   		if (freq != current_frequency)
   		{
			printk("\033[36m\nFunction = %s, Line = %d, (freq = %d) != (current_frequency = %d)\033[m\n", __PRETTY_FUNCTION__, __LINE__, freq, current_frequency);
   			for(i = 0; i < CONFIG_NR_CPUS; i++)
	        {
	        	freqs.cpu = i;
		        freqs.old = current_frequency;
		        freqs.new = freq;
		        cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
              	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
            }

			current_frequency = freq;

			change_interval(freqs.old, freqs.new);
   		}
   	}
	add_timer(&Check_Freq_timer);
	Check_Freq_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
}

extern int ORI_FREQ_KHZ;
static int __init init_procfs_msg(void)
{
	current_frequency = ORI_FREQ_KHZ;

	printk("\033[35mFunction = %s, Line = %d, current_frequency = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, current_frequency);

  	cpufreq_register_driver(&integrator_driver);
	proc_create("CPU_calibrating", S_IRUSR | S_IWUSR, NULL, &proc_CPU_calibrating_operations);
	proc_create("on_demand_ctl"  , S_IRUSR | S_IWUSR, NULL, &proc_on_demand_handshake_operations);

#if !defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND)
	init_timer(&Check_Freq_timer);
    Check_Freq_timer.data = 1;
    Check_Freq_timer.function = Mdrv_CPU_Freq_Check_callback;
    Check_Freq_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
    add_timer(&Check_Freq_timer);
#endif
	return 0;
}

static int __init CPU_calibrating_init(void)
{
	printk("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__);
	init_procfs_msg();
	return 0;
}

module_init(CPU_calibrating_init);
