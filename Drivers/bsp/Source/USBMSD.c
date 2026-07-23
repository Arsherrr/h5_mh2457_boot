#include "USBMSD.h"
#include "USBDevice.h"
#include "VirtualFatFS.h"
#include "lfs_msd.h"

#define LFS_TEST_PATH  "/test.txt"
#define LFS_LOGA_PATH  "/customer/H5/meter_save/log/meter_device_a.log"
#define LFS_LOGB_PATH  "/customer/H5/meter_save/log/meter_device_b.log"

#define FLASH_FILESYSTEM_ADDR   (0x08081000)
#define FLASH_FILESYSTEM_SIZE   (KB(256))
#define FLASH_SECTOR_SIZE       (KB(4))
#define TEST_FILE_SIZE          (KB(64))

#define FAT_SECTOR_SIZE         (512)

uint8_t sector_buf[FLASH_SECTOR_SIZE]  __attribute__((aligned(4))) = {0} ;
extern void send_char(uint8_t* data,uint32_t len);

InterfaceMSDStruct InterfaceMSD;

static bool MSDDiskStatus(InterfaceMSDStruct* self)
{
    return false;
}

static uint32_t MSDDiskRead(InterfaceMSDStruct* self, uint8_t* buffer, uint32_t blockAddr, uint32_t blockCount)
{
    return vfs_read(blockAddr, buffer, blockCount);
}

static void MSDDiskWrite(InterfaceMSDStruct* self, uint8_t* buffer, uint32_t blockAddr, uint32_t blockSize)
{
    vfs_write(blockAddr, buffer, blockSize);
}

#if (USBMSD_DEMO == USBMSD_DEMO_VFS_STREAM)
BlockStruct StreamBlock;

bool MSDStreamSend(uint32_t* buffer, uint32_t bytesToSend, void (*popCallback)(BlockStruct* block))
{
    __disable_irq();
    bool result = false;
    if (StreamBlock.Locked)
    {
        result = false;
    }
    else
    {
        StreamBlock.Buffer      = buffer;
        StreamBlock.Size        = bytesToSend;
        StreamBlock.PopCallback = popCallback;
        StreamBlock.Locked      = true;
        result                  = true;
    }
    __enable_irq();
    return result;
}

static uint32_t MSDStreamRead(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    printf("Read sector %d,%d.\n", sector_offset, num_sectors);

    return lfs_msd_read(LFS_TEST_PATH, sector_offset, data, num_sectors);
#if 0
    if (!StreamBlock.Locked)
        return 0;
    
    int readSize = num_sectors * VFS_SECTOR_SIZE;
    int readableSize = StreamBlock.Size - sector_offset*VFS_SECTOR_SIZE;
    
    readSize     = MIN(readSize, readableSize);
    FLASH_ReadPage(NULL,NULL,data,((uint32_t)StreamBlock.Buffer) + sector_offset*VFS_SECTOR_SIZE,readSize);
    
    printf("readSize %d.\n", readSize);
    
    return num_sectors * VFS_SECTOR_SIZE;
#endif
}

static void Flash_FileSystem_area_init(uint32_t addr,uint32_t size)
{
    uint32_t sector_num = 0;
    
    sector_num = size/FLASH_SECTOR_SIZE;
    if(size%FLASH_SECTOR_SIZE)sector_num++;
    
    for(uint32_t i = 0; i < sector_num; i++)
    {
        FLASH_EraseSector(addr + (i*FLASH_SECTOR_SIZE));
    }
}


