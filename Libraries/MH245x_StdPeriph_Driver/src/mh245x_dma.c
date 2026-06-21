// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_dma.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GET_DMA_Y(DMAy_Streamx)    ((DMA_TypeDef*)((uint32_t)DMAy_Streamx & DMA_BASE_MASK))
#define GET_STREAM_X(DMAy_Streamx) ((uint32_t)DMAy_Streamx & 0xFF - 0x10) / 0x18
#define GET_STREAM_FLAG_OFFSET(x)  (((x & 2U) << 3U) + (x & 1U) * 6U)
#define IS_HSTREAM(x)              (x & 4U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Deinitialize the DMAy Streamx registers to their default reset values.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *         to 7 to select the DMA Stream.
 * @retval None
 */
void DMA_DeInit(DMA_Stream_TypeDef* DMAy_Streamx)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

    /* Disable the selected DMAy Streamx */
    DMAy_Streamx->CR &= ~((uint32_t)DMA_SxCR_EN);

    /* Reset DMAy Streamx control register */
    DMAy_Streamx->CR = 0;

    /* Reset DMAy Streamx Number of Data to Transfer register */
    DMAy_Streamx->NDTR = 0;

    /* Reset DMAy Streamx peripheral address register */
    DMAy_Streamx->PAR = 0;

    /* Reset DMAy Streamx memory 0 address register */
    DMAy_Streamx->M0AR = 0;

    /* Reset DMAy Streamx memory 1 address register */
    DMAy_Streamx->M1AR = 0;

    /* Reset DMAy Streamx FIFO control register */
    DMAy_Streamx->FCR = (uint32_t)0x00000021;

    DMA_TypeDef* DMAy = GET_DMA_Y(DMAy_Streamx);

    uint8_t x = GET_STREAM_X(DMAy_Streamx);

    uint8_t flagOffset = GET_STREAM_FLAG_OFFSET(x);

    if (IS_HSTREAM(x))
    {
        DMAy->HIFCR = DMA_FLAG_ALL << flagOffset;
    }
    else
    {
        DMAy->LIFCR = DMA_FLAG_ALL << flagOffset;
    }
}

/**
 * @brief  Initializes the DMAy Streamx according to the specified parameters in
 *         the DMA_InitStruct structure.
 * @note   Before calling this function, it is recommended to check that the Stream
 *         is actually disabled using the function DMA_GetCmdStatus().
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *         to 7 to select the DMA Stream.
 * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
 *         the configuration information for the specified DMA Stream.
 * @retval None
 */
