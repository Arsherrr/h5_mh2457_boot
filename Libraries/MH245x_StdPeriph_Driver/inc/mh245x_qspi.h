// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_QSPI_H__
#define __PERIPH_QSPI_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __IO uint32_t FCU_CMD;
    __O uint32_t  ADDRESS;
    __IO uint32_t BYTE_NUM;
    __O uint32_t  WR_FIFO;
    __I uint32_t  RD_FIFO;
    __IO uint32_t DEVICE_PARA;
    __IO uint32_t REG_WDATA;
    __O uint32_t  REG_RDATA;
    __IO uint32_t INT_MASK;
    __IO uint32_t INT_UMASK;
    __IO uint32_t INT_MASK_STATUS;
    __IO uint32_t INT_STATUS;
    __IO uint32_t INT_RAWSTATUS;
    __IO uint32_t INT_CLEAR;
    __IO uint32_t CACHE_INTF_CMD;
    __IO uint32_t DMA_CNTL;
    __IO uint32_t FIFO_CNTL;
#if FEATURE_QSPI_SUPPORT_ADDR4
    __I uint32_t  RSVD0044[1];
    __IO uint32_t M7_CODE;
#endif
} QSPI_TypeDef;

/* --------  Feature Definition  -------------------------------------------- */

#ifndef FEATURE_QSPI_ENABLE_CACHE_AES
#define FEATURE_QSPI_ENABLE_CACHE_AES 0
#endif

#ifndef FEATURE_QSPI_SUPPORT_ADDR4
#define FEATURE_QSPI_SUPPORT_ADDR4 0
#endif


/* --------  Register Bit Definition  --------------------------------------- */
#define QSPI_FCU_CMD_CODE        ((uint32_t)0xFF000000U)
#define QSPI_FCU_CMD_BUS_MODE    ((uint32_t)0x00000300U)
#define QSPI_FCU_CMD_CMD_FORMAT  ((uint32_t)0x000000F0U)
#define QSPI_FCU_CMD_DONE        ((uint32_t)0x00000008U)
#define QSPI_FCU_CMD_BUSY        ((uint32_t)0x00000004U)
#define QSPI_FCU_CMD_ACCESS_ACK  ((uint32_t)0x00000002U)
#define QSPI_FCU_CMD_ACCESS_REQ  ((uint32_t)0x00000001U)
#define FCU_CMD_COMMAND_CODE_Pos (24)
#define FCU_CMD_BUS_MODE_Pos     (8)
#define FCU_CMD_CMD_FORMAT_Pos   (4)

#define QSPI_ADDRESS_ADR ((uint32_t)0xFFFFFFFFU)
#define QSPI_ADDRESS_M8  ((uint32_t)0x000000FFU)

#define QSPI_BYTE_NUM_WR_BYTE ((uint32_t)0x00001FFFU)
#define QSPI_BYTE_NUM_RD_BYTE ((uint32_t)0x1FFF0000U)

#define QSPI_WR_FIFO_WR_DATA ((uint32_t)0xFFFFFFFFU)
#define QSPI_RD_FIFO_RD_DATA ((uint32_t)0xFFFFFFFFU)

#define QSPI_DEVICE_PARA_US_COUNT        (0xFFFF0000U)
#define QSPI_DEVICE_PARA_SAMPLE_DLY_Pos  15
#define QSPI_DEVICE_PARA_SAMPLE_DLY      (0x00008000U)
#define QSPI_DEVICE_PARA_SAMPLE_PHA_Pos  14
#define QSPI_DEVICE_PARA_SAMPLE_PHA      (0x00004000U)
#define QSPI_DEVICE_PARA_PROTOCOL_Pos    8
#define QSPI_DEVICE_PARA_PROTOCOL        (0x00000300U)
#define QSPI_DEVICE_PARA_DUMMY_CYCLE_Pos 4
#define QSPI_DEVICE_PARA_DUMMY_CYCLE     (0x000000F0U)

// ------- Divider Note --------
// Divider use A,B : Divisor = FreqSel * A + B
#define QSPI_DEVICE_PARA_FLASH_READY       (0x0800U)
#define QSPI_DEVICE_PARA_FREQ_SEL          (0x000FU)

// Allowed Divisor: [2, 16]
#define QSPI_DEVICE_PARA_FREQ_DIV(divisor) (divisor - 2)

#define QSPI_MAX_DIVISOR 16

