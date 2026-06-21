// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_SDIO_H__
#define __PERIPH_SDIO_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t POWER;         /*!< SDIO power control register,    Address offset: 0x00 */
    __IO uint32_t CLKCR;         /*!< SDI clock control register,     Address offset: 0x04 */
    __IO uint32_t ARG;           /*!< SDIO argument register,         Address offset: 0x08 */
    __IO uint32_t CMD;           /*!< SDIO command register,          Address offset: 0x0C */
    __I uint32_t  RESPCMD;       /*!< SDIO command response register, Address offset: 0x10 */
    __I uint32_t  RESP1;         /*!< SDIO response 1 register,       Address offset: 0x14 */
    __I uint32_t  RESP2;         /*!< SDIO response 2 register,       Address offset: 0x18 */
    __I uint32_t  RESP3;         /*!< SDIO response 3 register,       Address offset: 0x1C */
    __I uint32_t  RESP4;         /*!< SDIO response 4 register,       Address offset: 0x20 */
    __IO uint32_t DTIMER;        /*!< SDIO data timer register,       Address offset: 0x24 */
    __IO uint32_t DLEN;          /*!< SDIO data length register,      Address offset: 0x28 */
    __IO uint32_t DCTRL;         /*!< SDIO data control register,     Address offset: 0x2C */
    __I uint32_t  DCOUNT;        /*!< SDIO data counter register,     Address offset: 0x30 */
    __I uint32_t  STA;           /*!< SDIO status register,           Address offset: 0x34 */
    __IO uint32_t ICR;           /*!< SDIO interrupt clear register,  Address offset: 0x38 */
    __IO uint32_t MASK;          /*!< SDIO mask register,             Address offset: 0x3C */
    uint32_t      RESERVED0[2];  /*!< Reserved, 0x40-0x44                                  */
    __I uint32_t  FIFOCNT;       /*!< SDIO FIFO counter register,     Address offset: 0x48 */
    uint32_t      RESERVED1[13]; /*!< Reserved, 0x4C-0x7C                                  */
    __IO uint32_t FIFO;          /*!< SDIO data FIFO register,        Address offset: 0x80 */
} SDIO_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/******************  Bit definition for SDIO_POWER register  ******************/
#define SDIO_POWER_PWRCTRL   ((uint8_t)0x03) /*!<PWRCTRL[1:0] bits (Power supply control bits) */
#define SDIO_POWER_PWRCTRL_0 ((uint8_t)0x01) /*!<Bit 0 */
#define SDIO_POWER_PWRCTRL_1 ((uint8_t)0x02) /*!<Bit 1 */

/******************  Bit definition for SDIO_CLKCR register  ******************/
#define SDIO_CLKCR_CLKDIV ((uint16_t)0x00FF) /*!<Clock divide factor             */
#define SDIO_CLKCR_CLKEN  ((uint16_t)0x0100) /*!<Clock enable bit                */
#define SDIO_CLKCR_PWRSAV ((uint16_t)0x0200) /*!<Power saving configuration bit  */
#define SDIO_CLKCR_BYPASS ((uint16_t)0x0400) /*!<Clock divider bypass enable bit */

#define SDIO_CLKCR_WIDBUS   ((uint16_t)0x1800) /*!<WIDBUS[1:0] bits (Wide bus mode enable bit) */
#define SDIO_CLKCR_WIDBUS_0 ((uint16_t)0x0800) /*!<Bit 0 */
#define SDIO_CLKCR_WIDBUS_1 ((uint16_t)0x1000) /*!<Bit 1 */

#define SDIO_CLKCR_NEGEDGE ((uint16_t)0x2000) /*!<SDIO_CK dephasing selection bit */
#define SDIO_CLKCR_HWFC_EN ((uint16_t)0x4000) /*!<HW Flow Control enable          */

/*******************  Bit definition for SDIO_ARG register  *******************/
#define SDIO_ARG_CMDARG ((uint32_t)0xFFFFFFFF) /*!<Command argument */

/*******************  Bit definition for SDIO_CMD register  *******************/
#define SDIO_CMD_CMDINDEX ((uint16_t)0x003F) /*!<Command Index                               */

#define SDIO_CMD_WAITRESP   ((uint16_t)0x00C0) /*!<WAITRESP[1:0] bits (Wait for response bits) */
#define SDIO_CMD_WAITRESP_0 ((uint16_t)0x0040) /*!< Bit 0 */
#define SDIO_CMD_WAITRESP_1 ((uint16_t)0x0080) /*!< Bit 1 */

#define SDIO_CMD_WAITINT     ((uint16_t)0x0100) /*!<CPSM Waits for Interrupt Request                               */
#define SDIO_CMD_WAITPEND    ((uint16_t)0x0200) /*!<CPSM Waits for ends of data transfer (CmdPend internal signal) */
#define SDIO_CMD_CPSMEN      ((uint16_t)0x0400) /*!<Command path state machine (CPSM) Enable bit                   */
#define SDIO_CMD_SDIOSUSPEND ((uint16_t)0x0800) /*!<SD I/O suspend command                                         */
#define SDIO_CMD_ENCMDCOMPL  ((uint16_t)0x1000) /*!<Enable CMD completion                                          */
#define SDIO_CMD_NIEN        ((uint16_t)0x2000) /*!<Not Interrupt Enable */
#define SDIO_CMD_CEATACMD    ((uint16_t)0x4000) /*!<CE-ATA command       */

