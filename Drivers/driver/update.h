
#ifndef  _UPDATE_H_
#define  _UPDATE_H_
#include "memory.h"
#include "flash.h"

extern const char name[10];
extern const char ver[10];
extern u32 ticks_standby;

/**************************************************************************************************************************
系统保留 [4K]
BOOT [600K]
APP [< 2M]
系统参数 [4K] - 共享信息, SN 等
LOGO [400K]
文本 [14M]
图片 [13M +]
文件系统 [FLASH 末尾 1440K]
**************************************************************************************************************************/

//配置
#define ROM_BASE                        (__BOOT_BASE)
#define ADDR_APP                        (__APP_BASE)
//#define ADDR_SN                         (0x81FE000) //存放在AT425
#define UPD_INFO_SIZE                   (4096)
#define ADDR_UPD_INFO                   (RES_LOGO_BASE - UPD_INFO_SIZE)
#define ADDR_APP_CRC_INFO               (ADDR_UPD_INFO - UPD_INFO_SIZE)
#define PACK_SIZE_MCU                   (4096) //MCU每包数据大小
#define PACK_SIZE_FLASH                 (4096) //FLASH每包数据大小

#define FLASH_OFFSET_LOGO               (RES_LOGO_BASE) //包含FLASH和LOGO
#define FLASH_OFFSET_FLASH              (RES_TEXT_BASE)

#define SIZE_LOGO                       (RES_TEXT_BASE - RES_LOGO_BASE)
#define SIZE_FLASH                      (RES_FS_BASE - RES_LOGO_BASE)
#define SIZE_MCU                        (ADDR_UPD_INFO - ADDR_APP)

#define ADDR_APP_NAME                   (ADDR_APP+0x1000)
#define ADDR_APP_VER                    (ADDR_APP+0x1000+0x10) //APP版本号所在地址
#define ADDR_FLASH_VER                  (FLASH_OFFSET_FLASH+0x10) //FLASH版本号所在地址
#define ADDR_APP_INFO                   (ADDR_APP+0x1000+0x30) //APP信息所在地址
#define ADDR_APP_CRC                    (ADDR_APP+0x1000+0x30) //APP文件CRC所在地址
#define ADDR_FLASH_CRC                  (FLASH_OFFSET_FLASH+0x30) //FLASH文件CRC所在地址

#define CRC_ATTRI_CODE                  (0x5C6B2023) //CRC特征码
#define APP_CRC_INFO_MAGIC              (0x43524341)
#define APP_CRC_INFO_VERSION            (1)

#define AES_GCM_KEY_SIZE                (16)
#define AES_GCM_NONCE_SIZE              (12)
#define AES_GCM_TAG_SIZE                (16)
#define AES_GCM_KEY                     {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F}
#define AES_GCM_NONCE                   {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB}

#define LEN_IAP_NAME                    (4) //产品名称的长度

typedef struct __attribute__((packed)) {
    u32 magic;
    u32 version;
    u32 app_addr;
    u32 app_size;
    u32 app_crc;
    u32 info_crc;
} app_crc_info_t;

#if (PACK_SIZE_FLASH != 4096)
    #error "PACK_SIZE_FLASH must be 4096"
#endif

//获取版本号
#define CMD_GET_VER                     (0x0301)
//下发网络端版本号
#define CMD_SEND_VER                    (0x0302)
//下发升级文件大小
#define CMD_SEND_FILE_SIZE              (0x0303)
//下发升级数据
#define CMD_SEND_DATA                   (0x0304)
//下发升级状态
#define CMD_SEND_UPD_STATUS             (0x0305)
//获取工作模式
#define CMD_GET_WM                      (0x0203)
//获取序列号
#define CMD_GET_SN                      (0x0201)
//设置序列号
#define CMD_SET_SN                      (0x0202)
//MCU上行命令相对于上位机下行命令的偏移值
#define CMD_OFFSET_TO_PC                (0x4000)

#define CMD_OFFSET_FROM_AT425           (0x0100)

//通知425进入APP模式
#define CMD_TO_APP                      (0x0310)
//通知425进入IAP模式
#define CMD_TO_IAP                      (0x0311)

void update_handle(void);
void update_init(void);
s8 app_crc_info_save(u32 app_size);
s8 app_crc_info_check(void);
void beep_on(void);
void beep_off(void);
int to_app(void);

#endif

