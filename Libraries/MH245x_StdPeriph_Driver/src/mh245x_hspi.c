// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

/* Include ------------------------------------------------------------------*/
#include "mh245x_hspi.h"

/**
 * @brief  Fills each HSPI_InitStruct member with its default value.
 * @param  HSPI_InitStruct : pointer to a HSPI_InitTypeDef structure which will
 *         be initialized.
 * @retval None
 */
void HSPI_StructInit(HSPI_InitTypeDef* HSPI_InitStruct)
{
    /* Set the default configuration */
    HSPI_InitStruct->HSPI_BAUDR_Sckdv           = 256;
    HSPI_InitStruct->HSPI_Endian_Conv           = HSPI_ENDIAN_CONV_DISABLE;
    HSPI_InitStruct->HSPI_Data_Frame_Size       = 0x07;
    HSPI_InitStruct->HSPI_TranFIFO_Threshold    = 0x0F;
    HSPI_InitStruct->HSPI_ReceiveFIFO_Threshold = 0x00;
    HSPI_InitStruct->HSPI_CSMode                = HSPI_CS_STAYLOW;
    HSPI_InitStruct->HSPI_RxSampleDelay         = 0;
}

/**
 * @brief  Deinitializes the HSPI registers to their default reset values.
 * @param  None
 * @retval None
 */
void HSPI_DeInit(void)
{
    PeripheralReset(PeripheralHSPI);
}

/**
 * @brief  Initializes the HSPI according to the specified parameters in the HSPI_InitStruct.
 * @param  HSPI_InitStruct: pointer to a HSPI_InitTypeDef structure that contains
 *         the configuration information for the HSPI.
 * @retval None
 */
void HSPI_Init(HSPI_InitTypeDef* HSPI_InitStruct)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_HSPI_BAUDR_Sckdv(HSPI_InitStruct->HSPI_BAUDR_Sckdv));
    assert_param(IS_HSPI_ENDIAN_CONV(HSPI_InitStruct->HSPI_Endian_Conv));
    assert_param(IS_HSPI_Data_Frame_Size(HSPI_InitStruct->HSPI_Data_Frame_Size));
    assert_param(IS_HSPI_TFIFO_Threshold(HSPI_InitStruct->HSPI_TranFIFO_Threshold));
    assert_param(IS_HSPI_RFIFO_Threshold(HSPI_InitStruct->HSPI_ReceiveFIFO_Threshold));
    assert_param(IS_HSPI_CS_MODE(HSPI_InitStruct->HSPI_CSMode));
    assert_param(IS_HSPI_RXSAMPLE_DLY(HSPI_InitStruct->HSPI_RxSampleDelay));

    if (HSPI->SSIENR & HSPI_SSIENR_SSIEN)
    {
        HSPI->SSIENR &= ~HSPI_SSIENR_SSIEN;
    }

    HSPI->BAUDR = HSPI_InitStruct->HSPI_BAUDR_Sckdv;
    tmpreg      = HSPI->CTRLR0;
    tmpreg &= ~(HSPI_CTRLR0_FRF | HSPI_CTRLR0_SSTE | HSPI_CTRLR0_DFS32 | HSPI_CTRLR0_SECONV);
    tmpreg |= (uint32_t)(HSPI_InitStruct->HSPI_CSMode | HSPI_InitStruct->HSPI_Endian_Conv | ((uint32_t)HSPI_InitStruct->HSPI_Data_Frame_Size << 16));
    HSPI->CTRLR0        = tmpreg;
    HSPI->TXFTLR        = HSPI_InitStruct->HSPI_TranFIFO_Threshold;
    HSPI->RXFTLR        = HSPI_InitStruct->HSPI_ReceiveFIFO_Threshold;
    HSPI->RX_SAMPLY_DLY = HSPI_InitStruct->HSPI_RxSampleDelay;
    HSPI->SER |= BIT(0);
}

/**
 * @brief  Configure the HSPI as standard SPI according to the specified
 *         parameters in the HSPI_StandardSpiStruct.
 * @param  HSPI_StandardSpiStruct:  pointer to a HSPI_StandardSpiDef structure that
 *         contains the configuration information for standard SPI mode of HSPI.
 * @note   Valid only HSPI Disable
 * @retval None
 */
