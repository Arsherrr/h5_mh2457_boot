// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh2457.h"

#ifndef __PERIPH_CAN_H__
#define __PERIPH_CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

/**
 * @brief Controller Area Network TxMailBox
 */
typedef struct
{
    __IO uint32_t TIR;  /*!< CAN TX mailbox identifier register */
    __IO uint32_t TDTR; /*!< CAN mailbox data length control and time stamp register */
    __IO uint32_t TDLR; /*!< CAN mailbox data low register */
    __IO uint32_t TDHR; /*!< CAN mailbox data high register */
} CAN_TxMailBox_TypeDef;

/**
 * @brief Controller Area Network FIFOMailBox
 */
typedef struct
{
    __IO uint32_t RIR;  /*!< CAN receive FIFO mailbox identifier register */
    __IO uint32_t RDTR; /*!< CAN receive FIFO mailbox data length control and time stamp register */
    __IO uint32_t RDLR; /*!< CAN receive FIFO mailbox data low register */
    __IO uint32_t RDHR; /*!< CAN receive FIFO mailbox data high register */
} CAN_FIFOMailBox_TypeDef;

/**
 * @brief Controller Area Network FilterRegister
 */
typedef struct
{
    __IO uint32_t FR1; /*!< CAN Filter bank register 1 */
    __IO uint32_t FR2; /*!< CAN Filter bank register 1 */
} CAN_FilterRegister_TypeDef;

/**
 * @brief Controller Area Network
 */
typedef struct
{
    __IO uint32_t              MCR;                 /*!< CAN master control register,         Address offset: 0x00          */
    __IO uint32_t              MSR;                 /*!< CAN master status register,          Address offset: 0x04          */
    __IO uint32_t              TSR;                 /*!< CAN transmit status register,        Address offset: 0x08          */
    __IO uint32_t              RF0R;                /*!< CAN receive FIFO 0 register,         Address offset: 0x0C          */
    __IO uint32_t              RF1R;                /*!< CAN receive FIFO 1 register,         Address offset: 0x10          */
    __IO uint32_t              IER;                 /*!< CAN interrupt enable register,       Address offset: 0x14          */
    __IO uint32_t              ESR;                 /*!< CAN error status register,           Address offset: 0x18          */
    __IO uint32_t              BTR;                 /*!< CAN bit timing register,             Address offset: 0x1C          */
    uint32_t                   RESERVED0[88];       /*!< Reserved, 0x020 - 0x17F                                            */
    CAN_TxMailBox_TypeDef      sTxMailBox[3];       /*!< CAN Tx MailBox,                      Address offset: 0x180 - 0x1AC */
    CAN_FIFOMailBox_TypeDef    sFIFOMailBox[2];     /*!< CAN FIFO MailBox,                    Address offset: 0x1B0 - 0x1CC */
    uint32_t                   RESERVED1[12];       /*!< Reserved, 0x1D0 - 0x1FF                                            */
    __IO uint32_t              FMR;                 /*!< CAN filter master register,          Address offset: 0x200         */
    __IO uint32_t              FM1R;                /*!< CAN filter mode register,            Address offset: 0x204         */
    uint32_t                   RESERVED2;           /*!< Reserved, 0x208                                                    */
    __IO uint32_t              FS1R;                /*!< CAN filter scale register,           Address offset: 0x20C         */
    uint32_t                   RESERVED3;           /*!< Reserved, 0x210                                                    */
    __IO uint32_t              FFA1R;               /*!< CAN filter FIFO assignment register, Address offset: 0x214         */
    uint32_t                   RESERVED4;           /*!< Reserved, 0x218                                                    */
    __IO uint32_t              FA1R;                /*!< CAN filter activation register,      Address offset: 0x21C         */
    uint32_t                   RESERVED5[8];        /*!< Reserved, 0x220-0x23F                                              */
    CAN_FilterRegister_TypeDef sFilterRegister[28]; /*!< CAN Filter Register,                 Address offset: 0x240-0x31C   */
} CAN_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */
/*!<CAN control and status registers */
/*******************  Bit definition for CAN_MCR register  ********************/
#define CAN_MCR_INRQ  ((uint16_t)0x0001) /*!<Initialization Request */
#define CAN_MCR_SLEEP ((uint16_t)0x0002) /*!<Sleep Mode Request */
#define CAN_MCR_TXFP  ((uint16_t)0x0004) /*!<Transmit FIFO Priority */
#define CAN_MCR_RFLM  ((uint16_t)0x0008) /*!<Receive FIFO Locked Mode */
#define CAN_MCR_NART  ((uint16_t)0x0010) /*!<No Automatic Retransmission */
#define CAN_MCR_AWUM  ((uint16_t)0x0020) /*!<Automatic Wakeup Mode */
#define CAN_MCR_ABOM  ((uint16_t)0x0040) /*!<Automatic Bus-Off Management */
#define CAN_MCR_TTCM  ((uint16_t)0x0080) /*!<Time Triggered Communication Mode */
#define CAN_MCR_RESET ((uint16_t)0x8000) /*!<bxCAN software master reset */

/*******************  Bit definition for CAN_MSR register  ********************/
#define CAN_MSR_INAK  ((uint16_t)0x0001) /*!<Initialization Acknowledge */
#define CAN_MSR_SLAK  ((uint16_t)0x0002) /*!<Sleep Acknowledge */
#define CAN_MSR_ERRI  ((uint16_t)0x0004) /*!<Error Interrupt */
#define CAN_MSR_WKUI  ((uint16_t)0x0008) /*!<Wakeup Interrupt */
#define CAN_MSR_SLAKI ((uint16_t)0x0010) /*!<Sleep Acknowledge Interrupt */
#define CAN_MSR_TXM   ((uint16_t)0x0100) /*!<Transmit Mode */
#define CAN_MSR_RXM   ((uint16_t)0x0200) /*!<Receive Mode */
#define CAN_MSR_SAMP  ((uint16_t)0x0400) /*!<Last Sample Point */
#define CAN_MSR_RX    ((uint16_t)0x0800) /*!<CAN Rx Signal */

/*******************  Bit definition for CAN_TSR register  ********************/
#define CAN_TSR_RQCP0 ((uint32_t)0x00000001) /*!<Request Completed Mailbox0 */
#define CAN_TSR_TXOK0 ((uint32_t)0x00000002) /*!<Transmission OK of Mailbox0 */
#define CAN_TSR_ALST0 ((uint32_t)0x00000004) /*!<Arbitration Lost for Mailbox0 */
#define CAN_TSR_TERR0 ((uint32_t)0x00000008) /*!<Transmission Error of Mailbox0 */
#define CAN_TSR_ABRQ0 ((uint32_t)0x00000080) /*!<Abort Request for Mailbox0 */
#define CAN_TSR_RQCP1 ((uint32_t)0x00000100) /*!<Request Completed Mailbox1 */
#define CAN_TSR_TXOK1 ((uint32_t)0x00000200) /*!<Transmission OK of Mailbox1 */
#define CAN_TSR_ALST1 ((uint32_t)0x00000400) /*!<Arbitration Lost for Mailbox1 */
#define CAN_TSR_TERR1 ((uint32_t)0x00000800) /*!<Transmission Error of Mailbox1 */
#define CAN_TSR_ABRQ1 ((uint32_t)0x00008000) /*!<Abort Request for Mailbox 1 */
#define CAN_TSR_RQCP2 ((uint32_t)0x00010000) /*!<Request Completed Mailbox2 */
#define CAN_TSR_TXOK2 ((uint32_t)0x00020000) /*!<Transmission OK of Mailbox 2 */
#define CAN_TSR_ALST2 ((uint32_t)0x00040000) /*!<Arbitration Lost for mailbox 2 */
#define CAN_TSR_TERR2 ((uint32_t)0x00080000) /*!<Transmission Error of Mailbox 2 */
#define CAN_TSR_ABRQ2 ((uint32_t)0x00800000) /*!<Abort Request for Mailbox 2 */
#define CAN_TSR_CODE  ((uint32_t)0x03000000) /*!<Mailbox Code */

#define CAN_TSR_TME  ((uint32_t)0x1C000000) /*!<TME[2:0] bits */
#define CAN_TSR_TME0 ((uint32_t)0x04000000) /*!<Transmit Mailbox 0 Empty */
#define CAN_TSR_TME1 ((uint32_t)0x08000000) /*!<Transmit Mailbox 1 Empty */
#define CAN_TSR_TME2 ((uint32_t)0x10000000) /*!<Transmit Mailbox 2 Empty */

#define CAN_TSR_LOW  ((uint32_t)0xE0000000) /*!<LOW[2:0] bits */
#define CAN_TSR_LOW0 ((uint32_t)0x20000000) /*!<Lowest Priority Flag for Mailbox 0 */
#define CAN_TSR_LOW1 ((uint32_t)0x40000000) /*!<Lowest Priority Flag for Mailbox 1 */
#define CAN_TSR_LOW2 ((uint32_t)0x80000000) /*!<Lowest Priority Flag for Mailbox 2 */

