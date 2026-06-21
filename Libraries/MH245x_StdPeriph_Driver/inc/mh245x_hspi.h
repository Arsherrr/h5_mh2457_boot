// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_HSPI_H__
#define __PERIPH_HSPI_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t CTRLR0;              /*!< HSPI control register 0,                                 Address offset: 0x00 */
    __IO uint32_t CTRLR1;              /*!< HSPI control register 1,                                 Address offset: 0x04 */
    __IO uint32_t SSIENR;              /*!< HSPI enable register,                                    Address offset: 0x08 */
    __IO uint32_t MWCR;                /*!< HSPI microwire control register,                         Address offset: 0x0C */
    __IO uint32_t SER;                 /*!< HSPI slave enable register,                              Address offset: 0x10 */
    __IO uint32_t BAUDR;               /*!< HSPI buad rate register,                                 Address offset: 0x14 */
    __IO uint32_t TXFTLR;              /*!< HSPI transmit fifo threshold level register,             Address offset: 0x18 */
    __IO uint32_t RXFTLR;              /*!< HSPI receive fifo threshold level register,              Address offset: 0x1C */
    __IO uint32_t TXFLR;               /*!< HSPI transmit fifo level register,                       Address offset: 0x20 */
    __IO uint32_t RXFLR;               /*!< HSPI receive fifo level register,                        Address offset: 0x24 */
    __IO uint32_t SR;                  /*!< HSPI status register,                                    Address offset: 0x28 */
    __IO uint32_t IMR;                 /*!< HSPI interrupt mask register,                            Address offset: 0x2C */
    __IO uint32_t ISR;                 /*!< HSPI interrupt status register,                          Address offset: 0x30 */
    __IO uint32_t RISR;                /*!< HSPI RAW interrupt status register,                      Address offset: 0x34 */
    __IO uint32_t TXOICR;              /*!< HSPI transmit fifo overflow interrupt clear register,    Address offset: 0x38 */
    __IO uint32_t RXOICR;              /*!< HSPI receive fifo overflow interrupt clear register,     Address offset: 0x3C */
    __IO uint32_t RXUICR;              /*!< HSPI receive fifo underflow interrupt clear register,    Address offset: 0x40 */
    __IO uint32_t MSTICR;              /*!< HSPI multi-master interrupt clear register,              Address offset: 0x44 */
    __IO uint32_t ICR;                 /*!< HSPI interrupt clear register,                           Address offset: 0x48 */
    __IO uint32_t DMACR;               /*!< HSPI DMA control register,                               Address offset: 0x4C */
    __IO uint32_t DMATDLR;             /*!< HSPI DMA transmit data level register,                   Address offset: 0x50 */
    __IO uint32_t DMARDLR;             /*!< HSPI DMA receive data level register,                    Address offset: 0x54 */
    uint32_t      RESERVED0[2];        /*!< Reserved, 0x58-0x5C*/
    __IO uint32_t DR;                  /*!< HSPI data register,                                      Address offset: 0x60 */
    uint32_t      RESERVED1[35];       /*!< Reserved, 0x64-0xEC*/
    __IO uint32_t RX_SAMPLY_DLY;       /*!< HSPI rx sample delay register,                           Address offset: 0xF0 */
    __IO uint32_t SPI_CTRLR0;          /*!< HSPI spi control register,                               Address offset: 0xF4 */
    __IO uint32_t TXD_DRIVE_EDGE;      /*!< HSPI transmit drive edge register,                       Address offset: 0xF8 */
    __IO uint32_t INST_REG_WR;         /*!< HSPI XIP write cmd register,                             Address offset: 0xFC */
    __IO uint32_t INST_REG_RD;         /*!< HSPI XIP read cmd register,                              Address offset: 0x100 */
    __IO uint32_t XIP_DP_CR;           /*!< HSPI XIP and deepsleep register,                         Address offset: 0x104 */
    __IO uint32_t DEEP_POWER_DOWN_CMD; /*!< HSPI Deep power down cmd register,                       Address offset: 0x108 */
    __IO uint32_t WAKE_CNT;            /*!< HSPI wake cnt register,                                  Address offset: 0x10C */
    __IO uint32_t PS_LC;
    uint32_t      RESERVED2[6]; /*!< Reserved, 0x114-0x128*/
    __IO uint32_t BYTE_NUM;     /*!< HSPI byte number register,                               Address offset: 0x12C */
} HSPI_TypeDef;

/******************************************************************************/
/*                                                                            */
/*                                HSPI                                         */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for HSPI_CTRLR0 register  *************/
#define HSPI_CTRLR0_SECONV ((uint32_t)0x02000000) /* Set the Endianness for data register reads */
#define HSPI_CTRLR0_SSTE   ((uint32_t)0x01000000) /* Slave Select Toggle Enable */

#define HSPI_CTRLR0_SPIFRF   ((uint32_t)0x00600000) /*!<SPIFRF[22:21] bits (SPI Frame Format) */
#define HSPI_CTRLR0_SPIFRF_0 ((uint32_t)0x00200000) /*!<Bit 0 */
#define HSPI_CTRLR0_SPIFRF_1 ((uint32_t)0x00400000) /*!<Bit 1 */

#define HSPI_CTRLR0_DFS32   ((uint32_t)0x001F0000) /*!<DFS32[20:16] bits (Data Frame Size) */
#define HSPI_CTRLR0_DFS32_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define HSPI_CTRLR0_DFS32_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define HSPI_CTRLR0_DFS32_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define HSPI_CTRLR0_DFS32_3 ((uint32_t)0x00080000) /*!<Bit 3 */
#define HSPI_CTRLR0_DFS32_4 ((uint32_t)0x00100000) /*!<Bit 4 */

