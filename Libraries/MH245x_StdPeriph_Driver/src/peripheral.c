#include "peripheral.h"

bool ClockEnable(ClockNodeEnum clockIndex, bool isEnable)
{
    ClockNodeStruct* node = ClockMap + clockIndex;
    if (node->Type > ClockTypeGate)
        return false;

    if (node->Width == 0)
        return true;

    if (isEnable ^ (node->Type == ClockTypeGate && !node->IsActiveHigh))
    {
        *node->Register |= BIT(node->Offset);
    }
    else
    {
        *node->Register &= ~BIT(node->Offset);
    }

    if (clockIndex == ClockNodePLL3G)
    {
        RCC->PLL3LOAD = 0x3F;
        SystemDelayUs(1);
        RCC->PLL3LOAD = 0;
    }

    if (!node->Timeout)
        return true;

    uint8_t tryCount = node->Timeout;
    do {
        if (((*node->Register >> (node->Offset + 1)) & 1) == isEnable)
            return true;
        SystemDelayUs(tryCount ? 1000 : 0);
    }
    while (tryCount--);

    return false;
}

bool ClockDivide(ClockNodeEnum clockIndex, uint32_t divisor)
{
    ClockNodeStruct* node = ClockMap + clockIndex;
    if (node->Type != ClockTypeDivider)
        return false;

    uint32_t regVar = 0;

    bool hasBypassBit = node->DividerType == DividerTypeBypassArithmetic || node->DividerType == DividerTypeBypassGeometric;

    if (hasBypassBit)
    {
        if (divisor >= node->B)
        {
            regVar |= BIT(node->Width - 1);
        }
        else
        {
            divisor = node->B;
        }
    }

    if (node->DividerType <= DividerTypeBypassArithmetic)
    {
        regVar |= (divisor - node->B) / node->A;
    }
    else if (node->DividerType <= DividerTypeBypassGeometric)
    {
        if (node->A == 2 && divisor > node->B)
        {
            regVar |= __CLZ(node->B) - __CLZ(divisor);
        }
    }
    else if (node->DividerType == DividerTypeFractional)
    {
        regVar = (uint32_t)(((uint64_t)divisor << node->A) / FEATURE_FRACTIONAL_PRECISION);
    }

    if (regVar > BITM(node->Width))
        return false;

    *node->Register = (*node->Register & ~BITW(node->Offset, node->Width)) | regVar << node->Offset;

    return true;
}

bool ClockMultiply(ClockNodeEnum clockIndex, uint32_t multiple)
{
    ClockNodeStruct* node = ClockMap + clockIndex;
    if (node->Type != ClockTypeMultiplier)
        return false;

    uint32_t regVar = 0;

    if (node->MultiplierType == MultiplierTypeArithmetic)
    {
        regVar = (multiple - node->B) / node->A;
    }
    else if (node->MultiplierType == MultiplierTypeFractional)
    {
        if (node->B)
        {
            uint32_t multipleB = multiple / FEATURE_FRACTIONAL_PRECISION;
            ClockMultiply((ClockNodeEnum)node->B, multipleB);
            multiple -= multipleB * FEATURE_FRACTIONAL_PRECISION;
        }
        regVar = (uint32_t)(((uint64_t)multiple << node->A) / FEATURE_FRACTIONAL_PRECISION);
    }

    *node->Register = (*node->Register & ~BITW(node->Offset, node->Width)) | regVar << node->Offset;

    return true;
}

bool ClockSelect(ClockNodeEnum clockIndex, ClockNodeEnum selection)
{
    ClockNodeStruct* node = ClockMap + clockIndex;
    if (node->Type != ClockTypeSelector)
        return false;

    for (uint32_t i = 0; i < node->Count; i++)
    {
        if (node->Nodes[i] == selection)
        {
            *node->Register = (*node->Register & ~BITW(node->Offset, node->Width)) | i << node->Offset;

            if (!node->Timeout)
                return true;

            uint8_t tryCount = node->Timeout;
            do {
                if (((*node->Register >> (node->Offset + node->Width)) & BITM(node->Width)) == i)
                    return true;
                SystemDelayUs(tryCount ? 1000 : 0);
            }
            while (tryCount--);

            return false;
        }
    }

    return false;
}

uint32_t ClockGetDivisor(ClockNodeEnum clockIndex)
{
    ClockNodeStruct* node = ClockMap + clockIndex;
    if (node->Type != ClockTypeDivider)
        return 0;

    volatile uint32_t divisor = (*node->Register >> node->Offset) & BITM(node->Width);

    bool hasBypassBit = node->DividerType == DividerTypeBypassArithmetic || node->DividerType == DividerTypeBypassGeometric;

    if (hasBypassBit)
    {
        if (!(divisor >> (node->Width - 1)))
            return 1;
        divisor &= BITM(node->Width - 1);
    }

    if (node->DividerType <= DividerTypeBypassArithmetic)
    {
        divisor = node->A * divisor + node->B;
    }
    else if (node->DividerType <= DividerTypeBypassGeometric)
    {
        uint32_t nDivisor = node->B;
        for (int i = 0; i < divisor; i++)
        {
            nDivisor *= node->A;
        }
        divisor = nDivisor;
    }
    else if (node->DividerType == DividerTypeFractional)
    {
        divisor = (uint32_t)((uint64_t)divisor * FEATURE_FRACTIONAL_PRECISION >> node->A);
    }

    return divisor;
}

