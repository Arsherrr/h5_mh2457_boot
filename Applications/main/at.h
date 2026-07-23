#ifndef __AT_H
#define __AT_H

#define AT_PACK_SIZE                       (1024)

/* 获取版本号. */
#define AT_CMD_GET_VER                     (0xF001)

/* 下发网络端版本号. */
#define AT_CMD_SEND_VER                    (0xF002)

/* 下发升级文件大小. */
#define AT_CMD_SEND_FILE_SIZE              (0xF003)

/* 下发升级数据. */
#define AT_CMD_SEND_DATA                   (0xF004)

/* 下发升级状态. */
#define AT_CMD_SEND_UPD_STATUS             (0xF005)

/* 获取工作模式. */
#define AT_CMD_GET_WM                      (0xF006)

/* 获取序列号. */
#define AT_CMD_GET_SN                      (0xF007)

/* 设置序列号. */
#define AT_CMD_SET_SN                      (0xF008)

#endif