/*****************  Bit definition for SDIO_RESPCMD register  *****************/
#define SDIO_RESPCMD_RESPCMD ((uint8_t)0x3F) /*!<Response command index */

/******************  Bit definition for SDIO_RESP0 register  ******************/
#define SDIO_RESP0_CARDSTATUS0 ((uint32_t)0xFFFFFFFF) /*!<Card Status */

/******************  Bit definition for SDIO_RESP1 register  ******************/
#define SDIO_RESP1_CARDSTATUS1 ((uint32_t)0xFFFFFFFF) /*!<Card Status */

/******************  Bit definition for SDIO_RESP2 register  ******************/
#define SDIO_RESP2_CARDSTATUS2 ((uint32_t)0xFFFFFFFF) /*!<Card Status */

/******************  Bit definition for SDIO_RESP3 register  ******************/
#define SDIO_RESP3_CARDSTATUS3 ((uint32_t)0xFFFFFFFF) /*!<Card Status */

/******************  Bit definition for SDIO_RESP4 register  ******************/
#define SDIO_RESP4_CARDSTATUS4 ((uint32_t)0xFFFFFFFF) /*!<Card Status */

/******************  Bit definition for SDIO_DTIMER register  *****************/
#define SDIO_DTIMER_DATATIME ((uint32_t)0xFFFFFFFF) /*!<Data timeout period. */

/******************  Bit definition for SDIO_DLEN register  *******************/
#define SDIO_DLEN_DATALENGTH ((uint32_t)0x01FFFFFF) /*!<Data length value    */

/******************  Bit definition for SDIO_DCTRL register  ******************/
#define SDIO_DCTRL_DTEN   ((uint16_t)0x0001) /*!<Data transfer enabled bit         */
#define SDIO_DCTRL_DTDIR  ((uint16_t)0x0002) /*!<Data transfer direction selection */
#define SDIO_DCTRL_DTMODE ((uint16_t)0x0004) /*!<Data transfer mode selection      */
#define SDIO_DCTRL_DMAEN  ((uint16_t)0x0008) /*!<DMA enabled bit                   */

#define SDIO_DCTRL_DBLOCKSIZE   ((uint16_t)0x00F0) /*!<DBLOCKSIZE[3:0] bits (Data block size) */
#define SDIO_DCTRL_DBLOCKSIZE_0 ((uint16_t)0x0010) /*!<Bit 0 */
#define SDIO_DCTRL_DBLOCKSIZE_1 ((uint16_t)0x0020) /*!<Bit 1 */
#define SDIO_DCTRL_DBLOCKSIZE_2 ((uint16_t)0x0040) /*!<Bit 2 */
#define SDIO_DCTRL_DBLOCKSIZE_3 ((uint16_t)0x0080) /*!<Bit 3 */

#define SDIO_DCTRL_RWSTART ((uint16_t)0x0100) /*!<Read wait start         */
#define SDIO_DCTRL_RWSTOP  ((uint16_t)0x0200) /*!<Read wait stop          */
#define SDIO_DCTRL_RWMOD   ((uint16_t)0x0400) /*!<Read wait mode          */
#define SDIO_DCTRL_SDIOEN  ((uint16_t)0x0800) /*!<SD I/O enable functions */

/******************  Bit definition for SDIO_DCOUNT register  *****************/
#define SDIO_DCOUNT_DATACOUNT ((uint32_t)0x01FFFFFF) /*!<Data count value */