static void MSDStreamWrite(uint32_t sector_offset, const uint8_t* data, uint32_t num_sectors)
{
    printf("Write sector %d,%d.\n", sector_offset, num_sectors);
    
    lfs_msd_write(LFS_TEST_PATH, sector_offset, data, num_sectors);
#if 0
    if (!StreamBlock.Locked)
        return;
    
    uint32_t writeSize = num_sectors * VFS_SECTOR_SIZE;
    uint32_t write_addr = 0;
    
    uint32_t flash_sector_num = 0;
    uint32_t flash_sector_addr = 0;
    uint32_t flash_sector_ablesize = 0;
    uint32_t flash_sector_headremainder = 0;
    uint32_t flash_sector_endremainder = 0;
    
    uint32_t sector_buf_write_addr = 0;
    uint32_t sector_buf_write_size = 0;
    
    memset(sector_buf,0,sizeof(sector_buf));
    
    write_addr = (uint32_t)(((uint8_t*)StreamBlock.Buffer) + sector_offset * VFS_SECTOR_SIZE);
    
    if((write_addr&0xFFFFF000) == ((write_addr + writeSize)&0xFFFFF000))
    {
        flash_sector_num = 1;
    }
    else
    {
        flash_sector_headremainder = FLASH_SECTOR_SIZE - (write_addr % FLASH_SECTOR_SIZE);
        flash_sector_endremainder = ((write_addr + writeSize) % FLASH_SECTOR_SIZE);
        flash_sector_num = (writeSize - flash_sector_headremainder - flash_sector_endremainder)/FLASH_SECTOR_SIZE;
        
        if(flash_sector_headremainder)flash_sector_num++;
        if(flash_sector_endremainder)flash_sector_num++;
    }
    
    printf_dbg("writeSize = %d\n",writeSize);
    printf_dbg("flash_sector_num = %d\n",flash_sector_num);
    
    for(uint32_t i = 0;i < flash_sector_num;i++)
    {
        //����Ҫд�뵽flash�ĵ�ַ
        write_addr = (uint32_t)(((uint8_t*)StreamBlock.Buffer) + sector_offset * VFS_SECTOR_SIZE + i*FLASH_SECTOR_SIZE);
        printf_dbg("write_adder = 0x%08x\n",write_addr);
        
        //����Ҫд�������
        flash_sector_addr = write_addr;
        flash_sector_addr &= ~0x00000FFF;
        printf_dbg("flash_sector_adder = 0x%08x\n",flash_sector_addr);
        
        //���㵱ǰ������д�����ݴ�С
        flash_sector_ablesize = FLASH_SECTOR_SIZE - (write_addr - flash_sector_addr);
        printf_dbg("flash_sector_ablesize = %d\n",flash_sector_ablesize);
        
        //����Ҫд�뵽��������ĵ�ַ
        sector_buf_write_addr = (uint32_t)(sector_buf + (write_addr - flash_sector_addr));
        printf_dbg("sector_buf_write_adder = 0x%08x\n",sector_buf_write_addr);
        
        //���㱾��Ҫд������ݴ�С
        sector_buf_write_size = writeSize;
        if(sector_buf_write_size > flash_sector_ablesize)sector_buf_write_size = flash_sector_ablesize;
        printf_dbg("sector_buf_write_size = %d\n",sector_buf_write_size);
        
        //��ȡ�������ݵ�����
        for(uint32_t j = 0; j < FLASH_SECTOR_SIZE/QSPI_PAGE_SIZE;j++)
        {
            FLASH_ReadPage(NULL,NULL,sector_buf + j*QSPI_PAGE_SIZE,flash_sector_addr + j*QSPI_PAGE_SIZE,QSPI_PAGE_SIZE);
        }
        
        //д�����ݵ���������
        memcpy((void*)sector_buf_write_addr,data,sector_buf_write_size);
        
        //��������
        FLASH_EraseSector(flash_sector_addr);
        
        //��������������д�뵽ָ������
        for(uint32_t j = 0; j < FLASH_SECTOR_SIZE/QSPI_PAGE_SIZE;j++)
        {
            FLASH_ProgramPage(NULL, NULL,flash_sector_addr + j*QSPI_PAGE_SIZE, QSPI_PAGE_SIZE, sector_buf + j*QSPI_PAGE_SIZE);
        }
        
        writeSize -= sector_buf_write_size;
    }
    printf_dbg("\n");
#endif
}

static uint32_t meter_loga_read(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    return lfs_msd_read(LFS_LOGA_PATH, sector_offset, data, num_sectors);
}

static uint32_t meter_logb_read(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    return lfs_msd_read(LFS_LOGB_PATH, sector_offset, data, num_sectors);
}

static void meter_loga_write(uint32_t sector_offset, const uint8_t* data, uint32_t num_sectors)
{
    lfs_msd_write(LFS_LOGA_PATH, sector_offset, data, num_sectors);
}

static void meter_logb_write(uint32_t sector_offset, const uint8_t* data, uint32_t num_sectors)
{
    lfs_msd_write(LFS_LOGB_PATH, sector_offset, data, num_sectors);
}

extern void *get_log_cache(void);
extern int get_log_cache_size(void);
extern unsigned int get_log_mtime(void);

