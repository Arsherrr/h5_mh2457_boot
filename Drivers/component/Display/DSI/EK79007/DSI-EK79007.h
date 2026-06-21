#ifndef __DSI_EK79007_H__
#define __DSI_EK79007_H__

#include "DisplayDSI.h"

#define DISPLAY_EK79007_PRESET_S1121_A101KF63_56A 0 // 10.1" 1024*600

#ifndef CONFIG_DISPLAY_EK79007_PRESET
#define CONFIG_DISPLAY_EK79007_PRESET DISPLAY_EK79007_PRESET_S1121_A101KF63_56A
#endif

/*!
    TargetPixelClock:    |      42 | MHz
    TargetPixelPeriod:   |   23.81 | ns
    TargetFPS:           |      60 | Frame
    FrameClockCount:     |  700000 |
    TargetHorizonCount:  |    1059 |
    TargetVerticalCount: |     661 |
    ---------------------+---------+
    HSA                  |       5 |
    HBP                  |      10 |
    HACT                 |    1024 |
    HFP                  |      20 |
    ---------------------+---------+
    VSA                  |       1 |
    VBP                  |      10 |
    VACT                 |     600 |
    VFP                  |      50 |
*/

#define EK79007_PIXEL_CLOCK 42000
#define EK79007_LANE_CLOCK  500000

#define EK79007_HSA  5
#define EK79007_HBP  10
#define EK79007_HACT 1024
#define EK79007_HFP  20

#define EK79007_VSA  1
#define EK79007_VBP  10
#define EK79007_VACT 600
#define EK79007_VFP  50

extern bool EK79007Constractor(DisplayStruct* display);

#endif // __DSI_EK79007_H__
