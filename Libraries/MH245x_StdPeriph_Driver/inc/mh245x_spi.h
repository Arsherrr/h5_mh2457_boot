// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_SPI_H__
#define __PERIPH_SPI_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint16_t CR1;       /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
    uint16_t      RESERVED0; /*!< Reserved, 0x02                                                           */
    __IO uint16_t CR2;       /*!< SPI control register 2,                             Address offset: 0x04 */
    uint16_t      RESERVED1; /*!< Reserved, 0x06                                                           */
    __IO uint16_t SR;        /*!< SPI status register,                                Address offset: 0x08 */
    uint16_t      RESERVED2; /*!< Reserved, 0x0A                                                           */
    __IO uint16_t DR;        /*!< SPI data register,                                  Address offset: 0x0C */
    uint16_t      RESERVED3; /*!< Reserved, 0x0E                                                           */
    __IO uint16_t CRCPR;     /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
    uint16_t      RESERVED4; /*!< Reserved, 0x12                                                           */
    __IO uint16_t RXCRCR;    /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
    uint16_t      RESERVED5; /*!< Reserved, 0x16                                                           */
    __IO uint16_t TXCRCR;    /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
    uint16_t      RESERVED6; /*!< Reserved, 0x1A                                                           */
    __IO uint16_t I2SCFGR;   /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
    uint16_t      RESERVED7; /*!< Reserved, 0x1E                                                           */
    __IO uint16_t I2SPR;     /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
    uint16_t      RESERVED8; /*!< Reserved, 0x22                                                           */
	uint32_t 	  RESERVED[7];
	__IO uint32_t CR3;		 /*!< SPI control register 3 (not used in I2S mode),      Address offset: 0x40 */
	__IO uint32_t SR2;		 /*!< SPI status register,                                Address offset: 0x44 */
	__IO uint32_t FIFOCLRR;  /*!< SPI FIFO clear register,                         	  Address offset: 0x44 */
	__IO uint32_t EXTR;	
} SPI_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/*******************  Bit definition for SPI_CR1 register  ********************/
#define SPI_CR1_CPHA ((uint16_t)0x0001) /*!<Clock Phase      */
#define SPI_CR1_CPOL ((uint16_t)0x0002) /*!<Clock Polarity   */
#define SPI_CR1_MSTR ((uint16_t)0x0004) /*!<Master Selection */

#define SPI_CR1_BR   ((uint16_t)0x0038) /*!<BR[2:0] bits (Baud Rate Control) */
#define SPI_CR1_BR_0 ((uint16_t)0x0008) /*!<Bit 0 */
#define SPI_CR1_BR_1 ((uint16_t)0x0010) /*!<Bit 1 */
#define SPI_CR1_BR_2 ((uint16_t)0x0020) /*!<Bit 2 */

#define SPI_CR1_SPE      ((uint16_t)0x0040) /*!<SPI Enable                          */
#define SPI_CR1_LSBFIRST ((uint16_t)0x0080) /*!<Frame Format                        */
#define SPI_CR1_SSI      ((uint16_t)0x0100) /*!<Internal slave select               */
#define SPI_CR1_SSM      ((uint16_t)0x0200) /*!<Software slave management           */
#define SPI_CR1_RXONLY   ((uint16_t)0x0400) /*!<Receive only                        */
#define SPI_CR1_DFF      ((uint16_t)0x0800) /*!<Data Frame Format                   */
#define SPI_CR1_CRCNEXT  ((uint16_t)0x1000) /*!<Transmit CRC next                   */
#define SPI_CR1_CRCEN    ((uint16_t)0x2000) /*!<Hardware CRC calculation enable     */
#define SPI_CR1_BIDIOE   ((uint16_t)0x4000) /*!<Output enable in bidirectional mode */
#define SPI_CR1_BIDIMODE ((uint16_t)0x8000) /*!<Bidirectional data mode enable      */

/*******************  Bit definition for SPI_CR2 register  ********************/
#define SPI_CR2_RXDMAEN ((uint8_t)0x01) /*!<Rx Buffer DMA Enable                 */
#define SPI_CR2_TXDMAEN ((uint8_t)0x02) /*!<Tx Buffer DMA Enable                 */
#define SPI_CR2_SSOE    ((uint8_t)0x04) /*!<SS Output Enable                     */
#define SPI_CR2_ERRIE   ((uint8_t)0x20) /*!<Error Interrupt Enable               */
#define SPI_CR2_RXNEIE  ((uint8_t)0x40) /*!<RX buffer Not Empty Interrupt Enable */
#define SPI_CR2_TXEIE   ((uint8_t)0x80) /*!<Tx buffer Empty Interrupt Enable     */