/******************  Bit definition for SDIO_STA register  ********************/
#define SDIO_STA_CCRCFAIL ((uint32_t)0x00000001) /*!<Command response received (CRC check failed)  */
#define SDIO_STA_DCRCFAIL ((uint32_t)0x00000002) /*!<Data block sent/received (CRC check failed)   */
#define SDIO_STA_CTIMEOUT ((uint32_t)0x00000004) /*!<Command response timeout                      */
#define SDIO_STA_DTIMEOUT ((uint32_t)0x00000008) /*!<Data timeout                                  */
#define SDIO_STA_TXUNDERR ((uint32_t)0x00000010) /*!<Transmit FIFO underrun error                  */
#define SDIO_STA_RXOVERR  ((uint32_t)0x00000020) /*!<Received FIFO overrun error                   */
#define SDIO_STA_CMDREND  ((uint32_t)0x00000040) /*!<Command response received (CRC check passed)  */
#define SDIO_STA_CMDSENT  ((uint32_t)0x00000080) /*!<Command sent (no response required)           */
#define SDIO_STA_DATAEND  ((uint32_t)0x00000100) /*!<Data end (data counter, SDIDCOUNT, is zero)   */
#define SDIO_STA_STBITERR ((uint32_t)0x00000200) /*!<Start bit not detected on all data signals in wide bus mode */
#define SDIO_STA_DBCKEND  ((uint32_t)0x00000400) /*!<Data block sent/received (CRC check passed)   */
#define SDIO_STA_CMDACT   ((uint32_t)0x00000800) /*!<Command transfer in progress                  */
#define SDIO_STA_TXACT    ((uint32_t)0x00001000) /*!<Data transmit in progress                     */
#define SDIO_STA_RXACT    ((uint32_t)0x00002000) /*!<Data receive in progress                      */
#define SDIO_STA_TXFIFOHE ((uint32_t)0x00004000) /*!<Transmit FIFO Half Empty: at least 8 words can be written into the FIFO */
#define SDIO_STA_RXFIFOHF ((uint32_t)0x00008000) /*!<Receive FIFO Half Full: there are at least 8 words in the FIFO */
#define SDIO_STA_TXFIFOF  ((uint32_t)0x00010000) /*!<Transmit FIFO full                            */
#define SDIO_STA_RXFIFOF  ((uint32_t)0x00020000) /*!<Receive FIFO full                             */
#define SDIO_STA_TXFIFOE  ((uint32_t)0x00040000) /*!<Transmit FIFO empty                           */
#define SDIO_STA_RXFIFOE  ((uint32_t)0x00080000) /*!<Receive FIFO empty                            */
#define SDIO_STA_TXDAVL   ((uint32_t)0x00100000) /*!<Data available in transmit FIFO               */
#define SDIO_STA_RXDAVL   ((uint32_t)0x00200000) /*!<Data available in receive FIFO                */
#define SDIO_STA_SDIOIT   ((uint32_t)0x00400000) /*!<SDIO interrupt received                       */
#define SDIO_STA_CEATAEND ((uint32_t)0x00800000) /*!<CE-ATA command completion signal received for CMD61 */

/*******************  Bit definition for SDIO_ICR register  *******************/
#define SDIO_ICR_CCRCFAILC ((uint32_t)0x00000001) /*!<CCRCFAIL flag clear bit */
#define SDIO_ICR_DCRCFAILC ((uint32_t)0x00000002) /*!<DCRCFAIL flag clear bit */
#define SDIO_ICR_CTIMEOUTC ((uint32_t)0x00000004) /*!<CTIMEOUT flag clear bit */
#define SDIO_ICR_DTIMEOUTC ((uint32_t)0x00000008) /*!<DTIMEOUT flag clear bit */
#define SDIO_ICR_TXUNDERRC ((uint32_t)0x00000010) /*!<TXUNDERR flag clear bit */
#define SDIO_ICR_RXOVERRC  ((uint32_t)0x00000020) /*!<RXOVERR flag clear bit  */
#define SDIO_ICR_CMDRENDC  ((uint32_t)0x00000040) /*!<CMDREND flag clear bit  */
#define SDIO_ICR_CMDSENTC  ((uint32_t)0x00000080) /*!<CMDSENT flag clear bit  */
#define SDIO_ICR_DATAENDC  ((uint32_t)0x00000100) /*!<DATAEND flag clear bit  */
#define SDIO_ICR_STBITERRC ((uint32_t)0x00000200) /*!<STBITERR flag clear bit */
#define SDIO_ICR_DBCKENDC  ((uint32_t)0x00000400) /*!<DBCKEND flag clear bit  */
#define SDIO_ICR_SDIOITC   ((uint32_t)0x00400000) /*!<SDIOIT flag clear bit   */
#define SDIO_ICR_CEATAENDC ((uint32_t)0x00800000) /*!<CEATAEND flag clear bit */

