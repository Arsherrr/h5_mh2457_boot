#ifndef __INTERFACECDC_H__
#define __INTERFACECDC_H__

#include "USBInterface.h"
#include "InterfaceSerial.h"

#define CDC_CLASS 0x02

#define CDC_SUBCLASS_DLCM 0x01
#define CDC_SUBCLASS_ACM  0x02
#define CDC_SUBCLASS_TCM  0x03

#define CDC_EXTEND_DESCRIPTOR_SIZE 0x13
#define CDC_LINE_CODING_SIZE       0x07
#define CDC_SET_LINE_CODING        0x20
#define CDC_GET_LINE_CODING        0x21
#define CDC_SET_CONTROL_LINE_STATE 0x22
#define CDC_SEND_BREAK             0x23

// Control Line State bits
#define CLS_DTR (1 << 0)
#define CLS_RTS (1 << 1)

#pragma pack(1)
typedef struct {
    // CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
    struct {
        uint8_t  bFunctionLength;
        uint8_t  bDescriptorType;
        uint8_t  bDescriptorSubtype;
        uint16_t bcdCDC;
    } Header;
    // Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
    struct {
        uint8_t bFunctionLength;
        uint8_t bDescriptorType;
        uint8_t bDescriptorSubtype;
        uint8_t bmCapabilities;
        uint8_t bDataInterface;
    } Management;
    // Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
    struct {
        uint8_t bFunctionLength;
        uint8_t bDescriptorType;
        uint8_t bDescriptorSubtype;
        uint8_t bmCapabilities;
    } ACM;
    // Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
    struct {
        uint8_t bFunctionLength;
        uint8_t bDescriptorType;
        uint8_t bDescriptorSubtype;
        uint8_t bMasterInterface;
        uint8_t bSlaveInterface0;
    } Union;
} CDCExtendDescriptorStruct;
#pragma pack()

typedef struct InterfaceCDCStruct_s InterfaceCDCStruct;

extern bool InterfaceCDCConstractor(InterfaceCDCStruct* self, InterfaceSerialStruct* serial);

struct InterfaceCDCStruct_s {
    USBInterfaceStruct base;
    union {
        EPConfigStruct InterfaceEPConfig[1];
        struct {
            EPConfigStruct TxEPConfig;
        };
    };

    InterfaceSerialStruct* Serial;

    union {
        CDCExtendDescriptorStruct ExtendDescriptor;

        uint32_t ExtendDescriptorData[ToUintSize(CDC_EXTEND_DESCRIPTOR_SIZE)];
    };

    union {
        // See PSTN120.pdf - 6.3.11 Table 17: Line Coding Structure
        struct {
            uint32_t DTERate;
            uint8_t  CharFormat;
            uint8_t  ParityType;
            uint8_t  DataBits;
        };
        uint32_t LineCodingData[ToUintSize(CDC_LINE_CODING_SIZE)];
    };

    // Application Callback
    void (*SetLineCodingCallback)(InterfaceCDCStruct* self, uint32_t baudrate, uint8_t stop, uint8_t parity, uint8_t bits);
};

#endif
