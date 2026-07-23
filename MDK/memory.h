// <<< Use Configuration Wizard in Context Menu >>>

/*--------------------- Flash Configuration ----------------------------------*/
//  <h> Flash Configuration
//   <o> Bootloader Base Address <0x0-0xFFFFFFFF>
//   <i> Bootloader 的起始地址
#define __BOOT_BASE     0x08001000

//   <o> Application Base Address <0x0-0xFFFFFFFF>
//   <i> APP 固件的起始地址
#define __APP_BASE      0x080E2000 

//   <o> ROM Size (in Bytes) <0x0-0xFFFFFFFF>
//   <i> 总 ROM 空间大小
#define __ROM_SIZE      0x2000000

// <o> ROM Base Address Selection <0=> BOOT <1=> APP
// <i> 选择芯片 ROM 的基地址是绑定到 Boot 还是 App
#define __ROM_BASE_SEL  0
// </h>

#if __ROM_BASE_SEL
#define __ROM_BASE      __APP_BASE
#else
#define __ROM_BASE      __BOOT_BASE
#endif

//  <h> Resource Flash Address
//    <o> Logo Base Address <0x0-0xFFFFFFFF>
//    <i> 开机 Logo 的 Flash 起始地址
#define RES_LOGO_BASE   0x08301000

//    <o> Text Base Address <0x0-0xFFFFFFFF>
//    <i> 文本资源的 Flash 起始地址
#define RES_TEXT_BASE   0x08501000

//    <o> Image Base Address <0x0-0xFFFFFFFF>
//    <i> 图片资源的 Flash 起始地址
#define RES_IMG_BASE    0x09401000

//    <o> Filesystem Base Address <0x0-0xFFFFFFFF>
//    <i> 文件系统的 Flash 起始地址
#define RES_FS_BASE    0x09F01000

//  </h>

/*--------------------- Embedded RAM Configuration ---------------------------*/
// <h> RAM Configuration
//    <o> RAM Code Size <0x20000000-0xFFFFFFFF>
//    <i> 用于运行在 RAM 中的代码
#define __RAM_CODE_SIZE   0x00001000

//    <o> RAM Base Address <0x20000000-0xFFFFFFFF>
//    <i> RAM 基地址
#define __RAM_BASE      0x20000000

//    <o> RAM Size <0x0-0xFFFFFFFF>
//    <i> RAM 大小
#define __RAM_SIZE      0x00140000

//  </h>

/*--------------------- Stack / Heap Configuration ---------------------------*/
// <h> Stack / Heap Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define __STACK_SIZE    0x4000

//   <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define __HEAP_SIZE     0x0100
// </h>

/*--------------------- CMSE Venner Configuration ---------------------------*/
// <h> CMSE Venner Configuration
//   <o>  CMSE Venner Size (in Bytes) <0x0-0xFFFFFFFF:32>
#define __CMSEVENEER_SIZE    0x200
// </h>

// <<< end of configuration section >>>
