// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_sysctrl.h"

/**
 * @brief  Resets the RCC clock configuration to the default reset state.
 * @note   The default reset state of the clock configuration is given below:
 *            - HSI ON and used as system clock source
 *            - HSE, PLL1, PLL2 and PLL3 OFF
 *            - AHB, APB1 and APB2 prescaler set to 1.
 *            - CSS, MCO1 and MCO2 OFF
 *            - All interrupts disabled
 * @note   This function doesn't modify the configuration of the
 *            - Peripheral clocks
 *            - LSI, LSE and RTC clocks
 * @param  None
 * @retval None
 */
void RCC_DeInit(void)
{
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000;

    /* Reset HSEON, CSSON, PLLON, PLL2 bits */
    RCC->CR &= (uint32_t)0xEAF6FFFF;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24006010;

    // PLL 2
    RCC->PLL2CFGR = 0x20003000;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;

    /* Disable Timers clock prescalers selection */
    RCC->DCKCFGR = 0x00000000;
}

/**
 * @brief  Configures the External High Speed oscillator (HSE).
 * @note   After enabling the HSE (RCC_HSE_ON), the application software should
 *         wait on HSERDY flag to be set indicating that HSE clock
 *         is stable and can be used to clock the PLL and/or system clock.
 * @note   HSE state can not be changed if it is used directly or through the
 *         PLL as system clock. In this case, you have to select another source
 *         of the system clock then change the HSE state (ex. disable it).
 * @note   The HSE is stopped by hardware when entering STOP and STANDBY modes.
 * @note   This function reset the CSSON bit, so if the Clock security system(CSS)
 *         was previously enabled you have to enable it again after calling this
 *         function.
 * @param  NewState: specifies the new state of the HSE.
 * @retval None
 */
ErrorStatus RCC_HSECmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    ClockEnable(ClockNodeECS, false);
    return (ErrorStatus)ClockEnable(ClockNodeECS, (bool)NewState);
}

/**
 * @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
 * @note   The calibration is used to compensate for the variations in voltage
 *         and temperature that influence the frequency of the internal HSI RC.
 * @param  HSICalibrationValue: specifies the calibration trimming value.
 *         This parameter must be a number between 0 and 0x1F.
 * @retval None
 */
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));

    tmpreg = RCC->CR;

    /* Clear HSITRIM[4:0] bits */
    tmpreg &= ~RCC_CR_HSITRIM;

    /* Set the HSITRIM[4:0] bits according to HSICalibrationValue value */
    tmpreg |= (uint32_t)HSICalibrationValue << 3;

    /* Store the new value */
    RCC->CR = tmpreg;
}

/**
 * @brief  Enables or disables the Internal High Speed oscillator (HSI).
 * @note   The HSI is stopped by hardware when entering STOP and STANDBY modes.
 *         It is used (enabled by hardware) as system clock source after startup
 *         from Reset, wakeup from STOP and STANDBY mode, or in case of failure
 *         of the HSE used directly or indirectly as system clock (if the Clock
 *         Security System CSS is enabled).
 * @note   HSI can not be stopped if it is used as system clock source. In this case,
 *         you have to select another source of the system clock then stop the HSI.
 * @note   After enabling the HSI, the application software should wait on HSIRDY
 *         flag to be set indicating that HSI clock is stable and can be used as
 *         system clock source.
 * @param  NewState: new state of the HSI.
 *          This parameter can be: ENABLE or DISABLE.
 * @note   When the HSI is stopped, HSIRDY flag goes low after 6 HSI oscillator
 *         clock cycles.
 * @retval None
 */
void RCC_HSICmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    ClockEnable(ClockNodeICS, NewState);
}

/**
 * @brief  Configures the External Low Speed oscillator (LSE).
 * @note   As the LSE is in the Backup domain and write access is denied to
 *         this domain after reset, you have to enable write access using
 *         PWR_BackupAccessCmd(ENABLE) function before to configure the LSE
 *         (to be done once after reset).
 * @note   After enabling the LSE (RCC_LSE_ON or RCC_LSE_Bypass), the application
 *         software should wait on LSERDY flag to be set indicating that LSE clock
 *         is stable and can be used to clock the RTC.
 * @param  RCC_LSE: specifies the new state of the LSE.
 *          This parameter can be one of the following values:
 *            @arg RCC_LSE_OFF: turn OFF the LSE oscillator, LSERDY flag goes low after
 *                              6 LSE oscillator clock cycles.
 *            @arg RCC_LSE_ON: turn ON the LSE oscillator
 *            @arg RCC_LSE_Bypass: LSE oscillator bypassed with external clock
 * @retval None
 */
void RCC_LSEConfig(uint8_t RCC_LSE)
{
    /* Check the parameters */
    assert_param(IS_RCC_LSE(RCC_LSE));

    /* Reset LSEON and LSEBYP bits before configuring the LSE ------------------*/
    /* Reset LSEON bit */
    RCC->BDCR &= ~RCC_BDCR_LSEON;

    /* Reset LSEBYP bit */
    RCC->BDCR &= ~RCC_BDCR_LSEBYP;

    /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) */
    switch (RCC_LSE)
    {
        case RCC_LSE_ON:
            /* Set LSEON bit */
            RCC->BDCR |= RCC_LSE_ON;
            break;
        case RCC_LSE_Bypass:
            /* Set LSEBYP and LSEON bits */
            RCC->BDCR |= (RCC_LSE_Bypass | RCC_LSE_ON);
            break;
        default:
            break;
    }
}

