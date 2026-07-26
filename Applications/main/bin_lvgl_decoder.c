#include "bin_lvgl_decoder.h"
#include "res_fs.h"
#include "lvgl.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/draw/lv_image_decoder_private.h"
#include "rtthread.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/* 打开/关闭 BIN_DEC 调试日志 */
// #define BIN_DEC_DEBUG

/* 资源 BIN 的起始地址. */
typedef struct __attribute__((packed)) {
    lv_image_header_t header;
    uint32_t data_size;
} flash_image_blob_header_t;

typedef struct {
    const jl_resource_info_t *res;
    lv_image_dsc_t *image;
} bin_session_t;

/* yl: draw_buf 缓存(按 res), 避免动画界面每帧重新解码/分配 bin 图导致堆碎片/损坏.
 * bin 图 data 指向 flash(常驻), 缓存 draw_buf 结构(28B/张)很便宜, 可长期持有. */
#define BIN_DEC_CACHE_MAX 8
static struct {
    const jl_resource_info_t *res;
    lv_draw_buf_t *img;
} s_bin_cache[BIN_DEC_CACHE_MAX];

// /* 按路径查找资源信息. */
// static const jl_resource_info_t *find_res_by_path(const char *path)
// {
//     if(path == NULL) return NULL;

//     const char *rel = path;
//     if(rel[0] == '/') rel++;

//     const char *prefix = "customer/H5/res/image/JL/";
//     size_t prefix_len = strlen(prefix);
//     if(strncmp(rel, prefix, prefix_len) == 0) rel += prefix_len;

//     char match_path[256];
//     snprintf(match_path, sizeof(match_path), "%s", rel);

//     const char *last_slash = strrchr(match_path, '/');
//     const char *base = last_slash ? last_slash + 1 : match_path;
//     char base_no_ext[128];
//     snprintf(base_no_ext, sizeof(base_no_ext), "%s", base);
//     char *dot = strrchr(base_no_ext, '.');
//     if(dot) *dot = '\0';

//     for(uint32_t i = 0; i < JL_RESOURCE_COUNT; i++) {
//         const char *sp = g_jl_resources[i].source_path;
//         const char *sp_slash = strrchr(sp, '/');
//         const char *sp_base = sp_slash ? sp_slash + 1 : sp;
//         char sp_base_no_ext[128];
//         snprintf(sp_base_no_ext, sizeof(sp_base_no_ext), "%s", sp_base);
//         char *spdot = strrchr(sp_base_no_ext, '.');
//         if(spdot) *spdot = '\0';

//         if(strcmp(sp_base_no_ext, base_no_ext) == 0) {
// #ifdef BIN_DEC_DEBUG
//             rt_kprintf("[BIN_DEC] match: %s -> id=%u fmt=%u src=%s\n", match_path, g_jl_resources[i].id, g_jl_resources[i].format, g_jl_resources[i].source_path);
// #endif
//             return &g_jl_resources[i];
//         }
//     }

// #ifdef BIN_DEC_DEBUG
//     rt_kprintf("[BIN_DEC] match failed: %s\n", match_path);
// #endif
//     return NULL;
// }

/* 检查 cf 是否为 LVGL 支持的合法颜色格式.
 * 防止坏 bin / offset 越界读到垃圾 cf, 导致 LVGL 内部查表越界取到 NULL 函数指针 (PC=0 HardFault). */
static bool cf_is_valid(lv_color_format_t cf)
{
    switch(cf) {
        case LV_COLOR_FORMAT_RGB565:
        case LV_COLOR_FORMAT_RGB888:
        case LV_COLOR_FORMAT_ARGB8888:
        case LV_COLOR_FORMAT_XRGB8888:
        case LV_COLOR_FORMAT_ARGB8565:
        case LV_COLOR_FORMAT_RGB565A8:
        case LV_COLOR_FORMAT_L8:
        case LV_COLOR_FORMAT_A8:
            return true;
        default:
            return false;
    }
}

/* 解析 BIN 内的图片头部. */
static bool fill_blob(const uint8_t *blob, uint32_t blob_size, flash_image_blob_header_t *out_hdr, const uint8_t **out_data)
{
    if(blob == NULL || out_hdr == NULL || out_data == NULL) return false;
    if(blob_size < sizeof(flash_image_blob_header_t)) return false;

    const flash_image_blob_header_t *hdr = (const flash_image_blob_header_t *)blob;

    /* 校验 magic, 防止 offset 越界后读到非图片数据. */
    if(hdr->header.magic != LV_IMAGE_HEADER_MAGIC) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] bad magic: 0x%08X\n", (unsigned int)hdr->header.magic);
