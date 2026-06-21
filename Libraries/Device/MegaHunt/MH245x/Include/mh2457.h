// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0
#ifndef MH2457_H
#define MH2457_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* -------------------------  Interrupt Number Definition  ------------------------ */
typedef enum IRQn {
    /* -------------------  Processor Exceptions Numbers  ----------------------------- */
    NonMaskableInt_IRQn   = -14, /*  2 Non Maskable Interrupt */
    HardFault_IRQn        = -13, /*  3 HardFault Interrupt */
    MemoryManagement_IRQn = -12, /*  4 Memory Management Interrupt */
    BusFault_IRQn         = -11, /*  5 Bus Fault Interrupt */
    UsageFault_IRQn       = -10, /*  6 Usage Fault Interrupt */
    SVCall_IRQn           = -5,  /* 11 SV Call Interrupt */
    DebugMonitor_IRQn     = -4,  /* 12 Debug Monitor Interrupt */
    PendSV_IRQn           = -2,  /* 14 PendSV Interrupt */
    SysTick_IRQn          = -1,  /* 15 System Tick Interrupt */

    /* -------------------  Processor Interrupt Numbers  ------------------------------ */
    WWDG_IRQn               = 0,  /*!< Window WatchDog Interrupt                                         */
    PVD_IRQn                = 1,  /*!< PVD through EXTI Line detection Interrupt                         */
    QSPI_IRQn               = 4,  /*!< QSPI Interrupt *                                                  */
    RCC_IRQn                = 5,  /*!< RCC global Interrupt                                              */
    EXTI0_IRQn              = 6,  /*!< EXTI Line0 Interrupt                                              */
    EXTI1_IRQn              = 7,  /*!< EXTI Line1 Interrupt                                              */
    EXTI2_IRQn              = 8,  /*!< EXTI Line2 Interrupt                                              */
    EXTI3_IRQn              = 9,  /*!< EXTI Line3 Interrupt                                              */
    EXTI4_IRQn              = 10, /*!< EXTI Line4 Interrupt                                              */
    DMA1_Stream0_IRQn       = 11, /*!< DMA1 Stream 0 global Interrupt                                    */
    DMA1_Stream1_IRQn       = 12, /*!< DMA1 Stream 1 global Interrupt                                    */
    DMA1_Stream2_IRQn       = 13, /*!< DMA1 Stream 2 global Interrupt                                    */
    DMA1_Stream3_IRQn       = 14, /*!< DMA1 Stream 3 global Interrupt                                    */
    DMA1_Stream4_IRQn       = 15, /*!< DMA1 Stream 4 global Interrupt                                    */
    DMA1_Stream5_IRQn       = 16, /*!< DMA1 Stream 5 global Interrupt                                    */
    DMA1_Stream6_IRQn       = 17, /*!< DMA1 Stream 6 global Interrupt                                    */
    ADC_IRQn                = 18, /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
    CAN1_TX_IRQn            = 19, /*!< CAN1 TX Interrupt                                                 */
    CAN1_RX0_IRQn           = 20, /*!< CAN1 RX0 Interrupt                                                */
    CAN1_RX1_IRQn           = 21, /*!< CAN1 RX1 Interrupt                                                */
    CAN1_SCE_IRQn           = 22, /*!< CAN1 SCE Interrupt                                                */
    EXTI9_5_IRQn            = 23, /*!< External Line[9:5] Interrupts                                     */
    TIM1_BRK_TIM9_IRQn      = 24, /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
    TIM1_UP_TIM10_IRQn      = 25, /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
    TIM1_TRG_COM_TIM11_IRQn = 26, /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
    TIM1_CC_IRQn            = 27, /*!< TIM1 Capture Compare Interrupt                                    */
    TIM2_IRQn               = 28, /*!< TIM2 global Interrupt                                             */
    TIM3_IRQn               = 29, /*!< TIM3 global Interrupt                                             */
    TIM4_IRQn               = 30, /*!< TIM4 global Interrupt                                             */
    I2C1_EV_IRQn            = 31, /*!< I2C1 Event Interrupt                                              */
    I2C1_ER_IRQn            = 32, /*!< I2C1 Error Interrupt                                              */
    I2C2_EV_IRQn            = 33, /*!< I2C2 Event Interrupt                                              */
    I2C2_ER_IRQn            = 34, /*!< I2C2 Error Interrupt                                              */
    SPI1_IRQn               = 35, /*!< SPI1 global Interrupt                                             */
    SPI2_IRQn               = 36, /*!< SPI2 global Interrupt                                             */
    USART1_IRQn             = 37, /*!< USART1 global Interrupt                                           */
    USART2_IRQn             = 38, /*!< USART2 global Interrupt                                           */
    USART3_IRQn             = 39, /*!< USART3 global Interrupt                                           */
    EXTI15_10_IRQn          = 40, /*!< External Line[15:10] Interrupts                                   */
    RTC_Alarm_IRQn          = 41, /*!< RTC Alarm Interrupt                                               */
    EXTI18_IRQn             = 42, /*!<                                                                   */
    TIM8_BRK_TIM12_IRQn     = 43, /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
    TIM8_UP_TIM13_IRQn      = 44, /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
    TIM8_TRG_COM_TIM14_IRQn = 45, /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
    TIM8_CC_IRQn            = 46, /*!< TIM8 Capture Compare Interrupt                                    */
    DMA1_Stream7_IRQn       = 47, /*!< DMA1 Stream7 Interrupt                                            */
    FSMC_IRQn               = 48, /*!< FSMC global Interrupt                                             */
    SDIO_IRQn               = 49, /*!< SDIO global Interrupt                                             */
    TIM5_IRQn               = 50, /*!< TIM5 global Interrupt                                             */
    SPI3_IRQn               = 51, /*!< SPI3 global Interrupt                                             */
    UART4_IRQn              = 52, /*!< UART4 global Interrupt                                            */
    TIM6_DAC_IRQn           = 54, /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
    TIM7_IRQn               = 55, /*!< TIM7 global interrupt                                             */
    DMA2_Stream0_IRQn       = 56, /*!< DMA2 Stream 0 global Interrupt                                    */
    DMA2_Stream1_IRQn       = 57, /*!< DMA2 Stream 1 global Interrupt                                    */
    DMA2_Stream2_IRQn       = 58, /*!< DMA2 Stream 2 global Interrupt                                    */
    DMA2_Stream3_IRQn       = 59, /*!< DMA2 Stream 3 global Interrupt                                    */
    DMA2_Stream4_IRQn       = 60, /*!< DMA2 Stream 4 global Interrupt                                    */
    ETH_IRQn                = 61, /*!< Ethernet global Interrupt                                         */
    ETH_WKUP_IRQn           = 62, /*!< Ethernet Wakeup through EXTI line Interrupt                       */
    CAN2_TX_IRQn            = 63, /*!< CAN2 TX Interrupt                                                 */
    CAN2_RX0_IRQn           = 64, /*!< CAN2 RX0 Interrupt                                                */
    CAN2_RX1_IRQn           = 65, /*!< CAN2 RX1 Interrupt                                                */
    CAN2_SCE_IRQn           = 66, /*!< CAN2 SCE Interrupt                                                */
    USB1_IRQn               = 67, /*!< USB1 global Interrupt                                             */
    DMA2_Stream5_IRQn       = 68, /*!< DMA2 Stream 5 global interrupt                                    */
    DMA2_Stream6_IRQn       = 69, /*!< DMA2 Stream 6 global interrupt                                    */
    DMA2_Stream7_IRQn       = 70, /*!< DMA2 Stream 7 global interrupt                                    */
    USART6_IRQn             = 71, /*!< USART6 global interrupt                                           */
    I2C3_EV_IRQn            = 72, /*!< I2C3 event interrupt                                              */
    I2C3_ER_IRQn            = 73, /*!< I2C3 error interrupt                                              */
    EXTI20_IRQn             = 76, /*!<                                                                   */
    DCMI_IRQn               = 78, /*!< DCMI global interrupt                                             */
    HASH_RNG_IRQn           = 80, /*!< Hash and Rng global interrupt                                     */
    FPU_IRQn                = 81, /*!< FPU global interrupt                                              */
    UART7_IRQn              = 82,
    SPI4_IRQn               = 84,
    LTDC_IRQn               = 88,
    DMA2D_IRQn              = 90,
    DSI_IRQn                = 92,
    HSPI_IRQn               = 96,
    JPEGD_IRQn              = 98,
    BUSERROR_IRQn           = 109,
} IRQn_Type;

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Using anonymous unions and disabling warnings  ----------------------- */
// clang-format off
#if defined (__CC_ARM)
    #pragma anon_unions
    #pragma diag_suppress 94 // Suppress Error 94: the size of an array must be greater than zero
