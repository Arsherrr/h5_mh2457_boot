#ifndef __DSI_DISPLAY_BSP_H__
#define __DSI_DISPLAY_BSP_H__

// Includes: DSI Panel Headers
#if USE_COMPONENT_DISPLAY_DSI_NT35510
#include "DSI-NT35510.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST7701S
#include "DSI-ST7701S.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_ILI9881C
#include "DSI-ILI9881C.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_JD9852
#include "DSI-JD9852.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST7797
#include "DSI-ST7797.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_GC9C01
#include "DSI-GC9C01.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_HX8394F
#include "DSI-HX8394F.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_NV3049ET
#include "DSI-NV3049ET.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_GC9503V
#include "DSI-GC9503V.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_JD9365DA
#include "DSI-JD9365DA.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST7703
#include "DSI-ST7703.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST77922
#include "DSI-ST77922.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_ILI9806E
#include "DSI-ILI9806E.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_IL79451A
#include "DSI-IL79451A.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_CO5300
#include "DSI-CO5300.h"
#endif
#if USE_COMPONENT_DISPLAY_DSI_EK79007
#include "DSI-EK79007.h"
#endif

// Select Board Configs of DSI
#if CONFIG_BOARD == 11
#define DSI_RESET_IO PC15
#define DSI_TE_IO    PC13
#define DSI_BACKLIGHT_IO PA1
#endif

#ifndef DSI_RESET_IO
#define DSI_RESET_IO IONone
#endif

#ifndef DSI_TE_IO
#define DSI_TE_IO IONone
#endif

#if defined(DSI_VDDIO_IO) && !defined(DSI_VDDIO_ON)
#define DSI_VDDIO_ON(isOn) IOSet(DSI_VDDIO_IO, isOn)
#endif

#if defined(DSI_VDD_IO) && !defined(DSI_VDD_ON)
#define DSI_VDD_ON(isOn) IOSet(DSI_VDD_IO, isOn)
#endif

#if defined(DSI_BACKLIGHT_IO) && !defined(DSI_BACKLIGHT_ON)
#define DSI_BACKLIGHT_ON(isOn) IOSet(DSI_BACKLIGHT_IO, isOn)
#endif

// Display Port handle
extern DisplayStruct DSIDisplay;

// Functions
extern bool DSIDisplayConfig(DisplayStruct* display);
extern bool DSIDisplaySetup(uint32_t* buffer, uint32_t size);

#endif // __DSI_DISPLAY_BSP_H__