/*******************  Bit definition for SPI_CR3 register  ********************/
#define SPI_CR3_RXFIFOEN 				((uint32_t)0x01) /*!<Rx FIFO  Enable                 */
#define SPI_CR3_TXFIFOEN 				((uint32_t)0x02) /*!<Tx FIFO  Enable                 */
#define SPI_CR3_RXFIFO_PFULLIE			((uint32_t)0x04)
#define SPI_CR3_TXFIFO_PFEMPIE			((uint32_t)0x08)
#define SPI_CR3_RXFIFO_FULLIE			((uint32_t)0x10)
#define SPI_CR3_TXFIFO_FULLIE			((uint32_t)0x20)
#define SPI_EXTR_ASYNCEN				((uint32_t)0x04)
#define SPI_EXTR_ASYNC_SAMPLE_DELAY		((uint32_t)0x02)
#define SPI_EXTR_RX_SAMPLE_DEALY		((uint32_t)0x01)

/********************  Bit definition for SPI_SR register  ********************/
#define SPI_SR_RXNE   ((uint8_t)0x01) /*!<Receive buffer Not Empty */
#define SPI_SR_TXE    ((uint8_t)0x02) /*!<Transmit buffer Empty    */
#define SPI_SR_CHSIDE ((uint8_t)0x04) /*!<Channel side             */
#define SPI_SR_UDR    ((uint8_t)0x08) /*!<Underrun flag            */
#define SPI_SR_CRCERR ((uint8_t)0x10) /*!<CRC Error flag           */
#define SPI_SR_MODF   ((uint8_t)0x20) /*!<Mode fault               */
#define SPI_SR_OVR    ((uint8_t)0x40) /*!<Overrun flag             */
#define SPI_SR_BSY    ((uint8_t)0x80) /*!<Busy flag                */

/********************  Bit definition for SPI_DR register  ********************/
#define SPI_DR_DR ((uint16_t)0xFFFF) /*!<Data Register           */

/*******************  Bit definition for SPI_CRCPR register  ******************/
#define SPI_CRCPR_CRCPOLY ((uint16_t)0xFFFF) /*!<CRC polynomial register */

/******************  Bit definition for SPI_RXCRCR register  ******************/
#define SPI_RXCRCR_RXCRC ((uint16_t)0xFFFF) /*!<Rx CRC Register         */

/******************  Bit definition for SPI_TXCRCR register  ******************/
#define SPI_TXCRCR_TXCRC ((uint16_t)0xFFFF) /*!<Tx CRC Register         */

/******************  Bit definition for SPI_I2SCFGR register  *****************/
#define SPI_I2SCFGR_CHLEN ((uint16_t)0x0001) /*!<Channel length (number of bits per audio channel) */

#define SPI_I2SCFGR_DATLEN   ((uint16_t)0x0006) /*!<DATLEN[1:0] bits (Data length to be transferred)  */
#define SPI_I2SCFGR_DATLEN_0 ((uint16_t)0x0002) /*!<Bit 0 */
#define SPI_I2SCFGR_DATLEN_1 ((uint16_t)0x0004) /*!<Bit 1 */

#define SPI_I2SCFGR_CKPOL ((uint16_t)0x0008) /*!<steady state clock polarity               */

#define SPI_I2SCFGR_I2SSTD   ((uint16_t)0x0030) /*!<I2SSTD[1:0] bits (I2S standard selection) */
#define SPI_I2SCFGR_I2SSTD_0 ((uint16_t)0x0010) /*!<Bit 0 */
#define SPI_I2SCFGR_I2SSTD_1 ((uint16_t)0x0020) /*!<Bit 1 */

#define SPI_I2SCFGR_PCMSYNC ((uint16_t)0x0080) /*!<PCM frame synchronization                 */

#define SPI_I2SCFGR_I2SCFG   ((uint16_t)0x0300) /*!<I2SCFG[1:0] bits (I2S configuration mode) */
#define SPI_I2SCFGR_I2SCFG_0 ((uint16_t)0x0100) /*!<Bit 0 */
#define SPI_I2SCFGR_I2SCFG_1 ((uint16_t)0x0200) /*!<Bit 1 */

