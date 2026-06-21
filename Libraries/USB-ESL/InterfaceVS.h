#ifndef __INTERFACEVS_H__
#define __INTERFACEVS_H__

#include "USBInterface.h"
#include "InterfaceVC.h"

#ifndef CONFIG_USBESL_UVC_MAX_PAYLOAD
#define CONFIG_USBESL_UVC_MAX_PAYLOAD 3060U
#endif

#ifndef CONFIG_USBESL_UVC_HLE
#define CONFIG_USBESL_UVC_HLE 12U
#endif

#define VS_MAX_PACKET_SIZE (CONFIG_USBESL_UVC_HLE + CONFIG_USBESL_UVC_MAX_PAYLOAD)

#pragma pack(1)

typedef struct {
    struct {
        uint8_t  bLength;             // Size of this descriptor, in bytes.
        uint8_t  bDescriptorType;     // CS_INTERFACE
        uint8_t  bDescriptorSubtype;  // VS_INPUT_HEADER
        uint8_t  bNumFormats;         // One format descriptor follows.
        uint16_t wTotalLength;        // Total size of class-specific VideoStreaming interface descriptors
        uint8_t  bEndpointAddress;    // Address of the isochronous endpoint used for video data
        uint8_t  bmInfo;              // No dynamic format change supported.
        uint8_t  bTerminalLink;       // This VideoStreaming interface supplies terminal ID 4 (Output Terminal).
        uint8_t  bStillCaptureMethod; // Device supports still image capture method 3.
        uint8_t  bTriggerSupport;     // Hardware trigger is not supported for still image capture.
        uint8_t  bTriggerUsage;       // Hardware trigger should initiate a still image capture.
        uint8_t  bControlSize;        // Size of the bmaControls field.
        uint8_t  bmaControls;         // No VideoStreaming specific controls are supported.
    } Header;

    struct {
        uint8_t bLength;              // Size of this descriptor, in bytes.
        uint8_t bDescriptorType;      // CS_INTERFACE
        uint8_t bDescriptorSubtype;   // VS_FORMAT_MJPEG
        uint8_t bFormatIndex;         // First (and only) format descriptor
        uint8_t bNumFrameDescriptors; // One frame descriptor for this format follows
        uint8_t bmFlags;              // Uses fixed size samples.
        uint8_t bDefaultFrameIndex;   // Default frame index is 1.
        uint8_t bAspectRatioX;        // Non-interlaced stream – not required.
        uint8_t bAspectRatioY;        // Non-interlaced stream – not required.
        uint8_t bmInterlaceFlags;     // Progressive
        uint8_t bCopyProtect;         // No restrictions imposed on the duplication of this video stream.
    } Format;

    struct {
        uint8_t  bLength;                // Size of this descriptor, in bytes.
        uint8_t  bDescriptorType;        // CS_INTERFACE
        uint8_t  bDescriptorSubtype;     // VS_FRAME_MJPEG
        uint8_t  bFrameIndex;            // First (and only) frame descriptor
        uint8_t  bmCapabilities;         // D1: Fixed frame-rate.
        uint16_t wWidth;                 // Width of frame
        uint16_t wHeight;                // Height of frame
        uint32_t dwMinBitRate;           // Min bit rate in bits/s
        uint32_t dwMaxBitRate;           // Max bit rate in bits/s
        uint32_t dwMaxVideoFrameBufSize; // Maximum video or still frame size, in bytes.
        uint32_t dwDefaultFrameInterval; // Default frame interval
        uint8_t  bFrameIntervalType;     // Discrete frame interval
        uint32_t dwFrameInterval;        // Minimum frame interval
    } Frame;

    /*
    struct {
        uint8_t  bLength;               // Size of this descriptor, in bytes.
        uint8_t  bDescriptorType;       // CS_INTERFACE
        uint8_t  bDescriptorSubtype;    // VS_STILL_FRAME
        uint8_t  bEndpointAddress;      // IN Endpoint, Endpoint number 6
        uint8_t  bNumImageSizePatterns; // Number of Image Size patterns of this format: 2
        uint15_t wWidth;                // Width of frame is 800 pixels
        uint16_t wHeight;               // Height of frame is 600 pixels
        uint8_t  bNumCompressionPtn;    // Number of Compression pattern of this format: 1
        uint8_t  bCompression;          // Compression of the still image in pattern 1: 100
    } StillImageFrame;
    */
} VSClassDescriptorStruct;

typedef struct {
    uint16_t bmHint;
    uint8_t  bFormatIndex;
    uint8_t  bFrameIndex;
    uint32_t dwFrameInterval;
    uint16_t wKeyFrameRate;
    uint16_t wPFrameRate;
    uint16_t wCompQuality;
    uint16_t wCompWindowSize;
    uint16_t wDelay;
    uint32_t dwMaxVideoFrameSize;
    uint32_t dwMaxPayloadTransferSize;
    uint32_t dwClockFrequency;
    uint8_t  bmFramingInfo;
    uint8_t  bPreferedVersion;
    uint8_t  bMinVersion;
    uint8_t  bMaxVersion;
#if CONFIG_UVC_VERSION == UVC_VERSION_1_5
    uint8_t  bUsage;
    uint8_t  bBitDepthLuma;
    uint8_t  bmSettings;
    uint8_t  bMaxNumberOfRefFramesPlus1;
    uint16_t bmRateControlModes;
    uint64_t bmLayoutPerStream;
#endif
} VSConrtolsStruct;

typedef struct {
    struct {
        uint16_t HLE : 8;

        bool bFID : 1;
        bool bEOF : 1;
        bool bPTS : 1;
        bool bSCR : 1;
        bool bREG : 1;
        bool bSTI : 1;
        bool bERR : 1;
        bool bEOH : 1;
    } Header;

    uint16_t Length;
    uint32_t Padding[(CONFIG_USBESL_UVC_HLE - 4) / 4];
    uint32_t Data[ToUintSize(CONFIG_USBESL_UVC_MAX_PAYLOAD)];
} VSPayloadStruct;

#pragma pack()

typedef struct InterfaceVSStruct_s InterfaceVSStruct;

extern bool InterfaceVSConstractor(InterfaceVSStruct* self);

struct InterfaceVSStruct_s {
    USBInterfaceStruct base;
    union {
        EPConfigStruct InterfaceEPConfig[1];
        EPConfigStruct InputEPConfig;
    };

    uint32_t (*IutputCallback)(uint8_t* buffer, uint32_t length);

    VSClassDescriptorStruct ClassDescriptor;

    __ALIGNED(4) VSPayloadStruct Payload[2];

    VSConrtolsStruct Control;

    uint8_t* Buffer;
    uint32_t Size;
    uint32_t Offset;
    uint8_t  PayloadIndex;
};

#endif
