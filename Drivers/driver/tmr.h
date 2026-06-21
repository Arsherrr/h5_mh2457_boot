#ifndef __TMR_H
#define __TMR_H

extern vu32 ticks_sys;

u32 get_time_tick(void);
int delay_us_init(void);
void delay_us(u16 us);
void delay_ms(u32 ms);

#endif
