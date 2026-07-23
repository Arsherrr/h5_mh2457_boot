#ifndef __USB_H
#define __USB_H

int is_ota_mode(void);

/**
 * @brief  配置为 OTA 升级模式.
 * @note   USB VCP.
 */
void usb_ota_mode(void);

/**
 * @brief  配置为 OTA 升级模式.
 * @note   USB MSD.
 */
void usb_log_mode(void);

/**
 * @brief 退出 USB.
 */
void usb_stop(void);

#endif
