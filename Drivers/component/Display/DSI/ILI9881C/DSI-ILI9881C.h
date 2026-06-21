#ifndef __DSI_ILI9881C_H__
#define __DSI_ILI9881C_H__

#include "DisplayDSI.h"

#define DISPLAY_ILI9881C_PRESET_KD050HDFIA020 0 // 5.0"  720x1280
#define DISPLAY_ILI9881C_PRESET_HD50004C30    1 // 4.96" 720x1280
#define DISPLAY_ILI9881C_PRESET_KD080WXFID015 2 // 8.0"  800x1280
#define DISPLAY_ILI9881C_PRESET_XHT080BS027A0 3 // 8.0"  800x1280
#define DISPLAY_ILI9881C_PRESET_XHT101CS001A0 4 // 10.1" 800x1280
#define DISPLAY_ILI9881C_PRESET_QT101IM2QA805 5 // 10.1" 800x1280

#ifndef CONFIG_DISPLAY_ILI9881C_PRESET
#define CONFIG_DISPLAY_ILI9881C_PRESET DISPLAY_ILI9881C_PRESET_HD50004C30
#endif

/*!
    TargetPixelClock:    |      42 |      42 | MHz
    TargetPixelPeriod:   |   23.81 |   23.81 | ns
    TargetFPS:           |      36 |      36 | Frame
    FrameClockCount:     | 1166667 | 1166667 |
    TargetHorizonCount:  |     892 |     892 |
    TargetVerticalCount: |    1308 |    1308 |
    ---------------------+---------+---------+
    HSA                  |      10 |      10 |
    HBP                  |      20 |      30 |
    HACT                 |     800 |     720 |
    HFP                  |      62 |     132 |
    ---------------------+---------+---------+
    VSA                  |       2 |       2 | Min: 2
    VBP                  |      16 |      16 | Min: 14
    VACT                 |    1280 |    1280 |
    VFP                  |      10 |      10 | Min: 8
*/

#if CONFIG_DISPLAY_ILI9881C_PRESET >= DISPLAY_ILI9881C_PRESET_KD080WXFID015

#define ILI9881C_PIXEL_CLOCK 42000
#define ILI9881C_LANE_CLOCK  800000

#define ILI9881C_HSA  10
#define ILI9881C_HBP  20
#define ILI9881C_HACT 800
#define ILI9881C_HFP  62

#define ILI9881C_VSA  2
#define ILI9881C_VBP  16
#define ILI9881C_VACT 1280
#define ILI9881C_VFP  10

#else

#define ILI9881C_PIXEL_CLOCK 42000
#define ILI9881C_LANE_CLOCK  800000

#define ILI9881C_HSA  10
#define ILI9881C_HBP  30
#define ILI9881C_HACT 720
#define ILI9881C_HFP  132

#define ILI9881C_VSA  2
#define ILI9881C_VBP  16
#define ILI9881C_VACT 1280
#define ILI9881C_VFP  10

#endif

extern bool ILI9881CConstractor(DisplayStruct* display);

#endif // __DSI_ILI9881C_H__
