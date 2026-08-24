#include "lvgl.h"
#include "ui_selftest.h"
#include "text.h"
#include "rtt_cli.h"

extern lv_indev_t *g_indevKeyPad;
extern int language;
extern int menu_select;
extern void factory_mode_hotkey_enable(int enable);

static lv_group_t *check_group = NULL;
static int s_app_ok = 0;
static int s_res_ok = 0;

static void selftest_lang_reload(void)
{
    ui_system_self_check_init(s_app_ok, s_res_ok);
}

LV_FONT_DECLARE(lv_font_puhui_42);
LV_FONT_DECLARE(lv_font_puhui_40);

#define COLOR_OK    (0x60A95F)
#define COLOR_FAIL  (0xC45C5C)
#define COLOR_TEXT  (0xE2E8F0)

#define BADGE_SZ     (36)
#define ROW_H        (56)
#define BLOCK_MIN_W  (420)
#define BLOCK_MAX_W  (800)  /* 尽量单行标题，超长仍换行 */

static void check_btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_key_t key = lv_event_get_key(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_KEY && key == LV_KEY_ENTER) {
        lv_obj_add_state(btn, LV_STATE_PRESSED);

        if (check_group) {
            lv_group_del(check_group);
            check_group = NULL;
        }
        lv_indev_set_group(g_indevKeyPad, NULL);
        menu_select = 1;
    }
}

/** 圆圈填满 + 白色镂空 ✓ / ✕ */
static lv_obj_t *create_status_badge(lv_obj_t *parent, int ok)
{
    uint32_t accent = ok ? COLOR_OK : COLOR_FAIL;

    lv_obj_t *badge = lv_obj_create(parent);
    lv_obj_remove_style_all(badge);
    lv_obj_set_size(badge, BADGE_SZ, BADGE_SZ);
    lv_obj_set_style_radius(badge, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(badge, lv_color_hex(accent), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(badge, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(badge, 0, LV_PART_MAIN);
    lv_obj_remove_flag(badge, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *mark = lv_label_create(badge);
    lv_label_set_text(mark, ok ? LV_SYMBOL_OK : LV_SYMBOL_CLOSE);
    lv_obj_set_style_text_font(mark, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_set_style_text_color(mark, lv_color_white(), LV_PART_MAIN);
    lv_obj_center(mark);

    return badge;
}

static void create_status_row(lv_obj_t *parent, int ok, const char *name)
{
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_size(row, LV_PCT(100), ROW_H);
    lv_obj_set_layout(row, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(row, 18, 0);

    create_status_badge(row, ok);

    lv_obj_t *label = lv_label_create(row);
    lv_label_set_text(label, name);
    lv_obj_set_flex_grow(label, 1);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(label, &lv_font_puhui_40, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
}

void ui_system_self_check_init(int app_ok, int res_ok)
{
    s_app_ok = app_ok;
    s_res_ok = res_ok;

    if (check_group) {
        lv_group_del(check_group);
        check_group = NULL;
    }
    lv_indev_set_group(g_indevKeyPad, NULL);

    lv_obj_t *scr = lv_obj_create(NULL);
    lv_screen_load(scr);

    /* 纯黑底 + 一层白色透明 */
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *veil = lv_obj_create(scr);
    lv_obj_remove_style_all(veil);
    lv_obj_set_size(veil, 854, 480);
    lv_obj_set_pos(veil, 0, 0);
    lv_obj_set_style_bg_color(veil, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(veil, LV_OPA_10, LV_PART_MAIN);
    lv_obj_remove_flag(veil, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *content = lv_obj_create(scr);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, 854, 480);
    lv_obj_set_pos(content, 0, 0);
    lv_obj_set_layout(content, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(content, 36, 0);
    lv_obj_remove_flag(content, LV_OBJ_FLAG_SCROLLABLE);

    /* 标题 + 列表同一左对齐块；宽度按标题单行测算，尽量不换行 */
    const char *title_txt = get_string(language, TEXT_SELFTEST_TITLE);
    lv_point_t title_sz;
    lv_text_get_size(&title_sz, title_txt, &lv_font_puhui_42, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

    int32_t block_w = title_sz.x;
    if (block_w < BLOCK_MIN_W) {
        block_w = BLOCK_MIN_W;
    }
    if (block_w > BLOCK_MAX_W) {
        block_w = BLOCK_MAX_W;
    }

    lv_obj_t *block = lv_obj_create(content);
    lv_obj_remove_style_all(block);
    lv_obj_set_size(block, block_w, LV_SIZE_CONTENT);
    lv_obj_set_layout(block, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(block, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(block, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_gap(block, 36, 0);
    lv_obj_remove_flag(block, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title = lv_label_create(block);
    lv_label_set_text(title, title_txt);
    lv_obj_set_width(title, LV_PCT(100));
    lv_label_set_long_mode(title, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(title, &lv_font_puhui_42, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

    lv_obj_t *list = lv_obj_create(block);
    lv_obj_remove_style_all(list);
    lv_obj_set_size(list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_layout(list, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(list, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_gap(list, 12, 0);

    create_status_row(list, app_ok, get_string(language, TEXT_SELFTEST_APP));
    create_status_row(list, res_ok, get_string(language, TEXT_SELFTEST_RES));

    /* 去升级按钮 */
    lv_obj_t *btn = lv_button_create(content);
    lv_obj_set_size(btn, LV_SIZE_CONTENT, 64);
    lv_obj_set_style_pad_hor(btn, 32, LV_PART_MAIN);
    lv_obj_set_style_pad_ver(btn, 12, LV_PART_MAIN);
    lv_obj_set_style_radius(btn, 32, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(btn, 0, LV_STATE_FOCUSED);
    lv_obj_set_style_outline_width(btn, 0, LV_STATE_FOCUS_KEY);
    lv_obj_set_style_bg_opa(btn, LV_OPA_10, LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(btn, lv_color_white(), LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(btn, LV_OPA_20, LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(btn, LV_OPA_20, LV_STATE_FOCUSED | LV_STATE_PRESSED);

    lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, get_string(language, TEXT_SELFTEST_UPDATE));
    lv_obj_center(btn_label);
    lv_obj_set_style_text_font(btn_label, &lv_font_puhui_40, LV_PART_MAIN);
    lv_obj_set_style_text_color(btn_label, lv_color_white(), LV_PART_MAIN);

    check_group = lv_group_create();
    lv_indev_set_group(g_indevKeyPad, check_group);
    lv_group_add_obj(check_group, btn);
    lv_group_focus_obj(btn);
    lv_obj_add_event_cb(btn, check_btn_event_cb, LV_EVENT_KEY, NULL);

    rtt_cli_bind_reload(selftest_lang_reload);
    factory_mode_hotkey_enable(0);
}
