
#ifndef  _UPDATE_H_
#define  _UPDATE_H_

extern const char name[10];
extern const char ver[10];
extern u32 ticks_standby;

/**************************************************************************************************************************
0x8000000~0x8001000    系统保留 //4K

0x8001000~0x81FE000    IAP //大约2M

0x81FE000~0x81FF000    SN //4K

0x81FF000~0x8200000    升级标志 //4K

0x8200000~0x8400000    APP  //2M

0x8500000~0x8500000    字库 //1M

0x8500000~0x8600000    LOGO //1M

0x8600000~0x8E00000    图片资源 //8M

0x8E00000~0x9000000   系统参数 //2M
这个分配是针对16M LASH来说的
**************************************************************************************************************************/

//配置
#define ROM_BASE                        (0x8001000)
#define ADDR_APP                        (0x080B0000)
//#define ADDR_SN                         (0x81FE000) //存放在AT425
#define UPD_INFO_SIZE                   (1024)
#define ADDR_UPD_INFO                   (0x08301000 - UPD_INFO_SIZE)
#define PACK_SIZE_MCU                   (4096) //MCU每包数据大小
#define PACK_SIZE_FLASH                 (4096) //FLASH每包数据大小
#define FLASH_OFFSET_LOGO               (0x8400000) //包含FLASH和LOGO
#define FLASH_OFFSET_FLASH              (0x8500000)
#define SIZE_LOGO                       (FLASH_OFFSET_FLASH-FLASH_OFFSET_LOGO)
#define SIZE_FLASH                      (24*1024*1024UL) //对于32M的flash来说是这样，16M的flash只有8M
#define SIZE_MCU                        (2*1024*1024)

#define ADDR_APP_VER                    (ADDR_APP+0x1000+0x10) //APP版本号所在地址
#define ADDR_FLASH_VER                  (FLASH_OFFSET_FLASH+0x10) //FLASH版本号所在地址
#define ADDR_APP_CRC                    (ADDR_APP+0x1000+0x30) //APP文件CRC所在地址
#define ADDR_FLASH_CRC                  (FLASH_OFFSET_FLASH+0x30) //FLASH文件CRC所在地址

#define CRC_ATTRI_CODE                  (0x5C6B2023) //CRC特征码

#define LEN_IAP_NAME                    (3) //产品名称的长度


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
void beep_on(void);
void beep_off(void);
void to_app(void);

#endif