#define SPI_I2SCFGR_I2SE   ((uint16_t)0x0400) /*!<I2S Enable         */
#define SPI_I2SCFGR_I2SMOD ((uint16_t)0x0800) /*!<I2S mode selection */

/******************  Bit definition for SPI_I2SPR register  *******************/
#define SPI_I2SPR_I2SDIV ((uint16_t)0x00FF) /*!<I2S Linear prescaler         */
#define SPI_I2SPR_ODD    ((uint16_t)0x0100) /*!<Odd factor for the prescaler */
#define SPI_I2SPR_MCKOE  ((uint16_t)0x0200) /*!<Master Clock Output Enable   */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  SPI Init structure definition
 */

typedef struct
{
    uint16_t SPI_Direction; /*!< Specifies the SPI unidirectional or bidirectional data mode.
                                 This parameter can be a value of @ref SPI_data_direction */

    uint16_t SPI_Mode; /*!< Specifies the SPI operating mode.
                            This parameter can be a value of @ref SPI_mode */

    uint16_t SPI_DataSize; /*!< Specifies the SPI data size.
                                This parameter can be a value of @ref SPI_data_size */

    uint16_t SPI_CPOL; /*!< Specifies the serial clock steady state.
                            This parameter can be a value of @ref SPI_Clock_Polarity */

    uint16_t SPI_CPHA; /*!< Specifies the clock active edge for the bit capture.
                            This parameter can be a value of @ref SPI_Clock_Phase */

    uint16_t SPI_NSS; /*!< Specifies whether the NSS signal is managed by
                           hardware (NSS pin) or by software using the SSI bit.
                           This parameter can be a value of @ref SPI_Slave_Select_management */

    uint16_t SPI_BaudRatePrescaler; /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_BaudRate_Prescaler
                                         @note The communication clock is derived from the master
                                               clock. The slave clock does not need to be set. */

    uint16_t SPI_FirstBit; /*!< Specifies whether data transfers start from MSB or LSB bit.
                                This parameter can be a value of @ref SPI_MSB_LSB_transmission */

    uint16_t SPI_CRCPolynomial; /*!< Specifies the polynomial used for the CRC calculation. */
} SPI_InitTypeDef;

/**
 * @brief  I2S Init structure definition
 */

typedef struct
{

    uint16_t I2S_Mode; /*!< Specifies the I2S operating mode.
                            This parameter can be a value of @ref I2S_Mode */

    uint16_t I2S_Standard; /*!< Specifies the standard used for the I2S communication.
                                This parameter can be a value of @ref I2S_Standard */

    uint16_t I2S_DataFormat; /*!< Specifies the data format for the I2S communication.
                                  This parameter can be a value of @ref I2S_Data_Format */

    uint16_t I2S_MCLKOutput; /*!< Specifies whether the I2S MCLK output is enabled or not.
                                  This parameter can be a value of @ref I2S_MCLK_Output */

    uint32_t I2S_AudioFreq; /*!< Specifies the frequency selected for the I2S communication.
                                 This parameter can be a value of @ref I2S_Audio_Frequency */

    uint16_t I2S_CPOL; /*!< Specifies the idle state of the I2S clock.
                            This parameter can be a value of @ref I2S_Clock_Polarity */
} I2S_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup SPI_Exported_Constants
 * @{
 */

#define IS_SPI_ALL_PERIPH(PERIPH) \
    (((PERIPH) == SPI1) || ((PERIPH) == SPI2) || ((PERIPH) == SPI3) || ((PERIPH) == SPI4))

#define IS_SPI_ALL_PERIPH_EXT(PERIPH)                                                                                                    \
    (((PERIPH) == SPI1) || ((PERIPH) == SPI2) || ((PERIPH) == SPI3) || ((PERIPH) == SPI4) || \
     ((PERIPH) == I2S2ext) || ((PERIPH) == I2S3ext))

#define IS_SPI_23_PERIPH(PERIPH) (((PERIPH) == SPI2) || ((PERIPH) == SPI3))

#define IS_SPI_23_PERIPH_EXT(PERIPH) (((PERIPH) == SPI2) || ((PERIPH) == SPI3) || ((PERIPH) == I2S2ext) || ((PERIPH) == I2S3ext))

#define IS_I2S_EXT_PERIPH(PERIPH) (((PERIPH) == I2S2ext) || ((PERIPH) == I2S3ext))

/** @defgroup SPI_data_direction
 * @{
 */

#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0000)
#define SPI_Direction_2Lines_RxOnly     ((uint16_t)0x0400)
#define SPI_Direction_1Line_Rx          ((uint16_t)0x8000)
#define SPI_Direction_1Line_Tx          ((uint16_t)0xC000)
#define IS_SPI_DIRECTION_MODE(MODE)                                                                                                  \
    (((MODE) == SPI_Direction_2Lines_FullDuplex) || ((MODE) == SPI_Direction_2Lines_RxOnly) || ((MODE) == SPI_Direction_1Line_Rx) || \
     ((MODE) == SPI_Direction_1Line_Tx))
/**
 * @}
 */

/** @defgroup SPI_mode
 * @{
 */

#define SPI_Mode_Master   ((uint16_t)0x0104)
#define SPI_Mode_Slave    ((uint16_t)0x0000)
#define IS_SPI_MODE(MODE) (((MODE) == SPI_Mode_Master) || ((MODE) == SPI_Mode_Slave))
/**
 * @}
 */

/** @defgroup SPI_data_size
 * @{
 */

#define SPI_DataSize_16b          ((uint16_t)0x0800)
#define SPI_DataSize_8b           ((uint16_t)0x0000)
#define IS_SPI_DATASIZE(DATASIZE) (((DATASIZE) == SPI_DataSize_16b) || ((DATASIZE) == SPI_DataSize_8b))
/**
 * @}
 */

/** @defgroup SPI_Clock_Polarity
 * @{
 */

#define SPI_CPOL_Low      ((uint16_t)0x0000)
#define SPI_CPOL_High     ((uint16_t)0x0002)
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_Low) || ((CPOL) == SPI_CPOL_High))
/**
 * @}
 */