/**
 * @brief  Configures the main PLL clock source, multiplication and division factors.
 * @note   This function must be used only when the main PLL is disabled.
 *
 * @param  PLLSource: specifies the PLL entry clock source.
 *          This parameter can be one of the following values:
 *            @arg RCC_PLLSource_HSI: HSI oscillator clock selected as PLL clock entry
 *            @arg RCC_PLLSource_HSE: HSE oscillator clock selected as PLL clock entry
 * @note   This clock source (RCC_PLLSource) is common for the main PLL, PLL2 and PLL3.
 *
 * @param  m: specifies the division factor for PLL VCO input clock
 *          This parameter must be a number between 0 and 63.
 * @note   You have to set the PLLM parameter correctly to ensure that the VCO input
 *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
 *         of 2 MHz to limit PLL jitter.
 *
 * @param  n: specifies the multiplication factor for PLL VCO output clock
 *          This parameter must be a number between 50 and 432.
 * @note   You have to set the parameter correctly to ensure that the VCO
 *         output frequency is between 50 and 432 MHz.
 *
 * @param  p: specifies the division factor for main system clock (SYSCLK)
 *          This parameter must be a number in the range {2, 4, 6, or 8}.
 * @note   You have to set the PLLP parameter correctly to not exceed 168 MHz on
 *         the System clock frequency.
 *
 * @param  q: specifies the division factor for reference clock source
 *          This parameter must be a number between 2 and 15.
 * @note   If the PLLQ is selected as 48M reference clock source, you have to set the
 *         PLLQ parameter correctly to have 48 MHz clock.
 *
 * @param  r: specifies the division factor for reference clock source
 *          This parameter must be a number between 2 and 7.
 *
 * @retval None
 */
void RCC_PLL1Config(uint32_t PLLSource, uint32_t m, uint32_t n, uint32_t p, uint32_t q, uint32_t r)
{
    /* Check the parameters */
    assert_param(IS_RCC_PLL_SOURCE(PLLSource));
    assert_param(IS_RCC_PLLM_VALUE(m));
    assert_param(IS_RCC_PLLN_VALUE(n));
    assert_param(IS_RCC_PLLP_VALUE(p));
    assert_param(IS_RCC_PLLQ_VALUE(q));
    assert_param(IS_RCC_PLLR_VALUE(r));

    ClockSelect(ClockNodeSRC, (ClockNodeEnum)PLLSource);
    ClockDivide(ClockNodePLL1M, m);
    ClockMultiply(ClockNodePLL1, n);
    ClockDivide(ClockNodePLL1P, p);
    ClockDivide(ClockNodePLL1Q, q);
    ClockDivide(ClockNodePLL1R, r);
}

/**
 * @brief  Enables or disables the main PLL.
 * @note   After enabling the main PLL, the application software should wait on
 *         PLLRDY flag to be set indicating that PLL clock is stable and can
 *         be used as system clock source.
 * @note   The main PLL can not be disabled if it is used as system clock source
 * @note   The main PLL is disabled by hardware when entering STOP and STANDBY modes.
 * @param  NewState: new state of the main PLL. This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_PLL1Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    ClockEnable(ClockNodePLL1G, NewState);
}

/**
 * @brief  Configures the PLL2 clock multiplication and division factors.
 *
 * @note   This function must be used only when the PLL2 is disabled.
 * @note   PLL2 clock source is common with the main PLL (configured in RCC_PLL1Config function)
 *
 * @param  n: specifies the multiplication factor for PLL VCO output clock
 *          This parameter must be a number between 50 and 432.
 * @note   You have to set the parameter correctly to ensure that the VCO output frequency
 *         is between 50 and 432 MHz.
 *
 * @param  p: specifies the division factor for peripherals
 *          This parameter must be a number between 2 and 7.
 *
 * @param  q: specifies the division factor for
 *          This parameter must be a number between 2 and 15.
 *
 * @param  r: specifies the division factor for peripherals
 *          This parameter must be a number between 2 and 7.
 *
 * @retval None
 */
void RCC_PLL2Config(uint32_t n, uint32_t p, uint32_t q, uint32_t r)
{
    /* Check the parameters */
    assert_param(IS_RCC_PLLN_VALUE(n));
    assert_param(IS_RCC_PLLP_VALUE(p));
    assert_param(IS_RCC_PLLQ_VALUE(q));
    assert_param(IS_RCC_PLLR_VALUE(r));

    ClockMultiply(ClockNodePLL2, n);
    ClockDivide(ClockNodePLL2P, p);
    ClockDivide(ClockNodePLL2Q, q);
    ClockDivide(ClockNodePLL2R, r);
}

