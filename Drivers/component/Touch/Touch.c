#include "Touch.h"

static void ConvertCoordinate(TouchStruct* touch, uint16_t* x, uint16_t* y) {
    if (touch->NativeCoordConversion)
        return;

    if (touch->SwitchXY) {
        *x ^= *y;
        *y ^= *x;
        *x ^= *y;
    }

    if (touch->ReverseX)
        *x = touch->X - *x;

    if (touch->ReverseY)
        *y = touch->Y - *y;
}
void show_xy(u16 x, u16 y);
static bool GetCoordinate(TouchStruct* touch, bool* isTouched, uint16_t* x, uint16_t* y) {
    TouchCoordinateStruct coordinate;
#if USE_RTOS_FREERTOS
    if (uxQueueMessagesWaiting(touch->CoordinateQueue))
        xQueueReceive(touch->CoordinateQueue, &coordinate, 0);
    else
#elif USE_RTOS_RTTHREAD
    if (touch->CoordinateQueue->entry > 0)
    {
        /* 非阻塞接收（timeout = 0） */
        rt_mq_recv(touch->CoordinateQueue,
                   &coordinate,
                   sizeof(coordinate),
                   0);
    }
    else
#endif
        coordinate = touch->Coordinate;

    *isTouched = coordinate.IsTouched;

    if (*isTouched) {
        *x = coordinate.X;
        *y = coordinate.Y;
        ConvertCoordinate(touch, x, y);
    }
#if USE_RTOS_FREERTOS
    return uxQueueMessagesWaiting(touch->CoordinateQueue) > 0;
#elif USE_RTOS_RTTHREAD

    return touch->CoordinateQueue->entry > 0;

#else
    return false;
#endif
}

static void SetCoordinate(TouchStruct* touch, bool isTouched, uint16_t x, uint16_t y) {
    TouchCoordinateStruct coordinate = (TouchCoordinateStruct) {.X = x, .Y = y, .IsTouched = isTouched};
#if USE_RTOS_FREERTOS
    TouchCoordinateStruct lastCoordinate;
    if (!isTouched && (xQueuePeek(touch->CoordinateQueue, &lastCoordinate, (TickType_t)0) == pdTRUE) && !lastCoordinate.IsTouched) {
        return;
    }

    if (uxQueueSpacesAvailable(touch->CoordinateQueue) == 0) {
        void* dummy;
        xQueueReceive(touch->CoordinateQueue, &dummy, 0);
    }
    xQueueSend(touch->CoordinateQueue, &coordinate, 0);
#elif USE_RTOS_RTTHREAD
    TouchCoordinateStruct lastCoordinate;

    /* ---------- 1. xQueuePeek → RT-Thread 5.x 等价实现 ---------- */
    /* RT-Thread 5.x 没有 peek，需要用 recv + send 实现“伪 peek” */

    if (touch->CoordinateQueue->entry > 0)   /* 队列中有消息 */
    {
        /* 先取出一条 */
        if (rt_mq_recv(touch->CoordinateQueue,
                       &lastCoordinate,
                       sizeof(lastCoordinate),
                       0) == RT_EOK)
        {
            /* 再放回去（保持队列内容不变） */
            rt_mq_send(touch->CoordinateQueue,
                       &lastCoordinate,
                       sizeof(lastCoordinate));

            /* 原 FreeRTOS 条件逻辑 */
            if (!isTouched && !lastCoordinate.IsTouched)
                return;
        }
    }

    /* ---------- 2. uxQueueSpacesAvailable → RT-Thread 5.x 等价 ---------- */
    /* RT-Thread 5.x 直接访问内部结构体字段 */

    if (touch->CoordinateQueue->entry >= touch->CoordinateQueue->max_msgs)
    {
        /* 队列满了 → 丢弃最旧的一条 */
        TouchCoordinateStruct dummy;
        rt_mq_recv(touch->CoordinateQueue,
                   &dummy,
                   sizeof(dummy),
                   0);
    }

    /* ---------- 3. xQueueSend → rt_mq_send ---------- */

    rt_mq_send(touch->CoordinateQueue,
               &coordinate,
               sizeof(coordinate));


#endif
    touch->Coordinate = coordinate;
}

static bool GetEvent(TouchStruct* touch) {
    // always fire event if no interrupt IO is set
    if (touch->IntIO == IONone)
        return true;
    if (EXTI->PR & IOPin(touch->IntIO)) {
        EXTI->PR = IOPin(touch->IntIO);
        return true;
    }
    return false;
}

