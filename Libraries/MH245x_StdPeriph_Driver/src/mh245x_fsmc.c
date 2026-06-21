// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_fsmc.h"

/* Private typedef -----------------------------------------------------------*/
const FSMC_NORSRAMTimingInitTypeDef FSMC_DefaultTimingStruct = {
    0x0F,             /* FSMC_AddressSetupTime */
    0x0F,             /* FSMC_AddressHoldTime */
    0xFF,             /* FSMC_DataSetupTime */
    0x0F,             /* FSMC_BusTurnAroundDuration */
    0x0F,             /* FSMC_CLKDivision */
    0x0F,             /* FSMC_DataLatency */
    FSMC_AccessMode_A /* FSMC_AccessMode */
};
/* Private define ------------------------------------------------------------*/

/* --------------------- FSMC registers bit mask ---------------------------- */
/* FSMC BCRx Mask */
#define BCR_MBKEN_SET   ((uint32_t)0x00000001)
#define BCR_MBKEN_RESET ((uint32_t)0x000FFFFE)
#define BCR_FACCEN_SET  ((uint32_t)0x00000040)

/* FSMC PCRx Mask */
#define PCR_PBKEN_SET       ((uint32_t)0x00000004)
#define PCR_PBKEN_RESET     ((uint32_t)0x000FFFFB)
#define PCR_ECCEN_SET       ((uint32_t)0x00000040)
#define PCR_ECCEN_RESET     ((uint32_t)0x000FFFBF)
#define PCR_MEMORYTYPE_NAND ((uint32_t)0x00000008)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup FSMC_Private_Functions
 * @{
 */

/** @defgroup FSMC_Group1 NOR/SRAM Controller functions
 *  @brief   NOR/SRAM Controller functions
 *
@verbatim
 ===============================================================================
                    ##### NOR and SRAM Controller functions #####
 ===============================================================================

 [..] The following sequence should be followed to configure the FSMC to interface
      with SRAM, PSRAM, NOR or OneNAND memory connected to the NOR/SRAM Bank:

   (#) Enable the clock for the FSMC and associated GPIOs using the following functions:
          RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_FSMC, ENABLE);
          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

   (#) FSMC pins configuration
       (++) Connect the involved FSMC pins to AF12 using the following function
            GPIO_PinAFConfig(GPIOx, GPIO_PinSourcex, GPIO_AF_FSMC);
       (++) Configure these FSMC pins in alternate function mode by calling the function
            GPIO_Init();

   (#) Declare a FSMC_NORSRAMInitTypeDef structure, for example:
          FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
      and fill the FSMC_NORSRAMInitStructure variable with the allowed values of
      the structure member.

   (#) Initialize the NOR/SRAM Controller by calling the function
          FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

   (#) Then enable the NOR/SRAM Bank, for example:
          FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);

   (#) At this stage you can read/write from/to the memory connected to the NOR/SRAM Bank.

@endverbatim
  * @{
  */

/**
 * @brief  De-initializes the FSMC NOR/SRAM Banks registers to their default
 *   reset values.
 * @param  FSMC_Bank: specifies the FSMC Bank to be used
 *          This parameter can be one of the following values:
 *            @arg FSMC_Bank1_NORSRAM1: FSMC Bank1 NOR/SRAM1
 *            @arg FSMC_Bank1_NORSRAM2: FSMC Bank1 NOR/SRAM2
 *            @arg FSMC_Bank1_NORSRAM3: FSMC Bank1 NOR/SRAM3
 *            @arg FSMC_Bank1_NORSRAM4: FSMC Bank1 NOR/SRAM4
 * @retval None
 */
void FSMC_NORSRAMDeInit(uint32_t FSMC_Bank)
{
    /* Check the parameter */
    assert_param(IS_FSMC_NORSRAM_BANK(FSMC_Bank));

    /* FSMC_Bank1_NORSRAM1 */
    if (FSMC_Bank == FSMC_Bank1_NORSRAM1)
    {
        FSMC_Bank1->BTCR[FSMC_Bank] = 0x000030DB;
    }
    /* FSMC_Bank1_NORSRAM2,  FSMC_Bank1_NORSRAM3 or FSMC_Bank1_NORSRAM4 */
    else
    {
        FSMC_Bank1->BTCR[FSMC_Bank] = 0x000030D2;
    }
    FSMC_Bank1->BTCR[FSMC_Bank + 1] = 0x0FFFFFFF;
    FSMC_Bank1E->BWTR[FSMC_Bank]    = 0x0FFFFFFF;
}

/**
 * @brief  Initializes the FSMC NOR/SRAM Banks according to the specified
 *         parameters in the FSMC_NORSRAMInitStruct.
 * @param  FSMC_NORSRAMInitStruct : pointer to a FSMC_NORSRAMInitTypeDef structure
 *         that contains the configuration information for the FSMC NOR/SRAM
 *         specified Banks.
 * @retval None
 */
void FSMC_NORSRAMInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{
    uint32_t tmpbcr = 0, tmpbtr = 0, tmpbwr = 0;

    /* Check the parameters */
    assert_param(IS_FSMC_NORSRAM_BANK(FSMC_NORSRAMInitStruct->FSMC_Bank));
    assert_param(IS_FSMC_MUX(FSMC_NORSRAMInitStruct->FSMC_DataAddressMux));
    assert_param(IS_FSMC_MEMORY(FSMC_NORSRAMInitStruct->FSMC_MemoryType));
    assert_param(IS_FSMC_MEMORY_WIDTH(FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth));
    assert_param(IS_FSMC_BURSTMODE(FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode));
    assert_param(IS_FSMC_ASYNWAIT(FSMC_NORSRAMInitStruct->FSMC_AsynchronousWait));
    assert_param(IS_FSMC_WAIT_POLARITY(FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity));
    assert_param(IS_FSMC_WRAP_MODE(FSMC_NORSRAMInitStruct->FSMC_WrapMode));
    assert_param(IS_FSMC_WAIT_SIGNAL_ACTIVE(FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive));
    assert_param(IS_FSMC_WRITE_OPERATION(FSMC_NORSRAMInitStruct->FSMC_WriteOperation));
    assert_param(IS_FSMC_WAITE_SIGNAL(FSMC_NORSRAMInitStruct->FSMC_WaitSignal));
    assert_param(IS_FSMC_EXTENDED_MODE(FSMC_NORSRAMInitStruct->FSMC_ExtendedMode));
    assert_param(IS_FSMC_WRITE_BURST(FSMC_NORSRAMInitStruct->FSMC_WriteBurst));
    assert_param(IS_FSMC_ADDRESS_SETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime));
    assert_param(IS_FSMC_ADDRESS_HOLD_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime));
    assert_param(IS_FSMC_DATASETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime));
    assert_param(IS_FSMC_TURNAROUND_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration));
    assert_param(IS_FSMC_CLK_DIV(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision));
    assert_param(IS_FSMC_DATA_LATENCY(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency));
    assert_param(IS_FSMC_ACCESS_MODE(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode));

    /* Get the BTCR register value */
    tmpbcr = FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank];

    /* Clear MBKEN, MUXEN, MTYP, MWID, FACCEN, BURSTEN, WAITPOL, WRAPMOD, WAITCFG, WREN,
             WAITEN, EXTMOD, ASYNCWAIT, CBURSTRW and CCLKEN bits */
    tmpbcr &=
        ((uint32_t)~(FSMC_BCR1_MBKEN | FSMC_BCR1_MUXEN | FSMC_BCR1_MTYP | FSMC_BCR1_MWID | FSMC_BCR1_FACCEN | FSMC_BCR1_BURSTEN | FSMC_BCR1_WAITPOL |
                     FSMC_BCR1_WRAPMOD | FSMC_BCR1_WAITCFG | FSMC_BCR1_WREN | FSMC_BCR1_WAITEN | FSMC_BCR1_EXTMOD | FSMC_BCR1_ASYNCWAIT | FSMC_BCR1_CBURSTRW));

    /* Bank1 NOR/SRAM control register configuration */
    tmpbcr |= (uint32_t)FSMC_NORSRAMInitStruct->FSMC_DataAddressMux | FSMC_NORSRAMInitStruct->FSMC_MemoryType | FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth |
              FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode | FSMC_NORSRAMInitStruct->FSMC_AsynchronousWait | FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity |
              FSMC_NORSRAMInitStruct->FSMC_WrapMode | FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive | FSMC_NORSRAMInitStruct->FSMC_WriteOperation |
              FSMC_NORSRAMInitStruct->FSMC_WaitSignal | FSMC_NORSRAMInitStruct->FSMC_ExtendedMode | FSMC_NORSRAMInitStruct->FSMC_WriteBurst;

    FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank] = tmpbcr;

    if (FSMC_NORSRAMInitStruct->FSMC_MemoryType == FSMC_MemoryType_NOR)
    {
        FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank] |= (uint32_t)BCR_FACCEN_SET;
    }

    /* Get the BTCR register value */
    tmpbtr = FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank + 1];

    /* Clear ADDSET, ADDHLD, DATAST, BUSTURN, CLKDIV, DATLAT and ACCMOD bits */
    tmpbtr &=
        ((uint32_t)~(FSMC_BTR1_ADDSET | FSMC_BTR1_ADDHLD | FSMC_BTR1_DATAST | FSMC_BTR1_BUSTURN | FSMC_BTR1_CLKDIV | FSMC_BTR1_DATLAT | FSMC_BTR1_ACCMOD));

    /* Bank1 NOR/SRAM timing register configuration */
    tmpbtr |= (uint32_t)FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime |
              (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime << 4) |
              (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime << 8) |
              (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration << 16) |
              (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision << 20) |
              (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency << 24) |
              FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode;

    FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank + 1] = tmpbtr;

    /* Bank1 NOR/SRAM timing register for write configuration, if extended mode is used */
    if (FSMC_NORSRAMInitStruct->FSMC_ExtendedMode == FSMC_ExtendedMode_Enable)
    {
        assert_param(IS_FSMC_ADDRESS_SETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime));
        assert_param(IS_FSMC_ADDRESS_HOLD_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime));
        assert_param(IS_FSMC_DATASETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime));
        assert_param(IS_FSMC_TURNAROUND_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_BusTurnAroundDuration));
        assert_param(IS_FSMC_ACCESS_MODE(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode));

        /* Get the BWTR register value */
        tmpbwr = FSMC_Bank1E->BWTR[FSMC_NORSRAMInitStruct->FSMC_Bank];

        /* Clear ADDSET, ADDHLD, DATAST, BUSTURN, and ACCMOD bits */
        tmpbwr &= ((uint32_t) ~(FSMC_BWTR1_ADDSET | FSMC_BWTR1_ADDHLD | FSMC_BWTR1_DATAST | FSMC_BWTR1_BUSTURN | FSMC_BWTR1_ACCMOD));

        tmpbwr |= (uint32_t)FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime |
                  (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime << 4) |
                  (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime << 8) |
                  (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_BusTurnAroundDuration << 16) |
                  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode;

        FSMC_Bank1E->BWTR[FSMC_NORSRAMInitStruct->FSMC_Bank] = tmpbwr;
    }
    else
    {
        FSMC_Bank1E->BWTR[FSMC_NORSRAMInitStruct->FSMC_Bank] = 0x0FFFFFFF;
    }
}

