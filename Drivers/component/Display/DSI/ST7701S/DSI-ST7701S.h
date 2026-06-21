#ifndef __DSI_ST7701S_H__
#define __DSI_ST7701S_H__

#include "DisplayDSI.h"

// Presets for display ID: 0x8802 / 0x9903
#define DISPLAY_ST7701S_PRESET_KD050FWFID042 0 // 5.0"  480x854 - 0x8802
#define DISPLAY_ST7701S_PRESET_H050018C00    1 // 5.0"  480x854 - 0x9903
#define DISPLAY_ST7701S_PRESET_HD228001C31   2 // 2.8"  368x552 - 0x8802 / 0x9903
#define DISPLAY_ST7701S_PRESET_HD40007C30    3 // 3.95" 480x480
#define DISPLAY_ST7701S_PRESET_39HG192_1_T   4 // 3.97" 480x800 - 0x9903
#define DISPLAY_ST7701S_PRESET_B24C35065N_01 5 // 3.5"  480x800
#define DISPLAY_ST7701S_PRESET_HD43008C25    6 // 4.3"  480x800

#ifndef CONFIG_DISPLAY_ST7701S_PRESET
#define CONFIG_DISPLAY_ST7701S_PRESET DISPLAY_ST7701S_PRESET_KD050FWFID042
#endif

/*!
    TargetPixelClock:    |      30 |      30 |      30 |      30 | MHz
    TargetPixelPeriod:   |   33.33 |   33.33 |   33.33 |   33.33 | ns
    TargetFPS:           |      60 |      60 |      60 |      60 | Frame
    FrameClockCount:     | 5000000 | 5000000 | 5000000 | 5000000 |
    TargetHorizonCount:  |     558 |     800 |     800 |     558 |
    TargetVerticalCount: |     896 |     625 |     625 |     896 |
    ---------------------+---------+---------+---------+---------+
    HSA                  |       8 |      32 |      20 |       8 |
    HBP                  |      10 |     100 |      80 |      10 |
    HACT                 |     480 |     368 |     480 |     480 |
    HFP                  |      60 |     300 |     220 |      60 |
    ---------------------+---------+---------+---------+---------+
    VSA                  |       2 |       2 |       2 |       2 |
    VBP                  |      18 |      16 |      18 |      18 |
    VACT                 |     854 |     552 |     480 |     800 |
    VFP                  |      22 |      55 |     125 |      76 |
*/

#if CONFIG_DISPLAY_ST7701S_PRESET <= DISPLAY_ST7701S_PRESET_H050018C00
#define ST7701S_PIXEL_CLOCK 30000
#define ST7701S_LANE_CLOCK  500000

#define ST7701S_HSA  8
#define ST7701S_HBP  10
#define ST7701S_HACT 480
#define ST7701S_HFP  60

#define ST7701S_VSA  2
#define ST7701S_VBP  18
#define ST7701S_VACT 854
#define ST7701S_VFP  22

#elif CONFIG_DISPLAY_ST7701S_PRESET == DISPLAY_ST7701S_PRESET_HD228001C31
#define ST7701S_PIXEL_CLOCK 30000
#define ST7701S_LANE_CLOCK  500000

#define ST7701S_HSA  32
#define ST7701S_HBP  100
#define ST7701S_HACT 368
#define ST7701S_HFP  300

#define ST7701S_VSA  2
#define ST7701S_VBP  16
#define ST7701S_VACT 552
#define ST7701S_VFP  55

#elif CONFIG_DISPLAY_ST7701S_PRESET == DISPLAY_ST7701S_PRESET_HD40007C30
#define ST7701S_PIXEL_CLOCK 30000
#define ST7701S_LANE_CLOCK  500000

#define ST7701S_HSA  20
#define ST7701S_HBP  80
#define ST7701S_HACT 480
#define ST7701S_HFP  220

#define ST7701S_VSA  2
#define ST7701S_VBP  18
#define ST7701S_VACT 480
#define ST7701S_VFP  125

#elif CONFIG_DISPLAY_ST7701S_PRESET >= DISPLAY_ST7701S_PRESET_39HG192_1_T
#define ST7701S_PIXEL_CLOCK 30000
#define ST7701S_LANE_CLOCK  500000

#define ST7701S_HSA  8
#define ST7701S_HBP  10
#define ST7701S_HACT 480
#define ST7701S_HFP  60

#define ST7701S_VSA  2
#define ST7701S_VBP  18
#define ST7701S_VACT 800
#define ST7701S_VFP  76

#endif

extern bool ST7701SConstractor(DisplayStruct* display);

#endif // __DSI_ST7701S_H__
