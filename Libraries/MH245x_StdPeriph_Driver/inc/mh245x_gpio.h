// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_GPIO_H__
#define __PERIPH_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mh2457.h"

/* --------  Feature Definition  -------------------------------------------- */
/*!
    @brief Number of GPIO groups
*/
#ifndef FEATURE_GPIO_GROUP_NUM
#define FEATURE_GPIO_GROUP_NUM 16
#endif

/* --------  Register Definition  ------------------------------------------- */

/**
 * @brief General Purpose I/O
 */
typedef struct
{
    __IO uint32_t MODER;   /*!< GPIO port mode register,               Address offset: 0x00      */
    __IO uint32_t OTYPER;  /*!< GPIO port output type register,        Address offset: 0x04      */
    __IO uint32_t OSPEEDR; /*!< GPIO port output speed register,       Address offset: 0x08      */
    __IO uint32_t PUPDR;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    __IO uint32_t IDR;     /*!< GPIO port input data register,         Address offset: 0x10      */
    __IO uint32_t ODR;     /*!< GPIO port output data register,        Address offset: 0x14      */
    union
    {
        __IO uint32_t BSRR; /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
        struct
        {
            __IO uint16_t BSRRL;
            __IO uint16_t BSRRH;
        };
    };
    __IO uint32_t LCKR;   /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    __IO uint32_t AFR[2]; /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    __I uint32_t  RSVD28[50]; /*!< Address offset: 0x28-0xEC */
    __IO uint32_t DSR;        /*!< GPIO drive strength registers,          Address offset: 0xF0 */
} GPIO_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/******************  Bits definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODER0   ((uint32_t)0x00000003)
#define GPIO_MODER_MODER0_0 ((uint32_t)0x00000001)
#define GPIO_MODER_MODER0_1 ((uint32_t)0x00000002)

#define GPIO_MODER_MODER1   ((uint32_t)0x0000000C)
#define GPIO_MODER_MODER1_0 ((uint32_t)0x00000004)
#define GPIO_MODER_MODER1_1 ((uint32_t)0x00000008)

#define GPIO_MODER_MODER2   ((uint32_t)0x00000030)
#define GPIO_MODER_MODER2_0 ((uint32_t)0x00000010)
#define GPIO_MODER_MODER2_1 ((uint32_t)0x00000020)

#define GPIO_MODER_MODER3   ((uint32_t)0x000000C0)
#define GPIO_MODER_MODER3_0 ((uint32_t)0x00000040)
#define GPIO_MODER_MODER3_1 ((uint32_t)0x00000080)

#define GPIO_MODER_MODER4   ((uint32_t)0x00000300)
#define GPIO_MODER_MODER4_0 ((uint32_t)0x00000100)
#define GPIO_MODER_MODER4_1 ((uint32_t)0x00000200)

#define GPIO_MODER_MODER5   ((uint32_t)0x00000C00)
#define GPIO_MODER_MODER5_0 ((uint32_t)0x00000400)
#define GPIO_MODER_MODER5_1 ((uint32_t)0x00000800)

#define GPIO_MODER_MODER6   ((uint32_t)0x00003000)
#define GPIO_MODER_MODER6_0 ((uint32_t)0x00001000)
#define GPIO_MODER_MODER6_1 ((uint32_t)0x00002000)

#define GPIO_MODER_MODER7   ((uint32_t)0x0000C000)
#define GPIO_MODER_MODER7_0 ((uint32_t)0x00004000)
#define GPIO_MODER_MODER7_1 ((uint32_t)0x00008000)

#define GPIO_MODER_MODER8   ((uint32_t)0x00030000)
#define GPIO_MODER_MODER8_0 ((uint32_t)0x00010000)
#define GPIO_MODER_MODER8_1 ((uint32_t)0x00020000)

#define GPIO_MODER_MODER9   ((uint32_t)0x000C0000)
#define GPIO_MODER_MODER9_0 ((uint32_t)0x00040000)
#define GPIO_MODER_MODER9_1 ((uint32_t)0x00080000)

#define GPIO_MODER_MODER10   ((uint32_t)0x00300000)
#define GPIO_MODER_MODER10_0 ((uint32_t)0x00100000)
#define GPIO_MODER_MODER10_1 ((uint32_t)0x00200000)

#define GPIO_MODER_MODER11   ((uint32_t)0x00C00000)
#define GPIO_MODER_MODER11_0 ((uint32_t)0x00400000)
#define GPIO_MODER_MODER11_1 ((uint32_t)0x00800000)

#define GPIO_MODER_MODER12   ((uint32_t)0x03000000)
#define GPIO_MODER_MODER12_0 ((uint32_t)0x01000000)
#define GPIO_MODER_MODER12_1 ((uint32_t)0x02000000)

#define GPIO_MODER_MODER13   ((uint32_t)0x0C000000)
#define GPIO_MODER_MODER13_0 ((uint32_t)0x04000000)
#define GPIO_MODER_MODER13_1 ((uint32_t)0x08000000)

#define GPIO_MODER_MODER14   ((uint32_t)0x30000000)
#define GPIO_MODER_MODER14_0 ((uint32_t)0x10000000)
#define GPIO_MODER_MODER14_1 ((uint32_t)0x20000000)

#define GPIO_MODER_MODER15   ((uint32_t)0xC0000000)
#define GPIO_MODER_MODER15_0 ((uint32_t)0x40000000)
#define GPIO_MODER_MODER15_1 ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_OTYPER register  ****************/
#define GPIO_OTYPER_OT_0  ((uint32_t)0x00000001)
#define GPIO_OTYPER_OT_1  ((uint32_t)0x00000002)
#define GPIO_OTYPER_OT_2  ((uint32_t)0x00000004)
#define GPIO_OTYPER_OT_3  ((uint32_t)0x00000008)
#define GPIO_OTYPER_OT_4  ((uint32_t)0x00000010)
#define GPIO_OTYPER_OT_5  ((uint32_t)0x00000020)
#define GPIO_OTYPER_OT_6  ((uint32_t)0x00000040)
#define GPIO_OTYPER_OT_7  ((uint32_t)0x00000080)
#define GPIO_OTYPER_OT_8  ((uint32_t)0x00000100)
#define GPIO_OTYPER_OT_9  ((uint32_t)0x00000200)
#define GPIO_OTYPER_OT_10 ((uint32_t)0x00000400)
#define GPIO_OTYPER_OT_11 ((uint32_t)0x00000800)
#define GPIO_OTYPER_OT_12 ((uint32_t)0x00001000)
#define GPIO_OTYPER_OT_13 ((uint32_t)0x00002000)
#define GPIO_OTYPER_OT_14 ((uint32_t)0x00004000)
#define GPIO_OTYPER_OT_15 ((uint32_t)0x00008000)

