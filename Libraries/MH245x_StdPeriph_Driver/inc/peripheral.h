// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__

#include "mh2457.h"

/* --------  Feature Definition  -------------------------------------------- */

#ifndef FEATURE_FRACTIONAL_PRECISION
#define FEATURE_FRACTIONAL_PRECISION 2000000UL
#endif

/* --------  Public Type  --------------------------------------------------- */

typedef enum
{
    ClockTypeSource,
    ClockTypeGate,
    ClockTypeDivider,
    ClockTypeMultiplier,
    ClockTypeSelector,
    ClockTypeGeneric,
} ClockTypeEnum;

typedef enum
{
    DividerTypeArithmetic,       // Arithmetic Divider: A[n] = a * n + b
    DividerTypeBypassArithmetic, // Bypass Arithmetic Divider: Arithmetic Divider with Bypass Bit
    DividerTypeGeometric,        // Geometric Divider: A[n] = (a ^ n) * b
    DividerTypeBypassGeometric,  // Bypass Geometric Divider: Geometric Divider with Bypass Bit
    DividerTypeFractional,       // Fractional Divider: A[n] = R * (2 ^ A) / FEATURE_FRACTIONAL_PRECISION, F(o) = F(parent) / R * FEATURE_FRACTIONAL_PRECISION;
} DividerTypeEnum;

typedef enum
{
    MultiplierTypeArithmetic, // Arithmetic Multiplier: A[n] = a * n + b
    MultiplierTypeFractional, // Fractional Multiplier: A[n] = R * (2 ^ A) / FEATURE_FRACTIONAL_PRECISION, F(o) = F(parent) / FEATURE_FRACTIONAL_PRECISION * R ;
} MultiplierTypeEnum;

typedef const struct
{
    ClockTypeEnum Type : 4;
    uint8_t       Timeout : 4; // Timeout for Source / Gate / Selector

    union
    {
        ClockNodeEnum Parent : 8;
        uint8_t       Count; // Selection Count for Selector
    };

    uint8_t Offset; // Bit Offset of Register
    uint8_t Width;  // Bit Width of Register

    __IO uint32_t* Register; // Register Address

    union
    {
        bool IsActiveHigh; // Parameters of Gate, Is Gate Bit Active High

        uint32_t Frequency; // Frequency of Clock Source

        struct
        {
            uint8_t A;
            int8_t  B;
            union
            {
                DividerTypeEnum    DividerType : 8;
                MultiplierTypeEnum MultiplierType : 8;
            };
        }; // Parameters of Divider / Multiplier

        const uint8_t Nodes[4]; // Clock Nodes of Selector
    };
} ClockNodeStruct;

extern ClockNodeStruct ClockMap[];

extern PeripheralStruct PeripheralMap[];

/* --------  Public Macro Function  ----------------------------------------- */

#define __ARGC8__(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, N, ...) N

#define __VA_ARGC__(...) __ARGC8__(0, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define ClockMakeRegister(register, offset, width) .Offset = offset, .Width = width, .Register = &register

#define ClockMakeSource(register, offset, width, frequency, timeout) \
    {                                                                \
        .Type      = ClockTypeSource,                                \
        .Frequency = frequency,                                      \
        .Timeout   = timeout,                                        \
        ClockMakeRegister(register, offset, width),                  \
    }

#define ClockMakeGate(parent, register, offset, width, isActiveHigh, timeout) \
    {                                                                         \
        .Type         = ClockTypeGate,                                        \
        .Parent       = parent,                                               \
        .IsActiveHigh = isActiveHigh,                                         \
        .Timeout      = timeout,                                              \
        ClockMakeRegister(register, offset, width),                           \
    }

#define ClockMakeDivider(parent, register, offset, width, dividerType, a, b) \
    {                                                                        \
        .Type        = ClockTypeDivider,                                     \
        .Parent      = parent,                                               \
        .DividerType = dividerType,                                          \
        .A           = a,                                                    \
        .B           = b,                                                    \
        ClockMakeRegister(register, offset, width),                          \
    }

#define ClockMakeMultiplier(parent, register, offset, width, multiplierType, a, b) \
    {                                                                              \
        .Type           = ClockTypeMultiplier,                                     \
        .Parent         = parent,                                                  \
        .MultiplierType = multiplierType,                                          \
        .A              = a,                                                       \
        .B              = b,                                                       \
        ClockMakeRegister(register, offset, width),                                \
    }

#define ClockMakeSelector(register, offset, width, timeout, ...) \
    {                                                            \
        .Type    = ClockTypeSelector,                            \
        .Count   = __VA_ARGC__(__VA_ARGS__),                     \
        .Timeout = timeout,                                      \
        .Nodes   = {__VA_ARGS__},                                \
        ClockMakeRegister(register, offset, width),              \
    }

#define ClockRatio(ratio) ((uint32_t)(ratio * FEATURE_FRACTIONAL_PRECISION))

/* --------  Public Function  ----------------------------------------------- */

// Enable Clock, the type of clock node can be ClockTypeSource / ClockTypeGate
extern bool ClockEnable(ClockNodeEnum clockIndex, bool isEnable);

extern bool ClockDivide(ClockNodeEnum clockIndex, uint32_t divisor);

extern bool ClockMultiply(ClockNodeEnum clockIndex, uint32_t multiple);

extern bool ClockSelect(ClockNodeEnum clockIndex, ClockNodeEnum selection);

extern uint32_t ClockGetDivisor(ClockNodeEnum clockIndex);

extern uint32_t ClockGetMultiple(ClockNodeEnum clockIndex);

extern ClockNodeEnum ClockGetSelection(ClockNodeEnum clockIndex);

extern uint32_t ClockGet(ClockNodeEnum clockIndex);

extern PeripheralEnum PeripheralFind(void* peripheralBase, PeripheralEnum start, PeripheralEnum end);

extern void PeripheralEnable(PeripheralEnum peripheralIndex, bool isEnable);

extern void PeripheralResetState(PeripheralEnum peripheralIndex, bool isEnable);

extern void PeripheralReset(PeripheralEnum peripheralIndex);

#endif // __PERIPHERAL_H__
