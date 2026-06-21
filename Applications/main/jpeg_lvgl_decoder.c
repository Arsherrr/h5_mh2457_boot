#include "jpeg_lvgl_decoder.h"
#include "jpeg_hw.h"
#include "res.h"
#include "lvgl.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/draw/lv_image_decoder_private.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define RES_BIN_BASE_ADDR 0x0910B600UL

/* 打开/关闭 JPEG_DEC 调试日志 */
// #define JPEG_DEC_DEBUG

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t header_size;
    uint32_t data_offset;
} __attribute__((packed)) jl_res_bin_header_t;

static const jl_res_bin_header_t *s_hdr = (const jl_res_bin_header_t *)RES_BIN_BASE_ADDR;
static const uint8_t *s_bin_base = (const uint8_t *)RES_BIN_BASE_ADDR;

typedef struct {
    const jl_resource_info_t *res;
    const uint8_t *jpeg_src_ptr;
    uint32_t jpeg_src_len;
    lv_image_dsc_t *image;
    uint8_t *raw_buf;
} jpeg_session_t;

static const jl_resource_info_t *find_res_by_path(const char *path);
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
    if (s_cached_raw_buf) {
        lv_free(s_cached_raw_buf);
        s_cached_raw_buf = NULL;
    }
    s_cached_res = NULL;
}

