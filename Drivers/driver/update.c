/******************************************************************************
V1.0 (2025/05/09 16:00:00)
1 初始版本
******************************************************************************/

#include <stdlib.h>
#include "include.h"
#include "uart.h"
#include "update.h"
#include "crc.h"
#include "tmr.h"
#include "USBBSP.h"
#include "at.h"
#include "ui_ota.h"
#include "memory.h"
#include "text.h"
#include "res.h"
#include "aes_gcm.h"

const char name[10] __attribute__((section(".ARM.__at_0x8002000"))) = "KWH5";
const char ver[10] __attribute__((section(".ARM.__at_0x8002010"))) = "1.0.0";
const char hw[10] __attribute__((section(".ARM.__at_0x8002020"))) = "1.0.0";

extern int language;

vu8 backlight_flag;

void disp_dbg_code(s8 code)
{
    static char dbg_buff[100] = {0};

    switch (code) {
    case 0:
        ui_set_notice_id(TEXT_OTA_UPGRADING, COLOR_WHITE);
        break;

    case 1:
        /* 正在更新 LOGO. */
        ui_set_notice_id(TEXT_OTA_UPGRADE_LOGO, COLOR_WHITE);
        break;
    
    case 2:
        /* 正在更新图片. */
        ui_set_notice_id(TEXT_OTA_UPGRADE_IMG, COLOR_WHITE);
        break;

    case 3:
        /* 正在更新文本. */
        ui_set_notice_id(TEXT_OTA_UPGRADE_TEXT, COLOR_WHITE);
        break;

#if 0
    case -1:
        /* 包序号不连续. */
        ui_set_pct_color(COLOR_RED);
        ui_set_notice("Discontinuous packet sequence.", COLOR_RED);
        break;

    case -2:
        /* 无效 FLASH BIN 文件. */
        ui_set_pct_color(COLOR_RED);
        ui_set_notice("Invalid flash file.", COLOR_RED);
        break;

    case -3:
        /* 无效 FLASH 类型 (上位机的问题). */
        ui_set_pct_color(COLOR_RED);
        ui_set_notice("Invalid file type.", COLOR_RED);
        break;

    case -6:
    case -7:
    case -8:
        /* 文件大小超出限制. */
        ui_set_pct_color(COLOR_RED);
        ui_set_notice("Firmware size exceeds capacity.", COLOR_RED);
        break;
#endif

    default:
        sprintf(dbg_buff, "%s (%c%d)", get_string(language, TEXT_OTA_FAILED), 
                                       (code < 0) ? 'E' : 'U', 
                                       (code < 0) ? -code : code);
        ui_set_pct_color(COLOR_RED);
        ui_set_notice(dbg_buff, COLOR_RED);
        break;
    }
}

void disp_dbg_code2(u8 code)
{
    static char msg[100] = {0};
    ui_set_pct_color(COLOR_RED);
    sprintf(msg, "%s (E%d)", get_string(language, TEXT_OTA_FAILED), code);
    ui_set_notice(msg, COLOR_RED);
}

void disp_success(void)
{
    ui_set_pct_visible(0);
    ui_set_notice_id(TEXT_OTA_SUCCESS, COLOR_GREEN);
}

void disp_verifying(void)
{
    ui_set_pct_visible(0);
    ui_set_notice("Verifying...", COLOR_WHITE);
}

void update_process_bar(u16 percent)
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
static u32 base_addr;
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

static u8 aes_gcm_enabled;
static u32 aes_gcm_cipher_size;
static u8 aes_gcm_tag[AES_GCM_TAG_SIZE];
static const u8 aes_gcm_key[AES_GCM_KEY_SIZE] = AES_GCM_KEY;
static const u8 aes_gcm_nonce[AES_GCM_NONCE_SIZE] = AES_GCM_NONCE;
static aes_gcm_ctx_t aes_gcm_ctx;
static u32 aes_gcm_decrypted_size;
static u32 app_decrypted_size;
static u32 flash_decrypted_size;

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

static u32 app_crc_info_calc_crc(app_crc_info_t *info)
{
    return crc32_hw((u8*)info, sizeof(app_crc_info_t) - sizeof(info->info_crc));
}

