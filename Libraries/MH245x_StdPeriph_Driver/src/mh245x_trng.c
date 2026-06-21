// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_trng.h"

const uint32_t TRNG_RNG_AMA_PD_Mask[1]      = {TRNG_RNG_ANA_PD_ALL_Mask};
const uint32_t TRNG_RNG_CSR_S128_Mask[1]    = {TRNG_RNG_CSR_S128_TRNG0_Mask};

/**
 * @method	TRNG_Get
 * @brief
 * @param	rand
 * @param	TRNGx
 * @retval An ErrorStatus enumeration value:
 *          - SUCCESS: HSE oscillator is stable and ready to use
 *          - ERROR: HSE oscillator not yet ready
 */
ErrorStatus TRNG_Get(uint32_t rand[4], TRNG_ChannelTypeDef TRNGx)
{
    ErrorStatus ret = ERROR;
	
    assert_param(IS_TRNG_CHANNEL(TRNGx));

    if (TRNG->RNG_CSR & TRNG_RNG_CSR_S128_Mask[TRNGx])
    {
        rand[0] = TRNG->RNG_DATA[TRNGx];
        rand[1] = TRNG->RNG_DATA[TRNGx];
        rand[2] = TRNG->RNG_DATA[TRNGx];
        rand[3] = TRNG->RNG_DATA[TRNGx];
        ret     = SUCCESS;
    }
    return ret;
}

/**
 * @method	TRNG_SetPseudoRandom
 * @brief
 * @param	PseudoRandom
 * @retval
 */
void TRNG_SetPseudoRandom(uint32_t PseudoRandom)
{
    TRNG->RNG_PN = PseudoRandom;
}


/**
 * @method	TRNG_SelectPDSource
 * @brief
 * @param	SOURCEx
 * @retval
 */
void TRNG_SelectPDSource(TRNG_PDSourceTypeDef SOURCEx)
{
    assert_param(IS_TRNG_PDSource(SOURCEx));

    TRNG->RNG_ANA = (TRNG->RNG_ANA & ~(0xFF << 12)) | (SOURCEx << 12);
}

/**
 * @method	TRNG_ITConfig
 * @brief
 * @param	NewState
 * @retval
 */
void TRNG_ITConfig(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        TRNG->RNG_CSR |= TRNG_RNG_CSR_INTP_EN_Mask;
    }
    else
    {
        TRNG->RNG_CSR &= ~TRNG_RNG_CSR_INTP_EN_Mask;
    }
}

/**
 * @brief  获取中断状态
 * @param  TRNG_IT:
 *         TRNG_IT_RNG0_S128
 *         TRNG_IT_TOT_D_FAIL
 *         TRNG_IT_TOT_FAIL
 *         TRNG_IT_CHI_FAIL
 * @retval The new state of USART_IT (SET or RESET).
 */
ITStatus TRNG_GetITStatus(uint32_t TRNG_IT)
{
    assert_param(IS_TRNG_GET_IT(TRNG_IT));

    if (((TRNG->RNG_CSR) & TRNG_IT) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
 * @brief  清除中断标志位
 * @param  TRNG_IT:
 *         TRNG_IT_RNG0_S128
 *         TRNG_IT_TOT_D_FAIL
 *         TRNG_IT_TOT_FAIL
 *         TRNG_IT_CHI_FAIL
 * @retval None
 */
void TRNG_ClearITPendingBit(uint32_t TRNG_IT)
{
    assert_param(IS_TRNG_GET_IT(TRNG_IT));

    TRNG->RNG_CSR &= ~TRNG_IT;
}

/**
 * @brief  启动生成随机数
 * @param
 * @retval None
 */
void TRNG_Start(TRNG_ChannelTypeDef TRNGx)
{
    assert_param(IS_TRNG_CHANNEL(TRNGx));

    TRNG->RNG_ANA &= ~TRNG_RNG_AMA_PD_Mask[TRNGx];
    TRNG->RNG_CSR &= ~TRNG_RNG_CSR_S128_Mask[TRNGx];
}

/**
 * @brief  停止产生随机数
 * @param
 * @retval None
 */
void TRNG_Stop(TRNG_ChannelTypeDef TRNGx)
{
    assert_param(IS_TRNG_CHANNEL(TRNGx));

    TRNG->RNG_ANA |= TRNG_RNG_AMA_PD_Mask[TRNGx];
}