void DMA_Init(DMA_Stream_TypeDef* DMAy_Streamx, DMA_InitTypeDef* DMA_InitStruct)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_CHANNEL(DMA_InitStruct->DMA_Channel));
    assert_param(IS_DMA_DIRECTION(DMA_InitStruct->DMA_DIR));
    assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
    assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
    assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));
    assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
    assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
    assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
    assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
    assert_param(IS_DMA_FIFO_MODE_STATE(DMA_InitStruct->DMA_FIFOMode));
    assert_param(IS_DMA_FIFO_THRESHOLD(DMA_InitStruct->DMA_FIFOThreshold));
    assert_param(IS_DMA_MEMORY_BURST(DMA_InitStruct->DMA_MemoryBurst));
    assert_param(IS_DMA_PERIPHERAL_BURST(DMA_InitStruct->DMA_PeripheralBurst));

    /*------------------------- DMAy Streamx CR Configuration ------------------*/
    /* Get the DMAy_Streamx CR value */
    tmpreg = DMAy_Streamx->CR;

    /* Clear CHSEL, MBURST, PBURST, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
    tmpreg &= ((uint32_t) ~(DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST | DMA_SxCR_PL | DMA_SxCR_MSIZE | DMA_SxCR_PSIZE | DMA_SxCR_MINC | DMA_SxCR_PINC |
                            DMA_SxCR_CIRC | DMA_SxCR_DIR | DMA_SxCR_LINK));

    /* Configure DMAy Streamx: */
    /* Set CHSEL bits according to DMA_CHSEL value */
    /* Set DIR bits according to DMA_DIR value */
    /* Set PINC bit according to DMA_PeripheralInc value */
    /* Set MINC bit according to DMA_MemoryInc value */
    /* Set PSIZE bits according to DMA_PeripheralDataSize value */
    /* Set MSIZE bits according to DMA_MemoryDataSize value */
    /* Set CIRC bit according to DMA_Mode value */
    /* Set PL bits according to DMA_Priority value */
    /* Set MBURST bits according to DMA_MemoryBurst value */
    /* Set PBURST bits according to DMA_PeripheralBurst value */
    tmpreg |= DMA_InitStruct->DMA_Channel | DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
              DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize | DMA_InitStruct->DMA_Mode | DMA_InitStruct->DMA_Priority |
              DMA_InitStruct->DMA_MemoryBurst | DMA_InitStruct->DMA_PeripheralBurst;

    /* Write to DMAy Streamx CR register */
    DMAy_Streamx->CR = tmpreg;

    /*------------------------- DMAy Streamx FCR Configuration -----------------*/
    /* Get the DMAy_Streamx FCR value */
    tmpreg = DMAy_Streamx->FCR;

    /* Clear DMDIS and FTH bits */
    tmpreg &= (uint32_t) ~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);

    /* Configure DMAy Streamx FIFO:
      Set DMDIS bits according to DMA_FIFOMode value
      Set FTH bits according to DMA_FIFOThreshold value */
    tmpreg |= DMA_InitStruct->DMA_FIFOMode | DMA_InitStruct->DMA_FIFOThreshold;

    /* Write to DMAy Streamx CR */
    DMAy_Streamx->FCR = tmpreg;

#if FEATURE_DMA_SUPPORT_LINK
    if (tmpreg & DMA_SxCR_LINK)
    {
        uint8_t x = GET_STREAM_X(DMAy_Streamx);

        GET_DMA_Y(DMAy_Streamx)->LAR[x] = DMA_InitStruct->DMA_Memory0BaseAddr;
        return;
    }
#endif

    /*------------------------- DMAy Streamx NDTR Configuration ----------------*/
    /* Write to DMAy Streamx NDTR register */
    DMAy_Streamx->NDTR = DMA_InitStruct->DMA_BufferSize;

    /*------------------------- DMAy Streamx PAR Configuration -----------------*/
    /* Write to DMAy Streamx PAR */
    DMAy_Streamx->PAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

    /*------------------------- DMAy Streamx M0AR Configuration ----------------*/
    /* Write to DMAy Streamx M0AR */
    DMAy_Streamx->M0AR = DMA_InitStruct->DMA_Memory0BaseAddr;
}

/**
 * @brief  Fills each DMA_InitStruct member with its default value.
 * @param  DMA_InitStruct : pointer to a DMA_InitTypeDef structure which will
 *         be initialized.
 * @retval None
 */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
    /*-------------- Reset DMA init structure parameters values ----------------*/
    /* Initialize the DMA_Channel member */
    DMA_InitStruct->DMA_Channel = 0;

    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStruct->DMA_PeripheralBaseAddr = 0;

    /* Initialize the DMA_Memory0BaseAddr member */
    DMA_InitStruct->DMA_Memory0BaseAddr = 0;

    /* Initialize the DMA_DIR member */
    DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralToMemory;

    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct->DMA_BufferSize = 0;

    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;

    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

    /* Initialize the DMA_Mode member */
    DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;

    /* Initialize the DMA_Priority member */
    DMA_InitStruct->DMA_Priority = DMA_Priority_Low;

    /* Initialize the DMA_FIFOMode member */
    DMA_InitStruct->DMA_FIFOMode = DMA_FIFOMode_Disable;

    /* Initialize the DMA_FIFOThreshold member */
    DMA_InitStruct->DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

    /* Initialize the DMA_MemoryBurst member */
    DMA_InitStruct->DMA_MemoryBurst = DMA_MemoryBurst_Single;

    /* Initialize the DMA_PeripheralBurst member */
    DMA_InitStruct->DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
}

