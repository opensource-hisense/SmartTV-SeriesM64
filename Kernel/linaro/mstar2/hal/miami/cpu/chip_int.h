#ifndef __CHIP_INT_H__
#define __CHIP_INT_H__

#include <mach/platform.h>

/******************************************************
       CA9 Private Timer
*******************************************************/
#define INT_PPI_FIQ                     28 //GIC PPI FIQ number
#define INT_ID_PTIMER                   29
#define INT_PPI_IRQ                     31 //GIC PPI IRQ number


#define CONFIG_SPI                      0

#if CONFIG_SPI
   #define CONFIG_SPI_IRQ               1
   #define INT_SPI_IRQ_HOST0            63
   #define INT_SPI_IRQ_HOST1            64
   #define INT_SPI_IRQ_HOST2            65
   #define INT_SPI_IRQ_HOST3            66
   #define INT_SPI_FIQ_HOST0            67
   #define INT_SPI_FIQ_HOST1            68
   #define INT_SPI_FIQ_HOST2            69
   #define INT_SPI_FIQ_HOST3            70
   #define INT_SPI_IRQ                  INT_SPI_IRQ_HOST0
   #define SPI_MIN_NUM                  32
   #define SPI_MAX_NUM                  1020

#endif // end of CONFIG_SPI

//Cortex-A9 GIC PPI
#define IRQ_LOCALTIMER                  29
#define IRQ_LOCALWDOG                   30

//Cortex-A9 PMU(Performance Monitor Unit)
#define CHIP_IRQ_PMU0                   182
#define CHIP_IRQ_PMU1                   191
#define CHIP_IRQ_PMU2                   200
#define CHIP_IRQ_PMU3                   209

#define REG_INT_BASE_PA                 (0x1F000000 + (0x101900 << 1))
#define REG_INT_BASE                    (0xFD000000 + (0x101900 << 1))

#define NR_IRQS                         256

//IRQ registers
#define REG_IRQ_MASK_L                  (REG_INT_BASE + (0x0034 << 2))
#define REG_IRQ_MASK_H                  (REG_INT_BASE + (0x0035 << 2))
#define REG_IRQ_PENDING_L               (REG_INT_BASE + (0x003c << 2))
#define REG_IRQ_PENDING_H               (REG_INT_BASE + (0x003d << 2))

//IRQ EXP registers
#define REG_IRQ_EXP_MASK_L              (REG_INT_BASE + (0x0036 << 2))
#define REG_IRQ_EXP_MASK_H              (REG_INT_BASE + (0x0037 << 2))
#define REG_IRQ_EXP_PENDING_L           (REG_INT_BASE + (0x003e << 2))
#define REG_IRQ_EXP_PENDING_H           (REG_INT_BASE + (0x003f << 2))

#if defined(CONFIG_MP_PLATFORM_MSTAR_LEGANCY_INTR)
//FIQ registers
#define REG_FIQ_MASK_L                  (REG_INT_BASE + (0x0024 << 2))
#define REG_FIQ_MASK_H                  (REG_INT_BASE + (0x0025 << 2))
#define REG_FIQ_CLEAR_L                 (REG_INT_BASE + (0x002c << 2))
#define REG_FIQ_CLEAR_H                 (REG_INT_BASE + (0x002d << 2))
#define REG_FIQ_PENDING_L               (REG_INT_BASE + (0x002c << 2))
#define REG_FIQ_PENDING_H               (REG_INT_BASE + (0x002d << 2))

//FIQ EXP registers
#define REG_FIQ_EXP_MASK_L              (REG_INT_BASE + (0x0026 << 2))
#define REG_FIQ_EXP_MASK_H              (REG_INT_BASE + (0x0027 << 2))
#define REG_FIQ_EXP_CLEAR_L             (REG_INT_BASE + (0x002e << 2))
#define REG_FIQ_EXP_CLEAR_H             (REG_INT_BASE + (0x002f << 2))
#define REG_FIQ_EXP_PENDING_L           (REG_INT_BASE + (0x002e << 2))
#define REG_FIQ_EXP_PENDING_H           (REG_INT_BASE + (0x002f << 2))

#else

//FIQ registers
#define REG_FIQ_MASK_L                  (REG_INT_BASE + (0x0004 << 2))
#define REG_FIQ_MASK_H                  (REG_INT_BASE + (0x0005 << 2))
#define REG_FIQ_CLEAR_L                 (REG_INT_BASE + (0x000c << 2))
#define REG_FIQ_CLEAR_H                 (REG_INT_BASE + (0x000d << 2))
#define REG_FIQ_PENDING_L               (REG_INT_BASE + (0x000c << 2))
#define REG_FIQ_PENDING_H               (REG_INT_BASE + (0x000d << 2))

