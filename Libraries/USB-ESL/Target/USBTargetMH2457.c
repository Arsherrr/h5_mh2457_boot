#include "USBCoreZoffyDefine.h"
#include "USBESL.h"

inline void USBTargetConstractor(USBHALStruct* hal) {
    if (hal->USBBase == USB_ZOFFY1_BASE) {
        hal->IsDMAEnabled    = false;
        hal->IsDeviceEnabled = true;
        hal->IsHostEnabled   = false;
        hal->IsSRPEnabled    = false;
    }
    else {
        hal->IsDMAEnabled    = true;
        hal->IsDeviceEnabled = true;
        hal->IsHostEnabled   = false;
        hal->IsSRPEnabled    = false;
        hal->PhyType         = USBPhyTypeUTMI;
        hal->SpeedConfig     = USBSpeedConfigHigh;
    }
}

inline void USBTargetDelayUs(uint32_t us) {
    SystemDelayUs(us);
}

inline void USBTargetEnablePhy(USBHALStruct* hal, bool isEnable) {
    if (!isEnable || hal->PhyType == USBPhyTypeUTMI) {
        hal->Zoffy.USB->GREGS.GCFG = 0;
    }
    else {
        hal->Zoffy.USB->GREGS.GCFG = BIT(16); // Power ON

        if (!(hal->IsHNPEnabled || hal->IsSRPEnabled))
            hal->Zoffy.USB->GREGS.GCFG |= BIT(21); // NOVUBS
        else
            hal->Zoffy.USB->GREGS.GCFG |= BIT(19) | BIT(18); // VBUS A|B EN

        if (hal->PhyType == USBPhyTypeFSI2C) {
            hal->Zoffy.USB->GREGS.GI2CCTL = //
                BIT(26) |                   // I2C Addr 0x2D
                BIT(23);                    // I2C Enable
        }
        if (hal->USBBase == USB_ZOFFY1_BASE) {
            hal->Zoffy.MaxFifoSize = 0x140; // Fix FIFO size of USB 1 to 1280 bytes
        }
    }
}

inline void USBTargetEnableInterrupt(USBHALStruct* hal, bool isEnable) {
    IRQn_Type irq = USB_ZOFFY1_IRQ;
    if (isEnable) {
        NVIC_EnableIRQ(irq);
    }
    else {
        NVIC_DisableIRQ(irq);
    }
}

inline void USBTargetEnableModule(USBHALStruct* hal, bool isEnable) {
    if (hal->USBBase == USB_ZOFFY1_BASE) {
        // Configure DM DP Pins
        IOSetup(PA11, USB_IO_CONFIG_DATA);
        IOSetup(PA12, USB_IO_CONFIG_DATA);

        // Configure ID Pin
        if (hal->IsDeviceEnabled && hal->IsHostEnabled) {
            IOSetup(PA10, USB_IO_CONFIG_ID);
        }

        // Configure VBUS Pin
        if (hal->IsSRPEnabled || hal->IsHNPEnabled) {
            IOSetup(PA9, USB_IO_CONFIG_VBUS);
        }
    }

    if (isEnable && (ClockGet(ClockNodeREF) / 1000000 != 48)) {
        ClockMultiply(ClockNodePLL3, ClockRatio(768 / 12.0));
        ClockDivide(ClockNodePLL3R, ClockRatio(768 / 48.0));
        ClockSelect(ClockNodeREF, ClockNodePLL3R);
        ClockEnable(ClockNodePLL3G, true);
    }

    PeripheralEnable(PeripheralUSB1, isEnable);
}

inline void USBTargetResetModule(USBHALStruct* hal) {
    PeripheralReset(PeripheralUSB1);
}
inline uint8_t USBTargetGetSN(uint8_t* sn, uint8_t length) {
    // TODO
    return 0;
}