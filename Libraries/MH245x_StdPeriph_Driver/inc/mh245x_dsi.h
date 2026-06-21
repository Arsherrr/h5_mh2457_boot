// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_DSI_H__
#define __PERIPH_DSI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mh2457.h"

/* --------  Feature Definition  -------------------------------------------- */

#ifndef FEATURE_DSI_SW_LPCMD
#define FEATURE_DSI_SW_LPCMD 0
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    volatile uint32_t VERSION;    // 0000h: Core Version.
    volatile uint32_t PWRUP;      // 0004h: Power up.
    volatile uint32_t CLKCFG;     // 0008h: Factor for internal dividers.
    volatile uint32_t DPIVCID;    // 000Ch: DPI Virtual Channel ID.
    volatile uint32_t DPICOLOR;   // 0010h: DPI color coding.
    volatile uint32_t DPICFG;     // 0014h: DPI signals polarity.
    volatile uint32_t DPILPTIM;   // 0018h: Low Power commands' timing.
    volatile uint32_t DBIVCID;    // 001Ch: DBI Virtual Channel ID.
    volatile uint32_t DBICFG;     // 0020h: DBI pixels bit-width.
    volatile uint32_t DBIPARTEN;  // 0024h: DBI partition.
    volatile uint32_t DBICMDSIZE; // 0028h: DBI command size.
    volatile uint32_t PCKCFG;     // 002Ch: EoTp, BTA, CRC and ECC Configuration.
    volatile uint32_t GVCID;      // 0030h: Read responses Virtual Channel ID.
    volatile uint32_t MODE;       // 0034h: Operation mode.

    volatile uint32_t VMODE;  // 0038h: Video mode configuration.
    volatile uint32_t PKTS;   // 003Ch: Video Packets size.
    volatile uint32_t CHUNKS; // 0040h: Number of Chunks.
    volatile uint32_t NPKTS;  // 0034h: Null Packets size.
    volatile uint32_t HSA;    // 0048h: HSA time.
    volatile uint32_t HBP;    // 004Ch: HBP time.
    volatile uint32_t HLINE;  // 0050h: Overall video line time.
    volatile uint32_t VSA;    // 0054h: VSA period.
    volatile uint32_t VBP;    // 0058h: VBP period.
    volatile uint32_t VFP;    // 005Ch: VFP period.
    volatile uint32_t VACT;   // 0060h: Video vertical resolution.

    volatile uint32_t ECMDSIZE; // 0064h: eDPI packets size.
    volatile uint32_t CMDMODE;  // 0068h: Command Mode operation configuration.
    volatile uint32_t GHDR;     // 006Ch: Generic Interface Packet Header.
    volatile uint32_t GPLD;     // 0070h: Generic Interface Packets Payload.
    volatile uint32_t GSTS;     // 0074h: Generic Interface and DBI FIFO status.

    volatile uint32_t TRIGTO; // 0078h: Timeout Trigger Configuration.
    volatile uint32_t HSRDTO; // 007Ch: Peripheral timeout after HS read operations.
    volatile uint32_t LPRDTO; // 0080h: Peripheral timeout after LP read operations.
    volatile uint32_t HSWRTO; // 0084h: Peripheral timeout after HS write operations.
    volatile uint32_t LPWRTO; // 0088h: Peripheral timeout after LP write operations.
    volatile uint32_t BTATO;  // 008Ch: Peripheral timeout after BTA completion.

    volatile uint32_t SDF3D;    // 0090h: 3D information for VSS packets.
    volatile uint32_t CLCFG;    // 0094h: Non-continuous Clock configuration.
    volatile uint32_t PHYCLTIM; // 0098h: Time configuration for (clock lane) transitions between HS and LP.
    volatile uint32_t PHYDLTIM; // 009Ch: Time configuration for (data lanes) transitions between HS and LP.

    volatile uint32_t PHYCR;    // 00A0h: D-PHY's PLL and Resets.
    volatile uint32_t PHYCFG;   // 00A4h: Active lanes and Stop State minimum time in Stop State.
    volatile uint32_t PHYUCR;   // 00A8h: Transitions from and to ULPS, using D-PHY.
    volatile uint32_t PHYTTRIG; // 00ACh: Pins related to D-PHY triggers.
    volatile uint32_t PHYSTS;   // 00B0h: D-PHY Status.
    volatile uint32_t PHYVCR;   // 00B4h: D-PHY control register.
    volatile uint32_t PHYVDR;   // 00B8h: D-PHY data register.

    volatile uint32_t INTSTS0; // 00BCh: Interrupts status 0.
    volatile uint32_t INTSTS1; // 00C0h: Interrupts status 1.
    volatile uint32_t INTMSK0; // 00C4h: Interrupts mask 0.
    volatile uint32_t INTMSK1; // 00C8h: Interrupts mask 1.
    volatile uint32_t PHYCAL;  // 00CCh: D-PHY skew calibration.

    volatile uint32_t Reserved00D0[2]; // 00D0h: Reserved
    volatile uint32_t INTFORCE0;       // 00D8h: Force INTSTS0
    volatile uint32_t INTFORCE1;       // 00DCh: Force INTSTS1
    volatile uint32_t AUCR;            // 00E0h: Automatic ULPS control.
    volatile uint32_t AUDELAY;         // 00E4h: ULPS transition delay.
    volatile uint32_t AUWAKEUP;        // 00E8h: D-PHY wakeup time.
    volatile uint32_t DSCCR;           // 00F0h: Display Stream Compression.
    volatile uint32_t PHYRDTIM;        // 00F4h: PHY timings - Maximum time required to perform a read command.
    volatile uint32_t AUMINTIM;        // 00F8h: PHY Timings - Transition between ulpsactivenot and ulpsexitreq.
    volatile uint32_t PHYMODE;         // 00FCh: PHY interface.

    volatile uint32_t VSCR;                       // 0100h: DPI Shadow Feature.
    volatile uint32_t Reserved0104[2];            // 0104h: Reserved
    volatile uint32_t PVCIDACT;                   // 010Ch: Actual DPI Virtual Channel ID.
    volatile uint32_t PCOLORACT;                  // 0110h: Actual DPI Color Coding.
    volatile uint32_t Reserved0114;               // 0114h: Reserved.
    volatile uint32_t PLPTIMACT;                  // 0118h: Actual DPI Low Power Commands' Timing.
    volatile uint32_t ETECFG;                     // 011Ch: TE for Hardware operations.
    volatile uint32_t Reserved0120[6];            // 0120h: Reserved
    volatile uint32_t VMODEACT;                   // 0138h:
    volatile uint32_t PKTSACT;                    // 013Ch:
    volatile uint32_t CHUNKSACT;                  // 0140h:
    volatile uint32_t NPKTSACT;                   // 0144h:
    volatile uint32_t HSAACT;                     // 0148h:
    volatile uint32_t HBPACT;                     // 014Ch:
    volatile uint32_t HLINEACT;                   // 0150h:
    volatile uint32_t VSAACT;                     // 0154h:
    volatile uint32_t VBPACT;                     // 0158h:
    volatile uint32_t VFPACT;                     // 015Ch:
    volatile uint32_t VACTACT;                    // 0160h:
    volatile uint32_t Reserved0164;               // 0164h: Reserved
    volatile uint32_t VPSTS;                      // 0168h: eDPI and DPI FIFOs status.
    volatile uint32_t Reserved016C[9];            // 016Ch: Reserved
    volatile uint32_t SDF3DACT;                   // 0190h: Actual SDF3D.
    volatile uint32_t Reserved0194[3];            // 0194h: Reserved
    volatile uint32_t DSCID;                      // 0200h:
    volatile uint32_t DSCVERSION;                 // 0204h:
    volatile uint32_t DSCFLATNESS_DET_THRES;      // 0208h:
    volatile uint32_t DSCDELAY;                   // 020Ch:
    volatile uint32_t DSCCOMPRESSED_LINE_SIZE;    // 0210h:
    volatile uint32_t DSCLINES_IN_EXCESS;         // 0214h:
    volatile uint32_t DSCRBUF_ADDR_LAST_LINE_ADJ; // 0218h:
    volatile uint32_t DSCMODE;                    // 021Ch:
    volatile uint32_t DSCINTSTS;                  // 0220h:
    volatile uint32_t DSCINTMSK;                  // 0224h:
    volatile uint32_t DSCINTFORCE;                // 0228h:
    volatile uint32_t DSCFIFOSTSSEL;              // 022Ch:
    volatile uint32_t DSCFIFOSTS;                 // 0230h:
    volatile uint32_t DSCFIFOSTS2;                // 0234h:
    volatile uint32_t DSCFIFOWORDCOUNT;           // 0238h:
    volatile uint32_t DSCFIFOWRODCOUNT2;          // 023Ch:
    volatile uint32_t Reserved0240[8];            // 0240h: Reserved
    volatile uint8_t  DSCPPS[88];                 // 0260h: Picture Parameter Set.
    volatile uint32_t Reserved02B8[12];           // 0240h: Reserved
    volatile uint32_t INTSTS0AP;                  // 0300h: Internal Diagnosis Group 0.
    volatile uint32_t INTMSK0AP;                  // 0304h:
    volatile uint32_t INTFORCE0AP;                // 0308h:
    volatile uint32_t Reserved030C[1];            // 030Ch: Reserved
    volatile uint32_t INTSTS1AP;                  // 0310h:
    volatile uint32_t INTMSK1AP;                  // 0314h:
    volatile uint32_t INTFORCE1AP;                // 0318h:
    volatile uint32_t Reserved031C[1];            // 031Ch: Reserved
    volatile uint32_t INTSTS2AP;                  // 0320h:
    volatile uint32_t INTMSK2AP;                  // 0324h:
    volatile uint32_t INTFORCE2AP;                // 0328h:
    volatile uint32_t Reserved032C[1];            // 032Ch: Reserved
    volatile uint32_t TO_HSTXRDY_CFG_AP;          // 0340h:
    volatile uint32_t TO_LPTXRDY_CFG_AP;          // 0344h:
    volatile uint32_t TO_LPTXTRIG_CFG_AP;         // 0348h:
    volatile uint32_t TO_LPTXULPS_CFG_AP;         // 034Ch:
    volatile uint32_t TO_HSTX_CFG_AP;             // 0350h:
    volatile uint32_t TO_LPRX_CFG_AP;             // 0354h:
    volatile uint32_t TO_BTA_CFG_AP;              // 0358h:
    volatile uint32_t TO_CLK_DIV_AP;              // 035Ch:
    volatile uint32_t Reserved0360[8];            // 0360h: Reserved
    volatile uint32_t ERR_INJ_CTRL_AP;            // 0380h:
    volatile uint32_t ERR_INJ_CHK_MSK_AP;         // 0384h:
    volatile uint32_t ERR_INJ_DATA_MSK_AP;        // 0388h:
    volatile uint32_t ERR_INJ_ST_AP;              // 038Ch:
} DSIRegisterStruct;