/** @defgroup SPI_Clock_Phase
 * @{
 */

#define SPI_CPHA_1Edge    ((uint16_t)0x0000)
#define SPI_CPHA_2Edge    ((uint16_t)0x0001)
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1Edge) || ((CPHA) == SPI_CPHA_2Edge))
/**
 * @}
 */

/** @defgroup SPI_Slave_Select_management
 * @{
 */

#define SPI_NSS_Soft    ((uint16_t)0x0200)
#define SPI_NSS_Hard    ((uint16_t)0x0000)
#define IS_SPI_NSS(NSS) (((NSS) == SPI_NSS_Soft) || ((NSS) == SPI_NSS_Hard))
/**
 * @}
 */

/** @defgroup SPI_BaudRate_Prescaler
 * @{
 */

#define SPI_BaudRatePrescaler_2   ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4   ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8   ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16  ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32  ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64  ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128 ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256 ((uint16_t)0x0038)
#define IS_SPI_BAUDRATE_PRESCALER(PRESCALER)                                                                                                \
    (((PRESCALER) == SPI_BaudRatePrescaler_2) || ((PRESCALER) == SPI_BaudRatePrescaler_4) || ((PRESCALER) == SPI_BaudRatePrescaler_8) ||    \
     ((PRESCALER) == SPI_BaudRatePrescaler_16) || ((PRESCALER) == SPI_BaudRatePrescaler_32) || ((PRESCALER) == SPI_BaudRatePrescaler_64) || \
     ((PRESCALER) == SPI_BaudRatePrescaler_128) || ((PRESCALER) == SPI_BaudRatePrescaler_256))
/**
 * @}
 */

/** @defgroup SPI_MSB_LSB_transmission
 * @{
 */

#define SPI_FirstBit_MSB      ((uint16_t)0x0000)
#define SPI_FirstBit_LSB      ((uint16_t)0x0080)
#define IS_SPI_FIRST_BIT(BIT) (((BIT) == SPI_FirstBit_MSB) || ((BIT) == SPI_FirstBit_LSB))
/**
 * @}
 */

/** @defgroup SPI_I2S_Mode
 * @{
 */

#define I2S_Mode_SlaveTx  ((uint16_t)0x0000)
#define I2S_Mode_SlaveRx  ((uint16_t)0x0100)
#define I2S_Mode_MasterTx ((uint16_t)0x0200)
#define I2S_Mode_MasterRx ((uint16_t)0x0300)
#define IS_I2S_MODE(MODE) (((MODE) == I2S_Mode_SlaveTx) || ((MODE) == I2S_Mode_SlaveRx) || ((MODE) == I2S_Mode_MasterTx) || ((MODE) == I2S_Mode_MasterRx))
/**
 * @}
 */

