#ifndef __DSI_ST77922_H__
#define __DSI_ST77922_H__

#include "DisplayDSI.h"

// Presets
#define DISPLAY_ST77922_PRESET_H040022A00 0 // 3.95" 480x480

#ifndef CONFIG_DISPLAY_ST77922_PRESET
#define CONFIG_DISPLAY_ST77922_PRESET DISPLAY_ST77922_PRESET_H040022A00
#endif

/*!
    TargetPixelClock:    |      30 | MHz
    TargetPixelPeriod:   |   33.33 | ns
    TargetFPS:           |      60 | Frame
    FrameClockCount:     | 5000000 |
    TargetHorizonCount:  |     969 |
    TargetVerticalCount: |     516 |
    ---------------------+---------+
    HSA                  |       9 |
    HBP                  |     200 |
    HACT                 |     480 |
    HFP                  |     280 |
    ---------------------+---------+
    VSA                  |       2 |
    VBP                  |      16 |
    VACT                 |     480 |
    VFP                  |      18 |
*/

#if CONFIG_DISPLAY_ST77922_PRESET == DISPLAY_ST77922_PRESET_H040022A00
#define ST77922_PIXEL_CLOCK 30000
#define ST77922_LANE_CLOCK  600000

#define ST77922_HSA  9
#define ST77922_HBP  280
#define ST77922_HACT 480
#define ST77922_HFP  200

#define ST77922_VSA  2
#define ST77922_VBP  16
#define ST77922_VACT 480
#define ST77922_VFP  18

#endif

extern bool ST77922Constractor(DisplayStruct* display);

#endif // __DSI_ST77922_H__