/* --------  Exported types  ------------------------------------------------ */

typedef enum
{
    DCSNop                = 0x00, // N Operation
    DCSSoftReset          = 0x01, // Software Reset
    DCSGetCompressionMode = 0x03, // Get the current compression mode
    DCSGetDisplayID       = 0x04, // Get the display ID
    DCSGetRedChannel      = 0x06, // Get the red component of the pixel at (0, 0).
    DCSGetGreenChannel    = 0x07, // Get the green component of the pixel at (0, 0).
    DCSGetBlueChannel     = 0x08, // Get the blue component of the pixel at (0, 0).
    DCSGetPowerMode       = 0x0A, // Get the current power mode.
    DCSGetAddressMode     = 0x0B, // Get the data order for transfers from the Host to the display module and from the frame memory to the display device.
    DCSGetPixelFormat     = 0x0C, // Get the current pixel format.
    DCSGetDisplayMode     = 0x0D, // Get the current display mode from the peripheral.
    DCSGetSignalMode      = 0x0E, // Get display module signaling mode.
    DCSGetDiagNsticResult = 0x0F, // Get Peripheral Self-DiagNstic Result

    DCSEnterSleepMode   = 0x10, // Power for the display panel is off.
    DCSExitSleepMode    = 0x11, // Power for the display panel is on.
    DCSEnterPartialMode = 0x12, // Part of the display area is used for image display.
    DCSEnterNormalMode  = 0x13, // The whole display area is used for image display.

    DCSExitInvertMode   = 0x20, // Displayed image colors are Nt inverted.
    DCSEnterInvertMode  = 0x21, // Displayed image colors are inverted.
    DCSSetAllPixelOff   = 0x22, // Set all pixel off.
    DCSSetAllPixelOn    = 0x23, // Set all pixel on.
    DCSSetGammaCurve    = 0x26, // Selects the gamma curve used by the display device.
    DCSSetDisplayOff    = 0x28, // Blanks the display device.
    DCSSetDisplayOn     = 0x29, // Show the image on the display device.
    DCSSetColumnAddress = 0x2A, // Set the column extent.
    DCSSetPageAddress   = 0x2B, // Set the page extent.
    DCSWriteMemoryStart = 0x2C, // Transfer image data from the Host Processor to the peripheral starting at the location provided by SetColumnAddress and
                                // SetPageAddress.
    DCSWriteLUT        = 0x2D,  // Fills the peripheral look-up table with the provided data.
    DCSReadMemoryStart = 0x2E,  // Transfer image data from the peripheral to the Host Processor interface starting at the location provided by SetColumnAddress
                                // and SetPageAddress.

    DCSSetPartialRows      = 0x30, // Defines the number of rows in the partial display area on the display device.
    DCSSetPartialColumns   = 0x31, // Defines the number of columns in the partial display area on the display device.
    DCSSetScrollArea       = 0x33, // Defines the vertical scrolling and fixed areaon display device.
    DCSSetTearOff          = 0x34, // Synchronization information is Nt sent from the display module to the host processor.
    DCSSetTearOn           = 0x35, // Synchronization information is sent from the display module to the host processor at the start of VFP.
    DCSSetAddressMode      = 0x36, // Set the data order for transfers from the Host to the display module and from the frame memory to the display device.
    DCSSetScrollStart      = 0x37, // Defines the vertical scrolling starting point.
    DCSExitIdleMode        = 0x38, // Full color depth is used on the display panel.
    DCSEnterIdleMode       = 0x39, // Reduced color depth is used on the display panel.
    DCSSetPixelFormat      = 0x3A, // Defines how many bits per pixel are used in the interface.
    DCSWriteMemoryContinue = 0x3C, // Transfer image information from the Host Processor interface to the peripheral from the last written location.
    DCSSet3DControl        = 0x3D, // 3D is used on the display panel.
    DCSReadMemoryContinue  = 0x3E, // Read image data from the peripheral continuing after the last readMemoryContinue or readMemoryStart.
    DCSGet3DControl        = 0x3F, // Get display module 3D Mode.

    DCSSetVsyncTiming  = 0x40, // Set VSYNC timing
    DCSSetTearScanline = 0x44, // Synchronization information is sent from the display module to the host processor when the display device refresh reaches the
                               // provided scanline.
    DCSGetScanline     = 0x45, // Get the current scanline.
    DCSReadDDBStart    = 0xA1, // Read the DDB from the provided location.
    DCSReadDDBContinue = 0xA8, // Continue reading the DDB from the last read location.
} DCSEnum;

