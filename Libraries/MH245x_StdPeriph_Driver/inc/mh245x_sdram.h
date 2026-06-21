// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_SDRAM_H__
#define __PERIPH_SDRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mh2457.h"

/* --------  Register Definition  ------------------------------------------- */
/*
    SDRAM registers structure
*/
typedef struct
{
    __IO uint32_t SCONR;  /*!< SDRAM configuration register,                           Address offset: 0x00 */
    __IO uint32_t STMG0R; /*!< SDRAM timing register0,                                 Address offset: 0x04 */
    __IO uint32_t STMG1R; /*!< SDRAM timing register1,                                 Address offset: 0x08 */
    __IO uint32_t SCTLR;  /*!< SDRAM control register,                                 Address offset: 0x0C */
    __IO uint32_t SREFR;  /*!< SDRAM refresh register,                                 Address offset: 0x10 */

    __IO uint32_t SCSLR0_LOW; /*!< SDRAM Chip select register0,                            Address offset: 0x14 */
    __IO uint32_t SCSLR1_LOW; /*!< SDRAM Chip select register1,                            Address offset: 0x18 */
} SDRAM_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/********************  Bit definition for SDRAM_SCONR register  *************/
#define SDRAM_SCONR_DATA_WIDTH_Pos (13UL) /*!<DATA_WIDTH[14:13] */
#define SDRAM_SCONR_DATA_WIDTH_Msk (0x03UL << SDRAM_SCONR_DATA_WIDTH_Pos)
#define SDRAM_SCONR_DATA_WIDTH     SDRAM_SCONR_DATA_WIDTH_Msk

#define SDRAM_SCONR_COL_ADDR_WIDTH_Pos (9UL) /*!<COL_ADDR_WIDTH[12:9] */
#define SDRAM_SCONR_COL_ADDR_WIDTH_Msk (0x0FUL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_SCONR_COL_ADDR_WIDTH     SDRAM_SCONR_COL_ADDR_WIDTH_Msk

#define SDRAM_SCONR_ROW_ADDR_WIDTH_Pos (5UL) /*!<ROL_ADDR_WIDTH[8:5] */
#define SDRAM_SCONR_ROW_ADDR_WIDTH_Msk (0x0FUL << SDRAM_SCONR_ROW_ADDR_WIDTH_Pos)
#define SDRAM_SCONR_ROW_ADDR_WIDTH     SDRAM_SCONR_ROW_ADDR_WIDTH_Msk

#define SDRAM_SCONR_BANK_ADDR_WIDTH_Pos (3UL) /*!<BANK_ADDR_WIDTH[4:3] */
#define SDRAM_SCONR_BANK_ADDR_WIDTH_Msk (0x03UL << SDRAM_SCONR_BANK_ADDR_WIDTH_Pos)
#define SDRAM_SCONR_BANK_ADDR_WIDTH     SDRAM_SCONR_BANK_ADDR_WIDTH_Msk

/********************  Bit definition for SDRAM_STMG0R register  *************/
#define SDRAM_STMG0R_RC_Pos (22UL) /*!<RC[25:22] */
#define SDRAM_STMG0R_RC_Msk (0x0FUL << SDRAM_STMG0R_RC_Pos)
#define SDRAM_STMG0R_RC     SDRAM_STMG0R_RC_Msk

#define SDRAM_STMG0R_XSR_Pos_0 (27UL) /*!<XSR[31:27] [21:18] */
#define SDRAM_STMG0R_XSR_Pos_1 (18UL)
#define SDRAM_STMG0R_XSR_Msk   ((0x0FUL << SDRAM_STMG0R_XSR_Pos_1) | (0x1FUL << SDRAM_STMG0R_XSR_Pos_0))
#define SDRAM_STMG0R_XSR       SDRAM_STMG0R_XSR_Msk

