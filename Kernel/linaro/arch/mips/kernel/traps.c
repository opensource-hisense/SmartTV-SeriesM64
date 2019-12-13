/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999, 2000, 01, 06 Ralf Baechle
 * Copyright (C) 1995, 1996 Paul M. Antoine
 * Copyright (C) 1998 Ulf Carlsson
 * Copyright (C) 1999 Silicon Graphics, Inc.
 * Kevin D. Kissell, kevink@mips.com and Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2002, 2003, 2004, 2005, 2007  Maciej W. Rozycki
 * Copyright (C) 2000, 2001, 2012 MIPS Technologies, Inc.  All rights reserved.
 */
#include <linux/bug.h>
#include <linux/compiler.h>
#include <linux/context_tracking.h>
#include <linux/kexec.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/kallsyms.h>
#include <linux/bootmem.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <linux/kgdb.h>
#include <linux/kdebug.h>
#include <linux/kprobes.h>
#include <linux/notifier.h>
#include <linux/kdb.h>
#include <linux/irq.h>
#include <linux/perf_event.h>

#include <asm/bootinfo.h>
#include <asm/branch.h>
#include <asm/break.h>
#include <asm/cop2.h>
#include <asm/cpu.h>
#include <asm/dsp.h>
#include <asm/fpu.h>
#include <asm/fpu_emulator.h>
#include <asm/idle.h>
#include <asm/mipsregs.h>
#include <asm/mipsmtregs.h>
#include <asm/module.h>
#include <asm/pgtable.h>
#include <asm/ptrace.h>
#include <asm/sections.h>
#include <asm/tlbdebug.h>
#include <asm/traps.h>
#include <asm/uaccess.h>
#include <asm/watch.h>
#include <asm/mmu_context.h>
#include <asm/types.h>
#include <asm/stacktrace.h>
#include <asm/uasm.h>

#ifdef CONFIG_MP_DEBUG_TOOL_KDEBUG
#include <linux/bootmem.h>
#include <linux/vmalloc.h>
#endif

extern void check_wait(void);
extern asmlinkage void rollback_handle_int(void);
extern asmlinkage void handle_int(void);
extern u32 handle_tlbl[];
extern u32 handle_tlbs[];
extern u32 handle_tlbm[];
extern asmlinkage void handle_adel(void);
extern asmlinkage void handle_ades(void);
extern asmlinkage void handle_ibe(void);
extern asmlinkage void handle_dbe(void);
extern asmlinkage void handle_sys(void);
extern asmlinkage void handle_bp(void);
extern asmlinkage void handle_ri(void);
extern asmlinkage void handle_ri_rdhwr_vivt(void);
extern asmlinkage void handle_ri_rdhwr(void);
extern asmlinkage void handle_cpu(void);
extern asmlinkage void handle_ov(void);
extern asmlinkage void handle_tr(void);
extern asmlinkage void handle_fpe(void);
extern asmlinkage void handle_mdmx(void);
extern asmlinkage void handle_watch(void);
extern asmlinkage void handle_mt(void);
extern asmlinkage void handle_dsp(void);
extern asmlinkage void handle_mcheck(void);
extern asmlinkage void handle_reserved(void);

void (*board_be_init)(void);
int (*board_be_handler)(struct pt_regs *regs, int is_fixup);
void (*board_nmi_handler_setup)(void);
void (*board_ejtag_handler_setup)(void);
void (*board_bind_eic_interrupt)(int irq, int regset);
void (*board_ebase_setup)(void);
void __cpuinitdata(*board_cache_error_setup)(void);

static void show_raw_backtrace(unsigned long reg29)
{
	unsigned long *sp = (unsigned long *)(reg29 & ~3);
	unsigned long addr;

	printk("Call Trace:");
#ifdef CONFIG_KALLSYMS
	printk("\n");
#endif
	while (!kstack_end(sp)) {
		unsigned long __user *p =
			(unsigned long __user *)(unsigned long)sp++;
		if (__get_user(addr, p)) {
			printk(" (Bad stack address)");
			break;
		}
		if (__kernel_text_address(addr))
			print_ip_sym(addr);
	}
	printk("\n");
}

#ifdef CONFIG_KALLSYMS
int raw_show_trace;
static int __init set_raw_show_trace(char *str)
{
	raw_show_trace = 1;
	return 1;
}
__setup("raw_show_trace", set_raw_show_trace);
#endif

static void show_backtrace(struct task_struct *task, const struct pt_regs *regs)
{
	unsigned long sp = regs->regs[29];
	unsigned long ra = regs->regs[31];
	unsigned long pc = regs->cp0_epc;

	if (!task)
		task = current;

	if (raw_show_trace || !__kernel_text_address(pc)) {
		show_raw_backtrace(sp);
		return;
	}
	printk("Call Trace:\n");
	do {
#ifdef CONFIG_MP_DEBUG_TOOL_KDEBUG
		unsigned long where = pc;
		pc = unwind_stack(task, &sp, pc, &ra);
		if (pc <= 0) /* Checking the Reutrning zero on error condition */
			break;
		printk("[<%p>] %pS", (void *) where, (void *) where);
		printk(" from[<%p>] %pS\n", (void *) pc, (void *) pc);
#else
		print_ip_sym(pc);
		pc = unwind_stack(task, &sp, pc, &ra);
#endif
	} while (pc);
	printk("\n");
}

#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
void dump_pid_maps(struct task_struct *task)
{
	struct task_struct *t;
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	struct file *file;
	unsigned long long pgoff = 0;
	unsigned long ino = 0;
	dev_t dev = 0;
	int tpid = 0;
	char path_buf[256];

	printk(KERN_ALERT "-----------------------------------------------------------\n");
	printk(KERN_ALERT "* dump maps on pid (%d)\n", task->pid);
	printk(KERN_ALERT "-----------------------------------------------------------\n");

	if (!down_read_trylock(&task->mm->mmap_sem)) {
		printk(KERN_ALERT "down_read_trylock() failed... do not dump pid maps info\n");
		return;
	}

	vma = task->mm->mmap;
	while (vma) {
		file = vma->vm_file;
		if (file) {
			struct inode *inode = file->f_dentry->d_inode;
			dev = inode->i_sb->s_dev;
			ino = inode->i_ino;
			pgoff = ((loff_t)vma->vm_pgoff) << PAGE_SHIFT;
		} else {
			dev = 0;
			ino = 0;
			pgoff = 0;
		}

		printk(KERN_ALERT "%08lx-%08lx %c%c%c%c %08llx %02x:%02x %-10lu ",
				vma->vm_start,
				vma->vm_end,
				vma->vm_flags & VM_READ ? 'r' : '-',
				vma->vm_flags & VM_WRITE ? 'w' : '-',
				vma->vm_flags & VM_EXEC ? 'x' : '-',
				vma->vm_flags & VM_MAYSHARE ? 's' : 'p',
				pgoff,
				MAJOR(dev), MINOR(dev), ino);

		if (file) {
			char* p = d_path(&(file->f_path),path_buf, 256);
			if (!IS_ERR(p)) printk("%s", p);
		} else {
			const char *name = arch_vma_name(vma);
			mm = vma->vm_mm;
			tpid = 0;

			if (!name) {
				if (mm) {
					if (vma->vm_start <= mm->brk &&
					    vma->vm_end >= mm->start_brk) {
						name = "[heap]";
					} else if (vma->vm_start <= mm->start_stack &&
						   vma->vm_end >= mm->start_stack) {
						name = "[stack]";
					} else {
						t = task;
						do{
							if (vma->vm_start <= t->user_ssp &&
							    vma->vm_end >= t->user_ssp){
								tpid = t->pid;
								name = t->comm;
								break;
							}
						}while_each_thread(task, t);
					}
				} else {
					name = "[vdso]";
				}
			}
			if (name) {
				if (tpid)
					printk("[tstack: %s: %d]", name, tpid);
				else
					printk("%s", name);
			}
		}
		printk( "\n");

		vma = vma->vm_next;
	}
	up_read(&task->mm->mmap_sem);
	printk(KERN_ALERT "-----------------------------------------------------------\n\n");
}
/*
static void dump_mem_kernel(const char *str, unsigned long bottom, unsigned long top)
{
	unsigned long p = bottom & ~31;
	int i;

	printk("%s(0x%08lx to 0x%08lx)\n", str, bottom, top);

	for (p = bottom & ~31; p <= top;) {
		printk("%04lx: ", p & 0xffff);

		for (i = 0; i < 8; i++, p += 4) {

			if (p < bottom || p > top)
				printk("         ");
			else
				printk("%08lx ", *(unsigned long*)p);
		}
		printk ("\n");
	}
}
*/
static void dump_mem(const char *str, unsigned long bottom, unsigned long top)
{
	unsigned long p = bottom & ~31;
	mm_segment_t fs;
	int i;

	/*
	 * We need to switch to kernel mode so that we can use __get_user
	 * to safely read from kernel space.  Note that we now dump the
	 * code first, just in case the backtrace kills us.
	 */
	fs = get_fs();
	set_fs(KERNEL_DS);

	printk(KERN_ALERT "%s(0x%08lx to 0x%08lx)\n", str, bottom, top);

	for (p = bottom & ~31; p <= top;) {
		printk(KERN_ALERT "%04lx: ", p & 0xffff);

		for (i = 0; i < 8; i++, p += 4) {
			unsigned int val;

			if (p < bottom || p > top)
				printk("         ");
			else {
				__get_user(val, (unsigned long *)p);
				printk("%08x ", val);
			}
		}
		printk ("\n");
	}

	set_fs(fs);
}

/*
 * VDLP 4.2.x Chelsea Kernel Patch
 * Show user stack
 *  : assumes that user program uses frame pointer
 *  : TODO : consider context safety
 */
void show_user_stack(struct task_struct *task, struct pt_regs * regs)
{
	struct vm_area_struct *vma;

	vma = find_vma(task->mm, task->user_ssp);
	if (vma) {
		printk(KERN_ALERT
				"task stack info : pid(%d) stack area (0x%08lx ~ 0x%08lx)\n",
				(int)task->pid, vma->vm_start, vma->vm_end );
	}
	else
	{
		printk(KERN_ALERT "pid(%d) : printing user stack failed.\n", (int)task->pid);
		return;
	}

	if( regs->regs[29] < vma->vm_start )
	{
		printk(KERN_ALERT  "pid(%d) : seems stack overflow.\n"
				"  sp(%lx), stack vma (0x%08lx ~ 0x%08lx)\n"
				, (int)task->pid, regs->regs[29], vma->vm_start, vma->vm_end );
		return;
	}

	printk(KERN_ALERT "-----------------------------------------------------------\n");
	printk(KERN_ALERT "* dump user stack\n");
	printk(KERN_ALERT "-----------------------------------------------------------\n");
	dump_mem( "dump user stack", regs->regs[29], task->user_ssp);
	printk(KERN_ALERT "-----------------------------------------------------------\n\n");
}

