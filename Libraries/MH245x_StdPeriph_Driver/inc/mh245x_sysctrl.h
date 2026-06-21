// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_SYSCTRL_H__
#define __PERIPH_SYSCTRL_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

/**
 * @brief Reset and Clock Control
 */
typedef struct
{
    __IO uint32_t CR;      /*!< RCC clock control register,                                  Address offset: 0x00 */
    __IO uint32_t PLLCFGR; /*!< RCC PLL configuration register,                              Address offset: 0x04 */
    __IO uint32_t CFGR;    /*!< RCC clock configuration register,                            Address offset: 0x08 */
    __IO uint32_t CIR;     /*!< RCC clock interrupt register,                                Address offset: 0x0C */
    union
    {
        __IO uint32_t AHBRSTR[4];
        struct
        {
            __IO uint32_t AHB1RSTR; /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
            __IO uint32_t AHB2RSTR; /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
            __IO uint32_t AHB3RSTR; /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
            __IO uint32_t AHB4RSTR; /*!< RCC AHB4 peripheral reset register,                          Address offset: 0x1C */
        };
    };
    union
    {
        __IO uint32_t APBRSTR[4];
        struct
        {
            __IO uint32_t APB1RSTR; /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
            __IO uint32_t APB2RSTR; /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
            __IO uint32_t APB3RSTR; /*!< RCC APB3 peripheral reset register,                          Address offset: 0x28 */
            __IO uint32_t APB4RSTR; /*!< RCC APB4 peripheral reset register,                          Address offset: 0x2C */
        };
    };
    union
    {
        __IO uint32_t AHBENR[4];
        struct
        {
            __IO uint32_t AHB1ENR; /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
            __IO uint32_t AHB2ENR; /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
            __IO uint32_t AHB3ENR; /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
            __IO uint32_t AHB4ENR; /*!< RCC AHB4 peripheral clock register,                          Address offset: 0x3C */
        };
    };
    union
    {
        __IO uint32_t APBENR[4];
        struct
        {
            __IO uint32_t APB1ENR; /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
            __IO uint32_t APB2ENR; /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
            __IO uint32_t APB3ENR; /*!< RCC APB3 peripheral clock enable register,                   Address offset: 0x48 */
            __IO uint32_t APB4ENR; /*!< RCC APB4 peripheral clock enable register,                   Address offset: 0x4C */
        };
    };
    union
    {
        __IO uint32_t AHBLPENR[4];
        struct
        {
            __IO uint32_t AHB1LPENR; /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
            __IO uint32_t AHB2LPENR; /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
            __IO uint32_t AHB3LPENR; /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
            __IO uint32_t AHB4LPENR; /*!< RCC AHB4 peripheral clock enable in low power mode register, Address offset: 0x5C */
        };
    };
    union
    {
        __IO uint32_t APBLPENR[4];
        struct
        {
            __IO uint32_t APB1LPENR; /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
            __IO uint32_t APB2LPENR; /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
            __IO uint32_t APB3LPENR; /*!< RCC APB3 peripheral clock enable in low power mode register, Address offset: 0x68 */
            __IO uint32_t APB4LPENR; /*!< RCC APB4 peripheral clock enable in low power mode register, Address offset: 0x6C */
        };
    };
    __IO uint32_t BDCR;       /*!< RCC Backup domain control register,                          Address offset: 0x70 */
    __IO uint32_t CSR;        /*!< RCC clock control & status register,                         Address offset: 0x74 */
    __I uint32_t  RSVD78[2];  /*!< Reserved, 0x78-0x7C                                                               */
    __IO uint32_t SSCGR;      /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
    __IO uint32_t PLL2CFGR;   /*!< RCC PLL2 configuration register,                             Address offset: 0x84 */
    __IO uint32_t RSVD88;     /*!< Reserved, 0x88                                                                    */
    __IO uint32_t DCKCFGR;    /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */
    __I uint32_t  RSVD90[15]; /*!< Reserved, 0x90-0xC8                                                               */
    __IO uint32_t PLL3LOOP;   /*!< PLL3 loop register                                           Address offset: 0xCC */
    __IO uint32_t PLL3I2S;    /*!< PLL3 I2S control register                                    Address offset: 0xD0 */
    __IO uint32_t PLL3EXT;    /*!< PLL3 I2S control register                                    Address offset: 0xD4 */
    __IO uint32_t PLL3NFC;    /*!< PLL3 NFC control register                                    Address offset: 0xD8 */
    __IO uint32_t PLL3USB;    /*!< PLL3 USB control register                                    Address offset: 0xDC */
    __IO uint32_t PLL3UART;   /*!< PLL3 UART control register                                   Address offset: 0xE0 */
    __IO uint32_t PLL3LOAD;   /*!< PLL3 load register                                           Address offset: 0xE4 */
    __IO uint32_t PLLPRE;     /*!< PLL prescaler register                                       Address offset: 0xE8 */
    __I uint32_t  RSVDEC[1];  /*!< Reserved, 0xEC                                                                    */
    __IO uint32_t ECR;        /*!< External control register                                    Address offset: 0xF0 */
} RCC_TypeDef, SYSCTRL_TypeDef;

/**
 * @brief System configuration controller
 */
typedef struct
{
    __IO uint32_t MEMRMP;                       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
    __IO uint32_t PMC;                          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
    __IO uint32_t EXTICR[4];                    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
    uint32_t      RESERVED[(0x30 - 0x18) >> 2]; /*!< Reserved, 0x18-0x2C                                                     */
    __IO uint32_t EXMEMCFG;                     /*!< External memory configuration,                     Address offset: 0x30 */
} SYSCFG_TypeDef;

/**
 * @brief Peripheral configuration controller
 */
typedef struct
{
    __IO uint32_t LOCK;               /*!< Address offset: 0x00 */
    __IO uint32_t PCFG_BOOT;          /*!< Address offset: 0x04 */
    __IO uint32_t PCFG_PWR;           /*!< Address offset: 0x08 */
    __I uint32_t  RSVD0[10];          /*!< Address offset: 0x0C~0x30 */
    __IO uint32_t HSI_CAL;            /*!< Address offset: 0x34 */
    __IO uint32_t NO_USE;             /*!< Address offset: 0x38 */
    __IO uint32_t LSI_CAL;            /*!< Address offset: 0x3C */
    __I uint32_t  RSVD1[35];          /*!< Address offset: 0x40~0xC8 */
    __IO uint32_t PHER_CTRL1;         /*!< Address offset: 0xCC */
    __I uint32_t  RSVD2[3];           /*!< Address offset: 0xD0~0xC8 */
    __IO uint32_t SOFT_USE;           /*!< Address offset: 0xDC */
    __I uint32_t  RSVD3[3];           /*!< Address offset: 0xE0~0xE8 */
    __IO uint32_t USBHS_CFG;          /*!< Address offset: 0xEC */
    __IO uint32_t USBFS_CFG;          /*!< Address offset: 0xF0 */
    __IO uint32_t HSI_TRIM_USBCAL;    /*!< Address offset: 0xF4 */
    __IO uint32_t HSI_TRIM_32KCAL;    /*!< Address offset: 0xF8 */
    __I uint32_t  RSVD4[17];          /*!< Address offset: 0xFC~0x13C */
    __IO uint32_t USER_RSVD0;         /*!< Address offset: 0x140 */
    __IO uint32_t USER_RSVD1;         /*!< Address offset: 0x144 */
    __I uint32_t  RSVD5[6];           /*!< Address offset: 0x148~0x15C */
    __IO uint32_t HSI_TRIM_USBCAL1MS; /*!< Address offset: 0x160 */
    __IO uint32_t RSVD6[1];            /*!< Address offset: 0x164 */
    __IO uint32_t USBHS_CR1;          /*!< Address offset: 0x168 */
    __IO uint32_t USBHS_CR2;          /*!< Address offset: 0x16C */
    __IO uint32_t USBHS_CR3;          /*!< Address offset: 0x170 */
    __I uint32_t  RSVD7[11];          /*!< Address offset: 0x174~0x19C */
    __IO uint32_t CHIP_ID;            /*!< Address offset: 0x1A0 */
    __I uint32_t  RSVD8[3];           /*!< Address offset: 0x1A4~0x1AC */
    __IO uint32_t REMAP_UNLOCK;       /*!< Address offset: 0x1B0 */
} PCFG_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/********************  Bit definition for RCC_CR register  ********************/
#define RCC_CR_HSION   BIT0
#define RCC_CR_HSIRDY  BIT1
#define RCC_CR_HSITRIM BITS(7, 3)
#define RCC_CR_HSICAL  BITS(15, 8)
#define RCC_CR_HSEON   BIT16
#define RCC_CR_HSERDY  BIT17
#define RCC_CR_HSEBYP  BIT18
#define RCC_CR_CSSON   BIT19
#define RCC_CR_PLL1ON  BIT24
#define RCC_CR_PLL1RDY BIT25
#define RCC_CR_PLL2ON  BIT26
#define RCC_CR_PLL2RDY BIT27
#define RCC_CR_PLL3ON  BIT28
#define RCC_CR_PLL3RDY BIT29