#define HSPI_CTRLR0_CFS   ((uint32_t)0x0000F000) /*!<CFS[15:12] bits (Control Frame Size) */
#define HSPI_CTRLR0_CFS_0 ((uint32_t)0x00001000) /*!<Bit 0 */
#define HSPI_CTRLR0_CFS_1 ((uint32_t)0x00002000) /*!<Bit 1 */
#define HSPI_CTRLR0_CFS_2 ((uint32_t)0x00004000) /*!<Bit 2 */
#define HSPI_CTRLR0_CFS_3 ((uint32_t)0x00008000) /*!<Bit 3 */

#define HSPI_CTRLR0_SRL ((uint32_t)0x00000800) /* Shift Register Loop */

#define HSPI_CTRLR0_TMOD   ((uint32_t)0x00000300) /*!<TMOD[9:8] bits (Transfer Mode) */
#define HSPI_CTRLR0_TMOD_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define HSPI_CTRLR0_TMOD_1 ((uint32_t)0x00000200) /*!<Bit 1 */

#define HSPI_CTRLR0_SCPOL ((uint32_t)0x00000080) /* Serial clock polarity */
#define HSPI_CTRLR0_SCPH  ((uint32_t)0x00000040) /* Serial clock phase */

#define HSPI_CTRLR0_FRF   ((uint32_t)0x00000030) /*!<FRF[5:4] bits (Frame Format) */
#define HSPI_CTRLR0_FRF_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define HSPI_CTRLR0_FRF_1 ((uint32_t)0x00000020) /*!<Bit 1 */

/********************  Bit definition for HSPI_CTRLR1 register  *************/
#define HSPI_CTRLR1_NDF ((uint32_t)0x0000FFFF) /*!<NDF[15:0] bits (Number of data frames) */

/********************  Bit definition for HSPI_SSIENR register  *************/
#define HSPI_SSIENR_SSIEN ((uint32_t)0x00000001) /* HSPI Enable */

/********************  Bit definition for HSPI_MWCR register  *************/
#define HSPI_MWCR_MHS   ((uint32_t)0x00000004) /* Microwire Handshaking */
#define HSPI_MWCR_MDD   ((uint32_t)0x00000002) /* Microwire Control */
#define HSPI_MWCR_MWMOD ((uint32_t)0x00000001) /* Microwire Transfer Mode */

/********************  Bit definition for HSPI_SER register  *************/
#define HSPI_SER_SER   ((uint32_t)0x0000000F) /*!<SER[3:0] bits (Slave Select Enable Flag) */
#define HSPI_SER_SER_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define HSPI_SER_SER_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define HSPI_SER_SER_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define HSPI_SER_SER_3 ((uint32_t)0x00000008) /*!<Bit 3 */

/********************  Bit definition for HSPI_BAUDR register  *************/
#define HSPI_BAUDR_SCKDV ((uint32_t)0x0000FFFF) /*!<SCKDV[15:0] bits (HSPI Clock Divider) */

/********************  Bit definition for HSPI_TXFTLR register  *************/
#define HSPI_TXFTLR_TFT ((uint32_t)0x0000001F) /*!<TFT[4:0] bits (Transmit FIFO Threshold) */

/********************  Bit definition for HSPI_RXFTLR register  *************/
#define HSPI_RXFTLR_RFT ((uint32_t)0x0000001F) /*!<RFT[4:0] bits (Receive FIFO Threshold) */

/********************  Bit definition for HSPI_TXFLR register  *************/
#define HSPI_TXFLR_TXFLR ((uint32_t)0x0000003F) /*!<TXFLR[4:0] bits (Transmit FIFO Level) */

/********************  Bit definition for HSPI_RXFLR register  *************/
#define HSPI_RXFLR_RXFLR ((uint32_t)0x0000003F) /*!<RXFLR[4:0] bits (Receive FIFO Level) */

/********************  Bit definition for HSPI_SR register  *************/
#define HSPI_SR_DCOL ((uint32_t)0x00000040) /*Data Collision Error */
#define HSPI_SR_TXE  ((uint32_t)0x00000020) /*Transmission Error */
#define HSPI_SR_RFF  ((uint32_t)0x00000010) /*Receive FIFO Full */
#define HSPI_SR_RFNE ((uint32_t)0x00000008) /*Receive FIFO Not Emppty */
#define HSPI_SR_TFE  ((uint32_t)0x00000004) /*Transmit FIFO Emppty */
#define HSPI_SR_TFNF ((uint32_t)0x00000002) /*Transmit FIFO Not Full */
#define HSPI_SR_BUSY ((uint32_t)0x00000001) /*HSPI Busy Flag */

/********************  Bit definition for HSPI_IMR register  *************/
#define HSPI_IMR_MSTIM ((uint32_t)0x00000020) /*Multi-Master Contention Interrupt Mask */
#define HSPI_IMR_RXFIM ((uint32_t)0x00000010) /*Receive FIFO Full Interrupt Mask */
#define HSPI_IMR_RXOIM ((uint32_t)0x00000008) /*Receive FIFO Overflow Interrupt Mask */
#define HSPI_IMR_RXUIM ((uint32_t)0x00000004) /*Receive FIFO Underflow Interrupt Mask */
#define HSPI_IMR_TXOIM ((uint32_t)0x00000002) /*Transmit FIFO Overflow Interrupt Mask */
#define HSPI_IMR_TXEIM ((uint32_t)0x00000001) /*Transmit FIFO Empty Interrupt Mask */

