#include "spi.h"
#include "io_config.h"
#include "rtthread.h"

/* OS. */
#define QMI_DELAY(n)            rt_hw_us_delay(n)
#define QMI_MODE_INIT_CALL(fn)  INIT_BOARD_EXPORT(fn)
#define QMI_INIT_CALL(fn)       INIT_DEVICE_EXPORT(fn)

/* CS 引脚控制. */
#define QMI_CS_LOW()            GPIO_ResetBits(QMI8658A_NS_GPIO, QMI8658A_NS_PIN)
#define QMI_CS_HIGH()           GPIO_SetBits(QMI8658A_NS_GPIO, QMI8658A_NS_PIN)

/**
 * @brief  qmi8658 芯片上电时, CS 低电平选择 SPI 模式.
 * @param  None
 * @retval 0 成功, 其他 失败.
 */
int qmi8658_spi_mode_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 芯片上电时, CS 低电平选择 SPI 模式 */
    QMI_CS_LOW();

    RCC_AHB1PeriphClockCmd(QMI8658A_SCK_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    GPIO_InitStructure.GPIO_Pin = QMI8658A_NS_PIN;
    GPIO_Init(QMI8658A_NS_GPIO, &GPIO_InitStructure);

    QMI_DELAY(100);
    QMI_CS_HIGH();

    return 0;
}
QMI_MODE_INIT_CALL(qmi8658_spi_mode_init);

/**
 * @brief  qmi8658 驱动初始化.
 * @param  None
 * @retval 0 成功, 其他 失败.
 */
