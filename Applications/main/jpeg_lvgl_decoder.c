#include "jpeg_lvgl_decoder.h"
#include "jpeg_hw.h"
#include "res.h"
#include "lvgl.h"
#include "res_fs.h"
#include "log.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/draw/lv_image_decoder_private.h"
#include "lvgl.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define JPEG_DEC_BUF_SIZE  (855 * 480 * 3)

/* [ 配置 ] 是否开启 LOG 日志. */
#define USE_JPEG_LOG 1

/* [ 配置 ] LOG 等级. 
 * LOG_LEVEL_TRACE
 * LOG_LEVEL_INFO
 * LOG_LEVEL_WARN
 * LOG_LEVEL_ERROR
 * LOG_LEVEL_NONE
 */
#define JPEG_LOG_LEVEL ( LOG_LEVEL_WARN )

#if USE_JPEG_LOG
#define _JPEG_LOG_LEVEL JPEG_LOG_LEVEL
#else
#define _JPEG_LOG_LEVEL LOG_LEVEL_NONE
#endif

#if _JPEG_LOG_LEVEL <= LOG_LEVEL_TRACE
#define JPEG_LOG_TRACE(...) printf("[JPEG DEC] " __VA_ARGS__)
#else
#define JPEG_LOG_TRACE(...)
#endif
#if _JPEG_LOG_LEVEL <= LOG_LEVEL_INFO
#define JPEG_LOG_INFO(...) printf("[JPEG DEC] " __VA_ARGS__)
#else
#define JPEG_LOG_INFO(...)
#endif
#if _JPEG_LOG_LEVEL <= LOG_LEVEL_WARN
#define JPEG_LOG_WARN(...) printf("[JPEG DEC] [WARN] " __VA_ARGS__)
#else
#define JPEG_LOG_WARN(...)
#endif
#if _JPEG_LOG_LEVEL <= LOG_LEVEL_ERROR
#define JPEG_LOG_ERROR(...) printf("[JPEG DEC] [ERROR] " __VA_ARGS__)
#else
#define JPEG_LOG_ERROR(...)
#endif

typedef struct {
    const jl_resource_info_t *res;
    const uint8_t *jpeg_src_ptr;
    uint32_t jpeg_src_len;
    lv_image_dsc_t *image;
    uint8_t *raw_buf;
} jpeg_session_t;

static void free_jpeg_session(jpeg_session_t *session);

/* 单张 JPEG 的动态缓存, 首次解码后复用. */
static const jl_resource_info_t *s_cached_res = NULL;
static lv_image_dsc_t *s_cached_image = NULL;
static uint8_t *s_cached_raw_buf = NULL;
static uint8_t s_cache_in_use = 0;

static void free_jpeg_session(jpeg_session_t *session)
{
    if (session == NULL) return;
    lv_free(session);
}

static void free_cached_jpeg(void)
{
    if (s_cached_image) {
        lv_free(s_cached_image);
        s_cached_image = NULL;
    }
    s_cached_res = NULL;
}

