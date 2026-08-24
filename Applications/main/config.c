#include "lfs.h"
#include "mh2457.h"
#include "lvgl.h"

#define CONFIG_PATH  "/customer/H5/meter_save/meter_device_config.json"
#define IMU_CFG_PATH "/customer/H5/meter_save/qmi8658_calib.bin"

extern lfs_t lfs;
extern void display_set_rotation(uint32_t value);

int language = 3; /* 默认英语. */
int lcd_direction = 0;
int lcd_brightness = 100;
int utc_offset = 0;

static int delete_file(const char *path)
{
    int err = lfs_remove(&lfs, path);
    if (err == LFS_ERR_OK) {
        printf("[CFG] File deleted successfully.\r\n");
        return 0;
    } else if (err == LFS_ERR_NOENT) {
        printf("[CFG] File does not exist, no need to delete.\r\n");
        return 0; 
    } else {
        printf("[CFG] [ERROR] Failed to delete file, error code: %d\r\n", err);
        return err;
    }
}

/**
 * @brief 从 JSON 字符串中安全、高效地提取整型键值
 * @param json_str 包含完整 JSON 内容的输入缓冲区
 * @param key 需要查找的键名，必须带双引号，例如 "\"language\""
 * @param out_val 用于存放解析出来的整型数值的指针
 * @return int 0 代表成功获取，-1 代表未找到该键或格式错误
 */
static int json_get_value(const char *json_str, const char *key, int *out_val)
{
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

// 配置文件
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

void read_config(void)
{
    lfs_file_t file;
    struct lfs_info info;
    const char *file_path = CONFIG_PATH;

    int err = lfs_stat(&lfs, file_path, &info);
    if (err < 0) {
        if (err == LFS_ERR_NOENT) {
            printf("[CFG] [WARN] File does not exist!\r\n");
        } else {
            printf("[CFG] [ERROR] Read stat failed, error code: %d\r\n", err);
        }
        return;
    }

    if (info.type != LFS_TYPE_REG) {
        printf("[CFG] [WARN] Target path is not a regular file!\r\n");
        return;
    }
    printf("[CFG] File found! Size: %u bytes.\r\n", (unsigned int)info.size);

    if (info.size == 0) {
        printf("[CFG] [WARN] File is empty.\r\n");
        return;
    }

    size_t alloc_size = info.size + 1;
    char *config_buf = (char *)(SDRAM_MEM_BASE);

    memset(config_buf, 0, alloc_size);

    /* 读取配置文件. */
    err = lfs_file_open(&lfs, &file, file_path, LFS_O_RDONLY);
    if (err < 0) {
        printf("[CFG] [ERROR] Open file failed, error code: %d\r\n", err);
        return;
    }

    lfs_ssize_t read_size = lfs_file_read(&lfs, &file, config_buf, info.size);
    if (read_size < 0) {
        printf("[CFG] [ERROR] Read file failed! Error code: %d\r\n", (int)read_size);
    } else {
        /* 提取 language. */
        if (json_get_value(config_buf, "\"language\"", &language) == 0) {
            printf("[CFG] language: %d\r\n", language);
        } else {
            printf("[CFG] [WARN] language not found\r\n");
        }

        /* 提取 lcd_direction. */
        if (json_get_value(config_buf, "\"lcd_direction\"", &lcd_direction) == 0) {
            printf("[CFG] lcd_direction: %d\r\n", lcd_direction);
        } else {
            printf("[CFG] [WARN] lcd_direction not found\r\n");
        }

        /* 提取 lcd_brightness. */
        if (json_get_value(config_buf, "\"lcd_brightness\"", &lcd_brightness) == 0) {
            printf("[CFG] lcd_brightness: %d\r\n", lcd_brightness);
        } else {
            printf("[CFG] [WARN] lcd_brightness not found\r\n");
        }

        /* 提取 utc_offset. */
        if (json_get_value(config_buf, "\"utc_offset\"", &utc_offset) == 0) {
            printf("[CFG] utc_offset: %d\r\n", utc_offset);
        } else {
            printf("[CFG] [WARN] utc_offset not found\r\n");
        }
    }

    lfs_file_close(&lfs, &file);

    /* 设置屏幕显示方向. */
    display_set_rotation(lcd_direction ? LV_DISPLAY_ROTATION_180 : LV_DISPLAY_ROTATION_0);
}

/**
 * @brief 恢复默认设置.
 * @note  删除对应文件达到重置的效果.
 *        [0] 菜单配置文件
 *        [1] IMU 配置文件
 */
int reset_config(void)
{
    int ret = 0;

    ret = delete_file(CONFIG_PATH);
    if (ret != 0) return ret;

    return delete_file(IMU_CFG_PATH);
}

int system_restart(void)
{
    int err = lfs_unmount(&lfs);
    if (err < 0) {
        printf("[LFS] [ERROR] Unmount fs failed (%d).\n", err);
    }

    /* 重置. */
    NVIC_SystemReset();
}

int get_lang(void)
{
    return language;
}
