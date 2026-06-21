// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_CACHE_H__
#define __PERIPH_CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mh2457.h"

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t CACHE_I0;
    __IO uint32_t CACHE_I1;
    __IO uint32_t CACHE_I2;
    __IO uint32_t CACHE_I3;
    __IO uint32_t CACHE_K0;
    __IO uint32_t CACHE_K1;
    __IO uint32_t CACHE_K2;
    __IO uint32_t CACHE_K3;
    __IO uint32_t CACHE_CS;
    __IO uint32_t CACHE_REF;
    __IO uint32_t CACHE_REFIDX;
    __I uint32_t  RSVD002C[5];
    __IO uint32_t CACHE_CONFIG;
    __I uint32_t  RSVD0044[8];
    __IO uint32_t RCNT[2];
    __IO uint32_t MCNT[2];
    __IO uint32_t CACHE_SADDR;
    __IO uint32_t CACHE_EADDR;
} CACHE_TypeDef;


/* --------  Register Bit Definition  --------------------------------------- */

#define CACHE_REFRESH        ((uint32_t)0x80000000)
#define CACHE_REFRESH_ALLTAG ((uint32_t)0x40000000)

#define CACHE_KEY_GEN_START ((uint32_t)0x80000000)

#define CACHE_IS_BUSY ((uint32_t)0x20000000)

#define CACHE_SM4_SEL ((uint32_t)0x10000000)

#define ICACHE_SIZE               ((uint32_t)0x8000)
#define DCACHE_SIZE               ((uint32_t)0x4000)
#define CACHE_PARTICLE_SIZE       (0x20)
#define CACHE_ADDRESS_START       ((uint32_t)0x08000000)
#define CACHE_ADDRESS_MAX         ((uint32_t)0x01FFFFFF)
#define IS_CACHE_ADDR_VALID(addr) (((addr)&CACHE_ADDRESS_START) == CACHE_ADDRESS_START)

#define CACHE_AES_BYPASS   (0xA5)
#define CACHE_KEY_GEN      (0xA5)
#define CACHE_WRAP_ENABLE  (0xA5)
#define CACHE_WRAP_DISABLE (0x5A)

#define CACHE_ZONE_ENCRYPT ((uint32_t)0xA5000000)

// ICACHE 32KB
#define CACHE_CODE_BUS_OFFSET_POS   (0)
#define CACHE_CODE_BUS_OFFSET_WIDTH (5)
#define CACHE_CODE_BUS_OFFSET_MASK  ((uint32_t)0x001F)

#define CACHE_CODE_BUS_SET_POS   (5)
#define CACHE_CODE_BUS_SET_WIDTH (8)
#define CACHE_CODE_BUS_SET_MASK  ((uint32_t)0x00FF)

#define CACHE_CODE_BUS_TAG_POS   (13)
#define CACHE_CODE_BUS_TAG_WIDTH (19)
#define CACHE_CODE_BUS_TAG_MASK  ((uint32_t)0x7FFFF)

// DCACHE 16KB
#define CACHE_DATA_BUS_OFFSET_POS   (0)
#define CACHE_DATA_BUS_OFFSET_WIDTH (5)
#define CACHE_DATA_BUS_OFFSET_MASK  ((uint32_t)0x001F)

#define CACHE_DATA_BUS_SET_POS   (5)
#define CACHE_DATA_BUS_SET_WIDTH (7)
#define CACHE_DATA_BUS_SET_MASK  ((uint32_t)0x007F)

#define CACHE_DATA_BUS_TAG_POS   (12)
#define CACHE_DATA_BUS_TAG_WIDTH (20)
#define CACHE_DATA_BUS_TAG_MASK  ((uint32_t)0xFFFFF)

#define ICACHE_BUILD_INDEX_OFFSET(x) (((x)&CACHE_CODE_BUS_OFFSET_MASK) << CACHE_CODE_BUS_OFFSET_POS)
#define ICACHE_BUILD_INDEX_SET(x)    (((x)&CACHE_CODE_BUS_SET_MASK) << CACHE_CODE_BUS_SET_POS)
#define ICACHE_BUILD_INDEX_TAG(x)    (((x)&CACHE_CODE_BUS_TAG_MASK) << CACHE_CODE_BUS_TAG_POS)
#define ICACHE_ADDRESS_BUILD(base, index_set, tag_way, offset) \
    ((base) | CACHE_BUILD_INDEX_SET(index_set) | CACHE_BUILD_INDEX_TAG(tag_way) | CACHE_BUILD_INDEX_OFFSET(offset))

#define DCACHE_BUILD_INDEX_OFFSET(x) (((x)&CACHE_DATA_BUS_OFFSET_MASK) << CACHE_DATA_BUS_OFFSET_POS)
#define DCACHE_BUILD_INDEX_SET(x)    (((x)&CACHE_DATA_BUS_SET_MASK) << CACHE_DATA_BUS_SET_POS)
#define DCACHE_BUILD_INDEX_TAG(x)    (((x)&CACHE_DATA_BUS_TAG_MASK) << CACHE_DATA_BUS_TAG_POS)
#define DCACHE_ADDRESS_BUILD(base, index_set, tag_way, offset) \
    ((base) | DCACHE_BUILD_INDEX_SET(index_set) | DCACHE_BUILD_INDEX_TAG(tag_way) | DCACHE_BUILD_INDEX_OFFSET(offset))

#define ICACHE_TAG_NUM(x) ((x >> CACHE_CODE_BUS_TAG_POS) & CACHE_CODE_BUS_TAG_MASK)
#define ICACHE_SET_NUM(x) ((x >> CACHE_CODE_BUS_SET_POS) & CACHE_CODE_BUS_SET_MASK)
#define ICACHE_OFF_NUM(x) ((x >> CACHE_CODE_BUS_OFFSET_POS) & CACHE_CODE_BUS_OFFSET_MASK)

#define DCACHE_TAG_NUM(x) ((x >> CACHE_DATA_BUS_TAG_POS) & CACHE_DATA_BUS_TAG_MASK)
#define DCACHE_SET_NUM(x) ((x >> CACHE_DATA_BUS_SET_POS) & CACHE_DATA_BUS_SET_MASK)
#define DCACHE_OFF_NUM(x) ((x >> CACHE_DATA_BUS_OFFSET_POS) & CACHE_DATA_BUS_OFFSET_MASK)

/* --------  Export Type ---------------------------------------------------- */

typedef enum
{
    CACHE_Encrypt_Mode_All = 0x0,
    CACHE_Encrypt_Mode_Zone,
} CACHE_EncryptModeTypeDef;
#define IS_CACHE_ENCRYPT_MODE(MODE) (((MODE) == CACHE_Encrypt_Mode_All) || ((MODE) == CACHE_Encrypt_Mode_Zone))

typedef struct
{
    uint32_t I[4];
    uint32_t K[4];

    uint32_t aes_enable;
    uint32_t Address;
    uint32_t size;
    uint32_t algorithm;

    uint32_t encrypt_mode;
    uint32_t encrypt_saddr;
    uint32_t encrypt_eaddr;
} CACHE_InitTypeDef;

/* --------  Function ------------------------------------------------------- */

extern void CACHE_Init(CACHE_TypeDef* Cache, CACHE_InitTypeDef* CACHE_InitStruct);
extern void CACHE_Clean(CACHE_TypeDef* Cache, CACHE_InitTypeDef* CACHE_InitStruct);
extern void CACHE_CleanAll(CACHE_TypeDef* Cache);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_CACHE_H__
