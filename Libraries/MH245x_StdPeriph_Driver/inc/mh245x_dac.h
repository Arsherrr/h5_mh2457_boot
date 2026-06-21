// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh2457.h"

#ifndef __PERIPH_DAC_H__
#define __PERIPH_DAC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t CR;      /*!< DAC control register,                                    Address offset: 0x00 */
    __IO uint32_t SWTRIGR; /*!< DAC software trigger register,                           Address offset: 0x04 */
    __IO uint32_t DHR12R1; /*!< DAC channel1 12-bit right-aligned data holding register, Address offset: 0x08 */
    __IO uint32_t DHR12L1; /*!< DAC channel1 12-bit left aligned data holding register,  Address offset: 0x0C */
    __IO uint32_t DHR8R1;  /*!< DAC channel1 8-bit right aligned data holding register,  Address offset: 0x10 */
    __IO uint32_t DHR12R2; /*!< DAC channel2 12-bit right aligned data holding register, Address offset: 0x14 */
    __IO uint32_t DHR12L2; /*!< DAC channel2 12-bit left aligned data holding register,  Address offset: 0x18 */
    __IO uint32_t DHR8R2;  /*!< DAC channel2 8-bit right-aligned data holding register,  Address offset: 0x1C */
    __IO uint32_t DHR12RD; /*!< Dual DAC 12-bit right-aligned data holding register,     Address offset: 0x20 */
    __IO uint32_t DHR12LD; /*!< DUAL DAC 12-bit left aligned data holding register,      Address offset: 0x24 */
    __IO uint32_t DHR8RD;  /*!< DUAL DAC 8-bit right aligned data holding register,      Address offset: 0x28 */
    __IO uint32_t DOR1;    /*!< DAC channel1 data output register,                       Address offset: 0x2C */
    __IO uint32_t DOR2;    /*!< DAC channel2 data output register,                       Address offset: 0x30 */
    __IO uint32_t SR;      /*!< DAC status register,                                     Address offset: 0x34 */
} DAC_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/********************  Bit definition for DAC_CR register  ********************/
#define DAC_CR_EN1   ((uint32_t)0x00000001) /*!<DAC channel1 enable */
#define DAC_CR_BOFF1 ((uint32_t)0x00000002) /*!<DAC channel1 output buffer disable */
#define DAC_CR_TEN1  ((uint32_t)0x00000004) /*!<DAC channel1 Trigger enable */

#define DAC_CR_TSEL1   ((uint32_t)0x00000038) /*!<TSEL1[2:0] (DAC channel1 Trigger selection) */
#define DAC_CR_TSEL1_0 ((uint32_t)0x00000008) /*!<Bit 0 */
#define DAC_CR_TSEL1_1 ((uint32_t)0x00000010) /*!<Bit 1 */
#define DAC_CR_TSEL1_2 ((uint32_t)0x00000020) /*!<Bit 2 */

#define DAC_CR_WAVE1   ((uint32_t)0x000000C0) /*!<WAVE1[1:0] (DAC channel1 noise/triangle wave generation enable) */
#define DAC_CR_WAVE1_0 ((uint32_t)0x00000040) /*!<Bit 0 */
#define DAC_CR_WAVE1_1 ((uint32_t)0x00000080) /*!<Bit 1 */

#define DAC_CR_MAMP1   ((uint32_t)0x00000F00) /*!<MAMP1[3:0] (DAC channel1 Mask/Amplitude selector) */
#define DAC_CR_MAMP1_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define DAC_CR_MAMP1_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define DAC_CR_MAMP1_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define DAC_CR_MAMP1_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define DAC_CR_DMAEN1    ((uint32_t)0x00001000) /*!<DAC channel1 DMA enable */
#define DAC_CR_DMAUDRIE1 ((uint32_t)0x00002000) /*!<DAC channel1 DMA underrun interrupt enable*/
#define DAC_CR_EN2       ((uint32_t)0x00010000) /*!<DAC channel2 enable */
#define DAC_CR_BOFF2     ((uint32_t)0x00020000) /*!<DAC channel2 output buffer disable */
#define DAC_CR_TEN2      ((uint32_t)0x00040000) /*!<DAC channel2 Trigger enable */

