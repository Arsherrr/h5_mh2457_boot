#ifndef __DSI_ST7797_H__
#define __DSI_ST7797_H__

#include "DisplayDSI.h"

// Line Time = 800 * 50 ns ( 20M ) = 40us
#define ST7797_HSA  80
#define ST7797_HBP  200
#define ST7797_HACT 320
#define ST7797_HFP  200

// Frame Time = 500 * 40 us = 20ms
#define ST7797_VSA  8
#define ST7797_VBP  6
#define ST7797_VACT 480
#define ST7797_VFP  6

extern bool ST7797Constractor(DisplayStruct* display);

#endif // __DSI_ST7797_H__
