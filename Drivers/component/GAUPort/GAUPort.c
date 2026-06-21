#include "GAUPort.h"
#include <stdio.h>

static uint8_t colorCode(GAUColorEnum color) {
#define __COLOR_CODE_ARGB8888 0x00U
#define __COLOR_CODE_RGB888   0x01U
#define __COLOR_CODE_RGB565   0x02U
#define __COLOR_CODE_ARGB1555 0x03U
#define __COLOR_CODE_ARGB4444 0x04U
#define __COLOR_CODE_L8       0x05U
#define __COLOR_CODE_AL44     0x06U
#define __COLOR_CODE_AL88     0x07U
#define __COLOR_CODE_L4       0x08U
#define __COLOR_CODE_A8       0x09U
#define __COLOR_CODE_A4       0x0AU

    uint8_t colorCode = 0;
    switch (color) {
        case GAUColorL8:
            colorCode = __COLOR_CODE_L8;
            break;
        case GAUColorRGB565:
            colorCode = __COLOR_CODE_RGB565;
            break;
        case GAUColorRGB888:
            colorCode = __COLOR_CODE_RGB888;
            break;
        case GAUColorARGB8888:
            colorCode = __COLOR_CODE_ARGB8888;
            break;
        default:
            break;
    }
    return colorCode;
}

static void deleteTask(GAUPortStruct* self, GAUTaskStruct* task);

static bool runTask(GAUPortStruct* self, GAUTaskStruct* task) {
    DMA2D_TypeDef* gau = self->GAU;

    bool isRunning = false;
    if (task->Type == GAUTaskCopy) {
        gau->CR = 0;

        // source buffer
        gau->FGPFCCR = colorCode(task->SourceColor);
        gau->FGMAR   = (uint32_t)task->Source;
        gau->FGOR    = task->SourceOffset;
        gau->FGCOLR  = 0;

        // targets buffer
        gau->OPFCCR = colorCode(task->TargetColor) | (task->Rotation << 9);
        gau->OMAR   = (uint32_t)task->Target;
        gau->OOR    = task->TargetOffset;
        gau->OCOLR  = 0;

        gau->NLR = (task->TargetWidth << DMA2D_NLR_PL_Pos) | (task->TargetHeight << DMA2D_NLR_NL_Pos);

        // start
        gau->IFCR  = 0x3FU; // reset flags
        gau->AMTCR = 0;
        if (task->Rotation || (task->SourceColor != task->TargetColor)) {
            gau->CR = BIT16; // Enable PFC
        }
        gau->CR |= //
            BIT9 | // Transfer Complete Interrupt
            BIT0;  // Start
        isRunning = true;
    }
    else {
        // TODO: Fill / Blend / Decode / Event Tasks
#if !USE_RTOS_RTTHREAD
        if (task->EndCallback)
            task->EndCallback(task);

        deleteTask(self, task);
#endif
    }

    return isRunning;
}

#if USE_RTOS_RTTHREAD
/* -------------------- 全局信号量 -------------------- */
static rt_sem_t GAUSemaphore = RT_NULL; /* 用于等待 GAU 任务执行完成. */
static rt_sem_t dma2d_sem = RT_NULL;    /* 用于 DMA2D 资源互斥. */

/* -------------------- 创建任务结构 -------------------- */
static GAUTaskStruct* createTask(GAUPortStruct* self)
{
    return (GAUTaskStruct*)rt_malloc(sizeof(GAUTaskStruct));
}

/* -------------------- 删除任务结构 -------------------- */
static void deleteTask(GAUPortStruct* self, GAUTaskStruct* task)
{
    rt_free(task);
}

/* -------------------- 向任务队列发送任务 -------------------- */
static bool startTask(GAUPortStruct* self, GAUTaskStruct* task)
{
    rt_err_t ret = rt_mq_send(self->TaskQueue, &task, sizeof(void*));
    if (ret != RT_EOK) {
        printf("[GAU] [WARN] Message queue send failed (ret=%d).\n", ret);
        return false;
    }

    return true;
}