#if USE_RTOS_FREERTOS
static void TouchEventTask(TouchStruct* touch) {
    while (xSemaphoreTake(touch->EventSemaphore, portMAX_DELAY) == pdPASS) {
        touch->Update(touch);
    }
}

void TouchEvent(TouchStruct* touch) {
    if (GetEvent(touch)) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(touch->EventSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
#elif USE_RTOS_RTTHREAD
/* ---------------- TouchEventTask ---------------- */
static void TouchEventTask(TouchStruct* touch)
{
    while (rt_sem_take(touch->EventSemaphore, RT_WAITING_FOREVER) == RT_EOK)
    {
        touch->Update(touch);
    }
}

/* ---------------- TouchEvent (ISR) ---------------- */
void TouchEvent(TouchStruct* touch)
{
    if (GetEvent(touch))
    {
        /* RT-Thread ISR 中直接 release 信号量即可 */
        rt_sem_release(touch->EventSemaphore);
    }
}

#else
void TouchEvent(TouchStruct* touch) {
    if (GetEvent(touch))
        touch->Update(touch);
}
#endif

bool TouchConstractor(TouchStruct* touch, I2CPortStruct* i2cPort) {
    memset(touch, 0, sizeof(TouchStruct));
    touch->I2CPort = i2cPort;

    touch->IntIO   = IONone;
    touch->ResetIO = IONone;

#if USE_RTOS_FREERTOS
    touch->CoordinateQueue = xQueueCreate(CONFIG_TOUCH_MAX_COORDINATE_QUEUE_LENGTH, sizeof(TouchCoordinateStruct));
    touch->EventSemaphore  = xSemaphoreCreateBinary();

    if (                                                      //
        !xTaskCreate(                                         //
            (TaskFunction_t)TouchEventTask,                   //
            "Touch",                                          //
            RTOSStackSizeKB(CONFIG_TOUCH_TASK_STACK_SIZE_KB), //
            touch,                                            //
            GONFIG_TOUCH_TASK_PRIORITY,                       //
            NULL                                              //
            )                                                 //
    )
        return false;
#elif USE_RTOS_RTTHREAD
    /* 创建消息队列 */
    touch->CoordinateQueue = rt_mq_create("touch_q",
                                        sizeof(TouchCoordinateStruct),
                                        CONFIG_TOUCH_MAX_COORDINATE_QUEUE_LENGTH,
                                        RT_IPC_FLAG_PRIO);
    if (touch->CoordinateQueue == RT_NULL)
        return false;

    /* 创建二值信号量 */
    touch->EventSemaphore = rt_sem_create("touch_sem", 0, RT_IPC_FLAG_PRIO);
    if (touch->EventSemaphore == RT_NULL)
        return false;

    /* 创建 TouchEventTask 线程 */
    touch->TaskHandle = rt_thread_create("Touch",
                                        (void (*)(void*))TouchEventTask,
                                        touch,
                                        CONFIG_TOUCH_TASK_STACK_SIZE_KB * 1024,
                                        CONFIG_TOUCH_TASK_PRIORITY,
                                        10);
    if (touch->TaskHandle == RT_NULL)
        return false;

    rt_thread_startup(touch->TaskHandle);

#endif

    touch->SetCoordinate = SetCoordinate;
    touch->GetCoordinate = GetCoordinate;

    return true;
}

void TouchEventInit(TouchStruct* touch) {
    if (touch->IntIO == IONone)
        return;

    PeripheralEnable(PeripheralEXTI, true);
    PeripheralEnable(PeripheralSYSCFG, true);

    IOSetup(touch->IntIO, MakeIOConfig(IOModeInput, 0, IOPullUp, IOSpeedLow, IODriveLow));

    // set the source pin of EXTI line
    uint8_t pinIndex  = IOSource(touch->IntIO);
    uint8_t portIndex = touch->IntIO >> 4;
    uint8_t maskPos   = (pinIndex & 0x3) << 2;

    SYSCFG->EXTICR[pinIndex >> 2] = (SYSCFG->EXTICR[pinIndex >> 2] & ~(0xF << maskPos)) | portIndex << maskPos;

    // enable the EXTI event
    if (touch->IsRasingEvent) {
        EXTI->RTSR |= IOPin(touch->IntIO);
        EXTI->FTSR &= ~IOPin(touch->IntIO);
    }
    else {
        EXTI->RTSR &= ~IOPin(touch->IntIO);
        EXTI->FTSR |= IOPin(touch->IntIO);
    }
    EXTI->IMR |= IOPin(touch->IntIO);
}