#ifdef CONFIG_SHOW_EPC_RA_INFO
void show_epc_ra(struct task_struct* task, struct pt_regs * regs)
{
       struct vm_area_struct *epc_vma = NULL;
       struct vm_area_struct *ra_vma = NULL;
       unsigned long addr_epc, addr_ra;

       printk("\n");
       printk("--------------------------------------------------------------------------------------\n");
       printk("EPC, RA MEMINFO\n");
       printk("--------------------------------------------------------------------------------------\n");

       addr_epc = regs->cp0_epc - 0x400;   // for 1024 byte
       addr_ra = regs->regs[31] - 0x800;   // for 2048 byte

       // find vma including epc, ra
       // calculate a print range according to vma
       epc_vma = find_vma( task->mm, regs->cp0_epc);
       if ( epc_vma == NULL )
       {
               printk("No VMA for epc\n");
               return ;
       }
       if( addr_epc < epc_vma->vm_start )
               addr_epc = epc_vma->vm_start;

       ra_vma = find_vma( task->mm, regs->regs[31]);
       if ( ra_vma == NULL )
       {
               printk("No VMA for ra\n");
               return ;
       }
       if( addr_ra < ra_vma->vm_start )
               addr_ra = ra_vma->vm_start;

       // find a duplicated address range case1
       if( (addr_ra<=regs->cp0_epc) && (regs->cp0_epc<regs->regs[31]) )
       {
               addr_ra = regs->cp0_epc + 0x4;
       }
       // find a duplicated address range case2
       else if( (addr_epc<=regs->regs[31]) && (regs->regs[31]<regs->cp0_epc) )
       {
               addr_epc = regs->regs[31] + 0x4;
       }


       // dump
       printk("epc:%lx, ra:%lx\n", regs->cp0_epc, regs->regs[31]);
       printk("--------------------------------------------------------------------------------------\n");
       dump_mem("EPC meminfo ", addr_epc, regs->cp0_epc );
       printk("--------------------------------------------------------------------------------------\n");
       dump_mem("RA meminfo ", addr_ra, regs->regs[31]);
       printk("--------------------------------------------------------------------------------------\n");

       printk("\n");
}

static void show_epc_ra_kernel(const struct pt_regs * regs)
{
       unsigned long addr_epc, addr_ra;
	int valid_pc = 0, valid_lr = 0;
	int valid_pc_mod, valid_lr_mod;
	struct module *mod;

       addr_epc = regs->cp0_epc - 0x400;   // for 1024 byte
       addr_ra = regs->regs[31] - 0x800;   // for 2048 byte

	valid_pc_mod=((regs->cp0_epc >= VMALLOC_START && regs->cp0_epc < VMALLOC_END)
#ifdef MODULE_START
		      || (regs->cp0_epc >= MODULE_START && regs->cp0_epc < MODULE_START)
#endif
		      );
	valid_lr_mod=((regs->regs[31] >= VMALLOC_START && regs->regs[31] < VMALLOC_END)
#ifdef MODULE_START
		      || (regs->regs[31] >= MODULE_START && regs->regs[31] < MODULE_START)
#endif
		      );

	valid_pc = (TASK_SIZE <= regs->cp0_epc && regs->cp0_epc < (unsigned long)high_memory) || valid_pc_mod;
	valid_lr = (TASK_SIZE <= regs->regs[31] && regs->regs[31] < (unsigned long)high_memory) || valid_lr_mod;

	/* Adjust the addr_epc according to the correct module virtual memory range. */
	if(valid_pc) {
		if (addr_epc < TASK_SIZE)
			addr_epc = TASK_SIZE;
		else if (valid_pc_mod) {
			mod = __module_address(regs->cp0_epc);

			if (!within_module_init(addr_epc, mod) &&
			    !within_module_core(addr_epc, mod))
				addr_epc = regs->cp0_epc & PAGE_MASK;
		}
	}

	/* Adjust the addr_ra according to the correct module virtual memory range. */
	if(valid_lr) {
		if (addr_ra < TASK_SIZE)
			addr_ra = TASK_SIZE;
		else if (valid_lr_mod) {
			mod = __module_address(regs->regs[31]);

			if (!within_module_init(addr_ra, mod) &&
			    !within_module_core(addr_ra, mod))
				addr_ra = regs->regs[31] & PAGE_MASK;
		}
	}

       if( valid_pc && valid_lr )
       {
               // find a duplicated address rage case1
               if( (addr_ra<=regs->cp0_epc) && (regs->cp0_epc<regs->regs[31]) )
               {
                       addr_ra = regs->cp0_epc + 0x4;
               }
               // find a duplicated address rage case2
               else if( (addr_epc<=regs->regs[31]) && (regs->regs[31]<regs->cp0_epc) )
               {
                       addr_epc = regs->regs[31] + 0x4;
               }
       }

       printk("--------------------------------------------------------------------------------------\n");
       printk("[VDLP] DISPLAY EPC, RA in KERNEL Level\n");
       printk("epc:%lx, ra:%lx\n", regs->cp0_epc, regs->regs[31]);
       printk("--------------------------------------------------------------------------------------\n");
       if( valid_pc ){
               dump_mem_kernel("EPC meminfo in kernel", addr_epc, regs->cp0_epc );
	}
       else{
               printk("[VDLP] Invalid pc addr\n");
	}
       printk("--------------------------------------------------------------------------------------\n");
       if( valid_lr ){
               dump_mem_kernel("RA meminfo in kernel", addr_ra, regs->regs[31]);
	}
       else{
               printk("[VDLP] Invalid lr addr\n");
	}
       printk("--------------------------------------------------------------------------------------\n");

       printk("\n");
}

#ifdef CONFIG_DUMP_RANGE_BASED_ON_REGISTER
int is_valid_kernel_addr(unsigned long register_value)
{
	if(register_value < PAGE_OFFSET || !virt_addr_valid((void*)register_value)){ //includes checking NULL and user address
		return 0;
	}
	else{
		return 1;
	}
}

void show_register_memory_kernel(struct pt_regs * regs)
{
	unsigned long start_addr_for_printing = 0, end_addr_for_printing = 0;
	int register_num;

	printk("--------------------------------------------------------------------------------------\n");
	printk("REGISTER MEMORY INFO\n");
	printk("--------------------------------------------------------------------------------------\n");

	for(register_num = 0; register_num<sizeof(regs->regs)/sizeof(regs->regs[0]); register_num++) {

		printk("\n\n* REGISTER : r%d\n",register_num);

		start_addr_for_printing = (regs->regs[register_num] & PAGE_MASK) - 0x1000;               //-4kbyte
		if(regs->regs[register_num] >= 0xfffff000){    // if virtual address is 0xffffffff, skip dump address to prevent overflow
                        end_addr_for_printing = 0xffffffff;
               }else{
			end_addr_for_printing = (regs->regs[register_num] & PAGE_MASK) + PAGE_SIZE + 0xfff;}     //+about 8kbyte
		if(!is_valid_kernel_addr(regs->regs[register_num])) {

			printk("# Register value 0x%lx is wrong address.\n", regs->regs[register_num]);
			printk("# We can't do anything.\n");
			printk("# So, we search next register.\n");

			continue;
		}
		if(!is_valid_kernel_addr(start_addr_for_printing)) {

			printk("# 'start_addr_for_printing' is wrong address.\n");
			printk("# So, we use just 'regs->regs[register_num] & PAGE_MASK)'\n");

			start_addr_for_printing = (regs->regs[register_num] & PAGE_MASK);
		}
		if(!is_valid_kernel_addr(end_addr_for_printing)) {

			printk("# 'end_addr_for_printing' is wrong address.\n");
			printk("# So, we use 'PAGE_ALIGN(regs->regs[register_num]) + PAGE_SIZE-1'\n");

			end_addr_for_printing = (regs->regs[register_num] & PAGE_MASK) + PAGE_SIZE-1;
		}

		// dump
		printk("# r%d register :0x%lx, start_addr : 0x%lx, end_addr : 0x%lx\n", register_num, regs->regs[register_num], start_addr_for_printing, end_addr_for_printing);
		printk("--------------------------------------------------------------------------------------\n");
		dump_mem_kernel("meminfo ", start_addr_for_printing, end_addr_for_printing );
		printk("--------------------------------------------------------------------------------------\n");
		printk("\n");
	}
}
#endif
#endif

#ifndef CONFIG_SEPARATE_PRINTK_FROM_USER
#define sep_printk_start
#define sep_printk_end
#else
extern void _sep_printk_start(void);
extern void _sep_printk_end(void);
#define sep_printk_start  _sep_printk_start
#define sep_printk_end _sep_printk_end
#endif

#ifdef CONFIG_RUN_TIMER_DEBUG
extern void show_timer_list(void);
#endif

void show_info_kdebug(struct task_struct *task, struct pt_regs * regs)
{
	static atomic_t prn_once = ATOMIC_INIT(0);

	if(atomic_cmpxchg(&prn_once, 0, 1))
	{
		return;
	}

#ifdef CONFIG_SEPARATE_PRINTK_FROM_USER
	sep_printk_start();
#endif

	console_verbose();      /* BSP patch : enable console while show_info_kdebug */

	preempt_disable();
	/*if(addr)
	{
	        show_pte(task->mm, addr);
	}*/
#ifdef CONFIG_VDLP_VERSION_INFO
        printk(KERN_ALERT"================================================================================\n");
        printk(KERN_ALERT" KERNEL Version : %s\n", DTV_KERNEL_VERSION);
        printk(KERN_ALERT"================================================================================\n");
#endif

#ifdef CONFIG_SHOW_EPC_RA_INFO
        show_epc_ra(task, regs);
#endif
#ifdef CONFIG_RUN_TIMER_DEBUG
		show_timer_list();
#endif

	show_regs(regs);
	dump_pid_maps(task);
	show_user_stack(task, regs);
	preempt_enable();

#ifdef CONFIG_SEPARATE_PRINTK_FROM_USER
	sep_printk_end();
#endif
}
EXPORT_SYMBOL(show_info_kdebug);
#endif /* CONFIG_SHOW_FAULT_TRACE_INFO */

