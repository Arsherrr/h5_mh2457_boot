#ifndef __DSI_NT35510_H__
#define __DSI_NT35510_H__

#include "DisplayDSI.h"

#if CONFIG_NT35510_LANDSCAPE

#define NT35510_HSA  2
#define NT35510_HBP  2
#define NT35510_HACT 800
#define NT35510_HFP  2

#define NT35510_VSA  16
#define NT35510_VBP  320
#define NT35510_VACT 480
#define NT35510_VFP  16

#else

#define NT35510_HSA  8
#define NT35510_HBP  160
#define NT35510_HACT 480
#define NT35510_HFP  0

#define NT35510_VSA  8
#define NT35510_VBP  32
#define NT35510_VACT 800
#define NT35510_VFP  0

#endif

extern bool NT35510Constractor(DisplayStruct* display);

#endif // __DSI_NT35510_H__