/**
 * @brief  Enables or disables the PLL2.
 * @note   The PLL2 is disabled by hardware when entering STOP and STANDBY modes.
 * @param  NewState: new state of the PLL2. This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_PLL2Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    ClockEnable(ClockNodePLL2G, NewState);
}

/*!
    @brief  Configures the PLL3.

    @note Usage:
        RCC_PLL3Cmd(DISABLE);
        RCC_PLL3Config(                  //
            ClockRatio(960.0 / 12.0),    //
            ClockRatio(960.0 / 24.0),    //
            ClockRatio(960.0 / 27.12),   //
            ClockRatio(960.0 / 48.0),    //
            ClockRatio(960.0 / 88.4736), //
            ClockRatio(960.0 / 12.288)   //
        );
        RCC_PLL3Cmd(ENABLE);

    @note PLL3_VCO_FREQ (MHz) :
        - Range : [ 500.0, 1000.0 ]
        - Typ.  : { 678.0, 705.12, 732.24, 759.36, 786.48, 813.6, 840.72, 867.84, 960.0 }

    @note PLL3_SRC_FREQ (MHz) : 12.0

    @note PLL3_EXT_FREQ (MHz) :
        - PLL3_EXT_FREQ < (PLL3_VCO_FREQ / 8)
        - Typ. : { 6.0, 12.0, 24.0, 27.0 }

    @note PLL3_NFC_FREQ (MHz) : 27.12

    @note PLL3_REF_FREQ (MHz) : 48.0

    @note PLL3_UART_FREQ (MHz) : 88.4736

    @note PLL3_I2S_FREQ (MHz) :
        - PLL3_I2S_FREQ < (PLL3_VCO_FREQ / 8)
        - Typ. : { 12.0, 12.288 }

    @param multiple: specifies the multiple for PLL 3 PLL 3 Multiplier Loop
        This parameter should be ClockRatio(PLL3_VCO_FREQ / PLL3_SRC_FREQ)

    @param e: specifies the divisor for PLL 3 Divider E(xternal Reference Clock)
        This parameter should be ClockRatio(PLL3_VCO_FREQ / PLL3_EXT_FREQ)

    @param n: specifies the divisor for PLL 3 Divider N(FC)
              This parameter should be ClockRatio(PLL3_VCO_FREQ / PLL3_NFC_FREQ)

    @param r: specifies the divisor for PLL 3 Divider R(eference) Clock (48M)
              This parameter should be ClockRatio(PLL3_VCO_FREQ / PLL3_REF_FREQ)

    @param u: specifies the divisor for PLL 3 Divider U(ART) Clock (88.4736M)
              This parameter should be ClockRatio(PLL3_VCO_FREQ / PLL3_UART_FREQ)

    @param i: specifies the divisor for PLL 3 Divider I(2S)
              This parameter should be ClockRatio(PLL3_VCO_FREQ / PLL3_I2S_FREQ)

    @retval None
*/
void RCC_PLL3Config(uint32_t multiple, uint32_t e, uint32_t n, uint32_t r, uint32_t u, uint32_t i)
{
    ClockMultiply(ClockNodePLL3, multiple);
    ClockDivide(ClockNodePLL3E, e);
    ClockDivide(ClockNodePLL3N, n);
    ClockDivide(ClockNodePLL3R, r);
    ClockDivide(ClockNodePLL3U, u);
    ClockDivide(ClockNodePLL3I, i);
}

/*!
    @brief  Enables or disables the PLL3.
    @param  NewState: new state of the PLL2. This parameter can be: ENABLE or DISABLE.
    @retval None
*/
void RCC_PLL3Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    ClockEnable(ClockNodePLL3G, NewState);
}

/**
 * @brief  Enables or disables the Clock Security System.
 * @note   If a failure is detected on the HSE oscillator clock, this oscillator
 *         is automatically disabled and an interrupt is generated to inform the
 *         software about the failure (Clock Security System Interrupt, CSSI),
 *         allowing the MCU to perform rescue operations. The CSSI is linked to
 *         the Cortex-M4 NMI (Non-Maskable Interrupt) exception vector.
 * @param  NewState: new state of the Clock Security System.
 *         This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->CR |= RCC_CR_CSSON;
    }
    else
    {
        RCC->CR &= ~RCC_CR_CSSON;
    }
}

/**
 * @brief  Selects the clock source to output on MCO1 pin(PA8).
 * @note   PA8 should be configured in alternate function mode.
 * @param  RCC_MCO1Source: specifies the clock source to output.
 *          This parameter can be one of the following values:
 *            @arg RCC_MCO1Source_HSI: HSI clock selected as MCO1 source
 *            @arg RCC_MCO1Source_LSE: LSE clock selected as MCO1 source
 *            @arg RCC_MCO1Source_HSE: HSE clock selected as MCO1 source
 *            @arg RCC_MCO1Source_PLLCLK: main PLL clock selected as MCO1 source
 * @param  RCC_MCO1Div: specifies the MCO1 prescaler.
 *          This parameter can be one of the following values:
 *            @arg RCC_MCO1Div_1: no division applied to MCO1 clock
 *            @arg RCC_MCO1Div_2: division by 2 applied to MCO1 clock
 *            @arg RCC_MCO1Div_3: division by 3 applied to MCO1 clock
 *            @arg RCC_MCO1Div_4: division by 4 applied to MCO1 clock
 *            @arg RCC_MCO1Div_5: division by 5 applied to MCO1 clock
 * @retval None
 */
void RCC_MCO1Config(uint32_t RCC_MCO1Source, uint32_t RCC_MCO1Div)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_MCO1SOURCE(RCC_MCO1Source));
    assert_param(IS_RCC_MCO1DIV(RCC_MCO1Div));

    tmpreg = RCC->CFGR;

    /* Clear MCO1 and MCO1PRE bits */
    tmpreg &= ~(RCC_CFGR_MCO1 | RCC_CFGR_MCO1PRE);

    /* Select MCO1 clock source and prescaler */
    tmpreg |= RCC_MCO1Source | RCC_MCO1Div;

    /* Store the new value */
    RCC->CFGR = tmpreg;
}

