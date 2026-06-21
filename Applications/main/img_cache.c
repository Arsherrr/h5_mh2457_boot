#include "img_cache.h"
#include "res_fs.h"
#include "rtthread.h"
#include "lvgl.h"
#include "../../../Libraries/OSS/lvgl/9.2.2/src/libs/lodepng/lodepng.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define RES_BIN_BASE_ADDR 0x0910B600UL

/* 先按照这种方式缓存, 内存应该是够的, 后面可以改成按需缓存.  */
static const uint32_t IMG_CACHE_INDEX_LIST[] = {
    /* Memu 小图标. */ 117, 118,
};

#define IMG_CACHE_COUNT (sizeof(IMG_CACHE_INDEX_LIST) / sizeof(IMG_CACHE_INDEX_LIST[0]))

/* [ 配置 ] 是否开启 LOG 日志. */
#define USE_IMG_CACHE_LOG 1

/* [ 配置 ] LOG 等级. 
 * LOG_LEVEL_TRACE
 * LOG_LEVEL_INFO
 * LOG_LEVEL_WARN
 * LOG_LEVEL_ERROR
 * LOG_LEVEL_NONE
*/
#define IMG_CACHE_LOG_LEVEL ( LOG_LEVEL_INFO )

#if USE_IMG_CACHE_LOG
#define _IMG_CACHE_LOG_LEVEL IMG_CACHE_LOG_LEVEL
#else
#define _IMG_CACHE_LOG_LEVEL LOG_LEVEL_NONE
#endif

#if _IMG_CACHE_LOG_LEVEL <= LOG_LEVEL_TRACE
#define IMG_CACHE_LOG_TRACE(...) rt_kprintf("[IMG CACHE] " __VA_ARGS__)
#else
#define IMG_CACHE_LOG_TRACE(...)
#endif
#if _IMG_CACHE_LOG_LEVEL <= LOG_LEVEL_INFO
#define IMG_CACHE_LOG_INFO(...) rt_kprintf("[IMG CACHE] " __VA_ARGS__)
#else
#define IMG_CACHE_LOG_INFO(...)
#endif
#if _IMG_CACHE_LOG_LEVEL <= LOG_LEVEL_WARN
#define IMG_CACHE_LOG_WARN(...) rt_kprintf("[IMG CACHE] [WARN] " __VA_ARGS__)
#else
#define IMG_CACHE_LOG_WARN(...)
#endif
#if _IMG_CACHE_LOG_LEVEL <= LOG_LEVEL_ERROR
#define IMG_CACHE_LOG_ERROR(...) rt_kprintf("[IMG CACHE] [ERROR] " __VA_ARGS__)
#else
#define IMG_CACHE_LOG_ERROR(...)
#endif

extern void *sdram_malloc(rt_size_t size);
extern void sdram_free(void *ptr);

static const jl_res_bin_header_t *s_hdr = (const jl_res_bin_header_t *)RES_BIN_BASE_ADDR;
static const uint8_t *s_bin_base = (const uint8_t *)RES_BIN_BASE_ADDR;

static img_cache_entry_t s_img_cache[IMG_CACHE_COUNT];
static bool s_img_cache_inited = false;

