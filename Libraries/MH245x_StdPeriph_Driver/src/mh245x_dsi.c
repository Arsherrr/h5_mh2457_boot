// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#include "mh245x_dsi.h"

#define DSI_PHY_REF_CLK 24000

#define DSI_PHY_PLLN_MAX 64
#define DSI_PHY_PLLM_MAX 300

#define DSI_PHY_CODE_PLLVCO 0x10
#define DSI_PHY_CODE_PLLN   0x17
#define DSI_PHY_CODE_PLLM   0x18
#define DSI_PHY_CODE_PLLCR  0x19

static uint8_t PHYWrite(DSIRegisterStruct* dsi, uint32_t waddr, uint32_t wdata)
{
    dsi->PHYVDR = (0x10000 | waddr);
    dsi->PHYVCR = 0x2;
    dsi->PHYVCR = 0x0;
    dsi->PHYVDR = wdata;
    dsi->PHYVCR = 0x2;
    dsi->PHYVCR = 0x0;
    return (dsi->PHYVDR & 0xFF00) >> 8;
}

#if FEATURE_DSI_SW_LPCMD

static void LPWriteSOT(DSIRegisterStruct* dsi)
{
    PHYWrite(dsi, 0x43, 0x04 | 2); // LP 10
    PHYWrite(dsi, 0x43, 0x04 | 0); // LP 00
    PHYWrite(dsi, 0x43, 0x04 | 1); // LP 01
    PHYWrite(dsi, 0x43, 0x04 | 0); // LP 00
}

static void LPWriteData(DSIRegisterStruct* dsi, uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        PHYWrite(dsi, 0x43, 0x04 | ((data >> i & 1) + 1)); // LP 01 / 10
        PHYWrite(dsi, 0x43, 0x04);                         // LP 00
    }
}

static void LPWriteEOT(DSIRegisterStruct* dsi)
{
    PHYWrite(dsi, 0x43, 0x04 | 2); // LP 10
    PHYWrite(dsi, 0x43, 0);        // LP 00
}

static const uint8_t MIPIEccMask[24] = {
    0x07, 0x0B, 0x0D, 0x0E, 0x13, 0x15, 0x16, 0x19, //
    0x1A, 0x1C, 0x23, 0x25, 0x26, 0x29, 0x2A, 0x2C, //
    0x31, 0x32, 0x34, 0x38, 0x1F, 0x2F, 0x37, 0x3B, //
};

static void GenerateECC(uint32_t* data)
{
    uint8_t* ecc = ((uint8_t*)data) + 3;
    for (int i = 0; i < 24; i++)
    {
        uint8_t eccN = ((*data >> i) & 1) ? MIPIEccMask[i] : 0x00;
        eccN |= (~MIPIEccMask[i]) & (*ecc ^ *ecc);
        *ecc ^= eccN;
    }
}

static uint16_t GenerateCRC(uint8_t* data, uint16_t length)
{
    CRC->CSR = 0x15;
    CRC->INI = 0xFFFF;
    CRC->XOR = 0x0000;
    CRC->CR  = BIT0;
    do {
        CRC->DR = *data++;
    }
    while (--length);
    return (uint16_t)CRC->DR;
}

static bool LPWrite(DSIStruct* self, DSIDataTypeEnum dataType, uint8_t* payload, uint16_t length)
{
    DSIRegisterStruct* dsi = self->Reg;

    uint32_t gHeader;
    uint16_t gCRC = 0;

    if ((dataType & BIT3) == 0)
    {
        if (length > 2)
            return false;

        uint16_t headerData = (length ? payload[0] : 0) | (length == 2 ? payload[1] << 8 : 0);

        gHeader = dataType | (self->DPIVirtualChannelID << 6) | (headerData << 8);
        length  = 0;
    }
    else
    {
        gHeader = dataType | (self->DPIVirtualChannelID << 6) | (length << 8);
        gCRC    = GenerateCRC(payload, length);
    }
    GenerateECC(&gHeader);

    LPWriteSOT(dsi);
    LPWriteData(dsi, 0x87);
    uint8_t* pHeader = (uint8_t*)&gHeader;
    for (int i = 0; i < 4; i++)
    {
        LPWriteData(dsi, *pHeader++);
    }

    if (length)
    {
        for (uint16_t i = 0; i < length; i++)
        {
            LPWriteData(dsi, payload[i]);
        }

        LPWriteData(dsi, gCRC & 0xFF);
        LPWriteData(dsi, gCRC >> 8);
    }
    LPWriteEOT(dsi);

    return true;
}

