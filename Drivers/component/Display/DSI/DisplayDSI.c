#include "DisplayDSI.h"

static void DSIClockInit2(DisplayStruct* display) {
    //  - Reference Clock
    if (ClockGet(ClockNodePLL3R) != 48000000) {
        uint32_t pllSRC = ClockGet(ClockNodeSRC);
        ClockMultiply(ClockNodePLL3, ClockRatio(768.0 / ((float)pllSRC / 1000000.0)));
        ClockDivide(ClockNodePLL3R, ClockRatio(768.0 / 48.0));
        ClockSelect(ClockNodeREF, ClockNodePLL3R);
        ClockEnable(ClockNodePLL3G, true);
    }

    if (ClockGetSelection(ClockNodeREF) != ClockNodePLL3R)
        ClockSelect(ClockNodeREF, ClockNodePLL3R); // Select PLL3R as Reference 48MHz Clock

    PeripheralEnable(PeripheralPCFG, true);
    PCFG->HSI_CAL |= BIT24;
}

static void DSIClockInit(DisplayStruct* display) {
    //  - Reference Clock
    if (ClockGet(ClockNodeREF) != 48000000) {
        ClockMultiply(ClockNodePLL3, ClockRatio(768.0 / 12.0));
        ClockDivide(ClockNodePLL3R, ClockRatio(768.0 / 48.0));
        ClockSelect(ClockNodeREF, ClockNodePLL3R);
        ClockEnable(ClockNodePLL3G, true);
    }
    ClockSelect(ClockNodeREF, ClockNodePLL3R); // Select PLL3R as Reference 48MHz Clock

    PeripheralEnable(PeripheralPCFG, true);
    PCFG->HSI_CAL |= BIT24;
}

static bool DSIConfigInit(DisplayStruct* display) {
    DSIStruct* dsi = display->DSIHandle;

    const DSIPacketStruct* packet = display->ConfigPackets;
    while (packet->DataType != DSIDataTypeEnd) {
        if (packet->DataType == DSIDataTypeNull) {
            SystemDelay(packet->Length);
        }
        else {
            if (!dsi->Write(dsi, packet->DataType, packet->Payload, packet->Length))
                return false;
        }
        packet++;
    }

    return true;
}