/********************  Bit definition for RCC_PLLCFGR register  ***************/
#define RCC_PLLCFGR_PLLM    BITS(5, 0)
#define RCC_PLLCFGR_PLLN    BITS(16, 4)
#define RCC_PLLCFGR_PLLP    BITS(17, 16)
#define RCC_PLLCFGR_PLL1PEN BIT18
#define RCC_PLLCFGR_PLLSRC  BIT22
#define RCC_PLLCFGR_PLL1Q   BITS(27, 24)
#define RCC_PLLCFGR_PLL1R   BITS(31, 28)

/********************  Bit definition for CFGR register  ******************/
#define RCC_CFGR_SW      BITS(1, 0)
#define RCC_CFGR_SWS     BITS(3, 2)
#define RCC_CFGR_HPRE    BITS(7, 4)
#define RCC_CFGR_PPRE1   BITS(12, 10)
#define RCC_CFGR_PPRE2   BITS(15, 13)
#define RCC_CFGR_RTCPRE  BITS(20, 16)
#define RCC_CFGR_MCO1    BITS(22, 21)
#define RCC_CFGR_I2SSRC  BIT23
#define RCC_CFGR_MCO1PRE BITS(26, 24)
#define RCC_CFGR_MCO2PRE BITS(29, 27)
#define RCC_CFGR_MCO2    BITS(31, 30)

/********************  Bit definition for RCC_CIR register  *******************/
#define RCC_CIR_LSIRDYF   BIT0
#define RCC_CIR_LSERDYF   BIT1
#define RCC_CIR_HSIRDYF   BIT2
#define RCC_CIR_HSERDYF   BIT3
#define RCC_CIR_PLL1RDYF  BIT4
#define RCC_CIR_PLL2RDYF  BIT5
#define RCC_CIR_CSSF      BIT7
#define RCC_CIR_LSIRDYIE  BIT8
#define RCC_CIR_LSERDYIE  BIT9
#define RCC_CIR_HSIRDYIE  BIT10
#define RCC_CIR_HSERDYIE  BIT11
#define RCC_CIR_PLLRDYIE  BIT12
#define RCC_CIR_PLL2RDYIE BIT13
#define RCC_CIR_LSIRDYC   BIT16
#define RCC_CIR_LSERDYC   BIT17
#define RCC_CIR_HSIRDYC   BIT18
#define RCC_CIR_HSERDYC   BIT19
#define RCC_CIR_PLL1RDYC  BIT20
#define RCC_CIR_PLL2RDYC  BIT21
#define RCC_CIR_CSSC      BIT23

/********************  Bit definition for RCC_BDCR register  ******************/
#define RCC_BDCR_LSEON  BIT0
#define RCC_BDCR_LSERDY BIT1
#define RCC_BDCR_LSEBYP BIT2
#define RCC_BDCR_RTCSEL BITS(9, 8)
#define RCC_BDCR_BDRST  BIT16

/********************  Bit definition for RCC_CSR register  *******************/
#define RCC_CSR_LSIRDY   BIT1
#define RCC_CSR_BATTRSTF BIT23
#define RCC_CSR_RMVF     BIT24
#define RCC_CSR_MATTRSTF BIT25
#define RCC_CSR_PINRSTF  BIT26
#define RCC_CSR_PORRSTF  BIT27
#define RCC_CSR_SFTRSTF  BIT28
#define RCC_CSR_IWDGRSTF BIT29
#define RCC_CSR_WWDGRSTF BIT30
#define RCC_CSR_LPWRRSTF BIT31

/********************  Bit definition for RCC_SSCGR register  *****************/
#define RCC_SSCGR_MODPER    BITS(12, 0)
#define RCC_SSCGR_INCSTEP   BITS(27, 13)
#define RCC_SSCGR_SPREADSEL BIT30
#define RCC_SSCGR_SSCGEN    BIT31

/********************  Bit definition for RCC_PLL2CFGR register  ************/
#define RCC_PLL2CFGR_N   BITS(14, 6)
#define RCC_PLL2CFGR_P   BITS(17, 16)
#define RCC_PLL2CFGR_PEN BIT(18)
#define RCC_PLL2CFGR_Q   BITS(27, 24)
#define RCC_PLL2CFGR_R   BITS(31, 28)

/********************  Bit definition for RCC_DCKCFGR register  ************/
#define RCC_DCKCFGR_DSISEL  BITS(1, 0)
#define RCC_DCKCFGR_48MSEL  BIT(27)
#define RCC_DCKCFGR_SDIOSEL BIT(28)

/********************  Bit definition for RCC_FREQLOOP register  ************/
#define RCC_FREQLOOP_LOOP BITS(21, 0)

/********************  Bit definition for RCC_FREQI2S register  ************/
#define RCC_FREQI2S_FREQ BITS(21, 0)

/********************  Bit definition for RCC_FREQEXT register  ************/
#define RCC_FREQEXT_FREQ BITS(21, 0)

/********************  Bit definition for RCC_FREQNFC register  ************/
#define RCC_FRENFC_FREQ BITS(21, 0)

/********************  Bit definition for RCC_FREQUSB register  ************/
#define RCC_FREUSB_FREQ BITS(21, 0)

/********************  Bit definition for RCC_FREQUART register  ************/
#define RCC_FREQUART_FREQ BITS(21, 0)

/********************  Bit definition for RCC_FREQLOAD register  ************/
#define RCC_FREQLOAD_LOOP BIT0
#define RCC_FREQLOAD_EXT  BIT1
#define RCC_FREQLOAD_NFC  BIT2
#define RCC_FREQLOAD_USB  BIT3
#define RCC_FREQLOAD_UART BIT4
#define RCC_FREQLOAD_I2S  BIT5

/********************  Bit definition for RCC_PRE register  ************/
#define RCC_PRE_CPUPRE BITS(3, 0)
#define RCC_PRE_SDMPRE BITS(7, 4)

/******************  Bit definition for SYSCFG_MEMRMP register  ***************/
#define SYSCFG_MEMRMP_MEM_MODE BITS(3, 0)
#define SYSCFG_MEMRMP_EXMEM_MODE BIT31

/******************  Bit definition for SYSCFG_PMC register  ******************/
#define SYSCFG_PMC_IWSTOPPAEN    BIT0
#define SYSCFG_PMC_IWSTANDBYPAEN BIT1
#define SYSCFG_PMC_SDIODMAMODE   BIT8
#define SYSCFG_PMC_MII_RMII_SEL  BIT23

