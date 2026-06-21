#ifndef __TOUCH_BSP_H__
#define __TOUCH_BSP_H__

#include "Touch.h"

#if USE_COMPONENT_TOUCH_GOODIX
#include "GoodixTouch.h"
#endif
#if USE_COMPONENT_TOUCH_SITRONIX
#include "SitronixTouch.h"
#endif
#if USE_COMPONENT_TOUCH_HYNITRON
#include "HynitronTouch.h"
#endif
#if USE_COMPONENT_TOUCH_CHIPSEMI
#include "ChipsemiTouch.h"
#endif
#if USE_COMPONENT_TOUCH_ILITEK
#include "IlitekTouch.h"
#endif

#if CONFIG_BOARD == 10

#define TOUCH_I2C_PERIPH PeripheralI2C3
#define TOUCH_I2C_SCL_IO PC15
#define TOUCH_I2C_SDA_IO PC14
#define TOUCH_RST_IO     PC13
#define TOUCH_INT_IO     PE15

#define TOUCH_INT_EXTI EXTI15_10_

#elif CONFIG_BOARD == 11

#define TOUCH_I2C_PERIPH PeripheralI2C3
#define TOUCH_I2C_SCL_IO PA8
#define TOUCH_I2C_SDA_IO PC9
#define TOUCH_RST_IO     PE15
#define TOUCH_INT_IO     PC0

#define TOUCH_INT_EXTI EXTI0_

#endif

#ifndef TOUCH_I2C_PERIPH
#define TOUCH_I2C_PERIPH PeripheralNull
#endif

#ifndef TOUCH_I2C_SCL_IO
#define TOUCH_I2C_SCL_IO IONone
#endif

#ifndef TOUCH_I2C_SDA_IO
#define TOUCH_I2C_SDA_IO IONone
#endif

#ifndef TOUCH_I2C_SPEED
#define TOUCH_I2C_SPEED 800
#endif

#ifndef TOUCH_RST_IO
#define TOUCH_RST_IO IONone
#endif

#ifndef TOUCH_INT_IO
#define TOUCH_INT_IO IONone
#endif

extern TouchStruct TouchPort;

extern bool TouchSetup(uint16_t x, uint16_t y, bool reverseX, bool reverseY, bool switchXY);

#endif // __TOUCH_BSP_H__
