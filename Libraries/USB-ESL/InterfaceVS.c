#include "USBDevice.h"
#include "InterfaceVS.h"

static void Init(InterfaceVSStruct* self) {
    USBSpeedEnum speed                     = self->base.Device->HAL->Speed;
    self->Control.dwMaxPayloadTransferSize = self->InputEPConfig.MaxPacket;
    if (speed == USBSpeedHigh) {
        self->Control.dwMaxPayloadTransferSize *= self->InputEPConfig.AdditionalPacket + 1;
    }
    else {
        self->Control.dwMaxPayloadTransferSize = MIN(self->Control.dwMaxPayloadTransferSize, 1020);
    }
}

static void FillPayload(InterfaceVSStruct* self, VSPayloadStruct* payload) {
    payload->Length = MIN((self->Size - self->Offset), self->Control.dwMaxPayloadTransferSize - CONFIG_USBESL_UVC_HLE);

    if (payload->Length == 0) {
        self->Buffer         = NULL;
        self->Offset         = 0;
        payload->Header.bEOF = 0;
        payload->Header.bFID = 0;
        return;
    }

    memcpy(payload->Data, &self->Buffer[self->Offset], payload->Length);
    self->Offset += payload->Length;
    payload->Header.bFID = 1;
    if (self->Offset == self->Size) {
        payload->Header.bEOF = 1;
    }
    else {
        payload->Header.bEOF = 0;
    }
}

static bool IEPCallback(InterfaceVSStruct* self, uint32_t* buffer, uint32_t bytesRead) {
    VSPayloadStruct* currentPayload = self->Payload + self->PayloadIndex;
    VSPayloadStruct* nextPayload    = self->Payload + !self->PayloadIndex;

    if (self->Buffer) {
        if (self->Offset == 0) {
            FillPayload(self, currentPayload);
        }
        USBHALStruct* hal = self->base.Device->HAL;
        hal->EndpointStartWrite(hal, self->InputEPConfig.Index, (uint32_t*)currentPayload, CONFIG_USBESL_UVC_HLE + currentPayload->Length);

        FillPayload(self, nextPayload);

        self->PayloadIndex = !self->PayloadIndex;
    }
    else {
        USBHALStruct* hal = self->base.Device->HAL;
        hal->EndpointStartWrite(hal, self->InputEPConfig.Index, (uint32_t*)currentPayload, CONFIG_USBESL_UVC_HLE + currentPayload->Length);
    }

    return true;
}

static bool ClassRequestCallback(InterfaceVSStruct* self, USBSetupTransferStruct* transfer) {
    bool success = false;

    uint8_t controlID = transfer->Packet.wIndex >> 8;

    uint8_t* descriptorBuffer = (uint8_t*)&self->base.Device->Descriptor;
    switch (transfer->Packet.bRequest) {
        case GET_CUR:
        case GET_MIN:
        case GET_MAX:
            transfer->Length    = sizeof(VSConrtolsStruct);
            transfer->Buffer    = (uint8_t*)(&self->Control);
            transfer->Direction = DEVICE_TO_HOST;
            success             = true;
            break;
        case SET_CUR:
            if (transfer->Buffer == NULL) {
                transfer->Length    = sizeof(VSConrtolsStruct);
                transfer->Buffer    = descriptorBuffer;
                transfer->Direction = HOST_TO_DEVICE;
            }
            else {
                VSConrtolsStruct* setControl = (VSConrtolsStruct*)descriptorBuffer;
            }
            success = true;
            break;
        default:
            break;
    }

    return success;
}

static bool DeviceCallback(InterfaceVSStruct* self, USBDeviceStateEnum state, USBDeviceEventEnum event) {
    switch (event) {
        case USBDeviceSetInterfaceEvent:
            if (self->base.SelectedAlternate) {
                self->Buffer = NULL;
                self->Size   = 0;
                self->Offset = 0;
                FillPayload(self, self->Payload);
            }

            USBHALStruct* hal = self->base.Device->HAL;
            hal->EndpointStartWrite(hal, self->InputEPConfig.Index, NULL, 0);
            break;
        default:
            break;
    }
    return true;
}

