#include "lfs.h"
#include "lvgl.h"
#include "lfs_msd.h"

extern lfs_t lfs;

/* 缓存使用动态内存. */
#define USD_MALLOC_MEM  (1)

#define LOGA_PATH       "/customer/H5/meter_save/log/meter_device_a.log"
#define LOGB_PATH       "/customer/H5/meter_save/log/meter_device_b.log"

extern uint8_t *yuv_buf;

static uint8_t *log_buff = NULL;
static int log_cache_size = 0;
static int usb_mode = 0;
static unsigned int mtime = 1784145600; /* 2026-07-15 20:00:00 */

static int log_time_get(void)
{
    unsigned int atime = lfs_get_file_mtime(LOGA_PATH);
    unsigned int btime = lfs_get_file_mtime(LOGB_PATH);

    if (atime == 0 && btime != 0) {    
        mtime = btime;
    } else if (atime != 0 && btime == 0) {
        mtime = atime;
    } else if (atime != 0 && btime != 0) {
        mtime = (atime > btime) ? atime : btime;
    }

    return mtime;
}

static uint32_t log_file_append(const char* path, uint8_t* dest_ptr)
{
    struct lfs_info st;
    lfs_file_t file;
    lfs_ssize_t len = 0;

    /* 获取文件信息. */
    if (lfs_stat(&lfs, path, &st) != LFS_ERR_OK) {
        return 0;
    }

    if (st.size == 0) {
        return 0;
    }

    int err = lfs_file_open(&lfs, &file, path, LFS_O_RDONLY);
    if (err < 0) {
        return 0;
    }

    len = lfs_file_read(&lfs, &file, dest_ptr, st.size);
    lfs_file_close(&lfs, &file);

    return (len > 0) ? (uint32_t)len : 0;
}

/*
 * @brief 创建日志缓存.
 * @retval 0: 成功, -1: 失败.
 * @note 官方的 USBMSD 是在中断中处理的, 
         直接对文件进行操作会有问题, 
         所以这里使用文件缓存.
 */
int log_cache_create(void)
{
    /* 获取两个文件的总大小. */
    int a_size = lfs_get_file_size(LOGA_PATH);
    int b_size = lfs_get_file_size(LOGB_PATH);
    int fsize  = a_size + b_size;

    /* 清零. */
    log_cache_size = 0;

    if (fsize == 0) {
        printf("[USB] Empty files.\n");
        return 0;
    }

    /* 直接使用 jpeg_hw.c 中的 yuv_buf, 在日志上传期间用不到. */
#if USD_MALLOC_MEM
    /* 申请空间. */
    log_buff = (uint8_t *)lv_malloc(fsize);
    if (log_buff == 0) {
        printf("[USB] [ERROR] OOM.\n");
        return -1;
    }
#else
    log_buff = yuv_buf;
#endif

    lv_memset(log_buff, 0, fsize);

    /* 只有 a.log */
    if (a_size != 0 && b_size == 0) {
        fsize = log_file_append(LOGA_PATH, log_buff);
        goto OUT;
    }

    /* 只有 b.log */
    if (b_size != 0 && a_size == 0) {
        fsize = log_file_append(LOGB_PATH, log_buff);
        goto OUT;
    }

    /* 两个文件都存在. */
    // 比较最后的修改时间 st_mtime
    // if (stat_b.st_mtime > stat_a.st_mtime)
    // 比较文件大小, 时间不一定准.
    if (a_size > b_size) {   
        /* 先拷贝 a.log */
        fsize = log_file_append(LOGA_PATH, log_buff);
        /* 再拷贝 b.log */
        fsize += log_file_append(LOGB_PATH, log_buff + fsize);
    } else {
        /* 先拷贝 b.log */
        fsize = log_file_append(LOGB_PATH, log_buff);
        /* 再拷贝 a.log */
        fsize += log_file_append(LOGA_PATH, log_buff + fsize);
    }

OUT:
    log_cache_size = fsize;

    /* 获取修改时间. */
    log_time_get();

    return 0;
}

void log_cache_destroy(void)
{
#if USD_MALLOC_MEM
    if (log_buff) lv_free(log_buff);
#endif
}

/**
 * @brief 获取日志缓存.
 * @retval 缓存地址.
 */
void *get_log_cache(void)
{
    return log_buff;
}

/**
 * @brief 获取日志修改时间.
 * @retval 日志修改时间.
 */ 
unsigned int get_log_mtime(void)
{
    return mtime;
}

/**
 * @brief 获取日志缓存大小.
 * @retval 缓存大小.
 */
int get_log_cache_size(void)
{
    return log_cache_size;
}