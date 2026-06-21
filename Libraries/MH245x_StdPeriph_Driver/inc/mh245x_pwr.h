// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_PWR_H__
#define __PERIPH_PWR_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t CR;           /*!< PWR power control register,        Address offset: 0x00 */
    __IO uint32_t CSR;          /*!< PWR power control/status register, Address offset: 0x04 */
    __IO uint32_t RSVD;         /*!< PWR Reserved register, Address offset: 0x08 */
    __IO uint32_t CR1;          /*!< PWR power control/status register, Address offset: 0x0C */
} PWR_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/********************  Bit definition for PWR_CR register  ********************/
#define PWR_CR_MODE             ((uint32_t)0x00000003) /*!< PDDS[1:0] bits */
#define PWR_CR_MODE_STOP        ((uint32_t)0x00000000) /*!< Stop Mode */
#define PWR_CR_MODE_STANDBY     ((uint32_t)0x00000001) /*!< Standby Mode */

#define PWR_CR_CSBF           ((uint32_t)0x00000008) /*!< Clear Standby Flag */
#define PWR_CR_PVDE           ((uint32_t)0x00000010) /*!< Power Voltage Detector Enable */

#define PWR_CR_PLS   ((uint32_t)0x000000E0) /*!< PLS[2:0] bits (PVD Level Selection) */
#define PWR_CR_PLS_0 ((uint32_t)0x00000020) /*!< Bit 0 */
#define PWR_CR_PLS_1 ((uint32_t)0x00000040) /*!< Bit 1 */
#define PWR_CR_PLS_2 ((uint32_t)0x00000080) /*!< Bit 2 */
/*!< PVD level configuration */
#define PWR_CR_PLS_LEV0 ((uint32_t)0x00000000) /*!< PVD level 0 */
#define PWR_CR_PLS_LEV1 ((uint32_t)0x00000020) /*!< PVD level 1 */
#define PWR_CR_PLS_LEV2 ((uint32_t)0x00000040) /*!< PVD level 2 */
#define PWR_CR_PLS_LEV3 ((uint32_t)0x00000060) /*!< PVD level 3 */
#define PWR_CR_PLS_LEV4 ((uint32_t)0x00000080) /*!< PVD level 4 */
#define PWR_CR_PLS_LEV5 ((uint32_t)0x000000A0) /*!< PVD level 5 */
#define PWR_CR_PLS_LEV6 ((uint32_t)0x000000C0) /*!< PVD level 6 */
#define PWR_CR_PLS_LEV7 ((uint32_t)0x000000E0) /*!< PVD level 7 */

#define PWR_CR_DBP   ((uint32_t)0x00000100) /*!< Disable Backup Domain write protection                     */
#define PWR_CR_LPDS  ((uint32_t)0x00000400) /*!< Low-Power Regulator in Stop under-drive mode               */

#define PWR_CR_RTMEM              ((uint32_t)0x00002000)   /*!< Retain memorys */
#define PWR_CR_PDROM              ((uint32_t)0x00008000)   /*!< PD ROM in Stop Mode */
#define PWR_CR_CCM_STATE          ((uint32_t)0x00030000)   /*!< PD_CCM[1:0] bits */
#define PWR_CR_CCM_STATE_ON       ((uint32_t)0x00030000)   /*!< CCM Power on in Stop Mode */
#define PWR_CR_CCM_STATE_RT       ((uint32_t)0x00010000)   /*!< CCM Power retain in Stop Mode  */

#define PWR_CR_SRAM5_STATE        ((uint32_t)0x000C0000)   /*!< PD_SRAM5[1:0] bits */
#define PWR_CR_SRAM5_STATE_ON     ((uint32_t)0x000C0000)   /*!< SRAM5 Power on in Stop Mode */
#define PWR_CR_SRAM5_STATE_RT     ((uint32_t)0x00040000)   /*!< SRAM5 Power retain in Stop Mode  */
#define PWR_CR_SRAM5_STATE_PD     ((uint32_t)0x00080000)   /*!< SRAM5 Power down in Stop Mode  */

