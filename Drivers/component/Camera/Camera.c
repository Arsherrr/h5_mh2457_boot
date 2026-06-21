#include "Camera.h"

bool CameraClockInit(CameraStruct* camera) {
    if (camera->ClockIO == IONone) {
        if (camera->ClockSetup) {
            camera->ClockSetup();
        }
        return true;
    }

    // Enable timer clock
    PeripheralEnable(camera->Timer, true);
    PeripheralReset(camera->Timer);

    uint8_t timerAF    = 0;
    uint8_t timerIndex = camera->Timer - PeripheralTIMStart;
    timerIndex++;
    if (timerIndex <= 2)
        timerAF = 1;
    else if (timerIndex <= 5)
        timerAF = 2;
    else if (timerIndex <= 11)
        timerAF = 3;
    else if (timerIndex <= 14)
        timerAF = 9;

    // Configure XCLK IO as alternate function
    IOConfigStruct ioConfig = MakeIOConfig(IOModeAlternate, timerAF, IOPullNone, IOSpeedHigh, IODriveLow);
    IOSetup(camera->ClockIO, ioConfig);

    TIM_TypeDef* tim = (TIM_TypeDef*)PeripheralMap[camera->Timer].Base;

    uint32_t timerRootClock = ClockGet(PeripheralMap[camera->Timer].Node) / 1000;
    uint16_t timerDivisor   = timerRootClock / camera->FrequencyKHz;

    if (timerDivisor == 0)
        return false;

    // reduce timer divisor clock error
    uint32_t timerError = timerRootClock / timerDivisor - camera->FrequencyKHz;
    if (timerError && (camera->FrequencyKHz - timerRootClock / (timerDivisor + 1) < timerError))
        timerDivisor += 1;

    // Configure timer to generate square wave
    TIM_TimeBaseInitTypeDef timerConfig = {0};
    timerConfig.TIM_Period              = 1;
    timerConfig.TIM_Prescaler           = timerDivisor - 1;
    timerConfig.TIM_ClockDivision       = 0;
    timerConfig.TIM_CounterMode         = TIM_CounterMode_Up;
    TIM_TimeBaseInit(tim, &timerConfig);

    TIM_OCInitTypeDef timerOC = {0};
    timerOC.TIM_OCMode        = TIM_OCMode_PWM1;
    timerOC.TIM_OutputState   = TIM_OutputState_Enable;
    timerOC.TIM_Pulse         = 1;
    timerOC.TIM_OCPolarity    = TIM_OCPolarity_High;
    switch (camera->TimerChannel) {
        case 1:
            TIM_OC1Init(tim, &timerOC);
            break;
        case 2:
            TIM_OC2Init(tim, &timerOC);
            break;
        case 3:
            TIM_OC3Init(tim, &timerOC);
            break;
        case 4:
            TIM_OC4Init(tim, &timerOC);
            break;
        default:
            break;
    }

    // for advance timer
    TIM_CtrlPWMOutputs(tim, ENABLE);

    TIM_Cmd(tim, ENABLE);

    return true;
}

#define CameraPower(isOn, isNegativePower)    IOSet(camera->PowerIO, isOn ^ isNegativePower)
#define CameraReset(isReset, isNegativeReset) IOSet(camera->ResetIO, isReset ^ isNegativeReset)

bool CameraStartupSequence(CameraStruct* camera, bool isStart, bool isNegativePower, bool isNegativeReset, uint8_t tPower, uint8_t tReset, uint8_t tReady) {
    // Setup PWDN control io
    if (camera->PowerIO != IONone) {
        IOSetup(camera->PowerIO, IO_DEFAULT_OUTPUT_CONFIG);
    }

    // Setup RST control io
    if (camera->ResetIO != IONone) {
        IOSetup(camera->ResetIO, IO_DEFAULT_OUTPUT_CONFIG);
    }

    // power off and reset at first
    CameraPower(false, isNegativePower);
    CameraReset(true, isNegativeReset);
    SystemDelay(tPower);

    if (isStart) {
        // power on
        CameraPower(true, isNegativePower);
        SystemDelay(tReset);
        // release reset
        CameraReset(false, isNegativeReset);
        SystemDelay(tReady);
    }

    return true;
}

static CameraStruct* dvpCamera = NULL;

static bool DVPStart(CameraStruct* camera, bool isStart) {
    DCMI_CaptureCmd((FunctionalState)isStart);

    if (camera->ExecuteCommand)
        camera->ExecuteCommand(camera, isStart ? CameraCommandResume : CameraCommandSuspend, NULL);

    return true;
}

bool CameraDVPConstractor(CameraStruct* camera, I2CPortStruct* i2cPort, uint32_t* frameBuffer) {
    memset(camera, 0, sizeof(CameraStruct));
    camera->Port        = CameraPortDVP;
    camera->I2CPort     = i2cPort;
    camera->FrameBuffer = frameBuffer;

    camera->PowerIO = IONone;
    camera->ResetIO = IONone;
    camera->ClockIO = IONone;

    camera->Start = DVPStart;

    dvpCamera = camera;

    return true;
}

