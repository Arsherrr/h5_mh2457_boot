#include "LVGLPort.h"
#if USE_RTOS_RTTHREAD
#include "rtthread.h"
#endif
#include <stdio.h>

// Configs
#ifndef CONFIG_LVGL_DISPLAY_ORIENTATION
#define CONFIG_LVGL_DISPLAY_ORIENTATION 0
#endif

#ifndef CONFIG_LVGL_PORT_SUPPORT_ROTATION
#define CONFIG_LVGL_PORT_SUPPORT_ROTATION 0
#endif

#ifndef CONFIG_LVGL_PORT_MAX_FLUSH_TIMEOUT
#define CONFIG_LVGL_PORT_MAX_FLUSH_TIMEOUT 200
#endif

// Wrap Macros
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WRAP(function) $Sub$$##function
#define __REAL(function) $Super$$##function
#elif defined(__GNUC__)
#define __WRAP(function) __wrap_##function
#define __REAL(function) __real_##function
#endif

// Lvgl Buffer
static lv_color_t* lv_buf  = (lv_color_t*)0x60000000;
static uint32_t    lv_size = 854 * 480 * 6;

// Display Flush
#if USE_RTOS_RTTHREAD
#define enterCritial() rt_enter_critical()
#define exitCritial()  rt_exit_critical()
#define giveDisplay() rt_sem_release(displayFlushSemaphore);

static rt_sem_t displayFlushSemaphore = RT_NULL;

static void displayLayerBufferChangeEvent(DisplayLayerStruct* layer) {
    giveDisplay();
}

static inline bool takeDisplay(void)
{
    return rt_sem_take(displayFlushSemaphore,
                       rt_tick_from_millisecond(CONFIG_LVGL_PORT_MAX_FLUSH_TIMEOUT)) == RT_EOK;
}

#else
static volatile bool isDisplayFlushing;

#define enterCritial() __disable_irq()
#define exitCritial()  __enable_irq()

#define giveDisplay() isDisplayFlushing = false;

static inline bool takeDisplay() {
    volatile int timeout = CONFIG_LVGL_PORT_MAX_FLUSH_TIMEOUT;
    do {
        if (!isDisplayFlushing)
            break;
        SystemDelay(1);
    }
    while (--timeout);

    isDisplayFlushing = true;
    return timeout > 0;
}

static void displayLayerBufferChangeEvent(DisplayLayerStruct* layer) {
    giveDisplay();
}
#endif

// GAUPort
#if USE_COMPONENT_GAUPORT
static GAUPortStruct* GAUPort;

#if USE_RTOS_RTTHREAD
#define createGAUPort() rt_malloc(sizeof(GAUPortStruct))

#else
static GAUPortStruct lvglGAUPort;

#define createGAUPort() &lvglGAUPort
#endif

static GAUColorEnum GAUColorFromBPP(uint8_t colorBPP) {
    GAUColorEnum color = GAUColorRGB565;

    if (colorBPP > 24)
        color = GAUColorARGB8888;
    else if (colorBPP > 16)
        color = GAUColorRGB888;
    else if (colorBPP > 8)
        color = GAUColorRGB565;
    else
        color = GAUColorL8;

    return color;
}

#if CONFIG_LVGL_GAU_CACHE_SIZE
#define DISP_WIDTH               ( 854 ) /* 屏宽. */
#define DISP_HEIGHT              ( 480 ) /* 屏高. */
#define LVGL_BUFFER_LINES        ( 80  ) /* 缓存行数. */
#define COLOR_DEPTH_BYTE         ( (LV_COLOR_DEPTH + 7) >> 3 )
#define LVGL_LINE_STRIDE_BYTES   (((DISP_WIDTH * COLOR_DEPTH_BYTE) + 3) & ~3) /* 保证 4 字节对齐. */

