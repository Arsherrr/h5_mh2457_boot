// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_EXTI_H__
#define __PERIPH_EXTI_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

/**
 * @brief External Interrupt/Event Controller
 */
typedef struct
{
    __IO uint32_t IMR;   /*!< EXTI Interrupt mask register,            Address offset: 0x00 */
    __IO uint32_t EMR;   /*!< EXTI Event mask register,                Address offset: 0x04 */
    __IO uint32_t RTSR;  /*!< EXTI Rising trigger selection register,  Address offset: 0x08 */
    __IO uint32_t FTSR;  /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
    __IO uint32_t SWIER; /*!< EXTI Software interrupt event register,  Address offset: 0x10 */
    __IO uint32_t PR;    /*!< EXTI Pending register,                   Address offset: 0x14 */
} EXTI_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/*******************  Bit definition for EXTI_IMR register  *******************/
#define EXTI_IMR_MR0  ((uint32_t)0x00000001) /*!< Interrupt Mask on line 0 */
#define EXTI_IMR_MR1  ((uint32_t)0x00000002) /*!< Interrupt Mask on line 1 */
#define EXTI_IMR_MR2  ((uint32_t)0x00000004) /*!< Interrupt Mask on line 2 */
#define EXTI_IMR_MR3  ((uint32_t)0x00000008) /*!< Interrupt Mask on line 3 */
#define EXTI_IMR_MR4  ((uint32_t)0x00000010) /*!< Interrupt Mask on line 4 */
#define EXTI_IMR_MR5  ((uint32_t)0x00000020) /*!< Interrupt Mask on line 5 */
#define EXTI_IMR_MR6  ((uint32_t)0x00000040) /*!< Interrupt Mask on line 6 */
#define EXTI_IMR_MR7  ((uint32_t)0x00000080) /*!< Interrupt Mask on line 7 */
#define EXTI_IMR_MR8  ((uint32_t)0x00000100) /*!< Interrupt Mask on line 8 */
#define EXTI_IMR_MR9  ((uint32_t)0x00000200) /*!< Interrupt Mask on line 9 */
#define EXTI_IMR_MR10 ((uint32_t)0x00000400) /*!< Interrupt Mask on line 10 */
#define EXTI_IMR_MR11 ((uint32_t)0x00000800) /*!< Interrupt Mask on line 11 */
#define EXTI_IMR_MR12 ((uint32_t)0x00001000) /*!< Interrupt Mask on line 12 */
#define EXTI_IMR_MR13 ((uint32_t)0x00002000) /*!< Interrupt Mask on line 13 */
#define EXTI_IMR_MR14 ((uint32_t)0x00004000) /*!< Interrupt Mask on line 14 */
#define EXTI_IMR_MR15 ((uint32_t)0x00008000) /*!< Interrupt Mask on line 15 */
#define EXTI_IMR_MR16 ((uint32_t)0x00010000) /*!< Interrupt Mask on line 16 */
#define EXTI_IMR_MR17 ((uint32_t)0x00020000) /*!< Interrupt Mask on line 17 */
#define EXTI_IMR_MR18 ((uint32_t)0x00040000) /*!< Interrupt Mask on line 18 */
#define EXTI_IMR_MR19 ((uint32_t)0x00080000) /*!< Interrupt Mask on line 19 */
#define EXTI_IMR_MR23 ((uint32_t)0x00800000) /*!< Interrupt Mask on line 23 */