/**
 * @brief  Enables or disables the specified DMAy Streamx.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *         to 7 to select the DMA Stream.
 * @param  NewState: new state of the DMAy Streamx.
 *          This parameter can be: ENABLE or DISABLE.
 *
 * @note  This function may be used to perform Pause-Resume operation. When a
 *        transfer is ongoing, calling this function to disable the Stream will
 *        cause the transfer to be paused. All configuration registers and the
 *        number of remaining data will be preserved. When calling again this
 *        function to re-enable the Stream, the transfer will be resumed from
 *        the point where it was paused.
 *
 * @note  After configuring the DMA Stream (DMA_Init() function) and enabling the
 *        stream, it is recommended to check (or wait until) the DMA Stream is
 *        effectively enabled. A Stream may remain disabled if a configuration
 *        parameter is wrong.
 *        After disabling a DMA Stream, it is also recommended to check (or wait
 *        until) the DMA Stream is effectively disabled. If a Stream is disabled
 *        while a data transfer is ongoing, the current data will be transferred
 *        and the Stream will be effectively disabled only after the transfer of
 *        this single data is finished.
 *
 * @retval None
 */
void DMA_Cmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected DMAy Streamx by setting EN bit */
        DMAy_Streamx->CR |= (uint32_t)DMA_SxCR_EN;
    }
    else
    {
        /* Disable the selected DMAy Streamx by clearing EN bit */
        DMAy_Streamx->CR &= ~(uint32_t)DMA_SxCR_EN;
    }
}

/**
 * @brief  Configures, when the PINC (Peripheral Increment address mode) bit is
 *         set, if the peripheral address should be incremented with the data
 *         size (configured with PSIZE bits) or by a fixed offset equal to 4
 *         (32-bit aligned addresses).
 *
 * @note   This function has no effect if the Peripheral Increment mode is disabled.
 *
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  DMA_Pincos: specifies the Peripheral increment offset size.
 *          This parameter can be one of the following values:
 *            @arg DMA_PINCOS_Psize: Peripheral address increment is done
 *                                   accordingly to PSIZE parameter.
 *            @arg DMA_PINCOS_WordAligned: Peripheral address increment offset is
 *                                         fixed to 4 (32-bit aligned addresses).
 * @retval None
 */
void DMA_PeriphIncOffsetSizeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_Pincos)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_PINCOS_SIZE(DMA_Pincos));

    /* Check the needed Peripheral increment offset */
    if (DMA_Pincos != DMA_PINCOS_Psize)
    {
        /* Configure DMA_SxCR_PINCOS bit with the input parameter */
        DMAy_Streamx->CR |= (uint32_t)DMA_SxCR_PINCOS;
    }
    else
    {
        /* Clear the PINCOS bit: Peripheral address incremented according to PSIZE */
        DMAy_Streamx->CR &= ~(uint32_t)DMA_SxCR_PINCOS;
    }
}

/**
 * @brief  Configures, when the DMAy Streamx is disabled, the flow controller for
 *         the next transactions (Peripheral or Memory).
 *
 * @note   Before enabling this feature, check if the used peripheral supports
 *         the Flow Controller mode or not.
 *
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  DMA_FlowCtrl: specifies the DMA flow controller.
 *          This parameter can be one of the following values:
 *            @arg DMA_FlowCtrl_Memory: DMAy_Streamx transactions flow controller is
 *                                      the DMA controller.
 *            @arg DMA_FlowCtrl_Peripheral: DMAy_Streamx transactions flow controller
 *                                          is the peripheral.
 * @retval None
 */