/*****************  Bit definition for SYSCFG_EXTICR1 register  ***************/
#define SYSCFG_EXTICR1_EXTI0 ((uint16_t)0x000F) /*!<EXTI 0 configuration */
#define SYSCFG_EXTICR1_EXTI1 ((uint16_t)0x00F0) /*!<EXTI 1 configuration */
#define SYSCFG_EXTICR1_EXTI2 ((uint16_t)0x0F00) /*!<EXTI 2 configuration */
#define SYSCFG_EXTICR1_EXTI3 ((uint16_t)0xF000) /*!<EXTI 3 configuration */
/**
 * @brief   EXTI0 configuration
 */
#define SYSCFG_EXTICR1_EXTI0_PA ((uint16_t)0x0000) /*!<PA[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PB ((uint16_t)0x0001) /*!<PB[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PC ((uint16_t)0x0002) /*!<PC[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PD ((uint16_t)0x0003) /*!<PD[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PE ((uint16_t)0x0004) /*!<PE[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PF ((uint16_t)0x0005) /*!<PF[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PG ((uint16_t)0x0006) /*!<PG[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PH ((uint16_t)0x0007) /*!<PH[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PI ((uint16_t)0x0008) /*!<PI[0] pin */

/**
 * @brief   EXTI1 configuration
 */
#define SYSCFG_EXTICR1_EXTI1_PA ((uint16_t)0x0000) /*!<PA[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PB ((uint16_t)0x0010) /*!<PB[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PC ((uint16_t)0x0020) /*!<PC[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PD ((uint16_t)0x0030) /*!<PD[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PE ((uint16_t)0x0040) /*!<PE[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PF ((uint16_t)0x0050) /*!<PF[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PG ((uint16_t)0x0060) /*!<PG[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PH ((uint16_t)0x0070) /*!<PH[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PI ((uint16_t)0x0080) /*!<PI[1] pin */

/**
 * @brief   EXTI2 configuration
 */
#define SYSCFG_EXTICR1_EXTI2_PA ((uint16_t)0x0000) /*!<PA[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PB ((uint16_t)0x0100) /*!<PB[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PC ((uint16_t)0x0200) /*!<PC[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PD ((uint16_t)0x0300) /*!<PD[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PE ((uint16_t)0x0400) /*!<PE[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PF ((uint16_t)0x0500) /*!<PF[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PG ((uint16_t)0x0600) /*!<PG[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PH ((uint16_t)0x0700) /*!<PH[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PI ((uint16_t)0x0800) /*!<PI[2] pin */

/**
 * @brief   EXTI3 configuration
 */
#define SYSCFG_EXTICR1_EXTI3_PA ((uint16_t)0x0000) /*!<PA[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PB ((uint16_t)0x1000) /*!<PB[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PC ((uint16_t)0x2000) /*!<PC[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PD ((uint16_t)0x3000) /*!<PD[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PE ((uint16_t)0x4000) /*!<PE[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PF ((uint16_t)0x5000) /*!<PF[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PG ((uint16_t)0x6000) /*!<PG[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PH ((uint16_t)0x7000) /*!<PH[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PI ((uint16_t)0x8000) /*!<PI[3] pin */

/*****************  Bit definition for SYSCFG_EXTICR2 register  ***************/
#define SYSCFG_EXTICR2_EXTI4 ((uint16_t)0x000F) /*!<EXTI 4 configuration */
#define SYSCFG_EXTICR2_EXTI5 ((uint16_t)0x00F0) /*!<EXTI 5 configuration */
#define SYSCFG_EXTICR2_EXTI6 ((uint16_t)0x0F00) /*!<EXTI 6 configuration */
#define SYSCFG_EXTICR2_EXTI7 ((uint16_t)0xF000) /*!<EXTI 7 configuration */
/**
 * @brief   EXTI4 configuration
 */
#define SYSCFG_EXTICR2_EXTI4_PA ((uint16_t)0x0000) /*!<PA[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PB ((uint16_t)0x0001) /*!<PB[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PC ((uint16_t)0x0002) /*!<PC[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PD ((uint16_t)0x0003) /*!<PD[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PE ((uint16_t)0x0004) /*!<PE[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PF ((uint16_t)0x0005) /*!<PF[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PG ((uint16_t)0x0006) /*!<PG[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PH ((uint16_t)0x0007) /*!<PH[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PI ((uint16_t)0x0008) /*!<PI[4] pin */

/**
 * @brief   EXTI5 configuration
 */
#define SYSCFG_EXTICR2_EXTI5_PA ((uint16_t)0x0000) /*!<PA[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PB ((uint16_t)0x0010) /*!<PB[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PC ((uint16_t)0x0020) /*!<PC[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PD ((uint16_t)0x0030) /*!<PD[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PE ((uint16_t)0x0040) /*!<PE[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PF ((uint16_t)0x0050) /*!<PF[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PG ((uint16_t)0x0060) /*!<PG[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PH ((uint16_t)0x0070) /*!<PH[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PI ((uint16_t)0x0080) /*!<PI[5] pin */

/**
 * @brief   EXTI6 configuration
 */
#define SYSCFG_EXTICR2_EXTI6_PA ((uint16_t)0x0000) /*!<PA[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PB ((uint16_t)0x0100) /*!<PB[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PC ((uint16_t)0x0200) /*!<PC[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PD ((uint16_t)0x0300) /*!<PD[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PE ((uint16_t)0x0400) /*!<PE[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PF ((uint16_t)0x0500) /*!<PF[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PG ((uint16_t)0x0600) /*!<PG[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PH ((uint16_t)0x0700) /*!<PH[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PI ((uint16_t)0x0800) /*!<PI[6] pin */

/**
 * @brief   EXTI7 configuration
 */
#define SYSCFG_EXTICR2_EXTI7_PA ((uint16_t)0x0000) /*!<PA[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PB ((uint16_t)0x1000) /*!<PB[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PC ((uint16_t)0x2000) /*!<PC[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PD ((uint16_t)0x3000) /*!<PD[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PE ((uint16_t)0x4000) /*!<PE[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PF ((uint16_t)0x5000) /*!<PF[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PG ((uint16_t)0x6000) /*!<PG[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PH ((uint16_t)0x7000) /*!<PH[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PI ((uint16_t)0x8000) /*!<PI[7] pin */

/*****************  Bit definition for SYSCFG_EXTICR3 register  ***************/
#define SYSCFG_EXTICR3_EXTI8  ((uint16_t)0x000F) /*!<EXTI 8 configuration */
#define SYSCFG_EXTICR3_EXTI9  ((uint16_t)0x00F0) /*!<EXTI 9 configuration */
#define SYSCFG_EXTICR3_EXTI10 ((uint16_t)0x0F00) /*!<EXTI 10 configuration */
#define SYSCFG_EXTICR3_EXTI11 ((uint16_t)0xF000) /*!<EXTI 11 configuration */

/**
 * @brief   EXTI8 configuration
 */
#define SYSCFG_EXTICR3_EXTI8_PA ((uint16_t)0x0000) /*!<PA[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PB ((uint16_t)0x0001) /*!<PB[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PC ((uint16_t)0x0002) /*!<PC[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PD ((uint16_t)0x0003) /*!<PD[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PE ((uint16_t)0x0004) /*!<PE[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PF ((uint16_t)0x0005) /*!<PF[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PG ((uint16_t)0x0006) /*!<PG[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PH ((uint16_t)0x0007) /*!<PH[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PI ((uint16_t)0x0008) /*!<PI[8] pin */

/**
 * @brief   EXTI9 configuration
 */