#define GAU_CACHE_BLOCK_SIZE     (LVGL_LINE_STRIDE_BYTES * LVGL_BUFFER_LINES) /* 单个 Block 的总字节数. */
#define GAU_CACHE_BLOCK_COUNT    (2 + CONFIG_LVGL_PORT_SUPPORT_ROTATION)
__attribute__((aligned(4))) static lv_color_t gauCacheBlock[GAU_CACHE_BLOCK_COUNT][((GAU_CACHE_BLOCK_SIZE / sizeof(lv_color_t)) + 3) & ~3];

#if USE_RTOS_RTTHREAD
static rt_mq_t gauCacheBlockQueue = RT_NULL;
#define createCacheBlocks() \
    gauCacheBlockQueue = rt_mq_create("gau_blk", sizeof(void*), GAU_CACHE_BLOCK_COUNT, RT_IPC_FLAG_PRIO)

static void* takeCacheBlock(void)
{
    void* cacheBlock;

    if (gauCacheBlockQueue == RT_NULL) {
        printf("[cache] gauCacheBlockQueue is NULL\n");
        return NULL;
    }

    /* RT-Thread 的 rt_mq_recv 返回 RT_EOK 表示成功 */
    if (rt_mq_recv(gauCacheBlockQueue,
                   &cacheBlock,
                   sizeof(void*),
                   RT_WAITING_FOREVER) <= 0)
    {
        printf("[cache] take cache block failed\n");
    }

    return cacheBlock;
}

static void giveCacheBlock(void* cacheBlock)
{
    if (cacheBlock == NULL) return;

    /* RT-Thread 的 rt_mq_send 返回 RT_EOK 表示成功 */
    if (rt_mq_send(gauCacheBlockQueue,
                   &cacheBlock,
                   sizeof(void*)) != RT_EOK)
    {
        printf("[cache] give cache block failed\n");
    }
}

#else
static volatile uint32_t gauCacheBlockMask;
#define createCacheBlocks() gauCacheBlockMask = 0

static void* takeCacheBlock(void) {
    while (gauCacheBlockMask == 0) SystemDelay(1);

    for (int i = 0; i < GAU_CACHE_BLOCK_COUNT; i++) {
        if (gauCacheBlockMask & BIT(i)) {
            enterCritial();
            gauCacheBlockMask &= ~BIT(i);
            exitCritial();
            return gauCacheBlock[i];
        }
    }

    __BKPT();
    return NULL;
}

static void giveCacheBlock(void* cacheBlock) {
    for (int i = 0; i < GAU_CACHE_BLOCK_COUNT; i++) {
        if (cacheBlock == gauCacheBlock[i]) {
            gauCacheBlockMask |= BIT(i);
            return;
        }
    }
    __BKPT();
}
#endif

static void giveCacheEvent(GAUTaskStruct* task) {
    giveCacheBlock(task->UserData);
}

#define SWAP(x, y) \
    {              \
        x ^= y;    \
        y ^= x;    \
        x ^= y;    \
    }

