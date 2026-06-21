#ifndef __USB_TARGET_CONFIG_MH2457_H__
#define __USB_TARGET_CONFIG_MH2457_H__

#define USE_USBESL_CORE_ZOFFY 1

// Zoffy Features
#define FEATURE_USBCORE_ZOFFY_DYNAMIC_DMA_MODE 1

#define FEATURE_USBCORE_ZOFFY_MAX_EP_COUNT (6)

#include "mh2457.h"

// USB Core Base Address
#define USB_ZOFFY1_BASE 0x50000000
#define USB_ZOFFY1_IRQ  USB1_IRQn

#define USB_ZOFFY_BASE USB_ZOFFY1_BASE
#define USB_ZOFFY_IRQ  USB_ZOFFY1_IRQ

#ifndef USB_IO_CONFIG_DATA
#define USB_IO_CONFIG_DATA MakeIOConfig(IOModeAlternate, GPIO_AF_USB, IOPullNone, IOSpeedHigh, IODriveHigh)
#endif

#ifndef USB_IO_CONFIG_ID
#define USB_IO_CONFIG_ID MakeIOConfig(IOModeAlternate, GPIO_AF_USB, IOPullUp, IOSpeedMedium, IODriveMedium)
#endif

#ifndef USB_IO_CONFIG_VBUS
#define USB_IO_CONFIG_VBUS MakeIOConfig(IOModeInput, 0, IOPullNone, IOSpeedMedium, IODriveMedium)
#endif

#endif
