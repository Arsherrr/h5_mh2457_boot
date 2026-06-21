#include "jpeg_hw.h"
#include "mh2457.h"
#include "tmr.h"
#include "lvgl.h"
#include <string.h>

/* 打开/关闭 JPEG_HW 调试日志 */
// #define JPEG_HW_DEBUG

static bool s_inited = false;

static uint32_t align_up_u32(uint32_t value, uint32_t align);

static bool wait_dma_tc(DMA_Stream_TypeDef *stream, uint32_t timeout_ms)
{
    while(timeout_ms--) {
        if (DMA_GetFlagStatus(stream, DMA_FLAG_TCIF) != RESET) return true;
        delay_us(100);
    }
    return false;
}

static void jpeg_dma_stop(void)
{
    DMA_Cmd(DMA2_Stream5, DISABLE);
    DMA_Cmd(DMA2_Stream0, DISABLE);
    JPEG_Cmd(DISABLE);
}

static void jpeg_dma_config(void)
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
    dma_cfg.DMA_MemoryBurst         = DMA_MemoryBurst_INC4;
    dma_cfg.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;
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

static void dma2d_yuv_to_rgb565_init(const JPEG_InfoTypeDef *jpeg_info)
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
    DMA2D_SetOutputColorMode(DMA2D, DMA2D_OUTPUT_MODE_RGB565);
    DMA2D_SetLineOffsetMode(DMA2D, DMA2D_LINE_OFFSET_PIXELS);
    DMA2D_SetOutputRotationMode(DMA2D, DMA2D_ROTATION_0);
}

static bool dma2d_yuv_to_rgb565(const uint8_t *yuv, uint8_t *rgb565)
{
    DMA2D_ClearFlag_TC(DMA2D);
    DMA2D_ClearFlag_TE(DMA2D);
    DMA2D_ClearFlag_CE(DMA2D);
    DMA2D_ClearFlag_CAE(DMA2D);

    DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)yuv);
    DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)rgb565);

    DMA2D_Start(DMA2D);
    while(DMA2D_IsActiveFlag_TC(DMA2D) == 0) {
        if (DMA2D_IsActiveFlag_TE(DMA2D) != 0 || DMA2D_IsActiveFlag_CE(DMA2D) != 0) {
#ifdef JPEG_HW_DEBUG
            printf("[JPEG_HW] dma2d error, ISR=%08X\n", DMA2D->ISR);
#endif
            break;
        }
        delay_us(100);
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
    return aligned_w * aligned_h * 3u;
}

void jpeg_hw_init(void)
{
    if (s_inited) return;

    PeripheralEnable(PeripheralJPEGD, 1);
    PeripheralEnable(PeripheralDMA2D, 1);
    PeripheralEnable(PeripheralDMA2, 1);

    jpeg_dma_config();
    JPEG_SetInFifoThreshold(16);
    JPEG_SetOutFifoThreshold(16);
    s_inited = true;
}

void jpeg_hw_get_info(JPEG_InfoTypeDef *info)
{
    if (info == NULL) return;
    JPEG_GetInfo(info);
}

bool jpeg_hw_decode_block(const uint8_t *src, uint32_t size, uint32_t width, uint32_t height, uint8_t *dst, uint32_t dst_size)
{
    if (src == NULL || dst == NULL || size == 0 || width == 0 || height == 0 || dst_size == 0) return false;

    const uint32_t rgb_size = width * height * 2u;
    if (rgb_size > dst_size) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] rgb buffer too small, need=%u dst=%u\n", rgb_size, dst_size);
#endif
        return false;
    }

    /* 这里待优化. */
    uint8_t *jpeg_ram = (uint8_t *)lv_malloc(size);
    if (jpeg_ram == NULL) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] jpeg input buffer oom, size=%u\n", size);
#endif
        return false;
    }
    memcpy(jpeg_ram, src, size);

    const uint32_t yuv_size = jpeg_max_yuv444_buffer_size(width, height);
    uint8_t *yuv_buf = (uint8_t *)lv_malloc(yuv_size);
    if (yuv_buf == NULL) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] yuv buffer oom, size=%u\n", yuv_size);
#endif
        lv_free(jpeg_ram);
        return false;
    }

    JPEG_InfoTypeDef jpeg_info;
    memset(&jpeg_info, 0, sizeof(jpeg_info));

#ifdef JPEG_HW_DEBUG
    printf("[JPEG_HW] decode+yuv2rgb start, in=%u, yuv=%u, rgb=%u, img=%ux%u\n",
               size, yuv_size, dst_size, width, height);
#endif

    JPEG_Reset();
    jpeg_dma_stop();
    jpeg_dma_config();

    DMA2_Stream5->M0AR = (uint32_t)jpeg_ram;
    DMA2_Stream5->NDTR = size;
    DMA2_Stream0->M0AR = (uint32_t)yuv_buf;
    DMA2_Stream0->NDTR = 0;

    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF);
    DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF);

    JPEG_Cmd(ENABLE);
    DMA_Cmd(DMA2_Stream0, ENABLE);
    DMA_Cmd(DMA2_Stream5, ENABLE);

    if (!wait_dma_tc(DMA2_Stream5, 5000)) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] input dma timeout\n");
#endif
        jpeg_dma_stop();
        lv_free(yuv_buf);
        lv_free(jpeg_ram);
        return false;
    }
    
    if (!wait_dma_tc(DMA2_Stream0, 5000)) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] output dma timeout\n");
#endif
        jpeg_dma_stop();
        lv_free(yuv_buf);
        lv_free(jpeg_ram);
        return false;
    }

    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF);
    DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF);

    JPEG_GetInfo(&jpeg_info);
    JPEG_Cmd(DISABLE);

    if (jpeg_info.ImageWidth == 0 || jpeg_info.ImageHeight == 0) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] invalid jpeg info, w=%u h=%u css=%u\n",
                   jpeg_info.ImageWidth, jpeg_info.ImageHeight, jpeg_info.ChromaSubsampling);
#endif
        lv_free(yuv_buf);
        lv_free(jpeg_ram);
        return false;
    }

    if (((uint32_t)jpeg_info.ImageWidth * (uint32_t)jpeg_info.ImageHeight * 2u) > dst_size) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] rgb buffer too small, need=%u dst=%u\n",
                   (uint32_t)jpeg_info.ImageWidth * (uint32_t)jpeg_info.ImageHeight * 2u,
                   dst_size);
#endif
        lv_free(yuv_buf);
        lv_free(jpeg_ram);
        return false;
    }

    /* 现在走 420 流程, 导致画质丢失, 后面优化. */
    dma2d_yuv_to_rgb565_init(&jpeg_info);
    if (!dma2d_yuv_to_rgb565(yuv_buf, dst)) {
#ifdef JPEG_HW_DEBUG
        printf("[JPEG_HW] dma2d convert failed\n");
#endif
        lv_free(yuv_buf);
        lv_free(jpeg_ram);
        return false;
    }

    lv_free(yuv_buf);
    lv_free(jpeg_ram);
#ifdef JPEG_HW_DEBUG
    printf("[JPEG_HW] decode+yuv2rgb done, w=%u h=%u css=%u\n",
               jpeg_info.ImageWidth, jpeg_info.ImageHeight, jpeg_info.ChromaSubsampling);
#endif
    return true;
}
