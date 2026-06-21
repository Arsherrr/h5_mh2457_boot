// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_RTC_H__
#define __PERIPH_RTC_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */
typedef struct
{
    __IO uint32_t RTC_CS;
    __IO uint32_t RTC_REF;
    __IO uint32_t RTC_ARM;
    __I uint32_t  RTC_TIM;
    __O uint32_t  RTC_INTCLR;
    __IO uint32_t OSC32K_CR;
    __IO uint32_t RTC_ATTA_TIM;
} RTC_TypeDef;

FlagStatus RTC_IsReady(void);

void     RTC_ResetCounter(void);
uint32_t RTC_GetCounter(void);

void     RTC_SetRefRegister(uint32_t RefValue);
uint32_t RTC_GetRefRegister(void);

void RTC_SetAlarm(uint32_t AlarmValue);

uint32_t RTC_GetAttrackTime(void);

void     RTC_ITConfig(FunctionalState NewState);
void     RTC_ClearITPendingBit(void);
ITStatus RTC_GetITStatus(void);

	
#ifdef __cplusplus
}
#endif

#endif

/**************************      (C) COPYRIGHT Megahunt    *****END OF FILE****/
