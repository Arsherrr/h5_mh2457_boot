#include "DBI-DisplayBSP.h"

DisplayStruct DBIDisplay;

__WEAK bool DBIDisplayConfig(DisplayStruct* display) {
    return true;
}

static bool DBIPanelConstractor(DisplayStruct* display) {
#if USE_COMPONENT_DISPLAY_DBI_ILI9806
    if (ILI9806Constractor(display))
        return true;
#endif
    return false;
}

static IOEnum dbiIOList[] = CONFIG_DBI_IO_LIST;

bool DBIDisplaySetup(uint32_t* buffer, uint32_t size) {

    DisplayStruct* display = &DBIDisplay;
    DisplayDBIConstractor(display);
    display->ResetIO         = DBI_RESET_IO;
    display->TearingEffectIO = DBI_TE_IO;

    display->IOCount = CONFIG_DBI_IO_COUNT;
    display->IOList  = dbiIOList;

    // Panel constractor
    if (!DBIPanelConstractor(display))
        return false;

    if (!DBIDisplayConfig(display))
        return false;

    DisplayLayerConstractor(display, buffer, size);

    return display->Init(display);
}
