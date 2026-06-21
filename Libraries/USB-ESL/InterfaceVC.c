#include "USBDevice.h"
#include "InterfaceVC.h"
#include "InterfaceVS.h"

static void Init(InterfaceVCStruct* self) {
    self->ClassDescriptor.Header.baInterfaceNr = self->base.Index + 1;
}

bool InterfaceVCConstractor(InterfaceVCStruct* self) {
    memset(self, 0, sizeof(InterfaceVCStruct));

    self->base.Init = (USBInterfaceInitType)Init;

    // USB Video shall have at least two interfaces: control + streaming
    self->base.AssociateInterfaceCount       = 2;
    self->base.AssociateSubClass             = VIDEO_SUBCLASS_VIDEO_INTERFACE_COLLECTION;
    self->base.InterfaceExtendDescriptorSize = sizeof(VCClassDescriptorStruct);
    self->base.InterfaceExtendDescriptor     = (uint8_t*)(&self->ClassDescriptor);

    self->ClassDescriptor = (VCClassDescriptorStruct) {
        .Header =
            {
                .bLength            = VC_HEADER_DESCRIPTOR_SIZE,
                .bDescriptorType    = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubType = VC_HEADER_DESCRIPTOR,
                .bcdUVC             = CONFIG_UVC_VERSION,
                .wTotalLength       = sizeof(VCClassDescriptorStruct),
                .dwClockFrequency   = 6000000,
                .bInCollection      = 0x01,
                .baInterfaceNr      = 0, // filled during Init()
            },
        .InputTerminal =
            {
                .bLength                  = VC_INPUT_TERMINAL_DESCRIPTOR_SIZE,
                .bDescriptorType          = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype       = VC_INPUT_TERMINAL_DESCRIPTOR,
                .bTerminalID              = 1,
                .wTerminalType            = ITT_CAMERA,
                .bAssocTerminal           = 0,
                .iTerminal                = 0,
                .wObjectiveFocalLengthMin = 0,
                .wObjectiveFocalLengthMax = 0,
                .wOcularFocalLength       = 0,
                .bControlSize             = 0x03,
                .bmControls               = 0,
            },
        .OutputTerminal =
            {
                .bLength            = VC_OUTPUT_TERMINAL_DESCRIPTOR_SIZE,
                .bDescriptorType    = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype = VC_OUTPUT_TERMINAL_DESCRIPTOR,
                .bTerminalID        = 2,
                .wTerminalType      = TT_STREAMING,
                .bAssocTerminal     = 0,
                .bSourceID          = 1,
                .iTerminal          = 0,
            },
        .ProcessingUnit =
            {
                .bLength            = VC_PROCESSING_UNIT_DESCRIPTOR_SIZE,
                .bDescriptorType    = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype = VC_PROCESSING_UNIT_DESCRIPTOR,
                .bUnitID            = 3,
                .bSourceID          = 1,
                .wMaxMultiplier     = 0,
                .bControlSize       = 2,
                .bmControls         = 0,
                .iProcessing        = 0,
                .bmVideoStandards   = 0,
            },
    };

    USBInterfaceConstractor((USBInterfaceStruct*)self, VIDEO_CLASS, VIDEO_SUBCLASS_VIDEOCONTROL, VIDEO_PROTOCOL_15, 0, NULL);

    return true;
}
