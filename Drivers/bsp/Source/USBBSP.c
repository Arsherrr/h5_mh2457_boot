#include "USBBSP.h"

static unsigned int usb_type = USE_COMPONENT_USBVCP;
static wchar_t USBUniqueSN[16] = {0};

#define SOC_MAX_SN_LENGTH 16
void USBGenerateUniqueSN(wchar_t* uniqueSN, uint32_t maxLength)
{
    // Dynamic USB Device SN format:
    //   [interface functions]-[ChipSN sum]
    uint8_t usbSN[32] = {0};

    maxLength = MIN((maxLength - 1), sizeof(usbSN));

    uint8_t snIndex = 0;

    if (usb_type == USE_COMPONENT_USBMSD) {
        #if USE_COMPONENT_USBMSD
            usbSN[snIndex++] = 'M';
        #endif // USE_COMPONENT_USBMSD
    } else if (usb_type == USE_COMPONENT_USBVCP) {
        
        #if USE_COMPONENT_USBVCP
            usbSN[snIndex++] = 'V';
        #endif // USE_COMPONENT_USBVCP
    } else {
        #if USE_COMPONENT_USBHID
            usbSN[snIndex++] = 'D';
        #endif // USE_COMPONENT_USBHID
        #if USE_COMPONENT_USBCCID
            usbSN[snIndex++] = 'C';
        #endif // USE_COMPONENT_USBCCID
        #if USE_COMPONENT_USBVENDOR
            usbSN[snIndex++] = 'N';
        #endif // USE_COMPONENT_USBVENDOR
        #if USE_COMPONENT_USBUVC
            usbSN[snIndex++] = 'U';
        #endif // USE_COMPONENT_USBUVC
            usbSN[snIndex++] = '-';
    }

    uint8_t socSN[SOC_MAX_SN_LENGTH] = {0};

    uint8_t socSNLength = USBTargetGetSN(socSN, sizeof(socSN));

    uint8_t usnLength = MIN((maxLength - snIndex), 8);
    uint8_t shift     = 0;
    for (int i = 0; i < socSNLength * 2; i++) 
	{
        int usnIndex = i % usnLength + snIndex;
        shift += usbSN[usnIndex] + socSN[i % socSNLength];
        usbSN[usnIndex] = shift % 36;
    }

    for (int i = 0; i < snIndex; i++)
    {
        uniqueSN[i] = usbSN[i];
    }
    for (int i = snIndex; i < snIndex + usnLength; i++)
    {
        uniqueSN[i] = usbSN[i] + (usbSN[i] < 0x0A ? '0' : '7');
    }
}

USBHALStruct    USBHAL1;
USBDeviceStruct USBDevice1;

void USBInterruptSetup(void)
{
    uint8_t priority = 0xFF >> (8 - __NVIC_PRIO_BITS + NVIC_GetPriorityGrouping());
    // Set the interrupt priority of USB Core1
    NVIC_SetPriority(USB_ZOFFY1_IRQ, priority);
    // Will enable when hal is inited
    NVIC_DisableIRQ(USB_ZOFFY1_IRQ);
}

void USBSetup(unsigned int type)
{
    /* 设置 USB 配置类型. */
    usb_type = type;

    USBInterruptSetup();

    // ******** USB Core 1 Setup ********
    // USBHAL1 Init
    USBHALStruct* hal = &USBHAL1;
    USBCoreZoffyConstractor(hal, USB_ZOFFY1_BASE);

    // Override the default settings of USBHAL1 here
    // hal->PhyType = USBPhyTypeFSI2C;

    hal->Init(hal);

    // USBDevice1 Init
    USBDeviceStruct* device = &USBDevice1;

    USBDeviceConstractor(device);

    device->Manufacturer           = L"Megahunt";
    device->Product                = L"USB-ESL Composite Device";
    device->ConfigurationName      = L"Default";
    device->MaxPower               = 100; // 100mA, Max 500mA
    device->Attributes.SelfPowered = true;

    // Generate unique SN for USB
    USBGenerateUniqueSN(USBUniqueSN, sizeof(USBUniqueSN) / sizeof(wchar_t));
    device->SerialNumber = USBUniqueSN;

    // Configure USBDevice's VendorID, ProductID and ProductRelease
    device->Init(device, hal, 0x28E9, 0x018A, 0x0001);

    if (usb_type == USE_COMPONENT_USBMSD) {
        #if USE_COMPONENT_USBMSD
            MSDSetup(device);
        #endif // USE_COMPONENT_USBMSD
    } else if (usb_type == USE_COMPONENT_USBVCP) {
        #if USE_COMPONENT_USBVCP
            VCPSetup(device);
        #endif // USE_COMPONENT_USBVCP
    } else {
        #if USE_COMPONENT_USBVENDOR
            VendorSetup(device);
        #endif // USE_COMPONENT_USBVENDOR
        
        #if USE_COMPONENT_USBHID
            HIDSetup(device);
        #endif // USE_COMPONENT_USBHID
    }

    // USBDevice1 Start
    device->Start(device);
}

void USBStop(void)
{
    // ******** USB Core 1 Stop ********
    if (USBDevice1.InterfaceCount)
    {
        USBDevice1.Stop(&USBDevice1);
        USBTargetEnablePhy(USBDevice1.HAL, false);
        USBTargetEnableInterrupt(USBDevice1.HAL, false);
    }
}

void USB1_IRQHandler(void)
{
    USBHAL1.Interrupt(&USBHAL1);
}

void USBLoop(void)
{
#if USE_COMPONENT_USBVCP
    VCPLoop();
#endif // USE_COMPONENT_USBVCP

#if USE_COMPONENT_USBHID
    HIDLoop();
#endif // USE_COMPONENT_USBHID

#if USE_COMPONENT_USBUVC
    UVCLoop();
#endif // USE_COMPONENT_USBUVC
}