void DMA_FlowControllerConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FlowCtrl)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_FLOW_CTRL(DMA_FlowCtrl));

    /* Check the needed flow controller  */
    if (DMA_FlowCtrl != DMA_FlowCtrl_Memory)
    {
        /* Configure DMA_SxCR_PFCTRL bit with the input parameter */
        DMAy_Streamx->CR |= (uint32_t)DMA_SxCR_PFCTRL;
    }
    else
    {
        /* Clear the PFCTRL bit: Memory is the flow controller */
        DMAy_Streamx->CR &= ~(uint32_t)DMA_SxCR_PFCTRL;
    }
}

/**
 * @brief  Writes the number of data units to be transferred on the DMAy Streamx.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  Counter: Number of data units to be transferred (from 0 to 65535)
 *          Number of data items depends only on the Peripheral data format.
 *
 * @note   If Peripheral data format is Bytes: number of data units is equal
 *         to total number of bytes to be transferred.
 *
 * @note   If Peripheral data format is Half-Word: number of data units is
 *         equal to total number of bytes to be transferred / 2.
 *
 * @note   If Peripheral data format is Word: number of data units is equal
 *         to total  number of bytes to be transferred / 4.
 *
 * @note   In Memory-to-Memory transfer mode, the memory buffer pointed by
 *         DMAy_SxPAR register is considered as Peripheral.
 *
 * @retval The number of remaining data units in the current DMAy Streamx transfer.
 */
void DMA_SetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx, uint16_t Counter)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

    /* Write the number of data units to be transferred */
    DMAy_Streamx->NDTR = (uint16_t)Counter;
}

/**
 * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @retval The number of remaining data units in the current DMAy Streamx transfer.
 */
uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

    /* Return the number of remaining data units for DMAy Streamx */
    return ((uint16_t)(DMAy_Streamx->NDTR));
}

/**
 * @brief  Configures, when the DMAy Streamx is disabled, the double buffer mode
 *         and the current memory target.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  Memory1BaseAddr: the base address of the second buffer (Memory 1)
 * @param  DMA_CurrentMemory: specifies which memory will be first buffer for
 *         the transactions when the Stream will be enabled.
 *          This parameter can be one of the following values:
 *            @arg DMA_Memory_0: Memory 0 is the current buffer.
 *            @arg DMA_Memory_1: Memory 1 is the current buffer.
 *
 * @note   Memory0BaseAddr is set by the DMA structure configuration in DMA_Init().
 *
 * @retval None
 */
void DMA_DoubleBufferModeConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t Memory1BaseAddr, uint32_t DMA_CurrentMemory)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_CURRENT_MEM(DMA_CurrentMemory));

    if (DMA_CurrentMemory != DMA_Memory_0)
    {
        /* Set Memory 1 as current memory address */
        DMAy_Streamx->CR |= (uint32_t)(DMA_SxCR_CT);
    }
    else
    {
        /* Set Memory 0 as current memory address */
        DMAy_Streamx->CR &= ~(uint32_t)(DMA_SxCR_CT);
    }

    /* Write to DMAy Streamx M1AR */
    DMAy_Streamx->M1AR = Memory1BaseAddr;
}

/**
 * @brief  Enables or disables the double buffer mode for the selected DMA stream.
 * @note   This function can be called only when the DMA Stream is disabled.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  NewState: new state of the DMAy Streamx double buffer mode.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void DMA_DoubleBufferModeCmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Configure the Double Buffer mode */
    if (NewState != DISABLE)
    {
        /* Enable the Double buffer mode */
        DMAy_Streamx->CR |= (uint32_t)DMA_SxCR_DBM;
    }
    else
    {
        /* Disable the Double buffer mode */
        DMAy_Streamx->CR &= ~(uint32_t)DMA_SxCR_DBM;
    }
}

