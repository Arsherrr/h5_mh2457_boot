#ifndef __QMI8658_SPI_H
#define __QMI8658_SPI_H

#include "mh2457.h"

/**
 * @brief  qmi8658 芯片上电时, CS 低电平选择 SPI 模式.
 * @param  None
 * @retval 0 成功, 其他 失败.
 */
int qmi8658_spi_mode_init(void);

/**
 * @brief  qmi8658 驱动初始化.
 * @param  None
 * @retval 0 成功, 其他 失败.
 */
int qmi8658_spi_init(void);

/**
 * @brief SPI传输函数
 * @param tx_buf: 发送缓冲区
 * @param rx_buf: 接收缓冲区
 * @param len: 传输长度
 * @return 成功返回 0，失败返回 -1
 */
int qmi8658_spi_transfer(uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len);

#endif