#define SYSCFG_EXTICR3_EXTI9_PA ((uint16_t)0x0000) /*!<PA[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PB ((uint16_t)0x0010) /*!<PB[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PC ((uint16_t)0x0020) /*!<PC[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PD ((uint16_t)0x0030) /*!<PD[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PE ((uint16_t)0x0040) /*!<PE[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PF ((uint16_t)0x0050) /*!<PF[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PG ((uint16_t)0x0060) /*!<PG[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PH ((uint16_t)0x0070) /*!<PH[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PI ((uint16_t)0x0080) /*!<PI[9] pin */

/**
 * @brief   EXTI10 configuration
 */
#define SYSCFG_EXTICR3_EXTI10_PA ((uint16_t)0x0000) /*!<PA[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PB ((uint16_t)0x0100) /*!<PB[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PC ((uint16_t)0x0200) /*!<PC[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PD ((uint16_t)0x0300) /*!<PD[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PE ((uint16_t)0x0400) /*!<PE[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PF ((uint16_t)0x0500) /*!<PF[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PG ((uint16_t)0x0600) /*!<PG[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PH ((uint16_t)0x0700) /*!<PH[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PI ((uint16_t)0x0800) /*!<PI[10] pin */

/**
 * @brief   EXTI11 configuration
 */
#define SYSCFG_EXTICR3_EXTI11_PA ((uint16_t)0x0000) /*!<PA[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PB ((uint16_t)0x1000) /*!<PB[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PC ((uint16_t)0x2000) /*!<PC[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PD ((uint16_t)0x3000) /*!<PD[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PE ((uint16_t)0x4000) /*!<PE[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PF ((uint16_t)0x5000) /*!<PF[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PG ((uint16_t)0x6000) /*!<PG[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PH ((uint16_t)0x7000) /*!<PH[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PI ((uint16_t)0x8000) /*!<PI[11] pin */

/*****************  Bit definition for SYSCFG_EXTICR4 register  ***************/
#define SYSCFG_EXTICR4_EXTI12 ((uint16_t)0x000F) /*!<EXTI 12 configuration */
#define SYSCFG_EXTICR4_EXTI13 ((uint16_t)0x00F0) /*!<EXTI 13 configuration */
#define SYSCFG_EXTICR4_EXTI14 ((uint16_t)0x0F00) /*!<EXTI 14 configuration */
#define SYSCFG_EXTICR4_EXTI15 ((uint16_t)0xF000) /*!<EXTI 15 configuration */
/**
 * @brief   EXTI12 configuration
 */
#define SYSCFG_EXTICR4_EXTI12_PA ((uint16_t)0x0000) /*!<PA[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PB ((uint16_t)0x0001) /*!<PB[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PC ((uint16_t)0x0002) /*!<PC[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PD ((uint16_t)0x0003) /*!<PD[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PE ((uint16_t)0x0004) /*!<PE[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PF ((uint16_t)0x0005) /*!<PF[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PG ((uint16_t)0x0006) /*!<PG[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PH ((uint16_t)0x0007) /*!<PH[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PI ((uint16_t)0x0008) /*!<PI[12] pin */

/**
 * @brief   EXTI13 configuration
 */
#define SYSCFG_EXTICR4_EXTI13_PA ((uint16_t)0x0000) /*!<PA[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PB ((uint16_t)0x0010) /*!<PB[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PC ((uint16_t)0x0020) /*!<PC[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PD ((uint16_t)0x0030) /*!<PD[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PE ((uint16_t)0x0040) /*!<PE[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PF ((uint16_t)0x0050) /*!<PF[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PG ((uint16_t)0x0060) /*!<PG[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PH ((uint16_t)0x0070) /*!<PH[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PI ((uint16_t)0x0008) /*!<PI[13] pin */

/**
 * @brief   EXTI14 configuration
 */
#define SYSCFG_EXTICR4_EXTI14_PA ((uint16_t)0x0000) /*!<PA[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PB ((uint16_t)0x0100) /*!<PB[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PC ((uint16_t)0x0200) /*!<PC[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PD ((uint16_t)0x0300) /*!<PD[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PE ((uint16_t)0x0400) /*!<PE[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PF ((uint16_t)0x0500) /*!<PF[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PG ((uint16_t)0x0600) /*!<PG[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PH ((uint16_t)0x0700) /*!<PH[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PI ((uint16_t)0x0800) /*!<PI[14] pin */

/**
 * @brief   EXTI15 configuration
 */
#define SYSCFG_EXTICR4_EXTI15_PA ((uint16_t)0x0000) /*!<PA[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PB ((uint16_t)0x1000) /*!<PB[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PC ((uint16_t)0x2000) /*!<PC[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PD ((uint16_t)0x3000) /*!<PD[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PE ((uint16_t)0x4000) /*!<PE[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PF ((uint16_t)0x5000) /*!<PF[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PG ((uint16_t)0x6000) /*!<PG[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PH ((uint16_t)0x7000) /*!<PH[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PI ((uint16_t)0x8000) /*!<PI[15] pin */

/******************  Bit definition for SYSCFG_EXMEMCFG register  ****************/
#define SYSCFG_EXMEMCFG_SDRAM_SCLKSEL  BIT0
#define SYSCFG_EXMEMCFG_SDRAM_DIVSEL   BITS(5, 1)
#define SYSCFG_EXMEMCFG_SDRAM_DCLKSEL  BIT6
#define SYSCFG_EXMEMCFG_FSMC_DLYSAMP   BIT7
#define SYSCFG_EXMEMCFG_FSMC_DLYSEL    BITS(9, 8)
#define SYSCFG_EXMEMCFG_OPI_DQS_DLYSEL BITS(14, 10)
#define SYSCFG_EXMEMCFG_OPI_DQS_NODLY  BIT15
#define SYSCFG_EXMEMCFG_SDIO_CLKSSEL   BITS(19, 16)
#define SYSCFG_EXMEMCFG_SDIO_CLKDSEL   BITS(23, 20)

/** @defgroup SYSCFG_EXTI_Port_Sources
 * @{
 */
#define EXTI_PortSourceGPIOA ((uint8_t)0x00)
#define EXTI_PortSourceGPIOB ((uint8_t)0x01)
#define EXTI_PortSourceGPIOC ((uint8_t)0x02)
#define EXTI_PortSourceGPIOD ((uint8_t)0x03)
#define EXTI_PortSourceGPIOE ((uint8_t)0x04)
#define EXTI_PortSourceGPIOF ((uint8_t)0x05)
#define EXTI_PortSourceGPIOG ((uint8_t)0x06)
#define EXTI_PortSourceGPIOH ((uint8_t)0x07)
#define EXTI_PortSourceGPIOI ((uint8_t)0x08)

#define IS_EXTI_PORT_SOURCE(PORTSOURCE)                                                                                            \
    (((PORTSOURCE) == EXTI_PortSourceGPIOA) || ((PORTSOURCE) == EXTI_PortSourceGPIOB) || ((PORTSOURCE) == EXTI_PortSourceGPIOC) || \
     ((PORTSOURCE) == EXTI_PortSourceGPIOD) || ((PORTSOURCE) == EXTI_PortSourceGPIOE) || ((PORTSOURCE) == EXTI_PortSourceGPIOF) || \
     ((PORTSOURCE) == EXTI_PortSourceGPIOG) || ((PORTSOURCE) == EXTI_PortSourceGPIOH) || ((PORTSOURCE) == EXTI_PortSourceGPIOI))

/**
 * @}
 */

/** @defgroup SYSCFG_EXTI_Pin_Sources
 * @{
 */
