// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_DMA_H__
#define __PERIPH_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DMA_Stream_TypeDef_s DMA_Stream_TypeDef;

typedef struct DMA_TypeDef_s DMA_TypeDef;

#include "mh2457.h"

/* --------  Feature Definition  -------------------------------------------- */

#ifndef FEATURE_DMA_EXPAND_RANGE
#define FEATURE_DMA_EXPAND_RANGE 0
#endif

#ifndef FEATURE_DMA_SUPPORT_LINK
#define FEATURE_DMA_SUPPORT_LINK 0
#endif

/* --------  Register Definition  ------------------------------------------- */

struct DMA_Stream_TypeDef_s
{
    __IO uint32_t CR;   /*!< DMA stream x configuration register      */
    __IO uint32_t NDTR; /*!< DMA stream x number of data register     */
    __IO uint32_t PAR;  /*!< DMA stream x peripheral address register */
    __IO uint32_t M0AR; /*!< DMA stream x memory 0 address register   */
    __IO uint32_t M1AR; /*!< DMA stream x memory 1 address register   */
    __IO uint32_t FCR;  /*!< DMA stream x FIFO control register       */
};

struct DMA_TypeDef_s
{
    __IO uint32_t LISR;  /*!< DMA low interrupt status register,      Address offset: 0x00 */
    __IO uint32_t HISR;  /*!< DMA high interrupt status register,     Address offset: 0x04 */
    __IO uint32_t LIFCR; /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
    __IO uint32_t HIFCR; /*!< DMA high interrupt flag clear register, Address offset: 0x0C */

    DMA_Stream_TypeDef STREAM[8];
#if FEATURE_DMA_SUPPORT_LINK
    __IO uint32_t LAR[8];
#endif
};

typedef struct
{
    uint32_t DMA_Channel; /*!< Specifies the channel used for the specified stream.
                               This parameter can be a value of @ref DMA_channel */

    uint32_t DMA_PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Streamx. */

    uint32_t DMA_Memory0BaseAddr; /*!< Specifies the memory 0 base address for DMAy Streamx.
                                       This memory is the default memory used when double buffer mode is
                                       not enabled. */

    uint32_t DMA_DIR; /*!< Specifies if the data will be transferred from memory to peripheral,
                           from memory to memory or from peripheral to memory.
                           This parameter can be a value of @ref DMA_data_transfer_direction */

    uint32_t DMA_BufferSize; /*!< Specifies the buffer size, in data unit, of the specified Stream.
                                  The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                  or DMA_MemoryDataSize members depending in the transfer direction. */

    uint32_t DMA_PeripheralInc; /*!< Specifies whether the Peripheral address register should be incremented or not.
                                     This parameter can be a value of @ref DMA_peripheral_incremented_mode */

    uint32_t DMA_MemoryInc; /*!< Specifies whether the memory address register should be incremented or not.
                                 This parameter can be a value of @ref DMA_memory_incremented_mode */

    uint32_t DMA_PeripheralDataSize; /*!< Specifies the Peripheral data width.
                                          This parameter can be a value of @ref DMA_peripheral_data_size */

    uint32_t DMA_MemoryDataSize; /*!< Specifies the Memory data width.
                                      This parameter can be a value of @ref DMA_memory_data_size */

    uint32_t DMA_Mode; /*!< Specifies the operation mode of the DMAy Streamx.
                            This parameter can be a value of @ref DMA_mode
                            @note The circular buffer mode cannot be used if the memory-to-memory
                                  data transfer is configured on the selected Stream */

    uint32_t DMA_Priority; /*!< Specifies the software priority for the DMAy Streamx.
                                This parameter can be a value of @ref DMA_priority_level */

    uint32_t DMA_FIFOMode; /*!< Specifies if the FIFO mode or Direct mode will be used for the specified Stream.
                                 This parameter can be a value of @ref DMA_fifo_direct_mode
                                 @note The Direct mode (FIFO mode disabled) cannot be used if the
                                        memory-to-memory data transfer is configured on the selected Stream */

    uint32_t DMA_FIFOThreshold; /*!< Specifies the FIFO threshold level.
                                     This parameter can be a value of @ref DMA_fifo_threshold_level */

    uint32_t DMA_MemoryBurst; /*!< Specifies the Burst transfer configuration for the memory transfers.
                                   It specifies the amount of data to be transferred in a single non interruptable
                                   transaction. This parameter can be a value of @ref DMA_memory_burst
                                   @note The burst mode is possible only if the address Increment mode is enabled. */

    uint32_t DMA_PeripheralBurst; /*!< Specifies the Burst transfer configuration for the peripheral transfers.
                                       It specifies the amount of data to be transferred in a single non interruptable
                                       transaction. This parameter can be a value of @ref DMA_peripheral_burst
                                       @note The burst mode is possible only if the address Increment mode is enabled. */
} DMA_InitTypeDef;

#if FEATURE_DMA_SUPPORT_LINK
/**
 * DMA link type define
 */
