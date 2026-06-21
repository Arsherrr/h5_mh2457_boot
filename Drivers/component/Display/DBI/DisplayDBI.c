#include "DisplayDBI.h"

static bool DBIWirtePacket(DisplayStruct* display, DBIPacketStruct* packet) {
    if (packet->IsRead)
        return false;
    *display->CommandPointer = packet->Command;
    for (int i = 0; i < packet->Length; i++) {
        *display->DataPointer = packet->Payload[i];
    }
    return true;
}

static bool DBIReadPacket(DisplayStruct* display, DBIPacketStruct* packet, uint8_t* readBuffer) {
    if (!packet->IsRead)
        return false;
    *display->CommandPointer = packet->Command;
    for (int i = 0; i < packet->Dummy; i++) {
        *display->DataPointer;
    }
    for (int i = 0; i < packet->Length; i++) {
        readBuffer[i] = *display->DataPointer;
    }
    return true;
}

static bool DisplayDBIInit(DisplayStruct* display) {
    uint8_t fsmcBank = 0;
    uint8_t fsmcRS   = 0;

    if (display->IOCount) {
        IOConfigStruct ioConfig = MakeIOConfig(IOModeAlternate, GPIO_AF10_FSMC, IOPullNone, IOSpeedHigh, IODriveHigh);

        IOEnum* dbiIO = display->IOList;

        for (int i = 0; i < display->IOCount; i++) {
            if (*dbiIO == IONone) {
                dbiIO++;
                continue;
            }
            else if (i == 0) {
                static const uint8_t fsmcBankIO[] = {
                    PD7, PG9, PG10, PG12, // Bank 0-3
                };
                for (fsmcBank = 0; fsmcBank <= 3; fsmcBank++) {
                    if (*dbiIO == fsmcBankIO[fsmcBank])
                        break;
                    else if (fsmcBank == 3)
                        return false;
                }
                fsmcBank <<= 1;
                ioConfig.Alternate = GPIO_AF_FSMC;
            }
            else if (i == 1) {
                static const uint8_t fsmcRSIO[] = {
                    PF0,  PF1,  PF2,  PF3, PF4, PF5, PF12, PF13, // RS 0-7
                    PF14, PF15, PG0,  PG1, PG2, PG3, PG4,  PG5,  // RS 8-15
                    PD11, PD12, PD13, PE3, PE4, PE5, PE6,  PE2,  // RS 16-23
                    PG13, PG14,                                  // RS 24-25
                };
                for (fsmcRS = 0; fsmcRS <= 25; fsmcRS++) {
                    if (*dbiIO == fsmcRSIO[fsmcRS])
                        break;
                    else if (fsmcRS == 25)
                        return false;
                }
                ioConfig.Alternate = GPIO_AF_FSMC;
            }
            else {
                ioConfig.Alternate = GPIO_AF10_FSMC;
            }
            IOSetup(*(dbiIO++), ioConfig);
        }
    }

    PeripheralEnable(PeripheralFSMC, true);
    PeripheralReset(PeripheralFSMC);

    uint32_t fsmcClockPeriod = 1000000000U / ClockGet(ClockNodeAHB);

    FSMC_NORSRAMTimingInitTypeDef fsmcTimming = {0};

    fsmcTimming.FSMC_DataSetupTime = (display->SetupTime + fsmcClockPeriod - 1) / fsmcClockPeriod;

    FSMC_NORSRAMInitTypeDef fsmcConfig = {
        .FSMC_Bank                  = fsmcBank,
        .FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable,
        .FSMC_MemoryType            = FSMC_MemoryType_SRAM,
        .FSMC_MemoryDataWidth       = display->BusWidth == 8 ? FSMC_MemoryDataWidth_8b : FSMC_MemoryDataWidth_16b,
        .FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable,
        .FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low,
        .FSMC_WrapMode              = FSMC_WrapMode_Disable,
        .FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState,
        .FSMC_WriteOperation        = FSMC_WriteOperation_Enable,
        .FSMC_WaitSignal            = FSMC_WaitSignal_Disable,
        .FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable,
        .FSMC_WriteBurst            = FSMC_WriteBurst_Disable,
        .FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable,
        .FSMC_ReadWriteTimingStruct = &fsmcTimming,
        .FSMC_WriteTimingStruct     = NULL,
    };

    FSMC_NORSRAMInit(&fsmcConfig);
    FSMC_NORSRAMCmd(fsmcBank, ENABLE);

    display->CommandPointer = (__IO uint16_t*)(FSMC_MEM_BASE + (fsmcBank << 25));
    display->DataPointer    = (__IO uint16_t*)(FSMC_MEM_BASE + (fsmcBank << 25) + (1 << fsmcRS) * (display->BusWidth / 8));

    DBIPacketStruct* packet = display->DBIConfigPackets;
    while (packet->Command || packet->Length || packet->Payload) {
        if (packet->Length && packet->Payload == NULL) {
            SystemDelay(packet->Length);
        }
        else {
            DBIWirtePacket(display, packet);
        }
        packet++;
    }

    return true;
}