#endif
        return false;
    }

    /* 校验颜色格式, 非法 cf 会让 LVGL 查表越界. */
    if(!cf_is_valid((lv_color_format_t)hdr->header.cf)) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] bad cf: %u\n", (unsigned int)hdr->header.cf);
#endif
        return false;
    }

    /* 校验尺寸, 0 尺寸或异常大会导致后续 stride/数据访问出错. */
    if(hdr->header.w == 0 || hdr->header.h == 0 ||
       hdr->header.w > 4096 || hdr->header.h > 4096) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] bad size: %ux%u\n",
                   (unsigned int)hdr->header.w, (unsigned int)hdr->header.h);
#endif
        return false;
    }

    /* 校验 data_size 不超过 blob 剩余空间, 防止 LVGL 按 data_size 读取越界. */
    if(hdr->data_size > blob_size - sizeof(flash_image_blob_header_t)) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] data_size overflow: %u > %u\n",
                   (unsigned int)hdr->data_size,
                   (unsigned int)(blob_size - sizeof(flash_image_blob_header_t)));
#endif
        return false;
    }

    *out_hdr = *hdr;
    *out_data = blob + sizeof(flash_image_blob_header_t);
    return true;
}

static lv_image_dsc_t *create_flash_image_dsc(const flash_image_blob_header_t *blob_hdr, const uint8_t *blob_data)
{
    if(blob_hdr == NULL || blob_data == NULL) return NULL;

    lv_draw_buf_t *img = (lv_draw_buf_t *)rt_malloc(sizeof(lv_draw_buf_t));
    if(img == NULL) {
        return NULL;
    }

    memset(img, 0, sizeof(*img));
    img->header = blob_hdr->header;
    img->data_size = blob_hdr->data_size;
    img->data = (uint8_t *)blob_data;
    img->unaligned_data = (void *)blob_data;
    img->handlers = lv_draw_buf_get_image_handlers();  /* 必须: 否则 LVGL 对齐/销毁时解引用 NULL handlers → PC=0 HardFault */
#ifdef BIN_DEC_DEBUG
    rt_kprintf("[BIN_DEC] draw_buf built: img=%p handlers=%p data=%p %s\n",
               img, img->handlers, img->data,
               img->handlers ? "OK" : "<<< NULL! PC=0 risk");
#endif
    return (lv_image_dsc_t *)img;
}

/* 提供图片信息给 LVGL. */
static lv_result_t bin_info_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc, lv_image_header_t * header)
{
    LV_UNUSED(decoder);
    if(dsc == NULL || dsc->src == NULL || header == NULL) return LV_RESULT_INVALID;

    const char *path = (const char *)dsc->src;
#ifdef BIN_DEC_DEBUG
    rt_kprintf("[BIN_DEC] info_cb: %s\n", path ? path : "(null)");
#endif

    const char *dot = strrchr(path, '.');
    if(dot == NULL || strcmp(dot, ".bin") != 0) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] info reject ext: %s\n", path ? path : "(null)");
#endif
        return LV_RESULT_INVALID;
    }

    const jl_resource_info_t *res = find_res_by_path(path);
    if(res == NULL) return LV_RESULT_INVALID;
    if(res->format != 2) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] info not bin-format: id=%u fmt=%u src=%s\n", res->id, res->format, res->source_path);
#endif
        return LV_RESULT_INVALID;
    }

    const uint8_t *blob = g_res_base + g_res_hdr->data_offset + res->offset;
    flash_image_blob_header_t blob_hdr;
    const uint8_t *blob_data = NULL;
    if(!fill_blob(blob, res->size, &blob_hdr, &blob_data)) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] info invalid blob: id=%u src=%s\n", res->id, res->source_path);
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
    rt_kprintf("[BIN_DEC] info hit: id=%u src=%s cf=%u w=%u h=%u data_size=%u\n",
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
    rt_kprintf("[BIN_DEC] open_cb: %s\n", path ? path : "(null)");
#endif

    const char *dot = strrchr(path, '.');
    if(dot == NULL || strcmp(dot, ".bin") != 0) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] open reject ext: %s\n", path ? path : "(null)");
