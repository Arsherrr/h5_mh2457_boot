// The file contains global config definitons of the project.
// It is force included to MDK4/5 projects by append `--preinclude global-config.h`(ARMCC) or `-include global-config.h`(ARMClang) to `C/C++ - Misc Controls`
// All the global config defintions can be set here, such as `USE_FULL_ASSERT`, `USE_STDPERIPH_DRIVER`, etc...
// Note: The `C/C++ - Preprocessor Symbols - Define` should not be set if this file has been included!

// Definitions of Lvgl-Widgets
//#define CONFIG_BOARD 10     //MH2457_QFN100_EVB
#define CONFIG_BOARD 11       //MH2457_QFN88_EVB

// Definitions of lvgl-demo
#define USE_OSS_LVGL_DEMO 0
#define USE_OSS_LVGL_DEMO_WIDGETS 0

// Definitions of lvgl-core
#define USE_OSS_LVGL 1

// Definitions of rtos-freertos
#define USE_RTOS_FREERTOS 0
#define USE_RTOS_FREERTOS_HEAP4 0

#define USE_RTOS_RTTHREAD 0

#define CONFIG_LVGL_GAU_CACHE_SIZE (512 * 1024) /* 不在这里计算, 只是作为开启的宏. */

#define CONFIG_LVGL_PORT_SUPPORT_ROTATION 1

#define LV_BUF_SIZE (480 * 854 * 6)

// Definitions of component
#define USE_COMPONENT_DISPLAY 12
// #define USE_COMPONENT_DISPLAY_DBI 14
//#define USE_COMPONENT_DISPLAY_DBI_ILI9806 17
#define USE_COMPONENT_DISPLAY_DPI 18
#define USE_COMPONENT_DISPLAY_DSI 21
//#define USE_COMPONENT_DISPLAY_DSI_CO5300 24
//#define USE_COMPONENT_DISPLAY_DSI_EK79007 25
//#define USE_COMPONENT_DISPLAY_DSI_GC9503V 26
//#define USE_COMPONENT_DISPLAY_DSI_GC9C01 27
//#define USE_COMPONENT_DISPLAY_DSI_HX8394F 28
//#define USE_COMPONENT_DISPLAY_DSI_IL79451A 29
//#define USE_COMPONENT_DISPLAY_DSI_ILI9806E 30
//#define USE_COMPONENT_DISPLAY_DSI_ILI9881C 31
#define USE_COMPONENT_DISPLAY_DSI_JD9365DA 32
//#define USE_COMPONENT_DISPLAY_DSI_JD9852 33
//#define USE_COMPONENT_DISPLAY_DSI_NT35510 34
//#define USE_COMPONENT_DISPLAY_DSI_NV3049ET 35
//#define USE_COMPONENT_DISPLAY_DSI_ST7701S 36
//#define USE_COMPONENT_DISPLAY_DSI_ST7703 37
//#define USE_COMPONENT_DISPLAY_DSI_ST77922 38
//#define USE_COMPONENT_DISPLAY_DSI_ST7797 39
#define USE_COMPONENT_GAUPORT 41
// #define USE_COMPONENT_I2CPORT 42
// #define USE_COMPONENT_RETARGETIO 44
//#define USE_COMPONENT_TOUCH 49
//#define USE_COMPONENT_TOUCH_CHIPSEMI 52
// #define USE_COMPONENT_TOUCH_GOODIX 53
//#define USE_COMPONENT_TOUCH_HYNITRON 54
//#define USE_COMPONENT_TOUCH_ILITEK 55
//#define USE_COMPONENT_TOUCH_SITRONIX 56
#define USE_COMPONENT_USBBSP 67
#define USE_COMPONENT_USBVCP 72

#define MOVE_TO_RAM __attribute__((section("RAM_CODE")))