static bool DSIInit(DisplayStruct* display, bool isConfiguring) {
    DSIClockInit(display);

    PeripheralEnable(PeripheralDSI, true);
    DSIStruct* dsi = display->DSIHandle;

    if (dsi->IsCommandMode) {

        dsi->CommandSize = display->HorizontalActive;

        dsi->IsLPGenericShortWrite0Para = isConfiguring;
        dsi->IsLPGenericShortWrite1Para = isConfiguring;
        dsi->IsLPGenericShortWrite2Para = isConfiguring;
        dsi->IsLPGenericShortRead0Para  = isConfiguring;
        dsi->IsLPGenericShortRead1Para  = isConfiguring;
        dsi->IsLPGenericShortRead2Para  = isConfiguring;
        dsi->IsLPGenericLongWrite       = isConfiguring;

        dsi->IsLPDCSShortWrite0Para = isConfiguring;
        dsi->IsLPDCSShortWirte1Para = isConfiguring;
        dsi->IsLPDCSShortRead0Para  = isConfiguring;
        dsi->IsLPDCSLongWrite       = isConfiguring;

        dsi->IsLPMaxReadPacketSize = true;
    }
    else {
        uint32_t pixelClock    = ClockGet(ClockNodeDPC) / 1000;
        uint32_t laneByteClock = dsi->LaneClock >> 3;

        // dsi->VideoMode = DSIVideoModeSyncPulses;
        // dsi->VideoMode = DSIVideoModeSyncEvents;
        dsi->VideoMode = DSIVideoModeBurst;

        dsi->IsLPVSAEnabled  = true;
        dsi->IsLPVBPEnabled  = true;
        dsi->IsLPVFPEnabled  = true;
        dsi->IsLPVACTEnabled = true;
        dsi->IsLPHBPEnabled  = true;
        dsi->IsLPHFPEnabled  = true;

        // dsi->IsFrameEndAckEnabled = true;
        dsi->IsLPCommandOnly = isConfiguring;

        dsi->VideoChunkCount = 0;
        dsi->VideoPacketSize = (display->HorizontalActive + display->HorizontalDummy) / (dsi->VideoChunkCount ? dsi->VideoChunkCount : 1);
        //dsi->VideoNullSize   = dsi->VideoPacketSize;

        dsi->HorizontalSyncActivePeriod = display->HorizontalSyncActive * laneByteClock / pixelClock;
        dsi->HorizontalBackPorchPeriod  = display->HorizontalBackPorch * laneByteClock / pixelClock;
        dsi->HorizontalTotalSize = (display->HorizontalSyncActive + display->HorizontalBackPorch + display->HorizontalActive + display->HorizontalFrontPorch +
                                    display->HorizontalDummy) *
                                   laneByteClock / pixelClock;

        dsi->VerticalSyncActivePeriod = display->VerticalSyncActive;
        dsi->VerticalBackPorchPeriod  = display->VerticalBackPorch;
        dsi->VerticalFrontPorchPeriod = display->VerticalFrontPorch;
        dsi->VerticalActivePeriod     = display->VerticalActive;

        dsi->VideoMaxBlankLPCommandSize  = 32;
        dsi->VideoMaxActiveLPCommandSize = 0;

        dsi->IsDataEnableActiveLow = false;
        dsi->IsVsyncActiveLow      = false;
        dsi->IsHsyncActiveLow      = false;
    }

    dsi->Init(dsi);
    if (isConfiguring)
        return dsi->Start(dsi, true);

    return true;
}

bool DisplayDSIInit(DisplayStruct* display) {
    if (!DSIConfigInit(display))
        return false;

    if (!DisplayDPIInit(display))
        return false;

    if (!DSIInit(display, false))
        return false;

    return true;
}

bool DisplayDSIStart(DisplayStruct* display, bool isStart) {
    DSIStruct* dsi = display->DSIHandle;
    if (!dsi->Start(dsi, isStart))
        return false;

    if (!dsi->IsCommandMode)
        return DisplayDPIStart(display, isStart);
    else if (display->RefreshMode == DisplayRefreshAdaptive) {
        // Start refresh from buffer
        display->Refresh(display, display->Layer->Buffer->Pointer, display->HorizontalActive, display->VerticalActive, 0, 0, NULL);
    }

    return true;
}

void DSI_IRQHandler(void) {
    uint32_t dsiInt1 = *(uint32_t*)(DSI_BASE + 0xC0);
    if (dsiInt1) {
        __NOP();
    }
}

uint32_t DisplayDSIReadID(DisplayStruct* display, const DSIPacketStruct* readSequence) {
    if (!DSIInit(display, true))
        return 0xFFFFFFFF;

    DSIStruct* dsi = display->DSIHandle;

    uint64_t displayID = 0;

    uint8_t tryCount = 3;
    do {
        if (readSequence == NULL) {
            if (dsi->Read(dsi, DSIDataTypeDCSRead, (uint8_t[]) {DCSGetDisplayID, 0x00}, 2, (uint8_t*)&displayID, 3)) {
                return __REV(displayID);
            }
            continue;
        }

        uint8_t readCount = 0;

        const DSIPacketStruct* packet = readSequence;
        while (packet->DataType != DSIDataTypeEnd) {
            if (packet->DataType == DSIDataTypeDCSRead) {
                if (readCount + packet->Length > 4)
                    break;

                if (!dsi->Read(dsi, DSIDataTypeDCSRead, (uint8_t[]) {packet->Command, 0x00}, 2, (uint8_t*)&displayID + readCount, packet->Length)) {
                    break;
                }

                readCount += packet->Length;
            }
            else if (packet->DataType == DSIDataTypeNull) {
                SystemDelay(packet->Length);
            }
            else {
                dsi->Write(dsi, packet->DataType, packet->Payload, packet->Length);
            }
            packet++;
        }

        if (packet->DataType == DSIDataTypeEnd)
            return __REV(displayID);
    }
    while (--tryCount);

    return 0;
}