/******************  Bits definition for GPIO_OSPEEDR register  ***************/
#define GPIO_OSPEEDER_OSPEEDR0   ((uint32_t)0x00000003)
#define GPIO_OSPEEDER_OSPEEDR0_0 ((uint32_t)0x00000001)
#define GPIO_OSPEEDER_OSPEEDR0_1 ((uint32_t)0x00000002)

#define GPIO_OSPEEDER_OSPEEDR1   ((uint32_t)0x0000000C)
#define GPIO_OSPEEDER_OSPEEDR1_0 ((uint32_t)0x00000004)
#define GPIO_OSPEEDER_OSPEEDR1_1 ((uint32_t)0x00000008)

#define GPIO_OSPEEDER_OSPEEDR2   ((uint32_t)0x00000030)
#define GPIO_OSPEEDER_OSPEEDR2_0 ((uint32_t)0x00000010)
#define GPIO_OSPEEDER_OSPEEDR2_1 ((uint32_t)0x00000020)

#define GPIO_OSPEEDER_OSPEEDR3   ((uint32_t)0x000000C0)
#define GPIO_OSPEEDER_OSPEEDR3_0 ((uint32_t)0x00000040)
#define GPIO_OSPEEDER_OSPEEDR3_1 ((uint32_t)0x00000080)

#define GPIO_OSPEEDER_OSPEEDR4   ((uint32_t)0x00000300)
#define GPIO_OSPEEDER_OSPEEDR4_0 ((uint32_t)0x00000100)
#define GPIO_OSPEEDER_OSPEEDR4_1 ((uint32_t)0x00000200)

#define GPIO_OSPEEDER_OSPEEDR5   ((uint32_t)0x00000C00)
#define GPIO_OSPEEDER_OSPEEDR5_0 ((uint32_t)0x00000400)
#define GPIO_OSPEEDER_OSPEEDR5_1 ((uint32_t)0x00000800)

#define GPIO_OSPEEDER_OSPEEDR6   ((uint32_t)0x00003000)
#define GPIO_OSPEEDER_OSPEEDR6_0 ((uint32_t)0x00001000)
#define GPIO_OSPEEDER_OSPEEDR6_1 ((uint32_t)0x00002000)

#define GPIO_OSPEEDER_OSPEEDR7   ((uint32_t)0x0000C000)
#define GPIO_OSPEEDER_OSPEEDR7_0 ((uint32_t)0x00004000)
#define GPIO_OSPEEDER_OSPEEDR7_1 ((uint32_t)0x00008000)

