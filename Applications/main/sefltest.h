#ifndef SEFLTEST_H_
#define SEFLTEST_H_

#include <stdint.h>

/** APP 快速 CRC 校验, 1=通过 0=失败 */
uint8_t check_app_fast_crc_hw(void);

/** APP 完整性 (空区/名称/CRC), 1=完好 0=损坏 */
uint8_t check_app_integrity(void);

/** LOGO + TEXT + IMG 资源头校验, 1=完好 0=损坏 */
uint8_t check_res_integrity(void);

#endif /* SEFLTEST_H_ */
