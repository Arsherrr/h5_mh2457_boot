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
#include "sefltest.h"
#include "rtt_cli.h"

#ifndef USE_HW_JPEG
#define USE_HW_JPEG 1
#endif

#define IS_KEY_ESC()   get_key_state(KEY4)
#define IS_KEY_UP()    get_key_state(KEY3)
#define IS_KEY_DOWN()  get_key_state(KEY2)
#define IS_KEY_ENTER() get_key_state(KEY1)

/* MH2457 没有备份寄存器, 只能用 flash 保存共享信息. */
#define SM_ADDR   (RES_LOGO_BASE - FLASH_SECTOR_SIZE)

#define SM_MAGIC  (0x72616873)

typedef enum {
    SM_EVENT_NONE = 0,
    SM_EVENT_OTA,
    SM_EVENT_FCT_MODE,
    SM_EVENT_SHUTDOWN,
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

/* 上下同时按下后, 松键前仍拦截导航, 避免单键残留带动光标 */
static int s_factory_combo_guard = 0;
/* OTA / 系统检测禁用；菜单及其它功能页启用 */
static int s_factory_hotkey_en = 1;

static void keypad_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    LV_UNUSED(indev);

    data->state = LV_INDEV_STATE_RELEASED;

    /* 上下同时按:菜单光标不动 */
    if (IS_KEY_UP() && IS_KEY_DOWN()) {
        if (s_factory_hotkey_en) {
            s_factory_combo_guard = 1;
        }
        return;
    }

    /* 组合键松开过程:仍按着上或下时也不动光标 */
    if (s_factory_hotkey_en && s_factory_combo_guard) {
        if (IS_KEY_UP() || IS_KEY_DOWN()) {
            return;
        }
        s_factory_combo_guard = 0;
    }

    /* 输密码阶段(ESC/UP):允许光标随键移动 */

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
}

int to_app(void);

void factory_mode_hotkey_enable(int enable)
{
    s_factory_hotkey_en = enable ? 1 : 0;
    if (!s_factory_hotkey_en) {
        s_factory_combo_guard = 0;
    }
}

static void factory_mode_glow_fade_exec(void *obj, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)v, LV_PART_MAIN);
}

static void factory_mode_glow_anim_ready(lv_anim_t *a)
{
    lv_obj_t *glow = (lv_obj_t *)lv_anim_get_user_data(a);
    if (glow != NULL && lv_obj_is_valid(glow)) {
        lv_obj_delete(glow);
    }
}