#endif

static void PHYInit(DSIStruct* self)
{
    DSIRegisterStruct* dsi = self->Reg;

    uint16_t laneFreqIn10MHz = self->LaneClock / 10000;
    uint8_t  hsFreqRangeCode = 0x00;
    if (laneFreqIn10MHz < 11)
    { // [80, 110)
        hsFreqRangeCode = (laneFreqIn10MHz - 8) << 4;
    }
    else if (laneFreqIn10MHz < 16)
    { // [110, 160)
        hsFreqRangeCode = laneFreqIn10MHz - 9;
        hsFreqRangeCode = ((hsFreqRangeCode % 3) << 4) | (hsFreqRangeCode / 3);
    }
    else if (laneFreqIn10MHz < 25)
    { // [160, 250)
        hsFreqRangeCode = laneFreqIn10MHz / 2 - 7;
        hsFreqRangeCode = ((hsFreqRangeCode % 3) << 4) | (hsFreqRangeCode / 3 + 2);
    }
    else if (laneFreqIn10MHz < 36)
    { // [250, 360)
        hsFreqRangeCode = laneFreqIn10MHz / 3 - 8;
        hsFreqRangeCode = ((hsFreqRangeCode % 2) << 4) | ((hsFreqRangeCode / 2) + 4);
    }
    else if (laneFreqIn10MHz < 40)
    { // [360, 400)
        hsFreqRangeCode = 2 << 4 | 5;
    }
    else if (laneFreqIn10MHz < 71)
    { // [400, 700]
        hsFreqRangeCode = laneFreqIn10MHz / 5 - 8;
        hsFreqRangeCode = ((hsFreqRangeCode % 2) << 4) | ((hsFreqRangeCode / 2) + 6);
    }
    else
    { // [700, 1500]
        if (laneFreqIn10MHz >= 150)
            laneFreqIn10MHz = 149;
        hsFreqRangeCode = laneFreqIn10MHz / 5 - 14;
        hsFreqRangeCode = ((hsFreqRangeCode % 4) << 4) | ((hsFreqRangeCode / 4) + 9);
    }
    hsFreqRangeCode <<= 1;

    PHYWrite(dsi, 0x44, hsFreqRangeCode);

    uint16_t bestN = 0;
    uint16_t bestM = 0;

    uint32_t bestError = DSI_PHY_REF_CLK;

    for (uint16_t n = 1; n <= DSI_PHY_PLLN_MAX; n++)
    {
        uint32_t nClock = DSI_PHY_REF_CLK / n;
        uint32_t error  = self->LaneClock / 2 % nClock * 2;
        uint16_t m      = self->LaneClock / 2 / nClock * 2;

        if (m > DSI_PHY_PLLM_MAX)
            continue;

        if (error < bestError)
        {
            bestError = error;
            bestN     = n;
            bestM     = m;
            if (bestError == 0)
                break;
        }
    }

    if (bestN && bestM)
    {
        bestN--;
        bestM--;
        PHYWrite(dsi, DSI_PHY_CODE_PLLCR, 0x30);
        PHYWrite(dsi, DSI_PHY_CODE_PLLN, bestN);
        PHYWrite(dsi, DSI_PHY_CODE_PLLM, bestM & 0x1F);
        PHYWrite(dsi, DSI_PHY_CODE_PLLM, 0x80 | (bestM >> 5));
    }
}

