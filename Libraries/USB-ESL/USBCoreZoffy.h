

/*!
    @brief Indicates whether to get the mode of DMA dynamically.
*/
#ifndef FEATURE_USBCORE_ZOFFY_DYNAMIC_DMA_MODE
#define FEATURE_USBCORE_ZOFFY_DYNAMIC_DMA_MODE 0
#endif

/*!
    @brief Indicates whether USB Core Zoffy supports DMA.
    @note  Only meaningful when FEATURE_USBCORE_ZOFFY_DYNAMIC_DMA_MODE is 0.
*/
#ifndef FEATURE_USBCORE_ZOFFY_SUPPORT_DMA
#define FEATURE_USBCORE_ZOFFY_SUPPORT_DMA 0
#endif

/*!
    @brief Indicates whether to get the mode of FIFO dynamically.
*/
#ifndef FEATURE_USBCORE_ZOFFY_DYNAMIC_FIFO_MODE
#define FEATURE_USBCORE_ZOFFY_DYNAMIC_FIFO_MODE 0
#endif

/*!
    @brief Indicates whether USB Core Zoffy supports dedicated tx FIFO.
    @note  Only meaningful when FEATURE_USBCORE_ZOFFY_DYNAMIC_FIFO_MODE is 0.
*/
#ifndef FEATURE_USBCORE_ZOFFY_SUPPORT_DEDICATED_FIFO
#define FEATURE_USBCORE_ZOFFY_SUPPORT_DEDICATED_FIFO 1
#endif

// USB max endpoint count
#ifndef FEATURE_USBCORE_ZOFFY_MAX_EP_COUNT
#error "Must define FEATURE_USBCORE_ZOFFY_MAX_EP_COUNT in USBTarget<MCU>.h"
#endif

#ifndef CONFIG_USB_DEVICE_EP_COUNT
#define CONFIG_USB_DEVICE_EP_COUNT FEATURE_USBCORE_ZOFFY_MAX_EP_COUNT
#endif

typedef struct USBZoffyStruct_s     USBZoffyStruct;
typedef struct USBCoreZoffyStruct_s USBCoreZoffyStruct;

struct USBCoreZoffyStruct_s {
    USBZoffyStruct* USB;

    uint16_t MaxFifoSize;
    uint16_t MinRxFifoSize;

    union {
        struct {
            uint8_t TxFifoIndex : 4;
            uint8_t : 2;
            bool IsDedicatedTFifo : 1;
            bool IsDMASupported : 1;
        };
        struct {
            uint8_t IEPProcessing : 4;

            bool IsTxFifoBusy : 1;
            uint8_t : 3;
        };
    };

    bool IsHostSupported : 1;
    bool IsDeviceSupported : 1;
    bool IsSRPSupported : 1;
    bool IsHNPSupported : 1;

    uint8_t  OEPToDisable : 4;
    uint16_t OEPSplitMask;
};
