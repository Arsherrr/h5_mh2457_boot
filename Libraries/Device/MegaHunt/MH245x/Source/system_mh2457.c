// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh2457.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

#ifndef CONFIG_SYSCLK
/*!
    @brief System default clock config
    @note  Override the default config by define `CONFIG_SYSCLK` in `mh2457_conf.h`

    @param useECS: if external clock source is used
    @param m: PLL1 M division [2, 63]
    @param n: PLL1 N multiple [50, 511]
    @param p: PLL1 P division {1, 2, 4, 6, 8}
    @param q: PLL1 Q division [2, 15]
    @param r: PLL1 R division [2, 15]
    @param ahb: AHB division {1, 2, 4, 8, 16 ... 512}
    @param apb: APB division {1, 2, 4, 8, 16 ... 512}
 */
#define CONFIG_SYSCLK SYSCLK_300M_ECS
#endif

// SYSCLK internal definitions start

// System Clock Presets:   M   N    P  Q  R
#define SYSCLK_300M     0, 12, 300, 1, 6, 6, 2, 2
#define SYSCLK_300M_ECS 1, 12, 300, 1, 6, 6, 2, 2
#define SYSCLK_288M     0, 12, 288, 1, 6, 6, 2, 2
#define SYSCLK_288M_ECS 1, 12, 288, 1, 6, 6, 2, 2
#define SYSCLK_48M      0, 12, 192, 4, 6, 6, 2, 2

#define __SYSCLK(useECS, m, n, p, q, r, ahb, apb) ((useECS ? HSE_VALUE : HSI_VALUE) / m * n / p)

#define SYSCLK(config...) __SYSCLK(config)
// SYSCLK internal definitions end

#ifndef CONFIG_QSPI_TIMMING

#if SYSCLK(CONFIG_SYSCLK) >= 240000000U
#define CONFIG_QSPI_TIMMING (QSPI_DEVICE_PARA_FREQ_DIV(6) | QSPI_DEVICE_PARA_SAMPLE_PHA)
#elif SYSCLK(CONFIG_SYSCLK) >= 180000000U
#define CONFIG_QSPI_TIMMING (QSPI_DEVICE_PARA_FREQ_DIV(4) | QSPI_DEVICE_PARA_SAMPLE_PHA)
#elif SYSCLK(CONFIG_SYSCLK) >= 120000000U
#define CONFIG_QSPI_TIMMING (QSPI_DEVICE_PARA_FREQ_DIV(2) | QSPI_DEVICE_PARA_SAMPLE_PHA)
#else
#define CONFIG_QSPI_TIMMING (QSPI_DEVICE_PARA_FREQ_DIV(2))
#endif

#endif

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/

uint32_t SystemCoreClock = SYSCLK(CONFIG_SYSCLK);

volatile uint32_t idlingPeriod = SYSCLK(CONFIG_SYSCLK) / (62500 * 4);

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[128];

/*----------------------------------------------------------------------------
  Static Functions
 *----------------------------------------------------------------------------*/

#if defined(__CC_ARM)
__ASM static void QSPIClockConfig(uint32_t qspiParam) {
    mov32 r1, #__cpp(QSPI_BASE + 0x14);
    str   r0, [r1];

    ; // clang-format off
0   ldr   r2, [r1];
    cmp   r2, r0;
    bne   %b0;
    ; // clang-format on

    bx lr;
}
#else
__attribute__((noinline)) static void QSPIClockConfig(uint32_t qspiParam) {
    __ASM volatile( //
        "str  %[rP], [%[rQ]]\n"

        "0:\n"
        "    ldr  r2, [%[rQ]]\n"
        "    cmp  r2, %[rP]\n"
        "    bne  0b\n" //
        : :             //
        [rQ] "r"(QSPI_BASE + 0x14),
        [rP] "r"(qspiParam) //
    );
}
#endif

/*----------------------------------------------------------------------------
  System Functions
 *----------------------------------------------------------------------------*/

void SystemInit(void) {
#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    SCB->VTOR = (uint32_t)&(__VECTOR_TABLE[0]);
#endif

#if defined(__FPU_USED) && (__FPU_USED == 1U)
    SCB->CPACR |= ((3U << 10U * 2U) | /* enable CP10 Full Access */
                   (3U << 11U * 2U)); /* enable CP11 Full Access */
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

    SystemClockConfig(CONFIG_SYSCLK);
}

