// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_USART_H__
#define __PERIPH_USART_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Feature Definition  -------------------------------------------- */

#ifndef FEATURE_USART_RXFIFO_PRESENT
#define FEATURE_USART_RXFIFO_PRESENT 0
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint16_t SR;        /*!< USART Status register,                   Address offset: 0x00 */
    uint16_t      RESERVED0; /*!< Reserved, 0x02                                                */
    __IO uint16_t DR;        /*!< USART Data register,                     Address offset: 0x04 */
    uint16_t      RESERVED1; /*!< Reserved, 0x06                                                */
    __IO uint16_t BRR;       /*!< USART Baud rate register,                Address offset: 0x08 */
    uint16_t      RESERVED2; /*!< Reserved, 0x0A                                                */
    __IO uint16_t CR1;       /*!< USART Control register 1,                Address offset: 0x0C */
    uint16_t      RESERVED3; /*!< Reserved, 0x0E                                                */
    __IO uint16_t CR2;       /*!< USART Control register 2,                Address offset: 0x10 */
    uint16_t      RESERVED4; /*!< Reserved, 0x12                                                */
    __IO uint16_t CR3;       /*!< USART Control register 3,                Address offset: 0x14 */
    uint16_t      RESERVED5; /*!< Reserved, 0x16                                                */
    __IO uint16_t GTPR;      /*!< USART Guard time and prescaler register, Address offset: 0x18 */
    uint16_t      RESERVED6; /*!< Reserved, 0x1A                                                */
#if FEATURE_USART_RXFIFO_PRESENT
    __IO uint16_t CR4;       /*!< USART Control register 4,                Address offset: 0x1C */
    uint16_t      RESERVED7; /*!< Reserved, 0x1E                                                */
#endif
} USART_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/*******************  Bit definition for USART_SR register  *******************/
#define USART_SR_PE   ((uint16_t)0x0001) /*!<Parity Error                 */
#define USART_SR_FE   ((uint16_t)0x0002) /*!<Framing Error                */
#define USART_SR_NE   ((uint16_t)0x0004) /*!<Noise Error Flag             */
#define USART_SR_ORE  ((uint16_t)0x0008) /*!<OverRun Error                */
#define USART_SR_IDLE ((uint16_t)0x0010) /*!<IDLE line detected           */
#define USART_SR_RXNE ((uint16_t)0x0020) /*!<Read Data Register Not Empty */
#define USART_SR_TC   ((uint16_t)0x0040) /*!<Transmission Complete        */
#define USART_SR_TXE  ((uint16_t)0x0080) /*!<Transmit Data Register Empty */
#define USART_SR_LBD  ((uint16_t)0x0100) /*!<LIN Break Detection Flag     */
#define USART_SR_CTS  ((uint16_t)0x0200) /*!<CTS Flag                     */

/*******************  Bit definition for USART_DR register  *******************/
#define USART_DR_DR ((uint16_t)0x01FF) /*!<Data value */

/******************  Bit definition for USART_BRR register  *******************/
#define USART_BRR_DIV_Fraction ((uint16_t)0x000F) /*!<Fraction of USARTDIV */
#define USART_BRR_DIV_Mantissa ((uint16_t)0xFFF0) /*!<Mantissa of USARTDIV */

/******************  Bit definition for USART_CR1 register  *******************/
#define USART_CR1_SBK    ((uint16_t)0x0001) /*!<Send Break                             */
#define USART_CR1_RWU    ((uint16_t)0x0002) /*!<Receiver wakeup                        */
#define USART_CR1_RE     ((uint16_t)0x0004) /*!<Receiver Enable                        */
#define USART_CR1_TE     ((uint16_t)0x0008) /*!<Transmitter Enable                     */
#define USART_CR1_IDLEIE ((uint16_t)0x0010) /*!<IDLE Interrupt Enable                  */
#define USART_CR1_RXNEIE ((uint16_t)0x0020) /*!<RXNE Interrupt Enable                  */
#define USART_CR1_TCIE   ((uint16_t)0x0040) /*!<Transmission Complete Interrupt Enable */
#define USART_CR1_TXEIE  ((uint16_t)0x0080) /*!<PE Interrupt Enable                    */
#define USART_CR1_PEIE   ((uint16_t)0x0100) /*!<PE Interrupt Enable                    */
#define USART_CR1_PS     ((uint16_t)0x0200) /*!<Parity Selection                       */
#define USART_CR1_PCE    ((uint16_t)0x0400) /*!<Parity Control Enable                  */
#define USART_CR1_WAKE   ((uint16_t)0x0800) /*!<Wakeup method                          */
#define USART_CR1_M      ((uint16_t)0x1000) /*!<Word length                            */
#define USART_CR1_UE     ((uint16_t)0x2000) /*!<USART Enable                           */
#define USART_CR1_OVER8  ((uint16_t)0x8000) /*!<USART Oversampling by 8 enable         */

