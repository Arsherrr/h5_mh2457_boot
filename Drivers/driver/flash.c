#include "flash.h"

#if 1
static u8 is_need_erase(u8* buf_old, u8* buf_new, u16 len)
{
    u16 i;
    u8 old;
    for (i = 0; i < len; i++)
    {
        old = *buf_old++;
        old = ~old;
        if ((old & (*buf_new++)) != 0)
        {
            return 1;
        }
    }
    return 0;
}

static u8 flash_sector_buf(u8* src, u32 addr, u16 len)
{
    u16 i;
    u16 j; /* 用于延时 */
    u32 sector_addr; /* 扇区首址 */
    u8 erase; /* 1表示需要擦除 */
    u8 ret;
    QSPI_CommandTypeDef cmdType;
    u8 buf[FLASH_SECTOR_SIZE];

    if (len == 0)
    {
        return 1;
    }

    if ((addr+len) > (FLASH_SECTOR_SIZE+FLASH_SIZE))
    {
        return 0;
    }

    if (len > FLASH_SECTOR_SIZE)
    {
        return 0;
    }

    if (memcmp((void *)addr, (void *)src, len) == 0)
    {
        return 1;
    }

    /* 判断是否需要先擦除扇区 */
    /* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
    erase = 0;
    memcpy((void *)buf, (void *)addr, len);
    if (is_need_erase(buf, src, len))
    {
        erase = 1;
    }

    sector_addr = addr & (~(FLASH_SECTOR_SIZE - 1));

    if (len == FLASH_SECTOR_SIZE) /* 整个扇区都改写 */
    {
        memcpy((void *)buf, (void *)src, len);
    }
    else //需要擦除就要把整个扇区先读出来
    {
        /* 先将整个扇区的数据读出 */
        memcpy((void *)buf, (void *)sector_addr, FLASH_SECTOR_SIZE);
        /* 再用新数据覆盖 */
        i = addr & (FLASH_SECTOR_SIZE - 1);
        memcpy(&buf[i], src, len);
    }
    /* 写完之后进行校验，如果不正确则重写，最多3次 */
    ret = 0;
	cmdType.Instruction = QUAD_INPUT_PAGE_PROG_CMD;
	cmdType.BusMode = QSPI_BUSMODE_114;
	cmdType.CmdFormat = QSPI_CMDFORMAT_CMD8_ADDR24_PDAT;
    for (i = 0; i < 3; i++)
    {
        /* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
        if (erase == 1)
        {
            FLASH_EraseSector(sector_addr); /* 擦除1个扇区 */
        }

        for (j = 0; j < FLASH_SECTOR_SIZE; j += QSPI_PAGE_SIZE)
        {
            FLASH_ProgramPage(&cmdType, NULL, sector_addr+j, QSPI_PAGE_SIZE, (uint8_t*)&buf[j]);
        }

        if (!memcmp((void*)sector_addr, (void*)buf, FLASH_SECTOR_SIZE))
        {
            ret = 1;
            break;
        }
        else
        {
            erase = 1; //重写需要擦除

            /* 失败后延迟一段时间再重试 */
            for (j = 0; j < 20000; j++)
            {
                ;
            }
        }
    }

    return ret;
}