static void factory_mode_show_glow(void)
{
    lv_obj_t *scr = lv_screen_active();
    lv_coord_t scr_w = lv_obj_get_width(scr);
    lv_coord_t scr_h = lv_obj_get_height(scr);
    /* 留出外圈空间给 shadow/outline, 否则会被屏幕裁掉 */
    const lv_coord_t margin = 18;

    lv_obj_t *glow = lv_obj_create(scr);
    lv_obj_remove_style_all(glow);
    lv_obj_set_size(glow, scr_w - margin * 2, scr_h - margin * 2);
    lv_obj_center(glow);
    lv_obj_clear_flag(glow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(glow, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_radius(glow, 14, LV_PART_MAIN);
    lv_obj_move_foreground(glow);

    /* 连续柔光:外圈 outline + shadow, 不用多层硬边框 */
    lv_obj_set_style_outline_color(glow, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_outline_width(glow, 16, LV_PART_MAIN);
    lv_obj_set_style_outline_pad(glow, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_opa(glow, LV_OPA_20, LV_PART_MAIN);

    lv_obj_set_style_shadow_color(glow, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_shadow_width(glow, 40, LV_PART_MAIN);
    lv_obj_set_style_shadow_spread(glow, 8, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(glow, LV_OPA_40, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_x(glow, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(glow, 0, LV_PART_MAIN);

    /* 很细的高光芯, 避免生硬粗白框 */
    lv_obj_set_style_border_color(glow, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_border_width(glow, 2, LV_PART_MAIN);
    lv_obj_set_style_border_opa(glow, LV_OPA_40, LV_PART_MAIN);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, glow);
    lv_anim_set_user_data(&a, glow);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_duration(&a, 360);
    lv_anim_set_delay(&a, 60);
    lv_anim_set_exec_cb(&a, factory_mode_glow_fade_exec);
    lv_anim_set_ready_cb(&a, factory_mode_glow_anim_ready);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}

static int factory_mode_mark_and_jump_app(void)
{
    shared_mem_t sm = {
        .magic = SM_MAGIC,
        .event = SM_EVENT_FCT_MODE,
    };

    if (flash_erase(SM_ADDR - FLASH_BASE_ADDR, FLASH_SECTOR_SIZE) < 0) {
        return -1;
    }

    if (flash_write(SM_ADDR - FLASH_BASE_ADDR, (const uint8_t *)&sm, sizeof(sm)) < 0) {
        return -1;
    }

    printf("Enter Factory Test Mode.\n");
    return to_app();
}

typedef enum {
    FACTORY_KEY_IDLE = 0,
    FACTORY_KEY_WAIT_ESC_1,
    FACTORY_KEY_WAIT_ESC_2,
    FACTORY_KEY_WAIT_ESC_3,
    FACTORY_KEY_WAIT_UP_1,
    FACTORY_KEY_WAIT_UP_2,
    FACTORY_KEY_GOTO,
} factory_key_state_e;

#define FACTORY_KEY_DEBOUNCE_MS  80
#define FACTORY_KEY_TIMEOUT_MS   10000

typedef enum {
    FACTORY_INPUT_NONE = 0,
    FACTORY_INPUT_START,
    FACTORY_INPUT_ESC,
    FACTORY_INPUT_UP,
    FACTORY_INPUT_DOWN,
    FACTORY_INPUT_OTHER,
} factory_input_e;

static factory_input_e factory_mode_get_input(void)
{
    uint8_t up = IS_KEY_UP() ? 1 : 0;
    uint8_t down = IS_KEY_DOWN() ? 1 : 0;
    uint8_t esc = IS_KEY_ESC() ? 1 : 0;
    uint8_t enter = IS_KEY_ENTER() ? 1 : 0;

    if (!up && !down && !esc && !enter) {
        return FACTORY_INPUT_NONE;
    }

    if (up && down) {
        return FACTORY_INPUT_START;
    }

    if (esc && !up && !down && !enter) {
        return FACTORY_INPUT_ESC;
    }

    if (up && !down && !esc && !enter) {
        return FACTORY_INPUT_UP;
    }

    if (down && !up && !esc && !enter) {
        return FACTORY_INPUT_DOWN;
    }

    return FACTORY_INPUT_OTHER;
}

static void factory_mode_key_monitor(void)
{
    if (!s_factory_hotkey_en) {
        return;
    }

    static factory_key_state_e state = FACTORY_KEY_IDLE;
    static factory_input_e last_input = FACTORY_INPUT_NONE;
    static factory_input_e stable_input = FACTORY_INPUT_NONE;
    static factory_input_e handled_input = FACTORY_INPUT_NONE;
    static uint32_t input_change_tick = 0;
    static uint32_t password_start_tick = 0;

    factory_input_e input = factory_mode_get_input();
    uint32_t now = lv_tick_get();

    if (state != FACTORY_KEY_IDLE && now - password_start_tick >= FACTORY_KEY_TIMEOUT_MS) {
        state = FACTORY_KEY_IDLE;
    }

    if (input != last_input) {
        last_input = input;
        input_change_tick = now;
        return;
    }

    if (input != stable_input) {
        if (now - input_change_tick < FACTORY_KEY_DEBOUNCE_MS) {
            return;
        }

        stable_input = input;
    }

    if (stable_input == FACTORY_INPUT_NONE) {
        handled_input = FACTORY_INPUT_NONE;
        return;
    }

    if (stable_input == handled_input) {
        return;
    }

    handled_input = stable_input;

    if (stable_input == FACTORY_INPUT_START) {
        state = FACTORY_KEY_WAIT_ESC_1;
        password_start_tick = now;
        s_factory_combo_guard = 1;
        factory_mode_show_glow();
        return;
    }

    switch (state) {
    case FACTORY_KEY_IDLE:
        break;

    case FACTORY_KEY_WAIT_ESC_1:
    case FACTORY_KEY_WAIT_ESC_2:
    case FACTORY_KEY_WAIT_ESC_3:
        if (stable_input == FACTORY_INPUT_ESC) {
            ++state;
        } else {
            state = FACTORY_KEY_IDLE;
        }
        break;

    case FACTORY_KEY_WAIT_UP_1:
    case FACTORY_KEY_WAIT_UP_2:
        if (stable_input == FACTORY_INPUT_UP) {
            if (++state == FACTORY_KEY_GOTO) {
                state = FACTORY_KEY_IDLE;
                factory_mode_mark_and_jump_app();
            }
        } else {
            state = FACTORY_KEY_IDLE;
        }
        break;

    default:
        state = FACTORY_KEY_IDLE;
        break;
    }
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

        if (check_app_fast_crc_hw() == 0) {
            return -1;
        }

        if (app_crc_info_check() != 0) {
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
    uint8_t app_ok = 0;
    uint8_t res_ok = 0;

    SysTick_Config(SystemCoreClock / 1000);
    SEGGER_RTT_Init();
    rtt_cli_init();
    delay_us_init();
    key_init();
    io_init();

    printf("%s VERSION: %s\n", name, ver);

    /* 上电先做 APP / 资源完整性检查 */
    app_ok = check_app_integrity();
    res_ok = check_res_integrity();

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
        } else if (!app_ok || !res_ok) {
            /* APP 或资源损坏: 显示自检提示页, 引导升级 */
            printf("Self-check fail: app=%u res=%u\n", app_ok, res_ok);
            ui_system_self_check_init(app_ok, res_ok);
        } else {
            /* 主菜单. */
            ui_menu_init();
        }

        /* 刷新一次. */
        lv_timer_handler();

        /* 防止 ENTER 键不松开. */
        while (IS_KEY_ENTER());
        lv_indev_set_read_cb(g_indevKeyPad, keypad_read); /* 绑定读取回调. */

        /* 通知 AT32 进入 bootloader. */
        at_enter_boot_mode();

        while (is_ota_mode() == 0) {
            rtt_cli_poll();
            factory_mode_key_monitor();
            menu_select_handler();
            lv_timer_handler();
        }

        update_init();
        is_from_app(1);

        while (1) {
            rtt_cli_poll();
            factory_mode_key_monitor();
            menu_select_handler();
            lv_timer_handler();
            update_handle();
        }
    } else {
        printf("Run to main.\n");
        if (app_ok && res_ok && to_app() == 0) {
            /* 已跳转到 APP */
        } else {
            printf("Run to main failed. app=%u res=%u\n", app_ok, res_ok);
            goto BOOT;
        }
    }

    return 0;
}
