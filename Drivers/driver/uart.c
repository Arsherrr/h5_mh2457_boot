#include "mh2457.h"
#include "io_config.h"
#include "uart.h"
#include "tmr.h"

#define GPS_RX_BUF_SIZE       (4048)
#define AT_RX_BUF_SIZE        (4048)
#define GPS_TX_BUF_SIZE       (1024)
#define AT_TX_BUF_SIZE        (1024)

uint8_t gps_dma_rx_buf[GPS_RX_BUF_SIZE];
uint8_t at_dma_rx_buf[AT_RX_BUF_SIZE];
uint8_t gps_tx_buf[GPS_TX_BUF_SIZE];
uint8_t at_tx_buf[AT_TX_BUF_SIZE];

typedef struct {
    uint8_t *buff;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
} uart_rx_t;

typedef struct {
    uint8_t busy;
    uint8_t *buff;
    uint16_t size;
    uint16_t len;
} uart_tx_t;

typedef struct {
    uint32_t baudrate;
    USART_TypeDef *uart;
    GPIO_TypeDef *tx_gpio;
    GPIO_TypeDef *rx_gpio;
    uint16_t tx_pin;
    uint16_t rx_pin;
    uint32_t tx_gpio_clk;
    uint32_t rx_gpio_clk;
    uint32_t periph_clk;
    uint8_t tx_source;
    uint8_t rx_source;
    uint8_t tx_af;
    uint8_t rx_af;
    uint32_t dma_clk;
    DMA_Stream_TypeDef *rx_stream;
    DMA_Stream_TypeDef *tx_stream;
    uint32_t rx_channel;
    uint32_t tx_channel;
    IRQn_Type uart_irqn;
    IRQn_Type tx_irqn;
    uint32_t tx_tcif;
    uint32_t tx_flags;
    uint8_t *rx_buf;
    uint16_t rx_buf_size;
    uart_rx_t *rx;
    uart_tx_t *tx;
} uart_cfg_t;

static uart_rx_t gps_rx = {.buff = gps_dma_rx_buf, .head = 0, .tail = 0, .size = GPS_RX_BUF_SIZE};
static uart_rx_t at_rx  = {.buff = at_dma_rx_buf,  .head = 0, .tail = 0, .size = AT_RX_BUF_SIZE};
static uart_tx_t gps_tx = {.busy = 0, .buff = gps_tx_buf, .size = GPS_TX_BUF_SIZE, .len = 0};
static uart_tx_t at_tx  = {.busy = 0, .buff = at_tx_buf,  .size = AT_TX_BUF_SIZE, .len = 0};

static const uart_cfg_t uart_cfg_map[] = {
    [UART_ID_GPS] = {
        .baudrate = GPS_UART_BAUDRATE,
        .uart = GPS_UART,
        .tx_gpio = GPS_UART_TX_GPIO,
        .rx_gpio = GPS_UART_RX_GPIO,
        .tx_pin = GPS_UART_TX_PIN,
        .rx_pin = GPS_UART_RX_PIN,
        .tx_gpio_clk = GPS_UART_TX_GPIO_CLK,
        .rx_gpio_clk = GPS_UART_RX_GPIO_CLK,
        .periph_clk = GPS_UART_PERIPH,
        .tx_source = GPS_UART_TX_SOURCE,
        .rx_source = GPS_UART_RX_SOURCE,
        .tx_af = GPS_UART_TX_AF,
        .rx_af = GPS_UART_RX_AF,
        .dma_clk = GPS_DMA_CLK,
        .rx_stream = GPS_DMA_RX_STREAM,
        .tx_stream = GPS_DMA_TX_STREAM,
        .rx_channel = GPS_DMA_RX_CHANNEL,
        .tx_channel = GPS_DMA_TX_CHANNEL,
        .uart_irqn = GPS_UART_IRQn,
        .tx_irqn = GPS_DMA_TX_IRQN,
        .tx_tcif = GPS_DMA_TX_TCIF,
        .tx_flags = GPS_DMA_TX_FLAGS,
        .rx_buf = gps_dma_rx_buf,
        .rx_buf_size = GPS_RX_BUF_SIZE,
        .rx = &gps_rx,
        .tx = &gps_tx,
    },
    [UART_ID_AT] = {
        .baudrate = AT_UART_BAUDRATE,
        .uart = AT_UART,
        .tx_gpio = AT_UART_TX_GPIO,
        .rx_gpio = AT_UART_RX_GPIO,
        .tx_pin = AT_UART_TX_PIN,
        .rx_pin = AT_UART_RX_PIN,
        .tx_gpio_clk = AT_UART_TX_GPIO_CLK,
        .rx_gpio_clk = AT_UART_RX_GPIO_CLK,
        .periph_clk = AT_UART_PERIPH,
        .tx_source = AT_UART_TX_SOURCE,
        .rx_source = AT_UART_RX_SOURCE,
        .tx_af = AT_UART_TX_AF,
        .rx_af = AT_UART_RX_AF,
        .dma_clk = AT_DMA_CLK,
        .rx_stream = AT_DMA_RX_STREAM,
        .tx_stream = AT_DMA_TX_STREAM,
        .rx_channel = AT_DMA_RX_CHANNEL,
        .tx_channel = AT_DMA_TX_CHANNEL,
        .uart_irqn = AT_UART_IRQn,
        .tx_irqn = AT_DMA_TX_IRQN,
        .tx_tcif = AT_DMA_TX_TCIF,
        .tx_flags = AT_DMA_TX_FLAGS,
        .rx_buf = at_dma_rx_buf,
        .rx_buf_size = AT_RX_BUF_SIZE,
        .rx = &at_rx,
        .tx = &at_tx,
    },
};

