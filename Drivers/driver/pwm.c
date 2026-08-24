#include "mh2457.h"
#include "io_config.h"
#include "pwm.h"
#include "tmr.h"

#define LCD_PWM_TIM                 TIM2
#define LCD_PWM_TIM_CLK             RCC_APB1Periph_TIM2
#define LCD_PWM_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define LCD_PWM_GPIO                DSI_BL_GPIO
#define LCD_PWM_PIN                 DSI_BL_PIN
#define LCD_PWM_AF                  GPIO_AF_TIM2
#define LCD_PWM_PINSOURCE           GPIO_PinSource1
#define LCD_PWM_CHANNEL             TIM_Channel_2

#define BEEP_PWM_TIM                TIM4
#define BEEP_PWM_TIM_CLK            RCC_APB1Periph_TIM4
#define BEEP_PWM_GPIO_CLK           RCC_AHB1Periph_GPIOB
#define BEEP_PWM_GPIO               BEEP_GPIO
#define BEEP_PWM_PIN                BEEP_PIN
#define BEEP_PWM_AF                 GPIO_AF_TIM4
#define BEEP_PWM_PINSOURCE          GPIO_PinSource9
#define BEEP_PWM_CHANNEL            TIM_Channel_4

#define PWM_MAX_PERCENT             100U
#define LCD_PWM_PERIOD              999U

/* 配置为 2kHz */
#define BEEP_PWM_PRESCALER          83U
#define BEEP_PWM_PERIOD             999U
#define BEEP_PWM_DUTY_PERCENT       100U

static uint32_t pwm_map_percent_to_pulse(uint32_t period, uint8_t percent)
{
    if (percent > PWM_MAX_PERCENT) {
        percent = PWM_MAX_PERCENT;
    }

    return ((period + 1U) * percent) / PWM_MAX_PERCENT;
}

static void pwm_gpio_tim_init(GPIO_TypeDef *gpio_port,
                              uint16_t gpio_pin,
                              uint8_t pin_source,
                              uint8_t gpio_af,
                              uint32_t gpio_clk,
                              uint32_t tim_clk,
                              TIM_TypeDef *tim,
                              uint8_t channel,
                              uint16_t period)
{
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef tim_base;
    TIM_OCInitTypeDef oc;

    RCC_AHB1PeriphClockCmd(gpio_clk, ENABLE);
    RCC_APB1PeriphClockCmd(tim_clk, ENABLE);

    GPIO_PinAFConfig(gpio_port, pin_source, gpio_af);

    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = gpio_pin;
    GPIO_Init(gpio_port, &gpio);

    TIM_TimeBaseStructInit(&tim_base);
    tim_base.TIM_Prescaler = (tim == BEEP_PWM_TIM) ? BEEP_PWM_PRESCALER : 0U;
    tim_base.TIM_CounterMode = TIM_CounterMode_Up;
    tim_base.TIM_Period = period;
    tim_base.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_base.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(tim, &tim_base);

    if(tim == BEEP_PWM_TIM) {
        TIM_PrescalerConfig(tim, BEEP_PWM_PRESCALER, TIM_PSCReloadMode_Immediate);
    }

    TIM_OCStructInit(&oc);
    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OCPolarity = TIM_OCPolarity_High;
    oc.TIM_Pulse = 0;

    switch (channel) {
    case TIM_Channel_1:
        TIM_OC1Init(tim, &oc);
        TIM_OC1PreloadConfig(tim, TIM_OCPreload_Enable);
        break;
    case TIM_Channel_2:
        TIM_OC2Init(tim, &oc);
        TIM_OC2PreloadConfig(tim, TIM_OCPreload_Enable);
        break;
    case TIM_Channel_3:
        TIM_OC3Init(tim, &oc);
        TIM_OC3PreloadConfig(tim, TIM_OCPreload_Enable);
        break;
    case TIM_Channel_4:
        TIM_OC4Init(tim, &oc);
        TIM_OC4PreloadConfig(tim, TIM_OCPreload_Enable);
        break;
    default:
        break;
    }

    TIM_ARRPreloadConfig(tim, ENABLE);
    TIM_Cmd(tim, ENABLE);
}

static void pwm_set_duty_by_channel(TIM_TypeDef *tim, uint8_t channel, uint8_t percent)
{
    uint32_t period = (tim == LCD_PWM_TIM) ? LCD_PWM_PERIOD : BEEP_PWM_PERIOD;
    uint32_t pulse = pwm_map_percent_to_pulse(period, percent);

    switch (channel) {
    case TIM_Channel_1:
        TIM_SetCompare1(tim, pulse);
        break;
    case TIM_Channel_2:
        TIM_SetCompare2(tim, pulse);
        break;
    case TIM_Channel_3:
        TIM_SetCompare3(tim, pulse);
        break;
    case TIM_Channel_4:
        TIM_SetCompare4(tim, pulse);
        break;
    default:
        break;
    }
}

int pwm_bl_init(void)
{
    pwm_gpio_tim_init(LCD_PWM_GPIO,
                      LCD_PWM_PIN,
                      LCD_PWM_PINSOURCE,
                      LCD_PWM_AF,
                      LCD_PWM_GPIO_CLK,
                      LCD_PWM_TIM_CLK,
                      LCD_PWM_TIM,
                      LCD_PWM_CHANNEL,
                      LCD_PWM_PERIOD);
    pwm_set_duty_by_channel(LCD_PWM_TIM, LCD_PWM_CHANNEL, 0);
    return 0;
}

int pwm_beep_init(void)
{
    pwm_gpio_tim_init(BEEP_PWM_GPIO,
                      BEEP_PWM_PIN,
                      BEEP_PWM_PINSOURCE,
                      BEEP_PWM_AF,
                      BEEP_PWM_GPIO_CLK,
                      BEEP_PWM_TIM_CLK,
                      BEEP_PWM_TIM,
                      BEEP_PWM_CHANNEL,
                      BEEP_PWM_PERIOD);
    pwm_set_duty_by_channel(BEEP_PWM_TIM, BEEP_PWM_CHANNEL, 0);

    pwm_beep_enable(0);
    return 0;
}

int pwm_beep_beep(uint16_t ms)
{
    pwm_beep_set_percent(BEEP_PWM_DUTY_PERCENT);
    pwm_beep_enable(1);
    delay_ms(ms);
    pwm_beep_enable(0);
    pwm_beep_set_percent(0);
    return 0;
}

int pwm_bl_set_percent(uint8_t percent)
{
    pwm_set_duty_by_channel(LCD_PWM_TIM, LCD_PWM_CHANNEL, percent);
    return 0;
}

int pwm_beep_set_percent(uint8_t percent)
{
    pwm_set_duty_by_channel(BEEP_PWM_TIM, BEEP_PWM_CHANNEL, percent);
    return 0;
}

int pwm_bl_enable(uint8_t enable)
{
    if (enable) {
        TIM_Cmd(LCD_PWM_TIM, ENABLE);
    } else {
        TIM_Cmd(LCD_PWM_TIM, DISABLE);
    }
    return 0;
}

int pwm_beep_enable(uint8_t enable)
{
    if (enable) {
        TIM_Cmd(BEEP_PWM_TIM, ENABLE);
    } else {
        TIM_Cmd(BEEP_PWM_TIM, DISABLE);
    }
    return 0;
}

int pwm_driver_init(void)
{
    pwm_bl_init();
    pwm_beep_init();
    return 0;
}