int qmi8658_spi_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(QMI8658A_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(QMI8658A_SPI_CLK, ENABLE);

    /* SPI 引脚复用配置. */
    GPIO_PinAFConfig(QMI8658A_SCK_GPIO, QMI8658A_SCK_SOURCE, QMI8658A_SCK_AF);
    GPIO_PinAFConfig(QMI8658A_MISO_GPIO, QMI8658A_MISO_SOURCE, QMI8658A_MISO_AF);
    GPIO_PinAFConfig(QMI8658A_MOSI_GPIO, QMI8658A_MOSI_SOURCE, QMI8658A_MOSI_AF);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = QMI8658A_SCK_PIN;
    GPIO_Init(QMI8658A_SCK_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = QMI8658A_MISO_PIN;
    GPIO_Init(QMI8658A_MISO_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = QMI8658A_MOSI_PIN;
    GPIO_Init(QMI8658A_MOSI_GPIO, &GPIO_InitStructure);

    /* 配置 SPI 参数. */ 
    SPI_I2S_DeInit(QMI8658A_SPI);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; /* QMI8658 支持 Mode 0, 3. */
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    /* 频率最大 15MHz, APB1 时钟为 75MHz, 所以最小为 8 分频. */
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);

    /* INT 配置为输入. */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin = QMI8658A_INT1_PIN;
    GPIO_Init(QMI8658A_INT1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = QMI8658A_INT2_PIN;
    GPIO_Init(QMI8658A_INT2_GPIO, &GPIO_InitStructure);

    /* 配置 INT1 中断. */
    SYSCFG_EXTILineConfig(QMI8658A_INT1_EXTI_PORT, QMI8658A_INT1_EXTI_SRC);
    EXTI_InitStructure.EXTI_Line = QMI8658A_INT1_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; /* 上升沿触发. */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = QMI8658A_INT1_EXTI_IRQN;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    return 0;
}
QMI_INIT_CALL(qmi8658_spi_init);

/* --- 2. 读写寄存器函数 --- */
/**
 * @brief  SPI 读写寄存器.
 * @param  byte: 要读写的字节.
 * @retval 读写的字节.
*/
static uint8_t SPI_ReadWrite(uint8_t byte)
{
    uint16_t cnt = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {
        if (cnt++ > 500) {
            rt_kprintf("[SPI] Wait timeout.\n");
            return -1;
        }
        QMI_DELAY(2);
    }
    SPI_I2S_SendData(SPI2, byte);
    cnt = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {
        if (cnt++ > 500) {
            rt_kprintf("[SPI] Wait timeout.\n");
            return -1;
        }
        QMI_DELAY(2);
    }
    return SPI_I2S_ReceiveData(SPI2);
}

/**
 * @brief SPI传输函数
 * @param tx_buf: 发送缓冲区
 * @param rx_buf: 接收缓冲区
 * @param len: 传输长度
 * @return 成功返回 0，失败返回 -1
 */
int qmi8658_spi_transfer(uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len)
{
    if (len == 0 || tx_buf == 0 || rx_buf == 0) return -1;

    QMI_CS_LOW();
    for (uint32_t i = 0; i < len; i++) {
        rx_buf[i] = SPI_ReadWrite(tx_buf[i]);
    }
    QMI_CS_HIGH();
    return 0;
}

void QMI8658_WriteReg(uint8_t reg, uint8_t val) {
    QMI_CS_LOW();
    SPI_ReadWrite(reg & 0x7F); // 写操作最高位为0
    SPI_ReadWrite(val);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
    QMI_CS_HIGH();
}

uint8_t QMI8658_ReadReg(uint8_t reg) {
    uint8_t val;
    QMI_CS_LOW();
    SPI_ReadWrite(reg | 0x80); // 读操作最高位为1
    val = SPI_ReadWrite(0xFF);
    QMI_CS_HIGH();
    return val;
}

// 寄存器地址定义
#define QMI8658_WHO_AM_I   0x00
#define QMI8658_CTRL1      0x02
#define QMI8658_CTRL2      0x03
#define QMI8658_CTRL3      0x04
#define QMI8658_CTRL7      0x08
#define QMI8658_CTRL9      0x0A
#define QMI8658_STATUS0    0x2D
#define QMI8658_AX_L       0x35

typedef struct {
    float acc_x, acc_y, acc_z;
    float gyro_x, gyro_y, gyro_z;
} QMI8658_Data_t;

/* --- 3. 传感器初始化序列 --- */
static int qmi8658_test(void) {
    __IO u8 vol = 0;
    __IO uint16_t timeout = 500;
    // qmi8658_spi_init();

    // 确认 ID (应为 0x05)
    while(1)
    {
        vol = QMI8658_ReadReg(QMI8658_WHO_AM_I);
        if (vol == 0x05)
            break;
        
        QMI_DELAY(10);
    }

    // 2. 软复位 (非常重要：确保清除之前调试残留的错误状态)
    QMI8658_WriteReg(0x60, 0xB0); 
    QMI_DELAY(20000); // 复位后必须等待

    // 3. CTRL1: 必须使能地址自增(Bit 6)，否则读取 12 字节时数据不会更新
    // 0x40 = 0100 0000 (SIM=0: 4-wire, AI=1: Auto Inc)
    // QMI8658_WriteReg(QMI8658_CTRL1, 0x40);
    // vol = QMI8658_ReadReg(QMI8658_CTRL1);

    // 4. CTRL2: 加速度计配置
    // 0x13: ±4g (0x01 << 4), 500Hz (0x03) | 开启 LPF (低通滤波)
    // 注意：建议先用 ±4g 测试，量程越小原始数据越明显
    QMI8658_WriteReg(QMI8658_CTRL2, 0x13); 
    vol = QMI8658_ReadReg(QMI8658_CTRL2);

    // 5. CTRL3: 陀螺仪配置
    // 0x63: ±1024dps (0x06 << 4), 500Hz (0x03) | 开启 LPF
    QMI8658_WriteReg(QMI8658_CTRL3, 0x63);
    vol = QMI8658_ReadReg(QMI8658_CTRL3);

    // 6. CTRL5: 设置数据输出的低通滤波器频率 (可选，增强稳定性)
    // 0x11: 使能加速度计和陀螺仪的 LPF，并设置合适的带宽
    QMI8658_WriteReg(0x06, 0x11);

    // 7. CTRL7: 引擎控制 (必须最后开启)
    // 0x03: 使能 Acc 和 Gyro
    QMI8658_WriteReg(QMI8658_CTRL7, 0x03);
    
    // 8. 启动延时 (关键！)
    // 传感器内部电路稳定和 ODR 开始采样需要时间
    QMI_DELAY(50);
    
    while(timeout--) {
        vol = QMI8658_ReadReg(0x2D); // 读取状态寄存器 0
        if (vol & 0x01) {            // 判断 Bit 0 是否为 1
            break;                   // 就绪，退出循环
        }
        QMI_DELAY(1);         // 没就绪就等 1ms
    }

    return 0;
}
// INIT_APP_EXPORT(QMI8658_Init_All);

/* --- 4. 数据读取与换算 --- */
uint8_t QMI8658_ReadData(QMI8658_Data_t *data) {
    uint8_t raw[12];
    int16_t acc_raw[3], gyro_raw[3];

    QMI_CS_LOW();
    SPI_ReadWrite(QMI8658_AX_L | 0x80);
    for(int i=0; i<12; i++) raw[i] = SPI_ReadWrite(0xFF);
    QMI_CS_HIGH();

    // 原始数据拼合 (LSB first)
    for(int i=0; i<3; i++) {
        acc_raw[i] = (int16_t)(raw[2*i+1] << 8 | raw[2*i]);
        gyro_raw[i] = (int16_t)(raw[2*i+7] << 8 | raw[2*i+6]);
    }

    // 换算 (±8g 对应 4096 LSB/g, ±2048dps 对应 16 LSB/dps)
    data->acc_x = acc_raw[0] / 4096.0f;
    data->acc_y = acc_raw[1] / 4096.0f;
    data->acc_z = acc_raw[2] / 4096.0f;
    
    data->gyro_x = gyro_raw[0] / 16.0f;
    data->gyro_y = gyro_raw[1] / 16.0f;
    data->gyro_z = gyro_raw[2] / 16.0f;

    return 1;
}

/* --- 5. 中断服务函数 --- */
void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
        // 清除中断标志
        EXTI_ClearITPendingBit(EXTI_Line11);

        // FreeRTOS 通知逻辑
        // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // vTaskNotifyGiveFromISR(xSensorTaskHandle, &xHigherPriorityTaskWoken);
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