typedef enum
{
    DSIColorCoding16C1     = 0,
    DSIColorCoding16C2     = 1,
    DSIColorCoding16C3     = 2,
    DSIColorCoding18C1     = 3,
    DSIColorCoding18C2     = 4,
    DSIColorCoding24       = 5,
    DSIColorCoding20YUV422 = 6,
    DSIColorCoding24YUV422 = 7,
    DSIColorCoding16YUV422 = 8,
    DSIColorCoding30DSCENC = 9,
    DSIColorCoding36       = 10,
    DSIColorCoding12YUV420 = 11,
    DSIColorCoding24DSC24  = 12,
} DSIColorCodingEnum;

typedef enum
{
    DSILanes1 = 0,
    DSILanes2 = 1,
    DSILanes3 = 2,
    DSILanes4 = 3,
} DSILanesEnum;

typedef enum
{
    DSIVideoModeSyncPulses = 0,
    DSIVideoModeSyncEvents = 1,
    DSIVideoModeBurst      = 2,
} DSIVideoModeEnum;

typedef enum
{
    DSIDataTypeNull           = 0x00, // Null packet, for delay or other
    DSIDataTypeShortWrite     = 0x03, // Generic short write, no parameters
    DSIDataTypeShortRead      = 0x04, // Generic short read, no parameters
    DSIDataTypeShortWrite1    = 0x13, // Generic short write, one parameter
    DSIDataTypeShortRead1     = 0x14, // Generic short read, one parameter
    DSIDataTypeShortWrite2    = 0x23, // Generic short write, two parameters
    DSIDataTypeShortRead2     = 0x24, // Generic short read, two parameters
    DSIDataTypeLongWrite      = 0x29, // Generic long write
    DSIDataTypeMaxReturnSize  = 0x37, // Set Maximum Return Packet Size
    DSIDataTypeDCSShortWrite  = 0x05, // DCS short write, no parameters
    DSIDataTypeDCSRead        = 0x06, // DCS short read
    DSIDataTypeDCSShortWrite1 = 0x15, // DCS short write, one parameter
    DSIDataTypeDCSLongWrite   = 0x39, // DCS long write

    DSIDataTypeEnd = 0xFF, // End Array
} DSIDataTypeEnum;