/*******************  Bit definition for CAN_RF0R register  *******************/
#define CAN_RF0R_FMP0  ((uint8_t)0x03) /*!<FIFO 0 Message Pending */
#define CAN_RF0R_FULL0 ((uint8_t)0x08) /*!<FIFO 0 Full */
#define CAN_RF0R_FOVR0 ((uint8_t)0x10) /*!<FIFO 0 Overrun */
#define CAN_RF0R_RFOM0 ((uint8_t)0x20) /*!<Release FIFO 0 Output Mailbox */

/*******************  Bit definition for CAN_RF1R register  *******************/
#define CAN_RF1R_FMP1  ((uint8_t)0x03) /*!<FIFO 1 Message Pending */
#define CAN_RF1R_FULL1 ((uint8_t)0x08) /*!<FIFO 1 Full */
#define CAN_RF1R_FOVR1 ((uint8_t)0x10) /*!<FIFO 1 Overrun */
#define CAN_RF1R_RFOM1 ((uint8_t)0x20) /*!<Release FIFO 1 Output Mailbox */

/********************  Bit definition for CAN_IER register  *******************/
#define CAN_IER_TMEIE  ((uint32_t)0x00000001) /*!<Transmit Mailbox Empty Interrupt Enable */
#define CAN_IER_FMPIE0 ((uint32_t)0x00000002) /*!<FIFO Message Pending Interrupt Enable */
#define CAN_IER_FFIE0  ((uint32_t)0x00000004) /*!<FIFO Full Interrupt Enable */
#define CAN_IER_FOVIE0 ((uint32_t)0x00000008) /*!<FIFO Overrun Interrupt Enable */
#define CAN_IER_FMPIE1 ((uint32_t)0x00000010) /*!<FIFO Message Pending Interrupt Enable */
#define CAN_IER_FFIE1  ((uint32_t)0x00000020) /*!<FIFO Full Interrupt Enable */
#define CAN_IER_FOVIE1 ((uint32_t)0x00000040) /*!<FIFO Overrun Interrupt Enable */
#define CAN_IER_EWGIE  ((uint32_t)0x00000100) /*!<Error Warning Interrupt Enable */
#define CAN_IER_EPVIE  ((uint32_t)0x00000200) /*!<Error Passive Interrupt Enable */
#define CAN_IER_BOFIE  ((uint32_t)0x00000400) /*!<Bus-Off Interrupt Enable */
#define CAN_IER_LECIE  ((uint32_t)0x00000800) /*!<Last Error Code Interrupt Enable */
#define CAN_IER_ERRIE  ((uint32_t)0x00008000) /*!<Error Interrupt Enable */
#define CAN_IER_WKUIE  ((uint32_t)0x00010000) /*!<Wakeup Interrupt Enable */
#define CAN_IER_SLKIE  ((uint32_t)0x00020000) /*!<Sleep Interrupt Enable */

/********************  Bit definition for CAN_ESR register  *******************/
#define CAN_ESR_EWGF ((uint32_t)0x00000001) /*!<Error Warning Flag */
#define CAN_ESR_EPVF ((uint32_t)0x00000002) /*!<Error Passive Flag */
#define CAN_ESR_BOFF ((uint32_t)0x00000004) /*!<Bus-Off Flag */

#define CAN_ESR_LEC   ((uint32_t)0x00000070) /*!<LEC[2:0] bits (Last Error Code) */
#define CAN_ESR_LEC_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define CAN_ESR_LEC_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define CAN_ESR_LEC_2 ((uint32_t)0x00000040) /*!<Bit 2 */

#define CAN_ESR_TEC ((uint32_t)0x00FF0000) /*!<Least significant byte of the 9-bit Transmit Error Counter */
#define CAN_ESR_REC ((uint32_t)0xFF000000) /*!<Receive Error Counter */

/*******************  Bit definition for CAN_BTR register  ********************/
#define CAN_BTR_BRP  ((uint32_t)0x000003FF) /*!<Baud Rate Prescaler */
#define CAN_BTR_TS1  ((uint32_t)0x000F0000) /*!<Time Segment 1 */
#define CAN_BTR_TS2  ((uint32_t)0x00700000) /*!<Time Segment 2 */
#define CAN_BTR_SJW  ((uint32_t)0x03000000) /*!<Resynchronization Jump Width */
#define CAN_BTR_LBKM ((uint32_t)0x40000000) /*!<Loop Back Mode (Debug) */
#define CAN_BTR_SILM ((uint32_t)0x80000000) /*!<Silent Mode */

/*!<Mailbox registers */
/******************  Bit definition for CAN_TI0R register  ********************/
#define CAN_TI0R_TXRQ ((uint32_t)0x00000001) /*!<Transmit Mailbox Request */
#define CAN_TI0R_RTR  ((uint32_t)0x00000002) /*!<Remote Transmission Request */
#define CAN_TI0R_IDE  ((uint32_t)0x00000004) /*!<Identifier Extension */
#define CAN_TI0R_EXID ((uint32_t)0x001FFFF8) /*!<Extended Identifier */
#define CAN_TI0R_STID ((uint32_t)0xFFE00000) /*!<Standard Identifier or Extended Identifier */

/******************  Bit definition for CAN_TDT0R register  *******************/
#define CAN_TDT0R_DLC  ((uint32_t)0x0000000F) /*!<Data Length Code */
#define CAN_TDT0R_TGT  ((uint32_t)0x00000100) /*!<Transmit Global Time */
#define CAN_TDT0R_TIME ((uint32_t)0xFFFF0000) /*!<Message Time Stamp */

/******************  Bit definition for CAN_TDL0R register  *******************/
#define CAN_TDL0R_DATA0 ((uint32_t)0x000000FF) /*!<Data byte 0 */
#define CAN_TDL0R_DATA1 ((uint32_t)0x0000FF00) /*!<Data byte 1 */
#define CAN_TDL0R_DATA2 ((uint32_t)0x00FF0000) /*!<Data byte 2 */
#define CAN_TDL0R_DATA3 ((uint32_t)0xFF000000) /*!<Data byte 3 */

/******************  Bit definition for CAN_TDH0R register  *******************/
#define CAN_TDH0R_DATA4 ((uint32_t)0x000000FF) /*!<Data byte 4 */
#define CAN_TDH0R_DATA5 ((uint32_t)0x0000FF00) /*!<Data byte 5 */
#define CAN_TDH0R_DATA6 ((uint32_t)0x00FF0000) /*!<Data byte 6 */
#define CAN_TDH0R_DATA7 ((uint32_t)0xFF000000) /*!<Data byte 7 */

/*******************  Bit definition for CAN_TI1R register  *******************/
#define CAN_TI1R_TXRQ ((uint32_t)0x00000001) /*!<Transmit Mailbox Request */
#define CAN_TI1R_RTR  ((uint32_t)0x00000002) /*!<Remote Transmission Request */
#define CAN_TI1R_IDE  ((uint32_t)0x00000004) /*!<Identifier Extension */
#define CAN_TI1R_EXID ((uint32_t)0x001FFFF8) /*!<Extended Identifier */
#define CAN_TI1R_STID ((uint32_t)0xFFE00000) /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_TDT1R register  ******************/
#define CAN_TDT1R_DLC  ((uint32_t)0x0000000F) /*!<Data Length Code */
#define CAN_TDT1R_TGT  ((uint32_t)0x00000100) /*!<Transmit Global Time */
#define CAN_TDT1R_TIME ((uint32_t)0xFFFF0000) /*!<Message Time Stamp */

/*******************  Bit definition for CAN_TDL1R register  ******************/
#define CAN_TDL1R_DATA0 ((uint32_t)0x000000FF) /*!<Data byte 0 */
#define CAN_TDL1R_DATA1 ((uint32_t)0x0000FF00) /*!<Data byte 1 */
#define CAN_TDL1R_DATA2 ((uint32_t)0x00FF0000) /*!<Data byte 2 */
#define CAN_TDL1R_DATA3 ((uint32_t)0xFF000000) /*!<Data byte 3 */

/*******************  Bit definition for CAN_TDH1R register  ******************/
#define CAN_TDH1R_DATA4 ((uint32_t)0x000000FF) /*!<Data byte 4 */
#define CAN_TDH1R_DATA5 ((uint32_t)0x0000FF00) /*!<Data byte 5 */
#define CAN_TDH1R_DATA6 ((uint32_t)0x00FF0000) /*!<Data byte 6 */
#define CAN_TDH1R_DATA7 ((uint32_t)0xFF000000) /*!<Data byte 7 */