#define SDRAM_STMG0R_RCAR_Pos (14UL) /*!<RCAR[17:14] */
#define SDRAM_STMG0R_RCAR_Msk (0x0FUL << SDRAM_STMG0R_RCAR_Pos)
#define SDRAM_STMG0R_RCAR     SDRAM_STMG0R_RCAR_Msk

#define SDRAM_STMG0R_WR_Pos (12UL) /*!<WR[13:12] */
#define SDRAM_STMG0R_WR_Msk (0x03UL << SDRAM_STMG0R_WR_Pos)
#define SDRAM_STMG0R_WR     SDRAM_STMG0R_WR_Msk

#define SDRAM_STMG0R_RP_Pos (9UL) /*!<RP[11:9] */
#define SDRAM_STMG0R_RP_Msk (0x07UL << SDRAM_STMG0R_RP_Pos)
#define SDRAM_STMG0R_RP     SDRAM_STMG0R_RP_Msk

#define SDRAM_STMG0R_RCD_Pos (6UL) /*!<RCD[8:6] */
#define SDRAM_STMG0R_RCD_Msk (0x07UL << SDRAM_STMG0R_RCD_Pos)
#define SDRAM_STMG0R_RCD     SDRAM_STMG0R_RCD_Msk

#define SDRAM_STMG0R_RAS_MIN_Pos (2UL) /*!<RAS_MIN[5:2] */
#define SDRAM_STMG0R_RAS_MIN_Msk (0x0FUL << SDRAM_STMG0R_RAS_MIN_Pos)
#define SDRAM_STMG0R_RAS_MIN     SDRAM_STMG0R_RAS_MIN_Msk

#define SDRAM_STMG0R_CAS_LATE_Pos_0 (26UL) /*!<CAS_LATE[26] [1:0] */
#define SDRAM_STMG0R_CAS_LATE_Pos_1 (0UL)
#define SDRAM_STMG0R_CAS_LATE_Msk   ((0x01UL << SDRAM_STMG0R_CAS_LATE_Pos_0) | (0x03UL << SDRAM_STMG0R_CAS_LATE_Pos_1))
#define SDRAM_STMG0R_CAS_LATE       SDRAM_STMG0R_CAS_LATE_Msk

/********************  Bit definition for SDRAM_STMG1R register  *************/
#define SDRAM_STMG1R_WTR_Pos (20UL) /*!<WTR[21:20] */
#define SDRAM_STMG1R_WTR_Msk (0x03UL << SDRAM_STMG1R_WTR_Pos)
#define SDRAM_STMG1R_WTR     SDRAM_STMG1R_WTR_Msk

#define SDRAM_STMG1R_NUM_INIT_REF_Pos (16UL) /*!<NUM_INIT_REF[19:16] */
#define SDRAM_STMG1R_NUM_INIT_REF_Msk (0x0FUL << SDRAM_STMG1R_NUM_INIT_REF_Pos)
#define SDRAM_STMG1R_NUM_INIT_REF     SDRAM_STMG1R_NUM_INIT_REF_Msk

#define SDRAM_STMG1R_INIT_Pos (0UL) /*!<NUM_INIT_REF[15:0] */
#define SDRAM_STMG1R_INIT_Msk (0xFFUL << SDRAM_STMG1R_INIT_Pos)
#define SDRAM_STMG1R_INIT     SDRAM_STMG1R_INIT_Msk

/********************  Bit definition for SDRAM_SCTLR register  *************/
#define SDRAM_SCTLR_EXN_MODE_REG_UPDATE_Pos (18UL) /*!<EXN_MODE_REG_UPDATE[18] */
#define SDRAM_SCTLR_EXN_MODE_REG_UPDATE_Msk (0x01UL << SDRAM_SCTLR_EXN_MODE_REG_UPDATE_Pos)
#define SDRAM_SCTLR_EXN_MODE_REG_UPDATE     SDRAM_SCTLR_EXN_MODE_REG_UPDATE_Msk