/*******************  Bit definition for EXTI_EMR register  *******************/
#define EXTI_EMR_MR0  ((uint32_t)0x00000001) /*!< Event Mask on line 0 */
#define EXTI_EMR_MR1  ((uint32_t)0x00000002) /*!< Event Mask on line 1 */
#define EXTI_EMR_MR2  ((uint32_t)0x00000004) /*!< Event Mask on line 2 */
#define EXTI_EMR_MR3  ((uint32_t)0x00000008) /*!< Event Mask on line 3 */
#define EXTI_EMR_MR4  ((uint32_t)0x00000010) /*!< Event Mask on line 4 */
#define EXTI_EMR_MR5  ((uint32_t)0x00000020) /*!< Event Mask on line 5 */
#define EXTI_EMR_MR6  ((uint32_t)0x00000040) /*!< Event Mask on line 6 */
#define EXTI_EMR_MR7  ((uint32_t)0x00000080) /*!< Event Mask on line 7 */
#define EXTI_EMR_MR8  ((uint32_t)0x00000100) /*!< Event Mask on line 8 */
#define EXTI_EMR_MR9  ((uint32_t)0x00000200) /*!< Event Mask on line 9 */
#define EXTI_EMR_MR10 ((uint32_t)0x00000400) /*!< Event Mask on line 10 */
#define EXTI_EMR_MR11 ((uint32_t)0x00000800) /*!< Event Mask on line 11 */
#define EXTI_EMR_MR12 ((uint32_t)0x00001000) /*!< Event Mask on line 12 */
#define EXTI_EMR_MR13 ((uint32_t)0x00002000) /*!< Event Mask on line 13 */
#define EXTI_EMR_MR14 ((uint32_t)0x00004000) /*!< Event Mask on line 14 */
#define EXTI_EMR_MR15 ((uint32_t)0x00008000) /*!< Event Mask on line 15 */
#define EXTI_EMR_MR16 ((uint32_t)0x00010000) /*!< Event Mask on line 16 */
#define EXTI_EMR_MR17 ((uint32_t)0x00020000) /*!< Event Mask on line 17 */
#define EXTI_EMR_MR18 ((uint32_t)0x00040000) /*!< Event Mask on line 18 */
#define EXTI_EMR_MR19 ((uint32_t)0x00080000) /*!< Event Mask on line 19 */
#define EXTI_EMR_MR23 ((uint32_t)0x00800000) /*!< Event Mask on line 19 */

/******************  Bit definition for EXTI_RTSR register  *******************/
#define EXTI_RTSR_TR0  ((uint32_t)0x00000001) /*!< Rising trigger event configuration bit of line 0 */
#define EXTI_RTSR_TR1  ((uint32_t)0x00000002) /*!< Rising trigger event configuration bit of line 1 */
#define EXTI_RTSR_TR2  ((uint32_t)0x00000004) /*!< Rising trigger event configuration bit of line 2 */
#define EXTI_RTSR_TR3  ((uint32_t)0x00000008) /*!< Rising trigger event configuration bit of line 3 */
#define EXTI_RTSR_TR4  ((uint32_t)0x00000010) /*!< Rising trigger event configuration bit of line 4 */
#define EXTI_RTSR_TR5  ((uint32_t)0x00000020) /*!< Rising trigger event configuration bit of line 5 */
#define EXTI_RTSR_TR6  ((uint32_t)0x00000040) /*!< Rising trigger event configuration bit of line 6 */
#define EXTI_RTSR_TR7  ((uint32_t)0x00000080) /*!< Rising trigger event configuration bit of line 7 */
#define EXTI_RTSR_TR8  ((uint32_t)0x00000100) /*!< Rising trigger event configuration bit of line 8 */
#define EXTI_RTSR_TR9  ((uint32_t)0x00000200) /*!< Rising trigger event configuration bit of line 9 */
#define EXTI_RTSR_TR10 ((uint32_t)0x00000400) /*!< Rising trigger event configuration bit of line 10 */
#define EXTI_RTSR_TR11 ((uint32_t)0x00000800) /*!< Rising trigger event configuration bit of line 11 */
#define EXTI_RTSR_TR12 ((uint32_t)0x00001000) /*!< Rising trigger event configuration bit of line 12 */
#define EXTI_RTSR_TR13 ((uint32_t)0x00002000) /*!< Rising trigger event configuration bit of line 13 */
#define EXTI_RTSR_TR14 ((uint32_t)0x00004000) /*!< Rising trigger event configuration bit of line 14 */
#define EXTI_RTSR_TR15 ((uint32_t)0x00008000) /*!< Rising trigger event configuration bit of line 15 */
#define EXTI_RTSR_TR16 ((uint32_t)0x00010000) /*!< Rising trigger event configuration bit of line 16 */
#define EXTI_RTSR_TR17 ((uint32_t)0x00020000) /*!< Rising trigger event configuration bit of line 17 */
#define EXTI_RTSR_TR18 ((uint32_t)0x00040000) /*!< Rising trigger event configuration bit of line 18 */
#define EXTI_RTSR_TR19 ((uint32_t)0x00080000) /*!< Rising trigger event configuration bit of line 19 */
#define EXTI_RTSR_TR23 ((uint32_t)0x00800000) /*!< Rising trigger event configuration bit of line 23 */

