#ifndef __INTERFACEDFU_H__
#define __INTERFACEDFU_H__

#include "USBInterface.h"

#define DFU_FUNCTIONAL_DESCRIPTOR_SIZE (0x09)
#define DFU_FUNCTIONAL_DESCRIPTOR_TYPE (0x21)
#define DFU_VERSION                    (0x0110)

#define DFU_DETACH    (0x00)
#define DFU_DNLOAD    (0x01)
#define DFU_UPLOAD    (0x02)
#define DFU_GETSTATUS (0x03)
#define DFU_CLRSTATUS (0x04)
#define DFU_GETSTATE  (0x05)
#define DFU_ABORT     (0x06)

typedef enum {
    DFUStatus_OK              = 0x00,
    DFUStatus_errTARGET       = 0x01,
    DFUStatus_errFILE         = 0x02,
    DFUStatus_errWRITE        = 0x03,
    DFUStatus_errERASE        = 0x04,
    DFUStatus_errCHECK_ERASED = 0x05,
    DFUStatus_errPROG         = 0x06,
    DFUStatus_errVERIFY       = 0x07,
    DFUStatus_errADDRESS      = 0x08,
    DFUStatus_errNOTDONE      = 0x09,
    DFUStatus_errFIRMWARE     = 0x0A,
    DFUStatus_errVENDOR       = 0x0B,
    DFUStatus_errUSBR         = 0x0C,
    DFUStatus_errPOR          = 0x0D,
    DFUStatus_errUNKNOWN      = 0x0E,
    DFUStatus_errSTALLEDPKT   = 0x0F,
} DFUStatusEnum;

typedef enum {
    DFUState_appIDLE                = 0x00,
    DFUState_appDETACH              = 0x01,
    DFUState_dfuIDLE                = 0x02,
    DFUState_dfuDNLOAD_SYNC         = 0x03,
    DFUState_dfuDNBUSY              = 0x04,
    DFUState_dfuDNLOAD_IDLE         = 0x05,
    DFUState_dfuMANIFEST_SYNC       = 0x06,
    DFUState_dfuMANIFEST            = 0x07,
    DFUState_dfuMANIFEST_WAIT_RESET = 0x08,
    DFUState_dfuUPLOAD_IDLE         = 0x09,
    DFUState_dfuERROR               = 0x0A,
} DFUStateEnum;

#pragma pack(1)
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    struct {
        bool bitCanDnload : 1;
        bool bitCanUpload : 1;
        bool bitManifestationTolerant : 1;
        bool bitWillDetach : 1;

        uint8_t bitReserved : 4;
    } bmAttributes;
    uint16_t wDetachTimeOut;
    uint16_t wTransferSize;
    uint16_t bcdDFUVersion;
} DFUFunctionalDescriptorStruct;

typedef struct {
    uint8_t  bStatus;
    uint32_t bwPollTimeout : 24;
    uint8_t  bState;
    uint8_t  iString;
} DFUStatusStruct;
#pragma pack()

typedef struct InterfaceDFUStruct_s InterfaceDFUStruct;

extern bool InterfaceDFUConstractor(InterfaceDFUStruct* self);

struct InterfaceDFUStruct_s {
    USBInterfaceStruct base;

    DFUStatusEnum (*UploadCallback)(InterfaceDFUStruct* self, uint8_t alternate, uint8_t** buffer, uint32_t address, uint16_t block, uint16_t* length);
    DFUStatusEnum (*DownloadCallback)(InterfaceDFUStruct* self, uint8_t alternate, uint8_t* buffer, uint32_t address, uint16_t block, uint16_t length);
    DFUStatusEnum (*ManifestCallback)(InterfaceDFUStruct* self, bool isProcessing);

    DFUFunctionalDescriptorStruct Function;

    DFUStateEnum  State;
    DFUStatusEnum Status;

    uint32_t Address;
    union {
        uint8_t* Buffer;

        DFUStatusStruct* StatusBuffer;
    };
};

#endif