//FIQ EXP registers
#define REG_FIQ_EXP_MASK_L              (REG_INT_BASE + (0x0006 << 2))
#define REG_FIQ_EXP_MASK_H              (REG_INT_BASE + (0x0007 << 2))
#define REG_FIQ_EXP_CLEAR_L             (REG_INT_BASE + (0x000e << 2))
#define REG_FIQ_EXP_CLEAR_H             (REG_INT_BASE + (0x000f << 2))
#define REG_FIQ_EXP_PENDING_L           (REG_INT_BASE + (0x000e << 2))
#define REG_FIQ_EXP_PENDING_H           (REG_INT_BASE + (0x000f << 2))
#endif /* CONFIG_MP_PLATFORM_MSTAR_LEGANCY_INTR */

#ifdef CONFIG_MP_PLATFORM_INT_1_to_1_SPI
#define MSTAR_IRQ_BASE                  32
#define MSTAR_FIQ_BASE                  96
#define MSTAR_INT_BASE                  MSTAR_IRQ_BASE
#define MSTAR_CHIP_INT_END              224
#else
#define MSTAR_IRQ_BASE                  192
#define MSTAR_FIQ_BASE                  128
#define MSTAR_INT_BASE                  MSTAR_FIQ_BASE
#define MSTAR_CHIP_INT_END              256
#endif