/**
 * @brief  Configures the Memory address for the next buffer transfer in double
 *         buffer mode (for dynamic use). This function can be called when the
 *         DMA Stream is enabled and when the transfer is ongoing.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  MemoryBaseAddr: The base address of the target memory buffer
 * @param  DMA_MemoryTarget: Next memory target to be used.
 *         This parameter can be one of the following values:
 *            @arg DMA_Memory_0: To use the memory address 0
 *            @arg DMA_Memory_1: To use the memory address 1
 *
 * @note    It is not allowed to modify the Base Address of a target Memory when
 *          this target is involved in the current transfer. ie. If the DMA Stream
 *          is currently transferring to/from Memory 1, then it not possible to
 *          modify Base address of Memory 1, but it is possible to modify Base
 *          address of Memory 0.
 *          To know which Memory is currently used, you can use the function
 *          DMA_GetCurrentMemoryTarget().
 *
 * @retval None
 */
void DMA_MemoryTargetConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t MemoryBaseAddr, uint32_t DMA_MemoryTarget)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_CURRENT_MEM(DMA_MemoryTarget));

    /* Check the Memory target to be configured */
    if (DMA_MemoryTarget != DMA_Memory_0)
    {
        /* Write to DMAy Streamx M1AR */
        DMAy_Streamx->M1AR = MemoryBaseAddr;
    }
    else
    {
        /* Write to DMAy Streamx M0AR */
        DMAy_Streamx->M0AR = MemoryBaseAddr;
    }
}

/**
 * @brief  Returns the current memory target used by double buffer transfer.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @retval The memory target number: 0 for Memory0 or 1 for Memory1.
 */
uint32_t DMA_GetCurrentMemoryTarget(DMA_Stream_TypeDef* DMAy_Streamx)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

    /* Get the current memory target */
    if ((DMAy_Streamx->CR & DMA_SxCR_CT) != 0)
    {
        /* Current memory buffer used is Memory 1 */
        tmp = 1;
    }
    else
    {
        /* Current memory buffer used is Memory 0 */
        tmp = 0;
    }
    return tmp;
}

/**
 * @brief  Returns the status of EN bit for the specified DMAy Streamx.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 *
 * @note    After configuring the DMA Stream (DMA_Init() function) and enabling
 *          the stream, it is recommended to check (or wait until) the DMA Stream
 *          is effectively enabled. A Stream may remain disabled if a configuration
 *          parameter is wrong.
 *          After disabling a DMA Stream, it is also recommended to check (or wait
 *          until) the DMA Stream is effectively disabled. If a Stream is disabled
 *          while a data transfer is ongoing, the current data will be transferred
 *          and the Stream will be effectively disabled only after the transfer
 *          of this single data is finished.
 *
 * @retval Current state of the DMAy Streamx (ENABLE or DISABLE).
 */
FunctionalState DMA_GetCmdStatus(DMA_Stream_TypeDef* DMAy_Streamx)
{
    FunctionalState state = DISABLE;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

    if ((DMAy_Streamx->CR & (uint32_t)DMA_SxCR_EN) != 0)
    {
        /* The selected DMAy Streamx EN bit is set (DMA is still transferring) */
        state = ENABLE;
    }
    else
    {
        /* The selected DMAy Streamx EN bit is cleared (DMA is disabled and
            all transfers are complete) */
        state = DISABLE;
    }
    return state;
}

/**
 * @brief  Returns the current DMAy Streamx FIFO filled level.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *         to 7 to select the DMA Stream.
 * @retval The FIFO filling state.
 *           - DMA_FIFOStatus_Less1QuarterFull: when FIFO is less than 1 quarter-full
 *                                               and not empty.
 *           - DMA_FIFOStatus_1QuarterFull: if more than 1 quarter-full.
 *           - DMA_FIFOStatus_HalfFull: if more than 1 half-full.
 *           - DMA_FIFOStatus_3QuartersFull: if more than 3 quarters-full.
 *           - DMA_FIFOStatus_Empty: when FIFO is empty
 *           - DMA_FIFOStatus_Full: when FIFO is full
 */