/********************  Bit definition for HSPI_ISR register  *************/
#define HSPI_ISR_MSTIS ((uint32_t)0x00000020) /*Multi-Master Contention Interrupt Status is Active */
#define HSPI_ISR_RXFIS ((uint32_t)0x00000010) /*Receive FIFO Full Interrupt Status is Active */
#define HSPI_ISR_RXOIS ((uint32_t)0x00000008) /*Receive FIFO Overflow Interrupt Status is Active */
#define HSPI_ISR_RXUIS ((uint32_t)0x00000004) /*Receive FIFO Underflow Interrupt Status is Active */
#define HSPI_ISR_TXOIS ((uint32_t)0x00000002) /*Transmit FIFO Overflow Interrupt Status is Active */
#define HSPI_ISR_TXEIS ((uint32_t)0x00000001) /*Transmit FIFO Empty Interrupt Status is Active */

/********************  Bit definition for HSPI_RISR register  *************/
#define HSPI_ISR_MSTIR ((uint32_t)0x00000020) /*Multi-Master Contention Raw Interrupt Status is Active */
#define HSPI_ISR_RXFIR ((uint32_t)0x00000010) /*Receive FIFO Full Raw Interrupt Status is Active */
#define HSPI_ISR_RXOIR ((uint32_t)0x00000008) /*Receive FIFO Overflow Raw Interrupt Status is Active */
#define HSPI_ISR_RXUIR ((uint32_t)0x00000004) /*Receive FIFO Underflow Raw Interrupt Status is Active */
#define HSPI_ISR_TXOIR ((uint32_t)0x00000002) /*Transmit FIFO Overflow Raw Interrupt Status is Active */
#define HSPI_ISR_TXEIR ((uint32_t)0x00000001) /*Transmit FIFO Empty Raw Interrupt Status is Active */

/********************  Bit definition for HSPI_TXOICR register  *************/
#define HSPI_TXOICR_TXOICR ((uint32_t)0x00000001) /*Clear Transmit FIFO Overflow Interrupt */

/********************  Bit definition for HSPI_RXOICR register  *************/
#define HSPI_RXOICR_RXOICR ((uint32_t)0x00000001) /*Clear Receive FIFO Overflow Interrupt */

/********************  Bit definition for HSPI_RXUICR register  *************/
#define HSPI_RXUICR_RXUICR ((uint32_t)0x00000001) /*Clear Receive FIFO Underflow Interrupt */

/********************  Bit definition for HSPI_MSTICR register  *************/
#define HSPI_MSTICR_MSTICR ((uint32_t)0x00000001) /*Clear Multi-Master Interrupt */

/********************  Bit definition for HSPI_ICR register  *************/
#define HSPI_ICR_ICR ((uint32_t)0x00000001) /*Clear Interrupt */

/********************  Bit definition for HSPI_DMACR register  *************/
#define HSPI_DMACR_TDMAE ((uint32_t)0x00000002) /*Transmit DMA Enable */
#define HSPI_DMACR_RDMAE ((uint32_t)0x00000001) /*Receive DMA Enable */

/********************  Bit definition for HSPI_DMATDLR register  *************/
#define HSPI_DMATDLR_DMATDL ((uint32_t)0x0000001F) /*!<DMATDL[4:0] bits (Transmit Data Level) */

/********************  Bit definition for HSPI_DMARDLR register  *************/
#define HSPI_DMARDLR_DMARDL ((uint32_t)0x0000001F) /*!<DMARDL[4:0] bits (Receive Data Level) */

/********************  Bit definition for HSPI_RXSAMPLYDLY register  *************/
#define HSPI_RXSAMPLYDLY_RSD ((uint32_t)0x000000FF) /*!<RSD[7:0] bits (Rxd Sample Delay) */

/********************  Bit definition for HSPI_SPICTRLR0 register  *************/
#define HSPI_SPICTRLR0_SPIRXDSEN  ((uint32_t)0x00040000) /*Read data strobe enable bit */
#define HSPI_SPICTRLR0_INSTDDREN  ((uint32_t)0x00020000) /*Instruction DDR Enable bit */
#define HSPI_SPICTRLR0_SPIDDREN   ((uint32_t)0x00010000) /*SPI DDR Enable bit */
#define HSPI_SPICTRLR0_WAITCYCLES ((uint32_t)0x0000F800) /*!<WAITCYCLES[15:11] bits (Wait cycles) */

#define HSPI_SPICTRLR0_INSTL   ((uint32_t)0x00000300) /*!<INSTL[9:8] bits (Instruction Length) */
#define HSPI_SPICTRLR0_INSTL_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define HSPI_SPICTRLR0_INSTL_1 ((uint32_t)0x00000200) /*!<Bit 1 */

#define HSPI_SPICTRLR0_ADDRL       ((uint32_t)0x0000003C) /*!<ADDRL[5:2] bits (Adress Length) */
#define HSPI_SPICTRLR0_TRANSTYPE   ((uint32_t)0x00000003) /*!<ADDRL[1:0] bits (Adress and instruction transfer format) */
#define HSPI_SPICTRLR0_TRANSTYPE_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define HSPI_SPICTRLR0_TRANSTYPE_1 ((uint32_t)0x00000002) /*!<Bit 1 */

/********************  Bit definition for HSPI_TXDDRIVEEDGE register  *************/
#define HSPI_TXDDRIVEEDGE_TDE ((uint32_t)0x000000FF) /*!<TDE[7:0] bits (TXD Drive Edge) */

/********************  Bit definition for HSPI_INSTREGWR register  *************/
#define HSPI_INSTREGWR_WR ((uint32_t)0xFFFFFFFF) /*!<WR[31:0] bits */

/********************  Bit definition for HSPI_INSTREGRD register  *************/
#define HSPI_INSTREGWR_RD ((uint32_t)0xFFFFFFFF) /*!<RD[31:0] bits */

