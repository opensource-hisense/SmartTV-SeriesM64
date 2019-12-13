#ifndef __CHIP_INT_H__
#define __CHIP_INT_H__

#include <mach/platform.h>

/******************************************************
       CA9 Private Timer
*******************************************************/
#define INT_PPI_FIQ                     28 //GIC PPI FIQ number
#define INT_ID_PTIMER                   29
#define INT_PPI_IRQ                     31 //GIC PPI IRQ number


#define CONFIG_SPI                      1

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
#define CHIP_IRQ_PMU0                   166
#define CHIP_IRQ_PMU1                   172
#define CHIP_IRQ_PMU2                   178
#define CHIP_IRQ_PMU3                   184

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
    E_IRQ_UART0                         = E_IRQL_START + 0,
    E_IRQ_PM_SLEEP                      = E_IRQL_START + 1,
    E_IRQ_UHC30                         = E_IRQL_START + 2,
    E_IRQ_MVD                           = E_IRQL_START + 3,
    E_IRQ_PS                            = E_IRQL_START + 4,
    E_IRQ_NFIE                          = E_IRQL_START + 5,
    E_IRQ_USB                           = E_IRQL_START + 6,
    E_IRQ_UHC                           = E_IRQL_START + 7,
    E_IRQ_MIIC5                         = E_IRQL_START + 8,
    E_IRQ_EMAC                          = E_IRQL_START + 9,
    E_IRQ_DISP                          = E_IRQL_START + 10, /*be used in "kernel/irq/proc.c"*/
    E_IRQ_MSPI                          = E_IRQL_START + 11,
    E_IRQ_MIIC3                         = E_IRQL_START + 12,
    E_IRQ_EVD                           = E_IRQL_START + 13,
    E_IRQ_COMB                          = E_IRQL_START + 14,
    E_IRQ_LDM_DMA0                      = E_IRQL_START + 15,
    E_IRQL_END                          = 15 + E_IRQL_START,

    E_IRQH_START                        = 16 + E_IRQL_START,
    E_IRQ_TSP2HK                        = E_IRQH_START + 0,
    E_IRQ_VE                            = E_IRQH_START + 1,
    E_IRQ_CIMAX2MCU                     = E_IRQH_START + 2,
    E_IRQ_DC                            = E_IRQH_START + 3,
    E_IRQ_GOP                           = E_IRQH_START + 4,
    E_IRQ_PCM                           = E_IRQH_START + 5,
    E_IRQ_LDM_DMA1                      = E_IRQH_START + 6,
    E_IRQ_SMART                         = E_IRQH_START + 7,
    E_IRQ_UART4                         = E_IRQH_START + 8,
    E_IRQ_OTG                           = E_IRQH_START + 9,
    E_IRQ_MIIC4                         = E_IRQH_START + 10,
    E_IRQ_SCM                           = E_IRQH_START + 11,
    E_IRQ_VBI                           = E_IRQH_START + 12,
    E_IRQ_MVD2ARM                       = E_IRQH_START + 13,
    E_IRQ_GPD                           = E_IRQH_START + 14,
    E_IRQ_ADCDVI2RIU                    = E_IRQH_START + 15,
    E_IRQH_END                          = 15 + E_IRQH_START,

    // FIQ
    E_FIQL_START                        = MSTAR_FIQ_BASE,
    E_FIQ_EXTIMER0                      = E_FIQL_START + 0,
    E_FIQ_EXTIMER1                      = E_FIQL_START + 1,
    E_FIQ_WDT                           = E_FIQL_START + 2,
    E_FIQ_USB30                         = E_FIQL_START + 3,
    E_FIQ_MB_AUR2TOMCU0                 = E_FIQL_START + 4,
    E_FIQ_MB_AUR2TOMCU1                 = E_FIQL_START + 5,
    E_FIQ_MB_DSP2TOMCU0                 = E_FIQL_START + 6,
    E_FIQ_MB_DSP2TOMCU1                 = E_FIQL_START + 7,
    E_FIQ_USB                           = E_FIQL_START + 8,
    E_FIQ_UHC                           = E_FIQL_START + 9,
    E_FIQ_OTG                           = E_FIQL_START + 10,
    E_FIQ_MB_AUR2TO_MCU0                = E_FIQL_START + 11,
    E_FIQ_MB_AUR2TO_MCU1                = E_FIQL_START + 12,
    E_FIQ_LAN_ESD                       = E_FIQL_START + 13,
    E_FIQ_SE_DSP2UP                     = E_FIQL_START + 14,
    E_FIQ_TSP2AEON                      = E_FIQL_START + 15,
    E_FIQL_END                          = 15 + E_FIQL_START,

    E_FIQH_START                        = 16 + E_FIQL_START,
    E_FIQ_VIVALDI_STR                   = E_FIQH_START + 0,
    E_FIQ_VIVALDI_PTS                   = E_FIQH_START + 1,
    E_FIQ_DSP_MIU_PROT                  = E_FIQH_START + 2,
    E_FIQ_XIU_TIMEOUT                   = E_FIQH_START + 3,
    E_FIQ_DMDMCU2HK_INT                 = E_FIQH_START + 4,
    E_FIQ_VSYNC_VE4VBI                  = E_FIQH_START + 5,
    E_FIQ_FIELD_VE4VBI                  = E_FIQH_START + 6,
    E_FIQ_VDMCU2HK                      = E_FIQH_START + 7,
    E_FIQ_VE_DONE_TT                    = E_FIQH_START + 8,
    E_FIQ_ERROR_RESP                    = E_FIQH_START + 9,
    E_FIQ_PM_SD_CDZ                     = E_FIQH_START + 10,
    E_FIQ_TEMPERATURE_fail              = E_FIQH_START + 11,
    E_FIQ_HDMITX_IRQ_EDGE               = E_FIQH_START + 12,
    E_FIQ_DEC_DSP2UP                    = E_FIQH_START + 13,
    E_FIQ_TSO                           = E_FIQH_START + 14,
    E_FIQ_DSP2ARM                       = E_FIQH_START + 15,
    E_FIQH_END                          = 15 + E_FIQH_START,

    //IRQEXP
    E_IRQEXPL_START                     = 16 + E_IRQH_START,
    E_IRQEXPL_HVD                       = E_IRQEXPL_START + 0,
    E_IRQEXPL_USB1                      = E_IRQEXPL_START + 1,
    E_IRQEXPL_UHC1                      = E_IRQEXPL_START + 2,
    E_IRQEXPL_SECEMAC                   = E_IRQEXPL_START + 3,
    E_IRQEXPL_USB2                      = E_IRQEXPL_START + 4,
    E_IRQEXPL_UHC2                      = E_IRQEXPL_START + 5,
    E_IRQEXPL_AEON2HI                   = E_IRQEXPL_START + 6,
    E_IRQEXPL_UART1                     = E_IRQEXPL_START + 7,
    E_IRQEXPL_UART2                     = E_IRQEXPL_START + 8,
    E_IRQEXPL_MSPI1                     = E_IRQEXPL_START + 9,
    E_IRQEXPL_MIU                       = E_IRQEXPL_START + 10,
    E_IRQEXPL_DIPW                      = E_IRQEXPL_START + 11,
    E_IRQEXPL_MIIC2                     = E_IRQEXPL_START + 12,
    E_IRQEXPL_JPD                       = E_IRQEXPL_START + 13,
    E_IRQEXPL_PM_IRQ_OUT                = E_IRQEXPL_START + 14,
    E_IRQEXPL_MFE                       = E_IRQEXPL_START + 15,
    E_IRQEXPL_END                       = 15 + E_IRQEXPL_START,

    E_IRQEXPH_START                     = 16 + E_IRQEXPL_START,
    E_IRQEXPH_BDMA_MERGE                = E_IRQEXPH_START + 0,
    E_IRQEXPH_UART3                     = E_IRQEXPH_START + 1,
    E_IRQEXPH_UART2MCU                  = E_IRQEXPH_START + 2,
    E_IRQEXPH_UARTDMA2MCU               = E_IRQEXPH_START + 3,
    E_IRQEXPH_HDMITX_IRQ                = E_IRQEXPH_START + 4,
    E_IRQEXPH_G3D2MCU                   = E_IRQEXPH_START + 5,
    E_IRQEXPH_CEC                       = E_IRQEXPH_START + 6,
    E_IRQEXPH_HDCP_IIC                  = E_IRQEXPH_START + 7,
    E_IRQEXPH_HDCP_X74                  = E_IRQEXPH_START + 8,
    E_IRQEXPH_WADR_ERR_INT              = E_IRQEXPH_START + 9,
    E_IRQEXPH_GE                        = E_IRQEXPH_START + 10,
    E_IRQEXPH_SDIO                      = E_IRQEXPH_START + 11,
    E_IRQEXPH_CMDQ                      = E_IRQEXPH_START + 12,
    E_IRQEXPH_MIIC1                     = E_IRQEXPH_START + 13,
    E_IRQEXPH_IRQ_OUT_PLUG              = E_IRQEXPH_START + 14,
    E_IRQEXPH_MIIC0                     = E_IRQEXPH_START + 15,
    E_IRQEXPH_END                       = 15 + E_IRQEXPH_START,

    // FIQEXP
    E_FIQEXPL_START                     = 16 + E_FIQH_START,
    E_FIQEXPL_TEMPERATURE_RISE          = E_FIQEXPL_START + 0,
    E_FIQEXPL_AU_DMA_BUF_INT            = E_FIQEXPL_START + 1,
    E_FIQ_IR                            = E_FIQEXPL_START + 2,
    E_FIQEXPL_PM_SD_CDZ1                = E_FIQEXPL_START + 3,
    E_FIQEXPL_8051_TO_SECURER2          = E_FIQEXPL_START + 4,
    E_FIQEXPL_8051_TO_ARMC1             = E_FIQEXPL_START + 5,
    E_FIQEXPL_8051_TO_ARM               = E_FIQEXPL_START + 6,
    E_FIQEXPL_EXT_GPIO0                 = E_FIQEXPL_START + 7,
    E_FIQEXPL_ARM_TO_SECURE_R2          = E_FIQEXPL_START + 8,
    E_FIQEXPL_CA9_SCUEVABORT            = E_FIQEXPL_START + 9,
    E_FIQEXPL_ARM_TO_8051               = E_FIQEXPL_START + 10,
    E_FIQEXPL_EXT_GPIO1                 = E_FIQEXPL_START + 11,
    E_FIQEXPL_ARMC1_TO_SECURER2         = E_FIQEXPL_START + 12,
    E_FIQEXPL_TIMER2                    = E_FIQEXPL_START + 13,
    E_FIQEXPL_ARMC1_TO_8051             = E_FIQEXPL_START + 14,
    E_FIQEXPL_EXT_GPIO2                 = E_FIQEXPL_START + 15,
    E_FIQEXPL_END                       = 15 + E_FIQEXPL_START,

    E_FIQEXPH_START                     = 16 + E_FIQEXPL_START,
    E_FIQEXPL_SECURE_R2_TO_ARMC1        = E_FIQEXPH_START + 0,
    E_FIQEXPL_SECURE_R2_TO_ARM          = E_FIQEXPH_START + 1,
    E_FIQEXPL_SECURE_R2_TO_8051         = E_FIQEXPH_START + 2,
    E_FIQEXPH_USB1                      = E_FIQEXPH_START + 3,
    E_FIQEXPH_UHC1                      = E_FIQEXPH_START + 4,
    E_FIQEXPH_USB2                      = E_FIQEXPH_START + 5,
    E_FIQEXPH_UHC2                      = E_FIQEXPH_START + 6,
    E_FIQEXPH_EXT_GPIO3                 = E_FIQEXPH_START + 7,
    E_FIQEXPH_EXT_GPIO4                 = E_FIQEXPH_START + 8,
    E_FIQEXPH_EXT_GPIO5                 = E_FIQEXPH_START + 9,
    E_FIQEXPH_EXT_GPIO6                 = E_FIQEXPH_START + 10,
    E_FIQEXPH_VE_SW                     = E_FIQEXPH_START + 11,
    E_FIQEXPH_PWM_FP_I                  = E_FIQEXPH_START + 12,
    E_FIQEXPH_PWM_RP_R                  = E_FIQEXPH_START + 13,
    E_FIQEXPH_PWM_FP_R                  = E_FIQEXPH_START + 14,
    E_FIQEXPH_EXT_GPIO7                 = E_FIQEXPH_START + 15,
    E_FIQEXPH_END                       = 15 + E_FIQEXPL_START,

    E_IRQ_FIQ_ALL                       = 0xFF //all IRQs & FIQs
} InterruptNum;