/** @defgroup SPI_I2S_Standard
 * @{
 */

#define I2S_Standard_Phillips ((uint16_t)0x0000)
#define I2S_Standard_MSB      ((uint16_t)0x0010)
#define I2S_Standard_LSB      ((uint16_t)0x0020)
#define I2S_Standard_PCMShort ((uint16_t)0x0030)
#define I2S_Standard_PCMLong  ((uint16_t)0x00B0)
#define IS_I2S_STANDARD(STANDARD)                                                                                                                              \
    (((STANDARD) == I2S_Standard_Phillips) || ((STANDARD) == I2S_Standard_MSB) || ((STANDARD) == I2S_Standard_LSB) || ((STANDARD) == I2S_Standard_PCMShort) || \
     ((STANDARD) == I2S_Standard_PCMLong))
/**
 * @}
 */

/** @defgroup SPI_I2S_Data_Format
 * @{
 */

#define I2S_DataFormat_16b         ((uint16_t)0x0000)
#define I2S_DataFormat_16bextended ((uint16_t)0x0001)
#define I2S_DataFormat_24b         ((uint16_t)0x0003)
#define I2S_DataFormat_32b         ((uint16_t)0x0005)
#define IS_I2S_DATA_FORMAT(FORMAT) \
    (((FORMAT) == I2S_DataFormat_16b) || ((FORMAT) == I2S_DataFormat_16bextended) || ((FORMAT) == I2S_DataFormat_24b) || ((FORMAT) == I2S_DataFormat_32b))
/**
 * @}
 */

/** @defgroup SPI_I2S_MCLK_Output
 * @{
 */

#define I2S_MCLKOutput_Enable      ((uint16_t)0x0200)
#define I2S_MCLKOutput_Disable     ((uint16_t)0x0000)
#define IS_I2S_MCLK_OUTPUT(OUTPUT) (((OUTPUT) == I2S_MCLKOutput_Enable) || ((OUTPUT) == I2S_MCLKOutput_Disable))
/**
 * @}
 */

/** @defgroup SPI_I2S_Audio_Frequency
 * @{
 */

#define I2S_AudioFreq_192k    ((uint32_t)192000)
#define I2S_AudioFreq_96k     ((uint32_t)96000)
#define I2S_AudioFreq_48k     ((uint32_t)48000)
#define I2S_AudioFreq_44k     ((uint32_t)44100)
#define I2S_AudioFreq_32k     ((uint32_t)32000)
#define I2S_AudioFreq_22k     ((uint32_t)22050)
#define I2S_AudioFreq_16k     ((uint32_t)16000)
#define I2S_AudioFreq_11k     ((uint32_t)11025)
#define I2S_AudioFreq_8k      ((uint32_t)8000)
#define I2S_AudioFreq_Default ((uint32_t)2)

#define IS_I2S_AUDIO_FREQ(FREQ) ((((FREQ) >= I2S_AudioFreq_8k) && ((FREQ) <= I2S_AudioFreq_192k)) || ((FREQ) == I2S_AudioFreq_Default))
/**
 * @}
 */

/** @defgroup SPI_I2S_Clock_Polarity
 * @{
 */

#define I2S_CPOL_Low      ((uint16_t)0x0000)
#define I2S_CPOL_High     ((uint16_t)0x0008)
#define IS_I2S_CPOL(CPOL) (((CPOL) == I2S_CPOL_Low) || ((CPOL) == I2S_CPOL_High))
/**
 * @}
 */

/** @defgroup SPI_I2S_DMA_transfer_requests
 * @{
 */

#define SPI_I2S_DMAReq_Tx         ((uint16_t)0x0002)
#define SPI_I2S_DMAReq_Rx         ((uint16_t)0x0001)
#define IS_SPI_I2S_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFFFC) == 0x00) && ((DMAREQ) != 0x00))
/**
 * @}
 */

/** @defgroup SPI_NSS_internal_software_management
 * @{
 */

#define SPI_NSSInternalSoft_Set       ((uint16_t)0x0100)
#define SPI_NSSInternalSoft_Reset     ((uint16_t)0xFEFF)
#define IS_SPI_NSS_INTERNAL(INTERNAL) (((INTERNAL) == SPI_NSSInternalSoft_Set) || ((INTERNAL) == SPI_NSSInternalSoft_Reset))
/**
 * @}
 */