/******************  Bit definition for USART_CR2 register  *******************/
#define USART_CR2_ADD   ((uint16_t)0x000F) /*!<Address of the USART node            */
#define USART_CR2_LBDL  ((uint16_t)0x0020) /*!<LIN Break Detection Length           */
#define USART_CR2_LBDIE ((uint16_t)0x0040) /*!<LIN Break Detection Interrupt Enable */
#define USART_CR2_LBCL  ((uint16_t)0x0100) /*!<Last Bit Clock pulse                 */
#define USART_CR2_CPHA  ((uint16_t)0x0200) /*!<Clock Phase                          */
#define USART_CR2_CPOL  ((uint16_t)0x0400) /*!<Clock Polarity                       */
#define USART_CR2_CLKEN ((uint16_t)0x0800) /*!<Clock Enable                         */

#define USART_CR2_STOP   ((uint16_t)0x3000) /*!<STOP[1:0] bits (STOP bits) */
#define USART_CR2_STOP_0 ((uint16_t)0x1000) /*!<Bit 0 */
#define USART_CR2_STOP_1 ((uint16_t)0x2000) /*!<Bit 1 */

#define USART_CR2_LINEN ((uint16_t)0x4000) /*!<LIN mode enable */

/******************  Bit definition for USART_CR3 register  *******************/
#define USART_CR3_EIE    ((uint16_t)0x0001) /*!<Error Interrupt Enable      */
#define USART_CR3_IREN   ((uint16_t)0x0002) /*!<IrDA mode Enable            */
#define USART_CR3_IRLP   ((uint16_t)0x0004) /*!<IrDA Low-Power              */
#define USART_CR3_HDSEL  ((uint16_t)0x0008) /*!<Half-Duplex Selection       */
#define USART_CR3_NACK   ((uint16_t)0x0010) /*!<Smartcard NACK enable       */
#define USART_CR3_SCEN   ((uint16_t)0x0020) /*!<Smartcard mode enable       */
#define USART_CR3_DMAR   ((uint16_t)0x0040) /*!<DMA Enable Receiver         */
#define USART_CR3_DMAT   ((uint16_t)0x0080) /*!<DMA Enable Transmitter      */
#define USART_CR3_RTSE   ((uint16_t)0x0100) /*!<RTS Enable                  */
#define USART_CR3_CTSE   ((uint16_t)0x0200) /*!<CTS Enable                  */
#define USART_CR3_CTSIE  ((uint16_t)0x0400) /*!<CTS Interrupt Enable        */
#define USART_CR3_ONEBIT ((uint16_t)0x0800) /*!<USART One bit method enable */

/******************  Bit definition for USART_GTPR register  ******************/
#define USART_GTPR_PSC   ((uint16_t)0x00FF) /*!<PSC[7:0] bits (Prescaler value) */
#define USART_GTPR_PSC_0 ((uint16_t)0x0001) /*!<Bit 0 */
#define USART_GTPR_PSC_1 ((uint16_t)0x0002) /*!<Bit 1 */
#define USART_GTPR_PSC_2 ((uint16_t)0x0004) /*!<Bit 2 */
#define USART_GTPR_PSC_3 ((uint16_t)0x0008) /*!<Bit 3 */
#define USART_GTPR_PSC_4 ((uint16_t)0x0010) /*!<Bit 4 */
#define USART_GTPR_PSC_5 ((uint16_t)0x0020) /*!<Bit 5 */
#define USART_GTPR_PSC_6 ((uint16_t)0x0040) /*!<Bit 6 */
#define USART_GTPR_PSC_7 ((uint16_t)0x0080) /*!<Bit 7 */

#define USART_GTPR_GT ((uint16_t)0xFF00) /*!<Guard time value */

/* --------  Exported types  ------------------------------------------------ */

