#ifndef __IMG_CACHE_H
#define __IMG_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t header_size;
    uint32_t data_offset;
} __attribute__((packed)) jl_res_bin_header_t;

typedef struct {
    uint32_t key;
    uint32_t res_index;
    uint16_t width;
    uint16_t height;
    uint32_t stride;
    uint32_t data_size;
    lv_color_format_t cf;
    uint8_t *bitmap;
    lv_draw_buf_t draw_buf;
    const char *source_path;
    bool occupied;
} img_cache_entry_t;

int img_cache_init(void);
void img_cache_deinit(void);
const img_cache_entry_t *img_cache_get_by_id(uint32_t id);
const img_cache_entry_t *img_cache_get_by_index(uint32_t res_index);

#ifdef __cplusplus
}
#endif
#endif