static void app_aes_gcm_start(u32 cipher_size)
{
    aes_gcm_cipher_size = cipher_size;
    aes_gcm_decrypted_size = 0;
    app_decrypted_size = 0;
    flash_decrypted_size = 0;
    memset((void*)aes_gcm_tag, 0, sizeof(aes_gcm_tag));
    aes_gcm_init(&aes_gcm_ctx, aes_gcm_key, aes_gcm_nonce);
    aes_gcm_enabled = 1;
}

static u32 get_effective_payload_len(u32 file_size, u32 packet_index, u32 packet_payload_size, u32 frame_payload_len)
{
    u32 sent_size = packet_index * packet_payload_size;
    u32 remain_size;

    if (sent_size >= file_size)
    {
        return 0;
    }

    remain_size = file_size - sent_size;
    return (remain_size < frame_payload_len) ? remain_size : frame_payload_len;
}

static s8 app_aes_gcm_decrypt_payload(u8 *data, u32 data_len, u32 *plain_len)
{
    u32 decrypt_len;
    u32 tag_len;

    *plain_len = data_len;

    if (!aes_gcm_enabled)
    {
        return 0;
    }

    if (aes_gcm_decrypted_size >= aes_gcm_cipher_size)
    {
        decrypt_len = 0;
    }
    else if ((aes_gcm_decrypted_size + data_len) > aes_gcm_cipher_size)
    {
        decrypt_len = aes_gcm_cipher_size - aes_gcm_decrypted_size;
    }
    else
    {
        decrypt_len = data_len;
    }

    if (decrypt_len)
    {
        aes_gcm_decrypt_update(&aes_gcm_ctx, data, decrypt_len);
        aes_gcm_decrypted_size += decrypt_len;
    }

    tag_len = data_len - decrypt_len;
    if (tag_len)
    {
        if (tag_len > AES_GCM_TAG_SIZE)
        {
            return -2;
        }
        memcpy((void*)aes_gcm_tag, (void*)&data[decrypt_len], tag_len);
    }

    *plain_len = decrypt_len;
    return 0;
}

static s8 app_aes_gcm_finish(void)
{
    if (!aes_gcm_enabled)
    {
        return 0;
    }

    if (aes_gcm_decrypted_size != aes_gcm_cipher_size)
    {
        return -1;
    }

    return aes_gcm_decrypt_finish(&aes_gcm_ctx, aes_gcm_tag);
}

s8 app_crc_info_save(u32 app_size)
{
    app_crc_info_t info;

    if ((app_size == 0) || (app_size > SIZE_MCU))
    {
        return -1;
    }

    info.magic = APP_CRC_INFO_MAGIC;
    info.version = APP_CRC_INFO_VERSION;
    info.app_addr = ADDR_APP;
    info.app_size = app_size;
    info.app_crc = crc32_flash_hw(ADDR_APP, app_size, buf_tt, sizeof(buf_tt));
    crc32_hw_init();
    info.info_crc = app_crc_info_calc_crc(&info);

    FLASH_EraseSector(ADDR_APP_CRC_INFO);
    CACHE_CleanAll(DCACHE);
    FLASH_ProgramPage(NULL, NULL, ADDR_APP_CRC_INFO, sizeof(info), (u8*)&info);
    CACHE_CleanAll(DCACHE);

    if (memcmp((void*)ADDR_APP_CRC_INFO, (void*)&info, sizeof(info)) != 0)
    {
        return -2;
    }

    return 0;
}