void HSPI_StandardSPI_Config(HSPI_StandardSpiDef* HSPI_StandardSpiStruct)
{
    uint32_t tmpreg = 0;
    uint32_t txnum  = HSPI_StandardSpiStruct->Tx_NumberDFrame * ((((HSPI->CTRLR0 & HSPI_CTRLR0_DFS32) >> 16) + 1) / 8);

    /* Check the parameters */
    assert_param(IS_HSPI_STANDARDSPI_CKMODE(HSPI_StandardSpiStruct->CKMode));
    assert_param(IS_HSPI_SandardSPI_Transfer_Mode(HSPI_StandardSpiStruct->Transfer_Mode));
    assert_param(IS_HSPI_Rx_NumberDFrame(HSPI_StandardSpiStruct->Rx_NumberDFrame));
    assert_param(IS_HSPI_Tx_NumberDFrame(txnum));

    if (HSPI->SSIENR & HSPI_SSIENR_SSIEN)
    {
        HSPI->SSIENR &= ~HSPI_SSIENR_SSIEN;
    }

    tmpreg = HSPI->CTRLR0;
    tmpreg &= ~(HSPI_CTRLR0_SPIFRF | HSPI_CTRLR0_SCPH | HSPI_CTRLR0_SCPOL | HSPI_CTRLR0_TMOD_0 | HSPI_CTRLR0_TMOD_1);
    tmpreg |= HSPI_Standard_SPI;
    tmpreg |= (uint32_t)(HSPI_StandardSpiStruct->CKMode | HSPI_StandardSpiStruct->Transfer_Mode);
    HSPI->CTRLR0 = tmpreg;

    if (HSPI_StandardSpiStruct->Rx_NumberDFrame != 0)
    {
        HSPI->CTRLR1 = HSPI_StandardSpiStruct->Rx_NumberDFrame - 1;
    }
    else
    {
        HSPI->CTRLR1 = HSPI_StandardSpiStruct->Rx_NumberDFrame;
    }
    HSPI->BYTE_NUM = txnum;
}

/**
 * @brief  Configure the HSPI as Dual SPI according to the specified
 *         parameters in the HSPI_DualSpiStruct.
 * @param  HSPI_DualSpiStruct:  pointer to a HSPI_DualSpiDef structure that
 *         contains the configuration information for Dual SPI mode of HSPI.
 * @note   Valid only HSPI Disable
 * @retval None
 */
void HSPI_DualSPI_Config(HSPI_DualSpiDef* HSPI_DualSpiStruct)
{
    uint32_t tmpreg = 0;
    uint32_t txnum  = HSPI_DualSpiStruct->Tx_NumberDFrame * ((((HSPI->CTRLR0 & HSPI_CTRLR0_DFS32) >> 16) + 1) / 8);

    /* Check the parameters */
    assert_param(IS_HSPI_DUALSPI_CKMODE(HSPI_DualSpiStruct->CKMode));
    assert_param(IS_HSPI_DUALSPI_LINEMODE(HSPI_DualSpiStruct->Line_Mode));
    assert_param(IS_HSPI_DualSPI_Direction(HSPI_DualSpiStruct->Direction_Mode));
    assert_param(IS_HSPI_Wait_Cycles(HSPI_DualSpiStruct->Wait_Cycles));
    assert_param(IS_HSPI_InstLen(HSPI_DualSpiStruct->Instruction_Len));
    assert_param(IS_HSPI_Address_Len(HSPI_DualSpiStruct->Address_Len));
    assert_param(IS_HSPI_Rx_NumberDFrame(HSPI_DualSpiStruct->Rx_NumberDFrame));
    assert_param(IS_HSPI_Tx_NumberDFrame(txnum));

    if (HSPI->SSIENR & HSPI_SSIENR_SSIEN)
    {
        HSPI->SSIENR &= ~HSPI_SSIENR_SSIEN;
    }

    tmpreg = HSPI->CTRLR0;
    tmpreg &= ~(HSPI_CTRLR0_SPIFRF | HSPI_CTRLR0_SCPH | HSPI_CTRLR0_SCPOL | HSPI_CTRLR0_TMOD_0 | HSPI_CTRLR0_TMOD_1);
    tmpreg |= HSPI_Dual_SPI;
    tmpreg |= (uint32_t)(HSPI_DualSpiStruct->CKMode | HSPI_DualSpiStruct->Direction_Mode);
    HSPI->CTRLR0 = tmpreg;

    tmpreg = HSPI->SPI_CTRLR0;
    tmpreg &= ~(HSPI_SPICTRLR0_WAITCYCLES | HSPI_SPICTRLR0_INSTL | HSPI_SPICTRLR0_ADDRL | HSPI_SPICTRLR0_TRANSTYPE);
    tmpreg |= (uint32_t)(HSPI_DualSpiStruct->Address_Len | HSPI_DualSpiStruct->Instruction_Len | ((uint32_t)HSPI_DualSpiStruct->Wait_Cycles << 11) |
                         HSPI_DualSpiStruct->Line_Mode);
    HSPI->SPI_CTRLR0 = tmpreg;

    if (HSPI_DualSpiStruct->Rx_NumberDFrame != 0)
    {
        HSPI->CTRLR1 = HSPI_DualSpiStruct->Rx_NumberDFrame - 1;
    }
    else
    {
        HSPI->CTRLR1 = HSPI_DualSpiStruct->Rx_NumberDFrame;
    }
    HSPI->BYTE_NUM = txnum;
}

