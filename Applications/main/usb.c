#include "mh2457.h"
#include "USBBSP.h"
#include "io.h"
#include "log.h"
#include "lvgl.h"
#include "ui_ota.h"

extern const lv_image_dsc_t update_bg_dark;

extern int log_cache_create(void);
extern void log_cache_destroy(void);
extern int jpg_init(lv_image_dsc_t * desc, lv_draw_buf_t *decoded);

lv_draw_buf_t update_bg_buf;

static int ota_mode = 0;

int is_ota_mode(void)
{
    return ota_mode;
}

/**
 * @brief  配置为 OTA 升级模式.
 * @note   USB VCP.
 */
void usb_ota_mode(void)
{
    jpg_init((lv_image_dsc_t *)&update_bg_dark, &update_bg_buf);

    /* UI 初始化. */
    ui_ota_init();

    /* 刷新一次. */
    lv_timer_handler();

    QSPI_Init(NULL);
    QSPI_SetLatency(0);

    usb_disconnect();
    USBSetup(USE_COMPONENT_USBVCP);

    ota_mode = 1;
}

/**
 * @brief  配置为 OTA 升级模式.
 * @note   USB MSD.
 */
void usb_log_mode(void)
{
    /* 刷新一次. */
    lv_timer_handler();

    log_cache_create();

    usb_disconnect();
    USBSetup(USE_COMPONENT_USBMSD);
}

/**
 * @brief 退出 USB.
 */
void usb_stop(void)
{
    USBStop();
    log_cache_destroy();
}
