/**
 * at.c — 从MCU升级协议独立测试代码
 *
 * 用途：通过 UART1 发送 0xF006 命令让从MCU停止CAN活动，
 *       以解决 USB(DP/DM) 与 CAN 引脚复用冲突问题。
 *
 * 协议帧格式（升级协议，小端长度）：
 *   @@@@  + 长度(2B LE) + 消息类型(2B BE) + 数据(不定长) + CRC16(2B LE) + \r\n
 *
 * 应答消息类型 = 请求消息类型 + 0x100
 *   0xF001 → 0xF101  查询版本
 *   0xF002 → 0xF102  通知升级
 *   0xF005 → 0xF105  升级完成
 *   0xF006 → 0xF106  停止通信/进入升级前导
 *
 * 硬件接口（由用户提供）：
 *   void uart_write(uint8_t uart_id, uint8_t *data, uint16_t len);
 *   uint16_t uart_read(uint8_t uart_id, uint8_t *buf, uint16_t len);  // 非阻塞
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* ===================== 用户需要实现的接口 ===================== */
extern void     uart_write(uint8_t uart_id, uint8_t *data, uint16_t len);
extern uint16_t uart_read(uint8_t uart_id, uint8_t *buf, uint16_t len);
extern void     delay_ms(uint32_t ms);

#define UART_ID  1
#define UART_BAUDRATE  38400   // 原代码波特率

/* ===================== 协议常量 ===================== */
#define FRAME_HEADER    "@@"
#define FRAME_HEADER_LEN 4
#define FRAME_TAIL      "\r\n"
#define FRAME_TAIL_LEN   2
#define CRC_SIZE         2

#define RING_BUF_SIZE    4096    // 接收环形缓冲区
#define RECV_BUF_SIZE    2048    // 单帧接收缓冲
#define SEND_BUF_SIZE    4096    // 发送缓冲

/* ===================== 升级消息类型 ===================== */
#define MSG_QUERY_VER      0xF001  // 查询版本号
#define MSG_NOTIFY_UPGRADE 0xF002  // 通知进入升级
#define MSG_FILE_SIZE      0xF003  // 下发文件大小
#define MSG_SEND_DATA      0xF004  // 下发数据包
#define MSG_UPGRADE_DONE   0xF005  // 升级完成
#define MSG_ENTER_UPGRADE  0xF006  // 进入升级前导（停止通信）
#define MSG_GET_SN         0xF007  // 获取序列号

/* 应答偏移量 */
#define MSG_RESP_OFFSET  0x100