static lv_result_t decoder_info_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc, lv_image_header_t * header)
{
    LV_UNUSED(decoder);
    if (dsc == NULL || dsc->src == NULL || header == NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;

    JPEG_LOG_TRACE("info_cb: %s\n", path ? path : "(null)");

    const jl_resource_info_t *res = find_res_by_path(path);
    if (res == NULL || res->format != 1) return LV_RESULT_INVALID;

    header->magic = LV_IMAGE_HEADER_MAGIC;
    header->cf = LV_COLOR_FORMAT_RGB888;
    header->w = res->width;
    header->h = res->height;
    header->stride = LV_DRAW_BUF_STRIDE(res->width, LV_COLOR_FORMAT_RGB888);
    header->flags = 0;

    JPEG_LOG_TRACE("info hit: id=%u src=%s cf=%u w=%u h=%u data_size=%u\n",
               res->id, res->source_path,
               (unsigned int)header->cf,
               (unsigned int)header->w,
               (unsigned int)header->h,
               (unsigned int)res->size);

    return LV_RESULT_OK;
}

static lv_result_t decoder_open_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    LV_UNUSED(decoder);
    if (dsc == NULL || dsc->src == NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
    const jl_resource_info_t *res = find_res_by_path(path);
    if (res == NULL || res->format != 1) return LV_RESULT_INVALID;

    JPEG_LOG_INFO("open_cb: %s\n", path ? path : "(null)");

    if (s_cached_res == res && s_cached_image != NULL && s_cached_raw_buf != NULL) {
        dsc->header = s_cached_image->header;
        dsc->decoded = (lv_draw_buf_t *)s_cached_image;
        dsc->user_data = NULL;
        JPEG_LOG_INFO("[JPEG_DEC] cache hit: id=%u src=%s img=%p data=%p\n",
                   res->id, res->source_path, s_cached_image, s_cached_raw_buf);
        return LV_RESULT_OK;
    }

    if (s_cache_in_use) {
        JPEG_LOG_INFO("cache busy\n");
        return LV_RESULT_INVALID;
    }
    s_cache_in_use = 1;

    /* 切换到新图片前, 先释放旧缓存. */
    free_cached_jpeg();

    /* 校验图片尺寸, 确保不超出固定缓冲区上限. */
    const uint32_t rgb_size = (uint32_t)res->width * (uint32_t)res->height * 2u;
    if (rgb_size > JPEG_DEC_BUF_SIZE) {
        s_cache_in_use = 0;
        JPEG_LOG_ERROR("image too large: %ux%u needs %u > %d\n",
                       res->width, res->height, rgb_size, JPEG_DEC_BUF_SIZE);
        return LV_RESULT_INVALID;
    }

    jpeg_session_t *session = (jpeg_session_t *)lv_malloc(sizeof(jpeg_session_t));
    if (session == NULL) {
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }
    memset(session, 0, sizeof(*session));

    session->res = res;
    session->jpeg_src_ptr = g_res_base + g_res_hdr->data_offset + res->offset;
    session->jpeg_src_len = res->size;

    if (s_cached_raw_buf == NULL) {
        free_jpeg_session(session);
        s_cache_in_use = 0;
        JPEG_LOG_ERROR("s_cached_raw_buf not initialized\n");
        return LV_RESULT_INVALID;
    }

    s_cached_image = (lv_image_dsc_t *)lv_malloc(sizeof(lv_draw_buf_t));
    if (s_cached_image == NULL) {
        free_jpeg_session(session);
        s_cache_in_use = 0;
        JPEG_LOG_WARN("OOM (cached_image).\n");
        return LV_RESULT_INVALID;
    }
    memset(s_cached_image, 0, sizeof(lv_draw_buf_t));
    s_cached_image->header.magic = LV_IMAGE_HEADER_MAGIC;
    s_cached_image->header.cf = LV_COLOR_FORMAT_RGB888;
    s_cached_image->header.w = res->width;
    s_cached_image->header.h = res->height;
    s_cached_image->header.stride = LV_DRAW_BUF_STRIDE(res->width, LV_COLOR_FORMAT_RGB888);
    s_cached_image->header.flags = 0;
    s_cached_image->data_size = rgb_size;
    s_cached_image->data = s_cached_raw_buf;
    s_cached_image->reserved = NULL;

    /* 使用硬件 JPEG 解码. */
    if (!jpeg_hw_dec_handler(session->jpeg_src_ptr, session->jpeg_src_len,
                              res->width, res->height,
                              s_cached_raw_buf, rgb_size,
                              DMA2D_OUTPUT_MODE_RGB888, 
                              DMA2D_ROTATION_0)) {
        JPEG_LOG_ERROR("jpeg decode failed: %s\n", path);
        lv_free(s_cached_image);
        s_cached_image = NULL;
        free_jpeg_session(session);
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }

    s_cached_res = res;
    dsc->header = s_cached_image->header;
    dsc->decoded = (lv_draw_buf_t *)s_cached_image;
    dsc->user_data = session;

    JPEG_LOG_TRACE("open ok: id=%u src=%s magic=0x%02X cf=%u w=%u h=%u data_size=%u img=%p data=%p first=%02X %02X %02X %02X\n",
               res->id, res->source_path,
               (unsigned int)s_cached_image->header.magic,
               (unsigned int)s_cached_image->header.cf,
               (unsigned int)s_cached_image->header.w,
               (unsigned int)s_cached_image->header.h,
               (unsigned int)s_cached_image->data_size,
               s_cached_image,
               s_cached_raw_buf,
               s_cached_raw_buf[0], s_cached_raw_buf[1], s_cached_raw_buf[2], s_cached_raw_buf[3]);
    return LV_RESULT_OK;
}

static void decoder_close_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    LV_UNUSED(decoder);
    if (dsc == NULL) return;

    jpeg_session_t *session = (jpeg_session_t *)dsc->user_data;
    if (session) {
        JPEG_LOG_INFO("close_cb: session=%p\n", session);
        free_jpeg_session(session);
        dsc->user_data = NULL;
    }
    s_cache_in_use = 0;
    dsc->decoded = NULL;
}

