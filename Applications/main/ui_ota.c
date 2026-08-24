#include "lvgl.h"
#include "ui_ota.h"
#include "text.h"
#include "rtt_cli.h"

extern void factory_mode_hotkey_enable(int enable);

#define SURFACE_W  (LV_PCT(80))
#define SURFACE_H  (220)

static lv_obj_t *bg  = NULL;
static lv_obj_t *bg1 = NULL;
static lv_obj_t *process = NULL;
static lv_obj_t *surface = NULL;
static lv_obj_t *cont = NULL;
static lv_obj_t *info_pct = NULL;
static lv_obj_t *info_tips = NULL;
static lv_obj_t *bar = NULL;

static text_id_e s_tips_id = TEXT_OTA_STANDBY;
static ui_color_e s_tips_color = COLOR_WHITE;

extern lv_draw_buf_t update_bg_buf;
extern lv_draw_buf_t update_progress_bg_buf;
extern lv_draw_buf_t update_progress_fg_buf;

extern int language;

LV_FONT_DECLARE(lv_font_puhui_42);
LV_FONT_DECLARE(lv_font_puhui_40);

/* puhui 缺字形时挂 montserrat fallback；字库补全后仍可保留 */
static lv_font_t font_ota_tips;

static void ota_lang_reload(void)
{
    if (info_tips == NULL) {
        return;
    }
    if (s_tips_id != TEXT_ID_RSVD) {
        lv_label_set_text(info_tips, get_string(language, s_tips_id));
    }
}

void ui_ota_init(void)
{
    lv_obj_t *scr = lv_scr_act();

    font_ota_tips = lv_font_puhui_40;
    font_ota_tips.fallback = &lv_font_montserrat_42;

    bg  = lv_image_create(scr);
    bg1 = lv_image_create(scr);
    process = lv_image_create(scr);
    lv_image_set_src(bg, &update_bg_buf);

    /* 半透明 surface：先创建，稍后按文字/进度条实际区域贴到其下方 */
    surface = lv_obj_create(scr);
    lv_obj_set_size(surface, SURFACE_W, SURFACE_H);
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
    lv_obj_set_style_pad_all(surface, 0, LV_PART_MAIN);
    lv_obj_remove_flag(surface, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(surface, LV_OBJ_FLAG_CLICKABLE);

    /* 百分比 + 提示语：保持原先居中并下移 70 的位置 */
    cont = lv_obj_create(scr);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    info_pct = lv_label_create(cont);
    lv_label_set_text(info_pct, " ");
    lv_obj_set_style_text_font(info_pct, &lv_font_puhui_42, 0);
    lv_obj_set_style_text_color(info_pct, lv_color_hex(0xC0C0C0), 0);

    info_tips = lv_label_create(cont);
    s_tips_id = TEXT_OTA_STANDBY;
    s_tips_color = COLOR_WHITE;
    lv_label_set_text(info_tips, get_string(language, TEXT_OTA_STANDBY));
    lv_obj_set_style_text_font(info_tips, &font_ota_tips, 0);
    lv_obj_set_style_text_color(info_tips, lv_color_hex(0xC0C0C0), 0);

    lv_obj_center(cont);
    lv_obj_set_y(cont, lv_obj_get_y_aligned(cont) + 70);

    /* 进度条：保持原先绝对坐标 */
    bar = lv_bar_create(scr);
    lv_obj_set_style_radius(bar, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 0, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(bar, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(bar, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_PART_INDICATOR);

    lv_obj_set_size(bar, 620, 28);
    lv_obj_set_pos(bar, 117, 390);

    lv_obj_set_style_bg_image_src(bar, &update_progress_bg_buf, LV_PART_MAIN);
    lv_obj_set_style_bg_image_tiled(bar, false, LV_PART_MAIN);
    lv_obj_set_style_bg_image_opa(bar, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_set_style_bg_image_src(bar, &update_progress_fg_buf, LV_PART_INDICATOR);
    lv_obj_set_style_bg_image_tiled(bar, false, LV_PART_INDICATOR);
    lv_obj_set_style_height(bar, LV_PCT(100), LV_PART_INDICATOR);
    lv_obj_set_style_bg_image_opa(bar, LV_OPA_COVER, LV_PART_INDICATOR);

    lv_obj_set_style_clip_corner(bar, true, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(bar, true, LV_PART_INDICATOR);
    lv_bar_set_range(bar, 0, 100);

    /* surface 贴在百分比与进度条下方（背后），文字/进度条坐标不变 */
    lv_obj_update_layout(scr);
    {
        const lv_coord_t pad = 28;
        lv_coord_t top = lv_obj_get_y(cont) - pad;
        lv_coord_t bottom = lv_obj_get_y(bar) + lv_obj_get_height(bar) + pad;
        if (top < 0) top = 0;
        lv_obj_set_height(surface, bottom - top);
        lv_obj_align(surface, LV_ALIGN_TOP_MID, 0, top);
        lv_obj_move_to_index(surface, lv_obj_get_index(bg) + 1);
    }

    rtt_cli_bind_reload(ota_lang_reload);
    factory_mode_hotkey_enable(0);
}

void process_update(unsigned int pct)
{
    unsigned int bar_pct;
    unsigned int whole;
    unsigned int frac;

    if (pct > 10000) pct = 10000;

    bar_pct = pct / 100;
    whole = pct / 100;
    frac = pct % 100;

    ui_set_pct_visible(1);
    ui_set_pct_color(COLOR_WHITE);
    lv_bar_set_value(bar, bar_pct, LV_ANIM_OFF);

    if (pct == 0 || pct == 10000) {
        lv_label_set_text_fmt(info_pct, "%d%%", whole);
    } else {
        lv_label_set_text_fmt(info_pct, "%d.%02d%%", whole, frac);
    }
}

static lv_color_t ui_get_color(ui_color_e color)
{
    switch (color) {
    default:
    case COLOR_WHITE:
        /* 常規提示：浅灰 */
        return lv_color_make(0xC0, 0xC0, 0xC0);
    case COLOR_YELLOW:
        /* 警告黄 */
        return lv_color_make(0xF5, 0xB7, 0x33);
    case COLOR_RED:
        /* 错误红：与进度条同级饱和度，略亮，深色底上更清晰 */
        return lv_color_make(0xFF, 0x5C, 0x6A);
    case COLOR_GREEN:
        /* 成功色：与进度条电光蓝统一 */
        return lv_color_make(0x2E, 0xB5, 0xFF);
    }
}

void ui_set_notice(char *msg, ui_color_e color)
{
    if (msg) {
        s_tips_id = TEXT_ID_RSVD; /* 动态/英文硬编码文案，切语言时不覆盖 */
        s_tips_color = color;
        lv_obj_set_style_text_color(info_tips, ui_get_color(color), 0);
        lv_label_set_text(info_tips, msg);
    }
}

void ui_set_notice_id(text_id_e id, ui_color_e color)
{
    s_tips_id = id;
    s_tips_color = color;
    if (info_tips) {
        lv_obj_set_style_text_color(info_tips, ui_get_color(color), 0);
        lv_label_set_text(info_tips, get_string(language, id));
    }
}

void ui_set_pct_color(ui_color_e color)
{
    lv_obj_set_style_text_color(info_pct, ui_get_color(color), 0);
}

void ui_set_pct_visible(unsigned char visible)
{
    if (visible) {
        lv_obj_set_style_opa(info_pct, LV_OPA_100, LV_PART_MAIN);
    } else {
        lv_obj_set_style_opa(info_pct, LV_OPA_TRANSP, LV_PART_MAIN);
    }
}