/*
 * This routine abuses get_user()/put_user() to reference pointers
 * with at least a bit of error checking ...
 */
static void show_stacktrace(struct task_struct *task,
	const struct pt_regs *regs)
{
	const int field = 2 * sizeof(unsigned long);
	long stackdata;
	int i;
	unsigned long __user *sp = (unsigned long __user *)regs->regs[29];

	printk("Stack :");
	i = 0;
	while ((unsigned long) sp & (PAGE_SIZE - 1)) {
		if (i && ((i % (64 / field)) == 0))
			printk("\n	 ");
		if (i > 39) {
			printk(" ...");
			break;
		}

		if (__get_user(stackdata, sp++)) {
			printk(" (Bad stack address)");
			break;
		}

		printk(" %0*lx", field, stackdata);
		i++;
	}
	printk("\n");
	show_backtrace(task, regs);
}

void show_stack(struct task_struct *task, unsigned long *sp)
{
	struct pt_regs regs;
	if (sp) {
		regs.regs[29] = (unsigned long)sp;
		regs.regs[31] = 0;
		regs.cp0_epc = 0;
	} else {
		if (task && task != current) {
			regs.regs[29] = task->thread.reg29;
			regs.regs[31] = 0;
			regs.cp0_epc = task->thread.reg31;
#ifdef CONFIG_KGDB_KDB
		} else if (atomic_read(&kgdb_active) != -1 &&
			   kdb_current_regs) {
			memcpy(&regs, kdb_current_regs, sizeof(regs));
#endif /* CONFIG_KGDB_KDB */
		} else {
			prepare_frametrace(&regs);
		}
	}
	show_stacktrace(task, &regs);
}

static void show_code(unsigned int __user *pc)
{
	long i;
	unsigned short __user *pc16 = NULL;

	printk("\nCode:");

	if ((unsigned long)pc & 1)
		pc16 = (unsigned short __user *)((unsigned long)pc & ~1);
	for(i = -3 ; i < 6 ; i++) {
		unsigned int insn;
		if (pc16 ? __get_user(insn, pc16 + i) : __get_user(insn, pc + i)) {
			printk(" (Bad address in epc)\n");
			break;
		}
		printk("%c%0*x%c", (i?' ':'<'), pc16 ? 4 : 8, insn, (i?' ':'>'));
	}
}

static void __show_regs(const struct pt_regs *regs)
{
	const int field = 2 * sizeof(unsigned long);
	unsigned int cause = regs->cp0_cause;
	int i;

	show_regs_print_info(KERN_DEFAULT);

	/*
	 * Saved main processor registers
	 */
	for (i = 0; i < 32; ) {
		if ((i % 4) == 0)
			printk("$%2d   :", i);
		if (i == 0)
			printk(" %0*lx", field, 0UL);
		else if (i == 26 || i == 27)
			printk(" %*s", field, "");
		else
			printk(" %0*lx", field, regs->regs[i]);

		i++;
		if ((i % 4) == 0)
			printk("\n");
	}

#ifdef CONFIG_CPU_HAS_SMARTMIPS
	printk("Acx    : %0*lx\n", field, regs->acx);
#endif
	printk("Hi    : %0*lx\n", field, regs->hi);
	printk("Lo    : %0*lx\n", field, regs->lo);

	/*
	 * Saved cp0 registers
	 */
	printk("epc   : %0*lx %pS\n", field, regs->cp0_epc,
	       (void *) regs->cp0_epc);
	printk("    %s\n", print_tainted());
	printk("ra    : %0*lx %pS\n", field, regs->regs[31],
	       (void *) regs->regs[31]);

	printk("Status: %08x	", (uint32_t) regs->cp0_status);

	if (current_cpu_data.isa_level == MIPS_CPU_ISA_I) {
		if (regs->cp0_status & ST0_KUO)
			printk("KUo ");
		if (regs->cp0_status & ST0_IEO)
			printk("IEo ");
		if (regs->cp0_status & ST0_KUP)
			printk("KUp ");
		if (regs->cp0_status & ST0_IEP)
			printk("IEp ");
		if (regs->cp0_status & ST0_KUC)
			printk("KUc ");
		if (regs->cp0_status & ST0_IEC)
			printk("IEc ");
	} else {
		if (regs->cp0_status & ST0_KX)
			printk("KX ");
		if (regs->cp0_status & ST0_SX)
			printk("SX ");
		if (regs->cp0_status & ST0_UX)
			printk("UX ");
		switch (regs->cp0_status & ST0_KSU) {
		case KSU_USER:
			printk("USER ");
			break;
		case KSU_SUPERVISOR:
			printk("SUPERVISOR ");
			break;
		case KSU_KERNEL:
			printk("KERNEL ");
			break;
		default:
			printk("BAD_MODE ");
			break;
		}
		if (regs->cp0_status & ST0_ERL)
			printk("ERL ");
		if (regs->cp0_status & ST0_EXL)
			printk("EXL ");
		if (regs->cp0_status & ST0_IE)
			printk("IE ");
	}
	printk("\n");

	printk("Cause : %08x\n", cause);

	cause = (cause & CAUSEF_EXCCODE) >> CAUSEB_EXCCODE;
	if (1 <= cause && cause <= 5)
		printk("BadVA : %0*lx\n", field, regs->cp0_badvaddr);

	printk("PrId  : %08x (%s)\n", read_c0_prid(),
	       cpu_name_string());
}

/*
 * FIXME: really the generic show_regs should take a const pointer argument.
 */
void show_regs(struct pt_regs *regs)
{
	__show_regs((struct pt_regs *)regs);
}

void show_registers(struct pt_regs *regs)
{
	const int field = 2 * sizeof(unsigned long);

	__show_regs(regs);
	print_modules();
	printk("Process %s (pid: %d, threadinfo=%p, task=%p, tls=%0*lx)\n",
	       current->comm, current->pid, current_thread_info(), current,
	      field, current_thread_info()->tp_value);
	if (cpu_has_userlocal) {
		unsigned long tls;

		tls = read_c0_userlocal();
		if (tls != current_thread_info()->tp_value)
			printk("*HwTLS: %0*lx\n", field, tls);
	}

	show_stacktrace(current, regs);
	show_code((unsigned int __user *) regs->cp0_epc);
	printk("\n");
}

static int regs_to_trapnr(struct pt_regs *regs)
{
	return (regs->cp0_cause >> 2) & 0x1f;
}

static DEFINE_RAW_SPINLOCK(die_lock);

void __noreturn die(const char *str, struct pt_regs *regs)
{
	static int die_counter;
	int sig = SIGSEGV;
#ifdef CONFIG_MIPS_MT_SMTC
	unsigned long dvpret;
#endif /* CONFIG_MIPS_MT_SMTC */

	oops_enter();

	if (notify_die(DIE_OOPS, str, regs, 0, regs_to_trapnr(regs), SIGSEGV) == NOTIFY_STOP)
		sig = 0;

	console_verbose();
	raw_spin_lock_irq(&die_lock);
#ifdef CONFIG_MIPS_MT_SMTC
	dvpret = dvpe();
#endif /* CONFIG_MIPS_MT_SMTC */
	bust_spinlocks(1);
#ifdef CONFIG_MIPS_MT_SMTC
	mips_mt_regdump(dvpret);
#endif /* CONFIG_MIPS_MT_SMTC */

	printk("%s[#%d]:\n", str, ++die_counter);
	show_registers(regs);
	add_taint(TAINT_DIE, LOCKDEP_NOW_UNRELIABLE);
	raw_spin_unlock_irq(&die_lock);

	oops_exit();

	if (in_interrupt())
		panic("Fatal exception in interrupt");

	if (panic_on_oops) {
		printk(KERN_EMERG "Fatal exception: panic in 5 seconds");
		ssleep(5);
		panic("Fatal exception");
	}

	if (regs && kexec_should_crash(current))
		crash_kexec(regs);

	do_exit(sig);
}

extern struct exception_table_entry __start___dbe_table[];
extern struct exception_table_entry __stop___dbe_table[];

__asm__(
"	.section	__dbe_table, \"a\"\n"
"	.previous			\n");

/* Given an address, look for it in the exception tables. */
static const struct exception_table_entry *search_dbe_tables(unsigned long addr)
{
	const struct exception_table_entry *e;

	e = search_extable(__start___dbe_table, __stop___dbe_table - 1, addr);
	if (!e)
		e = search_module_dbetables(addr);
	return e;
}

asmlinkage void do_be(struct pt_regs *regs)
{
	const int field = 2 * sizeof(unsigned long);
	const struct exception_table_entry *fixup = NULL;
	int data = regs->cp0_cause & 4;
	int action = MIPS_BE_FATAL;
	enum ctx_state prev_state;

	prev_state = exception_enter();
	/* XXX For now.	 Fixme, this searches the wrong table ...  */
	if (data && !user_mode(regs))
		fixup = search_dbe_tables(exception_epc(regs));

	if (fixup)
		action = MIPS_BE_FIXUP;

	if (board_be_handler)
		action = board_be_handler(regs, fixup != NULL);

	switch (action) {
	case MIPS_BE_DISCARD:
		goto out;
	case MIPS_BE_FIXUP:
		if (fixup) {
			regs->cp0_epc = fixup->nextinsn;
			goto out;
		}
		break;
	default:
		break;
	}

	/*
	 * Assume it would be too dangerous to continue ...
	 */
	printk(KERN_ALERT "%s bus error, epc == %0*lx, ra == %0*lx\n",
	       data ? "Data" : "Instruction",
	       field, regs->cp0_epc, field, regs->regs[31]);
	if (notify_die(DIE_OOPS, "bus error", regs, 0, regs_to_trapnr(regs), SIGBUS)
	    == NOTIFY_STOP)
		goto out;

	die_if_kernel("Oops", regs);
	force_sig(SIGBUS, current);

out:
	exception_exit(prev_state);
}

/*
 * ll/sc, rdhwr, sync emulation
 */

#define OPCODE 0xfc000000
#define BASE   0x03e00000
#define RT     0x001f0000
#define OFFSET 0x0000ffff
#define LL     0xc0000000
#define SC     0xe0000000
#define SPEC0  0x00000000
#define SPEC3  0x7c000000
#define RD     0x0000f800
#define FUNC   0x0000003f
#define SYNC   0x0000000f
#define RDHWR  0x0000003b