s8 app_crc_info_check(void)
{
    return 0;
    
    app_crc_info_t info;
    u32 crc;

    memcpy((void*)&info, (void*)ADDR_APP_CRC_INFO, sizeof(info));

    if ((info.magic != APP_CRC_INFO_MAGIC) ||
        (info.version != APP_CRC_INFO_VERSION) ||
        (info.app_addr != ADDR_APP) ||
        (info.app_size == 0) ||
        (info.app_size > SIZE_MCU))
    {
        return -1;
    }

    crc32_hw_init();
    if (app_crc_info_calc_crc(&info) != info.info_crc)
    {
        return -2;
    }

    crc = crc32_flash_hw(ADDR_APP, info.app_size, buf_tt, sizeof(buf_tt));
    if (crc != info.app_crc)
    {
        return -3;
    }

    return 0;
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

static s8 res_type = -1;

/* TEXT / IMAGE 先写数据、最后再写头，升级中断时 magic 无效，避免半包资源被当成可用. */
#define RES_HDR_SLOT_IMG  0
#define RES_HDR_SLOT_TXT  1
#define RES_HDR_PAGE_SIZE QSPI_PAGE_SIZE

typedef struct {
    u8 pending;
    u16 len;
    u32 addr;
    u8 buf[RES_HDR_PAGE_SIZE];
} res_hdr_stash_t;

static res_hdr_stash_t res_hdr_stash[2];

static s8 res_hdr_slot_by_type(s8 type)
{
    if (type == 2) {
        return RES_HDR_SLOT_IMG;
    }
    if (type == 3) {
        return RES_HDR_SLOT_TXT;
    }
    return -1;
}

static s8 res_flash_program(u32 addr, const u8 *data, u32 len)
{
    u32 off = 0;

    while (off < len) {
        u32 chunk = RES_HDR_PAGE_SIZE - ((addr + off) % RES_HDR_PAGE_SIZE);

        if (chunk > (len - off)) {
            chunk = len - off;
        }

        FLASH_ProgramPage(NULL, NULL, addr + off, chunk, (u8 *)&data[off]);
        CACHE_CleanAll(DCACHE);

        if (memcmp((void *)(addr + off), (void *)&data[off], chunk) != 0) {
            return -1;
        }

        off += chunk;
    }

    return 0;
}

static void res_hdr_stash_save(s8 type, u32 addr, const u8 *page, u16 len)
{
    s8 slot = res_hdr_slot_by_type(type);

    if ((slot < 0) || (len == 0) || (len > sizeof(res_hdr_stash[0].buf))) {
        return;
    }

    res_hdr_stash[slot].pending = 1;
    res_hdr_stash[slot].len = len;
    res_hdr_stash[slot].addr = addr;
    memcpy(res_hdr_stash[slot].buf, page, len);
}

static s8 res_hdr_prepare(u32 addr, u8 *data, u32 data_len)
{
    s8 type;
    u32 expect_magic;

    if (addr == RES_TEXT_BASE) {
        type = 3;
        expect_magic = TEXT_HDR_MAGIC;
    } else if (addr == RES_IMG_BASE) {
        type = 2;
        expect_magic = RES_MAGIC;
    } else {
        return 0;
    }

    if (*((u32 *)data) != expect_magic) {
        return -2;
    }

    if (data_len <= RES_HDR_PAGE_SIZE) {
        return -2;
    }

    /* 整页暂存，首 256B（含 magic）等整包完成后再写. */
    res_hdr_stash_save(type, addr, data, RES_HDR_PAGE_SIZE);

    if (res_type != type) {
        res_type = type;
        disp_dbg_code(res_type);
    }

    return 1;
}

static s8 res_hdr_write_body(u32 addr, u8 *data, u32 data_len)
{
    /* 先擦掉含 magic 的首扇区，首页保持 0xFF，只写 256 之后的数据. */
    FLASH_EraseSector(addr);
    CACHE_CleanAll(DCACHE);

    if (*((volatile u32 *)addr) != 0xFFFFFFFF) {
        return -1;
    }

    if (res_flash_program(addr + RES_HDR_PAGE_SIZE,
                          &data[RES_HDR_PAGE_SIZE],
                          data_len - RES_HDR_PAGE_SIZE) != 0) {
        return -1;
    }

    return 0;
}

static s8 res_hdr_commit_all(void)
{
    const u8 order[2] = { RES_HDR_SLOT_TXT, RES_HDR_SLOT_IMG };
    u8 i, slot;

    for (i = 0; i < 2; i++) {
        slot = order[i];
        if (!res_hdr_stash[slot].pending) {
            continue;
        }

        if (res_flash_program(res_hdr_stash[slot].addr,
                              res_hdr_stash[slot].buf,
                              res_hdr_stash[slot].len) != 0) {
            return -13;
        }

        res_hdr_stash[slot].pending = 0;
    }

    return 0;
}

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
    int8_t ret8;

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

    case 5:
        /**
         * 写入到外部 flash
         * @@[2] + LEN[2] + 0x0304[2] + TYPE[1] + SN[2] + DATA[N] + CRC[2] + \r\n[2]
         * TYPE: 文件类型
         *       1: MCU
         *       2: 无 LOGO 其他 FLAH 文件
         *       3: LOGO / LOGO + 其他 FLAH 文件
         * SN: 包号
         * DATA: 固件数据
         * 
         * 响应:
         * $$[2] + LEN[2] + 0x4304[2] + STAT[1] + SN[2] + CRC[2] + \r\n[2]
         * STAT: 状态
         *       0: 成功
         *       其他: 失败
         * SN: 包号
         */
        ret8 = 0;

        if (pack_num_cur > (pack_num_last + 1)) //包不连续
        {
            ret8 = -1;
            disp_dbg_code(ret8);
            goto ACK_LOOP_FLASH;
        }

        /* 重发包必须在 AES-GCM 之前丢弃，否则会把校验流算乱，最后一包报 E11. */
        if (pack_num_cur <= pack_num_last)
        {
            step = 0;
            goto ACK_LOOP_FLASH;
        }

        if (pack_num_cur == 0) {
            u32 payload_len = get_effective_payload_len(file_size_flash, pack_num_cur, PACK_SIZE_FLASH, cnt_r - 13);
            if (file_size_flash <= AES_GCM_TAG_SIZE)
            {
                ret8 = -11;
                disp_dbg_code(ret8);
                goto ACK_LOOP_FLASH;
            }
            app_aes_gcm_start(file_size_flash - AES_GCM_TAG_SIZE);
            if (app_aes_gcm_decrypt_payload(&buf_r[9], payload_len, &payload_len) != 0)
            {
                ret8 = -11;
                disp_dbg_code(ret8);
                goto ACK_LOOP_FLASH;
            }
            if (payload_len == 0)
            {
                step = 10;
                pack_num_last = pack_num_cur;
                goto ACK_LOOP_FLASH;
            }
            pack_size = payload_len;

            /**
             * LOGO 头: b'LOGO' (0X4F474F4C)
             * TEXT 头: b'TEXT' (0x54584554)
             * IMG  头: b'JRES' (0x5345524A)
             */
            uint32_t magic = *((uint32_t*)&buf_r[9]);
            if (magic == 0X4F474F4C) {
                /* LOGO. */
                base_addr = RES_LOGO_BASE;
                res_type  = 1;
            } else if (magic == 0x5345524A) {
                /* IMAGE. */
                base_addr = RES_IMG_BASE;
                res_type  = 2;
            } else if (magic == 0x54584554) {
                /* TEXT. */
                base_addr = RES_TEXT_BASE;
                res_type  = 3;
            } else {
                /* 无效文件. */
                ret8 = -2;
                disp_dbg_code(-2);
                goto ACK_LOOP_FLASH;
            }

            if (ret8 == 0) {
                disp_dbg_code(res_type);
            }
        } else {
            u32 payload_len = get_effective_payload_len(file_size_flash, pack_num_cur, PACK_SIZE_FLASH, cnt_r - 13);
            if (app_aes_gcm_decrypt_payload(&buf_r[9], payload_len, &payload_len) != 0)
            {
                ret8 = -11;
                disp_dbg_code(ret8);
                goto ACK_LOOP_FLASH;
            }
            pack_size = payload_len;

            uint32_t magic = *((uint32_t*)&buf_r[9]);
            s8 _type = -1;
            if (magic == 0X4F474F4C) {
                /* LOGO. */
                _type  = 1;
            } else if (magic == 0x5345524A) {
                /* IMAGE. */
                _type  = 2;
            } else if (magic == 0x54584554) {
                /* TEXT. */
                _type  = 3;
            }

            if (_type != -1 && _type != res_type) {
                res_type = _type;
                disp_dbg_code(res_type);
            }
        }

        if ((upd_type == UPD_LOGO) ||
            (upd_type == UPD_LOGO_EXT_FLASH) || 
            (upd_type == UPD_EXT_FLASH))
        {
            addr = base_addr + flash_decrypted_size;
        }
        else
        {
            ret8 = -3;
            disp_dbg_code(ret8);
            goto ACK_LOOP_FLASH;
        }

        if (pack_size == 0)
        {
            step = 10;
            pack_num_last = pack_num_cur;
            goto ACK_LOOP_FLASH;
        }

        ret = res_hdr_prepare(addr, &buf_r[9], pack_size);
        if (ret < 0) {
            ret8 = (s8)ret;
            disp_dbg_code(ret8);
            goto ACK_LOOP_FLASH;
        }

        if (ret > 0) {
            if (res_hdr_write_body(addr, &buf_r[9], pack_size) != 0) {
                step = 0;
                ret8 = 1;
            } else {
                step = 10;
                flash_decrypted_size += pack_size;
                pack_num_last = pack_num_cur;
            }
            goto ACK_LOOP_FLASH;
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
                ret8 = 1;
            }
            else
            {
                step = 10;
                flash_decrypted_size += pack_size;
                pack_num_last = pack_num_cur;
            }
	    }
        else
        {
            step = 10;
            flash_decrypted_size += pack_size;
            pack_num_last = pack_num_cur;
        }
