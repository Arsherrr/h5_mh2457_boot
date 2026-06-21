#include "DSI-ILI9806E.h"

static const DSIPacketStruct ILI9806EConfigs[] = {
#if CONFIG_DISPLAY_ILI9806E_PRESET == DISPLAY_ILI9806E_PRESET_KD050FWFIA019_02_C002A
    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x01),

    //GIP_1
    MakeDSIPacket(0x08, 0x10), //Output    SDA
    MakeDSIPacket(0x20, 0x00), //set DE/VSYNC mode
    MakeDSIPacket(0x21, 0x01), //DE = 1 Active
    MakeDSIPacket(0x30, 0x01), //Resolution setting 480 X 854
    MakeDSIPacket(0x31, 0x00), //Inversion setting  //02-2dot
    MakeDSIPacket(0x40, 0x16), //BT  AVDD,AVDD
    MakeDSIPacket(0x41, 0x33), //avdd +5.2v,avee-5.2v 33
    MakeDSIPacket(0x42, 0x03), //VGL=DDVDH+VCIP -DDVDL,VGH=2DDVDL-VCIP 
    MakeDSIPacket(0x43, 0x09), //SET VGH clamp level 
    MakeDSIPacket(0x44, 0x06), //SET VGL clamp level 

    //GIP_2
    MakeDSIPacket(0x50, 0x88), //VREG1
    MakeDSIPacket(0x51, 0x88), //VREG2
    MakeDSIPacket(0x52, 0x00), //Flicker MSB 
    MakeDSIPacket(0x53, 0x49), //Flicker LSB 
    MakeDSIPacket(0x55, 0x49), //Flicker

    //GIP_3
    MakeDSIPacket(0x60, 0x07), 
    MakeDSIPacket(0x61, 0x00), 
    MakeDSIPacket(0x62, 0x07), 
    MakeDSIPacket(0x63, 0x00), 

    //============Gamma START=============
    //Pos Register
    MakeDSIPacket(0xA0, 0x00),
    MakeDSIPacket(0xA1, 0x09),
    MakeDSIPacket(0xA2, 0x11),
    MakeDSIPacket(0xA3, 0x0B),
    MakeDSIPacket(0xA4, 0x05),
    MakeDSIPacket(0xA5, 0x08),
    MakeDSIPacket(0xA6, 0x06),
    MakeDSIPacket(0xA7, 0x04),
    MakeDSIPacket(0xA8, 0x09),
    MakeDSIPacket(0xA9, 0x0C),
    MakeDSIPacket(0xAA, 0x15),
    MakeDSIPacket(0xAB, 0x08),
    MakeDSIPacket(0xAC, 0x0F),
    MakeDSIPacket(0xAD, 0x12),
    MakeDSIPacket(0xAE, 0x09),
    MakeDSIPacket(0xAF, 0x00),

    //Neg Register
    MakeDSIPacket(0xC0, 0x00),
    MakeDSIPacket(0xC1, 0x09),
    MakeDSIPacket(0xC2, 0x10),
    MakeDSIPacket(0xC3, 0x0C),
    MakeDSIPacket(0xC4, 0x05),
    MakeDSIPacket(0xC5, 0x08),
    MakeDSIPacket(0xC6, 0x06),
    MakeDSIPacket(0xC7, 0x04),
    MakeDSIPacket(0xC8, 0x08),
    MakeDSIPacket(0xC9, 0x0C),
    MakeDSIPacket(0xCA, 0x14),
    MakeDSIPacket(0xCB, 0x08),
    MakeDSIPacket(0xCC, 0x0F),
    MakeDSIPacket(0xCD, 0x11),
    MakeDSIPacket(0xCE, 0x09),
    MakeDSIPacket(0xCF, 0x00),

    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x06), // Change to Page 6 CMD for GIP timing
    MakeDSIPacket(0x00, 0x21),
    MakeDSIPacket(0x01, 0x0A),
    MakeDSIPacket(0x02, 0x00),
    MakeDSIPacket(0x03, 0x00),
    MakeDSIPacket(0x04, 0x01),
    MakeDSIPacket(0x05, 0x01),
    MakeDSIPacket(0x06, 0x80),
    MakeDSIPacket(0x07, 0x06),
    MakeDSIPacket(0x08, 0x01),
    MakeDSIPacket(0x09, 0x80),
    MakeDSIPacket(0x0A, 0x00),
    MakeDSIPacket(0x0B, 0x00),
    MakeDSIPacket(0x0C, 0x0A),
    MakeDSIPacket(0x0D, 0x0A),
    MakeDSIPacket(0x0E, 0x00),
    MakeDSIPacket(0x0F, 0x00),
    MakeDSIPacket(0x10, 0xF0),
    MakeDSIPacket(0x11, 0xF4),
    MakeDSIPacket(0x12, 0x04),
    MakeDSIPacket(0x13, 0x00),
    MakeDSIPacket(0x14, 0x00),
    MakeDSIPacket(0x15, 0xC0),
    MakeDSIPacket(0x16, 0x08),
    MakeDSIPacket(0x17, 0x00),
    MakeDSIPacket(0x18, 0x00),
    MakeDSIPacket(0x19, 0x00),
    MakeDSIPacket(0x1A, 0x00),
    MakeDSIPacket(0x1B, 0x00),
    MakeDSIPacket(0x1C, 0x00),
    MakeDSIPacket(0x1D, 0x00),
    MakeDSIPacket(0x20, 0x01),
    MakeDSIPacket(0x21, 0x23),
    MakeDSIPacket(0x22, 0x45),
    MakeDSIPacket(0x23, 0x67),
    MakeDSIPacket(0x24, 0x01),
    MakeDSIPacket(0x25, 0x23),
    MakeDSIPacket(0x26, 0x45),
    MakeDSIPacket(0x27, 0x67),
    MakeDSIPacket(0x30, 0x01),
    MakeDSIPacket(0x31, 0x11),
    MakeDSIPacket(0x32, 0x00),
    MakeDSIPacket(0x33, 0xEE),
    MakeDSIPacket(0x34, 0xFF),
    MakeDSIPacket(0x35, 0xBB),
    MakeDSIPacket(0x36, 0xCA),
    MakeDSIPacket(0x37, 0xDD),
    MakeDSIPacket(0x38, 0xAC),
    MakeDSIPacket(0x39, 0x76),
    MakeDSIPacket(0x3A, 0x67),
    MakeDSIPacket(0x3B, 0x22),
    MakeDSIPacket(0x3C, 0x22),
    MakeDSIPacket(0x3D, 0x22),
    MakeDSIPacket(0x3E, 0x22),
    MakeDSIPacket(0x3F, 0x22),
    MakeDSIPacket(0x40, 0x22),
    MakeDSIPacket(0x52, 0x10),
    MakeDSIPacket(0x53, 0x10),

    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x07), // Change to Page 7 CMD for GIP timing
    MakeDSIPacket(0x06, 0x11),
    MakeDSIPacket(0x17, 0x22),
    MakeDSIPacket(0x02, 0x77),
    MakeDSIPacket(0xE1, 0x79),
    MakeDSIPacket(0xB3, 0x10),

    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x00), // Change to Page 0 CMD for GIP timing
    MakeDSIPacket(0x35),
    MakeDSIPacket(0x3A, 0x70),
