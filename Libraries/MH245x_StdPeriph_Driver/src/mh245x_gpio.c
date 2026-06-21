// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_gpio.h"

/**
 * @brief  De-initializes the GPIOx peripheral registers to their default reset values.
 * @note   By default, The GPIO pins are configured in input floating mode (except JTAG pins).
 * @param  GPIOx: where x can be A, B, C ...
 * @retval None
 */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    PeripheralReset(PeripheralFind(GPIOx, PeripheralGPIOStart, PeripheralGPIOEnd));
}

/**
 * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains
 *         the configuration information for the specified GPIO peripheral.
 * @retval None
 */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_PUPD(GPIO_InitStruct->GPIO_PuPd));

    /* ------------------------- Configure the port pins ---------------- */
    /*-- GPIO Mode Configuration --*/
    for (pinpos = 0x00; pinpos < 0x10; pinpos++)
    {
        pos = ((uint32_t)0x01) << pinpos;
        /* Get the port pins position */
        currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

        if (currentpin == pos)
        {
            GPIOx->MODER = (GPIOx->MODER & ~(GPIO_MODER_MODER0 << (pinpos * 2))) | (((uint32_t)GPIO_InitStruct->GPIO_Mode) << (pinpos * 2));

            if ((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF))
            {
                /* Check Speed mode parameters */
                assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));

                /* Speed mode configuration */
                GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2))) | ((uint32_t)(GPIO_InitStruct->GPIO_Speed) << (pinpos * 2));

                /* Check Output mode parameters */
                assert_param(IS_GPIO_OTYPE(GPIO_InitStruct->GPIO_OType));

                /* Output mode configuration*/
                GPIOx->OTYPER = (GPIOx->OTYPER & ~((GPIO_OTYPER_OT_0) << (pinpos))) | (((uint16_t)GPIO_InitStruct->GPIO_OType) << (pinpos));
            }

            /* Pull-up Pull down resistor configuration*/
            GPIOx->PUPDR = (GPIOx->PUPDR & ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2))) | (((uint32_t)GPIO_InitStruct->GPIO_PuPd) << (pinpos * 2));
        }
    }
}

/**
 * @brief  Fills each GPIO_InitStruct member with its default value.
 * @param  GPIO_InitStruct : pointer to a GPIO_InitTypeDef structure which will be initialized.
 * @retval None
 */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->GPIO_Pin   = GPIO_Pin_All;
    GPIO_InitStruct->GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct->GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct->GPIO_PuPd  = GPIO_PuPd_NOPULL;
}

/**
 * @brief  Locks GPIO Pins configuration registers.
 * @note   The locked registers are GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR,
 *         GPIOx_PUPDR, GPIOx_AFRL and GPIOx_AFRH.
 * @note   The configuration of the locked GPIO pins can no longer be modified
 *         until the next reset.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_Pin: specifies the port bit to be locked.
 *          This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @retval None
 */
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    __IO uint32_t tmp = 0x00010000;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    tmp |= GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Reset LCKK bit */
    GPIOx->LCKR = GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
}

/**
 * @}
 */

/** @defgroup GPIO_Group2 GPIO Read and Write
 *  @brief   GPIO Read and Write
 *
@verbatim
 ===============================================================================
                         ##### GPIO Read and Write #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
 * @brief  Reads the specified input port pin.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_Pin: specifies the port bit to read.
 *         This parameter can be GPIO_Pin_x where x can be (0..15).
 * @retval The input port pin value.
 */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
 * @brief  Reads the specified GPIO input data port.
 * @param  GPIOx: where x can be A, B, C ...
 * @retval GPIO input data port value.
 */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->IDR);
}

/**
 * @brief  Reads the specified output data port bit.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_Pin: specifies the port bit to read.
 *          This parameter can be GPIO_Pin_x where x can be (0..15).
 * @retval The output port pin value.
 */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if (((GPIOx->ODR) & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
 * @brief  Reads the specified GPIO output data port.
 * @param  GPIOx: where x can be A, B, C ...
 * @retval GPIO output data port value.
 */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->ODR);
}

/**
 * @brief  Sets the selected data port bits.
 * @note   This functions uses GPIOx_BSRR register to allow atomic read/modify
 *         accesses. In this way, there is no risk of an IRQ occurring between
 *         the read and the modify access.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_Pin: specifies the port bits to be written.
 *          This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @retval None
 */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->BSRR = GPIO_Pin;
}