#define GPIO_OSPEEDER_OSPEEDR8   ((uint32_t)0x00030000)
#define GPIO_OSPEEDER_OSPEEDR8_0 ((uint32_t)0x00010000)
#define GPIO_OSPEEDER_OSPEEDR8_1 ((uint32_t)0x00020000)

#define GPIO_OSPEEDER_OSPEEDR9   ((uint32_t)0x000C0000)
#define GPIO_OSPEEDER_OSPEEDR9_0 ((uint32_t)0x00040000)
#define GPIO_OSPEEDER_OSPEEDR9_1 ((uint32_t)0x00080000)

#define GPIO_OSPEEDER_OSPEEDR10   ((uint32_t)0x00300000)
#define GPIO_OSPEEDER_OSPEEDR10_0 ((uint32_t)0x00100000)
#define GPIO_OSPEEDER_OSPEEDR10_1 ((uint32_t)0x00200000)

#define GPIO_OSPEEDER_OSPEEDR11   ((uint32_t)0x00C00000)
#define GPIO_OSPEEDER_OSPEEDR11_0 ((uint32_t)0x00400000)
#define GPIO_OSPEEDER_OSPEEDR11_1 ((uint32_t)0x00800000)

#define GPIO_OSPEEDER_OSPEEDR12   ((uint32_t)0x03000000)
#define GPIO_OSPEEDER_OSPEEDR12_0 ((uint32_t)0x01000000)
#define GPIO_OSPEEDER_OSPEEDR12_1 ((uint32_t)0x02000000)

#define GPIO_OSPEEDER_OSPEEDR13   ((uint32_t)0x0C000000)
#define GPIO_OSPEEDER_OSPEEDR13_0 ((uint32_t)0x04000000)
#define GPIO_OSPEEDER_OSPEEDR13_1 ((uint32_t)0x08000000)

#define GPIO_OSPEEDER_OSPEEDR14   ((uint32_t)0x30000000)
#define GPIO_OSPEEDER_OSPEEDR14_0 ((uint32_t)0x10000000)
#define GPIO_OSPEEDER_OSPEEDR14_1 ((uint32_t)0x20000000)

#define GPIO_OSPEEDER_OSPEEDR15   ((uint32_t)0xC0000000)
#define GPIO_OSPEEDER_OSPEEDR15_0 ((uint32_t)0x40000000)
#define GPIO_OSPEEDER_OSPEEDR15_1 ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_PUPDR register  *****************/
#define GPIO_PUPDR_PUPDR0   ((uint32_t)0x00000003)
#define GPIO_PUPDR_PUPDR0_0 ((uint32_t)0x00000001)
#define GPIO_PUPDR_PUPDR0_1 ((uint32_t)0x00000002)

#define GPIO_PUPDR_PUPDR1   ((uint32_t)0x0000000C)
#define GPIO_PUPDR_PUPDR1_0 ((uint32_t)0x00000004)
#define GPIO_PUPDR_PUPDR1_1 ((uint32_t)0x00000008)

#define GPIO_PUPDR_PUPDR2   ((uint32_t)0x00000030)
#define GPIO_PUPDR_PUPDR2_0 ((uint32_t)0x00000010)
#define GPIO_PUPDR_PUPDR2_1 ((uint32_t)0x00000020)

#define GPIO_PUPDR_PUPDR3   ((uint32_t)0x000000C0)
#define GPIO_PUPDR_PUPDR3_0 ((uint32_t)0x00000040)
#define GPIO_PUPDR_PUPDR3_1 ((uint32_t)0x00000080)

#define GPIO_PUPDR_PUPDR4   ((uint32_t)0x00000300)
#define GPIO_PUPDR_PUPDR4_0 ((uint32_t)0x00000100)
#define GPIO_PUPDR_PUPDR4_1 ((uint32_t)0x00000200)

#define GPIO_PUPDR_PUPDR5   ((uint32_t)0x00000C00)
#define GPIO_PUPDR_PUPDR5_0 ((uint32_t)0x00000400)
#define GPIO_PUPDR_PUPDR5_1 ((uint32_t)0x00000800)

#define GPIO_PUPDR_PUPDR6   ((uint32_t)0x00003000)
#define GPIO_PUPDR_PUPDR6_0 ((uint32_t)0x00001000)
#define GPIO_PUPDR_PUPDR6_1 ((uint32_t)0x00002000)

#define GPIO_PUPDR_PUPDR7   ((uint32_t)0x0000C000)
#define GPIO_PUPDR_PUPDR7_0 ((uint32_t)0x00004000)
#define GPIO_PUPDR_PUPDR7_1 ((uint32_t)0x00008000)

