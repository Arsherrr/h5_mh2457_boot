#include "jpeg_hw.h"
#include "lvgl.h"
#include "mh2457.h"
#include "log.h"
#include "tmr.h"
#include <stdio.h>
#include <string.h>

/* [ 配置 ] 是否开启 LOG 日志. */
#define USE_JPEG_HW_LOG 1

/* [ 配置 ] LOG 等级. 
 * LOG_LEVEL_TRACE
 * LOG_LEVEL_INFO
 * LOG_LEVEL_WARN
 * LOG_LEVEL_ERROR
 * LOG_LEVEL_NONE
*/
#define JPEG_HW_LOG_LEVEL ( LOG_LEVEL_WARN )

#if USE_JPEG_HW_LOG
#define _JPEG_HW_LOG_LEVEL JPEG_HW_LOG_LEVEL
#else
#define _JPEG_HW_LOG_LEVEL LOG_LEVEL_NONE
#endif

#if _JPEG_HW_LOG_LEVEL <= LOG_LEVEL_TRACE
#define JPEG_HW_LOG_TRACE(...) printf("[JPEG HW] " __VA_ARGS__)
#else
#define JPEG_HW_LOG_TRACE(...)
#endif
#if _JPEG_HW_LOG_LEVEL <= LOG_LEVEL_INFO
#define JPEG_HW_LOG_INFO(...) printf("[JPEG HW] " __VA_ARGS__)
#else
#define JPEG_HW_LOG_INFO(...)
#endif
#if _JPEG_HW_LOG_LEVEL <= LOG_LEVEL_WARN
#define JPEG_HW_LOG_WARN(...) printf("[JPEG HW] [WARN] " __VA_ARGS__)
#else
#define JPEG_HW_LOG_WARN(...)
#endif
#if _JPEG_HW_LOG_LEVEL <= LOG_LEVEL_ERROR
#define JPEG_HW_LOG_ERROR(...) printf("[JPEG HW] [ERROR] " __VA_ARGS__)
#else
#define JPEG_HW_LOG_ERROR(...)
#endif

uint8_t *yuv_buf;
static bool s_inited = false;

static uint32_t align_up_u32(uint32_t value, uint32_t align);

static bool wait_dma_tc(DMA_Stream_TypeDef *stream, uint32_t timeout_ms)
{
    while(timeout_ms--) {
        if (DMA_GetFlagStatus(stream, DMA_FLAG_TCIF) != RESET) return true;
        delay_ms(1);
    }
    return false;
}

static void jpeg_dma_stop(void)
{
    DMA_Cmd(DMA2_Stream5, DISABLE);
    DMA_Cmd(DMA2_Stream0, DISABLE);
    JPEG_Cmd(DISABLE);
}

typedef enum {
    DMA_ALIGNED_BYTE,
    DMA_ALIGNED_HALFWORD,
    DMA_ALIGNED_WORD,
    DMA_ALIGNED_MAX = 0xFF,
} dma_aligned_e;

static const uint32_t mem_burst[]     = {DMA_MemoryBurst_INC16, DMA_MemoryBurst_INC8, DMA_MemoryBurst_INC4};
static const uint32_t mem_data_size[] = {DMA_MemoryDataSize_Byte, DMA_MemoryDataSize_HalfWord, DMA_MemoryDataSize_Word};

