#ifndef __GC0308_H__
#define __GC0308_H__

#include "Camera.h"

#ifndef CONFIG_GC0308_XCLK
#define CONFIG_GC0308_XCLK 24000000U
#endif

#ifndef CONFIG_GC0308_DEFAULT_COLOR
#define CONFIG_GC0308_DEFAULT_COLOR CameraColorRGB565BE
#endif

#define GC0308_I2C_ADDR (0x21)
#define GC0308_ID       (0x9B)

#define GC0308_64MClkTime (16)
#define GC0308_32MClkTime (31)
#define GC0308_21MClkTime (47)

#define GC0308_Hb    (0x0050)
#define GC0308_Hb_L8 (GC0308_Hb & 0x0FF)
#define GC0308_Hb_H4 (GC0308_Hb >> 8)

#define GC0308_Vb    (0x180) //(0x070) (Vb+VWv)*rowtime = AntiTime
#define GC0308_Vb_L8 (GC0308_Vb & 0x0FF)
#define GC0308_Vb_H4 (GC0308_Vb >> 8)

#define GC0308_Exp    (488 + GC0308_Vb)
#define GC0308_Exp_L8 (GC0308_Exp / 16 & 0x0FF)
#define GC0308_Exp_H4 (GC0308_Exp / 16 >> 8)

#define GC0308_ShDelay (0x42)

#define GC0308_WIDTH  640
#define GC0308_HEIGHT 480

#define GC0308_HW (640 + 8)
#define GC0308_VW (480 + 8)

#define GC0308_RowTime  ((GC0308_Hb + GC0308_ShDelay + GC0308_HW + 4) / 21) // 34us
#define GC0308_AntiTime (1000000 / 100)                                     // 10000us 50Hz

#define GC0308_Step    (GC0308_Exp / 64)
#define GC0308_Step_L8 (GC0308_Step & 0x0FF)
#define GC0308_Step_H4 (GC0308_Step >> 8)

#define GC0308_Exp1    (GC0308_Exp / 16)
#define GC0308_Exp1_L8 (GC0308_Exp1 & 0x0FF)
#define GC0308_Exp1_H4 (GC0308_Exp1 >> 8)
#define GC0308_Exp2    (GC0308_Exp / 4)
#define GC0308_Exp2_L8 (GC0308_Exp2 & 0x0FF)
#define GC0308_Exp2_H4 (GC0308_Exp2 >> 8)
#define GC0308_Exp3    (GC0308_Exp / 2)
#define GC0308_Exp3_L8 (GC0308_Exp3 & 0x0FF)
#define GC0308_Exp3_H4 (GC0308_Exp3 >> 8)
#define GC0308_Exp4    (GC0308_Exp / 1)
#define GC0308_Exp4_L8 (GC0308_Exp4 & 0x0FF)
#define GC0308_Exp4_H4 (GC0308_Exp4 >> 8)

#define GC0308_LSCEnable           0x01
#define GC0308_BadEnable           0x02
#define GC0308_NoiseEnable         0x04
#define GC0308_InterpolationEnable 0x08
#define GC0308_EdgeEnable          0x10
#define GC0308_CCEnable            0x20
#define GC0308_GammaEnable         0x40
#define GC0308_BKSEnable           0x80
#define GC0308_BlockEnable1 \
    (GC0308_LSCEnable | GC0308_BadEnable | GC0308_NoiseEnable | GC0308_InterpolationEnable | GC0308_EdgeEnable | GC0308_GammaEnable | GC0308_BKSEnable | 0x00)
#define GC0308_AutoEEEnable (0x20)
#define GC0308_AutoEE1Start (0x10)
#define GC0308_AutoEE1Slope (0x01)

#define GC0308_FORMAT_YUYV     (0xA2)
#define GC0308_FORMAT_RGB565BE (0xA6) // only Big-Endian Mode Supported
#define GC0308_FORMAT_GRAY     (0xB1)

extern bool GC0308Constractor(CameraStruct* camera);

#endif