#define GPIO_PUPDR_PUPDR8   ((uint32_t)0x00030000)
#define GPIO_PUPDR_PUPDR8_0 ((uint32_t)0x00010000)
#define GPIO_PUPDR_PUPDR8_1 ((uint32_t)0x00020000)

#define GPIO_PUPDR_PUPDR9   ((uint32_t)0x000C0000)
#define GPIO_PUPDR_PUPDR9_0 ((uint32_t)0x00040000)
#define GPIO_PUPDR_PUPDR9_1 ((uint32_t)0x00080000)

#define GPIO_PUPDR_PUPDR10   ((uint32_t)0x00300000)
#define GPIO_PUPDR_PUPDR10_0 ((uint32_t)0x00100000)
#define GPIO_PUPDR_PUPDR10_1 ((uint32_t)0x00200000)

#define GPIO_PUPDR_PUPDR11   ((uint32_t)0x00C00000)
#define GPIO_PUPDR_PUPDR11_0 ((uint32_t)0x00400000)
#define GPIO_PUPDR_PUPDR11_1 ((uint32_t)0x00800000)

#define GPIO_PUPDR_PUPDR12   ((uint32_t)0x03000000)
#define GPIO_PUPDR_PUPDR12_0 ((uint32_t)0x01000000)
#define GPIO_PUPDR_PUPDR12_1 ((uint32_t)0x02000000)

#define GPIO_PUPDR_PUPDR13   ((uint32_t)0x0C000000)
#define GPIO_PUPDR_PUPDR13_0 ((uint32_t)0x04000000)
#define GPIO_PUPDR_PUPDR13_1 ((uint32_t)0x08000000)

#define GPIO_PUPDR_PUPDR14   ((uint32_t)0x30000000)
#define GPIO_PUPDR_PUPDR14_0 ((uint32_t)0x10000000)
#define GPIO_PUPDR_PUPDR14_1 ((uint32_t)0x20000000)

