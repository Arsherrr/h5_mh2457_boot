// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_jpeg.h"

void JPEG_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        JPEGD->CONFR1 |= BIT3;
        JPEGD->CR |= BIT0;
        JPEGD->CONFR0 |= JPEG_CONFR0_START;
    }
    else 
    {
        JPEGD->CONFR1 &= ~BIT3;
        JPEGD->CR &= ~BIT0;
        JPEGD->CONFR0 &= ~JPEG_CONFR0_START;
    }
}

void JPEG_Reset(void)
{
    JPEGD->CONFR0 |= JPEG_CONFR0_SWR;
    JPEGD->CONFR0 &= ~JPEG_CONFR0_SWR;
}

void JPEG_LowPowerMode(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        JPEGD->CONFR0 |= JPEG_CONFR0_LP;
    }
    else 
    {
        JPEGD->CONFR0 &= ~JPEG_CONFR0_LP;
    }
}

void JPEG_GetInfo(JPEG_InfoTypeDef* pInfo)
{
    uint32_t yblockNb;
    uint32_t cBblockNb;
    uint32_t cRblockNb;
    
    if ((JPEGD->CONFR1 & JPEG_CONFIG1_NF) == 0x01)
    {
        pInfo->ColorSpace = JPEG_GRAYSCALE_COLORSPACE;
    }
    else if ((JPEGD->CONFR1 & JPEG_CONFIG1_NF) == 0x03)
    {
        pInfo->ColorSpace = JPEG_YCBCR_COLORSPACE;
    }
    else 
    {
        pInfo->ColorSpace = JPEG_YCBCR_COLORSPACE;
    }
    
    pInfo->ImageHeight = (JPEGD->CONFR1 & 0xFFFF0000UL) >> 16;
    
    pInfo->ImageWidth = (JPEGD->CONFR3 & 0xFFFF0000UL) >> 16;
    
    if (pInfo->ColorSpace == JPEG_YCBCR_COLORSPACE)
    {
        yblockNb = (JPEGD->CONFR4 & 0x000000F0UL) >> 4;
        cBblockNb = (JPEGD->CONFR5 & 0x000000F0UL) >> 4;
        cRblockNb = (JPEGD->CONFR6 & 0x000000F0UL) >> 4;
        
        if ((yblockNb == 2UL) && (cBblockNb == 3UL) && (cRblockNb == 4UL))
        {
            pInfo->ChromaSubsampling = JPEG_422_SUBSAMPLING;
        }
        else if ((yblockNb == 1UL) && (cBblockNb == 2UL) && (cRblockNb == 3UL))
        {
            pInfo->ChromaSubsampling = JPEG_444_SUBSAMPLING;
        }
        else if ((yblockNb == 4UL) && (cBblockNb == 5UL) && (cRblockNb == 6UL))
        {
            pInfo->ChromaSubsampling = JPEG_420_SUBSAMPLING;
        }
        else
        {
            pInfo->ChromaSubsampling = JPEG_444_SUBSAMPLING;
        }
    }
}

void JPEG_SetInFifoThreshold(uint32_t thr)
{
    assert_param(IS_INPUT_BUFFER_THRESHOLD(thr));
    JPEGD->INBUF_THR = thr;
}

void JPEG_SetOutFifoThreshold(uint32_t thr)
{
    assert_param(IS_OUTPUT_BUFFER_THRESHOLD(thr));
    JPEGD->OUTBUF_THR = thr;
}

void JPEG_ITConfig(uint32_t JPEG_IT, FunctionalState NewState)
{
    assert_param(IS_JPEG_IT(JPEG_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState != DISABLE)
    {
        JPEGD->CR |= JPEG_IT;
    }
    else 
    {
        JPEGD->CR &= ~JPEG_IT;
    }
}

FlagStatus JPEG_GetFlagStatus(uint32_t JPEG_FLAG)
{
    FlagStatus bitstatus = RESET;
    
    assert_param(IS_JPEG_FLAG(JPEG_FLAG));
    
    if ((JPEGD->SR & JPEG_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

void JPEG_ClearFlag(uint32_t JPEG_FLAG)
{
    assert_param(IS_JPEG_CLEAR_FLAG(JPEG_FLAG));
    
    JPEGD->CFR = (uint32_t)JPEG_FLAG;
}

ITStatus JPEG_GetITStatus(uint32_t JPEG_IT)
{
    ITStatus bitstatus = RESET;
    
    assert_param(IS_JPEG_IT(JPEG_IT));
    
    if ((JPEGD->SR & JPEG_IT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    if (((JPEGD->CR & JPEG_IT) != (uint32_t)RESET) && (bitstatus != (uint32_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

void JPEG_ClearITPendingBit(uint32_t JPEG_IT)
{
    assert_param(IS_LTDC_IT(JPEG_IT));

    JPEGD->CFR = (uint32_t)JPEG_IT;
}

void JPEG_SendData(uint8_t data)
{
    JPEGD->DIR = (uint32_t)data;
}

uint8_t JPEG_ReceiveData(void)
{
    return (uint8_t)JPEGD->DOR;
}