#if FEATURE_QSPI_SUPPORT_ADDR4
#define QSPI_FCU_CMD_ADDR_32BIT        BIT10
#define QSPI_DEVICE_PARA_SUB_BASE_ADDR BIT10
#define QSPI_CACHE_INTF_CMD_ADDR_32BIT BIT14
#else
#define QSPI_FCU_CMD_ADDR_32BIT        0U
#define QSPI_DEVICE_PARA_SUB_BASE_ADDR 0U
#define QSPI_CACHE_INTF_CMD_ADDR_32BIT 0U
#endif

#define QSPI_DEVICE_PARA_TIMMING (QSPI_DEVICE_PARA_US_COUNT | QSPI_DEVICE_PARA_SAMPLE_DLY | QSPI_DEVICE_PARA_SAMPLE_PHA | QSPI_DEVICE_PARA_FREQ_SEL)

#define QSPI_REG_WDATA ((uint32_t)0xFFFFFFFFU)
#define QSPI_REG_RDATA ((uint32_t)0xFFFFFFFFU)

#define QSPI_INT_MASK_CVS     ((uint32_t)0x00000100U)
#define QSPI_INT_MASK_PWRE    ((uint32_t)0x00000080U)
#define QSPI_INT_MASK_TFDM    ((uint32_t)0x00000040U)
#define QSPI_INT_MASK_RFDM    ((uint32_t)0x00000020U)
#define QSPI_INT_MASK_TFOM    ((uint32_t)0x00000010U)
#define QSPI_INT_MASK_TFUM    ((uint32_t)0x00000008U)
#define QSPI_INT_MASK_RFOM    ((uint32_t)0x00000004U)
#define QSPI_INT_MASK_RFUM    ((uint32_t)0x00000002U)
#define QSPI_INT_MASK_DONE_IM ((uint32_t)0x00000001U)

#define QSPI_INT_UMASK_CVS     ((uint32_t)0x00000100U)
#define QSPI_INT_UMASK_PWRE    ((uint32_t)0x00000080U)
#define QSPI_INT_UMSAK_TFDU    ((uint32_t)0x00000040U)
#define QSPI_INT_UMSAK_RFDU    ((uint32_t)0x00000020U)
#define QSPI_INT_UMSAK_TFOU    ((uint32_t)0x00000010U)
#define QSPI_INT_UMSAK_TFUU    ((uint32_t)0x00000008U)
#define QSPI_INT_UMSAK_RFOU    ((uint32_t)0x00000004U)
#define QSPI_INT_UMSAK_RFUU    ((uint32_t)0x00000002U)
#define QSPI_INT_UMSAK_DONE_IU ((uint32_t)0x00000001U)

#define QSPI_INT_MASK_STATUS_CVS     ((uint32_t)0x00000100U)
#define QSPI_INT_MASK_STATUS_PWRE    ((uint32_t)0x00000080U)
#define QSPI_INT_MASK_STATUS_TFDM    ((uint32_t)0x00000040U)
#define QSPI_INT_MASK_STATUS_RFDM    ((uint32_t)0x00000020U)
#define QSPI_INT_MASK_STATUS_TFOM    ((uint32_t)0x00000010U)
#define QSPI_INT_MASK_STATUS_TFUM    ((uint32_t)0x00000008U)
#define QSPI_INT_MASK_STATUS_RFOM    ((uint32_t)0x00000004U)
#define QSPI_INT_MASK_STATUS_RFUM    ((uint32_t)0x00000002U)
#define QSPI_INT_MASK_STATUS_DONE_IM ((uint32_t)0x00000001U)

#define QSPI_INT_STATUS_CVS     ((uint32_t)0x00000100U)
#define QSPI_INT_STATUS_PWRE    ((uint32_t)0x00000080U)
#define QSPI_INI_STATUS_TFDS    ((uint32_t)0x00000040U)
#define QSPI_INI_STATUS_RFDS    ((uint32_t)0x00000020U)
#define QSPI_INI_STATUS_TFOS    ((uint32_t)0x00000010U)
#define QSPI_INI_STATUS_TFUS    ((uint32_t)0x00000008U)
#define QSPI_INI_STATUS_RFOS    ((uint32_t)0x00000004U)
#define QSPI_INI_STATUS_RFUS    ((uint32_t)0x00000002U)
#define QSPI_INI_STATUS_DONE_IS ((uint32_t)0x00000001U)