/**
 * @brief  Selects the clock source to output on MCO2 pin(PC9).
 * @note   PC9 should be configured in alternate function mode.
 * @param  RCC_MCO2Source: specifies the clock source to output.
 *          This parameter can be one of the following values:
 *            @arg RCC_MCO2Source_SYSCLK: System clock (SYSCLK) selected as MCO2 source
 *            @arg RCC_MCO2SOURCE_I2SCLK: I2SCLK clock selected as MCO2 source
 *            @arg RCC_MCO2Source_HSE: HSE clock selected as MCO2 source
 *            @arg RCC_MCO2Source_PLLCLK: main PLL clock selected as MCO2 source
 * @param  RCC_MCO2Div: specifies the MCO2 prescaler.
 *          This parameter can be one of the following values:
 *            @arg RCC_MCO2Div_1: no division applied to MCO2 clock
 *            @arg RCC_MCO2Div_2: division by 2 applied to MCO2 clock
 *            @arg RCC_MCO2Div_3: division by 3 applied to MCO2 clock
 *            @arg RCC_MCO2Div_4: division by 4 applied to MCO2 clock
 *            @arg RCC_MCO2Div_5: division by 5 applied to MCO2 clock
 * @retval None
 */
void RCC_MCO2Config(uint32_t RCC_MCO2Source, uint32_t RCC_MCO2Div)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_MCO2SOURCE(RCC_MCO2Source));
    assert_param(IS_RCC_MCO2DIV(RCC_MCO2Div));

    tmpreg = RCC->CFGR;

    /* Clear MCO2 and MCO2PRE bits */
    tmpreg &= ~(RCC_CFGR_MCO2 | RCC_CFGR_MCO2PRE);

    /* Select MCO2 clock source and prescaler */
    tmpreg |= RCC_MCO2Source | RCC_MCO2Div;

    /* Store the new value */
    RCC->CFGR = tmpreg;
}

/**
 * @brief  Configures the system clock (SYSCLK).
 * @note   The HSI is used (enabled by hardware) as system clock source after
 *         startup from Reset, wake-up from STOP and STANDBY mode, or in case
 *         of failure of the HSE used directly or indirectly as system clock
 *         (if the Clock Security System CSS is enabled).
 * @note   A switch from one clock source to another occurs only if the target
 *         clock source is ready (clock stable after startup delay or PLL locked).
 *         If a clock source which is not yet ready is selected, the switch will
 *         occur when the clock source will be ready.
 *         You can use RCC_GetSYSCLKSource() function to know which clock is
 *         currently used as system clock source.
 * @param  RCC_SYSCLKSource: specifies the clock source used as system clock.
 *          This parameter can be one of the following values:
 *            @arg RCC_SYSCLKSource_HSI: HSI selected as system clock source
 *            @arg RCC_SYSCLKSource_HSE: HSE selected as system clock source
 *            @arg RCC_SYSCLKSource_PLL1P: PLL P selected as system clock source
 * @retval None
 */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));

    ClockSelect(ClockNodeSYSS, (ClockNodeEnum)RCC_SYSCLKSource);
}

/**
 * @brief  Returns the clock source used as system clock.
 * @param  None
 * @retval The clock source used as system clock. The returned value can be one
 *         of the following:
 *              - ClockNodeICS: HSI used as system clock
 *              - ClockNodeECS: HSE used as system clock
 *              - ClockNodePLL1P: PLL P used as system clock
 *              - ClockNodePLL1R: PLL R used as system clock
 */
uint8_t RCC_GetSYSCLKSource(void)
{
    return ClockGetSelection(ClockNodeSYSS);
}

/**
 * @brief  Configures the AHB clock (HCLK).
 * @note   Depending on the device voltage range, the software has to set correctly
 *         these bits to ensure that HCLK not exceed the maximum allowed frequency
 *         (for more details refer to section above
 *           "CPU, AHB and APB busses clocks configuration functions")
 * @param  RCC_SYSCLK: defines the AHB clock divider. This clock is derived from
 *         the system clock (SYSCLK).
 *          This parameter can be one of the following values:
 *            @arg RCC_SYSCLK_Div1: AHB clock = SYSCLK
 *            @arg RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
 *            @arg RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
 *            @arg RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
 *            @arg RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
 *            @arg RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
 *            @arg RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
 *            @arg RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
 *            @arg RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
 * @retval None
 */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
    /* Check the parameters */
    assert_param(IS_RCC_HCLK(RCC_SYSCLK));

    ClockDivide(ClockNodeAHB1, RCC_SYSCLK);
}

/**
 * @brief  Configures the Low Speed APB clock (PCLK1).
 * @param  RCC_HCLK: defines the APB1 clock divider. This clock is derived from
 *         the AHB clock (HCLK).
 *          This parameter can be one of the following values:
 *            @arg RCC_HCLK_Div1:  APB1 clock = HCLK
 *            @arg RCC_HCLK_Div2:  APB1 clock = HCLK/2
 *            @arg RCC_HCLK_Div4:  APB1 clock = HCLK/4
 *            @arg RCC_HCLK_Div8:  APB1 clock = HCLK/8
 *            @arg RCC_HCLK_Div16: APB1 clock = HCLK/16
 * @retval None
 */
void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
    /* Check the parameters */
    assert_param(IS_RCC_PCLK(RCC_HCLK));

    ClockDivide(ClockNodeAPB1, RCC_HCLK);
}