static void jpeg_dma_config(dma_aligned_e aligned)
{
    DMA_InitTypeDef dma_cfg;

    DMA_DeInit(DMA2_Stream5);
    memset(&dma_cfg, 0, sizeof(dma_cfg));
    dma_cfg.DMA_Channel             = DMA_Channel_0;
    dma_cfg.DMA_BufferSize          = 0;
    dma_cfg.DMA_DIR                 = DMA_DIR_MemoryToPeripheral;
    dma_cfg.DMA_FIFOMode            = DMA_FIFOMode_Enable;
    dma_cfg.DMA_FIFOThreshold       = DMA_FIFOThreshold_Full;
    dma_cfg.DMA_Memory0BaseAddr     = 0;
    dma_cfg.DMA_MemoryBurst         = mem_burst[(uint8_t)aligned];
    dma_cfg.DMA_MemoryDataSize      = mem_data_size[(uint8_t)aligned];
    dma_cfg.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    dma_cfg.DMA_Mode                = DMA_Mode_Normal;
    dma_cfg.DMA_PeripheralBaseAddr  = (uint32_t)&JPEGD->DIR;
    dma_cfg.DMA_PeripheralBurst     = DMA_PeripheralBurst_INC16;
    dma_cfg.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    dma_cfg.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    dma_cfg.DMA_Priority            = DMA_Priority_Medium;
    DMA_Init(DMA2_Stream5, &dma_cfg);

    DMA_DeInit(DMA2_Stream0);
    memset(&dma_cfg, 0, sizeof(dma_cfg));
    dma_cfg.DMA_Channel             = DMA_Channel_7;
    dma_cfg.DMA_BufferSize          = 0;
    dma_cfg.DMA_DIR                 = DMA_DIR_PeripheralToMemory;
    dma_cfg.DMA_FIFOMode            = DMA_FIFOMode_Enable;
    dma_cfg.DMA_FIFOThreshold       = DMA_FIFOThreshold_Full;
    dma_cfg.DMA_Memory0BaseAddr     = 0;
    dma_cfg.DMA_MemoryBurst         = DMA_MemoryBurst_INC4;
    dma_cfg.DMA_MemoryDataSize      = DMA_MemoryDataSize_Word;
    dma_cfg.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    dma_cfg.DMA_Mode                = DMA_Mode_Normal;
    dma_cfg.DMA_PeripheralBaseAddr  = (uint32_t)&JPEGD->DOR;
    dma_cfg.DMA_PeripheralBurst     = DMA_PeripheralBurst_INC16;
    dma_cfg.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    dma_cfg.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    dma_cfg.DMA_Priority            = DMA_Priority_Medium;
    DMA_Init(DMA2_Stream0, &dma_cfg);
    DMA_FlowControllerConfig(DMA2_Stream0, DMA_FlowCtrl_Peripheral);
}

void DMA2D_SoftReset(void)
{
    /* 使能 DMA2D 复位. */
    RCC->AHB4RSTR |= (1U << 2);

    for (volatile int i = 0; i < 10; i++) __NOP();

    /* 释放复位. */
    RCC->AHB4RSTR &= ~(1U << 2);

    /* 等待 DMA2D 完全退出复位状态. */
    for (volatile int i = 0; i < 10; i++) __NOP();
}

static void dma2d_yuv_to_rgb_init(const JPEG_InfoTypeDef *jpeg_info, uint32_t fmt, uint32_t rot)
{
    uint32_t align_base;
    uint32_t pl;
    uint32_t lo;
    uint32_t pl_lo;
    uint32_t aligned_pl;
    
    DMA2D_SoftReset();

    DMA2D_SetMode(DMA2D, DMA2D_MODE_M2M_PFC);
    DMA2D_FGND_SetColorMode(DMA2D, DMA2D_INPUT_MODE_YCBCR);

    if (jpeg_info->ChromaSubsampling == JPEG_420_SUBSAMPLING) {
        DMA2D_FGND_SetChrSubSampling(DMA2D, DMA2D_CSS_420);
        align_base = 16u;
    }
    else if (jpeg_info->ChromaSubsampling == JPEG_422_SUBSAMPLING) {
        DMA2D_FGND_SetChrSubSampling(DMA2D, DMA2D_CSS_422);
        align_base = 16u;
    }
    else if (jpeg_info->ChromaSubsampling == JPEG_444_SUBSAMPLING) {
        DMA2D_FGND_SetChrSubSampling(DMA2D, DMA2D_CSS_444);
        align_base = 8u;
    }
    else {
        align_base = 8u;
    }

    pl = jpeg_info->ImageWidth;
    aligned_pl = align_up_u32(pl, align_base);
    lo = aligned_pl - pl;
    pl_lo = pl + lo;

    DMA2D_FGND_SetLineOffset(DMA2D, lo);
    DMA2D_SetNbrOfPixelsPerLines(DMA2D, pl);
    DMA2D_SetNbrOfLines(DMA2D, jpeg_info->ImageHeight);
    DMA2D_SetLineOffset(DMA2D, 0);
    DMA2D_SetOutputColorMode(DMA2D, fmt);
    DMA2D_SetLineOffsetMode(DMA2D, DMA2D_LINE_OFFSET_PIXELS);
    DMA2D_SetOutputRotationMode(DMA2D, rot);
}