static bool DisplayDBIStart(DisplayStruct* display, bool isStart) {
    if (isStart && display->Layer->Buffer->Pointer) {
        // Start refresh from buffer
        display->Refresh(display, display->Layer->Buffer->Pointer, display->HorizontalActive, display->VerticalActive, 0, 0, NULL);
    }

    return true;
}

static bool DisplayDBIRefresh(DisplayStruct* display, void* buffer, uint16_t width, uint16_t height, uint16_t offsetX, uint16_t offsetY,
                              void (*ChangedCallback)(DisplayLayerStruct* layer)) {
    // Send Commands
    DBIWirtePacket(display,
                   &(DBIPacketStruct)MakeDBIPacket(DCSSetColumnAddress, MSB(offsetX), LSB(offsetX), MSB(offsetX + width - 1), LSB(offsetX + width - 1)));
    DBIWirtePacket(display,
                   &(DBIPacketStruct)MakeDBIPacket(DCSSetPageAddress, MSB(offsetY), LSB(offsetY), MSB(offsetY + height - 1), LSB(offsetY + height - 1)));

    // TODO: DMA
    *display->CommandPointer = DCSWriteMemoryStart;

    uint16_t* pBuffer = buffer;
    for (int i = 0; i < width * height; i++) {
        *display->DataPointer = *pBuffer++;
    }

    return true;
}

uint32_t DisplayDBIReadID(DisplayStruct* display, DBIPacketStruct* readSequence) {
    DisplayDBIInit(display);

    uint32_t displayID = 0;

    uint8_t tryCount = 3;
    do {
        if (readSequence == NULL) {
            if (DBIReadPacket(display, &(DBIPacketStruct)MakeDBIPacketRead(0x04, 1, 3), (uint8_t*)&displayID)) {
                return __REV(displayID);
            }
            continue;
        }

        uint8_t readCount = 0;

        DBIPacketStruct* packet = readSequence;
        while (packet->Command || packet->Length || packet->Payload) {
            if (packet->IsDelay) {
                SystemDelay(packet->Length);
            }
            else if (packet->IsRead) {
                if (readCount + packet->Length > 4)
                    break;
                DBIReadPacket(display, packet, (uint8_t*)&displayID + readCount);
                readCount += packet->Length;
            }
            else {
                DBIWirtePacket(display, packet);
            }
            packet++;
        }

        if (packet->IsEnd)
            return __REV(displayID);
    }
    while (--tryCount);

    return 0;
}

bool DisplayDBIConstractor(DisplayStruct* display) {
    memset(display, 0, sizeof(DisplayStruct));

    display->Port = DisplayPortDBI;

    display->ResetIO         = IONone;
    display->TearingEffectIO = IONone;

    display->Color = DisplayColorRGB565;

    display->Init    = DisplayDBIInit;
    display->Start   = DisplayDBIStart;
    display->Refresh = DisplayDBIRefresh;

    return true;
}