#elif CONFIG_DISPLAY_ILI9806E_PRESET == DISPLAY_ILI9806E_PRESET_KD043WVFIA083
    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x01),

    //GIP_1
    MakeDSIPacket(0x08, 0x10), //Output    SDA
    MakeDSIPacket(0x20, 0x00), //set DE/VSYNC mode
    MakeDSIPacket(0x21, 0x01), //DE = 1 Active
    MakeDSIPacket(0x30, 0x02), //Resolution setting 480 X 800
    MakeDSIPacket(0x31, 0x00), //Inversion setting  //02-2dot
    MakeDSIPacket(0x40, 0x14), //BT DDVDH DDVDL  //10,14,18 00	2XVCI
    MakeDSIPacket(0x41, 0x33), //avdd +5.2v,avee-5.2v 33
    MakeDSIPacket(0x42, 0x01), //VGL=DDVDH+VCIP -DDVDL,VGH=2DDVDL-VCIP
    MakeDSIPacket(0x43, 0x09), //SET VGH clamp level +15v
    MakeDSIPacket(0x44, 0x06), //SET VGL clamp level -12.8v
    MakeDSIPacket(0x45, 0x0A), // VGLO_REG :-12.3V

    //GIP_2
    MakeDSIPacket(0x50, 0x78), //VREG1 for positive Gamma  +4.5V
    MakeDSIPacket(0x51, 0x78), //VREG2 for negative Gamma
    MakeDSIPacket(0x52, 0x00), //VCOM
    MakeDSIPacket(0x53, 0x3A), //Forward Flicker
    MakeDSIPacket(0x54, 0x00), //VCOM
    MakeDSIPacket(0x55, 0x3A), //Backward Flicker
    MakeDSIPacket(0x56, 0x50),

    //GIP_3
    MakeDSIPacket(0x60, 0x07), //SDTI
    MakeDSIPacket(0x61, 0x06), //CRTI
    MakeDSIPacket(0x62, 0x06), //EQTI
    MakeDSIPacket(0x63, 0x04), //PCTI

    //============Gamma START=============
    //Pos Register
    MakeDSIPacket(0xA0, 0x00), //Positive Gamma
    MakeDSIPacket(0xA1, 0x13),
    MakeDSIPacket(0xA2, 0x19),
    MakeDSIPacket(0xA3, 0x0C),
    MakeDSIPacket(0xA4, 0x06),
    MakeDSIPacket(0xA5, 0x0A),
    MakeDSIPacket(0xA6, 0x06),
    MakeDSIPacket(0xA7, 0x04),
    MakeDSIPacket(0xA8, 0x09),
    MakeDSIPacket(0xA9, 0x08),
    MakeDSIPacket(0xAA, 0x12),
    MakeDSIPacket(0xAB, 0x06),
    MakeDSIPacket(0xAC, 0x0E),
    MakeDSIPacket(0xAD, 0x0E),
    MakeDSIPacket(0xAE, 0x09),
    MakeDSIPacket(0xAF, 0x00),

    //Neg Register
    MakeDSIPacket(0xC0, 0x00), //Negative Gamma
    MakeDSIPacket(0xC1, 0x0D),
    MakeDSIPacket(0xC2, 0x18),
    MakeDSIPacket(0xC3, 0x0D),
    MakeDSIPacket(0xC4, 0x06),
    MakeDSIPacket(0xC5, 0x09),
    MakeDSIPacket(0xC6, 0x07),
    MakeDSIPacket(0xC7, 0x05),
    MakeDSIPacket(0xC8, 0x08),
    MakeDSIPacket(0xC9, 0x0E),
    MakeDSIPacket(0xCA, 0x12),
    MakeDSIPacket(0xCB, 0x09),
    MakeDSIPacket(0xCC, 0x0E),
    MakeDSIPacket(0xCD, 0x0E),
    MakeDSIPacket(0xCE, 0x08),
    MakeDSIPacket(0xCF, 0x00),

    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x06), // Change to Page 6 CMD for GIP timing
    MakeDSIPacket(0x00, 0x20),
    MakeDSIPacket(0x01, 0x04),
    MakeDSIPacket(0x02, 0x00),
    MakeDSIPacket(0x03, 0x00),
    MakeDSIPacket(0x04, 0x01),
    MakeDSIPacket(0x05, 0x01),
    MakeDSIPacket(0x06, 0x88),
    MakeDSIPacket(0x07, 0x04),
    MakeDSIPacket(0x08, 0x01),
    MakeDSIPacket(0x09, 0x90),
    MakeDSIPacket(0x0a, 0x03),
    MakeDSIPacket(0x0b, 0x01),
    MakeDSIPacket(0x0c, 0x01),
    MakeDSIPacket(0x0d, 0x01),
    MakeDSIPacket(0x0e, 0x00),
    MakeDSIPacket(0x0f, 0x00),

    MakeDSIPacket(0x10, 0x55),
    MakeDSIPacket(0x11, 0x53),
    MakeDSIPacket(0x12, 0x01),
    MakeDSIPacket(0x13, 0x0D),
    MakeDSIPacket(0x14, 0x0D),
    MakeDSIPacket(0x15, 0x43),
    MakeDSIPacket(0x16, 0x0B),
    MakeDSIPacket(0x17, 0x00),
    MakeDSIPacket(0x18, 0x00),
    MakeDSIPacket(0x19, 0x00),
    MakeDSIPacket(0x1a, 0x00),
    MakeDSIPacket(0x1b, 0x00),
    MakeDSIPacket(0x1c, 0x00),
    MakeDSIPacket(0x1d, 0x00),

    MakeDSIPacket(0x20, 0x01),
    MakeDSIPacket(0x21, 0x23),
    MakeDSIPacket(0x22, 0x45),
    MakeDSIPacket(0x23, 0x67),
    MakeDSIPacket(0x24, 0x01),
    MakeDSIPacket(0x25, 0x23),
    MakeDSIPacket(0x26, 0x45),
    MakeDSIPacket(0x27, 0x67),
    MakeDSIPacket(0x28, 0x55),

    MakeDSIPacket(0x30, 0x02),
    MakeDSIPacket(0x31, 0x22),
    MakeDSIPacket(0x32, 0x11),
    MakeDSIPacket(0x33, 0xAA),
    MakeDSIPacket(0x34, 0xBB),
    MakeDSIPacket(0x35, 0x66),
    MakeDSIPacket(0x36, 0x00),
    MakeDSIPacket(0x37, 0x22),
    MakeDSIPacket(0x38, 0x22),
    MakeDSIPacket(0x39, 0x22),
    MakeDSIPacket(0x3a, 0x22),
    MakeDSIPacket(0x3b, 0x22),
    MakeDSIPacket(0x3c, 0x22),
    MakeDSIPacket(0x3d, 0x22),
    MakeDSIPacket(0x3e, 0x22),
    MakeDSIPacket(0x3f, 0x22),
    MakeDSIPacket(0x40, 0x22),

    MakeDSIPacket(0x52, 0x10),
    MakeDSIPacket(0x53, 0x12),
    MakeDSIPacket(0x54, 0x13),

    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x07), // Change to Page 7 CMD for GIP timing
    MakeDSIPacket(0x17, 0x32),
    MakeDSIPacket(0x18, 0x1D),
    MakeDSIPacket(0x26, 0xB2),
    MakeDSIPacket(0x02, 0x77),
    MakeDSIPacket(0xE1, 0x79),
    MakeDSIPacket(0xB3, 0x10),

    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x00), // Change to Page 0 CMD for GIP timing
    MakeDSIPacket(0x36, 0x00),
    MakeDSIPacket(0x3A, 0x70),