static bool dma2d_yuv_to_rgb(const uint8_t *yuv, uint8_t *rgb)
{
    DMA2D_ClearFlag_TC(DMA2D);
    DMA2D_ClearFlag_TE(DMA2D);
    DMA2D_ClearFlag_CE(DMA2D);
    DMA2D_ClearFlag_CAE(DMA2D);

    DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)yuv);
    DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)rgb);

    DMA2D_Start(DMA2D);
    while(DMA2D_IsActiveFlag_TC(DMA2D) == 0) {
        if (DMA2D_IsActiveFlag_TE(DMA2D) != 0 || DMA2D_IsActiveFlag_CE(DMA2D) != 0) {
            JPEG_HW_LOG_ERROR("dma2d error, ISR=%08X\n", DMA2D->ISR);
            break;
        }
        delay_ms(1);
    }
    DMA2D_ClearFlag_TC(DMA2D);
    DMA2D_ClearFlag_TE(DMA2D);
    DMA2D_ClearFlag_CE(DMA2D);
    DMA2D_ClearFlag_CAE(DMA2D);
    return true;
}

static uint32_t align_up_u32(uint32_t value, uint32_t align)
{
    return (value + align - 1u) & ~(align - 1u);
}

static uint32_t jpeg_max_yuv444_buffer_size(uint32_t width, uint32_t height)
{
    uint32_t aligned_w = align_up_u32(width, 8u);
    uint32_t aligned_h = align_up_u32(height, 8u);
    return aligned_w * aligned_h * 4u;
}

int jpeg_hw_init(void)
{
    if (s_inited) return -1;
    
    jpeg_hw_mem_init();

    PeripheralEnable(PeripheralJPEGD, 1);
    PeripheralEnable(PeripheralDMA2D, 1);
    PeripheralEnable(PeripheralDMA2, 1);

    JPEG_SetInFifoThreshold(16);
    JPEG_SetOutFifoThreshold(16);
    s_inited = true;

    return 0;
}

void jpeg_hw_get_info(JPEG_InfoTypeDef *info)
{
    if (info == NULL) return;
    JPEG_GetInfo(info);
}

bool jpeg_hw_decode_block(const uint8_t *src, uint32_t size, uint32_t width, uint32_t height, uint8_t *dst, uint32_t dst_size)
{
    return jpeg_hw_dec_handler(src, size, width, height, dst, dst_size, DMA2D_OUTPUT_MODE_RGB565, DMA2D_ROTATION_0);
}

/**
 * @brief  JPEG 硬件解码:
 * @param  src: jpeg 图片地址.
 * @param  size: jpeg 图片大小.
 * @param  width: 图片宽度.
 * @param  height: 图片高度.
 * @param  dst: 转换后保存的地址.
 * @param  dst_size: dst 指向的空间大小, 用于判断是否可以装下转换后的数据.
 * @param  fmt: 转换格式
 *         DMA2D_OUTPUT_MODE_ARGB8888
 *         DMA2D_OUTPUT_MODE_RGB888
 *         DMA2D_OUTPUT_MODE_RGB565
 *         DMA2D_OUTPUT_MODE_ARGB1555
 *         DMA2D_OUTPUT_MODE_ARGB4444
 * @param  rot: 旋转角度
 *         DMA2D_ROTATION_0  
 *         DMA2D_ROTATION_90 
 *         DMA2D_ROTATION_180
 *         DMA2D_ROTATION_270
 */