#define GPIO_PUPDR_PUPDR15   ((uint32_t)0xC0000000)
#define GPIO_PUPDR_PUPDR15_0 ((uint32_t)0x40000000)
#define GPIO_PUPDR_PUPDR15_1 ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_IDR register  *******************/
#define GPIO_IDR_IDR_0  ((uint32_t)0x00000001)
#define GPIO_IDR_IDR_1  ((uint32_t)0x00000002)
#define GPIO_IDR_IDR_2  ((uint32_t)0x00000004)
#define GPIO_IDR_IDR_3  ((uint32_t)0x00000008)
#define GPIO_IDR_IDR_4  ((uint32_t)0x00000010)
#define GPIO_IDR_IDR_5  ((uint32_t)0x00000020)
#define GPIO_IDR_IDR_6  ((uint32_t)0x00000040)
#define GPIO_IDR_IDR_7  ((uint32_t)0x00000080)
#define GPIO_IDR_IDR_8  ((uint32_t)0x00000100)
#define GPIO_IDR_IDR_9  ((uint32_t)0x00000200)
#define GPIO_IDR_IDR_10 ((uint32_t)0x00000400)
#define GPIO_IDR_IDR_11 ((uint32_t)0x00000800)
#define GPIO_IDR_IDR_12 ((uint32_t)0x00001000)
#define GPIO_IDR_IDR_13 ((uint32_t)0x00002000)
#define GPIO_IDR_IDR_14 ((uint32_t)0x00004000)
#define GPIO_IDR_IDR_15 ((uint32_t)0x00008000)
/* Old GPIO_IDR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_IDR_0  GPIO_IDR_IDR_0
#define GPIO_OTYPER_IDR_1  GPIO_IDR_IDR_1
#define GPIO_OTYPER_IDR_2  GPIO_IDR_IDR_2
#define GPIO_OTYPER_IDR_3  GPIO_IDR_IDR_3
#define GPIO_OTYPER_IDR_4  GPIO_IDR_IDR_4
#define GPIO_OTYPER_IDR_5  GPIO_IDR_IDR_5
#define GPIO_OTYPER_IDR_6  GPIO_IDR_IDR_6
#define GPIO_OTYPER_IDR_7  GPIO_IDR_IDR_7
#define GPIO_OTYPER_IDR_8  GPIO_IDR_IDR_8
#define GPIO_OTYPER_IDR_9  GPIO_IDR_IDR_9
#define GPIO_OTYPER_IDR_10 GPIO_IDR_IDR_10
#define GPIO_OTYPER_IDR_11 GPIO_IDR_IDR_11
#define GPIO_OTYPER_IDR_12 GPIO_IDR_IDR_12
#define GPIO_OTYPER_IDR_13 GPIO_IDR_IDR_13
#define GPIO_OTYPER_IDR_14 GPIO_IDR_IDR_14
#define GPIO_OTYPER_IDR_15 GPIO_IDR_IDR_15

/******************  Bits definition for GPIO_ODR register  *******************/
#define GPIO_ODR_ODR_0  ((uint32_t)0x00000001)
#define GPIO_ODR_ODR_1  ((uint32_t)0x00000002)
#define GPIO_ODR_ODR_2  ((uint32_t)0x00000004)
#define GPIO_ODR_ODR_3  ((uint32_t)0x00000008)
#define GPIO_ODR_ODR_4  ((uint32_t)0x00000010)
#define GPIO_ODR_ODR_5  ((uint32_t)0x00000020)
#define GPIO_ODR_ODR_6  ((uint32_t)0x00000040)
#define GPIO_ODR_ODR_7  ((uint32_t)0x00000080)
#define GPIO_ODR_ODR_8  ((uint32_t)0x00000100)
#define GPIO_ODR_ODR_9  ((uint32_t)0x00000200)
#define GPIO_ODR_ODR_10 ((uint32_t)0x00000400)
#define GPIO_ODR_ODR_11 ((uint32_t)0x00000800)
#define GPIO_ODR_ODR_12 ((uint32_t)0x00001000)
#define GPIO_ODR_ODR_13 ((uint32_t)0x00002000)
#define GPIO_ODR_ODR_14 ((uint32_t)0x00004000)
#define GPIO_ODR_ODR_15 ((uint32_t)0x00008000)
/* Old GPIO_ODR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_ODR_0  GPIO_ODR_ODR_0
#define GPIO_OTYPER_ODR_1  GPIO_ODR_ODR_1
#define GPIO_OTYPER_ODR_2  GPIO_ODR_ODR_2
#define GPIO_OTYPER_ODR_3  GPIO_ODR_ODR_3
#define GPIO_OTYPER_ODR_4  GPIO_ODR_ODR_4
#define GPIO_OTYPER_ODR_5  GPIO_ODR_ODR_5
#define GPIO_OTYPER_ODR_6  GPIO_ODR_ODR_6
#define GPIO_OTYPER_ODR_7  GPIO_ODR_ODR_7
#define GPIO_OTYPER_ODR_8  GPIO_ODR_ODR_8
#define GPIO_OTYPER_ODR_9  GPIO_ODR_ODR_9
#define GPIO_OTYPER_ODR_10 GPIO_ODR_ODR_10
#define GPIO_OTYPER_ODR_11 GPIO_ODR_ODR_11
#define GPIO_OTYPER_ODR_12 GPIO_ODR_ODR_12
#define GPIO_OTYPER_ODR_13 GPIO_ODR_ODR_13
#define GPIO_OTYPER_ODR_14 GPIO_ODR_ODR_14
#define GPIO_OTYPER_ODR_15 GPIO_ODR_ODR_15

/******************  Bits definition for GPIO_BSRR register  ******************/
#define GPIO_BSRR_BS_0  ((uint32_t)0x00000001)
#define GPIO_BSRR_BS_1  ((uint32_t)0x00000002)
#define GPIO_BSRR_BS_2  ((uint32_t)0x00000004)
#define GPIO_BSRR_BS_3  ((uint32_t)0x00000008)
#define GPIO_BSRR_BS_4  ((uint32_t)0x00000010)
#define GPIO_BSRR_BS_5  ((uint32_t)0x00000020)
#define GPIO_BSRR_BS_6  ((uint32_t)0x00000040)
#define GPIO_BSRR_BS_7  ((uint32_t)0x00000080)
#define GPIO_BSRR_BS_8  ((uint32_t)0x00000100)
#define GPIO_BSRR_BS_9  ((uint32_t)0x00000200)
#define GPIO_BSRR_BS_10 ((uint32_t)0x00000400)
#define GPIO_BSRR_BS_11 ((uint32_t)0x00000800)
#define GPIO_BSRR_BS_12 ((uint32_t)0x00001000)
#define GPIO_BSRR_BS_13 ((uint32_t)0x00002000)
#define GPIO_BSRR_BS_14 ((uint32_t)0x00004000)
#define GPIO_BSRR_BS_15 ((uint32_t)0x00008000)
#define GPIO_BSRR_BR_0  ((uint32_t)0x00010000)
#define GPIO_BSRR_BR_1  ((uint32_t)0x00020000)
#define GPIO_BSRR_BR_2  ((uint32_t)0x00040000)
#define GPIO_BSRR_BR_3  ((uint32_t)0x00080000)
#define GPIO_BSRR_BR_4  ((uint32_t)0x00100000)
#define GPIO_BSRR_BR_5  ((uint32_t)0x00200000)
#define GPIO_BSRR_BR_6  ((uint32_t)0x00400000)
#define GPIO_BSRR_BR_7  ((uint32_t)0x00800000)
#define GPIO_BSRR_BR_8  ((uint32_t)0x01000000)
#define GPIO_BSRR_BR_9  ((uint32_t)0x02000000)
#define GPIO_BSRR_BR_10 ((uint32_t)0x04000000)
#define GPIO_BSRR_BR_11 ((uint32_t)0x08000000)
#define GPIO_BSRR_BR_12 ((uint32_t)0x10000000)
#define GPIO_BSRR_BR_13 ((uint32_t)0x20000000)
#define GPIO_BSRR_BR_14 ((uint32_t)0x40000000)
#define GPIO_BSRR_BR_15 ((uint32_t)0x80000000)