static const uart_cfg_t *uart_get_cfg(uint8_t uart_id)
{
    if (uart_id >= (sizeof(uart_cfg_map) / sizeof(uart_cfg_map[0]))) {
        return NULL;
    }
    return &uart_cfg_map[uart_id];
}

static void ring_write(uart_rx_t *r, const uint8_t *data, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++) {
        uint16_t next = (r->head + 1U) % r->size;
        if (next == r->tail) {
            break;
        }
        r->buff[r->head] = data[i];
        r->head = next;
    }
}

static uint16_t ring_read(uart_rx_t *r, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++) {
        if (r->tail == r->head) {
            break;
        }
        buf[i] = r->buff[r->tail];
        r->tail = (r->tail + 1U) % r->size;
    }
    return i;
}

static void uart_config(uint8_t uart_id, uint32_t baudrate)
{
    GPIO_InitTypeDef gpio;
    USART_InitTypeDef usart;
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);

    if (cfg == NULL) {
        return;
    }

    PeripheralEnable(cfg->tx_gpio_clk, true);
    PeripheralEnable(cfg->rx_gpio_clk, true);
    PeripheralEnable(cfg->periph_clk, true);

    USART_Cmd(cfg->uart, DISABLE);

    GPIO_PinAFConfig(cfg->tx_gpio, cfg->tx_source, cfg->tx_af);
    GPIO_PinAFConfig(cfg->rx_gpio, cfg->rx_source, cfg->rx_af);

    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    gpio.GPIO_Pin = cfg->tx_pin;
    GPIO_Init(cfg->tx_gpio, &gpio);

    gpio.GPIO_Pin = cfg->rx_pin;
    GPIO_Init(cfg->rx_gpio, &gpio);

    usart.USART_BaudRate = baudrate;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(cfg->uart, &usart);
    USART_Cmd(cfg->uart, ENABLE);
}

static void uart_dma_config(uint8_t uart_id)
{
    DMA_InitTypeDef dma;
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);

    if (cfg == NULL) {
        return;
    }

    PeripheralEnable(cfg->dma_clk, true);
    DMA_DeInit(cfg->rx_stream);
    DMA_DeInit(cfg->tx_stream);

    while (DMA_GetCmdStatus(cfg->rx_stream) != DISABLE) {}
    while (DMA_GetCmdStatus(cfg->tx_stream) != DISABLE) {}

    dma.DMA_PeripheralBaseAddr = (uint32_t)&(cfg->uart->DR);
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_Priority = DMA_Priority_High;
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    dma.DMA_Channel = cfg->rx_channel;
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_Memory0BaseAddr = (uint32_t)cfg->rx_buf;
    dma.DMA_BufferSize = cfg->rx_buf_size;
    dma.DMA_Mode = DMA_Mode_Circular;
    DMA_Init(cfg->rx_stream, &dma);

    dma.DMA_Channel = cfg->tx_channel;
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    dma.DMA_Memory0BaseAddr = 0;
    dma.DMA_BufferSize = 0;
    dma.DMA_Mode = DMA_Mode_Normal;
    DMA_Init(cfg->tx_stream, &dma);

    USART_DMACmd(cfg->uart, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(cfg->rx_stream, ENABLE);
}

