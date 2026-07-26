#include <stdio.h>
#include "LVGLPort.h"
#include "uart.h"
#include "res_fs.h"
#include "key.h"
#include "io.h"
#include "jpeg_hw.h"
#include "png_lvgl_decoder.h"
#include "jpeg_lvgl_decoder.h"
#include "img_cache.h"
#include "tmr.h"
#include "flash.h"
#include "pwm.h"
#include "SEGGER_RTT.h"
#include "update.h"
#include "io_config.h"
#include "USBBSP.h"
#include "ui.h"

#ifndef USE_HW_JPEG
#define USE_HW_JPEG 1
#endif

#define IS_KEY_ESC()   get_key_state(KEY1)
#define IS_KEY_UP()    get_key_state(KEY2)
#define IS_KEY_DOWN()  get_key_state(KEY3)
#define IS_KEY_ENTER() get_key_state(KEY4)

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t type;
} shared_info_t;

int fputc(int c, FILE* stream)
{
    if (stream == stdout || stream == stderr) {
        SEGGER_RTT_PutChar(0, (char)c);
        return c;
    }

    return EOF;
}

vu8 usb_insert = 0;

void beep_on(void)
{
    pwm_beep_set_percent(99);
    pwm_beep_enable(1);
}

void beep_off(void)
{
    pwm_beep_enable(0);
    pwm_beep_set_percent(0);
}

static void keypad_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    LV_UNUSED(indev);

    data->state = LV_INDEV_STATE_RELEASED;

    if (IS_KEY_UP()) {
        data->key = LV_KEY_UP;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (IS_KEY_DOWN()) {
        data->key = LV_KEY_DOWN;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (IS_KEY_ESC()) {
        data->key = LV_KEY_BACKSPACE;//LV_KEY_ESC;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (IS_KEY_ENTER()) {
        data->key = LV_KEY_ENTER;
        data->state = LV_INDEV_STATE_PRESSED;
    }
}

static lv_indev_t * g_indevKeyPad = NULL;
static void lv_indev_init(void)
{
	g_indevKeyPad = lv_indev_create(); /* 创建输入设备. */
    lv_indev_set_type(g_indevKeyPad, LV_INDEV_TYPE_KEYPAD); /* 设置为键盘类型. */
    lv_indev_set_read_cb(g_indevKeyPad, keypad_read); /* 绑定读取回调. */
}

static u8 is_from_app(u8 erase)
{
    u8 buf[4] = {0x55, 0xAA, 0xBC, 0x85};
    u32 addr = ADDR_UPD_INFO;
    if (!memcmp((void*)addr, (void*)buf, 4))
    {
        if (erase)
        {
            // FLASH_EraseSector(addr);
            // CACHE_CleanAll(DCACHE);
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

static u8 is_upd_mode(void)
{
    u16 cnt_usb_insert = 0;
    u16 cnt_usb_uninsert = 0;
    u16 cnt_press = 0;
    u16 cnt_release = 0;

    if (is_from_app(0))
    {
        return 1;
    }

    beep_on();

    while ((cnt_press+cnt_release) < 1500)
    {
        if (GPIO_ReadInputDataBit(USB_DETECT_GPIO, USB_DETECT_PIN)==RESET)
            cnt_usb_insert++;
        else
            cnt_usb_uninsert++;

        if ((cnt_usb_insert>250) && (cnt_release>200)) //Ö»²åÁËUSB£¬Ã»ÓÐ°´¼ü
        {
            return 0;
        }

        if ((cnt_usb_insert>250) && (cnt_press>200)) //°´¼üÓÖ²åÁËUSB
        {
            return 1;
        }

        if ((cnt_press>100) || (cnt_release>100))
        {
            beep_off();
        }

        if (get_key_state(KEY1))
            cnt_press++;
        else
            cnt_release++;

        delay_ms(1);
    }

    if ((cnt_press>1200) && (cnt_usb_insert>150))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void to_app(void)
{
    typedef int (*jump)(void);
    volatile int *ptr = (int *)ADDR_APP;
    static jump app;

    if (*ptr != 0xFFFFFFFF)
    {
        uint8_t i;

        DSI_BACKLIGHT_ON(false);

        __disable_irq();
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

        PeripheralReset(PeripheralTIM6);
        PeripheralReset(PeripheralUSB1);
        PeripheralReset(PeripheralDMA2D);
        PeripheralReset(PeripheralDSI);
        PeripheralReset(PeripheralLTDC);
        PeripheralEnable(PeripheralTIM6, false);
        PeripheralEnable(PeripheralUSB1, false);
        PeripheralEnable(PeripheralDMA2D, false);
        PeripheralEnable(PeripheralDSI, false);
        PeripheralEnable(PeripheralLTDC, false);

        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL = 0;
        for (i = 0; i < 8; i++)
        {
            NVIC->ICER[i] = 0xFFFFFFFF;
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }
        __enable_irq();

        app = (jump)(*(__IO uint32_t*)(ADDR_APP + 4));
        __set_MSP(*(__IO uint32_t*) ADDR_APP);
        app();
    }
}

void usb_disconnect(void)
{
    PeripheralEnable(PeripheralGPIOA, true); 
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 拉低. */
    GPIO_ResetBits(GPIOA, GPIO_Pin_12); 
    
    SystemDelay(300); 
}

extern void test_enter_upgrade_mode(void);
extern void test_upgrade_done(void);
extern void test_query_version(void);
int main(void)
{
    SysTick_Config(SystemCoreClock / 1000);
    SEGGER_RTT_Init();
    delay_us_init();
    key_init();
    io_init();
    uart_init(UART_ID_AT, AT_UART_BAUDRATE);

    printf("%s VERSION: %s\n", name, ver);

    if (is_upd_mode()) {
        printf("Enter Update Mode.\n");
        test_enter_upgrade_mode();

        QSPI_Init(NULL);
        QSPI_SetLatency(0);

        usb_disconnect();
        USBSetup();
        
        LVGLSetup();
        DSI_BACKLIGHT_ON(true);
        
        lv_indev_init();
        
        res_init();
        res_fs_init();
        png_lvgl_decoder_init();
        jpeg_hw_init();
        jpeg_lvgl_decoder_init();
        
        update_init();
        is_from_app(1);
        ui_init();
        
        usb_insert = 1;

        while (1)
        {
            lv_timer_handler();
            update_handle();
            // test_query_version();
            // delay_ms(1000);
        }
    }
    else
    {
        printf("Run to main.\n");
        test_upgrade_done();
        to_app();
    }

    return 0;
}