/*******************  Bit definition for CAN_TI2R register  *******************/
#define CAN_TI2R_TXRQ ((uint32_t)0x00000001) /*!<Transmit Mailbox Request */
#define CAN_TI2R_RTR  ((uint32_t)0x00000002) /*!<Remote Transmission Request */
#define CAN_TI2R_IDE  ((uint32_t)0x00000004) /*!<Identifier Extension */
#define CAN_TI2R_EXID ((uint32_t)0x001FFFF8) /*!<Extended identifier */
#define CAN_TI2R_STID ((uint32_t)0xFFE00000) /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_TDT2R register  ******************/
#define CAN_TDT2R_DLC  ((uint32_t)0x0000000F) /*!<Data Length Code */
#define CAN_TDT2R_TGT  ((uint32_t)0x00000100) /*!<Transmit Global Time */
#define CAN_TDT2R_TIME ((uint32_t)0xFFFF0000) /*!<Message Time Stamp */

/*******************  Bit definition for CAN_TDL2R register  ******************/
#define CAN_TDL2R_DATA0 ((uint32_t)0x000000FF) /*!<Data byte 0 */
#define CAN_TDL2R_DATA1 ((uint32_t)0x0000FF00) /*!<Data byte 1 */
#define CAN_TDL2R_DATA2 ((uint32_t)0x00FF0000) /*!<Data byte 2 */
#define CAN_TDL2R_DATA3 ((uint32_t)0xFF000000) /*!<Data byte 3 */

/*******************  Bit definition for CAN_TDH2R register  ******************/
#define CAN_TDH2R_DATA4 ((uint32_t)0x000000FF) /*!<Data byte 4 */
#define CAN_TDH2R_DATA5 ((uint32_t)0x0000FF00) /*!<Data byte 5 */
#define CAN_TDH2R_DATA6 ((uint32_t)0x00FF0000) /*!<Data byte 6 */
#define CAN_TDH2R_DATA7 ((uint32_t)0xFF000000) /*!<Data byte 7 */

/*******************  Bit definition for CAN_RI0R register  *******************/
#define CAN_RI0R_RTR  ((uint32_t)0x00000002) /*!<Remote Transmission Request */
#define CAN_RI0R_IDE  ((uint32_t)0x00000004) /*!<Identifier Extension */
#define CAN_RI0R_EXID ((uint32_t)0x001FFFF8) /*!<Extended Identifier */
#define CAN_RI0R_STID ((uint32_t)0xFFE00000) /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_RDT0R register  ******************/
#define CAN_RDT0R_DLC  ((uint32_t)0x0000000F) /*!<Data Length Code */
#define CAN_RDT0R_FMI  ((uint32_t)0x0000FF00) /*!<Filter Match Index */
#define CAN_RDT0R_TIME ((uint32_t)0xFFFF0000) /*!<Message Time Stamp */

/*******************  Bit definition for CAN_RDL0R register  ******************/
#define CAN_RDL0R_DATA0 ((uint32_t)0x000000FF) /*!<Data byte 0 */
#define CAN_RDL0R_DATA1 ((uint32_t)0x0000FF00) /*!<Data byte 1 */
#define CAN_RDL0R_DATA2 ((uint32_t)0x00FF0000) /*!<Data byte 2 */
#define CAN_RDL0R_DATA3 ((uint32_t)0xFF000000) /*!<Data byte 3 */

/*******************  Bit definition for CAN_RDH0R register  ******************/
#define CAN_RDH0R_DATA4 ((uint32_t)0x000000FF) /*!<Data byte 4 */
#define CAN_RDH0R_DATA5 ((uint32_t)0x0000FF00) /*!<Data byte 5 */
#define CAN_RDH0R_DATA6 ((uint32_t)0x00FF0000) /*!<Data byte 6 */
#define CAN_RDH0R_DATA7 ((uint32_t)0xFF000000) /*!<Data byte 7 */

/*******************  Bit definition for CAN_RI1R register  *******************/
#define CAN_RI1R_RTR  ((uint32_t)0x00000002) /*!<Remote Transmission Request */
#define CAN_RI1R_IDE  ((uint32_t)0x00000004) /*!<Identifier Extension */
#define CAN_RI1R_EXID ((uint32_t)0x001FFFF8) /*!<Extended identifier */
#define CAN_RI1R_STID ((uint32_t)0xFFE00000) /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_RDT1R register  ******************/
#define CAN_RDT1R_DLC  ((uint32_t)0x0000000F) /*!<Data Length Code */
#define CAN_RDT1R_FMI  ((uint32_t)0x0000FF00) /*!<Filter Match Index */
#define CAN_RDT1R_TIME ((uint32_t)0xFFFF0000) /*!<Message Time Stamp */

/*******************  Bit definition for CAN_RDL1R register  ******************/
#define CAN_RDL1R_DATA0 ((uint32_t)0x000000FF) /*!<Data byte 0 */
#define CAN_RDL1R_DATA1 ((uint32_t)0x0000FF00) /*!<Data byte 1 */
#define CAN_RDL1R_DATA2 ((uint32_t)0x00FF0000) /*!<Data byte 2 */
#define CAN_RDL1R_DATA3 ((uint32_t)0xFF000000) /*!<Data byte 3 */

/*******************  Bit definition for CAN_RDH1R register  ******************/
#define CAN_RDH1R_DATA4 ((uint32_t)0x000000FF) /*!<Data byte 4 */
#define CAN_RDH1R_DATA5 ((uint32_t)0x0000FF00) /*!<Data byte 5 */
#define CAN_RDH1R_DATA6 ((uint32_t)0x00FF0000) /*!<Data byte 6 */
#define CAN_RDH1R_DATA7 ((uint32_t)0xFF000000) /*!<Data byte 7 */

/*!<CAN filter registers */
/*******************  Bit definition for CAN_FMR register  ********************/
#define CAN_FMR_FINIT ((uint8_t)0x01) /*!<Filter Init Mode */

/*******************  Bit definition for CAN_FM1R register  *******************/
#define CAN_FM1R_FBM   ((uint16_t)0x3FFF) /*!<Filter Mode */
#define CAN_FM1R_FBM0  ((uint16_t)0x0001) /*!<Filter Init Mode bit 0 */
#define CAN_FM1R_FBM1  ((uint16_t)0x0002) /*!<Filter Init Mode bit 1 */
#define CAN_FM1R_FBM2  ((uint16_t)0x0004) /*!<Filter Init Mode bit 2 */
#define CAN_FM1R_FBM3  ((uint16_t)0x0008) /*!<Filter Init Mode bit 3 */
#define CAN_FM1R_FBM4  ((uint16_t)0x0010) /*!<Filter Init Mode bit 4 */
#define CAN_FM1R_FBM5  ((uint16_t)0x0020) /*!<Filter Init Mode bit 5 */
#define CAN_FM1R_FBM6  ((uint16_t)0x0040) /*!<Filter Init Mode bit 6 */
#define CAN_FM1R_FBM7  ((uint16_t)0x0080) /*!<Filter Init Mode bit 7 */
#define CAN_FM1R_FBM8  ((uint16_t)0x0100) /*!<Filter Init Mode bit 8 */
#define CAN_FM1R_FBM9  ((uint16_t)0x0200) /*!<Filter Init Mode bit 9 */
#define CAN_FM1R_FBM10 ((uint16_t)0x0400) /*!<Filter Init Mode bit 10 */
#define CAN_FM1R_FBM11 ((uint16_t)0x0800) /*!<Filter Init Mode bit 11 */
#define CAN_FM1R_FBM12 ((uint16_t)0x1000) /*!<Filter Init Mode bit 12 */
#define CAN_FM1R_FBM13 ((uint16_t)0x2000) /*!<Filter Init Mode bit 13 */

/*******************  Bit definition for CAN_FS1R register  *******************/
#define CAN_FS1R_FSC   ((uint16_t)0x3FFF) /*!<Filter Scale Configuration */
#define CAN_FS1R_FSC0  ((uint16_t)0x0001) /*!<Filter Scale Configuration bit 0 */
#define CAN_FS1R_FSC1  ((uint16_t)0x0002) /*!<Filter Scale Configuration bit 1 */
#define CAN_FS1R_FSC2  ((uint16_t)0x0004) /*!<Filter Scale Configuration bit 2 */
#define CAN_FS1R_FSC3  ((uint16_t)0x0008) /*!<Filter Scale Configuration bit 3 */
#define CAN_FS1R_FSC4  ((uint16_t)0x0010) /*!<Filter Scale Configuration bit 4 */
#define CAN_FS1R_FSC5  ((uint16_t)0x0020) /*!<Filter Scale Configuration bit 5 */
#define CAN_FS1R_FSC6  ((uint16_t)0x0040) /*!<Filter Scale Configuration bit 6 */
#define CAN_FS1R_FSC7  ((uint16_t)0x0080) /*!<Filter Scale Configuration bit 7 */
#define CAN_FS1R_FSC8  ((uint16_t)0x0100) /*!<Filter Scale Configuration bit 8 */
#define CAN_FS1R_FSC9  ((uint16_t)0x0200) /*!<Filter Scale Configuration bit 9 */
#define CAN_FS1R_FSC10 ((uint16_t)0x0400) /*!<Filter Scale Configuration bit 10 */
#define CAN_FS1R_FSC11 ((uint16_t)0x0800) /*!<Filter Scale Configuration bit 11 */
#define CAN_FS1R_FSC12 ((uint16_t)0x1000) /*!<Filter Scale Configuration bit 12 */
#define CAN_FS1R_FSC13 ((uint16_t)0x2000) /*!<Filter Scale Configuration bit 13 */

