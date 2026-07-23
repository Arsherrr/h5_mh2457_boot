#ifndef __FLASH_H
#define __FLASH_H
#include "mh2457.h"
#include "memory.h"

#define FLASH_SIZE                  (1020*1024UL) /* 4K 对齐. */
#define FLASH_BASE_ADDR             (RES_FS_BASE)
#define FLASH_SECTOR_SIZE           (4096)


/*
*********************************************************************************************************
*   函 数 名: flash_write_buf
*   功能说明: 写1个扇区并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
               如果无关的数据不是必须保留的，最好在写满的时候调用擦除函数主动擦除，
               否则在写满以后每次写数据都会触发擦除
*   形    参:   buf : 数据源缓冲区；
*               addr ：目标区域首地址
*               len ：数据个数
*   返 回 值: 1 : 成功， 0 ： 失败
*********************************************************************************************************
*/
u8 flash_write_buf(u8* buf, u32 addr, u16 len);

int flash_init( void );
int flash_read( long addr, uint8_t* buf, size_t size );
int flash_write( long addr, const uint8_t* buf, size_t size );
int flash_erase( long addr, size_t size );

#endif

