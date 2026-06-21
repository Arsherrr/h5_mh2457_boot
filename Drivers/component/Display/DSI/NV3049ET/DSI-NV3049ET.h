#ifndef __DSI_NV3049ET_H__
#define __DSI_NV3049ET_H__

#include "DisplayDSI.h"

#define DISPLAY_NV3049ET_PRESET_ENH_TV0313A002 0 // 3.13" 376x960

#ifndef CONFIG_DISPLAY_NV3049ET_PRESET
#define CONFIG_DISPLAY_NV3049ET_PRESET DISPLAY_NV3049ET_PRESET_ENH_TV0313A002
#endif

/*!
    TargetPixelClock:    |     30 | MHz
    TargetPixelPeriod:   |  33.33 | ns
    TargetFPS:           |     60 | Frame
    FrameClockCount:     | 500000 |
    TargetHorizonCount:  |    500 |
    TargetVerticalCount: |   1000 |
    ---------------------+--------+
    HSA                  |      8 |
    HBP                  |     16 |
    HACT                 |    376 |
    HFP                  |    100 |
    ---------------------+--------+
    VSA                  |      2 |
    VBP                  |      8 |
    VACT                 |    960 |
    VFP                  |     30 |
*/

#define NV3049ET_PIXEL_CLOCK 30000
#define NV3049ET_LANE_CLOCK  500000

#define NV3049ET_HSA  8
#define NV3049ET_HBP  16
#define NV3049ET_HACT 376
#define NV3049ET_HFP  100

#define NV3049ET_VSA  2
#define NV3049ET_VBP  18
#define NV3049ET_VACT 960
#define NV3049ET_VFP  20

extern bool NV3049ETConstractor(DisplayStruct* display);

#endif // __DSI_NV3049ET_H__