static void uart_int_config(uint8_t uart_id)
{
    NVIC_InitTypeDef nvic;
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);

    if (cfg == NULL) {
        return;
    }

    DMA_ITConfig(cfg->tx_stream, DMA_IT_TC, ENABLE);
    USART_ITConfig(cfg->uart, USART_IT_IDLE, ENABLE);

    nvic.NVIC_IRQChannel = cfg->tx_irqn;
    nvic.NVIC_IRQChannelPreemptionPriority = 6;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    nvic.NVIC_IRQChannel = cfg->uart_irqn;
    nvic.NVIC_IRQChannelPreemptionPriority = 6;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

static void uart_deinit(uint8_t uart_id)
{
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);

    if (cfg == NULL) {
        return;
    }

    DMA_Cmd(cfg->rx_stream, DISABLE);
    DMA_Cmd(cfg->tx_stream, DISABLE);
    USART_ITConfig(cfg->uart, USART_IT_IDLE, DISABLE);
    DMA_ITConfig(cfg->tx_stream, DMA_IT_TC, DISABLE);
    USART_DMACmd(cfg->uart, USART_DMAReq_Rx | USART_DMAReq_Tx, DISABLE);
    USART_Cmd(cfg->uart, DISABLE);
}

void uarts_init(void)
{
    uart_init(UART_ID_GPS, uart_cfg_map[UART_ID_GPS].baudrate);
    uart_dma_config(UART_ID_GPS);
    uart_int_config(UART_ID_GPS);
    uart_init(UART_ID_AT, uart_cfg_map[UART_ID_AT].baudrate);
    uart_dma_config(UART_ID_AT);
    uart_int_config(UART_ID_AT);
}

void uart_init(uint8_t uart_id, uint32_t baudrate)
{
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);

    if (cfg == NULL) {
        return;
    }

    uart_deinit(uart_id);
    uart_config(uart_id, baudrate);
    uart_dma_config(uart_id);
    uart_int_config(uart_id);
}

void uart_write(uint8_t uart_id, uint8_t *data, uint16_t len)
{
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);
    uart_tx_t *tx;
    DMA_Stream_TypeDef *stream;
    uint32_t flag;

    if ((cfg == NULL) || (data == NULL) || (len == 0U)) {
        return;
    }

    tx = cfg->tx;
    stream = cfg->tx_stream;
    flag = cfg->tx_flags;

    while (tx->busy) {
        /* (优化)超时直接发/返回. */
        ;
    }

    if (len > tx->size) {
        len = tx->size;
    }

    tx->busy = 1;
    tx->len = len;
    memcpy(tx->buff, data, len);

    DMA_Cmd(stream, DISABLE);
    while (DMA_GetCmdStatus(stream) != DISABLE) {}

    DMA_ClearFlag(stream, flag);
    stream->M0AR = (uint32_t)tx->buff;
    DMA_SetCurrDataCounter(stream, len);
    DMA_Cmd(stream, ENABLE);
}

void uart_write_polling(uint8_t uart_id, uint8_t *data, uint16_t len)
{
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);
    USART_TypeDef *uart;
    uint16_t i;

    if ((cfg == NULL) || (data == NULL) || (len == 0U)) {
        return;
    }

    uart = cfg->uart;
    for (i = 0; i < len; i++) {
        while (USART_GetFlagStatus(uart, USART_FLAG_TXE) == RESET) {
            ;
        }
        USART_SendData(uart, data[i]);
    }

    while (USART_GetFlagStatus(uart, USART_FLAG_TC) == RESET) {
        ;
    }
}

static uart_rx_t *uart_get_rx(uint8_t uart_id)
{
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);
    return (cfg == NULL) ? NULL : cfg->rx;
}