typedef struct
{
    uint32_t DMA_PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Streamx. */
    uint32_t DMA_MemoryBaseAddr;     /*!< Specifies the memory base address for DMAy Streamx. */
    uint32_t DMA_BufferSize;         /*!< Specifies the buffer size, in data unit, of the specified Stream.
                                          The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                          or DMA_MemoryDataSize members depending in the transfer direction. */
    uint32_t DMA_NextLinkBaseAddr;   /*!< Specifies the next dma link base addr. */
} DMA_LinkTypeDef;
#endif

/* --------  Register Bit Definition  --------------------------------------- */

#if FEATURE_DMA_EXPAND_RANGE
#define MBURST_OFFSET 28
#define MBURST_WIDTH  3

#define PBURST_OFFSET 21
#define PBURST_WIDTH  3

#define DMA_SxNDTR_LIMIT 0xFFFFFFFFUL
#else
#define MBURST_OFFSET 23
#define MBURST_WIDTH  2

#define PBURST_OFFSET 21
#define PBURST_WIDTH  2

#define DMA_SxNDTR_LIMIT 0xFFFFUL
#endif

/********************  Bits definition for DMA_SxCR register  *****************/
#if FEATURE_DMA_SUPPORT_LINK
#define DMA_SxCR_LINK  BIT31
#define DMA_SxCR_LTCIE BIT24
#else
#define DMA_SxCR_LINK  0
#define DMA_SxCR_LTCIE 0
#endif
#define DMA_SxCR_CHSEL   BITS(27, 25)
#define DMA_SxCR_MBURST  BITW(MBURST_OFFSET, MBURST_WIDTH)
#define DMA_SxCR_PBURST  BITW(PBURST_OFFSET, PBURST_WIDTH)
#define DMA_SxCR_ACK     BIT20
#define DMA_SxCR_CT      BIT19
#define DMA_SxCR_DBM     BIT18
#define DMA_SxCR_PL      BITS(17, 16)
#define DMA_SxCR_PL_0    BIT16
#define DMA_SxCR_PL_1    BIT17
#define DMA_SxCR_PINCOS  BIT15
#define DMA_SxCR_MSIZE   BITS(14, 13)
#define DMA_SxCR_MSIZE_0 BIT13
#define DMA_SxCR_MSIZE_1 BIT14
#define DMA_SxCR_PSIZE   BITS(12, 11)
#define DMA_SxCR_PSIZE_0 BIT11
#define DMA_SxCR_PSIZE_1 BIT12
#define DMA_SxCR_MINC    BIT10
#define DMA_SxCR_PINC    BIT9
#define DMA_SxCR_CIRC    BIT8
#define DMA_SxCR_DIR     BITS(7, 6)
#define DMA_SxCR_DIR_0   BIT6
#define DMA_SxCR_DIR_1   BIT7
#define DMA_SxCR_PFCTRL  BIT5
#define DMA_SxCR_TCIE    BIT4
#define DMA_SxCR_HTIE    BIT3
#define DMA_SxCR_TEIE    BIT2
#define DMA_SxCR_DMEIE   BIT1
#define DMA_SxCR_EN      BIT0

/********************  Bits definition for DMA_SxNDTR register  **************/
#define DMA_SxNDTR (DMA_SxNDTR_LIMIT)

/********************  Bits definition for DMA_SxFCR register  ****************/
#define DMA_SxFCR_FEIE  ((uint32_t)0x00000080)
#define DMA_SxFCR_FS    ((uint32_t)0x00000038)
#define DMA_SxFCR_FS_0  ((uint32_t)0x00000008)
#define DMA_SxFCR_FS_1  ((uint32_t)0x00000010)
#define DMA_SxFCR_FS_2  ((uint32_t)0x00000020)
#define DMA_SxFCR_DMDIS ((uint32_t)0x00000004)
#define DMA_SxFCR_FTH   ((uint32_t)0x00000003)
#define DMA_SxFCR_FTH_0 ((uint32_t)0x00000001)
#define DMA_SxFCR_FTH_1 ((uint32_t)0x00000002)

/********************  Bits definition for DMA_L/HISR register  ***************/
#define DMA_ISR_TCIF  BIT5
#define DMA_ISR_HTIF  BIT4
#define DMA_ISR_TEIF  BIT3
#define DMA_ISR_DMEIF BIT2
#define DMA_ISR_LTCIF BIT1
#define DMA_ISR_FEIF  BIT0

/********************  Bits definition for DMA_L/HIFCR register  ****************/
#define DMA_IFCR_CTCIF  DMA_ISR_TCIF
#define DMA_IFCR_CHTIF  DMA_ISR_HTIF
#define DMA_IFCR_CTEIF  DMA_ISR_TEIF
#define DMA_IFCR_CDMEIF DMA_ISR_DMEIF
#define DMA_IFCR_CLTCIF DMA_ISR_LTCIF
#define DMA_IFCR_CFEIF  DMA_ISR_FEIF

// DMA status flag register definitions
#define DMA_SFLAG(flag, x) ((flag) << (((x & 2U) << 3U) + (x & 1U) * 6U))