/* ===================== CRC16 查表 ===================== */
static const uint16_t fcstab[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/* ===================== CRC16 计算 ===================== */
static uint16_t crc_make(uint8_t *src, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    while (len-- > 0) {
        crc = ((crc >> 8) ^ fcstab[(crc ^ *src++) & 0x00FF]);
    }
    crc ^= 0xFFFF;
    return crc;
}

/* ===================== 协议打包（升级协议，小端长度） ===================== */
/**
 * 打包升级协议帧
 * @param msg_type  消息类型
 * @param frame     输出帧缓冲区
 * @param data      协议数据
 * @param data_len  数据长度（可为0，表示空数据）
 * @return 帧总长度
 */
static uint16_t upgrade_pack(uint16_t msg_type, uint8_t *frame,
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
    crc = crc_make(frame, pos);
    frame[pos++] = (crc >> 0) & 0xFF;
    frame[pos++] = (crc >> 8) & 0xFF;

    /* 7. 协议尾 \r\n */
    frame[pos++] = '\r';
    frame[pos++] = '\n';

    return pos;
}

/* ===================== 协议解包（升级协议） ===================== */
/**
 * 解包升级协议应答帧
 * @param expected_type 期望的消息类型（请求类型 + 0x100）
 * @param frame         接收的完整帧
 * @param data          输出：协议数据
 * @return >=0 数据长度，-1 帧太短，-2 CRC校验失败，-3 消息类型不匹配
 */
static int16_t upgrade_unpack(uint16_t expected_type, uint8_t *frame, uint8_t *data)
{
    uint16_t frame_len;
    uint16_t recv_type;
    uint16_t crc_expect, crc_actual;
    int16_t  data_len;

    /* 长度字段是 小端: frame[2]低字节, frame[3]高字节 */
    frame_len = frame[2] + (frame[3] << 8);

    if (frame_len < 10) {
        return -1;  // 帧太短
    }

    /* CRC 校验 */
    crc_actual  = frame[frame_len - 4] + (frame[frame_len - 3] << 8);
    crc_expect  = crc_make(frame, frame_len - 4);
    if (crc_actual != crc_expect) {
        return -2;  // CRC 校验失败
    }

    /* 消息类型 (大端) */
    recv_type = (frame[4] << 8) | frame[5];

    if (recv_type != expected_type) {
        return -3;  // 消息类型不匹配
    }

    /* 提取数据 */
    data_len = (int16_t)(frame_len - 10);
    if (data_len > 0) {
        memcpy(data, frame + 6, data_len);
    }

    return data_len;
}

/* ===================== 帧接收器（环形缓冲 + 帧提取） ===================== */
static uint8_t  g_ring_buf[RING_BUF_SIZE];
static uint16_t g_ring_head = 0;  // 写入位置
static uint16_t g_ring_tail = 0;  // 读取位置
static uint16_t g_ring_count = 0; // 缓冲区字节数

/* 从 UART 读取数据并填入环形缓冲区 */
static void ring_feed(void)
{
    uint8_t tmp[256];
    uint16_t n = uart_read(UART_ID, tmp, sizeof(tmp));
    for (uint16_t i = 0; i < n; i++) {
        g_ring_buf[g_ring_head] = tmp[i];
        g_ring_head = (g_ring_head + 1) % RING_BUF_SIZE;
        g_ring_count++;
        if (g_ring_count > RING_BUF_SIZE) {
            /* 溢出：丢弃最旧字节 */
            g_ring_tail = (g_ring_tail + 1) % RING_BUF_SIZE;
            g_ring_count = RING_BUF_SIZE;
        }
    }
}

/* 清空环形缓冲区 */
static void ring_clear(void)
{
    g_ring_head  = 0;
    g_ring_tail  = 0;
    g_ring_count = 0;
}

/* 在环形缓冲区中搜索一帧，成功则拷贝到 frame 并移除该帧 */
static uint8_t ring_get_frame(uint8_t *frame, uint16_t *frame_len)
{
    uint16_t count = g_ring_count;
    uint16_t tail  = g_ring_tail;
    uint16_t i, j;

    if (count < 10) return 0;  // 最小帧长 10

    /* 搜索应答帧头 $$（后两字节被长度覆盖，请求帧 @@，应答帧 $$） */
    for (i = 0; i <= count - 2; i++) {
        uint8_t *p = &g_ring_buf[(tail + i) % RING_BUF_SIZE];
        if (p[0] == '$' && p[1] == '$') {
            /* 向后搜索帧尾 \r\n */
            uint16_t search_start = i + 4;  // 从帧头后开始搜
            uint16_t remaining   = count - search_start;

            for (j = 0; j <= remaining - 2; j++) {
                uint8_t *q = &g_ring_buf[(tail + search_start + j) % RING_BUF_SIZE];
                if (q[0] == '\r' && q[1] == '\n') {
                    uint16_t total = search_start + j + 2;
                    /* 拷贝整帧 */
                    for (uint16_t k = 0; k < total; k++) {
                        frame[k] = g_ring_buf[(tail + k) % RING_BUF_SIZE];
                    }
                    *frame_len = total;
                    /* 从环形缓冲中移除 */
                    g_ring_tail  = (tail + total) % RING_BUF_SIZE;
                    g_ring_count = count - total;
                    return 1;
                }
            }
        }
    }

    /* 无完整帧，安全丢弃部分旧数据防止无限增长 */
    if (count > (RING_BUF_SIZE / 2)) {
        /* 保留最后 512 字节 */
        uint16_t keep = 512;
        g_ring_tail  = (tail + count - keep) % RING_BUF_SIZE;
        g_ring_count = keep;
    }

    return 0;
}

/* ===================== 同步发送并等待应答 ===================== */
/**
 * 发送命令并等待应答
 * @param msg_type  请求消息类型
 * @param send_data 发送数据
 * @param send_len  发送数据长度
 * @param recv_data 接收数据缓冲区
 * @param timeout_ms 超时(ms)
 * @return >=0 应答数据长度，-1 超时，-2 CRC错，-3 类型不匹配
 */
static int16_t upgrade_send_and_recv(uint16_t msg_type,
                                      uint8_t *send_data, uint16_t send_len,
                                      uint8_t *recv_data, uint32_t timeout_ms)
{
    uint8_t send_frame[SEND_BUF_SIZE];
    uint8_t recv_frame[RECV_BUF_SIZE];
    uint16_t send_total;
    uint16_t recv_total;
    uint32_t elapsed = 0;
    const uint32_t poll_interval = 10;  // 10ms 轮询
    uint16_t resp_type = msg_type + MSG_RESP_OFFSET;

    /* 清空接收缓冲 */
    {
        uint8_t drain[256];
        while (uart_read(UART_ID, drain, sizeof(drain)) > 0) {}
    }
    ring_clear();

    /* 打包并发送 */
    send_total = upgrade_pack(msg_type, send_frame, send_data, send_len);
    uart_write(UART_ID, send_frame, send_total);

    printf("  -> TX [%04X] %d bytes\n", msg_type, send_total);
    for (uint16_t i = 0; i < send_total; i++) {
        printf("%02X ", send_frame[i]);
    }
    printf("\n");

    /* 等待应答 */
    while (elapsed < timeout_ms) {
        delay_ms(poll_interval);
        elapsed += poll_interval;

        /* 喂入环形缓冲 */
        ring_feed();

        /* 尝试提取帧 */
        if (ring_get_frame(recv_frame, &recv_total)) {
            printf("  <- RX %d bytes:", recv_total);
            for (uint16_t i = 0; i < recv_total; i++) {
                printf(" %02X", recv_frame[i]);
            }
            printf("\n");

            return upgrade_unpack(resp_type, recv_frame, recv_data);
        }
    }

    printf("  <- timeout (%lu ms)\n", (unsigned long)elapsed);
    return -1;
}

/* ===================== 测试用例 ===================== */

void test_enter_upgrade(void)
{
    int16_t ret;

    printf("\n========================================\n");
    printf("Test: 0xF006 — 停止从MCU通信/进入升级前导\n");
    printf("========================================\n");

    ret = upgrade_send_and_recv(MSG_ENTER_UPGRADE, NULL, 0,
                                 NULL, 3000);
    if (ret >= 0) {
        printf("  OK 成功: 从MCU已停止CAN活动, 数据长度=%d\n", ret);
    } else {
        printf("  FAIL 失败: 错误码=%d\n", ret);
    }
}

void test_query_version(void)
{
    int16_t ret;
    uint8_t recv_data[256] = {0};

    printf("\n========================================\n");
    printf("Test: 0xF001 — 查询从MCU版本号\n");
    printf("========================================\n");

    ret = upgrade_send_and_recv(MSG_QUERY_VER, NULL, 0,
                                 recv_data, 3000);
    if (ret >= 30) {
        char hw_name[11]  = {0};
        char iap_ver[11]  = {0};
        char sw_ver[11]   = {0};
        char hw_ver[11]   = {0};
        memcpy(hw_name, recv_data + 0,  10);
        memcpy(iap_ver, recv_data + 10, 10);
        memcpy(sw_ver,  recv_data + 20, 10);
        memcpy(hw_ver,  recv_data + 30, 10);

        printf("  OK 设备名 : %s\n", hw_name);
        printf("     IAP版本 : %s\n", iap_ver);
        printf("     软件版本: %s\n", sw_ver);
        printf("     硬件版本: %s\n", hw_ver);
    } else if (ret >= 0) {
        printf("  FAIL 版本数据太短: %d 字节 (期望>=30)\n", ret);
    } else {
        printf("  FAIL 失败: 错误码=%d\n", ret);
    }
}

void test_enter_upgrade_mode(void)
{
    int16_t ret;
    uint8_t recv_data[256] = {0};
    uint8_t send_data[20]  = {0};

    printf("\n========================================\n");
    printf("Test: 0xF002 — MCU 进入升级\n");
    printf("========================================\n");

    memcpy(send_data,      "KWH5",  5);
    memcpy(send_data + 10, "1.1.0", 6);

    ret = upgrade_send_and_recv(MSG_NOTIFY_UPGRADE, send_data, 20,
                                 recv_data, 3000);
}

void test_upgrade_done(void)
{
    int16_t ret;
    uint8_t send_data[1] = {0};  // 0 = 升级完成通知

    printf("\n========================================\n");
    printf("Test: 0xF005 — 通知升级完成（从MCU复位）\n");
    printf("========================================\n");

    ret = upgrade_send_and_recv(MSG_UPGRADE_DONE, send_data, 1,
                                 NULL, 5000);
    if (ret >= 0) {
        printf("  OK 成功: 从MCU已复位, 数据长度=%d\n", ret);
    } else {
        printf("  FAIL 失败: 错误码=%d\n", ret);
    }
}

static void test_full_flow(void)
{
    int16_t ret;
    uint8_t recv_data[256] = {0};

    printf("\n========================================\n");
    printf("完整流程: 停止CAN -> 查询版本 -> 恢复\n");
    printf("========================================\n");

    /* Step 1: 停止通信 */
    printf("\n[1/4] 发送 0xF006 停止CAN...\n");
    ret = upgrade_send_and_recv(MSG_ENTER_UPGRADE, NULL, 0, recv_data, 3000);
    if (ret < 0) {
        printf("  FAIL 0xF006 失败, 终止.\n");
        return;
    }
    printf("  OK CAN已停止. 此时USB VCP可安全使用!\n");

    /* Step 2: 查询版本 */
    printf("\n[2/4] 查询版本...\n");
    ret = upgrade_send_and_recv(MSG_QUERY_VER, NULL, 0, recv_data, 3000);
    if (ret >= 30) {
        char sw_ver[11] = {0};
        memcpy(sw_ver, recv_data + 20, 10);
        printf("  OK SW Ver: %s\n", sw_ver);
    } else {
        printf("  WARN 查询版本失败, 继续...\n");
    }

    /* Step 3: 模拟 USB VCP 工作时间 */
    printf("\n[3/4] USB VCP 工作中... (模拟 3 秒)\n");
    delay_ms(3000);

    /* Step 4: 恢复/退出 */
    printf("\n[4/4] 发送 0xF005 复位从MCU...\n");
    {
        uint8_t sd[1] = {0};
        ret = upgrade_send_and_recv(MSG_UPGRADE_DONE, sd, 1, NULL, 5000);
        if (ret >= 0) {
            printf("  OK 从MCU已复位, CAN通信恢复.\n");
        } else {
            printf("  WARN 复位命令无应答 (可能已自动复位).\n");
        }
    }
}

/* ===================== 入口 ===================== */
int main_test(void)
{
    printf("============================================\n");
    printf("  从MCU 升级协议测试工具\n");
    printf("  UART%d @ %d bps\n", UART_ID, UART_BAUDRATE);
    printf("============================================\n");

    /* 
     * 选择测试项（取消注释需要的行）:
     */
    // test_enter_upgrade();     // 仅 0xF006
    // test_query_version();     // 仅 0xF001
    // test_upgrade_done();      // 仅 0xF005
    test_full_flow();            // 完整流程

    printf("\n============================================\n");
    printf("  测试结束.\n");
    printf("============================================\n");

    return 0;
}
