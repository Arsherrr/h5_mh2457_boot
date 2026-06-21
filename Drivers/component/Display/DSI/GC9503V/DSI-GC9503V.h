#ifndef __DSI_GC9503V_H__
#define __DSI_GC9503V_H__

#include "DisplayDSI.h"

#define DISPLAY_GC9503V_PRESET_JT04524123   0 // 4.45" 480x854
#define DISPLAY_GC9503V_PRESET_FSD63B9001_A 1 // 6.2"  360x960
#define DISPLAY_GC9503V_PRESET_HD397014C31  2 // 3.97" 480x800

#ifndef CONFIG_DISPLAY_GC9503V_PRESET
#define CONFIG_DISPLAY_GC9503V_PRESET DISPLAY_GC9503V_PRESET_HD397014C31
#endif

/*!
    TargetPixelClock:    |      30 |      30 |      30 | MHz
    TargetPixelPeriod:   |   33.33 |   33.33 |   33.33 | ns
    TargetFPS:           |      60 |      60 |      60 | Frame
    FrameClockCount:     |  500000 |  700000 | 5000000 |
    TargetHorizonCount:  |     558 |     500 |     580 |
    TargetVerticalCount: |     896 |    1000 |     862 |
    ---------------------+---------+---------+---------+
    HSA                  |       8 |       8 |      10 |
    HBP                  |      10 |      12 |      10 |
    HDUM                 |       0 |      60 |       0 |
    HACT                 |     480 |     360 |     480 |
    HFP                  |      60 |      60 |      80 |
    ---------------------+---------+---------+---------+
    VSA                  |       2 |       8 |      10 |
    VBP                  |      18 |      16 |      20 |
    VACT                 |     854 |     960 |     800 |
    VFP                  |      22 |      16 |      32 |
*/
#if CONFIG_DISPLAY_GC9503V_PRESET == DISPLAY_GC9503V_PRESET_JT04524123
#define GC9503V_PIXEL_CLOCK 42000
#define GC9503V_LANE_CLOCK  500000

#define GC9503V_HSA  8
#define GC9503V_HBP  10
#define GC9503V_HACT 480
#define GC9503V_HFP  60

#define GC9503V_VSA  2
#define GC9503V_VBP  18
#define GC9503V_VACT 854
#define GC9503V_VFP  22
#elif CONFIG_DISPLAY_GC9503V_PRESET == DISPLAY_GC9503V_PRESET_FSD63B9001_A
#define GC9503V_PIXEL_CLOCK 42000
#define GC9503V_LANE_CLOCK  500000

#define GC9503V_HSA  8
#define GC9503V_HBP  12
#define GC9503V_HDUM 60
#define GC9503V_HACT 360
#define GC9503V_HFP  60

#define GC9503V_VSA  8
#define GC9503V_VBP  16
#define GC9503V_VACT 960
#define GC9503V_VFP  16
#elif CONFIG_DISPLAY_GC9503V_PRESET == DISPLAY_GC9503V_PRESET_HD397014C31
#define GC9503V_PIXEL_CLOCK 30000
#define GC9503V_LANE_CLOCK  500000

#define GC9503V_HSA  10
#define GC9503V_HBP  10
#define GC9503V_HACT 480
#define GC9503V_HFP  80

#define GC9503V_VSA  10
#define GC9503V_VBP  20
#define GC9503V_VACT 800
#define GC9503V_VFP  32
#endif

#ifndef GC9503V_HDUM
#define GC9503V_HDUM 0
#endif

extern bool GC9503VConstractor(DisplayStruct* display);

#endif // __DSI_GC9503V_H__