/********************  Bit definition for HSPI_INSTREGRD register  *************/
#define HSPI_XIPDPCR_FLASHON    ((uint32_t)0x00000020) /*Force Flash On */
#define HSPI_XIPDPCR_MODEDEVICE ((uint32_t)0x00000010) /*Mode Device Select */

#define HSPI_XIPDPCR_FORMAT   ((uint32_t)0x0000000C) /*!<FORMAT[3:2] bits(Format select in deep sleep) */
#define HSPI_XIPDPCR_FORMAT_0 ((uint32_t)0x00000004) /*!<Bit 0 */
#define HSPI_XIPDPCR_FORMAT_1 ((uint32_t)0x00000008) /*!<Bit 1 */

#define HSPI_XIPDPCR_INST1   ((uint32_t)0x00000003) /*!<INST1[1:0] bits(Length select in deep sleep) */
#define HSPI_XIPDPCR_INST1_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define HSPI_XIPDPCR_INST1_1 ((uint32_t)0x00000002) /*!<Bit 1 */

/********************  Bit definition for HSPI_DEEPPOWERDOWNCMD register  *************/
#define HSPI_DEEPPOWERDOWNCMD_DP ((uint32_t)0xFFFF0000) /*!<DP[31:16] bits(NOR FLASH sleep cmd in deep sleep) */
#define HSPI_DEEPPOWERDOWNCMD_WK ((uint32_t)0x0000FFFF) /*!<WK[15:0] bits(NOR FLASH wake up cmd in deep sleep) */

/********************  Bit definition for HSPI_WAKECNT register  *************/
#define HSPI_WAKECNT_COUNT ((uint32_t)0xFFFFFFFF) /*!<COUNT[31:0] bits(Time count for wake up) */

/* Exported types ------------------------------------------------------------*/

typedef struct
{
    /**
     * @brief / * Common Config Param
     */
    uint32_t HSPI_BAUDR_Sckdv;           /* Specifies the HSPI clock divider (SCLK_OUT = HSPI_CLK / HSPI_BAUDR_Sckdv)
                                            This parameter can be any even value between 2 and 65534*/
    uint32_t HSPI_Endian_Conv;           /* Specifies the HSPI Endianness for data register reads
                                            This parameter can be a value of @ref HSPI_Endianness_Conversion*/
    uint32_t HSPI_Data_Frame_Size;       /* Specifies the HSPI data frame size
                                            This parameter can be a value between 0x03 and 0x1f*/
    uint32_t HSPI_TranFIFO_Threshold;    /* Specifies the HSPI level of entries(or below) at which the transmit FIFO controller trigeers an interrupt
                                            This parameter can be any value between 0 and 31*/
    uint32_t HSPI_ReceiveFIFO_Threshold; /* Specifies the HSPI level of entries(or above) at which the receive FIFO controller trigeers an interrupt
                                            This parameter can be any value between 0 and 31*/
    uint32_t HSPI_CSMode;                /* Specifies the HSPI slave select toggle enable between consecutive data frames
                                            This parameter can be a value of @ref HSPI_CS_Mode*/
    uint32_t HSPI_RxSampleDelay;         /* Specifies delay the sample of the rxd input port
                                            This parameter can be a value of @ref HSPI_CS_Mode*/
} HSPI_InitTypeDef;

typedef struct
{
    uint32_t CKMode; /* Specifies the HSPI mode
                        This parameter can be a value of @ref HSPI_StandardSpi_CKMode*/

    uint32_t Transfer_Mode; /* Specifies the HSPI transfer mode for serial communication
                               This parameter can be a value of @ref HSPI_SandardSPI_Transfer_Mode*/

    uint32_t Rx_NumberDFrame; /* Specifies the HSPI number of data frames to be continuously received (Vaild in Receive Only Mode or EEPROM Read Mode)
                                  This parameter can be any value between 0 and 65535*/

    uint32_t Tx_NumberDFrame; /* Specifies the HSPI number of data frames to be continuously transmit
                                 This parameter range follow rule Tx_NumberDFrame * (HSPI_Data_Frame_Size + 1) / 8 < 8192
                                 That is ,the toal number of bytes transferred is less than 8192*/

} HSPI_StandardSpiDef;

typedef struct
{

    uint32_t CKMode; /* Specifies the HSPI mode
                         This parameter can be a value of @ref HSPI_DualSpi_CKMode*/

    uint32_t Line_Mode; /* Specifies the HSPI line mode
                            This parameter can be a value of @ref HSPI_DualSPI_Line_Mode*/

    uint32_t Direction_Mode; /* Specifies the HSPI transfer mode for serial communication
                                 This parameter can be a value of @ref HSPI_DualSPI_Direction*/

    uint32_t Wait_Cycles;     /* Specifies the HSPI number if wait cycles in dual/quad mode between control frames transmit and data reception
                                  This parameter can be any value between 0 and 31*/
    uint32_t Instruction_Len; /* Specifies the HSPI dual/quad mode instruction length
                                  This parameter can be a value of @ref HSPI_Instruction_Len*/
    uint32_t Address_Len;     /* Specifies the HSPI adress length to be transmitted
                                  This parameter can be a value of @ref HSPI_Address_Len*/

    uint32_t Rx_NumberDFrame; /* Specifies the HSPI number of data frames to be continuously received (Vaild in Receive Only Mode or EEPROM Read Mode)
                                 This parameter can be any value between 0 and 65535*/

    uint32_t Tx_NumberDFrame; /* Specifies the HSPI number of data frames to be continuously transmit
                                 This parameter range follow rule Tx_NumberDFrame * (HSPI_Data_Frame_Size + 1) / 8 < 8192
                                 That is ,the toal number of bytes transferred is less than 8192*/

} HSPI_DualSpiDef;

