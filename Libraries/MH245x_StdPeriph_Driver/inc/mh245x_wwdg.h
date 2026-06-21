// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_WWDG_H__
#define __PERIPH_WWDG_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t CR;  /*!< WWDG Control register,       Address offset: 0x00 */
    __IO uint32_t CFR; /*!< WWDG Configuration register, Address offset: 0x04 */
    __IO uint32_t SR;  /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/*******************  Bit definition for WWDG_CR register  ********************/
#define WWDG_CR_T   ((uint8_t)0x7F) /*!<T[6:0] bits (7-Bit counter (MSB to LSB)) */
#define WWDG_CR_T_0 ((uint8_t)0x01) /*!<Bit 0 */
#define WWDG_CR_T_1 ((uint8_t)0x02) /*!<Bit 1 */
#define WWDG_CR_T_2 ((uint8_t)0x04) /*!<Bit 2 */
#define WWDG_CR_T_3 ((uint8_t)0x08) /*!<Bit 3 */
#define WWDG_CR_T_4 ((uint8_t)0x10) /*!<Bit 4 */
#define WWDG_CR_T_5 ((uint8_t)0x20) /*!<Bit 5 */
#define WWDG_CR_T_6 ((uint8_t)0x40) /*!<Bit 6 */
/* Legacy defines */
#define WWDG_CR_T0 WWDG_CR_T_0
#define WWDG_CR_T1 WWDG_CR_T_1
#define WWDG_CR_T2 WWDG_CR_T_2
#define WWDG_CR_T3 WWDG_CR_T_3
#define WWDG_CR_T4 WWDG_CR_T_4
#define WWDG_CR_T5 WWDG_CR_T_5
#define WWDG_CR_T6 WWDG_CR_T_6

#define WWDG_CR_WDGA ((uint8_t)0x80) /*!<Activation bit */

/*******************  Bit definition for WWDG_CFR register  *******************/
#define WWDG_CFR_W   ((uint16_t)0x007F) /*!<W[6:0] bits (7-bit window value) */
#define WWDG_CFR_W_0 ((uint16_t)0x0001) /*!<Bit 0 */
#define WWDG_CFR_W_1 ((uint16_t)0x0002) /*!<Bit 1 */
#define WWDG_CFR_W_2 ((uint16_t)0x0004) /*!<Bit 2 */
#define WWDG_CFR_W_3 ((uint16_t)0x0008) /*!<Bit 3 */
#define WWDG_CFR_W_4 ((uint16_t)0x0010) /*!<Bit 4 */
#define WWDG_CFR_W_5 ((uint16_t)0x0020) /*!<Bit 5 */
#define WWDG_CFR_W_6 ((uint16_t)0x0040) /*!<Bit 6 */
/* Legacy defines */
#define WWDG_CFR_W0 WWDG_CFR_W_0
#define WWDG_CFR_W1 WWDG_CFR_W_1
#define WWDG_CFR_W2 WWDG_CFR_W_2
#define WWDG_CFR_W3 WWDG_CFR_W_3
#define WWDG_CFR_W4 WWDG_CFR_W_4
#define WWDG_CFR_W5 WWDG_CFR_W_5
#define WWDG_CFR_W6 WWDG_CFR_W_6

#define WWDG_CFR_WDGTB   ((uint16_t)0x0180) /*!<WDGTB[1:0] bits (Timer Base) */
#define WWDG_CFR_WDGTB_0 ((uint16_t)0x0080) /*!<Bit 0 */
#define WWDG_CFR_WDGTB_1 ((uint16_t)0x0100) /*!<Bit 1 */
/* Legacy defines */
#define WWDG_CFR_WDGTB0 WWDG_CFR_WDGTB_0
#define WWDG_CFR_WDGTB1 WWDG_CFR_WDGTB_1

#define WWDG_CFR_EWI ((uint16_t)0x0200) /*!<Early Wakeup Interrupt */

/*******************  Bit definition for WWDG_SR register  ********************/
#define WWDG_SR_EWIF ((uint8_t)0x01) /*!<Early Wakeup Interrupt Flag */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define WWDG_Prescaler_1 ((uint32_t)0x00000000)
#define WWDG_Prescaler_2 ((uint32_t)0x00000080)
#define WWDG_Prescaler_4 ((uint32_t)0x00000100)
#define WWDG_Prescaler_8 ((uint32_t)0x00000180)
#define IS_WWDG_PRESCALER(PRESCALER) \
    (((PRESCALER) == WWDG_Prescaler_1) || ((PRESCALER) == WWDG_Prescaler_2) || ((PRESCALER) == WWDG_Prescaler_4) || ((PRESCALER) == WWDG_Prescaler_8))
#define IS_WWDG_WINDOW_VALUE(VALUE) ((VALUE) <= 0x7F)
#define IS_WWDG_COUNTER(COUNTER)    (((COUNTER) >= 0x40) && ((COUNTER) <= 0x7F))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the WWDG configuration to the default reset state ****/
void WWDG_DeInit(void);

/* Prescaler, Refresh window and Counter configuration functions **************/
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler);
void WWDG_SetWindowValue(uint8_t WindowValue);
void WWDG_EnableIT(void);
void WWDG_SetCounter(uint8_t Counter);

/* WWDG activation function ***************************************************/
void WWDG_Enable(uint8_t Counter);

/* Interrupts and flags management functions **********************************/
FlagStatus WWDG_GetFlagStatus(void);
void       WWDG_ClearFlag(void);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_WWDG_H__
