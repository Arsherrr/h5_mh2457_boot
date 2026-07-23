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
#include "img.h"
#include "usb.h"
#include "SDRAMBSP.h"
#include "ui_selftest.h"

#ifndef USE_HW_JPEG
#define USE_HW_JPEG 1
#endif

#define IS_KEY_ESC()   get_key_state(KEY1)
#define IS_KEY_UP()    get_key_state(KEY2)
#define IS_KEY_DOWN()  get_key_state(KEY3)
#define IS_KEY_ENTER() get_key_state(KEY4)

/* MH2457 没有备份寄存器, 只能用 flash 保存共享信息. */
#define SM_ADDR   (RES_LOGO_BASE - FLASH_SECTOR_SIZE)

#define SM_MAGIC  (0x72616873)

typedef enum {
    SM_EVENT_NONE = 0,
    SM_EVENT_OTA,
    SM_EVENT_RESVD = 0xFFFF,
} sm_event_e;

typedef struct __attribute__((packed)) {
    uint32_t   magic;
    sm_event_e event;
} shared_mem_t;

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
        data->key = LV_KEY_LEFT;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (IS_KEY_DOWN()) {
        data->key = LV_KEY_RIGHT;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (IS_KEY_ESC()) {
        data->key = LV_KEY_BACKSPACE;//LV_KEY_ESC;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (IS_KEY_ENTER()) {
        data->key = LV_KEY_ENTER;
        data->state = LV_INDEV_STATE_PRESSED;
    }
}

lv_indev_t * g_indevKeyPad = NULL;
static void lv_indev_init(void)
{
	g_indevKeyPad = lv_indev_create(); /* 创建输入设备. */
    lv_indev_set_type(g_indevKeyPad, LV_INDEV_TYPE_KEYPAD); /* 设置为键盘类型. */
    lv_indev_set_read_cb(g_indevKeyPad, keypad_read); /* 绑定读取回调. */
}

extern int menu_select;
extern int reset_config(void);
extern int system_restart(void);
void menu_select_handler(void)
{
    if (menu_select) {
        if (menu_select == 1) {
            usb_ota_mode();
        } else if (menu_select == 2) {
            usb_log_mode();
        } else if (menu_select == 3) {
            reset_config();
            uint32_t tick = lv_tick_get();
            while (1) {
                if (lv_tick_get() - tick >= 3000) break;
                lv_timer_handler();
            }
            system_restart();
        }

        menu_select = 0;
    }
}

static u8 is_from_app(u8 erase)
{
    shared_mem_t *sm = (shared_mem_t *)SM_ADDR;
    
    if (sm->magic == SM_MAGIC && sm->event == SM_EVENT_OTA)
    {
        if (erase)
        {
            FLASH_EraseSector(SM_ADDR);
            CACHE_CleanAll(DCACHE);
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

int to_app(void)
{
    typedef int (*jump)(void);
    volatile int *ptr = (int *)ADDR_APP;
    static jump app;

    if (*ptr != 0xFFFFFFFF)
    {
        if (strncmp(name, (char *)ADDR_APP_NAME, LEN_IAP_NAME) != 0) {
            return -1;
        }

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
        
        return 0;
    }
    
    return -1;
}

extern int16_t at_enter_boot(void);
static void at_enter_boot_mode(void)
{
    uint8_t try = 3;
    do {
        if (at_enter_boot() >= 0) {
            printf("[AT] Enter boot successfully.\n");
            return;
        }
    } while (--try);
    
    printf("[AT] Enter boot failed\n");
}

extern void fs_init(void);
extern int log_cache_create(void);
extern void log_cache_destroy(void);
extern void read_config(void);
extern void ui_menu_init(void);

int main(void)
{
    SysTick_Config(SystemCoreClock / 1000);
    SEGGER_RTT_Init();
    delay_us_init();
    key_init();
    io_init();

    printf("%s VERSION: %s\n", name, ver);
    
    if (is_upd_mode()) {
BOOT:
        printf("Enter Update Mode.\n");
        uart_init(UART_ID_AT, AT_UART_BAUDRATE);

        SDRAMSetup();

        /* 文件系统. */
        fs_init();
        read_config();

        /* LVGL 初始化. */
        LVGLSetup();
        DSI_BACKLIGHT_ON(true);
        lv_indev_init();

        /* JPEG 硬件编码初始化. */
        jpeg_hw_mem_init();
        jpeg_hw_init();
        img_init();

        usb_insert = 1;

        if (is_from_app(0)) {
            /* 直接进入 OTA 升级界面. */
            usb_ota_mode();
        } else {
            /* 主菜单. */
            ui_menu_init();
            // ui_system_self_check_init(0, 0);
        }

        /* 刷新一次. */
        lv_timer_handler();

        /* 通知 AT32 进入 bootloader. */
        at_enter_boot_mode();

        while (is_ota_mode() == 0) {
            menu_select_handler();
            lv_timer_handler();
        }

        update_init();
        is_from_app(1);

        while (1) {
            menu_select_handler();
            lv_timer_handler();
            update_handle();
        }
    } else {
        printf("Run to main.\n");
        if (to_app() == -1) {
            printf("Run to main failed.\n");
            goto BOOT;
        }
    }

    return 0;
}
