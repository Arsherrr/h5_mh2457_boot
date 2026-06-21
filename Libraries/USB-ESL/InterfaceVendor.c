#include "InterfaceVendor.h"
#include "USBDevice.h"

static void Init(InterfaceVendorStruct* self) {}

static bool VendorRequestCallback(InterfaceVendorStruct* self, USBSetupTransferStruct* transfer) {
    bool success = false;

    if (transfer->Packet.bmRequestType.Type != VENDOR_TYPE)
        return success;

    uint8_t* deviceVendorBuffer = (uint8_t*)self->base.Device->Descriptor.Vendor;

    if (transfer->Packet.bmRequestType.dataTransferDirection) { // Device to Host: Vendor In
        transfer->Length = transfer->Packet.wLength;
        if (self->InputDataCallback && self->InputDataCallback(self, deviceVendorBuffer, &transfer->Length)) {
            transfer->Buffer    = deviceVendorBuffer;
            transfer->Direction = DEVICE_TO_HOST;
            success             = true;
        }
    }
    else { // Host to Device: Vendor Out
        if (transfer->Buffer == NULL) {
            // Setup request: Prepare buffer to save data
            transfer->Length    = transfer->Packet.wLength;
            transfer->Buffer    = deviceVendorBuffer;
            transfer->Direction = HOST_TO_DEVICE;
            success             = true;
        }
        else {
            // Data phase completed: Data saved
            if (self->OutputDataCallback && self->OutputDataCallback(self, deviceVendorBuffer, transfer->Packet.wLength)) {
                success = true;
            }
        }
    }

    return success;
}

bool InterfaceVendorConstractor(InterfaceVendorStruct* self) {
    memset(self, 0, sizeof(InterfaceVendorStruct));

    self->base.Init = (USBInterfaceInitType)Init;

    self->base.ClassRequestCallback = (ClassRequestCallbackType)VendorRequestCallback;

    USBInterfaceConstractor((USBInterfaceStruct*)self, 0xFF, 0x00, 0x00, 0, NULL);
    return true;
}
