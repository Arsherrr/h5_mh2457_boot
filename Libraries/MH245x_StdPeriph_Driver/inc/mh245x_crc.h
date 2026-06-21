// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_CRC_H__
#define __PERIPH_CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mh2457.h"

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t DR;        /*!< CRC Data register,             Address offset: 0x00 */
    __IO uint8_t  IDR;       /*!< CRC Independent data register, Address offset: 0x04 */
    uint8_t       RESERVED0; /*!< Reserved, 0x05                                      */
    uint16_t      RESERVED1; /*!< Reserved, 0x06                                      */
    __IO uint32_t CR;        /*!< CRC Control register,          Address offset: 0x08 */
    __IO uint32_t CSR;       /*!< CRC Control exten register,    Address offset: 0x0C */
    __IO uint32_t INI;       /*!< CRC Init value register,       Address offset: 0x10 */
    __IO uint32_t XOR;       /*!< CRC Result XOR register,       Address offset: 0x14 */
} CRC_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/*******************  Bit definition for CRC_DR register  *********************/
#define CRC_DR_DR ((uint32_t)0xFFFFFFFF) /*!< Data register bits */

/*******************  Bit definition for CRC_IDR register  ********************/
#define CRC_IDR_IDR ((uint8_t)0xFF) /*!< General-purpose 8-bit data register bits */

/********************  Bit definition for CRC_CR register  ********************/
#define CRC_CR_RESET ((uint8_t)0x01) /*!< RESET bit */

/* --------  Public Function  ----------------------------------------------- */

extern void     CRC_ResetDR(void);
extern uint32_t CRC_CalcCRC(uint32_t Data);
extern uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);
extern uint32_t CRC_GetCRC(void);
extern void     CRC_SetIDRegister(uint8_t IDValue);
extern uint8_t  CRC_GetIDRegister(void);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_CRC_H__