#define DMA_LISR_TCIF(x)  DMA_SFLAG(DMA_ISR_TCIF, x)
#define DMA_LISR_HTIF(x)  DMA_SFLAG(DMA_ISR_HTIF, x)
#define DMA_LISR_TEIF(x)  DMA_SFLAG(DMA_ISR_TEIF, x)
#define DMA_LISR_DMEIF(x) DMA_SFLAG(DMA_ISR_DMEIF, x)
#define DMA_LISR_LTCIF(x) DMA_SFLAG(DMA_ISR_LTCIF, x)
#define DMA_LISR_FEIF(x)  DMA_SFLAG(DMA_ISR_FEIF, x)

#define DMA_HISR_TCIF(x)  DMA_SFLAG(DMA_ISR_TCIF, x)
#define DMA_HISR_HTIF(x)  DMA_SFLAG(DMA_ISR_HTIF, x)
#define DMA_HISR_TEIF(x)  DMA_SFLAG(DMA_ISR_TEIF, x)
#define DMA_HISR_DMEIF(x) DMA_SFLAG(DMA_ISR_DMEIF, x)
#define DMA_HISR_LTCIF(x) DMA_SFLAG(DMA_ISR_LTCIF, x)
#define DMA_HISR_FEIF(x)  DMA_SFLAG(DMA_ISR_FEIF, x)

#define DMA_LIFCR_CTCIF(x)  DMA_SFLAG(DMA_IFCR_CTCIF, x)
#define DMA_LIFCR_CHTIF(x)  DMA_SFLAG(DMA_IFCR_CHTIF, x)
#define DMA_LIFCR_CTEIF(x)  DMA_SFLAG(DMA_IFCR_CTEIF, x)
#define DMA_LIFCR_CDMEIF(x) DMA_SFLAG(DMA_IFCR_CDMEIF, x)
#define DMA_LIFCR_CLTCIF(x) DMA_SFLAG(DMA_IFCR_CLTCIF, x)
#define DMA_LIFCR_CFEIF(x)  DMA_SFLAG(DMA_IFCR_CFEIF, x)

#define DMA_HIFCR_CTCIF(x)  DMA_SFLAG(DMA_IFCR_CTCIF, x)
#define DMA_HIFCR_CHTIF(x)  DMA_SFLAG(DMA_IFCR_CHTIF, x)
#define DMA_HIFCR_CTEIF(x)  DMA_SFLAG(DMA_IFCR_CTEIF, x)
#define DMA_HIFCR_CDMEIF(x) DMA_SFLAG(DMA_IFCR_CDMEIF, x)
#define DMA_HIFCR_CLTCIF(x) DMA_SFLAG(DMA_IFCR_CLTCIF, x)
#define DMA_HIFCR_CFEIF(x)  DMA_SFLAG(DMA_IFCR_CFEIF, x)

/********************  Bits definition for DMA_LISR register  *****************/
#define DMA_LISR_TCIF3  DMA_LISR_TCIF(3)
#define DMA_LISR_HTIF3  DMA_LISR_HTIF(3)
#define DMA_LISR_TEIF3  DMA_LISR_TEIF(3)
#define DMA_LISR_DMEIF3 DMA_LISR_DMEIF(3)
#define DMA_LISR_FEIF3  DMA_LISR_FEIF(3)
#define DMA_LISR_TCIF2  DMA_LISR_TCIF(2)
#define DMA_LISR_HTIF2  DMA_LISR_HTIF(2)
#define DMA_LISR_TEIF2  DMA_LISR_TEIF(2)
#define DMA_LISR_DMEIF2 DMA_LISR_DMEIF(2)
#define DMA_LISR_FEIF2  DMA_LISR_FEIF(2)
#define DMA_LISR_TCIF1  DMA_LISR_TCIF(1)
#define DMA_LISR_HTIF1  DMA_LISR_HTIF(1)
#define DMA_LISR_TEIF1  DMA_LISR_TEIF(1)
#define DMA_LISR_DMEIF1 DMA_LISR_DMEIF(1)
#define DMA_LISR_FEIF1  DMA_LISR_FEIF(1)
#define DMA_LISR_TCIF0  DMA_LISR_TCIF(0)
#define DMA_LISR_HTIF0  DMA_LISR_HTIF(0)
#define DMA_LISR_TEIF0  DMA_LISR_TEIF(0)
#define DMA_LISR_DMEIF0 DMA_LISR_DMEIF(0)
#define DMA_LISR_FEIF0  DMA_LISR_FEIF(0)