bool InterfaceVSConstractor(InterfaceVSStruct* self) {
    memset(self, 0, sizeof(InterfaceVSStruct));

    self->base.Init = (USBInterfaceInitType)Init;

    self->InputEPConfig = (EPConfigStruct) {
        .Index            = 1,
        .Direct           = EPDirectIn,
        .Type             = EPTypeISO,
        .MaxPacket        = MIN(VS_MAX_PACKET_SIZE, 1024),
        .MaxBuffer        = NULL,
        .Interval         = 1,
        .HSInterval       = 1,
        .AdditionalBuffer = 1,
        .AdditionalPacket = 0,
        .Buffer           = NULL,
        .Callback         = (EPCallbackType)IEPCallback,
    };

    self->base.AlternateCount = 1;

    self->base.DeviceCallback       = (DeviceCallbackType)DeviceCallback;
    self->base.ClassRequestCallback = (ClassRequestCallbackType)ClassRequestCallback;

    self->base.InterfaceExtendDescriptorSize = sizeof(VSClassDescriptorStruct);
    self->base.InterfaceExtendDescriptor     = (uint8_t*)(&self->ClassDescriptor);

    self->ClassDescriptor = (VSClassDescriptorStruct) {
        .Header =
            {
                .bLength             = VS_INPUT_HEADER_SIZE,
                .bDescriptorType     = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype  = VS_INPUT_HEADER,
                .bNumFormats         = 0x01,
                .wTotalLength        = sizeof(VSClassDescriptorStruct),
                .bEndpointAddress    = 0x81,
                .bmInfo              = 0x00,
                .bTerminalLink       = 2,
                .bStillCaptureMethod = 0x00,
                .bTriggerSupport     = 0x00,
                .bTriggerUsage       = 0x00,
                .bControlSize        = 0x01,
                .bmaControls         = 0x00,
            },
        .Format =
            {
                .bLength              = VS_FORMAT_MJPEG_SIZE,
                .bDescriptorType      = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype   = VS_FORMAT_MJPEG,
                .bFormatIndex         = 0x01,
                .bNumFrameDescriptors = 0x01,
                .bmFlags              = 0x01,
                .bDefaultFrameIndex   = 0x01,
                .bAspectRatioX        = 0x00,
                .bAspectRatioY        = 0x00,
                .bmInterlaceFlags     = 0x02,
                .bCopyProtect         = 0x00,
            },
        .Frame =
            {
                .bLength                = VS_FRAME_MJPEG_SIZE,
                .bDescriptorType        = CS_INTERFACE_DESCRIPTOR,
                .bDescriptorSubtype     = VS_FRAME_MJPEG,
                .bFrameIndex            = 0x01,
                .bmCapabilities         = 0x02,
                .wWidth                 = 1920,
                .wHeight                = 1080,
                .dwMinBitRate           = 28800,
                .dwMaxBitRate           = 102400,
                .dwMaxVideoFrameBufSize = 0x00000800,
                .dwDefaultFrameInterval = 500000,
                .bFrameIntervalType     = 0x01,
                .dwFrameInterval        = 500000,
            },
    };

    self->Control = (VSConrtolsStruct) {
        .bFormatIndex             = 1,
        .bFrameIndex              = 1,
        .dwFrameInterval          = 500000,
        .dwMaxVideoFrameSize      = 524288,
        .dwMaxPayloadTransferSize = VS_MAX_PACKET_SIZE,
    };

    self->Payload[0].Header.HLE = CONFIG_USBESL_UVC_HLE;
    self->Payload[1].Header.HLE = CONFIG_USBESL_UVC_HLE;

    USBInterfaceConstractor((USBInterfaceStruct*)self, VIDEO_CLASS, VIDEO_SUBCLASS_VIDEOSTREAMING, 0x00, 1, self->InterfaceEPConfig);

    return true;
}
