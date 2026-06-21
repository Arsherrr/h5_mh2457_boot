// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0
#ifndef SYSTEM_MH2457_H
#define SYSTEM_MH2457_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CONFIG_I2SE_VALUE
#define CONFIG_I2SE_VALUE (12000000U)
#endif

#ifndef HSE_VALUE
#define HSE_VALUE (12000000U) /*!< Value of the External oscillator in Hz */
#endif

#ifndef HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT ((uint16_t)0x05000) /*!< Time out for HSE start up */
#endif

#ifndef HSI_VALUE
#define HSI_VALUE (12000000U) /*!< Value of the Internal oscillator in Hz*/
#endif

// assert macro
#ifndef assert_param
#ifdef USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t*)__FILE__, __LINE__))
extern void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */
#endif

// Exception / Interrupt Handler Function Prototype
typedef void (*VECTOR_TABLE_Type)(void);

// System Clock Frequency (Core Clock)
extern uint32_t SystemCoreClock;

// Setup the microcontroller system.
extern void SystemInit(void);

// Config System Clock
extern void SystemClockConfig(bool useECS, uint8_t m, uint16_t n, uint8_t p, uint8_t q, uint8_t r, uint8_t ahb, uint8_t apb);

// Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
extern void SystemCoreClockUpdate(void);

// System microsecond delay (weak) function
extern void SystemDelayUs(volatile uint32_t us);

// System millisecond delay (weak) function
extern void SystemDelay(volatile uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_MH2457_H */
