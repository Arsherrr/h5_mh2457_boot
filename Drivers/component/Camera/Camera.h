#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "I2CPort.h"

/* --------  Default Use/Config Definition  --------------------------------- */

#ifndef USE_CAMERA_PORT_CSI
#define USE_CAMERA_PORT_CSI 1
#endif

#ifndef USE_CAMERA_PORT_DVP
#define USE_CAMERA_PORT_DVP 1
#endif

/* --------  Exported types  ------------------------------------------------ */

typedef enum {
    CameraPortDVP,
    CameraPortCSI,
} CameraPortEnum;

/*!
    CameraColorEnum:
        BITS[6]:   pixelBigEndian
        BITS[5:4]: PixelByteCount - 1
        BITS[3:2]: PixelColorType:
            0: RAW, 1: YUV, 2: RGB, 3: RSVD
        BITS[1:0]: PixelColorSubType
*/
typedef enum {
    CameraColorRaw8RGGB  = 0x00,
    CameraColorRaw8BGGR  = 0x01,
    CameraColorRaw8GBRG  = 0x02,
    CameraColorRaw8GRGG  = 0x03,
    CameraColorRaw8      = CameraColorRaw8BGGR,
    CameraColorRaw10RGGB = 0x10,
    CameraColorRaw10BGGR = 0x11,
    CameraColorRaw10GBRG = 0x12,
    CameraColorRaw10GRGG = 0x13,
    CameraColorRaw10     = CameraColorRaw10BGGR,

    CameraColorYUV400 = 0x04,
    CameraColorYUV420 = 0x14,
    CameraColorYUV422 = 0x15,
    CameraColorYUV444 = 0x24,

    CameraColorRGB444 = 0x18,
    CameraColorRGB555 = 0x19,
    CameraColorRGB565 = 0x1A,
    CameraColorRGB666 = 0x28,
    CameraColorRGB888 = 0x29,

    CameraColorRGB565BE = CameraColorRGB565 | BIT6,
} CameraColorEnum;

typedef struct {
    uint16_t XOffset;
    uint16_t YOffset;
    uint16_t X;
    uint16_t Y;
} CameraWindowStruct;

typedef enum {
    CameraCommandSuspend,
    CameraCommandResume,
    CameraCommandSleep,
    CameraCommandWakeup,

    CameraCommandSetFlip,
    CameraCommandSetExposure,
} CameraCommandEnum;

typedef struct CameraStruct_s CameraStruct;

struct CameraStruct_s {
    I2CPortStruct* I2CPort;
    uint8_t        I2CAddress;

    CameraPortEnum Port : 8;

    IOEnum PowerIO : 8;
    IOEnum ResetIO : 8;
    IOEnum ClockIO : 8;

    // Config
    bool IsSnapshot : 1;

    union {
        // Use timer clock if ClockIO is not IONone
        struct {
            uint16_t FrequencyKHz;

            PeripheralEnum Timer;

            uint8_t TimerChannel;
        };
        // Use ClockSetup() if ClockIO is IONone and the ClockSetup is not NULL
        bool (*ClockSetup)(void);
    };

    union {
        // DVP Config
        struct {
            bool    IsHsyncValidLow : 1;
            bool    IsVsyncValidLow : 1;
            bool    IsPclkFallingSampling : 1;
            uint8_t IOCount;
            IOEnum* IOList;
        };
    };

    CameraWindowStruct Window;
    CameraWindowStruct CropWindow;
    CameraWindowStruct MemoryWindow;

    CameraColorEnum Color : 8;
    CameraColorEnum MemoryColor : 8;

    uint32_t* FrameBuffer;

    bool (*Init)(CameraStruct* camera);
    bool (*Start)(CameraStruct* camera, bool isStart);

    bool (*ExecuteCommand)(CameraStruct* camera, CameraCommandEnum command, uint32_t parameter);

    void (*SyncCallback)(CameraStruct* camera, bool isStartSync);
};

/* --------  Public Function  ----------------------------------------------- */

extern bool CameraClockInit(CameraStruct* camera);

extern bool CameraStartupSequence(CameraStruct* camera, bool isStart, bool isNegativePower, bool isNegativeReset, uint8_t tPower, uint8_t tReset,
                                  uint8_t tReady);

extern bool CameraDVPConstractor(CameraStruct* camera, I2CPortStruct* i2cPort, uint32_t* frameBuffer);

extern bool CameraInit(CameraStruct* camera);

#endif
