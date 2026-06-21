#ifndef __DSI_CO5300_H__
#define __DSI_CO5300_H__

#include "DisplayDSI.h"

#define DISPLAY_CO5300_PRESET_H0175Y003AMT003 0

#ifndef CONFIG_DISPLAY_CO5300_PRESET
#define CONFIG_DISPLAY_CO5300_PRESET DISPLAY_CO5300_PRESET_H0175Y003AMT003
#endif

/*!
    TargetPixelClock:    |     30 | MHz
    TargetPixelPeriod:   |     20 | ns
    TargetFPS:           |     50 | Frame
    FrameClockCount:     | 600000 |
    TargetHorizonCount:  |   1085 |
    TargetVerticalCount: |    553 |
    ---------------------+--------+
    HSA                  |     19 | Total Min (35us)
    HBP                  |    300 |
    HACT                 |    466 |
    HFP                  |    300 |
    ---------------------+--------+
    VSA                  |      2 |
    VBP                  |      8 |
    VACT                 |    466 |
    VFP                  |     40 |
*/

#define CO5300_PIXEL_CLOCK 30000
#define CO5300_LANE_CLOCK  360000

#define CO5300_HSA  19
#define CO5300_HBP  300
#define CO5300_HACT 466
#define CO5300_HFP  300

#define CO5300_VSA  6
#define CO5300_VBP  18
#define CO5300_VACT 466
#define CO5300_VFP  63

extern bool CO5300Constractor(DisplayStruct* display);

#endif // __DSI_CO5300_H__