typedef struct
{
    DSIDataTypeEnum DataType;

    uint16_t Length;
    union
    {
        uint8_t* Payload;
        uint8_t  Command;
    };
} DSIPacketStruct;

typedef struct DSIStruct_s DSIStruct;

struct DSIStruct_s
{
    bool (*Init)(DSIStruct* self);

    bool (*Start)(DSIStruct* self, bool isStart);

    bool (*Write)(DSIStruct* self, DSIDataTypeEnum dataType, uint8_t* payload, uint16_t length);
    bool (*Read)(DSIStruct* self, DSIDataTypeEnum dataType, uint8_t* payload, uint16_t payloadLength, uint8_t* readBuffer, uint16_t readLength);

    // Public Member Start
    // Configs
    uint32_t LaneClock;       // Lane clock in Kbps
    uint16_t EscapeClock;     // Escape clock in Kbps
    uint16_t TimeoutClock;    // Timeout clock in KHz
    uint8_t  PhyStopWaitTime; // The minimum time PHY needs to stay in StopState before requesting an high-speed transmission
    uint8_t  PhyTransitTime;  //

    uint16_t CommandPacketInterval; // The Command Packet Interval in ns

    bool IsAutoStopClockLane : 1; // If the clock lane auto stop

    DSILanesEnum Lanes : 2; // The max active data lane index

