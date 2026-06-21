#include "SEGGER_RTT.h"

static struct rt_device rtt_dev;

/*
 * 适配 RT-Thread 字符设备接口。
 * rt_kprintf() 走控制台时会经过这里，最终写入 J-Link RTT 上行缓冲区。
 */
static rt_ssize_t rtt_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    RT_UNUSED(dev);
    RT_UNUSED(pos);

    if (buffer == RT_NULL || size == 0) {
        return 0;
    }

    return (rt_ssize_t)SEGGER_RTT_Read(0, buffer, size);
}

static rt_ssize_t rtt_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    RT_UNUSED(dev);
    RT_UNUSED(pos);

    if ((buffer == RT_NULL) || (size == 0U)) {
        return 0;
    }

    return (rt_ssize_t)SEGGER_RTT_Write(0, buffer, size);
}

static rt_err_t rtt_control(rt_device_t dev, int cmd, void *args)
{
    RT_UNUSED(dev);
    RT_UNUSED(args);
    RT_UNUSED(cmd);

    return RT_EOK;
}

static const struct rt_device_ops rtt_ops =
{
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rtt_read,
    rtt_write,
    rtt_control
};

int rt_hw_rtt_init(void)
{
    rt_err_t ret;

    SEGGER_RTT_Init();

    rt_memset(&rtt_dev, 0, sizeof(rtt_dev));
    rtt_dev.type = RT_Device_Class_Char;
    rtt_dev.ops = &rtt_ops;

    ret = rt_device_register(&rtt_dev, "rtt0", RT_DEVICE_FLAG_RDWR);
    if (ret != RT_EOK) {
        return ret;
    }

    if (rt_console_set_device("rtt0") == RT_NULL) {
        return -RT_ERROR;
    }

    rt_kprintf("[RTT] console redirected to SEGGER RTT.\r\n");
    rt_kprintf("[RTT] output test message.\r\n");

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_rtt_init);