/*  microMIPS definitions   */
#define MM_POOL32A_FUNC 0xfc00ffff
#define MM_RDHWR        0x00006b3c
#define MM_RS           0x001f0000
#define MM_RT           0x03e00000

/*
 * The ll_bit is cleared by r*_switch.S
 */

unsigned int ll_bit;
struct task_struct *ll_task;

static inline int simulate_ll(struct pt_regs *regs, unsigned int opcode)
{
	unsigned long value, __user *vaddr;
	long offset;

	/*
	 * analyse the ll instruction that just caused a ri exception
	 * and put the referenced address to addr.
	 */

	/* sign extend offset */
	offset = opcode & OFFSET;
	offset <<= 16;
	offset >>= 16;

	vaddr = (unsigned long __user *)
		((unsigned long)(regs->regs[(opcode & BASE) >> 21]) + offset);

	if ((unsigned long)vaddr & 3)
		return SIGBUS;
	if (get_user(value, vaddr))
		return SIGSEGV;

	preempt_disable();

	if (ll_task == NULL || ll_task == current) {
		ll_bit = 1;
	} else {
		ll_bit = 0;
	}
	ll_task = current;

	preempt_enable();

	regs->regs[(opcode & RT) >> 16] = value;

	return 0;
}

static inline int simulate_sc(struct pt_regs *regs, unsigned int opcode)
{
	unsigned long __user *vaddr;
	unsigned long reg;
	long offset;

	/*
	 * analyse the sc instruction that just caused a ri exception
	 * and put the referenced address to addr.
	 */

	/* sign extend offset */
	offset = opcode & OFFSET;
	offset <<= 16;
	offset >>= 16;

	vaddr = (unsigned long __user *)
		((unsigned long)(regs->regs[(opcode & BASE) >> 21]) + offset);
	reg = (opcode & RT) >> 16;

	if ((unsigned long)vaddr & 3)
		return SIGBUS;

	preempt_disable();

	if (ll_bit == 0 || ll_task != current) {
		regs->regs[reg] = 0;
		preempt_enable();
		return 0;
	}

	preempt_enable();

	if (put_user(regs->regs[reg], vaddr))
		return SIGSEGV;

	regs->regs[reg] = 1;

	return 0;
}

/*
 * ll uses the opcode of lwc0 and sc uses the opcode of swc0.  That is both
 * opcodes are supposed to result in coprocessor unusable exceptions if
 * executed on ll/sc-less processors.  That's the theory.  In practice a
 * few processors such as NEC's VR4100 throw reserved instruction exceptions
 * instead, so we're doing the emulation thing in both exception handlers.
 */
static int simulate_llsc(struct pt_regs *regs, unsigned int opcode)
{
	if ((opcode & OPCODE) == LL) {
		perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS,
				1, regs, 0);
		return simulate_ll(regs, opcode);
	}
	if ((opcode & OPCODE) == SC) {
		perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS,
				1, regs, 0);
		return simulate_sc(regs, opcode);
	}

	return -1;			/* Must be something else ... */
}

/*
 * Simulate trapping 'rdhwr' instructions to provide user accessible
 * registers not implemented in hardware.
 */
static int simulate_rdhwr(struct pt_regs *regs, int rd, int rt)
{
	struct thread_info *ti = task_thread_info(current);

	perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS,
			1, regs, 0);
	switch (rd) {
	case 0:		/* CPU number */
		regs->regs[rt] = smp_processor_id();
		return 0;
	case 1:		/* SYNCI length */
		regs->regs[rt] = min(current_cpu_data.dcache.linesz,
				     current_cpu_data.icache.linesz);
		return 0;
	case 2:		/* Read count register */
		regs->regs[rt] = read_c0_count();
		return 0;
	case 3:		/* Count register resolution */
		switch (current_cpu_data.cputype) {
		case CPU_20KC:
		case CPU_25KF:
			regs->regs[rt] = 1;
			break;
		default:
			regs->regs[rt] = 2;
		}
		return 0;
	case 29:
		regs->regs[rt] = ti->tp_value;
		return 0;
	default:
		return -1;
	}
}

static int simulate_rdhwr_normal(struct pt_regs *regs, unsigned int opcode)
{
	if ((opcode & OPCODE) == SPEC3 && (opcode & FUNC) == RDHWR) {
		int rd = (opcode & RD) >> 11;
		int rt = (opcode & RT) >> 16;

		simulate_rdhwr(regs, rd, rt);
		return 0;
	}

	/* Not ours.  */
	return -1;
}

static int simulate_rdhwr_mm(struct pt_regs *regs, unsigned short opcode)
{
	if ((opcode & MM_POOL32A_FUNC) == MM_RDHWR) {
		int rd = (opcode & MM_RS) >> 16;
		int rt = (opcode & MM_RT) >> 21;
		simulate_rdhwr(regs, rd, rt);
		return 0;
	}

	/* Not ours.  */
	return -1;
}

static int simulate_sync(struct pt_regs *regs, unsigned int opcode)
{
	if ((opcode & OPCODE) == SPEC0 && (opcode & FUNC) == SYNC) {
		perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS,
				1, regs, 0);
		return 0;
	}

	return -1;			/* Must be something else ... */
}

asmlinkage void do_ov(struct pt_regs *regs)
{
	enum ctx_state prev_state;
	siginfo_t info;

	prev_state = exception_enter();
	die_if_kernel("Integer overflow", regs);

	info.si_code = FPE_INTOVF;
	info.si_signo = SIGFPE;
	info.si_errno = 0;
	info.si_addr = (void __user *) regs->cp0_epc;
#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))	
	{
		printk(KERN_ALERT "%s() : sending SIGFPE to %s, PID:%d\n", __func__,  current->comm, current->pid);
		show_info_kdebug(current, regs);
	}
#endif

	force_sig_info(SIGFPE, &info, current);
	exception_exit(prev_state);
}

#ifdef CONFIG_MP_PLATFORM_MIPS
static int process_fpemu_return(int sig, void __user *fault_addr, struct pt_regs *regs)
#else
static int process_fpemu_return(int sig, void __user *fault_addr)
#endif // CONFIG_MP_PLATFORM_MIPS

{
	if (sig == SIGSEGV || sig == SIGBUS) {
		struct siginfo si = {0};
		si.si_addr = fault_addr;
		si.si_signo = sig;
		if (sig == SIGSEGV) {
			if (find_vma(current->mm, (unsigned long)fault_addr))
				si.si_code = SEGV_ACCERR;
			else
				si.si_code = SEGV_MAPERR;
		} else {
			si.si_code = BUS_ADRERR;
		}
#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
		printk(KERN_ALERT "%s() : sending SIG:%d to %s, PID:%d\n", __func__, sig, current->comm, current->pid);
		show_info_kdebug(current, regs);
#endif
		force_sig_info(sig, &si, current);
		return 1;
	} else if (sig) {
#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
		printk(KERN_ALERT "%s() : sending SIG:%d to %s, PID:%d\n", __func__, sig, current->comm, current->pid);
		show_info_kdebug(current, regs);
#endif
		force_sig(sig, current);
		return 1;
	} else {
		return 0;
	}
}

/*
 * XXX Delayed fp exceptions when doing a lazy ctx switch XXX
 */
asmlinkage void do_fpe(struct pt_regs *regs, unsigned long fcr31)
{
	enum ctx_state prev_state;
	siginfo_t info = {0};

	prev_state = exception_enter();
	if (notify_die(DIE_FP, "FP exception", regs, 0, regs_to_trapnr(regs), SIGFPE)
	    == NOTIFY_STOP)
		goto out;
	die_if_kernel("FP exception in kernel code", regs);

	if (fcr31 & FPU_CSR_UNI_X) {
		int sig;
		void __user *fault_addr = NULL;

		/*
		 * Unimplemented operation exception.  If we've got the full
		 * software emulator on-board, let's use it...
		 *
		 * Force FPU to dump state into task/thread context.  We're
		 * moving a lot of data here for what is probably a single
		 * instruction, but the alternative is to pre-decode the FP
		 * register operands before invoking the emulator, which seems
		 * a bit extreme for what should be an infrequent event.
		 */
		/* Ensure 'resume' not overwrite saved fp context again. */
		lose_fpu(1);

		/* Run the emulator */
		sig = fpu_emulator_cop1Handler(regs, &current->thread.fpu, 1,
					       &fault_addr);

		/*
		 * We can't allow the emulated instruction to leave any of
		 * the cause bit set in $fcr31.
		 */
		current->thread.fpu.fcr31 &= ~FPU_CSR_ALL_X;

		/* Restore the hardware register state */
		own_fpu(1);	/* Using the FPU again.	 */

		/* If something went wrong, signal */
#ifdef CONFIG_MP_PLATFORM_MIPS
		process_fpemu_return(sig, fault_addr, regs);
#else
		process_fpemu_return(sig, fault_addr);
#endif // CONFIG_MP_PLATFORM_MIPS

		goto out;
	} else if (fcr31 & FPU_CSR_INV_X)
		info.si_code = FPE_FLTINV;
	else if (fcr31 & FPU_CSR_DIV_X)
		info.si_code = FPE_FLTDIV;
	else if (fcr31 & FPU_CSR_OVF_X)
		info.si_code = FPE_FLTOVF;
	else if (fcr31 & FPU_CSR_UDF_X)
		info.si_code = FPE_FLTUND;
	else if (fcr31 & FPU_CSR_INE_X)
		info.si_code = FPE_FLTRES;
	else
		info.si_code = __SI_FAULT;
	info.si_signo = SIGFPE;
	info.si_errno = 0;
	info.si_addr = (void __user *) regs->cp0_epc;
#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
	{
		printk(KERN_ALERT "%s() : sending SIGFPE to %s, PID:%d\n", __func__, current->comm, current->pid);
		show_info_kdebug(current, regs);
	}
#endif
	force_sig_info(SIGFPE, &info, current);

out:
	exception_exit(prev_state);
}