/*******************************************************/
/*   THE IRQ AND FIQ ARE NOT COMPLETED.                */
/*   FOR EACH IP OWNER, PLEASE REVIEW IT BY YOURSELF   */
/*******************************************************/
typedef enum
{
    // IRQ
    E_IRQL_START                        = MSTAR_IRQ_BASE,
    E_IRQ_UART0                         = E_IRQL_START + 0,     //IRQ0
    E_IRQ_PM_SLEEP                      = E_IRQL_START + 1,     //IRQ1
    E_IRQ_USB30                         = E_IRQL_START + 2,     //IRQ2
    E_IRQ_MVD                           = E_IRQL_START + 3,     //IRQ3
    E_IRQ_PS                            = E_IRQL_START + 4,     //IRQ4
    E_IRQ_NFIE                          = E_IRQL_START + 5,     //IRQ5
    E_IRQ_USB                           = E_IRQL_START + 6,     //IRQ6
    E_IRQ_UHC                           = E_IRQL_START + 7,     //IRQ7
    E_IRQ_MIIC5                         = E_IRQL_START + 8,     //IRQ8
    E_IRQ_EMAC                          = E_IRQL_START + 9,     //IRQ9
    E_IRQ_DISP                          = E_IRQL_START + 10,    //IRQ10
    E_IRQ_MSPI                          = E_IRQL_START + 11,    //IRQ11
    E_IRQ_MIIC3                         = E_IRQL_START + 12,    //IRQ12
    E_IRQ_EVD                           = E_IRQL_START + 13,    //IRQ13
    E_IRQ_VD                            = E_IRQL_START + 14,    //IRQ14
    E_IRQ_ZDEC                          = E_IRQL_START + 15,    //IRQ15
    E_IRQL_END                          = 15 + E_IRQL_START,

    E_IRQH_START                        = 16 + E_IRQL_START,
    E_IRQ_TSP2HK                        = E_IRQH_START + 0,     //IRQ16
    E_IRQ_VE                            = E_IRQH_START + 1,     //IRQ17
    E_IRQ_MHL_CBUS_PM                   = E_IRQH_START + 2,     //IRQ18
    E_IRQ_DC                            = E_IRQH_START + 3,     //IRQ19
    E_IRQ_GOP                           = E_IRQH_START + 4,     //IRQ20
    E_IRQ_PCM                           = E_IRQH_START + 5,     //IRQ21
    E_IRQ_MCARD                         = E_IRQH_START + 5,     //IRQ21
    E_IRQ_ERROR_RESP                    = E_IRQH_START + 6,     //IRQ22
    E_IRQ_SMART                         = E_IRQH_START + 7,     //IRQ23
    E_IRQ_UART4                         = E_IRQH_START + 8,     //IRQ24
    E_IRQ_USB30_UHC                     = E_IRQH_START + 9,     //IRQ25
    E_IRQ_MIIC4                         = E_IRQH_START + 10,    //IRQ26
    E_IRQ_SCM                           = E_IRQH_START + 11,    //IRQ27
    E_IRQ_VBI                           = E_IRQH_START + 12,    //IRQ28
    E_IRQ_MVD2ARM                       = E_IRQH_START + 13,    //IRQ29
    E_IRQ_GPD                           = E_IRQH_START + 14,    //IRQ30
    E_IRQ_ADCDVI2RIU                    = E_IRQH_START + 15,    //IRQ31
    E_IRQH_END                          = 15 + E_IRQH_START,

    //IRQEXP
    E_IRQEXPL_START                     = 16 + E_IRQH_START,
    E_IRQ_HVD                           = E_IRQEXPL_START + 0,  //IRQ32
    E_IRQ_USB30_UHC1                    = E_IRQEXPL_START + 1,  //IRQ33
    E_IRQ_UHC1                          = E_IRQEXPL_START + 2,  //IRQ34
    E_IRQ_FRC                           = E_IRQEXPL_START + 3,  //IRQ35
    E_IRQ_USB1                          = E_IRQEXPL_START + 4,  //IRQ36
    E_IRQ_USB2                          = E_IRQEXPL_START + 4,  //IRQ36
    E_IRQ_UHC2                          = E_IRQEXPL_START + 5,  //IRQ37
    E_IRQ_AEON2HI                       = E_IRQEXPL_START + 6,  //IRQ38
    E_IRQ_UART1                         = E_IRQEXPL_START + 7,  //IRQ39
    E_IRQ_UART2                         = E_IRQEXPL_START + 8,  //IRQ40
    E_IRQ_MSPI1                         = E_IRQEXPL_START + 9,  //IRQ41
    E_IRQ_MIU_SECURITY                  = E_IRQEXPL_START + 10, //IRQ42
    E_IRQ_DIPW                          = E_IRQEXPL_START + 11, //IRQ43
    E_IRQ_MIIC2                         = E_IRQEXPL_START + 12, //IRQ44
    E_IRQ_JPD                           = E_IRQEXPL_START + 13, //IRQ45
    E_IRQ_PM_IRQ                        = E_IRQEXPL_START + 14, //IRQ46
    E_IRQ_MFE                           = E_IRQEXPL_START + 15, //IRQ47
    E_IRQEXPL_END                       = 15 + E_IRQEXPL_START,

    E_IRQEXPH_START                     = 16 + E_IRQEXPL_START,
    E_IRQ_BDMA                          = E_IRQEXPH_START + 0,  //IRQ48
    E_IRQ_UART3                         = E_IRQEXPH_START + 1,  //IRQ49
    E_IRQ_UART2MCU                      = E_IRQEXPH_START + 2,  //IRQ50
    E_IRQ_URDMA2MCU                     = E_IRQEXPH_START + 3,  //IRQ51
    E_IRQ_DVI_HDMI_HDCP                 = E_IRQEXPH_START + 4,  //IRQ52
    E_IRQ_G3D2MCU                       = E_IRQEXPH_START + 5,  //IRQ53
    E_IRQ_GE                            = E_IRQEXPH_START + 6,  //IRQ54
    E_IRQ_HDCP_IIC                      = E_IRQEXPH_START + 7,  //IRQ55
    E_IRQ_HDCP_X74                      = E_IRQEXPH_START + 8,  //IRQ56
    E_IRQ_WADR_ERR_INT                  = E_IRQEXPH_START + 9,  //IRQ57
    E_IRQ_DCSUB                         = E_IRQEXPH_START + 10, //IRQ58
    E_IRQ_SDIO                          = E_IRQEXPH_START + 11, //IRQ59
    E_IRQ_CMDQ                          = E_IRQEXPH_START + 12, //IRQ60
    E_IRQ_MIIC1                         = E_IRQEXPH_START + 13, //IRQ61
    E_IRQ_USB30_HS0                     = E_IRQEXPH_START + 14, //IRQ62
    E_IRQ_USB30_HS1                     = E_IRQEXPH_START + 14, //IRQ62
    E_IRQ_MIIC0                         = E_IRQEXPH_START + 15, //IRQ63
    E_IRQEXPH_END                       = 15 + E_IRQEXPH_START,

    // FIQ
    E_FIQL_START                        = MSTAR_FIQ_BASE,
    E_FIQ_EXTIMER0                      = E_FIQL_START + 0,     //FIQ0
    E_FIQ_EXTIMER1                      = E_FIQL_START + 1,     //FIQ1
    E_FIQ_WDT                           = E_FIQL_START + 2,     //FIQ2
    E_FIQ_USB3                          = E_FIQL_START + 3,     //FIQ3
    E_FIQ_MB_AUR2TOMCU0                 = E_FIQL_START + 4,     //FIQ4
    E_FIQ_MB_AUR2TOMCU1                 = E_FIQL_START + 5,     //FIQ5
    E_FIQ_MB_DSP2TOMCU0                 = E_FIQL_START + 6,     //FIQ6
    E_FIQ_MB_DSP2TOMCU1                 = E_FIQL_START + 7,     //FIQ7
    E_FIQ_USB                           = E_FIQL_START + 8,     //FIQ8
    E_FIQ_UHC                           = E_FIQL_START + 9,     //FIQ9
    E_FIQ_UHC3                          = E_FIQL_START + 10,    //FIQ10
    E_FIQ_MB_AUR2TOMCU2                 = E_FIQL_START + 11,    //FIQ11
    E_FIQ_MB_AUR2TOMCU3                 = E_FIQL_START + 12,    //FIQ12
    E_FIQ_EMAC                          = E_FIQL_START + 13,    //FIQ13
    E_FIQ_SE_DSP2UP                     = E_FIQL_START + 14,    //FIQ14
    E_FIQ_TSP2AEON                      = E_FIQL_START + 15,    //FIQ15
    E_FIQL_END                          = 15 + E_FIQL_START,

    E_FIQH_START                        = 16 + E_FIQL_START,
    E_FIQ_VIVALDI_STR                   = E_FIQH_START + 0,     //FIQ16
    E_FIQ_VIVALDI_PTS                   = E_FIQH_START + 1,     //FIQ17
    E_FIQ_DSP_MIU_PROT                  = E_FIQH_START + 2,     //FIQ18
    E_FIQ_XIU_TIMEOUT                   = E_FIQH_START + 3,     //FIQ19
    E_FIQ_DMDMCU2HK_INT                 = E_FIQH_START + 4,     //FIQ20
    E_FIQ_VSYNC_VE4VBI                  = E_FIQH_START + 5,     //FIQ21
    E_FIQ_FIELD_VE4VBI                  = E_FIQH_START + 6,     //FIQ22
    E_FIQ_VDMCU2HK                      = E_FIQH_START + 7,     //FIQ23
    E_FIQ_VE_DONE_TT                    = E_FIQH_START + 8,     //FIQ24
    E_FIQ_LDM_DMA_DONE1                 = E_FIQH_START + 9,     //FIQ25
    E_FIQ_PM_SD_CDZ                     = E_FIQH_START + 10,    //FIQ26
    E_FIQ_TEMPERATURE_FALL              = E_FIQH_START + 11,    //FIQ27
    E_FIQ_AFEC_VSYNC                    = E_FIQH_START + 12,    //FIQ28
    E_FIQ_DEC_DSP2UP                    = E_FIQH_START + 13,    //FIQ29
    E_FIQ_TSO                           = E_FIQH_START + 14,    //FIQ30
    E_FIQ_DSP2ARM                       = E_FIQH_START + 15,    //FIQ31
    E_FIQH_END                          = 15 + E_FIQH_START,

    // FIQEXP
    E_FIQEXPL_START                     = 16 + E_FIQH_START,
    E_FIQ_TEMPERATURE_RISE              = E_FIQEXPL_START + 0,  //FIQ32
    E_FIQ_AU_DMA_BUF_INT                = E_FIQEXPL_START + 1,  //FIQ33
    E_FIQ_IR                            = E_FIQEXPL_START + 2,  //FIQ34
    E_FIQ_PM_SD_CDZ1                    = E_FIQEXPL_START + 3,  //FIQ35
    E_FIQ_8051_TO_SECURER2              = E_FIQEXPL_START + 4,  //FIQ36
    E_FIQ_8051_TO_ARMC1                 = E_FIQEXPL_START + 5,  //FIQ37
    E_FIQ_8051_TO_ARM                   = E_FIQEXPL_START + 6,  //FIQ38
    E_FIQ_EXT_GPIO0                     = E_FIQEXPL_START + 7,  //FIQ39
    E_FIQ_ARM_TO_SECURER2               = E_FIQEXPL_START + 8,  //FIQ40
    E_FIQ_CA9_SECURE_ABORT              = E_FIQEXPL_START + 9,  //FIQ41
    E_FIQ_ARM_TO_8051                   = E_FIQEXPL_START + 10, //FIQ42
    E_FIQ_EXT_GPIO1                     = E_FIQEXPL_START + 11, //FIQ43
    E_FIQ_ARMC1_TO_SECURER2             = E_FIQEXPL_START + 12, //FIQ44
    E_FIQ_LDM_DMA_DONE0                 = E_FIQEXPL_START + 13, //FIQ45
    E_FIQ_ARMC1_TO_8051                 = E_FIQEXPL_START + 14, //FIQ46
    E_FIQ_EXT_GPIO2                     = E_FIQEXPL_START + 15, //FIQ47
    E_FIQ_END                           = 15 + E_FIQEXPL_START,

    E_FIQEXPH_START                     = 16 + E_FIQEXPL_START,
    E_FIQ_SECURER2_TO_ARMC1             = E_FIQEXPH_START + 0,  //FIQ48
    E_FIQ_SECURER2_TO_ARM               = E_FIQEXPH_START + 1,  //FIQ49
    E_FIQ_SECURER2_TO_8051              = E_FIQEXPH_START + 2,  //FIQ50
    E_FIQ_USB1                          = E_FIQEXPH_START + 3,  //FIQ51
    E_FIQ_UHC1                          = E_FIQEXPH_START + 4,  //FIQ52
    E_FIQ_USB2                          = E_FIQEXPH_START + 5,  //FIQ53
    E_FIQ_UHC2                          = E_FIQEXPH_START + 6,  //FIQ54
    E_FIQ_EXT_GPIO3                     = E_FIQEXPH_START + 7,  //FIQ55
    E_FIQ_EXT_GPIO4                     = E_FIQEXPH_START + 8,  //FIQ56
    E_FIQ_EXT_GPIO5                     = E_FIQEXPH_START + 9,  //FIQ57
    E_FIQ_EXT_GPIO6                     = E_FIQEXPH_START + 10, //FIQ58
    E_FIQ_PWM_RP_I                      = E_FIQEXPH_START + 11, //FIQ59
    E_FIQ_PWM_FP_I                      = E_FIQEXPH_START + 12, //FIQ60
    E_FIQ_PWM_RP_R                      = E_FIQEXPH_START + 13, //FIQ61
    E_FIQ_PWM_FP_R                      = E_FIQEXPH_START + 14, //FIQ62
    E_FIQ_EXT_GPIO7                     = E_FIQEXPH_START + 15, //FIQ63
    E_FIQEXPH_END                       = 15 + E_FIQEXPL_START,

    E_IRQ_FIQ_ALL                       = 0xFF //all IRQs & FIQs
} InterruptNum;

