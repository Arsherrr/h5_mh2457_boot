#ifndef __KEY_H
#define __KEY_H

#define KEY1  (0)
#define KEY2  (1)
#define KEY3  (2)
#define KEY4  (3)

/**
 * @brief  按键 IO 初始化.
 * @param  None
 * @retval 0 成功, 其他 失败.
 */
int key_init(void);

/**
 * @brief 获取按键状态.
 * @param  key 按键编号 [0, 3]
 *     @arg 0: KEY1
 *     @arg 1: KEY2
 *     @arg 2: KEY3
 *     @arg 3: KEY4
 * @retval 按键状态 [0, 1]
 *         0: 释放
 *         1: 按下
 */
u8 get_key_state(u8 key);

#endif
