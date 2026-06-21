#ifndef __DSI_GC9C01_H__
#define __DSI_GC9C01_H__

#include "DisplayDSI.h"

/*!
    TargetPixelClock:    |      24 | MHz
    TargetPixelPeriod:   |   41.67 | ns
    TargetFPS:           |      60 | Frame
    FrameClockCount:     |  400000 |
    TargetHorizonCount:  |     800 |
    TargetVerticalCount: |     500 |
    ---------------------+---------+
    HSA (Min:2)          |      20 |
    HBP (Min:1.88us)     |     320 |
    HFP (Min:0.94us)     |     300 |
    ---------------------+---------+
    VSA (Min:2)          |      20 |
    VBP (Min:18)         |      20 |
    VFP (Min:8)          |     100 |
*/

#define GC9C01_HSA  2
#define GC9C01_HBP  8
#define GC9C01_HACT 360
#define GC9C01_HFP  430

#define GC9C01_VSA  1
#define GC9C01_VBP  0
#define GC9C01_VACT 360
#define GC9C01_VFP  2

extern bool GC9C01Constractor(DisplayStruct* display);

#endif // __DSI_GC9C01_H__