/**
 * @brief  Configures the High Speed APB clock (PCLK2).
 * @param  RCC_HCLK: defines the APB2 clock divider. This clock is derived from
 *         the AHB clock (HCLK).
 *          This parameter can be one of the following values:
 *            @arg RCC_HCLK_Div1:  APB2 clock = HCLK
 *            @arg RCC_HCLK_Div2:  APB2 clock = HCLK/2
 *            @arg RCC_HCLK_Div4:  APB2 clock = HCLK/4
 *            @arg RCC_HCLK_Div8:  APB2 clock = HCLK/8
 *            @arg RCC_HCLK_Div16: APB2 clock = HCLK/16
 * @retval None
 */
void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
    /* Check the parameters */
    assert_param(IS_RCC_PCLK(RCC_HCLK));

    ClockDivide(ClockNodeAPB2, RCC_HCLK);
}

/**
 * @brief  Returns the frequencies of different on chip clocks; SYSCLK, HCLK,
 *         PCLK1 and PCLK2.
 *
 * @note   The system frequency computed by this function is not the real
 *         frequency in the chip. It is calculated based on the predefined
 *         constant and the selected clock source:
 * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
 * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
 * @note     If SYSCLK source is PLL, function returns values based on HSE_VALUE(**)
 *           or HSI_VALUE(*) multiplied/divided by the PLL factors.
 * @note     (*) HSI_VALUE is a constant defined in system_mh2457.h file (default value
 *               16 MHz) but the real value may vary depending on the variations
 *               in voltage and temperature.
 * @note     (**) HSE_VALUE is a constant defined in system_mh2457.h file (default value
 *                25 MHz), user has to ensure that HSE_VALUE is same as the real
 *                frequency of the crystal used. Otherwise, this function may
 *                have wrong result.
 *
 * @note   The result of this function could be not correct when using fractional
 *         value for HSE crystal.
 *
 * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
 *          the clocks frequencies.
 *
 * @note   This function can be used by the user application to compute the
 *         baudrate for the communication peripherals or configure other parameters.
 * @note   Each time SYSCLK, HCLK, PCLK1 and/or PCLK2 clock changes, this function
 *         must be called to update the structure's field. Otherwise, any
 *         configuration based on this function will be incorrect.
 *
 * @retval None
 */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
    /* SYS clock frequency */
    RCC_Clocks->SYSCLK_Frequency = ClockGet(ClockNodeSYS);

    /* HCLK clock frequency */
    RCC_Clocks->HCLK_Frequency = ClockGet(ClockNodeAHB1);

    /* PCLK1 clock frequency */
    RCC_Clocks->PCLK1_Frequency = ClockGet(ClockNodeAPB1);

    /* PCLK2 clock frequency */
    RCC_Clocks->PCLK2_Frequency = ClockGet(ClockNodeAPB2);
}

/**
 * @brief  Configures the RTC clock (RTCCLK).
 * @note   As the RTC clock configuration bits are in the Backup domain and write
 *         access is denied to this domain after reset, you have to enable write
 *         access using PWR_BackupAccessCmd(ENABLE) function before to configure
 *         the RTC clock source (to be done once after reset).
 * @note   Once the RTC clock is configured it can't be changed unless the
 *         Backup domain is reset using RCC_BackupResetCmd() function, or by
 *         a Power On Reset (POR).
 *
 * @param  RCC_RTCCLKSource: specifies the RTC clock source.
 *          This parameter can be one of the following values:
 *            @arg RCC_RTCCLKSource_LSE: LSE selected as RTC clock
 *            @arg RCC_RTCCLKSource_LSI: LSI selected as RTC clock
 *            @arg RCC_RTCCLKSource_HSE_Divx: HSE clock divided by x selected
 *                                            as RTC clock, where x:[2,31]
 *
 * @note   If the LSE or LSI is used as RTC clock source, the RTC continues to
 *         work in STOP and STANDBY modes, and can be used as wakeup source.
 *         However, when the HSE clock is used as RTC clock source, the RTC
 *         cannot be used in STOP and STANDBY modes.
 * @note   The maximum input clock frequency for RTC is 1MHz (when using HSE as
 *         RTC clock source).
 *
 * @retval None
 */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));

    if ((RCC_RTCCLKSource & 0x00000300) == 0x00000300)
    { /* If HSE is selected as RTC clock source, configure HSE division factor for RTC clock */
        tmpreg = RCC->CFGR;

        /* Clear RTCPRE[4:0] bits */
        tmpreg &= ~RCC_CFGR_RTCPRE;

        /* Configure HSE division factor for RTC clock */
        tmpreg |= (RCC_RTCCLKSource & 0xFFFFCFF);

        /* Store the new value */
        RCC->CFGR = tmpreg;
    }

    /* Select the RTC clock source */
    RCC->BDCR = (RCC->BDCR & ~(3 << 8)) | (RCC_RTCCLKSource & 0x00000FFF);
}

/**
 * @brief  Forces or releases the Backup domain reset.
 * @note   This function resets the RTC peripheral (including the backup registers)
 *         and the RTC clock source selection in RCC_CSR register.
 * @note   The BKPSRAM is not affected by this reset.
 * @param  NewState: new state of the Backup domain reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_BackupResetCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->BDCR |= RCC_BDCR_BDRST;
    }
    else
    {
        RCC->BDCR &= ~RCC_BDCR_BDRST;
    }
}

/**
 * @brief  Configures the I2S clock source (I2SCLK).
 * @note   This function must be called before enabling the I2S APB clock.
 * @param  RCC_I2SCLKSource: specifies the I2S clock source.
 *          This parameter can be one of the following values:
 *            @arg RCC_I2S2CLKSource_PLL3: PLL3 clock used as I2S clock source
 *            @arg RCC_I2S2CLKSource_Ext: External clock mapped on the I2S_CKIN pin
 *                                        used as I2S clock source
 * @retval None
 */