/********************  Bits definition for DMA_HISR register  *****************/
#define DMA_HISR_TCIF7  DMA_HISR_TCIF(7)
#define DMA_HISR_HTIF7  DMA_HISR_HTIF(7)
#define DMA_HISR_TEIF7  DMA_HISR_TEIF(7)
#define DMA_HISR_DMEIF7 DMA_HISR_DMEIF(7)
#define DMA_HISR_FEIF7  DMA_HISR_FEIF(7)
#define DMA_HISR_TCIF6  DMA_HISR_TCIF(6)
#define DMA_HISR_HTIF6  DMA_HISR_HTIF(6)
#define DMA_HISR_TEIF6  DMA_HISR_TEIF(6)
#define DMA_HISR_DMEIF6 DMA_HISR_DMEIF(6)
#define DMA_HISR_FEIF6  DMA_HISR_FEIF(6)
#define DMA_HISR_TCIF5  DMA_HISR_TCIF(5)
#define DMA_HISR_HTIF5  DMA_HISR_HTIF(5)
#define DMA_HISR_TEIF5  DMA_HISR_TEIF(5)
#define DMA_HISR_DMEIF5 DMA_HISR_DMEIF(5)
#define DMA_HISR_FEIF5  DMA_HISR_FEIF(5)
#define DMA_HISR_TCIF4  DMA_HISR_TCIF(4)
#define DMA_HISR_HTIF4  DMA_HISR_HTIF(4)
#define DMA_HISR_TEIF4  DMA_HISR_TEIF(4)
#define DMA_HISR_DMEIF4 DMA_HISR_DMEIF(4)
#define DMA_HISR_FEIF4  DMA_HISR_FEIF(4)

/********************  Bits definition for DMA_LIFCR register  ****************/
#define DMA_LIFCR_CTCIF3  DMA_LIFCR_CTCIF(3)
#define DMA_LIFCR_CHTIF3  DMA_LIFCR_CHTIF(3)
#define DMA_LIFCR_CTEIF3  DMA_LIFCR_CTEIF(3)
#define DMA_LIFCR_CDMEIF3 DMA_LIFCR_CDMEIF(3)
#define DMA_LIFCR_CFEIF3  DMA_LIFCR_CFEIF(3)
#define DMA_LIFCR_CTCIF2  DMA_LIFCR_CTCIF(2)
#define DMA_LIFCR_CHTIF2  DMA_LIFCR_CHTIF(2)
#define DMA_LIFCR_CTEIF2  DMA_LIFCR_CTEIF(2)
#define DMA_LIFCR_CDMEIF2 DMA_LIFCR_CDMEIF(2)
#define DMA_LIFCR_CFEIF2  DMA_LIFCR_CFEIF(2)
#define DMA_LIFCR_CTCIF1  DMA_LIFCR_CTCIF(1)
#define DMA_LIFCR_CHTIF1  DMA_LIFCR_CHTIF(1)
#define DMA_LIFCR_CTEIF1  DMA_LIFCR_CTEIF(1)
#define DMA_LIFCR_CDMEIF1 DMA_LIFCR_CDMEIF(1)
#define DMA_LIFCR_CFEIF1  DMA_LIFCR_CFEIF(1)
#define DMA_LIFCR_CTCIF0  DMA_LIFCR_CTCIF(0)
#define DMA_LIFCR_CHTIF0  DMA_LIFCR_CHTIF(0)
#define DMA_LIFCR_CTEIF0  DMA_LIFCR_CTEIF(0)
#define DMA_LIFCR_CDMEIF0 DMA_LIFCR_CDMEIF(0)
#define DMA_LIFCR_CFEIF0  DMA_LIFCR_CFEIF(0)

/********************  Bits definition for DMA_HIFCR  register  ****************/
#define DMA_HIFCR_CTCIF7  DMA_HIFCR_CTCIF(7)
#define DMA_HIFCR_CHTIF7  DMA_HIFCR_CHTIF(7)
#define DMA_HIFCR_CTEIF7  DMA_HIFCR_CTEIF(7)
#define DMA_HIFCR_CDMEIF7 DMA_HIFCR_CDMEIF(7)
#define DMA_HIFCR_CFEIF7  DMA_HIFCR_CFEIF(7)
#define DMA_HIFCR_CTCIF6  DMA_HIFCR_CTCIF(6)
#define DMA_HIFCR_CHTIF6  DMA_HIFCR_CHTIF(6)
#define DMA_HIFCR_CTEIF6  DMA_HIFCR_CTEIF(6)
#define DMA_HIFCR_CDMEIF6 DMA_HIFCR_CDMEIF(6)
#define DMA_HIFCR_CFEIF6  DMA_HIFCR_CFEIF(6)
#define DMA_HIFCR_CTCIF5  DMA_HIFCR_CTCIF(5)
#define DMA_HIFCR_CHTIF5  DMA_HIFCR_CHTIF(5)
#define DMA_HIFCR_CTEIF5  DMA_HIFCR_CTEIF(5)
#define DMA_HIFCR_CDMEIF5 DMA_HIFCR_CDMEIF(5)
#define DMA_HIFCR_CFEIF5  DMA_HIFCR_CFEIF(5)
#define DMA_HIFCR_CTCIF4  DMA_HIFCR_CTCIF(4)
#define DMA_HIFCR_CHTIF4  DMA_HIFCR_CHTIF(4)
#define DMA_HIFCR_CTEIF4  DMA_HIFCR_CTEIF(4)
#define DMA_HIFCR_CDMEIF4 DMA_HIFCR_CDMEIF(4)
#define DMA_HIFCR_CFEIF4  DMA_HIFCR_CFEIF(4)