/* --------  Exported types  ------------------------------------------------ */

#define IS_GPIO_ALL_PERIPH(PERIPH) ((((uint32_t)PERIPH - GPIOA_BASE) % 0x400 == 0) && (((uint32_t)PERIPH - GPIOA_BASE) / 0x400 <= FEATURE_GPIO_GROUP_NUM))

/**
 * @brief  GPIO Configuration Mode enumeration
 */
typedef enum
{
    GPIO_Mode_IN  = 0x00, /*!< GPIO Input Mode */
    GPIO_Mode_OUT = 0x01, /*!< GPIO Output Mode */
    GPIO_Mode_AF  = 0x02, /*!< GPIO Alternate function Mode */
    GPIO_Mode_AN  = 0x03  /*!< GPIO Analog Mode */
} GPIOMode_TypeDef;
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_IN) || ((MODE) == GPIO_Mode_OUT) || ((MODE) == GPIO_Mode_AF) || ((MODE) == GPIO_Mode_AN))

/**
 * @brief  GPIO Output type enumeration
 */
typedef enum
{
    GPIO_OType_PP = 0x00,
    GPIO_OType_OD = 0x01
} GPIOOType_TypeDef;
#define IS_GPIO_OTYPE(OTYPE) (((OTYPE) == GPIO_OType_PP) || ((OTYPE) == GPIO_OType_OD))

/**
 * @brief  GPIO Output Maximum frequency enumeration
 */
typedef enum
{
    GPIO_Low_Speed    = 0x00, /*!< Low speed    */
    GPIO_Medium_Speed = 0x01, /*!< Medium speed */
    GPIO_Fast_Speed   = 0x02, /*!< Fast speed   */
    GPIO_High_Speed   = 0x03  /*!< High speed   */
} GPIOSpeed_TypeDef;

/* Add legacy definition */
#define GPIO_Speed_2MHz   GPIO_Low_Speed
#define GPIO_Speed_25MHz  GPIO_Medium_Speed
#define GPIO_Speed_50MHz  GPIO_Fast_Speed
#define GPIO_Speed_100MHz GPIO_High_Speed

#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Low_Speed) || ((SPEED) == GPIO_Medium_Speed) || ((SPEED) == GPIO_Fast_Speed) || ((SPEED) == GPIO_High_Speed))

/**
 * @brief  GPIO Configuration PullUp PullDown enumeration
 */
typedef enum
{
    GPIO_PuPd_NOPULL = 0x00,
    GPIO_PuPd_UP     = 0x01,
    GPIO_PuPd_DOWN   = 0x02
} GPIOPuPd_TypeDef;
#define IS_GPIO_PUPD(PUPD) (((PUPD) == GPIO_PuPd_NOPULL) || ((PUPD) == GPIO_PuPd_UP) || ((PUPD) == GPIO_PuPd_DOWN))

/**
 * @brief  GPIO Bit SET and Bit RESET enumeration
 */
typedef enum
{
    Bit_RESET = 0,
    Bit_SET
} BitAction;
#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

/**
 * @brief   GPIO Init structure definition
 */
