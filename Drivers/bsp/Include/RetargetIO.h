#ifndef __RETARGET_IO_H__
#define __RETARGET_IO_H__

#include <stdio.h>
#include "mh2457.h"

#define CONFIG_RETARGETIO_DEFAULT_SERIAL PeripheralUSART1, PA9, PA10

extern void RetargetIOSetup(PeripheralEnum peripheral, IOEnum txIO, IOEnum rxIO);

#endif // __RETARGET_IO_H__
