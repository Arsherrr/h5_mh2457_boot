; Copyright (c) 2011-2023 Megahunt Technologies Inc.
; SPDX-License-Identifier: Apache-2.0

; << Use Configuration Wizard in Context Menu >>>

; <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
Stack_Size      EQU      0x00004000

                AREA     STACK, NOINIT, READWRITE, ALIGN=3
__stack_limit
Stack_Mem       SPACE    Stack_Size
__initial_sp

; <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
Heap_Size       EQU      0x00002000

                IF       Heap_Size != 0                      ; Heap is provided
                AREA     HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE    Heap_Size
__heap_limit
                ENDIF


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA     RESET, DATA, READONLY
                EXPORT   __Vectors
                EXPORT   __Vectors_End
                EXPORT   __Vectors_Size

__Vectors       DCD      __initial_sp                        ;     Top of Stack
                DCD      Reset_Handler                       ;     Reset Handler
                DCD      NMI_Handler                         ; -14 NMI Handler
                DCD      HardFault_Handler                   ; -13 Hard Fault Handler
                DCD      MemManage_Handler                   ; -12 MPU Fault Handler
                DCD      BusFault_Handler                    ; -11 Bus Fault Handler
                DCD      UsageFault_Handler                  ; -10 Usage Fault Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      SVC_Handler                         ;  -5 SVC Handler
                DCD      DebugMon_Handler                    ;  -4 Debug Monitor Handler
                DCD      0                                   ;     Reserved
                DCD      PendSV_Handler                      ;  -2 PendSV Handler
                DCD      SysTick_Handler                     ;  -1 SysTick Handler

                ; Interrupts
                DCD      WWDG_IRQHandler                     ;
                DCD      PVD_IRQHandler                      ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      QSPI_IRQHandler                     ;
                DCD      RCC_IRQHandler                      ;
                DCD      EXTI0_IRQHandler                    ;
                DCD      EXTI1_IRQHandler                    ;
                DCD      EXTI2_IRQHandler                    ;
                DCD      EXTI3_IRQHandler                    ;
                DCD      EXTI4_IRQHandler                    ;
                DCD      DMA1_Stream0_IRQHandler             ;
                DCD      DMA1_Stream1_IRQHandler             ;
                DCD      DMA1_Stream2_IRQHandler             ;
                DCD      DMA1_Stream3_IRQHandler             ;
                DCD      DMA1_Stream4_IRQHandler             ;
                DCD      DMA1_Stream5_IRQHandler             ;
                DCD      DMA1_Stream6_IRQHandler             ;
                DCD      ADC_IRQHandler                      ;
                DCD      CAN1_TX_IRQHandler                  ;
                DCD      CAN1_RX0_IRQHandler                 ;
                DCD      CAN1_RX1_IRQHandler                 ;
                DCD      CAN1_SCE_IRQHandler                 ;
                DCD      EXTI9_5_IRQHandler                  ;
                DCD      TIM1_BRK_TIM9_IRQHandler            ;
                DCD      TIM1_UP_TIM10_IRQHandler            ;
                DCD      TIM1_TRG_COM_TIM11_IRQHandler       ;
                DCD      TIM1_CC_IRQHandler                  ;
                DCD      TIM2_IRQHandler                     ;
                DCD      TIM3_IRQHandler                     ;
                DCD      TIM4_IRQHandler                     ;
                DCD      I2C1_EV_IRQHandler                  ;
                DCD      I2C1_ER_IRQHandler                  ;
                DCD      I2C2_EV_IRQHandler                  ;
                DCD      I2C2_ER_IRQHandler                  ;
                DCD      SPI1_IRQHandler                     ;
                DCD      SPI2_IRQHandler                     ;
                DCD      USART1_IRQHandler                   ;
                DCD      USART2_IRQHandler                   ;
                DCD      USART3_IRQHandler                   ;
                DCD      EXTI15_10_IRQHandler                ;
                DCD      RTC_Alarm_IRQHandler                ;
                DCD      0                                   ;
                DCD      TIM8_BRK_TIM12_IRQHandler           ;
                DCD      TIM8_UP_TIM13_IRQHandler            ;
                DCD      TIM8_TRG_COM_TIM14_IRQHandler       ;
                DCD      TIM8_CC_IRQHandler                  ;
                DCD      DMA1_Stream7_IRQHandler             ;
                DCD      FSMC_IRQHandler                     ;
                DCD      SDIO_IRQHandler                     ;
                DCD      TIM5_IRQHandler                     ;
                DCD      SPI3_IRQHandler                     ;
                DCD      UART4_IRQHandler                    ;
                DCD      0                                   ;
                DCD      TIM6_DAC_IRQHandler                 ;
                DCD      TIM7_IRQHandler                     ;
                DCD      DMA2_Stream0_IRQHandler             ;
                DCD      DMA2_Stream1_IRQHandler             ;
                DCD      DMA2_Stream2_IRQHandler             ;
                DCD      DMA2_Stream3_IRQHandler             ;
                DCD      DMA2_Stream4_IRQHandler             ;
                DCD      ETH_IRQHandler                      ;
                DCD      0                                   ;
                DCD      CAN2_TX_IRQHandler                  ;
                DCD      CAN2_RX0_IRQHandler                 ;
                DCD      CAN2_RX1_IRQHandler                 ;
                DCD      CAN2_SCE_IRQHandler                 ;
                DCD      USB1_IRQHandler                     ;
                DCD      DMA2_Stream5_IRQHandler             ;
                DCD      DMA2_Stream6_IRQHandler             ;
                DCD      DMA2_Stream7_IRQHandler             ;
                DCD      USART6_IRQHandler                   ;
                DCD      I2C3_EV_IRQHandler                  ;
                DCD      I2C3_ER_IRQHandler                  ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      DCMI_IRQHandler                     ;
                DCD      0                                   ;
                DCD      HASH_RNG_IRQHandler                 ;
                DCD      FPU_IRQHandler                      ;
                DCD      UART7_IRQHandler                    ;
                DCD      0                                   ;
                DCD      SPI4_IRQHandler                     ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      LTDC_IRQHandler                     ;
                DCD      0                                   ;
                DCD      DMA2D_IRQHandler                    ;
                DCD      0                                   ;
                DCD      DSI_IRQHandler                      ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      HSPI_IRQHandler                     ;
                DCD      0                                   ;
                DCD      JPEGD_IRQHandler                    ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      0                                   ;
                DCD      BUSERROR_IRQHandler                 ;
                SPACE    (116 * 4)                           ; Interrupts 108 .. 224 are left out
