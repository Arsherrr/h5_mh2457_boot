#ifndef __LFS_MSD_H
#define __LFS_MSD_H

int lfs_get_file_size(const char *fn);

unsigned int lfs_get_file_mtime(const char *fn);

void unix_to_fat_time(int utc, int timestamp, uint16_t *fat_date, uint16_t *fat_time);

/**
 * @brief  文件读取回调.
 */
uint32_t lfs_msd_read(const char *fn, uint32_t sector_offset, uint8_t* data, uint32_t num_sectors);


/**
 * @brief  文件写入回调.
 */
void lfs_msd_write(const char *fn, uint32_t sector_offset, const uint8_t* data, uint32_t num_sectors);

#endif
