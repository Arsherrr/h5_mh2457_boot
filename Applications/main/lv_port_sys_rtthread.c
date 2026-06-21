/**
 * lv_port_sys_rtthread.c
 * LVGL 9.x 在 RT-Thread 上的系统适配（tick、延时、互斥）
 */

#include <rtthread.h>
#include "lvgl.h"

/*=====================
 *  TICK 驱动（1ms）
 *====================*/

static rt_timer_t lvgl_tick_timer = RT_NULL;

/* 1ms 周期定时器回调：给 LVGL 增加系统节拍 */
static void lvgl_tick_timer_cb(void *parameter)
{
    LV_UNUSED(parameter);
    /* 每 1ms 调一次 */
    lv_tick_inc(1);
}

/**
 * 在系统启动时调用一次
 * 比如在 main() 里：
 *   lv_init();
 *   lv_port_sys_init();
 */
void lv_port_sys_init(void)
{
    /* 创建 1ms 周期的软件定时器 */
    rt_tick_t period = rt_tick_from_millisecond(1);
    if (period == 0)
    {
        /* 防止 RT_TICK_PER_SECOND 太小导致 period 为 0 */
        period = 1;
    }

    lvgl_tick_timer = rt_timer_create("lv_tick",
                                      lvgl_tick_timer_cb,
                                      RT_NULL,
                                      period,
                                      RT_TIMER_FLAG_PERIODIC);
    if (lvgl_tick_timer != RT_NULL)
    {
        rt_timer_start(lvgl_tick_timer);
    }
}

/**
 * 如果你有需要在系统退出或重启前释放资源，可以调用
 */
void lv_port_sys_deinit(void)
{
    if (lvgl_tick_timer)
    {
        rt_timer_stop(lvgl_tick_timer);
        rt_timer_delete(lvgl_tick_timer);
        lvgl_tick_timer = RT_NULL;
    }
}

/*=====================
 *  延时封装
 *====================*/

/**
 * LVGL 里有些地方会调用 sleep / delay，你可以在自己的封装里用到它
 */
void lv_port_sleep(uint32_t ms)
{
    rt_thread_mdelay(ms);
}

/*=====================
 *  互斥锁封装（可选）
 *  如果你开启了 LVGL 的多线程支持，可以用 RT-Thread 的 mutex
 *====================*/

typedef struct
{
    rt_mutex_t handle;
} lv_port_mutex_t;

/* 创建互斥锁 */
bool lv_port_mutex_init(lv_port_mutex_t *m)
{
    if (!m) return false;

    m->handle = rt_mutex_create("lv_mtx", RT_IPC_FLAG_PRIO);
    return (m->handle != RT_NULL);
}

/* 删除互斥锁 */
void lv_port_mutex_deinit(lv_port_mutex_t *m)
{
    if (!m || !m->handle) return;

    rt_mutex_delete(m->handle);
    m->handle = RT_NULL;
}

/* 加锁 */
bool lv_port_mutex_lock(lv_port_mutex_t *m)
{
    if (!m || !m->handle) return false;

    return (rt_mutex_take(m->handle, RT_WAITING_FOREVER) == RT_EOK);
}

/* 解锁 */
void lv_port_mutex_unlock(lv_port_mutex_t *m)
{
    if (!m || !m->handle) return;

    rt_mutex_release(m->handle);
}