/******************  Bit definition for SDIO_MASK register  *******************/
#define SDIO_MASK_CCRCFAILIE ((uint32_t)0x00000001) /*!<Command CRC Fail Interrupt Enable          */
#define SDIO_MASK_DCRCFAILIE ((uint32_t)0x00000002) /*!<Data CRC Fail Interrupt Enable             */
#define SDIO_MASK_CTIMEOUTIE ((uint32_t)0x00000004) /*!<Command TimeOut Interrupt Enable           */
#define SDIO_MASK_DTIMEOUTIE ((uint32_t)0x00000008) /*!<Data TimeOut Interrupt Enable              */
#define SDIO_MASK_TXUNDERRIE ((uint32_t)0x00000010) /*!<Tx FIFO UnderRun Error Interrupt Enable    */
#define SDIO_MASK_RXOVERRIE  ((uint32_t)0x00000020) /*!<Rx FIFO OverRun Error Interrupt Enable     */
#define SDIO_MASK_CMDRENDIE  ((uint32_t)0x00000040) /*!<Command Response Received Interrupt Enable */
#define SDIO_MASK_CMDSENTIE  ((uint32_t)0x00000080) /*!<Command Sent Interrupt Enable              */
#define SDIO_MASK_DATAENDIE  ((uint32_t)0x00000100) /*!<Data End Interrupt Enable                  */
#define SDIO_MASK_STBITERRIE ((uint32_t)0x00000200) /*!<Start Bit Error Interrupt Enable           */
#define SDIO_MASK_DBCKENDIE  ((uint32_t)0x00000400) /*!<Data Block End Interrupt Enable            */
#define SDIO_MASK_CMDACTIE   ((uint32_t)0x00000800) /*!<CCommand Acting Interrupt Enable           */
#define SDIO_MASK_TXACTIE    ((uint32_t)0x00001000) /*!<Data Transmit Acting Interrupt Enable      */
#define SDIO_MASK_RXACTIE    ((uint32_t)0x00002000) /*!<Data receive acting interrupt enabled      */
#define SDIO_MASK_TXFIFOHEIE ((uint32_t)0x00004000) /*!<Tx FIFO Half Empty interrupt Enable        */
#define SDIO_MASK_RXFIFOHFIE ((uint32_t)0x00008000) /*!<Rx FIFO Half Full interrupt Enable         */
#define SDIO_MASK_TXFIFOFIE  ((uint32_t)0x00010000) /*!<Tx FIFO Full interrupt Enable              */
#define SDIO_MASK_RXFIFOFIE  ((uint32_t)0x00020000) /*!<Rx FIFO Full interrupt Enable              */
#define SDIO_MASK_TXFIFOEIE  ((uint32_t)0x00040000) /*!<Tx FIFO Empty interrupt Enable             */
#define SDIO_MASK_RXFIFOEIE  ((uint32_t)0x00080000) /*!<Rx FIFO Empty interrupt Enable             */
#define SDIO_MASK_TXDAVLIE   ((uint32_t)0x00100000) /*!<Data available in Tx FIFO interrupt Enable */
#define SDIO_MASK_RXDAVLIE   ((uint32_t)0x00200000) /*!<Data available in Rx FIFO interrupt Enable */
#define SDIO_MASK_SDIOITIE   ((uint32_t)0x00400000) /*!<SDIO Mode Interrupt Received interrupt Enable */
#define SDIO_MASK_CEATAENDIE ((uint32_t)0x00800000) /*!<CE-ATA command completion signal received Interrupt Enable */

/*****************  Bit definition for SDIO_FIFOCNT register  *****************/
#define SDIO_FIFOCNT_FIFOCOUNT ((uint32_t)0x00FFFFFF) /*!<Remaining number of words to be written to or read from the FIFO */

/******************  Bit definition for SDIO_FIFO register  *******************/
#define SDIO_FIFO_FIFODATA ((uint32_t)0xFFFFFFFF) /*!<Receive and transmit FIFO data */

/* Exported types ------------------------------------------------------------*/

typedef struct
{
    uint32_t SDIO_ClockEdge; /*!< Specifies the clock transition on which the bit capture is made.
                                  This parameter can be a value of @ref SDIO_Clock_Edge */

    uint32_t SDIO_ClockBypass; /*!< Specifies whether the SDIO Clock divider bypass is
                                    enabled or disabled.
                                    This parameter can be a value of @ref SDIO_Clock_Bypass */

    uint32_t SDIO_ClockPowerSave; /*!< Specifies whether SDIO Clock output is enabled or
                                       disabled when the bus is idle.
                                       This parameter can be a value of @ref SDIO_Clock_Power_Save */

    uint32_t SDIO_BusWide; /*!< Specifies the SDIO bus width.
                                This parameter can be a value of @ref SDIO_Bus_Wide */

    uint32_t SDIO_HardwareFlowControl; /*!< Specifies whether the SDIO hardware flow control is enabled or disabled.
                                            This parameter can be a value of @ref SDIO_Hardware_Flow_Control */

    uint8_t SDIO_ClockDiv; /*!< Specifies the clock frequency of the SDIO controller.
                                This parameter can be a value between 0x00 and 0xFF. */

} SDIO_InitTypeDef;

typedef struct
{
    uint32_t SDIO_Argument; /*!< Specifies the SDIO command argument which is sent
                                 to a card as part of a command message. If a command
                                 contains an argument, it must be loaded into this register
                                 before writing the command to the command register */

    uint32_t SDIO_CmdIndex; /*!< Specifies the SDIO command index. It must be lower than 0x40. */

    uint32_t SDIO_Response; /*!< Specifies the SDIO response type.
                                 This parameter can be a value of @ref SDIO_Response_Type */

    uint32_t SDIO_Wait; /*!< Specifies whether SDIO wait for interrupt request is enabled or disabled.
                             This parameter can be a value of @ref SDIO_Wait_Interrupt_State */

    uint32_t SDIO_CPSM; /*!< Specifies whether SDIO Command path state machine (CPSM)
                             is enabled or disabled.
                             This parameter can be a value of @ref SDIO_CPSM_State */
} SDIO_CmdInitTypeDef;