static void lvglSyncBuffer(lv_display_t* disp, bool isSyncArea, void* source, void* target, const lv_area_t* area) {
    GAUColorEnum color = GAUColorFromBPP(LV_COLOR_DEPTH);

    uint16_t drawWidth  = lv_display_get_horizontal_resolution(disp);
    uint16_t drawHeight = lv_display_get_vertical_resolution(disp);

    GAURotationEnum gauRotation = GAURotation0;

    uint16_t width  = lv_area_get_width(area);
    uint16_t height = lv_area_get_height(area);

    uint16_t rotatedX1 = area->x1, rotatedY1 = area->y1;
    uint16_t rotatedWidth = width, rotatedHeight = height;
    switch (lv_display_get_rotation(disp)) {
        case LV_DISPLAY_ROTATION_0:
            /* lvgl 本身就是横屏，无旋转的情况下直接旋转 270 对应硬件的竖屏显示. */
            rotatedX1     = area->y1;
            rotatedY1     = drawWidth - (area->x2 + 1);
            rotatedWidth  = height;
            rotatedHeight = width;
            gauRotation   = GAURotation270;
            SWAP(drawWidth, drawHeight);
            break;
        case LV_DISPLAY_ROTATION_90:
            rotatedX1     = area->y1;
            rotatedY1     = drawWidth - (area->x2 + 1);
            rotatedWidth  = height;
            rotatedHeight = width;
            gauRotation   = GAURotation270;
            SWAP(drawWidth, drawHeight);
            break;
        case LV_DISPLAY_ROTATION_180:
            rotatedX1     = drawWidth - (area->x2 + 1);
            rotatedY1     = drawHeight - (area->y2 + 1);
            rotatedWidth  = width;
            rotatedHeight = height;
            gauRotation   = GAURotation180;
            break;
        case LV_DISPLAY_ROTATION_270:
            rotatedX1     = drawHeight - (area->y2 + 1);
            rotatedY1     = area->x1;
            rotatedWidth  = height;
            rotatedHeight = width;
            gauRotation   = GAURotation90;
            SWAP(drawWidth, drawHeight);
            break;
        default:
            LV_LOG_ERROR("lvgl rotation error!");
            break;
    }

    target = target + (rotatedX1 + rotatedY1 * drawWidth) * (color & 0x0F);

    uint16_t offset = drawWidth - rotatedWidth;
    if (!isSyncArea) {
        void* gauSource = source;
        if (gauRotation) {
#if CONFIG_LVGL_PORT_SUPPORT_ROTATION
            gauSource = takeCacheBlock();
            if (GAUPort->AddCopyTask(GAUPort, gauRotation, source, color, 0, gauSource, color, 0, width, height, giveCacheEvent, source) == false) {
                printf("[GAU] [WARN] Create rotation task failed.\n");
                /* 释放资源. */
                giveCacheBlock(source);
                giveCacheBlock(gauSource);
                return;
            }
#else
            LV_LOG_ERROR("Lvgl port with rotation support should enable `CONFIG_LVGL_PORT_SUPPORT_ROTATION`");
#endif
        }

        if (GAUPort->AddCopyTask(GAUPort, GAURotation0, gauSource, color, 0, target, color, offset, rotatedWidth, rotatedHeight, giveCacheEvent, gauSource) == false) {
            printf("[GAU] [WARN] Create copy task failed.\n");
            /* 释放资源. */
            giveCacheBlock(gauSource);
            return;
        }
    }
    else {
        source = source + (rotatedX1 + rotatedY1 * drawWidth) * (color & 0x0F);
        GAUPort->AddCopyTask(GAUPort, GAURotation0, source, color, offset, target, color, offset, rotatedWidth, rotatedHeight, NULL, NULL);
    }
}

static void lvglSaveSyncAreas(lv_display_t* disp) {
    // save sync_areas for next buffer
    for (uint32_t i = 0; i < disp->inv_p; i++) {
        if (disp->inv_area_joined[i])
            continue;

        lv_area_t* sync_area = lv_ll_ins_tail(&disp->sync_areas);
        *sync_area           = disp->inv_areas[i];
    }
}

static void layerBufferChangeEvent(GAUTaskStruct* task) {
    DisplayLayerStruct* layer = (DisplayLayerStruct*)task->UserData;
    layer->ChangeBuffer(layer, NULL, displayLayerBufferChangeEvent);
}

