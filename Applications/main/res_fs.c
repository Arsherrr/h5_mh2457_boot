#include "res_fs.h"
#include "res.h"
#include "lvgl.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define RES_FS_LETTER 'A'
#define RES_FS_ROOT_PREFIX "customer/H5/res/"
/* 资源 BIN 的起始地址. */
#define RES_BIN_BASE_ADDR 0x0910B600UL

#ifndef LV_FS_MAX_PATH_LENGTH
#define LV_FS_MAX_PATH_LENGTH 256
#endif

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t header_size;
    uint32_t data_offset;
} jl_res_bin_header_t;

static const jl_res_bin_header_t *g_res_hdr = (const jl_res_bin_header_t *)RES_BIN_BASE_ADDR;
static const uint8_t *g_bin_base = (const uint8_t *)RES_BIN_BASE_ADDR;

static void strip_ext(char *path)
{
    char *dot = strrchr(path, '.');
    if(dot) *dot = '\0';
}

static const char *base_name(const char *path)
{
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

/* 按路径查找资源信息. */
const jl_resource_info_t *find_res_by_path(const char *path)
{
    if(path == NULL) return NULL;

    int index = perfect_hash(base_name(path));
    if (index != -1) {
        // /* 再次验证. */
        // if (strcmp(filename, g_jl_resources[index].source_path) == 0)
        {
            /* 现在 freetype otf 格式有问题, 先临时重定向到 AlibabaPuHuiTi Regular 字体, 方便测试. */
            if (index == 1) index = 0;
            return &g_jl_resources[index];
        }
    }
    return NULL;
#if 0
    const char *rel = path;
    if(rel[0] == '/') rel++;

    const char *prefix = RES_FS_ROOT_PREFIX;
    size_t prefix_len = strlen(prefix);
    if(strncmp(rel, prefix, prefix_len) == 0) {
        rel += prefix_len;
    }

    char match_path[LV_FS_MAX_PATH_LENGTH];
    snprintf(match_path, sizeof(match_path), "%s", rel);
    // printf("[RESFS] normalize: raw=%s rel=%s\n", path, match_path);

    char rel_base[LV_FS_MAX_PATH_LENGTH];
    snprintf(rel_base, sizeof(rel_base), "%s", match_path);
    strip_ext(rel_base);

    for(uint32_t i = 0; i < JL_RESOURCE_COUNT; i++) {
        const char *sp = g_jl_resources[i].source_path;
        char sp_full[LV_FS_MAX_PATH_LENGTH];
        snprintf(sp_full, sizeof(sp_full), "%s", sp);
        char sp_base[LV_FS_MAX_PATH_LENGTH];
        snprintf(sp_base, sizeof(sp_base), "%s", sp);
        strip_ext(sp_base);

        if(strcmp(sp_full, match_path) == 0 || strcmp(sp_base, rel_base) == 0 || strcmp(base_name(sp_full), base_name(match_path)) == 0) {
            // printf("[RESFS] match: %s -> id=%u fmt=%u src=%s\n",
            //            match_path, g_jl_resources[i].id, g_jl_resources[i].format, sp_full);
            if (i == 1) {
                /* 现在 freetype otf 格式有问题, 先临时重定向到 AlibabaPuHuiTi Regular 字体, 方便测试. */
                return &g_jl_resources[0];
            } else {
                return &g_jl_resources[i];
            }
        }
    }

    printf("[RESFS] match failed: %s base=%s\n", match_path, rel_base);
    return NULL;
#endif
}

typedef struct {
    const jl_resource_info_t *res;
    uint32_t pos;
    uint8_t in_use;
} res_file_t;

#define RES_FS_MAX_OPEN_FILES 8
static res_file_t s_open_files[RES_FS_MAX_OPEN_FILES];

static res_file_t *alloc_res_file(void)
{
    for(uint32_t i = 0; i < RES_FS_MAX_OPEN_FILES; i++) {
        if(!s_open_files[i].in_use) {
            memset(&s_open_files[i], 0, sizeof(s_open_files[i]));
            s_open_files[i].in_use = 1;
            return &s_open_files[i];
        }
    }
    return NULL;
}

static void free_res_file(res_file_t *f)
{
    if(f) memset(f, 0, sizeof(*f));
}

/* 打开资源文件并返回文件句柄. */
static void * res_open_cb(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode)
{
    LV_UNUSED(drv);
    if(mode != LV_FS_MODE_RD) return NULL;

    const jl_resource_info_t *res = find_res_by_path(path);
    if(res == NULL) {
        printf("[RESFS] open miss: %s\n", path ? path : "(null)");
        return NULL;
    }

    res_file_t *f = alloc_res_file();
    if(f == NULL) {
        printf("[RESFS] open oom: %s\n", path ? path : "(null)");
        return NULL;
    }
    f->res = res;
    f->pos = 0;

    // printf("[RESFS] open hit: %s -> id=%u path=%s offset=%u size=%u type=%u\n",
    //            path, res->id, res->source_path, res->offset, res->size, res->format);
    const uint8_t *src = (const uint8_t *)(g_bin_base + g_res_hdr->data_offset + res->offset);
    // printf("[RESFS] head: %02X %02X %02X %02X %02X %02X %02X %02X\n",
    //            src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7]);
    return (void *)f;
}

static lv_fs_res_t res_close_cb(lv_fs_drv_t * drv, void * file_p)
{
    LV_UNUSED(drv);
    if(file_p) free_res_file((res_file_t *)file_p);
    return LV_FS_RES_OK;
}

/* 从资源 BIN 中读取数据. */
static lv_fs_res_t res_read_cb(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    LV_UNUSED(drv);
    if(file_p == NULL || buf == NULL) return LV_FS_RES_INV_PARAM;

    res_file_t *f = (res_file_t *)file_p;
    const jl_resource_info_t *res = f->res;

    if(f->pos >= res->size) {
        if(br) *br = 0;
        // printf("[RESFS] read eof: pos=%u size=%u\n", f->pos, res->size);
        return LV_FS_RES_OK;
    }

    uint32_t remain = res->size - f->pos;
    if(btr < remain) remain = btr;

    uint8_t *src = (uint8_t *)(g_bin_base + g_res_hdr->data_offset + res->offset + f->pos);
    memcpy(buf, src, remain);
    f->pos += remain;
    if(br) *br = remain;
    if(remain > 0) {
        ;
        // printf("[RESFS] read chunk: pos=%u remain=%u first=%02X %02X %02X %02X\n",
        //            f->pos, remain, src[0], src[1], src[2], src[3]);
    }
    return LV_FS_RES_OK;
}

/* 调整资源文件读取位置. */
static lv_fs_res_t res_seek_cb(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence)
{
    LV_UNUSED(drv);
    if(file_p == NULL) return LV_FS_RES_INV_PARAM;

    res_file_t *f = (res_file_t *)file_p;
    const jl_resource_info_t *res = f->res;
    uint32_t new_pos = f->pos;

    switch(whence) {
    case LV_FS_SEEK_SET: new_pos = pos; break;
    case LV_FS_SEEK_CUR: new_pos = f->pos + pos; break;
    case LV_FS_SEEK_END: new_pos = res->size + pos; break;
    default: return LV_FS_RES_INV_PARAM;
    }

    if(new_pos > res->size) new_pos = res->size;
    f->pos = new_pos;
    return LV_FS_RES_OK;
}

static lv_fs_res_t res_tell_cb(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    LV_UNUSED(drv);
    if(file_p == NULL || pos_p == NULL) return LV_FS_RES_INV_PARAM;

    res_file_t *f = (res_file_t *)file_p;
    *pos_p = f->pos;
    return LV_FS_RES_OK;
}

/* 注册资源文件系统驱动. */
void res_fs_init(void)
{
    printf("[RESFS] init check: base=0x%08X magic=0x%08X version=%u count=%u data_offset=%u\n",
               (unsigned int)RES_BIN_BASE_ADDR,
               (unsigned int)g_res_hdr->magic,
               (unsigned int)g_res_hdr->version,
               (unsigned int)g_res_hdr->count,
               (unsigned int)g_res_hdr->data_offset);

    if(g_res_hdr->magic != 0x5345524AUL || g_res_hdr->version != 1) {
        printf("[RESFS] init failed: invalid bin header\n");
        return;
    }

    static lv_fs_drv_t s_drv;
    lv_fs_drv_init(&s_drv);
    s_drv.letter = RES_FS_LETTER;
    s_drv.open_cb = res_open_cb;
    s_drv.close_cb = res_close_cb;
    s_drv.read_cb = res_read_cb;
    s_drv.seek_cb = res_seek_cb;
    s_drv.tell_cb = res_tell_cb;
    lv_fs_drv_register(&s_drv);
    printf("[RESFS] driver registered as %c:\n", RES_FS_LETTER);
}
