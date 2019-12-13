obj-y	:= clock.o
ifeq ($(KERNELVERSION),3.10.23)
else
obj-$(CONFIG_LOCAL_TIMERS) += localtimer.o
endif
obj-$(CONFIG_PLAT_VERSATILE_CLCD) += clcd.o
obj-$(CONFIG_PLAT_VERSATILE_FPGA_IRQ) += fpga-irq.o
obj-$(CONFIG_PLAT_VERSATILE_LEDS) += leds.o
ifeq ($(KERNELVERSION),3.10.23)
obj-$(CONFIG_PLAT_VERSATILE_SCHED_CLOCK) += sched-clock-new.o
else
ifeq ($(KERNELVERSION),3.8.13)
obj-$(CONFIG_PLAT_VERSATILE_SCHED_CLOCK) += sched-clock-new.o
else
obj-$(CONFIG_PLAT_VERSATILE_SCHED_CLOCK) += sched-clock.o
endif
endif
obj-$(CONFIG_SMP) += headsmp.o platsmp.o
obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o