static bool lvglRefreshSyncAreas(lv_display_t* disp, void* currentBuffer, void* nextBuffer) {
    // refresh sync areas
    if (lv_ll_is_empty(&disp->sync_areas))
        return false;

    uint32_t hor_res = lv_display_get_horizontal_resolution(disp);
    uint32_t ver_res = lv_display_get_vertical_resolution(disp);

    /*Iterate through invalidated areas to see if sync area should be copied*/
    lv_area_t  res[4] = {0};
    lv_area_t *sync_area, *new_area, *next_area;
    for (uint16_t i = 0; i < disp->inv_p; i++) {
        /*Skip joined areas*/
        if (disp->inv_area_joined[i])
            continue;

        /*Iterate over sync areas*/
        sync_area = lv_ll_get_head(&disp->sync_areas);
        while (sync_area != NULL) {
            /*Get next sync area*/
            next_area = lv_ll_get_next(&disp->sync_areas, sync_area);

            /*Remove intersect of redraw area from sync area and get remaining areas*/
            int8_t res_c = lv_area_diff(res, sync_area, &disp->inv_areas[i]);

            /*New sub areas created after removing intersect*/
            if (res_c != -1) {
                /*Replace old sync area with new areas*/
                for (int8_t j = 0; j < res_c; j++) {
                    new_area  = lv_ll_ins_prev(&disp->sync_areas, sync_area);
                    *new_area = res[j];
                }
                lv_ll_remove(&disp->sync_areas, sync_area);
                lv_free(sync_area);
            }

            /*Move on to next sync area*/
            sync_area = next_area;
        }
    }

    if (currentBuffer != nextBuffer) {
        lv_area_t disp_area = {0, 0, (int32_t)hor_res - 1, (int32_t)ver_res - 1};
        /*Copy sync areas (if any remaining)*/
        for (sync_area = lv_ll_get_head(&disp->sync_areas); sync_area != NULL; sync_area = lv_ll_get_next(&disp->sync_areas, sync_area)) {
            lv_area_intersect(sync_area, sync_area, &disp_area);

            lvglSyncBuffer(disp, true, currentBuffer, nextBuffer, sync_area);
        }
    }

    /*Clear sync areas*/
    lv_ll_clear(&disp->sync_areas);

    return true;
}

#endif

static void gauPortSetup(void) {
    // GAU Setup
    GAUPort = createGAUPort();
    GAUPortConstractor(GAUPort, PeripheralDMA2D);
    GAUPort->Init(GAUPort, true);

#if CONFIG_LVGL_GAU_CACHE_SIZE
    createCacheBlocks();
    for (int i = 0; i < GAU_CACHE_BLOCK_COUNT; i++) {
        giveCacheBlock(gauCacheBlock[i]);
    }
#endif
}

#endif