#define SDRAM_SCTLR_RD_READY_MODE_Pos (17UL) /*!<RD_READY_MODE[17] */
#define SDRAM_SCTLR_RD_READY_MODE_Msk (0x01UL << SDRAM_SCTLR_RD_READY_MODE_Pos)
#define SDRAM_SCTLR_RD_READY_MODE     SDRAM_SCTLR_RD_READY_MODE_Msk

#define SDRAM_SCTLR_NUM_OPEN_BANKS_Pos (12UL) /*!<NUM_OPEN_BANKS[16:12] */
#define SDRAM_SCTLR_NUM_OPEN_BANKS_Msk (0x1FUL << SDRAM_SCTLR_NUM_OPEN_BANKS_Pos)
#define SDRAM_SCTLR_NUM_OPEN_BANKS     SDRAM_SCTLR_NUM_OPEN_BANKS_Msk

#define SDRAM_SCTLR_SELF_REFRESH_STATUS_Pos (11UL) /*!<SELF_REFRESH_STATUS[11] */
#define SDRAM_SCTLR_SELF_REFRESH_STATUS_Msk (0x01UL << SDRAM_SCTLR_SELF_REFRESH_STATUS_Pos)
#define SDRAM_SCTLR_SELF_REFRESH_STATUS     SDRAM_SCTLR_SELF_REFRESH_STATUS_Msk

#define SDRAM_SCTLR_SET_MODE_REG_Pos (9UL) /*!<SET_MODE_REG[9] */
#define SDRAM_SCTLR_SET_MODE_REG_Msk (0x01UL << SDRAM_SCTLR_SET_MODE_REG_Pos)
#define SDRAM_SCTLR_SET_MODE_REG     SDRAM_SCTLR_SET_MODE_REG_Msk

#define SDRAM_SCTLR_READ_PIPE_Pos (6UL) /*!<READ_PIPE[8:6] */
#define SDRAM_SCTLR_READ_PIPE_Msk (0x07UL << SDRAM_SCTLR_READ_PIPE_Pos)
#define SDRAM_SCTLR_READ_PIPE     SDRAM_SCTLR_READ_PIPE_Msk

#define SDRAM_SCTLR_FULL_REFRESH_AFTER_SR_Pos (5UL) /*!<FULL_REFRESH_AFTER_SR[5] */
#define SDRAM_SCTLR_FULL_REFRESH_AFTER_SR_Msk (0x01UL << SDRAM_SCTLR_FULL_REFRESH_AFTER_SR_Pos)
#define SDRAM_SCTLR_FULL_REFRESH_AFTER_SR     SDRAM_SCTLR_FULL_REFRESH_AFTER_SR_Msk

#define SDRAM_SCTLR_FULL_REFRESH_BEFORE_SR_Pos (4UL) /*!<FULL_REFRESH_BEFORE_SR[4] */
#define SDRAM_SCTLR_FULL_REFRESH_BEFORE_SR_Msk (0x01UL << SDRAM_SCTLR_FULL_REFRESH_BEFORE_SR_Pos)
#define SDRAM_SCTLR_FULL_REFRESH_BEFORE_SR     SDRAM_SCTLR_FULL_REFRESH_BEFORE_SR_Msk

#define SDRAM_SCTLR_PRECHARGE_ALGORITHM_Pos (3UL) /*!<PRECHARGE_ALGORITHM[3] */
#define SDRAM_SCTLR_PRECHARGE_ALGORITHM_Msk (0x01UL << SDRAM_SCTLR_PRECHARGE_ALGORITHM_Pos)
#define SDRAM_SCTLR_PRECHARGE_ALGORITHM     SDRAM_SCTLR_PRECHARGE_ALGORITHM_Msk