bool jpeg_hw_dec_handler(const uint8_t *src, uint32_t size, uint32_t width, uint32_t height, 
                         uint8_t *dst, uint32_t dst_size, uint32_t fmt, uint32_t rot)
{
    if (src == NULL || dst == NULL || size == 0 || width == 0 || height == 0 || dst_size == 0) return false;

    /* fmt assert... */

    const uint32_t rgb_size = width * height * 3u; /* RGB888. */
    if (rgb_size > dst_size) {
        JPEG_HW_LOG_WARN("rgb buffer too small, need=%u dst=%u\n", rgb_size, dst_size);
        return false;
    }

    dma_aligned_e dma_aligned = DMA_ALIGNED_BYTE;

    if ((size & 0x03) == 0) {
        /* 4 字节对齐. */
        dma_aligned = DMA_ALIGNED_WORD;
        JPEG_HW_LOG_INFO("JPEG word aligned.\n");
    } else if ((size & 0x01) == 0) {
        /* 2 字节对齐. */
        dma_aligned = DMA_ALIGNED_HALFWORD;
        JPEG_HW_LOG_INFO("JPEG half word aligned.\n");
    } else {
        JPEG_HW_LOG_INFO("JPEG byte aligned.\n");
    }

    /* JPG 不超过 dst 大小(width * height * 3). */
    uint8_t *jpeg_ram = (uint8_t *)dst;

    /* Flash -> SDRAM. */
    memcpy(jpeg_ram, src, size);
    CACHE_CleanAll(DCACHE);

    if (yuv_buf == NULL) {
        JPEG_HW_LOG_ERROR("yuv buffer is NULL\n");
        return false;
    }

    JPEG_InfoTypeDef jpeg_info;
    memset(&jpeg_info, 0, sizeof(jpeg_info));

    JPEG_HW_LOG_INFO("decode+yuv2rgb start, in=%u, yuv=%u, rgb=%u, img=%ux%u\n",
               size, yuv_size, dst_size, width, height);

    JPEG_Reset();
    jpeg_dma_stop();
    jpeg_dma_config(dma_aligned);

    DMA2_Stream5->M0AR = (uint32_t)jpeg_ram;
    DMA2_Stream5->NDTR = size;
    DMA2_Stream0->M0AR = (uint32_t)yuv_buf;
    DMA2_Stream0->NDTR = 0;

    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF  | 
                                DMA_FLAG_FEIF  | 
                                DMA_FLAG_TEIF  | 
                                DMA_FLAG_HTIF  | 
                                DMA_FLAG_DMEIF | 
                                DMA_FLAG_LTCIF);

    DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF  | 
                                DMA_FLAG_FEIF  | 
                                DMA_FLAG_TEIF  | 
                                DMA_FLAG_HTIF  | 
                                DMA_FLAG_DMEIF | 
                                DMA_FLAG_LTCIF);

    JPEG_Cmd(ENABLE);
    DMA_Cmd(DMA2_Stream0, ENABLE);
    DMA_Cmd(DMA2_Stream5, ENABLE);

    if (wait_dma_tc(DMA2_Stream5, 200) == false) {
        JPEG_HW_LOG_ERROR("input dma timeout\n");
        jpeg_dma_stop();
        return false;
    }
    
    if (!wait_dma_tc(DMA2_Stream0, 200)) {
        JPEG_HW_LOG_ERROR("output dma timeout\n");
        jpeg_dma_stop();
        return false;
    }

    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF);
    DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF);

    JPEG_GetInfo(&jpeg_info);
    JPEG_Cmd(DISABLE);

    if (jpeg_info.ImageWidth == 0 || jpeg_info.ImageHeight == 0) {
        JPEG_HW_LOG_ERROR("invalid jpeg info, w=%u h=%u css=%u\n",
                   jpeg_info.ImageWidth, jpeg_info.ImageHeight, jpeg_info.ChromaSubsampling);
        return false;
    }

    if (((uint32_t)jpeg_info.ImageWidth * (uint32_t)jpeg_info.ImageHeight * 2u) > dst_size) {
        JPEG_HW_LOG_WARN("rgb buffer too small, need=%u dst=%u\n",
                   (uint32_t)jpeg_info.ImageWidth * (uint32_t)jpeg_info.ImageHeight * 2u,
                   dst_size);
        return false;
    }

    dma2d_yuv_to_rgb_init(&jpeg_info, fmt, rot);
    if (!dma2d_yuv_to_rgb(yuv_buf, dst)) {
        JPEG_HW_LOG_ERROR("dma2d convert failed\n");
        return false;
    }

    JPEG_HW_LOG_INFO("decode+yuv2rgb done, w=%u h=%u css=%u\n",
               jpeg_info.ImageWidth, jpeg_info.ImageHeight, jpeg_info.ChromaSubsampling);
    return true;
}

int jpeg_hw_mem_init(void)
{
    const uint32_t yuv_size = jpeg_max_yuv444_buffer_size(854, 480);
    yuv_buf = (uint8_t *)lv_malloc(yuv_size);

    if (yuv_buf == NULL) {
        JPEG_HW_LOG_ERROR("yuv buffer oom, size=%u\n", yuv_size);
        return -1;
    }
    
    return 0;
}