/******************  Bit definition for EXTI_FTSR register  *******************/
#define EXTI_FTSR_TR0  ((uint32_t)0x00000001) /*!< Falling trigger event configuration bit of line 0 */
#define EXTI_FTSR_TR1  ((uint32_t)0x00000002) /*!< Falling trigger event configuration bit of line 1 */
#define EXTI_FTSR_TR2  ((uint32_t)0x00000004) /*!< Falling trigger event configuration bit of line 2 */
#define EXTI_FTSR_TR3  ((uint32_t)0x00000008) /*!< Falling trigger event configuration bit of line 3 */
#define EXTI_FTSR_TR4  ((uint32_t)0x00000010) /*!< Falling trigger event configuration bit of line 4 */
#define EXTI_FTSR_TR5  ((uint32_t)0x00000020) /*!< Falling trigger event configuration bit of line 5 */
#define EXTI_FTSR_TR6  ((uint32_t)0x00000040) /*!< Falling trigger event configuration bit of line 6 */
#define EXTI_FTSR_TR7  ((uint32_t)0x00000080) /*!< Falling trigger event configuration bit of line 7 */
#define EXTI_FTSR_TR8  ((uint32_t)0x00000100) /*!< Falling trigger event configuration bit of line 8 */
#define EXTI_FTSR_TR9  ((uint32_t)0x00000200) /*!< Falling trigger event configuration bit of line 9 */
#define EXTI_FTSR_TR10 ((uint32_t)0x00000400) /*!< Falling trigger event configuration bit of line 10 */
#define EXTI_FTSR_TR11 ((uint32_t)0x00000800) /*!< Falling trigger event configuration bit of line 11 */
#define EXTI_FTSR_TR12 ((uint32_t)0x00001000) /*!< Falling trigger event configuration bit of line 12 */
#define EXTI_FTSR_TR13 ((uint32_t)0x00002000) /*!< Falling trigger event configuration bit of line 13 */
#define EXTI_FTSR_TR14 ((uint32_t)0x00004000) /*!< Falling trigger event configuration bit of line 14 */
#define EXTI_FTSR_TR15 ((uint32_t)0x00008000) /*!< Falling trigger event configuration bit of line 15 */
#define EXTI_FTSR_TR16 ((uint32_t)0x00010000) /*!< Falling trigger event configuration bit of line 16 */
#define EXTI_FTSR_TR17 ((uint32_t)0x00020000) /*!< Falling trigger event configuration bit of line 17 */
#define EXTI_FTSR_TR18 ((uint32_t)0x00040000) /*!< Falling trigger event configuration bit of line 18 */
#define EXTI_FTSR_TR19 ((uint32_t)0x00080000) /*!< Falling trigger event configuration bit of line 19 */
#define EXTI_FTSR_TR23 ((uint32_t)0x00800000) /*!< Falling trigger event configuration bit of line 23 */

/******************  Bit definition for EXTI_SWIER register  ******************/
#define EXTI_SWIER_SWIER0  ((uint32_t)0x00000001) /*!< Software Interrupt on line 0 */
#define EXTI_SWIER_SWIER1  ((uint32_t)0x00000002) /*!< Software Interrupt on line 1 */
#define EXTI_SWIER_SWIER2  ((uint32_t)0x00000004) /*!< Software Interrupt on line 2 */
#define EXTI_SWIER_SWIER3  ((uint32_t)0x00000008) /*!< Software Interrupt on line 3 */
#define EXTI_SWIER_SWIER4  ((uint32_t)0x00000010) /*!< Software Interrupt on line 4 */
#define EXTI_SWIER_SWIER5  ((uint32_t)0x00000020) /*!< Software Interrupt on line 5 */
#define EXTI_SWIER_SWIER6  ((uint32_t)0x00000040) /*!< Software Interrupt on line 6 */
#define EXTI_SWIER_SWIER7  ((uint32_t)0x00000080) /*!< Software Interrupt on line 7 */
#define EXTI_SWIER_SWIER8  ((uint32_t)0x00000100) /*!< Software Interrupt on line 8 */
#define EXTI_SWIER_SWIER9  ((uint32_t)0x00000200) /*!< Software Interrupt on line 9 */
#define EXTI_SWIER_SWIER10 ((uint32_t)0x00000400) /*!< Software Interrupt on line 10 */
#define EXTI_SWIER_SWIER11 ((uint32_t)0x00000800) /*!< Software Interrupt on line 11 */
#define EXTI_SWIER_SWIER12 ((uint32_t)0x00001000) /*!< Software Interrupt on line 12 */
#define EXTI_SWIER_SWIER13 ((uint32_t)0x00002000) /*!< Software Interrupt on line 13 */
#define EXTI_SWIER_SWIER14 ((uint32_t)0x00004000) /*!< Software Interrupt on line 14 */
#define EXTI_SWIER_SWIER15 ((uint32_t)0x00008000) /*!< Software Interrupt on line 15 */
#define EXTI_SWIER_SWIER16 ((uint32_t)0x00010000) /*!< Software Interrupt on line 16 */
#define EXTI_SWIER_SWIER17 ((uint32_t)0x00020000) /*!< Software Interrupt on line 17 */
#define EXTI_SWIER_SWIER18 ((uint32_t)0x00040000) /*!< Software Interrupt on line 18 */
#define EXTI_SWIER_SWIER19 ((uint32_t)0x00080000) /*!< Software Interrupt on line 19 */
#define EXTI_SWIER_SWIER23 ((uint32_t)0x00800000) /*!< Software Interrupt on line 23 */