/* Exported constants --------------------------------------------------------*/

/* Masks Definition */
#define DMA_BASE_MASK (0xFFFFFF00U)

/** @defgroup DMA_Exported_Constants
 * @{
 */

#define IS_DMA_ALL_CONTROLLER(CONTROLLER) (PeripheralFind((void*)(CONTROLLER), PeripheralDMAStart, PeripheralDMAEnd) != PeripheralNull)

#define IS_DMA_ALL_PERIPH(PERIPH) (IS_DMA_ALL_CONTROLLER((uint32_t)PERIPH & DMA_BASE_MASK) && ((uint32_t)PERIPH & 0xFF - 0x10) % 0x18 == 0)

/** @defgroup DMA_channel
 * @{
 */
#define DMA_Channel_0 ((uint32_t)0x00000000)
#define DMA_Channel_1 ((uint32_t)0x02000000)
#define DMA_Channel_2 ((uint32_t)0x04000000)
#define DMA_Channel_3 ((uint32_t)0x06000000)
#define DMA_Channel_4 ((uint32_t)0x08000000)
#define DMA_Channel_5 ((uint32_t)0x0A000000)
#define DMA_Channel_6 ((uint32_t)0x0C000000)
#define DMA_Channel_7 ((uint32_t)0x0E000000)

#define IS_DMA_CHANNEL(CHANNEL)                                                                                                      \
    (((CHANNEL) == DMA_Channel_0) || ((CHANNEL) == DMA_Channel_1) || ((CHANNEL) == DMA_Channel_2) || ((CHANNEL) == DMA_Channel_3) || \
     ((CHANNEL) == DMA_Channel_4) || ((CHANNEL) == DMA_Channel_5) || ((CHANNEL) == DMA_Channel_6) || ((CHANNEL) == DMA_Channel_7))
/**
 * @}
 */

/** @defgroup DMA_data_transfer_direction
 * @{
 */
#define DMA_DIR_PeripheralToMemory ((uint32_t)0x00000000)
#define DMA_DIR_MemoryToPeripheral ((uint32_t)0x00000040)
#define DMA_DIR_MemoryToMemory     ((uint32_t)0x00000080)

#define IS_DMA_DIRECTION(DIRECTION) \
    (((DIRECTION) == DMA_DIR_PeripheralToMemory) || ((DIRECTION) == DMA_DIR_MemoryToPeripheral) || ((DIRECTION) == DMA_DIR_MemoryToMemory))
/**
 * @}
 */

/** @defgroup DMA_data_buffer_size
 * @{
 */
#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) <= DMA_SxNDTR_LIMIT))
/**
 * @}
 */

/** @defgroup DMA_peripheral_incremented_mode
 * @{
 */
#define DMA_PeripheralInc_Enable  ((uint32_t)0x00000200)
#define DMA_PeripheralInc_Disable ((uint32_t)0x00000000)

#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || ((STATE) == DMA_PeripheralInc_Disable))
/**
 * @}
 */

/** @defgroup DMA_memory_incremented_mode
 * @{
 */
#define DMA_MemoryInc_Enable  ((uint32_t)0x00000400)
#define DMA_MemoryInc_Disable ((uint32_t)0x00000000)

#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || ((STATE) == DMA_MemoryInc_Disable))
/**
 * @}
 */

/** @defgroup DMA_peripheral_data_size
 * @{
 */
#define DMA_PeripheralDataSize_Byte     ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord ((uint32_t)0x00000800)
#define DMA_PeripheralDataSize_Word     ((uint32_t)0x00001000)

#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) \
    (((SIZE) == DMA_PeripheralDataSize_Byte) || ((SIZE) == DMA_PeripheralDataSize_HalfWord) || ((SIZE) == DMA_PeripheralDataSize_Word))
/**
 * @}
 */

/** @defgroup DMA_memory_data_size
 * @{
 */
#define DMA_MemoryDataSize_Byte     ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord ((uint32_t)0x00002000)
#define DMA_MemoryDataSize_Word     ((uint32_t)0x00004000)

#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || ((SIZE) == DMA_MemoryDataSize_HalfWord) || ((SIZE) == DMA_MemoryDataSize_Word))
/**
 * @}
 */

/** @defgroup DMA_mode
 * @{
 */
#define DMA_Mode_Normal   0x00000000UL
#define DMA_Mode_Circular DMA_SxCR_CIRC
#if FEATURE_DMA_SUPPORT_LINK
#define DMA_Mode_Link (DMA_SxCR_LINK)
#endif
#define IS_DMA_MODE(MODE) ((MODE & (DMA_SxCR_CIRC | DMA_SxCR_LINK)) == 0)
/**
 * @}
 */

/** @defgroup DMA_priority_level
 * @{
 */
#define DMA_Priority_Low      ((uint32_t)0x00000000)
#define DMA_Priority_Medium   ((uint32_t)0x00010000)
#define DMA_Priority_High     ((uint32_t)0x00020000)
#define DMA_Priority_VeryHigh ((uint32_t)0x00030000)