// REG_FIQ_MASK_L
//FIQ Low 16 bits
#define FIQL_MASK                       0xFFFF
#define FIQ_EXTIMER0                    (0x01 << (E_FIQ_EXTIMER0            - E_FIQL_START))
#define FIQ_EXTIMER1                    (0x01 << (E_FIQ_EXTIMER1            - E_FIQL_START))
#define FIQ_WDT                         (0x01 << (E_FIQ_WDT                 - E_FIQL_START))
#define FIQ_USB30                       (0x01 << (E_FIQ_USB30               - E_FIQL_START))
#define FIQ_MB_AUR2TOMCU0               (0x01 << (E_FIQ_MB_AUR2TOMCU0       - E_FIQL_START))
#define FIQ_MB_AUR2TOMCU1               (0x01 << (E_FIQ_MB_AUR2TOMCU1       - E_FIQL_START))
#define FIQ_MB_DSP2TOMCU0               (0x01 << (E_FIQ_MB_DSP2TOMCU0       - E_FIQL_START))
#define FIQ_MB_DSP2TOMCU1               (0x01 << (E_FIQ_MB_DSP2TOMCU1       - E_FIQL_START))
#define FIQ_USB                         (0x01 << (E_FIQ_USB                 - E_FIQL_START))
#define FIQ_UHC                         (0x01 << (E_FIQ_UHC                 - E_FIQL_START))
#define FIQ_OTG                         (0x01 << (E_FIQ_OTG                 - E_FIQL_START))
#define FIQ_MB_AUR2TO_MCU0              (0x01 << (E_FIQ_MB_AUR2TO_MCU0      - E_FIQL_START))
#define FIQ_MB_AUR2TO_MCU1              (0x01 << (E_FIQ_MB_AUR2TO_MCU1      - E_FIQL_START))
#define FIQ_LAN_ESD                     (0x01 << (E_FIQ_LAN_ESD             - E_FIQL_START))
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
#define FIQ_ERROR_RESP                  (0x01 << (E_FIQ_ERROR_RESP                - E_FIQH_START))
#define E_FIQ_PM_SD_CDZ                 (0x01 << (E_FIQ_PM_SD_CDZ                  - E_FIQH_START))
#define E_FIQ_TEMPERATURE_fail          (0x01 << (E_FIQ_TEMPERATURE_fail          - E_FIQH_START))
#define E_FIQ_HDMITX_IRQ_EDGE           (0x01 << (E_FIQ_HDMITX_IRQ_EDGE          - E_FIQH_START))
#define E_FIQ_DEC_DSP2UP                (0x01 << (E_FIQ_DEC_DSP2UP                - E_FIQH_START))
#define E_FIQ_TSO                       (0x01 << (E_FIQ_TSO             - E_FIQH_START))
#define E_FIQ_DSP2ARM                   (0x01 << (E_FIQ_DSP2ARM             - E_FIQH_START))


