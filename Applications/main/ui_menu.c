#include <stdio.h>
#include "lvgl.h"
#include "usb.h"
#include "text.h"
#include "mh2457.h"
#include "tmr.h"
#include "rtt_cli.h"

int menu_select = 0;

// ==================== 1. 外部接口与全局句柄声明 ====================
extern lv_indev_t *g_indevKeyPad; // 您的键盘设备句柄

extern int language;
extern void factory_mode_hotkey_enable(int enable);

LV_FONT_DECLARE(lv_font_puhui_42);
LV_FONT_DECLARE(lv_font_puhui_40);

extern lv_draw_buf_t menu_back;

extern lv_draw_buf_t menu_bg_buf;

static lv_obj_t *main_menu_scr = NULL;
static lv_group_t *menu_group = NULL;

static lv_obj_t *func_page_scr = NULL;
static lv_group_t *func_group = NULL;

/* 0=主菜单 1=日志 2=重置确认 3=重置进行中 */
static int s_menu_page = 0;
static lv_obj_t *s_lock_tips = NULL;

#define CANVAS_WIDTH 854
#define CANVAS_HEIGHT 480

// 内部事件回调提前声明
static void menu_btn_event_cb(lv_event_t *e);
static void page_usb_event_cb(lv_event_t *e);
static void page_factory_event_cb(lv_event_t *e);
static void open_logs_page(void);
static void open_factory_page(void);
static void ui_menu_lang_reload(void);