void RCC_I2SCLKConfig(uint32_t RCC_I2SCLKSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_I2SCLK_SOURCE(RCC_I2SCLKSource));

    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_I2SSRC)) | RCC_I2SCLKSource;
}

/**
 * @brief  Enables or disables the AHB1 peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 * @param  RCC_AHB1Periph: specifies the AHB1 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB1Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB1_CLOCK_PERIPH(RCC_AHB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB1ENR |= RCC_AHB1Periph;
    }
    else
    {
        RCC->AHB1ENR &= ~RCC_AHB1Periph;
    }
}

/**
 * @brief  Enables or disables the AHB2 peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 * @param  RCC_AHB2Periph: specifies the AHB2 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB2Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB2_CLOCK_PERIPH(RCC_AHB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB2ENR |= RCC_AHB2Periph;
    }
    else
    {
        RCC->AHB2ENR &= ~RCC_AHB2Periph;
    }
}

/**
 * @brief  Enables or disables the AHB3 peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 * @param  RCC_AHB3Periph: specifies the AHB3 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB3Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB3_CLOCK_PERIPH(RCC_AHB3Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB3ENR |= RCC_AHB3Periph;
    }
    else
    {
        RCC->AHB3ENR &= ~RCC_AHB3Periph;
    }
}

/**
 * @brief  Enables or disables the AHB4 peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 * @param  RCC_AHB4Periph: specifies the AHB4 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB4Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB4PeriphClockCmd(uint32_t RCC_AHB4Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB4_CLOCK_PERIPH(RCC_AHB4Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB4ENR |= RCC_AHB4Periph;
    }
    else
    {
        RCC->AHB4ENR &= ~RCC_AHB4Periph;
    }
}

/**
 * @brief  Enables or disables the Low Speed APB (APB1) peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 * @param  RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_APB1Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->APB1ENR |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1ENR &= ~RCC_APB1Periph;
    }
}

/**
 * @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
 * @note   After reset, the peripheral clock (used for registers read/write access)
 *         is disabled and the application software has to enable this clock before
 *         using it.
 * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_APB2Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->APB2ENR |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2ENR &= ~RCC_APB2Periph;
    }
}

/**
 * @brief  Forces or releases AHB1 peripheral reset.
 * @param  RCC_AHB1Periph: specifies the AHB1 peripheral to reset.
 *          This parameter can be any combination of RCC_AHB1Periph_*.
 * @param  NewState: new state of the specified peripheral reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB1_RESET_PERIPH(RCC_AHB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB1RSTR |= RCC_AHB1Periph;
    }
    else
    {
        RCC->AHB1RSTR &= ~RCC_AHB1Periph;
    }
}

/**
 * @brief  Forces or releases AHB2 peripheral reset.
 * @param  RCC_AHB2Periph: specifies the AHB2 peripheral to reset.
 *          This parameter can be any combination of RCC_AHB2Periph_*.
 * @param  NewState: new state of the specified peripheral reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB2_RESET_PERIPH(RCC_AHB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB2RSTR |= RCC_AHB2Periph;
    }
    else
    {
        RCC->AHB2RSTR &= ~RCC_AHB2Periph;
    }
}

/**
 * @brief  Forces or releases AHB3 peripheral reset.
 * @param  RCC_AHB3Periph: specifies the AHB3 peripheral to reset.
 *          This parameter can be any combination of RCC_AHB3Periph_*.
 * @param  NewState: new state of the specified peripheral reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB3_RESET_PERIPH(RCC_AHB3Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB3RSTR |= RCC_AHB3Periph;
    }
    else
    {
        RCC->AHB3RSTR &= ~RCC_AHB3Periph;
    }
}

/**
 * @brief  Forces or releases AHB4 peripheral reset.
 * @param  RCC_AHB4Periph: specifies the AHB4 peripheral to reset.
 *          This parameter can be any combination of RCC_AHB4Periph_*.
 * @param  NewState: new state of the specified peripheral reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB4PeriphResetCmd(uint32_t RCC_AHB4Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB4_RESET_PERIPH(RCC_AHB4Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHB4RSTR |= RCC_AHB4Periph;
    }
    else
    {
        RCC->AHB4RSTR &= ~RCC_AHB4Periph;
    }
}

/**
 * @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
 * @param  RCC_APB1Periph: specifies the APB1 peripheral to reset.
 *          This parameter can be any combination of RCC_APB1Periph_*.
 * @param  NewState: new state of the specified peripheral reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->APB1RSTR |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1RSTR &= ~RCC_APB1Periph;
    }
}

/**
 * @brief  Forces or releases High Speed APB (APB2) peripheral reset.
 * @param  RCC_APB2Periph: specifies the APB2 peripheral to reset.
 *          This parameter can be any combination of RCC_APB2Periph_*.
 * @param  NewState: new state of the specified peripheral reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB2_RESET_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->APB2RSTR |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2RSTR &= ~RCC_APB2Periph;
    }
}

/**
 * @brief  Enables or disables the AHB1 peripheral clock during Low Power (Sleep) mode.
 * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
 *         power consumption.
 * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
 * @note   By default, all peripheral clocks are enabled during SLEEP mode.
 * @param  RCC_AHB1Periph: specifies the AHB1 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB1Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB1PeriphClockLPModeCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB1_LPMODE_PERIPH(RCC_AHB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->AHB1LPENR |= RCC_AHB1Periph;
    }
    else
    {
        RCC->AHB1LPENR &= ~RCC_AHB1Periph;
    }
}

/**
 * @brief  Enables or disables the AHB2 peripheral clock during Low Power (Sleep) mode.
 * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
 *           power consumption.
 * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
 * @note   By default, all peripheral clocks are enabled during SLEEP mode.
 * @param  RCC_AHB2Periph: specifies the AHB2 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB2Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB2PeriphClockLPModeCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB2_LPMODE_PERIPH(RCC_AHB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->AHB2LPENR |= RCC_AHB2Periph;
    }
    else
    {
        RCC->AHB2LPENR &= ~RCC_AHB2Periph;
    }
}

/**
 * @brief  Enables or disables the AHB3 peripheral clock during Low Power (Sleep) mode.
 * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
 *         power consumption.
 * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
 * @note   By default, all peripheral clocks are enabled during SLEEP mode.
 * @param  RCC_AHB3Periph: specifies the AHB3 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB3Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB3PeriphClockLPModeCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB3_LPMODE_PERIPH(RCC_AHB3Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->AHB3LPENR |= RCC_AHB3Periph;
    }
    else
    {
        RCC->AHB3LPENR &= ~RCC_AHB3Periph;
    }
}

/**
 * @brief  Enables or disables the AHB4 peripheral clock during Low Power (Sleep) mode.
 * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
 *         power consumption.
 * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
 * @note   By default, all peripheral clocks are enabled during SLEEP mode.
 * @param  RCC_AHB4Periph: specifies the AHB4 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_AHB4Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_AHB4PeriphClockLPModeCmd(uint32_t RCC_AHB4Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB4_LPMODE_PERIPH(RCC_AHB4Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->AHB4LPENR |= RCC_AHB4Periph;
    }
    else
    {
        RCC->AHB4LPENR &= ~RCC_AHB4Periph;
    }
}

/**
 * @brief  Enables or disables the APB1 peripheral clock during Low Power (Sleep) mode.
 * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
 *         power consumption.
 * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
 * @note   By default, all peripheral clocks are enabled during SLEEP mode.
 * @param  RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_APB1Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_APB1PeriphClockLPModeCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->APB1LPENR |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1LPENR &= ~RCC_APB1Periph;
    }
}

/**
 * @brief  Enables or disables the APB2 peripheral clock during Low Power (Sleep) mode.
 * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
 *         power consumption.
 * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
 * @note   By default, all peripheral clocks are enabled during SLEEP mode.
 * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
 *          This parameter can be any combination of RCC_APB2Periph_*.
 * @param  NewState: new state of the specified peripheral clock.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_APB2PeriphClockLPModeCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        RCC->APB2LPENR |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2LPENR &= ~RCC_APB2Periph;
    }
}

/**
 * @brief  Checks whether the specified RCC flag is set or not.
 * @param  RCC_FLAG: specifies the flag to check.
 *          This parameter can be one of the following values:
 *            @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
 *            @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
 *            @arg RCC_FLAG_PLL1RDY: main PLL clock ready
 *            @arg RCC_FLAG_PLL2RDY: PLL2 clock ready
 *            @arg RCC_FLAG_PLL3RDY: PLL3 clock ready
 *            @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
 *            @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
 *            @arg RCC_FLAG_PINRST: Pin reset
 *            @arg RCC_FLAG_PORRST: POR/PDR reset
 *            @arg RCC_FLAG_SFTRST: Software reset
 *            @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
 *            @arg RCC_FLAG_WWDGRST: Window Watchdog reset
 *            @arg RCC_FLAG_LPWRRST: Low Power reset
 *            @arg RCC_FLAG_GLBRST: Global reset
 *            @arg RCC_FLAG_BATTRST: Battery domain attack reset
 *            @arg RCC_FLAG_MATTRST: Main LDO domain attack reset
 * @retval The new state of RCC_FLAG (SET or RESET).
 */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
    uint8_t    FLAG_MASK = 0x1F;
    uint32_t   tmp       = 0;
    uint32_t   statusreg = 0;
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_RCC_FLAG(RCC_FLAG));

    /* Get the RCC register index */
    tmp = RCC_FLAG >> 5;
    if (tmp == 1) /* The flag to check is in CR register */
    {
        statusreg = RCC->CR;
    }
    else if (tmp == 2) /* The flag to check is in BDCR register */
    {
        statusreg = RCC->BDCR;
    }
    else /* The flag to check is in CSR register */
    {
        statusreg = RCC->CSR;
    }

    /* Get the flag position */
    tmp = RCC_FLAG & FLAG_MASK;
    if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    /* Return the flag status */
    return bitstatus;
}

