#ifndef __DSI_JD9365DA_H__
#define __DSI_JD9365DA_H__

#include "DisplayDSI.h"

#define DISPLAY_JD9365DA_PRESET_FCX10104B_32    0 // 10.1" 800x1280
#define DISPLAY_JD9365DA_PRESET_NMLCD10M39      1 // 10.1" 800x1280
#define DISPLAY_JD9365DA_PRESET_HTG1015853281BB 2 // 10.1" 800x1280
#define DISPLAY_JD9365DA_PRESET_ZSX055F2491     3 // 10.1" 720x1280
#define DISPLAY_JD9161C_PRESET_XXXXX            4 //480*854

#ifndef CONFIG_DISPLAY_JD9365DA_PRESET
#define CONFIG_DISPLAY_JD9365DA_PRESET DISPLAY_JD9161C_PRESET_XXXXX
#endif

/*!
    TargetPixelClock:    |      42 | MHz
    TargetPixelPeriod:   |   23.81 | ns
    TargetFPS:           |      36 | Frame
    FrameClockCount:     | 1166667 |
    TargetHorizonCount:  |     892 |
    TargetVerticalCount: |    1308 |
    ---------------------+---------+
    HSA                  |      10 |
    HBP                  |      20 |
    HACT                 |     800 |
    HFP                  |      62 |
    ---------------------+---------+
    VSA                  |       2 |
    VBP                  |      16 |
    VACT                 |    1280 |
    VFP                  |      10 |
*/
#if CONFIG_DISPLAY_JD9365DA_PRESET == DISPLAY_JD9365DA_PRESET_ZSX055F2491
#define JD9365DA_PIXEL_CLOCK 42000
#define JD9365DA_LANE_CLOCK  560000

//832
#define JD9365DA_HSA  24
#define JD9365DA_HBP  48
#define JD9365DA_HACT 720
#define JD9365DA_HFP  40
//1330
#define JD9365DA_VSA  10
#define JD9365DA_VBP  10
#define JD9365DA_VACT 1280
#define JD9365DA_VFP  30

//#define JD9365DA_HSA  10
//#define JD9365DA_HBP  20
//#define JD9365DA_HACT 720
//#define JD9365DA_HFP  62

//#define JD9365DA_VSA  2
//#define JD9365DA_VBP  16
//#define JD9365DA_VACT 1280
//#define JD9365DA_VFP  10
#elif CONFIG_DISPLAY_JD9365DA_PRESET == DISPLAY_JD9161C_PRESET_XXXXX
//#define JD9365DA_PIXEL_CLOCK 22000 //OK
//#define JD9365DA_LANE_CLOCK  280000 //OK
#define JD9365DA_PIXEL_CLOCK 22000
#define JD9365DA_LANE_CLOCK  280000
//525
#define JD9365DA_HSA  15
#define JD9365DA_HBP  15
#define JD9365DA_HACT 480
#define JD9365DA_HFP  15
//874
#define JD9365DA_VSA  4
#define JD9365DA_VBP  4
#define JD9365DA_VACT 854
#define JD9365DA_VFP  12
#else
#define JD9365DA_PIXEL_CLOCK 42000
#define JD9365DA_LANE_CLOCK  600000

#define JD9365DA_HSA  10
#define JD9365DA_HBP  20
#define JD9365DA_HACT 800
#define JD9365DA_HFP  62

#define JD9365DA_VSA  2
#define JD9365DA_VBP  16
#define JD9365DA_VACT 1280
#define JD9365DA_VFP  10
#endif
extern bool JD9365DAConstractor(DisplayStruct* display);

#endif // __DSI_JD9365DA_H__
