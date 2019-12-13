#ifndef _CHIP_SETUP_H_
#define _CHIP_SETUP_H_

#define MIPS_MIU0_BASE              (0xA0000000)
#define MIPS_MIU1_BASE              (0xC0000000)
#define MIU1_BUS_BASE               (0x60000000)
#define HIGH_MEM_BUS_BASE           (0x50000000)
#define HIGH_MEM_PHY_BASE           (0x10000000)
#define HIGH_MEM_MAX_BUS_ADDR       (0x80000000)
#define RIU_VIRT_BASE               (0xBF000000)

void Chip_Flush_Memory(void);
void Chip_Read_Memory(void) ;
void Chip_Flush_Memory_Range(unsigned long pAddress ,unsigned long size);
void Chip_Read_Memory_Range(unsigned long pAddress ,unsigned long size);
unsigned int Chip_Query_MIPS_CLK(void);
void Chip_L2_cache_wback_inv(unsigned long addr, unsigned long size);
void Chip_L2_cache_inv(unsigned long addr, unsigned long size);

#endif
