/******************************************************************************
V1.0 (2025/05/09 16:00:00)
1 初始版本
******************************************************************************/

#include "include.h"
#include "uart.h"
#include "update.h"
#include "crc.h"
#include "tmr.h"
#include "USBBSP.h"
#include "at.h"
#include "ui.h"

const char name[10] __attribute__((section(".ARM.__at_0x8002000"))) = "H5";
const char ver[10] __attribute__((section(".ARM.__at_0x8002010"))) = "1.0";
const char hw[10] __attribute__((section(".ARM.__at_0x8002020"))) = "1.0";

vu8 backlight_flag;

void disp_dbg_code(u8 code)
{}
void disp_dbg_code2(u8 code)
{}
void disp_success(void)
{}
void disp_verifying(void)
{}

void update_process_bar(u8 percent)
{
    process_update(percent);
}

void uart_send(u8* buf, u16 len)
{
    uart_write(UART_ID_AT, buf, len);
}

int q_uart_pop(u8 *byte)
{
    u16 len = uart_read(UART_ID_AT, byte, 1);
    return (len > 0) ? len : -1;
}

//升级类型
static enum
{
    UPD_NULL = 0,
    UPD_APP, //mcu app
    UPD_EXT_FLASH, //ext flash不含logo
    UPD_LOGO, //logo
    UPD_LOGO_EXT_FLASH, //logo和ext flash
} upd_type;

u16 cmd_req;
u8 upd_flg;
u8 upd_anyway; //强制升级
__attribute__((aligned(4))) static u8 buf_s[6*1024];
__attribute__((aligned(4))) static u8 buf_tt[6*1024];
u8 *buf_r;
static u16 i, crc;
static u8 tmp, step = 0;
static s32 ret;
static u16 cnt_r;
static u32 addr;
static s32 pack_num_cur;
static s32 pack_num_last;

static u16 pack_size;
static u8 sn[16];
static u16 len;
static u32 file_size_app;
static u32 file_size_flash;
static u32 total_packs;
static u8 buf_t[20];
static u8 dev_name[10];
static u8 ver_iap[10];
static u8 ver_app[10];
static u8 ver_flash[10];
static u8 cnt_file; //需要升级的文件数量
static u8 cnt_file_bak;
static u16 crc_app, crc_flash;
static u32 code_crc_app, code_crc_flash;

static u32 addr_at425;
static u32 size_at425;
static u32 pack_cnt_at425;
static u32 ticks_to;

typedef void (*P_FUN)(void);
static P_FUN jump;

//IAP回复上位机
u16 pack_buf_s(u16 cmd, u8* data, u16 len)
{
    u16 crc;

    cmd += CMD_OFFSET_TO_PC;

    buf_s[0] = '$';
    buf_s[1] = '$';
    buf_s[2] = (len + 10) & 0xFF;
    buf_s[3] = (len + 10) >> 8;
    buf_s[4] = cmd >> 8;
    buf_s[5] = cmd & 0xFF;

    if (data == NULL)
    {
        len = 0;
    }

    memcpy(&buf_s[6], data, len);

    crc = crc16(buf_s, len + 6, 0);

    buf_s[6 + len] = crc & 0xFF;
    buf_s[7 + len] = crc >> 8;
    buf_s[8 + len] = '\r';
    buf_s[9 + len] = '\n';

    return len + 10;
}

//IAP发命令到AT425
u16 pack_buf_at425(u16 cmd, u8* data, u16 len)
{
    u16 crc;

    buf_s[0] = '@';
    buf_s[1] = '@';
    buf_s[2] = (len + 10) & 0xFF;
    buf_s[3] = (len + 10) >> 8;
    buf_s[4] = cmd >> 8;
    buf_s[5] = cmd & 0xFF;

    if (data == NULL)
    {
        len = 0;
    }

    memcpy(&buf_s[6], data, len);

    crc = crc16(buf_s, len + 6, 0);

    buf_s[6 + len] = crc & 0xFF;
    buf_s[7 + len] = crc >> 8;
    buf_s[8 + len] = '\r';
    buf_s[9 + len] = '\n';

    return len + 10;
}

static u16 app_verify(u8* data, u32 len)
{
    return crc16(data, len, 0);
}