void SystemCoreClockUpdate(void) {
    SystemCoreClock = ClockGet(ClockNodeSYS);

    idlingPeriod = ((uint32_t)SystemDelayUs >= SRAM_BASE) ? 6 : ((uint32_t)SystemDelayUs <= 0x10000 ? 12 : 4);
    idlingPeriod = SystemCoreClock / 62500 / idlingPeriod;
}

__WEAK void SystemClockConfig(bool useECS, uint8_t m, uint16_t n, uint8_t p, uint8_t q, uint8_t r, uint8_t ahb, uint8_t apb) {
    SystemCoreClockUpdate();

    // Select Clock Source
    ClockNodeEnum clockSource = (useECS && ClockEnable(ClockNodeECS, true)) ? ClockNodeECS : ClockNodeICS;

    volatile bool isCacheHit = false;

    uint32_t qspiParam = (QSPI->DEVICE_PARA & ~QSPI_DEVICE_PARA_TIMMING);

CacheHit:
    // Step 1: Select internal/external clock as system clock
    if (isCacheHit) {
        ClockSelect(ClockNodeSYSS, clockSource);
        QSPIClockConfig(qspiParam);
    }

    // Step 2: Config system PLL clock
    if (isCacheHit) {
        SystemCoreClockUpdate();

        // Main PLL
        ClockEnable(ClockNodePLL1G, false);
        ClockSelect(ClockNodeSRC, clockSource);
        ClockDivide(ClockNodePLL1M, m);
        ClockMultiply(ClockNodePLL1, n);

        clockSource = ClockNodePLL1P;

        // Main PLL Divide
        ClockDivide(ClockNodePLL1P, p);
        ClockDivide(ClockNodePLL1Q, q);
        ClockDivide(ClockNodePLL1R, r);
        ClockEnable(ClockNodePLL1G, true);

        // System Clock Divide
        ClockDivide(ClockNodeSYS, 1);

        // AHB / APB Clock Config
        ClockDivide(ClockNodeAHB1, ahb);
        ClockDivide(ClockNodeAPB1, apb);
        ClockDivide(ClockNodeAPB2, apb);

        qspiParam |= CONFIG_QSPI_TIMMING;
    }

    // Step 3: Select system PLL clock as system clock
    if (isCacheHit) {
        ClockSelect(ClockNodeSYSS, clockSource);
        QSPIClockConfig(qspiParam);
    }

    // Step 0: Hit Cache
    if (!isCacheHit) {
        ClockSelect(ClockNodeSYSS, ClockGetSelection(ClockNodeSYSS));
        QSPIClockConfig(QSPI->DEVICE_PARA);
        isCacheHit = true;
        goto CacheHit;
    }

    // End: Update System Clock
    SystemCoreClockUpdate();
    QSPIClockConfig(qspiParam | (SystemCoreClock / 1000000 << 16));
}