typedef struct
{

    uint32_t CKMode; /* Specifies the HSPI mode
                         This parameter can be a value of @ref HSPI_QuadSpi_CKMode*/

    uint32_t Line_Mode; /* Specifies the HSPI line mode
                            This parameter can be a value of @ref HSPI_QuadSPI_Line_Mode*/

    uint32_t Direction_Mode; /* Specifies the HSPI transfer mode for serial communication
                                 This parameter can be a value of @ref HSPI_QuadSPI_Direction*/

    uint32_t Wait_Cycles;     /* Specifies the HSPI number if wait cycles in dual/quad mode between control frames transmit and data reception
                                  This parameter can be any value between 0 and 31*/
    uint32_t Instruction_Len; /* Specifies the HSPI dual/quad mode instruction length
                                  This parameter can be a value of @ref HSPI_Instruction_Len*/
    uint32_t Address_Len;     /* Specifies the HSPI adress length to be transmitted
                                  This parameter can be a value of @ref HSPI_Address_Len*/

    uint32_t Rx_NumberDFrame; /* Specifies the HSPI number of data frames to be continuously received (Vaild in Receive Only Mode or EEPROM Read Mode)
                                   This parameter can be any value between 0 and 65535*/

    uint32_t Tx_NumberDFrame; /* Specifies the HSPI number of data frames to be continuously transmit
                                 This parameter range follow rule Tx_NumberDFrame * (HSPI_Data_Frame_Size + 1) / 8 < 8192
                                 That is ,the toal number of bytes transferred is less than 8192*/

} HSPI_QuadSpiDef;

/** @defgroup HSPI_SPI_MODE
 * @{
 */
#define HSPI_Standard_SPI      ((uint32_t)0x00000000)
#define HSPI_Dual_SPI          ((uint32_t)HSPI_CTRLR0_SPIFRF_0)
#define HSPI_Quad_SPI          ((uint32_t)HSPI_CTRLR0_SPIFRF_1)
#define IS_HSPI_SPI_MODE(MODE) (((MODE) == HSPI_Standard_SPI) || ((MODE) == HSPI_Dual_SPI) || ((MODE) == HSPI_Quad_SPI))
/**
 * @}
 */

/** @defgroup HSPI_BAUDR_Sckdv
 * @{
 */
#define IS_HSPI_BAUDR_Sckdv(DIV) (((DIV) > 0x0000001) && ((DIV) < 0x0000FFFF) && (((DIV) % 2) == 0))
/**
 * @}
 */

/** @defgroup HSPI_Endianness_Conversion
 * @{
 */
#define HSPI_ENDIAN_CONV_DISABLE    ((uint32_t)0x00000000)
#define HSPI_ENDIAN_CONV_ENABLE     ((uint32_t)HSPI_CTRLR0_SECONV)
#define IS_HSPI_ENDIAN_CONV(ENDIAN) (((ENDIAN) == HSPI_ENDIAN_CONV_DISABLE) || ((ENDIAN) == HSPI_ENDIAN_CONV_ENABLE))
/**
 * @}
 */

/** @defgroup HSPI_Data_Frame_Size
 * @{
 */
#define HSPI_DFS_8                    ((uint8_t)0x07)
#define HSPI_DFS_16                   ((uint8_t)0x0F)
#define HSPI_DFS_24                   ((uint8_t)0x17)
#define HSPI_DFS_32                   ((uint8_t)0x1F)
#define IS_HSPI_Data_Frame_Size(SIZE) (((SIZE) >= 0x00000003) && ((SIZE) <= 0x0000001f))
/**
 * @}
 */

/** @defgroup HSPI_TranFIFO_Threshold
 * @{
 */
#define IS_HSPI_TFIFO_Threshold(TFIFO) ((TFIFO) <= (uint32_t)(0x0000001F))
/**
 * @}
 */

/** @defgroup HSPI_ReceiveFIFO_Threshold
 * @{
 */
#define IS_HSPI_RFIFO_Threshold(RFIFO) ((RFIFO) <= (uint32_t)(0x0000001F))
/**
 * @}
 */

/** @defgroup HSPI_CS_Mode
 * @{
 */
#define HSPI_CS_STAYLOW     ((uint32_t)0x00000000)
#define HSPI_CS_TOGGLE      ((uint32_t)HSPI_CTRLR0_SSTE)
#define IS_HSPI_CS_MODE(CS) (((CS) == HSPI_CS_STAYLOW) || ((CS) == HSPI_CS_TOGGLE))
/**
 * @}
 */

/** @defgroup HSPI_StandardSpi_CKMode
 * @{
 */
#define HSPI_STANDARDSPI_CKMODE0 ((uint32_t)0x00000000)
#define HSPI_STANDARDSPI_CKMODE1 ((uint32_t)HSPI_CTRLR0_SCPH | (0x00000000))
#define HSPI_STANDARDSPI_CKMODE2 ((uint32_t)HSPI_CTRLR0_SCPOL | (0x00000000))
#define HSPI_STANDARDSPI_CKMODE3 ((uint32_t)(HSPI_CTRLR0_SCPOL | HSPI_CTRLR0_SCPH))
#define IS_HSPI_STANDARDSPI_CKMODE(CKMODE)                                                                                         \
    (((CKMODE) == HSPI_STANDARDSPI_CKMODE0) || ((CKMODE) == HSPI_STANDARDSPI_CKMODE1) || ((CKMODE) == HSPI_STANDARDSPI_CKMODE2) || \
     ((CKMODE) == HSPI_STANDARDSPI_CKMODE3))
/**
 * @}
 */

/** @defgroup HSPI_SandardSPI_Transfer_Mode
 * @{
 */