uint32_t DMA_GetFIFOStatus(DMA_Stream_TypeDef* DMAy_Streamx)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

    /* Get the FIFO level bits */
    tmpreg = (uint32_t)((DMAy_Streamx->FCR & DMA_SxFCR_FS));

    return tmpreg;
}

/**
 * @brief  Checks whether the specified DMAy Streamx flag is set or not.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  DMA_FLAG: specifies the flag to check.
 *          This parameter can be one of the following values:
 *            @arg DMA_FLAG_TCIF:  Stream transfer complete flag
 *            @arg DMA_FLAG_HTIF:  Stream half transfer complete flag
 *            @arg DMA_FLAG_TEIF:  Stream transfer error flag
 *            @arg DMA_FLAG_DMEIF: Stream direct mode error flag
 *            @arg DMA_FLAG_LTCIF: Stream link transfer complete flag
 *            @arg DMA_FLAG_FEIF:  Stream FIFO error flag
 *         Where x can be 0 to 7 to select the DMA Stream.
 * @retval The new state of DMA_FLAG (SET or RESET).
 */
FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_GET_FLAG(DMA_FLAG));

    DMA_TypeDef* DMAy = GET_DMA_Y(DMAy_Streamx);

    /* Get the stream index x */
    uint8_t x = GET_STREAM_X(DMAy_Streamx);

    /* Check if the interrupt pending flag is in LISR or HISR */
    if (IS_HSTREAM(x))
    {
        /* Get DMAy HISR register value */
        tmpreg = DMAy->HISR;
    }
    else
    {
        /* Get DMAy LISR register value */
        tmpreg = DMAy->LISR;
    }

    /* Get the DMA interrupt status bits */
    tmpreg >>= GET_STREAM_FLAG_OFFSET(x);

    /* Check the status of the specified DMA flag */
    if (tmpreg & DMA_FLAG)
    {
        /* DMA_FLAG is set */
        return SET;
    }

    /* DMA_FLAG is reset */
    return RESET;
}

/**
 * @brief  Clears the DMAy Streamx's pending flags.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  DMA_FLAG: specifies the flag to clear.
 *          This parameter can be any combination of the following values:
 *            @arg DMA_FLAG_TCIF:  Stream transfer complete flag
 *            @arg DMA_FLAG_HTIF:  Stream half transfer complete flag
 *            @arg DMA_FLAG_TEIF:  Stream transfer error flag
 *            @arg DMA_FLAG_DMEIF: Stream direct mode error flag
 *            @arg DMA_FLAG_LTCIF: Stream link transfer complete flag
 *            @arg DMA_FLAG_FEIF:  Stream FIFO error flag
 *         Where x can be 0 to 7 to select the DMA Stream.
 * @retval None
 */
void DMA_ClearFlag(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_CLEAR_FLAG(DMA_FLAG));

    DMA_TypeDef* DMAy = GET_DMA_Y(DMAy_Streamx);

    /* Get the stream index x */
    uint8_t x = GET_STREAM_X(DMAy_Streamx);

    DMA_FLAG <<= GET_STREAM_FLAG_OFFSET(x);

    /* Check if LIFCR or HIFCR register is targeted */
    if (IS_HSTREAM(x))
    {
        /* Set DMAy HIFCR register clear flag bits */
        DMAy->HIFCR = DMA_FLAG;
    }
    else
    {
        /* Set DMAy LIFCR register clear flag bits */
        DMAy->LIFCR = DMA_FLAG;
    }
}

