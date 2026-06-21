// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_sdram.h"

/**
 * @brief  Deinitializes the SDRAM peripheral registers to their default reset values.
 * @param  None
 * @retval None
 */
void SDRAM_DeInit(void)
{
    PeripheralReset(PeripheralSDRAM);
}

/**
 * @brief  Initializes the SDRAM peripheral according to the specified
 *         parameters in the SDRAM_InitStruct .
 * @param  SDRAM_InitStruct: pointer to a SDRAM_InitTypeDef structure that contains
 *         the configuration information for the specified SDRAM peripheral.
 * @retval None
 */
void SDRAM_Init(SDRAM_InitTypeDef* SDRAM_InitStruct)
{
    uint32_t tmp_reg   = SDRAM->SCONR;
    uint32_t tmp_sctlr = SDRAM->SCTLR;

    assert_param(IS_SDRAM_MEMORY_DATA_WIDTH(SDRAM_InitStruct->MemoryDataWidth));
    assert_param(IS_SDRAM_BANK_ADDR_BITS_NUMBER(SDRAM_InitStruct->BankAddrBitsNumber));
    assert_param(IS_SDRAM_COLUMN_BITS_NUMBER(SDRAM_InitStruct->ColumnBitsNumber));
    assert_param(IS_SDRAM_ROW_BITS_NUMBER(SDRAM_InitStruct->RowBitsNumber));
    assert_param(IS_SDRAM_PRECHARGE_ALGORIHM_MODE(SDRAM_InitStruct->PrechargeAlgorihm));
    assert_param(IS_SDRAM_BEFOR_ENTER_SR_MODE(SDRAM_InitStruct->FullRefreshBeforeSR));
    assert_param(IS_SDRAM_AFTER_ENTER_SR_MODE(SDRAM_InitStruct->FullRefreshAfterSR));
    assert_param(IS_SDRAM_READ_PIPE_NUM(SDRAM_InitStruct->ReadPipe));
    assert_param(IS_SDRAM_CLK_EDGE_SEL(SDRAM_InitStruct->ClkEdgeSel));

    tmp_reg = (tmp_reg & ~SDRAM_SCONR_DATA_WIDTH) | SDRAM_InitStruct->MemoryDataWidth;
    tmp_reg = (tmp_reg & ~SDRAM_SCONR_BANK_ADDR_WIDTH) | SDRAM_InitStruct->BankAddrBitsNumber;
    tmp_reg = (tmp_reg & ~SDRAM_SCONR_COL_ADDR_WIDTH) | SDRAM_InitStruct->ColumnBitsNumber;
    tmp_reg = (tmp_reg & ~SDRAM_SCONR_ROW_ADDR_WIDTH) | SDRAM_InitStruct->RowBitsNumber;

    tmp_sctlr = (tmp_sctlr & ~SDRAM_SCTLR_PRECHARGE_ALGORITHM) | SDRAM_InitStruct->PrechargeAlgorihm;
    tmp_sctlr = (tmp_sctlr & ~SDRAM_SCTLR_FULL_REFRESH_BEFORE_SR) | SDRAM_InitStruct->FullRefreshBeforeSR;
    tmp_sctlr = (tmp_sctlr & ~SDRAM_SCTLR_FULL_REFRESH_AFTER_SR) | SDRAM_InitStruct->FullRefreshAfterSR;
    tmp_sctlr = (tmp_sctlr & ~SDRAM_SCTLR_READ_PIPE) | (SDRAM_InitStruct->ReadPipe << SDRAM_SCTLR_READ_PIPE_Pos);

    SDRAM->SCONR = tmp_reg;
    SDRAM->SCTLR = tmp_sctlr;

    SYSCFG->EXMEMCFG = (SYSCFG->EXMEMCFG & ~SDRAM_ClkEdgeSelectRising) | SDRAM_InitStruct->ClkEdgeSel;

    SDRAM->SCSLR0_LOW = (uint32_t)0x60000000;
    SDRAM->SCSLR1_LOW = (uint32_t)0x64000000;
}

