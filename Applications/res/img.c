#include <stdio.h>
#include "lvgl.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/libs/lodepng/lodepng.h"
#include "jpeg_hw.h"

extern const lv_image_dsc_t update_progress_bg_dark;
extern const lv_image_dsc_t update_progress_fg_dark;
extern const lv_image_dsc_t back;
extern const lv_image_dsc_t menu_bg;

lv_draw_buf_t update_progress_bg_buf;
lv_draw_buf_t update_progress_fg_buf;

lv_draw_buf_t menu_back;
lv_draw_buf_t menu_bg_buf;

#define RAW_DATA_SIZE  ( 854 * 480 * 3 )
static uint8_t * jpg_rgb888 = NULL;

int jpg_init(lv_image_dsc_t * desc, lv_draw_buf_t *decoded)
{
    if (desc == NULL || decoded == NULL) return -1;

    /* JPEG 硬件解码. */
    if (!jpeg_hw_dec_handler(desc->data, 
                             desc->data_size,
                             desc->header.w, 
                             desc->header.h,
                             jpg_rgb888, 
                             RAW_DATA_SIZE,
                             DMA2D_OUTPUT_MODE_RGB888,
                             DMA2D_ROTATION_0)) {
        printf("[IMG] jpeg decode failed.\n");
        lv_free(jpg_rgb888);
        return -2;
    }

    memset(decoded, 0, sizeof(lv_draw_buf_t));
    decoded->header.magic   = LV_IMAGE_HEADER_MAGIC;
    decoded->header.cf      = LV_COLOR_FORMAT_RGB888;
    decoded->header.w       = desc->header.w;
    decoded->header.h       = desc->header.h;
    decoded->header.stride  = LV_DRAW_BUF_STRIDE(desc->header.w, LV_COLOR_FORMAT_RGB888);
    decoded->header.flags   = 0;
    decoded->data_size      = RAW_DATA_SIZE;
    decoded->data           = jpg_rgb888;
    decoded->unaligned_data = (void *)jpg_rgb888;
    decoded->handlers       = lv_draw_buf_get_image_handlers();

    return 0;
}

static void convert_argb8888_to_lvgl_bgra(uint8_t *img_p, uint32_t px_cnt)
{
    lv_color32_t *img_argb = (lv_color32_t *)img_p;
    for (uint32_t i = 0; i < px_cnt; i++) {
        uint8_t blue = img_argb[i].blue;
        img_argb[i].blue = img_argb[i].red;
        img_argb[i].red = blue;
    }
}

static int png_init(lv_image_dsc_t * desc, lv_draw_buf_t *out)
{
    if (desc == NULL) return -1;

    unsigned png_width = 0;
    unsigned png_height = 0;

    /* PNG 软件解码. */
    lv_draw_buf_t * decoded = NULL;
    unsigned error = lodepng_decode32((unsigned char **)&decoded, 
                                      &png_width, 
                                      &png_height, 
                                      desc->data, 
                                      desc->data_size);

    if (error || decoded == NULL || decoded->data == NULL) {
        if (decoded != NULL) lv_draw_buf_destroy(decoded);
        printf("[IMG] png decode failed error=%u %s\n", error, lodepng_error_text(error));
        return -1;
    }

    if (png_width != desc->header.w || png_height != desc->header.h) {
        printf("[IMG] png size mismatch res=%ux%u png=%ux%u\n",
                   (unsigned int)desc->header.w,
                   (unsigned int)desc->header.h,
                   png_width,
                   png_height);
                
        return -1;
    }
    
    convert_argb8888_to_lvgl_bgra(decoded->data, desc->header.w * desc->header.h);

    *out = *decoded;
    
    return 0;
}

int img_init(void)
{
    /* 申请内存 (不释放). */
    jpg_rgb888 = (uint8_t *)lv_malloc(RAW_DATA_SIZE);
    if (jpg_rgb888 == NULL) {
        printf("[IMG] JPG OOM.\n");
        return -1;
    }

    jpg_init((lv_image_dsc_t *)&menu_bg, &menu_bg_buf);
    
    png_init((lv_image_dsc_t *)&update_progress_bg_dark, &update_progress_bg_buf);
    png_init((lv_image_dsc_t *)&update_progress_fg_dark, &update_progress_fg_buf);

    /* 菜单返回图标 */
    png_init((lv_image_dsc_t *)&back, &menu_back);

    return 0;
}