uint16_t uart_rx_available(uint8_t uart_id)
{
    uart_rx_t *r = uart_get_rx(uart_id);
    uint16_t available;

    if (r == NULL) {
        return 0;
    }

    __disable_irq();
    if (r->head >= r->tail) {
        available = (uint16_t)(r->head - r->tail);
    } else {
        available = (uint16_t)(r->size - (r->tail - r->head));
    }
    __enable_irq();

    return available;
}

void uart_rx_flush(uint8_t uart_id)
{
    uart_rx_t *r = uart_get_rx(uart_id);

    if (r == NULL) {
        return;
    }

    __disable_irq();
    r->tail = r->head;
    __enable_irq();
}

uint16_t uart_read(uint8_t uart_id, uint8_t *buf, uint16_t len)
{
    uart_rx_t *r;
    uint16_t read_len;

    if ((buf == NULL) || (len == 0U)) {
        return 0;
    }

    r = uart_get_rx(uart_id);
    if (r == NULL) {
        return 0;
    }

    __disable_irq();
    read_len = ring_read(r, buf, len);
    __enable_irq();

    return read_len;
}

uint16_t uart_read_wait(uint8_t uart_id, uint8_t *buf, uint16_t len, uint32_t timeout_ms)
{
    uint32_t start = get_time_tick();
    uint32_t timeout_tick = timeout_ms;
    uint16_t total = 0;

    if ((buf == NULL) || (len == 0U)) {
        return 0;
    }

    while ((get_time_tick() - start) < timeout_tick) {
       uint16_t available = uart_rx_available(uart_id);
       if (available > 0U) {
           return uart_read(uart_id, buf, len);
       } else {
            delay_ms(5);
       }
    }

    return 0;
}

static void uart_dma_tx_isr(uint8_t uart_id)
{
    const uart_cfg_t *cfg = uart_get_cfg(uart_id);

    if (cfg == NULL) {
        return;
    }

    if (DMA_GetITStatus(cfg->tx_stream, cfg->tx_tcif) != RESET) {
        DMA_ClearITPendingBit(cfg->tx_stream, cfg->tx_tcif);
        cfg->tx->busy = 0;
    }
}

void DMA2_Stream7_IRQHandler(void)
{
    uart_dma_tx_isr(UART_ID_GPS);
}

void DMA1_Stream3_IRQHandler(void)
{
    uart_dma_tx_isr(UART_ID_AT);
}

static void uart_rx_to_ringbuf(uart_rx_t *r, uint8_t *dma_buf, uint16_t buf_size, DMA_Stream_TypeDef *stream)
{
    static uint16_t gps_old_pos = 0;
    static uint16_t at_old_pos = 0;
    uint16_t *old_pos = (stream == GPS_DMA_RX_STREAM) ? &gps_old_pos : &at_old_pos;
    uint16_t dma_pos = (uint16_t)(buf_size - DMA_GetCurrDataCounter(stream));

    if (dma_pos != *old_pos) {
        if (dma_pos > *old_pos) {
            ring_write(r, &dma_buf[*old_pos], dma_pos - *old_pos);
        } else {
            ring_write(r, &dma_buf[*old_pos], buf_size - *old_pos);
            ring_write(r, &dma_buf[0], dma_pos);
        }
        *old_pos = dma_pos;
    }
}

void GPS_UART_IRQHandler(void)
{
    if (USART_GetITStatus(GPS_UART, USART_IT_IDLE) != RESET) {
        volatile uint32_t temp;
        temp = GPS_UART->SR;
        temp = GPS_UART->DR;
        (void)temp;
        uart_rx_to_ringbuf(&gps_rx, gps_dma_rx_buf, GPS_RX_BUF_SIZE, GPS_DMA_RX_STREAM);
    }
}

void AT_UART_IRQHandler(void)
{
    if (USART_GetITStatus(AT_UART, USART_IT_IDLE) != RESET) {
        volatile uint32_t temp;
        temp = AT_UART->SR;
        temp = AT_UART->DR;
        (void)temp;
        uart_rx_to_ringbuf(&at_rx, at_dma_rx_buf, AT_RX_BUF_SIZE, DMA1_Stream1);
    }
}