#endif
    MakeDSIPacket(DCSExitSleepMode),
    MakeDSIDelay(120),
    MakeDSIPacket(DCSSetDisplayOn),
    MakeDSIDelay(20),

    MakeDSIPacketEnd(),
};

static const DSIPacketStruct ReadSequence[] = {
    MakeDSIPacket(0xFF, 0xFF, 0x98, 0x06, 0x04, 0x01),

    MakeDSIPacketRead(1, 0x00),
    MakeDSIPacketRead(1, 0x01),
    MakeDSIPacketRead(1, 0x02),

    MakeDSIPacketEnd(),
};

#define ILI9881C_DISPLAY_ID 0x98060400

bool ILI9806EConstractor(DisplayStruct* display) {

    DisplayStartupSequence(display, true, true, 20, 20, 120);

    // DSI configuring config
    uint32_t displayID = DisplayDSIReadID(display, ReadSequence);

    if (displayID != ILI9881C_DISPLAY_ID)
        return false;

    display->ID = displayID;

    display->HorizontalSyncActive = ILI9806E_HSA;
    display->HorizontalBackPorch  = ILI9806E_HBP;
    display->HorizontalActive     = ILI9806E_HACT;
    display->HorizontalFrontPorch = ILI9806E_HFP;

    display->VerticalSyncActive = ILI9806E_VSA;
    display->VerticalBackPorch  = ILI9806E_VBP;
    display->VerticalActive     = ILI9806E_VACT;
    display->VerticalFrontPorch = ILI9806E_VFP;

    display->PixelClock = ILI9806E_PIXEL_CLOCK;

    // DSI Config
    display->ConfigPackets = ILI9806EConfigs;
    display->Color         = DisplayColorRGB565;

    DSIStruct* dsi = display->DSIHandle;
    dsi->LaneClock = ILI9806E_LANE_CLOCK;
    dsi->Lanes     = DSILanes2;

    dsi->ColorCoding = DSIColorCoding16C1;

    dsi->IsCommandMode = false;
    return true;
}
