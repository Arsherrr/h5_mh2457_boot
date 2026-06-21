#include "DSI-GC9C01.h"

static const DSIPacketStruct GC9C01Configs[] = {
    // internal register enable
    MakeDSIPacket(0xfe),
    MakeDSIPacket(0xef),

    MakeDSIPacket(0x80, 0x11), //reg_en for 70\74
    MakeDSIPacket(0x81, 0x70), //reg_en for 7C\7D\7E
    MakeDSIPacket(0x82, 0x09), //reg_en for 90\93
    MakeDSIPacket(0x83, 0x03), //reg_en for 98\99
    MakeDSIPacket(0x84, 0x20), //reg_en for B5
    MakeDSIPacket(0x85, 0x42), //reg_en for B9\BE
    MakeDSIPacket(0x86, 0xfc), //reg_en for C2~7
    MakeDSIPacket(0x87, 0x09), //reg_en for C8\CB
    MakeDSIPacket(0x89, 0x10), //reg_en for EC
    MakeDSIPacket(0x8A, 0x4f), //reg_en for F0~3\F6
    MakeDSIPacket(0x8C, 0x59), //reg_en for 60\63\64\66
    MakeDSIPacket(0x8D, 0x51), //reg_en for 68\6C\6E
    MakeDSIPacket(0x8E, 0xae), //reg_en for A1~3\A5\A7
    MakeDSIPacket(0x8F, 0xf3), //reg_en for AC~F\A8\A9

    MakeDSIPacket(0x36, 0x00), // Memory Access Control
    MakeDSIPacket(0x3A, 0x66), // 565 frame

    //2COL
    MakeDSIPacket(0xEC, 0x77),

    //rtn 60Hz
    MakeDSIPacket(0x74, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00),

    //bvdd 3x
    MakeDSIPacket(0x98, 0x3E),
    //bvee -2x
    MakeDSIPacket(0x99, 0x3E),

    //VBP
    MakeDSIPacket(0xC3, 0x2A),
    //VBN
    MakeDSIPacket(0xC4, 0x18),

    MakeDSIPacket(0xA1, 0x01, 0x04),

    MakeDSIPacket(0xA2, 0x01, 0x04),

    MakeDSIPacket(0xA9, 0x1C), //IREF

    MakeDSIPacket(0xA5, 0x11), //VDDMA, 0x09//VDDML

    MakeDSIPacket(0xB9, 0x8A), //RTERM

    MakeDSIPacket(0xA8, 0x5E), //VBG_BUF, DVDD

    MakeDSIPacket(0xA7, 0x40),

    MakeDSIPacket(0xAF, 0x73), //VDDSOU ,VDDGM

    MakeDSIPacket(0xAE, 0x44), //VREE,VRDD

    MakeDSIPacket(0xAD, 0x38), //VRGL ,VDDSF

    MakeDSIPacket(0xA3, 0x5D), //

    MakeDSIPacket(0xC2, 0x02), //VREG_VREF

    MakeDSIPacket(0xC5, 0x11), //VREG1A

    MakeDSIPacket(0xC6, 0x0E), //VREG1B

    MakeDSIPacket(0xC7, 0x13), //VREG2A

    MakeDSIPacket(0xC8, 0x0D), //VREG2B

    //bvdd ref_ad
    MakeDSIPacket(0xCB, 0x02),

    MakeDSIPacket(0x7C, 0xB6, 0x26),

    MakeDSIPacket(0xAC, 0x24), //VGLO

    MakeDSIPacket(0xF6, 0x80), //EPF=2

    //gip start
    MakeDSIPacket(0xB5, 52, 0x09, 0x14), // VFP: 52->60FPS

    //STV1&2
    MakeDSIPacket(0x60, 0x38, 0x0B, 0x5B, 0x56),

    //STV3&4
    MakeDSIPacket(0x63, 0x3A, 0xE0, 0x5B, 0x56),

    //CLK_group1
    MakeDSIPacket(0x64, 0x38, 0x0D, 0x72, 0xDD, 0x5B, 0x56),

    //CLK_group1
    MakeDSIPacket(0x66, 0x38, 0x11, 0x72, 0xE1, 0x5B, 0x56),

    //FLC&FLV 1~2
    MakeDSIPacket(0x68, 0x3B, 0x08, 0x08, 0x00, 0x08, 0x29, 0x5B),

    MakeDSIPacket(0x6E, 0x00, 0x00, 0x00, 0x07, 0x01, 0x13, 0x11, 0x0B, 0x09, 0x16, 0x15, 0x1D, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x1D, 0x15,
                  0x16, 0x0A, 0x0C, 0x12, 0x14, 0x02, 0x08, 0x00, 0x00, 0x00),
    //gip end

    MakeDSIPacket(0xBE, 0x11), //SOU_BIAS_FIX

    // precharge GATE
    MakeDSIPacket(0x6C, 0xCC, 0x0C, 0xCC, 0x84, 0xCC, 0x04, 0x50),
    MakeDSIPacket(0x7D, 0x72),
    MakeDSIPacket(0x7E, 0x38),
    MakeDSIPacket(0x70, 0x02, 0x03, 0x09, 0x05, 0x0C, 0x06, 0x09, 0x05, 0x0C, 0x06),
    MakeDSIPacket(0x90, 0x06, 0x06, 0x05, 0x06),
    MakeDSIPacket(0x93, 0x45, 0xFF, 0x00),

    // Gamma
    MakeDSIPacket(0xF0, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A),
    MakeDSIPacket(0xF1, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F),
    MakeDSIPacket(0xF2, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A),
    MakeDSIPacket(0xF3, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F),

    MakeDSIPacket(DCSExitSleepMode),

    MakeDSIDelay(120),

    MakeDSIPacket(DCSSetDisplayOn),
    MakeDSIPacket(DCSSetTearOn, 0x00),

    MakeDSIPacketEnd(),
};

#define GC9C01_DISPLAY_ID 0x009C0100

bool GC9C01Constractor(DisplayStruct* display) {
    if (display->ID && display->ID != GC9C01_DISPLAY_ID)
        return false;

    DisplayStartupSequence(display, true, true, 10, 10, 100);

    if (!display->ID && (display->ID = DisplayDSIReadID(display, NULL)) != GC9C01_DISPLAY_ID)
        return false;

    display->HorizontalSyncActive = GC9C01_HSA;
    display->HorizontalBackPorch  = GC9C01_HBP;
    display->HorizontalActive     = GC9C01_HACT;
    display->HorizontalFrontPorch = GC9C01_HFP;

    display->VerticalSyncActive = GC9C01_VSA;
    display->VerticalBackPorch  = GC9C01_VBP;
    display->VerticalActive     = GC9C01_VACT;
    display->VerticalFrontPorch = GC9C01_VFP;

    display->PixelClock = 24000; // Pixel Clock 20M

    display->RefreshMode = DisplayRefreshContinuous; //DisplayRefreshAdaptive;

    display->IsTearingEffectValidLow = false;

    // DSI Config
    display->ConfigPackets = GC9C01Configs;
    display->Color         = DisplayColorRGB565;

    DSIStruct* dsi        = display->DSIHandle;
    dsi->LaneClock        = 320000; // 320M
    dsi->Lanes            = DSILanes1;
    dsi->ColorCoding      = DSIColorCoding24;
    dsi->IsCommandMode    = true;
    dsi->IsLPDCSLongWrite = true;

    return true;
}