#define IS_DMA_PRIORITY(PRIORITY) \
    (((PRIORITY) == DMA_Priority_Low) || ((PRIORITY) == DMA_Priority_Medium) || ((PRIORITY) == DMA_Priority_High) || ((PRIORITY) == DMA_Priority_VeryHigh))
/**
 * @}
 */

/** @defgroup DMA_fifo_direct_mode
 * @{
 */
#define DMA_FIFOMode_Disable ((uint32_t)0x00000000)
#define DMA_FIFOMode_Enable  ((uint32_t)0x00000004)

#define IS_DMA_FIFO_MODE_STATE(STATE) (((STATE) == DMA_FIFOMode_Disable) || ((STATE) == DMA_FIFOMode_Enable))
/**
 * @}
 */

/** @defgroup DMA_fifo_threshold_level
 * @{
 */
#define DMA_FIFOThreshold_1QuarterFull  ((uint32_t)0x00000000)
#define DMA_FIFOThreshold_HalfFull      ((uint32_t)0x00000001)
#define DMA_FIFOThreshold_3QuartersFull ((uint32_t)0x00000002)
#define DMA_FIFOThreshold_Full          ((uint32_t)0x00000003)

#define IS_DMA_FIFO_THRESHOLD(THRESHOLD)                                                                                                                   \
    (((THRESHOLD) == DMA_FIFOThreshold_1QuarterFull) || ((THRESHOLD) == DMA_FIFOThreshold_HalfFull) || ((THRESHOLD) == DMA_FIFOThreshold_3QuartersFull) || \
     ((THRESHOLD) == DMA_FIFOThreshold_Full))
/**
 * @}
 */

/** @defgroup DMA_memory_burst
 * @{
 */
#define DMA_MemoryBurst_Single (0 << MBURST_OFFSET)
#define DMA_MemoryBurst_INC4   (1 << MBURST_OFFSET)
#define DMA_MemoryBurst_INC8   (2 << MBURST_OFFSET)
#define DMA_MemoryBurst_INC16  (3 << MBURST_OFFSET)
#if FEATURE_DMA_EXPAND_RANGE
#define DMA_MemoryBurst_INC32  (4 << MBURST_OFFSET)
#define DMA_MemoryBurst_INC64  (5 << MBURST_OFFSET)
#define DMA_MemoryBurst_INC128 (6 << MBURST_OFFSET)
#endif

#define IS_DMA_MEMORY_BURST(BURST) ((BURST >> MBURST_OFFSET) < BITM(MBURST_WIDTH))
/**
 * @}
 */

/** @defgroup DMA_peripheral_burst
 * @{
 */
#define DMA_PeripheralBurst_Single (0 << PBURST_OFFSET)
#define DMA_PeripheralBurst_INC4   (1 << PBURST_OFFSET)
#define DMA_PeripheralBurst_INC8   (2 << PBURST_OFFSET)
#define DMA_PeripheralBurst_INC16  (3 << PBURST_OFFSET)
#if FEATURE_DMA_EXPAND_RANGE
#define DMA_PeripheralBurst_INC32  (4 << PBURST_OFFSET)
#define DMA_PeripheralBurst_INC64  (5 << PBURST_OFFSET)
#define DMA_PeripheralBurst_INC128 (6 << PBURST_OFFSET)
#endif

#define IS_DMA_PERIPHERAL_BURST(BURST) ((BURST >> PBURST_OFFSET) < BITM(PBURST_WIDTH))
/**
 * @}
 */

/** @defgroup DMA_fifo_status_level
 * @{
 */
#define DMA_FIFOStatus_Less1QuarterFull ((uint32_t)0x00000000 << 3)
#define DMA_FIFOStatus_1QuarterFull     ((uint32_t)0x00000001 << 3)
#define DMA_FIFOStatus_HalfFull         ((uint32_t)0x00000002 << 3)
#define DMA_FIFOStatus_3QuartersFull    ((uint32_t)0x00000003 << 3)
#define DMA_FIFOStatus_Empty            ((uint32_t)0x00000004 << 3)
#define DMA_FIFOStatus_Full             ((uint32_t)0x00000005 << 3)

#define IS_DMA_FIFO_STATUS(STATUS)                                                                                                          \
    (((STATUS) == DMA_FIFOStatus_Less1QuarterFull) || ((STATUS) == DMA_FIFOStatus_HalfFull) || ((STATUS) == DMA_FIFOStatus_1QuarterFull) || \
     ((STATUS) == DMA_FIFOStatus_3QuartersFull) || ((STATUS) == DMA_FIFOStatus_Full) || ((STATUS) == DMA_FIFOStatus_Empty))
/**
 * @}
 */

/** @defgroup DMA_flags_definition
 * @{
 */
#define DMA_FLAG_TCIF  DMA_ISR_TCIF
#define DMA_FLAG_HTIF  DMA_ISR_HTIF
#define DMA_FLAG_TEIF  DMA_ISR_TEIF
#define DMA_FLAG_DMEIF DMA_ISR_DMEIF
#define DMA_FLAG_LTCIF DMA_ISR_LTCIF
#define DMA_FLAG_FEIF  DMA_ISR_FEIF

