#ifndef __IO_CONFIG
#define __IO_CONFIG
#include "mh2457.h"

/* KEY. */
#define KEY1_GPIO_PORT (GPIOE)
#define KEY1_GPIO_PIN  (GPIO_Pin_13)
#define KEY2_GPIO_PORT (GPIOE)
#define KEY2_GPIO_PIN  (GPIO_Pin_12)
#define KEY3_GPIO_PORT (GPIOE)
#define KEY3_GPIO_PIN  (GPIO_Pin_11)
#define KEY4_GPIO_PORT (GPIOH)
#define KEY4_GPIO_PIN  (GPIO_Pin_14)

/* USB. */
#define USB_CHECK_GPIO_PIN (PC13)
#define USB_DP_GPIO_PIN    (PA12)
#define USB_DM_GPIO_PIN    (PA11)
#define USE_DP_UP          (PD2)

/* LCD. */
#define DSI_RESET_GPIO_PIN (PC15)
#define DSI_BL_GPIO        (GPIOA)
#define DSI_BL_PIN         (GPIO_Pin_1)

/* WIFI. */
#if 0
#define WIFI_EN_GPIO_PIN   (PI5)
#define SD_D0_GPIO_PIN     (PB4)
#define SD_D1_GPIO_PIN     (PA8)
#define SD_D2_GPIO_PIN     (PC10)
#define SD_D3_GPIO_PIN     (PC11)
#define SD_CMD_GPIO_PIN    (PA6)
#define SD_CLK_GPIO_PIN    (PB2)
#define PWR_EN_GPIO_PIN    (PC5)
#define WAKE_IN_GPIO_PIN   (PB0)
#define WAKE_OUT_GPIO_PIN  (PB1)
#endif

/* QMI8658A. */
#define QMI8658A_SPI             (SPI2)
#define QMI8658A_SPI_CLK         (RCC_APB1Periph_SPI2)
#define QMI8658A_GPIO_CLK        (RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOH)
#define QMI8658A_MOSI_GPIO       (GPIOB)
#define QMI8658A_MOSI_PIN        (GPIO_Pin_15)
#define QMI8658A_MISO_GPIO       (GPIOB)
#define QMI8658A_MISO_PIN        (GPIO_Pin_14)
#define QMI8658A_SCK_GPIO        (GPIOB)
#define QMI8658A_SCK_PIN         (GPIO_Pin_13)
#define QMI8658A_SCK_CLK         (RCC_AHB1Periph_GPIOB)
#define QMI8658A_NS_GPIO         (GPIOB)
#define QMI8658A_NS_PIN          (GPIO_Pin_12)
#define QMI8658A_MOSI_SOURCE     (GPIO_PinSource15)
#define QMI8658A_MISO_SOURCE     (GPIO_PinSource14)
#define QMI8658A_SCK_SOURCE      (GPIO_PinSource13)
#define QMI8658A_MOSI_AF         (GPIO_AF_SPI2)
#define QMI8658A_MISO_AF         (GPIO_AF_SPI2)
#define QMI8658A_SCK_AF          (GPIO_AF_SPI2)

#define QMI8658A_INT1_GPIO       (GPIOH)
#define QMI8658A_INT1_PIN        (GPIO_Pin_11)
#define QMI8658A_INT2_GPIO       (GPIOH)
#define QMI8658A_INT2_PIN        (GPIO_Pin_12)
#define QMI8658A_INT1_EXTI_PORT  (EXTI_PortSourceGPIOH)
#define QMI8658A_INT1_EXTI_SRC   (EXTI_PinSource11)
#define QMI8658A_INT1_EXTI_LINE  (EXTI_Line11)
#define QMI8658A_INT1_EXTI_IRQN  (EXTI15_10_IRQn)

/* GPS. */
#define GPS_ON_IO                (PG13)
#define GPS_ON_GPIO              (GPIOG)
#define GPS_ON_PIN               (GPIO_Pin_13)
#define GPS_EN_IO                (PG12)
#define GPS_EN_GPIO              (GPIOG)
#define GPS_EN_PIN               (GPIO_Pin_12)

#define UART_GPS_ID              UART_ID_GPS
#define UART_AT_ID               UART_ID_AT

