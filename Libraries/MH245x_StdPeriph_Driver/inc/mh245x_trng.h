// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_TRNG_H__
#define __PERIPH_TRNG_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    __IO uint32_t RNG_CSR;
    __IO uint32_t RNG_DATA[1];
    __I uint32_t  RES;
    __IO uint32_t RNG_ANA;
    __IO uint32_t RNG_INDEX;
    __IO uint32_t RNG_PN;
    __IO uint32_t RNG_PN1;
    __IO uint32_t RNG_PN2;
    __IO uint32_t RNG_PN3;
    __I uint32_t RNG_PN_BU;
    __IO uint32_t RNG_BD;
	
} TRNG_TypeDef;

/******************************************************************************/
/*                                                                            */
/*                          RNG Control Unit Block                            */
/*                                                                            */
/******************************************************************************/
/************ bit definition for TRNG RNG_CSR REGISTER ************/
#define TRNG_RNG_CSR_INTP_EN_Mask      	((uint32_t)0x0040)
#define TRNG_RNG_CSR_EDGE_SEL_Mask 		((uint32_t)0x0020)
#define TRNG_RNG_CSR_S128_TRNG0_Mask   	((uint32_t)0x0001)

/************ bit definition for TRNG RNG_ANA REGISTER ************/
#define TRNG_RNG_ANA_PD_TRNG0_Mask      ((uint32_t)0x00001000)
#define TRNG_RNG_ANA_PD_TRNG1_Mask      ((uint32_t)0x00002000)
#define TRNG_RNG_ANA_PD_TRNG2_Mask      ((uint32_t)0x00004000)
#define TRNG_RNG_ANA_PD_TRNG3_Mask      ((uint32_t)0x00008000)
#define TRNG_RNG_ANA_PD_ALL_Mask        ((uint32_t)0x0000F000)

/************ bit definition for TRNG RNG_INDEX REGISTER ************/
#define TRNG_RNG_INDEX_FIFO_RD_OV_Mask ((uint32_t)0x80000000)



/** @defgroup RNG_Exported_Types
 * @{
 */
typedef enum
{
    TRNG0
} TRNG_ChannelTypeDef;
#define IS_TRNG_CHANNEL(CHANNEL) (((CHANNEL) == TRNG0))

typedef enum
{
    TRNG_PDSource_0 = 0x0001,
    TRNG_PDSource_1 = 0x0002,
    TRNG_PDSource_2 = 0x0004,
    TRNG_PDSource_3 = 0x0008,
} TRNG_PDSourceTypeDef;
#define IS_TRNG_PDSource(PDSOURCE) \
    (((PDSOURCE) == TRNG_PDSource_0) || ((PDSOURCE) == TRNG_PDSource_1) || ((PDSOURCE) == TRNG_PDSource_2) || ((PDSOURCE) == TRNG_PDSource_3))

#define TRNG_IT_RNG0_S128   	((uint32_t)0x00000001)
#define TRNG_IT_TOT_D_FAIL	 	((uint32_t)0x00000002)
#define TRNG_IT_TOT_FAIL	 	((uint32_t)0x00000004)
#define TRNG_IT_CHI_FAIL	 	((uint32_t)0x00000008)
#define IS_TRNG_GET_IT(IT)  (((IT) == TRNG_IT_RNG0_S128) || ((IT) == TRNG_IT_TOT_D_FAIL) || ((IT) == TRNG_IT_TOT_FAIL) || ((IT) == TRNG_IT_CHI_FAIL))

/** @defgroup RNG_Exported_Functions
 * @{
 */
void TRNG_Start(TRNG_ChannelTypeDef TRNGx);
void TRNG_Stop(TRNG_ChannelTypeDef TRNGx);

ErrorStatus TRNG_Get(uint32_t rand[4], TRNG_ChannelTypeDef TRNGx);

void TRNG_SetPseudoRandom(uint32_t PseudoRandom);
void TRNG_ITConfig(FunctionalState NewState);

ITStatus TRNG_GetITStatus(uint32_t TRNG_IT);

void TRNG_ClearITPendingBit(uint32_t TRNG_IT);
void TRNG_SelectPDSource(TRNG_PDSourceTypeDef SOURCEx);

#ifdef __cplusplus
}
#endif

#endif

/**************************      (C) COPYRIGHT Megahunt    *****END OF FILE****/