/** @defgroup SPI_CRC_Transmit_Receive
 * @{
 */

#define SPI_CRC_Tx      ((uint8_t)0x00)
#define SPI_CRC_Rx      ((uint8_t)0x01)
#define IS_SPI_CRC(CRC) (((CRC) == SPI_CRC_Tx) || ((CRC) == SPI_CRC_Rx))
/**
 * @}
 */

/** @defgroup SPI_direction_transmit_receive
 * @{
 */

#define SPI_Direction_Rx            ((uint16_t)0xBFFF)
#define SPI_Direction_Tx            ((uint16_t)0x4000)
#define IS_SPI_DIRECTION(DIRECTION) (((DIRECTION) == SPI_Direction_Rx) || ((DIRECTION) == SPI_Direction_Tx))
/**
 * @}
 */

/** @defgroup SPI_I2S_interrupts_definition
 * @{
 */

#define SPI_I2S_IT_TXE    ((uint8_t)0x71)
#define SPI_I2S_IT_RXNE   ((uint8_t)0x60)
#define SPI_I2S_IT_ERR    ((uint8_t)0x50)
#define I2S_IT_UDR        ((uint8_t)0x53)
#define SPI_I2S_IT_TIFRFE ((uint8_t)0x58)

#define IS_SPI_I2S_CONFIG_IT(IT) (((IT) == SPI_I2S_IT_TXE) || ((IT) == SPI_I2S_IT_RXNE) || ((IT) == SPI_I2S_IT_ERR))

#define SPI_I2S_IT_OVR ((uint8_t)0x56)
#define SPI_IT_MODF    ((uint8_t)0x55)
#define SPI_IT_CRCERR  ((uint8_t)0x54)

#define IS_SPI_I2S_CLEAR_IT(IT) (((IT) == SPI_IT_CRCERR))

#define IS_SPI_I2S_GET_IT(IT)                                                                                                                 \
    (((IT) == SPI_I2S_IT_RXNE) || ((IT) == SPI_I2S_IT_TXE) || ((IT) == SPI_IT_CRCERR) || ((IT) == SPI_IT_MODF) || ((IT) == SPI_I2S_IT_OVR) || \
     ((IT) == I2S_IT_UDR) || ((IT) == SPI_I2S_IT_TIFRFE))
/**
 * @}
 */

/** @defgroup SPI_I2S_flags_definition
 * @{
 */

#define SPI_I2S_FLAG_RXNE   ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE    ((uint16_t)0x0002)
#define I2S_FLAG_CHSIDE     ((uint16_t)0x0004)
#define I2S_FLAG_UDR        ((uint16_t)0x0008)
#define SPI_FLAG_CRCERR     ((uint16_t)0x0010)
#define SPI_FLAG_MODF       ((uint16_t)0x0020)
#define SPI_I2S_FLAG_OVR    ((uint16_t)0x0040)
#define SPI_I2S_FLAG_BSY    ((uint16_t)0x0080)
#define SPI_I2S_FLAG_TIFRFE ((uint16_t)0x0100)

#define IS_SPI_I2S_CLEAR_FLAG(FLAG) (((FLAG) == SPI_FLAG_CRCERR))
#define IS_SPI_I2S_GET_FLAG(FLAG)                                                                                                                            \
    (((FLAG) == SPI_I2S_FLAG_BSY) || ((FLAG) == SPI_I2S_FLAG_OVR) || ((FLAG) == SPI_FLAG_MODF) || ((FLAG) == SPI_FLAG_CRCERR) || ((FLAG) == I2S_FLAG_UDR) || \
     ((FLAG) == I2S_FLAG_CHSIDE) || ((FLAG) == SPI_I2S_FLAG_TXE) || ((FLAG) == SPI_I2S_FLAG_RXNE) || ((FLAG) == SPI_I2S_FLAG_TIFRFE))
/**
 * @}
 */

/** @defgroup SPI_CRC_polynomial
 * @{
 */

#define IS_SPI_CRC_POLYNOMIAL(POLYNOMIAL) ((POLYNOMIAL) >= 0x1)
/**
 * @}
 */

/** @defgroup SPI_I2S_Legacy
 * @{
 */