static uint32_t log_cache_read(uint32_t sector_offset, uint8_t* data, uint32_t num_sectors)
{
    /* 1. 将请求的 "扇区" 转换成 SDRAM 中的 "字节偏移量" 和 "总字节数". */
    uint32_t byte_offset = sector_offset * FAT_SECTOR_SIZE;
    uint32_t byte_len    = num_sectors * FAT_SECTOR_SIZE;

    uint8_t *cache_ptr = (uint8_t *)get_log_cache();
    int total_size     = get_log_cache_size();

    if (cache_ptr == NULL || total_size <= 0) {
        memset(data, 0, byte_len);
        return byte_len;
    }

    if (byte_offset < (uint32_t)total_size) {
        /* 计算从当前偏移量开始, SDRAM 里还有多少有效数据. */
        uint32_t valid_len = (uint32_t)total_size - byte_offset;

        if (valid_len >= byte_len) {
            /* 有效数据之内, 整块拷贝. */
            memcpy(data, cache_ptr + byte_offset, byte_len);
        } else {
            /* 一部分有效, 一部分越界了. */
            /* 有效部分直接拷贝. */
            memcpy(data, cache_ptr + byte_offset, valid_len);

            /* 越界部分填 0. */
            memset(data + valid_len, 0, byte_len - valid_len);
        }
    }
    else
    {
        /* 超出范围, 全部填 0. */
        memset(data, 0, byte_len); 
    }

    return byte_len;
}

static void log_cache_write(uint32_t sector_offset, const uint8_t* data, uint32_t num_sectors)
{
    (void)sector_offset;
    (void)data;
    (void)num_sectors;
}

#elif (USBMSD_DEMO == USBMSD_DEMO_VFS_SWKEY)

static const uint8_t USBKeySWInquiry[36] = {
    0x00, // Direct Access Device
    0x80, // RMB 80
    0x02, // ISO/ECMA/ANSI
    0x02, // Response Data Format: SCIS-2
    0x1f, // Additional Length
    0x00, 0x00,
    0x00, // Reserved

    'S',  'W',  'U', 'K', 'E', 'Y', ' ', ' ',                                         //
    ' ',  ' ',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //
    ' ',  ' ',  ' ', ' '                                                              //
};

static bool SWKeyCBWDecode(InterfaceMSDStruct* self)
{
    switch (self->CBW.CB[0])
    {
        // override default SCSI Command or deal Custom SCSI Command decode here
        // case EXAMPLE_CMD:
        //     ExampleDecode(...);
        //     return true;
    }
    return false;
}

static bool SWKeyInProcess(InterfaceMSDStruct* self, uint8_t* buffer, uint32_t bytesWritten)
{
    switch (self->CBW.CB[0])
    {
        // override default SCSI Command or deal Custom SCSI Command in process here
        // case EXAMPLE_CMD:
        //     ExampleProcess(...);
        //     return true;
    }
    return false;
}

static bool SWKeyOutProcess(InterfaceMSDStruct* self, uint8_t* buffer, uint32_t bytesRead)
{
    switch (self->CBW.CB[0])
    {
        // override default SCSI Command or deal Custom SCSI Command out process here
        // case EXAMPLE_CMD:
        //     ExampleProcess(...);
        //     return true;
    }
    return false;
}

#elif (USBMSD_DEMO == USBMSD_DEMO_VFS_CDROM)

static const uint8_t USBCDROMInquiry[] = {
    0x05, 0x80, 0x02, 0x02, 0x1F, 0x00, 0x00, 0x00,      //
    'M',  'e',  'g',  'a',  'H',  'u',  'n',  't',       //
    'V',  'F',  'S',  '-',  'C',  'D',  'R',  'O',  'M', //
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,            //
    '1',  '.',  '0',  0x00,
};

#endif

uint32_t MSDReadBuffer[ToUintSize(VFS_SECTOR_SIZE)];
uint32_t MSDWriteBuffer[ToUintSize(VFS_SECTOR_SIZE)];