static inline void InternalClockConfig(DSIRegisterStruct* dsi, uint32_t laneByteClock, uint16_t escapeClock, uint16_t timeoutClock)
{
    uint8_t escapeDivision  = laneByteClock / escapeClock;
    uint8_t timeoutDivision = laneByteClock / timeoutClock;

    escapeDivision  = escapeDivision > 1 ? escapeDivision : 2;
    timeoutDivision = timeoutDivision > 1 ? timeoutDivision : 2;

    dsi->CLKCFG = escapeDivision | (timeoutDivision << 8);
}

static bool Init(DSIStruct* self)
{
    DSIRegisterStruct* dsi = self->Reg;

    // Init PHY
    PHYInit(self);
    //while ((dsi->PHYSTS & BIT(0)) != BIT(0)) {}
    // Date lane config
    dsi->PHYCFG = self->Lanes | (self->PhyStopWaitTime << 8);
    // Enable clock lane
    dsi->CLCFG = 0x1 | (self->IsAutoStopClockLane << 1);

    uint32_t laneByteClock = self->LaneClock >> 3;

    // Internal clock config
    InternalClockConfig(dsi, laneByteClock, self->EscapeClock, self->TimeoutClock);

    // Transit Time
    dsi->PHYDLTIM = self->PhyTransitTime | self->PhyTransitTime << 16;

    // Command Packet Interval
    uint16_t commandPacketInterval = self->CommandPacketInterval * (self->LaneClock >> 3) / 1000000;

    dsi->LPWRTO = commandPacketInterval;
    dsi->LPRDTO = commandPacketInterval;
    dsi->BTATO  = commandPacketInterval;

    dsi->DPICOLOR = self->ColorCoding | (self->IsLooselyColorCoding18 << 8);
    dsi->DPICFG   = self->PolarityConfig;

    // Set the dsi mode
    dsi->MODE = self->IsCommandMode;
    if (self->IsCommandMode)
    {
        dsi->DPIVCID  = self->DPIVirtualChannelID;
        dsi->ECMDSIZE = self->CommandSize;
        dsi->CMDMODE  = self->AckRequestConfig | (self->LPGenericCommandConfig << 8) | (self->LPDCSCommandConfig << 16) | (self->LPMaxReadCommandConfig << 24);

        dsi->PCKCFG = BIT2; // Enable BTA
    }
    else
    {
        dsi->VMODE  = self->VideoMode | (self->LPVideoConfig << 8) | (self->IsPatternGeneratorEnabled << 16);
        dsi->PKTS   = self->VideoPacketSize;
        dsi->CHUNKS = self->VideoChunkCount;
        dsi->NPKTS  = self->VideoNullSize;

        dsi->HSA   = self->HorizontalSyncActivePeriod;
        dsi->HBP   = self->HorizontalBackPorchPeriod;
        dsi->HLINE = self->HorizontalTotalSize;

        dsi->VSA  = self->VerticalSyncActivePeriod;
        dsi->VBP  = self->VerticalBackPorchPeriod;
        dsi->VFP  = self->VerticalFrontPorchPeriod;
        dsi->VACT = self->VerticalActivePeriod;

        dsi->DPILPTIM = (self->VideoMaxBlankLPCommandSize << 16) | self->VideoMaxActiveLPCommandSize;
    }

    dsi->INTMSK0 = 0;
    dsi->INTMSK1 = 0;
    return true;
}

static bool Start(DSIStruct* self, bool isStart)
{
    DSIRegisterStruct* dsi = self->Reg;

    if (isStart)
    {
        dsi->PWRUP = 0x1;
        dsi->PHYCR = 0x7;

        for (int tryCount = 0; tryCount < 200; tryCount++)
        {
            SystemDelayUs(10);
            if (dsi->PHYSTS & BIT0)
                return true;
        }

        return false;
    }
    else
    {
        dsi->PWRUP = 0x0;
        dsi->PHYCR = 0x0;
    }
    return true;
}

