// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_JPEG_H__
#define __PERIPH_JPEG_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */
typedef struct
{
    __IO uint32_t CONFR0;     // Address offset 00h
    __IO uint32_t CONFR1;     // Address offset 04h
    __IO uint32_t CONFR2;     // Address offset 08h
    __IO uint32_t CONFR3;     // Address offset 0Ch
    __IO uint32_t CONFR4;     // Address offset 10h
    __IO uint32_t CONFR5;     // Address offset 14h
    __IO uint32_t CONFR6;     // Address offset 18h
    __IO uint32_t CONFR7;     // Address offset 1Ch
    __IO uint32_t INBUF_THR;  // Address offset 20h
    __IO uint32_t OUTBUF_THR; // Address offset 24h
    __I uint32_t  RSVD0[2];   // Address offset 28h -- 2Ch
    __IO uint32_t CR;         // Address offset 30h
    __IO uint32_t SR;         // Address offset 34h
    __IO uint32_t CFR;        // Address offset 38h
    __I uint32_t  RSVD1;      // Address offset 3Ch
    __IO uint32_t DIR;        // Address offset 40h
    __IO uint32_t DOR;        // Address offset 44h
} JPEGD_TypeDef;

#define JPEG_CONFR0_START ((uint32_t)0x00000001)
#define JPEG_CONFR0_SWR   ((uint32_t)0x00000008)
#define JPEG_CONFR0_LP    ((uint32_t)0x00000010)

#define JPEG_CONFIG1_NF ((uint32_t)0x00000003)

#define JPEG_GRAYSCALE_COLORSPACE ((uint32_t)0x00000001)
#define JPEG_YCBCR_COLORSPACE     ((uint32_t)0x00000003)

#define JPEG_444_SUBSAMPLING ((uint32_t)0x00000000)
#define JPEG_420_SUBSAMPLING ((uint32_t)0x00000001)
#define JPEG_422_SUBSAMPLING ((uint32_t)0x00000002)

typedef struct
{
    uint32_t ColorSpace;
    uint32_t ChromaSubsampling;
    uint32_t ImageHeight;
    uint32_t ImageWidth;
} JPEG_InfoTypeDef;

#define IS_INPUT_BUFFER_THRESHOLD(THR)  ((THR) <= 32)
#define IS_OUTPUT_BUFFER_THRESHOLD(THR) ((THR) <= 32)

#define JPEG_IT_IFT  ((uint32_t)0x00000002)
#define JPEG_IT_IFNF ((uint32_t)0x00000004)
#define JPEG_IT_OFT  ((uint32_t)0x00000008)
#define JPEG_IT_OFNE ((uint32_t)0x00000010)
#define JPEG_IT_EOC  ((uint32_t)0x00000020)
#define JPEG_IT_HPD  ((uint32_t)0x00000040)
#define IS_JPEG_IT(IT) \
    (((IT) == JPEG_IT_IFT) || ((IT) == JPEG_IT_IFNF) || ((IT) == JPEG_IT_OFT) || ((IT) == JPEG_IT_OFNE) || ((IT) == JPEG_IT_EOC) || ((IT) == JPEG_IT_HPD))

#define JPEG_FLAG_CFGERR ((uint32_t)0x00000001)
#define JPEG_FLAG_IFT    ((uint32_t)0x00000002)
#define JPEG_FLAG_IFNF   ((uint32_t)0x00000004)
#define JPEG_FLAG_OFT    ((uint32_t)0x00000008)
#define JPEG_FLAG_OFNE   ((uint32_t)0x00000010)
#define JPEG_FLAG_EOC    ((uint32_t)0x00000020)
#define JPEG_FLAG_HPD    ((uint32_t)0x00000040)
#define JPEG_FLAG_CO     ((uint32_t)0x00000080)
#define IS_JPEG_FLAG(FLAG)                                                                                                                                 \
    (((FLAG) == JPEG_FLAG_CFGERR) || ((FLAG) == JPEG_FLAG_IFT) || ((FLAG) == JPEG_FLAG_IFNF) || ((FLAG) == JPEG_FLAG_OFT) || ((FLAG) == JPEG_FLAG_OFNE) || \
     ((FLAG) == JPEG_FLAG_EOC) || ((FLAG) == JPEG_FLAG_HPD) || ((FLAG) == JPEG_FLAG_CO))

#define IS_JPEG_CLEAR_FLAG(FLAG) (((FLAG) == JPEG_FLAG_EOC) || ((FLAG) == JPEG_FLAG_HPD))

#define IS_JPEG_CLEAR_IT_PENDING(IT) (((IT) == JPEG_FLAG_EOC) || ((IT) == JPEG_FLAG_HPD))

void       JPEG_Cmd(FunctionalState NewState);
void       JPEG_Reset(void);
void       JPEG_LowPowerMode(FunctionalState NewState);
void       JPEG_GetInfo(JPEG_InfoTypeDef* pInfo);
void       JPEG_SetInFifoThreshold(uint32_t thr);
void       JPEG_SetOutFifoThreshold(uint32_t thr);
void       JPEG_ITConfig(uint32_t JPEG_IT, FunctionalState NewState);
FlagStatus JPEG_GetFlagStatus(uint32_t JPEG_FLAG);
void       JPEG_ClearFlag(uint32_t JPEG_FLAG);
ITStatus   JPEG_GetITStatus(uint32_t JPEG_IT);
void       JPEG_ClearITPendingBit(uint32_t JPEG_IT);
void       JPEG_SendData(uint8_t data);
uint8_t    JPEG_ReceiveData(void);

#ifdef __cplusplus
}
#endif

#endif