static bool DisplayDSIRefresh(DisplayStruct* display, void* buffer, uint16_t width, uint16_t height, uint16_t offsetX, uint16_t offsetY,
                              void (*ChangedCallback)(DisplayLayerStruct* layer)) {
    DisplayLayerStruct* layer = display->Layer;
    if (display->RefreshMode == DisplayRefreshContinuous)
        return layer->ChangeBuffer(layer, buffer, ChangedCallback);

    // Adaptive Refresh
    //  Check if LTDC is disalbed
    if (LTDC->GCR & LTDC_GCR_LTDCEN)
        return true;

    //  Send Commands
    DSIPacketStruct packet[2] = {
        MakeDSIPacket(DCSSetColumnAddress, MSB(offsetX), LSB(offsetX), MSB(offsetX + width - 1), LSB(offsetX + width - 1)),
        MakeDSIPacket(DCSSetPageAddress, MSB(offsetY), LSB(offsetY), MSB(offsetY + height - 1), LSB(offsetY + height - 1)),
    };
    DSIStruct* dsi = display->DSIHandle;
    dsi->Write(dsi, packet[0].DataType, packet[0].Payload, packet[0].Length);
    dsi->Write(dsi, packet[1].DataType, packet[1].Payload, packet[1].Length);

    //  Change width and height of window
    uint16_t hStart = LTDC->BPCR >> 16;
    uint16_t vStart = LTDC->BPCR & 0xFFFF;
    LTDC->AWCR      = (hStart + width) << 16 | (vStart + height);
    //  Change width and height of layer
    layer->Register->WHPCR = (hStart + 1) | ((hStart + width) << 16);
    layer->Register->WVPCR = (vStart + 1) | ((vStart + height) << 16);
    //  Change the memory window
    layer->Register->CFBAR  = (uint32_t)buffer;
    layer->Register->CFBLR  = (layer->Register->CFBLR & 0xFFFF0000) | (width * 2);
    layer->Register->CFBLNR = height;

    //  Update registers
    LTDC->SRCR = LTDC_SRCR_IMR;

    //  Clear TE bit
    if (LTDC->DSI_WISR & BIT0) {
        LTDC->DSI_WIFCR = BIT0;
        LTDC->DSI_WIFCR = 0;
    }

    //  Enable LTDC to start refresh
    LTDC->DSI_WCR |= BIT3;

    layer->NextBuffer = buffer;
    if (ChangedCallback) {
        layer->ChangedCallback = ChangedCallback;
    }
    else {
        while (layer->NextBuffer) {
            __NOP();
        }
    }
    return true;
}

bool DisplayDSIConstractor(DisplayStruct* display, DSIStruct* dsiHandle) {
    memset(display, 0, sizeof(DisplayStruct));

    display->Port    = DisplayPortDSI;
    display->ResetIO = IONone;
    display->Color   = DisplayColorRGB565;

    display->Init    = DisplayDSIInit;
    display->Start   = DisplayDSIStart;
    display->Refresh = DisplayDSIRefresh;

    display->Layer[0].Register = (LTDC_Layer_TypeDef*)LTDC_Layer1_BASE;
    display->Layer[1].Register = (LTDC_Layer_TypeDef*)LTDC_Layer2_BASE;

    // DSI constract
    display->DSIHandle = dsiHandle;
    DSIConstractor(dsiHandle, DSI_BASE);

    return true;
}
