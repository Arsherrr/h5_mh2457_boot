#ifndef __DSI_JD9852_H__
#define __DSI_JD9852_H__

#include "DisplayDSI.h"

/*!
    TargetPixelClock:    |      24 | MHz
    TargetPixelPeriod:   |   41.67 | ns
    TargetFPS:           |      60 | Frame
    FrameClockCount:     |  400000 |
    TargetHorizonCount:  |     800 |
    TargetVerticalCount: |     500 |
    ---------------------+---------+
    HSA                  |      20 |
    HBP                  |     220 |
    HACT                 |     240 |
    HFP                  |     320 |
    ---------------------+---------+
    VSA                  |      10 |
    VBP                  |      10 |
    VACT                 |     320 |
    VFP                  |     160 |
*/

#define JD9852_PIXEL_CLOCK 20000
#define JD9852_LANE_CLOCK  320000

#define JD9852_HSA  20
#define JD9852_HBP  220
#define JD9852_HACT 240
#define JD9852_HFP  320

#define JD9852_VSA  10
#define JD9852_VBP  20
#define JD9852_VACT 320
#define JD9852_VFP  160

extern bool JD9852Constractor(DisplayStruct* display);

#endif // __DSI_JD9852_H__