/*******************  Bit definition for EXTI_PR register  ********************/
#define EXTI_PR_PR0  ((uint32_t)0x00000001) /*!< Pending bit for line 0 */
#define EXTI_PR_PR1  ((uint32_t)0x00000002) /*!< Pending bit for line 1 */
#define EXTI_PR_PR2  ((uint32_t)0x00000004) /*!< Pending bit for line 2 */
#define EXTI_PR_PR3  ((uint32_t)0x00000008) /*!< Pending bit for line 3 */
#define EXTI_PR_PR4  ((uint32_t)0x00000010) /*!< Pending bit for line 4 */
#define EXTI_PR_PR5  ((uint32_t)0x00000020) /*!< Pending bit for line 5 */
#define EXTI_PR_PR6  ((uint32_t)0x00000040) /*!< Pending bit for line 6 */
#define EXTI_PR_PR7  ((uint32_t)0x00000080) /*!< Pending bit for line 7 */
#define EXTI_PR_PR8  ((uint32_t)0x00000100) /*!< Pending bit for line 8 */
#define EXTI_PR_PR9  ((uint32_t)0x00000200) /*!< Pending bit for line 9 */
#define EXTI_PR_PR10 ((uint32_t)0x00000400) /*!< Pending bit for line 10 */
#define EXTI_PR_PR11 ((uint32_t)0x00000800) /*!< Pending bit for line 11 */
#define EXTI_PR_PR12 ((uint32_t)0x00001000) /*!< Pending bit for line 12 */
#define EXTI_PR_PR13 ((uint32_t)0x00002000) /*!< Pending bit for line 13 */
#define EXTI_PR_PR14 ((uint32_t)0x00004000) /*!< Pending bit for line 14 */
#define EXTI_PR_PR15 ((uint32_t)0x00008000) /*!< Pending bit for line 15 */
#define EXTI_PR_PR16 ((uint32_t)0x00010000) /*!< Pending bit for line 16 */
#define EXTI_PR_PR17 ((uint32_t)0x00020000) /*!< Pending bit for line 17 */
#define EXTI_PR_PR18 ((uint32_t)0x00040000) /*!< Pending bit for line 18 */
#define EXTI_PR_PR19 ((uint32_t)0x00080000) /*!< Pending bit for line 19 */
#define EXTI_PR_PR23 ((uint32_t)0x00800000) /*!< Pending bit for line 23 */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  EXTI mode enumeration
 */

typedef enum
{
    EXTI_Mode_Interrupt = 0x00,
    EXTI_Mode_Event     = 0x04
} EXTIMode_TypeDef;

#define IS_EXTI_MODE(MODE) (((MODE) == EXTI_Mode_Interrupt) || ((MODE) == EXTI_Mode_Event))

/**
 * @brief  EXTI Trigger enumeration
 */

typedef enum
{
    EXTI_Trigger_Rising         = 0x08,
    EXTI_Trigger_Falling        = 0x0C,
    EXTI_Trigger_Rising_Falling = 0x10
} EXTITrigger_TypeDef;

#define IS_EXTI_TRIGGER(TRIGGER) (((TRIGGER) == EXTI_Trigger_Rising) || ((TRIGGER) == EXTI_Trigger_Falling) || ((TRIGGER) == EXTI_Trigger_Rising_Falling))
/**
 * @brief  EXTI Init Structure definition
 */

