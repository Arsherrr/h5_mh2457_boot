#include "DSI-JD9852.h"

static const DSIPacketStruct JD9852Configs[] = {
    // PASSWORD
    MakeDSIPacket(0xDF, 0x98, 0x51, 0xE9),

    //---------------- PAGE0 --------------
    MakeDSIPacket(0xDE, 0x00),

    // VGMP,VGSP,VGMN,VGSN 4.2
    MakeDSIPacket(0xB7, 0x38, 0x97, 0x38, 0x11), //2F

    // Set_R_GAMMA
    MakeDSIPacket(0xC8, 0x3F, 0x38, 0x33, 0x2F, 0x32, 0x34, 0x2F, 0x2F, 0x2D, 0x2C, 0x27, 0x1A, 0x14, 0x0A, 0x06, 0x0E, 0x3F, 0x38, 0x33, 0x2F, 0x32, 0x34,
                  0x2F, 0x2F, 0x2D, 0x2C, 0x27, 0x1A, 0x14, 0x0A, 0x06, 0x0E),

    // POW_CTRL
    MakeDSIPacket(0xB9, 0x33, 0x08, 0xCC),

    // DCDC_SEL
    MakeDSIPacket(0xBB, 0x44, 0x7A, 0x30, 0x40, 0x7C, 0x60, 0x70, 0x70),

    // VDDD_CTRL
    MakeDSIPacket(0xBC, 0x38, 0x3C),

    // SETSTBA
    MakeDSIPacket(0xC0, 0x31, 0x20),

    // SETPANEL(default)
    MakeDSIPacket(0xC1, 0x12),

    // SETRGBCYC
    MakeDSIPacket(0xC3, 0x08, 0x00, 0x0A, 0x10, 0x08, 0x54, 0x45, 0x71, 0x2C),

    // ETRGBCYC(default)
    MakeDSIPacket(0xC4, 0x00, 0xA0, 0x79, 0x0E, 0x0A, 0x16, 0x79, 0x0E, 0x0A, 0x16, 0x79, 0x0E, 0x0A, 0x16, 0x82, 0x00, 0x03),

    // SET_GD(default)
    MakeDSIPacket(0xD0, 0x04, 0x0C, 0x6B, 0x0F, 0x07, 0x03),

    // RAMCTRL(default)
    MakeDSIPacket(0xD7, 0x13, 0x00),

    //---------------- PAGE2 --------------
    MakeDSIPacket(0xDE, 0x02),
    MakeDSIDelay(1),

    MakeDSIPacket(0xB7, 0x63, 0x00, 0x0C),

    //DCDC_SET
    MakeDSIPacket(0xB8, 0x1D, 0xA0, 0x2F, 0x04, 0x33),

    //SETRGBCYC2
    MakeDSIPacket(0xC1, 0x10, 0x66, 0x66, 0x01),

    //---------------- PAGE0 --------------
    MakeDSIPacket(0xDE, 0x00),

    // Sleep Out
    MakeDSIPacket(DCSExitSleepMode),
    MakeDSIDelay(120),

    //---------------- PAGE2 --------------
    MakeDSIPacket(0xDE, 0x02),
    MakeDSIDelay(1),

    // OSCM
    MakeDSIPacket(0xC5, 0x4E, 0x00, 0x00),
    MakeDSIDelay(1),

    // SETMIPI_2
    MakeDSIPacket(0xCA, 0x30, 0x20, 0xF4),
    MakeDSIDelay(1),

    //---------------- PAGE4 --------------

    MakeDSIPacket(0xDE, 0x04),
    MakeDSIDelay(1),

    // SETPHY3
    MakeDSIPacket(0xD3, 0x3C),
    MakeDSIDelay(1),
    //---------------- PAGE0 --------------

    MakeDSIPacket(0xDE, 0x00),
    MakeDSIDelay(1),

    MakeDSIPacket(DCSSetDisplayOn),

    MakeDSIPacketEnd(),
};

#define JD9852_DISPLAY_ID 0x98520100

bool JD9852Constractor(DisplayStruct* display) {
    if (display->ID && display->ID != JD9852_DISPLAY_ID)
        return false;

    DisplayStartupSequence(display, true, true, 20, 10, 100);

    if (!display->ID && (display->ID = DisplayDSIReadID(display, NULL)) != JD9852_DISPLAY_ID)
        return false;

    display->PixelClock = JD9852_PIXEL_CLOCK;

    display->HorizontalSyncActive = JD9852_HSA;
    display->HorizontalBackPorch  = JD9852_HBP;
    display->HorizontalActive     = JD9852_HACT;
    display->HorizontalFrontPorch = JD9852_HFP;

    display->VerticalSyncActive = JD9852_VSA;
    display->VerticalBackPorch  = JD9852_VBP;
    display->VerticalActive     = JD9852_VACT;
    display->VerticalFrontPorch = JD9852_VFP;

    // DSI Config
    display->ConfigPackets = JD9852Configs;
    display->Color         = DisplayColorRGB565;

    DSIStruct* dsi     = display->DSIHandle;
    dsi->LaneClock     = JD9852_LANE_CLOCK;
    dsi->Lanes         = DSILanes1;
    dsi->IsCommandMode = false;

    return true;
}