static u16 flash_verify(u32 addr, u32 len)
{
    u16 crc;
    u32 len_tmp;

    if (len < 4096) //必须大于4096
    {
        return 0;
    }

    memcpy((void*)buf_r, (void*)addr, 4096);
    crc = crc16(buf_r, 4096, 0);
    len -= 4096;
    addr += 4096;

    while (len)
    {
        if (len >= 4096)
            len_tmp = 4096;
        else
            len_tmp = len;

        memcpy((void*)buf_r, (void*)addr, len_tmp);
        crc = crc16(buf_r, len_tmp, crc);
        len -= len_tmp;
        addr += len_tmp;
    }

    return crc;
}

void update_init(void)
{
    buf_r = &buf_tt[3];
    ticks_standby = ticks_sys;
}

static u8 at_sub_idx;           // 当前子包索引 (0~3)
static u8 at_sub_total;         // 总子包数
static u8 at_data_buf[PACK_SIZE_MCU]; // 保存当前 4KB 数据
static u16 at_data_len;         // 实际数据长度（可能小于 4096）
static u16 at_pack_num;         // 当前处理的包序号

/**
  ******************************************************************************
  * 函数 : int main(void)
  * 描述 : 主函数
  * 参数 : None
  * 返回 : None
  ******************************************************************************
  */