/*
*********************************************************************************************************
*   函 数 名: flash_write_buf
*   功能说明: 写1个扇区并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
               如果无关的数据不是必须保留的，最好在写满的时候调用擦除函数主动擦除，
               否则在写满以后每次写数据都会触发擦除
*   形    参:   buf : 数据源缓冲区；
*               addr ：目标区域首地址
*               len ：数据个数
*   返 回 值: 1 : 成功， 0 ： 失败
*********************************************************************************************************
*/
u8 flash_write_buf(u8 *buf, u32 addr, u16 len)
{
    u16 page_cnt = 0, len_left = 0, addr_tmp = 0, count = 0, temp = 0;
    addr_tmp = addr % FLASH_SECTOR_SIZE;
    count = FLASH_SECTOR_SIZE - addr_tmp;
    page_cnt = len / FLASH_SECTOR_SIZE;
    len_left = len % FLASH_SECTOR_SIZE;
    if (addr_tmp == 0) /* 起始地址是页面首地址  */
    {
        if (page_cnt == 0) /* 数据长度小于页面大小 */
        {
            if (flash_sector_buf(buf, addr, len) == 0)
            {
                return 0;
            }
        }
        else /* 数据长度大于等于页面大小 */
        {
            while (page_cnt--)
            {
                if (flash_sector_buf(buf, addr, FLASH_SECTOR_SIZE) == 0)
                {
                    return 0;
                }
                addr += FLASH_SECTOR_SIZE;
                buf += FLASH_SECTOR_SIZE;
            }
            if (flash_sector_buf(buf, addr, len_left) == 0)
            {
                return 0;
            }
        }
    }
    else /* 起始地址不是页面首地址  */
    {
        if (page_cnt == 0) /* 数据长度小于页面大小 */
        {
            if (len_left > count) /* (len + addr) > SPI_FLASH_PAGESIZE */
            {
                temp = len_left - count;
                if (flash_sector_buf(buf, addr, count) == 0)
                {
                    return 0;
                }
                addr += count;
                buf += count;
                if (flash_sector_buf(buf, addr, temp) == 0)
                {
                    return 0;
                }
            }
            else
            {
                if (flash_sector_buf(buf, addr, len) == 0)
                {
                    return 0;
                }
            }
        }
        else /* 数据长度大于等于页面大小 */
        {
            len -= count;
            page_cnt = len / FLASH_SECTOR_SIZE;
            len_left = len % FLASH_SECTOR_SIZE;
            if (flash_sector_buf(buf, addr, count) == 0)
            {
                return 0;
            }
            addr += count;
            buf += count;
            while (page_cnt--)
            {
                if (flash_sector_buf(buf, addr, FLASH_SECTOR_SIZE) == 0)
                {
                    return 0;
                }
                addr += FLASH_SECTOR_SIZE;
                buf += FLASH_SECTOR_SIZE;
            }
            if (len_left != 0)
            {
                if (flash_sector_buf(buf, addr, len_left) == 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1; /* 成功 */
}

#else
#include "fal.h"
#include "fal_def.h"
#define MAX_RETRY_COUNT (3)

static int data_check(void *src, void *dst, uint32_t size)
{
    uint8_t *p_src = (uint8_t *)src, *p_dst = (uint8_t *)dst;

    return memcmp(p_dst, p_src, size);
}

static int erase_check(uint32_t addr, uint32_t page_num)
{
    uint32_t i = 0;
    uint8_t erase_buf[QSPI_PAGE_SIZE];

    memset(erase_buf, 0xFF, QSPI_PAGE_SIZE);

    for (i = 0; i < page_num; i++) {
        if (0 != data_check(erase_buf, (uint8_t *)(addr + i * QSPI_PAGE_SIZE), sizeof(erase_buf))){
            rt_kprintf("[FLASH HW] Erase Check failed!\n");
            return -1;
        }
    }

    return 0;
}

static int init( void )
{
    QSPI_Init(NULL);
    QSPI_SetLatency(0);

    return 0;
}

MOVE_TO_RAM
static int read( long addr, uint8_t* buf, size_t size )
{
    if (size == 0) return -1;

    /* 计算绝对内存地址. */
    uint32_t abs_addr = FLASH_BASE_ADDR + (uint32_t)addr;

    memcpy((void *)buf, (void *)abs_addr, size);

    /* 返回读取的字节数, 负数代表失败. */
    return (int)size;
}

MOVE_TO_RAM
static int write( long addr, const uint8_t* buf, size_t size )
{
    if (size == 0) return -1;

    uint32_t abs_addr = FLASH_BASE_ADDR + (uint32_t)addr;
    size_t bytes_written = 0;

    uint8_t retried = 0;
    while (bytes_written < size) {
        /* 计算当前页内剩余可写大小. */
        uint32_t current_addr = abs_addr + bytes_written;
        uint32_t page_offset = current_addr % QSPI_PAGE_SIZE;
        uint32_t chunk_size = QSPI_PAGE_SIZE - page_offset;

        if (chunk_size > (size - bytes_written)) {
            chunk_size = size - bytes_written;
        }

        /* 写入单页内容. */
        FLASH_ProgramPage(NULL, NULL, current_addr, chunk_size, (uint8_t *)(buf + bytes_written));
        
        CACHE_CleanAll(DCACHE);
        
        // for (uint32_t delay = 0; delay < 50000; delay++) __NOP();
        
        /* 本来应该检查 SR 寄存器的 BUSY 位的, 但是没有相应的接口, 而且不一定成功, 所以这里直接检查数据. */
        if (data_check((void *)current_addr, (void *)(buf + bytes_written), chunk_size) != 0) {
            /* 检查失败, 等待一段时间后重试. */
            if (retried == MAX_RETRY_COUNT) {
                /* 重试次数达到最大值, 返回失败. */
                return -1;
            } else {
                retried++;
                continue;
            }
        } else {
            retried = 0;
        }

        bytes_written += chunk_size;
    }

    return (int)size;
}

MOVE_TO_RAM
static int erase( long addr, size_t size )
{
    if (size == 0) return -1;

    /* 扇区必须对齐. */
    if ((addr % FLASH_SECTOR_SIZE) != 0 || 
        (size % FLASH_SECTOR_SIZE) != 0) {
        return -1;
    }

    uint32_t abs_addr = FLASH_BASE_ADDR + (uint32_t)addr;
    uint32_t end_addr = abs_addr + size;
    uint8_t  retried  = 0;
    while (abs_addr < end_addr) {
        /* 擦除当前扇区. */
        FLASH_EraseSector(abs_addr);
        CACHE_CleanAll(DCACHE);
        
        /* 本来应该检查 SR 寄存器的 BUSY 位的, 但是没有相应的接口, 而且不一定成功, 所以这里直接检查数据. */
        if (erase_check(abs_addr, FLASH_SECTOR_SIZE / QSPI_PAGE_SIZE) != 0) {
            /* 检查失败, 等待一段时间后重试. */
            if (erase_check(abs_addr, FLASH_SECTOR_SIZE / QSPI_PAGE_SIZE) != 0) {
                if (retried == MAX_RETRY_COUNT) {
                    /* 重试次数达到最大值, 返回失败. */
                    return -1;
                } else {
                    retried++;
                    continue;
                }
            }
        } else {
            retried = 0;
        }

        abs_addr += FLASH_SECTOR_SIZE;
    }

    return (int)size;
}

const struct fal_flash_dev nor_flash0 = {
    .name       = "norflash0",
    .addr       = FLASH_BASE_ADDR,
    .len        = FLASH_SIZE,
    .blk_size   = FLASH_SECTOR_SIZE,
    .ops        = { init, read, write, erase },
    .write_gran = 1,
};

#endif