#if defined(__CC_ARM)
__WEAK __ASM void SystemDelayUs(volatile uint32_t us) {
    sub   sp, sp, #4;
    mov32 r1, #__cpp(&idlingPeriod);
    str   r0, [ sp, #0 ];
    ldr   r0, [ sp, #0 ];
    ldr   r1, [r1];
    mul   r0, r0, r1;
    lsr   r0, r0, #4;

    subs  r0, #0xa;
    it    cc;
    movcc r0, #0;

    ; // clang-format off
0   subs r0, #1;
    nop;
    bcs %b0;
    ; // clang-format on

    ADD sp, sp, #4;
    BX  lr;
}
#else
__WEAK void SystemDelayUs(volatile uint32_t us) {
    __ASM volatile( //
        "mul   %[us], %[us], %[period]\n"
        "lsr   %[us], %[us], #4\n"

        "subs  %[us], #10\n"
        "it    cc\n"
        "movcc %[us],#0\n"

        "0:\n"
        "    subs  %[us], #1\n"
        "    nop\n"
        "    bcs   0b\n" //
        : :              //
        [us] "r"(us),
        [period] "r"(idlingPeriod) //
    );
}
#endif

__WEAK void SystemDelay(volatile uint32_t ms) {
    while (ms--) {
        SystemDelayUs(1000);
    }
}

/*----------------------------------------------------------------------------
  Peripheral Map
 *----------------------------------------------------------------------------*/
__WEAK PeripheralStruct PeripheralMap[] = {
    // AHB1
    [PeripheralGPIOA] = {GPIOA_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOA},
    [PeripheralGPIOB] = {GPIOB_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOB},
    [PeripheralGPIOC] = {GPIOC_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOC},
    [PeripheralGPIOD] = {GPIOD_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOD},
    [PeripheralGPIOE] = {GPIOE_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOE},
    [PeripheralGPIOF] = {GPIOF_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOF},
    [PeripheralGPIOG] = {GPIOG_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOG},
    [PeripheralGPIOH] = {GPIOH_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOH},
    [PeripheralGPIOI] = {GPIOI_BASE, ClockNodeAHB1, PERIPH_GATE_GPIOI},
    [PeripheralJPEGD] = {JPEGD_BASE, ClockNodeAHB1, PERIPH_GATE_JPEGD},
    [PeripheralCRC]   = {CRC_BASE, ClockNodeAHB1, PERIPH_GATE_CRC},
    [PeripheralETH]   = {ETH_BASE, ClockNodeAHB1, PERIPH_GATE_ETH},
    [PeripheralSYS]   = {RCC_BASE, ClockNodeAHB1, PERIPH_GATE_GLB},

    // AHB2
    [PeripheralFSMC] = {FSMC_R_BASE, ClockNodeAHB2, PERIPH_GATE_FSMC},
    [PeripheralOTP]  = {OTP_BASE, ClockNodeAHB2, PERIPH_GATE_OTP},
    [PeripheralHSPI] = {HSPI_BASE, ClockNodeAHB2, PERIPH_GATE_HSPI},
    [PeripheralDCMI] = {DCMI_BASE, ClockNodeAHB2, PERIPH_GATE_DCMI},
    [PeripheralTRNG] = {TRNG_BASE, ClockNodeAHB2, PERIPH_GATE_TRNG},
    [PeripheralUSB1] = {USB1_BASE, ClockNodeAHB2, PERIPH_GATE_USB1},

    // AHB3
    [PeripheralSDRAM] = {SDRAM_R_BASE, ClockNodeAHB3, PERIPH_GATE_SDRAM},

    // AHB4
    [PeripheralDMA1]  = {DMA1_BASE, ClockNodeAHB4, PERIPH_GATE_DMA1},
    [PeripheralDMA2]  = {DMA2_BASE, ClockNodeAHB4, PERIPH_GATE_DMA2},
    [PeripheralDMA2D] = {DMA2D_BASE, ClockNodeAHB4, PERIPH_GATE_DMA2D},
    [PeripheralLTDC]  = {LTDC_BASE, ClockNodeAHB4, PERIPH_GATE_LTDC},

    // APB1
    [PeripheralTIM2]   = {TIM2_BASE, ClockNodeAPB1, PERIPH_GATE_TIM2},
    [PeripheralTIM3]   = {TIM3_BASE, ClockNodeAPB1, PERIPH_GATE_TIM3},
    [PeripheralTIM4]   = {TIM4_BASE, ClockNodeAPB1, PERIPH_GATE_TIM4},
    [PeripheralTIM5]   = {TIM5_BASE, ClockNodeAPB1, PERIPH_GATE_TIM5},
    [PeripheralTIM6]   = {TIM6_BASE, ClockNodeAPB1, PERIPH_GATE_TIM6},
    [PeripheralTIM7]   = {TIM7_BASE, ClockNodeAPB1, PERIPH_GATE_TIM7},
    [PeripheralTIM12]  = {TIM12_BASE, ClockNodeAPB1, PERIPH_GATE_TIM12},
    [PeripheralTIM13]  = {TIM13_BASE, ClockNodeAPB1, PERIPH_GATE_TIM13},
    [PeripheralTIM14]  = {TIM14_BASE, ClockNodeAPB1, PERIPH_GATE_TIM14},
    [PeripheralWWDG]   = {WWDG_BASE, ClockNodeAPB1, PERIPH_GATE_WWDG},
    [PeripheralIWDG]   = {IWDG_BASE, ClockNodeAPB1, PERIPH_GATE_IWDG},
    [PeripheralSPI2]   = {SPI2_BASE, ClockNodeAPB1, PERIPH_GATE_SPI2},
    [PeripheralI2S2]   = {I2S2ext_BASE, ClockNodeAPB1, PERIPH_GATE_I2S2},
    [PeripheralSPI3]   = {SPI3_BASE, ClockNodeAPB1, PERIPH_GATE_SPI3},
    [PeripheralI2S3]   = {I2S3ext_BASE, ClockNodeAPB1, PERIPH_GATE_I2S3},
    [PeripheralUSART2] = {USART2_BASE, ClockNodeAPB1, PERIPH_GATE_USART2},
    [PeripheralUSART3] = {USART3_BASE, ClockNodeAPB1, PERIPH_GATE_USART3},
    [PeripheralUART4]  = {UART4_BASE, ClockNodeAPB1, PERIPH_GATE_UART4},
    [PeripheralI2C1]   = {I2C1_BASE, ClockNodeAPB1, PERIPH_GATE_I2C1},
    [PeripheralI2C2]   = {I2C2_BASE, ClockNodeAPB1, PERIPH_GATE_I2C2},
    [PeripheralI2C3]   = {I2C3_BASE, ClockNodeAPB1, PERIPH_GATE_I2C3},
    [PeripheralCAN1]   = {CAN1_BASE, ClockNodeAPB1, PERIPH_GATE_CAN1},
    [PeripheralCAN2]   = {CAN2_BASE, ClockNodeAPB1, PERIPH_GATE_CAN2},
    [PeripheralPWR]    = {PWR_BASE, ClockNodeAPB1, PERIPH_GATE_PWR},
    [PeripheralDAC]    = {DAC_BASE, ClockNodeAPB1, PERIPH_GATE_DAC},
    [PeripheralUART7]  = {UART7_BASE, ClockNodeAPB1, PERIPH_GATE_UART7},

    // APB2
    [PeripheralTIM1]   = {TIM1_BASE, ClockNodeAPB2, PERIPH_GATE_TIM1},
    [PeripheralTIM8]   = {TIM8_BASE, ClockNodeAPB2, PERIPH_GATE_TIM8},
    [PeripheralUSART1] = {USART1_BASE, ClockNodeAPB2, PERIPH_GATE_USART1},
    [PeripheralUSART6] = {USART6_BASE, ClockNodeAPB2, PERIPH_GATE_USART6},
    [PeripheralADC]    = {ADC_BASE, ClockNodeAPB2, PERIPH_GATE_ADC1},
    [PeripheralADC1]   = {ADC1_BASE, ClockNodeAPB2, PERIPH_GATE_ADC1},
    [PeripheralADC2]   = {ADC2_BASE, ClockNodeAPB2, PERIPH_GATE_ADC2},
    [PeripheralADC3]   = {ADC3_BASE, ClockNodeAPB2, PERIPH_GATE_ADC3},
    [PeripheralSDIO]   = {SDIO_BASE, ClockNodeAPB2, PERIPH_GATE_SDIO},
    [PeripheralSPI1]   = {SPI1_BASE, ClockNodeAPB2, PERIPH_GATE_SPI1},
    [PeripheralSPI4]   = {SPI4_BASE, ClockNodeAPB2, PERIPH_GATE_SPI4},
    [PeripheralSYSCFG] = {SYSCFG_BASE, ClockNodeAPB2, PERIPH_GATE_SYSCFG},
    [PeripheralPCFG]   = {PCFG_BASE, ClockNodeAPB2, PERIPH_GATE_PCFG},
    [PeripheralTIM9]   = {TIM9_BASE, ClockNodeAPB2, PERIPH_GATE_TIM9},
    [PeripheralTIM10]  = {TIM10_BASE, ClockNodeAPB2, PERIPH_GATE_TIM10},
    [PeripheralTIM11]  = {TIM11_BASE, ClockNodeAPB2, PERIPH_GATE_TIM11},
    [PeripheralDSI]    = {DSI_BASE, ClockNodeAPB2, PERIPH_GATE_DSI},

    [PeripheralEXTI] = {EXTI_BASE, ClockNodeSYS, PERIPH_GATE_NULL},
};

/*----------------------------------------------------------------------------
  Clock Map
 *----------------------------------------------------------------------------*/
__WEAK ClockNodeStruct ClockMap[] = {
    [ClockNodeICS] = ClockMakeSource(RCC->CR, 0, 2, HSI_VALUE, 2),
    [ClockNodeECS] = ClockMakeSource(RCC->CR, 16, 2, HSE_VALUE, 10),
    [ClockNodeSRC] = ClockMakeSelector(RCC->PLLCFGR, 22, 1, 0, ClockNodeICS, ClockNodeECS),

    [ClockNodePLL1G] = ClockMakeGate(ClockNodeSRC, RCC->CR, 24, 1, true, 10),
    [ClockNodePLL1M] = ClockMakeDivider(ClockNodePLL1G, RCC->PLLCFGR, 0, 6, DividerTypeArithmetic, 1, 0),
    [ClockNodePLL1]  = ClockMakeMultiplier(ClockNodePLL1M, RCC->PLLCFGR, 6, 9, MultiplierTypeArithmetic, 1, 0),
    [ClockNodePLL1P] = ClockMakeDivider(ClockNodePLL1, RCC->PLLCFGR, 16, 3, DividerTypeBypassArithmetic, 2, 2),
    [ClockNodePLL1Q] = ClockMakeDivider(ClockNodePLL1, RCC->PLLCFGR, 24, 4, DividerTypeArithmetic, 1, 0),
    [ClockNodePLL1R] = ClockMakeDivider(ClockNodePLL1, RCC->PLLCFGR, 28, 4, DividerTypeArithmetic, 1, 0),

    [ClockNodeSYSS] = ClockMakeSelector(RCC->CFGR, 0, 2, 0, ClockNodeICS, ClockNodeECS, ClockNodePLL1P),
    [ClockNodeSYS]  = ClockMakeDivider(ClockNodeSYSS, RCC->PLLPRE, 0, 4, DividerTypeBypassGeometric, 2, 2),
    [ClockNodeAHB1] = ClockMakeDivider(ClockNodeSYS, RCC->CFGR, 4, 4, DividerTypeBypassGeometric, 2, 2),
    [ClockNodeAPB1] = ClockMakeDivider(ClockNodeAHB1, RCC->CFGR, 10, 3, DividerTypeBypassGeometric, 2, 2),
    [ClockNodeAPB2] = ClockMakeDivider(ClockNodeAHB1, RCC->CFGR, 13, 3, DividerTypeBypassGeometric, 2, 2),

    [ClockNodePLL2G] = ClockMakeGate(ClockNodePLL1M, RCC->CR, 26, 1, true, 2),
    [ClockNodePLL2]  = ClockMakeMultiplier(ClockNodePLL2G, RCC->PLL2CFGR, 6, 9, MultiplierTypeArithmetic, 1, 0),
    [ClockNodePLL2P] = ClockMakeDivider(ClockNodePLL2, RCC->PLL2CFGR, 16, 3, DividerTypeBypassArithmetic, 2, 2),
    [ClockNodePLL2Q] = ClockMakeDivider(ClockNodePLL2, RCC->PLL2CFGR, 24, 4, DividerTypeArithmetic, 1, 0),
    [ClockNodePLL2R] = ClockMakeDivider(ClockNodePLL2, RCC->PLL2CFGR, 28, 4, DividerTypeArithmetic, 1, 0),

    [ClockNodePLL3G] = ClockMakeGate(ClockNodeSRC, RCC->CR, 28, 1, true, 10),
    [ClockNodePLL3]  = ClockMakeMultiplier(ClockNodePLL3G, RCC->PLL3LOOP, 0, 22, MultiplierTypeFractional, 14, 0),
    [ClockNodePLL3I] = ClockMakeDivider(ClockNodePLL3, RCC->PLL3I2S, 0, 22, DividerTypeFractional, 14, 0),
    [ClockNodePLL3E] = ClockMakeDivider(ClockNodePLL3, RCC->PLL3EXT, 0, 22, DividerTypeFractional, 14, 0),
    [ClockNodePLL3N] = ClockMakeDivider(ClockNodePLL3, RCC->PLL3NFC, 0, 22, DividerTypeFractional, 14, 0),
    [ClockNodePLL3R] = ClockMakeDivider(ClockNodePLL3, RCC->PLL3USB, 0, 22, DividerTypeFractional, 14, 0),
    [ClockNodePLL3U] = ClockMakeDivider(ClockNodePLL3, RCC->PLL3UART, 0, 22, DividerTypeFractional, 14, 0),

    [ClockNodeI2SE] = ClockMakeSource(RCC->CR, 0, 0, CONFIG_I2SE_VALUE, 0),

    [ClockNodeI2S]  = ClockMakeSelector(RCC->CFGR, 23, 1, 0, ClockNodePLL3I, ClockNodeI2SE),
    [ClockNodeDPC]  = ClockMakeSelector(RCC->DCKCFGR, 0, 2, 0, ClockNodePLL2R, ClockNodePLL2Q, ClockNodePLL2P, ClockNodePLL1R),
    [ClockNodeREF]  = ClockMakeSelector(RCC->DCKCFGR, 27, 1, 0, ClockNodePLL3R, ClockNodePLL1Q),
    [ClockNodeSDIO] = ClockMakeSelector(RCC->DCKCFGR, 28, 1, 0, ClockNodeREF, ClockNodeAHB),

    [ClockNodeQSPI]  = ClockMakeDivider(ClockNodeSYS, QSPI->DEVICE_PARA, 0, 4, DividerTypeArithmetic, 1, 2),
    [ClockNodeSDRAM] = ClockMakeDivider(ClockNodeSYS, RCC->PLLPRE, 4, 4, DividerTypeBypassArithmetic, 1, 2),
};