#endif
        return LV_RESULT_INVALID;
    }

    const jl_resource_info_t *res = find_res_by_path(path);
    if(res == NULL || res->format != 2) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] open miss: %s\n", path ? path : "(null)");
#endif
        return LV_RESULT_INVALID;
    }

    const uint8_t *blob = g_res_base + g_res_hdr->data_offset + res->offset;
    flash_image_blob_header_t blob_hdr;
    const uint8_t *blob_data = NULL;
    if(!fill_blob(blob, res->size, &blob_hdr, &blob_data)) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] invalid blob: id=%u src=%s\n", res->id, res->source_path);
#endif
        return LV_RESULT_INVALID;
    }

    /* yl: 先查 draw_buf 缓存(按 res), 命中直接复用, 不重新解码/分配 */
    for(int i = 0; i < BIN_DEC_CACHE_MAX; i++) {
        if(s_bin_cache[i].res == res && s_bin_cache[i].img != NULL) {
            dsc->header = blob_hdr.header;
            dsc->decoded = s_bin_cache[i].img;
            dsc->user_data = NULL;  /* 缓存命中, close_cb 不释放 */
#ifdef BIN_DEC_DEBUG
            rt_kprintf("[BIN_DEC] open cache HIT: id=%u src=%s img=%p\n",
                       res->id, res->source_path, s_bin_cache[i].img);
#endif
            return LV_RESULT_OK;
        }
    }

    /* cache miss: 解码 */
    lv_image_dsc_t *img = create_flash_image_dsc(&blob_hdr, blob_data);
    if(img == NULL) {
#ifdef BIN_DEC_DEBUG
        rt_kprintf("[BIN_DEC] image dsc oom: id=%u src=%s\n", res->id, res->source_path);
#endif
        return LV_RESULT_INVALID;
    }

    dsc->header = blob_hdr.header;
    dsc->decoded = (lv_draw_buf_t *)img;

    /* 存入缓存(找空槽); 满则 fallback 建 session 让 close_cb 释放 */
    int cached = 0;
    for(int i = 0; i < BIN_DEC_CACHE_MAX; i++) {
        if(s_bin_cache[i].res == NULL) {
            s_bin_cache[i].res = res;
            s_bin_cache[i].img = (lv_draw_buf_t *)img;
            cached = 1;
            break;
        }
    }
    if(cached) {
        dsc->user_data = NULL;  /* 已缓存, close_cb 不释放 */
    }
    else {
        bin_session_t *session = (bin_session_t *)rt_malloc(sizeof(bin_session_t));
        if(session == NULL) {
#ifdef BIN_DEC_DEBUG
            rt_kprintf("[BIN_DEC] session oom: id=%u src=%s\n", res->id, res->source_path);
#endif
            rt_free(img);
            return LV_RESULT_INVALID;
        }
        memset(session, 0, sizeof(*session));
        session->res = res;
        session->image = img;
        dsc->user_data = session;
    }

#ifdef BIN_DEC_DEBUG
    rt_kprintf("[BIN_DEC] open ok: id=%u src=%s cf=%u w=%u h=%u data_size=%u img=%p data=%p %s\n",
               res->id, res->source_path,
               (unsigned int)blob_hdr.header.cf,
               (unsigned int)blob_hdr.header.w,
               (unsigned int)blob_hdr.header.h,
               (unsigned int)blob_hdr.data_size,
               img,
               blob_data,
               cached ? "(cached)" : "(nocache)");
#endif
    return LV_RESULT_OK;
}

/* 释放 BIN 解码会话资源. */
static void bin_close_cb(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    LV_UNUSED(decoder);
    if(dsc == NULL) return;

    bin_session_t *session = (bin_session_t *)dsc->user_data;
#ifdef BIN_DEC_DEBUG
    rt_kprintf("[BIN_DEC] close_cb: session=%p %s\n",
               (void *)session, session ? "(free img+session)" : "(cached, skip free)");
#endif
    if(session) {
        if(session->image) rt_free(session->image);
        rt_free(session);
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
        rt_kprintf("[BIN_DEC] create bin decoder failed\n");
#endif
        return;
    }

    lv_image_decoder_set_info_cb(decoder, bin_info_cb);
    lv_image_decoder_set_open_cb(decoder, bin_open_cb);
    lv_image_decoder_set_close_cb(decoder, bin_close_cb);
#ifdef BIN_DEC_DEBUG
    rt_kprintf("[BIN_DEC] bin decoder registered\n");
#endif
}
