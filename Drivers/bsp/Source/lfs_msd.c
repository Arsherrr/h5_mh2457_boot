#include "lfs.h"
#include <string.h>
#include <time.h>

#define FAT_SECTOR_SIZE    ( 512 )   /* FAT文件系统的标准扇区大小. */

extern lfs_t lfs;

/**
 * @brief  获取 LittleFS 文件大小
 * @param  fn 文件路径
 * @return int 文件大小(字节数),如果文件不存在或出错返回 0
 */
int lfs_get_file_size(const char *fn)
{
    struct lfs_info info;

    if (lfs_stat(&lfs, fn, &info) == LFS_ERR_OK) {
        return (int)info.size;
    }

    return 0;
}

unsigned int lfs_get_file_mtime(const char *fn)
{
#define ATTR_TIMESTAMP  ( 0x74 )

    time_t mtime = 0;

    int res = lfs_getattr(&lfs, fn, ATTR_TIMESTAMP, &mtime, sizeof(time_t));
    if (res >= 0) {
        return (unsigned int)mtime;
    }

    return 0;
}

void unix_to_fat_time(int utc, int timestamp, uint16_t *fat_date, uint16_t *fat_time)
{
    int32_t  utc_time = utc * 3600;
    uint32_t local_time = (uint32_t)(timestamp + utc_time); 

    uint32_t seconds_in_day = local_time % 86400;
    uint32_t days_since_epoch = local_time / 86400;

    /* 时分秒. */
    int hour   = seconds_in_day / 3600;
    int minute = (seconds_in_day % 3600) / 60;
    int second = seconds_in_day % 60;

    /* 年, 月, 日 (1970 年 1 月 1 日开始算起, 包含闰年处理). */
    int year = 1970;
    while (1) {
        int days_in_year = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 366 : 365;
        if (days_since_epoch >= days_in_year) {
            days_since_epoch -= days_in_year;
            year++;
        } else {
            break;
        }
    }

    int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        month_days[1] = 29; /* 闰年 2 月 29 天. */
    }

    int month = 0;
    while (days_since_epoch >= month_days[month]) {
        days_since_epoch -= month_days[month];
        month++;
    }
    month += 1; /* 转换为 1-12 月. */
    int day = days_since_epoch + 1; /* 转换为 1-31 日. */

    /* FAT 边界安全限制. */
    if (year < 1980) {
        year = 1980; month = 1; day = 1;
    } else if (year > 2107) {
        year = 2107;
    }

    /* 标准 FAT 格式拼装. */
    *fat_date = (uint16_t)(((year - 1980) << 9) | (month << 5) | day);
    *fat_time = (uint16_t)((hour << 11) | (minute << 5) | (second >> 1));
}

/**
 * @brief  文件读取回调(将 LittleFS 中的文件模拟成扇区数据读出)
 */
uint32_t lfs_msd_read(const char *fn, uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    lfs_file_t file;
    lfs_off_t byte_offset = (lfs_off_t)sector_offset * FAT_SECTOR_SIZE;
    lfs_size_t byte_len = (lfs_size_t)num_sectors * FAT_SECTOR_SIZE;
    lfs_ssize_t bytes_read = 0;

    /* 只读模式打开文件. */
    int err = lfs_file_open(&lfs, &file, fn, LFS_O_RDONLY);
    if (err < 0) {
        /* 如果文件打开失败, 填充全 0 确保虚拟出的扇区设备不会死机 */
        memset(data, 0, byte_len);
        return byte_len; /* 返回请求的扇区数, 欺骗电脑文件还在. */
    }

    /* 移动文件指针到对应字节位置. */
    if (lfs_file_seek(&lfs, &file, byte_offset, LFS_SEEK_SET) >= 0) {
        bytes_read = lfs_file_read(&lfs, &file, data, byte_len);
    }

    lfs_file_close(&lfs, &file);

    /* 如果实际读取的数据不足(到了文件末尾), 剩余部分填充 0 */
    if (bytes_read < 0) {
        bytes_read = 0;
    }

    if ((size_t)bytes_read < byte_len) {
        memset(data + bytes_read, 0, byte_len - (size_t)bytes_read);
    }

    return byte_len;
}

/**
 * @brief  文件写入回调(将扇区数据写入 LittleFS 文件中)
 */
void lfs_msd_write(const char *fn, uint32_t sector_offset, const uint8_t* data, uint32_t num_sectors)
{
    lfs_file_t file;
    lfs_off_t byte_offset = (lfs_off_t)sector_offset * FAT_SECTOR_SIZE;
    lfs_size_t byte_len = (lfs_size_t)num_sectors * FAT_SECTOR_SIZE;

    /* 读写模式, 如果文件不存在则创建 (LFS_O_RDWR | LFS_O_CREAT). */
    int err = lfs_file_open(&lfs, &file, fn, LFS_O_RDWR | LFS_O_CREAT);
    if (err < 0) {
        return;
    }

    /* 移动文件指针到要写入的位置. */
    if (lfs_file_seek(&lfs, &file, byte_offset, LFS_SEEK_SET) >= 0) {
        lfs_file_write(&lfs, &file, data, byte_len);
    }

    lfs_file_close(&lfs, &file);
}