// #define REG_IRQ_PENDING_L
#define IRQL_MASK                       0xFFFF
#define IRQ_UART0                       (0x01 << (E_IRQ_UART0               - E_IRQL_START))
#define IRQ_PM_SLEEP                    (0x01 << (E_IRQ_PM_SLEEP            - E_IRQL_START))
#define IRQ_USB30                       (0x01 << (E_IRQ_USB30               - E_IRQL_START))
#define IRQ_MVD                         (0x01 << (E_IRQ_MVD                 - E_IRQL_START))
#define IRQ_PS                          (0x01 << (E_IRQ_PS                  - E_IRQL_START))
#define IRQ_NFIE                        (0x01 << (E_IRQ_NFIE                - E_IRQL_START))
#define IRQ_USB                         (0x01 << (E_IRQ_USB                 - E_IRQL_START))
#define IRQ_UHC                         (0x01 << (E_IRQ_UHC                 - E_IRQL_START))
#define IRQ_MIIC5                       (0x01 << (E_IRQ_MIIC5               - E_IRQL_START))
#define IRQ_EMAC                        (0x01 << (E_IRQ_EMAC                - E_IRQL_START))
#define IRQ_DISP                        (0x01 << (E_IRQ_DISP                - E_IRQL_START))
#define IRQ_MSPI                        (0x01 << (E_IRQ_MSPI                - E_IRQL_START))
#define IRQ_MIIC3                       (0x01 << (E_IRQ_MIIC3               - E_IRQL_START))
#define IRQ_EVD                         (0x01 << (E_IRQ_EVD                 - E_IRQL_START))
#define IRQ_VD                          (0x01 << (E_IRQ_VD                  - E_IRQL_START))
#define IRQ_ZDEC                        (0x01 << (E_IRQ_ZDEC                - E_IRQL_START))

