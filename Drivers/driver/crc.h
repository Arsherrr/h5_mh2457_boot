#ifndef _SYS_H_
#define _SYS_H_

extern u32 ticks_standby;

u16 crc16(u8 *buf, u32 len, u16 init);
void crc16_hw_init(void);
u16 crc16_hw(u8* buf, u32 len);
void crc32_hw_init(void);
u32 crc32_hw(u8* buf, u32 len);
u32 crc32_flash_hw(u32 addr, u32 len, u8* buf, u32 buf_size);

#endif