/**
 * @brief  Fills each FSMC_NORSRAMInitStruct member with its default value.
 * @param  FSMC_NORSRAMInitStruct: pointer to a FSMC_NORSRAMInitTypeDef structure
 *         which will be initialized.
 * @retval None
 */
void FSMC_NORSRAMStructInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{
    /* Reset NOR/SRAM Init structure parameters values */
    FSMC_NORSRAMInitStruct->FSMC_Bank                  = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStruct->FSMC_DataAddressMux        = FSMC_DataAddressMux_Enable;
    FSMC_NORSRAMInitStruct->FSMC_MemoryType            = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_8b;
    FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStruct->FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStruct->FSMC_WrapMode              = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStruct->FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStruct->FSMC_WaitSignal            = FSMC_WaitSignal_Enable;
    FSMC_NORSRAMInitStruct->FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStruct->FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct = (FSMC_NORSRAMTimingInitTypeDef*)((uint32_t)&FSMC_DefaultTimingStruct);
    FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct     = (FSMC_NORSRAMTimingInitTypeDef*)((uint32_t)&FSMC_DefaultTimingStruct);
}

/**
 * @brief  Enables or disables the specified NOR/SRAM Memory Bank.
 * @param  FSMC_Bank: specifies the FSMC Bank to be used
 *          This parameter can be one of the following values:
 *            @arg FSMC_Bank1_NORSRAM1: FSMC Bank1 NOR/SRAM1
 *            @arg FSMC_Bank1_NORSRAM2: FSMC Bank1 NOR/SRAM2
 *            @arg FSMC_Bank1_NORSRAM3: FSMC Bank1 NOR/SRAM3
 *            @arg FSMC_Bank1_NORSRAM4: FSMC Bank1 NOR/SRAM4
 * @param  NewState: new state of the FSMC_Bank. This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void FSMC_NORSRAMCmd(uint32_t FSMC_Bank, FunctionalState NewState)
{
    assert_param(IS_FSMC_NORSRAM_BANK(FSMC_Bank));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected NOR/SRAM Bank by setting the PBKEN bit in the BCRx register */
        FSMC_Bank1->BTCR[FSMC_Bank] |= BCR_MBKEN_SET;
    }
    else
    {
        /* Disable the selected NOR/SRAM Bank by clearing the PBKEN bit in the BCRx register */
        FSMC_Bank1->BTCR[FSMC_Bank] &= BCR_MBKEN_RESET;
    }
}
/**
 * @}
 */
