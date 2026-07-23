#ifndef RES_H_
#define RES_H_

#include "memory.h"
#include <stdint.h>

/* LOGO 文件包头. */
typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t width;
    uint16_t height;
    uint32_t size;
} logo_header_t;

/* Bin 文件包头. */
typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t header_size;
    uint32_t table_offset;
    uint32_t hash_offset;
    uint32_t data_offset;
} jl_res_bin_header_t;

/* 哈希表元数据 (位于 hash_offset 处). */
typedef struct __attribute__((packed)) {
    uint32_t G_offset;
    uint16_t G_len;
    uint32_t S1_offset;
    uint16_t S1_len;
    uint32_t S2_offset;
    uint16_t S2_len;
    uint32_t NG;
    uint32_t NS;
} jl_res_hash_meta_t;

typedef struct __attribute__((packed)) {
    uint32_t id;
    uint32_t offset;
    uint32_t size;
    uint16_t width;
    uint16_t height;
    uint8_t format;
    const char *source_path;
} jl_resource_info_t;

int res_init(void);
int perfect_hash(const char* key);

/* img.bin 文件头信息指针. */
extern const jl_res_bin_header_t *g_res_hdr;

/* img.bin flash 中的偏移地址. */
extern const uint8_t *g_res_base;

/* 目录表元素. */
extern jl_resource_info_t *g_jl_resources;

#endif /* RES_H_ */