#define DVP_IO_ALTERNATE 0x0D
#define DVP_DMA_PERIPH   PeripheralDMA2
#define DVP_DMA_STREAM   DMA2_Stream7
#define DVP_DMA_CHANNEL  DMA_Channel_1

// #define VINT PH11
// #define HINT PH12
// #define FINT PH14

static void DVPIOSetup(IOEnum* ioList, uint8_t ioCount) {
    for (int i = 0; i < ioCount; i++) {
        IOSetup(*(ioList++), MakeIOConfig(IOModeAlternate, DVP_IO_ALTERNATE, IOPullNone, IOSpeedHigh, IODriveLow));
    }
}

static uint32_t dvpBlockSize;

static void DVPDMAEnable(bool isEnable) {
    DMA_Cmd(DVP_DMA_STREAM, (FunctionalState)isEnable);
    while (DMA_GetCmdStatus(DVP_DMA_STREAM) != (FunctionalState)isEnable) {}

    if (!isEnable) {
        DMA_ClearFlag(DVP_DMA_STREAM, DMA_FLAG_ALL);

        DVP_DMA_STREAM->NDTR = dvpBlockSize;
        DVP_DMA_STREAM->CR &= ~(uint32_t)(DMA_SxCR_CT);
    }
}

static void DVPDMASynchronize(bool isStart) {
    if (isStart) {
        DVPDMAEnable(true);
        DVPDMAEnable(false);
    }
    else {
        // Wait until FIFO empty
        while (DCMI->SR & BIT2) {}
    }
    DVPDMAEnable(isStart);
}

static void DVPDMASetup(uint32_t* buffer, uint32_t size) {
    DMA_InitTypeDef dmaConfig = {0};

    PeripheralEnable(DVP_DMA_PERIPH, true);
    PeripheralReset(DVP_DMA_PERIPH);

    DMA_DeInit(DVP_DMA_STREAM);

    dvpBlockSize = size / 2 / 4;

    dmaConfig.DMA_Channel       = DVP_DMA_CHANNEL;
    dmaConfig.DMA_Mode          = DMA_Mode_Circular;
    dmaConfig.DMA_Priority      = DMA_Priority_High;
    dmaConfig.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    dmaConfig.DMA_FIFOMode      = DMA_FIFOMode_Enable;
    dmaConfig.DMA_DIR           = DMA_DIR_PeripheralToMemory;

    dmaConfig.DMA_PeripheralBaseAddr = (uint32_t)&DCMI->DR;
    dmaConfig.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    dmaConfig.DMA_PeripheralBurst    = DMA_PeripheralBurst_INC4;
    dmaConfig.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    dmaConfig.DMA_BufferSize         = dvpBlockSize;

    dmaConfig.DMA_Memory0BaseAddr = (uint32_t)buffer;
    dmaConfig.DMA_MemoryDataSize  = DMA_MemoryDataSize_Word;
    dmaConfig.DMA_MemoryBurst     = DMA_MemoryBurst_INC16;
    dmaConfig.DMA_MemoryInc       = DMA_MemoryInc_Enable;

    DMA_Init(DVP_DMA_STREAM, &dmaConfig);

    DMA_DoubleBufferModeConfig(DVP_DMA_STREAM, (uint32_t)buffer + size / 2, DMA_Memory_0);
    DMA_DoubleBufferModeCmd(DVP_DMA_STREAM, ENABLE);
}

#ifndef TRACE_DCMI
#define TRACE_DCMI 0
#endif