void MSDSetup(USBDeviceStruct* device)
{
    InterfaceMSDStruct* msd = &InterfaceMSD;
    InterfaceMSDConstractor(msd);

    msd->DiskStatusCallback = MSDDiskStatus;
    msd->DiskReadCallback   = MSDDiskRead;
    msd->DiskWriteCallback  = MSDDiskWrite;

#if (USBMSD_DEMO == USBMSD_DEMO_VFS_STREAM)
    #if 0
    // Visual FileSystem Init
    vfs_init((vfs_filename_t) {"ESL-Disk"}, MB(64));
    FatDirectoryEntry_t* streamFile = vfs_create_file((vfs_filename_t) {"DISK    RAW"}, MSDStreamRead, MSDStreamWrite, KB(512));
    streamFile->attributes          = VFS_FILE_ATTR_HIDDEN;

    msd->base.Name = L"USB-ESL Mass Storage Device";

    StreamBlock.Locked      = false;
    StreamBlock.Size        = 0;
    StreamBlock.Buffer      = NULL;
    StreamBlock.PopCallback = NULL;
    #else
    //Flash_FileSystem_area_init(FLASH_FILESYSTEM_ADDR,FLASH_FILESYSTEM_SIZE);
    
    vfs_init((vfs_filename_t) {"H5-Disk"}, FLASH_FILESYSTEM_SIZE);

    // /* 获取文件大小. */
    // int fsize = lfs_get_file_size(LFS_TEST_PATH);

    // /* 创建 FATFS 上的文件. */
    // if (fsize != 0) {
    //     FatDirectoryEntry_t* streamFile = vfs_create_file((vfs_filename_t) {"TEST    TXT"}, MSDStreamRead, MSDStreamWrite, fsize);
    //     streamFile->attributes          = VFS_FILE_ATTR_SYSTEM;
    // }

    // fsize = lfs_get_file_size(LFS_LOGA_PATH);
    // if (fsize != 0) {
    //     FatDirectoryEntry_t* streamFile = vfs_create_file((vfs_filename_t) {"METER_A LOG"}, meter_loga_read, meter_loga_write, fsize);
    //     streamFile->attributes          = VFS_FILE_ATTR_SYSTEM;
    // }

    // int fsize = lfs_get_file_size(LFS_LOGB_PATH);
    // if (fsize != 0) {
    //     FatDirectoryEntry_t* streamFile = vfs_create_file((vfs_filename_t) {"METER_B LOG"}, meter_logb_read, meter_logb_write, fsize);
    //     streamFile->attributes          = VFS_FILE_ATTR_SYSTEM;
    // }

    /* 获取日志缓存. */
    int fsize = get_log_cache_size();
    if (fsize != 0) {
        unsigned int mtime = get_log_mtime();
        uint16_t fat_date, fat_time;
        unix_to_fat_time(0, mtime, &fat_date, &fat_time);

        FatDirectoryEntry_t* streamFile = vfs_create_file((vfs_filename_t) {"METER   LOG"}, log_cache_read, log_cache_write, fsize);
        streamFile->attributes          = VFS_FILE_ATTR_SYSTEM;
        streamFile->modification_date   = fat_date;
        streamFile->modification_time   = fat_time;
        streamFile->accessed_date       = fat_date;
        streamFile->creation_date       = fat_date;
        streamFile->creation_time       = fat_time;
    }

    msd->base.Name = L"USB-ESL Mass Storage Device";

    StreamBlock.Locked      = true;
    StreamBlock.Size        = FLASH_FILESYSTEM_SIZE;
    StreamBlock.Buffer      = (uint32_t *)FLASH_FILESYSTEM_ADDR;
    
    StreamBlock.PopCallback = NULL;
    #endif
#elif (USBMSD_DEMO == USBMSD_DEMO_VFS_SWKEY)
    // Visual FileSystem Init
    vfs_init((vfs_filename_t) {"ESL-Key"}, MB(64));

    msd->base.Name = L"USB-ESL Key";

    msd->InquiryData        = (uint32_t*)USBKeySWInquiry;
    msd->InquirySize        = sizeof(USBKeySWInquiry);
    msd->CBWDecodeCallback  = SWKeyCBWDecode;
    msd->InProcessCallback  = SWKeyInProcess;
    msd->OutProcessCallback = SWKeyOutProcess;
#elif (USBMSD_DEMO == USBMSD_DEMO_VFS_CDROM)
    vfs_init((vfs_filename_t) {"ESL-CD"}, MB(64));

    msd->base.Name = L"USB-ESL CDROM";

    msd->InquiryData = (uint32_t*)USBCDROMInquiry;
    msd->InquirySize = sizeof(USBCDROMInquiry);
#endif

    // Setup MSD Properties
    msd->BlockSize       = VFS_SECTOR_SIZE;
    msd->BlockCount      = vfs_get_total_size() / VFS_SECTOR_SIZE;
    msd->ReadBuffer      = MSDReadBuffer;
    msd->ReadBufferSize  = sizeof(MSDReadBuffer);
    msd->WriteBuffer     = MSDWriteBuffer;
    msd->WriteBufferSize = sizeof(MSDWriteBuffer);

    // Setup MSD Endpoint
    msd->RxEPConfig.Index = 1;
    msd->TxEPConfig.Index = 1;

    // Improve the transfer performance of USB
    msd->RxEPConfig.MaxPacket        = 512;
    msd->RxEPConfig.AdditionalBuffer = 1;
    msd->TxEPConfig.MaxPacket        = 512;
    msd->TxEPConfig.AdditionalBuffer = 1;

    device->RegisterInterface(device, &msd->base);
}