#define EXTI_PinSource0  ((uint8_t)0x00)
#define EXTI_PinSource1  ((uint8_t)0x01)
#define EXTI_PinSource2  ((uint8_t)0x02)
#define EXTI_PinSource3  ((uint8_t)0x03)
#define EXTI_PinSource4  ((uint8_t)0x04)
#define EXTI_PinSource5  ((uint8_t)0x05)
#define EXTI_PinSource6  ((uint8_t)0x06)
#define EXTI_PinSource7  ((uint8_t)0x07)
#define EXTI_PinSource8  ((uint8_t)0x08)
#define EXTI_PinSource9  ((uint8_t)0x09)
#define EXTI_PinSource10 ((uint8_t)0x0A)
#define EXTI_PinSource11 ((uint8_t)0x0B)
#define EXTI_PinSource12 ((uint8_t)0x0C)
#define EXTI_PinSource13 ((uint8_t)0x0D)
#define EXTI_PinSource14 ((uint8_t)0x0E)
#define EXTI_PinSource15 ((uint8_t)0x0F)
#define IS_EXTI_PIN_SOURCE(PINSOURCE)                                                                                                                  \
    (((PINSOURCE) == EXTI_PinSource0) || ((PINSOURCE) == EXTI_PinSource1) || ((PINSOURCE) == EXTI_PinSource2) || ((PINSOURCE) == EXTI_PinSource3) ||   \
     ((PINSOURCE) == EXTI_PinSource4) || ((PINSOURCE) == EXTI_PinSource5) || ((PINSOURCE) == EXTI_PinSource6) || ((PINSOURCE) == EXTI_PinSource7) ||   \
     ((PINSOURCE) == EXTI_PinSource8) || ((PINSOURCE) == EXTI_PinSource9) || ((PINSOURCE) == EXTI_PinSource10) || ((PINSOURCE) == EXTI_PinSource11) || \
     ((PINSOURCE) == EXTI_PinSource12) || ((PINSOURCE) == EXTI_PinSource13) || ((PINSOURCE) == EXTI_PinSource14) || ((PINSOURCE) == EXTI_PinSource15))

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint32_t SYSCLK_Frequency; /*!<  SYSCLK clock frequency expressed in Hz */
    uint32_t HCLK_Frequency;   /*!<  HCLK clock frequency expressed in Hz   */
    uint32_t PCLK1_Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
    uint32_t PCLK2_Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
} RCC_ClocksTypeDef, SYSCTRL_ClocksTypeDef;

/* Exported constants --------------------------------------------------------*/

#define RCC_LSE_LOWPOWER_MODE  ((uint8_t)0x00)
#define RCC_LSE_HIGHDRIVE_MODE ((uint8_t)0x01)
#define IS_RCC_LSE_MODE(MODE)  (((MODE) == RCC_LSE_LOWPOWER_MODE) || ((MODE) == RCC_LSE_HIGHDRIVE_MODE))

#define RCC_PLLSource_HSI         ClockNodeICS
#define RCC_PLLSource_HSE         ClockNodeECS
#define IS_RCC_PLL_SOURCE(SOURCE) (((SOURCE) == RCC_PLLSource_HSI) || ((SOURCE) == RCC_PLLSource_HSE))
#define IS_RCC_PLLM_VALUE(VALUE)  ((VALUE) <= 63)
#define IS_RCC_PLLN_VALUE(VALUE)  ((50 <= (VALUE)) && ((VALUE) <= 432))
#define IS_RCC_PLLP_VALUE(VALUE)  (((VALUE) == 1) || ((VALUE) == 2) || ((VALUE) == 4) || ((VALUE) == 6) || ((VALUE) == 8))
#define IS_RCC_PLLQ_VALUE(VALUE)  ((2 <= (VALUE)) && ((VALUE) <= 15))
#define IS_RCC_PLLR_VALUE(VALUE)  ((2 <= (VALUE)) && ((VALUE) <= 7))

#define RCC_SYSCLKSource_HSI ClockNodeICS
#define RCC_SYSCLKSource_HSE ClockNodeECS
#define RCC_SYSCLKSource_PLL1P ClockNodePLL1P
#define RCC_SYSCLKSource_PLL         RCC_SYSCLKSource_PLL1P
#define IS_RCC_SYSCLK_SOURCE(SOURCE) (((SOURCE) == ClockNodeICS) || ((SOURCE) == ClockNodeECS) || ((SOURCE) == RCC_SYSCLKSource_PLL1P))

#define RCC_SYSCLK_Div1   (1U)
#define RCC_SYSCLK_Div2   (2U)
#define RCC_SYSCLK_Div4   (4U)
#define RCC_SYSCLK_Div8   (8U)
#define RCC_SYSCLK_Div16  (16U)
#define RCC_SYSCLK_Div64  (32U)
#define RCC_SYSCLK_Div128 (64U)
#define RCC_SYSCLK_Div256 (128U)
#define RCC_SYSCLK_Div512 (256U)
#define IS_RCC_HCLK(div)  ((div == 1) || (div == 2) || (div == 4) || (div == 8) || (div == 16) || (div == 32) || (div == 64) || (div == 128) || (div == 256))

#define RCC_HCLK_Div1    (1U)
#define RCC_HCLK_Div2    (2U)
#define RCC_HCLK_Div4    (4U)
#define RCC_HCLK_Div8    (8U)
#define RCC_HCLK_Div16   (16U)
#define IS_RCC_PCLK(div) ((div == 1) || (div == 2) || (div == 4) || (div == 8) || (div == 16))

#define RCC_IT_LSIRDY   RCC_CIR_LSIRDYF
#define RCC_IT_LSERDY   RCC_CIR_LSERDYF
#define RCC_IT_HSIRDY   RCC_CIR_HSIRDYF
#define RCC_IT_HSERDY   RCC_CIR_HSERDYF
#define RCC_IT_PLL1RDY  RCC_CIR_PLL1RDYF
#define RCC_IT_PLL2RDY  RCC_CIR_PLL2RDYF
#define RCC_IT_CSS      RCC_CIR_CSSF
#define RCC_RDYF_OFFEST 0
#define RCC_IE_OFFEST   8
#define RCC_RDYC_OFFEST 16

#define IS_RCC_IT(IT)                                                                                                                        \
    (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || ((IT) == RCC_IT_PLL1RDY) || \
     ((IT) == RCC_IT_PLL2RDY))

#define IS_RCC_GET_IT(IT)                                                                                                                    \
    (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || ((IT) == RCC_IT_PLL1RDY) || \
     ((IT) == RCC_IT_PLL2RDY) || ((IT) == RCC_IT_CSS))

#define IS_RCC_CLEAR_IT(IT) IS_RCC_GET_IT(IT)

#define RCC_LSE_OFF     0U
#define RCC_LSE_ON      RCC_BDCR_LSEON
#define RCC_LSE_Bypass  RCC_BDCR_LSEBYP
#define IS_RCC_LSE(LSE) (((LSE) == RCC_LSE_OFF) || ((LSE) == RCC_LSE_ON) || ((LSE) == RCC_LSE_Bypass))