// #define REG_IRQ_PENDING_L
#define IRQL_MASK                       0xFFFF
#define IRQ_UART0                       (0x01 << (E_IRQ_UART0               - E_IRQL_START))
#define IRQ_PM_SLEEP                    (0x01 << (E_IRQ_PM_SLEEP            - E_IRQL_START))
#define IRQ_UHC30                       (0x01 << (E_IRQ_UHC30               - E_IRQL_START))
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
#define IRQ_COMB                        (0x01 << (E_IRQ_COMB                - E_IRQL_START))
#define IRQ_LDM_DMA0                    (0x01 << (E_IRQ_LDM_DMA0            - E_IRQL_START))


// #define REG_IRQ_PENDING_H
#define IRQH_MASK                       0xFFFF
#define IRQ_TSP2HK                      (0x01 << (E_IRQ_TSP2HK              - E_IRQH_START))
#define IRQ_VE                          (0x01 << (E_IRQ_VE                  - E_IRQH_START))
#define IRQ_CIMAX2MCU                   (0x01 << (E_IRQ_CIMAX2MCU           - E_IRQH_START))
#define IRQ_DC                          (0x01 << (E_IRQ_DC                  - E_IRQH_START))
#define IRQ_GOP                         (0x01 << (E_IRQ_GOP                 - E_IRQH_START))
#define IRQ_PCM                         (0x01 << (E_IRQ_PCM                 - E_IRQH_START))
#define IRQ_LDM_DMA1                    (0x01 << (E_IRQ_LDM_DMA1            - E_IRQH_START))
#define IRQ_SMART                       (0x01 << (E_IRQ_SMART               - E_IRQH_START))
#define IRQ_UART4                       (0x01 << (E_IRQ_UART4               - E_IRQH_START))
#define IRQ_OTG                         (0x01 << (E_IRQ_OTG                 - E_IRQH_START))
#define IRQ_MIIC4                       (0x01 << (E_IRQ_MIIC4               - E_IRQH_START))
#define IRQ_SCM                         (0x01 << (E_IRQ_SCM                 - E_IRQH_START))
#define IRQ_VBI                         (0x01 << (E_IRQ_VBI                 - E_IRQH_START))
#define IRQ_MVD2ARM                     (0x01 << (E_IRQ_MVD2ARM             - E_IRQH_START))
#define IRQ_GPD                         (0x01 << (E_IRQ_GPD                 - E_IRQH_START))
#define IRQ_ADCDVI2RIU                  (0x01 << (E_IRQ_ADCDVI2RIU          - E_IRQH_START))

