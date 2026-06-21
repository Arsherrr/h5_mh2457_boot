#ifndef __DSI_AXS15260_H__
#define __DSI_AXS15260_H__

#include "DisplayDSI.h"

#define DISPLAY_AXS15260_PRESET_DAIKIN 0 // 3.95" 540x540

#ifndef CONFIG_DISPLAY_AXS15260_PRESET
#define CONFIG_DISPLAY_AXS15260_PRESET DISPLAY_AXS15260_PRESET_DAIKIN
#endif
/*!
    TargetPixelClock:    |     30 | MHz
    TargetPixelPeriod:   |  33.33 | ns
    TargetFPS:           |     60 | Frame
    FrameClockCount:     | 500000 |
    TargetHorizonCount:  |    761 |
    TargetVerticalCount: |    657 |
    ---------------------+--------+
    HSA                  |     21 |
    HBP                  |    100 |
    HACT                 |    540 |
    HFP                  |    100 |
    ---------------------+--------+
    VSA                  |      2 |
    VBP                  |     10 |
    VACT                 |    540 |
    VFP                  |    105 |
*/

#define AXS15260_PIXEL_CLOCK 30000
#define AXS15260_LANE_CLOCK  360000

#define AXS15260_HSA  11
#define AXS15260_HBP  10
#define AXS15260_HACT 540
#define AXS15260_HFP  200

#define AXS15260_VSA  2
#define AXS15260_VBP  10
#define AXS15260_VACT 540
#define AXS15260_VFP  105

extern bool AXS15260Constractor(DisplayStruct* display);

#endif // __DSI_AXS15260_H__