#define RCC_RTCCLKSource_LSE       ((uint32_t)0x00000100)
#define RCC_RTCCLKSource_LSI       ((uint32_t)0x00000200)
#define RCC_RTCCLKSource_HSE_Div2  ((uint32_t)0x00020300)
#define RCC_RTCCLKSource_HSE_Div3  ((uint32_t)0x00030300)
#define RCC_RTCCLKSource_HSE_Div4  ((uint32_t)0x00040300)
#define RCC_RTCCLKSource_HSE_Div5  ((uint32_t)0x00050300)
#define RCC_RTCCLKSource_HSE_Div6  ((uint32_t)0x00060300)
#define RCC_RTCCLKSource_HSE_Div7  ((uint32_t)0x00070300)
#define RCC_RTCCLKSource_HSE_Div8  ((uint32_t)0x00080300)
#define RCC_RTCCLKSource_HSE_Div9  ((uint32_t)0x00090300)
#define RCC_RTCCLKSource_HSE_Div10 ((uint32_t)0x000A0300)
#define RCC_RTCCLKSource_HSE_Div11 ((uint32_t)0x000B0300)
#define RCC_RTCCLKSource_HSE_Div12 ((uint32_t)0x000C0300)
#define RCC_RTCCLKSource_HSE_Div13 ((uint32_t)0x000D0300)
#define RCC_RTCCLKSource_HSE_Div14 ((uint32_t)0x000E0300)
#define RCC_RTCCLKSource_HSE_Div15 ((uint32_t)0x000F0300)
#define RCC_RTCCLKSource_HSE_Div16 ((uint32_t)0x00100300)
#define RCC_RTCCLKSource_HSE_Div17 ((uint32_t)0x00110300)
#define RCC_RTCCLKSource_HSE_Div18 ((uint32_t)0x00120300)
#define RCC_RTCCLKSource_HSE_Div19 ((uint32_t)0x00130300)
#define RCC_RTCCLKSource_HSE_Div20 ((uint32_t)0x00140300)
#define RCC_RTCCLKSource_HSE_Div21 ((uint32_t)0x00150300)
#define RCC_RTCCLKSource_HSE_Div22 ((uint32_t)0x00160300)
#define RCC_RTCCLKSource_HSE_Div23 ((uint32_t)0x00170300)
#define RCC_RTCCLKSource_HSE_Div24 ((uint32_t)0x00180300)
#define RCC_RTCCLKSource_HSE_Div25 ((uint32_t)0x00190300)
#define RCC_RTCCLKSource_HSE_Div26 ((uint32_t)0x001A0300)
#define RCC_RTCCLKSource_HSE_Div27 ((uint32_t)0x001B0300)
#define RCC_RTCCLKSource_HSE_Div28 ((uint32_t)0x001C0300)
#define RCC_RTCCLKSource_HSE_Div29 ((uint32_t)0x001D0300)
#define RCC_RTCCLKSource_HSE_Div30 ((uint32_t)0x001E0300)
#define RCC_RTCCLKSource_HSE_Div31 ((uint32_t)0x001F0300)
#define IS_RCC_RTCCLK_SOURCE(SOURCE)                                                                                                     \
    (((SOURCE) == RCC_RTCCLKSource_LSE) || ((SOURCE) == RCC_RTCCLKSource_LSI) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div2) ||              \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div3) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div4) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div5) ||    \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div6) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div7) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div8) ||    \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div9) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div10) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div11) ||  \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div12) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div13) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div14) || \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div15) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div16) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div17) || \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div18) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div19) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div20) || \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div21) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div22) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div23) || \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div24) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div25) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div26) || \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div27) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div28) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div29) || \
     ((SOURCE) == RCC_RTCCLKSource_HSE_Div30) || ((SOURCE) == RCC_RTCCLKSource_HSE_Div31))

// AHB1 Peripherals
#define RCC_AHB1Periph_GPIOA   BIT(PERIPH_GATE_GPIOA)
#define RCC_AHB1Periph_GPIOB   BIT(PERIPH_GATE_GPIOB)
#define RCC_AHB1Periph_GPIOC   BIT(PERIPH_GATE_GPIOC)
#define RCC_AHB1Periph_GPIOD   BIT(PERIPH_GATE_GPIOD)
#define RCC_AHB1Periph_GPIOE   BIT(PERIPH_GATE_GPIOE)
#define RCC_AHB1Periph_GPIOF   BIT(PERIPH_GATE_GPIOF)
#define RCC_AHB1Periph_GPIOG   BIT(PERIPH_GATE_GPIOG)
#define RCC_AHB1Periph_GPIOH   BIT(PERIPH_GATE_GPIOH)
#define RCC_AHB1Periph_GPIOI   BIT(PERIPH_GATE_GPIOI)
#define RCC_AHB1Periph_JPEGD   BIT(PERIPH_GATE_JPEGD)
#define RCC_AHB1Periph_CRC     BIT(PERIPH_GATE_CRC)
#define RCC_AHB1Periph_QSPI    BIT(PERIPH_GATE_QSPI)  // LP
#define RCC_AHB1Periph_SRAM1   BIT(PERIPH_GATE_SRAM1) // LP
#define RCC_AHB1Periph_SRAM2   BIT(PERIPH_GATE_SRAM2) // LP
#define RCC_AHB1Periph_SRAM3   BIT(PERIPH_GATE_SRAM3) // LP
#define RCC_AHB1Periph_SRAM4   BIT(PERIPH_GATE_SRAM4) // LP
#define RCC_AHB1Periph_SRAM5   BIT(PERIPH_GATE_SRAM5) // LP
#define RCC_AHB1Periph_CCM     BIT(PERIPH_GATE_CCM)   // EN
#define RCC_AHB1Periph_ETH     BIT(PERIPH_GATE_ETH)
#define RCC_AHB1Periph_ETH_TX  BIT(PERIPH_GATE_ETH_TX)  // EN / LP
#define RCC_AHB1Periph_ETH_RX  BIT(PERIPH_GATE_ETH_RX)  // EN / LP
#define RCC_AHB1Periph_ETH_PTP BIT(PERIPH_GATE_ETH_PTP) // EN / LP
#define RCC_AHB1Periph_GLB     BIT(PERIPH_GATE_GLB) // RST

#define RCC_AHB1Periph_RSVD       (BIT30 | BITS(24, 21) | BITS(14, 13) | BIT11 | BIT9)
#define RCC_AHB1Periph_ENR_RSVD   (RCC_AHB1Periph_RSVD | BITS(19, 15) | BIT31)
#define RCC_AHB1Periph_RSTR_RSVD  (RCC_AHB1Periph_RSVD | BITS(20, 15) | BITS(28, 26))
#define RCC_AHB1Periph_LPENR_RSVD (RCC_AHB1Periph_RSVD | BIT31)

#define RCC_AHB1Periph_GPIO_ALL BITM(PERIPH_GATE_GPIOI, PERIPH_GATE_GPIOA)
#define RCC_AHB1Periph_EN_ALL   (~RCC_AHB1Periph_ENR_RSVD)
#define RCC_AHB1Periph_RST_ALL  (~RCC_AHB1Periph_RSTR_RSVD)
#define RCC_AHB1Periph_LPEN_ALL (~RCC_AHB1Periph_LPENR_RSVD)

#define IS_RCC_AHB1_CLOCK_PERIPH(PERIPH)  ((((PERIPH) & RCC_AHB1Periph_ENR_RSVD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB1_RESET_PERIPH(PERIPH)  ((((PERIPH) & RCC_AHB1Periph_RSTR_RSVD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB1_LPMODE_PERIPH(PERIPH) ((((PERIPH) & RCC_AHB1Periph_LPENR_RSVD) == 0x00) && ((PERIPH) != 0x00))

// AHB2 Peripherals
#define RCC_AHB2Periph_FSMC               BIT(PERIPH_GATE_FSMC)
#define RCC_AHB2Periph_BPU                BIT(PERIPH_GATE_BPU)
#define RCC_AHB2Periph_OTP                BIT(PERIPH_GATE_OTP)
#define RCC_AHB2Periph_HSPI               BIT(PERIPH_GATE_HSPI)
#define RCC_AHB2Periph_DCMI               BIT(PERIPH_GATE_DCMI)
#define RCC_AHB2Periph_TRNG               BIT(PERIPH_GATE_TRNG)
#define RCC_AHB2Periph_USB1               BIT(PERIPH_GATE_USB1)

#define RCC_AHB2Periph_RSVD       (BITS(31, 11))
#define RCC_AHB2Periph_ENR_RSVD   (RCC_AHB2Periph_RSVD)
#define RCC_AHB2Periph_RSTR_RSVD  (RCC_AHB2Periph_RSVD | BIT2 | BIT5)
#define RCC_AHB2Periph_LPENR_RSVD (RCC_AHB2Periph_RSVD)

