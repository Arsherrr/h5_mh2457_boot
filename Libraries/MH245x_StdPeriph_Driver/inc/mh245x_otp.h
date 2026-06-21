// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_OTP_H__
#define __PERIPH_OTP_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __O uint32_t  DATA[FEATURE_OTP_SIZE >> 2];
    __I uint32_t  RSVD0400;
    __IO uint32_t CS;
    __IO uint32_t PROT;
    __IO uint32_t ADDR;
    __IO uint32_t PDATA;
    __IO uint32_t RO;
    __IO uint32_t ROL;
    __IO uint32_t HIDE;
    __IO uint32_t TIM;
    __I uint32_t  RSVD0424;
    __IO uint32_t RO1;
    __IO uint32_t ROL1;
    __IO uint32_t RO2;
    __IO uint32_t ROL2;
    __IO uint32_t RO3;
    __IO uint32_t ROL3;
} OTP_TypeDef;

/* --------  Exported Type  ------------------------------------------------- */

typedef enum
{
    OTP_Power_Down = 0,
    OTP_Power_On   = 1,
} OTP_PowerTypeDef;

typedef enum
{
    OTP_WriteNoDone = 0,
    OTP_WriteDone   = 1,
} OTP_WrieDoneTypeDef;

typedef enum
{
    OTP_ReadNotReady = 0,
    OTP_ReadReady    = 1,
} OTP_ReadReadyTypeDef;

typedef enum
{
    OTP_UnLocked = 0,
    OTP_Locked   = 1,
} OTP_LockTypeDef;

typedef enum
{
    OTP_UnHided = 0,
    OTP_Hided   = 1,
} OTP_HideTypeDef;

typedef enum
{
    OTP_Complete         = 0,
    OTP_ReadOnProgram    = 1, //在编程状态下对OTP进行读操作
    OTP_ProgramInROBlock = 2, //对只读取进行编程
    OTP_ProgramOutOfAddr = 3, //编程范围超过OTP范围
    OTP_TimeOut          = 6, //OTP完成标志位超时
    OTP_DataWrong        = 7,
} OTP_StatusTypeDef;

#define IS_OTP_ADDRESS(ADDRESS) (((ADDRESS) > OTP_BASE - 1) && ((ADDRESS) < OTP_BASE + FEATURE_OTP_SIZE))

/* --------  Public Function  ----------------------------------------------- */

void OTP_Unlock(void);
void OTP_Lock(void);
void OTP_ClearStatus(void);

void OTP_Init(void);

void              OTP_SetProtect(uint32_t u32Addr);
OTP_StatusTypeDef OTP_UnProtect(uint32_t u32Addr);

OTP_LockTypeDef   OTP_IsProtect(uint32_t u32Addr);
OTP_LockTypeDef   OTP_IsProtectLock(uint32_t u32Addr);
OTP_StatusTypeDef OTP_WriteByte(uint32_t addr, uint8_t w);

void            OTP_AllHide(void);
OTP_HideTypeDef OTP_AreaHide(uint32_t u32Addr);
OTP_HideTypeDef OTP_IsHide(uint32_t u32Addr);
void            OTP_PowerCmd(FunctionalState State);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_OTP_H__