typedef struct
{
    uint32_t SDIO_DataTimeOut; /*!< Specifies the data timeout period in card bus clock periods. */

    uint32_t SDIO_DataLength; /*!< Specifies the number of data bytes to be transferred. */

    uint32_t SDIO_DataBlockSize; /*!< Specifies the data block size for block transfer.
                                      This parameter can be a value of @ref SDIO_Data_Block_Size */

    uint32_t SDIO_TransferDir; /*!< Specifies the data transfer direction, whether the transfer
                                    is a read or write.
                                    This parameter can be a value of @ref SDIO_Transfer_Direction */

    uint32_t SDIO_TransferMode; /*!< Specifies whether data transfer is in stream or block mode.
                                     This parameter can be a value of @ref SDIO_Transfer_Type */

    uint32_t SDIO_DPSM; /*!< Specifies whether SDIO Data path state machine (DPSM)
                             is enabled or disabled.
                             This parameter can be a value of @ref SDIO_DPSM_State */
} SDIO_DataInitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup SDIO_Exported_Constants
 * @{
 */

/** @defgroup SDIO_Clock_Edge
 * @{
 */

#define SDIO_ClockEdge_Rising    ((uint32_t)0x00000000)
#define SDIO_ClockEdge_Falling   ((uint32_t)0x00002000)
#define IS_SDIO_CLOCK_EDGE(EDGE) (((EDGE) == SDIO_ClockEdge_Rising) || ((EDGE) == SDIO_ClockEdge_Falling))
/**
 * @}
 */

/** @defgroup SDIO_Clock_Bypass
 * @{
 */

#define SDIO_ClockBypass_Disable     ((uint32_t)0x00000000)
#define SDIO_ClockBypass_Enable      ((uint32_t)0x00000400)
#define IS_SDIO_CLOCK_BYPASS(BYPASS) (((BYPASS) == SDIO_ClockBypass_Disable) || ((BYPASS) == SDIO_ClockBypass_Enable))
/**
 * @}
 */

/** @defgroup SDIO_Clock_Power_Save
 * @{
 */

#define SDIO_ClockPowerSave_Disable    ((uint32_t)0x00000000)
#define SDIO_ClockPowerSave_Enable     ((uint32_t)0x00000200)
#define IS_SDIO_CLOCK_POWER_SAVE(SAVE) (((SAVE) == SDIO_ClockPowerSave_Disable) || ((SAVE) == SDIO_ClockPowerSave_Enable))
/**
 * @}
 */

/** @defgroup SDIO_Bus_Wide
 * @{
 */

#define SDIO_BusWide_1b        ((uint32_t)0x00000000)
#define SDIO_BusWide_4b        ((uint32_t)0x00000800)
#define SDIO_BusWide_8b        ((uint32_t)0x00001000)
#define IS_SDIO_BUS_WIDE(WIDE) (((WIDE) == SDIO_BusWide_1b) || ((WIDE) == SDIO_BusWide_4b) || ((WIDE) == SDIO_BusWide_8b))

/**
 * @}
 */

/** @defgroup SDIO_Hardware_Flow_Control
 * @{
 */

#define SDIO_HardwareFlowControl_Disable       ((uint32_t)0x00000000)
#define SDIO_HardwareFlowControl_Enable        ((uint32_t)0x00004000)
#define IS_SDIO_HARDWARE_FLOW_CONTROL(CONTROL) (((CONTROL) == SDIO_HardwareFlowControl_Disable) || ((CONTROL) == SDIO_HardwareFlowControl_Enable))
/**
 * @}
 */

/** @defgroup SDIO_Power_State
 * @{
 */

#define SDIO_PowerState_OFF        ((uint32_t)0x00000000)
#define SDIO_PowerState_ON         ((uint32_t)0x00000003)
#define IS_SDIO_POWER_STATE(STATE) (((STATE) == SDIO_PowerState_OFF) || ((STATE) == SDIO_PowerState_ON))
/**
 * @}
 */

/** @defgroup SDIO_Interrupt_sources
 * @{
 */

#define SDIO_IT_CCRCFAIL ((uint32_t)0x00000001)
#define SDIO_IT_DCRCFAIL ((uint32_t)0x00000002)
#define SDIO_IT_CTIMEOUT ((uint32_t)0x00000004)
#define SDIO_IT_DTIMEOUT ((uint32_t)0x00000008)
#define SDIO_IT_TXUNDERR ((uint32_t)0x00000010)
#define SDIO_IT_RXOVERR  ((uint32_t)0x00000020)
#define SDIO_IT_CMDREND  ((uint32_t)0x00000040)
#define SDIO_IT_CMDSENT  ((uint32_t)0x00000080)
#define SDIO_IT_DATAEND  ((uint32_t)0x00000100)
#define SDIO_IT_STBITERR ((uint32_t)0x00000200)
#define SDIO_IT_DBCKEND  ((uint32_t)0x00000400)
#define SDIO_IT_CMDACT   ((uint32_t)0x00000800)
#define SDIO_IT_TXACT    ((uint32_t)0x00001000)
#define SDIO_IT_RXACT    ((uint32_t)0x00002000)
#define SDIO_IT_TXFIFOHE ((uint32_t)0x00004000)
#define SDIO_IT_RXFIFOHF ((uint32_t)0x00008000)
#define SDIO_IT_TXFIFOF  ((uint32_t)0x00010000)
#define SDIO_IT_RXFIFOF  ((uint32_t)0x00020000)
#define SDIO_IT_TXFIFOE  ((uint32_t)0x00040000)
#define SDIO_IT_RXFIFOE  ((uint32_t)0x00080000)
#define SDIO_IT_TXDAVL   ((uint32_t)0x00100000)
#define SDIO_IT_RXDAVL   ((uint32_t)0x00200000)
#define SDIO_IT_SDIOIT   ((uint32_t)0x00400000)
#define SDIO_IT_CEATAEND ((uint32_t)0x00800000)
#define IS_SDIO_IT(IT)   ((((IT) & (uint32_t)0xFF000000) == 0x00) && ((IT) != (uint32_t)0x00))
/**
 * @}
 */

/** @defgroup SDIO_Command_Index
 * @{
 */

#define IS_SDIO_CMD_INDEX(INDEX) ((INDEX) < 0x40)
/**
 * @}
 */

/** @defgroup SDIO_Response_Type
 * @{
 */

#define SDIO_Response_No           ((uint32_t)0x00000000)
#define SDIO_Response_Short        ((uint32_t)0x00000040)
#define SDIO_Response_Long         ((uint32_t)0x000000C0)
#define IS_SDIO_RESPONSE(RESPONSE) (((RESPONSE) == SDIO_Response_No) || ((RESPONSE) == SDIO_Response_Short) || ((RESPONSE) == SDIO_Response_Long))
/**
 * @}
 */

/** @defgroup SDIO_Wait_Interrupt_State
 * @{
 */

#define SDIO_Wait_No       ((uint32_t)0x00000000) /*!< SDIO No Wait, TimeOut is enabled */
#define SDIO_Wait_IT       ((uint32_t)0x00000100) /*!< SDIO Wait Interrupt Request */
#define SDIO_Wait_Pend     ((uint32_t)0x00000200) /*!< SDIO Wait End of transfer */
#define IS_SDIO_WAIT(WAIT) (((WAIT) == SDIO_Wait_No) || ((WAIT) == SDIO_Wait_IT) || ((WAIT) == SDIO_Wait_Pend))
/**
 * @}
 */

/** @defgroup SDIO_CPSM_State
 * @{
 */

#define SDIO_CPSM_Disable  ((uint32_t)0x00000000)
#define SDIO_CPSM_Enable   ((uint32_t)0x00000400)
#define IS_SDIO_CPSM(CPSM) (((CPSM) == SDIO_CPSM_Enable) || ((CPSM) == SDIO_CPSM_Disable))
/**
 * @}
 */

/** @defgroup SDIO_Response_Registers
 * @{
 */

#define SDIO_RESP1         ((uint32_t)0x00000000)
#define SDIO_RESP2         ((uint32_t)0x00000004)
#define SDIO_RESP3         ((uint32_t)0x00000008)
#define SDIO_RESP4         ((uint32_t)0x0000000C)
#define IS_SDIO_RESP(RESP) (((RESP) == SDIO_RESP1) || ((RESP) == SDIO_RESP2) || ((RESP) == SDIO_RESP3) || ((RESP) == SDIO_RESP4))
/**
 * @}
 */

/** @defgroup SDIO_Data_Length
 * @{
 */

#define IS_SDIO_DATA_LENGTH(LENGTH) ((LENGTH) <= 0x01FFFFFF)
/**
 * @}
 */

/** @defgroup SDIO_Data_Block_Size
 * @{
 */

#define SDIO_DataBlockSize_1b     ((uint32_t)0x00000000)
#define SDIO_DataBlockSize_2b     ((uint32_t)0x00000010)
#define SDIO_DataBlockSize_4b     ((uint32_t)0x00000020)
#define SDIO_DataBlockSize_8b     ((uint32_t)0x00000030)
#define SDIO_DataBlockSize_16b    ((uint32_t)0x00000040)
#define SDIO_DataBlockSize_32b    ((uint32_t)0x00000050)
#define SDIO_DataBlockSize_64b    ((uint32_t)0x00000060)
#define SDIO_DataBlockSize_128b   ((uint32_t)0x00000070)
#define SDIO_DataBlockSize_256b   ((uint32_t)0x00000080)
#define SDIO_DataBlockSize_512b   ((uint32_t)0x00000090)
#define SDIO_DataBlockSize_1024b  ((uint32_t)0x000000A0)
#define SDIO_DataBlockSize_2048b  ((uint32_t)0x000000B0)
#define SDIO_DataBlockSize_4096b  ((uint32_t)0x000000C0)
#define SDIO_DataBlockSize_8192b  ((uint32_t)0x000000D0)
#define SDIO_DataBlockSize_16384b ((uint32_t)0x000000E0)
#define IS_SDIO_BLOCK_SIZE(SIZE)                                                                                                                              \
    (((SIZE) == SDIO_DataBlockSize_1b) || ((SIZE) == SDIO_DataBlockSize_2b) || ((SIZE) == SDIO_DataBlockSize_4b) || ((SIZE) == SDIO_DataBlockSize_8b) ||      \
     ((SIZE) == SDIO_DataBlockSize_16b) || ((SIZE) == SDIO_DataBlockSize_32b) || ((SIZE) == SDIO_DataBlockSize_64b) || ((SIZE) == SDIO_DataBlockSize_128b) || \
     ((SIZE) == SDIO_DataBlockSize_256b) || ((SIZE) == SDIO_DataBlockSize_512b) || ((SIZE) == SDIO_DataBlockSize_1024b) ||                                    \
     ((SIZE) == SDIO_DataBlockSize_2048b) || ((SIZE) == SDIO_DataBlockSize_4096b) || ((SIZE) == SDIO_DataBlockSize_8192b) ||                                  \
     ((SIZE) == SDIO_DataBlockSize_16384b))
/**
 * @}
 */

/** @defgroup SDIO_Transfer_Direction
 * @{
 */

#define SDIO_TransferDir_ToCard   ((uint32_t)0x00000000)
#define SDIO_TransferDir_ToSDIO   ((uint32_t)0x00000002)
#define IS_SDIO_TRANSFER_DIR(DIR) (((DIR) == SDIO_TransferDir_ToCard) || ((DIR) == SDIO_TransferDir_ToSDIO))
/**
 * @}
 */

/** @defgroup SDIO_Transfer_Type
 * @{
 */

#define SDIO_TransferMode_Block     ((uint32_t)0x00000000)
#define SDIO_TransferMode_Stream    ((uint32_t)0x00000004)
#define IS_SDIO_TRANSFER_MODE(MODE) (((MODE) == SDIO_TransferMode_Stream) || ((MODE) == SDIO_TransferMode_Block))
/**
 * @}
 */

/** @defgroup SDIO_DPSM_State
 * @{
 */

#define SDIO_DPSM_Disable  ((uint32_t)0x00000000)
#define SDIO_DPSM_Enable   ((uint32_t)0x00000001)
#define IS_SDIO_DPSM(DPSM) (((DPSM) == SDIO_DPSM_Enable) || ((DPSM) == SDIO_DPSM_Disable))
/**
 * @}
 */

/** @defgroup SDIO_Flags
 * @{
 */

#define SDIO_FLAG_CCRCFAIL ((uint32_t)0x00000001)
#define SDIO_FLAG_DCRCFAIL ((uint32_t)0x00000002)
#define SDIO_FLAG_CTIMEOUT ((uint32_t)0x00000004)
#define SDIO_FLAG_DTIMEOUT ((uint32_t)0x00000008)
#define SDIO_FLAG_TXUNDERR ((uint32_t)0x00000010)
#define SDIO_FLAG_RXOVERR  ((uint32_t)0x00000020)
#define SDIO_FLAG_CMDREND  ((uint32_t)0x00000040)
#define SDIO_FLAG_CMDSENT  ((uint32_t)0x00000080)
#define SDIO_FLAG_DATAEND  ((uint32_t)0x00000100)
#define SDIO_FLAG_STBITERR ((uint32_t)0x00000200)
#define SDIO_FLAG_DBCKEND  ((uint32_t)0x00000400)
#define SDIO_FLAG_CMDACT   ((uint32_t)0x00000800)
#define SDIO_FLAG_TXACT    ((uint32_t)0x00001000)
#define SDIO_FLAG_RXACT    ((uint32_t)0x00002000)
#define SDIO_FLAG_TXFIFOHE ((uint32_t)0x00004000)
#define SDIO_FLAG_RXFIFOHF ((uint32_t)0x00008000)
#define SDIO_FLAG_TXFIFOF  ((uint32_t)0x00010000)
#define SDIO_FLAG_RXFIFOF  ((uint32_t)0x00020000)
#define SDIO_FLAG_TXFIFOE  ((uint32_t)0x00040000)
#define SDIO_FLAG_RXFIFOE  ((uint32_t)0x00080000)
#define SDIO_FLAG_TXDAVL   ((uint32_t)0x00100000)
#define SDIO_FLAG_RXDAVL   ((uint32_t)0x00200000)
#define SDIO_FLAG_SDIOIT   ((uint32_t)0x00400000)
#define SDIO_FLAG_CEATAEND ((uint32_t)0x00800000)
#define IS_SDIO_FLAG(FLAG)                                                                                                                   \
    (((FLAG) == SDIO_FLAG_CCRCFAIL) || ((FLAG) == SDIO_FLAG_DCRCFAIL) || ((FLAG) == SDIO_FLAG_CTIMEOUT) || ((FLAG) == SDIO_FLAG_DTIMEOUT) || \
     ((FLAG) == SDIO_FLAG_TXUNDERR) || ((FLAG) == SDIO_FLAG_RXOVERR) || ((FLAG) == SDIO_FLAG_CMDREND) || ((FLAG) == SDIO_FLAG_CMDSENT) ||    \
     ((FLAG) == SDIO_FLAG_DATAEND) || ((FLAG) == SDIO_FLAG_STBITERR) || ((FLAG) == SDIO_FLAG_DBCKEND) || ((FLAG) == SDIO_FLAG_CMDACT) ||     \
     ((FLAG) == SDIO_FLAG_TXACT) || ((FLAG) == SDIO_FLAG_RXACT) || ((FLAG) == SDIO_FLAG_TXFIFOHE) || ((FLAG) == SDIO_FLAG_RXFIFOHF) ||       \
     ((FLAG) == SDIO_FLAG_TXFIFOF) || ((FLAG) == SDIO_FLAG_RXFIFOF) || ((FLAG) == SDIO_FLAG_TXFIFOE) || ((FLAG) == SDIO_FLAG_RXFIFOE) ||     \
     ((FLAG) == SDIO_FLAG_TXDAVL) || ((FLAG) == SDIO_FLAG_RXDAVL) || ((FLAG) == SDIO_FLAG_SDIOIT) || ((FLAG) == SDIO_FLAG_CEATAEND))

#define IS_SDIO_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFF3FF800) == 0x00) && ((FLAG) != (uint32_t)0x00))

#define IS_SDIO_GET_IT(IT)                                                                                                                                 \
    (((IT) == SDIO_IT_CCRCFAIL) || ((IT) == SDIO_IT_DCRCFAIL) || ((IT) == SDIO_IT_CTIMEOUT) || ((IT) == SDIO_IT_DTIMEOUT) || ((IT) == SDIO_IT_TXUNDERR) || \
     ((IT) == SDIO_IT_RXOVERR) || ((IT) == SDIO_IT_CMDREND) || ((IT) == SDIO_IT_CMDSENT) || ((IT) == SDIO_IT_DATAEND) || ((IT) == SDIO_IT_STBITERR) ||     \
     ((IT) == SDIO_IT_DBCKEND) || ((IT) == SDIO_IT_CMDACT) || ((IT) == SDIO_IT_TXACT) || ((IT) == SDIO_IT_RXACT) || ((IT) == SDIO_IT_TXFIFOHE) ||          \
     ((IT) == SDIO_IT_RXFIFOHF) || ((IT) == SDIO_IT_TXFIFOF) || ((IT) == SDIO_IT_RXFIFOF) || ((IT) == SDIO_IT_TXFIFOE) || ((IT) == SDIO_IT_RXFIFOE) ||     \
     ((IT) == SDIO_IT_TXDAVL) || ((IT) == SDIO_IT_RXDAVL) || ((IT) == SDIO_IT_SDIOIT) || ((IT) == SDIO_IT_CEATAEND))

#define IS_SDIO_CLEAR_IT(IT) ((((IT) & (uint32_t)0xFF3FF800) == 0x00) && ((IT) != (uint32_t)0x00))

/**
 * @}
 */

/** @defgroup SDIO_Read_Wait_Mode
 * @{
 */

#define SDIO_ReadWaitMode_DATA2     ((uint32_t)0x00000000)
#define SDIO_ReadWaitMode_CLK       ((uint32_t)0x00000001)
#define IS_SDIO_READWAIT_MODE(MODE) (((MODE) == SDIO_ReadWaitMode_CLK) || ((MODE) == SDIO_ReadWaitMode_DATA2))
/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*  Function used to set the SDIO configuration to the default reset state ****/
void SDIO_DeInit(void);

/* Initialization and Configuration functions *********************************/
void     SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct);
void     SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct);
void     SDIO_ClockCmd(FunctionalState NewState);
void     SDIO_SetPowerState(uint32_t SDIO_PowerState);
uint32_t SDIO_GetPowerState(void);