#define SDRAM_SCTLR_POWER_DOWN_MODE_Pos (2UL) /*!<POWER_DOWN_MODE[2] */
#define SDRAM_SCTLR_POWER_DOWN_MODE_Msk (0x01UL << SDRAM_SCTLR_POWER_DOWN_MODE_Pos)
#define SDRAM_SCTLR_POWER_DOWN_MODE     SDRAM_SCTLR_POWER_DOWN_MODE_Msk

#define SDRAM_SCTLR_SELF_REFRESH_Pos (1UL) /*!<SELF_REFRESH[1] */
#define SDRAM_SCTLR_SELF_REFRESH_Msk (0x01UL << SDRAM_SCTLR_SELF_REFRESH_Pos)
#define SDRAM_SCTLR_SELF_REFRESH     SDRAM_SCTLR_SELF_REFRESH_Msk

#define SDRAM_SCTLR_INITIALIZE_Pos (0UL) /*!<INITIALIZE[0] */
#define SDRAM_SCTLR_INITIALIZE_Msk (0x01UL << SDRAM_SCTLR_INITIALIZE_Pos)
#define SDRAM_SCTLR_INITIALIZE     SDRAM_SCTLR_INITIALIZE_Msk

/********************  Bit definition for SDRAM_SREFR register  *************/
#define SDRAM_SREFR_REF_Pos (0UL) /*!<REF[15:0] */
#define SDRAM_SREFR_REF_Msk (0xFFFFUL << SDRAM_SREFR_REF_Pos)
#define SDRAM_SREFR_REF     SDRAM_SREFR_REF_Msk

/* --------  Exported types  ------------------------------------------------ */

typedef struct
{
    uint32_t MemoryDataWidth;     /* Specifies SDRAM data width in bits */
    uint32_t BankAddrBitsNumber;  /* Number of bank address bits */
    uint32_t ColumnBitsNumber;    /* Number of address bits for column address */
    uint32_t RowBitsNumber;       /* Number of address bits for row address */
    uint32_t ClkEdgeSel;          /* Select clock sampling edge */
    uint32_t PrechargeAlgorihm;   /* Determines when row is precharged: Immediate, row precharged at end of r/w operation.
                                      Delay, row kept open after r/w operations*/
    uint32_t FullRefreshBeforeSR; /* Refresh All rows or one rows before entering self-refresh */
    uint32_t FullRefreshAfterSR;  /* Refresh All rows or one rows after entering self-refresh */
    uint32_t ReadPipe;            /* Number of registers inserted in read data path for SDRAM in order to correctly latch data */
} SDRAM_InitTypeDef;

typedef struct
{
    uint32_t CASLatency;        /* Delay in clock cycles between read command and availability of first data */
    uint32_t RasMinDelay;       /* Minimum delay between active and precharge commands */
    uint32_t RCDDelay;          /* Minimum delay between active and read/write commands */
    uint32_t RPDelay;           /* Precharge period */
    uint32_t WriteRecoveryTime; /* For writes delay from last data in to next precharge command */
    uint32_t RCARTime;          /* Auto-refresh period; minimum time between two auto-refresh commands */
    uint32_t XSRDelay;          /* Exit self-refresh to active or auto-refresh command time */
    uint32_t RCTime;            /* Active-to-active command period */
    uint32_t InitDelay;         /* Number of clock cycles to hold SDRAM inputs stable after power up, before issuing any commands */
    uint32_t InitRefNumber;     /* Number of auto-refreshes during initialization; */
    uint32_t RefCycle;          /* Number of clock cycles between consecutive refresh cycles */

} SDRAM_TimingTypeDef;

#define SDRAM_MemoryDataWidth_16bit       (0x00UL << SDRAM_SCONR_DATA_WIDTH_Pos)
#define SDRAM_MemoryDataWidth_32bit       (0x01UL << SDRAM_SCONR_DATA_WIDTH_Pos)
#define IS_SDRAM_MEMORY_DATA_WIDTH(WIDTH) (((WIDTH) == SDRAM_MemoryDataWidth_16bit) || ((WIDTH) == SDRAM_MemoryDataWidth_32bit))

