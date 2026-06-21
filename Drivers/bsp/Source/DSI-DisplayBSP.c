#include "DSI-DisplayBSP.h"

DisplayStruct DSIDisplay;

DSIStruct DSIHandle;

__WEAK bool DSIDisplayConfig(DisplayStruct* display) {
    return true;
}

static bool DSIPanelConstractor(DisplayStruct* display) {
#if USE_COMPONENT_DISPLAY_DSI_ILI9806E
    if (ILI9806EConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_NT35510
    if (NT35510Constractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST7701S
    if (ST7701SConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_ILI9881C
    if (ILI9881CConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_JD9852
    if (JD9852Constractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST7797
    if (ST7797Constractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_GC9C01
    if (GC9C01Constractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_HX8394F
    if (HX8394FConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_NV3049ET
    if (NV3049ETConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_GC9503V
    if (GC9503VConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_JD9365DA
    if (JD9365DAConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST7703
    if (ST7703Constractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_ST77922
    if (ST77922Constractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_IL79451A
    if (IL79451AConstractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_CO5300
    if (CO5300Constractor(display))
        return true;
#endif
#if USE_COMPONENT_DISPLAY_DSI_EK79007
    if (EK79007Constractor(display))
        return true;
#endif
    return false;
}

bool DSIDisplaySetup(uint32_t* buffer, uint32_t size) {
    // IO config
#ifdef DSI_VDDIO_IO
    IOSetup(DSI_VDDIO_IO, IO_DEFAULT_OUTPUT_CONFIG);
    DSI_VDDIO_ON(true);
#endif

#ifdef DSI_VDD_IO
    IOSetup(DSI_VDD_IO, IO_DEFAULT_OUTPUT_CONFIG);
    DSI_VDD_ON(true);
#endif

#ifdef DSI_BACKLIGHT_IO
    IOSetup(DSI_BACKLIGHT_IO, IO_DEFAULT_OUTPUT_CONFIG);
    DSI_BACKLIGHT_ON(false);
#endif

    DisplayStruct* display = &DSIDisplay;
    DisplayDSIConstractor(display, &DSIHandle);
    display->ResetIO         = DSI_RESET_IO;
    display->TearingEffectIO = DSI_TE_IO;

    // Use the default startup sequence to get DisplayID
    DisplayStartupSequence(display, true, true, 20, 20, 200);
    display->ID = DisplayDSIReadID(display, NULL);
    // return false if DSI is not present
    if (display->ID == 0xFFFFFFFF)
        return false;

    // Panel constractor
    if (!DSIPanelConstractor(display))
        return false;

    // custom config
    if (!DSIDisplayConfig(display))
        return false;

    DisplayLayerConstractor(display, buffer, size);

    return display->Init(display);
}