#define QSPI_INT_RAWSTATUSS_CVS    ((uint32_t)0x00000100U)
#define QSPI_INT_RAWSTATUS_PWRE    ((uint32_t)0x00000080U)
#define QSPI_INT_RAWSTATUS_TFDR    ((uint32_t)0x00000040U)
#define QSPI_INT_RAWSTATUS_RFDR    ((uint32_t)0x00000020U)
#define QSPI_INT_RAWSTATUS_TFOR    ((uint32_t)0x00000010U)
#define QSPI_INT_RAWSTATUS_TFUR    ((uint32_t)0x00000008U)
#define QSPI_INT_RAWSTATUS_RFOR    ((uint32_t)0x00000004U)
#define QSPI_INT_RAWSTATUS_RFUR    ((uint32_t)0x00000002U)
#define QSPI_INT_RAWSTATUS_DONE_IR ((uint32_t)0x00000001U)

#define QSPI_INT_CLEAR_CCVS		((uint32_t)0x00000100U)
#define QSPI_INT_CLEAR_CPWRE	((uint32_t)0x00000080U)
#define QSPI_INT_CLEAR_CTFD 	((uint32_t)0x00000040U)
#define QSPI_INT_CLEAR_CRFD 	((uint32_t)0x00000020U)
#define QSPI_INT_CLEAR_CTFO 	((uint32_t)0x00000010U)
#define QSPI_INT_CLEAR_CTFU 	((uint32_t)0x00000008U)
#define QSPI_INT_CLEAR_CRFO 	((uint32_t)0x00000004U)
#define QSPI_INT_CLEAR_CRFU 	((uint32_t)0x00000002U)
#define QSPI_INT_CLEAR_DONE 	((uint32_t)0x00000001U)

#define QSPI_CACHE_INTF_CMD_RELDS       ((uint32_t)0xFF000000U)
#define QSPI_CACHE_INTF_CMD_DS          ((uint32_t)0x00FF0000U)
#define QSPI_CACHE_INTF_CMD_RD_BUS_MODE ((uint32_t)0x00003000U)
#define QSPI_CACHE_INTF_CMD_RD_FORMAT   ((uint32_t)0x00000F00U)
#define QSPI_CACHE_INTF_CMD_RDCMD       ((uint32_t)0x000000FFU)

#define QSPI_DMA_CNTL_TX_EN ((uint32_t)0x00000001U)

#define QSPI_FIFO_CNTL_TFFH ((uint32_t)0x80000000U)
#define QSPI_FIFO_CNTL_TFE  ((uint32_t)0x00200000U)
#define QSPI_FIFO_CNTL_TFFL ((uint32_t)0x00100000U)
#define QSPI_FIFO_CNTL_TFL  ((uint32_t)0x000F0000U)
#define QSPI_FIFO_CNTL_RFFH ((uint32_t)0x00008000U)
#define QSPI_FIFO_CNTL_RFE  ((uint32_t)0x00000020U)
#define QSPI_FIFO_CNTL_RFFL ((uint32_t)0x00000010U)
#define QSPI_FIFO_CNTL_RFL  ((uint32_t)0x0000000FU)


/* --------  Flash Definition  ---------------------------------------------- */
#define QSPI_PAGE_SIZE 0x100

#define QSPI_SUPPORT_CHIP_JEDEC_ID_GD   0xC8
#define QSPI_SUPPORT_CHIP_JEDEC_ID_WD   0xEF
#define QSPI_SUPPORT_CHIP_JEDEC_ID_MXIC 0xC2
#define QSPI_SUPPORT_CHIP_JEDEC_ID_EON  0x1C
#define QSPI_SUPPORT_CHIP_JEDEC_ID_ZBIT 0x5E
#define QSPI_SUPPORT_CHIP_JEDEC_ID_PUYA 0x85
#define QSPI_SUPPORT_CHIP_JEDEC_ID_MICR 0x20
#define QSPI_SUPPORT_CHIP_JEDEC_ID_XMC  0x20

/* --------  Command definition  -------------------------------------------- */
#define RESET_ENABLE_CMD 0x66
#define RESET_MEMORY_CMD 0x99

#define ENTER_QPI_MODE_CMD 0x38
#define EXIT_QPI_MODE_CMD  0xFF

// Identification Operations
#define READ_ID_CMD       0x90
#define DUAL_READ_ID_CMD  0x92
#define QUAD_READ_ID_CMD  0x94
#define READ_JEDEC_ID_CMD 0x9F