#define HSPI_SandardSPI_TxRx                   ((uint32_t)0x00000000)
#define HSPI_SandardSPI_Tx                     ((uint32_t)HSPI_CTRLR0_TMOD_0)
#define HSPI_SandardSPI_Rx                     ((uint32_t)HSPI_CTRLR0_TMOD_1)
#define IS_HSPI_SandardSPI_Transfer_Mode(TRAN) (((TRAN) == HSPI_SandardSPI_TxRx) || ((TRAN) == HSPI_SandardSPI_Tx) || ((TRAN) == HSPI_SandardSPI_Rx))
/**
 * @}
 */

/** @defgroup HSPI_Wait_Cycles
 * @{
 */
#define IS_HSPI_Wait_Cycles(WAT) ((WAT) <= (uint32_t)(0x0000001F))
/**
 * @}
 */

/** @defgroup HSPI_Instruction_Len
 * @{
 */
#define HSPI_NO_INST         ((uint32_t)0x00000000)
#define HSPI_4_INST          ((uint32_t)HSPI_SPICTRLR0_INSTL_0)
#define HSPI_8_INST          ((uint32_t)HSPI_SPICTRLR0_INSTL_1)
#define HSPI_16_INST         ((uint32_t)(HSPI_SPICTRLR0_INSTL_0 | HSPI_SPICTRLR0_INSTL_1))
#define IS_HSPI_InstLen(LEN) (((LEN) == HSPI_NO_INST) || ((LEN) == HSPI_4_INST) || ((LEN) == HSPI_8_INST) || ((LEN) == HSPI_16_INST))
/**
 * @}
 */

/** @defgroup HSPI_Address_Len
 * @{
 */
#define HSPI_AdressLen_0  ((uint32_t)0x00000000)
#define HSPI_AdressLen_4  ((uint32_t)0x00000004)
#define HSPI_AdressLen_8  ((uint32_t)0x00000008)
#define HSPI_AdressLen_12 ((uint32_t)0x0000000C)
#define HSPI_AdressLen_16 ((uint32_t)0x00000010)
#define HSPI_AdressLen_20 ((uint32_t)0x00000014)
#define HSPI_AdressLen_24 ((uint32_t)0x00000018)
#define HSPI_AdressLen_28 ((uint32_t)0x0000001C)
#define HSPI_AdressLen_32 ((uint32_t)0x00000020)
#define HSPI_AdressLen_36 ((uint32_t)0x00000024)
#define HSPI_AdressLen_40 ((uint32_t)0x00000028)
#define HSPI_AdressLen_44 ((uint32_t)0x0000002C)
#define HSPI_AdressLen_48 ((uint32_t)0x00000030)
#define HSPI_AdressLen_52 ((uint32_t)0x00000034)
#define HSPI_AdressLen_56 ((uint32_t)0x00000038)
#define HSPI_AdressLen_60 ((uint32_t)0x0000003C)
#define IS_HSPI_Address_Len(LEN)                                                                                                     \
    (((LEN) == HSPI_AdressLen_0) || ((LEN) == HSPI_AdressLen_4) || ((LEN) == HSPI_AdressLen_8) || ((LEN) == HSPI_AdressLen_12) ||    \
     ((LEN) == HSPI_AdressLen_16) || ((LEN) == HSPI_AdressLen_20) || ((LEN) == HSPI_AdressLen_24) || ((LEN) == HSPI_AdressLen_28) || \
     ((LEN) == HSPI_AdressLen_32) || ((LEN) == HSPI_AdressLen_36) || ((LEN) == HSPI_AdressLen_40) || ((LEN) == HSPI_AdressLen_44) || \
     ((LEN) == HSPI_AdressLen_48) || ((LEN) == HSPI_AdressLen_52) || ((LEN) == HSPI_AdressLen_56) || ((LEN) == HSPI_AdressLen_60))
/**
 * @}
 */

/** @defgroup HSPI_NumberDFrame
 * @{
 */
#define IS_HSPI_Rx_NumberDFrame(NUM) ((NUM) <= 0x0000ffff)
/**
 * @}
 */

/** @defgroup HSPI_byte_num
 * @{
 */
#define IS_HSPI_Tx_NumberDFrame(NUM) ((NUM) <= (uint32_t)(0x00001FFF))
/**
 * @}
 */

/** @defgroup HSPI_DualSpi_CKMode
 * @{
 */
#define HSPI_DUALSPI_CKMODE0           ((uint32_t)0x00000000)
#define HSPI_DUALSPI_CKMODE3           ((uint32_t)(HSPI_CTRLR0_SCPOL | HSPI_CTRLR0_SCPH))
#define IS_HSPI_DUALSPI_CKMODE(CKMODE) (((CKMODE) == HSPI_DUALSPI_CKMODE0) || ((CKMODE) == HSPI_DUALSPI_CKMODE3))
/**
 * @}
 */

/** @defgroup HSPI_DualSPI_Line_Mode
 * @{
 */
#define HSPI_DualSPI_Line_112          ((uint32_t)0x00000000)
#define HSPI_DualSPI_Line_122          ((uint32_t)(HSPI_XIPDPCR_INST1_0))
#define HSPI_DualSPI_Line_222          ((uint32_t)(HSPI_XIPDPCR_INST1_1))
#define IS_HSPI_DUALSPI_LINEMODE(MODE) (((MODE) == HSPI_DualSPI_Line_112) || ((MODE) == HSPI_DualSPI_Line_122) || ((MODE) == HSPI_DualSPI_Line_222))
/**
 * @}
 */

/** @defgroup HSPI_DualSPI_Direction
 * @{
 */
#define HSPI_DualSPI_Tx                ((uint32_t)HSPI_CTRLR0_TMOD_0)
#define HSPI_DualSPI_Rx                ((uint32_t)HSPI_CTRLR0_TMOD_1)
#define IS_HSPI_DualSPI_Direction(DIR) (((DIR) == HSPI_DualSPI_Tx) || ((DIR) == HSPI_DualSPI_Rx))
/**
 * @}
 */

