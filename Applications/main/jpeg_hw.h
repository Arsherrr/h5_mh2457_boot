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

void jpeg_hw_init(void);
void jpeg_hw_get_info(JPEG_InfoTypeDef *info);
bool jpeg_hw_decode_block(const uint8_t *src, uint32_t size, uint32_t width, uint32_t height, uint8_t *dst, uint32_t dst_size);

#ifdef __cplusplus
}
#endif

#endif