// #define REG_IRQ_PENDING_H
#define IRQH_MASK                       0xFFFF
#define IRQ_TSP2HK                      (0x01 << (E_IRQ_TSP2HK              - E_IRQH_START))
#define IRQ_VE                          (0x01 << (E_IRQ_VE                  - E_IRQH_START))
#define IRQ_MHL_CBUS_PM                 (0x01 << (E_IRQ_MHL_CBUS_PM         - E_IRQH_START))
#define IRQ_DC                          (0x01 << (E_IRQ_DC                  - E_IRQH_START))
#define IRQ_GOP                         (0x01 << (E_IRQ_GOP                 - E_IRQH_START))
#define IRQ_PCM                         (0x01 << (E_IRQ_PCM                 - E_IRQH_START))
#define IRQ_MCARD                       (0x01 << (E_IRQ_MCARD               - E_IRQH_START))
#define IRQ_ERROR_RESP                  (0x01 << (E_IRQ_ERROR_RESP          - E_IRQH_START))
#define IRQ_SMART                       (0x01 << (E_IRQ_SMART               - E_IRQH_START))
#define IRQ_UART4                       (0x01 << (E_IRQ_UART4               - E_IRQH_START))
#define IRQ_USB30_UHC                   (0x01 << (E_IRQ_USB30_UHC           - E_IRQH_START))
//#define IRQ_UHC30                     (0x01 << (E_IRQ_USB30_UHC           - E_IRQH_START))
#define IRQ_MIIC4                       (0x01 << (E_IRQ_MIIC4               - E_IRQH_START))
#define IRQ_SCM                         (0x01 << (E_IRQ_SCM                 - E_IRQH_START))
#define IRQ_VBI                         (0x01 << (E_IRQ_VBI                 - E_IRQH_START))
#define IRQ_MVD2ARM                     (0x01 << (E_IRQ_MVD2ARM             - E_IRQH_START))
#define IRQ_GPD                         (0x01 << (E_IRQ_GPD                 - E_IRQH_START))
#define IRQ_ADCDVI2RIU                  (0x01 << (E_IRQ_ADCDVI2RIU          - E_IRQH_START))

