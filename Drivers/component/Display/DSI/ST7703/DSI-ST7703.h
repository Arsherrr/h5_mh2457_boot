#ifndef __DSI_ST7703_H__
#define __DSI_ST7703_H__

#include "DisplayDSI.h"

#define DISPLAY_ST7703_PRESET_PSD395006EB 0
#define DISPLAY_ST7703_PRESET_HD395003C30 1

#ifndef CONFIG_DISPLAY_ST7703_PRESET
#define CONFIG_DISPLAY_ST7703_PRESET DISPLAY_ST7703_PRESET_PSD395006EB
#endif

/*!
    TargetPixelClock:    |     42 |     42 | MHz
    TargetPixelPeriod:   |  23.81 |  23.81 | ns
    TargetFPS:           |     60 |     60 | Frame
    FrameClockCount:     | 700000 | 700000 |
    TargetHorizonCount:  |    915 |    710 |
    TargetVerticalCount: |    765 |    986 |
    ---------------------+--------+--------+
    HSA                  |     15 |      8 |
    HBP                  |     20 |     20 |
    HACT                 |    720 |    640 |
    HFP                  |    160 |     42 |
    ---------------------+--------+--------+
    VSA                  |      3 |      2 |
    VBP                  |     12 |     12 |
    VACT                 |    720 |    960 |
    VFP                  |     30 |     12 |
*/

#if CONFIG_DISPLAY_ST7703_PRESET <= DISPLAY_ST7703_PRESET_HD395003C30
#define ST7703_PIXEL_CLOCK 42000
#define ST7703_LANE_CLOCK  600000

#define ST7703_HSA  15
#define ST7703_HBP  20
#define ST7703_HACT 720
#define ST7703_HFP  160

#define ST7703_VSA  3
#define ST7703_VBP  12
#define ST7703_VACT 720
#define ST7703_VFP  30

#else

#define ST7703_PIXEL_CLOCK 42000
#define ST7703_LANE_CLOCK  600000

#define ST7703_HSA  8
#define ST7703_HBP  20
#define ST7703_HACT 640
#define ST7703_HFP  42

#define ST7703_VSA  2
#define ST7703_VBP  12
#define ST7703_VACT 960
#define ST7703_VFP  12

#endif

extern bool ST7703Constractor(DisplayStruct* display);

#endif // __DSI_ST7703_H__