#define IS_RCC_AHB2_CLOCK_PERIPH(PERIPH)  ((((PERIPH) & RCC_AHB2Periph_ENR_RSVD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB2_RESET_PERIPH(PERIPH)  ((((PERIPH) & RCC_AHB2Periph_RSTR_RSVD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB2_LPMODE_PERIPH(PERIPH) ((((PERIPH) & RCC_AHB2Periph_LPENR_RSVD) == 0x00) && ((PERIPH) != 0x00))

// AHB3 Peripherals
#define RCC_AHB3Periph_SDRAM              BIT(PERIPH_GATE_SDRAM)

#define RCC_AHB3Periph_RSVD BITS(31, 1)

#define IS_RCC_AHB3_CLOCK_PERIPH(PERIPH)  ((((PERIPH) & RCC_AHB3Periph_RSVD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB3_RESET_PERIPH(PERIPH)  IS_RCC_AHB3_CLOCK_PERIPH(PERIPH)
#define IS_RCC_AHB3_LPMODE_PERIPH(PERIPH) IS_RCC_AHB3_CLOCK_PERIPH(PERIPH)

// AHB4 Peripherals
#define RCC_AHB4Periph_DMA1               BIT(PERIPH_GATE_DMA1)
#define RCC_AHB4Periph_DMA2               BIT(PERIPH_GATE_DMA2)
#define RCC_AHB4Periph_DMA2D              BIT(PERIPH_GATE_DMA2D)
#define RCC_AHB4Periph_LTDC               BIT(PERIPH_GATE_LTDC)
#define RCC_AHB4Periph_IPI                BIT(PERIPH_GATE_IPI)
#define RCC_AHB4Periph_XSPI               BIT(PERIPH_GATE_XSPI)

#define RCC_AHB4Periph_RSVD (BITS(31, 7))

#define IS_RCC_AHB4_CLOCK_PERIPH(PERIPH)  ((((PERIPH) & RCC_AHB4Periph_RSVD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB4_RESET_PERIPH(PERIPH)  IS_RCC_AHB4_CLOCK_PERIPH(PERIPH)
#define IS_RCC_AHB4_LPMODE_PERIPH(PERIPH) IS_RCC_AHB4_CLOCK_PERIPH(PERIPH)

// APB1 Peripherals
#define RCC_APB1Periph_TIM2               BIT(PERIPH_GATE_TIM2)
#define RCC_APB1Periph_TIM3               BIT(PERIPH_GATE_TIM3)
#define RCC_APB1Periph_TIM4               BIT(PERIPH_GATE_TIM4)
#define RCC_APB1Periph_TIM5               BIT(PERIPH_GATE_TIM5)
#define RCC_APB1Periph_TIM6               BIT(PERIPH_GATE_TIM6)
#define RCC_APB1Periph_TIM7               BIT(PERIPH_GATE_TIM7)
#define RCC_APB1Periph_TIM12              BIT(PERIPH_GATE_TIM12)
#define RCC_APB1Periph_TIM13              BIT(PERIPH_GATE_TIM13)
#define RCC_APB1Periph_TIM14              BIT(PERIPH_GATE_TIM14)
#define RCC_APB1Periph_WWDG               BIT(PERIPH_GATE_WWDG)
#define RCC_APB1Periph_SPI2               BIT(PERIPH_GATE_SPI2)
#define RCC_APB1Periph_SPI3               BIT(PERIPH_GATE_SPI3)
#define RCC_APB1Periph_USART2             BIT(PERIPH_GATE_USART2)
#define RCC_APB1Periph_USART3             BIT(PERIPH_GATE_USART3)
#define RCC_APB1Periph_UART4              BIT(PERIPH_GATE_UART4)
#define RCC_APB1Periph_I2C1               BIT(PERIPH_GATE_I2C1)
#define RCC_APB1Periph_I2C2               BIT(PERIPH_GATE_I2C2)
#define RCC_APB1Periph_I2C3               BIT(PERIPH_GATE_I2C3)
#define RCC_APB1Periph_CAN1               BIT(PERIPH_GATE_CAN1)
#define RCC_APB1Periph_CAN2               BIT(PERIPH_GATE_CAN2)
#define RCC_APB1Periph_PWR                BIT(PERIPH_GATE_PWR)
#define RCC_APB1Periph_DAC                BIT(PERIPH_GATE_DAC)
#define RCC_APB1Periph_UART7              BIT(PERIPH_GATE_UART7)

#define RCC_APB1Periph_RSVD (BIT(27) | BIT(24) | BIT(16) | BITS(13, 12) | BITS(10, 9))

#define IS_RCC_APB1_PERIPH(PERIPH) ((((PERIPH) & RCC_APB1Periph_RSVD) == 0x00) && ((PERIPH) != 0x00))

// APB2 Peripherals
#define RCC_APB2Periph_TIM1        BIT(PERIPH_GATE_TIM1)
#define RCC_APB2Periph_TIM8        BIT(PERIPH_GATE_TIM8)
#define RCC_APB2Periph_USART1      BIT(PERIPH_GATE_USART1)
#define RCC_APB2Periph_USART6      BIT(PERIPH_GATE_USART6)
#define RCC_APB2Periph_ADC1        BIT(PERIPH_GATE_ADC1)
#define RCC_APB2Periph_ADC2        BIT(PERIPH_GATE_ADC2)
#define RCC_APB2Periph_ADC3        BIT(PERIPH_GATE_ADC3)
#define RCC_APB2Periph_SDIO        BIT(PERIPH_GATE_SDIO)
#define RCC_APB2Periph_SPI1        BIT(PERIPH_GATE_SPI1)
#define RCC_APB2Periph_SPI4        BIT(PERIPH_GATE_SPI4)
#define RCC_APB2Periph_SYSCFG      BIT(PERIPH_GATE_SYSCFG)
#define RCC_APB2Periph_PCFG        BIT(PERIPH_GATE_PCFG)
#define RCC_APB2Periph_TIM9        BIT(PERIPH_GATE_TIM9)
#define RCC_APB2Periph_TIM10       BIT(PERIPH_GATE_TIM10)
#define RCC_APB2Periph_TIM11       BIT(PERIPH_GATE_TIM11)
#define RCC_APB2Periph_DSI         BIT(PERIPH_GATE_DSI)

#define RCC_APB2Periph_RSVD      (BITS(31, 29) | BIT(19) | BIT(15) | BITS(7, 6) | BITS(3, 2))
#define RCC_APB2Periph_RSTR_RSVD (RCC_APB2Periph_RSVD | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3)

#define IS_RCC_APB2_PERIPH(PERIPH)       ((((PERIPH) & RCC_APB2Periph_RSVD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_APB2_RESET_PERIPH(PERIPH) ((((PERIPH) & RCC_APB2Periph_RSTR_RSVD) == 0x00) && ((PERIPH) != 0x00))


#define RCC_MCO1Source_HSI    ((uint32_t)0x00000000)
#define RCC_MCO1Source_LSE    BIT21
#define RCC_MCO1Source_HSE    BIT22
#define RCC_MCO1Source_PLLCLK BITS(22, 21)
#define RCC_MCO1Div_1         ((uint32_t)0x00000000)
#define RCC_MCO1Div_2         BIT26
#define RCC_MCO1Div_3         (BIT26 | BIT24)
#define RCC_MCO1Div_4         BITS(26, 25)
#define RCC_MCO1Div_5         BITS(26, 24)
#define IS_RCC_MCO1SOURCE(SOURCE) \
    (((SOURCE) == RCC_MCO1Source_HSI) || ((SOURCE) == RCC_MCO1Source_LSE) || ((SOURCE) == RCC_MCO1Source_HSE) || ((SOURCE) == RCC_MCO1Source_PLLCLK))