// ==================== 2. 主菜单：菜单卡片组件创建 ====================
static lv_obj_t *create_menu_card(lv_obj_t *parent, const char *symbol, const char *text, uint32_t user_data)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 220, 280);
    lv_obj_set_style_pad_all(card, 0, LV_PART_MAIN);

    lv_obj_set_layout(card, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(card, 20, 0);

    // === 未选中状态样式：高透精细亚克力 ===
    lv_obj_set_style_bg_color(card, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(card, LV_OPA_10, LV_PART_MAIN);
    lv_obj_set_style_radius(card, 30, LV_PART_MAIN);

    // 1px 的超细白色精致高光边，固定不参与状态切换
    lv_obj_set_style_border_color(card, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_border_width(card, 1, LV_PART_MAIN);
    lv_obj_set_style_border_opa(card, LV_OPA_20, LV_PART_MAIN);

    lv_obj_set_style_shadow_width(card, 20, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(card, lv_color_hex(0x020205), LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(card, LV_OPA_60, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(card, 8, LV_PART_MAIN);

    static lv_style_transition_dsc_t trans;
    static const lv_style_prop_t props[] = {LV_STYLE_BG_OPA, LV_STYLE_SHADOW_COLOR, LV_STYLE_SHADOW_WIDTH, 0};
    lv_style_transition_dsc_init(&trans, props, lv_anim_path_ease_out, 120, 0, NULL);
    lv_obj_set_style_transition(card, &trans, LV_PART_MAIN);

    // === 选中高亮状态样式 ===
    lv_obj_set_style_bg_opa(card, LV_OPA_20, LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(card, lv_color_white(), LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(card, LV_OPA_20, LV_STATE_FOCUSED);

    // 柔和的雾化冷白光，不加粗线边框
    lv_obj_set_style_shadow_color(card, lv_color_white(), LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(card, 35, LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(card, LV_OPA_30, LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_offset_y(card, 0, LV_STATE_FOCUSED);

    // 白字图标与文本
    lv_obj_t *icon = lv_label_create(card);
    lv_label_set_text(icon, symbol);
#if LV_FONT_MONTSERRAT_48
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_48, LV_PART_MAIN);
#else
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_24, LV_PART_MAIN);
#endif
    lv_obj_set_style_text_color(icon, lv_color_white(), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(card);
    lv_label_set_text(label, text);
#if LV_FONT_MONTSERRAT_24
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, LV_PART_MAIN);
#endif
    lv_obj_set_style_text_color(label, lv_color_hex(0xE2E8F0), LV_PART_MAIN);

    lv_obj_add_flag(card, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(card, (void *)(uintptr_t)user_data);
    lv_obj_add_event_cb(card, menu_btn_event_cb, LV_EVENT_KEY, NULL);

    return card;
}

// ==================== 3. 菜单背景：直接使用静态图片资源，避免无效死代码 ====================
static void draw_static_gradient_bg(lv_obj_t *parent)
{
    lv_obj_t *bg = lv_img_create(parent);
    lv_image_set_src(bg, &menu_bg_buf);
}

// ==================== 4. 主菜单页面初始化 ====================
void ui_menu_init(void)
{
    if (func_group) {
        lv_group_del(func_group);
        func_group = NULL;
    }
    if (func_page_scr) {
        lv_obj_delete(func_page_scr);
        func_page_scr = NULL;
    }
    if (menu_group) {
        lv_group_del(menu_group);
        menu_group = NULL;
    }
    lv_indev_set_group(g_indevKeyPad, NULL);

    s_menu_page = 0;
    s_lock_tips = NULL;

    main_menu_scr = lv_obj_create(NULL);
    lv_screen_load(main_menu_scr);

    // 绘制渐变圆背景
    draw_static_gradient_bg(main_menu_scr);

    menu_group = lv_group_create();
    lv_indev_set_group(g_indevKeyPad, menu_group);

    // 创建充满全屏的透明上层卡片容器
    lv_obj_t * container = lv_obj_create(main_menu_scr);
    lv_obj_set_size(container, 854, 480);
    lv_obj_set_pos(container, 0, 0); 
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(container, 0, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW); 
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(container, 40, 0);

    // 定义需要用到的 3 组文本与图标数据
    const char * icons[] = {LV_SYMBOL_DOWNLOAD, LV_SYMBOL_SAVE, LV_SYMBOL_REFRESH};
    const char * texts[3] = {get_string(language, TEXT_MENU_UPGRADE), get_string(language, TEXT_MENU_LOGS), get_string(language, TEXT_MENU_FACTORY_RESET)};
    const lv_color_t icon_colors[] = {
        lv_color_hex(0x60A95F),
        lv_color_hex(0xD4A53F),
        lv_color_hex(0xC45C5C),
    };
    lv_obj_t * cards[3];

    // 定义卡片的高级平滑样式过渡，120毫秒完成切换
    // static lv_style_transition_dsc_t trans;
    // static const lv_style_prop_t props[] = {LV_STYLE_BG_OPA, LV_STYLE_BORDER_OPA, LV_STYLE_BORDER_COLOR, 0};
    // lv_style_transition_dsc_init(&trans, props, lv_anim_path_ease_out, 120, 0, NULL);

    // 循环创建 3 个大卡片，合并您要求的所有样式修改
        // 最终完美微调版：利用下半区底部垫高，让换行文字整体平滑上移靠近图片
    for (int i = 0; i < 3; i++) {
        cards[i] = lv_obj_create(container);
        lv_obj_set_size(cards[i], 220, 280);
        lv_obj_set_style_pad_all(cards[i], 0, LV_PART_MAIN);
        
        // 卡片本身作为主 Flex 容器
        lv_obj_set_layout(cards[i], LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(cards[i], LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(cards[i], 
                            LV_FLEX_ALIGN_START, 
                            LV_FLEX_ALIGN_CENTER, 
                            LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_gap(cards[i], 0, 0); // 两个半区无缝拼接

        // === 默认未选中状态样式（无阴影、4px 透明边框占位，避免选中时布局跳动） ===
        lv_obj_set_style_bg_color(cards[i], lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(cards[i], LV_OPA_10, LV_PART_MAIN); 
        lv_obj_set_style_radius(cards[i], 30, LV_PART_MAIN);
        lv_obj_set_style_shadow_width(cards[i], 0, LV_PART_MAIN);

        lv_obj_set_style_border_color(cards[i], lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_border_width(cards[i], 1, LV_PART_MAIN);
        lv_obj_set_style_border_opa(cards[i], LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_outline_width(cards[i], 0, LV_PART_MAIN);
        lv_obj_set_style_outline_width(cards[i], 0, LV_STATE_FOCUSED);
        lv_obj_set_style_outline_width(cards[i], 0, LV_STATE_FOCUS_KEY);

        static lv_style_transition_dsc_t card_trans;
        static const lv_style_prop_t card_trans_props[] = {
            LV_STYLE_BG_OPA,
            LV_STYLE_BORDER_OPA,
            LV_STYLE_BORDER_COLOR,
            LV_STYLE_SHADOW_WIDTH,
            LV_STYLE_SHADOW_COLOR,
            LV_STYLE_TRANSLATE_Y,
            0
        };
        lv_style_transition_dsc_init(&card_trans, card_trans_props, lv_anim_path_ease_out, 120, 0, NULL);
        lv_obj_set_style_transition(cards[i], &card_trans, LV_PART_MAIN);

        // === 键盘选中（FOCUSED）高亮样式：4px 白色粗边框 ===
        lv_obj_set_style_bg_opa(cards[i], LV_OPA_20, LV_STATE_FOCUSED); 
        lv_obj_set_style_bg_color(cards[i], lv_color_hex(0xFFFFFF), LV_STATE_FOCUSED);
        lv_obj_set_style_border_color(cards[i], lv_color_hex(0xFFFFFF), LV_STATE_FOCUSED); 
        lv_obj_set_style_border_width(cards[i], 1, LV_STATE_FOCUSED);
        lv_obj_set_style_border_opa(cards[i], LV_OPA_80, LV_STATE_FOCUSED);
        lv_obj_set_style_shadow_color(cards[i], lv_color_hex(0x93C5FD), LV_STATE_FOCUSED);
        lv_obj_set_style_shadow_width(cards[i], 28, LV_STATE_FOCUSED);
        lv_obj_set_style_shadow_opa(cards[i], LV_OPA_30, LV_STATE_FOCUSED);
        lv_obj_set_style_translate_y(cards[i], -4, LV_STATE_FOCUSED);

        // 按下时只保留阴影和位移，边框宽度固定不参与状态切换。
        lv_obj_set_style_shadow_width(cards[i], 28, LV_STATE_PRESSED);
        lv_obj_set_style_translate_y(cards[i], -4, LV_STATE_PRESSED);
        lv_obj_set_style_shadow_width(cards[i], 28, LV_STATE_FOCUSED | LV_STATE_PRESSED);
        lv_obj_set_style_translate_y(cards[i], -4, LV_STATE_FOCUSED | LV_STATE_PRESSED);
        lv_obj_set_style_border_width(cards[i], 4, LV_STATE_FOCUSED | LV_STATE_PRESSED);
        lv_obj_set_style_border_opa(cards[i], LV_OPA_80, LV_STATE_FOCUSED | LV_STATE_PRESSED);

        // ----------------------------------------------------
        // 【上半分区】：高度 140px，锁死图标位置
        // ----------------------------------------------------
        lv_obj_t * upper_half = lv_obj_create(cards[i]);
        lv_obj_remove_style_all(upper_half); 
        lv_obj_set_size(upper_half, LV_PCT(100), 140);
        
        lv_obj_set_layout(upper_half, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(upper_half, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(upper_half, 
                            LV_FLEX_ALIGN_END,       // 纵向：底部对齐
                            LV_FLEX_ALIGN_CENTER,    // 横向：水平居中
                            LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_bottom(upper_half, 10, LV_PART_MAIN); // 距离中轴线留 10 像素间距

        // 创建 64x64 图标符号控件
        lv_obj_t * icon_label = lv_label_create(upper_half);
        lv_label_set_text(icon_label, icons[i]);
        lv_obj_set_style_text_font(icon_label, &lv_font_montserrat_48, LV_PART_MAIN);
        lv_obj_set_style_text_color(icon_label, icon_colors[i], LV_PART_MAIN);
        lv_obj_set_style_text_color(icon_label, lv_color_hex(0xFFFFFF), LV_STATE_FOCUSED);

        // ----------------------------------------------------
        // 【下半分区】：高度 140px，负责文字排版
        // ----------------------------------------------------
        lv_obj_t * lower_half = lv_obj_create(cards[i]);
        lv_obj_remove_style_all(lower_half); 
        lv_obj_set_size(lower_half, LV_PCT(100), 140);
        
        lv_obj_set_layout(lower_half, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(lower_half, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(lower_half, 
                            LV_FLEX_ALIGN_CENTER,    // 纵向：绝对垂直居中
                            LV_FLEX_ALIGN_CENTER,    // 横向：水平居中
                            LV_FLEX_ALIGN_CENTER);
        
        // 使用更稳的下内边距，让文案在不同长度下保持更稳定的垂直位置。
        lv_obj_set_style_pad_bottom(lower_half, 18, LV_PART_MAIN);

        // 创建文本标签
        lv_obj_t * label = lv_label_create(lower_half);
        lv_label_set_text(label, texts[i]); 
        
        lv_obj_set_width(label, LV_PCT(88));
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN); 
        lv_obj_set_style_text_line_space(label, 4, LV_PART_MAIN);

        lv_obj_set_style_text_font(label, &lv_font_puhui_40, LV_PART_MAIN);
        lv_obj_set_style_text_color(label, lv_color_hex(0xE2E8F0), LV_PART_MAIN);
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_STATE_FOCUSED);

        lv_obj_remove_flag(cards[i], LV_OBJ_FLAG_SCROLLABLE);       // 禁止滚动
        lv_obj_remove_flag(cards[i], LV_OBJ_FLAG_SCROLL_ON_FOCUS); // 聚焦时不进行自动滚动对齐
        lv_obj_set_scrollbar_mode(cards[i], LV_SCROLLBAR_MODE_OFF); // 保底关闭卡片自身滚动条

        // 加入键盘管理组与绑定回调
        lv_obj_add_flag(cards[i], LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_user_data(cards[i], (void*)(uintptr_t)(i + 1));
        lv_obj_add_event_cb(cards[i], menu_btn_event_cb, LV_EVENT_KEY, NULL);
        lv_group_add_obj(menu_group, cards[i]);
    }

    // 默认聚焦到第一个卡片
    lv_group_focus_obj(cards[0]);

    rtt_cli_bind_reload(ui_menu_lang_reload);
    factory_mode_hotkey_enable(1);
}

// ==================== 5. 辅助功能页公共按键指引 ====================
static void create_top_key_guide(lv_obj_t *parent, const char *text, int align_right)
{
    lv_obj_t *guide = lv_label_create(parent);
    lv_label_set_text(guide, text);
#if LV_FONT_MONTSERRAT_32
    lv_obj_set_style_text_font(guide, &lv_font_montserrat_32, LV_PART_MAIN);
#else
    lv_obj_set_style_text_font(guide, &lv_font_montserrat_24, LV_PART_MAIN);
#endif
    lv_obj_set_style_text_color(guide, lv_color_hex(0x94A3B8), LV_PART_MAIN);

    if (align_right)
    {
        lv_obj_set_align(guide, LV_ALIGN_TOP_RIGHT);
        lv_obj_set_pos(guide, -30, 20);
    }
    else
    {
        lv_obj_set_align(guide, LV_ALIGN_TOP_LEFT);
        lv_obj_set_pos(guide, 30, 20);
    }
}

// ==================== 5.1 子页创建（供菜单进入 / 语言刷新复用） ====================
static void open_logs_page(void)
{
    if (func_group) {
        lv_group_del(func_group);
        func_group = NULL;
    }
    if (func_page_scr) {
        lv_obj_delete(func_page_scr);
        func_page_scr = NULL;
    }

    func_group = lv_group_create();
    lv_indev_set_group(g_indevKeyPad, func_group);

    func_page_scr = lv_obj_create(NULL);
    lv_screen_load(func_page_scr);

    draw_static_gradient_bg(func_page_scr);

    lv_obj_t *surface = lv_obj_create(func_page_scr);
    lv_obj_set_size(surface, LV_PCT(80), LV_PCT(80));
    lv_obj_center(surface);
    lv_obj_set_style_bg_color(surface, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(surface, LV_OPA_10, LV_PART_MAIN);
    lv_obj_set_style_radius(surface, 24, LV_PART_MAIN);
    lv_obj_set_style_border_width(surface, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(surface, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_border_opa(surface, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(surface, 24, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(surface, lv_color_hex(0x020205), LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(surface, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(surface, 8, LV_PART_MAIN);
    lv_obj_remove_flag(surface, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(surface, 0, LV_PART_MAIN);

    lv_obj_t *back_img = lv_image_create(surface);
    lv_image_set_src(back_img, &menu_back);
    lv_obj_set_style_image_opa(back_img, LV_OPA_90, LV_PART_MAIN);
    lv_obj_align(back_img, LV_ALIGN_TOP_LEFT, 14, 14);

    lv_obj_t *center_cont = lv_obj_create(surface);
    lv_obj_remove_style_all(center_cont);
    lv_obj_set_size(center_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_center(center_cont);
    lv_obj_set_layout(center_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(center_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(center_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(center_cont, 25, 0);

    lv_obj_t *page_icon = lv_label_create(center_cont);
    lv_label_set_text(page_icon, LV_SYMBOL_SAVE);
    lv_obj_set_style_text_font(page_icon, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_style_text_color(page_icon, lv_color_hex(0xD4A53F), LV_PART_MAIN);

    lv_obj_t *tips = lv_label_create(center_cont);
    lv_label_set_text(tips, get_string(language, TEXT_LOGS_NOTICE));
    lv_obj_set_width(tips, LV_PCT(88));
    lv_label_set_long_mode(tips, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(tips, &lv_font_puhui_40, LV_PART_MAIN);
    lv_obj_set_style_text_color(tips, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_line_space(tips, 20, LV_PART_MAIN);
    lv_obj_set_style_text_align(tips, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_group_add_obj(func_group, func_page_scr);
    lv_group_focus_obj(func_page_scr);
    lv_obj_add_event_cb(func_page_scr, page_usb_event_cb, LV_EVENT_KEY, NULL);

    s_menu_page = 1;
    rtt_cli_bind_reload(ui_menu_lang_reload);
    factory_mode_hotkey_enable(1);
}

static void open_factory_page(void)
{
    if (func_group) {
        lv_group_del(func_group);
        func_group = NULL;
    }
    if (func_page_scr) {
        lv_obj_delete(func_page_scr);
        func_page_scr = NULL;
    }

    func_group = lv_group_create();
    lv_indev_set_group(g_indevKeyPad, func_group);

    func_page_scr = lv_obj_create(NULL);
    lv_screen_load(func_page_scr);

    draw_static_gradient_bg(func_page_scr);

    lv_obj_t *surface = lv_obj_create(func_page_scr);
    lv_obj_set_size(surface, LV_PCT(80), LV_PCT(80));
    lv_obj_center(surface);
    lv_obj_set_style_bg_color(surface, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(surface, LV_OPA_10, LV_PART_MAIN);
    lv_obj_set_style_radius(surface, 24, LV_PART_MAIN);
    lv_obj_set_style_border_width(surface, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(surface, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_border_opa(surface, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(surface, 24, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(surface, lv_color_hex(0x020205), LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(surface, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(surface, 8, LV_PART_MAIN);
    lv_obj_remove_flag(surface, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(surface, 0, LV_PART_MAIN);

    lv_obj_t *back_img = lv_image_create(surface);
    lv_image_set_src(back_img, &menu_back);
    lv_obj_set_style_image_opa(back_img, LV_OPA_90, LV_PART_MAIN);
    lv_obj_align(back_img, LV_ALIGN_TOP_LEFT, 14, 14);

    lv_obj_t *center_cont = lv_obj_create(surface);
    lv_obj_remove_style_all(center_cont);
    lv_obj_set_size(center_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_center(center_cont);
    lv_obj_set_layout(center_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(center_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(center_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(center_cont, 25, 0);

    lv_obj_t *page_icon = lv_label_create(center_cont);
    lv_label_set_text(page_icon, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_font(page_icon, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_style_text_color(page_icon, lv_color_hex(0xC45C5C), LV_PART_MAIN);

    lv_obj_t *tips = lv_label_create(center_cont);
    lv_label_set_text(tips, get_string(language, TEXT_FCT_RESET_COMFIRM));
    lv_obj_set_style_text_font(tips, &lv_font_puhui_40, LV_PART_MAIN);
    lv_obj_set_style_text_color(tips, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_line_space(tips, 20, LV_PART_MAIN);
    lv_obj_set_style_text_align(tips, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_group_add_obj(func_group, func_page_scr);
    lv_group_focus_obj(func_page_scr);
    lv_obj_add_event_cb(func_page_scr, page_factory_event_cb, LV_EVENT_KEY, NULL);

    s_menu_page = 2;
    rtt_cli_bind_reload(ui_menu_lang_reload);
    factory_mode_hotkey_enable(1);
}

static void open_factory_lock_page(void)
{
    if (func_group) {
        lv_group_del(func_group);
        func_group = NULL;
    }
    if (func_page_scr) {
        lv_obj_delete(func_page_scr);
        func_page_scr = NULL;
    }

    lv_obj_t *lock_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(lock_scr, lv_color_hex(0x05060C), LV_PART_MAIN);
    lv_screen_load(lock_scr);
    lv_indev_set_group(g_indevKeyPad, NULL);

    lv_obj_t *spinner = lv_spinner_create(lock_scr);
    lv_obj_set_size(spinner, 100, 100);
    lv_obj_center(spinner);
    lv_obj_set_style_arc_color(spinner, lv_color_hex(0xC45C5C), LV_PART_INDICATOR);

    s_lock_tips = lv_label_create(lock_scr);
    lv_label_set_text(s_lock_tips, get_string(language, TEXT_FCT_RESET_PROCESS));
    lv_obj_set_style_text_font(s_lock_tips, &lv_font_puhui_42, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_lock_tips, lv_color_hex(0xC0C0C0), LV_PART_MAIN);
    lv_obj_center(s_lock_tips);
    lv_obj_set_y(s_lock_tips, 90);

    s_menu_page = 3;
    rtt_cli_bind_reload(ui_menu_lang_reload);
    factory_mode_hotkey_enable(1);
}

static void ui_menu_lang_reload(void)
{
    int page = s_menu_page;

    if (page == 3) {
        if (s_lock_tips) {
            lv_label_set_text(s_lock_tips, get_string(language, TEXT_FCT_RESET_PROCESS));
        }
        return;
    }

    ui_menu_init();
    if (page == 1) {
        open_logs_page();
    } else if (page == 2) {
        open_factory_page();
    }
}

// ==================== 6. 主菜单业务触发与事件路由 ====================
static void menu_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *card = lv_event_get_target(e);
    uint32_t key = lv_event_get_key(e);
    uint32_t id = (uint32_t)(uintptr_t)lv_obj_get_user_data(card);

    if (key == LV_KEY_LEFT)
    {
        lv_group_focus_prev(menu_group);
        return;
    }
    if (key == LV_KEY_RIGHT)
    {
        lv_group_focus_next(menu_group);
        return;
    }

    if (key == LV_KEY_ENTER) {
        if (id == 1) {
            /* -------- [功能一]: 进入固件更新 -------- */
            lv_indev_set_group(g_indevKeyPad, NULL); 
            lv_group_delete(menu_group);
            menu_group = NULL;
            menu_select = 1;
        } 
        else if (id == 2) {
            open_logs_page();
            menu_select = 2;
        } 
        else if (id == 3) {
            open_factory_page();
        }
    }
}

// ==================== 7. 子页面按键回调控制 ====================
static void page_usb_event_cb(lv_event_t *e)
{
    uint32_t key = lv_event_get_key(e);
    if (key == LV_KEY_BACKSPACE)
    {
        lv_indev_set_group(g_indevKeyPad, menu_group);
        lv_group_delete(func_group);
        func_group = NULL;
        lv_screen_load(main_menu_scr);
        lv_obj_delete(func_page_scr);
        func_page_scr = NULL;

        s_menu_page = 0;
        rtt_cli_bind_reload(ui_menu_lang_reload);

        /* 停止 USB. */
        usb_stop();
    }
}
static void page_factory_event_cb(lv_event_t *e)
{
    uint32_t key = lv_event_get_key(e);
    if (key == LV_KEY_BACKSPACE)
    {
        lv_indev_set_group(g_indevKeyPad, menu_group);
        lv_group_delete(func_group);
        func_group = NULL;
        lv_screen_load(main_menu_scr);
        lv_obj_delete(func_page_scr);
        func_page_scr = NULL;

        s_menu_page = 0;
        rtt_cli_bind_reload(ui_menu_lang_reload);
        return;
    }
    if (key == LV_KEY_ENTER)
    {
        open_factory_lock_page();
        menu_select = 3;
    }
}