/**
 * @brief  Clears the RCC reset flags.
 *         The reset flags are: RCC_FLAG_PINRST, RCC_FLAG_PORRST,  RCC_FLAG_SFTRST,
 *         RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST,
 * @param  None
 * @retval None
 */
void RCC_ClearFlag(void)
{
    /* Set RMVF bit to clear the reset flags */
    RCC->CSR |= RCC_CSR_RMVF;
}

/**
 * @brief  Enables or disables the specified RCC interrupts.
 * @param  RCC_IT: specifies the RCC interrupt sources to be enabled or disabled.
 *          This parameter can be any combination of the following values:
 *            @arg RCC_IT_LSI: LSI ready interrupt
 *            @arg RCC_IT_LSE: LSE ready interrupt
 *            @arg RCC_IT_HSI: HSI ready interrupt
 *            @arg RCC_IT_HSE: HSE ready interrupt
 *            @arg RCC_IT_PLL1: main PLL ready interrupt
 *            @arg RCC_IT_PLL2: PLLI2S ready interrupt
 * @param  NewState: new state of the specified RCC interrupts.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_IT(RCC_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->CIR |= (RCC_IT << RCC_IE_OFFEST);
    }
    else
    {
        RCC->CIR &= ~(RCC_IT << RCC_IE_OFFEST);
    }
}

/**
 * @brief  Checks whether the specified RCC interrupt has occurred or not.
 * @param  RCC_IT: specifies the RCC interrupt source to check.
 *          This parameter can be one of the following values:
 *            @arg RCC_IT_LSIRDY: LSI ready interrupt
 *            @arg RCC_IT_LSERDY: LSE ready interrupt
 *            @arg RCC_IT_HSIRDY: HSI ready interrupt
 *            @arg RCC_IT_HSERDY: HSE ready interrupt
 *            @arg RCC_IT_PLL1RDY: main PLL ready interrupt
 *            @arg RCC_IT_PLL2RDY: PLL2 ready interrupt
 *            @arg RCC_IT_CSS: Clock Security System interrupt
 * @retval The new state of RCC_IT (SET or RESET).
 */