// #define REG_IRQEXP_PENDING_L
#define IRQEXPL_MASK                    0xFFFF
#define IRQ_HVD                         (0x01 << (E_IRQ_HVD                 - E_IRQEXPL_START))
#define IRQ_USB30_UHC1                  (0x01 << (E_IRQ_USB30_UHC1          - E_IRQEXPL_START))
#define IRQ_UHC1                        (0x01 << (E_IRQ_UHC1                - E_IRQEXPL_START))
//#define IRQEXPL_UHC1                  (0x01 << (E_IRQ_UHC1                - E_IRQEXPL_START))
#define IRQ_FRC                         (0x01 << (E_IRQ_FRC                 - E_IRQEXPL_START))
#define IRQ_USB1                        (0x01 << (E_IRQ_USB1                - E_IRQEXPL_START))
#define IRQ_USB2                        (0x01 << (E_IRQ_USB2                - E_IRQEXPL_START))
#define IRQ_UHC2                        (0x01 << (E_IRQ_UHC2                - E_IRQEXPL_START))
//#define IRQEXPL_UHC2                  (0x01 << (E_IRQ_UHC2                - E_IRQEXPL_START))
#define IRQ_AEON2HI                     (0x01 << (E_IRQ_AEON2HI             - E_IRQEXPL_START))
#define IRQ_UART1                       (0x01 << (E_IRQ_UART1               - E_IRQEXPL_START))
#define IRQ_UART2                       (0x01 << (E_IRQ_UART2               - E_IRQEXPL_START))
#define IRQ_MSPI1                       (0x01 << (E_IRQ_MSPI1               - E_IRQEXPL_START))
#define IRQ_MIU_SECURITY                (0x01 << (E_IRQ_MIU_SECURITY        - E_IRQEXPL_START))
#define IRQ_DIPW                        (0x01 << (E_IRQ_DIPW                - E_IRQEXPL_START))
#define IRQ_MIIC2                       (0x01 << (E_IRQ_MIIC2               - E_IRQEXPL_START))
#define IRQ_JPD                         (0x01 << (E_IRQ_JPD                 - E_IRQEXPL_START))
#define IRQ_PM_IRQ                      (0x01 << (E_IRQ_PM_IRQ              - E_IRQEXPL_START))
#define IRQ_MFE                         (0x01 << (E_IRQ_MFE                 - E_IRQEXPL_START))

// #define REG_IRQEXP_PENDING_H
#define IRQEXPH_MASK                    0xFFFF
#define IRQ_BDMA                        (0x01 << (E_IRQ_BDMA                - E_IRQEXPH_START))
#define IRQ_UART3                       (0x01 << (E_IRQ_UART3               - E_IRQEXPH_START))
#define IRQ_UART2MCU                    (0x01 << (E_IRQ_UART2MCU            - E_IRQEXPH_START))
#define IRQ_URDMA2MCU                   (0x01 << (E_IRQ_URDMA2MCU           - E_IRQEXPH_START))
#define IRQ_DVI_HDMI_HDCP               (0x01 << (E_IRQ_DVI_HDMI_HDCP       - E_IRQEXPH_START))
#define IRQ_G3D2MCU                     (0x01 << (E_IRQ_G3D2MCU             - E_IRQEXPH_START))
#define IRQ_GE                          (0x01 << (E_IRQ_GE                  - E_IRQEXPH_START))
#define IRQ_HDCP_IIC                    (0x01 << (E_IRQ_HDCP_IIC            - E_IRQEXPH_START))
#define IRQ_HDCP_X74                    (0x01 << (E_IRQ_HDCP_X74            - E_IRQEXPH_START))
#define IRQ_WADR_ERR_INT                (0x01 << (E_IRQ_WADR_ERR_INT        - E_IRQEXPH_START))
#define IRQ_DCSUB                       (0x01 << (E_IRQ_DCSUB               - E_IRQEXPH_START))
#define IRQ_SDIO                        (0x01 << (E_IRQ_SDIO                - E_IRQEXPH_START))
#define IRQ_CMDQ                        (0x01 << (E_IRQ_CMDQ                - E_IRQEXPH_START))
#define IRQ_MIIC1                       (0x01 << (E_IRQ_MIIC1               - E_IRQEXPH_START))
#define IRQ_USB30_HS0                   (0x01 << (E_IRQ_USB30_HS0           - E_IRQEXPH_START))
#define IRQ_USB30_HS1                   (0x01 << (E_IRQ_USB30_HS1           - E_IRQEXPH_START))
#define IRQ_MIIC0                       (0x01 << (E_IRQ_MIIC0               - E_IRQEXPH_START))