ACK_LOOP_FLASH:
        buf_t[0] = ret8;
        memcpy(&buf_t[1], (u8*)&pack_num_cur, 2);
        len = pack_buf_s(CMD_SEND_DATA, buf_t, 3);
        VCPSendBytes(buf_s, len);
        /* 失败/重发应答后必须回到接收态，否则会卡在 case 5 无法收下一包 */
        if (step == 5) {
            step = 0;
        }
        break;

    case 6: //
        /**
         * 写入到内部 ROM
         * @@[2] + LEN[2] + 0x0304[2] + TYPE[1] + SN[2] + DATA[N] + CRC[2] + \r\n[2]
         * TYPE: 文件类型
         *       1: MCU
         *       2: 无 LOGO 其他 FLAH 文件
         *       3: LOGO / LOGO + 其他 FLAH 文件
         * SN: 包号
         * DATA: 固件数据
         * 
         * 响应:
         * $$[2] + LEN[2] + 0x4304[2] + STAT[1] + SN[2] + CRC[2] + \r\n[2]
         * STAT: 状态
         *       0: 成功
         *       其他: 失败
         * SN: 包号
         */
        ret8 = 0;

        if (pack_num_cur > (pack_num_last + 1))
        {
            ret8 = -4;
            disp_dbg_code(ret8);
            goto ACK_LOOP_APP;
        }

        if (pack_num_cur <= pack_num_last)
        {
            step = 0;
            goto ACK_LOOP_APP;
        }

        if (pack_num_cur == 0)
        {
            u32 payload_len = get_effective_payload_len(file_size_app, pack_num_cur, PACK_SIZE_MCU, cnt_r - 13);
            if (file_size_app <= AES_GCM_TAG_SIZE)
            {
                ret8 = -11;
                disp_dbg_code(ret8);
                goto ACK_LOOP_APP;
            }
            app_aes_gcm_start(file_size_app - AES_GCM_TAG_SIZE);
            if (app_aes_gcm_decrypt_payload(&buf_r[9], payload_len, &payload_len) != 0)
            {
                ret8 = -11;
                disp_dbg_code(ret8);
                goto ACK_LOOP_APP;
            }
            if (payload_len == 0 || payload_len > PACK_SIZE_MCU)
            {
                ret8 = -11;
                disp_dbg_code(ret8);
                goto ACK_LOOP_APP;
            }

            pack_size = payload_len;

            /* 包头信息. */
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

                /* @@[2] + LEN[2] + 0xF003[2] + TYPE[1] + FILE_SIZE[4] + CRC[2] + \r\n[2] */
                uart_send(buf_s, len);
                step = 101;
                ticks_to = ticks_sys;
                break;
            }
            else
            {
                step = 10;
                pack_cnt_at425 = 0;
            }
        }

        if (pack_cnt_at425 && (pack_num_cur <= pack_cnt_at425))
        {
            /* 转换 ID 后转发给 AT32. */
            step = 101;
            len = cnt_r - 10; /* @@ + 2len + 2cmd + 2crc + \r\n */
            if (len == 0 || len > (pack_size + 3)) printf("[UPDATE] [ERROR] Invalid data len (%d)\n", len);

            if (aes_gcm_enabled)
            {
                u32 decrypt_len = get_effective_payload_len(file_size_app, pack_num_cur, PACK_SIZE_MCU, len - 3);
                if (app_aes_gcm_decrypt_payload(&buf_r[9], decrypt_len, &decrypt_len) != 0)
                {
                    ret8 = -11;
                    disp_dbg_code(ret8);
                    goto ACK_LOOP_APP;
                }

                if (decrypt_len == 0)
                {
                    step = 10;
                    pack_num_last = pack_num_cur;
                    goto ACK_LOOP_APP;
                }

                len = decrypt_len + 3;
            }

            len = pack_buf_at425(AT_CMD_SEND_DATA, (u8 *)&buf_r[6], len);
            uart_send(buf_s, len);
        }
        else
        {
            if (pack_cnt_at425)
                addr = ADDR_APP + app_decrypted_size;
            else
                addr = ADDR_APP + app_decrypted_size;

            if (aes_gcm_enabled)
            {
                u32 decrypt_len = get_effective_payload_len(file_size_app, pack_num_cur, PACK_SIZE_MCU, cnt_r - 13);
                if (app_aes_gcm_decrypt_payload(&buf_r[9], decrypt_len, &decrypt_len) != 0)
                {
                    ret8 = -11;
                    disp_dbg_code(ret8);
                    goto ACK_LOOP_APP;
                }
                if (decrypt_len == 0)
                {
                    step = 10;
                    pack_num_last = pack_num_cur;
                    goto ACK_LOOP_APP;
                }
                pack_size = decrypt_len;
            }
            
            if ((pack_num_cur + 1) == total_packs) {
                printf("hello");
            }

            if (!aes_gcm_enabled && ((pack_num_cur + 1) == total_packs)) //最后一包
            {
                if (file_size_app % pack_size)
                {
                    pack_size = file_size_app % pack_size;
                }
            }

            if (pack_size == 0)
            {
                step = 10;
                pack_num_last = pack_num_cur;
                goto ACK_LOOP_APP;
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
                    ret8 = 1;
                }
                else
                {
                    step = 10;
                    app_decrypted_size += pack_size;
                    pack_num_last = pack_num_cur;
                }
            }
            else
            {
                step = 10;
                app_decrypted_size += pack_size;
                pack_num_last = pack_num_cur;
            }
        }
