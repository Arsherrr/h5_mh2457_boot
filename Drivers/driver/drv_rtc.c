#include "mh2457.h"
#include "drv_rtc.h"
#include "rtthread.h"
#include "rtdevice.h"
#include <string.h>

#define START_YEAR      (1970)
#define SECOND_DAY      (86400)
#define SECOND_HOUR     (3600)
#define SECOND_MIN      (60)
#define RTC_DEVICE_NAME "rtc"

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t header_size;
    uint32_t data_offset;
} __attribute__((packed)) jl_res_bin_header_t;

const uint16_t mon_yday[][13] =
{
    /* Normal years. */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years. */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

static rt_rtc_dev_t s_rtc_dev;

rt_err_t rt_hw_rtc_register(rt_rtc_dev_t *rtc, const char *name, rt_uint32_t flag, void *data)
{
    if(rtc == RT_NULL || name == RT_NULL) return -RT_EINVAL;

    rtc->parent.type = RT_Device_Class_RTC;
    rtc->parent.flag = flag;
    rtc->parent.user_data = data;

    return rt_device_register(&rtc->parent, name, flag);
}

/* Set the RTC second counter. */
static void rtc_set_sec(unsigned int sec)
{
    RTC_ResetCounter();
    RTC_SetRefRegister(sec);
}

/* Determine whether a year is a leap year. */
static int is_leap_year(int year)
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

/* Get the number of days in a year. */
static int get_days_for_year(int year)
{
    return (is_leap_year(year) ? 366 : 365);
}

/* Get the current RTC seconds value. */
static unsigned int rtc_get_sec(void)
{
    return RTC_GetRefRegister() + RTC_GetCounter();
}

/* Calculate weekday using Zeller's formula. */
u8 get_week(int year, int month, int day)
{
    int c;
    int y;
    int m;
    int d;
    int W;

    if (month == 1 || month == 2) {
        month += 12;
        year--;
    }

    c = year / 100;
    y = year % 100;
    m = month;
    d = day;
    W = c / 4 - 2 * c + y + y / 4 + 26 * (m + 1) / 10 + d - 1;
    if (W < 0) return (W + (-W / 7 + 1) * 7) % 7;
    return W % 7;
}

/* Validate the day field. */
static rtc_error_t check_day(rtc_time_t* time)
{
    static const uint8_t day_check_norm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8_t max_day;
    int year;
    int month;

    if(time == RT_NULL) return RTC_MDAY_ERR;

    year = time->tm_year;
    month = time->tm_mon;
    if(month < 1 || month > 12) return RTC_MON_ERR;

    max_day = day_check_norm[month - 1];
    if(month == 2 && is_leap_year(year)) {
        max_day = 29;
    }

    if(time->tm_mday < 1 || time->tm_mday > max_day) {
        return RTC_MDAY_ERR;
    }
    return RTC_OK;
}

/* Convert calendar time to seconds. */
unsigned int mk_time(rtc_time_t* time)
{
    int year;
    int month;
    unsigned int days = 0;
    unsigned int sec = 0;
    int ily;

    if(time == RT_NULL) return 0;

    for(year = START_YEAR; year < time->tm_year; year++) {
        days += get_days_for_year(year);
    }

    ily = is_leap_year(time->tm_year);
    month = time->tm_mon;
    if(month < 1) month = 1;
    if(month > 12) month = 12;
    days += mon_yday[ily][month - 1];

    days += (unsigned int)(time->tm_mday - 1);
    sec = days * SECOND_DAY + (unsigned int)time->tm_hour * SECOND_HOUR +
          (unsigned int)time->tm_min * SECOND_MIN + (unsigned int)time->tm_sec;

    return sec;
}

/* Date based on Greenwich time. */
static void gm_time(unsigned int second, rtc_time_t* time)
{
    unsigned int left_seconds;
    unsigned int days;
    int cur_year;
    int left_days;
    int days_cur_year;
    int ily;
    int i;

    if(time == RT_NULL) return;

    left_seconds = second % SECOND_DAY;
    days = second / SECOND_DAY;
    cur_year = START_YEAR;
    left_days = (int)days;

    days_cur_year = get_days_for_year(cur_year);
    while (left_days >= days_cur_year) {
        left_days -= days_cur_year;
        cur_year++;
        days_cur_year = get_days_for_year(cur_year);
    }
    time->tm_year = cur_year;

    ily = is_leap_year(cur_year);
    for(i = 1; i < 13; i++) {
        if(left_days < mon_yday[ily][i]) {
            time->tm_mon = i;
            time->tm_mday = left_days - mon_yday[ily][i - 1] + 1;
            break;
        }
    }

    time->tm_hour = left_seconds / SECOND_HOUR;
    time->tm_min = (left_seconds % SECOND_HOUR) / SECOND_MIN;
    time->tm_sec = left_seconds % SECOND_MIN;
    time->tm_wday = get_week(time->tm_year, time->tm_mon, time->tm_mday);
}

/* Get the latest RTC time. */
void rtc_get_time(rtc_time_t* Time)
{
    unsigned int sec;

    sec = rtc_get_sec();
    gm_time(sec, Time);
}

/* Set the RTC time. */
rtc_error_t rtc_set_time(rtc_time_t* Time)
{
    uint32_t seconds;
    rtc_error_t ret;

    if(Time == RT_NULL) return RTC_MDAY_ERR;
    if((Time->tm_year > 2099) || (Time->tm_year < 1970)) return RTC_YEAR_ERR;
    if((Time->tm_mon > 12) || (Time->tm_mon == 0)) return RTC_MON_ERR;
    if((Time->tm_mday > 31) || (Time->tm_mday == 0)) return RTC_MDAY_ERR;
    if(Time->tm_hour > 23) return RTC_HOUR_ERR;
    if(Time->tm_min > 59) return RTC_MIN_ERR;
    if(Time->tm_sec > 59) return RTC_SEC_ERR;

    ret = check_day(Time);
    if(ret != RTC_OK) return ret;

    seconds = mk_time(Time);
    rtc_set_sec(seconds);
    return RTC_OK;
}

/* Set the current time. */
static void set_current_time(void)
{
    rtc_time_t ctime;

    const char *build_date = __DATE__;
    const char *build_time = __TIME__;
    static const char *month_str[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char mon_buf[4] = {0};
    int mon_idx = 0;

    ctime.tm_mon = 1;
    ctime.tm_year = 1970;
    ctime.tm_mday = 1;
    ctime.tm_hour = 0;
    ctime.tm_min = 0;
    ctime.tm_sec = 0;

    if (build_date != RT_NULL && build_time != RT_NULL) {
        mon_buf[0] = build_date[0];
        mon_buf[1] = build_date[1];
        mon_buf[2] = build_date[2];
        for (mon_idx = 0; mon_idx < 12; mon_idx++) {
            if (strcmp(mon_buf, month_str[mon_idx]) == 0) {
                ctime.tm_mon = mon_idx + 1;
                break;
            }
        }
        ctime.tm_mday = (build_date[4] == ' ' ? build_date[5] - '0' : (build_date[4] - '0') * 10 + (build_date[5] - '0'));
        ctime.tm_year = (build_date[7] - '0') * 1000 + (build_date[8] - '0') * 100 + (build_date[9] - '0') * 10 + (build_date[10] - '0');
        ctime.tm_hour = (build_time[0] - '0') * 10 + (build_time[1] - '0');
        ctime.tm_min = (build_time[3] - '0') * 10 + (build_time[4] - '0');
        ctime.tm_sec = (build_time[6] - '0') * 10 + (build_time[7] - '0');
    }

    rtc_set_time(&ctime);
}

/* RTC device get seconds callback. */
static rt_err_t rtc_dev_get_secs(time_t *sec)
{
    if(sec == RT_NULL) return -RT_EINVAL;
    *sec = (time_t)rtc_get_sec();
    return RT_EOK;
}

/* RTC device set seconds callback. */
static rt_err_t rtc_dev_set_secs(time_t *sec)
{
    if(sec == RT_NULL) return -RT_EINVAL;
    rtc_set_sec((unsigned int)(*sec));
    return RT_EOK;
}

/* RTC device get alarm callback. */
static rt_err_t rtc_dev_get_alarm(struct rt_rtc_wkalarm *alarm)
{
    RT_UNUSED(alarm);
    return -RT_ENOSYS;
}

/* RTC device set alarm callback. */
static rt_err_t rtc_dev_set_alarm(struct rt_rtc_wkalarm *alarm)
{
    RT_UNUSED(alarm);
    return -RT_ENOSYS;
}

/* RTC device get timeval callback. */
static rt_err_t rtc_dev_get_timeval(struct timeval *tv)
{
    time_t sec;

    if(tv == RT_NULL) return -RT_EINVAL;
    sec = (time_t)rtc_get_sec();
    tv->tv_sec = sec;
    tv->tv_usec = 0;
    return RT_EOK;
}

/* RTC device set timeval callback. */
static rt_err_t rtc_dev_set_timeval(struct timeval *tv)
{
    if(tv == RT_NULL) return -RT_EINVAL;
    rtc_set_sec((unsigned int)tv->tv_sec);
    return RT_EOK;
}

static const struct rt_rtc_ops s_rtc_ops =
{
    .init = RT_NULL,
    .get_secs = rtc_dev_get_secs,
    .set_secs = rtc_dev_set_secs,
    .get_alarm = rtc_dev_get_alarm,
    .set_alarm = rtc_dev_set_alarm,
    .get_timeval = rtc_dev_get_timeval,
    .set_timeval = rtc_dev_set_timeval,
};

/* RTC init. */
int rtc_config(void)
{
    uint32_t timeout;
    rt_err_t ret;

    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_BPU, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    timeout = 0xFFFFF;
    while((RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) && (timeout-- != 0)) {
        ;
    }
    if (timeout == 0) {
        rt_kprintf("[RTC] LSI start failed.\n");
        return -1;
    }

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RTC_ResetCounter();
    RTC_SetRefRegister(0);

    set_current_time();

    s_rtc_dev.parent.type = RT_Device_Class_RTC;
    s_rtc_dev.parent.flag = RT_DEVICE_FLAG_RDWR;
    s_rtc_dev.ops = &s_rtc_ops;

    ret = rt_hw_rtc_register(&s_rtc_dev, RTC_DEVICE_NAME, RT_DEVICE_FLAG_RDWR, RT_NULL);
    if (ret != RT_EOK) {
        rt_kprintf("[RTC] register device failed: %d\n", ret);
        return -1;
    }

    return 0;
}
INIT_BOARD_EXPORT(rtc_config);