/******************  Bit definition for CAN_FFA1R register  *******************/
#define CAN_FFA1R_FFA   ((uint16_t)0x3FFF) /*!<Filter FIFO Assignment */
#define CAN_FFA1R_FFA0  ((uint16_t)0x0001) /*!<Filter FIFO Assignment for Filter 0 */
#define CAN_FFA1R_FFA1  ((uint16_t)0x0002) /*!<Filter FIFO Assignment for Filter 1 */
#define CAN_FFA1R_FFA2  ((uint16_t)0x0004) /*!<Filter FIFO Assignment for Filter 2 */
#define CAN_FFA1R_FFA3  ((uint16_t)0x0008) /*!<Filter FIFO Assignment for Filter 3 */
#define CAN_FFA1R_FFA4  ((uint16_t)0x0010) /*!<Filter FIFO Assignment for Filter 4 */
#define CAN_FFA1R_FFA5  ((uint16_t)0x0020) /*!<Filter FIFO Assignment for Filter 5 */
#define CAN_FFA1R_FFA6  ((uint16_t)0x0040) /*!<Filter FIFO Assignment for Filter 6 */
#define CAN_FFA1R_FFA7  ((uint16_t)0x0080) /*!<Filter FIFO Assignment for Filter 7 */
#define CAN_FFA1R_FFA8  ((uint16_t)0x0100) /*!<Filter FIFO Assignment for Filter 8 */
#define CAN_FFA1R_FFA9  ((uint16_t)0x0200) /*!<Filter FIFO Assignment for Filter 9 */
#define CAN_FFA1R_FFA10 ((uint16_t)0x0400) /*!<Filter FIFO Assignment for Filter 10 */
#define CAN_FFA1R_FFA11 ((uint16_t)0x0800) /*!<Filter FIFO Assignment for Filter 11 */
#define CAN_FFA1R_FFA12 ((uint16_t)0x1000) /*!<Filter FIFO Assignment for Filter 12 */
#define CAN_FFA1R_FFA13 ((uint16_t)0x2000) /*!<Filter FIFO Assignment for Filter 13 */

/*******************  Bit definition for CAN_FA1R register  *******************/
#define CAN_FA1R_FACT   ((uint16_t)0x3FFF) /*!<Filter Active */
#define CAN_FA1R_FACT0  ((uint16_t)0x0001) /*!<Filter 0 Active */
#define CAN_FA1R_FACT1  ((uint16_t)0x0002) /*!<Filter 1 Active */
#define CAN_FA1R_FACT2  ((uint16_t)0x0004) /*!<Filter 2 Active */
#define CAN_FA1R_FACT3  ((uint16_t)0x0008) /*!<Filter 3 Active */
#define CAN_FA1R_FACT4  ((uint16_t)0x0010) /*!<Filter 4 Active */
#define CAN_FA1R_FACT5  ((uint16_t)0x0020) /*!<Filter 5 Active */
#define CAN_FA1R_FACT6  ((uint16_t)0x0040) /*!<Filter 6 Active */
#define CAN_FA1R_FACT7  ((uint16_t)0x0080) /*!<Filter 7 Active */
#define CAN_FA1R_FACT8  ((uint16_t)0x0100) /*!<Filter 8 Active */
#define CAN_FA1R_FACT9  ((uint16_t)0x0200) /*!<Filter 9 Active */
#define CAN_FA1R_FACT10 ((uint16_t)0x0400) /*!<Filter 10 Active */
#define CAN_FA1R_FACT11 ((uint16_t)0x0800) /*!<Filter 11 Active */
#define CAN_FA1R_FACT12 ((uint16_t)0x1000) /*!<Filter 12 Active */
#define CAN_FA1R_FACT13 ((uint16_t)0x2000) /*!<Filter 13 Active */