ACK_LOOP_APP:
        buf_t[0] = ret8;
        memcpy(&buf_t[1], (u8*)&pack_num_cur, 2);
        len = pack_buf_s(CMD_SEND_DATA, buf_t, 3);
        VCPSendBytes(buf_s, len);
        /* 失败/重发应答后回到接收态，避免卡在 case 6 */
        if (step == 6) {
            step = 0;
        }
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

    case 9:
        /**
         * 下发文件大小.
         * @@[2] + LEN[2] + 0x4303[2] + TYPE[1] + SIZE[4] + CRC[2] + \r\n[2]
         * TYPE: 文件类型
         *       1: MCU
         *       2: 无 LOGO 其他 FLAH 文件
         *       3: LOGO / LOGO + 其他 FLAH 文件
         * SIZE: 文件大小
         * 
         * 响应:
         * $$[2] + LEN[2] + 0x0303[2] + STAT[1] + CRC[2] + \r\n[2]
         * STAT: 状态
         *       0: 成功
         *       其他: 失败
         */
        ret8 = 0;
        step = 0;
        upd_type = UPD_NULL;
        pack_num_cur = 0;
        pack_num_last = -1;

        if (buf_r[6] == 1)
        {
            upd_type = UPD_APP;
            pack_size = PACK_SIZE_MCU;
            memcpy(&file_size_app, &buf_r[7], 4);
            total_packs = (file_size_app + pack_size - 1) / pack_size;
            if (file_size_app > SIZE_MCU)
            {
                ret8 = -6;
            }
        }
        else if (buf_r[6] == 2) //flash(logo not include)
        {
            upd_type = UPD_EXT_FLASH;
            pack_size = PACK_SIZE_FLASH;
            memcpy(&file_size_flash, &buf_r[7], 4);
            total_packs = (file_size_flash + pack_size - 1) / pack_size;

            if (file_size_flash > SIZE_FLASH)
            {
                ret8 = -7;
            }
        }
        else if (buf_r[6] == 3) //logo or logo+flash
        {
            memcpy(&file_size_flash, &buf_r[7], 4);

            if (file_size_flash > SIZE_LOGO)
            {
                upd_type = UPD_LOGO_EXT_FLASH;
                if (file_size_flash > (SIZE_FLASH + SIZE_LOGO))
                {
                    ret8 = -8;
                }
            }
            else
            {
                upd_type = UPD_LOGO;
            }

            pack_size = PACK_SIZE_FLASH;
            total_packs = (file_size_flash+pack_size-1) / pack_size;
        }
        else
        {
            /* 无效文件类型. */
            ret8 = -9;
        }

        if (ret8) {
            /* 显示错误信息. */
            disp_dbg_code(ret8);
        } else {
            /* 重置进度为 0. */
            update_process_bar(0);
            disp_dbg_code(0);
        }

        /* 响应. */
        buf_t[0] = ret8;
        // memcpy((void*)buf_t, (u8*)&pack_num_cur, 1);
        len = pack_buf_s(CMD_SEND_FILE_SIZE, buf_t, 1);
        VCPSendBytes(buf_s, len);
        break;

    case 10: //显示进度条
        step = 0;
        update_process_bar((pack_num_cur + 1) * 10000 / total_packs);
        if ((pack_num_cur + 1) == total_packs)
        {
            if (cnt_file)
            {
                cnt_file--;
            }
            if (!cnt_file)
            {
                upd_anyway = 0;
                if ((upd_type == UPD_APP) ||
                    (upd_type == UPD_EXT_FLASH) ||
                    (upd_type == UPD_LOGO) ||
                    (upd_type == UPD_LOGO_EXT_FLASH))
                {
                    if (app_aes_gcm_finish() != 0)
                    {
                        disp_dbg_code(-12);
                        break;
                    }
                }

                if (res_hdr_commit_all() != 0)
                {
                    disp_dbg_code(-13);
                    break;
                }

                if (upd_type == UPD_APP)
                {
                    if (app_crc_info_save(app_decrypted_size) != 0)
                    {
                        disp_dbg_code(-10);
                        break;
                    }
                }
                disp_success();
//                lv_refr_now(NULL);
                beep_on();
                delay_ms(200);
                beep_off();
                //delay_ms(3000);
                //to_app();
            }
            else if (res_hdr_commit_all() != 0)
            {
                disp_dbg_code(-13);
                break;
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
            ret8 = 101;
            disp_dbg_code2(ret8);
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
                ret8 = 102;
                disp_dbg_code2(ret8);
                goto ACK_LOOP_APP;
            }
        }
        if ((ticks_sys-ticks_to) > 10*1000)
        {
            step = 0;
            buf_t[0] = 1;
            ret8 = 103;
            disp_dbg_code2(ret8);
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
            ret8 = 104;
            disp_dbg_code2(ret8);
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
            ret8 = 105;
            disp_dbg_code2(ret8);
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
                    ret8 = 106;
                    disp_dbg_code2(ret8);
                    goto ACK_LOOP_APP;
                }
            }

            if (len >= 100)
            {
                step = 0;
                buf_t[0] = 1;
                ret8 = 107;
                disp_dbg_code2(ret8);
                goto ACK_LOOP_APP;
            }

            if (cnt_r >= 100)
            {
                step = 0;
                buf_t[0] = 1;
                ret8 = 108;
                disp_dbg_code2(ret8);
                goto ACK_LOOP_APP;
            }
        }
        break;

    case 106:
        step = 101;
        ret8 = 0;
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
                ret8 = -104;
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
