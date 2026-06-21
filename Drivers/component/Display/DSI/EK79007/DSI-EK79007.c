#include "DSI-EK79007.h"

static const DSIPacketStruct EK79007Configs[] = {
#if CONFIG_DISPLAY_EK79007_PRESET == DISPLAY_EK79007_PRESET_S1121_A101KF63_56A
    MakeDSIPacket(0x80, 0x4B),
    MakeDSIPacket(0x81, 0x99),
    MakeDSIPacket(0x82, 0xE5),
    MakeDSIPacket(0x83, 0x88),
    MakeDSIPacket(0x84, 0x85),
    MakeDSIPacket(0x85, 0x33),
    MakeDSIPacket(0x86, 0xB6),

    MakeDSIPacket(0xB2, 0x10),
#endif

    MakeDSIPacket(DCSExitSleepMode),
    MakeDSIDelay(120),

    MakeDSIPacketEnd(),
};

#define EK79007_DISPLAY_ID 0x79007000

static const DSIPacketStruct ReadSequence[] = {
    // Read the default register value to identify
    MakeDSIPacketRead(1, 0x36), //
    MakeDSIPacketRead(1, 0x80), //
    MakeDSIPacketRead(1, 0x82), //
    MakeDSIPacketRead(1, 0x86), //

    MakeDSIPacketEnd(),
};

bool EK79007Constractor(DisplayStruct* display) {
    DisplayStartupSequence(display, true, true, 20, 20, 120);

    uint32_t displayID = DisplayDSIReadID(display, ReadSequence);
    if (displayID != 0x01888888)
        return false;

    display->ID = EK79007_DISPLAY_ID;

    display->PixelClock = EK79007_PIXEL_CLOCK;

    display->HorizontalSyncActive = EK79007_HSA;
    display->HorizontalBackPorch  = EK79007_HBP;
    display->HorizontalActive     = EK79007_HACT;
    display->HorizontalFrontPorch = EK79007_HFP;

    display->VerticalSyncActive = EK79007_VSA;
    display->VerticalBackPorch  = EK79007_VBP;
    display->VerticalActive     = EK79007_VACT;
    display->VerticalFrontPorch = EK79007_VFP;

    // DSI Config
    display->ConfigPackets = EK79007Configs;
    display->Color         = DisplayColorRGB565;

    DSIStruct* dsi   = display->DSIHandle;
    dsi->LaneClock   = EK79007_LANE_CLOCK;
    dsi->Lanes       = DSILanes2;
    dsi->ColorCoding = DSIColorCoding16C1;

    dsi->IsCommandMode = false;

    return true;
}