static bool DVPSetup(CameraStruct* camera) {
    // IO
    if (camera->Port == CameraPortDVP)
        DVPIOSetup(camera->IOList, camera->IOCount);

#if defined(VINT) || defined(HINT) || defined(FINT)
    IOSetup(VINT, IO_DEFAULT_OUTPUT_CONFIG);
    IOSetup(HINT, IO_DEFAULT_OUTPUT_CONFIG);
    IOSetup(FINT, IO_DEFAULT_OUTPUT_CONFIG);
#endif

    uint8_t iColorByteCount = ((camera->MemoryColor & 0xF0) >> 4) + 1;

    PeripheralEnable(PeripheralDCMI, true);

    DCMI_Cmd(DISABLE);
    DCMI_CaptureCmd(DISABLE);

    DCMI_InitTypeDef dcmiConfig = {0};
    dcmiConfig.DCMI_CaptureMode = camera->IsSnapshot ? DCMI_CaptureMode_SnapShot : DCMI_CaptureMode_Continuous;
    dcmiConfig.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
    if (camera->Port == CameraPortCSI) {
        dcmiConfig.DCMI_PCKPolarity      = DCMI_PCKPolarity_Rising;
        dcmiConfig.DCMI_VSPolarity       = DCMI_VSPolarity_High;
        dcmiConfig.DCMI_HSPolarity       = DCMI_HSPolarity_Low;
        dcmiConfig.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_14b;

        iColorByteCount = 2;
    }
    else {
        dcmiConfig.DCMI_PCKPolarity      = camera->IsPclkFallingSampling ? DCMI_PCKPolarity_Falling : DCMI_PCKPolarity_Rising;
        dcmiConfig.DCMI_VSPolarity       = camera->IsVsyncValidLow ? DCMI_VSPolarity_High : DCMI_VSPolarity_Low;
        dcmiConfig.DCMI_HSPolarity       = camera->IsHsyncValidLow ? DCMI_HSPolarity_High : DCMI_HSPolarity_Low;
        dcmiConfig.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
    }
    dcmiConfig.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame; // DCMI_CaptureRate_1of4_Frame;

    dcmiConfig.DCMI_LineSelectMode = DCMI_LineSelect_Mode_AllLine;
    dcmiConfig.DCMI_ByteSelectMode = DCMI_ByteSelect_Mode_AllByte;

#if (FEATURE_DCMI_CR_REQSIZE || FEATURE_DCMI_FIFOSR_DMACR)
    dcmiConfig.DCMI_DMARequestSize = 4;
#endif

#if FEATURE_DCMI_MIPI_INPUT
    dcmiConfig.DCMI_InputMode = camera->Port == CameraPortDVP ? DCMI_Input_Mode_DVP : DCMI_Input_Mode_MIPI;
#endif

    DCMI_Init(&dcmiConfig);

    uint16_t width  = camera->Window.X;
    uint16_t height = camera->Window.Y;

    // Crop Window
    CameraWindowStruct* cropWindow = &camera->CropWindow;
    if (cropWindow->X > 0 && cropWindow->Y > 0 && (cropWindow->X + cropWindow->XOffset) <= width && (cropWindow->Y + cropWindow->YOffset) <= height) {
        DCMI_CROPInitTypeDef cropConfig = {
            .DCMI_HorizontalOffsetCount = cropWindow->XOffset * iColorByteCount,
            .DCMI_CaptureCount          = (cropWindow->X * iColorByteCount) - 1,

            .DCMI_VerticalStartLine = cropWindow->YOffset,
            .DCMI_VerticalLineCount = cropWindow->Y - 1,
        };

        DCMI_CROPConfig(&cropConfig);
        DCMI_CROPCmd(ENABLE);

        width  = cropWindow->X;
        height = cropWindow->Y;
    }

    // Interrupt
    NVIC_SetPriority(DCMI_IRQn, 1);
    NVIC_ClearPendingIRQ(DCMI_IRQn);
    NVIC_EnableIRQ(DCMI_IRQn);

    uint16_t dcmiITs = DCMI_IT_VSYNC | DCMI_IT_FRAME;
#if TRACE_DCMI
    dcmiITs |= DCMI_IT_LINE | DCMI_IT_OVF | DCMI_IT_ERR;
#endif

    DCMI_ITConfig(dcmiITs, ENABLE);
    DCMI_ClearITPendingBit(dcmiITs);

    DVPDMASetup(camera->FrameBuffer, width * height * iColorByteCount);

    DCMI_Cmd(ENABLE);
    return true;
}

void DCMI_IRQHandler(void) {
    if (DCMI_GetITStatus(DCMI_IT_VSYNC) == SET) {
#ifdef VINT
        IOH(VINT);
        IOL(VINT);
#endif
        DCMI_ClearITPendingBit(DCMI_IT_VSYNC);
        if (DCMI->CR & DCMI_CR_CAPTURE) {
            // Start Sync Event
            if (dvpCamera->SyncCallback)
                dvpCamera->SyncCallback(dvpCamera, true);
            DVPDMASynchronize(true);
        }
    }
    if (DCMI_GetITStatus(DCMI_IT_FRAME) == SET) {
#ifdef FINT
        IOH(FINT);
        IOL(FINT);
#endif
        DCMI_ClearITPendingBit(DCMI_IT_FRAME);
        DVPDMASynchronize(false);
        // End Sync Event
        if (dvpCamera->SyncCallback)
            dvpCamera->SyncCallback(dvpCamera, false);
    }

#if TRACE_DCMI
    if (DCMI_GetITStatus(DCMI_IT_LINE) == SET) {
#ifdef HINT
        IOH(HINT);
        IOL(HINT);
#endif
        DCMI_ClearITPendingBit(DCMI_IT_LINE);
    }
    if (DCMI_GetITStatus(DCMI_IT_OVF) == SET) {
        DCMI_ClearITPendingBit(DCMI_IT_OVF);
    }
    if (DCMI_GetITStatus(DCMI_IT_ERR) == SET) {
        DCMI_ClearITPendingBit(DCMI_IT_ERR);
    }
#endif
}

bool CameraInit(CameraStruct* camera) {
    DVPSetup(camera);
    return true;
}
