#ifndef __USBINTERFACE_H__
#define __USBINTERFACE_H__

#include "USBESL.h"

typedef void (*USBInterfaceInitType)(USBInterfaceStruct* interface);
typedef bool (*EPCallbackType)(USBInterfaceStruct* interface, uint32_t* buffer, uint32_t bytesTransfer);
typedef bool (*DeviceCallbackType)(USBInterfaceStruct* self, USBDeviceStateEnum state, USBDeviceEventEnum event);
typedef bool (*StandardRequestCallbackType)(USBInterfaceStruct* self, USBSetupTransferStruct* transfer);
typedef bool (*ClassRequestCallbackType)(USBInterfaceStruct* self, USBSetupTransferStruct* transfer);
typedef wchar_t* (*StringRequestCallbackType)(USBInterfaceStruct* self, uint8_t index);
typedef bool (*SofCallbackType)(USBInterfaceStruct* self);

struct InterfaceDescriptorStruct_s {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
};

struct EndpointDescriptorStruct_s {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
    uint8_t  bRefresh;
    uint8_t  bSynchAddress;
};

extern uint32_t GetInterfaceDescriptorSize(USBInterfaceStruct* self);
extern uint32_t GetInterfaceDescriptor(USBInterfaceStruct* self, uint8_t* descriptorBuffer);

extern bool USBInterfaceConstractor(USBInterfaceStruct* self, uint8_t iClass, uint8_t iSubClass, uint8_t iProtocol, uint8_t epCount, EPConfigStruct* epConfig);

struct USBInterfaceStruct_s {
    USBInterfaceInitType Init;

    USBDeviceStruct* Device;
    EPConfigStruct*  EPConfig;
    uint8_t          EPCount;

    uint8_t Index;
    uint8_t Class;
    uint8_t SubClass;
    uint8_t Protocol;

    uint8_t AssociateInterfaceCount;
    uint8_t AssociateSubClass;
    uint8_t AlternateCount : 4;
    uint8_t SelectedAlternate : 4;

    uint32_t InterfaceExtendDescriptorSize;
    uint8_t* InterfaceExtendDescriptor;

    wchar_t* Name;

    // Extends class callback
    DeviceCallbackType          DeviceCallback;
    StandardRequestCallbackType StandardRequestCallback;
    ClassRequestCallbackType    ClassRequestCallback;
    StringRequestCallbackType   StringRequestCallback;
    SofCallbackType             SofCallback;
};

#endif