static void do_trap_or_bp(struct pt_regs *regs, unsigned int code,
	const char *str)
{
	siginfo_t info;
	char b[40];

#ifdef CONFIG_KGDB_LOW_LEVEL_TRAP
	if (kgdb_ll_trap(DIE_TRAP, str, regs, code, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
		return;
#endif /* CONFIG_KGDB_LOW_LEVEL_TRAP */

	if (notify_die(DIE_TRAP, str, regs, code, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
		return;

	/*
	 * A short test says that IRIX 5.3 sends SIGTRAP for all trap
	 * insns, even for trap and break codes that indicate arithmetic
	 * failures.  Weird ...
	 * But should we continue the brokenness???  --macro
	 */
	switch (code) {
	case BRK_OVERFLOW:
	case BRK_DIVZERO:
		scnprintf(b, sizeof(b), "%s instruction in kernel code", str);
		die_if_kernel(b, regs);
		if (code == BRK_DIVZERO)
			info.si_code = FPE_INTDIV;
		else
			info.si_code = FPE_INTOVF;
		info.si_signo = SIGFPE;
		info.si_errno = 0;
		info.si_addr = (void __user *) regs->cp0_epc;
#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
		{
			printk(KERN_ALERT "%s() : sending SIGFPE to %s, PID:%d\n", __func__, current->comm, current->pid);
			show_info_kdebug(current, regs);
		}
#endif
		force_sig_info(SIGFPE, &info, current);
		break;
	case BRK_BUG:
		die_if_kernel("Kernel bug detected", regs);
		force_sig(SIGTRAP, current);
		break;
	case BRK_MEMU:
		/*
		 * Address errors may be deliberately induced by the FPU
		 * emulator to retake control of the CPU after executing the
		 * instruction in the delay slot of an emulated branch.
		 *
		 * Terminate if exception was recognized as a delay slot return
		 * otherwise handle as normal.
		 */
		if (do_dsemulret(regs))
			return;

		die_if_kernel("Math emu break/trap", regs);
		force_sig(SIGTRAP, current);
		break;
	default:
		scnprintf(b, sizeof(b), "%s instruction in kernel code", str);
		die_if_kernel(b, regs);
		force_sig(SIGTRAP, current);
	}
}

asmlinkage void do_bp(struct pt_regs *regs)
{
	unsigned int opcode, bcode;
	enum ctx_state prev_state;
	unsigned long epc;
	u16 instr[2];

	prev_state = exception_enter();
	if (get_isa16_mode(regs->cp0_epc)) {
		/* Calculate EPC. */
		epc = exception_epc(regs);
		if (cpu_has_mmips) {
			if ((__get_user(instr[0], (u16 __user *)msk_isa16_mode(epc)) ||
			    (__get_user(instr[1], (u16 __user *)msk_isa16_mode(epc + 2)))))
				goto out_sigsegv;
		    opcode = (instr[0] << 16) | instr[1];
		} else {
		    /* MIPS16e mode */
		    if (__get_user(instr[0], (u16 __user *)msk_isa16_mode(epc)))
				goto out_sigsegv;
		    bcode = (instr[0] >> 6) & 0x3f;
		    do_trap_or_bp(regs, bcode, "Break");
		    goto out;
		}
	} else {
		if (__get_user(opcode, (unsigned int __user *) exception_epc(regs)))
			goto out_sigsegv;
	}

	/*
	 * There is the ancient bug in the MIPS assemblers that the break
	 * code starts left to bit 16 instead to bit 6 in the opcode.
	 * Gas is bug-compatible, but not always, grrr...
	 * We handle both cases with a simple heuristics.  --macro
	 */
	bcode = ((opcode >> 6) & ((1 << 20) - 1));
	if (bcode >= (1 << 10))
		bcode >>= 10;

	/*
	 * notify the kprobe handlers, if instruction is likely to
	 * pertain to them.
	 */
	switch (bcode) {
	case BRK_KPROBE_BP:
		if (notify_die(DIE_BREAK, "debug", regs, bcode, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
			goto out;
		else
			break;
	case BRK_KPROBE_SSTEPBP:
		if (notify_die(DIE_SSTEPBP, "single_step", regs, bcode, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
			goto out;
		else
			break;
	default:
		break;
	}

	do_trap_or_bp(regs, bcode, "Break");

out:
	exception_exit(prev_state);
	return;

out_sigsegv:
	force_sig(SIGSEGV, current);
	goto out;
}

asmlinkage void do_tr(struct pt_regs *regs)
{
	u32 opcode, tcode = 0;
	enum ctx_state prev_state;
	u16 instr[2];
	unsigned long epc = msk_isa16_mode(exception_epc(regs));

	prev_state = exception_enter();
	if (get_isa16_mode(regs->cp0_epc)) {
		if (__get_user(instr[0], (u16 __user *)(epc + 0)) ||
		    __get_user(instr[1], (u16 __user *)(epc + 2)))
			goto out_sigsegv;
		opcode = (instr[0] << 16) | instr[1];
		/* Immediate versions don't provide a code.  */
		if (!(opcode & OPCODE))
			tcode = (opcode >> 12) & ((1 << 4) - 1);
	} else {
		if (__get_user(opcode, (u32 __user *)epc))
			goto out_sigsegv;
		/* Immediate versions don't provide a code.  */
		if (!(opcode & OPCODE))
			tcode = (opcode >> 6) & ((1 << 10) - 1);
	}

	do_trap_or_bp(regs, tcode, "Trap");

out:
	exception_exit(prev_state);
	return;

out_sigsegv:
	force_sig(SIGSEGV, current);
	goto out;
}

asmlinkage void do_ri(struct pt_regs *regs)
{
	unsigned int __user *epc = (unsigned int __user *)exception_epc(regs);
	unsigned long old_epc = regs->cp0_epc;
	unsigned long old31 = regs->regs[31];
	enum ctx_state prev_state;
	unsigned int opcode = 0;
	int status = -1;

	prev_state = exception_enter();
	if (notify_die(DIE_RI, "RI Fault", regs, 0, regs_to_trapnr(regs), SIGILL)
	    == NOTIFY_STOP)
		goto out;

	die_if_kernel("Reserved instruction in kernel code", regs);

	if (unlikely(compute_return_epc(regs) < 0))
		goto out;

	if (get_isa16_mode(regs->cp0_epc)) {
		unsigned short mmop[2] = { 0 };

		if (unlikely(get_user(mmop[0], epc) < 0))
			status = SIGSEGV;
		if (unlikely(get_user(mmop[1], epc) < 0))
			status = SIGSEGV;
		opcode = (mmop[0] << 16) | mmop[1];

		if (status < 0)
			status = simulate_rdhwr_mm(regs, opcode);
	} else {
		if (unlikely(get_user(opcode, epc) < 0))
			status = SIGSEGV;

		if (!cpu_has_llsc && status < 0)
			status = simulate_llsc(regs, opcode);

		if (status < 0)
			status = simulate_rdhwr_normal(regs, opcode);

		if (status < 0)
			status = simulate_sync(regs, opcode);
	}

	if (status < 0)
		status = SIGILL;

	if (unlikely(status > 0)) {
		regs->cp0_epc = old_epc;		/* Undo skip-over.  */
		regs->regs[31] = old31;
#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
		{
			if(status == SIGILL) {
				printk(KERN_ALERT "%s() : sending SIGILL to %s, PID:%d\n", __func__, current->comm, current->pid);
			} else {
				printk(KERN_ALERT "%s() : sending SIGNAL(%d) to %s, PID:%d\n"
						, __func__, status
						, current->comm, current->pid);
			}
			show_info_kdebug(current, regs);
		}
#endif
		force_sig(status, current);
	}

out:
	exception_exit(prev_state);
}

/*
 * MIPS MT processors may have fewer FPU contexts than CPU threads. If we've
 * emulated more than some threshold number of instructions, force migration to
 * a "CPU" that has FP support.
 */
static void mt_ase_fp_affinity(void)
{
#ifdef CONFIG_MIPS_MT_FPAFF
	if (mt_fpemul_threshold > 0 &&
	     ((current->thread.emulated_fp++ > mt_fpemul_threshold))) {
		/*
		 * If there's no FPU present, or if the application has already
		 * restricted the allowed set to exclude any CPUs with FPUs,
		 * we'll skip the procedure.
		 */
		if (cpus_intersects(current->cpus_allowed, mt_fpu_cpumask)) {
			cpumask_t tmask;

			current->thread.user_cpus_allowed
				= current->cpus_allowed;
			cpus_and(tmask, current->cpus_allowed,
				mt_fpu_cpumask);
			set_cpus_allowed_ptr(current, &tmask);
			set_thread_flag(TIF_FPUBOUND);
		}
	}
#endif /* CONFIG_MIPS_MT_FPAFF */
}

/*
 * No lock; only written during early bootup by CPU 0.
 */
static RAW_NOTIFIER_HEAD(cu2_chain);

int __ref register_cu2_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&cu2_chain, nb);
}

int cu2_notifier_call_chain(unsigned long val, void *v)
{
	return raw_notifier_call_chain(&cu2_chain, val, v);
}

static int default_cu2_call(struct notifier_block *nfb, unsigned long action,
	void *data)
{
	struct pt_regs *regs = data;

	switch (action) {
	default:
		die_if_kernel("Unhandled kernel unaligned access or invalid "
			      "instruction", regs);
		/* Fall through	 */

	case CU2_EXCEPTION:
		force_sig(SIGILL, current);
	}

	return NOTIFY_OK;
}

asmlinkage void do_cpu(struct pt_regs *regs)
{
	enum ctx_state prev_state;
	unsigned int __user *epc;
	unsigned long old_epc, old31;
	unsigned int opcode;
	unsigned int cpid;
	int status;
	unsigned long __maybe_unused flags;

	prev_state = exception_enter();
	die_if_kernel("do_cpu invoked from kernel context!", regs);

	cpid = (regs->cp0_cause >> CAUSEB_CE) & 3;

	switch (cpid) {
	case 0:
		epc = (unsigned int __user *)exception_epc(regs);
		old_epc = regs->cp0_epc;
		old31 = regs->regs[31];
		opcode = 0;
		status = -1;

		if (unlikely(compute_return_epc(regs) < 0))
			goto out;

		if (get_isa16_mode(regs->cp0_epc)) {
			unsigned short mmop[2] = { 0 };

			if (unlikely(get_user(mmop[0], epc) < 0))
				status = SIGSEGV;
			if (unlikely(get_user(mmop[1], epc) < 0))
				status = SIGSEGV;
			opcode = (mmop[0] << 16) | mmop[1];

			if (status < 0)
				status = simulate_rdhwr_mm(regs, opcode);
		} else {
			if (unlikely(get_user(opcode, epc) < 0))
				status = SIGSEGV;

			if (!cpu_has_llsc && status < 0)
				status = simulate_llsc(regs, opcode);

			if (status < 0)
				status = simulate_rdhwr_normal(regs, opcode);
		}

		if (status < 0)
			status = SIGILL;

		if (unlikely(status > 0)) {
			regs->cp0_epc = old_epc;	/* Undo skip-over.  */
			regs->regs[31] = old31;
			force_sig(status, current);
		}

		goto out;

	case 3:
		/*
		 * Old (MIPS I and MIPS II) processors will set this code
		 * for COP1X opcode instructions that replaced the original
		 * COP3 space.	We don't limit COP1 space instructions in
		 * the emulator according to the CPU ISA, so we want to
		 * treat COP1X instructions consistently regardless of which
		 * code the CPU chose.	Therefore we redirect this trap to
		 * the FP emulator too.
		 *
		 * Then some newer FPU-less processors use this code
		 * erroneously too, so they are covered by this choice
		 * as well.
		 */
		if (raw_cpu_has_fpu)
			break;
		/* Fall through.  */

	case 1:
		if (used_math())	/* Using the FPU again.	 */
			own_fpu(1);
		else {			/* First time FPU user.	 */
			init_fpu();
			set_used_math();
		}

		if (!raw_cpu_has_fpu) {
			int sig;
			void __user *fault_addr = NULL;
			sig = fpu_emulator_cop1Handler(regs,
						       &current->thread.fpu,
						       0, &fault_addr);

#ifdef CONFIG_MP_PLATFORM_MIPS
			if (!process_fpemu_return(sig, fault_addr, regs))
#else
			if (!process_fpemu_return(sig, fault_addr))
#endif  // CONFIG_MP_PLATFORM_MIPS
				mt_ase_fp_affinity();
		}

		goto out;

	case 2:
		raw_notifier_call_chain(&cu2_chain, CU2_EXCEPTION, regs);
		goto out;
	}

#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
	{
		printk(KERN_ALERT "%s() : sending SIGILL to %s, PID:%d\n", __func__, current->comm, current->pid);
		show_info_kdebug(current, regs);
	}
#endif
	force_sig(SIGILL, current);

out:
	exception_exit(prev_state);
}

asmlinkage void do_mdmx(struct pt_regs *regs)
{

	enum ctx_state prev_state;
#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
	{
		printk(KERN_ALERT "%s() : sending SIGILL to %s, PID:%d\n", __func__, current->comm, current->pid);
		show_info_kdebug(current, regs);
	}
#endif
	prev_state = exception_enter();
	force_sig(SIGILL, current);
	exception_exit(prev_state);
}

/*
 * Called with interrupts disabled.
 */
asmlinkage void do_watch(struct pt_regs *regs)
{
	enum ctx_state prev_state;
	u32 cause;

	prev_state = exception_enter();
	/*
	 * Clear WP (bit 22) bit of cause register so we don't loop
	 * forever.
	 */
	cause = read_c0_cause();
	cause &= ~(1 << 22);
	write_c0_cause(cause);

	/*
	 * If the current thread has the watch registers loaded, save
	 * their values and send SIGTRAP.  Otherwise another thread
	 * left the registers set, clear them and continue.
	 */
	if (test_tsk_thread_flag(current, TIF_LOAD_WATCH)) {
		mips_read_watch_registers();
		local_irq_enable();
		force_sig(SIGTRAP, current);
	} else {
		mips_clear_watch_registers();
		local_irq_enable();
	}
	exception_exit(prev_state);
}

asmlinkage void do_mcheck(struct pt_regs *regs)
{
	const int field = 2 * sizeof(unsigned long);
	int multi_match = regs->cp0_status & ST0_TS;
	enum ctx_state prev_state;

	prev_state = exception_enter();
	show_regs(regs);

	if (multi_match) {
		printk("Index	: %0x\n", read_c0_index());
		printk("Pagemask: %0x\n", read_c0_pagemask());
		printk("EntryHi : %0*lx\n", field, read_c0_entryhi());
		printk("EntryLo0: %0*lx\n", field, read_c0_entrylo0());
		printk("EntryLo1: %0*lx\n", field, read_c0_entrylo1());
		printk("\n");
		dump_tlb_all();
	}

	show_code((unsigned int __user *) regs->cp0_epc);

	/*
	 * Some chips may have other causes of machine check (e.g. SB1
	 * graduation timer)
	 */
	panic("Caught Machine Check exception - %scaused by multiple "
	      "matching entries in the TLB.",
	      (multi_match) ? "" : "not ");
	exception_exit(prev_state);
}

asmlinkage void do_mt(struct pt_regs *regs)
{
	int subcode;

	subcode = (read_vpe_c0_vpecontrol() & VPECONTROL_EXCPT)
			>> VPECONTROL_EXCPT_SHIFT;
	switch (subcode) {
	case 0:
		printk(KERN_DEBUG "Thread Underflow\n");
		break;
	case 1:
		printk(KERN_DEBUG "Thread Overflow\n");
		break;
	case 2:
		printk(KERN_DEBUG "Invalid YIELD Qualifier\n");
		break;
	case 3:
		printk(KERN_DEBUG "Gating Storage Exception\n");
		break;
	case 4:
		printk(KERN_DEBUG "YIELD Scheduler Exception\n");
		break;
	case 5:
		printk(KERN_DEBUG "Gating Storage Scheduler Exception\n");
		break;
	default:
		printk(KERN_DEBUG "*** UNKNOWN THREAD EXCEPTION %d ***\n",
			subcode);
		break;
	}
	die_if_kernel("MIPS MT Thread exception in kernel", regs);

#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
	{
		printk(KERN_ALERT "%s() : sending SIGILL to %s, PID:%d\n", __func__, current->comm, current->pid);
		show_info_kdebug(current, regs);
	}
#endif

	force_sig(SIGILL, current);
}


asmlinkage void do_dsp(struct pt_regs *regs)
{
	if (cpu_has_dsp)
		panic("Unexpected DSP exception");

#if ((CONFIG_MP_PLATFORM_MIPS == 1) && defined (CONFIG_SHOW_FAULT_TRACE_INFO))
	{
		printk(KERN_ALERT "%s() : sending SIGILL to %s, PID:%d\n", __func__, current->comm, current->pid);
		show_info_kdebug(current, regs);
	}
#endif

	force_sig(SIGILL, current);
}

asmlinkage void do_reserved(struct pt_regs *regs)
{
	/*
	 * Game over - no way to handle this if it ever occurs.	 Most probably
	 * caused by a new unknown cpu type or after another deadly
	 * hard/software error.
	 */
	show_regs(regs);
	panic("Caught reserved exception %ld - should not happen.",
	      (regs->cp0_cause & 0x7f) >> 2);
}

static int __initdata l1parity = 1;
static int __init nol1parity(char *s)
{
	l1parity = 0;
	return 1;
}
__setup("nol1par", nol1parity);
static int __initdata l2parity = 1;
static int __init nol2parity(char *s)
{
	l2parity = 0;
	return 1;
}
__setup("nol2par", nol2parity);

/*
 * Some MIPS CPUs can enable/disable for cache parity detection, but do
 * it different ways.
 */
static inline void parity_protection_init(void)
{
	switch (current_cpu_type()) {
	case CPU_24K:
	case CPU_34K:
	case CPU_74K:
	case CPU_1004K:
		{
#define ERRCTL_PE	0x80000000
#define ERRCTL_L2P	0x00800000
			unsigned long errctl;
			unsigned int l1parity_present, l2parity_present;

			errctl = read_c0_ecc();
			errctl &= ~(ERRCTL_PE|ERRCTL_L2P);

			/* probe L1 parity support */
			write_c0_ecc(errctl | ERRCTL_PE);
			back_to_back_c0_hazard();
			l1parity_present = (read_c0_ecc() & ERRCTL_PE);

			/* probe L2 parity support */
			write_c0_ecc(errctl|ERRCTL_L2P);
			back_to_back_c0_hazard();
			l2parity_present = (read_c0_ecc() & ERRCTL_L2P);

			if (l1parity_present && l2parity_present) {
				if (l1parity)
					errctl |= ERRCTL_PE;
				if (l1parity ^ l2parity)
					errctl |= ERRCTL_L2P;
			} else if (l1parity_present) {
				if (l1parity)
					errctl |= ERRCTL_PE;
			} else if (l2parity_present) {
				if (l2parity)
					errctl |= ERRCTL_L2P;
			} else {
				/* No parity available */
			}

			printk(KERN_INFO "Writing ErrCtl register=%08lx\n", errctl);

			write_c0_ecc(errctl);
			back_to_back_c0_hazard();
			errctl = read_c0_ecc();
			printk(KERN_INFO "Readback ErrCtl register=%08lx\n", errctl);

			if (l1parity_present)
				printk(KERN_INFO "Cache parity protection %sabled\n",
				       (errctl & ERRCTL_PE) ? "en" : "dis");

			if (l2parity_present) {
				if (l1parity_present && l1parity)
					errctl ^= ERRCTL_L2P;
				printk(KERN_INFO "L2 cache parity protection %sabled\n",
				       (errctl & ERRCTL_L2P) ? "en" : "dis");
			}
		}
		break;

	case CPU_5KC:
	case CPU_5KE:
	case CPU_LOONGSON1:
		write_c0_ecc(0x80000000);
		back_to_back_c0_hazard();
		/* Set the PE bit (bit 31) in the c0_errctl register. */
		printk(KERN_INFO "Cache parity protection %sabled\n",
		       (read_c0_ecc() & 0x80000000) ? "en" : "dis");
		break;
	case CPU_20KC:
	case CPU_25KF:
		/* Clear the DE bit (bit 16) in the c0_status register. */
		printk(KERN_INFO "Enable cache parity protection for "
		       "MIPS 20KC/25KF CPUs.\n");
		clear_c0_status(ST0_DE);
		break;
	default:
		break;
	}
}

asmlinkage void cache_parity_error(void)
{
	const int field = 2 * sizeof(unsigned long);
	unsigned int reg_val;

	/* For the moment, report the problem and hang. */
	printk("Cache error exception:\n");
	printk("cp0_errorepc == %0*lx\n", field, read_c0_errorepc());
	reg_val = read_c0_cacheerr();
	printk("c0_cacheerr == %08x\n", reg_val);

	printk("Decoded c0_cacheerr: %s cache fault in %s reference.\n",
	       reg_val & (1<<30) ? "secondary" : "primary",
	       reg_val & (1<<31) ? "data" : "insn");
	printk("Error bits: %s%s%s%s%s%s%s\n",
	       reg_val & (1<<29) ? "ED " : "",
	       reg_val & (1<<28) ? "ET " : "",
	       reg_val & (1<<26) ? "EE " : "",
	       reg_val & (1<<25) ? "EB " : "",
	       reg_val & (1<<24) ? "EI " : "",
	       reg_val & (1<<23) ? "E1 " : "",
	       reg_val & (1<<22) ? "E0 " : "");
	printk("IDX: 0x%08x\n", reg_val & ((1<<22)-1));

#if defined(CONFIG_CPU_MIPS32) || defined(CONFIG_CPU_MIPS64)
	if (reg_val & (1<<22))
		printk("DErrAddr0: 0x%0*lx\n", field, read_c0_derraddr0());

	if (reg_val & (1<<23))
		printk("DErrAddr1: 0x%0*lx\n", field, read_c0_derraddr1());
#endif

	panic("Can't handle the cache error!");
}

/*
 * SDBBP EJTAG debug exception handler.
 * We skip the instruction and return to the next instruction.
 */
void ejtag_exception_handler(struct pt_regs *regs)
{
	const int field = 2 * sizeof(unsigned long);
	unsigned long depc, old_epc, old_ra;
	unsigned int debug;

	printk(KERN_DEBUG "SDBBP EJTAG debug exception - not handled yet, just ignored!\n");
	depc = read_c0_depc();
	debug = read_c0_debug();
	printk(KERN_DEBUG "c0_depc = %0*lx, DEBUG = %08x\n", field, depc, debug);
	if (debug & 0x80000000) {
		/*
		 * In branch delay slot.
		 * We cheat a little bit here and use EPC to calculate the
		 * debug return address (DEPC). EPC is restored after the
		 * calculation.
		 */
		old_epc = regs->cp0_epc;
		old_ra = regs->regs[31];
		regs->cp0_epc = depc;
		compute_return_epc(regs);
		depc = regs->cp0_epc;
		regs->cp0_epc = old_epc;
		regs->regs[31] = old_ra;
	} else
		depc += 4;
	write_c0_depc(depc);

#if 0
	printk(KERN_DEBUG "\n\n----- Enable EJTAG single stepping ----\n\n");
	write_c0_debug(debug | 0x100);
#endif
}

/*
 * NMI exception handler.
 * No lock; only written during early bootup by CPU 0.
 */
static RAW_NOTIFIER_HEAD(nmi_chain);

int register_nmi_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&nmi_chain, nb);
}

void __noreturn nmi_exception_handler(struct pt_regs *regs)
{
	raw_notifier_call_chain(&nmi_chain, 0, regs);
	bust_spinlocks(1);
	printk("NMI taken!!!!\n");
	die("NMI", regs);
}

#define VECTORSPACING 0x100	/* for EI/VI mode */

unsigned long ebase;
unsigned long exception_handlers[32];
unsigned long vi_handlers[64];

void __init *set_except_vector(int n, void *addr)
{
	unsigned long handler = (unsigned long) addr;
	unsigned long old_handler;

#ifdef CONFIG_CPU_MICROMIPS
	/*
	 * Only the TLB handlers are cache aligned with an even
	 * address. All other handlers are on an odd address and
	 * require no modification. Otherwise, MIPS32 mode will
	 * be entered when handling any TLB exceptions. That
	 * would be bad...since we must stay in microMIPS mode.
	 */
	if (!(handler & 0x1))
		handler |= 1;
#endif
	old_handler = xchg(&exception_handlers[n], handler);

	if (n == 0 && cpu_has_divec) {
#ifdef CONFIG_CPU_MICROMIPS
		unsigned long jump_mask = ~((1 << 27) - 1);
#else
		unsigned long jump_mask = ~((1 << 28) - 1);
#endif
		u32 *buf = (u32 *)(ebase + 0x200);
		unsigned int k0 = 26;
		if ((handler & jump_mask) == ((ebase + 0x200) & jump_mask)) {
			uasm_i_j(&buf, handler & ~jump_mask);
			uasm_i_nop(&buf);
		} else {
			UASM_i_LA(&buf, k0, handler);
			uasm_i_jr(&buf, k0);
			uasm_i_nop(&buf);
		}
		local_flush_icache_range(ebase + 0x200, (unsigned long)buf);
	}
	return (void *)old_handler;
}

static void do_default_vi(void)
{
	show_regs(get_irq_regs());
	panic("Caught unexpected vectored interrupt.");
}

static void *set_vi_srs_handler(int n, vi_handler_t addr, int srs)
{
	unsigned long handler;
	unsigned long old_handler = vi_handlers[n];
	int srssets = current_cpu_data.srsets;
	u16 *h;
	unsigned char *b;

	BUG_ON(!cpu_has_veic && !cpu_has_vint);
	BUG_ON((n < 0) && (n > 9));

	if (addr == NULL) {
		handler = (unsigned long) do_default_vi;
		srs = 0;
	} else
		handler = (unsigned long) addr;
	vi_handlers[n] = handler;

	b = (unsigned char *)(ebase + 0x200 + n*VECTORSPACING);

	if (srs >= srssets)
		panic("Shadow register set %d not supported", srs);

	if (cpu_has_veic) {
		if (board_bind_eic_interrupt)
			board_bind_eic_interrupt(n, srs);
	} else if (cpu_has_vint) {
		/* SRSMap is only defined if shadow sets are implemented */
		if (srssets > 1)
			change_c0_srsmap(0xf << n*4, srs << n*4);
	}

	if (srs == 0) {
		/*
		 * If no shadow set is selected then use the default handler
		 * that does normal register saving and standard interrupt exit
		 */
		extern char except_vec_vi, except_vec_vi_lui;
		extern char except_vec_vi_ori, except_vec_vi_end;
		extern char rollback_except_vec_vi;
		char *vec_start = using_rollback_handler() ?
			&rollback_except_vec_vi : &except_vec_vi;
#ifdef CONFIG_MIPS_MT_SMTC
		/*
		 * We need to provide the SMTC vectored interrupt handler
		 * not only with the address of the handler, but with the
		 * Status.IM bit to be masked before going there.
		 */
		extern char except_vec_vi_mori;
#if defined(CONFIG_CPU_MICROMIPS) || defined(CONFIG_CPU_BIG_ENDIAN)
		const int mori_offset = &except_vec_vi_mori - vec_start + 2;
#else
		const int mori_offset = &except_vec_vi_mori - vec_start;
#endif
#endif /* CONFIG_MIPS_MT_SMTC */
#if defined(CONFIG_CPU_MICROMIPS) || defined(CONFIG_CPU_BIG_ENDIAN)
		const int lui_offset = &except_vec_vi_lui - vec_start + 2;
		const int ori_offset = &except_vec_vi_ori - vec_start + 2;
#else
		const int lui_offset = &except_vec_vi_lui - vec_start;
		const int ori_offset = &except_vec_vi_ori - vec_start;
#endif
		const int handler_len = &except_vec_vi_end - vec_start;

		if (handler_len > VECTORSPACING) {
			/*
			 * Sigh... panicing won't help as the console
			 * is probably not configured :(
			 */
			panic("VECTORSPACING too small");
		}

		set_handler(((unsigned long)b - ebase), vec_start,
#ifdef CONFIG_CPU_MICROMIPS
				(handler_len - 1));
#else
				handler_len);
#endif
#ifdef CONFIG_MIPS_MT_SMTC
		BUG_ON(n > 7);	/* Vector index %d exceeds SMTC maximum. */

		h = (u16 *)(b + mori_offset);
		*h = (0x100 << n);
#endif /* CONFIG_MIPS_MT_SMTC */
		h = (u16 *)(b + lui_offset);
		*h = (handler >> 16) & 0xffff;
		h = (u16 *)(b + ori_offset);
		*h = (handler & 0xffff);
		local_flush_icache_range((unsigned long)b,
					 (unsigned long)(b+handler_len));
	}
	else {
		/*
		 * In other cases jump directly to the interrupt handler. It
		 * is the handler's responsibility to save registers if required
		 * (eg hi/lo) and return from the exception using "eret".
		 */
		u32 insn;

		h = (u16 *)b;
		/* j handler */
#ifdef CONFIG_CPU_MICROMIPS
		insn = 0xd4000000 | (((u32)handler & 0x07ffffff) >> 1);
#else
		insn = 0x08000000 | (((u32)handler & 0x0fffffff) >> 2);
#endif
		h[0] = (insn >> 16) & 0xffff;
		h[1] = insn & 0xffff;
		h[2] = 0;
		h[3] = 0;
		local_flush_icache_range((unsigned long)b,
					 (unsigned long)(b+8));
	}

	return (void *)old_handler;
}

void *set_vi_handler(int n, vi_handler_t addr)
{
	return set_vi_srs_handler(n, addr, 0);
}

extern void tlb_init(void);

/*
 * Timer interrupt
 */
int cp0_compare_irq;
EXPORT_SYMBOL_GPL(cp0_compare_irq);
int cp0_compare_irq_shift;

/*
 * Performance counter IRQ or -1 if shared with timer
 */
int cp0_perfcount_irq;
EXPORT_SYMBOL_GPL(cp0_perfcount_irq);

static int __cpuinitdata noulri;

static int __init ulri_disable(char *s)
{
	pr_info("Disabling ulri\n");
	noulri = 1;

	return 1;
}
__setup("noulri", ulri_disable);

void __cpuinit per_cpu_trap_init(bool is_boot_cpu)
{
	unsigned int cpu = smp_processor_id();
	unsigned int status_set = ST0_CU0;
	unsigned int hwrena = cpu_hwrena_impl_bits;
#ifdef CONFIG_MIPS_MT_SMTC
	int secondaryTC = 0;
	int bootTC = (cpu == 0);

	/*
	 * Only do per_cpu_trap_init() for first TC of Each VPE.
	 * Note that this hack assumes that the SMTC init code
	 * assigns TCs consecutively and in ascending order.
	 */

	if (((read_c0_tcbind() & TCBIND_CURTC) != 0) &&
	    ((read_c0_tcbind() & TCBIND_CURVPE) == cpu_data[cpu - 1].vpe_id))
		secondaryTC = 1;
#endif /* CONFIG_MIPS_MT_SMTC */

	/*
	 * Disable coprocessors and select 32-bit or 64-bit addressing
	 * and the 16/32 or 32/32 FPR register model.  Reset the BEV
	 * flag that some firmware may have left set and the TS bit (for
	 * IP27).  Set XX for ISA IV code to work.
	 */
#ifdef CONFIG_64BIT
	status_set |= ST0_FR|ST0_KX|ST0_SX|ST0_UX;
#endif
	if (current_cpu_data.isa_level & MIPS_CPU_ISA_IV)
		status_set |= ST0_XX;
	if (cpu_has_dsp)
		status_set |= ST0_MX;

	change_c0_status(ST0_CU|ST0_MX|ST0_RE|ST0_FR|ST0_BEV|ST0_TS|ST0_KX|ST0_SX|ST0_UX,
			 status_set);

	if (cpu_has_mips_r2)
		hwrena |= 0x0000000f;

	if (!noulri && cpu_has_userlocal)
		hwrena |= (1 << 29);

	if (hwrena)
		write_c0_hwrena(hwrena);

#ifdef CONFIG_MIPS_MT_SMTC
	if (!secondaryTC) {
#endif /* CONFIG_MIPS_MT_SMTC */

	if (cpu_has_veic || cpu_has_vint) {
		unsigned long sr = set_c0_status(ST0_BEV);
		write_c0_ebase(ebase);
		write_c0_status(sr);
		/* Setting vector spacing enables EI/VI mode  */
		change_c0_intctl(0x3e0, VECTORSPACING);
#ifdef CONFIG_MP_PLATFORM_MIPS_EBASE
	} else if (cpu_has_mips_r2) {
                write_c0_ebase(ebase);
#endif /* CONFIG_MP_PLATFORM_MIPS_EBASE */
	}

	if (cpu_has_divec) {
		if (cpu_has_mipsmt) {
			unsigned int vpflags = dvpe();
			set_c0_cause(CAUSEF_IV);
			evpe(vpflags);
		} else
			set_c0_cause(CAUSEF_IV);
	}

	/*
	 * Before R2 both interrupt numbers were fixed to 7, so on R2 only:
	 *
	 *  o read IntCtl.IPTI to determine the timer interrupt
	 *  o read IntCtl.IPPCI to determine the performance counter interrupt
	 */
	if (cpu_has_mips_r2) {
		cp0_compare_irq_shift = CAUSEB_TI - CAUSEB_IP;
		cp0_compare_irq = (read_c0_intctl() >> INTCTLB_IPTI) & 7;
		cp0_perfcount_irq = (read_c0_intctl() >> INTCTLB_IPPCI) & 7;
		if (cp0_perfcount_irq == cp0_compare_irq)
			cp0_perfcount_irq = -1;
	} else {
		cp0_compare_irq = CP0_LEGACY_COMPARE_IRQ;
		cp0_compare_irq_shift = CP0_LEGACY_PERFCNT_IRQ;
		cp0_perfcount_irq = -1;
	}

#ifdef CONFIG_MIPS_MT_SMTC
	}
#endif /* CONFIG_MIPS_MT_SMTC */

	if (!cpu_data[cpu].asid_cache)
		cpu_data[cpu].asid_cache = ASID_FIRST_VERSION;

	atomic_inc(&init_mm.mm_count);
	current->active_mm = &init_mm;
	BUG_ON(current->mm);
	enter_lazy_tlb(&init_mm, current);

#ifdef CONFIG_MIPS_MT_SMTC
	if (bootTC) {
#endif /* CONFIG_MIPS_MT_SMTC */
		/* Boot CPU's cache setup in setup_arch(). */
		if (!is_boot_cpu)
			cpu_cache_init();
		tlb_init();
#ifdef CONFIG_MIPS_MT_SMTC
	} else if (!secondaryTC) {
		/*
		 * First TC in non-boot VPE must do subset of tlb_init()
		 * for MMU countrol registers.
		 */
		write_c0_pagemask(PM_DEFAULT_MASK);
		write_c0_wired(0);
	}
#endif /* CONFIG_MIPS_MT_SMTC */
	TLBMISS_HANDLER_SETUP();
}

/* Install CPU exception handler */
void __cpuinit set_handler(unsigned long offset, void *addr, unsigned long size)
{
#ifdef CONFIG_CPU_MICROMIPS
	memcpy((void *)(ebase + offset), ((unsigned char *)addr - 1), size);
#else
	memcpy((void *)(ebase + offset), addr, size);
#endif
	local_flush_icache_range(ebase + offset, ebase + offset + size);
}

static char panic_null_cerr[] __cpuinitdata =
	"Trying to set NULL cache error exception handler";

/*
 * Install uncached CPU exception handler.
 * This is suitable only for the cache error exception which is the only
 * exception handler that is being run uncached.
 */
void __cpuinit set_uncached_handler(unsigned long offset, void *addr,
	unsigned long size)
{
	unsigned long uncached_ebase = CKSEG1ADDR(ebase);

	if (!addr)
		panic(panic_null_cerr);

	memcpy((void *)(uncached_ebase + offset), addr, size);
}

static int __initdata rdhwr_noopt;
static int __init set_rdhwr_noopt(char *str)
{
	rdhwr_noopt = 1;
	return 1;
}

__setup("rdhwr_noopt", set_rdhwr_noopt);

void __init trap_init(void)
{
	extern char except_vec3_generic;
	extern char except_vec4;
	extern char except_vec3_r4000;
	unsigned long i;

	check_wait();

#if defined(CONFIG_KGDB)
	if (kgdb_early_setup)
		return; /* Already done */
#endif

#ifdef CONFIG_MP_PLATFORM_MIPS_EBASE
	if (cpu_has_mips_r2 || cpu_has_veic || cpu_has_vint) {
#else
	if (cpu_has_veic || cpu_has_vint) {
#endif /* CONFIG_MP_PLATFORM_MIPS_EBASE */
		unsigned long size = 0x200 + VECTORSPACING*64;
		ebase = (unsigned long)
			__alloc_bootmem(size, 1 << fls(size), 0);
	} else {
#ifdef CONFIG_KVM_GUEST
#define KVM_GUEST_KSEG0     0x40000000
        ebase = KVM_GUEST_KSEG0;
#else
        ebase = CKSEG0;
#endif
		if (cpu_has_mips_r2)
			ebase += (read_c0_ebase() & 0x3ffff000);
	}

	if (board_ebase_setup)
		board_ebase_setup();
	per_cpu_trap_init(true);

	/*
	 * Copy the generic exception handlers to their final destination.
	 * This will be overriden later as suitable for a particular
	 * configuration.
	 */
	set_handler(0x180, &except_vec3_generic, 0x80);

	/*
	 * Setup default vectors
	 */
	for (i = 0; i <= 31; i++)
		set_except_vector(i, handle_reserved);

	/*
	 * Copy the EJTAG debug exception vector handler code to it's final
	 * destination.
	 */
	if (cpu_has_ejtag && board_ejtag_handler_setup)
		board_ejtag_handler_setup();

	/*
	 * Only some CPUs have the watch exceptions.
	 */
	if (cpu_has_watch)
		set_except_vector(23, handle_watch);

	/*
	 * Initialise interrupt handlers
	 */
	if (cpu_has_veic || cpu_has_vint) {
		int nvec = cpu_has_veic ? 64 : 8;
		for (i = 0; i < nvec; i++)
			set_vi_handler(i, NULL);
	}
	else if (cpu_has_divec)
		set_handler(0x200, &except_vec4, 0x8);

	/*
	 * Some CPUs can enable/disable for cache parity detection, but does
	 * it different ways.
	 */
	parity_protection_init();

	/*
	 * The Data Bus Errors / Instruction Bus Errors are signaled
	 * by external hardware.  Therefore these two exceptions
	 * may have board specific handlers.
	 */
	if (board_be_init)
		board_be_init();

	set_except_vector(0, using_rollback_handler() ? rollback_handle_int
						      : handle_int);
	set_except_vector(1, handle_tlbm);
	set_except_vector(2, handle_tlbl);
	set_except_vector(3, handle_tlbs);

	set_except_vector(4, handle_adel);
	set_except_vector(5, handle_ades);

	set_except_vector(6, handle_ibe);
	set_except_vector(7, handle_dbe);

	set_except_vector(8, handle_sys);
	set_except_vector(9, handle_bp);
	set_except_vector(10, rdhwr_noopt ? handle_ri :
			  (cpu_has_vtag_icache ?
			   handle_ri_rdhwr_vivt : handle_ri_rdhwr));
	set_except_vector(11, handle_cpu);
	set_except_vector(12, handle_ov);
	set_except_vector(13, handle_tr);

	if (current_cpu_type() == CPU_R6000 ||
	    current_cpu_type() == CPU_R6000A) {
		/*
		 * The R6000 is the only R-series CPU that features a machine
		 * check exception (similar to the R4000 cache error) and
		 * unaligned ldc1/sdc1 exception.  The handlers have not been
		 * written yet.	 Well, anyway there is no R6000 machine on the
		 * current list of targets for Linux/MIPS.
		 * (Duh, crap, there is someone with a triple R6k machine)
		 */
		//set_except_vector(14, handle_mc);
		//set_except_vector(15, handle_ndc);
	}


	if (board_nmi_handler_setup)
		board_nmi_handler_setup();

	if (cpu_has_fpu && !cpu_has_nofpuex)
		set_except_vector(15, handle_fpe);

	set_except_vector(22, handle_mdmx);

	if (cpu_has_mcheck)
		set_except_vector(24, handle_mcheck);

	if (cpu_has_mipsmt)
		set_except_vector(25, handle_mt);

	set_except_vector(26, handle_dsp);

	if (board_cache_error_setup)
		board_cache_error_setup();

	if (cpu_has_vce)
		/* Special exception: R4[04]00 uses also the divec space. */
		set_handler(0x180, &except_vec3_r4000, 0x100);
	else if (cpu_has_4kex)
		set_handler(0x180, &except_vec3_generic, 0x80);
	else
		set_handler(0x080, &except_vec3_generic, 0x80);

	local_flush_icache_range(ebase, ebase + 0x400);

	sort_extable(__start___dbe_table, __stop___dbe_table);

	cu2_notifier(default_cu2_call, 0x80000000);	/* Run last  */
}