#define PWR_CR_SRAM4_STATE        ((uint32_t)0x00300000)   /*!< PD_SRAM4[1:0] bits */
#define PWR_CR_SRAM4_STATE_ON     ((uint32_t)0x00300000)   /*!< SRAM4 Power on in Stop Mode */
#define PWR_CR_SRAM4_STATE_RT     ((uint32_t)0x00100000)   /*!< SRAM4 Power retain in Stop Mode  */
#define PWR_CR_SRAM4_STATE_PD     ((uint32_t)0x00200000)   /*!< SRAM4 Power down in Stop Mode  */

#define PWR_CR_SRAM3_STATE        ((uint32_t)0x00C00000)   /*!< PD_SRAM3[1:0] bits */
#define PWR_CR_SRAM3_STATE_ON     ((uint32_t)0x00C00000)   /*!< SRAM3 Power on in Stop Mode */
#define PWR_CR_SRAM3_STATE_RT     ((uint32_t)0x00400000)   /*!< SRAM3 Power retain in Stop Mode  */
#define PWR_CR_SRAM3_STATE_PD     ((uint32_t)0x00800000)   /*!< SRAM3 Power down in Stop Mode  */

#define PWR_CR_SRAM2_STATE        ((uint32_t)0x03000000)   /*!< PD_SRAM2[1:0] bits */
#define PWR_CR_SRAM2_STATE_ON     ((uint32_t)0x03000000)   /*!< SRAM2 Power on in Stop Mode */
#define PWR_CR_SRAM2_STATE_RT     ((uint32_t)0x01000000)   /*!< SRAM2 Power retain in Stop Mode  */

#define PWR_CR_SRAM1_STATE        ((uint32_t)0x0C000000)   /*!< PD_SRAM1[1:0] bits */
#define PWR_CR_SRAM1_STATE_ON     ((uint32_t)0x0C000000)   /*!< SRAM1 Power on in Stop Mode */
#define PWR_CR_SRAM1_STATE_RT     ((uint32_t)0x04000000)   /*!< SRAM1 Power retain in Stop Mode  */
#define PWR_CR_SRAM1_STATE_PD     ((uint32_t)0x08000000)   /*!< SRAM1 Power down in Stop Mode  */

#define PWR_CR_PDDMA2D            ((uint32_t)0x10000000)   /*!< PD DMA2D in Stop Mode */
#define PWR_CR_PDOTP              ((uint32_t)0x80000000)   /*!< PD OTP in Stop Mode */

/* Legacy define */
#define PWR_CR_PMODE PWR_CR_VOS

/*******************  Bit definition for PWR_CSR register  ********************/
#define PWR_CSR_WUF             BIT0  /*!< Wakeup Flag                                       */
#define PWR_CSR_SBF             BIT1  /*!< Standby Flag                                      */
#define PWR_CSR_PVDO            BIT2  /*!< PVD Output                                        */
#define PWR_CSR_EWUP_PA7        BIT4  /*!< Enable WKUP pin PA7                               */
#define PWR_CSR_EWUP_PD11       BIT5  /*!< Enable WKUP pin PD11                              */
#define PWR_CSR_EWUP_PA14       BIT6  /*!< Enable WKUP pin PA14                              */
#define PWR_CSR_EWUP_PE13       BIT7  /*!< Enable WKUP pin PE13                              */
#define PWR_CSR_EWUP_PA0        BIT8  /*!< Enable WKUP pin PA0                               */
#define PWR_CSR_PIWDG_STOP      BIT17 /*!< IWDG Pause In Stop Mode                           */
#define PWR_CSR_PIWDG_STANDBY   BIT18 /*!< IWDG Pause In Standby Mode                        */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup PWR_Exported_Constants
 * @{
 */

/** @defgroup PWR_PVD_detection_level
 * @{
 */
#define PWR_PVDLevel_0 PWR_CR_PLS_LEV0
#define PWR_PVDLevel_1 PWR_CR_PLS_LEV1
#define PWR_PVDLevel_2 PWR_CR_PLS_LEV2
#define PWR_PVDLevel_3 PWR_CR_PLS_LEV3
#define PWR_PVDLevel_4 PWR_CR_PLS_LEV4
#define PWR_PVDLevel_5 PWR_CR_PLS_LEV5
#define PWR_PVDLevel_6 PWR_CR_PLS_LEV6
#define PWR_PVDLevel_7 PWR_CR_PLS_LEV7

#define IS_PWR_PVD_LEVEL(LEVEL)                                                                                                  \
    (((LEVEL) == PWR_PVDLevel_0) || ((LEVEL) == PWR_PVDLevel_1) || ((LEVEL) == PWR_PVDLevel_2) || ((LEVEL) == PWR_PVDLevel_3) || \
     ((LEVEL) == PWR_PVDLevel_4) || ((LEVEL) == PWR_PVDLevel_5) || ((LEVEL) == PWR_PVDLevel_6) || ((LEVEL) == PWR_PVDLevel_7))
/**
 * @}
 */

/** @defgroup PWR_Regulator_state_in_STOP_mode
 * @{
 */
#define PWR_MainRegulator_ON       ((uint32_t)0x00000000)
#define PWR_LowPowerRegulator_ON   PWR_CR_LPDS

/* --- PWR_Legacy ---*/
#define PWR_Regulator_ON         PWR_MainRegulator_ON
#define PWR_Regulator_LowPower   PWR_LowPowerRegulator_ON

#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_MainRegulator_ON) || ((REGULATOR) == PWR_LowPowerRegulator_ON))

/**
 * @}
 */

/** @defgroup PWR_STOP_mode_entry
 * @{
 */
#define PWR_STOPEntry_WFI        ((uint8_t)0x01)
#define PWR_STOPEntry_WFE        ((uint8_t)0x02)
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPEntry_WFI) || ((ENTRY) == PWR_STOPEntry_WFE))
/**
 * @}
 */

