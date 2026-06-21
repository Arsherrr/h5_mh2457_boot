#include "rtthread.h"
#include "mh2457.h"
#include "lvgl.h"

#define LARGE_MEM_THRESHOLD ( 2 * 1024 )

extern void *sdram_malloc(rt_size_t size);
extern void *sdram_realloc(void *ptr, rt_size_t size);
extern void sdram_free(void *ptr);

void lv_mem_init(void)
{
}

void lv_mem_deinit(void)
{
}

void lv_mem_monitor_core(lv_mem_monitor_t * mon_p)
{
    if (mon_p) {
        mon_p->total_size = 0;
        mon_p->free_size = 0;
        mon_p->max_used = 0;
        mon_p->used_pct = 0;
        mon_p->frag_pct = 0;
    }
}

lv_result_t lv_mem_test_core(void)
{
    return LV_RESULT_OK;
}

void *lv_malloc_core(size_t size)
{
    if (size == 0) return NULL;

    if (size >= LARGE_MEM_THRESHOLD) {
        /* 大内存从 SDRAM 中分配. */
        return sdram_malloc(size);
    } else {
        /* 小内存从 SRAM 中分配. */
        return rt_malloc(size);
    }
}

void lv_free_core(void *ptr)
{
    if (ptr == NULL) return;

    if ((uintptr_t)ptr >= SDRAM_MEM_BASE) {
        sdram_free(ptr);
    } else {
        rt_free(ptr);
    }
}

void *lv_realloc_core(void *ptr, size_t new_size)
{
    if (new_size == 0) {
        /* 相当于直接释放. */
        lv_free_core(ptr);
        return NULL;
    }

    if (ptr == NULL) {
        /* 相当于 malloc. */
        return lv_malloc_core(new_size);
    }

    void *new_ptr = NULL;

    if (new_size > LARGE_MEM_THRESHOLD) {
        /* 在需要扩展大内存时, 重新在 SDRAM 中分配. */
        if ((uintptr_t)ptr >= SDRAM_MEM_BASE) {
            /* 原本内存就在 sdram 中, 直接重新分配. */
            new_ptr = sdram_realloc(ptr, new_size);
        } else {
            /* 在 sram 中, 需 free, 再从 sdram 中分配. */
            new_ptr = sdram_malloc(new_size);
            if (new_ptr) {
                rt_memcpy(new_ptr, ptr, new_size);
                rt_free(ptr);
            }
        }
    } else {
        /* 小内存的重新分配. */
        if ((uintptr_t)ptr >= SDRAM_MEM_BASE) {
            /* 原内存在 SDRAM, 需要迁移到 SRAM. */
            new_ptr = rt_malloc(new_size);
            if (new_ptr) {
                rt_memcpy(new_ptr, ptr, new_size);
                sdram_free(ptr);
            }
        } else {
            /* 原内存在 SRAM, 直接调用系统 realloc. */
            new_ptr = rt_realloc(ptr, new_size);
        }
    }
    
    return new_ptr;
}
