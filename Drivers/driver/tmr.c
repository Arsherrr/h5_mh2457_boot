#include "mh2457.h"
#include "tmr.h"
#include "lvgl.h"

vu32 ticks_sys;
static uint8_t tmr_inited = 0;

void SysTick_Handler(void)
{
    ticks_sys++;
    lv_tick_inc(1);
}

u32 get_time_tick(void)
{
    return ticks_sys;
}

void delay_ms(u32 ms)
{
#if 0
    u32 ticks_end = ticks_sys + ms;
    while (ticks_sys < ticks_end);
#else
    SystemDelay(ms);
#endif
}

int delay_us_init(void)
{
    if (tmr_inited) return 0;

    TIM_TimeBaseInitTypeDef TIM_BaseInitStruct;

    /* 1. 开启 TIM6 时钟 (TIM6 挂在 APB1 总线上). */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* 2. 配置 TIM6. */
    TIM_BaseInitStruct.TIM_Period = 0xFFFF; /* 自动重装载值设为最大. */
    TIM_BaseInitStruct.TIM_Prescaler = (SystemCoreClock / 2 / 1000000) - 1; /* 1MHz 计数频率 (1us/step). */
    TIM_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM6, &TIM_BaseInitStruct);

    /* 3. 让定时器保持运行, 不关闭. */
    TIM_SetCounter(TIM6, 0);
    TIM_Cmd(TIM6, ENABLE);

    tmr_inited = 1;
    return 0;
}

void delay_us(u16 us)
{
    if (tmr_inited) {
        /* 定时器已初始化. */
        uint16_t start = TIM_GetCounter(TIM6);
        while ((uint16_t)(TIM_GetCounter(TIM6) - start) < us);
    }  else {
        /* 定时器未初始化. */
        SystemDelayUs(us);
    }
}