// REG_FIQ_MASK_L
//FIQ Low 16 bits
#define FIQL_MASK                       0xFFFF
#define FIQ_EXTIMER0                    (0x01 << (E_FIQ_EXTIMER0            - E_FIQL_START))
#define FIQ_EXTIMER1                    (0x01 << (E_FIQ_EXTIMER1            - E_FIQL_START))
#define FIQ_WDT                         (0x01 << (E_FIQ_WDT                 - E_FIQL_START))
#define FIQ_USB3                        (0x01 << (E_FIQ_USB3                - E_FIQL_START))
#define FIQ_MB_AUR2TOMCU0               (0x01 << (E_FIQ_MB_AUR2TOMCU0       - E_FIQL_START))
#define FIQ_MB_AUR2TOMCU1               (0x01 << (E_FIQ_MB_AUR2TOMCU1       - E_FIQL_START))
#define FIQ_MB_DSP2TOMCU0               (0x01 << (E_FIQ_MB_DSP2TOMCU0       - E_FIQL_START))
#define FIQ_MB_DSP2TOMCU1               (0x01 << (E_FIQ_MB_DSP2TOMCU1       - E_FIQL_START))
#define FIQ_USB                         (0x01 << (E_FIQ_USB                 - E_FIQL_START))
#define FIQ_UHC                         (0x01 << (E_FIQ_UHC                 - E_FIQL_START))
#define FIQ_UHC3                        (0x01 << (E_FIQ_UHC3                - E_FIQL_START))
#define FIQ_MB_AUR2TOMCU2               (0x01 << (E_FIQ_MB_AUR2TOMCU2       - E_FIQL_START))
#define FIQ_MB_AUR2TOMCU3               (0x01 << (E_FIQ_MB_AUR2TOMCU3       - E_FIQL_START))
#define FIQ_EMAC                        (0x01 << (E_FIQ_EMAC                - E_FIQL_START))
#define FIQ_SE_DSP2UP                   (0x01 << (E_FIQ_SE_DSP2UP           - E_FIQL_START))
#define FIQ_TSP2AEON                    (0x01 << (E_FIQ_TSP2AEON            - E_FIQL_START))

// REG_FIQ_MASK_H
//FIQ High 16 bits
#define FIQH_MASK                       0xFFFF
#define FIQ_VIVALDI_STR                 (0x01 << (E_FIQ_VIVALDI_STR         - E_FIQH_START))
#define FIQ_VIVALDI_PTS                 (0x01 << (E_FIQ_VIVALDI_PTS         - E_FIQH_START))
#define FIQ_DSP_MIU_PROT                (0x01 << (E_FIQ_DSP_MIU_PROT        - E_FIQH_START))
#define FIQ_XIU_TIMEOUT                 (0x01 << (E_FIQ_XIU_TIMEOUT         - E_FIQH_START))
#define FIQ_DMDMCU2HK_INT               (0x01 << (E_FIQ_DMDMCU2HK_INT       - E_FIQH_START))
#define FIQ_VSYNC_VE4VBI                (0x01 << (E_FIQ_VSYNC_VE4VBI        - E_FIQH_START))
#define FIQ_FIELD_VE4VBI                (0x01 << (E_FIQ_FIELD_VE4VBI        - E_FIQH_START))
#define FIQ_VDMCU2HK                    (0x01 << (E_FIQ_VDMCU2HK            - E_FIQH_START))
#define FIQ_VE_DONE_TT                  (0x01 << (E_FIQ_VE_DONE_TT          - E_FIQH_START))
#define FIQ_LDM_DMA_DONE1               (0x01 << (E_FIQ_LDM_DMA_DONE1       - E_FIQH_START))
#define FIQ_PM_SD_CDZ                   (0x01 << (E_FIQ_PM_SD_CDZ           - E_FIQH_START))
#define FIQ_TEMPERATURE_FALL            (0x01 << (E_FIQ_TEMPERATURE_FALL    - E_FIQH_START))
#define FIQ_AFEC_VSYNC                  (0x01 << (E_FIQ_AFEC_VSYNC          - E_FIQH_START))
#define FIQ_DEC_DSP2UP                  (0x01 << (E_FIQ_DEC_DSP2UP          - E_FIQH_START))
#define FIQ_TSO                         (0x01 << (E_FIQ_TSO                 - E_FIQH_START))
#define FIQ_DSP2ARM                     (0x01 << (E_FIQ_DSP2ARM             - E_FIQH_START))