typedef struct
{
    uint32_t EXTI_Line; /*!< Specifies the EXTI lines to be enabled or disabled.
                             This parameter can be any combination value of @ref EXTI_Lines */

    EXTIMode_TypeDef EXTI_Mode; /*!< Specifies the mode for the EXTI lines.
                                     This parameter can be a value of @ref EXTIMode_TypeDef */

    EXTITrigger_TypeDef EXTI_Trigger; /*!< Specifies the trigger signal active edge for the EXTI lines.
                                           This parameter can be a value of @ref EXTITrigger_TypeDef */

    FunctionalState EXTI_LineCmd; /*!< Specifies the new state of the selected EXTI lines.
                                       This parameter can be set either to ENABLE or DISABLE */
} EXTI_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup EXTI_Exported_Constants
 * @{
 */

/** @defgroup EXTI_Lines
 * @{
 */

#define EXTI_Line0  ((uint32_t)0x00001)    /*!< External interrupt line 0 */
#define EXTI_Line1  ((uint32_t)0x00002)    /*!< External interrupt line 1 */
#define EXTI_Line2  ((uint32_t)0x00004)    /*!< External interrupt line 2 */
#define EXTI_Line3  ((uint32_t)0x00008)    /*!< External interrupt line 3 */
#define EXTI_Line4  ((uint32_t)0x00010)    /*!< External interrupt line 4 */
#define EXTI_Line5  ((uint32_t)0x00020)    /*!< External interrupt line 5 */
#define EXTI_Line6  ((uint32_t)0x00040)    /*!< External interrupt line 6 */
#define EXTI_Line7  ((uint32_t)0x00080)    /*!< External interrupt line 7 */
#define EXTI_Line8  ((uint32_t)0x00100)    /*!< External interrupt line 8 */
#define EXTI_Line9  ((uint32_t)0x00200)    /*!< External interrupt line 9 */
#define EXTI_Line10 ((uint32_t)0x00400)    /*!< External interrupt line 10 */
#define EXTI_Line11 ((uint32_t)0x00800)    /*!< External interrupt line 11 */
#define EXTI_Line12 ((uint32_t)0x01000)    /*!< External interrupt line 12 */
#define EXTI_Line13 ((uint32_t)0x02000)    /*!< External interrupt line 13 */
#define EXTI_Line14 ((uint32_t)0x04000)    /*!< External interrupt line 14 */
#define EXTI_Line15 ((uint32_t)0x08000)    /*!< External interrupt line 15 */
#define EXTI_Line16 ((uint32_t)0x10000)    /*!< External interrupt line 16 Connected to the PVD Output */
#define EXTI_Line17 ((uint32_t)0x20000)    /*!< External interrupt line 17 Connected to the RTC Alarm event */
#define EXTI_Line18 ((uint32_t)0x40000)    /*!< External interrupt line 18 Connected to the USB OTG FS Wakeup from suspend event */
#define EXTI_Line19 ((uint32_t)0x80000)    /*!< External interrupt line 19 Connected to the Ethernet Wakeup event */
#define EXTI_Line20 ((uint32_t)0x00100000) /*!< External interrupt line 20 Connected to the USB OTG HS (configured in FS) Wakeup event  */
#define EXTI_Line21 ((uint32_t)0x00200000) /*!< External interrupt line 21 Connected to the RTC Tamper and Time Stamp events */
#define EXTI_Line22 ((uint32_t)0x00400000) /*!< External interrupt line 22 Connected to the RTC Wakeup event */
#define EXTI_Line23 ((uint32_t)0x00800000) /*!< External interrupt line 23 Connected to the LPTIM Wakeup event */

#define IS_EXTI_LINE(LINE) ((((LINE) & (uint32_t)0xFF800000) == 0x00) && ((LINE) != (uint16_t)0x00))

#define IS_GET_EXTI_LINE(LINE)                                                                                                              \
    (((LINE) == EXTI_Line0) || ((LINE) == EXTI_Line1) || ((LINE) == EXTI_Line2) || ((LINE) == EXTI_Line3) || ((LINE) == EXTI_Line4) ||      \
     ((LINE) == EXTI_Line5) || ((LINE) == EXTI_Line6) || ((LINE) == EXTI_Line7) || ((LINE) == EXTI_Line8) || ((LINE) == EXTI_Line9) ||      \
     ((LINE) == EXTI_Line10) || ((LINE) == EXTI_Line11) || ((LINE) == EXTI_Line12) || ((LINE) == EXTI_Line13) || ((LINE) == EXTI_Line14) || \
     ((LINE) == EXTI_Line15) || ((LINE) == EXTI_Line16) || ((LINE) == EXTI_Line17) || ((LINE) == EXTI_Line18) || ((LINE) == EXTI_Line19) || \
     ((LINE) == EXTI_Line20) || ((LINE) == EXTI_Line21) || ((LINE) == EXTI_Line22) || ((LINE) == EXTI_Line23))

/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the EXTI configuration to the default reset state *****/
void EXTI_DeInit(void);

/* Initialization and Configuration functions *********************************/
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);

/* Interrupts and flags management functions **********************************/
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);
void       EXTI_ClearFlag(uint32_t EXTI_Line);
ITStatus   EXTI_GetITStatus(uint32_t EXTI_Line);
void       EXTI_ClearITPendingBit(uint32_t EXTI_Line);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_EXTI_H__