ITStatus RCC_GetITStatus(uint8_t RCC_IT)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_RCC_GET_IT(RCC_IT));

    /* Check the status of the specified RCC interrupt */
    if ((RCC->CIR & RCC_IT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    /* Return the RCC_IT status */
    return bitstatus;
}

/**
 * @brief  Clears the RCC's interrupt pending bits.
 * @param  RCC_IT: specifies the interrupt pending bit to clear.
 *          This parameter can be any combination of the following values:
 *            @arg RCC_IT_LSIRDY: LSI ready interrupt
 *            @arg RCC_IT_LSERDY: LSE ready interrupt
 *            @arg RCC_IT_HSIRDY: HSI ready interrupt
 *            @arg RCC_IT_HSERDY: HSE ready interrupt
 *            @arg RCC_IT_PLL1RDY: main PLL ready interrupt
 *            @arg RCC_IT_PLL2RDY: PLL2 ready interrupt
 *            @arg RCC_IT_CSS: Clock Security System interrupt
 * @retval None
 */
void RCC_ClearITPendingBit(uint8_t RCC_IT)
{
    /* Check the parameters */
    assert_param(IS_RCC_CLEAR_IT(RCC_IT));

    RCC->CIR |= (RCC_IT << RCC_RDYC_OFFEST);
}

/**
 * @brief  Selects the GPIO pin used as EXTI Line.
 * @param  EXTI_PortSourceGPIOx : selects the GPIO port to be used as source for
 *          EXTI lines where x can be (A..I)
 * @param  EXTI_PinSourcex: specifies the EXTI line to be configured.
 *           This parameter can be EXTI_PinSourcex where x can be (0..15)
 * @retval None
 */
void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
{
    uint32_t tmp = 0x00;

    /* Check the parameters */
    assert_param(IS_EXTI_PORT_SOURCE(EXTI_PortSourceGPIOx));
    assert_param(IS_EXTI_PIN_SOURCE(EXTI_PinSourcex));

    tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03));
    SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] &= ~tmp;
    SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
}

/**
 * @brief  Deinit SYSCFG
 * @param  None
 * @retval None
 */
void SYSCFG_DeInit(void)
{
    PeripheralReset(PeripheralSYSCFG);
}

/**
 * @brief  Changes the mapping of the specified pin.
 * @param  SYSCFG_Memory: selects the memory remapping.
 *         This parameter can be one of the following values:
 *            @arg SYSCFG_MemoryRemap_SRAM4_5: SRAM4&5 remap to 0x08000000
 *            @arg SYSCFG_MemoryRemap_SRAM5: SRAM5 remap to 0x08000000
 *            @arg SYSCFG_MemoryRemap_SDRAM: SDRAM remap to 0x10000000
 * @retval None
 */
void SYSCFG_MemoryRemapConfig(uint32_t SYSCFG_MemoryRemap)
{
    /* Check the parameters */
    assert_param(IS_SYSCFG_MEMORY_REMAP_CONFING(SYSCFG_MemoryRemap));

    SYSCFG->MEMRMP = SYSCFG_MemoryRemap;
}

/**
  * @brief  Get Chip SN
  * @param  ChipSN：16 Byte chip sn
  * @retval None
  */
void SYSCTRL_GetChipSN(uint8_t *ChipSN)
{
    PeripheralEnable(PeripheralOTP, true);
	memcpy(ChipSN, (uint8_t *)(SYSCTRL_CHIP_SN_ADDR), SYSCTRL_CHIP_SN_LEN);
}

/**
  * @brief  Enter USB Download Mode
  * @param  None
  * @retval None
  */
void PCFG_EnterUSBDownload(void)
{
    PeripheralEnable(PeripheralPCFG, true);
    PCFG->USER_RSVD0 = PCFG_USB_DOWNLOAD_FLAG;
    PeripheralReset(PeripheralSYS);
}
