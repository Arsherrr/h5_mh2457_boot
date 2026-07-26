#include "lvgl.h"

#define PROCESS_WIDTH  (620)

static lv_obj_t *bg  = NULL;
static lv_obj_t *bg1 = NULL;
static lv_obj_t *process = NULL;

void ui_init(void)
{
    lv_obj_t * scr = lv_scr_act();
    bg  = lv_image_create(scr);
    bg1 = lv_image_create(scr);
    process = lv_image_create(scr);

    lv_image_set_src(bg, "A:/customer/H5/res/image/JL/update/update_bg_dark.jpg");

    lv_obj_set_pos(bg1, 117, 360);
    lv_image_set_src(bg1, "A:/customer/H5/res/image/JL/update/update_progress_bg_dark.png");

    lv_obj_set_pos(process, 117, 360);
    lv_obj_set_width(process, 0);
    lv_image_set_src(process, "A:/customer/H5/res/image/JL/update/update_progress_fg_dark.png");
}

void process_update(uint8_t pct)
{
    if (pct > 100) pct = 100;
    
    lv_obj_set_pos(process, 117, 360);
    lv_obj_set_width(process, (int32_t)(PROCESS_WIDTH * 100.0f / (int)pct));
}