/**
 * @brief  Configuring the SDRAM timing according to the specified
 *         parameters in the SDRAM_TimingStruct .
 * @param  SDRAM_TimingStruct: pointer to a SDRAM_TimingTypeDef structure that contains
 *         the configuration information for the specified SDRAM peripheral.
 * @retval None
 */
void SDRAM_TimingConfig(SDRAM_TimingTypeDef* SDRAM_TimingStruct)
{
    uint32_t tmp_stmg0r = SDRAM->STMG0R;
    uint32_t tmp_stmg1r = SDRAM->STMG1R;

    assert_param(IS_SDRAM_CASLATENCY(SDRAM_TimingStruct->CASLatency));
    assert_param(IS_SDRAM_RAS_MIN(SDRAM_TimingStruct->RasMinDelay));
    assert_param(IS_SDRAM_RCD_DELAY(SDRAM_TimingStruct->RCDDelay));
    assert_param(IS_SDRAM_RP_DELAY(SDRAM_TimingStruct->RPDelay));
    assert_param(IS_SDRAM_WRITE_RECOVERY_TIME(SDRAM_TimingStruct->WriteRecoveryTime));
    assert_param(IS_SDRAM_RCAR_TIME(SDRAM_TimingStruct->RCARTime));
    assert_param(IS_SDRAM_XSR_DELAY(SDRAM_TimingStruct->XSRDelay));
    assert_param(IS_SDRAM_RC_TIME(SDRAM_TimingStruct->RCTime));
    assert_param(IS_SDRAM_INIT_DELAY(SDRAM_TimingStruct->InitDelay));
    assert_param(IS_SDRAM_INIT_REF_NUM(SDRAM_TimingStruct->InitRefNumber));
    assert_param(IS_SDRAM_REF_NUM(SDRAM_TimingStruct->RefCycle));

    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_CAS_LATE) | SDRAM_TimingStruct->CASLatency;
    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_RAS_MIN) | (SDRAM_TimingStruct->RasMinDelay << SDRAM_STMG0R_RAS_MIN_Pos);
    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_RCD) | (SDRAM_TimingStruct->RCDDelay << SDRAM_STMG0R_RCD_Pos);
    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_RP) | (SDRAM_TimingStruct->RPDelay << SDRAM_STMG0R_RP_Pos);
    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_WR) | (SDRAM_TimingStruct->WriteRecoveryTime << SDRAM_STMG0R_WR_Pos);
    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_RCAR) | (SDRAM_TimingStruct->RCARTime << SDRAM_STMG0R_RCAR_Pos);
    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_XSR) | ((SDRAM_TimingStruct->XSRDelay & (0x0fUL)) << SDRAM_STMG0R_XSR_Pos_1) |
                 ((SDRAM_TimingStruct->XSRDelay & (0x1f0UL)) << 23);
    tmp_stmg0r = (tmp_stmg0r & ~SDRAM_STMG0R_RC) | (SDRAM_TimingStruct->RCTime << SDRAM_STMG0R_RC_Pos);

    tmp_stmg1r = (tmp_stmg1r & ~SDRAM_STMG1R_INIT) | (SDRAM_TimingStruct->InitDelay << SDRAM_STMG1R_INIT_Pos);
    tmp_stmg1r = (tmp_stmg1r & ~SDRAM_STMG1R_NUM_INIT_REF) | (SDRAM_TimingStruct->InitRefNumber << SDRAM_STMG1R_NUM_INIT_REF_Pos);

    SDRAM->SREFR = (SDRAM->SREFR & ~SDRAM_SREFR_REF) | SDRAM_TimingStruct->RefCycle;

    SDRAM->STMG0R = tmp_stmg0r;
    SDRAM->STMG1R = tmp_stmg1r;
}

/**
 * @brief  configuring SDRAM control the chip select selection.
 * @param  chipNb: Indicates the index of the Chip select.
 * @param  baseAddr: Indicates the address of the Selected Chip select.
 * @note   The address of CS1 must be greater than CS0.
 *
 * @retval None
 */
void SDRAM_ChipSelectConfig(uint32_t chipNb, uint32_t baseAddr)
{
    assert_param(IS_SDRAM_CHIP_NUMBER(chipNb));
    assert_param(IS_SDRAM_BASE_ADDRESS(baseAddr));

    if (chipNb == 0)
    {
        SDRAM->SCSLR0_LOW = baseAddr & ~(0xFFFFUL);
    }
    else
    {
        SDRAM->SCSLR1_LOW = baseAddr & ~(0xFFFFUL);
    }
}

