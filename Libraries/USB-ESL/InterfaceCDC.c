#include "InterfaceCDC.h"
#include "USBDevice.h"

static void Init(InterfaceCDCStruct* self) {
    self->DTERate    = 115200;
    self->CharFormat = 0;
    self->ParityType = 0;
    self->DataBits   = 8;

    self->base.EPCount = self->TxEPConfig.Index ? 1 : 0;
}

static bool ClassRequestCallback(InterfaceCDCStruct* self, USBSetupTransferStruct* transfer) {
    bool success = false;
    switch (transfer->Packet.bRequest) {
        case CDC_GET_LINE_CODING:
            transfer->Length    = CDC_LINE_CODING_SIZE;
            transfer->Buffer    = (uint8_t*)self->LineCodingData;
            transfer->Direction = DEVICE_TO_HOST;
            success             = true;
            break;
        case CDC_SET_LINE_CODING:
            // Setup request
            if (transfer->Buffer == NULL) {
                transfer->Length    = CDC_LINE_CODING_SIZE;
                transfer->Buffer    = (uint8_t*)self->LineCodingData;
                transfer->Direction = HOST_TO_DEVICE;
                success             = true;
                break;
            }
            // Data phase completed
            if (self->SetLineCodingCallback) {
                self->SetLineCodingCallback(self, self->DTERate, self->CharFormat, self->ParityType, self->DataBits);
            }
            success = true;
            break;
        case CDC_SET_CONTROL_LINE_STATE: {
            self->Serial->StartProbe(self->Serial);
            success = true;
        } break;
        case CDC_SEND_BREAK:
            success = true;
            break;
        default:
            break;
    }
    return success;
}

bool InterfaceCDCConstractor(InterfaceCDCStruct* self, InterfaceSerialStruct* serial) {
    memset(self, 0, sizeof(InterfaceCDCStruct));

    self->base.Init = (USBInterfaceInitType)Init;

    self->base.AssociateInterfaceCount       = 2;
    self->base.AssociateSubClass             = CDC_SUBCLASS_ACM;
    self->base.ClassRequestCallback          = (ClassRequestCallbackType)ClassRequestCallback;
    self->base.InterfaceExtendDescriptorSize = CDC_EXTEND_DESCRIPTOR_SIZE;
    self->base.InterfaceExtendDescriptor     = (uint8_t*)self->ExtendDescriptorData;

    self->TxEPConfig = (EPConfigStruct) {
        .Index      = 0,
        .Direct     = EPDirectIn,
        .Type       = EPTypeInterrupt,
        .Interval   = 0x80,
        .HSInterval = 0x0B,
        .MaxPacket  = 16,
        .MaxBuffer  = 0,
        .Buffer     = NULL,
        .Callback   = NULL,
    };

    self->Serial = serial;

    self->ExtendDescriptor = (CDCExtendDescriptorStruct) {
        .Header =
            {
                .bFunctionLength    = 0x05,
                .bDescriptorType    = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype = 0x00,
                .bcdCDC             = 0x0110,
            },

        .Management =
            {

                .bFunctionLength    = 0x05,
                .bDescriptorType    = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype = 0x01,
                .bmCapabilities     = 0x03,
                .bDataInterface     = 0x01,
            },

        .ACM =
            {
                .bFunctionLength    = 0x04,
                .bDescriptorType    = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype = 0x02,
                .bmCapabilities     = 0x06,
            },

        .Union =
            {
                .bFunctionLength    = 0x05,
                .bDescriptorType    = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype = 0x06,
                .bMasterInterface   = 0x00,
                .bSlaveInterface0   = 0x01,
            },
    };

    USBInterfaceConstractor((USBInterfaceStruct*)self, CDC_CLASS, CDC_SUBCLASS_ACM, 0x00, 0, self->InterfaceEPConfig);
    return true;
}