// #define REG_FIQEXP_PENDING_L
#define FIQEXPL_MASK                    0xFFFF
#define FIQ_TEMPERATURE_RISE            (0x01 << (E_FIQ_TEMPERATURE_RISE    - E_FIQEXPL_START))
#define FIQ_AU_DMA_BUF_INT              (0x01 << (E_FIQ_AU_DMA_BUF_INT      - E_FIQEXPL_START))
#define FIQ_IR                          (0x01 << (E_FIQ_IR                  - E_FIQEXPL_START))
#define FIQ_PM_SD_CDZ1                  (0x01 << (E_FIQ_PM_SD_CDZ1          - E_FIQEXPL_START))
#define FIQ_8051_TO_SECURER2            (0x01 << (E_FIQ_8051_TO_SECURER2    - E_FIQEXPL_START))
#define FIQ_8051_TO_ARMC1               (0x01 << (E_FIQ_8051_TO_ARMC1       - E_FIQEXPL_START))
#define FIQ_8051_TO_ARM                 (0x01 << (E_FIQ_8051_TO_ARM         - E_FIQEXPL_START))
#define FIQ_EXT_GPIO0                   (0x01 << (E_FIQ_EXT_GPIO0           - E_FIQEXPL_START))
#define FIQ_ARM_TO_SECURER2             (0x01 << (E_FIQ_ARM_TO_SECURER2     - E_FIQEXPL_START))
#define FIQ_CA9_SECURE_ABORT            (0x01 << (E_FIQ_CA9_SECURE_ABORT    - E_FIQEXPL_START))
#define FIQ_ARM_TO_8051                 (0x01 << (E_FIQ_ARM_TO_8051         - E_FIQEXPL_START))
#define FIQ_EXT_GPIO1                   (0x01 << (E_FIQ_EXT_GPIO1           - E_FIQEXPL_START))
#define FIQ_ARMC1_TO_SECURER2           (0x01 << (E_FIQ_ARMC1_TO_SECURER2   - E_FIQEXPL_START))
#define FIQ_LDM_DMA_DONE0               (0x01 << (E_FIQ_LDM_DMA_DONE0       - E_FIQEXPL_START))
#define FIQ_ARMC1_TO_8051               (0x01 << (E_FIQ_ARMC1_TO_8051       - E_FIQEXPL_START))
#define FIQ_EXT_GPIO2                   (0x01 << (E_FIQ_EXT_GPIO2           - E_FIQEXPL_START))

// #define REG_FIQEXP_PENDING_H
#define FIQEXPH_MASK                    0xFFFF
#define FIQ_SECURER2_TO_ARMC1           (0x01 << (FIQ_SECURER2_TO_ARMC1     - E_FIQEXPH_START))
#define FIQ_SECURER2_TO_ARM             (0x01 << (FIQ_SECURER2_TO_ARM       - E_FIQEXPH_START))
#define FIQ_SECURER2_TO_8051            (0x01 << (FIQ_SECURER2_TO_8051      - E_FIQEXPH_START))
#define FIQ_USB1                        (0x01 << (FIQ_USB1                  - E_FIQEXPH_START))
#define FIQ_UHC1                        (0x01 << (FIQ_UHC1                  - E_FIQEXPH_START))
#define FIQ_USB2                        (0x01 << (FIQ_USB2                  - E_FIQEXPH_START))
#define FIQ_UHC2                        (0x01 << (FIQ_UHC2                  - E_FIQEXPH_START))
#define FIQ_EXT_GPIO3                   (0x01 << (FIQ_EXT_GPIO3             - E_FIQEXPH_START))
#define FIQ_EXT_GPIO4                   (0x01 << (FIQ_EXT_GPIO4             - E_FIQEXPH_START))
#define FIQ_EXT_GPIO5                   (0x01 << (FIQ_EXT_GPIO5             - E_FIQEXPH_START))
#define FIQ_EXT_GPIO6                   (0x01 << (FIQ_EXT_GPIO6             - E_FIQEXPH_START))
#define FIQ_PWM_RP_I                    (0x01 << (FIQ_PWM_RP_I              - E_FIQEXPH_START))
#define FIQ_PWM_FP_I                    (0x01 << (FIQ_PWM_FP_I              - E_FIQEXPH_START))
#define FIQ_PWM_RP_R                    (0x01 << (FIQ_PWM_RP_R              - E_FIQEXPH_START))
#define FIQ_PWM_FP_R                    (0x01 << (FIQ_PWM_FP_R              - E_FIQEXPH_START))
#define FIQ_EXT_GPIO7                   (0x01 << (FIQ_EXT_GPIO7             - E_FIQEXPH_START))

#define IRQL_EXP_ALL                    0xFFFF
#define IRQH_EXP_ALL                    0xFFFF
#define FIQL_EXP_ALL                    0xFFFF
#define FIQH_EXP_ALL                    0xFFFF

extern unsigned int interrupt_configs[MSTAR_CHIP_INT_END / 16];

void init_chip_spi_config(void);

#endif // #ifndef __CHIP_INT_H__