// USART Init Structure definition
typedef struct
{
    uint32_t USART_BaudRate; /*!< This member configures the USART communication baud rate.
                                  The baud rate is computed using the following formula:
                                   - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (USART_InitStruct->USART_BaudRate)))
                                   - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 8 * (OVR8+1)) + 0.5
                                  Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */

    uint16_t USART_WordLength; /*!< Specifies the number of data bits transmitted or received in a frame.
                                    This parameter can be a value of @ref USART_Word_Length */

    uint16_t USART_StopBits; /*!< Specifies the number of stop bits transmitted.
                                  This parameter can be a value of @ref USART_Stop_Bits */

    uint16_t USART_Parity; /*!< Specifies the parity mode.
                                This parameter can be a value of @ref USART_Parity
                                @note When parity is enabled, the computed parity is inserted
                                      at the MSB position of the transmitted data (9th bit when
                                      the word length is set to 9 data bits; 8th bit when the
                                      word length is set to 8 data bits). */

    uint16_t USART_Mode; /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                              This parameter can be a value of @ref USART_Mode */

    uint16_t USART_HardwareFlowControl; /*!< Specifies whether the hardware flow control mode is enabled
                                             or disabled.
                                             This parameter can be a value of @ref USART_Hardware_Flow_Control */
#if FEATURE_USART_RXFIFO_PRESENT
    uint16_t USART_RxFIFO; /*!< Specifies whether the Rx FIFO is enabled or disabled.
                                This parameter can be a value of @ref USART_Rx_FIFO */
#endif
} USART_InitTypeDef;

// USART Clock Init Structure definition
typedef struct
{

    uint16_t USART_Clock; /*!< Specifies whether the USART clock is enabled or disabled.
                               This parameter can be a value of @ref USART_Clock */

    uint16_t USART_CPOL; /*!< Specifies the steady state of the serial clock.
                              This parameter can be a value of @ref USART_Clock_Polarity */

    uint16_t USART_CPHA; /*!< Specifies the clock transition on which the bit capture is made.
                              This parameter can be a value of @ref USART_Clock_Phase */

    uint16_t USART_LastBit; /*!< Specifies whether the clock pulse corresponding to the last transmitted
                                 data bit (MSB) has to be output on the SCLK pin in synchronous mode.
                                 This parameter can be a value of @ref USART_Last_Bit */
} USART_ClockInitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup USART_Exported_Constants
 * @{
 */

#define IS_USART_ALL_PERIPH(PERIPH) (PeripheralFind(PERIPH, PeripheralUARTStart, PeripheralUARTEnd) != PeripheralNull)
#define IS_USART_PERIPH(PERIPH)     (PeripheralFind(PERIPH, PeripheralUSARTStart, PeripheralUSARTEnd) != PeripheralNull)

/** @defgroup USART_Word_Length
 * @{
 */

#define USART_WordLength_8b ((uint16_t)0x0000)
#define USART_WordLength_9b ((uint16_t)0x1000)

#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || ((LENGTH) == USART_WordLength_9b))

/** @defgroup USART_Stop_Bits
 * @{
 */

#define USART_StopBits_1   ((uint16_t)0x0000)
#define USART_StopBits_0_5 ((uint16_t)0x1000)
#define USART_StopBits_2   ((uint16_t)0x2000)
#define USART_StopBits_1_5 ((uint16_t)0x3000)
#define IS_USART_STOPBITS(STOPBITS) \
    (((STOPBITS) == USART_StopBits_1) || ((STOPBITS) == USART_StopBits_0_5) || ((STOPBITS) == USART_StopBits_2) || ((STOPBITS) == USART_StopBits_1_5))

/** @defgroup USART_Parity
 * @{
 */

#define USART_Parity_No         ((uint16_t)0x0000)
#define USART_Parity_Even       ((uint16_t)0x0400)
#define USART_Parity_Odd        ((uint16_t)0x0600)
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || ((PARITY) == USART_Parity_Even) || ((PARITY) == USART_Parity_Odd))

/** @defgroup USART_Mode
 * @{
 */

#define USART_Mode_Rx       ((uint16_t)0x0004)
#define USART_Mode_Tx       ((uint16_t)0x0008)
#define IS_USART_MODE(MODE) ((((MODE) & (uint16_t)0xFFF3) == 0x00) && ((MODE) != (uint16_t)0x00))

/** @defgroup USART_Hardware_Flow_Control
 * @{
 */
