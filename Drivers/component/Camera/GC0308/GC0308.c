#include "GC0308.h"

static const I2CByteConfigStruct GC0308Configs[];

static bool Init(CameraStruct* camera) {
    // I2C setup
    I2CPortStruct* i2cPort = camera->I2CPort;

    // Software reset
    if (!i2cPort->WriteValue(i2cPort, I2CTransferA1D1, 0xFE, 0x80)) {
        return false;
    }
    SystemDelay(10);

    // Load default config
    if (!i2cPort->ByteLoad(i2cPort, GC0308Configs)) {
        return false;
    }

    // Change Pixel Color
    uint8_t colorCode = GC0308_FORMAT_GRAY;
    switch (camera->Color) {
        case CameraColorYUV400:
            colorCode = GC0308_FORMAT_GRAY;
            break;
        case CameraColorRGB565BE:
            colorCode = GC0308_FORMAT_RGB565BE;
            break;
        case CameraColorYUV422:
            colorCode = GC0308_FORMAT_YUYV;
            break;
        default:
            break;
    }
    if (!i2cPort->WriteValue(i2cPort, I2CTransferA1D1, 0x24, colorCode)) {
        return false;
    }

    // Crop Window
    CameraWindowStruct* window = &camera->Window;
    CameraWindowStruct* crop   = &camera->CropWindow;
    if ((window->X > crop->X) || (window->Y > crop->Y)) {
        I2CByteConfigStruct cropConfig[] = {
            {0x46, 0x80 | (crop->XOffset >> 8 & 0x7) | (((crop->YOffset >> 8) & 0x3) << 4)},
            {0x47, crop->YOffset & 0xFF},
            {0x48, crop->XOffset & 0xFF}, // X Start
            {0x49, crop->Y >> 8},
            {0x4a, crop->Y & 0xFF},
            {0x4b, crop->X >> 8},
            {0x4c, crop->X & 0xFF},
            {},
        };

        if (!i2cPort->ByteLoad(i2cPort, (const I2CByteConfigStruct*)&cropConfig)) {
            return false;
        }
    }

    return CameraInit(camera);
}

bool GC0308Constractor(CameraStruct* camera) {
    camera->Window     = (CameraWindowStruct) {0, 0, GC0308_WIDTH, GC0308_HEIGHT};
    camera->CropWindow = (CameraWindowStruct) {0, 0, GC0308_WIDTH, GC0308_HEIGHT};
    camera->I2CAddress = GC0308_I2C_ADDR;

    camera->IsHsyncValidLow       = false;
    camera->IsVsyncValidLow       = true;
    camera->IsPclkFallingSampling = false;

    camera->Color       = CONFIG_GC0308_DEFAULT_COLOR;
    camera->MemoryColor = camera->Color;

    if (camera->ClockIO != IONone)
        camera->FrequencyKHz = CONFIG_GC0308_XCLK / 1000;

    if (!CameraClockInit(camera))
        return false;

    if (!CameraStartupSequence(camera, true, true, true, 10, 10, 10))
        return false;

    // I2C setup
    I2CPortStruct* i2cPort = camera->I2CPort;
    i2cPort->SetAddress(i2cPort, GC0308_I2C_ADDR);

    // Read ID
    uint8_t id = 0;
    if (!i2cPort->ReadValue(i2cPort, I2CTransferA1D1, 0x00, &id) || id != GC0308_ID)
        return false;

    camera->Init = Init;

    return true;
}

static const I2CByteConfigStruct GC0308Configs[] = {
    {0xfe, 0x00}, // Reg Page 0

    {0x01, GC0308_Hb_L8},  // HB
    {0x02, GC0308_Vb_L8},  // VB
    {0x03, GC0308_Exp_H4}, // 曝V光时间
    {0x04, GC0308_Exp_L8},
    {0x05, 0x00}, // 行起始像素0
    {0x06, 0x00},
    {0x07, 0x00}, // 列起始像素0
    {0x08, 0x00},
    {0x09, 0x01}, // 列像素数488
    {0x0A, 0xE8},
    {0x0B, 0x02}, // 行像素数648
    {0x0C, 0x88},
    {0x0D, 0x08}, // HSYNC前后延迟
    {0x0E, 0x08},
    {0x0F, GC0308_Vb_H4 << 4}, // HB/VB高4位

    {0x10, 0x22},
    {0x11, 0x0d},
    {0x12, 0x2a},
    {0x13, 0x00},
    {0x14, 0x13},
    {0x15, 0x0a},
    {0x16, 0x05},
    {0x17, 0x01},
    {0x1a, 0x2a},
    {0x1b, 0x00},
    {0x1c, 0x49},
    {0x1d, 0x9a},
    {0x1e, 0x61},
    {0x1f, 0x3f},

    {0x20, GC0308_BlockEnable1},

    {0x24, GC0308_FORMAT_GRAY}, // Default Format
    {0x25, 0x0F},               // 允许输出
    {0x26, 0x3A},               // VSYNC低有效

    {0x28, 0x00}, // 不分频

    {0xd2, 0x10}, // close AEC
    {0x22, 0x40}, // close AWB

    {0x5a, 0x56},
    {0x5b, 0x40},
    {0x5c, 0x4a},
    {0x22, 0x76}, // Open AWB

    {0x6e, 0x8F}, // (黑边增强)高4位为增强强度 越大越强 低四位为斜率越大越强
    {0x6f, 0x00}, // (白边增强)自动边缘增强

    {0xb3, 0x40}, // 对比度倍数 0x40为1.0
    {0xb4, 0x40}, // 对比度中心
    {0xb5, 0x00}, // 亮度偏移

    {0xd1, 0x11}, // 两帧调节一次AEC
    {0xd3, 0x40}, // 调节亮度
    {0xe2, GC0308_Step_H4},
    {0xe3, GC0308_Step_L8},
    {0xe4, GC0308_Exp1_H4},
    {0xe5, GC0308_Exp1_L8},
    {0xe6, GC0308_Exp2_H4},
    {0xe7, GC0308_Exp2_L8},
    {0xe8, GC0308_Exp3_H4},
    {0xe9, GC0308_Exp3_L8},
    {0xea, GC0308_Exp4_H4},
    {0xeb, GC0308_Exp4_L8},
    {0xec, 0x03 << 4},
    {0xed, 0x01},
    {0xef, 0x40},
    {0xee, 0x80},
    {0xd2, 0x90},
    {},
};