// #define REG_IRQEXP_PENDING_L
#define IRQEXPL_MASK                    0xFFFF
#define IRQEXPL_HVD                     (0x01 << (E_IRQEXPL_HVD             - E_IRQEXPL_START))
#define IRQEXPL_USB1                    (0x01 << (E_IRQEXPL_USB1            - E_IRQEXPL_START))
#define IRQEXPL_UHC1                    (0x01 << (E_IRQEXPL_UHC1            - E_IRQEXPL_START))
#define IRQEXPL_SECEMAC                 (0x01 << (E_IRQEXPL_SECEMAC         - E_IRQEXPL_START))
#define IRQEXPL_USB2                    (0x01 << (E_IRQEXPL_USB2            - E_IRQEXPL_START))
#define IRQEXPL_UHC2                    (0x01 << (E_IRQEXPL_UHC2            - E_IRQEXPL_START))
#define IRQEXPL_AEON2HI                 (0x01 << (E_IRQEXPL_AEON2HI         - E_IRQEXPL_START))
#define IRQEXPL_UART1                   (0x01 << (E_IRQEXPL_UART1           - E_IRQEXPL_START))
#define IRQEXPL_UART2                   (0x01 << (E_IRQEXPL_UART2           - E_IRQEXPL_START))
#define IRQEXPL_MSPI1                   (0x01 << (E_IRQEXPL_MSPI1           - E_IRQEXPL_START))
#define IRQEXPL_MIU                     (0x01 << (E_IRQEXPL_MIU             - E_IRQEXPL_START))
#define IRQEXPL_DIPW                    (0x01 << (E_IRQEXPL_DIPW            - E_IRQEXPL_START))
#define IRQEXPL_MIIC2                   (0x01 << (E_IRQEXPL_MIIC2           - E_IRQEXPL_START))
#define IRQEXPL_JPD                     (0x01 << (E_IRQEXPL_JPD             - E_IRQEXPL_START))
#define IRQEXPL_PM_IRQ_OUT              (0x01 << (E_IRQEXPL_PM_IRQ_OUT      - E_IRQEXPL_START))
#define IRQEXPL_MFE                     (0x01 << (E_IRQEXPL_MFE             - E_IRQEXPL_START))

