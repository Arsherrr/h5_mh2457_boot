#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mh2457.h"

#define UART_ID_GPS (0U)
#define UART_ID_AT  (1U)

void uarts_init(void);
void uart_init(uint8_t uart_id, uint32_t baudrate);
void uart_write(uint8_t uart_id, uint8_t *data, uint16_t len);
void uart_write_polling(uint8_t uart_id, uint8_t *data, uint16_t len);
uint16_t uart_read(uint8_t uart_id, uint8_t *buf, uint16_t len);
uint16_t uart_rx_available(uint8_t uart_id);
void uart_rx_flush(uint8_t uart_id);
uint16_t uart_read_wait(uint8_t uart_id, uint8_t *buf, uint16_t len, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif
#endif