typedef struct
{
    uint32_t GPIO_Pin; /*!< Specifies the GPIO pins to be configured.
                            This parameter can be any value of @ref GPIO_pins_define */

    GPIOMode_TypeDef GPIO_Mode; /*!< Specifies the operating mode for the selected pins.
                                     This parameter can be a value of @ref GPIOMode_TypeDef */

    GPIOSpeed_TypeDef GPIO_Speed; /*!< Specifies the speed for the selected pins.
                                       This parameter can be a value of @ref GPIOSpeed_TypeDef */

    GPIOOType_TypeDef GPIO_OType; /*!< Specifies the operating output type for the selected pins.
                                       This parameter can be a value of @ref GPIOOType_TypeDef */

    GPIOPuPd_TypeDef GPIO_PuPd; /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                     This parameter can be a value of @ref GPIOPuPd_TypeDef */
} GPIO_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants
 * @{
 */

/** @defgroup GPIO_pins_define
 * @{
 */
#define GPIO_Pin_0   ((uint16_t)0x0001) /* Pin 0 selected */
#define GPIO_Pin_1   ((uint16_t)0x0002) /* Pin 1 selected */
#define GPIO_Pin_2   ((uint16_t)0x0004) /* Pin 2 selected */
#define GPIO_Pin_3   ((uint16_t)0x0008) /* Pin 3 selected */
#define GPIO_Pin_4   ((uint16_t)0x0010) /* Pin 4 selected */
#define GPIO_Pin_5   ((uint16_t)0x0020) /* Pin 5 selected */
#define GPIO_Pin_6   ((uint16_t)0x0040) /* Pin 6 selected */
#define GPIO_Pin_7   ((uint16_t)0x0080) /* Pin 7 selected */
#define GPIO_Pin_8   ((uint16_t)0x0100) /* Pin 8 selected */
#define GPIO_Pin_9   ((uint16_t)0x0200) /* Pin 9 selected */
#define GPIO_Pin_10  ((uint16_t)0x0400) /* Pin 10 selected */
#define GPIO_Pin_11  ((uint16_t)0x0800) /* Pin 11 selected */
#define GPIO_Pin_12  ((uint16_t)0x1000) /* Pin 12 selected */
#define GPIO_Pin_13  ((uint16_t)0x2000) /* Pin 13 selected */
#define GPIO_Pin_14  ((uint16_t)0x4000) /* Pin 14 selected */
#define GPIO_Pin_15  ((uint16_t)0x8000) /* Pin 15 selected */
#define GPIO_Pin_All ((uint16_t)0xFFFF) /* All pins selected */

#define GPIO_PIN_MASK    ((uint32_t)0x0000FFFF) /* PIN mask for assert test */
#define IS_GPIO_PIN(PIN) (((PIN) & GPIO_PIN_MASK) != (uint32_t)0x00)
#define IS_GET_GPIO_PIN(PIN)                                                                                                                                 \
    (((PIN) == GPIO_Pin_0) || ((PIN) == GPIO_Pin_1) || ((PIN) == GPIO_Pin_2) || ((PIN) == GPIO_Pin_3) || ((PIN) == GPIO_Pin_4) || ((PIN) == GPIO_Pin_5) ||   \
     ((PIN) == GPIO_Pin_6) || ((PIN) == GPIO_Pin_7) || ((PIN) == GPIO_Pin_8) || ((PIN) == GPIO_Pin_9) || ((PIN) == GPIO_Pin_10) || ((PIN) == GPIO_Pin_11) || \
     ((PIN) == GPIO_Pin_12) || ((PIN) == GPIO_Pin_13) || ((PIN) == GPIO_Pin_14) || ((PIN) == GPIO_Pin_15))
/**
 * @}
 */

/** @defgroup GPIO_Pin_sources
 * @{
 */
#define GPIO_PinSource0  ((uint8_t)0x00)
#define GPIO_PinSource1  ((uint8_t)0x01)
#define GPIO_PinSource2  ((uint8_t)0x02)
#define GPIO_PinSource3  ((uint8_t)0x03)
#define GPIO_PinSource4  ((uint8_t)0x04)
#define GPIO_PinSource5  ((uint8_t)0x05)
#define GPIO_PinSource6  ((uint8_t)0x06)
#define GPIO_PinSource7  ((uint8_t)0x07)
#define GPIO_PinSource8  ((uint8_t)0x08)
#define GPIO_PinSource9  ((uint8_t)0x09)
#define GPIO_PinSource10 ((uint8_t)0x0A)
#define GPIO_PinSource11 ((uint8_t)0x0B)
#define GPIO_PinSource12 ((uint8_t)0x0C)
#define GPIO_PinSource13 ((uint8_t)0x0D)
#define GPIO_PinSource14 ((uint8_t)0x0E)
#define GPIO_PinSource15 ((uint8_t)0x0F)

#define IS_GPIO_PIN_SOURCE(PINSOURCE)                                                                                                                  \
    (((PINSOURCE) == GPIO_PinSource0) || ((PINSOURCE) == GPIO_PinSource1) || ((PINSOURCE) == GPIO_PinSource2) || ((PINSOURCE) == GPIO_PinSource3) ||   \
     ((PINSOURCE) == GPIO_PinSource4) || ((PINSOURCE) == GPIO_PinSource5) || ((PINSOURCE) == GPIO_PinSource6) || ((PINSOURCE) == GPIO_PinSource7) ||   \
     ((PINSOURCE) == GPIO_PinSource8) || ((PINSOURCE) == GPIO_PinSource9) || ((PINSOURCE) == GPIO_PinSource10) || ((PINSOURCE) == GPIO_PinSource11) || \
     ((PINSOURCE) == GPIO_PinSource12) || ((PINSOURCE) == GPIO_PinSource13) || ((PINSOURCE) == GPIO_PinSource14) || ((PINSOURCE) == GPIO_PinSource15))
/**
 * @}
 */

#define IS_GPIO_AF(AF) ((AF) <= 0xFU)

/* GPIO_Legacy */
#define GPIO_Mode_AIN GPIO_Mode_AN

typedef enum
{
    IOModeInput       = 0x0,       // GPIO Input Mode
    IOModeOutput      = 0x1,       // GPIO Output Mode
    IOModeAlternate   = 0x2,       // GPIO Alternate function Mode
    IOModeAnalog      = 0x3,       // GPIO Analog Mode
    IOModeOutputOD    = 0x1 | 0x8, // GPIO Output open-drain Mode
    IOModeAlternateOD = 0x2 | 0x8, // GPIO Alternate function open-drain Mode
} IOModeEnum;

typedef enum
{
    IOSpeedLow      = 0x0, // Low speed - default
    IOSpeedMedium   = 0x1, // Medium speed
    IOSpeedHigh     = 0x2, // High speed
    IOSpeedVeryHigh = 0x3  // Very High speed
} IOSpeedEnum;

typedef enum
{
    IODriveLow      = 0x0, // 0.8mA
    IODriveMedium   = 0x1, // 12mA
    IODriveHigh     = 0x2, // 16mA - default
    IODriveVeryHigh = 0x3, // 20mA
} IODriveEnum;

typedef enum
{
    IOPullNone = 0x0,
    IOPullUp   = 0x1,
    IOPullDown = 0x2
} IOPullEnum;

typedef struct
{
    IOModeEnum  Mode : 4;
    uint8_t     Alternate : 4;
    IOPullEnum  Pull : 2;
    IOSpeedEnum Speed : 2;
    IODriveEnum Drive : 2;
} IOConfigStruct;

/* --------  Public Macro Function  ----------------------------------------- */

#define IOPeripheral(io) ((PeripheralEnum)(PeripheralGPIOStart + (io >> 4)))
#define IOPin(io)        BIT(io & 0x0F)
#define IOPort(io)       ((GPIO_TypeDef*)(GPIOA_BASE + 0x400 * (io >> 4)))
#define IOSource(io)     (io & 0x0F)

#define IOInit(io, ioConfig)                  \
    PeripheralEnable(IOPeripheral(io), true); \
    (ioConfig)->GPIO_Pin = IOPin(io);         \
    GPIO_Init(IOPort(io), ioConfig);

#define IOH(io)        (IOPort(io)->BSRR = IOPin(io))
#define IOL(io)        (IOPort(io)->BSRR = IOPin(io) << 16)
#define IOSet(io, isH) (IOPort(io)->BSRR = IOPin(io) << (isH ? 0 : 16))

#define MakeIOConfig(mode, alternate, pull, speed, drive) \
    ((IOConfigStruct) {                                   \
        .Mode      = mode,                                \
        .Alternate = alternate,                           \
        .Pull      = pull,                                \
        .Speed     = speed,                               \
        .Drive     = drive,                               \
    })

#ifndef IO_DEFAULT_OUTPUT_CONFIG
#define IO_DEFAULT_OUTPUT_CONFIG MakeIOConfig(IOModeOutput, 0, IOPullNone, IOSpeedMedium, IODriveMedium)
#endif

#ifndef IO_DEFAULT_INPUT_CONFIG
#define IO_DEFAULT_INPUT_CONFIG MakeIOConfig(IOModeInput, 0, IOPullNone, IOSpeedLow, IODriveLow)
#endif

/* --------  Public Function  ----------------------------------------------- */

/*  Function used to set the GPIO configuration to the default reset state ****/
void GPIO_DeInit(GPIO_TypeDef* GPIOx);

/* Initialization and Configuration functions *********************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* GPIO Read and Write functions **********************************************/
uint8_t  GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t  GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* GPIO Alternate functions configuration function ****************************/
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF);

#define GPIO_DriveStrength_Low      (0x00)
#define GPIO_DriveStrength_Medium   (0x01)
#define GPIO_DriveStrength_High     (0x02)
#define GPIO_DriveStrength_VeryHigh (0x03)

#define IS_GPIO_DriveStrength(DS) ((DS) < 3)

void GPIO_DriveStrengthConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t GPIO_DriveStrength);


/* --------  Public Advance Function  --------------------------------------- */
extern void IOSetup(IOEnum io, IOConfigStruct config);

#ifdef __cplusplus
}
#endif

#endif
