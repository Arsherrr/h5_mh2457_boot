// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_IWDG_H__
#define __PERIPH_IWDG_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t KR;  /*!< IWDG Key register,       Address offset: 0x00 */
    __IO uint32_t PR;  /*!< IWDG Prescaler register, Address offset: 0x04 */
    __IO uint32_t RLR; /*!< IWDG Reload register,    Address offset: 0x08 */
    __IO uint32_t SR;  /*!< IWDG Status register,    Address offset: 0x0C */
} IWDG_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/*******************  Bit definition for IWDG_KR register  ********************/
#define IWDG_KR_KEY ((uint16_t)0xFFFF) /*!<Key value (write only, read 0000h)  */

/*******************  Bit definition for IWDG_PR register  ********************/
#define IWDG_PR_PR   ((uint8_t)0x07) /*!<PR[2:0] (Prescaler divider)         */
#define IWDG_PR_PR_0 ((uint8_t)0x01) /*!<Bit 0 */
#define IWDG_PR_PR_1 ((uint8_t)0x02) /*!<Bit 1 */
#define IWDG_PR_PR_2 ((uint8_t)0x04) /*!<Bit 2 */

/*******************  Bit definition for IWDG_RLR register  *******************/
#define IWDG_RLR_RL ((uint16_t)0x0FFF) /*!<Watchdog counter reload value        */

/*******************  Bit definition for IWDG_SR register  ********************/
#define IWDG_SR_PVU ((uint8_t)0x01) /*!<Watchdog prescaler value update      */
#define IWDG_SR_RVU ((uint8_t)0x02) /*!<Watchdog counter reload value update */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup IWDG_Exported_Constants
 * @{
 */

/** @defgroup IWDG_WriteAccess
 * @{
 */
#define IWDG_WriteAccess_Enable      ((uint16_t)0x5555)
#define IWDG_WriteAccess_Disable     ((uint16_t)0x0000)
#define IS_IWDG_WRITE_ACCESS(ACCESS) (((ACCESS) == IWDG_WriteAccess_Enable) || ((ACCESS) == IWDG_WriteAccess_Disable))
/**
 * @}
 */

/** @defgroup IWDG_prescaler
 * @{
 */
#define IWDG_Prescaler_4   ((uint8_t)0x00)
#define IWDG_Prescaler_8   ((uint8_t)0x01)
#define IWDG_Prescaler_16  ((uint8_t)0x02)
#define IWDG_Prescaler_32  ((uint8_t)0x03)
#define IWDG_Prescaler_64  ((uint8_t)0x04)
#define IWDG_Prescaler_128 ((uint8_t)0x05)
#define IWDG_Prescaler_256 ((uint8_t)0x06)
#define IS_IWDG_PRESCALER(PRESCALER)                                                                                                                       \
    (((PRESCALER) == IWDG_Prescaler_4) || ((PRESCALER) == IWDG_Prescaler_8) || ((PRESCALER) == IWDG_Prescaler_16) || ((PRESCALER) == IWDG_Prescaler_32) || \
     ((PRESCALER) == IWDG_Prescaler_64) || ((PRESCALER) == IWDG_Prescaler_128) || ((PRESCALER) == IWDG_Prescaler_256))
/**
 * @}
 */

/** @defgroup IWDG_Flag
 * @{
 */
#define IWDG_FLAG_PVU          ((uint16_t)0x0001)
#define IWDG_FLAG_RVU          ((uint16_t)0x0002)
#define IS_IWDG_FLAG(FLAG)     (((FLAG) == IWDG_FLAG_PVU) || ((FLAG) == IWDG_FLAG_RVU))
#define IS_IWDG_RELOAD(RELOAD) ((RELOAD) <= 0xFFF)
/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Prescaler and Counter configuration functions ******************************/
void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess);
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler);
void IWDG_SetReload(uint16_t Reload);
void IWDG_ReloadCounter(void);

/* IWDG activation function ***************************************************/
void IWDG_Enable(void);

/* Flag management function ***************************************************/
FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_IWDG_H__