/*******************  Bit definition for CAN_F0R1 register  *******************/
#define CAN_F0R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F0R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F0R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F0R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F0R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F0R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F0R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F0R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F0R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F0R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F0R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F0R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F0R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F0R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F0R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F0R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F0R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F0R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F0R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F0R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F0R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F0R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F0R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F0R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F0R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F0R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F0R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F0R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F0R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F0R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F0R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F0R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F1R1 register  *******************/
#define CAN_F1R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F1R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F1R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F1R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F1R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F1R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F1R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F1R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F1R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F1R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F1R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F1R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F1R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F1R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F1R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F1R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F1R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F1R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F1R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F1R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F1R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F1R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F1R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F1R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F1R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F1R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F1R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F1R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F1R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F1R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F1R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F1R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F2R1 register  *******************/
#define CAN_F2R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F2R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F2R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F2R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F2R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F2R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F2R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F2R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F2R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F2R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F2R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F2R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F2R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F2R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F2R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F2R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F2R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F2R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F2R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F2R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F2R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F2R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F2R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F2R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F2R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F2R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F2R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F2R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F2R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F2R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F2R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F2R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F3R1 register  *******************/
#define CAN_F3R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F3R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F3R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F3R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F3R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F3R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F3R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F3R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F3R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F3R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F3R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F3R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F3R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F3R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F3R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F3R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F3R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F3R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F3R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F3R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F3R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F3R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F3R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F3R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F3R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F3R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F3R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F3R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F3R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F3R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F3R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F3R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F4R1 register  *******************/
#define CAN_F4R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F4R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F4R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F4R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F4R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F4R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F4R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F4R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F4R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F4R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F4R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F4R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F4R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F4R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F4R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F4R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F4R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F4R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F4R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F4R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F4R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F4R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F4R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F4R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F4R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F4R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F4R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F4R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F4R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F4R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F4R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F4R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F5R1 register  *******************/
#define CAN_F5R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F5R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F5R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F5R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F5R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F5R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F5R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F5R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F5R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F5R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F5R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F5R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F5R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F5R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F5R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F5R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F5R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F5R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F5R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F5R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F5R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F5R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F5R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F5R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F5R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F5R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F5R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F5R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F5R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F5R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F5R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F5R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F6R1 register  *******************/
#define CAN_F6R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F6R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F6R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F6R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F6R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F6R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F6R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F6R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F6R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F6R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F6R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F6R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F6R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F6R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F6R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F6R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F6R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F6R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F6R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F6R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F6R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F6R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F6R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F6R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F6R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F6R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F6R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F6R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F6R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F6R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F6R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F6R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F7R1 register  *******************/
#define CAN_F7R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F7R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F7R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F7R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F7R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F7R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F7R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F7R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F7R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F7R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F7R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F7R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F7R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F7R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F7R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F7R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F7R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F7R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F7R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F7R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F7R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F7R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F7R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F7R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F7R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F7R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F7R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F7R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F7R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F7R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F7R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F7R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F8R1 register  *******************/
#define CAN_F8R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F8R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F8R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F8R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F8R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F8R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F8R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F8R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F8R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F8R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F8R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F8R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F8R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F8R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F8R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F8R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F8R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F8R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F8R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F8R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F8R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F8R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F8R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F8R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F8R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F8R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F8R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F8R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F8R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F8R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F8R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F8R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F9R1 register  *******************/
#define CAN_F9R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F9R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F9R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F9R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F9R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F9R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F9R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F9R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F9R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F9R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F9R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F9R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F9R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F9R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F9R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F9R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F9R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F9R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F9R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F9R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F9R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F9R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F9R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F9R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F9R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F9R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F9R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F9R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F9R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F9R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F9R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F9R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F10R1 register  ******************/
#define CAN_F10R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F10R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F10R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F10R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F10R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F10R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F10R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F10R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F10R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F10R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F10R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F10R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F10R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F10R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F10R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F10R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F10R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F10R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F10R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F10R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F10R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F10R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F10R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F10R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F10R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F10R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F10R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F10R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F10R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F10R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F10R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F10R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F11R1 register  ******************/
#define CAN_F11R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F11R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F11R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F11R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F11R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F11R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F11R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F11R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F11R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F11R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F11R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F11R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F11R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F11R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F11R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F11R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F11R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F11R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F11R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F11R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F11R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F11R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F11R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F11R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F11R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F11R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F11R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F11R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F11R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F11R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F11R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F11R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F12R1 register  ******************/
#define CAN_F12R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F12R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F12R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F12R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F12R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F12R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F12R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F12R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F12R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F12R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F12R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F12R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F12R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F12R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F12R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F12R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F12R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F12R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F12R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F12R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F12R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F12R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F12R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F12R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F12R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F12R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F12R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F12R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F12R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F12R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F12R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F12R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F13R1 register  ******************/
#define CAN_F13R1_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F13R1_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F13R1_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F13R1_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F13R1_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F13R1_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F13R1_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F13R1_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F13R1_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F13R1_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F13R1_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F13R1_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F13R1_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F13R1_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F13R1_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F13R1_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F13R1_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F13R1_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F13R1_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F13R1_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F13R1_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F13R1_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F13R1_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F13R1_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F13R1_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F13R1_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F13R1_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F13R1_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F13R1_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F13R1_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F13R1_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F13R1_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F0R2 register  *******************/
#define CAN_F0R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F0R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F0R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F0R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F0R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F0R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F0R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F0R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F0R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F0R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F0R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F0R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F0R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F0R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F0R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F0R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F0R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F0R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F0R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F0R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F0R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F0R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F0R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F0R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F0R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F0R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F0R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F0R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F0R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F0R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F0R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F0R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F1R2 register  *******************/
#define CAN_F1R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F1R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F1R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F1R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F1R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F1R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F1R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F1R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F1R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F1R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F1R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F1R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F1R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F1R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F1R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F1R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F1R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F1R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F1R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F1R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F1R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F1R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F1R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F1R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F1R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F1R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F1R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F1R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F1R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F1R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F1R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F1R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F2R2 register  *******************/
#define CAN_F2R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F2R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F2R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F2R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F2R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F2R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F2R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F2R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F2R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F2R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F2R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F2R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F2R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F2R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F2R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F2R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F2R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F2R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F2R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F2R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F2R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F2R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F2R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F2R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F2R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F2R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F2R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F2R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F2R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F2R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F2R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F2R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F3R2 register  *******************/
#define CAN_F3R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F3R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F3R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F3R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F3R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F3R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F3R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F3R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F3R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F3R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F3R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F3R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F3R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F3R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F3R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F3R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F3R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F3R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F3R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F3R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F3R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F3R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F3R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F3R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F3R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F3R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F3R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F3R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F3R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F3R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F3R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F3R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F4R2 register  *******************/
#define CAN_F4R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F4R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F4R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F4R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F4R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F4R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F4R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F4R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F4R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F4R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F4R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F4R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F4R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F4R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F4R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F4R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F4R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F4R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F4R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F4R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F4R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F4R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F4R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F4R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F4R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F4R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F4R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F4R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F4R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F4R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F4R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F4R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F5R2 register  *******************/
#define CAN_F5R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F5R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F5R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F5R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F5R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F5R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F5R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F5R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F5R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F5R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F5R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F5R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F5R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F5R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F5R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F5R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F5R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F5R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F5R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F5R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F5R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F5R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F5R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F5R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F5R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F5R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F5R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F5R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F5R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F5R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F5R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F5R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F6R2 register  *******************/
#define CAN_F6R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F6R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F6R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F6R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F6R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F6R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F6R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F6R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F6R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F6R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F6R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F6R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F6R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F6R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F6R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F6R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F6R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F6R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F6R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F6R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F6R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F6R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F6R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F6R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F6R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F6R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F6R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F6R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F6R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F6R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F6R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F6R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F7R2 register  *******************/
#define CAN_F7R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F7R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F7R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F7R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F7R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F7R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F7R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F7R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F7R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F7R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F7R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F7R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F7R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F7R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F7R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F7R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F7R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F7R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F7R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F7R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F7R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F7R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F7R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F7R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F7R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F7R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F7R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F7R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F7R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F7R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F7R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F7R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F8R2 register  *******************/
#define CAN_F8R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F8R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F8R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F8R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F8R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F8R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F8R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F8R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F8R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F8R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F8R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F8R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F8R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F8R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F8R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F8R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F8R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F8R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F8R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F8R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F8R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F8R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F8R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F8R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F8R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F8R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F8R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F8R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F8R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F8R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F8R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F8R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F9R2 register  *******************/
#define CAN_F9R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F9R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F9R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F9R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F9R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F9R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F9R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F9R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F9R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F9R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F9R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F9R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F9R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F9R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F9R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F9R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F9R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F9R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F9R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F9R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F9R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F9R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F9R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F9R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F9R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F9R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F9R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F9R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F9R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F9R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F9R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F9R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F10R2 register  ******************/
#define CAN_F10R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F10R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F10R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F10R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F10R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F10R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F10R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F10R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F10R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F10R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F10R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F10R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F10R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F10R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F10R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F10R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F10R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F10R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F10R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F10R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F10R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F10R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F10R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F10R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F10R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F10R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F10R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F10R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F10R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F10R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F10R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F10R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F11R2 register  ******************/
#define CAN_F11R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F11R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F11R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F11R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F11R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F11R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F11R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F11R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F11R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F11R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F11R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F11R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F11R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F11R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F11R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F11R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F11R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F11R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F11R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F11R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F11R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F11R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F11R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F11R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F11R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F11R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F11R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F11R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F11R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F11R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F11R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F11R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F12R2 register  ******************/
#define CAN_F12R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F12R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F12R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F12R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F12R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F12R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F12R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F12R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F12R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F12R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F12R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F12R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F12R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F12R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F12R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F12R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F12R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F12R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F12R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F12R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F12R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F12R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F12R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F12R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F12R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F12R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F12R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F12R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F12R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F12R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F12R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F12R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F13R2 register  ******************/
#define CAN_F13R2_FB0  ((uint32_t)0x00000001) /*!<Filter bit 0 */
#define CAN_F13R2_FB1  ((uint32_t)0x00000002) /*!<Filter bit 1 */
#define CAN_F13R2_FB2  ((uint32_t)0x00000004) /*!<Filter bit 2 */
#define CAN_F13R2_FB3  ((uint32_t)0x00000008) /*!<Filter bit 3 */
#define CAN_F13R2_FB4  ((uint32_t)0x00000010) /*!<Filter bit 4 */
#define CAN_F13R2_FB5  ((uint32_t)0x00000020) /*!<Filter bit 5 */
#define CAN_F13R2_FB6  ((uint32_t)0x00000040) /*!<Filter bit 6 */
#define CAN_F13R2_FB7  ((uint32_t)0x00000080) /*!<Filter bit 7 */
#define CAN_F13R2_FB8  ((uint32_t)0x00000100) /*!<Filter bit 8 */
#define CAN_F13R2_FB9  ((uint32_t)0x00000200) /*!<Filter bit 9 */
#define CAN_F13R2_FB10 ((uint32_t)0x00000400) /*!<Filter bit 10 */
#define CAN_F13R2_FB11 ((uint32_t)0x00000800) /*!<Filter bit 11 */
#define CAN_F13R2_FB12 ((uint32_t)0x00001000) /*!<Filter bit 12 */
#define CAN_F13R2_FB13 ((uint32_t)0x00002000) /*!<Filter bit 13 */
#define CAN_F13R2_FB14 ((uint32_t)0x00004000) /*!<Filter bit 14 */
#define CAN_F13R2_FB15 ((uint32_t)0x00008000) /*!<Filter bit 15 */
#define CAN_F13R2_FB16 ((uint32_t)0x00010000) /*!<Filter bit 16 */
#define CAN_F13R2_FB17 ((uint32_t)0x00020000) /*!<Filter bit 17 */
#define CAN_F13R2_FB18 ((uint32_t)0x00040000) /*!<Filter bit 18 */
#define CAN_F13R2_FB19 ((uint32_t)0x00080000) /*!<Filter bit 19 */
#define CAN_F13R2_FB20 ((uint32_t)0x00100000) /*!<Filter bit 20 */
#define CAN_F13R2_FB21 ((uint32_t)0x00200000) /*!<Filter bit 21 */
#define CAN_F13R2_FB22 ((uint32_t)0x00400000) /*!<Filter bit 22 */
#define CAN_F13R2_FB23 ((uint32_t)0x00800000) /*!<Filter bit 23 */
#define CAN_F13R2_FB24 ((uint32_t)0x01000000) /*!<Filter bit 24 */
#define CAN_F13R2_FB25 ((uint32_t)0x02000000) /*!<Filter bit 25 */
#define CAN_F13R2_FB26 ((uint32_t)0x04000000) /*!<Filter bit 26 */
#define CAN_F13R2_FB27 ((uint32_t)0x08000000) /*!<Filter bit 27 */
#define CAN_F13R2_FB28 ((uint32_t)0x10000000) /*!<Filter bit 28 */
#define CAN_F13R2_FB29 ((uint32_t)0x20000000) /*!<Filter bit 29 */
#define CAN_F13R2_FB30 ((uint32_t)0x40000000) /*!<Filter bit 30 */
#define CAN_F13R2_FB31 ((uint32_t)0x80000000) /*!<Filter bit 31 */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  CAN init structure definition
 */