#if 0
static const jl_resource_info_t *find_res_by_path(const char *path)
{
    if (path == NULL) return NULL;

    const char *rel = path;
    if (rel[0] == '/') rel++;

    const char *prefix = "customer/H5/res/image/JL/";
    size_t prefix_len = strlen(prefix);
    if (strncmp(rel, prefix, prefix_len) == 0) rel += prefix_len;

    char match_path[256];
    snprintf(match_path, sizeof(match_path), "%s", rel);

    const char *last_slash = strrchr(match_path, '/');
    const char *base = last_slash ? last_slash + 1 : match_path;
    char base_no_ext[128];
    snprintf(base_no_ext, sizeof(base_no_ext), "%s", base);
    char *dot = strrchr(base_no_ext, '.');
    if (dot) *dot = '\0';

    for(uint32_t i = 0; i < JL_RESOURCE_COUNT; i++) {
        const char *sp = g_jl_resources[i].source_path;
        const char *sp_slash = strrchr(sp, '/');
        const char *sp_base = sp_slash ? sp_slash + 1 : sp;
        char sp_base_no_ext[128];
        snprintf(sp_base_no_ext, sizeof(sp_base_no_ext), "%s", sp_base);
        char *spdot = strrchr(sp_base_no_ext, '.');
        if (spdot) *spdot = '\0';

        if (strcmp(sp_base_no_ext, base_no_ext) == 0) {
            return &g_jl_resources[i];
        }
    }
    return NULL;
}
#endif

void jpeg_lvgl_decoder_init(void)
{
    jpeg_mem_init();
    
    lv_image_decoder_t * decoder = lv_image_decoder_create();
    if (decoder == NULL) {
        JPEG_LOG_ERROR("create jpeg decoder failed\n");
        return;
    }

    lv_image_decoder_set_info_cb(decoder, decoder_info_cb);
    lv_image_decoder_set_open_cb(decoder, decoder_open_cb);
    lv_image_decoder_set_close_cb(decoder, decoder_close_cb);

    JPEG_LOG_INFO("jpeg decoder registered\n");
}

int jpeg_mem_init(void)
{
    /* 分配固定内存. */
    s_cached_raw_buf = (uint8_t *)lv_malloc(JPEG_DEC_BUF_SIZE);
    if (s_cached_raw_buf == NULL) {
        JPEG_LOG_ERROR("OOM: s_cached_raw_buf (fixed %d bytes)\n", JPEG_DEC_BUF_SIZE);
        return -1;
    }
    JPEG_LOG_INFO("s_cached_raw_buf allocated: %p, size=%d\n", s_cached_raw_buf, JPEG_DEC_BUF_SIZE);
    
    return 0;
}
