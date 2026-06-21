#ifndef __DRV_RTC_H__
#define __DRV_RTC_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
} rtc_time_t;

typedef enum {
    RTC_OK = 0,
    RTC_YEAR_ERR,
    RTC_MON_ERR,
    RTC_MDAY_ERR,
    RTC_HOUR_ERR,
    RTC_MIN_ERR,
    RTC_SEC_ERR,
} rtc_error_t;

int rtc_config(void);
unsigned int mk_time(rtc_time_t* time);
void rtc_get_time(rtc_time_t* time);
rtc_error_t rtc_set_time(rtc_time_t* time);

#ifdef __cplusplus
}
#endif

#endif