/**
 * @brief  Enables or disables the specified DMAy Streamx interrupts.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param DMA_IT: specifies the DMA interrupt sources to be enabled or disabled.
 *          This parameter can be any combination of the following values:
 *            @arg DMA_IT_TC:  Transfer complete interrupt mask
 *            @arg DMA_IT_HT:  Half transfer complete interrupt mask
 *            @arg DMA_IT_TE:  Transfer error interrupt mask
 *            @arg DMA_IT_DME: Direct mode error interrupt
 *            @arg DMA_IT_LTC: Link transfer complete interrupt mask
 *            @arg DMA_IT_FE:  FIFO error interrupt mask
 * @param  NewState: new state of the specified DMA interrupts.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void DMA_ITConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_CONFIG_IT(DMA_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Check if the DMA_IT parameter contains a FIFO interrupt */
    if ((DMA_IT & DMA_IT_FE) != 0)
    {
        if (NewState != DISABLE)
        {
            /* Enable the selected DMA FIFO interrupts */
            DMAy_Streamx->FCR |= (uint32_t)DMA_IT_FE;
        }
        else
        {
            /* Disable the selected DMA FIFO interrupts */
            DMAy_Streamx->FCR &= ~(uint32_t)DMA_IT_FE;
        }
    }

    /* Check if the DMA_IT parameter contains a Transfer interrupt */
    if (DMA_IT != DMA_IT_FE)
    {
        if (NewState != DISABLE)
        {
            /* Enable the selected DMA transfer interrupts */
            DMAy_Streamx->CR |= (uint32_t)(DMA_IT & DMA_TRANSFER_IT_ALL);
        }
        else
        {
            /* Disable the selected DMA transfer interrupts */
            DMAy_Streamx->CR &= ~(uint32_t)(DMA_IT & DMA_TRANSFER_IT_ALL);
        }
    }
}

/**
 * @brief  Checks whether the specified DMAy Streamx interrupt has occurred or not.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  DMA_IT: specifies the DMA interrupt source to check.
 *          This parameter can be one of the following values:
 *            @arg DMA_IT_TCIF:  Stream transfer complete interrupt
 *            @arg DMA_IT_HTIF:  Stream half transfer complete interrupt
 *            @arg DMA_IT_TEIF:  Stream transfer error interrupt
 *            @arg DMA_IT_DMEIF: Stream direct mode error interrupt
 *            @arg DMA_IT_LTCIF: Stream link transfer complete interrupt
 *            @arg DMA_IT_FEIF:  Stream FIFO error interrupt
 *         Where x can be 0 to 7 to select the DMA Stream.
 * @retval The new state of DMA_IT (SET or RESET).
 */
ITStatus DMA_GetITStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT)
{
    uint32_t enablestatus = 0;

    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
    assert_param(IS_DMA_GET_IT(DMA_IT));

    if (DMA_IT == DMA_IT_FEIF)
    {
        enablestatus = (DMAy_Streamx->FCR & DMA_IT_FE) >> 7;
    }
    else
    {
        /* Check if the interrupt enable bit is in the CR or FCR register */
        enablestatus = (DMAy_Streamx->CR & DMA_TRANSFER_IT_ALL) << 1;
#if FEATURE_DMA_SUPPORT_LINK
        enablestatus |= enablestatus >> 24;
#endif
    }

    enablestatus &= DMA_IT;

    if (!enablestatus)
    {
        return RESET;
    }

    return DMA_GetFlagStatus(DMAy_Streamx, enablestatus);
}

/**
 * @brief  Clears the DMAy Streamx's interrupt pending bits.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @param  DMA_IT: specifies the DMA interrupt pending bit to clear.
 *          This parameter can be any combination of the following values:
 *            @arg DMA_IT_TCIF:  Stream transfer complete interrupt
 *            @arg DMA_IT_HTIF:  Stream half transfer complete interrupt
 *            @arg DMA_IT_TEIF:  Stream transfer error interrupt
 *            @arg DMA_IT_DMEIF: Stream direct mode error interrupt
 *            @arg DMA_IT_LTCIF: Stream link transfer complete interrupt
 *            @arg DMA_IT_FEIF:  Stream FIFO error interrupt
 *         Where x can be 0 to 7 to select the DMA Stream.
 * @retval None
 */
void DMA_ClearITPendingBit(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT)
{
    DMA_ClearFlag(DMAy_Streamx, DMA_IT);
}