/** @defgroup PWR_Regulator_Voltage_Scale
 * @{
 */
#define PWR_Regulator_Voltage_Scale1 ((uint32_t)0x0000C000)
#define PWR_Regulator_Voltage_Scale2 ((uint32_t)0x00008000)
#define PWR_Regulator_Voltage_Scale3 ((uint32_t)0x00004000)
#define IS_PWR_REGULATOR_VOLTAGE(VOLTAGE) \
    (((VOLTAGE) == PWR_Regulator_Voltage_Scale1) || ((VOLTAGE) == PWR_Regulator_Voltage_Scale2) || ((VOLTAGE) == PWR_Regulator_Voltage_Scale3))
/**
 * @}
 */

/** @defgroup PWR_Flag
 * @{
 */
#define PWR_FLAG_WU   PWR_CSR_WUF
#define PWR_FLAG_SB   PWR_CSR_SBF
#define PWR_FLAG_PVDO PWR_CSR_PVDO

/* --- FLAG Legacy ---*/
#define PWR_FLAG_REGRDY PWR_FLAG_VOSRDY

#define IS_PWR_GET_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB) || ((FLAG) == PWR_FLAG_PVDO))

#define IS_PWR_CLEAR_FLAG(FLAG) IS_PWR_GET_FLAG(FLAG)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Function used to set the PWR configuration to the default reset state ******/
void PWR_DeInit(void);

/* Backup Domain Access function **********************************************/
void PWR_BackupAccessCmd(FunctionalState NewState);

/* PVD configuration functions ************************************************/
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel);
void PWR_PVDCmd(FunctionalState NewState);

/* WakeUp pins configuration functions ****************************************/
void PWR_WakeUpPinCmd(FunctionalState NewState);
/* Main and Backup Regulators configuration functions *************************/
void PWR_BackupRegulatorCmd(FunctionalState NewState);
void PWR_MainRegulatorModeConfig(uint32_t PWR_Regulator_Voltage);
void PWR_OverDriveCmd(FunctionalState NewState);
void PWR_OverDriveSWCmd(FunctionalState NewState);
void PWR_UnderDriveCmd(FunctionalState NewState);

///* FLASH Power Down configuration functions ***********************************/
//void PWR_FlashPowerDownCmd(FunctionalState NewState);

/* Low Power modes configuration functions ************************************/
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterUnderDriveSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterSTANDBYMode(void);

/* Flags management functions *************************************************/
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void       PWR_ClearFlag(uint32_t PWR_FLAG);

extern void PWR_EnterPowerDownMode(void);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_PWR_H__