#define SDRAM_BankAddrBitsNumber_1bit       (0x00UL << SDRAM_SCONR_BANK_ADDR_WIDTH_Pos)
#define SDRAM_BankAddrBitsNumber_2bit       (0x01UL << SDRAM_SCONR_BANK_ADDR_WIDTH_Pos)
#define IS_SDRAM_BANK_ADDR_BITS_NUMBER(NUM) (((NUM) == SDRAM_BankAddrBitsNumber_1bit) || ((NUM) == SDRAM_BankAddrBitsNumber_2bit))

#define SDRAM_ColumnBitsNumber_8bit  (0x07UL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_ColumnBitsNumber_9bit  (0x08UL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_ColumnBitsNumber_10bit (0x09UL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_ColumnBitsNumber_11bit (0x0AUL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_ColumnBitsNumber_12bit (0x0BUL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_ColumnBitsNumber_13bit (0x0CUL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_ColumnBitsNumber_14bit (0x0DUL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define SDRAM_ColumnBitsNumber_15bit (0x0EUL << SDRAM_SCONR_COL_ADDR_WIDTH_Pos)
#define IS_SDRAM_COLUMN_BITS_NUMBER(NUM)                                                                                              \
    (((NUM) == SDRAM_ColumnBitsNumber_8bit) || ((NUM) == SDRAM_ColumnBitsNumber_9bit) || ((NUM) == SDRAM_ColumnBitsNumber_10bit) ||   \
     ((NUM) == SDRAM_ColumnBitsNumber_11bit) || ((NUM) == SDRAM_ColumnBitsNumber_12bit) || ((NUM) == SDRAM_ColumnBitsNumber_13bit) || \
     ((NUM) == SDRAM_ColumnBitsNumber_14bit) || ((NUM) == SDRAM_ColumnBitsNumber_15bit))

#define SDRAM_RowBitsNumber_11bit (0x0AUL << SDRAM_SCONR_ROW_ADDR_WIDTH_Pos)
#define SDRAM_RowBitsNumber_12bit (0x0BUL << SDRAM_SCONR_ROW_ADDR_WIDTH_Pos)
#define SDRAM_RowBitsNumber_13bit (0x0CUL << SDRAM_SCONR_ROW_ADDR_WIDTH_Pos)
#define SDRAM_RowBitsNumber_14bit (0x0DUL << SDRAM_SCONR_ROW_ADDR_WIDTH_Pos)
#define SDRAM_RowBitsNumber_15bit (0x0EUL << SDRAM_SCONR_ROW_ADDR_WIDTH_Pos)
#define SDRAM_RowBitsNumber_16bit (0x0FUL << SDRAM_SCONR_ROW_ADDR_WIDTH_Pos)
#define IS_SDRAM_ROW_BITS_NUMBER(NUM)                                                                                        \
    (((NUM) == SDRAM_RowBitsNumber_11bit) || ((NUM) == SDRAM_RowBitsNumber_12bit) || ((NUM) == SDRAM_RowBitsNumber_13bit) || \
     ((NUM) == SDRAM_RowBitsNumber_14bit) || ((NUM) == SDRAM_RowBitsNumber_15bit) || ((NUM) == SDRAM_RowBitsNumber_16bit))

#define SDRAM_ClkEdgeSelectRising   (0x01UL)
#define SDRAM_ClkEdgeSelectFalling  (0x00UL)
#define IS_SDRAM_CLK_EDGE_SEL(EDGE) (((EDGE) == SDRAM_ClkEdgeSelectRising) || ((EDGE) == SDRAM_ClkEdgeSelectFalling))

