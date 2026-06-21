#ifndef __DISPLAY_BSP_H__
#define __DISPLAY_BSP_H__

#include "Display.h"
#include "DSI-DisplayBSP.h"
#include "DPI-DisplayBSP.h"
#include "DBI-DisplayBSP.h"

extern DisplayStruct* DisplaySelectSetup(uint32_t* buffer, uint32_t size);

#endif // __DISPLAY_BSP_H__