#define USART_HardwareFlowControl_None    ((uint16_t)0x0000)
#define USART_HardwareFlowControl_RTS     ((uint16_t)0x0100)
#define USART_HardwareFlowControl_CTS     ((uint16_t)0x0200)
#define USART_HardwareFlowControl_RTS_CTS ((uint16_t)0x0300)
#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)                                                                                                       \
    (((CONTROL) == USART_HardwareFlowControl_None) || ((CONTROL) == USART_HardwareFlowControl_RTS) || ((CONTROL) == USART_HardwareFlowControl_CTS) || \
     ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

#if FEATURE_USART_RXFIFO_PRESENT
/** @defgroup USART_Rx_FIFO
 * @{
 */
#define USART_RxFIFO_Disable 0
#define USART_RxFIFO_Enable  1
#endif

/** @defgroup USART_Clock
 * @{
 */
#define USART_Clock_Disable   ((uint16_t)0x0000)
#define USART_Clock_Enable    ((uint16_t)0x0800)
#define IS_USART_CLOCK(CLOCK) (((CLOCK) == USART_Clock_Disable) || ((CLOCK) == USART_Clock_Enable))

/** @defgroup USART_Clock_Polarity
 * @{
 */

#define USART_CPOL_Low      ((uint16_t)0x0000)
#define USART_CPOL_High     ((uint16_t)0x0400)
#define IS_USART_CPOL(CPOL) (((CPOL) == USART_CPOL_Low) || ((CPOL) == USART_CPOL_High))

/** @defgroup USART_Clock_Phase
 * @{
 */

#define USART_CPHA_1Edge    ((uint16_t)0x0000)
#define USART_CPHA_2Edge    ((uint16_t)0x0200)
#define IS_USART_CPHA(CPHA) (((CPHA) == USART_CPHA_1Edge) || ((CPHA) == USART_CPHA_2Edge))

/** @defgroup USART_Last_Bit
 * @{
 */

#define USART_LastBit_Disable     ((uint16_t)0x0000)
#define USART_LastBit_Enable      ((uint16_t)0x0100)
#define IS_USART_LASTBIT(LASTBIT) (((LASTBIT) == USART_LastBit_Disable) || ((LASTBIT) == USART_LastBit_Enable))

/** @defgroup USART_Interrupt_definition
 * @{
 */

#define USART_IT_PE     ((uint16_t)0x0028)
#define USART_IT_TXE    ((uint16_t)0x0727)
#define USART_IT_TC     ((uint16_t)0x0626)
#define USART_IT_RXNE   ((uint16_t)0x0525)
#define USART_IT_ORE_RX ((uint16_t)0x0325) /* In case interrupt is generated if the RXNEIE bit is set */
#define USART_IT_IDLE   ((uint16_t)0x0424)
#define USART_IT_LBD    ((uint16_t)0x0846)
#define USART_IT_CTS    ((uint16_t)0x096A)
#define USART_IT_ERR    ((uint16_t)0x0060)
#define USART_IT_ORE_ER ((uint16_t)0x0360) /* In case interrupt is generated if the EIE bit is set */
#define USART_IT_NE     ((uint16_t)0x0260)
#define USART_IT_FE     ((uint16_t)0x0160)
#define USART_IT_RFAF   ((uint16_t)0x0A84) /* Rx FIFO almost full. Avaliable only when FEATURE_USART_RXFIFO_PRESENT == 1.*/

/** @defgroup USART_Legacy
 * @{
 */
#define USART_IT_ORE USART_IT_ORE_ER

#define IS_USART_CONFIG_IT(IT)                                                                                                         \
    (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXE) || ((IT) == USART_IT_TC) || ((IT) == USART_IT_RXNE) || ((IT) == USART_IT_IDLE) || \
     ((IT) == USART_IT_LBD) || ((IT) == USART_IT_CTS) || ((IT) == USART_IT_ERR) || ((IT) == USART_IT_RFAF))

#define IS_USART_GET_IT(IT)                                                                                                                  \
    (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXE) || ((IT) == USART_IT_TC) || ((IT) == USART_IT_RXNE) || ((IT) == USART_IT_IDLE) ||       \
     ((IT) == USART_IT_LBD) || ((IT) == USART_IT_CTS) || ((IT) == USART_IT_ORE) || ((IT) == USART_IT_ORE_RX) || ((IT) == USART_IT_ORE_ER) || \
     ((IT) == USART_IT_NE) || ((IT) == USART_IT_FE) || ((IT) == USART_IT_RFAF))

#define IS_USART_CLEAR_IT(IT) (((IT) == USART_IT_TC) || ((IT) == USART_IT_RXNE) || ((IT) == USART_IT_LBD) || ((IT) == USART_IT_CTS))

/** @defgroup USART_DMA_Requests
 * @{
 */

#define USART_DMAReq_Tx         ((uint16_t)0x0080)
#define USART_DMAReq_Rx         ((uint16_t)0x0040)
#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFF3F) == 0x00) && ((DMAREQ) != (uint16_t)0x00))

/** @defgroup USART_WakeUp_methods
 * @{
 */

