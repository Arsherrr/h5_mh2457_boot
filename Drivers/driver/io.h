#ifndef __IO_H
#define __IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "io_config.h"
#include "mh2457.h"

#define GPS_ON_SET(stat)  IOSet(GPS_ON_IO, stat)
#define GPS_EN_SET(stat)  IOSet(GPS_EN_IO, stat)
#define MCU_EN_SET(stat)  IOSet(MCU_EN_IO, stat)
#define PWR_CTR_SET(stat) IOSet(PWR_CTR_IO, stat)
#define PR_CTR_SET(stat)  IOSet(PR_CTR_IO, stat)

void io_init(void);

void usb_disconnect(void);

#ifdef __cplusplus
}
#endif

#endif