typedef struct
{
    uint16_t CAN_Prescaler; /*!< Specifies the length of a time quantum.
                                 It ranges from 1 to 1024. */

    uint8_t CAN_Mode; /*!< Specifies the CAN operating mode.
                           This parameter can be a value of @ref CAN_operating_mode */

    uint8_t CAN_SJW; /*!< Specifies the maximum number of time quanta
                          the CAN hardware is allowed to lengthen or
                          shorten a bit to perform resynchronization.
                          This parameter can be a value of @ref CAN_synchronisation_jump_width */

    uint8_t CAN_BS1; /*!< Specifies the number of time quanta in Bit
                          Segment 1. This parameter can be a value of
                          @ref CAN_time_quantum_in_bit_segment_1 */

    uint8_t CAN_BS2; /*!< Specifies the number of time quanta in Bit Segment 2.
                          This parameter can be a value of @ref CAN_time_quantum_in_bit_segment_2 */

    FunctionalState CAN_TTCM; /*!< Enable or disable the time triggered communication mode.
                                  This parameter can be set either to ENABLE or DISABLE. */

    FunctionalState CAN_ABOM; /*!< Enable or disable the automatic bus-off management.
                                   This parameter can be set either to ENABLE or DISABLE. */

    FunctionalState CAN_AWUM; /*!< Enable or disable the automatic wake-up mode.
                                   This parameter can be set either to ENABLE or DISABLE. */

    FunctionalState CAN_NART; /*!< Enable or disable the non-automatic retransmission mode.
                                   This parameter can be set either to ENABLE or DISABLE. */

    FunctionalState CAN_RFLM; /*!< Enable or disable the Receive FIFO Locked mode.
                                   This parameter can be set either to ENABLE or DISABLE. */

    FunctionalState CAN_TXFP; /*!< Enable or disable the transmit FIFO priority.
                                   This parameter can be set either to ENABLE or DISABLE. */
} CAN_InitTypeDef;

/**
 * @brief  CAN filter init structure definition
 */
typedef struct
{
    uint16_t CAN_FilterIdHigh; /*!< Specifies the filter identification number (MSBs for a 32-bit
                                        configuration, first one for a 16-bit configuration).
                                        This parameter can be a value between 0x0000 and 0xFFFF */

    uint16_t CAN_FilterIdLow; /*!< Specifies the filter identification number (LSBs for a 32-bit
                                       configuration, second one for a 16-bit configuration).
                                       This parameter can be a value between 0x0000 and 0xFFFF */

    uint16_t CAN_FilterMaskIdHigh; /*!< Specifies the filter mask number or identification number,
                                            according to the mode (MSBs for a 32-bit configuration,
                                            first one for a 16-bit configuration).
                                            This parameter can be a value between 0x0000 and 0xFFFF */

    uint16_t CAN_FilterMaskIdLow; /*!< Specifies the filter mask number or identification number,
                                           according to the mode (LSBs for a 32-bit configuration,
                                           second one for a 16-bit configuration).
                                           This parameter can be a value between 0x0000 and 0xFFFF */

    uint16_t CAN_FilterFIFOAssignment; /*!< Specifies the FIFO (0 or 1) which will be assigned to the filter.
                                                This parameter can be a value of @ref CAN_filter_FIFO */

    uint8_t CAN_FilterNumber; /*!< Specifies the filter which will be initialized. It ranges from 0 to 13. */

    uint8_t CAN_FilterMode; /*!< Specifies the filter mode to be initialized.
                                     This parameter can be a value of @ref CAN_filter_mode */

    uint8_t CAN_FilterScale; /*!< Specifies the filter scale.
                                      This parameter can be a value of @ref CAN_filter_scale */

    FunctionalState CAN_FilterActivation; /*!< Enable or disable the filter.
                                                This parameter can be set either to ENABLE or DISABLE. */
} CAN_FilterInitTypeDef;

/**
 * @brief  CAN Tx message structure definition
 */
typedef struct
{
    uint32_t StdId; /*!< Specifies the standard identifier.
                         This parameter can be a value between 0 to 0x7FF. */

    uint32_t ExtId; /*!< Specifies the extended identifier.
                         This parameter can be a value between 0 to 0x1FFFFFFF. */

    uint8_t IDE; /*!< Specifies the type of identifier for the message that
                      will be transmitted. This parameter can be a value
                      of @ref CAN_identifier_type */

    uint8_t RTR; /*!< Specifies the type of frame for the message that will
                      be transmitted. This parameter can be a value of
                      @ref CAN_remote_transmission_request */

    uint8_t DLC; /*!< Specifies the length of the frame that will be
                      transmitted. This parameter can be a value between
                      0 to 8 */

    uint8_t Data[8]; /*!< Contains the data to be transmitted. It ranges from 0
                          to 0xFF. */
} CanTxMsg;

/**
 * @brief  CAN Rx message structure definition
 */
typedef struct
{
    uint32_t StdId; /*!< Specifies the standard identifier.
                         This parameter can be a value between 0 to 0x7FF. */

    uint32_t ExtId; /*!< Specifies the extended identifier.
                         This parameter can be a value between 0 to 0x1FFFFFFF. */

    uint8_t IDE; /*!< Specifies the type of identifier for the message that
                      will be received. This parameter can be a value of
                      @ref CAN_identifier_type */

    uint8_t RTR; /*!< Specifies the type of frame for the received message.
                      This parameter can be a value of
                      @ref CAN_remote_transmission_request */

    uint8_t DLC; /*!< Specifies the length of the frame that will be received.
                      This parameter can be a value between 0 to 8 */

    uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to
                          0xFF. */

    uint8_t FMI; /*!< Specifies the index of the filter the message stored in
                      the mailbox passes through. This parameter can be a
                      value between 0 to 0xFF */
} CanRxMsg;

/* Exported constants --------------------------------------------------------*/

/** @defgroup CAN_Exported_Constants
 * @{
 */

/** @defgroup CAN_InitStatus
 * @{
 */

#define CAN_InitStatus_Failed  ((uint8_t)0x00) /*!< CAN initialization failed */
#define CAN_InitStatus_Success ((uint8_t)0x01) /*!< CAN initialization OK */

/* Legacy defines */
#define CANINITFAILED CAN_InitStatus_Failed
#define CANINITOK     CAN_InitStatus_Success
/**
 * @}
 */

/** @defgroup CAN_operating_mode
 * @{
 */

#define CAN_Mode_Normal          ((uint8_t)0x00) /*!< normal mode */
#define CAN_Mode_LoopBack        ((uint8_t)0x01) /*!< loopback mode */
#define CAN_Mode_Silent          ((uint8_t)0x02) /*!< silent mode */
#define CAN_Mode_Silent_LoopBack ((uint8_t)0x03) /*!< loopback combined with silent mode */

#define IS_CAN_MODE(MODE) (((MODE) == CAN_Mode_Normal) || ((MODE) == CAN_Mode_LoopBack) || ((MODE) == CAN_Mode_Silent) || ((MODE) == CAN_Mode_Silent_LoopBack))
/**
 * @}
 */

/**
 * @defgroup CAN_operating_mode
 * @{
 */
#define CAN_OperatingMode_Initialization ((uint8_t)0x00) /*!< Initialization mode */
#define CAN_OperatingMode_Normal         ((uint8_t)0x01) /*!< Normal mode */
#define CAN_OperatingMode_Sleep          ((uint8_t)0x02) /*!< sleep mode */

#define IS_CAN_OPERATING_MODE(MODE) \
    (((MODE) == CAN_OperatingMode_Initialization) || ((MODE) == CAN_OperatingMode_Normal) || ((MODE) == CAN_OperatingMode_Sleep))
/**
 * @}
 */

/**
 * @defgroup CAN_operating_mode_status
 * @{
 */

#define CAN_ModeStatus_Failed  ((uint8_t)0x00)                   /*!< CAN entering the specific mode failed */
#define CAN_ModeStatus_Success ((uint8_t)!CAN_ModeStatus_Failed) /*!< CAN entering the specific mode Succeed */
/**
 * @}
 */

/** @defgroup CAN_synchronisation_jump_width
 * @{
 */
#define CAN_SJW_1tq ((uint8_t)0x00) /*!< 1 time quantum */
#define CAN_SJW_2tq ((uint8_t)0x01) /*!< 2 time quantum */
#define CAN_SJW_3tq ((uint8_t)0x02) /*!< 3 time quantum */
#define CAN_SJW_4tq ((uint8_t)0x03) /*!< 4 time quantum */

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq) || ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))
/**
 * @}
 */

/** @defgroup CAN_time_quantum_in_bit_segment_1
 * @{
 */
#define CAN_BS1_1tq  ((uint8_t)0x00) /*!< 1 time quantum */
#define CAN_BS1_2tq  ((uint8_t)0x01) /*!< 2 time quantum */
#define CAN_BS1_3tq  ((uint8_t)0x02) /*!< 3 time quantum */
#define CAN_BS1_4tq  ((uint8_t)0x03) /*!< 4 time quantum */
#define CAN_BS1_5tq  ((uint8_t)0x04) /*!< 5 time quantum */
#define CAN_BS1_6tq  ((uint8_t)0x05) /*!< 6 time quantum */
#define CAN_BS1_7tq  ((uint8_t)0x06) /*!< 7 time quantum */
#define CAN_BS1_8tq  ((uint8_t)0x07) /*!< 8 time quantum */
#define CAN_BS1_9tq  ((uint8_t)0x08) /*!< 9 time quantum */
#define CAN_BS1_10tq ((uint8_t)0x09) /*!< 10 time quantum */
#define CAN_BS1_11tq ((uint8_t)0x0A) /*!< 11 time quantum */
#define CAN_BS1_12tq ((uint8_t)0x0B) /*!< 12 time quantum */
#define CAN_BS1_13tq ((uint8_t)0x0C) /*!< 13 time quantum */
#define CAN_BS1_14tq ((uint8_t)0x0D) /*!< 14 time quantum */
#define CAN_BS1_15tq ((uint8_t)0x0E) /*!< 15 time quantum */
#define CAN_BS1_16tq ((uint8_t)0x0F) /*!< 16 time quantum */