/* Command path state machine (CPSM) management functions *********************/
void     SDIO_SendCommand(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct);
void     SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct);
uint8_t  SDIO_GetCommandResponse(void);
uint32_t SDIO_GetResponse(uint32_t SDIO_RESP);

/* Data path state machine (DPSM) management functions ************************/
void     SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
void     SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
uint32_t SDIO_GetDataCounter(void);
uint32_t SDIO_ReadData(void);
void     SDIO_WriteData(uint32_t Data);
uint32_t SDIO_GetFIFOCount(void);

/* SDIO IO Cards mode management functions ************************************/
void SDIO_StartSDIOReadWait(FunctionalState NewState);
void SDIO_StopSDIOReadWait(FunctionalState NewState);
void SDIO_SetSDIOReadWaitMode(uint32_t SDIO_ReadWaitMode);
void SDIO_SetSDIOOperation(FunctionalState NewState);
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState);

/* CE-ATA mode management functions *******************************************/
void SDIO_CommandCompletionCmd(FunctionalState NewState);
void SDIO_CEATAITCmd(FunctionalState NewState);
void SDIO_SendCEATACmd(FunctionalState NewState);

/* DMA transfers management functions *****************************************/
void SDIO_DMACmd(FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void       SDIO_ITConfig(uint32_t SDIO_IT, FunctionalState NewState);
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG);
void       SDIO_ClearFlag(uint32_t SDIO_FLAG);
ITStatus   SDIO_GetITStatus(uint32_t SDIO_IT);
void       SDIO_ClearITPendingBit(uint32_t SDIO_IT);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_SDIO_H__
