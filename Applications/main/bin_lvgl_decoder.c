#include "bin_lvgl_decoder.h"
#include "res_fs.h"
#include "lvgl.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/draw/lv_image_decoder_private.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define RES_BIN_BASE_ADDR 0x0910B600UL

/* 打开/关闭 BIN_DEC 调试日志 */
// #define BIN_DEC_DEBUG

/* 资源 BIN 的起始地址. */
typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t header_size;
    uint32_t data_offset;
} __attribute__((packed)) jl_res_bin_header_t;

typedef struct __attribute__((packed)) {
    lv_image_header_t header;
    uint32_t data_size;
} flash_image_blob_header_t;

static const jl_res_bin_header_t *s_hdr = (const jl_res_bin_header_t *)RES_BIN_BASE_ADDR;
static const uint8_t *s_bin_base = (const uint8_t *)RES_BIN_BASE_ADDR;

typedef struct {
    const jl_resource_info_t *res;
    lv_image_dsc_t *image;
} bin_session_t;

/* 解析 BIN 内的图片头部. */
static bool fill_blob(const uint8_t *blob, uint32_t blob_size, flash_image_blob_header_t *out_hdr, const uint8_t **out_data)
{
    if(blob == NULL || out_hdr == NULL || out_data == NULL) return false;
    if(blob_size < sizeof(flash_image_blob_header_t)) return false;

    const flash_image_blob_header_t *hdr = (const flash_image_blob_header_t *)blob;
    *out_hdr = *hdr;
    *out_data = blob + sizeof(flash_image_blob_header_t);
    return true;
}

static lv_image_dsc_t *create_flash_image_dsc(const flash_image_blob_header_t *blob_hdr, const uint8_t *blob_data)
{
    if(blob_hdr == NULL || blob_data == NULL) return NULL;

    lv_draw_buf_t *img = (lv_draw_buf_t *)lv_malloc(sizeof(lv_draw_buf_t));
    if(img == NULL) {
        return NULL;
    }

    memset(img, 0, sizeof(*img));
    img->header = blob_hdr->header;
    img->data_size = blob_hdr->data_size;
    img->data = (uint8_t *)blob_data;
    return (lv_image_dsc_t *)img;
}

/* 提供图片信息给 LVGL. */
static lv_result_t bin_info_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc, lv_image_header_t * header)
{
    LV_UNUSED(decoder);
    if(dsc == NULL || dsc->src == NULL || header == NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
#ifdef BIN_DEC_DEBUG
    printf("[BIN_DEC] info_cb: %s\n", path ? path : "(null)");
#endif

    const char *dot = strrchr(path, '.');
    if(dot == NULL || strcmp(dot, ".bin") != 0) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] info reject ext: %s\n", path ? path : "(null)");
#endif
        return LV_RESULT_INVALID;
    }

    const jl_resource_info_t *res = find_res_by_path(path);
    if(res == NULL) return LV_RESULT_INVALID;
    if(res->format != 2) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] info not bin-format: id=%u fmt=%u src=%s\n", res->id, res->format, res->source_path);
#endif
        return LV_RESULT_INVALID;
    }

    const uint8_t *blob = s_bin_base + s_hdr->data_offset + res->offset;
    flash_image_blob_header_t blob_hdr;
    const uint8_t *blob_data = NULL;
    if(!fill_blob(blob, res->size, &blob_hdr, &blob_data)) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] info invalid blob: id=%u src=%s\n", res->id, res->source_path);
#endif
        return LV_RESULT_INVALID;
    }

    header->magic = blob_hdr.header.magic;
    header->cf = blob_hdr.header.cf;
    header->w = blob_hdr.header.w;
    header->h = blob_hdr.header.h;
    header->stride = blob_hdr.header.stride;
    header->flags = blob_hdr.header.flags;

#ifdef BIN_DEC_DEBUG
    printf("[BIN_DEC] info hit: id=%u src=%s cf=%u w=%u h=%u data_size=%u\n",
               res->id, res->source_path,
               (unsigned int)header->cf,
               (unsigned int)header->w,
               (unsigned int)header->h,
               (unsigned int)blob_hdr.data_size);
#endif
    return LV_RESULT_OK;
}

/* 打开 BIN 图片并构建显示缓冲. */
static lv_result_t bin_open_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    LV_UNUSED(decoder);
    if(dsc == NULL || dsc->src == NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
#ifdef BIN_DEC_DEBUG
    printf("[BIN_DEC] open_cb: %s\n", path ? path : "(null)");
#endif

    const char *dot = strrchr(path, '.');
    if(dot == NULL || strcmp(dot, ".bin") != 0) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] open reject ext: %s\n", path ? path : "(null)");
#endif
        return LV_RESULT_INVALID;
    }

    const jl_resource_info_t *res = find_res_by_path(path);
    if(res == NULL || res->format != 2) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] open miss: %s\n", path ? path : "(null)");
#endif
        return LV_RESULT_INVALID;
    }

    const uint8_t *blob = s_bin_base + s_hdr->data_offset + res->offset;
    flash_image_blob_header_t blob_hdr;
    const uint8_t *blob_data = NULL;
    if(!fill_blob(blob, res->size, &blob_hdr, &blob_data)) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] invalid blob: id=%u src=%s\n", res->id, res->source_path);
#endif
        return LV_RESULT_INVALID;
    }

    lv_image_dsc_t *img = create_flash_image_dsc(&blob_hdr, blob_data);
    if(img == NULL) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] image dsc oom: id=%u src=%s\n", res->id, res->source_path);
#endif
        return LV_RESULT_INVALID;
    }

    bin_session_t *session = (bin_session_t *)lv_malloc(sizeof(bin_session_t));
    if(session == NULL) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] session oom: id=%u src=%s\n", res->id, res->source_path);
#endif
        lv_free(img);
        return LV_RESULT_INVALID;
    }
    memset(session, 0, sizeof(*session));
    session->res = res;
    session->image = img;

    dsc->header = blob_hdr.header;
    dsc->decoded = (lv_draw_buf_t *)img;
    dsc->user_data = session;

#ifdef BIN_DEC_DEBUG
    printf("[BIN_DEC] open ok: id=%u src=%s cf=%u w=%u h=%u data_size=%u img=%p data=%p\n",
               res->id, res->source_path,
               (unsigned int)blob_hdr.header.cf,
               (unsigned int)blob_hdr.header.w,
               (unsigned int)blob_hdr.header.h,
               (unsigned int)blob_hdr.data_size,
               img,
               blob_data);
#endif
    return LV_RESULT_OK;
}

/* 释放 BIN 解码会话资源. */
static void bin_close_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    LV_UNUSED(decoder);
    if(dsc == NULL) return;

    bin_session_t *session = (bin_session_t *)dsc->user_data;
    if(session) {
        if(session->image) lv_free(session->image);
        lv_free(session);
        dsc->user_data = NULL;
    }
    dsc->decoded = NULL;
}

/* 注册 BIN 图片解码器. */
void bin_lvgl_decoder_init(void)
{
    lv_image_decoder_t * decoder = lv_image_decoder_create();
    if(decoder == NULL) {
#ifdef BIN_DEC_DEBUG
        printf("[BIN_DEC] create bin decoder failed\n");
#endif
        return;
    }

    lv_image_decoder_set_info_cb(decoder, bin_info_cb);
    lv_image_decoder_set_open_cb(decoder, bin_open_cb);
    lv_image_decoder_set_close_cb(decoder, bin_close_cb);
#ifdef BIN_DEC_DEBUG
    printf("[BIN_DEC] bin decoder registered\n");
#endif
}