/* Legacy definitions, should never be used! */
#define DMA_FLAG_TCIF0  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF0  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF0  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF0 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF0 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF0  DMA_FLAG_FEIF

#define DMA_FLAG_TCIF1  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF1  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF1  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF1 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF1 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF1  DMA_FLAG_FEIF

#define DMA_FLAG_TCIF2  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF2  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF2  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF2 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF2 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF2  DMA_FLAG_FEIF

#define DMA_FLAG_TCIF3  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF3  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF3  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF3 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF3 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF3  DMA_FLAG_FEIF

#define DMA_FLAG_TCIF4  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF4  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF4  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF4 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF4 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF4  DMA_FLAG_FEIF

#define DMA_FLAG_TCIF5  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF5  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF5  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF5 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF5 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF5  DMA_FLAG_FEIF

#define DMA_FLAG_TCIF6  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF6  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF6  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF6 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF6 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF6  DMA_FLAG_FEIF

#define DMA_FLAG_TCIF7  DMA_FLAG_TCIF
#define DMA_FLAG_HTIF7  DMA_FLAG_HTIF
#define DMA_FLAG_TEIF7  DMA_FLAG_TEIF
#define DMA_FLAG_DMEIF7 DMA_FLAG_DMEIF
#define DMA_FLAG_LTCIF7 DMA_FLAG_LTCIF
#define DMA_FLAG_FEIF7  DMA_FLAG_FEIF

#define DMA_FLAG_ALL (DMA_FLAG_FEIF | DMA_FLAG_LTCIF | DMA_FLAG_DMEIF | DMA_FLAG_TEIF | DMA_FLAG_HTIF | DMA_FLAG_TCIF)

#define IS_DMA_CLEAR_FLAG(FLAG) ((FLAG & DMA_FLAG_ALL) != 0)

#define IS_DMA_GET_FLAG(FLAG)                                                                                                                           \
    (((FLAG) == DMA_FLAG_FEIF) || ((FLAG) == DMA_FLAG_LTCIF) || ((FLAG) == DMA_FLAG_DMEIF) || ((FLAG) == DMA_FLAG_TEIF) || ((FLAG) == DMA_FLAG_HTIF) || \
     ((FLAG) == DMA_FLAG_TCIF))

/**
 * @}
 */

/** @defgroup DMA_interrupt_enable_definitions
 * @{
 */
#define DMA_IT_TC  DMA_SxCR_TCIE
#define DMA_IT_HT  DMA_SxCR_HTIE
#define DMA_IT_TE  DMA_SxCR_TEIE
#define DMA_IT_DME DMA_SxCR_DMEIE
#define DMA_IT_LTC DMA_SxCR_LTCIE
#define DMA_IT_FE  DMA_SxFCR_FEIE

#define DMA_TRANSFER_IT_ALL (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE | DMA_IT_DME | DMA_IT_LTC)
#define DMA_IT_ALL          (DMA_TRANSFER_IT_ALL | DMA_IT_FE)

#define IS_DMA_CONFIG_IT(IT) ((IT & DMA_IT_ALL) != 0)
/**
 * @}
 */

/** @defgroup DMA_interrupts_definitions
 * @{
 */
#define DMA_IT_TCIF  DMA_FLAG_TCIF
#define DMA_IT_HTIF  DMA_FLAG_HTIF
#define DMA_IT_TEIF  DMA_FLAG_TEIF
#define DMA_IT_DMEIF DMA_FLAG_DMEIF
#define DMA_IT_LTCIF DMA_FLAG_LTCIF
#define DMA_IT_FEIF  DMA_FLAG_FEIF

/* Legacy definitions, should never be used! */
#define DMA_IT_TCIF0  DMA_IT_TCIF
#define DMA_IT_HTIF0  DMA_IT_HTIF
#define DMA_IT_TEIF0  DMA_IT_TEIF
#define DMA_IT_DMEIF0 DMA_IT_DMEIF
#define DMA_IT_LTCIF0 DMA_IT_LTCIF
#define DMA_IT_FEIF0  DMA_IT_FEIF

#define DMA_IT_TCIF1  DMA_IT_TCIF
#define DMA_IT_HTIF1  DMA_IT_HTIF
#define DMA_IT_TEIF1  DMA_IT_TEIF
#define DMA_IT_DMEIF1 DMA_IT_DMEIF
#define DMA_IT_LTCIF1 DMA_IT_LTCIF
#define DMA_IT_FEIF1  DMA_IT_FEIF

#define DMA_IT_TCIF2  DMA_IT_TCIF
#define DMA_IT_HTIF2  DMA_IT_HTIF
#define DMA_IT_TEIF2  DMA_IT_TEIF
#define DMA_IT_DMEIF2 DMA_IT_DMEIF
#define DMA_IT_LTCIF2 DMA_IT_LTCIF
#define DMA_IT_FEIF2  DMA_IT_FEIF