#define USART_WakeUp_IdleLine    ((uint16_t)0x0000)
#define USART_WakeUp_AddressMark ((uint16_t)0x0800)
#define IS_USART_WAKEUP(WAKEUP)  (((WAKEUP) == USART_WakeUp_IdleLine) || ((WAKEUP) == USART_WakeUp_AddressMark))

/** @defgroup USART_LIN_Break_Detection_Length
 * @{
 */

#define USART_LINBreakDetectLength_10b           ((uint16_t)0x0000)
#define USART_LINBreakDetectLength_11b           ((uint16_t)0x0020)
#define IS_USART_LIN_BREAK_DETECT_LENGTH(LENGTH) (((LENGTH) == USART_LINBreakDetectLength_10b) || ((LENGTH) == USART_LINBreakDetectLength_11b))

/** @defgroup USART_IrDA_Low_Power
 * @{
 */

#define USART_IrDAMode_LowPower  ((uint16_t)0x0004)
#define USART_IrDAMode_Normal    ((uint16_t)0x0000)
#define IS_USART_IRDA_MODE(MODE) (((MODE) == USART_IrDAMode_LowPower) || ((MODE) == USART_IrDAMode_Normal))

/** @defgroup USART_Flags
 * @{
 */

#define USART_FLAG_RFAF ((uint16_t)0x0400) /* Avaliable only when FEATURE_USART_RXFIFO_PRESENT == 1.*/
#define USART_FLAG_CTS  ((uint16_t)0x0200)
#define USART_FLAG_LBD  ((uint16_t)0x0100)
#define USART_FLAG_TXE  ((uint16_t)0x0080)
#define USART_FLAG_TC   ((uint16_t)0x0040)
#define USART_FLAG_RXNE ((uint16_t)0x0020)
#define USART_FLAG_IDLE ((uint16_t)0x0010)
#define USART_FLAG_ORE  ((uint16_t)0x0008)
#define USART_FLAG_NE   ((uint16_t)0x0004)
#define USART_FLAG_FE   ((uint16_t)0x0002)
#define USART_FLAG_PE   ((uint16_t)0x0001)
#define IS_USART_FLAG(FLAG)                                                                                                                                \
    (((FLAG) == USART_FLAG_PE) || ((FLAG) == USART_FLAG_TXE) || ((FLAG) == USART_FLAG_TC) || ((FLAG) == USART_FLAG_RXNE) || ((FLAG) == USART_FLAG_IDLE) || \
     ((FLAG) == USART_FLAG_LBD) || ((FLAG) == USART_FLAG_CTS) || ((FLAG) == USART_FLAG_ORE) || ((FLAG) == USART_FLAG_NE) || ((FLAG) == USART_FLAG_FE) ||   \
     ((FLAG) == USART_FLAG_RFAF))

#define IS_USART_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFC9F) == 0x00) && ((FLAG) != (uint16_t)0x00))

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 10500001))
#define IS_USART_ADDRESS(ADDRESS)   ((ADDRESS) <= 0xF)
#define IS_USART_DATA(DATA)         ((DATA) <= 0x1FF)

/* --------  Public Macro Function  ----------------------------------------- */
#if FEATURE_USART_RXFIFO_PRESENT
#define USART_GetRxFIFOLevel(USARTx) ((USARTx->CR4 >> 6) & BITS(3, 0))
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the USART configuration to the default reset state ***/
void USART_DeInit(USART_TypeDef* USARTx);

/* Initialization and Configuration functions *********************************/
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SetPrescaler(USART_TypeDef* USARTx, uint8_t USART_Prescaler);
void USART_OverSampling8Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_OneBitMethodCmd(USART_TypeDef* USARTx, FunctionalState NewState);

/* Data transfers functions ***************************************************/
void     USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
uint16_t USART_ReceiveData(USART_TypeDef* USARTx);

/* Multi-Processor Communication functions ************************************/
void USART_SetAddress(USART_TypeDef* USARTx, uint8_t USART_Address);
void USART_WakeUpConfig(USART_TypeDef* USARTx, uint16_t USART_WakeUp);
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState);

/* LIN mode functions *********************************************************/
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength);
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendBreak(USART_TypeDef* USARTx);

/* Half-duplex mode function **************************************************/
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState);

/* Smartcard mode functions ***************************************************/
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SetGuardTime(USART_TypeDef* USARTx, uint8_t USART_GuardTime);

/* IrDA mode functions ********************************************************/
void USART_IrDAConfig(USART_TypeDef* USARTx, uint16_t USART_IrDAMode);
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState);

/* DMA transfers management functions *****************************************/
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void       USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
void       USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG);
ITStatus   USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT);
void       USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_USART_H__
