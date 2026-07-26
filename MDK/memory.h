/*--------------------- Flash Configuration ----------------------------------
; <h> Flash Configuration
;   <o0> Flash Base Address <0x0-0xFFFFFFFF:8>
;   <o1> Flash Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
 *----------------------------------------------------------------------------*/
#define __BOOT_BASE     0x08001000
#define __APP_BASE      0x080B0000
#define __ROM_BASE      __BOOT_BASE
#define __ROM_SIZE      0x01FFF000

#define RES_TEST_BASE   0x08301000
#define RES_LOGO_BASE   0x0910C000
#define RES_IMG_BASE    0x09170000

#define __RAM_CODE_SIZE   ( 4 * 1024 )

/*--------------------- Embedded RAM Configuration ---------------------------
; <h> RAM Configuration
;   <o0> RAM Base Address    <0x0-0xFFFFFFFF:8>
;   <o1> RAM Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
 *----------------------------------------------------------------------------*/
#define __RAM_BASE      0x20000000
#define __RAM_SIZE      0x00140000

/*--------------------- Stack / Heap Configuration ---------------------------
; <h> Stack / Heap Configuration
;   <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;   <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
 *----------------------------------------------------------------------------*/
#define __STACK_SIZE    0x4000
#define __HEAP_SIZE     0x0100

/*--------------------- CMSE Venner Configuration ---------------------------
; <h> CMSE Venner Configuration
;   <o0>  CMSE Venner Size (in Bytes) <0x0-0xFFFFFFFF:32>
; </h>
 *----------------------------------------------------------------------------*/
#define __CMSEVENEER_SIZE    0x200