#define SDRAM_PrechargeAlgorihm_Immediate      (0x00UL)
#define SDRAM_PrechargeAlgorihm_Delayed        (0x01UL << SDRAM_SCTLR_PRECHARGE_ALGORITHM_Pos)
#define IS_SDRAM_PRECHARGE_ALGORIHM_MODE(MODE) (((MODE) == SDRAM_PrechargeAlgorihm_Immediate) || ((MODE) == SDRAM_PrechargeAlgorihm_Delayed))

#define SDRAM_RefreshOneRowBeforeEnterSR   (0x00UL)
#define SDRAM_RefreshAllRowBeforeEnterSR   (0x01UL << SDRAM_SCTLR_FULL_REFRESH_BEFORE_SR_Pos)
#define IS_SDRAM_BEFOR_ENTER_SR_MODE(MODE) (((MODE) == SDRAM_RefreshOneRowBeforeEnterSR) || ((MODE) == SDRAM_RefreshAllRowBeforeEnterSR))

#define SDRAM_RefreshOneRowAfterEnterSR    (0x00UL)
#define SDRAM_RefreshAllRowAfterEnterSR    (0x01UL << SDRAM_SCTLR_FULL_REFRESH_AFTER_SR_Pos)
#define IS_SDRAM_AFTER_ENTER_SR_MODE(MODE) (((MODE) == SDRAM_RefreshOneRowAfterEnterSR) || ((MODE) == SDRAM_RefreshAllRowAfterEnterSR))

#define IS_SDRAM_READ_PIPE_NUM(PIPE) ((PIPE) <= 0x07 && (PIPE) >= 0)

#define SDRAM_CASLatency_1CLK (0x00UL)
#define SDRAM_CASLatency_2CLK (0x01UL << SDRAM_STMG0R_CAS_LATE_Pos_1)
#define SDRAM_CASLatency_3CLK (0x02UL << SDRAM_STMG0R_CAS_LATE_Pos_1)
#define SDRAM_CASLatency_4CLK (0x03UL << SDRAM_STMG0R_CAS_LATE_Pos_1)
#define IS_SDRAM_CASLATENCY(LAT) \
    (((LAT) == SDRAM_CASLatency_1CLK) || ((LAT) == SDRAM_CASLatency_2CLK) || ((LAT) == SDRAM_CASLatency_3CLK) || ((LAT) == SDRAM_CASLatency_4CLK))

#define IS_SDRAM_RAS_MIN(RAS)            ((RAS) < 0x10 && (RAS) >= 0)
#define IS_SDRAM_RCD_DELAY(RCD)          ((RCD) < 0x08 && (RCD) >= 0)
#define IS_SDRAM_RP_DELAY(RP)            ((RP) < 0x08 && (RP) >= 0)
#define IS_SDRAM_WRITE_RECOVERY_TIME(WR) ((WR) < 0x03 && (WR) >= 0)
#define IS_SDRAM_RCAR_TIME(TIME)         ((TIME) < 0x10 && (TIME) >= 0)
#define IS_SDRAM_XSR_DELAY(XSR)          ((XSR) < 512 && (XSR) >= 0)
#define IS_SDRAM_RC_TIME(RC)             ((RC) < 0x10 && (RC) >= 0)
#define IS_SDRAM_INIT_DELAY(INIT)        ((INIT) <= 0xFFFF && (INIT) >= 0)
#define IS_SDRAM_INIT_REF_NUM(INIT)      ((INIT) < 0x10 && (INIT) >= 0)
#define IS_SDRAM_REF_NUM(REF)            ((REF) <= 0xFFFF && (REF) >= 0)