// Lvgl Display
static void *nextBuffer = NULL, *currentBuffer = NULL;
#if LVGL_VERSION_MAJOR == 9
static void lvglDisplayFlushEvent(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
    DisplayStruct* displayPort = (DisplayStruct*)lv_display_get_driver_data(disp);
#else
static void lvglDisplayFlushEvent(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* px_map) {
    lv_display_t* disp = lv_disp_get_default();

    DisplayStruct* displayPort = (DisplayStruct*)disp_drv->user_data;
#endif
    DisplayLayerStruct* layer = displayPort->Layer;

    if (displayPort->RefreshMode == DisplayRefreshContinuous) {
#if CONFIG_LVGL_GAU_CACHE_SIZE
        if (!lvglIsDirectMode(disp)) {
            if (!nextBuffer) {
                // switch current buffer and next buffer
                enterCritial();
                if ((layer->NextBuffer ? layer->NextBuffer : layer->CurrentBuffer) == layer->Buffer[0].Pointer) {
                    currentBuffer = layer->Buffer[0].Pointer;
                    nextBuffer    = layer->Buffer[1].Pointer;
                }
                else {
                    currentBuffer = layer->Buffer[1].Pointer;
                    nextBuffer    = layer->Buffer[0].Pointer;
                }
                exitCritial();

                lvglRefreshSyncAreas(disp, currentBuffer, nextBuffer);
                lvglSaveSyncAreas(disp);

                // wait until buffer flushed
                if (!takeDisplay()) {
                    LV_LOG_ERROR("display wait error");
                }
            }

            // sync draw buffer
            lvglSyncBuffer(disp, false, px_map, nextBuffer, area);
            if (lv_display_flush_is_last(disp)) {
                nextBuffer = NULL;
                GAUPort->AddEventTask(GAUPort, layerBufferChangeEvent, layer);
            }

            // take a free cache block to draw
            lvBufAct(disp) = takeCacheBlock();
        }
        else
#endif
        {
            if (lv_display_flush_is_last(disp)) {
                layer->ChangeBuffer(layer, lvBufAct(disp), displayLayerBufferChangeEvent);
                if (!takeDisplay()) {
                    LV_LOG_ERROR("display wait error");
                }
            }
        }
    }
    else {
        lv_area_t rArea = *area;
        /*
        area->x1 &= ~0x01;
        area->x2 &= ~0x01;
        area->y1 &= ~0x01;
        area->y2 &= ~0x01;
        */
        displayPort->Refresh(                                      //
            displayPort,                                           //
            lvBufActGotoXY(disp, rArea.x1, rArea.y1),              //
            lv_area_get_width(&rArea), lv_area_get_height(&rArea), //
            rArea.x1, rArea.y1,                                    //
            NULL                                                   //
        );
    }

    lv_display_flush_ready(disp);
}

#if LVGL_VERSION_MAJOR == 9

#if USE_RTOS_RTTHREAD
static uint32_t lvglGetTick(void)
{
    return rt_tick_get_millisecond();
}

#endif

#if USE_COMPONENT_GAUPORT
void __WRAP(lv_draw_buf_copy)(lv_draw_buf_t* dest, const lv_area_t* dest_area, const lv_draw_buf_t* src, const lv_area_t* src_area) {
    GAUColorEnum color = GAUColorFromBPP(lv_color_format_get_bpp(dest->header.cf));

    uint8_t* source = src_area ? lv_draw_buf_goto_xy(src, src_area->x1, src_area->y1) : lv_draw_buf_goto_xy(src, 0, 0);
    uint8_t* target = dest_area ? lv_draw_buf_goto_xy(dest, dest_area->x1, dest_area->y1) : lv_draw_buf_goto_xy(dest, 0, 0);

    uint16_t sourceOffset = src_area ? src->header.w - lv_area_get_width(src_area) : 0;

    uint16_t targetOffset, width, height;
    if (dest_area) {
        width        = lv_area_get_width(dest_area);
        height       = lv_area_get_height(dest_area);
        targetOffset = dest->header.w - width;
    }
    else {
        width        = dest->header.w;
        height       = dest->header.h;
        targetOffset = 0;
    }

    GAUPort->AddCopyTask(GAUPort, GAURotation0, source, color, sourceOffset, target, color, targetOffset, width, height, NULL, NULL);
}
#endif

static void lvglDisplaySetup(DisplayStruct* displayPort, uint16_t layerWidth, uint16_t layerHeight, void* buf0, void* buf1) {
#if USE_RTOS_RTTHREAD
    // set get tick call back function
    lv_tick_set_cb(lvglGetTick);
#endif

    lv_display_t* disp = lv_display_create(layerWidth, layerHeight);
    lv_display_set_flush_cb(disp, lvglDisplayFlushEvent);
#if CONFIG_LVGL_GAU_CACHE_SIZE
    lv_display_set_buffers(disp, takeCacheBlock(), NULL, sizeof(gauCacheBlock[0]), LV_DISPLAY_RENDER_MODE_PARTIAL);
#else
    lv_display_set_buffers(disp, buf0, buf1, layerWidth * layerHeight * (LV_COLOR_DEPTH >> 3), LV_DISPLAY_RENDER_MODE_DIRECT);
#endif
    lv_display_set_driver_data(disp, displayPort);
}

#else

static void lvglDisplaySetup(DisplayStruct* displayPort, uint16_t layerWidth, uint16_t layerHeight, void* buf0, void* buf1) {
    static lv_disp_drv_t      disp_drv; /*Descriptor of a display driver*/
    static lv_disp_draw_buf_t lv_draw_buf_dsc;

    lv_disp_drv_init(&disp_drv);

#if CONFIG_LVGL_GAU_CACHE_SIZE
    lv_disp_draw_buf_init(&lv_draw_buf_dsc, takeCacheBlock(), NULL, GAU_CACHE_BLOCK_SIZE / (LV_COLOR_DEPTH >> 3));
    disp_drv.direct_mode = 0;
#else
    lv_disp_draw_buf_init(&lv_draw_buf_dsc, buf0, buf1, layerWidth * layerHeight);
    disp_drv.direct_mode = 1;
#endif

    /*Set up the functions to access to your display*/

    /* Set the resolution of the display */
    disp_drv.hor_res = layerWidth;
    disp_drv.ver_res = layerHeight;

    /* Used to copy the buffer's content to the display */
    disp_drv.flush_cb = lvglDisplayFlushEvent;

    /* Set a display buffer */
    disp_drv.draw_buf = &lv_draw_buf_dsc;

    /* Required for full frame buffer */
    //disp_drv.full_refresh = 1;

    disp_drv.user_data = displayPort;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}
#endif

bool DSIDisplayConfig(DisplayStruct* display) {
#if LV_COLOR_DEPTH >= 24
    if (display->Color != DisplayColorRGB888) {
        display->Color = DisplayColorRGB888;
        // change the color coding to DSIColorCoding24 only if the 24-bit color is supported by driver IC
        display->DSIHandle->ColorCoding = DSIColorCoding24;
    }
#elif LV_COLOR_DEPTH == 16
    if (display->Color != DisplayColorRGB565) {
        display->Color = DisplayColorRGB565;

        display->DSIHandle->ColorCoding = DSIColorCoding16C1;
    }
#endif

    return true;
}

void log_cb(lv_log_level_t level, const char * buf)
{

    (void)(level);
    printf("[LVGL] %s\n", buf);
}

void LVGLSetup(void)
{    
#if USE_COMPONENT_GAUPORT
    gauPortSetup();
#endif

    // Display Setup
#if USE_RTOS_RTTHREAD
    displayFlushSemaphore = rt_sem_create("lvgl_flush", 0, RT_IPC_FLAG_PRIO);
#endif
    giveDisplay();

    DisplayStruct* displayPort = DisplaySelectSetup((uint32_t*)lv_buf, LV_BUF_SIZE);

    DisplayLayerStruct* displayLayer = NULL;

    uint16_t displayWidth, displayHeight;
    uint16_t layerWidth, layerHeight;

    void* buf0 = NULL;
    void* buf1 = NULL;

    if (displayPort == NULL) {
        // no display port initialized, set default layer config only for test
        layerWidth  = 320;
        layerHeight = 240;

        buf0 = lv_buf;
    }
    else {
        displayWidth  = displayPort->HorizontalActive;
        displayHeight = displayPort->VerticalActive;

        displayLayer = &displayPort->Layer[0];

        layerWidth  = displayLayer->Width;
        layerHeight = displayLayer->Height;

        buf0 = displayLayer->Buffer[0].Pointer;
        buf1 = displayPort->RefreshMode == DisplayRefreshAdaptive ? NULL : displayLayer->Buffer[1].Pointer;

        SWAP(layerWidth, layerHeight);
    }

    lv_init();

#if LV_MEM_POOL_EXPAND_SIZE
    void *lv_mem_ex = sdram_malloc(LV_MEM_POOL_EXPAND_SIZE);
    if (lv_mem_ex) {
        // Add expand heap from offset 1MB of SDRAM
        lv_tlsf_add_pool(LV_GLOBAL_DEFAULT()->tlsf_state.tlsf, lv_mem_ex, LV_MEM_POOL_EXPAND_SIZE);
    } else {
        printf("[LVGL] No free memory for lvgl expand heap.\n");
    }
#endif

    lvglDisplaySetup(displayPort, layerWidth, layerHeight, buf0, buf1);

    // lv_display_set_rotation(NULL, LV_DISPLAY_ROTATION_270);

    // Start display port
    if (displayPort) {
        DisplayTestPattern(displayPort);
        displayPort->Start(displayPort, true);
    }

#if LV_USE_LOG
    lv_log_register_print_cb(log_cb);
#endif
}