#elif defined (__ICCARM__)
    #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic ignored "-Wc11-extensions"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
    /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
    /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
    #pragma warning 586
#elif defined (__CSMC__)
    /* anonymous unions are enabled by default */
#else
    #warning Not supported compiler type
#endif
// clang-format on

/* --------  Configuration of Core Peripherals  ----------------------------------- */
// Core Feature
#define __CM4_REV              0x0001U /* Core revision r0p1 */
#define __MPU_PRESENT          1U      /* MPU present */
#define __NVIC_PRIO_BITS       4U      /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig 0U      /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT          1U      /* FPU present */

// Core Configuration
#ifndef __VTOR_PRESENT
#define __VTOR_PRESENT 1U /* VTOR present */
#endif

// Standard C Library
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// CMSIS Library
#include "core_cm4.h"      /* Processor and core peripherals */
#include "system_mh2457.h" /* System Header */

/* --------  Legacy typedef  ------------------------------------------------------ */
typedef enum {
    RESET = 0,
    SET   = !RESET
} FlagStatus, ITStatus;

typedef enum {
    DISABLE = 0,
    ENABLE  = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {
    FALSE = 0,
    TRUE  = !FALSE
} Boolean;

typedef enum {
    ERROR   = 0,
    SUCCESS = !ERROR
} ErrorStatus;

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32; /*!< Read Only */
typedef const int16_t sc16; /*!< Read Only */
typedef const int8_t  sc8;  /*!< Read Only */
typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32; /*!< Read Only */
typedef __I int16_t vsc16; /*!< Read Only */
typedef __I int8_t  vsc8;  /*!< Read Only */
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;

typedef const uint32_t uc32; /*!< Read Only */
typedef const uint16_t uc16; /*!< Read Only */
typedef const uint8_t  uc8;  /*!< Read Only */
typedef __IO uint32_t  vu32;
typedef __IO uint16_t  vu16;
typedef __IO uint8_t   vu8;

typedef __I uint32_t vuc32; /*!< Read Only */
typedef __I uint16_t vuc16; /*!< Read Only */
typedef __I uint8_t  vuc8;  /*!< Read Only */

/* --------  Common Macros  ------------------------------------------------------- */

#define BIT0  (0x00000001U)
#define BIT1  (0x00000002U)
#define BIT2  (0x00000004U)
#define BIT3  (0x00000008U)
#define BIT4  (0x00000010U)
#define BIT5  (0x00000020U)
#define BIT6  (0x00000040U)
#define BIT7  (0x00000080U)
#define BIT8  (0x00000100U)
#define BIT9  (0x00000200U)
#define BIT10 (0x00000400U)
#define BIT11 (0x00000800U)
#define BIT12 (0x00001000U)
#define BIT13 (0x00002000U)
#define BIT14 (0x00004000U)
#define BIT15 (0x00008000U)
#define BIT16 (0x00010000U)
#define BIT17 (0x00020000U)
#define BIT18 (0x00040000U)
#define BIT19 (0x00080000U)
#define BIT20 (0x00100000U)
#define BIT21 (0x00200000U)
#define BIT22 (0x00400000U)
#define BIT23 (0x00800000U)
#define BIT24 (0x01000000U)
#define BIT25 (0x02000000U)
#define BIT26 (0x04000000U)
#define BIT27 (0x08000000U)
#define BIT28 (0x10000000U)
#define BIT29 (0x20000000U)
#define BIT30 (0x40000000U)
#define BIT31 (0x80000000U)

#define BIT(n)                 (1UL << (n))
#define BITS(offsetH, offsetL) ((0xFFFFFFFFU >> (31 - offsetH)) & (0xFFFFFFFFU << (offsetL)))
#define BITM(width)            (0xFFFFFFFFU >> (32 - (width)))
#define BITW(offset, width)    (BITM(width) << offset)

#define LSB(n) ((n) & 0xff)
#define MSB(n) (((n) & 0xff00) >> 8)

#define SET_BIT(REG, BIT)                   ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)                 ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)                  ((REG) & (BIT))
#define CLEAR_REG(REG)                      ((REG) = (0x0))
#define WRITE_REG(REG, VAL)                 ((REG) = (VAL))
#define READ_REG(REG)                       ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */
/* --------  Clock Node List * ---------------------------------------------------- */

typedef enum {
    ClockNodeICS, // Internal Clock Source: 12M
    ClockNodeECS, // External Clock Source: 12M
    ClockNodeSRC, // Source Clock Source Selector: {ClockNodeICS, ClockNodeECS}

    ClockNodePLL1G, // Main PLL Gate G
    ClockNodePLL1M, // Main PLL Prescaler M: [2, 63]
    ClockNodePLL1,  // Main PLL Multiplier N: [50, 511]
    ClockNodePLL1P, // Main PLL Divider P: {1, 2, 4, 6, 8}
    ClockNodePLL1Q, // Main PLL Divider Q: [2, 15]
    ClockNodePLL1R, // Main PLL Divider R: [2, 15]

    ClockNodeSYSS,                // System Clock Selector: {ClockNodeICS, ClockNodeECS, ClockNodePLL1P}
    ClockNodeSYS,                 // System Clock Divider: {1, 2, 4, 8, 16 ... 512}
    ClockNodeAHB,                 // AHB Divider: {1, 2, 4, 8, 16 ... 512}
    ClockNodeAHB1 = ClockNodeAHB, // AHB1
    ClockNodeAHB2,                // AHB2
    ClockNodeAHB3,                // AHB3
    ClockNodeAHB4,                // AHB4
    ClockNodeAPB,
    ClockNodeAPB1 = ClockNodeAPB, // APB1 Divider: {1, 2, 4, 8, 16 ... 512}
    ClockNodeAPB2,                // APB2 Divider: {1, 2, 4, 8, 16 ... 512}

    ClockNodePLL2G, // PLL 2 Gate G
    ClockNodePLL2,  // PLL 2 Multiplier N: [50, 511]
    ClockNodePLL2P, // PLL 2 Divider P: {1, 2, 4, 6, 8}
    ClockNodePLL2Q, // PLL 2 Divider Q: [2, 15]
    ClockNodePLL2R, // PLL 2 Divider R: [2, 15]

    ClockNodePLL3G, // PLL 3 Gate G
    ClockNodePLL3,  // PLL 3 Multiplier Loop
    ClockNodePLL3I, // PLL 3 Divider I(2S)
    ClockNodePLL3E, // PLL 3 Divider E(xternal Reference Clock)
    ClockNodePLL3N, // PLL 3 Divider N(FC)
    ClockNodePLL3R, // PLL 3 Divider R(eference) Clock (48M)
    ClockNodePLL3U, // PLL 3 Divider U(ART) Clock (88.4736M)

    ClockNodeI2SE, // I2S E(xternal) Clock Source

    ClockNodeI2S,  // I2S Selector: {ClockNodePLL3I, ClockNodeI2SE}
    ClockNodeDPC,  // Display Pixel Clock Selector: {ClockNodePLL2R, ClockNodePLL2Q, ClockNodePLL2P, ClockNodePLL1R}
    ClockNodeREF,  // Reference Clock (48M) Selector: {ClockNodePLL3R, ClockNodePLL1Q}
                   //   - For SDIO, USB PHY, DSI PHY
    ClockNodeSDIO, // SDIO Selector: {ClockNodeREF, ClockNodeSYS}

    ClockNodeQSPI,  // QSPI Divider: [2, 16]
    ClockNodeSDRAM, // SDRAM Divider: [1, 4]

    ClockNodeCount,
    ClockNodeNull = ClockNodeCount,
} ClockNodeEnum;

/* --------  Peripheral Gate Index ------------------------------------------------ */
// AHB1
#define PERIPH_GATE_GPIOA   0  // EN / LP / RST
#define PERIPH_GATE_GPIOB   1  // EN / LP / RST
#define PERIPH_GATE_GPIOC   2  // EN / LP / RST
#define PERIPH_GATE_GPIOD   3  // EN / LP / RST
#define PERIPH_GATE_GPIOE   4  // EN / LP / RST
#define PERIPH_GATE_GPIOF   5  // EN / LP / RST
#define PERIPH_GATE_GPIOG   6  // EN / LP / RST
#define PERIPH_GATE_GPIOH   7  // EN / LP / RST
#define PERIPH_GATE_GPIOI   8  // EN / LP / RST
#define PERIPH_GATE_JPEGD   10 // EN / LP / RST
#define PERIPH_GATE_CRC     12 // EN / LP / RST
#define PERIPH_GATE_QSPI    15 //    / LP /
#define PERIPH_GATE_SRAM1   16 //    / LP /
#define PERIPH_GATE_SRAM2   17 //    / LP /
#define PERIPH_GATE_SRAM3   18 //    / LP /
#define PERIPH_GATE_SRAM4   19 //    / LP /
#define PERIPH_GATE_SRAM5   20 //    / LP /
#define PERIPH_GATE_CCM     20 // EN / LP /
#define PERIPH_GATE_ETH     25 // EN / LP / RST
#define PERIPH_GATE_ETH_TX  26 // EN / LP /
#define PERIPH_GATE_ETH_RX  27 // EN / LP /
#define PERIPH_GATE_ETH_PTP 28 // EN / LP /
#define PERIPH_GATE_GLB     31 //    /    / RST

// AHB2
#define PERIPH_GATE_FSMC 0  // EN / LP / RST
#define PERIPH_GATE_BPU  2  // EN /    /
#define PERIPH_GATE_OTP  5  // EN / LP /
#define PERIPH_GATE_HSPI 6  // EN / LP / RST
#define PERIPH_GATE_DCMI 8  // EN / LP / RST
#define PERIPH_GATE_TRNG 9  // EN / LP / RST
#define PERIPH_GATE_USB1 10 // EN / LP / RST

// AHB3
#define PERIPH_GATE_SDRAM 0 // EN / LP / RST

// AHB4
#define PERIPH_GATE_DMA1  0 // EN / LP / RST
#define PERIPH_GATE_DMA2  1 // EN / LP / RST
#define PERIPH_GATE_DMA2D 2 // EN / LP / RST
#define PERIPH_GATE_LTDC  4 // EN / LP / RST

// APB1
#define PERIPH_GATE_TIM2   0  // EN / LP / RST
#define PERIPH_GATE_TIM3   1  // EN / LP / RST
#define PERIPH_GATE_TIM4   2  // EN / LP / RST
#define PERIPH_GATE_TIM5   3  // EN / LP / RST
#define PERIPH_GATE_TIM6   4  // EN / LP / RST
#define PERIPH_GATE_TIM7   5  // EN / LP / RST
#define PERIPH_GATE_TIM12  6  // EN / LP / RST
#define PERIPH_GATE_TIM13  7  // EN / LP / RST
#define PERIPH_GATE_TIM14  8  // EN / LP / RST
#define PERIPH_GATE_WWDG   11 // EN / LP / RST
#define PERIPH_GATE_IWDG   11 //    /    /
#define PERIPH_GATE_SPI2   14 // EN / LP / RST
#define PERIPH_GATE_I2S2   14 // EN / LP / RST
#define PERIPH_GATE_SPI3   15 // EN / LP / RST
#define PERIPH_GATE_I2S3   15 // EN / LP / RST
#define PERIPH_GATE_USART2 17 // EN / LP / RST
#define PERIPH_GATE_USART3 18 // EN / LP / RST
#define PERIPH_GATE_UART4  19 // EN / LP / RST
#define PERIPH_GATE_I2C1   21 // EN / LP / RST
#define PERIPH_GATE_I2C2   22 // EN / LP / RST
#define PERIPH_GATE_I2C3   23 // EN / LP / RST
#define PERIPH_GATE_CAN1   25 // EN / LP / RST
#define PERIPH_GATE_CAN2   26 // EN / LP / RST
#define PERIPH_GATE_PWR    28 // EN / LP / RST
#define PERIPH_GATE_DAC    29 // EN / LP / RST
#define PERIPH_GATE_UART7  30 // EN / LP / RST

// APB2
#define PERIPH_GATE_TIM1   0  // EN / LP / RST
#define PERIPH_GATE_TIM8   1  // EN / LP / RST
#define PERIPH_GATE_USART1 4  // EN / LP / RST
#define PERIPH_GATE_USART6 5  // EN / LP / RST
#define PERIPH_GATE_ADC1   8  // EN / LP / RST
#define PERIPH_GATE_ADC2   9  // EN / LP /
#define PERIPH_GATE_ADC3   10 // EN / LP /
#define PERIPH_GATE_SDIO   11 // EN / LP / RST
#define PERIPH_GATE_SPI1   12 // EN / LP / RST
#define PERIPH_GATE_SPI4   13 // EN / LP / RST
#define PERIPH_GATE_SYSCFG 14 // EN / LP / RST
#define PERIPH_GATE_TIM9   16 // EN / LP / RST
#define PERIPH_GATE_TIM10  17 // EN / LP / RST
#define PERIPH_GATE_TIM11  18 // EN / LP / RST
#define PERIPH_GATE_PCFG   25 // EN /    / RST
#define PERIPH_GATE_DSI    27 // EN / LP / RST

#define PERIPH_GATE_NULL 0

/* --------  Peripheral List  ----------------------------------------------------- */
typedef enum {
    PeripheralGPIOA,
    PeripheralGPIOB,
    PeripheralGPIOC,
    PeripheralGPIOD,
    PeripheralGPIOE,
    PeripheralGPIOF,
    PeripheralGPIOG,
    PeripheralGPIOH,
    PeripheralGPIOI,
    PeripheralJPEGD,
    PeripheralCRC,
    PeripheralETH,
    PeripheralSYS,

    PeripheralFSMC,
    PeripheralOTP,
    PeripheralHSPI,
    PeripheralDCMI,
    PeripheralTRNG,
    PeripheralUSB1,

    PeripheralSDRAM,

    PeripheralDMA1,
    PeripheralDMA2,
    PeripheralDMA2D,
    PeripheralLTDC,

    PeripheralTIM1,
    PeripheralTIM2,
    PeripheralTIM3,
    PeripheralTIM4,
    PeripheralTIM5,
    PeripheralTIM6,
    PeripheralTIM7,
    PeripheralTIM8,
    PeripheralTIM9,
    PeripheralTIM10,
    PeripheralTIM11,
    PeripheralTIM12,
    PeripheralTIM13,
    PeripheralTIM14,

    PeripheralWWDG,
    PeripheralIWDG,

    PeripheralUSART1,
    PeripheralUSART2,
    PeripheralUSART3,
    PeripheralUSART6,
    PeripheralUART4,
    PeripheralUART7,

    PeripheralI2C1,
    PeripheralI2C2,
    PeripheralI2C3,
    PeripheralCAN1,
    PeripheralCAN2,
    PeripheralDAC,
    PeripheralADC,
    PeripheralADC1,
    PeripheralADC2,
    PeripheralADC3,

    PeripheralSDIO,
    PeripheralSPI1,
    PeripheralSPI2,
    PeripheralSPI3,
    PeripheralSPI4,
    PeripheralI2S2,
    PeripheralI2S3,

    PeripheralPWR,
    PeripheralSYSCFG,
    PeripheralPCFG,
    PeripheralDSI,

    PeripheralEXTI,
    PeripheralCRAM,

    PeripheralCount,
    PeripheralNull = PeripheralCount,
} PeripheralEnum;

#define PeripheralGPIOStart PeripheralGPIOA
#define PeripheralGPIOEnd   PeripheralGPIOI

#define PeripheralDMAStart PeripheralDMA1
#define PeripheralDMAEnd   PeripheralDMA2

#define PeripheralI2CStart PeripheralI2C1
#define PeripheralI2CEnd   PeripheralI2C3

#define PeripheralSPIStart PeripheralSPI1
#define PeripheralSPIEnd   PeripheralSPI4

#define PeripheralCANStart PeripheralCAN1
#define PeripheralCANEnd   PeripheralCAN2

#define PeripheralUARTStart PeripheralUSART1
#define PeripheralUARTEnd   PeripheralUART7

#define PeripheralUSARTStart PeripheralUSART1
#define PeripheralUSARTEnd   PeripheralUSART6

#define PeripheralTIMStart PeripheralTIM1
#define PeripheralTIMEnd   PeripheralTIM14

#define PeripheralUSPIStart PeripheralHSPI
#define PeripheralUSPIEnd   PeripheralHSPI

typedef const struct {
    uint32_t      Base;
    ClockNodeEnum Node : 8;
    uint8_t       Gate;
} PeripheralStruct;

/* --------  Peripheral Features  ------------------------------------------------- */

// PMI Features
#define FEATURE_PERIPH_AHBRSTR ((__IO uint32_t*)(SYSCTRL_BASE + 0x10))
#define FEATURE_PERIPH_APBRSTR ((__IO uint32_t*)(SYSCTRL_BASE + 0x20))
#define FEATURE_PERIPH_AHBENR  ((__IO uint32_t*)(SYSCTRL_BASE + 0x30))
#define FEATURE_PERIPH_APBENR  ((__IO uint32_t*)(SYSCTRL_BASE + 0x40))

// DMA Features
#define FEATURE_DMA_EXPAND_RANGE 1
#define FEATURE_DMA_SUPPORT_LINK 1

// GPIO Features
#define FEATURE_GPIO_GROUP_NUM 9

// USART Features
#define FEATURE_USART_RXFIFO_PRESENT 1

// QSPI Features
#define FEATURE_QSPI_SUPPORT_ADDR4 1
#define FEATURE_QSPI_SUPPORT_SUB_BASE_ADDR 1

// DCMI Features
#define FEATURE_DCMI_FIFOSR_DMACR 1

// LTDC Features
#define FEATURE_LTDC_SUPPORT_BURST 1

// OTP Features
#define FEATURE_OTP_SIZE 0x0400UL

/* --------  Peripheral Registers  ------------------------------------------------ */
/* --------  Peripheral Definitions  ---------------------------------------------- */

typedef enum {
    PA0  = 0x00,
    PA1  = 0x01,
    PA2  = 0x02,
    PA3  = 0x03,
    PA4  = 0x04,
    PA5  = 0x05,
    PA6  = 0x06,
    PA7  = 0x07,
    PA8  = 0x08,
    PA9  = 0x09,
    PA10 = 0x0A,
    PA11 = 0x0B,
    PA12 = 0x0C,
    PA13 = 0x0D,
    PA14 = 0x0E,
    PA15 = 0x0F,

    PB0  = 0x10,
    PB1  = 0x11,
    PB2  = 0x12,
    PB3  = 0x13,
    PB4  = 0x14,
    PB5  = 0x15,
    PB6  = 0x16,
    PB7  = 0x17,
    PB8  = 0x18,
    PB9  = 0x19,
    PB10 = 0x1A,
    PB11 = 0x1B,
    PB12 = 0x1C,
    PB13 = 0x1D,
    PB14 = 0x1E,
    PB15 = 0x1F,

    PC0  = 0x20,
    PC1  = 0x21,
    PC2  = 0x22,
    PC3  = 0x23,
    PC4  = 0x24,
    PC5  = 0x25,
    PC6  = 0x26,
    PC7  = 0x27,
    PC8  = 0x28,
    PC9  = 0x29,
    PC10 = 0x2A,
    PC11 = 0x2B,
    PC12 = 0x2C,
    PC13 = 0x2D,
    PC14 = 0x2E,
    PC15 = 0x2F,

    PD0  = 0x30,
    PD1  = 0x31,
    PD2  = 0x32,
    PD3  = 0x33,
    PD4  = 0x34,
    PD5  = 0x35,
    PD6  = 0x36,
    PD7  = 0x37,
    PD8  = 0x38,
    PD9  = 0x39,
    PD10 = 0x3A,
    PD11 = 0x3B,
    PD12 = 0x3C,
    PD13 = 0x3D,
    PD14 = 0x3E,
    PD15 = 0x3F,

    PE0  = 0x40,
    PE1  = 0x41,
    PE2  = 0x42,
    PE3  = 0x43,
    PE4  = 0x44,
    PE5  = 0x45,
    PE6  = 0x46,
    PE7  = 0x47,
    PE8  = 0x48,
    PE9  = 0x49,
    PE10 = 0x4A,
    PE11 = 0x4B,
    PE12 = 0x4C,
    PE13 = 0x4D,
    PE14 = 0x4E,
    PE15 = 0x4F,

    PF0  = 0x50,
    PF1  = 0x51,
    PF2  = 0x52,
    PF3  = 0x53,
    PF4  = 0x54,
    PF5  = 0x55,
    PF6  = 0x56,
    PF7  = 0x57,
    PF8  = 0x58,
    PF9  = 0x59,
    PF10 = 0x5A,
    PF11 = 0x5B,
    PF12 = 0x5C,
    PF13 = 0x5D,
    PF14 = 0x5E,
    PF15 = 0x5F,

    PG0  = 0x60,
    PG1  = 0x61,
    PG2  = 0x62,
    PG3  = 0x63,
    PG4  = 0x64,
    PG5  = 0x65,
    PG6  = 0x66,
    PG7  = 0x67,
    PG8  = 0x68,
    PG9  = 0x69,
    PG10 = 0x6A,
    PG11 = 0x6B,
    PG12 = 0x6C,
    PG13 = 0x6D,
    PG14 = 0x6E,
    PG15 = 0x6F,

    PH0  = 0x70,
    PH1  = 0x71,
    PH2  = 0x72,
    PH3  = 0x73,
    PH4  = 0x74,
    PH5  = 0x75,
    PH6  = 0x76,
    PH7  = 0x77,
    PH8  = 0x78,
    PH9  = 0x79,
    PH10 = 0x7A,
    PH11 = 0x7B,
    PH12 = 0x7C,
    PH13 = 0x7D,
    PH14 = 0x7E,
    PH15 = 0x7F,

    PI0  = 0x80,
    PI1  = 0x81,
    PI2  = 0x82,
    PI3  = 0x83,
    PI4  = 0x84,
    PI5  = 0x85,
    PI6  = 0x86,
    PI7  = 0x87,
    PI8  = 0x88,
    PI9  = 0x89,
    PI10 = 0x8A,
    PI11 = 0x8B,
    PI12 = 0x8C,
    PI13 = 0x8D,
    PI14 = 0x8E,
    PI15 = 0x8F,

    IONone = 0xFF,
} IOEnum;

// GPIO Alternat Function

//  AF 0 selection
#define GPIO_AF_MCO   ((uint8_t)0x00) /* MCO (MCO1 and MCO2) Alternate Function mapping */
#define GPIO_AF_SWJ   ((uint8_t)0x00) /* SWJ (SWD and JTAG) Alternate Function mapping */
#define GPIO_AF_SDRAM ((uint8_t)0x00) /* SDRAM Alternate Function mapping */

// AF 1 selection
#define GPIO_AF_TIM1  ((uint8_t)0x01) /* TIM1 Alternate Function mapping */
#define GPIO_AF_TIM2  ((uint8_t)0x01) /* TIM2 Alternate Function mapping */
#define GPIO_AF1_HSPI ((uint8_t)0x01) /* HSPI Alternate Function mapping */

// AF 2 selection
#define GPIO_AF_TIM3  ((uint8_t)0x02) /* TIM3 Alternate Function mapping */
#define GPIO_AF_TIM4  ((uint8_t)0x02) /* TIM4 Alternate Function mapping */
#define GPIO_AF_TIM5  ((uint8_t)0x02) /* TIM5 Alternate Function mapping */
#define GPIO_AF2_HSPI ((uint8_t)0x02) /* HSPI Alternate Function mapping */

// AF 3 selection
#define GPIO_AF_TIM8  ((uint8_t)0x03) /* TIM8 Alternate Function mapping */
#define GPIO_AF_TIM9  ((uint8_t)0x03) /* TIM9 Alternate Function mapping */
#define GPIO_AF_TIM10 ((uint8_t)0x03) /* TIM10 Alternate Function mapping */
#define GPIO_AF_TIM11 ((uint8_t)0x03) /* TIM11 Alternate Function mapping */
#define GPIO_AF3_HSPI ((uint8_t)0x03) /* HSPI Alternate Function mapping */

// AF 4 selection
#define GPIO_AF_I2C1 ((uint8_t)0x04) /* I2C1 Alternate Function mapping */
#define GPIO_AF_I2C2 ((uint8_t)0x04) /* I2C2 Alternate Function mapping */
#define GPIO_AF_I2C3 ((uint8_t)0x04) /* I2C3 Alternate Function mapping */

// AF 5 selection
#define GPIO_AF_SPI1  ((uint8_t)0x05) /* SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF_SPI2  ((uint8_t)0x05) /* SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF5_SPI3 ((uint8_t)0x05) /* SPI3/I2S3 Alternate Function mapping */
#define GPIO_AF_SPI4  ((uint8_t)0x05) /* SPI4/I2S4 Alternate Function mapping */

// AF 6 selection
#define GPIO_AF_SPI3    ((uint8_t)0x06) /* SPI3/I2S3 Alternate Function mapping  */
#define GPIO_AF6_SPI1   ((uint8_t)0x06) /* SPI1 Alternate Function mapping       */
#define GPIO_AF6_SPI2   ((uint8_t)0x06) /* SPI2 Alternate Function mapping       */
#define GPIO_AF6_SPI4   ((uint8_t)0x06) /* SPI4 Alternate Function mapping       */
#define GPIO_AF6_HSPI   ((uint8_t)0x06) /* Quad-SPI Alternate Function mapping   */
#define GPIO_AF_I2S2ext ((uint8_t)0x06) /* I2S2ext_SD Alternate Function mapping */

// AF 7 selection
#define GPIO_AF_USART1  ((uint8_t)0x07) /* USART1 Alternate Function mapping  */
#define GPIO_AF_USART2  ((uint8_t)0x07) /* USART2 Alternate Function mapping  */
#define GPIO_AF_USART3  ((uint8_t)0x07) /* USART3 Alternate Function mapping  */
#define GPIO_AF7_SPI2   ((uint8_t)0x07) /* SPI2/I2S2ext Alternate Function mapping */
#define GPIO_AF7_SPI3   ((uint8_t)0x07) /* SPI3/I2S3ext Alternate Function mapping */
#define GPIO_AF_I2S3ext GPIO_AF7_SPI3

// AF 8 selection
#define GPIO_AF_UART4   ((uint8_t)0x08) /* UART4 Alternate Function mapping  */
#define GPIO_AF_USART6  ((uint8_t)0x08) /* USART6 Alternate Function mapping */
#define GPIO_AF_UART7   ((uint8_t)0x08) /* UART7 Alternate Function mapping  */
#define GPIO_AF8_USART2 ((uint8_t)0x08) /* UART2 Alternate Function mapping  */

// AF 9 selection
#define GPIO_AF_CAN1  ((uint8_t)0x09) /* CAN1 Alternate Function mapping  */
#define GPIO_AF_CAN2  ((uint8_t)0x09) /* CAN2 Alternate Function mapping  */
#define GPIO_AF_TIM12 ((uint8_t)0x09) /* TIM12 Alternate Function mapping */
#define GPIO_AF_TIM13 ((uint8_t)0x09) /* TIM13 Alternate Function mapping */
#define GPIO_AF_TIM14 ((uint8_t)0x09) /* TIM14 Alternate Function mapping */
#define GPIO_AF9_I2C2 ((uint8_t)0x09) /* I2C2 Alternate Function mapping  */
#define GPIO_AF9_I2C3 ((uint8_t)0x09) /* I2C3 Alternate Function mapping */
#define GPIO_AF9_LTDC ((uint8_t)0x09) /* LTDC Alternate Function mapping */

// AF 10 selection
#define GPIO_AF_USB    ((uint8_t)0xA) /* USB  Alternate Function mapping */
#define GPIO_AF10_FSMC ((uint8_t)0xA) /* FSMC Alternate Function mapping */

// AF 11 selection
#define GPIO_AF_ETH ((uint8_t)0x0B) /* ETHERNET Alternate Function mapping */

// AF 12 selection
#define GPIO_AF_FSMC ((uint8_t)0xC) /* FSMC Alternate Function mapping */
#define GPIO_AF_SDIO ((uint8_t)0xC) /* SDIO Alternate Function mapping */

// AF 13 selection
#define GPIO_AF_DCMI   ((uint8_t)0x0D) /* DCMI Alternate Function mapping */
#define GPIO_AF_DSI    ((uint8_t)0x0D) /* DSI Alternate Function mapping */
#define GPIO_AF13_LTDC ((uint8_t)0x0D) /* LTDC Alternate Function mapping */

// AF 14 selection
#define GPIO_AF_LTDC ((uint8_t)0x0E) /* LCD-TFT Alternate Function mapping */

// AF 15 selection
#define GPIO_AF_EVENTOUT ((uint8_t)0x0F) /* EVENTOUT Alternate Function mapping */

/* --------  Peripheral Headers  -------------------------------------------------- */
#include "peripheral.h"

/* ========================================================================= */
/* ============     Device Specific Peripheral Address Map      ============ */
/* ========================================================================= */
/* Peripheral and RAM base address */
#define FLASH_BASE  (0x08000000UL) /* FLASH         Base Address */
#define SRAM_BASE   (0x20000000UL) /* SRAM          Base Address */
#define SRAM1_BASE  (0x20000000UL) /* SRAM1(160 KB) Base Address */
#define SRAM2_BASE  (0x20028000UL) /* SRAM2(32  KB) Base Address */
#define SRAM3_BASE  (0x20030000UL) /* SRAM3(832 KB) Base Address */
#define SRAM4_BASE  (0x20100000UL) /* SRAM4(128 KB) Base Address */
#define SRAM5_BASE  (0x20120000UL) /* SRAM5(128 KB) Base Address */
#define PERIPH_BASE (0x40000000UL) /* Peripheral    Base Address */

#define BKPSRAM_BASE (0x500A0200UL) /* Backup SRAM(4 KB) Base Address */

#define SDRAM_MEM_BASE (0x60000000UL)

#define FSMC_MEM_BASE (0x70000000UL)

#define SRAM_BB_BASE   (0x22000000UL) /* SRAM          Bit-Band Base Address */
#define SRAM1_BB_BASE  (0x22000000UL) /* SRAM1(160 KB) Bit-Band Base Address */
#define SRAM2_BB_BASE  (0x22500000UL) /* SRAM2(32  KB) Bit-Band Base Address */
#define SRAM3_BB_BASE  (0x22600000UL) /* SRAM3(832 KB) Bit-Band Base Address */
#define PERIPH_BB_BASE (0x42000000UL) /* Peripheral    Bit-Band Base Address */

#define SRAM_SIZE (0x00140000UL)

/* Peripheral memory map */
#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x10000000)
#define AHB3PERIPH_BASE (PERIPH_BASE + 0x60000000)
#define AHB4PERIPH_BASE (PERIPH_BASE + 0x60002000)

/* APB1 peripherals */
#define TIM2_BASE (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE (APB1PERIPH_BASE + 0x1400)

#define TIM12_BASE   (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASE   (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASE   (APB1PERIPH_BASE + 0x2000)
#define WWDG_BASE    (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE    (APB1PERIPH_BASE + 0x3000)
#define I2S2ext_BASE (APB1PERIPH_BASE + 0x3400)
#define SPI2_BASE    (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE    (APB1PERIPH_BASE + 0x3C00)

#define I2S3ext_BASE (APB1PERIPH_BASE + 0x4000)
#define USART2_BASE  (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE  (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE   (APB1PERIPH_BASE + 0x4C00)
#define I2C1_BASE    (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE    (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE    (APB1PERIPH_BASE + 0x5C00)

#define CAN1_BASE (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE (APB1PERIPH_BASE + 0x6800)

#define PWR_BASE   (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE   (APB1PERIPH_BASE + 0x7400)
#define UART7_BASE (APB1PERIPH_BASE + 0x7800)

/* APB2 peripherals */
#define TIM1_BASE   (APB2PERIPH_BASE + 0x0000)
#define TIM8_BASE   (APB2PERIPH_BASE + 0x0400)
#define USART1_BASE (APB2PERIPH_BASE + 0x1000)
#define USART6_BASE (APB2PERIPH_BASE + 0x1400)
#define ADC1_BASE   (APB2PERIPH_BASE + 0x2000)
#define ADC2_BASE   (APB2PERIPH_BASE + 0x2100)
#define ADC3_BASE   (APB2PERIPH_BASE + 0x2200)
#define ADC_BASE    (APB2PERIPH_BASE + 0x2300)
#define SDIO_BASE   (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE   (APB2PERIPH_BASE + 0x3000)
#define SPI4_BASE   (APB2PERIPH_BASE + 0x3400)
#define SYSCFG_BASE (APB2PERIPH_BASE + 0x3800)
#define EXTI_BASE   (APB2PERIPH_BASE + 0x3C00)
#define TIM9_BASE   (APB2PERIPH_BASE + 0x4000)
#define TIM10_BASE  (APB2PERIPH_BASE + 0x4400)
#define TIM11_BASE  (APB2PERIPH_BASE + 0x4800)
#define PCFG_BASE   (APB2PERIPH_BASE + 0x6400)
#define DSI_BASE    (APB2PERIPH_BASE + 0x6C00)

/*!< AHB1 peripherals */
#define GPIOA_BASE   (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE   (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE   (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE   (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE   (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE   (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE   (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE   (AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASE   (AHB1PERIPH_BASE + 0x2000)
#define JPEGD_BASE   (AHB1PERIPH_BASE + 0x2800)
#define CRC_BASE     (AHB1PERIPH_BASE + 0x3000)
#define SYSCTRL_BASE (AHB1PERIPH_BASE + 0x3800)
#define RCC_BASE     (AHB1PERIPH_BASE + 0x3800)
#define QSPI_BASE    (AHB1PERIPH_BASE + 0x3C00)
#define ICACHE_BASE  (AHB1PERIPH_BASE + 0xC000)
#define DCACHE_BASE  (AHB1PERIPH_BASE + 0xC400)

#define DMA1_BASE         (AHB1PERIPH_BASE + 0x6000)
#define DMA1_Stream0_BASE (DMA1_BASE + 0x010)
#define DMA1_Stream1_BASE (DMA1_BASE + 0x028)
#define DMA1_Stream2_BASE (DMA1_BASE + 0x040)
#define DMA1_Stream3_BASE (DMA1_BASE + 0x058)
#define DMA1_Stream4_BASE (DMA1_BASE + 0x070)
#define DMA1_Stream5_BASE (DMA1_BASE + 0x088)
#define DMA1_Stream6_BASE (DMA1_BASE + 0x0A0)
#define DMA1_Stream7_BASE (DMA1_BASE + 0x0B8)
#define DMA2_BASE         (AHB1PERIPH_BASE + 0x6400)
#define DMA2_Stream0_BASE (DMA2_BASE + 0x010)
#define DMA2_Stream1_BASE (DMA2_BASE + 0x028)
#define DMA2_Stream2_BASE (DMA2_BASE + 0x040)
#define DMA2_Stream3_BASE (DMA2_BASE + 0x058)
#define DMA2_Stream4_BASE (DMA2_BASE + 0x070)
#define DMA2_Stream5_BASE (DMA2_BASE + 0x088)
#define DMA2_Stream6_BASE (DMA2_BASE + 0x0A0)
#define DMA2_Stream7_BASE (DMA2_BASE + 0x0B8)
#define LTDC_BASE         (AHB1PERIPH_BASE + 0xCC00)
#define LTDC_Layer1_BASE  (LTDC_BASE + 0x84)
#define LTDC_Layer2_BASE  (LTDC_BASE + 0x104)
#define ETH_BASE          (AHB1PERIPH_BASE + 0x8000)
#define ETH_MAC_BASE      (ETH_BASE)
#define ETH_MMC_BASE      (ETH_BASE + 0x0100)
#define ETH_PTP_BASE      (ETH_BASE + 0x0700)
#define ETH_DMA_BASE      (ETH_BASE + 0x1000)
#define DMA2D_BASE        (AHB1PERIPH_BASE + 0xB000)

/* AHB2 peripherals */
#define USB1_BASE (AHB2PERIPH_BASE + 0x00000)
#define DCMI_BASE (AHB2PERIPH_BASE + 0x50000)
#define TRNG_BASE (AHB2PERIPH_BASE + 0x60800)
#define HSPI_BASE (AHB2PERIPH_BASE + 0x75400)
#define OTP_BASE  (AHB2PERIPH_BASE + 0x76000)
#define RTC_BASE  (AHB2PERIPH_BASE + 0xA00A0)
/* AHB0 peripherals but in AHB2 peripherals region */

/* AHB3 peripherals */
#define FSMC_R_BASE (AHB3PERIPH_BASE)
/* FSMC Bankx registers base address */
#define FSMC_Bank1_R_BASE  (FSMC_R_BASE + 0x0000)
#define FSMC_Bank1E_R_BASE (FSMC_R_BASE + 0x0104)
#define FSMC_Bank2_R_BASE  (FSMC_R_BASE + 0x0060)
#define FSMC_Bank3_R_BASE  (FSMC_R_BASE + 0x0080)
#define FSMC_Bank4_R_BASE  (FSMC_R_BASE + 0x00A0)
/* AHB0 peripherals but in AHB3 peripherals region */

/* Debug MCU registers base address */
#define DBGMCU_BASE ((uint32_t)0xE0042000)

/* AHB4 peripherals */
#define SDRAM_R_BASE (AHB4PERIPH_BASE)

/* ========================================================================= */
/* ============             Peripheral declaration              ============ */
/* ========================================================================= */
#define GPIOA        ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB        ((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC        ((GPIO_TypeDef*)GPIOC_BASE)
#define GPIOD        ((GPIO_TypeDef*)GPIOD_BASE)
#define GPIOE        ((GPIO_TypeDef*)GPIOE_BASE)
#define GPIOF        ((GPIO_TypeDef*)GPIOF_BASE)
#define GPIOG        ((GPIO_TypeDef*)GPIOG_BASE)
#define GPIOH        ((GPIO_TypeDef*)GPIOH_BASE)
#define GPIOI        ((GPIO_TypeDef*)GPIOI_BASE)
#define JPEGD        ((JPEGD_TypeDef*)JPEGD_BASE)
#define SYSCTRL      ((SYSCTRL_TypeDef*)SYSCTRL_BASE)
#define RCC          ((RCC_TypeDef*)RCC_BASE)
#define QSPI         ((QSPI_TypeDef*)QSPI_BASE)
#define DMA1         ((DMA_TypeDef*)DMA1_BASE)
#define DMA1_Stream0 ((DMA_Stream_TypeDef*)DMA1_Stream0_BASE)
#define DMA1_Stream1 ((DMA_Stream_TypeDef*)DMA1_Stream1_BASE)
#define DMA1_Stream2 ((DMA_Stream_TypeDef*)DMA1_Stream2_BASE)
#define DMA1_Stream3 ((DMA_Stream_TypeDef*)DMA1_Stream3_BASE)
#define DMA1_Stream4 ((DMA_Stream_TypeDef*)DMA1_Stream4_BASE)
#define DMA1_Stream5 ((DMA_Stream_TypeDef*)DMA1_Stream5_BASE)
#define DMA1_Stream6 ((DMA_Stream_TypeDef*)DMA1_Stream6_BASE)
#define DMA1_Stream7 ((DMA_Stream_TypeDef*)DMA1_Stream7_BASE)
#define DMA2         ((DMA_TypeDef*)DMA2_BASE)
#define DMA2_Stream0 ((DMA_Stream_TypeDef*)DMA2_Stream0_BASE)
#define DMA2_Stream1 ((DMA_Stream_TypeDef*)DMA2_Stream1_BASE)
#define DMA2_Stream2 ((DMA_Stream_TypeDef*)DMA2_Stream2_BASE)
#define DMA2_Stream3 ((DMA_Stream_TypeDef*)DMA2_Stream3_BASE)
#define DMA2_Stream4 ((DMA_Stream_TypeDef*)DMA2_Stream4_BASE)
#define DMA2_Stream5 ((DMA_Stream_TypeDef*)DMA2_Stream5_BASE)
#define DMA2_Stream6 ((DMA_Stream_TypeDef*)DMA2_Stream6_BASE)
#define DMA2_Stream7 ((DMA_Stream_TypeDef*)DMA2_Stream7_BASE)
#define DMA2D        ((DMA2D_TypeDef*)DMA2D_BASE)
#define ICACHE       ((CACHE_TypeDef*)ICACHE_BASE)
#define DCACHE       ((CACHE_TypeDef*)DCACHE_BASE)

#define DCMI ((DCMI_TypeDef*)DCMI_BASE)
#define TRNG ((TRNG_TypeDef*)TRNG_BASE)
#define OTP  ((OTP_TypeDef*)OTP_BASE)
#define RTC  ((RTC_TypeDef*)RTC_BASE)

#define HSPI  ((HSPI_TypeDef*)HSPI_BASE)
#define SDRAM ((SDRAM_TypeDef*)SDRAM_R_BASE)

#define PCFG   ((PCFG_TypeDef*)PCFG_BASE)
#define SYSCFG ((SYSCFG_TypeDef*)SYSCFG_BASE)

#define LTDC        ((LTDC_TypeDef*)LTDC_BASE)
#define LTDC_Layer1 ((LTDC_Layer_TypeDef*)LTDC_Layer1_BASE)
#define LTDC_Layer2 ((LTDC_Layer_TypeDef*)LTDC_Layer2_BASE)

#define TIM2    ((TIM_TypeDef*)TIM2_BASE)
#define TIM3    ((TIM_TypeDef*)TIM3_BASE)
#define TIM4    ((TIM_TypeDef*)TIM4_BASE)
#define TIM5    ((TIM_TypeDef*)TIM5_BASE)
#define TIM6    ((TIM_TypeDef*)TIM6_BASE)
#define TIM7    ((TIM_TypeDef*)TIM7_BASE)
#define TIM12   ((TIM_TypeDef*)TIM12_BASE)
#define TIM13   ((TIM_TypeDef*)TIM13_BASE)
#define TIM14   ((TIM_TypeDef*)TIM14_BASE)
#define WWDG    ((WWDG_TypeDef*)WWDG_BASE)
#define IWDG    ((IWDG_TypeDef*)IWDG_BASE)
#define I2S2ext ((SPI_TypeDef*)I2S2ext_BASE)
#define SPI2    ((SPI_TypeDef*)SPI2_BASE)
#define SPI3    ((SPI_TypeDef*)SPI3_BASE)
#define I2S3ext ((SPI_TypeDef*)I2S3ext_BASE)
#define USART2  ((USART_TypeDef*)USART2_BASE)
#define USART3  ((USART_TypeDef*)USART3_BASE)
#define UART4   ((USART_TypeDef*)UART4_BASE)
#define I2C1    ((I2C_TypeDef*)I2C1_BASE)
#define I2C2    ((I2C_TypeDef*)I2C2_BASE)
#define I2C3    ((I2C_TypeDef*)I2C3_BASE)

#define CAN1 ((CAN_TypeDef*)CAN1_BASE)
#define CAN2 ((CAN_TypeDef*)CAN2_BASE)

#define PWR    ((PWR_TypeDef*)PWR_BASE)
#define DAC    ((DAC_TypeDef*)DAC_BASE)
#define UART7  ((USART_TypeDef*)UART7_BASE)
#define TIM1   ((TIM_TypeDef*)TIM1_BASE)
#define TIM8   ((TIM_TypeDef*)TIM8_BASE)
#define USART1 ((USART_TypeDef*)USART1_BASE)
#define USART6 ((USART_TypeDef*)USART6_BASE)
#define ADC    ((ADC_Common_TypeDef*)ADC_BASE)
#define ADC1   ((ADC_TypeDef*)ADC1_BASE)
#define ADC2   ((ADC_TypeDef*)ADC2_BASE)
#define ADC3   ((ADC_TypeDef*)ADC3_BASE)
#define SDIO   ((SDIO_TypeDef*)SDIO_BASE)
#define SPI1   ((SPI_TypeDef*)SPI1_BASE)
#define SPI4   ((SPI_TypeDef*)SPI4_BASE)
#define EXTI   ((EXTI_TypeDef*)EXTI_BASE)
#define TIM9   ((TIM_TypeDef*)TIM9_BASE)
#define TIM10  ((TIM_TypeDef*)TIM10_BASE)
#define TIM11  ((TIM_TypeDef*)TIM11_BASE)
#define CRC    ((CRC_TypeDef*)CRC_BASE)
#define ETH    ((ETH_TypeDef*)ETH_BASE)

#define FSMC_Bank1  ((FSMC_Bank1_TypeDef*)FSMC_Bank1_R_BASE)
#define FSMC_Bank1E ((FSMC_Bank1E_TypeDef*)FSMC_Bank1E_R_BASE)
#define FSMC_Bank2  ((FSMC_Bank2_TypeDef*)FSMC_Bank2_R_BASE)
#define FSMC_Bank3  ((FSMC_Bank3_TypeDef*)FSMC_Bank3_R_BASE)
#define FSMC_Bank4  ((FSMC_Bank4_TypeDef*)FSMC_Bank4_R_BASE)

#define DBGMCU ((DBGMCU_TypeDef*)DBGMCU_BASE)


#include "mh245x_sysctrl.h"
#include "mh245x_pwr.h"
#include "mh245x_cache.h"
#include "mh245x_iwdg.h"
#include "mh245x_wwdg.h"
#include "mh245x_dma.h"
#include "mh245x_dma2d.h"
#include "mh245x_tim.h"
#include "mh245x_gpio.h"
#include "mh245x_exti.h"
#include "mh245x_usart.h"
#include "mh245x_i2c.h"
#include "mh245x_spi.h"
#include "mh245x_qspi.h"
#include "mh245x_hspi.h"
#include "mh245x_sdram.h"
#include "mh245x_sdio.h"
#include "mh245x_dsi.h"
#include "mh245x_dcmi.h"
#include "mh245x_ltdc.h"
#include "mh245x_fsmc.h"
#include "mh245x_jpeg.h"
#include "mh245x_can.h"
#include "mh245x_eth.h"
#include "mh245x_crc.h"
#include "mh245x_trng.h"
#include "mh245x_otp.h"
#include "mh245x_rtc.h"
#include "mh245x_adc.h"
#include "mh245x_dac.h"
#include "mh245x_misc.h"

#ifdef USE_STDPERIPH_DRIVER
#include "mh245x_conf.h"
#endif // USE_STDPERIPH_DRIVER

#ifdef __cplusplus
}
#endif

#endif /* MH2457_H */