// #define REG_IRQEXP_PENDING_H
#define IRQEXPH_MASK                    0xFFFF
#define E_IRQEXPH_BDMA_MERGE            (0x01 << (E_IRQEXPH_BDMA_MERGE      - E_IRQEXPH_START))
#define E_IRQEXPH_UART3                 (0x01 << (E_IRQEXPH_UART3           - E_IRQEXPH_START))
#define IRQEXPH_UART2MCU                (0x01 << (E_IRQEXPH_UART2MCU        - E_IRQEXPH_START))
#define IRQEXPH_URDMA2MCU               (0x01 << (E_IRQEXPH_UARTDMA2MCU     - E_IRQEXPH_START))
#define IRQEXPH_DVI_HDMI_HDCP           (0x01 << (E_IRQEXPH_HDMITX_IRQ      - E_IRQEXPH_START))
#define IRQEXPH_EXT_GPIO6               (0x01 << (E_IRQEXPH_G3D2MCU         - E_IRQEXPH_START))
#define IRQEXPH_CEC                     (0x01 << (E_IRQEXPH_CEC             - E_IRQEXPH_START))
#define IRQEXPH_HDCP_IIC                (0x01 << (E_IRQEXPH_HDCP_IIC        - E_IRQEXPH_START))
#define IRQEXPH_HDCP_X74                (0x01 << (E_IRQEXPH_HDCP_X74        - E_IRQEXPH_START))
#define IRQEXPH_WADR_ERR_INT            (0x01 << (E_IRQEXPH_WADR_ERR_INT    - E_IRQEXPH_START))
#define IRQEXPH_GE                      (0x01 << (E_IRQEXPH_GE              - E_IRQEXPH_START))
#define IRQEXPH_SDIO                    (0x01 << (E_IRQEXPH_SDIO            - E_IRQEXPH_START))
#define IRQEXPH_CMDQ                    (0x01 << (E_IRQEXPH_CMDQ            - E_IRQEXPH_START))
#define IRQEXPH_MIIC1                   (0x01 << (E_IRQEXPH_MIIC1           - E_IRQEXPH_START))
#define IRQEXPH_IRQ_OUT_PLUG            (0x01 << (E_IRQEXPH_IRQ_OUT_PLUG    - E_IRQEXPH_START))
#define IRQEXPH_MIIC0                   (0x01 << (E_IRQEXPH_MIIC0           - E_IRQEXPH_START))