#define SDRAM_CommandEnterInitialize  (SDRAM_SCTLR_INITIALIZE)
#define SDRAM_CommandEnterSelfRefresh (SDRAM_SCTLR_SELF_REFRESH)
#define SDRAM_CommandExitSelfRefresh  (SDRAM_SCTLR_SELF_REFRESH | (0x80000000UL))
#define SDRAM_CommandEnterPowerDown   (SDRAM_SCTLR_POWER_DOWN_MODE)
#define SDRAM_CommandExitPowerDown    (SDRAM_SCTLR_POWER_DOWN_MODE | (0x80000000UL))
#define IS_SDRAM_COMMAND(CMD)                                                                                                         \
    (((CMD) == SDRAM_CommandEnterInitialize) || ((CMD) == SDRAM_CommandEnterSelfRefresh) || ((CMD) == SDRAM_CommandEnterPowerDown) || \
     ((CMD) == SDRAM_CommandExitSelfRefresh) || ((CMD) == SDRAM_CommandExitPowerDown))

#define SDRAM_CommandStatus_Initialize  (SDRAM_SCTLR_INITIALIZE)
#define SDRAM_CommandStatus_SelfRefresh (SDRAM_SCTLR_SELF_REFRESH_STATUS)
#define SDRAM_CommandStatus_PowerDown   (SDRAM_SCTLR_POWER_DOWN_MODE)
#define IS_SDRAM_COMMAND_STATUS(STATUS) \
    (((STATUS) == SDRAM_CommandStatus_Initialize) || ((STATUS) == SDRAM_CommandStatus_SelfRefresh) || ((STATUS) == SDRAM_CommandStatus_PowerDown))

#define IS_SDRAM_CHIP_NUMBER(NUM)   ((NUM) < 0x02 && (NUM) >= 0)
#define IS_SDRAM_BASE_ADDRESS(ADDR) ((ADDR) < 0x68000000 && (ADDR) >= 0x60000000)

#define IS_SDRAM_SAMPLE_DELAY(DELAY) ((DELAY) <= 0x1F)

#define SDRAM_CLKDIV_Div1   ((uint32_t)0x00000000)
#define SDRAM_CLKDIV_Div2   ((uint32_t)0x00000080)
#define SDRAM_CLKDIV_Div3   ((uint32_t)0x00000090)
#define SDRAM_CLKDIV_Div4   ((uint32_t)0x000000A0)
#define SDRAM_CLKDIV_Div8   ((uint32_t)0x000000B0)
#define SDRAM_CLKDIV_Div16  ((uint32_t)0x000000C0)
#define SDRAM_CLKDIV_Div128 ((uint32_t)0x000000D0)
#define SDRAM_CLKDIV_Div256 ((uint32_t)0x000000E0)
#define SDRAM_CLKDIV_Div512 ((uint32_t)0x000000F0)
#define IS_SDRAM_CLKDIV_VALUE(VALUE)                                                                                                              \
    (((VALUE) == SDRAM_CLKDIV_Div1) || ((VALUE) == SDRAM_CLKDIV_Div2) || ((VALUE) == SDRAM_CLKDIV_Div3) || ((VALUE) == SDRAM_CLKDIV_Div4) ||      \
     ((VALUE) == SDRAM_CLKDIV_Div8) || ((VALUE) == SDRAM_CLKDIV_Div16) || ((VALUE) == SDRAM_CLKDIV_Div128) || ((VALUE) == SDRAM_CLKDIV_Div256) || \
     ((VALUE) == SDRAM_CLKDIV_Div512))

/* --------  Public Function  ----------------------------------------------- */

extern void SDRAM_DeInit(void);
extern void SDRAM_Init(SDRAM_InitTypeDef* SDRAM_InitStruct);
extern void SDRAM_TimingConfig(SDRAM_TimingTypeDef* Timing);
extern void SDRAM_SendCommand(uint32_t command);

extern FlagStatus SDRAM_GetCommandStatus(uint32_t cmdStatus);

extern void SDRAM_ChipSelectConfig(uint32_t chipNb, uint32_t baseAddr);
extern void SDRAM_SampleDelayConfig(uint32_t SampleDelay);
extern void SDRAM_CLKDivConfig(uint32_t SDRAM_CLKDiv);
extern void SDRAM_RemapCmd(FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif
