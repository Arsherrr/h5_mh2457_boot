#include "res_fs.h"
#include "img_cache.h"
#include "lvgl.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/draw/lv_image_decoder_private.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/libs/lodepng/lodepng.h"
#include "rtthread.h"
#include <string.h>
#include <stdint.h>
#include "log.h"

#define RES_BIN_BASE_ADDR 0x0910B600UL

/* lodepng_decode32() in LVGL's lodepng.c allocates with lv_malloc(). */
#define lodepng_free lv_free

/* [ 配置 ] 是否开启 LOG 日志. */
#define USE_PNG_LOG 1

/* [ 配置 ] LOG 等级. 
 * LOG_LEVEL_TRACE
 * LOG_LEVEL_INFO
 * LOG_LEVEL_WARN
 * LOG_LEVEL_ERROR
 * LOG_LEVEL_NONE
*/
#define PNG_LOG_LEVEL ( LOG_LEVEL_WARN )

#if USE_PNG_LOG
#define _PNG_LOG_LEVEL PNG_LOG_LEVEL
#else
#define _PNG_LOG_LEVEL LOG_LEVEL_NONE
#endif

#if _PNG_LOG_LEVEL <= LOG_LEVEL_TRACE
#define PNG_LOG_TRACE(...) rt_kprintf("[PNG DEC] " __VA_ARGS__)
#else
#define PNG_LOG_TRACE(...)
#endif
#if _PNG_LOG_LEVEL <= LOG_LEVEL_INFO
#define PNG_LOG_INFO(...) rt_kprintf("[PNG DEC] " __VA_ARGS__)
#else
#define PNG_LOG_INFO(...)
#endif
#if _PNG_LOG_LEVEL <= LOG_LEVEL_WARN
#define PNG_LOG_WARN(...) rt_kprintf("[PNG DEC] [WARN] " __VA_ARGS__)
#else
#define PNG_LOG_WARN(...)
#endif
#if _PNG_LOG_LEVEL <= LOG_LEVEL_ERROR
#define PNG_LOG_ERROR(...) rt_kprintf("[PNG DEC] [ERROR] " __VA_ARGS__)
#else
#define PNG_LOG_ERROR(...)
#endif

//typedef struct {
//    uint32_t magic;
//    uint16_t version;
//    uint16_t count;
//    uint32_t header_size;
//    uint32_t data_offset;
//} __attribute__((packed)) jl_res_bin_header_t;

static const jl_res_bin_header_t *s_hdr = (const jl_res_bin_header_t *)RES_BIN_BASE_ADDR;
static const uint8_t *s_bin_base = (const uint8_t *)RES_BIN_BASE_ADDR;

typedef struct {
    const jl_resource_info_t *res;
    uint8_t from_img_cache;
} png_session_t;

/* 单张 PNG 的动态缓存, 首次解码后复用. */
static const jl_resource_info_t *s_cached_res = RT_NULL;
static lv_draw_buf_t *s_cached_image = RT_NULL;
static uint8_t *s_cached_raw_buf = RT_NULL;
static uint8_t s_cache_in_use = 0;
static uint8_t s_cached_from_img_cache = 0;

extern void *sdram_malloc(rt_size_t size);
extern void sdram_free(void *ptr);

static void free_png_session(png_session_t *session)
{
    if (session == RT_NULL) return;
    rt_free(session);
}

static void free_cached_png(void)
{
    if (s_cached_image && !s_cached_from_img_cache) {
        lv_draw_buf_destroy(s_cached_image);
    }
    s_cached_image = RT_NULL;
    s_cached_raw_buf = RT_NULL;
    s_cached_res = RT_NULL;
    s_cached_from_img_cache = 0;
}

static uint16_t rgba8888_to_rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)((r & 0xF8) << 8) |
           (uint16_t)((g & 0xFC) << 3) |
           (uint16_t)(b >> 3);
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

static uint32_t png_read_be32(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) |
           ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) |
           (uint32_t)p[3];
}

static lv_result_t decoder_info_cb(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc, lv_image_header_t *header)
{
    LV_UNUSED(decoder);
    if (dsc == RT_NULL || dsc->src == RT_NULL || header == RT_NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
    const jl_resource_info_t *res = find_res_by_path(path);
    if (res == RT_NULL || res->format != 2) return LV_RESULT_INVALID;

    const uint8_t *png_src_ptr = s_bin_base + s_hdr->data_offset + res->offset;
    static const uint8_t png_magic[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    if (res->size < 24u || memcmp(png_src_ptr, png_magic, sizeof(png_magic)) != 0) return LV_RESULT_INVALID;

    uint32_t png_width = png_read_be32(&png_src_ptr[16]);
    uint32_t png_height = png_read_be32(&png_src_ptr[20]);

    header->magic = LV_IMAGE_HEADER_MAGIC;
    header->cf = LV_COLOR_FORMAT_ARGB8888;
    header->w = png_width;
    header->h = png_height;
    header->stride = png_width * 4u;
    header->flags = 0;

    PNG_LOG_INFO("info hit: id=%u src=%s cf=%u w=%u h=%u stride=%u res_w=%u res_h=%u\n",
               res->id, res->source_path,
               (unsigned int)header->cf,
               (unsigned int)header->w,
               (unsigned int)header->h,
               (unsigned int)header->stride,
               (unsigned int)res->width,
               (unsigned int)res->height);
    return LV_RESULT_OK;
}

static lv_result_t decoder_open_cb(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc)
{
    LV_UNUSED(decoder);
    if (dsc == RT_NULL || dsc->src == RT_NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
    const jl_resource_info_t *res = find_res_by_path(path);
    if (res == RT_NULL || res->format != 2) return LV_RESULT_INVALID;

    PNG_LOG_INFO("open_cb: %s\n", path ? path : "(null)");


    if (s_cache_in_use) {
        PNG_LOG_WARN("cache busy\n");
        return LV_RESULT_INVALID;
    }
    s_cache_in_use = 1;

    free_cached_png();

    png_session_t *session = (png_session_t *)rt_malloc(sizeof(png_session_t));
    if (session == RT_NULL) {
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }
    memset(session, 0, sizeof(*session));
    session->res = res;

    const img_cache_entry_t *cache_entry = img_cache_get_by_id(res->id);
    if (cache_entry != RT_NULL && cache_entry->bitmap != RT_NULL) {
        session->from_img_cache = 1;
        s_cached_from_img_cache = 1;
        s_cached_image = (lv_draw_buf_t *)&cache_entry->draw_buf;
        s_cached_raw_buf = cache_entry->bitmap;
        s_cached_res = res;
        dsc->header = s_cached_image->header;
        dsc->decoded = s_cached_image;
        dsc->user_data = session;

        PNG_LOG_INFO("img cache hit: id=%u src=%s cf=%u w=%u h=%u stride=%u data_size=%u img=%p data=%p\n",
                   res->id, res->source_path,
                   (unsigned int)s_cached_image->header.cf,
                   (unsigned int)s_cached_image->header.w,
                   (unsigned int)s_cached_image->header.h,
                   (unsigned int)s_cached_image->header.stride,
                   (unsigned int)s_cached_image->data_size,
                   s_cached_image,
                   s_cached_raw_buf);
        return LV_RESULT_OK;
    }

    const uint8_t *png_src_ptr = s_bin_base + s_hdr->data_offset + res->offset;
    const uint32_t png_src_len = res->size;

    unsigned png_width = 0;
    unsigned png_height = 0;
    lv_draw_buf_t *decoded = RT_NULL;
    unsigned error = lodepng_decode32((unsigned char **)&decoded, &png_width, &png_height, png_src_ptr, png_src_len);
    if (error || decoded == RT_NULL) {
        PNG_LOG_ERROR("lodepng decode failed: %s error=%u %s\n",
                   path, error, lodepng_error_text(error));
        free_png_session(session);
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }

    if (png_width != res->width || png_height != res->height) {
        PNG_LOG_ERROR("size mismatch: res=%ux%u png=%ux%u src=%s, use png header\n",
                   (unsigned int)res->width, (unsigned int)res->height,
                   png_width, png_height, path);
    }

    convert_argb8888_to_lvgl_bgra(decoded->data, (uint32_t)png_width * (uint32_t)png_height);

    s_cached_from_img_cache = 0;
    s_cached_image = decoded;
    s_cached_raw_buf = decoded->data;
    s_cached_res = res;
    dsc->header = s_cached_image->header;
    dsc->decoded = s_cached_image;
    dsc->user_data = session;

    PNG_LOG_INFO("open ok: id=%u src=%s cf=%u w=%u h=%u stride=%u data_size=%u img=%p data=%p\n",
               res->id, res->source_path,
               (unsigned int)s_cached_image->header.cf,
               (unsigned int)s_cached_image->header.w,
               (unsigned int)s_cached_image->header.h,
               (unsigned int)s_cached_image->header.stride,
               (unsigned int)s_cached_image->data_size,
               s_cached_image,
               s_cached_raw_buf);
    return LV_RESULT_OK;
}

static void decoder_close_cb(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc)
{
    LV_UNUSED(decoder);
    if (dsc == RT_NULL) return;

    png_session_t *session = (png_session_t *)dsc->user_data;
    if (session) {
        PNG_LOG_INFO("close_cb: session=%p\n", session);
        free_png_session(session);
        dsc->user_data = NULL;
    }
    s_cache_in_use = 0;
    dsc->decoded = NULL;
}

void png_lvgl_decoder_init(void)
{
    lv_image_decoder_t *decoder = lv_image_decoder_create();
    if (decoder == RT_NULL) {
        PNG_LOG_ERROR("create png decoder failed\n");
        return;
    }

    lv_image_decoder_set_info_cb(decoder, decoder_info_cb);
    lv_image_decoder_set_open_cb(decoder, decoder_open_cb);
    lv_image_decoder_set_close_cb(decoder, decoder_close_cb);
    PNG_LOG_INFO("png decoder registered\n");
}