#define IS_RCC_MCO1DIV(DIV) \
    (((DIV) == RCC_MCO1Div_1) || ((DIV) == RCC_MCO1Div_2) || ((DIV) == RCC_MCO1Div_3) || ((DIV) == RCC_MCO1Div_4) || ((DIV) == RCC_MCO1Div_5))

#define RCC_MCO2Source_SYSCLK    ((uint32_t)0x00000000)
#define RCC_MCO2Source_PLLI2SCLK BIT30
#define RCC_MCO2Source_HSE       BIT31
#define RCC_MCO2Source_PLLCLK    BITS(31, 30)
#define RCC_MCO2Div_1            ((uint32_t)0x00000000)
#define RCC_MCO2Div_2            BIT29
#define RCC_MCO2Div_3            (BIT29 | BIT27)
#define RCC_MCO2Div_4            BITS(29, 28)
#define RCC_MCO2Div_5            BITS(29, 27)
#define IS_RCC_MCO2SOURCE(SOURCE) \
    (((SOURCE) == RCC_MCO2Source_SYSCLK) || ((SOURCE) == RCC_MCO2Source_PLLI2SCLK) || ((SOURCE) == RCC_MCO2Source_HSE) || ((SOURCE) == RCC_MCO2Source_PLLCLK))

#define IS_RCC_MCO2DIV(DIV) \
    (((DIV) == RCC_MCO2Div_1) || ((DIV) == RCC_MCO2Div_2) || ((DIV) == RCC_MCO2Div_3) || ((DIV) == RCC_MCO2Div_4) || ((DIV) == RCC_MCO2Div_5))

#define RCC_I2S2CLKSource_PLL3       ((uint32_t)0x00000000)
#define RCC_I2S2CLKSource_Ext        RCC_CFGR_I2SSRC
#define IS_RCC_I2SCLK_SOURCE(SOURCE) (((SOURCE) == RCC_I2S2CLKSource_PLL3) || ((SOURCE) == RCC_I2S2CLKSource_Ext))

#define RCC_FLAG_HSIRDY  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY  ((uint8_t)0x31)
#define RCC_FLAG_PLL1RDY ((uint8_t)0x39)
#define RCC_FLAG_PLL2RDY ((uint8_t)0x3B)
#define RCC_FLAG_LSERDY  ((uint8_t)0x41)
#define RCC_FLAG_LSIRDY  ((uint8_t)0x61)
#define RCC_FLAG_PINRST  ((uint8_t)0x7A)
#define RCC_FLAG_PORRST  ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST ((uint8_t)0x7F)
#define RCC_FLAG_PLL3RDY ((uint8_t)0x3D)
#define RCC_FLAG_GLBRST  ((uint8_t)0x76)
#define RCC_FLAG_BATTRST ((uint8_t)0x77)
#define RCC_FLAG_MATTRST ((uint8_t)0x79)

#define IS_RCC_FLAG(FLAG)                                                                                                            \
    (((FLAG) == RCC_FLAG_HSIRDY) || ((FLAG) == RCC_FLAG_HSERDY) || ((FLAG) == RCC_FLAG_PLL1RDY) || ((FLAG) == RCC_FLAG_LSERDY) ||    \
     ((FLAG) == RCC_FLAG_LSIRDY) || ((FLAG) == RCC_FLAG_PINRST) || ((FLAG) == RCC_FLAG_PORRST) || ((FLAG) == RCC_FLAG_SFTRST) ||     \
     ((FLAG) == RCC_FLAG_IWDGRST) || ((FLAG) == RCC_FLAG_WWDGRST) || ((FLAG) == RCC_FLAG_LPWRRST) || ((FLAG) == RCC_FLAG_PLL2RDY) || \
     ((FLAG) == RCC_FLAG_PLL3RDY) || ((FLAG) == RCC_FLAG_GLBRST) || ((FLAG) == RCC_FLAG_BATTRST) || ((FLAG) == RCC_FLAG_MATTRST))

#define IS_RCC_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1F)

#define SYSCFG_MemoryRemap_SRAM4_5            BIT0
#define SYSCFG_MemoryRemap_SRAM5              BIT1
#define SYSCFG_MemoryRemap_SDRAM              BIT3
#define IS_SYSCFG_MEMORY_REMAP_CONFING(REMAP) ((REMAP & (BITS(3, 0) | BIT31)) != 0)

#define PCFG_USB_DOWNLOAD_FLAG 	(0x55U)

#define SYSCTRL_CHIP_SN_ADDR 	(0x50076006)
#define SYSCTRL_CHIP_SN_LEN  	(0x10)


/* Internal/external clocks, PLL, CSS and MCO configuration functions *********/
void        RCC_DeInit(void);
ErrorStatus RCC_HSECmd(FunctionalState NewState);
void        RCC_HSICmd(FunctionalState NewState);
void        RCC_LSEConfig(uint8_t RCC_LSE);
void        RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
void        RCC_PLL1Cmd(FunctionalState NewState);
void        RCC_PLL1Config(uint32_t PLLSource, uint32_t m, uint32_t n, uint32_t p, uint32_t q, uint32_t r);
void        RCC_PLL2Cmd(FunctionalState NewState);
void        RCC_PLL2Config(uint32_t n, uint32_t p, uint32_t q, uint32_t r);
void        RCC_PLL3Config(uint32_t multiple, uint32_t e, uint32_t n, uint32_t r, uint32_t u, uint32_t i);
void        RCC_PLL3Cmd(FunctionalState NewState);
#define RCC_HSEConfig RCC_HSECmd
#define RCC_PLLConfig RCC_PLL1Config
#define RCC_PLLCmd    RCC_PLL1Cmd
void RCC_ClockSecuritySystemCmd(FunctionalState NewState);
void RCC_MCO1Config(uint32_t RCC_MCO1Source, uint32_t RCC_MCO1Div);
void RCC_MCO2Config(uint32_t RCC_MCO2Source, uint32_t RCC_MCO2Div);

/* System, AHB and APB busses clocks configuration functions ******************/
void    RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);
void    RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void    RCC_PCLK1Config(uint32_t RCC_HCLK);
void    RCC_PCLK2Config(uint32_t RCC_HCLK);
uint8_t RCC_GetSYSCLKSource(void);
void    RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

/* Peripheral clocks configuration functions **********************************/
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource);
void RCC_BackupResetCmd(FunctionalState NewState);
void RCC_I2SCLKConfig(uint32_t RCC_I2SCLKSource);

// Peripheral Enable Command
void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void RCC_AHB4PeriphClockCmd(uint32_t RCC_AHB4Periph, FunctionalState NewState);

void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

// Peripheral Reset Command
void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void RCC_AHB4PeriphResetCmd(uint32_t RCC_AHB4Periph, FunctionalState NewState);

void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

// Peripheral Low Power Mode Command
void RCC_AHB1PeriphClockLPModeCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphClockLPModeCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphClockLPModeCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void RCC_AHB4PeriphClockLPModeCmd(uint32_t RCC_AHB4Periph, FunctionalState NewState);

void RCC_APB1PeriphClockLPModeCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void RCC_APB2PeriphClockLPModeCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void       RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState);
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG);
void       RCC_ClearFlag(void);
ITStatus   RCC_GetITStatus(uint8_t RCC_IT);
void       RCC_ClearITPendingBit(uint8_t RCC_IT);

// SYSCFG Functions
extern void SYSCFG_DeInit(void);
extern void SYSCFG_MemoryRemapConfig(uint32_t SYSCFG_MemoryRemap);
extern void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex);

// PCFG Functions
extern void PCFG_EnterUSBDownload(void);

//SN Functions
void SYSCTRL_GetChipSN(uint8_t *ChipSN);
#ifdef __cplusplus
}
#endif

#endif // __SYSCTRL_H__