static void img_cache_free_entry(img_cache_entry_t *entry)
{
    if (entry == RT_NULL) return;
    if (entry->bitmap != RT_NULL) {
        sdram_free(entry->bitmap);
    }
    memset(entry, 0, sizeof(*entry));
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

static bool img_cache_decode_png_to_sdram(uint32_t res_index, img_cache_entry_t *entry)
{
    if (res_index >= JL_RESOURCE_COUNT || entry == RT_NULL) return false;
    const jl_resource_info_t *res = &g_jl_resources[res_index];
    if (res->format != 2u || res->width == 0u || res->height == 0u) {
        IMG_CACHE_LOG_INFO("skip index=%u id=%u format=%u size=%ux%u src=%s\n",
                   (unsigned int)res_index,
                   (unsigned int)res->id,
                   (unsigned int)res->format,
                   (unsigned int)res->width,
                   (unsigned int)res->height,
                   res->source_path ? res->source_path : "(null)");
        return false;
    }

    const uint8_t *png_src_ptr = s_bin_base + s_hdr->data_offset + res->offset;
    const uint32_t png_src_len = res->size;

    unsigned png_width = 0;
    unsigned png_height = 0;
    lv_draw_buf_t *decoded = RT_NULL;
    unsigned error = lodepng_decode32((unsigned char **)&decoded, &png_width, &png_height, png_src_ptr, png_src_len);
    if (error || decoded == RT_NULL || decoded->data == RT_NULL) {
        IMG_CACHE_LOG_ERROR("decode failed index=%u id=%u error=%u %s src=%s\n",
                   (unsigned int)res_index,
                   (unsigned int)res->id,
                   error,
                   lodepng_error_text(error),
                   res->source_path ? res->source_path : "(null)");
        if (decoded != RT_NULL) lv_draw_buf_destroy(decoded);
        return false;
    }

    if (png_width != res->width || png_height != res->height) {
        IMG_CACHE_LOG_ERROR("size mismatch index=%u id=%u res=%ux%u png=%ux%u src=%s\n",
                   (unsigned int)res_index,
                   (unsigned int)res->id,
                   (unsigned int)res->width,
                   (unsigned int)res->height,
                   png_width,
                   png_height,
                   res->source_path ? res->source_path : "(null)");
    }

    const uint32_t stride = png_width * 4u;
    const uint32_t data_size = stride * png_height;
    uint8_t *bitmap = (uint8_t *)sdram_malloc(data_size);
    if (bitmap == RT_NULL) {
        IMG_CACHE_LOG_ERROR("sdram alloc failed index=%u id=%u size=%u src=%s\n",
                   (unsigned int)res_index,
                   (unsigned int)res->id,
                   (unsigned int)data_size,
                   res->source_path ? res->source_path : "(null)");
        lv_draw_buf_destroy(decoded);
        return false;
    }

    rt_memset(bitmap, 0, data_size);
    for (uint32_t y = 0; y < png_height; y++) {
        rt_memcpy(bitmap + y * stride,
               (uint8_t *)decoded->data + y * decoded->header.stride,
               png_width * 4u);
    }

    convert_argb8888_to_lvgl_bgra(bitmap, (uint32_t)png_width * (uint32_t)png_height);

    entry->key = res->id;
    entry->res_index = res_index;
    entry->width = (uint16_t)png_width;
    entry->height = (uint16_t)png_height;
    entry->stride = stride;
    entry->data_size = data_size;
    entry->cf = LV_COLOR_FORMAT_ARGB8888;
    entry->bitmap = bitmap;
    rt_memset(&entry->draw_buf, 0, sizeof(entry->draw_buf));
    entry->draw_buf.header.magic = LV_IMAGE_HEADER_MAGIC;
    entry->draw_buf.header.cf = entry->cf;
    entry->draw_buf.header.w = entry->width;
    entry->draw_buf.header.h = entry->height;
    entry->draw_buf.header.stride = entry->stride;
    entry->draw_buf.header.flags = 0;
    entry->draw_buf.data_size = entry->data_size;
    entry->draw_buf.data = entry->bitmap;
    entry->draw_buf.unaligned_data = entry->bitmap;
    entry->source_path = res->source_path;
    entry->occupied = true;

    lv_draw_buf_destroy(decoded);

    IMG_CACHE_LOG_INFO("cached index=%u id=%u %ux%u stride=%u size=%u data=%p src=%s\n",
               (unsigned int)res_index,
               (unsigned int)entry->key,
               (unsigned int)entry->width,
               (unsigned int)entry->height,
               (unsigned int)entry->stride,
               (unsigned int)entry->data_size,
               entry->bitmap,
               entry->source_path ? entry->source_path : "(null)");
    return true;
}

int img_cache_init(void)
{
    if (s_img_cache_inited) return 0;
    rt_memset(s_img_cache, 0, sizeof(s_img_cache));

    uint32_t ok_count = 0;
    for (uint32_t i = 0; i < IMG_CACHE_COUNT; i++) {
        uint32_t index = IMG_CACHE_INDEX_LIST[i];
        img_cache_entry_t *entry = &s_img_cache[i];
        if (img_cache_decode_png_to_sdram(index, entry)) {
            ok_count++;
        }
    }

    s_img_cache_inited = true;
    IMG_CACHE_LOG_INFO("init done: %u/%u cached\n", (unsigned int)ok_count, (unsigned int)IMG_CACHE_COUNT);
    return 0;
}

const img_cache_entry_t *img_cache_get_by_id(uint32_t id)
{
    /* 改二分查找/平衡二叉树/哈希表. */
    for (uint32_t i = 0; i < IMG_CACHE_COUNT; i++) {
        if (s_img_cache[i].occupied && s_img_cache[i].key == id) {
            return &s_img_cache[i];
        }
    }
    return RT_NULL;
}

void img_cache_deinit(void)
{
    for (uint32_t i = 0; i < IMG_CACHE_COUNT; i++) {
        img_cache_free_entry(&s_img_cache[i]);
    }
    s_img_cache_inited = false;
}