/**
 * @brief  Sends Command to the SDRAM bank.
 * @param  command: Command to be send.
 *
 * @retval None
 */
void SDRAM_SendCommand(uint32_t command)
{
    assert_param(IS_SDRAM_COMMAND(command));

    if (command & BIT31)
    {
        SDRAM->SCTLR &= ~(command & ~BIT31);
    }
    else
    {
        SDRAM->SCTLR |= command;
    }
}

/**
 * @brief  Checks if the command has been sent successfully.
 * @param  cmdStatus: specifies the command to check.
 *
 * @retval None
 */
FlagStatus SDRAM_GetCommandStatus(uint32_t cmdStatus)
{
    assert_param(IS_SDRAM_COMMAND_STATUS(cmdStatus));

    if (SDRAM->SCTLR & cmdStatus)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
 * @brief  Config the sampling delay of the clock signal.
 * @param  SampleDelay: specifies the delay time.
 *
 * @retval None
 */
void SDRAM_SampleDelayConfig(uint32_t SampleDelay)
{
    assert_param(IS_SDRAM_SAMPLE_DELAY(SampleDelay));

    PeripheralEnable(PeripheralSYSCFG, true);
    if (SampleDelay == 0)
    {
        SYSCFG->EXMEMCFG &= ~BIT6;
        SYSCFG->EXMEMCFG &= ~(0x1FUL << 1);
    }
    else
    {
        SYSCFG->EXMEMCFG = (SYSCFG->EXMEMCFG & ~(0x1FUL << 1)) | (SampleDelay << 1);
        SYSCFG->EXMEMCFG |= BIT6;
    }
}

/**
 * @brief  Configures the SDRAM clock Divider coming from SYSCLK.
 *
 * @param  SDRAM_CLKDiv: specifies the SYSCLK division factor for SDRAM clock .
 *          SDRAM clock frequency = f(SYSCLK) / RCC_SDRAMDiv
 *          This parameter can be one of the following values:
 *            @arg SDRAM_CLKDIV_Div1: SDRAM clock = f(SYSCLK)
 *            @arg SDRAM_CLKDIV_Div2: SDRAM clock = f(SYSCLK)/2
 *            @arg SDRAM_CLKDIV_Div3: SDRAM clock = f(SYSCLK)/3
 *            @arg SDRAM_CLKDIV_Div4: SDRAM clock = f(SYSCLK)/4
 *            @arg SDRAM_CLKDIV_Div8: SDRAM clock = f(SYSCLK)/8
 *            @arg SDRAM_CLKDIV_Div16: SDRAM clock = f(SYSCLK)/16
 *            @arg SDRAM_CLKDIV_Div128: SDRAM clock = f(SYSCLK)/128
 *            @arg SDRAM_CLKDIV_Div256: SDRAM clock = f(SYSCLK)/256
 *            @arg SDRAM_CLKDIV_Div512: SDRAM clock = f(SYSCLK)/512
 *
 * @retval None
 */
void SDRAM_CLKDivConfig(uint32_t SDRAM_CLKDiv)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_SDRAM_CLKDIV_VALUE(SDRAM_CLKDiv));

    tmpreg = RCC->PLLPRE;

    /* Clear PLLPRE[7:4] bits */
    tmpreg &= ~(0x0FUL << 4);

    /* Set PLLSAIDIVR values */
    tmpreg |= SDRAM_CLKDiv;

    /* Store the new value */
    RCC->PLLPRE = tmpreg;
}

/**
 * @brief  Enable or Disable the SDRAM address(0x60000000) Remap to 0x10000000 address.
 *
 * @param  NewState: new state of the specified SDRAM address remap.
 *          This parameter can be: ENABLE or DISABLE.
 *
 * @retval None
 */
void SDRAM_RemapCmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        SYSCFG->MEMRMP |= BIT3;
    }
    else
    {
        SYSCFG->MEMRMP &= ~BIT3;
    }
}