#define DAC_CR_TSEL2   ((uint32_t)0x00380000) /*!<TSEL2[2:0] (DAC channel2 Trigger selection) */
#define DAC_CR_TSEL2_0 ((uint32_t)0x00080000) /*!<Bit 0 */
#define DAC_CR_TSEL2_1 ((uint32_t)0x00100000) /*!<Bit 1 */
#define DAC_CR_TSEL2_2 ((uint32_t)0x00200000) /*!<Bit 2 */

#define DAC_CR_WAVE2   ((uint32_t)0x00C00000) /*!<WAVE2[1:0] (DAC channel2 noise/triangle wave generation enable) */
#define DAC_CR_WAVE2_0 ((uint32_t)0x00400000) /*!<Bit 0 */
#define DAC_CR_WAVE2_1 ((uint32_t)0x00800000) /*!<Bit 1 */

#define DAC_CR_MAMP2   ((uint32_t)0x0F000000) /*!<MAMP2[3:0] (DAC channel2 Mask/Amplitude selector) */
#define DAC_CR_MAMP2_0 ((uint32_t)0x01000000) /*!<Bit 0 */
#define DAC_CR_MAMP2_1 ((uint32_t)0x02000000) /*!<Bit 1 */
#define DAC_CR_MAMP2_2 ((uint32_t)0x04000000) /*!<Bit 2 */
#define DAC_CR_MAMP2_3 ((uint32_t)0x08000000) /*!<Bit 3 */

#define DAC_CR_DMAEN2    ((uint32_t)0x10000000)  /*!<DAC channel2 DMA enabled */
#define DAC_CR_DMAUDRIE2 ((uint32_t)0x20000000U) /*!<DAC channel2 DMA underrun interrupt enable*/

/*****************  Bit definition for DAC_SWTRIGR register  ******************/
#define DAC_SWTRIGR_SWTRIG1 ((uint8_t)0x01) /*!<DAC channel1 software trigger */
#define DAC_SWTRIGR_SWTRIG2 ((uint8_t)0x02) /*!<DAC channel2 software trigger */

/*****************  Bit definition for DAC_DHR12R1 register  ******************/
#define DAC_DHR12R1_DACC1DHR ((uint16_t)0x0FFF) /*!<DAC channel1 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L1 register  ******************/
#define DAC_DHR12L1_DACC1DHR ((uint16_t)0xFFF0) /*!<DAC channel1 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R1 register  ******************/
#define DAC_DHR8R1_DACC1DHR ((uint8_t)0xFF) /*!<DAC channel1 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12R2 register  ******************/
#define DAC_DHR12R2_DACC2DHR ((uint16_t)0x0FFF) /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L2 register  ******************/
#define DAC_DHR12L2_DACC2DHR ((uint16_t)0xFFF0) /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R2 register  ******************/
#define DAC_DHR8R2_DACC2DHR ((uint8_t)0xFF) /*!<DAC channel2 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12RD register  ******************/
#define DAC_DHR12RD_DACC1DHR ((uint32_t)0x00000FFF) /*!<DAC channel1 12-bit Right aligned data */
#define DAC_DHR12RD_DACC2DHR ((uint32_t)0x0FFF0000) /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12LD register  ******************/
#define DAC_DHR12LD_DACC1DHR ((uint32_t)0x0000FFF0) /*!<DAC channel1 12-bit Left aligned data */
#define DAC_DHR12LD_DACC2DHR ((uint32_t)0xFFF00000) /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8RD register  ******************/
#define DAC_DHR8RD_DACC1DHR ((uint16_t)0x00FF) /*!<DAC channel1 8-bit Right aligned data */
#define DAC_DHR8RD_DACC2DHR ((uint16_t)0xFF00) /*!<DAC channel2 8-bit Right aligned data */

/*******************  Bit definition for DAC_DOR1 register  *******************/
#define DAC_DOR1_DACC1DOR ((uint16_t)0x0FFF) /*!<DAC channel1 data output */

/*******************  Bit definition for DAC_DOR2 register  *******************/
#define DAC_DOR2_DACC2DOR ((uint16_t)0x0FFF) /*!<DAC channel2 data output */

/********************  Bit definition for DAC_SR register  ********************/
#define DAC_SR_DMAUDR1 ((uint32_t)0x00002000) /*!<DAC channel1 DMA underrun flag */
#define DAC_SR_DMAUDR2 ((uint32_t)0x20000000) /*!<DAC channel2 DMA underrun flag */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  DAC Init structure definition
 */

typedef struct
{
    uint32_t DAC_Trigger; /*!< Specifies the external trigger for the selected DAC channel.
                               This parameter can be a value of @ref DAC_trigger_selection */

    uint32_t DAC_WaveGeneration; /*!< Specifies whether DAC channel noise waves or triangle waves
                                      are generated, or whether no wave is generated.
                                      This parameter can be a value of @ref DAC_wave_generation */

    uint32_t DAC_LFSRUnmask_TriangleAmplitude; /*!< Specifies the LFSR mask for noise wave generation or
                                                    the maximum amplitude triangle generation for the DAC channel.
                                                    This parameter can be a value of @ref DAC_lfsrunmask_triangleamplitude */

    uint32_t DAC_OutputBuffer; /*!< Specifies whether the DAC channel output buffer is enabled or disabled.
                                    This parameter can be a value of @ref DAC_output_buffer */
} DAC_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup DAC_Exported_Constants
 * @{
 */

/** @defgroup DAC_trigger_selection
 * @{
 */

#define DAC_Trigger_None                                                                                       \
    ((uint32_t)0x00000000)                         /*!< Conversion is automatic once the DAC1_DHRxxxx register \
                                                        has been loaded, and not by external trigger */
#define DAC_Trigger_T2_TRGO ((uint32_t)0x00000024) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T4_TRGO ((uint32_t)0x0000002C) /*!< TIM4 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T5_TRGO ((uint32_t)0x0000001C) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T6_TRGO ((uint32_t)0x00000004) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T7_TRGO ((uint32_t)0x00000014) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T8_TRGO ((uint32_t)0x0000000C) /*!< TIM8 TRGO selected as external conversion trigger for DAC channel */

#define DAC_Trigger_Ext_IT9  ((uint32_t)0x00000034) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_Trigger_Software ((uint32_t)0x0000003C) /*!< Conversion started by software trigger for DAC channel */

#define IS_DAC_TRIGGER(TRIGGER)                                                                                                                              \
    (((TRIGGER) == DAC_Trigger_None) || ((TRIGGER) == DAC_Trigger_T6_TRGO) || ((TRIGGER) == DAC_Trigger_T8_TRGO) || ((TRIGGER) == DAC_Trigger_T7_TRGO) ||    \
     ((TRIGGER) == DAC_Trigger_T5_TRGO) || ((TRIGGER) == DAC_Trigger_T2_TRGO) || ((TRIGGER) == DAC_Trigger_T4_TRGO) || ((TRIGGER) == DAC_Trigger_Ext_IT9) || \
     ((TRIGGER) == DAC_Trigger_Software))

/**
 * @}
 */

/** @defgroup DAC_wave_generation
 * @{
 */

#define DAC_WaveGeneration_None     ((uint32_t)0x00000000)
#define DAC_WaveGeneration_Noise    ((uint32_t)0x00000040)
#define DAC_WaveGeneration_Triangle ((uint32_t)0x00000080)
#define IS_DAC_GENERATE_WAVE(WAVE)  (((WAVE) == DAC_WaveGeneration_None) || ((WAVE) == DAC_WaveGeneration_Noise) || ((WAVE) == DAC_WaveGeneration_Triangle))
/**
 * @}
 */

/** @defgroup DAC_lfsrunmask_triangleamplitude
 * @{
 */

#define DAC_LFSRUnmask_Bit0        ((uint32_t)0x00000000) /*!< Unmask DAC channel LFSR bit0 for noise wave generation */
#define DAC_LFSRUnmask_Bits1_0     ((uint32_t)0x00000100) /*!< Unmask DAC channel LFSR bit[1:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits2_0     ((uint32_t)0x00000200) /*!< Unmask DAC channel LFSR bit[2:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits3_0     ((uint32_t)0x00000300) /*!< Unmask DAC channel LFSR bit[3:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits4_0     ((uint32_t)0x00000400) /*!< Unmask DAC channel LFSR bit[4:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits5_0     ((uint32_t)0x00000500) /*!< Unmask DAC channel LFSR bit[5:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits6_0     ((uint32_t)0x00000600) /*!< Unmask DAC channel LFSR bit[6:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits7_0     ((uint32_t)0x00000700) /*!< Unmask DAC channel LFSR bit[7:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits8_0     ((uint32_t)0x00000800) /*!< Unmask DAC channel LFSR bit[8:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits9_0     ((uint32_t)0x00000900) /*!< Unmask DAC channel LFSR bit[9:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits10_0    ((uint32_t)0x00000A00) /*!< Unmask DAC channel LFSR bit[10:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits11_0    ((uint32_t)0x00000B00) /*!< Unmask DAC channel LFSR bit[11:0] for noise wave generation */
#define DAC_TriangleAmplitude_1    ((uint32_t)0x00000000) /*!< Select max triangle amplitude of 1 */
#define DAC_TriangleAmplitude_3    ((uint32_t)0x00000100) /*!< Select max triangle amplitude of 3 */
#define DAC_TriangleAmplitude_7    ((uint32_t)0x00000200) /*!< Select max triangle amplitude of 7 */
#define DAC_TriangleAmplitude_15   ((uint32_t)0x00000300) /*!< Select max triangle amplitude of 15 */
#define DAC_TriangleAmplitude_31   ((uint32_t)0x00000400) /*!< Select max triangle amplitude of 31 */
#define DAC_TriangleAmplitude_63   ((uint32_t)0x00000500) /*!< Select max triangle amplitude of 63 */
#define DAC_TriangleAmplitude_127  ((uint32_t)0x00000600) /*!< Select max triangle amplitude of 127 */
#define DAC_TriangleAmplitude_255  ((uint32_t)0x00000700) /*!< Select max triangle amplitude of 255 */
#define DAC_TriangleAmplitude_511  ((uint32_t)0x00000800) /*!< Select max triangle amplitude of 511 */
#define DAC_TriangleAmplitude_1023 ((uint32_t)0x00000900) /*!< Select max triangle amplitude of 1023 */
#define DAC_TriangleAmplitude_2047 ((uint32_t)0x00000A00) /*!< Select max triangle amplitude of 2047 */
#define DAC_TriangleAmplitude_4095 ((uint32_t)0x00000B00) /*!< Select max triangle amplitude of 4095 */

#define IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(VALUE)                                                                                                          \
    (((VALUE) == DAC_LFSRUnmask_Bit0) || ((VALUE) == DAC_LFSRUnmask_Bits1_0) || ((VALUE) == DAC_LFSRUnmask_Bits2_0) || ((VALUE) == DAC_LFSRUnmask_Bits3_0) || \
     ((VALUE) == DAC_LFSRUnmask_Bits4_0) || ((VALUE) == DAC_LFSRUnmask_Bits5_0) || ((VALUE) == DAC_LFSRUnmask_Bits6_0) ||                                     \
     ((VALUE) == DAC_LFSRUnmask_Bits7_0) || ((VALUE) == DAC_LFSRUnmask_Bits8_0) || ((VALUE) == DAC_LFSRUnmask_Bits9_0) ||                                     \
     ((VALUE) == DAC_LFSRUnmask_Bits10_0) || ((VALUE) == DAC_LFSRUnmask_Bits11_0) || ((VALUE) == DAC_TriangleAmplitude_1) ||                                  \
     ((VALUE) == DAC_TriangleAmplitude_3) || ((VALUE) == DAC_TriangleAmplitude_7) || ((VALUE) == DAC_TriangleAmplitude_15) ||                                 \
     ((VALUE) == DAC_TriangleAmplitude_31) || ((VALUE) == DAC_TriangleAmplitude_63) || ((VALUE) == DAC_TriangleAmplitude_127) ||                              \
     ((VALUE) == DAC_TriangleAmplitude_255) || ((VALUE) == DAC_TriangleAmplitude_511) || ((VALUE) == DAC_TriangleAmplitude_1023) ||                           \
     ((VALUE) == DAC_TriangleAmplitude_2047) || ((VALUE) == DAC_TriangleAmplitude_4095))
/**
 * @}
 */

/** @defgroup DAC_output_buffer
 * @{
 */

#define DAC_OutputBuffer_Enable           ((uint32_t)0x00000000)
#define DAC_OutputBuffer_Disable          ((uint32_t)0x00000002)
#define IS_DAC_OUTPUT_BUFFER_STATE(STATE) (((STATE) == DAC_OutputBuffer_Enable) || ((STATE) == DAC_OutputBuffer_Disable))
/**
 * @}
 */

/** @defgroup DAC_Channel_selection
 * @{
 */

#define DAC_Channel_1           ((uint32_t)0x00000000)
#define DAC_Channel_2           ((uint32_t)0x00000010)
#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_Channel_1) || ((CHANNEL) == DAC_Channel_2))
/**
 * @}
 */

/** @defgroup DAC_data_alignement
 * @{
 */

#define DAC_Align_12b_R     ((uint32_t)0x00000000)
#define DAC_Align_12b_L     ((uint32_t)0x00000004)
#define DAC_Align_8b_R      ((uint32_t)0x00000008)
#define IS_DAC_ALIGN(ALIGN) (((ALIGN) == DAC_Align_12b_R) || ((ALIGN) == DAC_Align_12b_L) || ((ALIGN) == DAC_Align_8b_R))
/**
 * @}
 */

/** @defgroup DAC_wave_generation
 * @{
 */

#define DAC_Wave_Noise    ((uint32_t)0x00000040)
#define DAC_Wave_Triangle ((uint32_t)0x00000080)
#define IS_DAC_WAVE(WAVE) (((WAVE) == DAC_Wave_Noise) || ((WAVE) == DAC_Wave_Triangle))
/**
 * @}
 */

/** @defgroup DAC_data
 * @{
 */

#define IS_DAC_DATA(DATA) ((DATA) <= 0xFFF0)
/**
 * @}
 */

/** @defgroup DAC_interrupts_definition
 * @{
 */
#define DAC_IT_DMAUDR ((uint32_t)0x00002000)
#define IS_DAC_IT(IT) (((IT) == DAC_IT_DMAUDR))

/**
 * @}
 */

/** @defgroup DAC_flags_definition
 * @{
 */

#define DAC_FLAG_DMAUDR   ((uint32_t)0x00002000)
#define IS_DAC_FLAG(FLAG) (((FLAG) == DAC_FLAG_DMAUDR))

/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the DAC configuration to the default reset state *****/
void DAC_DeInit(void);

/*  DAC channels configuration: trigger, output buffer, data format functions */
void     DAC_Init(uint32_t DAC_Channel, DAC_InitTypeDef* DAC_InitStruct);
void     DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct);
void     DAC_Cmd(uint32_t DAC_Channel, FunctionalState NewState);
void     DAC_SoftwareTriggerCmd(uint32_t DAC_Channel, FunctionalState NewState);
void     DAC_DualSoftwareTriggerCmd(FunctionalState NewState);
void     DAC_WaveGenerationCmd(uint32_t DAC_Channel, uint32_t DAC_Wave, FunctionalState NewState);
void     DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data);
void     DAC_SetChannel2Data(uint32_t DAC_Align, uint16_t Data);
void     DAC_SetDualChannelData(uint32_t DAC_Align, uint16_t Data2, uint16_t Data1);
uint16_t DAC_GetDataOutputValue(uint32_t DAC_Channel);

/* DMA management functions ***************************************************/
void DAC_DMACmd(uint32_t DAC_Channel, FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void       DAC_ITConfig(uint32_t DAC_Channel, uint32_t DAC_IT, FunctionalState NewState);
FlagStatus DAC_GetFlagStatus(uint32_t DAC_Channel, uint32_t DAC_FLAG);
void       DAC_ClearFlag(uint32_t DAC_Channel, uint32_t DAC_FLAG);
ITStatus   DAC_GetITStatus(uint32_t DAC_Channel, uint32_t DAC_IT);
void       DAC_ClearITPendingBit(uint32_t DAC_Channel, uint32_t DAC_IT);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_DAC_H__