/**
 * @brief  Clears the selected data port bits.
 * @note   This functions uses GPIOx_BSRR register to allow atomic read/modify
 *         accesses. In this way, there is no risk of an IRQ occurring between
 *         the read and the modify access.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_Pin: specifies the port bits to be written.
 *          This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @retval None
 */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16;
}

/**
 * @brief  Sets or clears the selected data port bit.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_Pin: specifies the port bit to be written.
 *          This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * @param  BitVal: specifies the value to be written to the selected bit.
 *          This parameter can be one of the BitAction enum values:
 *            @arg Bit_RESET: to clear the port pin
 *            @arg Bit_SET: to set the port pin
 * @retval None
 */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_ACTION(BitVal));

    if (BitVal != Bit_RESET)
    {
        GPIOx->BSRR = GPIO_Pin;
    }
    else
    {
        GPIOx->BSRR = (uint32_t)GPIO_Pin << 16;
    }
}

/**
 * @brief  Writes data to the specified GPIO data port.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  PortVal: specifies the value to be written to the port output data register.
 * @retval None
 */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->ODR = PortVal;
}

/**
 * @brief  Toggles the specified GPIO pins..
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_Pin: Specifies the pins to be toggled.
 * @retval None
 */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->ODR ^= GPIO_Pin;
}

/**
 * @brief  Changes the mapping of the specified pin.
 * @param  GPIOx: where x can be A, B, C ...
 * @param  GPIO_PinSource: specifies the pin for the Alternate function.
 *         This parameter can be GPIO_PinSourcex where x can be (0..15).
 * @param  GPIO_AFSelection: selects the pin to used as Alternate function.
 *          This parameter can be one of the following values:
 *            @arg GPIO_AF_* : see gpio feature definitions in target header
 * @retval None
 */
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    assert_param(IS_GPIO_AF(GPIO_AF));

    temp = GPIOx->AFR[GPIO_PinSource >> 3U];
    temp &= ~((uint32_t)0xFU << ((uint32_t)(GPIO_PinSource & (uint32_t)0x07U) * 4U));
    temp |= ((uint32_t)GPIO_AF << (((uint32_t)GPIO_PinSource & (uint32_t)0x07U) * 4U));
    GPIOx->AFR[GPIO_PinSource >> 3U] = temp;
}

void GPIO_DriveStrengthConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t GPIO_DriveStrength)
{
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_DriveStrength(GPIO_DriveStrength));

    __IO uint32_t* reg = &GPIOx->DSR;

    for (uint8_t pos = 0; pos < 16; pos++)
    {
        if (GPIO_Pin & BIT(pos))
        {
            *reg = (*reg & ~(((uint32_t)0x03) << (2 * pos))) | (((uint32_t)GPIO_DriveStrength) << (2 * pos));
        }
    }
}

void IOSetup(IOEnum io, IOConfigStruct config)
{
    GPIO_TypeDef* port = IOPort(io);

    uint8_t pos = IOSource(io) << 1;

    PeripheralEnable(IOPeripheral(io), true);

    volatile uint32_t mask = 3 << pos;
    volatile uint32_t value;

    // Mode configuration
    value       = port->MODER & ~mask;
    port->MODER = value | ((config.Mode & 3) << pos);

    if ((config.Mode & 3) == IOModeAlternate || (config.Mode & 3) == IOModeOutput)
    {
        // Speed mode configuration
        value         = port->OSPEEDR & ~mask;
        port->OSPEEDR = value | (config.Speed << pos);

        // Output mode configuration
        value        = port->OTYPER & ~BIT(pos >> 1);
        port->OTYPER = value | ((config.Mode > 3) << (pos >> 1));

        // Drive configuration
        value     = port->DSR & ~mask;
        port->DSR = value | (config.Drive << pos);
    }

    // Pull-up Pull down resistor configuration
    value       = port->PUPDR & ~mask;
    port->PUPDR = value | (config.Pull << pos);

    // Alternate function configuration
    pos <<= 1;
    mask                = 0xF << (pos & 0x1F);
    value               = port->AFR[pos >> 5] & ~mask;
    port->AFR[pos >> 5] = value | (config.Alternate << (pos & 0x1F));
}
