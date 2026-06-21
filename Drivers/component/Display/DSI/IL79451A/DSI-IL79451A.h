#ifndef __DSI_IL79451A_H__
#define __DSI_IL79451A_H__

#include "DisplayDSI.h"

#define DISPLAY_IL79451A_PRESET_HL03489HD652 0 // 6.52" 576x1280

#ifndef CONFIG_DISPLAY_IL79451A_PRESET
#define CONFIG_DISPLAY_IL79451A_PRESET DISPLAY_IL79451A_PRESET_HL03489HD652
#endif

/*!
    TargetPixelClock:    |      42 | MHz
    TargetPixelPeriod:   |   23.81 | ns
    TargetFPS:           |      42 | Frame
    FrameClockCount:     | 1000000 |
    TargetHorizonCount:  |     731 |
    TargetVerticalCount: |    1368 |
    ---------------------+---------+
    HSA                  |      15 |
    HBP                  |      20 |
    HACT                 |     576 |
    HFP                  |     120 |
    ---------------------+---------+
    VSA                  |       2 |
    VBP                  |      18 |
    VACT                 |    1280 |
    VFP                  |      68 |
*/

#if CONFIG_DISPLAY_IL79451A_PRESET == DISPLAY_IL79451A_PRESET_HL03489HD652

#define IL79451A_PIXEL_CLOCK 42000
#define IL79451A_LANE_CLOCK  600000

#define IL79451A_HSA  15
#define IL79451A_HBP  20
#define IL79451A_HACT 576
#define IL79451A_HFP  120

#define IL79451A_VSA  2
#define IL79451A_VBP  18
#define IL79451A_VACT 1280
#define IL79451A_VFP  68

#endif

extern bool IL79451AConstractor(DisplayStruct* display);

#endif // __DSI_IL79451A_H__
