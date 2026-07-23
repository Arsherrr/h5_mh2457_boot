#include "lvgl.h"
#include "ui_ota.h"
#include "text.h"

#define PROCESS_WIDTH  (620)

static lv_obj_t *bg  = NULL;
static lv_obj_t *bg1 = NULL;
static lv_obj_t *process = NULL;
static lv_obj_t *cont = NULL;
static lv_obj_t *info_pct = NULL;
static lv_obj_t *info_tips = NULL;
static lv_obj_t *bar = NULL;

extern lv_draw_buf_t update_bg_buf;
extern lv_draw_buf_t update_progress_bg_buf;
extern lv_draw_buf_t update_progress_fg_buf;

extern int language;

LV_FONT_DECLARE(lv_font_puhui_42);
LV_FONT_DECLARE(lv_font_puhui_40);

void ui_ota_init(void)
{
    lv_obj_t * scr = lv_scr_act();
    bg  = lv_image_create(scr);
    bg1 = lv_image_create(scr);
    process = lv_image_create(scr);

    lv_image_set_src(bg, &update_bg_buf);

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

    /* 设置 Bar 背景. */
    lv_obj_set_style_bg_image_src(bar, &update_progress_bg_buf, LV_PART_MAIN);
    lv_obj_set_style_bg_image_tiled(bar, false, LV_PART_MAIN);
    lv_obj_set_style_bg_image_opa(bar, LV_OPA_COVER, LV_PART_MAIN);

    /* 设置进度部分. */
    lv_obj_set_style_bg_image_src(bar, &update_progress_fg_buf, LV_PART_INDICATOR);
    lv_obj_set_style_bg_image_tiled(bar, false, LV_PART_INDICATOR);
    lv_obj_set_style_height(bar, LV_PCT(100), LV_PART_INDICATOR); 
    lv_obj_set_style_bg_image_opa(bar, LV_OPA_COVER, LV_PART_INDICATOR);

    lv_obj_set_style_clip_corner(bar, true, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(bar, true, LV_PART_INDICATOR);
    
    lv_bar_set_range(bar, 0, 100);

    /* 显示文本. */
    lv_obj_t * cont = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(cont); 
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX); 
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, 
                        LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

    /* Progress. */
    info_pct = lv_label_create(cont);
    lv_label_set_text(info_pct, " ");
    lv_obj_set_style_text_font(info_pct, &lv_font_puhui_42, 0);
    lv_obj_set_style_text_color(info_pct, lv_color_hex(0xC0C0C0), 0);

    /* Information. */
    info_tips = lv_label_create(cont);
    lv_label_set_text(info_tips, get_string(language, TEXT_OTA_STANDBY));
    lv_obj_set_style_text_font(info_tips, &lv_font_puhui_40, 0);
    lv_obj_set_style_text_color(info_tips, lv_color_hex(0xC0C0C0), 0);

    lv_obj_center(cont);
    lv_obj_set_y(cont, lv_obj_get_y_aligned(cont) + 70);
}

void process_update(uint8_t pct)
{
    if (pct > 100) pct = 100;

    ui_set_pct_visible(1);
    ui_set_pct_color(COLOR_WHITE);
    lv_bar_set_value(bar, pct, LV_ANIM_OFF);

    lv_label_set_text_fmt(info_pct, "%d%%", pct);
}

static lv_color_t ui_get_color(ui_color_e color)
{
    switch (color) {
    defualt:
    case COLOR_WHITE:
        /* 灰色 */
        return lv_color_make(0xC0, 0xC0, 0xC0);
    case COLOR_YELLOW:
        /* 黄色. */
        return lv_color_make(0xF5, 0xB7, 0x33);
    case COLOR_RED:
        /* 红色. */
        return lv_color_make(0xE6, 0x47, 0x5A);
    case COLOR_GREEN:
        /* 绿色. */
        return lv_color_make(0x1B, 0xC2, 0x5E);
    }
}

void ui_set_notice(char *msg, ui_color_e color)
{
    if (msg) {
        lv_obj_set_style_text_color(info_tips, ui_get_color(color), 0);
        lv_label_set_text(info_tips, msg);
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