__Vectors_End
__Vectors_Size  EQU      __Vectors_End - __Vectors

                AREA     |.text|, CODE, READONLY

; Reset Handler
Reset_Handler   PROC
                EXPORT   Reset_Handler
                IMPORT   SystemInit
                IMPORT   __main

                LDR      R0, =__Vectors
                LDR      R1, =0xE000ED08
                STR      R0, [R1]
                LDR      R0, [R0]
                MOV      SP, R0

                LDR      R0, =SystemInit
                BLX      R0
                LDR      R0, =__main
                BX       R0
                ENDP

; The default macro is not used for HardFault_Handler
; because this results in a poor debug illusion.
HardFault_Handler PROC
                EXPORT   HardFault_Handler         [WEAK]
                B        .
                ENDP

; Macro to define default exception/interrupt handlers.
; Default handler are weak symbols with an endless loop.
; They can be overwritten by real handlers.
                MACRO
                Set_Default_Handler  $Handler_Name
$Handler_Name   PROC
                EXPORT   $Handler_Name             [WEAK]
                B        .
                ENDP
                MEND


; Default exception/interrupt handler

                Set_Default_Handler  NMI_Handler
                Set_Default_Handler  MemManage_Handler
                Set_Default_Handler  BusFault_Handler
                Set_Default_Handler  UsageFault_Handler
                Set_Default_Handler  SVC_Handler
                Set_Default_Handler  DebugMon_Handler
                Set_Default_Handler  PendSV_Handler
                Set_Default_Handler  SysTick_Handler

                Set_Default_Handler  WWDG_IRQHandler
                Set_Default_Handler  PVD_IRQHandler
                Set_Default_Handler  QSPI_IRQHandler
                Set_Default_Handler  RCC_IRQHandler
                Set_Default_Handler  EXTI0_IRQHandler
                Set_Default_Handler  EXTI1_IRQHandler
                Set_Default_Handler  EXTI2_IRQHandler
                Set_Default_Handler  EXTI3_IRQHandler
                Set_Default_Handler  EXTI4_IRQHandler
                Set_Default_Handler  DMA1_Stream0_IRQHandler
                Set_Default_Handler  DMA1_Stream1_IRQHandler
                Set_Default_Handler  DMA1_Stream2_IRQHandler
                Set_Default_Handler  DMA1_Stream3_IRQHandler
                Set_Default_Handler  DMA1_Stream4_IRQHandler
                Set_Default_Handler  DMA1_Stream5_IRQHandler
                Set_Default_Handler  DMA1_Stream6_IRQHandler
                Set_Default_Handler  ADC_IRQHandler
                Set_Default_Handler  CAN1_TX_IRQHandler
                Set_Default_Handler  CAN1_RX0_IRQHandler
                Set_Default_Handler  CAN1_RX1_IRQHandler
                Set_Default_Handler  CAN1_SCE_IRQHandler
                Set_Default_Handler  EXTI9_5_IRQHandler
                Set_Default_Handler  TIM1_BRK_TIM9_IRQHandler
                Set_Default_Handler  TIM1_UP_TIM10_IRQHandler
                Set_Default_Handler  TIM1_TRG_COM_TIM11_IRQHandler
                Set_Default_Handler  TIM1_CC_IRQHandler
                Set_Default_Handler  TIM2_IRQHandler
                Set_Default_Handler  TIM3_IRQHandler
                Set_Default_Handler  TIM4_IRQHandler
                Set_Default_Handler  I2C1_EV_IRQHandler
                Set_Default_Handler  I2C1_ER_IRQHandler
                Set_Default_Handler  I2C2_EV_IRQHandler
                Set_Default_Handler  I2C2_ER_IRQHandler
                Set_Default_Handler  SPI1_IRQHandler
                Set_Default_Handler  SPI2_IRQHandler
                Set_Default_Handler  USART1_IRQHandler
                Set_Default_Handler  USART2_IRQHandler
                Set_Default_Handler  USART3_IRQHandler
                Set_Default_Handler  EXTI15_10_IRQHandler
                Set_Default_Handler  RTC_Alarm_IRQHandler
                Set_Default_Handler  TIM8_BRK_TIM12_IRQHandler
                Set_Default_Handler  TIM8_UP_TIM13_IRQHandler
                Set_Default_Handler  TIM8_TRG_COM_TIM14_IRQHandler
                Set_Default_Handler  TIM8_CC_IRQHandler
                Set_Default_Handler  DMA1_Stream7_IRQHandler
                Set_Default_Handler  FSMC_IRQHandler
                Set_Default_Handler  SDIO_IRQHandler
                Set_Default_Handler  TIM5_IRQHandler
                Set_Default_Handler  SPI3_IRQHandler
                Set_Default_Handler  UART4_IRQHandler
                Set_Default_Handler  TIM6_DAC_IRQHandler
                Set_Default_Handler  TIM7_IRQHandler
                Set_Default_Handler  DMA2_Stream0_IRQHandler
                Set_Default_Handler  DMA2_Stream1_IRQHandler
                Set_Default_Handler  DMA2_Stream2_IRQHandler
                Set_Default_Handler  DMA2_Stream3_IRQHandler
                Set_Default_Handler  DMA2_Stream4_IRQHandler
                Set_Default_Handler  ETH_IRQHandler
                Set_Default_Handler  CAN2_TX_IRQHandler
                Set_Default_Handler  CAN2_RX0_IRQHandler
                Set_Default_Handler  CAN2_RX1_IRQHandler
                Set_Default_Handler  CAN2_SCE_IRQHandler
                Set_Default_Handler  USB1_IRQHandler
                Set_Default_Handler  DMA2_Stream5_IRQHandler
                Set_Default_Handler  DMA2_Stream6_IRQHandler
                Set_Default_Handler  DMA2_Stream7_IRQHandler
                Set_Default_Handler  USART6_IRQHandler
                Set_Default_Handler  I2C3_EV_IRQHandler
                Set_Default_Handler  I2C3_ER_IRQHandler
                Set_Default_Handler  DCMI_IRQHandler
                Set_Default_Handler  HASH_RNG_IRQHandler
                Set_Default_Handler  FPU_IRQHandler
                Set_Default_Handler  UART7_IRQHandler
                Set_Default_Handler  SPI4_IRQHandler
                Set_Default_Handler  LTDC_IRQHandler
                Set_Default_Handler  DMA2D_IRQHandler
                Set_Default_Handler  DSI_IRQHandler
                Set_Default_Handler  HSPI_IRQHandler
                Set_Default_Handler  JPEGD_IRQHandler
                Set_Default_Handler  BUSERROR_IRQHandler

                ALIGN

; User setup Stack & Heap

                IF       :LNOT::DEF:__MICROLIB
                IMPORT   __use_two_region_memory
                ENDIF

                EXPORT   __stack_limit
                EXPORT   __initial_sp
                IF       Heap_Size != 0                      ; Heap is provided
                EXPORT   __heap_base
                EXPORT   __heap_limit
                ENDIF

                END
