#include "io_config.h"
#include "key.h"

static const GPIO_TypeDef* key_port[] = {
    KEY1_GPIO_PORT, 
    KEY2_GPIO_PORT, 
    KEY3_GPIO_PORT, 
    KEY4_GPIO_PORT
};
static const u16 key_pin[] = {
    KEY1_GPIO_PIN, 
    KEY2_GPIO_PIN, 
    KEY3_GPIO_PIN, 
    KEY4_GPIO_PIN
};
static const u8 key_num = sizeof(key_port) / sizeof(key_port[0]);

/**
 * @brief  按键 IO 初始化.
 * @param  None
 * @retval 0 成功, 其他 失败.
 */
int key_init(void)
{
    PeripheralEnable(PeripheralGPIOE, true);
    PeripheralEnable(PeripheralGPIOH, true);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    for (u8 i = 0; i < key_num; i++) {
        GPIO_InitStructure.GPIO_Pin = key_pin[i];
        GPIO_Init((GPIO_TypeDef*)key_port[i], &GPIO_InitStructure);
    }

    return 0;
}

/**
 * @brief 获取按键状态.
 * @param  key 按键编号 [0, 3]
 *     @arg 0: key1
 *     @arg 1: key2
 *     @arg 2: key3
 *     @arg 3: key4
 * @retval 按键状态 [0, 1]
 *         0: 释放
 *         1: 按下
 */
u8 get_key_state(u8 key)
{
    return (GPIO_ReadInputDataBit((GPIO_TypeDef*)key_port[key], 
                                  key_pin[key]) == Bit_RESET);
}