// Read Operations
#define READ_CMD                            0x03
#define FAST_READ_CMD                       0x0B
#define DUAL_OUT_FAST_READ_CMD              0x3B
#define DUAL_INOUT_FAST_READ_CMD            0xBB
#define QUAD_OUT_FAST_READ_CMD              0x6B
#define QUAD_INOUT_FAST_READ_CMD            0xEB
#define QUAD_INOUT_FAST_READ_CMD_4BYTE_ADDR 0xEC

// Write Operations
#define WRITE_ENABLE_CMD  0x06
#define WRITE_DISABLE_CMD 0x04

// Register Operations
#define READ_STATUS_REG1_CMD 0x05
#define READ_STATUS_REG2_CMD 0x35
#define READ_STATUS_REG3_CMD 0x15

#define WRITE_STATUS_REG1_CMD 0x01
#define WRITE_STATUS_REG2_CMD 0x31
#define WRITE_STATUS_REG3_CMD 0x11

// Program Operations
#define PAGE_PROG_CMD                       0x02
#define QUAD_INPUT_PAGE_PROG_CMD            0x32
#define QUAD_INPUT_PAGE_PROG_CMD_4BYTE_ADDR 0x34

// Erase Operations
#define SECTOR_ERASE_CMD    0x20
#define SECTOR_ERASE4_CMD   0x21
#define BLOCK_32K_ERASE_CMD 0x52
#define BLOCK_64K_ERASE_CMD 0xD8
#define CHIP_ERASE_CMD      0xC7

#define PROG_ERASE_RESUME_CMD  0x7A
#define PROG_ERASE_SUSPEND_CMD 0x75

#define SET_BURST_WITH_WRAP          0x77
#define RELEASE_FROM_DEEP_POWER_DOWN 0xAB
#define DEEP_POWER_DOWN              0xB9

// 4-Byte Address Mode Operations
#define ENTER_4BYTES_ADDRESS_MODE 0xB7
#define EXIT_4BYTES_ADDRESS_MODE  0xE9

/* --------  QSPI Interrupt Definitions  ------------------------------------ */
#define QSPI_IT_CODE_VERIFY_SUCCESS QSPI_INT_MASK_STATUS_CVS
#define QSPI_IT_PCROP_WR_ERROR 		QSPI_INT_MASK_STATUS_PWRE
#define QSPI_IT_TX_FIFO_DATA QSPI_INT_MASK_STATUS_TFDM
#define QSPI_IT_RX_FIFO_DATA QSPI_INT_MASK_STATUS_RFDM
#define QSPI_IT_TX_FIFO_OF   QSPI_INT_MASK_STATUS_TFOM
#define QSPI_IT_TX_FIFO_UF   QSPI_INT_MASK_STATUS_TFUM
#define QSPI_IT_RX_FIFO_OF   QSPI_INT_MASK_STATUS_RFOM
#define QSPI_IT_RX_FIFO_UF   QSPI_INT_MASK_STATUS_RFUM
#define QSPI_IT_DONE_INT     QSPI_INT_MASK_STATUS_DONE_IM
#define QSPI_IT_ALL \
    (QSPI_IT_TX_FIFO_DATA | QSPI_IT_RX_FIFO_DATA | QSPI_IT_TX_FIFO_OF | QSPI_IT_TX_FIFO_UF | QSPI_IT_RX_FIFO_OF | QSPI_IT_RX_FIFO_UF | QSPI_IT_DONE_INT)

/* --------  Export Type ---------------------------------------------------- */

typedef enum
{
    QSPI_BUSMODE_111 = 0x00, //CMD-ADDR-DATA = 1-1-1
    QSPI_BUSMODE_114 = 0x01, //CMD-ADDR-DATA = 1-1-4
    QSPI_BUSMODE_144 = 0x02, //CMD-ADDR-DATA = 1-4-4
    QSPI_BUSMODE_444 = 0x03, //CMD-ADDR-DATA = 4-4-4
} QSPI_BusModeTypeDef;

typedef enum
{
    QSPI_CMDFORMAT_CMD8                    = 0x00,
    QSPI_CMDFORMAT_CMD8_RREG8              = 0x01,
    QSPI_CMDFORMAT_CMD8_RREG16             = 0x02,
    QSPI_CMDFORMAT_CMD8_RREG24             = 0x03,
    QSPI_CMDFORMAT_CMD8_DMY24_WREG8        = 0x04,
    QSPI_CMDFORMAT_CMD8_ADDR24_RREG8       = 0x05,
    QSPI_CMDFORMAT_CMD8_ADDR24_RREG16      = 0x06,
    QSPI_CMDFORMAT_CMD8_WREG8              = 0x07,
    QSPI_CMDFORMAT_CMD8_WREG16             = 0x08,
    QSPI_CMDFORMAT_CMD8_ADDR24             = 0x09,
    QSPI_CMDFORMAT_CMD8_ADDR24_RDAT        = 0x0A,
    QSPI_CMDFORMAT_CMD8_ADDR24_DMY_RDAT    = 0x0B,
    QSPI_CMDFORMAT_CMD8_ADDR24_M8_DMY_RDAT = 0x0C,
    QSPI_CMDFORMAT_CMD8_ADDR24_PDAT        = 0x0D
} QSPI_CmdFormatTypeDef;

