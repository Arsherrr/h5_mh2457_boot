#include "InterfaceDFU.h"
#include "USBDevice.h"

static void Init(InterfaceDFUStruct* self) {
    util_assert(self->Buffer != NULL);
    util_assert(self->Function.wTransferSize != 0);

    self->State  = DFUState_dfuIDLE;
    self->Status = DFUStatus_OK;
}

static bool DFURequestCallback(InterfaceDFUStruct* self, USBSetupTransferStruct* transfer) {
    bool success = false;
    switch (transfer->Packet.bRequest) {
        case DFU_DETACH:
            // Not supported
            break;
        case DFU_DNLOAD:
            // For the download process has been simplified, there is no dfuDNLOAD-SYNC state in the process.
            if (self->State == DFUState_dfuIDLE && self->Function.bmAttributes.bitCanDnload) {
                self->State   = DFUState_dfuDNLOAD_IDLE;
                self->Address = 0;
            }

            if (self->State == DFUState_dfuDNLOAD_IDLE) {
                uint16_t block  = transfer->Packet.wValue;
                uint16_t length = transfer->Packet.wLength;
                if (transfer->Buffer == NULL) {
                    transfer->Length    = length;
                    transfer->Buffer    = self->Buffer;
                    transfer->Direction = HOST_TO_DEVICE;

                    if (length == 0) {
                        self->Status = DFUStatus_OK;
                        if (self->ManifestCallback)
                            self->Status = self->ManifestCallback(self, true);

                        self->State = DFUState_dfuMANIFEST;
                    }

                    success = true;
                    break;
                }
                else {
                    self->Status = DFUStatus_OK;
                    if (self->DownloadCallback)
                        self->Status = self->DownloadCallback(self, self->base.SelectedAlternate, self->Buffer, self->Address, block, length);

                    if (self->Status != DFUStatus_OK) {
                        self->State = DFUState_dfuERROR;
                        break;
                    }

                    self->State = DFUState_dfuDNBUSY;

                    self->Address += length;

                    success = true;
                }
            }
            break;
        case DFU_UPLOAD:
            if (self->State == DFUState_dfuIDLE && self->Function.bmAttributes.bitCanUpload) {
                self->State   = DFUState_dfuUPLOAD_IDLE;
                self->Address = 0;
            }

            if (self->State == DFUState_dfuUPLOAD_IDLE) {
                uint16_t block  = transfer->Packet.wValue;
                uint16_t length = transfer->Packet.wLength;

                uint8_t** buffer = &self->Buffer;

                self->Status = DFUStatus_OK;
                if (self->UploadCallback)
                    self->Status = self->UploadCallback(self, self->base.SelectedAlternate, buffer, self->Address, block, &length);

                if (self->Status != DFUStatus_OK) {
                    self->State = DFUState_dfuERROR;
                    break;
                }

                length = MIN(length, self->Function.wTransferSize);

                if (length < transfer->Packet.wLength)
                    self->State = DFUState_dfuIDLE;

                transfer->Length    = length;
                transfer->Buffer    = *buffer;
                transfer->Direction = DEVICE_TO_HOST;

                self->Address += length;

                success = true;
            }
            else {
                self->State  = DFUState_dfuERROR;
                self->Status = DFUStatus_errFIRMWARE;
            }

            break;
        case DFU_GETSTATUS:
            self->StatusBuffer->bStatus = self->Status;
            self->StatusBuffer->bState  = self->State;

            self->StatusBuffer->bwPollTimeout = 0;
            self->StatusBuffer->iString       = 0;

            transfer->Length    = sizeof(DFUStatusStruct);
            transfer->Buffer    = self->Buffer;
            transfer->Direction = DEVICE_TO_HOST;

            if (self->State == DFUState_dfuDNBUSY)
                self->State = DFUState_dfuDNLOAD_IDLE;
            else if (self->State == DFUState_dfuMANIFEST) {
                // Manifeset done should be called after setup transfer process
                if (!transfer->Callback) {
                    transfer->Callback = self->base.Device->Setup;
                    return true;
                }

                if (self->ManifestCallback)
                    self->ManifestCallback(self, false);
            }

            success = true;
            break;
        case DFU_CLRSTATUS:
            if (self->State == DFUState_dfuERROR) {
                self->State = DFUState_dfuIDLE;
            }
            success = true;
            break;
        case DFU_GETSTATE:
            self->Buffer[0]     = self->State;
            transfer->Length    = 1;
            transfer->Buffer    = self->Buffer;
            transfer->Direction = DEVICE_TO_HOST;

            success = true;
            break;
        case DFU_ABORT:
            if (                                            //
                self->State == DFUState_dfuIDLE ||          //
                self->State == DFUState_dfuDNLOAD_SYNC ||   //
                self->State == DFUState_dfuDNLOAD_IDLE ||   //
                self->State == DFUState_dfuMANIFEST_SYNC || //
                self->State == DFUState_dfuUPLOAD_IDLE      //
            ) {
                self->State = DFUState_dfuIDLE;
            }
            transfer->Length    = 0;
            transfer->Direction = HOST_TO_DEVICE;
            success             = true;
            break;
        default:
            break;
    }
    return success;
}

bool InterfaceDFUConstractor(InterfaceDFUStruct* self) {
    *self = (InterfaceDFUStruct) {
        .base =
            {
                .Init                          = (USBInterfaceInitType)Init,
                .ClassRequestCallback          = (ClassRequestCallbackType)DFURequestCallback,
                .InterfaceExtendDescriptorSize = DFU_FUNCTIONAL_DESCRIPTOR_SIZE,
                .InterfaceExtendDescriptor     = (uint8_t*)(&self->Function),
            },

        .Function =
            {
                .bLength         = DFU_FUNCTIONAL_DESCRIPTOR_SIZE,
                .bDescriptorType = DFU_FUNCTIONAL_DESCRIPTOR_TYPE,

                .bmAttributes =
                    {
                        .bitCanDnload             = true,
                        .bitCanUpload             = true,
                        .bitManifestationTolerant = false,
                        .bitWillDetach            = true,
                    },

                .wDetachTimeOut = 0,
                .wTransferSize  = 0,
                .bcdDFUVersion  = DFU_VERSION,
            },
    };

    USBInterfaceConstractor(&self->base, 0xFE, 0x01, 0x02, 0, NULL);
    return true;
}