/** @defgroup HSPI_QuadSpi_CKMode
 * @{
 */
#define HSPI_QUADSPI_CKMODE0           ((uint32_t)0x00000000)
#define HSPI_QUADSPI_CKMODE3           ((uint32_t)(HSPI_CTRLR0_SCPOL | HSPI_CTRLR0_SCPH))
#define IS_HSPI_QUADSPI_CKMODE(CKMODE) (((CKMODE) == HSPI_QUADSPI_CKMODE0) || ((CKMODE) == HSPI_QUADSPI_CKMODE3))
/**
 * @}
 */

/** @defgroup HSPI_QuadSPI_Line_Mode
 * @{
 */
#define HSPI_QuadSPI_Line_114          ((uint32_t)0x00000000)
#define HSPI_QuadSPI_Line_144          ((uint32_t)(HSPI_XIPDPCR_INST1_0))
#define HSPI_QuadSPI_Line_444          ((uint32_t)(HSPI_XIPDPCR_INST1_1))
#define IS_HSPI_QUADSPI_LINEMODE(MODE) (((MODE) == HSPI_QuadSPI_Line_114) || ((MODE) == HSPI_QuadSPI_Line_144) || ((MODE) == HSPI_QuadSPI_Line_444))
/**
 * @}
 */

/** @defgroup HSPI_QuadSPI_Direction
 * @{
 */
#define HSPI_QuadSPI_Tx                ((uint32_t)HSPI_CTRLR0_TMOD_0)
#define HSPI_QuadSPI_Rx                ((uint32_t)HSPI_CTRLR0_TMOD_1)
#define IS_HSPI_QUADSPI_Direction(DIR) (((DIR) == HSPI_QuadSPI_Tx) || ((DIR) == HSPI_QuadSPI_Rx))
/**
 * @}
 */

typedef struct
{

    uint32_t HSPI_Tran_DMA;    /* Specifies the HSPI transmit DMA status
                                  This parameter can be a value of @ref HSPI_Tran_DMA*/
    uint32_t HSPI_Receive_DMA; /* Specifies the HSPI receive DMA status
                                  This parameter can be a value of @ref HSPI_Receive_DMA*/
    uint32_t HSPI_TDMA_DLevel; /* Specifies the HSPI DMA transmit data level
                                  This parameter can be any value between 0 and 31*/
    uint32_t HSPI_RDMA_DLevel; /* Specifies the HSPI DMA receive data level
                                  This parameter can be any value between 0 and 31*/
} HSPI_DMAConfigTypeDef;

/** @defgroup HSPI_Tran_DMA
 * @{
 */
#define HSPI_TDMA_Disable  ((uint32_t)0x00000000)
#define HSPI_TDMA_Enable   ((uint32_t)HSPI_DMACR_TDMAE)
#define IS_HSPI_TDMA(TDMA) (((TDMA) == HSPI_TDMA_Disable) || ((TDMA) == HSPI_TDMA_Enable))
/**
 * @}
 */

/** @defgroup HSPI_Receive_DMA
 * @{
 */
#define HSPI_RDMA_Disable  ((uint32_t)0x00000000)
#define HSPI_RDMA_Enable   ((uint32_t)HSPI_DMACR_RDMAE)
#define IS_HSPI_RDMA(RDMA) (((RDMA) == HSPI_RDMA_Disable) || ((RDMA) == HSPI_RDMA_Enable))
/**
 * @}
 */

/** @defgroup HSPI_TDMA_DLevel
 * @{
 */
#define IS_HSPI_TDMA_DLevel(TFIFO) ((TFIFO) <= (uint32_t)(0x0000001F))
/**
 * @}
 */

/** @defgroup HSPI_RDMA_DLevel
 * @{
 */
#define IS_HSPI_RDMA_DLevel(TFIFO) ((TFIFO) <= (uint32_t)(0x0000001F))
/**
 * @}
 */

/** @defgroup HSPI_RXSAMPLE_DLY
 * @{
 */
#define IS_HSPI_RXSAMPLE_DLY(DLY) ((DLY) <= (uint32_t)(0x000000FF))
/**
 * @}
 */
#define HSPI_FLAG_BUSY                 ((uint32_t)0x00000001)
#define HSPI_FLAG_TX_FIFO_NOT_FULL     ((uint32_t)0x00000002)
#define HSPI_FLAG_TX_FIFO_EMPTY        ((uint32_t)0x00000004)
#define HSPI_FLAG_RX_FIFO_NOT_EMPTY    ((uint32_t)0x00000008)
#define HSPI_FLAG_RX_FIFO_FULL         ((uint32_t)0x00000010)
#define HSPI_FLAG_TX_FIFO_ERROR        ((uint32_t)0x00000020)
#define HSPI_FLAG_DATA_COLLISION_ERROR ((uint32_t)0x00000040)

#define HSPI_FLAG_TX_FIFO_EMPTY_IM    ((uint32_t)0x10000001)
#define HSPI_FLAG_TX_FIFO_OVERFLOW_IM ((uint32_t)0x10000002)
#define HSPI_FLAG_RX_FIFO_UNDERLOW_IM ((uint32_t)0x10000004)
#define HSPI_FLAG_RX_FIFO_OVERFLOW_IM ((uint32_t)0x10000008)
#define HSPI_FLAG_RX_FIFO_FULL_IM     ((uint32_t)0x10000010)
#define HSPI_FLAG_MULTI_MASTER_IM     ((uint32_t)0x10000020)