#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16tq)
/**
 * @}
 */

/** @defgroup CAN_time_quantum_in_bit_segment_2
 * @{
 */
#define CAN_BS2_1tq ((uint8_t)0x00) /*!< 1 time quantum */
#define CAN_BS2_2tq ((uint8_t)0x01) /*!< 2 time quantum */
#define CAN_BS2_3tq ((uint8_t)0x02) /*!< 3 time quantum */
#define CAN_BS2_4tq ((uint8_t)0x03) /*!< 4 time quantum */
#define CAN_BS2_5tq ((uint8_t)0x04) /*!< 5 time quantum */
#define CAN_BS2_6tq ((uint8_t)0x05) /*!< 6 time quantum */
#define CAN_BS2_7tq ((uint8_t)0x06) /*!< 7 time quantum */
#define CAN_BS2_8tq ((uint8_t)0x07) /*!< 8 time quantum */

#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8tq)
/**
 * @}
 */

/** @defgroup CAN_clock_prescaler
 * @{
 */
#define IS_CAN_PRESCALER(PRESCALER) (((PRESCALER) >= 1) && ((PRESCALER) <= 1024))
/**
 * @}
 */

/** @defgroup CAN_filter_number
 * @{
 */
#define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 27)
/**
 * @}
 */

/** @defgroup CAN_filter_mode
 * @{
 */
#define CAN_FilterMode_IdMask ((uint8_t)0x00) /*!< identifier/mask mode */
#define CAN_FilterMode_IdList ((uint8_t)0x01) /*!< identifier list mode */

#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FilterMode_IdMask) || ((MODE) == CAN_FilterMode_IdList))
/**
 * @}
 */

/** @defgroup CAN_filter_scale
 * @{
 */
#define CAN_FilterScale_16bit ((uint8_t)0x00) /*!< Two 16-bit filters */
#define CAN_FilterScale_32bit ((uint8_t)0x01) /*!< One 32-bit filter */

#define IS_CAN_FILTER_SCALE(SCALE) (((SCALE) == CAN_FilterScale_16bit) || ((SCALE) == CAN_FilterScale_32bit))
/**
 * @}
 */

/** @defgroup CAN_filter_FIFO
 * @{
 */
#define CAN_Filter_FIFO0         ((uint8_t)0x00) /*!< Filter FIFO 0 assignment for filter x */
#define CAN_Filter_FIFO1         ((uint8_t)0x01) /*!< Filter FIFO 1 assignment for filter x */
#define IS_CAN_FILTER_FIFO(FIFO) (((FIFO) == CAN_FilterFIFO0) || ((FIFO) == CAN_FilterFIFO1))

/* Legacy defines */
#define CAN_FilterFIFO0 CAN_Filter_FIFO0
#define CAN_FilterFIFO1 CAN_Filter_FIFO1
/**
 * @}
 */

/** @defgroup CAN_Start_bank_filter_for_slave_CAN
 * @{
 */
#define IS_CAN_BANKNUMBER(BANKNUMBER) (((BANKNUMBER) >= 1) && ((BANKNUMBER) <= 27))
/**
 * @}
 */

/** @defgroup CAN_Tx
 * @{
 */
#define IS_CAN_TRANSMITMAILBOX(TRANSMITMAILBOX) ((TRANSMITMAILBOX) <= ((uint8_t)0x02))
#define IS_CAN_STDID(STDID)                     ((STDID) <= ((uint32_t)0x7FF))
#define IS_CAN_EXTID(EXTID)                     ((EXTID) <= ((uint32_t)0x1FFFFFFF))
#define IS_CAN_DLC(DLC)                         ((DLC) <= ((uint8_t)0x08))
/**
 * @}
 */

/** @defgroup CAN_identifier_type
 * @{
 */
#define CAN_Id_Standard       ((uint32_t)0x00000000) /*!< Standard Id */
#define CAN_Id_Extended       ((uint32_t)0x00000004) /*!< Extended Id */
#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_Id_Standard) || ((IDTYPE) == CAN_Id_Extended))

/* Legacy defines */
#define CAN_ID_STD CAN_Id_Standard
#define CAN_ID_EXT CAN_Id_Extended
/**
 * @}
 */

/** @defgroup CAN_remote_transmission_request
 * @{
 */
#define CAN_RTR_Data    ((uint32_t)0x00000000) /*!< Data frame */
#define CAN_RTR_Remote  ((uint32_t)0x00000002) /*!< Remote frame */
#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_Data) || ((RTR) == CAN_RTR_Remote))

/* Legacy defines */
#define CAN_RTR_DATA   CAN_RTR_Data
#define CAN_RTR_REMOTE CAN_RTR_Remote
/**
 * @}
 */

/** @defgroup CAN_transmit_constants
 * @{
 */
#define CAN_TxStatus_Failed  ((uint8_t)0x00) /*!< CAN transmission failed */
#define CAN_TxStatus_Ok      ((uint8_t)0x01) /*!< CAN transmission succeeded */
#define CAN_TxStatus_Pending ((uint8_t)0x02) /*!< CAN transmission pending */
#define CAN_TxStatus_NoMailBox                    \
    ((uint8_t)0x04) /*!< CAN cell did not provide \
                         an empty mailbox */
/* Legacy defines */
#define CANTXFAILED  CAN_TxStatus_Failed
#define CANTXOK      CAN_TxStatus_Ok
#define CANTXPENDING CAN_TxStatus_Pending
#define CAN_NO_MB    CAN_TxStatus_NoMailBox
/**
 * @}
 */

/** @defgroup CAN_receive_FIFO_number_constants
 * @{
 */
#define CAN_FIFO0 ((uint8_t)0x00) /*!< CAN FIFO 0 used to receive */
#define CAN_FIFO1 ((uint8_t)0x01) /*!< CAN FIFO 1 used to receive */

#define IS_CAN_FIFO(FIFO) (((FIFO) == CAN_FIFO0) || ((FIFO) == CAN_FIFO1))
/**
 * @}
 */

/** @defgroup CAN_sleep_constants
 * @{
 */
#define CAN_Sleep_Failed ((uint8_t)0x00) /*!< CAN did not enter the sleep mode */
#define CAN_Sleep_Ok     ((uint8_t)0x01) /*!< CAN entered the sleep mode */

/* Legacy defines */
#define CANSLEEPFAILED CAN_Sleep_Failed
#define CANSLEEPOK     CAN_Sleep_Ok
/**
 * @}
 */

/** @defgroup CAN_wake_up_constants
 * @{
 */
#define CAN_WakeUp_Failed ((uint8_t)0x00) /*!< CAN did not leave the sleep mode */
#define CAN_WakeUp_Ok     ((uint8_t)0x01) /*!< CAN leaved the sleep mode */

/* Legacy defines */
#define CANWAKEUPFAILED CAN_WakeUp_Failed
#define CANWAKEUPOK     CAN_WakeUp_Ok
/**
 * @}
 */

/**
 * @defgroup CAN_Error_Code_constants
 * @{
 */
#define CAN_ErrorCode_NoErr           ((uint8_t)0x00) /*!< No Error */
#define CAN_ErrorCode_StuffErr        ((uint8_t)0x10) /*!< Stuff Error */
#define CAN_ErrorCode_FormErr         ((uint8_t)0x20) /*!< Form Error */
#define CAN_ErrorCode_ACKErr          ((uint8_t)0x30) /*!< Acknowledgment Error */
#define CAN_ErrorCode_BitRecessiveErr ((uint8_t)0x40) /*!< Bit Recessive Error */
#define CAN_ErrorCode_BitDominantErr  ((uint8_t)0x50) /*!< Bit Dominant Error */
#define CAN_ErrorCode_CRCErr          ((uint8_t)0x60) /*!< CRC Error  */
#define CAN_ErrorCode_SoftwareSetErr  ((uint8_t)0x70) /*!< Software Set Error */
/**
 * @}
 */

/** @defgroup CAN_flags
 * @{
 */
/* If the flag is 0x3XXXXXXX, it means that it can be used with CAN_GetFlagStatus()
   and CAN_ClearFlag() functions. */
/* If the flag is 0x1XXXXXXX, it means that it can only be used with
   CAN_GetFlagStatus() function.  */

