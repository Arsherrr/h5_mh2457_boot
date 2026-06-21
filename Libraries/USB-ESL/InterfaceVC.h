#ifndef __INTERFACEVC_H__
#define __INTERFACEVC_H__

#include "USBInterface.h"

#ifndef CONFIG_UVC_VERSION
#define CONFIG_UVC_VERSION UVC_VERSION_1_5
#endif

// Video Class Versions
#define UVC_VERSION_1_5 0x0150U
#define UVC_VERSION_1_1 0x0110U

// Video Class
#define VIDEO_CLASS                               0x0E
#define VIDEO_SUBCLASS_VIDEOCONTROL               0x01
#define VIDEO_SUBCLASS_VIDEOSTREAMING             0x02
#define VIDEO_SUBCLASS_VIDEO_INTERFACE_COLLECTION 0x03
#define VIDEO_PROTOCOL_NONE                       0x00
#define VIDEO_PROTOCOL_15                         0x01

// Video Class-Specific VC Interface Descriptor Subtypes
#define VC_HEADER_DESCRIPTOR          0x01
#define VC_INPUT_TERMINAL_DESCRIPTOR  0x02
#define VC_OUTPUT_TERMINAL_DESCRIPTOR 0x03
#define VC_SELECTOR_UNIT_DESCRIPTOR   0x04
#define VC_PROCESSING_UNIT_DESCRIPTOR 0x05
#define VC_EXTENSION_UNIT_DESCRIPTOR  0x06
#define VC_ENCODING_UNIT_DESCRIPTOR   0x07

#define VC_HEADER_DESCRIPTOR_SIZE          0x0D
#define VC_INPUT_TERMINAL_DESCRIPTOR_SIZE  0x12
#define VC_OUTPUT_TERMINAL_DESCRIPTOR_SIZE 0x09
#define VC_PROCESSING_UNIT_DESCRIPTOR_SIZE 0x0C

// B.1. USB Terminal Types
#define TT_VENDOR_SPECIFIC \
    0x0100 // A Terminal dealing with a signal carried over a vendor-specific interface.
           // The vendor-specific interface descriptor must contain a field that references the Terminal.

#define TT_STREAMING \
    0x0101 // A Terminal dealing with a signal carried over an endpoint in a VideoStreaming interface.
           // The VideoStreaming interface descriptor points to the associated Terminal through the bTerminalLinkfield.

// B.2. Input Terminal Type
#define ITT_VENDOR_SPECIFIC       0x0200 // Vendor-Specific Input Terminal.
#define ITT_CAMERA                0x0201 // Camera sensor. To be used only in Camera Terminal descriptors.
#define ITT_MEDIA_TRANSPORT_INPUT 0x0202 // Sequential media. To be used only in Media Transport Terminal Descriptors.

// Video Class-Specific VS Interface Descriptor Subtypes
#define VS_INPUT_HEADER          0x01
#define VS_OUTPUT_HEADER         0x02
#define VS_STILL_IMAGE_FRAME     0x03
#define VS_FORMAT_UNCOMPRESSED   0x04
#define VS_FRAME_UNCOMPRESSED    0x05
#define VS_FORMAT_MJPEG          0x06
#define VS_FRAME_MJPEG           0x07
#define VS_FORMAT_MPEG2TS        0x0A
#define VS_FORMAT_DV             0x0C
#define VS_COLORFORMAT           0x0D
#define VS_FORMAT_FRAME_BASED    0x10
#define VS_FRAME_FRAME_BASED     0x11
#define VS_FORMAT_STREAM_BASED   0x12
#define VS_FORMAT_H264           0x13
#define VS_FRAME_H264            0x14
#define VS_FORMAT_H264_SIMULCAST 0x15
#define VS_FORMAT_VP8            0x16
#define VS_FRAME_VP8             0x17
#define VS_FORMAT_VP8_SIMULCAST  0x18