uint32_t ClockGetMultiple(ClockNodeEnum clockIndex)
{
    ClockNodeStruct* node = ClockMap + clockIndex;
    if (node->Type != ClockTypeMultiplier)
        return false;

    uint32_t regVar = (*node->Register >> node->Offset) & BITM(node->Width);
    if (node->MultiplierType == MultiplierTypeArithmetic)
    {
        return node->A * (regVar) + node->B;
    }
    else if (node->MultiplierType == MultiplierTypeFractional)
    {
        return (uint32_t)(((uint64_t)regVar * FEATURE_FRACTIONAL_PRECISION) >> node->A);
    }

    return 0;
}

ClockNodeEnum ClockGetSelection(ClockNodeEnum clockIndex)
{
    ClockNodeStruct* node = ClockMap + clockIndex;
    if (node->Type != ClockTypeSelector)
        return ClockNodeNull;

    uint8_t selectIndex = (*node->Register >> node->Offset) & BITM(node->Width);
    if (selectIndex > node->Count)
        return ClockNodeNull;

    return (ClockNodeEnum)node->Nodes[selectIndex];
}

uint32_t ClockGet(ClockNodeEnum clockIndex)
{
    ClockNodeStruct* node = ClockMap + clockIndex;

    switch (node->Type)
    {
        case ClockTypeGate:
            return (*node->Register >> node->Offset & BIT0 ^ !node->IsActiveHigh) ? ClockGet(node->Parent) : 0;
        case ClockTypeSource:
            return ((node->Width == 0) || (*node->Register & BIT(node->Offset + 1))) ? node->Frequency : 0;
        case ClockTypeDivider: {
            uint32_t divisor   = ClockGetDivisor(clockIndex);
            uint32_t frequency = ClockGet(node->Parent);
            if (node->DividerType == DividerTypeFractional)
                return (uint32_t)((uint64_t)frequency * FEATURE_FRACTIONAL_PRECISION / divisor);
            else
                return frequency / divisor;
        }
        case ClockTypeMultiplier: {
            uint32_t multiple  = ClockGetMultiple(clockIndex);
            uint64_t frequency = ClockGet(node->Parent);
            if (node->MultiplierType == MultiplierTypeFractional)
            {
                frequency = frequency * multiple / FEATURE_FRACTIONAL_PRECISION;
                if (node->B)
                    frequency += ClockGet((ClockNodeEnum)node->B);
                uint32_t remainder = (uint32_t)frequency % 100;
                return (uint32_t)(frequency - remainder + (remainder > 40 ? 100 : 0));
            }
            else
                return frequency * multiple;
        }
        case ClockTypeSelector: {
            ClockNodeEnum selection = ClockGetSelection(clockIndex);
            return selection == ClockNodeNull ? 0 : ClockGet(selection);
        }
        default:
            break;
    }

    return 0;
}

inline PeripheralEnum PeripheralFind(void* peripheralBase, PeripheralEnum start, PeripheralEnum end)
{
    PeripheralStruct* peripheral = NULL;
    for (PeripheralEnum peripheralIndex = start; peripheralIndex <= end; peripheralIndex++)
    {
        peripheral = PeripheralMap + peripheralIndex;
        if ((uint32_t)peripheralBase == peripheral->Base)
            return peripheralIndex;
    }

    return PeripheralNull;
}

inline void PeripheralEnable(PeripheralEnum peripheralIndex, bool isEnable)
{
    PeripheralStruct* peripheral = PeripheralMap + peripheralIndex;

    __IO uint32_t* enr = 0;
    if (peripheral->Node < ClockNodeAPB)
    {
        enr = FEATURE_PERIPH_AHBENR + (peripheral->Node - ClockNodeAHB);
    }
    else
    {
        enr = FEATURE_PERIPH_APBENR + (peripheral->Node - ClockNodeAPB);
    }

    if (isEnable)
    {
        *enr |= BIT(peripheral->Gate);
    }
    else
    {
        *enr &= ~BIT(peripheral->Gate);
    }
}

inline void PeripheralResetState(PeripheralEnum peripheralIndex, bool isEnable)
{
    PeripheralStruct* peripheral = PeripheralMap + peripheralIndex;

    __IO uint32_t* rstr = 0;
    if (peripheral->Node < ClockNodeAPB)
    {
        rstr = FEATURE_PERIPH_AHBRSTR + (peripheral->Node - ClockNodeAHB);
    }
    else
    {
        rstr = FEATURE_PERIPH_APBRSTR + (peripheral->Node - ClockNodeAPB);
    }

    if (isEnable)
    {
        *rstr |= BIT(peripheral->Gate);
    }
    else
    {
        *rstr &= ~BIT(peripheral->Gate);
    }
}

inline void PeripheralReset(PeripheralEnum peripheralIndex)
{
    PeripheralResetState(peripheralIndex, true);
    PeripheralResetState(peripheralIndex, false);
}