typedef enum
{
    QSPI_PROTOCOL_CLPL = 0x00,
    QSPI_PROTOCOL_CHPH = 0x03
} QSPI_ProtocolTypedef;

typedef enum
{
    QSPI_STATUS_OK               = (0x00),
    QSPI_STATUS_ERROR            = (0x01),
    QSPI_STATUS_BUSY             = (0x02),
    QSPI_STATUS_NOT_SUPPORTED    = (0x04),
    QSPI_STATUS_SUSPENDED        = (0x08),
    QSPI_STATUS_CRYPT_FAIL       = (0x0A),
    QSPI_STATUS_ADDR_NOT_ALIGNED = (0x0C),
} QSPI_StatusTypeDef;

typedef struct
{
    // Device Para
    uint8_t SampleDly;   // Default:0
    uint8_t SamplePha;   // Default:0
    uint8_t Protocol;    // Defualt: QSPI_PROTOCOL_CLPL
    uint8_t DummyCycles; // Include M7:0  Defualt: 6
    uint8_t FreqSel;     // Defualt: QSPI_FREQSEL_HCLK_DIV4

    // Setting Cache
    uint8_t Cache_Cmd_ReleaseDeepInstruction; // Defualt: 0xAB
    uint8_t Cache_Cmd_DeepInstruction;        // Defualt: 0xB9
    uint8_t Cache_Cmd_ReadBusMode;            // Defualt: QSPI_BUSMODE_144
    uint8_t Cache_Cmd_ReadFormat;             // Defualt: QSPI_CMDFORMAT_CMD8_ADDR24_DMY_RDAT
    uint8_t Cache_Cmd_ReadInstruction;        // Defualt: 0xEB
} QSPI_InitTypeDef;

typedef struct
{
    uint8_t               Instruction;
    QSPI_BusModeTypeDef   BusMode;
    QSPI_CmdFormatTypeDef CmdFormat;
} QSPI_CommandTypeDef;




#define ROM_QSPI_ProgramCrypt             (*((uint8_t (*)(uint8_t *, uint32_t, uint32_t))(*(uint32_t *)0x001C)))	
#define ROM_QSPI_EraseSector              (*((uint8_t (*)(QSPI_CommandTypeDef *, uint32_t))(*(uint32_t *)0x0024)))
#define ROM_QSPI_ProgramPage              (*((uint8_t (*)(QSPI_CommandTypeDef *, DMA_Stream_TypeDef *, uint32_t, uint32_t, uint8_t *))(*(uint32_t *)0x0028)))
#define ROM_QSPI_ReadPage                 (*((uint8_t (*)(QSPI_CommandTypeDef *, DMA_Stream_TypeDef*, uint8_t *, uint32_t, uint32_t))(*(uint32_t *)0x0034)))
#define ROM_QSPI_BlockErase64K            (*((uint8_t (*)(QSPI_CommandTypeDef *, uint32_t))(*(uint32_t *)0x0038)))	

uint8_t FLASH_EraseSector(uint32_t sectorAddress);
uint8_t FLASH_ProgramPage(QSPI_CommandTypeDef *cmdParam, DMA_Stream_TypeDef *DMA_Channelx, uint32_t addr, uint32_t size, uint8_t *buffer);
uint8_t FLASH_ProgramCrypt(uint32_t addr,uint32_t size,uint8_t *buffer);
uint8_t FLASH_ReadPage(QSPI_CommandTypeDef *cmdParam,DMA_Stream_TypeDef* DMAy_Streamx, uint8_t* buf, uint32_t addr, uint32_t sz);
uint8_t FLASH_BlockErase64K(uint32_t SectorAddress);


uint32_t QSPI_ReadID(QSPI_CommandTypeDef* cmdParam);
void QSPI_Init(QSPI_InitTypeDef* mhqspi);
void QSPI_SetLatency(uint32_t u32UsClk);

#ifdef __cplusplus
}
#endif

#endif
