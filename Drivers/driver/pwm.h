#ifndef __PWM_H
#define __PWM_H

#include "mh2457.h"
#include <stdint.h>

/* LCD 背光 PWM 初始化. */
int pwm_bl_init(void);
/* 蜂鸣器 PWM 初始化. */
int pwm_beep_init(void);
/* 设置 LCD 背光占空比，范围 0~100%. */
int pwm_bl_set_percent(uint8_t percent);
/* 设置蜂鸣器占空比，范围 0~100%. */
int pwm_beep_set_percent(uint8_t percent);
/* 使能/关闭 LCD 背光 PWM 输出. */
int pwm_bl_enable(uint8_t enable);
/* 使能/关闭蜂鸣器 PWM 输出. */
int pwm_beep_enable(uint8_t enable);
int pwm_beep_beep(uint16_t ms);

int pwm_driver_init(void);

#endif /* __PWM_H */