/* Transmit Flags */
#define CAN_FLAG_RQCP0 ((uint32_t)0x38000001) /*!< Request MailBox0 Flag */
#define CAN_FLAG_RQCP1 ((uint32_t)0x38000100) /*!< Request MailBox1 Flag */
#define CAN_FLAG_RQCP2 ((uint32_t)0x38010000) /*!< Request MailBox2 Flag */

/* Receive Flags */
#define CAN_FLAG_FMP0 ((uint32_t)0x12000003) /*!< FIFO 0 Message Pending Flag */
#define CAN_FLAG_FF0  ((uint32_t)0x32000008) /*!< FIFO 0 Full Flag            */
#define CAN_FLAG_FOV0 ((uint32_t)0x32000010) /*!< FIFO 0 Overrun Flag         */
#define CAN_FLAG_FMP1 ((uint32_t)0x14000003) /*!< FIFO 1 Message Pending Flag */
#define CAN_FLAG_FF1  ((uint32_t)0x34000008) /*!< FIFO 1 Full Flag            */
#define CAN_FLAG_FOV1 ((uint32_t)0x34000010) /*!< FIFO 1 Overrun Flag         */

/* Operating Mode Flags */
#define CAN_FLAG_WKU  ((uint32_t)0x31000008) /*!< Wake up Flag */
#define CAN_FLAG_SLAK ((uint32_t)0x31000012) /*!< Sleep acknowledge Flag */
/* @note When SLAK interrupt is disabled (SLKIE=0), no polling on SLAKI is possible.
         In this case the SLAK bit can be polled.*/

/* Error Flags */
#define CAN_FLAG_EWG ((uint32_t)0x10F00001) /*!< Error Warning Flag   */
#define CAN_FLAG_EPV ((uint32_t)0x10F00002) /*!< Error Passive Flag   */
#define CAN_FLAG_BOF ((uint32_t)0x10F00004) /*!< Bus-Off Flag         */
#define CAN_FLAG_LEC ((uint32_t)0x30F00070) /*!< Last error code Flag */

#define IS_CAN_GET_FLAG(FLAG)                                                                                                                       \
    (((FLAG) == CAN_FLAG_LEC) || ((FLAG) == CAN_FLAG_BOF) || ((FLAG) == CAN_FLAG_EPV) || ((FLAG) == CAN_FLAG_EWG) || ((FLAG) == CAN_FLAG_WKU) ||    \
     ((FLAG) == CAN_FLAG_FOV0) || ((FLAG) == CAN_FLAG_FF0) || ((FLAG) == CAN_FLAG_FMP0) || ((FLAG) == CAN_FLAG_FOV1) || ((FLAG) == CAN_FLAG_FF1) || \
     ((FLAG) == CAN_FLAG_FMP1) || ((FLAG) == CAN_FLAG_RQCP2) || ((FLAG) == CAN_FLAG_RQCP1) || ((FLAG) == CAN_FLAG_RQCP0) || ((FLAG) == CAN_FLAG_SLAK))

#define IS_CAN_CLEAR_FLAG(FLAG)                                                                                                                        \
    (((FLAG) == CAN_FLAG_LEC) || ((FLAG) == CAN_FLAG_RQCP2) || ((FLAG) == CAN_FLAG_RQCP1) || ((FLAG) == CAN_FLAG_RQCP0) || ((FLAG) == CAN_FLAG_FF0) || \
     ((FLAG) == CAN_FLAG_FOV0) || ((FLAG) == CAN_FLAG_FF1) || ((FLAG) == CAN_FLAG_FOV1) || ((FLAG) == CAN_FLAG_WKU) || ((FLAG) == CAN_FLAG_SLAK))
/**
 * @}
 */

/** @defgroup CAN_interrupts
 * @{
 */
#define CAN_IT_TME ((uint32_t)0x00000001) /*!< Transmit mailbox empty Interrupt*/

/* Receive Interrupts */
#define CAN_IT_FMP0 ((uint32_t)0x00000002) /*!< FIFO 0 message pending Interrupt*/
#define CAN_IT_FF0  ((uint32_t)0x00000004) /*!< FIFO 0 full Interrupt*/
#define CAN_IT_FOV0 ((uint32_t)0x00000008) /*!< FIFO 0 overrun Interrupt*/
#define CAN_IT_FMP1 ((uint32_t)0x00000010) /*!< FIFO 1 message pending Interrupt*/
#define CAN_IT_FF1  ((uint32_t)0x00000020) /*!< FIFO 1 full Interrupt*/
#define CAN_IT_FOV1 ((uint32_t)0x00000040) /*!< FIFO 1 overrun Interrupt*/

/* Operating Mode Interrupts */
#define CAN_IT_WKU ((uint32_t)0x00010000) /*!< Wake-up Interrupt*/
#define CAN_IT_SLK ((uint32_t)0x00020000) /*!< Sleep acknowledge Interrupt*/

/* Error Interrupts */
#define CAN_IT_EWG ((uint32_t)0x00000100) /*!< Error warning Interrupt*/
#define CAN_IT_EPV ((uint32_t)0x00000200) /*!< Error passive Interrupt*/
#define CAN_IT_BOF ((uint32_t)0x00000400) /*!< Bus-off Interrupt*/
#define CAN_IT_LEC ((uint32_t)0x00000800) /*!< Last error code Interrupt*/
#define CAN_IT_ERR ((uint32_t)0x00008000) /*!< Error Interrupt*/

/* Flags named as Interrupts : kept only for FW compatibility */
#define CAN_IT_RQCP0 CAN_IT_TME
#define CAN_IT_RQCP1 CAN_IT_TME
#define CAN_IT_RQCP2 CAN_IT_TME

#define IS_CAN_IT(IT)                                                                                                                                   \
    (((IT) == CAN_IT_TME) || ((IT) == CAN_IT_FMP0) || ((IT) == CAN_IT_FF0) || ((IT) == CAN_IT_FOV0) || ((IT) == CAN_IT_FMP1) || ((IT) == CAN_IT_FF1) || \
     ((IT) == CAN_IT_FOV1) || ((IT) == CAN_IT_EWG) || ((IT) == CAN_IT_EPV) || ((IT) == CAN_IT_BOF) || ((IT) == CAN_IT_LEC) || ((IT) == CAN_IT_ERR) ||   \
     ((IT) == CAN_IT_WKU) || ((IT) == CAN_IT_SLK))

#define IS_CAN_CLEAR_IT(IT)                                                                                                                            \
    (((IT) == CAN_IT_TME) || ((IT) == CAN_IT_FF0) || ((IT) == CAN_IT_FOV0) || ((IT) == CAN_IT_FF1) || ((IT) == CAN_IT_FOV1) || ((IT) == CAN_IT_EWG) || \
     ((IT) == CAN_IT_EPV) || ((IT) == CAN_IT_BOF) || ((IT) == CAN_IT_LEC) || ((IT) == CAN_IT_ERR) || ((IT) == CAN_IT_WKU) || ((IT) == CAN_IT_SLK))
/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the CAN configuration to the default reset state *****/
void CAN_DeInit(CAN_TypeDef* CANx);

/* Initialization and Configuration functions *********************************/
uint8_t CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);

void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct);

void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct);

void CAN_SlaveStartBank(uint8_t CAN_BankNumber);

void CAN_DBGFreeze(CAN_TypeDef* CANx, FunctionalState NewState);
void CAN_TTComModeCmd(CAN_TypeDef* CANx, FunctionalState NewState);

/* CAN Frames Transmission functions ******************************************/
uint8_t CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage);
uint8_t CAN_TransmitStatus(CAN_TypeDef* CANx, uint8_t TransmitMailbox);
void    CAN_CancelTransmit(CAN_TypeDef* CANx, uint8_t Mailbox);

/* CAN Frames Reception functions *********************************************/
void    CAN_Receive(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg* RxMessage);
void    CAN_FIFORelease(CAN_TypeDef* CANx, uint8_t FIFONumber);
uint8_t CAN_MessagePending(CAN_TypeDef* CANx, uint8_t FIFONumber);

/* Operation modes functions **************************************************/
uint8_t CAN_OperatingModeRequest(CAN_TypeDef* CANx, uint8_t CAN_OperatingMode);
uint8_t CAN_Sleep(CAN_TypeDef* CANx);
uint8_t CAN_WakeUp(CAN_TypeDef* CANx);

/* CAN Bus Error management functions *****************************************/
uint8_t CAN_GetLastErrorCode(CAN_TypeDef* CANx);
uint8_t CAN_GetReceiveErrorCounter(CAN_TypeDef* CANx);
uint8_t CAN_GetLSBTransmitErrorCounter(CAN_TypeDef* CANx);

/* Interrupts and flags management functions **********************************/
void       CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, FunctionalState NewState);
FlagStatus CAN_GetFlagStatus(CAN_TypeDef* CANx, uint32_t CAN_FLAG);
void       CAN_ClearFlag(CAN_TypeDef* CANx, uint32_t CAN_FLAG);
ITStatus   CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT);
void       CAN_ClearITPendingBit(CAN_TypeDef* CANx, uint32_t CAN_IT);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_CAN_H__