static lv_result_t decoder_info_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc, lv_image_header_t * header)
{
    LV_UNUSED(decoder);
    if (dsc == NULL || dsc->src == NULL || header == NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
#ifdef JPEG_DEC_DEBUG
    printf("[JPEG_DEC] info_cb: %s\n", path ? path : "(null)");
#endif

    const jl_resource_info_t *res = find_res_by_path(path);
    if (res == NULL || res->format != 1) return LV_RESULT_INVALID;

    header->magic = LV_IMAGE_HEADER_MAGIC;
    header->cf = LV_COLOR_FORMAT_RGB565;
    header->w = res->width;
    header->h = res->height;
    header->stride = LV_DRAW_BUF_STRIDE(res->width, LV_COLOR_FORMAT_RGB565);
    header->flags = 0;

#ifdef JPEG_DEC_DEBUG
    printf("[JPEG_DEC] info hit: id=%u src=%s cf=%u w=%u h=%u data_size=%u\n",
               res->id, res->source_path,
               (unsigned int)header->cf,
               (unsigned int)header->w,
               (unsigned int)header->h,
               (unsigned int)res->size);
#endif
    return LV_RESULT_OK;
}

static lv_result_t decoder_open_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    LV_UNUSED(decoder);
    if (dsc == NULL || dsc->src == NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
    const jl_resource_info_t *res = find_res_by_path(path);
    if (res == NULL || res->format != 1) return LV_RESULT_INVALID;

#ifdef JPEG_DEC_DEBUG
    printf("[JPEG_DEC] open_cb: %s\n", path ? path : "(null)");
#endif

    if (s_cached_res == res && s_cached_image != NULL && s_cached_raw_buf != NULL) {
        dsc->header = s_cached_image->header;
        dsc->decoded = (lv_draw_buf_t *)s_cached_image;
        dsc->user_data = NULL;
#ifdef JPEG_DEC_DEBUG
        printf("[JPEG_DEC] cache hit: id=%u src=%s img=%p data=%p\n",
                   res->id, res->source_path, s_cached_image, s_cached_raw_buf);
#endif
        return LV_RESULT_OK;
    }

    if (s_cache_in_use) {
#ifdef JPEG_DEC_DEBUG
        printf("[JPEG_DEC] cache busy\n");
#endif
        return LV_RESULT_INVALID;
    }
    s_cache_in_use = 1;

    /* 切换到新图片前, 先释放旧缓存. */
    free_cached_jpeg();

    jpeg_session_t *session = (jpeg_session_t *)lv_malloc(sizeof(jpeg_session_t));
    if (session == NULL) {
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }
    memset(session, 0, sizeof(*session));

    session->res = res;
    session->jpeg_src_ptr = s_bin_base + s_hdr->data_offset + res->offset;
    session->jpeg_src_len = res->size;

    const uint32_t rgb_size = (uint32_t)res->width * (uint32_t)res->height * 2u;
    s_cached_raw_buf = (uint8_t *)lv_malloc(rgb_size); /* 指定使用 SDRAM 空间. */
    if (s_cached_raw_buf == NULL) {
        free_jpeg_session(session);
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }

    s_cached_image = (lv_image_dsc_t *)lv_malloc(sizeof(lv_draw_buf_t));
    if (s_cached_image == NULL) {
        lv_free(s_cached_raw_buf);
        s_cached_raw_buf = NULL;
        free_jpeg_session(session);
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }
    memset(s_cached_image, 0, sizeof(lv_draw_buf_t));
    s_cached_image->header.magic = LV_IMAGE_HEADER_MAGIC;
    s_cached_image->header.cf = LV_COLOR_FORMAT_RGB565;
    s_cached_image->header.w = res->width;
    s_cached_image->header.h = res->height;
    s_cached_image->header.stride = LV_DRAW_BUF_STRIDE(res->width, LV_COLOR_FORMAT_RGB565);
    s_cached_image->header.flags = 0;
    s_cached_image->data_size = rgb_size;
    s_cached_image->data = s_cached_raw_buf;
    s_cached_image->reserved = NULL;

    /* 使用硬件 JPEG 解码. */
    if (!jpeg_hw_decode_block(session->jpeg_src_ptr, session->jpeg_src_len,
                             res->width, res->height,
                             s_cached_raw_buf, rgb_size)) {
#ifdef JPEG_DEC_DEBUG
        printf("[JPEG_DEC] jpeg decode failed: %s\n", path);
#endif
        lv_free(s_cached_image);
        s_cached_image = NULL;
        lv_free(s_cached_raw_buf);
        s_cached_raw_buf = NULL;
        free_jpeg_session(session);
        s_cache_in_use = 0;
        return LV_RESULT_INVALID;
    }

    s_cached_res = res;
    dsc->header = s_cached_image->header;
    dsc->decoded = (lv_draw_buf_t *)s_cached_image;
    dsc->user_data = session;

#ifdef JPEG_DEC_DEBUG
    printf("[JPEG_DEC] open ok: id=%u src=%s magic=0x%02X cf=%u w=%u h=%u data_size=%u img=%p data=%p first=%02X %02X %02X %02X\n",
               res->id, res->source_path,
               (unsigned int)s_cached_image->header.magic,
               (unsigned int)s_cached_image->header.cf,
               (unsigned int)s_cached_image->header.w,
               (unsigned int)s_cached_image->header.h,
               (unsigned int)s_cached_image->data_size,
               s_cached_image,
               s_cached_raw_buf,
               s_cached_raw_buf[0], s_cached_raw_buf[1], s_cached_raw_buf[2], s_cached_raw_buf[3]);
#endif
    return LV_RESULT_OK;
}

static void decoder_close_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    LV_UNUSED(decoder);
    if (dsc == NULL) return;

    jpeg_session_t *session = (jpeg_session_t *)dsc->user_data;
    if (session) {
#ifdef JPEG_DEC_DEBUG
        printf("[JPEG_DEC] close_cb: session=%p\n", session);
#endif
        free_jpeg_session(session);
        dsc->user_data = NULL;
    }
    s_cache_in_use = 0;
    dsc->decoded = NULL;
}

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

void jpeg_lvgl_decoder_init(void)
{
    /* 初始化硬件 JPEG. */
    jpeg_hw_init();

    lv_image_decoder_t * decoder = lv_image_decoder_create();
    if (decoder == NULL) {
#ifdef JPEG_DEC_DEBUG
        printf("[JPEG_DEC] create jpeg decoder failed\n");
#endif
        return;
    }

    lv_image_decoder_set_info_cb(decoder, decoder_info_cb);
    lv_image_decoder_set_open_cb(decoder, decoder_open_cb);
    lv_image_decoder_set_close_cb(decoder, decoder_close_cb);
#ifdef JPEG_DEC_DEBUG
    printf("[JPEG_DEC] jpeg decoder registered\n");
#endif
}