/**
 * @brief  Configure the HSPI as Quad SPI according to the specified
 *         parameters in the HSPI_DualSpiStruct.
 * @param  HSPI_QuadSpiStruct:  pointer to a HSPI_QuadSpiDef structure that
 *         contains the configuration information for Quad SPI mode of HSPI.
 * @note   Valid only HSPI Disable
 * @retval None
 */
void HSPI_QuadSPI_Config(HSPI_QuadSpiDef* HSPI_QuadSpiStruct)
{
    uint32_t tmpreg = 0;
    uint32_t txnum  = HSPI_QuadSpiStruct->Tx_NumberDFrame * ((((HSPI->CTRLR0 & HSPI_CTRLR0_DFS32) >> 16) + 1) / 8);

    /* Check the parameters */
    assert_param(IS_HSPI_QUADSPI_CKMODE(HSPI_QuadSpiStruct->CKMode));
    assert_param(IS_HSPI_QUADSPI_LINEMODE(HSPI_QuadSpiStruct->Line_Mode));
    assert_param(IS_HSPI_QUADSPI_Direction(HSPI_QuadSpiStruct->Direction_Mode));
    assert_param(IS_HSPI_Wait_Cycles(HSPI_QuadSpiStruct->Wait_Cycles));
    assert_param(IS_HSPI_InstLen(HSPI_QuadSpiStruct->Instruction_Len));
    assert_param(IS_HSPI_Address_Len(HSPI_QuadSpiStruct->Address_Len));
    assert_param(IS_HSPI_Rx_NumberDFrame(HSPI_QuadSpiStruct->Rx_NumberDFrame));
    assert_param(IS_HSPI_Tx_NumberDFrame(txnum));

    if (HSPI->SSIENR & HSPI_SSIENR_SSIEN)
    {
        HSPI->SSIENR &= ~HSPI_SSIENR_SSIEN;
    }

    tmpreg = HSPI->CTRLR0;
    tmpreg &= ~(HSPI_CTRLR0_SPIFRF | HSPI_CTRLR0_SCPH | HSPI_CTRLR0_SCPOL | HSPI_CTRLR0_TMOD_0 | HSPI_CTRLR0_TMOD_1);
    tmpreg |= HSPI_Quad_SPI;
    tmpreg |= (uint32_t)(HSPI_QuadSpiStruct->CKMode | HSPI_QuadSpiStruct->Direction_Mode);
    HSPI->CTRLR0 = tmpreg;

    tmpreg = HSPI->SPI_CTRLR0;
    tmpreg &= ~(HSPI_SPICTRLR0_WAITCYCLES | HSPI_SPICTRLR0_INSTL | HSPI_SPICTRLR0_ADDRL | HSPI_SPICTRLR0_TRANSTYPE);
    tmpreg |= (uint32_t)(HSPI_QuadSpiStruct->Address_Len | HSPI_QuadSpiStruct->Instruction_Len | ((uint32_t)HSPI_QuadSpiStruct->Wait_Cycles << 11) |
                         HSPI_QuadSpiStruct->Line_Mode);
    HSPI->SPI_CTRLR0 = tmpreg;

    if (HSPI_QuadSpiStruct->Rx_NumberDFrame != 0)
    {
        HSPI->CTRLR1 = HSPI_QuadSpiStruct->Rx_NumberDFrame - 1;
    }
    else
    {
        HSPI->CTRLR1 = HSPI_QuadSpiStruct->Rx_NumberDFrame;
    }
    HSPI->BYTE_NUM = txnum;
}