/* -------------------- Dispatcher 线程函数 -------------------- */
static void DispatcherTask(GAUPortStruct* self)
{
    GAUTaskStruct* task = RT_NULL;

    while (rt_mq_recv(self->TaskQueue,
                      &task,
                      sizeof(void*),
                      RT_WAITING_FOREVER) > 0)
    {
        /* NULL 表示退出 */
        if (task == RT_NULL)
            break;

        /* 循环清除计数. */
        while (rt_sem_take(GAUSemaphore, RT_WAITING_NO) == RT_EOK);

        /* 获取 DMA2D 使用权. */
        if (rt_sem_take(dma2d_sem,
                        rt_tick_from_millisecond(CONFIG_GAU_TIMEOUT)) != RT_EOK)
        {
            /* 超时, 直接使用. */
            printf("[dma2d] take semaphore failed\n");
        }

        /* 执行任务 */
        if (runTask(self, task))
        {
            if (rt_sem_take(GAUSemaphore,
                            rt_tick_from_millisecond(CONFIG_GAU_TIMEOUT)) != RT_EOK)
            {
                printf("[gau] take semaphore failed\n");
            }
        }

        /* 释放 DMA2D 使用权. */
        rt_sem_release(dma2d_sem);

        /* 回调 */
        if (task->EndCallback)
            task->EndCallback(task);

        deleteTask(self, task);
    }

    /* 删除当前线程 */
    rt_thread_delete(rt_thread_self());
}

/* -------------------- DMA2D 中断处理 -------------------- */
void DMA2D_IRQHandler(void)
{
    rt_interrupt_enter();
    DMA2D->IFCR = 0x3FU;

    /* RT-Thread ISR 中直接 release 信号量即可 */
    rt_sem_release(GAUSemaphore);
    rt_interrupt_leave();
}

/* 霸占 DMA2D 使用权. */
int dma2d_take(rt_int32_t time)
{
    return rt_sem_take(dma2d_sem,
                        rt_tick_from_millisecond(time));
}

/* 释放 DMA2D 使用. */
void dma2d_release(void)
{
    rt_sem_release(dma2d_sem);
}

#else

static GAUPortStruct* gauPort;

static GAUTaskStruct* createTask(GAUPortStruct* self) {
    GAUTaskStruct* task = NULL;
    do {
        __disable_irq();
        if (self->TaskCount < CONFIG_GAU_MAX_TASK_QUEUE_LENGTH) {
            task = self->TaskQueue + ((self->TaskOffset + self->TaskCount) % CONFIG_GAU_MAX_TASK_QUEUE_LENGTH);
            break;
        }
        __enable_irq();
        SystemDelay(1);
    }
    while (task == NULL);
    return task;
}

static void deleteTask(GAUPortStruct* self, GAUTaskStruct* task) {
    __disable_irq();
    self->TaskOffset++;
    self->TaskOffset %= CONFIG_GAU_MAX_TASK_QUEUE_LENGTH;
    self->TaskCount--;
    __enable_irq();
}

static bool startTask(GAUPortStruct* self, GAUTaskStruct* task) {
    __disable_irq();

    self->TaskCount++;
    if (self->TaskCount == 1)
        runTask(self, task);

    __enable_irq();
    return true;
}

void DMA2D_IRQHandler(void) {
    DMA2D->IFCR = 0x3FU;

    GAUPortStruct* self = gauPort;

    if (!self)
        return;

    GAUTaskStruct* task = self->TaskQueue + self->TaskOffset;
    if (task->EndCallback)
        task->EndCallback(task);

    deleteTask(self, task);

    while (self->TaskCount) {
        task = self->TaskQueue + self->TaskOffset;
        if (runTask(self, task))
            break;
    }
}
#endif