#define SPI_DMAReq_Tx         SPI_I2S_DMAReq_Tx
#define SPI_DMAReq_Rx         SPI_I2S_DMAReq_Rx
#define SPI_IT_TXE            SPI_I2S_IT_TXE
#define SPI_IT_RXNE           SPI_I2S_IT_RXNE
#define SPI_IT_ERR            SPI_I2S_IT_ERR
#define SPI_IT_OVR            SPI_I2S_IT_OVR
#define SPI_FLAG_RXNE         SPI_I2S_FLAG_RXNE
#define SPI_FLAG_TXE          SPI_I2S_FLAG_TXE
#define SPI_FLAG_OVR          SPI_I2S_FLAG_OVR
#define SPI_FLAG_BSY          SPI_I2S_FLAG_BSY
#define SPI_DeInit            SPI_I2S_DeInit
#define SPI_ITConfig          SPI_I2S_ITConfig
#define SPI_DMACmd            SPI_I2S_DMACmd
#define SPI_SendData          SPI_I2S_SendData
#define SPI_ReceiveData       SPI_I2S_ReceiveData
#define SPI_GetFlagStatus     SPI_I2S_GetFlagStatus
#define SPI_ClearFlag         SPI_I2S_ClearFlag
#define SPI_GetITStatus       SPI_I2S_GetITStatus
#define SPI_ClearITPendingBit SPI_I2S_ClearITPendingBit
/**
 * @}
 */
 
 
/*spi ex */
#define SPI_TXFIFO_POS 						((uint8_t)0x08)
#define SPI_RXFIFO_POS 						((uint8_t)0x04)
#define SPI_TXFIFO_CLEAR					((uint8_t)0x02)
#define SPI_RXFIFO_CLEAR					((uint8_t)0x01)

#define SPI_RFFLR_POS 						((uint8_t)0x02)
#define SPI_TFFLR_POS 						((uint8_t)0x07)
#define SPI_RXFIFO_IT_PFULL					((uint32_t)0x04)
#define SPI_TXFIFO_IT_PEMP					((uint32_t)0x08)
#define SPI_RXFIFO_IT_FULL					((uint32_t)0x1000)
#define SPI_TXFIFO_IT_FULL					((uint32_t)0x2000)

#define IS_SPI_FIFO_IT(IT) (((IT) == SPI_RXFIFO_IT_PFULL) || ((IT) == SPI_TXFIFO_IT_PEMP) || ((IT) == SPI_RXFIFO_IT_FULL) || ((IT) ==SPI_TXFIFO_IT_FULL))

#define SPI_RXFIFO_FLAG_PFULL				((uint32_t)0x01)
#define SPI_TXFIFO_FLAG_PEMP				((uint32_t)0x02)
#define SPI_RXFIFO_FLAG_FULL				((uint32_t)0x1000)
#define SPI_TXFIFO_FLAG_FULL				((uint32_t)0x2000)
#define IS_SPI_FIFO_FLAG(FLAG) (((FLAG) == SPI_RXFIFO_FLAG_PFULL) || ((FLAG) == SPI_TXFIFO_FLAG_PEMP) || ((FLAG) == SPI_TXFIFO_FLAG_FULL) || ((FLAG) == SPI_RXFIFO_FLAG_FULL))
/** @defgroup SPI_TX_FIFO_Threshold
 * @{
 */
#define SPI_TXFIFO_THRESHOLD_1				((uint8_t)0x00)
#define SPI_TXFIFO_THRESHOLD_2				((uint8_t)0x01)
#define SPI_TXFIFO_THRESHOLD_3				((uint8_t)0x02)
#define SPI_TXFIFO_THRESHOLD_4				((uint8_t)0x03)
#define SPI_TXFIFO_THRESHOLD_5				((uint8_t)0x04)
#define SPI_TXFIFO_THRESHOLD_6				((uint8_t)0x05)
#define SPI_TXFIFO_THRESHOLD_7				((uint8_t)0x06)
#define SPI_TXFIFO_THRESHOLD_8				((uint8_t)0x07)
#define SPI_TXFIFO_THRESHOLD_9				((uint8_t)0x08)
#define SPI_TXFIFO_THRESHOLD_10				((uint8_t)0x09)
#define SPI_TXFIFO_THRESHOLD_11				((uint8_t)0x0A)
#define SPI_TXFIFO_THRESHOLD_12				((uint8_t)0x0B)
#define SPI_TXFIFO_THRESHOLD_13				((uint8_t)0x0C)
#define SPI_TXFIFO_THRESHOLD_14				((uint8_t)0x0D)
#define SPI_TXFIFO_THRESHOLD_15				((uint8_t)0x0E)
#define SPI_TXFIFO_THRESHOLD_16				((uint8_t)0x0F)

