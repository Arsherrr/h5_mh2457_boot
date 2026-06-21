#ifndef __DSI_ILI9806E_H__
#define __DSI_ILI9806E_H__

#include "DisplayDSI.h"

#define DISPLAY_ILI9806E_PRESET_KD043WVFIA083          1 // 480x800
#define DISPLAY_ILI9806E_PRESET_KD050FWFIA019_02_C002A 0 // 5.0"  480x854

#ifndef CONFIG_DISPLAY_ILI9806E_PRESET
#define CONFIG_DISPLAY_ILI9806E_PRESET DISPLAY_ILI9806E_PRESET_KD050FWFIA019_02_C002A
#endif

#ifndef CONFIG_DISPLAY_ILI9806E_USE_RGB888
#define CONFIG_DISPLAY_ILI9806E_USE_RGB888 1
#endif

/*!
    TargetPixelClock:    |      30 | MHz
    TargetPixelPeriod:   |   33.33 | ns
    TargetFPS:           |      60 | Frame
    FrameClockCount:     | 5000000 |
    TargetHorizonCount:  |     558 |
    TargetVerticalCount: |     896 |
    ---------------------+---------+
    HSA                  |       8 |
    HBP                  |      10 |
    HACT                 |     480 |
    HFP                  |      60 |
    ---------------------+---------+
    VSA                  |       2 |
    VBP                  |      18 |
    VACT                 |     800 |
    VFP                  |      76 |
*/

#if CONFIG_DISPLAY_ILI9806E_PRESET == DISPLAY_ILI9806E_PRESET_KD050FWFIA019_02_C002A
#define ILI9806E_PIXEL_CLOCK 30000
#define ILI9806E_LANE_CLOCK  500000

#define ILI9806E_HSA  8
#define ILI9806E_HBP  10
#define ILI9806E_HACT 480
#define ILI9806E_HFP  60

#define ILI9806E_VSA  2
#define ILI9806E_VBP  18
#define ILI9806E_VACT 854
#define ILI9806E_VFP  22
#elif CONFIG_DISPLAY_ILI9806E_PRESET == DISPLAY_ILI9806E_PRESET_KD043WVFIA083
#define ILI9806E_PIXEL_CLOCK 30000
#define ILI9806E_LANE_CLOCK  500000

#define ILI9806E_HSA  8
#define ILI9806E_HBP  10
#define ILI9806E_HACT 480
#define ILI9806E_HFP  60

#define ILI9806E_VSA  2
#define ILI9806E_VBP  18
#define ILI9806E_VACT 800
#define ILI9806E_VFP  76
#endif
extern bool ILI9806EConstractor(DisplayStruct* display);

#endif // __DSI_ILI9806E_H__
