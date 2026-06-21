/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 */

#include "board.h"
#include "SDRAMBSP.h"
#include "mh245x_sdram.h"
#include "global-config.h"

void SystemClock_Config(void)
{
    
}
		
uint32_t uwTickFreq = 1;		
__IO uint32_t uwTick;	
static uint32_t _systick_ms = 1;
void rt_hw_board_init(void)
{
		
    SystemCoreClockUpdate();

    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    NVIC_SetPriority(SysTick_IRQn, 0xFF);

    _systick_ms = 1000u / RT_TICK_PER_SECOND;
    if (_systick_ms == 0)
        _systick_ms = 1;
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
//    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
	rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif
}
void rt_hw_inc_tick(void)
{
	uwTick += uwTickFreq;
}
extern vu32 ticks_sys;
/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    ticks_sys++;

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

uint32_t rt_hw_get_tick(void)
{
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
        rt_hw_inc_tick();

    return uwTick;
}

/* ------------- SDRAM HEAP ------------- */
static struct rt_memheap sdram_heap; 

void *sdram_malloc(rt_size_t size)
{
    return rt_memheap_alloc(&sdram_heap, size);
}

void *sdram_realloc(void *ptr, rt_size_t size)
{
    return rt_memheap_realloc(&sdram_heap, ptr, size);
}

void sdram_free(void *ptr)
{
    rt_memheap_free(ptr);
}

static int SDRAM_init(void)
{
#define MEM2_ADDR (SDRAM_MEM_BASE)
#define MEM2_SIZE (8388608)

    SDRAMSetup();

    rt_memheap_init(&sdram_heap, 
        "sdram",
        (void *)MEM2_ADDR, 
        (rt_size_t)MEM2_SIZE);
        
    return 0;
}
INIT_BOARD_EXPORT(SDRAM_init);