/**
 * @brief  Configure data frame size to FIFO
 * @param  DataFrameSize:  data frame size to FIFO
 *         range is 3~31.
 * @note   Valid only HSPI Disable
 * @retval None
 */
void HSPI_DataFrameSizeConfig(uint32_t DataFrameSize)
{
    /* Check the parameters */
    assert_param(IS_HSPI_Data_Frame_Size(DataFrameSize));

    if (HSPI->SSIENR & HSPI_SSIENR_SSIEN)
    {
        HSPI->SSIENR &= ~HSPI_SSIENR_SSIEN;
    }

    HSPI->CTRLR0 = (HSPI->CTRLR0 & (~HSPI_CTRLR0_DFS32)) | (DataFrameSize << 16);
}

/**
 * @brief  Configure the HSPI rxd sample delay
 * @param  SampleDelay:  Sample delay value
 * @note   Valid only HSPI Disable
 * @retval None
 */
void HSPI_RxSampleDelay_Config(uint32_t SampleDelay)
{
    /* Check the parameters */
    assert_param(IS_HSPI_RXSAMPLE_DLY(SampleDelay));

    if (HSPI->SSIENR & HSPI_SSIENR_SSIEN)
    {
        HSPI->SSIENR &= ~HSPI_SSIENR_SSIEN;
    }

    HSPI->RX_SAMPLY_DLY = SampleDelay;
}

/**
 * @brief  Configure the DMA of HSPI according to the specified
 *         parameters in the HSPI_DmaConfigStruct.
 * @param  HSPI_DmaConfigStruct:  pointer to a HSPI_DMAConfigTypeDef structure that
 *         contains the configuration information for DMA of HSPI.
 * @note   Valid only HSPI Disable
 * @retval None
 */
void HSPI_DmaConfig(HSPI_DMAConfigTypeDef* HSPI_DmaConfigStruct)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_HSPI_RDMA_DLevel(HSPI_DmaConfigStruct->HSPI_RDMA_DLevel));
    assert_param(IS_HSPI_TDMA_DLevel(HSPI_DmaConfigStruct->HSPI_TDMA_DLevel));
    assert_param(IS_HSPI_RDMA(HSPI_DmaConfigStruct->HSPI_Receive_DMA));
    assert_param(IS_HSPI_TDMA(HSPI_DmaConfigStruct->HSPI_Tran_DMA));

    HSPI->DMATDLR = HSPI_DmaConfigStruct->HSPI_TDMA_DLevel;
    HSPI->DMARDLR = HSPI_DmaConfigStruct->HSPI_RDMA_DLevel;
    tmpreg        = HSPI->DMACR;
    tmpreg &= ~(HSPI_DMACR_RDMAE | HSPI_DMACR_TDMAE);
    tmpreg |= (uint32_t)(HSPI_DmaConfigStruct->HSPI_Receive_DMA | HSPI_DmaConfigStruct->HSPI_Tran_DMA);
    HSPI->DMACR = tmpreg;
}

/**
 * @brief  Enable or disables the HSPI peripheral.
 * @param  NewState:  new state of the HSPI peripheral.
 *          This parameter can be: ENABLE or DISABLE.
 * @note   None
 * @retval None
 */
void HSPI_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the HSPI */
        HSPI->SSIENR |= HSPI_SSIENR_SSIEN;
    }
    else
    {
        /* Disable the HSPI */
        HSPI->SSIENR &= ~HSPI_SSIENR_SSIEN;
    }
}