#define VS_INPUT_HEADER_SIZE 0x0E
#define VS_FORMAT_MJPEG_SIZE 0x0B
#define VS_FRAME_MJPEG_SIZE  0x1E

// Video Class-Specific Request Codes
#define SET_CUR     0x01
#define SET_CUR_ALL 0x11
#define GET_CUR     0x81
#define GET_MIN     0x82
#define GET_MAX     0x83
#define GET_RES     0x84
#define GET_LEN     0x85
#define GET_INFO    0x86
#define GET_DEF     0x87
#define GET_CUR_ALL 0x91
#define GET_MIN_ALL 0x92
#define GET_MAX_ALL 0x93
#define GET_RES_ALL 0x94
#define GET_DEF_ALL 0x97

// VideoStreaming Interface Control Selectors
#define VS_PROBE_CONTROL                0x01
#define VS_COMMIT_CONTROL               0x02
#define VS_STILL_PROBE_CONTROL          0x03
#define VS_STILL_COMMIT_CONTROL         0x04
#define VS_STILL_IMAGE_TRIGGER_CONTROL  0x05
#define VS_STREAM_ERROR_CODE_CONTROL    0x06
#define VS_GENERATE_KEY_FRAME_CONTROL   0x07
#define VS_UPDATE_FRAME_SEGMENT_CONTROL 0x08
#define VS_SYNCH_DELAY_CONTROL          0x09

#pragma pack(1)

typedef struct {
    struct {
        uint8_t  bLength;
        uint8_t  bDescriptorType;
        uint8_t  bDescriptorSubType;
        uint16_t bcdUVC;
        uint16_t wTotalLength;
        uint32_t dwClockFrequency;
        uint8_t  bInCollection;
        uint8_t  baInterfaceNr;
    } Header;

    struct {
        uint8_t  bLength;
        uint8_t  bDescriptorType;
        uint8_t  bDescriptorSubtype;
        uint8_t  bTerminalID;
        uint16_t wTerminalType;
        uint8_t  bAssocTerminal;
        uint8_t  iTerminal;
        uint16_t wObjectiveFocalLengthMin;
        uint16_t wObjectiveFocalLengthMax;
        uint16_t wOcularFocalLength;
        uint8_t  bControlSize;
        uint32_t bmControls : 24;
    } InputTerminal;

    struct {
        uint8_t  bLength;            // Size of this descriptor, in bytes.
        uint8_t  bDescriptorType;    // CS_INTERFACE
        uint8_t  bDescriptorSubtype; // VC_OUTPUT_TERMINAL
        uint8_t  bTerminalID;        // ID of this terminal
        uint16_t wTerminalType;      // TT_STREAMING type. This terminal is a USB streaming terminal.
        uint8_t  bAssocTerminal;     // No association
        uint8_t  bSourceID;          // The input pin of this unit is connected to the output pin of unit 1 – Selector unit.
        uint8_t  iTerminal;          // Unused
    } OutputTerminal;

    struct {
        uint8_t  bLength;            // Size of this descriptor, in bytes.
        uint8_t  bDescriptorType;    // CS_INTERFACE
        uint8_t  bDescriptorSubtype; // VC_PROCESSING_UNIT
        uint8_t  bUnitID;            // ID of this unit
        uint8_t  bSourceID;          // This input pin of this unit is connected to the output pin of unit with ID 0x04.
        uint16_t wMaxMultiplier;     // unused
        uint8_t  bControlSize;       // Size of the bmControls field, in bytes.
        uint16_t bmControls;         // Brightness control supported
        uint8_t  iProcessing;        // Unused
        uint8_t  bmVideoStandards;   // Unused
    } ProcessingUnit;
} VCClassDescriptorStruct;

#pragma pack()

typedef struct InterfaceVCStruct_s InterfaceVCStruct;

extern bool InterfaceVCConstractor(InterfaceVCStruct* self);

struct InterfaceVCStruct_s {
    USBInterfaceStruct base;

    VCClassDescriptorStruct ClassDescriptor;

    bool IsMute;
};

#endif