static bool Write(DSIStruct* self, DSIDataTypeEnum dataType, uint8_t* payload, uint16_t length)
{
    DSIRegisterStruct* dsi = self->Reg;

    /* Wait for Command FIFO Empty */
    while ((dsi->GSTS & BIT0) == 0) __NOP();

    if ((dataType & BIT3) == 0)
    {
        if (length > 2)
            return false;

        uint16_t headerData = (length ? payload[0] : 0) | (length == 2 ? payload[1] << 8 : 0);

        dsi->GHDR = dataType | (self->DPIVirtualChannelID << 6) | (headerData << 8);
        return true;
    }

    for (uint16_t i = 0; i < length; i += 4)
    {
        dsi->GPLD = payload[i] | payload[i + 1] << 8 | payload[i + 2] << 16 | payload[i + 3] << 24;
    }

    dsi->GHDR = dataType | (self->DPIVirtualChannelID << 6) | (length << 8);
    return true;
}

static bool Read(DSIStruct* self, DSIDataTypeEnum dataType, uint8_t* payload, uint16_t payloadLength, uint8_t* readBuffer, uint16_t readLength)
{
    volatile DSIRegisterStruct* dsi = self->Reg;

    // Clear INTSTS
    while (dsi->INTSTS0 || dsi->INTSTS1) __NOP();

    // calculate the approximate max read timeout value
    // Clock Base: Timeout Clock
    dsi->TRIGTO = (readLength + 16) * 16 * self->TimeoutClock * 2 / self->EscapeClock;

    if (!Write(self, DSIDataTypeMaxReturnSize, (uint8_t[]) {LSB(readLength), MSB(readLength)}, 2))
        return false;

    if (!Write(self, dataType, payload, payloadLength))
        return false;

    volatile uint32_t intsts0, intsts1;

    // wait for phy in rx mode
    do {
        SystemDelayUs(1);
        intsts1 = dsi->INTSTS1;
    }
    while (((dsi->PHYSTS & BIT1) == 0) && ((dsi->GSTS & BIT6) || ((dsi->GSTS & (BIT0 | BIT16)) == 0)) && !intsts1);

    // wait for read done, phy switch to tx mode or timeout
    while (((dsi->PHYSTS & BIT1) || (dsi->GSTS & BIT6)) && !intsts1)
    {
        intsts1 = dsi->INTSTS1;
    }

    intsts0 = dsi->INTSTS0;
    if (!intsts1)
    {
        intsts1 = dsi->INTSTS1;
    }

    if (intsts0 || intsts1 || (dsi->GSTS & BIT4))
    {
        // Reset DSI Host
        Start(self, false);
        Start(self, true);

        return false;
    }

    while (readLength)
    {
        volatile uint32_t readPayload = dsi->GPLD;
        if (readLength >= 4)
        {
            *((uint32_t*)readBuffer) = readPayload;
            readBuffer += 4;
            readLength -= 4;
        }
        else
        {
            for (int i = 0; i < readLength; i++)
            {
                *readBuffer++ = *((uint8_t*)&readPayload + i);
            }
            break;
        }
    }

    return true;
}

bool DSIConstractor(DSIStruct* self, uint32_t dsiBase)
{
    memset(self, 0, sizeof(DSIStruct));

    self->Init  = Init;
    self->Start = Start;

#if FEATURE_DSI_SW_LPCMD
    PeripheralEnable(PeripheralCRC, true);
    self->Write = LPWrite;
#else
    self->Write = Write;
#endif
    self->Read = Read;

    self->LaneClock    = 320000; // 320Mbps
    self->EscapeClock  = 20000;  // 20MHz
    self->TimeoutClock = 1000;   // 1MHz

    self->Lanes = DSILanes1;

    self->IsCommandMode       = true;
    self->IsAutoStopClockLane = true;

    self->DSIBase = dsiBase;

    self->PhyTransitTime = 20;

    self->CommandPacketInterval = 1000; // 1000ns

    return true;
}
