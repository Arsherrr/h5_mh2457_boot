#ifndef __DSI_HX8394F_H__
#define __DSI_HX8394F_H__

#include "DisplayDSI.h"

#define DISPLAY_HX8394F_PRESET_FLR_T164HS500_V0 0 // 5.00" 720x1280
#define DISPLAY_HX8394F_PRESET_FLR_T163HS550_V2 1 // 5.46" 720x1280
#define DISPLAY_HX8394F_PRESET_499BH3003N0      2 // 5.00" 720x1280

#ifndef CONFIG_DISPLAY_HX8394F_PRESET
#define CONFIG_DISPLAY_HX8394F_PRESET DISPLAY_HX8394F_PRESET_FLR_T163HS550_V2
#endif

#define DISPLAY_HX8394F_LANES 2

/*!
    TargetPixelClock:    |      42 | MHz
    TargetPixelPeriod:   |   23.81 | ns
    TargetFPS:           |      36 | Frame
    FrameClockCount:     | 1166667 |
    TargetHorizonCount:  |     892 |
    TargetVerticalCount: |    1308 |
    ---------------------+---------+
    HSA                  |      10 |
    HBP                  |      30 |
    HACT                 |     720 |
    HFP                  |     132 |
    ---------------------+---------+
    VSA                  |       2 | Min 2
    VBP                  |      16 | Min 2
    VACT                 |    1280 |
    VFP                  |      10 | Min 2
*/

#define HX8394F_PIXEL_CLOCK 42000
#define HX8394F_LANE_CLOCK  600000

#define HX8394F_HSA  10
#define HX8394F_HBP  20
#define HX8394F_HACT 720
#define HX8394F_HFP  132

#define HX8394F_VSA  2
#define HX8394F_VBP  16
#define HX8394F_VACT 1280
#define HX8394F_VFP  10

extern bool HX8394FConstractor(DisplayStruct* display);

#endif // __DSI_HX8394F_H__
