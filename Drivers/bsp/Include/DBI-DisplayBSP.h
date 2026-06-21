#ifndef __DBI_DISPLAY_BSP_H__
#define __DBI_DISPLAY_BSP_H__

#include "Display.h"

#if USE_COMPONENT_DISPLAY_DBI_ILI9806
#include "DBI-ILI9806.h"
#endif

// Select Board Configs of DPI
#if (CONFIG_BOARD == 10)
#define DBI_RESET_IO PA8
#define CONFIG_DBI_IO_LIST                                                             \
    ((IOEnum[]) {                                                                      \
        PG10, PG13, PI10, PG7,                         /* Control Pins: CS RS WR RD */ \
        PI3,  PI4,  PI5,  PI6, PI7,  PH13, PH14, PH15, /* Data Pins: DB0~DB7        */ \
        PI0,  PI1,  PI2,  PH9, PE15, PH11, PH12, PG6,  /* Data Pins: DB8~DB15       */ \
    })
#else
// No DBI Port
#define CONFIG_DBI_IO_LIST  ((IOEnum[]) {})
#define CONFIG_DBI_IO_COUNT 0
#endif

#ifndef DBI_RESET_IO
#define DBI_RESET_IO IONone
#endif

#ifndef DBI_TE_IO
#define DBI_TE_IO IONone
#endif

#ifndef CONFIG_DBI_IO_LIST
/*!
    @brief DBI IO List

    @param CS: #0 { PG10 }
    @param RS: #1 { PG13 }
    @param WR: #2 { PI10 }
    @param RD: #3 { PG7  }

    @param DB0:  { PI3  }
    @param DB1:  { PI4  }
    @param DB2:  { PI5  }
    @param DB3:  { PI6  }
    @param DB4:  { PI7  }
    @param DB5:  { PH13 }
    @param DB6:  { PH14 }
    @param DB7:  { PH15 }
    @param DB8:  { PI0  }
    @param DB9:  { PI1  }
    @param DB10: { PI2  }
    @param DB11: { PH9  }
    @param DB12: { PH10 }
    @param DB13: { PH11 }
    @param DB14: { PH12 }
    @param DB15: { PG6  }
*/
#endif

#ifndef CONFIG_DBI_IO_COUNT
#define CONFIG_DBI_IO_COUNT (sizeof(CONFIG_DBI_IO_LIST) / sizeof(IOEnum))
#endif

extern DisplayStruct DBIDisplay;

extern bool DBIDisplayConfig(DisplayStruct* display);

extern bool DBIDisplaySetup(uint32_t* buffer, uint32_t size);

#endif // __DBI_DISPLAY_BSP_H__
