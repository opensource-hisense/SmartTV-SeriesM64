extern unsigned long lx_mem_addr;
extern unsigned long lx_mem_size;
extern unsigned long lx_mem2_addr;
extern unsigned long lx_mem2_size;
extern unsigned long lx_mem3_addr;
extern unsigned long lx_mem3_size;

#ifndef INVALID_PHY_ADDR
#define INVALID_PHY_ADDR (~0UL)
#endif

#define MSTAR_MIU0_BUS_BASE                      0x00000000
#define MSTAR_MIU1_BUS_BASE                      0x60000000
#define MSTAR_MIU2_BUS_BASE                      (~0UL)
#define MSTAR_MIU3_BUS_BASE                      (~0UL)


#define REG_ADDR(addr)                          (*((volatile U16*)(0xBF200000 + (addr << 1))))
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
#define MIU_CLI_AEON_RW                     	0x0005
#define MIU_PROTECT_4                       	0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                           	0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842


#define REG_CHIP_NAND_MODE						0x1EA0
#define REG_CHIP_NAND_MODE_MASK					0x0380
//#define REG_CHIP_NAND_MODE_PCMA				0x0200
//#define REG_CHIP_NAND_MODE_PCMD				0x0100
#define REG_CHIP_PCMCFG                         0x1E9E
#define REG_CHIP_PCMCFG_MASK                	0xC000
#define REG_CHIP_PCMCFG_CTRL_EN             	0xC000 //AD pads & Ctrl pads enable
#define REG_SD_CONFIG2                          0x1EAE
#define REG_SD_CONFIG2_MASK                 	0x0F00 //reg_sd_config & reg_sd_config2

#define REG_CHIP_PCM_PE                         0x1E16
#define REG_CHIP_PCM_PE_MASK                	0x00FF // PCM_D
#define REG_CHIP_PCM_CTL_MASK               	0xFF00 // PCM_CTRL (except PCM_CD, PCM_RESET)
#define REG_CHIP_PCM_PE1                        0x1E18
#define REG_CHIP_PCM_PE1_MASK               	0x00FF // PCM_A
#define REG_CHIP_PCM_D_PE                       0x1E1A
#define REG_CHIP_PCM_D_PE_MASK              	0x0003


#define MS_MIU_INTERVAL 0x20000000

#define MIPS_MIU0_BUS_BASE                    	MSTAR_MIU0_BUS_BASE
#define MIPS_MIU1_BUS_BASE                      MSTAR_MIU1_BUS_BASE
#define MIPS_MIU2_BUS_BASE                    	MSTAR_MIU2_BUS_BASE
#define MIPS_MIU3_BUS_BASE                      MSTAR_MIU3_BUS_BASE


#define MIU0_BUS_BASE MIPS_MIU0_BUS_BASE
#define MIU1_BUS_BASE MIPS_MIU1_BUS_BASE
#define MIU2_BUS_BASE MIPS_MIU2_BUS_BASE
#define MIU3_BUS_BASE MIPS_MIU3_BUS_BASE

// Debug register 
#define R_DEBUG                     0xBF206700 // mailbox 0 

// L2 Pre-Fetch Buffer
#define REG_PREFTECH_L2_BASE (0xBF203000)
#define REG_PREFTECH_L2_CONFIG   (0xBF203100)
#define PREFTECH_L2_CONFIG_DISABLE (0x0002)
#define REG_PREFTECH_L2_CONTROL  (0xBF203104)
#define PREFTECH_L2_CONTROL_MODESEL   (0x000F)
#define PREFTECH_L2_CONTROL_ACTSTART  (0x0010)
#define PREFTECH_L2_CONTROL_MODE0DONE (0x0100)
#define PREFTECH_L2_CONTROL_MODE34DONE (0x0400)

#define PREFTECH_L2_CONTROL_MODE PREFTECH_L2_CONTROL_MODE0DONE


// L2 op mode 
#define L2_OP_MODE_FLUSH_ALL_AND_INVALID_CACHE 0x0004
#define L2_OP_MODE_INITIAL_ALL_INVALID 0x0000
#define REG_CACHE_OP_FIRE 0x0010

#define L2_OP_MODE L2_OP_MODE_INITIAL_ALL_INVALID