#define DMA_IT_TCIF3  DMA_IT_TCIF
#define DMA_IT_HTIF3  DMA_IT_HTIF
#define DMA_IT_TEIF3  DMA_IT_TEIF
#define DMA_IT_DMEIF3 DMA_IT_DMEIF
#define DMA_IT_LTCIF3 DMA_IT_LTCIF
#define DMA_IT_FEIF3  DMA_IT_FEIF

#define DMA_IT_TCIF4  DMA_IT_TCIF
#define DMA_IT_HTIF4  DMA_IT_HTIF
#define DMA_IT_TEIF4  DMA_IT_TEIF
#define DMA_IT_DMEIF4 DMA_IT_DMEIF
#define DMA_IT_LTCIF4 DMA_IT_LTCIF
#define DMA_IT_FEIF4  DMA_IT_FEIF

#define DMA_IT_TCIF5  DMA_IT_TCIF
#define DMA_IT_HTIF5  DMA_IT_HTIF
#define DMA_IT_TEIF5  DMA_IT_TEIF
#define DMA_IT_DMEIF5 DMA_IT_DMEIF
#define DMA_IT_LTCIF5 DMA_IT_LTCIF
#define DMA_IT_FEIF5  DMA_IT_FEIF

#define DMA_IT_TCIF6  DMA_IT_TCIF
#define DMA_IT_HTIF6  DMA_IT_HTIF
#define DMA_IT_TEIF6  DMA_IT_TEIF
#define DMA_IT_DMEIF6 DMA_IT_DMEIF
#define DMA_IT_LTCIF6 DMA_IT_LTCIF
#define DMA_IT_FEIF6  DMA_IT_FEIF

#define DMA_IT_TCIF7  DMA_IT_TCIF
#define DMA_IT_HTIF7  DMA_IT_HTIF
#define DMA_IT_TEIF7  DMA_IT_TEIF
#define DMA_IT_DMEIF7 DMA_IT_DMEIF
#define DMA_IT_LTCIF7 DMA_IT_LTCIF
#define DMA_IT_FEIF7  DMA_IT_FEIF

#define IS_DMA_CLEAR_IT(IT) IS_DMA_CLEAR_FLAG(IT)

#define IS_DMA_GET_IT(IT) IS_DMA_GET_FLAG(IT)
/**
 * @}
 */

/** @defgroup DMA_peripheral_increment_offset
 * @{
 */
#define DMA_PINCOS_Psize       ((uint32_t)0x00000000)
#define DMA_PINCOS_WordAligned ((uint32_t)0x00008000)

#define IS_DMA_PINCOS_SIZE(SIZE) (((SIZE) == DMA_PINCOS_Psize) || ((SIZE) == DMA_PINCOS_WordAligned))
/**
 * @}
 */

/** @defgroup DMA_flow_controller_definitions
 * @{
 */
#define DMA_FlowCtrl_Memory     ((uint32_t)0x00000000)
#define DMA_FlowCtrl_Peripheral ((uint32_t)0x00000020)

#define IS_DMA_FLOW_CTRL(CTRL) (((CTRL) == DMA_FlowCtrl_Memory) || ((CTRL) == DMA_FlowCtrl_Peripheral))
/**
 * @}
 */

/** @defgroup DMA_memory_targets_definitions
 * @{
 */
#define DMA_Memory_0 ((uint32_t)0x00000000)
#define DMA_Memory_1 ((uint32_t)0x00080000)

#define IS_DMA_CURRENT_MEM(MEM) (((MEM) == DMA_Memory_0) || ((MEM) == DMA_Memory_1))
/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the DMA configuration to the default reset state *****/
void DMA_DeInit(DMA_Stream_TypeDef* DMAy_Streamx);

/* Initialization and Configuration functions *********************************/
void DMA_Init(DMA_Stream_TypeDef* DMAy_Streamx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
void DMA_Cmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState);

/* Optional Configuration functions *******************************************/
void DMA_PeriphIncOffsetSizeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_Pincos);
void DMA_FlowControllerConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FlowCtrl);

/* Data Counter functions *****************************************************/
void     DMA_SetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx, uint16_t Counter);
uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx);

/* Double Buffer mode functions ***********************************************/
void     DMA_DoubleBufferModeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t Memory1BaseAddr, uint32_t DMA_CurrentMemory);
void     DMA_DoubleBufferModeCmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState);
void     DMA_MemoryTargetConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t MemoryBaseAddr, uint32_t DMA_MemoryTarget);
uint32_t DMA_GetCurrentMemoryTarget(DMA_Stream_TypeDef* DMAy_Streamx);

/* Interrupts and flags management functions **********************************/
FunctionalState DMA_GetCmdStatus(DMA_Stream_TypeDef* DMAy_Streamx);
uint32_t        DMA_GetFIFOStatus(DMA_Stream_TypeDef* DMAy_Streamx);
FlagStatus      DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG);
void            DMA_ClearFlag(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG);
void            DMA_ITConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT, FunctionalState NewState);
ITStatus        DMA_GetITStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT);
void            DMA_ClearITPendingBit(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_DMA_H__