#define GPS_UART              USART1
#define GPS_UART_BAUDRATE     9600
#define GPS_UART_PERIPH       PeripheralUSART1
#define GPS_UART_RX_GPIO      GPIOB
#define GPS_UART_RX_GPIO_CLK  PeripheralGPIOB
#define GPS_UART_RX_PIN       GPIO_Pin_7
#define GPS_UART_RX_AF        GPIO_AF_USART1
#define GPS_UART_RX_SOURCE    GPIO_PinSource7
#define GPS_UART_TX_GPIO      GPIOB
#define GPS_UART_TX_GPIO_CLK  PeripheralGPIOB
#define GPS_UART_TX_PIN       GPIO_Pin_6
#define GPS_UART_TX_AF        GPIO_AF_USART1
#define GPS_UART_TX_SOURCE    GPIO_PinSource6
#define GPS_DMA_CLK           PeripheralDMA2
#define GPS_DMA_RX_STREAM     DMA2_Stream2
#define GPS_DMA_TX_STREAM     DMA2_Stream7
#define GPS_DMA_RX_CHANNEL    DMA_Channel_4
#define GPS_DMA_TX_CHANNEL    DMA_Channel_4
#define GPS_DMA_RX_IRQN       DMA2_Stream2_IRQn
#define GPS_DMA_TX_IRQN       DMA2_Stream7_IRQn
#define GPS_DMA_TX_TCIF       DMA_IT_TCIF7
#define GPS_DMA_TX_FLAGS      (DMA_FLAG_FEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TCIF7)
#define GPS_UART_IRQHandler   USART1_IRQHandler
#define GPS_UART_IRQn         USART1_IRQn
#define GPS_DMA_TX_IRQHandler DMA2_Stream7_IRQHandler

/* ARTERY. */
#define AT_UART               USART3
#define AT_UART_BAUDRATE      38400
#define AT_UART_PERIPH        PeripheralUSART3
#define AT_UART_RX_GPIO       GPIOB
#define AT_UART_RX_GPIO_CLK   PeripheralGPIOB
#define AT_UART_RX_PIN        GPIO_Pin_11
#define AT_UART_RX_AF         GPIO_AF_USART3
#define AT_UART_RX_SOURCE     GPIO_PinSource11
#define AT_UART_TX_GPIO       GPIOB
#define AT_UART_TX_GPIO_CLK   PeripheralGPIOB
#define AT_UART_TX_PIN        GPIO_Pin_10
#define AT_UART_TX_AF         GPIO_AF_USART3
#define AT_UART_TX_SOURCE     GPIO_PinSource10
#define AT_DMA_CLK            PeripheralDMA1
#define AT_DMA_RX_STREAM      DMA1_Stream1
#define AT_DMA_TX_STREAM      DMA1_Stream3
#define AT_DMA_RX_CHANNEL     DMA_Channel_4
#define AT_DMA_TX_CHANNEL     DMA_Channel_4
#define AT_DMA_RX_IRQN        DMA1_Stream1_IRQn
#define AT_DMA_TX_IRQN        DMA1_Stream3_IRQn
#define AT_DMA_TX_TCIF        DMA_IT_TCIF3
#define AT_DMA_TX_FLAGS       (DMA_FLAG_FEIF3 | DMA_FLAG_DMEIF3 | DMA_FLAG_TEIF3 | DMA_FLAG_HTIF3 | DMA_FLAG_TCIF3)
#define AT_UART_IRQHandler    USART3_IRQHandler
#define AT_UART_IRQn          USART3_IRQn
#define AT_DMA_TX_IRQHandler  DMA1_Stream3_IRQHandler

