#ifndef __INTERFACEVENDOR_H__
#define __INTERFACEVENDOR_H__

#include "USBInterface.h"

typedef struct InterfaceVendorStruct_s InterfaceVendorStruct;

extern bool InterfaceVendorConstractor(InterfaceVendorStruct* self);

struct InterfaceVendorStruct_s {
    USBInterfaceStruct base;

    bool (*InputDataCallback)(InterfaceVendorStruct* self, uint8_t* inputData, uint16_t* bytesToWrite);
    bool (*OutputDataCallback)(InterfaceVendorStruct* self, uint8_t* outputData, uint16_t bytesRead);
};

#endif
