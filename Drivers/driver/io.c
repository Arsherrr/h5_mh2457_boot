#include "io.h"

/**
 * @brief 初始化 IO 口.
 * @param  None
 * @retval None
 */
void io_init(void)
{
    PeripheralEnable(IO_PERIPH, true);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    /* GPS ON. */
    GPIO_InitStructure.GPIO_Pin = GPS_ON_PIN;
    GPIO_Init(GPS_ON_GPIO, &GPIO_InitStructure);

    /* GPS EN. */
    GPIO_InitStructure.GPIO_Pin = GPS_EN_PIN;
    GPIO_Init(GPS_EN_GPIO, &GPIO_InitStructure);

    /* MCU EN. */
    GPIO_InitStructure.GPIO_Pin = MCU_EN_PIN;
    GPIO_Init(MCU_EN_GPIO, &GPIO_InitStructure);

    /* PWR CTR. */
    GPIO_InitStructure.GPIO_Pin = PWR_CTR_PIN;
    GPIO_Init(PWR_CTR_GPIO, &GPIO_InitStructure);

    /* PR CTR. */
    GPIO_InitStructure.GPIO_Pin = PR_CTR_PIN;
    GPIO_Init(PR_CTR_GPIO, &GPIO_InitStructure);
    
    GPS_ON_SET(0);
    GPS_EN_SET(0);
    MCU_EN_SET(0); /* H OFF, L ON. */
    PWR_CTR_SET(1);
}