/**
 * @brief  Get tranmsit FIFO current data level.
 * @param  None
 * @note   None
 * @retval Tranmsit FIFO current data level
 */
uint32_t HSPI_GetTxFifoLevel(void)
{
    return HSPI->TXFLR;
}

/**
 * @brief  Get receive FIFO current data level.
 * @param  None
 * @note   None
 * @retval receive FIFO current data level
 */
uint32_t HSPI_GetRxFifoLevel(void)
{
    return HSPI->RXFLR;
}

/**
 * @brief  Enables or disables the specified HSPI interrupts.
 * @param  HSPI_IT: specifies the HSPI interrupt sources to be enabled or disabled.
 *          This parameter can be one of the following values:
 *            @arg HSPI_IT_TX_EMPTY: Transmit FIFO empty interrupt mask
 *            @arg HSPI_IT_TX_OVERFLOW: Transmit FIFO overflow interrupt mask
 *            @arg HSPI_IT_RX_UNDERFLOW: Receive FIFO underflow interrupt mask
 *            @arg HSPI_IT_RX_OVERFLOW: Receive FIFO overflow interrupt mask
 *            @arg HSPI_IT_RX_FULL: Receive FIFO full interrupt mask
 *            @arg HSPI_IT_MUILT_MASTER: Multi-Master Contention Interrupt Mask
 * @param  NewState: new state of the specified HSPI interrupts.
 *          This parameter can be: ENABLE or DISABLE.
 * @note   Valid only HSPI Disable
 * @retval None
 */
void HSPI_ITConfig(uint16_t HSPI_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_HSPI_CONFIG_IT(HSPI_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        HSPI->IMR |= HSPI_IT;
    }
    else
    {
        /* Disable the Interrupt sources */
        HSPI->IMR &= ~HSPI_IT;
    }
}

/**
 * @brief  Checks whether the specified HSPI flag is set or not.
 * @param  HSPI_FLAG: specifies the flag to check.
 *          This parameter can be one of the following values:
 *            @arg HSPI_FLAG_BUSY: HSPI busy flag
 *            @arg HSPI_FLAG_TX_FIFO_NOT_FULL: Transmit FIFO not full flag
 *            @arg HSPI_FLAG_TX_FIFO_EMPTY: Transmit FIFO Empty flag
 *            @arg HSPI_FLAG_RX_FIFO_NOT_EMPTY: Receive FIFO Not Empty flag
 *            @arg HSPI_FLAG_RX_FIFO_FULL: Receive FIFO Full flag
 *            @arg HSPI_FLAG_TX_FIFO_ERROR: Transmission Error flag
 *            @arg HSPI_FLAG_DATA_COLLISION_ERROR: Data Collision Error flag
 *            @arg HSPI_FLAG_TX_FIFO_EMPTY_IM: Transmit FIFO Empty Interrupt Mask flag
 *            @arg HSPI_FLAG_TX_FIFO_OVERFLOW_IM: Transmit FIFO Overflow Interrupt Mask flag
 *            @arg HSPI_FLAG_RX_FIFO_UNDERLOW_IM: Receive FIFO Underflow Interrupt Mask flag
 *            @arg HSPI_FLAG_RX_FIFO_OVERFLOW_IM: Receive FIFO Overflow Interrupt Mask flag
 *            @arg HSPI_FLAG_RX_FIFO_FULL_IM: Receive FIFO Full Interrupt Mask flag
 *            @arg HSPI_FLAG_MULTI_MASTER_IM: Multi-Master Contention Interrupt Mask flag
 *            @arg HSPI_FLAG_TX_FIFO_EMPTY_RI: Transmit FIFO Empty Raw Interrupt Status is Active flag
 *            @arg HSPI_FLAG_TX_FIFO_OVERFLOW_RI: Transmit FIFO Overflow Raw Interrupt Status is Active flag
 *            @arg HSPI_FLAG_RX_FIFO_UNDERLOW_RI: Receive FIFO Underflow Raw Interrupt Status is Active flag
 *            @arg HSPI_FLAG_RX_FIFO_OVERFLOW_RI: Receive FIFO Overflow Raw Interrupt Status is Active flag
 *            @arg HSPI_FLAG_RX_FIFO_FULL_RI: Receive FIFO Full Raw Interrupt Status is Active flag
 *            @arg HSPI_FLAG_MULTI_MASTER_RI: Multi-Master Contention Raw Interrupt Status is Active flag
 * @retval The new state of HSPI_FLAG (SET or RESET).
 */