static bool Init(GAUPortStruct* self, bool isInit) {
    if (isInit) {
        PeripheralEnable(self->Peripheral, true);
        PeripheralReset(self->Peripheral);

#if USE_RTOS_RTTHREAD
        /* 创建消息队列 */
        self->TaskQueue = rt_mq_create("gau_q",
                                    sizeof(GAUTaskStruct*),
                                    CONFIG_GAU_MAX_TASK_QUEUE_LENGTH,
                                    RT_IPC_FLAG_PRIO);

        if (self->TaskQueue == RT_NULL)
            return false;

        /* 创建 Dispatcher 线程 */
        self->TaskHandle = rt_thread_create("GAU",
                                            (void (*)(void*))DispatcherTask,
                                            self,
                                            CONFIG_GAU_TASK_STACK_SIZE_KB * 1024,
                                            CONFIG_GAU_TASK_PRIORITY,
                                            10);

        if (self->TaskHandle == RT_NULL)
            return false;

        rt_thread_startup(self->TaskHandle);

        /* 创建二值信号量 */
        GAUSemaphore = rt_sem_create("gau_sem", 0, RT_IPC_FLAG_PRIO);
        dma2d_sem = rt_sem_create("dma2d_sem", 1, RT_IPC_FLAG_PRIO); /* 一开始可以获取资源. */

        if (GAUSemaphore == RT_NULL)
            return false;

        if (dma2d_sem == RT_NULL)
            return false;
#else
        self->TaskOffset = 0;
        self->TaskCount  = 0;

        gauPort = self;
#endif
        NVIC_SetPriority(DMA2D_IRQn, CONFIG_GAU_INTERRUPT_PRIORITY);
        NVIC_EnableIRQ(DMA2D_IRQn);
    }
    else {
#if  USE_RTOS_RTTHREAD
        /* 向队列发送 NULL（表示退出 DispatcherTask） */
        rt_mq_send(self->TaskQueue, RT_NULL, sizeof(void*));

        /* 删除消息队列 */
        rt_mq_delete(self->TaskQueue);

        /* 删除信号量 */
        rt_sem_delete(GAUSemaphore);

#else
        gauPort = NULL;
#endif
        NVIC_DisableIRQ(DMA2D_IRQn);
    }

    return true;
}

static bool AddCopyTask(      //
    GAUPortStruct*  self,     //
    GAURotationEnum rotation, //

    void*        source,       //
    GAUColorEnum sourceColor,  //
    uint16_t     sourceOffset, //

    void*        target,       //
    GAUColorEnum targetColor,  //
    uint16_t     targetOffset, //

    uint16_t width,  //
    uint16_t height, //

    GAUTaskCallbackType endCallback, //
    void*               userData     //
) {
    GAUTaskStruct* task = createTask(self);

    if (!task)
        return false;

    task->Type = GAUTaskCopy;

    task->Rotation = rotation;

    task->Source       = source;
    task->SourceColor  = sourceColor;
    task->SourceOffset = sourceOffset;

    task->Target       = target;
    task->TargetColor  = targetColor;
    task->TargetOffset = targetOffset;

    task->TargetWidth  = width;
    task->TargetHeight = height;

    task->EndCallback = endCallback;
    task->UserData    = userData;

    uint8_t retried = 50;
    while (startTask(self, task) == false) {
        /* 启动任务失败可能是消息队列已经满了. */
        if (--retried == 0) {
            printf("[GAU] [WARN] Start task timeout.\n");
            return false;
        }
#if USE_RTOS_RTTHREAD
        rt_thread_mdelay(1);
#else
        SystemDelay(1);
#endif
    }

    return true;
}

bool AddEventTask(                   //
    GAUPortStruct*      self,        //
    GAUTaskCallbackType endCallback, //
    void*               userData     //
) {
    GAUTaskStruct* task = createTask(self);

    if (!task)
        return false;

    task->Type = GAUTaskEvent;

    task->EndCallback = endCallback;
    task->UserData    = userData;

    return startTask(self, task);
}

bool GAUPortConstractor(GAUPortStruct* self, PeripheralEnum peripheralGAU) {
    memset(self, 0, sizeof(GAUPortStruct));

    self->Init         = Init;
    self->AddCopyTask  = AddCopyTask;
    self->AddEventTask = AddEventTask;

    self->Peripheral = peripheralGAU;

    self->GAUBase = PeripheralMap[peripheralGAU].Base;

    return true;
}
