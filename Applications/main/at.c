#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "tmr.h"
#include "crc.h"

#define UART_ID  ( UART_ID_AT )

#define RING_BUF_SIZE    128    // 接收环形缓冲区
#define RECV_BUF_SIZE    128    // 单帧接收缓冲
#define SEND_BUF_SIZE    128    // 发送缓冲
#define FRAME_SIZE       128

#define MSG_NOTIFY_UPGRADE 0xF002  // 通知进入升级
#define MSG_ENTER_APP      0x0310  // 进入 APP

/* 应答偏移量 */
#define MSG_RESP_OFFSET  0x100

/* ===================== 协议打包（升级协议，小端长度） ===================== */
/**
 * 打包升级协议帧
 * @param msg_type  消息类型
 * @param frame     输出帧缓冲区
 * @param data      协议数据
 * @param data_len  数据长度（可为0，表示空数据）
 * @return 帧总长度
 */
static uint16_t at_pack(uint16_t msg_type, uint8_t *frame,
                              uint8_t *data, uint16_t data_len)
{
    uint16_t pos = 0;
    uint16_t crc;
    uint16_t total_len;

    /* 1. 协议头 @@@@ */
    memcpy(frame + pos, "@@", 2);
    pos += 4;

    /* 2. 消息类型 (大端) */
    frame[pos++] = (msg_type >> 8) & 0xFF;
    frame[pos++] = (msg_type >> 0) & 0xFF;

    /* 3. 协议数据 */
    if (data_len > 0 && data != NULL) {
        memcpy(frame + pos, data, data_len);
        pos += data_len;
    }

    /* 4. CRC + \r\n 共 4 字节 */
    total_len = pos + 4;

    /* 5. 填写长度 (小端!) — 注意位置: frame[2], frame[3] */
    frame[2] = (total_len >> 0) & 0xFF;   // 低字节在前
    frame[3] = (total_len >> 8) & 0xFF;   // 高字节在后

    /* 6. CRC16 (小端) */
    crc = crc16(frame, pos, 0);
    frame[pos++] = (crc >> 0) & 0xFF;
    frame[pos++] = (crc >> 8) & 0xFF;

    /* 7. 协议尾 \r\n */
    frame[pos++] = '\r';
    frame[pos++] = '\n';

    return pos;
}

static uint8_t frame[FRAME_SIZE];
static uint8_t step = 0;
static uint8_t cnt  = 0;

static void frame_parse_init(void)
{
    step = 0;
    cnt  = 0;
}

static inline int frame_parse(uint8_t byte)
{
    static uint8_t len = 0;

    switch (step) {
    case 0:
    case 1:
        /* $$ */
        if (byte == '$') {
            frame[cnt++] = byte;
            ++step;
        } else {
            step = 0;
            cnt  = 0;
        }
        break;

    case 2:
    case 3:
        /* LEN. */
        frame[cnt++] = byte;
        ++step;

        if (step == 4) {
            /* 判断长度的合法性. */
            len = (frame[3] << 8) + frame[2];
            if (len > FRAME_SIZE) {
                step = 0;
                cnt  = 0;
            }
        }
        break;

    case 4:
        frame[cnt++] = byte;
        if (cnt == len)
        {
            if ((byte != '\n') || (frame[cnt - 2] != '\r')) return -2;
            
            uint16_t crc;
            uint32_t __len;
            crc = crc16(frame, cnt - 4, 0);
            if (crc == ((frame[cnt - 3] << 8) + frame[cnt - 4])) {
                __len = cnt;
                step = 0;
                cnt  = 0;
                return __len;
            } else {
                return -2;
            }
        }
        break;

    default:
        step = 0;
        cnt  = 0;
    }

    return 0;
}

/**
 * 发送命令并等待应答
 * @param msg_type  请求消息类型
 * @param send_data 发送数据
 * @param send_len  发送数据长度
 * @param recv_data 接收数据缓冲区
 * @param timeout_ms 超时(ms)
 * @return >=0 应答数据长度，-1 超时，-2 CRC错，-3 类型不匹配
 */
static int16_t at_send_read(uint16_t msg_type,
                            uint8_t *send_data, uint16_t send_len,
                            uint8_t *recv_data, uint32_t timeout_ms)
{
    static uint8_t send_frame[SEND_BUF_SIZE];
    static uint8_t recv_frame[RECV_BUF_SIZE];
    uint16_t send_total;
    uint32_t elapsed = 0;
    const uint32_t poll_interval = 10;  /* 10ms 轮询. */
    uint16_t resp_type = msg_type + MSG_RESP_OFFSET;

    /* 清空接收缓冲 */
    while (uart_read(UART_ID, recv_frame, RECV_BUF_SIZE) > 0) {}

    /* 打包并发送 */
    send_total = at_pack(msg_type, send_frame, send_data, send_len);
    uart_write(UART_ID, send_frame, send_total);

    frame_parse_init();

    /* 等待应答 */
    while (elapsed < timeout_ms) {
        int ret;
        uint16_t n = uart_read(UART_ID, recv_frame, RECV_BUF_SIZE);
        if (n) {
            for (uint16_t i = 0; i < n; i++) {
                ret = frame_parse(recv_frame[i]);
                if (ret > 0) {
                    if ((frame[4] << 8) + frame[5] != resp_type) return -3;
                    memcpy(recv_data, frame, ret);
                    return ret;
                } else if (ret < 0) {
                    return ret;
                }
            }
        }

        delay_ms(poll_interval);
        elapsed += poll_interval;
    }

    printf("[AT] Received timeout.\n");
    return -1;
}

/**
 * @brief 进入 BOOT.
 */
int16_t at_enter_boot(void)
{
    int16_t ret = at_send_read(MSG_NOTIFY_UPGRADE, NULL, 0, NULL, 100);
    return ret;
    if (ret >= 0) {
        printf("[AT] Enter boot successfully.\n");
    } else {
        printf("[AT] Enter boot failed, %d\n", ret);
    }
}

/**
 * @brief 进入 APP.
 */
void at_enter_app(void)
{
    int16_t ret = at_send_read(MSG_ENTER_APP, NULL, 0, NULL, 100);
    if (ret >= 0) {
        printf("[AT] Enter App successfully.\n");
    } else {
        printf("[AT] Enter App failed, %d\n", ret);
    }
}