#define IS_SPI_TXFIFO_THRESHOLD(THRESHOLD) (((THRESHOLD) >= SPI_TXFIFO_THRESHOLD_1) || ((THRESHOLD) <= SPI_TXFIFO_THRESHOLD_16))
/**
 * @}
 */

/** @defgroup SPI_RX_FIFO_Threshold
 * @{
 */
#define SPI_RXFIFO_THRESHOLD_1				((uint8_t)0x00)
#define SPI_RXFIFO_THRESHOLD_2				((uint8_t)0x01)
#define SPI_RXFIFO_THRESHOLD_3				((uint8_t)0x02)
#define SPI_RXFIFO_THRESHOLD_4				((uint8_t)0x03)
#define SPI_RXFIFO_THRESHOLD_5				((uint8_t)0x04)
#define SPI_RXFIFO_THRESHOLD_6				((uint8_t)0x05)
#define SPI_RXFIFO_THRESHOLD_7				((uint8_t)0x06)
#define SPI_RXFIFO_THRESHOLD_8				((uint8_t)0x07)
#define SPI_RXFIFO_THRESHOLD_9				((uint8_t)0x08)
#define SPI_RXFIFO_THRESHOLD_10				((uint8_t)0x09)
#define SPI_RXFIFO_THRESHOLD_11				((uint8_t)0x0A)
#define SPI_RXFIFO_THRESHOLD_12				((uint8_t)0x0B)
#define SPI_RXFIFO_THRESHOLD_13				((uint8_t)0x0C)
#define SPI_RXFIFO_THRESHOLD_14				((uint8_t)0x0D)
#define SPI_RXFIFO_THRESHOLD_15				((uint8_t)0x0E)
#define SPI_RXFIFO_THRESHOLD_16				((uint8_t)0x0F)

#define IS_SPI_RXFIFO_THRESHOLD(THRESHOLD)	(((THRESHOLD) >= SPI_RXFIFO_THRESHOLD_1) || ((THRESHOLD) <= SPI_RXFIFO_THRESHOLD_16))
/**
 * @}
 */
 
/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the SPI configuration to the default reset state *****/
void SPI_I2S_DeInit(SPI_TypeDef* SPIx);

/* Initialization and Configuration functions *********************************/
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct);
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);
void I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct);
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize);
void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, uint16_t SPI_Direction);
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft);
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_TIModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState);

void I2S_FullDuplexConfig(SPI_TypeDef* I2Sxext, I2S_InitTypeDef* I2S_InitStruct);

/* Data transfers functions ***************************************************/
void     SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx);

/* Hardware CRC Calculation functions *****************************************/
void     SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState);
void     SPI_TransmitCRC(SPI_TypeDef* SPIx);
uint16_t SPI_GetCRC(SPI_TypeDef* SPIx, uint8_t SPI_CRC);
uint16_t SPI_GetCRCPolynomial(SPI_TypeDef* SPIx);

/* DMA transfers management functions *****************************************/
void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void       SPI_I2S_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT, FunctionalState NewState);
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG);
void       SPI_I2S_ClearFlag(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG);
ITStatus   SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT);
void       SPI_I2S_ClearITPendingBit(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT);



void SPI_TxFIFOCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_RxFIFOCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_TxFIFOThresholdConfig(SPI_TypeDef* SPIx, uint8_t Threshold);
void SPI_RxFIFOThresholdConfig(SPI_TypeDef* SPIx, uint8_t Threshold);
void SPI_FIFOITConfig(SPI_TypeDef* SPIx, uint32_t SPI_FIFO_IT, FunctionalState NewState);
FlagStatus SPI_FIFO_GetFlagStatus(SPI_TypeDef* SPIx, uint32_t SPI_FIFO_FLAG);
uint8_t SPI_TXFIFO_GetNum(SPI_TypeDef* SPIx);
uint8_t SPI_RXFIFO_GetNum(SPI_TypeDef* SPIx);
void SPI_AsyncModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_AsyncSampleDelayCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_ReceiveSampleDelayCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_TXFIFOClear(SPI_TypeDef* SPIx);
void SPI_RXFIFOClear(SPI_TypeDef* SPIx);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_SPI_H__