// #define REG_FIQEXP_PENDING_L
#define FIQEXPL_MASK                    0xFFFF
#define FIQEXPL_TEMPERATURE_RISE        (0x01 << (E_FIQEXPL_TEMPERATURE_RISE    - E_FIQEXPL_START))
#define FIQEXPL_AU_DMA_BUF_INT          (0x01 << (E_FIQEXPL_AU_DMA_BUF_INT      - E_FIQEXPL_START))
#define FIQ_IR                          (0x01 << (E_FIQ_IR               - E_FIQEXPL_START))
#define FIQEXPL_PM_SD_CDZ1              (0x01 << (E_FIQEXPL_PM_SD_CDZ1          - E_FIQEXPL_START))
#define FIQEXPL_8051_TO_SECURER2        (0x01 << (E_FIQEXPL_8051_TO_SECURER2    - E_FIQEXPL_START))
#define FIQEXPL_8051_TO_ARMC1           (0x01 << (E_FIQEXPL_8051_TO_ARMC1       - E_FIQEXPL_START))
#define FIQEXPL_8051_TO_ARM             (0x01 << (E_FIQEXPL_8051_TO_ARM         - E_FIQEXPL_START))
#define FIQEXPL_EXT_GPIO0               (0x01 << (E_FIQEXPL_EXT_GPIO0           - E_FIQEXPL_START))
#define FIQEXPL_ARM_TO_SECURE_R2        (0x01 << (E_FIQEXPL_ARM_TO_SECURE_R2    - E_FIQEXPL_START))
#define FIQEXPL_CA9_SCUEVABORT          (0x01 << (E_FIQEXPL_CA9_SCUEVABORT      - E_FIQEXPL_START))
#define FIQEXPL_ARM_TO_8051             (0x01 << (E_FIQEXPL_ARM_TO_8051         - E_FIQEXPL_START))
#define FIQEXPL_EXT_GPIO1               (0x01 << (E_FIQEXPL_EXT_GPIO1           - E_FIQEXPL_START))
#define FIQEXPL_ARMC1_TO_SECURER2       (0x01 << (E_FIQEXPL_ARMC1_TO_SECURER2   - E_FIQEXPL_START))
#define FIQEXPL_TIMER2                  (0x01 << (E_FIQEXPL_TIMER2              - E_FIQEXPL_START))
#define FIQEXPL_ARMC1_TO_8051           (0x01 << (E_FIQEXPL_ARMC1_TO_8051       - E_FIQEXPL_START))
#define FIQEXPL_EXT_GPIO2               (0x01 << (E_FIQEXPL_EXT_GPIO2           - E_FIQEXPL_START))