void update_handle(void)
{
    u16 index;

    switch (step)
    {
    case 0:
        cnt_r = 0;
        //memset(buf_r, 0, sizeof(buf_r));
        ret = VCPReadBytes(&tmp, 1);
        if ((ret == 1) && (tmp == '@'))
        {
            step = 1;
            buf_r[cnt_r++] = tmp;
            ticks_standby = ticks_sys;
            if (!backlight_flag)
            {
                // backlight_on();
            }
        }
        break;

    case 1:
        if (VCPReadBytes(&tmp, 1) == 1)
        {
            if (tmp == '@')
            {
                step = 2;
                buf_r[cnt_r++] = tmp;
            }
            else
            {
                step = 0;
            }
        }
        break;

    case 2: //长度
        ret = VCPReadBytes(&tmp, 1);
        if (1 == ret)
        {
            buf_r[cnt_r++] = tmp;
            if (cnt_r >= 4)
            {
                step = 3;
            }
        }
        break;

    case 3:
        ret = VCPReadBytes(&tmp, 1);
        if (1 == ret)
        {
            buf_r[cnt_r++] = tmp;

            len = (buf_r[3] << 8) + buf_r[2];

            if ((len == cnt_r) && (tmp == '\n') && (buf_r[cnt_r - 2] == '\r'))
            {
                u16 crc;
                crc = crc16(buf_r, cnt_r - 4, 0);
                if (crc == ((buf_r[cnt_r - 3] << 8) + buf_r[cnt_r - 4]))
                {
                    step = 4; //接收到一帧数据
                }
                else
                {
                    step = 0;
                }
            }

            if (len >= (PACK_SIZE_FLASH + 20))
            {
                step = 0;
            }

            if (cnt_r >= (PACK_SIZE_FLASH + 20))
            {
                step = 0;
            }
        }
        break;

    case 4:
        step = 0;
        cmd_req = (buf_r[4] << 8) + buf_r[5];
        if (cmd_req == CMD_SEND_DATA) //下发升级数据包
        {
            memcpy((u8*)&pack_num_cur, &buf_r[7], 2);

            if ((buf_r[6] == 2) || (buf_r[6] == 3))
                step = 5;
            else if (buf_r[6] == 1)
                step = 6;
        }
        else if (cmd_req == CMD_GET_VER) //查询软件版号
        {
            step = 8;
        }
        else if (cmd_req == CMD_SEND_VER) //下发网络端最新软件版本号
        {
            step = 11;
        }
        else if (cmd_req == CMD_SEND_FILE_SIZE) //下发文件大小
        {
            step = 9;
        }
        else if (cmd_req == CMD_GET_SN) //查询序列号
        {
            step = 12;
        }
        else if (cmd_req == CMD_SET_SN) //设置序列号
        {
            step = 7;
            sn[0] = 0xEB;
            sn[1] = 0x85;
            sn[2] = 0xCE;
            memcpy((u8*)&sn[3], &buf_r[6], 12);
            sn[15] = 0;
        }
        break;

    case 5: //写入到外部flash
        if (pack_num_cur > (pack_num_last + 1)) //包不连续
        {
            disp_dbg_code(1);
        }

        if (pack_num_cur <= pack_num_last) //发送之前的包直接返回成功，否则会重复计算CRC
        {
            step = 0;
            buf_t[0] = 0;
            goto ACK_LOOP_FLASH;
        }

        if ((upd_type == UPD_LOGO) || (upd_type == UPD_LOGO_EXT_FLASH))
        {
            addr = FLASH_OFFSET_LOGO + pack_num_cur * pack_size;
        }
        else if (upd_type == UPD_EXT_FLASH)
        {
            addr = FLASH_OFFSET_FLASH + pack_num_cur * pack_size;
        }
        else
        {
            disp_dbg_code(3);
        }

        if ((pack_num_cur + 1) == total_packs) //最后一包
        {
            if (file_size_flash % pack_size)
            {
                pack_size = file_size_flash % pack_size;
            }
        }

	    if (memcmp((void*)addr, (void*)&buf_r[9], pack_size))
	    {
	        if (!(addr % PACK_SIZE_FLASH))
	        {
	            FLASH_EraseSector(addr);
	            CACHE_CleanAll(DCACHE);
	        }

	        FLASH_ProgramPage(NULL, NULL, addr, pack_size, &buf_r[9]);
	        CACHE_CleanAll(DCACHE);

            if (memcmp((void*)addr, (void*)&buf_r[9], pack_size))
            {
                step = 0;
                buf_t[0] = 1;
            }
            else
            {
                step = 10;
                buf_t[0] = 0;
                pack_num_last = pack_num_cur;
            }
	    }
        else
        {
            step = 10;
            buf_t[0] = 0;
            pack_num_last = pack_num_cur;
        }
ACK_LOOP_FLASH:
        memcpy(&buf_t[1], (u8*)&pack_num_cur, 2);
        len = pack_buf_s(CMD_SEND_DATA, buf_t, 3);
        VCPSendBytes(buf_s, len);
        break;

    case 6: //写入到内部rom
        /* @@ + LEN(2) + ID(2) + Payload + CRC(2) + \r\n */
        if (pack_num_cur == 0)
        {
            u32 magic = 0;
            u8 i, xor = 0;

            magic = *((u32 *)&buf_r[9 + 0x3AC]);

            for (i = 0, xor = 0; i < 12; i++)
            {
                xor ^= buf_r[9+0x3AC+i];
            }

            if (magic == 0x5A5A5A5A && xor == buf_r[9+0x3AC+12])
            {
                memcpy((void*)&addr_at425, (void*)&buf_r[9+0x3B0], 4);
                memcpy((void*)&size_at425, (void*)&buf_r[9+0x3B0+4], 4);
                pack_cnt_at425 = size_at425 / pack_size;
                if (size_at425 % pack_size) pack_cnt_at425++;
                buf_t[0] = 1; /* 1: Internal Flash, 2: External Flash (not Logo), 3: Logo / Logo + External Flash. */
                memcpy((void*)&buf_t[1], (void*)&size_at425, 4);
                len = pack_buf_at425(AT_CMD_SEND_FILE_SIZE, buf_t, 5);
                uart_send(buf_s, len);
                step = 101;
                ticks_to = ticks_sys;
                break;
            }
            else
            {
                step = 10;
                buf_t[0] = 0;
                pack_cnt_at425 = 0;
            }
        }

        if (pack_num_cur > (pack_num_last + 1))
        {
            disp_dbg_code(4);
        }

        if (pack_num_cur <= pack_num_last) //发送之前的包直接返回成功，否则会重复计算CRC
        {
            step = 0;
            buf_t[0] = 0;
            goto ACK_LOOP_APP;
        }

        if (pack_cnt_at425 && (pack_num_cur <= pack_cnt_at425))
        {
            /* 转换 ID 后转发给 AT32. */
            step = 101;
            len = cnt_r - 10; /* @@ + 2len + 2cmd + 2crc + \r\n */
            if (len == 0 || len > (pack_size + 3)) printf("[UPDATE] [ERROR] Invalid data len (%d)\n", len);

            len = pack_buf_at425(AT_CMD_SEND_DATA, (u8 *)&buf_r[6], len);
            uart_send(buf_s, len);
        }
        else
        {
            if (pack_cnt_at425)
                addr = ADDR_APP + (pack_num_cur-pack_cnt_at425-1) * pack_size;
            else
                addr = ADDR_APP + pack_num_cur * pack_size;

            if ((pack_num_cur + 1) == total_packs) //最后一包
            {
                if (file_size_app % pack_size)
                {
                    pack_size = file_size_app % pack_size;
                }
            }

            if (memcmp((void*)addr, (void*)&buf_r[9], pack_size))
            {
                if (!(addr % PACK_SIZE_MCU))
                {
                    FLASH_EraseSector(addr);
                    CACHE_CleanAll(DCACHE);
                }

                FLASH_ProgramPage(NULL, NULL, addr, pack_size, &buf_r[9]);
                CACHE_CleanAll(DCACHE);

                if (memcmp((void*)addr, (void*)&buf_r[9], pack_size))
                {
                    step = 0;
                    buf_t[0] = 1;
                }
                else
                {
                    step = 10;
                    buf_t[0] = 0;
                    pack_num_last = pack_num_cur;
                }
            }
            else
            {
                step = 10;
                buf_t[0] = 0;
                pack_num_last = pack_num_cur;
            }
        }
ACK_LOOP_APP:
        memcpy(&buf_t[1], (u8*)&pack_num_cur, 2);
        len = pack_buf_s(CMD_SEND_DATA, buf_t, 3);
        VCPSendBytes(buf_s, len);
        break;

    case 7: //设置设备ID
        step = 0;
#if 0 //一直没用到这个功能，且iap被写保护了，所以关闭这个功能，直接返回失败
        if (!memcmp((void*)ADDR_SN, sn, 16)) //一样的话无需设置
        {
            buf_t[0] = 0;
            len = pack_buf_s(CMD_SET_SN, buf_t, 1);
            VCPSendBytes(buf_s, len);
        }
        else
        {
            flash_unlock();
            addr = ADDR_APP - PACK_SIZE_MCU;
            memcpy((void*)buf_r, (void*)addr, PACK_SIZE_MCU);

            flash_sector_erase(addr);//擦除片内IAP最后一个扇区

            memcpy((void*)&buf_r[PACK_SIZE_MCU - 16], (void*)sn, 16);

            for (i = 0; i < PACK_SIZE_MCU; i += 4)
            {
                flash_word_program(addr + i, *(u32*)&buf_r[i]);
            }

            flash_lock();

            if (memcmp((void*)addr, buf_r, PACK_SIZE_MCU))
            {
                buf_t[0] = 1;
                len = pack_buf_s(CMD_SET_SN, buf_t, 1);
                VCPSendBytes(buf_s, len);
            }
            else
            {
                buf_t[0] = 0;
                len = pack_buf_s(CMD_SET_SN, &buf_t[0], 1);
                VCPSendBytes(buf_s, len);
            }
        }
#endif
        buf_t[0] = 1;
        len = pack_buf_s(CMD_SET_SN, buf_t, 1);
        VCPSendBytes(buf_s, len);
        break;

    case 8: //上报软件版本号
        step = 0;
        memset(buf_r, 0, 512);

        strcpy((char*)&buf_r[0], (char*)name); //产品型号
        strcpy((char*)&buf_r[10], (char*)ver);  //IAP版本号

        if (!memcmp((char*)(ADDR_APP + 0x1000), (char*)name, LEN_IAP_NAME))
        {
            if (*(u16*)ADDR_APP_VER == 0xFFFF)
                strcpy((char*)&buf_r[20], "1.0.0"); //mcu版本号
            else
            {
                strncpy((char*)&buf_r[20], (char*)ADDR_APP_VER, 10);
                buf_r[20+9] = 0;
            }
        }
        else
        {
            strcpy((char*)&buf_r[20], "1.0.0"); //mcu版本号
        }

        memcpy((void*)&buf_r[1024], (void*)FLASH_OFFSET_FLASH, 16); //使用&buf_r[1024]暂存flash 名字

        if (!memcmp((char*)&buf_r[1024], (char*)name, LEN_IAP_NAME))
        {
            memcpy((void*)&buf_r[1024], (void*)ADDR_FLASH_VER, 16);
            if (*(u16*)&buf_r[1024] == 0xFFFF)
                strcpy((char*)&buf_r[30], "1.0.0"); //flash版本号
            else
            {
                strncpy((char*)&buf_r[30], (char*)&buf_r[1024], 10);
                buf_r[30+9] = 0;
            }
        }
        else
        {
            strcpy((char*)&buf_r[30], "1.0.0"); //flash版本号
        }

        if (upd_anyway)
        {
            strcpy((char*)&buf_r[20], "1.0.0"); //mcu版本号
            strcpy((char*)&buf_r[30], "1.0.0"); //flash版本号
        }

        buf_r[40] = PACK_SIZE_MCU & 0xFF;
        buf_r[41] = (PACK_SIZE_MCU >> 8) & 0xFF;
        buf_r[42] = PACK_SIZE_FLASH & 0xFF;
        buf_r[43] = (PACK_SIZE_FLASH >> 8) & 0xFF;

        len = pack_buf_s(CMD_GET_VER, buf_r, 44);
        VCPSendBytes(buf_s, len);
        break;

    case 9: //处理文件大小
        step = 0;
        upd_type = UPD_NULL;
        pack_num_cur = 0;
        pack_num_last = -1;

//        lv_label_set_text(guider_ui.screen_label_update_tip, "Updating...");
//        lv_obj_set_style_text_color(guider_ui.screen_label_update_tip, lv_color_hex(0x00FF00), LV_PART_MAIN|LV_STATE_DEFAULT);

        if (buf_r[6] == 1)
        {
            upd_type = UPD_APP;
            pack_size = PACK_SIZE_MCU;
            memcpy(&file_size_app, &buf_r[7], 4);
            total_packs = (file_size_app+pack_size-1) / pack_size;
            if (file_size_app > SIZE_MCU)
            {
                disp_dbg_code(6);
            }
        }
        else if (buf_r[6] == 2) //flash(logo not include)
        {
            upd_type = UPD_EXT_FLASH;
            pack_size = PACK_SIZE_FLASH;
            memcpy(&file_size_flash, &buf_r[7], 4);
            total_packs = (file_size_flash+pack_size-1) / pack_size;

            if (file_size_flash > SIZE_FLASH)
            {
                disp_dbg_code(7);
            }
        }
        else if (buf_r[6] == 3) //logo or logo+flash
        {
            memcpy(&file_size_flash, &buf_r[7], 4);

            if (file_size_flash > SIZE_LOGO)
            {
                upd_type = UPD_LOGO_EXT_FLASH;
                if (file_size_flash > (SIZE_FLASH+SIZE_LOGO))
                {
                    disp_dbg_code(8);
                }
            }
            else
            {
                upd_type = UPD_LOGO;
            }

            pack_size = PACK_SIZE_FLASH;
            total_packs = (file_size_flash+pack_size-1) / pack_size;
        }
        buf_t[0] = 0;
        memcpy((void*)buf_t, (u8*)&pack_num_cur, 1);
        len = pack_buf_s(CMD_SEND_FILE_SIZE, buf_t, 1);
        VCPSendBytes(buf_s, len);
        break;

    case 10: //显示进度条
        step = 0;
        update_process_bar((pack_num_cur+1)*100 / total_packs);
        if ((pack_num_cur + 1) == total_packs)
        {
            if (cnt_file)
            {
                cnt_file--;
            }
            if (!cnt_file)
            {
                upd_anyway = 0;
                disp_success();
//                lv_refr_now(NULL);
                beep_on();
                delay_ms(200);
                beep_off();
                //delay_ms(3000);
                //to_app();
            }
        }
        break;

    case 11: //接收网络端软件版本号
        step = 0;
        crc_app = 0xFFFF;
        crc_flash = 0xFFFF;
        code_crc_app = 0;
        code_crc_flash = 0;
        cnt_file = buf_r[6];
        cnt_file_bak = cnt_file;
        memcpy((void*)dev_name, (void*)&buf_r[7], 10);
        memcpy((void*)ver_iap, (void*)&buf_r[17], 10);
        memcpy((void*)ver_app, (void*)&buf_r[27], 10);
        memcpy((void*)ver_flash, (void*)&buf_r[37], 10);
        buf_t[0] = 0;
        len = pack_buf_s(CMD_SEND_VER, buf_t, 1);
        VCPSendBytes(buf_s, len);
        break;

    case 12: //查询序列号
        step = 101;
        len = pack_buf_at425(AT_CMD_GET_SN, NULL, 0);
        uart_send(buf_s, len);
        break;

/*
 * *************************** AT32 升级通讯解析 ***************************
 */

    case 101:
        cnt_r = 0;
        //memset(buf_r, 0, sizeof(buf_r));
        ret = q_uart_pop(&tmp);
        if ((ret != -1) && (tmp == '$'))
        {
            step = 102;
            ticks_to = ticks_sys;
            buf_r[cnt_r++] = tmp;
        }
        if ((ticks_sys-ticks_to) > 10*1000)
        {
            step = 0;
            buf_t[0] = 1;
            disp_dbg_code2(101);
            goto ACK_LOOP_APP;
        }
        break;

    case 102:
        if (q_uart_pop(&tmp) != -1)
        {
            if (tmp == '$')
            {
                step = 103;
                ticks_to = ticks_sys;
                buf_r[cnt_r++] = tmp;
            }
            else
            {
                step = 0;
                buf_t[0] = 1;
                disp_dbg_code2(102);
                goto ACK_LOOP_APP;
            }
        }
        if ((ticks_sys-ticks_to) > 10*1000)
        {
            step = 0;
            buf_t[0] = 1;
            disp_dbg_code2(103);
            goto ACK_LOOP_APP;
        }
        break;

    case 103: //长度字节1
        if (q_uart_pop(&tmp) != -1)
        {
            step = 104;
            ticks_to = ticks_sys;
            buf_r[cnt_r++] = tmp;
        }
        if ((ticks_sys-ticks_to) > 10*1000)
        {
            step = 0;
            buf_t[0] = 1;
            disp_dbg_code2(104);
            goto ACK_LOOP_APP;
        }
        break;

    case 104: //长度字节2
        if (q_uart_pop(&tmp) != -1)
        {
            step = 105;
            ticks_to = ticks_sys;
            buf_r[cnt_r++] = tmp;
        }
        if ((ticks_sys-ticks_to) > 10*1000)
        {
            step = 0;
            buf_t[0] = 1;
            disp_dbg_code2(105);
            goto ACK_LOOP_APP;
        }
        break;

    case 105:
        if (q_uart_pop(&tmp) != -1)
        {
            buf_r[cnt_r++] = tmp;

            len = (buf_r[3] << 8) + buf_r[2];

            if ((len == cnt_r) && (tmp == '\n') && (buf_r[cnt_r - 2] == '\r'))
            {
                u16 crc;
                crc = crc16(buf_r, cnt_r - 4, 0);
                if (crc == ((buf_r[cnt_r - 3] << 8) + buf_r[cnt_r - 4]))
                {
                    step = 106; //接收到一帧数据
                }
                else
                {
                    step = 0;
                    buf_t[0] = 1;
                    disp_dbg_code2(106);
                    goto ACK_LOOP_APP;
                }
            }

            if (len >= 100)
            {
                step = 0;
                buf_t[0] = 1;
                disp_dbg_code2(107);
                goto ACK_LOOP_APP;
            }

            if (cnt_r >= 100)
            {
                step = 0;
                buf_t[0] = 1;
                disp_dbg_code2(108);
                goto ACK_LOOP_APP;
            }
        }
        break;

    case 106:
        step = 101;
        cmd_req = (buf_r[4] << 8) + buf_r[5];
        cmd_req -= CMD_OFFSET_FROM_AT425;
        if (cmd_req == AT_CMD_SEND_DATA) //下发升级数据包
        {
            if (!buf_r[6])
            {
                step = 10;
                buf_t[0] = 0;
            }
            else
            {
                step = 0;
                buf_t[0] = 1;
            }

            pack_num_last = pack_num_cur;
            ticks_to = ticks_sys;
            goto ACK_LOOP_APP;
        }
        //else if (cmd_req == CMD_SEND_VER) //下发网络端最新软件版本号
        //{
        //    buf_r[0] = 1;
        //    memcpy((void*)&buf_r[1], (void*)&size_at425, 4);
        //    len = pack_buf_at425(CMD_SEND_FILE_SIZE, buf_r, 5);
        //    uart_send(buf_s, len);
        //}
        else if (cmd_req == AT_CMD_SEND_FILE_SIZE) //下发文件大小
        {
            step = 10;
            buf_t[0] = 0;
            pack_num_last = pack_num_cur;
            ticks_to = ticks_sys;
            goto ACK_LOOP_APP;
        }
        else if (cmd_req == AT_CMD_GET_SN)
        {
            step = 0;
            memcpy((void*)buf_t, (void*)&buf_r[6], 4);
            len = pack_buf_s(CMD_GET_SN, buf_t, 12);
            VCPSendBytes(buf_s, len);
            pack_num_last = pack_num_cur;
            ticks_to = ticks_sys;
        }
        break;

    default:
        step = 0;
        break;
    }
}
