#ifndef __DSI_ST7102_H__
#define __DSI_ST7102_H__

#include "DisplayDSI.h"

#define DISPLAY_ST7102_PRESET_BH4961XT1_1 0 // 4.96" 480x854

#ifndef CONFIG_DISPLAY_ST7102_PRESET
#define CONFIG_DISPLAY_ST7102_PRESET DISPLAY_ST7102_PRESET_BH4961XT1_1
#endif

/*!
    TargetPixelClock:    |   33.33 | MHz
    TargetPixelPeriod:   |      30 | ns
    TargetFPS:           |      60 | Frame
    FrameClockCount:     | 5555555 |
    TargetHorizonCount:  |     512 |
    TargetVerticalCount: |    1085 |
    ---------------------+---------+
    HSA                  |       2 |
    HBP                  |      10 |
    HACT                 |     480 |
    HFP                  |      20 |
    ---------------------+---------+
    VSA                  |       2 |
    VBP                  |       9 |
    VACT                 |     854 |
    VFP                  |     220 | > 3ms for Touch
*/

#if CONFIG_DISPLAY_ST7102_PRESET == DISPLAY_ST7102_PRESET_BH4961XT1_1

#define ST7102_PIXEL_CLOCK 33333
#define ST7102_LANE_CLOCK  600000

#define ST7102_HSA  2
#define ST7102_HBP  10
#define ST7102_HACT 480
#define ST7102_HFP  20

#define ST7102_VSA  2
#define ST7102_VBP  9
#define ST7102_VACT 854
#define ST7102_VFP  220

#endif

extern bool ST7102Constractor(DisplayStruct* display);

#endif // __DSI_ST7102_H__