// #define REG_FIQEXP_PENDING_H
#define FIQEXPL_SECURE_R2_TO_ARMC1      (0x01 << (E_FIQEXPL_SECURE_R2_TO_ARMC1  - E_FIQEXPH_START))
#define FIQEXPL_SECURE_R2_TO_ARM        (0x01 << (E_FIQEXPL_SECURE_R2_TO_ARM    - E_FIQEXPH_START))
#define FIQEXPL_SECURE_R2_TO_8051       (0x01 << (E_FIQEXPL_SECURE_R2_TO_8051   - E_FIQEXPH_START))
#define FIQEXPH_USB1                    (0x01 << (E_FIQEXPH_USB1                - E_FIQEXPH_START))
#define FIQEXPH_UHC1                    (0x01 << (E_FIQEXPH_UHC1                - E_FIQEXPH_START))
#define FIQEXPH_USB2                    (0x01 << (E_FIQEXPH_USB2                - E_FIQEXPH_START))
#define FIQEXPH_UHC2                    (0x01 << (E_FIQEXPH_UHC2                - E_FIQEXPH_START))
#define FIQEXPH_EXT_GPIO3               (0x01 << (E_FIQEXPH_EXT_GPIO3           - E_FIQEXPH_START))
#define FIQEXPH_EXT_GPIO4               (0x01 << (E_FIQEXPH_EXT_GPIO4           - E_FIQEXPH_START))
#define FIQEXPH_EXT_GPIO5               (0x01 << (E_FIQEXPH_EXT_GPIO5           - E_FIQEXPH_START))
#define FIQEXPH_EXT_GPIO6               (0x01 << (E_FIQEXPH_EXT_GPIO6           - E_FIQEXPH_START))
#define FIQEXPH_VE_SW                   (0x01 << (E_FIQEXPH_VE_SW               - E_FIQEXPH_START))
#define FIQEXPH_PWM_FP_I                (0x01 << (E_FIQEXPH_PWM_FP_I            - E_FIQEXPH_START))
#define FIQEXPH_PWM_RP_R                (0x01 << (E_FIQEXPH_PWM_RP_R            - E_FIQEXPH_START))
#define FIQEXPH_PWM_FP_R                (0x01 << (E_FIQEXPH_PWM_FP_R            - E_FIQEXPH_START))
#define FIQEXPH_EXT_GPIO7               (0x01 << (E_FIQEXPH_EXT_GPIO7           - E_FIQEXPH_START))

#define IRQL_EXP_ALL                    0xFFFF
#define IRQH_EXP_ALL                    0xFFFF
#define FIQL_EXP_ALL                    0xFFFF
#define FIQH_EXP_ALL                    0xFFFF

extern unsigned int interrupt_configs[MSTAR_CHIP_INT_END/16];

void init_chip_spi_config(void);

#endif // #ifndef __CHIP_INT_H__