#define HSPI_FLAG_TX_FIFO_EMPTY_RI    ((uint32_t)0x20000001)
#define HSPI_FLAG_TX_FIFO_OVERFLOW_RI ((uint32_t)0x20000002)
#define HSPI_FLAG_RX_FIFO_UNDERLOW_RI ((uint32_t)0x20000004)
#define HSPI_FLAG_RX_FIFO_OVERFLOW_RI ((uint32_t)0x20000008)
#define HSPI_FLAG_RX_FIFO_FULL_RI     ((uint32_t)0x20000010)
#define HSPI_FLAG_MULTI_MASTER_RI     ((uint32_t)0x20000020)

#define IS_HSPI_GET_FLAG(FLAG)                                                                                                                                 \
    (((FLAG) == HSPI_FLAG_BUSY) || ((FLAG) == HSPI_FLAG_TX_FIFO_NOT_FULL) || ((FLAG) == HSPI_FLAG_TX_FIFO_EMPTY) || ((FLAG) == HSPI_FLAG_RX_FIFO_NOT_EMPTY) || \
     ((FLAG) == HSPI_FLAG_RX_FIFO_FULL) || ((FLAG) == HSPI_FLAG_TX_FIFO_ERROR) || ((FLAG) == HSPI_FLAG_TX_FIFO_EMPTY_IM) ||                                    \
     ((FLAG) == HSPI_FLAG_TX_FIFO_OVERFLOW_IM) || ((FLAG) == HSPI_FLAG_RX_FIFO_UNDERLOW_IM) || ((FLAG) == HSPI_FLAG_RX_FIFO_OVERFLOW_IM) ||                    \
     ((FLAG) == HSPI_FLAG_RX_FIFO_FULL_IM) || ((FLAG) == HSPI_FLAG_TX_FIFO_EMPTY_RI) || ((FLAG) == HSPI_FLAG_TX_FIFO_OVERFLOW_RI) ||                           \
     ((FLAG) == HSPI_FLAG_RX_FIFO_UNDERLOW_RI) || ((FLAG) == HSPI_FLAG_RX_FIFO_OVERFLOW_RI) || ((FLAG) == HSPI_FLAG_RX_FIFO_FULL_RI) ||                        \
     ((FLAG) == HSPI_FLAG_DATA_COLLISION_ERROR) || ((FLAG) == HSPI_FLAG_MULTI_MASTER_IM) || ((FLAG) == HSPI_FLAG_MULTI_MASTER_RI))

#define HSPI_IT_TX_EMPTY     ((uint32_t)0x00000001)
#define HSPI_IT_TX_OVERFLOW  ((uint32_t)0x00000002)
#define HSPI_IT_RX_UNDERFLOW ((uint32_t)0x00000004)
#define HSPI_IT_RX_OVERFLOW  ((uint32_t)0x00000008)
#define HSPI_IT_RX_FULL      ((uint32_t)0x00000010)
#define HSPI_IT_MUILT_MASTER ((uint32_t)0x00000020)

#define IS_HSPI_CONFIG_IT(IT)                                                                                                          \
    (((IT) == HSPI_IT_TX_EMPTY) || ((IT) == HSPI_IT_TX_OVERFLOW) || ((IT) == HSPI_IT_RX_UNDERFLOW) || ((IT) == HSPI_IT_RX_OVERFLOW) || \
     ((IT) == HSPI_IT_RX_FULL) || ((IT) == HSPI_IT_MUILT_MASTER))

#define IS_HSPI_GET_IT(IT)                                                                                                             \
    (((IT) == HSPI_IT_TX_EMPTY) || ((IT) == HSPI_IT_TX_OVERFLOW) || ((IT) == HSPI_IT_RX_UNDERFLOW) || ((IT) == HSPI_IT_RX_OVERFLOW) || \
     ((IT) == HSPI_IT_RX_FULL) || ((IT) == HSPI_IT_MUILT_MASTER))

#define IS_HSPI_CLEAR_IT(IT)                                                                                                           \
    (((IT) == HSPI_IT_TX_EMPTY) || ((IT) == HSPI_IT_TX_OVERFLOW) || ((IT) == HSPI_IT_RX_UNDERFLOW) || ((IT) == HSPI_IT_RX_OVERFLOW) || \
     ((IT) == HSPI_IT_RX_FULL) || ((IT) == HSPI_IT_MUILT_MASTER))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void       HSPI_DeInit(void);
void       HSPI_Init(HSPI_InitTypeDef* HSPI_InitStruct);
void       HSPI_StructInit(HSPI_InitTypeDef* HSPI_InitStruct);
void       HSPI_StandardSPI_Config(HSPI_StandardSpiDef* HSPI_StandardSpiStruct);
void       HSPI_DualSPI_Config(HSPI_DualSpiDef* HSPI_DualSpiStruct);
void       HSPI_QuadSPI_Config(HSPI_QuadSpiDef* HSPI_QuadSpiStruct);
void       HSPI_DmaConfig(HSPI_DMAConfigTypeDef* HSPI_DmaConfigStruct);
void       HSPI_Cmd(FunctionalState NewState);
void       HSPI_DataFrameSizeConfig(uint32_t DataFrameSize);
void       HSPI_RxSampleDelay_Config(uint32_t SampleDelay);
void       HSPI_ITConfig(uint16_t HSPI_IT, FunctionalState NewState);
FlagStatus HSPI_GetFlagStatus(uint32_t HSPI_FLAG);
void       HSPI_ClearITPendingBit(uint32_t HSPI_IT);
ITStatus   HSPI_GetITStatus(uint32_t HSPI_IT);
uint32_t   HSPI_GetTxFifoLevel(void);
uint32_t   HSPI_GetRxFifoLevel(void);
void       HSPI_SendData(uint32_t data);
uint32_t   HSPI_ReceiveData(void);

#ifdef __cplusplus
}
#endif

#endif
