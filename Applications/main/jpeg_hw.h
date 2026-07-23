/**
 * ******************************************************************************
 * @file    jpeg_hw.h
 * @brief   MH2457 JPEGD/DMA2D hardware wrapper for block-style decode
 * ******************************************************************************
 */

#ifndef __JPEG_HW_H
#define __JPEG_HW_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mh245x_jpeg.h"
#include <stdbool.h>
#include <stdint.h>

#define JPEG_HW_BLOCK_HEIGHT 16

int jpeg_hw_mem_init(void);
int jpeg_hw_init(void);
void jpeg_hw_get_info(JPEG_InfoTypeDef *info);
   
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
                         uint8_t *dst, uint32_t dst_size, uint32_t fmt, uint32_t rot);

#ifdef __cplusplus
}
#endif

#endif