    uint8_t DPIVirtualChannelID : 2;

    bool IsCommandMode : 1;

    DSIColorCodingEnum ColorCoding : 4;
    // Activate loosely packed variant to 18-bit color coding
    bool IsLooselyColorCoding18 : 1;

    bool IsPatternGeneratorEnabled : 1;

    uint16_t CommandSize;

    union
    {
        struct
        {
            uint32_t PolarityConfig : 5;
            uint32_t AckRequestConfig : 2;
            uint32_t LPMaxReadCommandConfig : 1;
            uint32_t LPGenericCommandConfig : 7;
            uint32_t LPDCSCommandConfig : 4;
        };
        struct
        {
            // Polarity Config
            bool IsDataEnableActiveLow : 1;
            bool IsVsyncActiveLow : 1;
            bool IsHsyncActiveLow : 1;
            bool IsShutdownActiveLow : 1;
            bool IsColorModeActiveLow : 1;

            // Command Config
            bool IsTearingEffectAckEnabled : 1;
            bool IsAckRequestEnabled : 1;

            bool IsLPMaxReadPacketSize : 1;

            bool IsLPGenericShortWrite0Para : 1;
            bool IsLPGenericShortWrite1Para : 1;
            bool IsLPGenericShortWrite2Para : 1;
            bool IsLPGenericShortRead0Para : 1;
            bool IsLPGenericShortRead1Para : 1;
            bool IsLPGenericShortRead2Para : 1;
            bool IsLPGenericLongWrite : 1;

            bool IsLPDCSShortWrite0Para : 1;
            bool IsLPDCSShortWirte1Para : 1;
            bool IsLPDCSShortRead0Para : 1;
            bool IsLPDCSLongWrite : 1;
        };
    };

    DSIVideoModeEnum VideoMode : 2;

    uint16_t VideoPacketSize : 14;

    union
    {
        struct
        {
            uint8_t LPVideoConfig;
        };
        struct
        {
            bool IsLPVSAEnabled : 1;
            bool IsLPVBPEnabled : 1;
            bool IsLPVFPEnabled : 1;
            bool IsLPVACTEnabled : 1;
            bool IsLPHBPEnabled : 1;
            bool IsLPHFPEnabled : 1;
            bool IsFrameEndAckEnabled : 1;
            bool IsLPCommandOnly : 1;
        };
    };

    uint16_t VideoChunkCount : 13;
    uint16_t VideoNullSize : 13;
    uint16_t HorizontalSyncActivePeriod : 13;
    uint16_t HorizontalBackPorchPeriod : 13;
    uint16_t HorizontalTotalSize : 14;

    uint16_t VerticalSyncActivePeriod : 10;
    uint16_t VerticalBackPorchPeriod : 10;
    uint16_t VerticalFrontPorchPeriod : 10;
    uint16_t VerticalActivePeriod : 14;

    uint8_t VideoMaxBlankLPCommandSize;
    uint8_t VideoMaxActiveLPCommandSize;

    // Public Member End

    union
    {
        uint32_t DSIBase;

        DSIRegisterStruct* Reg;
    };
};

/* --------  Public Function  ----------------------------------------------- */

extern bool DSIConstractor(DSIStruct* self, uint32_t dsiBase);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_DSI_H__