FlagStatus HSPI_GetFlagStatus(uint32_t HSPI_FLAG)
{
    FlagStatus bitstatus = RESET;
    uint32_t   hspireg = 0, tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_HSPI_GET_FLAG(HSPI_FLAG));

    hspireg = HSPI_FLAG >> 28;
    if (hspireg == 0x00)
    {
        tmpreg = HSPI->SR;
    }
    else if (hspireg == 0x01)
    {
        tmpreg = HSPI->IMR;
    }
    else if (hspireg == 0x02)
    {
        tmpreg = HSPI->RISR;
    }

    if ((tmpreg & HSPI_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the HSPI_FLAG status */
    return bitstatus;
}

/**
 * @brief  Clears the HSPI's pending flags.
 * @param  HSPI_IT: specifies the flag to clear.
 *          This parameter can be any combination of the following values:
 *            @arg HSPI_IT_TX_OVERFLOW: Clear Transmit FIFO Overflow Interrupt
 *            @arg HSPI_IT_RX_UNDERFLOW: Clear Receive FIFO Underflow Interrupt
 *            @arg HSPI_IT_RX_OVERFLOW: Clear Receive FIFO Overflow Interrupt
 *            @arg HSPI_IT_MUILT_MASTER: Clear Multi-Master Interrupt
 * @retval None
 */
void HSPI_ClearITPendingBit(uint32_t HSPI_IT)
{
    volatile uint32_t tempreg = 0;

    assert_param(IS_HSPI_CLEAR_IT(HSPI_IT));

    if (HSPI_IT & HSPI_IT_TX_OVERFLOW)
    {
        tempreg = HSPI->TXOICR;
    }

    if (HSPI_IT & HSPI_IT_RX_OVERFLOW)
    {
        tempreg = HSPI->RXOICR;
    }

    if (HSPI_IT & HSPI_IT_RX_UNDERFLOW)
    {
        tempreg = HSPI->RXUICR;
    }

    if (HSPI_IT & HSPI_IT_MUILT_MASTER)
    {
        tempreg = HSPI->MSTICR;
    }
}

/**
 * @brief  Checks whether the specified HSPI interrupt has occurred or not.
 * @param  HSPI_IT: specifies the HSPI interrupt source to check.
 *          This parameter can be one of the following values:
 *            @arg HSPI_IT_TX_EMPTY: Transmit FIFO Empty Interrupt mask
 *            @arg HSPI_IT_TX_OVERFLOW: Transmit FIFO Overflow Interrupt mask
 *            @arg HSPI_IT_RX_UNDERFLOW: Receive FIFO Underflow Interrupt mask
 *            @arg HSPI_IT_RX_OVERFLOW: Receive FIFO Overflow Interrupt mask
 *            @arg HSPI_IT_RX_FULL: Receive FIFO Full Interrupt mask
 *            @arg HSPI_IT_MUILT_MASTER: Multi-Master Contention Interrupt mask
 * @retval The new state of HSPI_IT (SET or RESET).
 */
ITStatus HSPI_GetITStatus(uint32_t HSPI_IT)
{
    ITStatus bitstatus = RESET;
    uint32_t itstatus  = 0;

    /* Check the parameters */
    assert_param(IS_HSPI_GET_IT(HSPI_IT));

    itstatus = HSPI->ISR & HSPI_IT; /* Only masked interrupts are checked */
    if ((itstatus != (uint16_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
 * @brief  Set data to hspi FIFO.
 * @param  data: data to hspi FIFO .
 * @retval None
 */
void HSPI_SendData(uint32_t data)
{
    HSPI->DR = data;
}

/**
 * @brief  Get data from hspi FIFO.
 * @param  None
 * @retval data from hspi FIFO
 */
uint32_t HSPI_ReceiveData(void)
{
    return HSPI->DR;
}