#define UART_CFG_UART(id)        ((id) == UART_ID_GPS ? GPS_UART : AT_UART)
#define UART_CFG_BAUD(id)        ((id) == UART_ID_GPS ? GPS_UART_BAUDRATE : AT_UART_BAUDRATE)
#define UART_CFG_PERIPH(id)      ((id) == UART_ID_GPS ? GPS_UART_PERIPH : AT_UART_PERIPH)
#define UART_CFG_RX_GPIO(id)     ((id) == UART_ID_GPS ? GPS_UART_RX_GPIO : AT_UART_RX_GPIO)
#define UART_CFG_RX_CLK(id)      ((id) == UART_ID_GPS ? GPS_UART_RX_GPIO_CLK : AT_UART_RX_GPIO_CLK)
#define UART_CFG_RX_PIN(id)      ((id) == UART_ID_GPS ? GPS_UART_RX_PIN : AT_UART_RX_PIN)
#define UART_CFG_RX_AF(id)       ((id) == UART_ID_GPS ? GPS_UART_RX_AF : AT_UART_RX_AF)
#define UART_CFG_RX_SRC(id)      ((id) == UART_ID_GPS ? GPS_UART_RX_SOURCE : AT_UART_RX_SOURCE)
#define UART_CFG_TX_GPIO(id)     ((id) == UART_ID_GPS ? GPS_UART_TX_GPIO : AT_UART_TX_GPIO)
#define UART_CFG_TX_CLK(id)      ((id) == UART_ID_GPS ? GPS_UART_TX_GPIO_CLK : AT_UART_TX_GPIO_CLK)
#define UART_CFG_TX_PIN(id)      ((id) == UART_ID_GPS ? GPS_UART_TX_PIN : AT_UART_TX_PIN)
#define UART_CFG_TX_AF(id)       ((id) == UART_ID_GPS ? GPS_UART_TX_AF : AT_UART_TX_AF)
#define UART_CFG_TX_SRC(id)      ((id) == UART_ID_GPS ? GPS_UART_TX_SOURCE : AT_UART_TX_SOURCE)
#define UART_CFG_DMA_CLK(id)     ((id) == UART_ID_GPS ? GPS_DMA_CLK : AT_DMA_CLK)
#define UART_CFG_RX_STREAM(id)   ((id) == UART_ID_GPS ? GPS_DMA_RX_STREAM : AT_DMA_RX_STREAM)
#define UART_CFG_TX_STREAM(id)   ((id) == UART_ID_GPS ? GPS_DMA_TX_STREAM : AT_DMA_TX_STREAM)
#define UART_CFG_RX_CH(id)       ((id) == UART_ID_GPS ? GPS_DMA_RX_CHANNEL : AT_DMA_RX_CHANNEL)
#define UART_CFG_TX_CH(id)       ((id) == UART_ID_GPS ? GPS_DMA_TX_CHANNEL : AT_DMA_TX_CHANNEL)
#define UART_CFG_RX_IRQN(id)     ((id) == UART_ID_GPS ? GPS_DMA_RX_IRQN : AT_DMA_RX_IRQN)
#define UART_CFG_TX_IRQN(id)     ((id) == UART_ID_GPS ? GPS_DMA_TX_IRQN : AT_DMA_TX_IRQN)
#define UART_CFG_TX_TCIF(id)     ((id) == UART_ID_GPS ? GPS_DMA_TX_TCIF : AT_DMA_TX_TCIF)
#define UART_CFG_TX_FLAGS(id)    ((id) == UART_ID_GPS ? GPS_DMA_TX_FLAGS : AT_DMA_TX_FLAGS)
#define UART_CFG_UART_IRQN(id)   ((id) == UART_ID_GPS ? GPS_UART_IRQn : AT_UART_IRQn)
#define UART_CFG_TX_IRQHANDLER(id) ((id) == UART_ID_GPS ? GPS_DMA_TX_IRQHandler : AT_DMA_TX_IRQHandler)


/* BEEP. */
#define BEEP_GPIO    (GPIOB)
#define BEEP_PIN     (GPIO_Pin_9)

#define IO_PERIPH    (PeripheralGPIOG | PeripheralGPIOA | PeripheralGPIOC)
#define MCU_EN_IO    (PG14)
#define MCU_EN_GPIO  (GPIOG)
#define MCU_EN_PIN   (GPIO_Pin_14)

#define PWR_CTR_GPIO (GPIOG)
#define PWR_CTR_PIN  (GPIO_Pin_11)
#define PWR_CTR_IO   (PG11)

#define PR_CTR_IO    (PA10)
#define PR_CTR_GPIO  (GPIOA)
#define PR_CTR_PIN   (GPIO_Pin_10)

/* USB Detect. */
#define USB_DETECT_IO    (PC13)
#define USB_DETECT_GPIO  (GPIOC)
#define USB_DETECT_PIN   (GPIO_Pin_13)

#endif
