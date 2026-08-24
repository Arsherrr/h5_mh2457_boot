#include "mh2457.h"
#include "memory.h"
#include "update.h"
#include "res.h"
#include "sefltest.h"
#include <stdio.h>
#include <string.h>

/* 与 App / Python main.py 写入的元数据布局一致 */
typedef struct {
    uint32_t image_size;  /* 固件实际大小 (字节), 4 字节对齐 */
    uint32_t crc32;       /* 前256 + 后256 字节的标准 CRC32 */
    uint32_t ymd_info;    /* 编译日期 (如 20260729) */
    uint32_t hms_info;    /* 编译时间 (如 143015) */
} app_meta_t;

#define APP_START_ADDR    ( __APP_BASE )
#define META_ADDR         ( ADDR_APP_INFO )
#define FAST_CRC_BYTES    (256u)
#define FAST_CRC_WORDS    (FAST_CRC_BYTES / 4u)

#define RES_LOGO_REGION_SIZE  (RES_TEXT_BASE - RES_LOGO_BASE)
#define RES_TEXT_REGION_SIZE  (RES_IMG_BASE  - RES_TEXT_BASE)
#define RES_IMG_REGION_SIZE   (RES_FS_BASE   - RES_IMG_BASE)

/*
 * 匹配 Python binascii.crc32 (IEEE CRC-32), 按 32 位字加速:
 *   BYTE_NUM=11 (4 bytes), XOR_OUT, REV_OUT, REV_IN, TYPE=CRC32, BYTE_REV=0
 */
#define CRC32_CSR_WORD  (0xF6u)

static void crc32_hw_feed_words(const uint32_t *p, uint32_t words)
{
    while (words--) {
        CRC->DR = *p++;
    }
}

uint8_t check_app_fast_crc_hw(void)
{
    app_meta_t *meta = (app_meta_t *)META_ADDR;
    uint32_t total_bytes = meta->image_size;
    uint32_t expected_crc = meta->crc32;
    uint32_t calc_crc;
    const uint32_t *p_start;
    const uint32_t *p_end;

    if (total_bytes == 0xFFFFFFFF ||
        total_bytes < (FAST_CRC_BYTES * 2u) ||
        total_bytes > __ROM_SIZE ||
        (total_bytes & 3u) != 0u) {
        return 0;
    }

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
    CRC->CSR = CRC32_CSR_WORD;
    CRC->INI = 0xFFFFFFFF;
    CRC->XOR = 0xFFFFFFFF;
    CRC_ResetDR();

    p_start = (const uint32_t *)APP_START_ADDR;
    p_end   = (const uint32_t *)(APP_START_ADDR + total_bytes - FAST_CRC_BYTES);

    crc32_hw_feed_words(p_start, FAST_CRC_WORDS);
    crc32_hw_feed_words(p_end,   FAST_CRC_WORDS);

    calc_crc = CRC->DR;
    printf("[SELFTEST] APP CRC expect=0x%08X calc=0x%08X\r\n", expected_crc, calc_crc);

    return (calc_crc == expected_crc) ? 1 : 0;
}

uint8_t check_app_integrity(void)
{
    volatile uint32_t *ptr = (volatile uint32_t *)ADDR_APP;

    if (*ptr == 0xFFFFFFFF) {
        printf("[SELFTEST] APP empty\r\n");
        return 0;
    }

    if (strncmp(name, (char *)ADDR_APP_NAME, LEN_IAP_NAME) != 0) {
        printf("[SELFTEST] APP name mismatch\r\n");
        return 0;
    }

    if (check_app_fast_crc_hw() == 0) {
        printf("[SELFTEST] APP CRC fail\r\n");
        return 0;
    }

    return 1;
}

static uint8_t check_logo_header(void)
{
    const logo_header_t *hdr = (const logo_header_t *)RES_LOGO_BASE;

    if (hdr->magic != LOGO_HDR_MAGIC) {
        printf("[SELFTEST] LOGO magic=0x%08X\r\n", (unsigned)hdr->magic);
        return 0;
    }
    if (hdr->width == 0 || hdr->height == 0) {
        return 0;
    }
    if (hdr->size == 0 || hdr->size == 0xFFFFFFFF ||
        hdr->size > (RES_LOGO_REGION_SIZE - sizeof(logo_header_t))) {
        printf("[SELFTEST] LOGO size invalid %u\r\n", (unsigned)hdr->size);
        return 0;
    }
    return 1;
}

static uint8_t check_text_header(void)
{
    const text_header_t *hdr = (const text_header_t *)RES_TEXT_BASE;

    if (hdr->magic != TEXT_HDR_MAGIC) {
        printf("[SELFTEST] TEXT magic=0x%08X\r\n", (unsigned)hdr->magic);
        return 0;
    }
    if (hdr->size == 0 || hdr->size == 0xFFFFFFFF ||
        hdr->size > (RES_TEXT_REGION_SIZE - sizeof(text_header_t))) {
        printf("[SELFTEST] TEXT size invalid %u\r\n", (unsigned)hdr->size);
        return 0;
    }
    return 1;
}

static uint8_t check_img_header(void)
{
    const jl_res_bin_header_t *hdr = (const jl_res_bin_header_t *)RES_IMG_BASE;

    if (hdr->magic != RES_MAGIC) {
        printf("[SELFTEST] IMG magic=0x%08X\r\n", (unsigned)hdr->magic);
        return 0;
    }
    if (hdr->count == 0 || hdr->count == 0xFFFF) {
        return 0;
    }
    if (hdr->header_size < sizeof(jl_res_bin_header_t) ||
        hdr->header_size > RES_IMG_REGION_SIZE) {
        return 0;
    }
    if (hdr->table_offset >= RES_IMG_REGION_SIZE ||
        hdr->hash_offset  >= RES_IMG_REGION_SIZE ||
        hdr->data_offset  >= RES_IMG_REGION_SIZE) {
        printf("[SELFTEST] IMG offsets out of range\r\n");
        return 0;
    }
    return 1;
}

uint8_t check_res_integrity(void)
{
    uint8_t logo_ok = 1; // check_logo_header();
    uint8_t text_ok = check_text_header();
    uint8_t img_ok  = check_img_header();

    printf("[SELFTEST] RES logo=%u text=%u img=%u\r\n", logo_ok, text_ok, img_ok);
    return (logo_ok && text_ok && img_ok) ? 1 : 0;
}
