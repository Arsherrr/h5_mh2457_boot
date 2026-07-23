#include <stdio.h>
#include "lfs.h"
#include "flash.h"
#include "memory.h"
#include <string.h>
#include "lvgl.h"

lfs_t lfs;
struct lfs_config cfg;
int lfs_inited = 0;

/**
 * @brief littlefs 读.
 */
static int fs_read(const struct lfs_config *c, 
                   lfs_block_t block,
                   lfs_off_t off, 
                   void *buffer, 
                   lfs_size_t size)
{
    long offset = (long)(block * FLASH_SECTOR_SIZE + off);
    if (flash_read(offset, (uint8_t *)buffer, size) >= 0) {
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

/**
 * @brief littlefs 写.
 */
static int fs_prog(const struct lfs_config *c, 
                   lfs_block_t block,
                   lfs_off_t off, 
                   const void *buffer, 
                   lfs_size_t size)
{
    long offset = (long)(block * FLASH_SECTOR_SIZE + off);
    if (flash_write(offset, (const uint8_t *)buffer, size) >= 0) {
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

/**
 * @brief littlefs 擦除.
 */
static int fs_erase(const struct lfs_config *c, lfs_block_t block)
{
    long offset = (long)(block * FLASH_SECTOR_SIZE);
    if (flash_erase(offset, FLASH_SECTOR_SIZE) >= 0) {
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

/**
 * @brief littlefs 同步.
 */
static int fs_sync(const struct lfs_config *c)
{
    return LFS_ERR_OK;
}

static void lfs_config(void)
{
    flash_init();

    cfg.read  = fs_read;
    cfg.prog  = fs_prog;
    cfg.erase = fs_erase;
    cfg.sync  = fs_sync;

    /* 几何参数配置. */
    cfg.read_size = 1;                           
    cfg.prog_size = 1;                           
    cfg.block_size = FLASH_SECTOR_SIZE;
    cfg.block_count = (FLASH_SIZE / FLASH_SECTOR_SIZE);

    /* 静态分配缓冲区. */
    cfg.cache_size = 64;                         
    cfg.lookahead_size = 16;                     
    cfg.block_cycles = 500;                      

    static uint8_t read_buf[64];
    static uint8_t prog_buf[64];
    static uint8_t lookahead_buf[16];
    
    cfg.read_buffer = read_buf;
    cfg.prog_buffer = prog_buf;
    cfg.lookahead_buffer = lookahead_buf;
}

static void fs_test(lfs_t *lfs, const char *path)
{
    lfs_dir_t dir;
    struct lfs_info info;
    char next_path[256]; // 用于拼接子目录路径的缓冲区

    // 打开当前目录
    int err = lfs_dir_open(lfs, &dir, path);
    if (err < 0) {
        printf("Failed to open %s, error: %d\r\n", path, err);
        return;
    }

    // 循环读取当前目录下的所有条目
    while (1) {
        int res = lfs_dir_read(lfs, &dir, &info);
        if (res < 0) {
            printf("Error reading %s: %d\r\n", path, res);
            break;
        }
        if (res == 0) {
            break; // 读取完毕
        }

        // 严格过滤掉 "." 和 ".." 目录，否则会导致无限死循环！
        if (strcmp(info.name, ".") == 0 || strcmp(info.name, "..") == 0) {
            continue;
        }

        // 1. 如果是文件，直接打印绝对路径和大小
        if (info.type == LFS_TYPE_REG) {
            if (strcmp(path, "/") == 0) {
                printf("[FILE] /%s  (%d bytes)\r\n", info.name, info.size);
            } else {
                printf("[FILE] %s/%s  (%d bytes)\r\n", path, info.name, info.size);
            }
        } 
        // 2. 如果是文件夹，先打印当前文件夹路径，再深入递归读取
        else if (info.type == LFS_TYPE_DIR) {
            // 拼接子目录的完整路径
            if (strcmp(path, "/") == 0) {
                snprintf(next_path, sizeof(next_path), "/%s", info.name);
            } else {
                snprintf(next_path, sizeof(next_path), "%s/%s", path, info.name);
            }

            printf("[DIR]  %s/\r\n", next_path);

            // 核心：递归调用自身，去读取子目录内部的文件
            fs_test(lfs, next_path);
        }
    }

    // 关闭当前目录句柄
    lfs_dir_close(lfs, &dir);
}

/**
 * @brief 从 JSON 字符串中安全、高效地提取整型键值
 * @param json_str 包含完整 JSON 内容的输入缓冲区
 * @param key 需要查找的键名，必须带双引号，例如 "\"language\""
 * @param out_val 用于存放解析出来的整型数值的指针
 * @return int 0 代表成功获取，-1 代表未找到该键或格式错误
 */
int get_json_int_value(const char *json_str, const char *key, int *out_val) {
    // 1. 在整个 JSON 字符串中精确定位键名
    char *key_ptr = strstr(json_str, key);
    if (key_ptr == NULL) {
        return -1; // 未找到该关键字
    }

    // 2. 找到键名后面紧跟的冒号 ':'
    char *colon_ptr = strchr(key_ptr, ':');
    if (colon_ptr == NULL) {
        return -1; // JSON 格式异常，没有冒号
    }

    // 3. 越过冒号，跳过中间可能存在的任何空格、制表符或换行符
    char *val_ptr = colon_ptr + 1;
    while (*val_ptr == ' ' || *val_ptr == '\t' || *val_ptr == '\r' || *val_ptr == '\n') {
        val_ptr++;
    }

    // 4. 将字符串形式的数字转换为真正的整数
    // 标准的 atoi 函数非常聪明，它遇到非数字字符（比如逗号 ','、换行 '\n' 或右大括号 '}'）会自动停止解析并安全返回
    *out_val = atoi(val_ptr);
    return 0;
}

// "system": {
//     "language": 1,
//     "unit": 0,
//     "button_sound": 0,
//     "volume": 0,
//     "alarm_beep": 0,
//     "lcd_direction": 0,
//     "lcd_brightness": 100,
//     "run_mode": 0,
//     "utc_offset": 800,
//     "car_dtc_id": 0,
//     "theme": 0,
//     "auto_set_theme_flag": 0
//     "ShowMeterOneIndex": 0
//   },
//   "alarm": {
//     "rpm": 5000,
//     "vss": 150,
//     "ect": 110,
//     "vol": 800
//   },
//   "car": {
//     "capacity": 16,
//     "eng_on_vol": 1210,
//     "vol_offset": 0,
//     "mile_init": 0,
//     "fuel_init": 0.00,
//     "mile_ratio": 100,
//     "fuel_ratio": 100,
//     "stop_start": 0
//   }
//   "tpms": {
//     "left_front": "000000000000",
//     "right_front": "000000000000",
//     "right_rear": "000000000000",
//     "left_rear": "000000000000"
//   }
//   "wifi": {
//     "ssd": "",
//     "pwd": "",
//   }
// }

void boot_read_config_file(lfs_t *lfs) {
    lfs_file_t file;
    struct lfs_info info;
    const char *file_path = "/customer/H5/meter_save/meter_device_config.json";

    printf("\r\n--- Checking File Stat: %s ---\r\n", file_path);

    // 1. 使用 lfs_stat 检查文件是否存在并获取其状态 [1, 3]
    int err = lfs_stat(lfs, file_path, &info);
    if (err < 0) {
        if (err == LFS_ERR_NOENT) { // -2
            printf("[WARN] File does not exist! (lfs_stat returned LFS_ERR_NOENT)\r\n");
        } else {
            printf("[ERROR] lfs_stat failed, error code: %d\r\n", err);
        }
        return; // 文件不存在，安全退出
    }

    // 2. 确认这是一个普通文件，并打印其大小 [1]
    if (info.type != LFS_TYPE_REG) {
        printf("[WARN] Target path is not a regular file!\r\n");
        return;
    }
    printf("File found! Size: %u bytes.\r\n", (unsigned int)info.size);

    // 如果文件大小为 0，则无需分配内存
    if (info.size == 0) {
        printf("[WARN] File is empty.\r\n");
        return;
    }

    // 3. 使用 lv_malloc 动态分配内存（额外多给 1 个字节用来存放字符串结束符 '\0'）
    size_t alloc_size = info.size + 1;
    char *dynamic_buf = (char *)lv_malloc(alloc_size);
    if (dynamic_buf == NULL) {
        printf("[ERROR] lv_malloc failed! Not enough RAM to allocate %d bytes.\r\n", alloc_size);
        return; // 内存分配失败，防止空指针异常
    }

    // 确保整个缓冲区清零
    memset(dynamic_buf, 0, alloc_size);

    // 4. 正式打开并读取文件 [1]
    err = lfs_file_open(lfs, &file, file_path, LFS_O_RDONLY);
    if (err < 0) {
        printf("[ERROR] lfs_file_open failed, error code: %d\r\n", err);
        lv_free(dynamic_buf); // 打开失败，务必释放刚刚申请的内存！
        return;
    }

    lfs_ssize_t read_size = lfs_file_read(lfs, &file, dynamic_buf, info.size);
    if (read_size < 0) {
        printf("[ERROR] lfs_file_read failed! Error code: %d\r\n", (int)read_size);
    } else {
        printf("[SUCCESS] Read %d bytes into dynamic buffer.\r\n", (int)read_size);
        printf("================= DYNAMIC JSON CONTENT =================\r\n");
        printf("%s\r\n", dynamic_buf); // 此时 dynamic_buf[info.size] 必然是 \0，可安全作为字符串打印
        printf("========================================================\r\n");
        
        // 5. 在这里进行你的业务逻辑，例如解析 JSON 配置
        // your_json_parse_func(dynamic_buf);
    }

    int language = 0;
    int lcd_direction = 0;
    int lcd_brightness = 0;

    // 提取 language
    if (get_json_int_value(dynamic_buf, "\"language\"", &language) == 0) {
        printf("[SUCCESS] (language): %d\r\n", language);
    } else {
        printf("[FAILED] language not found\r\n");
    }

    // 提取 lcd_direction
    if (get_json_int_value(dynamic_buf, "\"lcd_direction\"", &lcd_direction) == 0) {
        printf("[SUCCESS] (lcd_direction): %d\r\n", lcd_direction);
    } else {
        printf("[FAILED] lcd_direction not found\r\n");
    }

    // 提取 lcd_brightness
    if (get_json_int_value(dynamic_buf, "\"lcd_brightness\"", &lcd_brightness) == 0) {
        printf("[SUCCESS] (lcd_brightness): %d\r\n", lcd_brightness);
    } else {
        printf("[FAILED] lcd_brightness not found\r\n");
    }

    // 6. 最后的清理工作：关闭文件，并释放动态内存 [1]
    lfs_file_close(lfs, &file);
    lv_free(dynamic_buf); 
    printf("Memory and file handles released.\r\n");
}

void fs_init(void)
{
    if (lfs_inited) return;
    
    /* LFS 参数配置. */
    lfs_config();

    /* LFS 挂载. */
    int err = lfs_mount(&lfs, &cfg);
    if (err < 0) {
        printf("[LFS] Mount failed! Error code: %d\r\n", err);
        return;
    }

    printf("[LFS] Mounted successfully!\r\n");
    
    lfs_inited = 1;
}
