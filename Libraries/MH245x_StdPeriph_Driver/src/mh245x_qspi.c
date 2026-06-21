#include "mh245x_qspi.h"
#include "mh245x_dma.h"

typedef struct
{
    uint8_t               Instruction;
    QSPI_BusModeTypeDef   BusMode;
    QSPI_CmdFormatTypeDef CmdFormat;
    uint32_t              Address;
    uint32_t WrData;
    uint32_t RdData;
} MH_CommandTypeDef;

#define MH_QSPI_TIMEOUT_DEFAULT_CNT		(50000)
void QSPI_Init(QSPI_InitTypeDef* mhqspi)
{
    if (mhqspi == NULL)
    {
        QSPI->DEVICE_PARA = ((QSPI->DEVICE_PARA & ~0xCCFF) | 0x86E) | QSPI_DEVICE_PARA_SAMPLE_PHA | QSPI_DEVICE_PARA_SUB_BASE_ADDR;
    }
    else
    {
        QSPI->DEVICE_PARA = (uint32_t)(                                          //
            (QSPI->DEVICE_PARA & ~0xFFFF) |                                      //
            ((mhqspi->SampleDly & 0x01) << QSPI_DEVICE_PARA_SAMPLE_DLY_Pos) |    //
            ((mhqspi->SamplePha & 0x01) << QSPI_DEVICE_PARA_SAMPLE_PHA_Pos) |    //
            ((mhqspi->Protocol & 0x03) << QSPI_DEVICE_PARA_PROTOCOL_Pos) |       //
            ((mhqspi->DummyCycles & 0x0F) << QSPI_DEVICE_PARA_DUMMY_CYCLE_Pos) | //
            QSPI_DEVICE_PARA_FLASH_READY |                                       //
            QSPI_DEVICE_PARA_SUB_BASE_ADDR |                                     //
            (mhqspi->FreqSel & QSPI_DEVICE_PARA_FREQ_SEL)                        //
        );
    }
}

void QSPI_SetLatency(uint32_t u32UsClk)
{
	RCC_ClocksTypeDef clocks;
	
	RCC_GetClocksFreq(&clocks);	
	
    if (0 == u32UsClk)
    {
        QSPI->DEVICE_PARA = (QSPI->DEVICE_PARA & ~0xFFFF0000) | ((clocks.SYSCLK_Frequency / 1000000) << 16);
    }
    else
    {
        QSPI->DEVICE_PARA = (QSPI->DEVICE_PARA & ~0xFFFF0000) | (u32UsClk << 16);
    }
}

MOVE_TO_RAM
uint8_t FLASH_EraseSector(uint32_t sectorAddress)
{
    uint8_t ret;
    __disable_irq();
	__disable_fault_irq();	
    ret = ROM_QSPI_EraseSector(NULL, sectorAddress);
	__enable_fault_irq();
	__enable_irq();
    return ret;
}

MOVE_TO_RAM
uint8_t FLASH_ProgramPage(QSPI_CommandTypeDef *cmdParam, DMA_Stream_TypeDef *DMA_Channelx, uint32_t addr, uint32_t size, uint8_t *buffer)
{
    uint8_t ret;

	__disable_irq();
	__disable_fault_irq();	
    ret = ROM_QSPI_ProgramPage(cmdParam, DMA_Channelx, addr, size, buffer);
	__enable_fault_irq();
	__enable_irq();
	
	return ret;   
}

MOVE_TO_RAM
uint8_t FLASH_ProgramCrypt(uint32_t addr,uint32_t size,uint8_t *buffer)
{
    uint8_t ret;
	*(uint32_t *)(0x40023834) |= BIT7; 
	__disable_irq();
	__disable_fault_irq();	
    ret = ROM_QSPI_ProgramCrypt(buffer, addr, size);
	__enable_fault_irq();
	__enable_irq();
	*(uint32_t *)(0x40023834) &= ~BIT7; 
	return ret;   
}

MOVE_TO_RAM
uint8_t FLASH_ReadPage(QSPI_CommandTypeDef *cmdParam, DMA_Stream_TypeDef* DMAy_Streamx, uint8_t* buf, uint32_t addr, uint32_t sz)
{
	uint8_t ret;

	__disable_irq();
	__disable_fault_irq();	
    ret = ROM_QSPI_ReadPage(cmdParam, DMAy_Streamx, buf, addr, sz);
	__enable_fault_irq();
	__enable_irq();
	
    return ret;   
	
}

uint8_t FLASH_BlockErase64K(uint32_t SectorAddress)
{
	 uint8_t ret;
    __disable_irq();
	__disable_fault_irq();	
    ret = ROM_QSPI_BlockErase64K(NULL, SectorAddress);
    __enable_fault_irq();
	__enable_irq();
	
    return ret;
}

#define MH_QSPI_ACCESS_REQ_ENABLE  (0x00000001U)

// Excute command and wait until timeout
static inline __attribute__((section("RAM_CODE"), __used__))  QSPI_StatusTypeDef ExcuteCommand(MH_CommandTypeDef* cmd, int32_t timeout)
{
    QSPI->REG_WDATA = cmd->WrData;
#if FEATURE_QSPI_SUPPORT_ADDR4
    QSPI->ADDRESS = cmd->Address;
#else
    QSPI->ADDRESS = cmd->Address << 8;
#endif

    QSPI->FCU_CMD = (QSPI->FCU_CMD & ~(QSPI_FCU_CMD_CODE | QSPI_FCU_CMD_BUS_MODE | QSPI_FCU_CMD_CMD_FORMAT | QSPI_FCU_CMD_ACCESS_REQ)) |
                    ((uint32_t)(cmd->Instruction << 24)) | ((uint32_t)(cmd->BusMode << 8)) | ((uint32_t)(cmd->CmdFormat << 4)) | (MH_QSPI_ACCESS_REQ_ENABLE);

    /* Wait For command done */
    for (uint32_t i = 0; i < timeout; i += 4)
    {
        if (QSPI->INT_RAWSTATUS & BIT0)
        {
            QSPI->INT_CLEAR = 0x01;

            cmd->RdData = QSPI->REG_RDATA;
            return QSPI_STATUS_OK;
        }
    }

    return QSPI_STATUS_ERROR;
}

static inline void __attribute__((section("RAM_CODE"), __used__)) CacheWaitFree(void)
{
    while (ICACHE->CACHE_CS & CACHE_IS_BUSY) {}
    while (DCACHE->CACHE_CS & CACHE_IS_BUSY) {}
}

uint32_t __attribute__((section("RAM_CODE"), __used__)) QSPI_ReadID(QSPI_CommandTypeDef* cmdParam)
{
    MH_CommandTypeDef sCommand;

    CacheWaitFree();

    if (cmdParam == NULL)
    {
        sCommand.Instruction = READ_JEDEC_ID_CMD;
        sCommand.BusMode     = QSPI_BUSMODE_111;
        sCommand.CmdFormat   = QSPI_CMDFORMAT_CMD8_RREG24; //0x03U;
    }
    else
    {
        sCommand.Instruction = cmdParam->Instruction; //READ_JEDEC_ID_CMD;
        sCommand.BusMode     = cmdParam->BusMode;     //busMode;
        sCommand.CmdFormat   = cmdParam->CmdFormat;   //0x03U;
    }

    if (ExcuteCommand(&sCommand, MH_QSPI_TIMEOUT_DEFAULT_CNT) != QSPI_STATUS_OK)
    {
        return QSPI_STATUS_ERROR;
    }

    return (sCommand.RdData & 0x00FFFFFF);
}
