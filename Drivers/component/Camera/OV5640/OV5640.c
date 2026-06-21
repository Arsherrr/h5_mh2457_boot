#include "OV5640.h"

static const I2CShortConfigStruct OV5640ConfigClock[];
static const I2CShortConfigStruct OV5640ConfigMIPI[];
static const I2CShortConfigStruct OV5640ConfigDVP[];
static const I2CShortConfigStruct OV5640ConfigISP[];

static I2CShortConfigStruct OV5640ConfigFormatTiming[];

static bool ExcuteCommand(CameraStruct* camera, CameraCommandEnum command, uint32_t parameter) {
    I2CPortStruct* i2cPort = camera->I2CPort;

    if (i2cPort->Address != camera->I2CAddress)
        i2cPort->SetAddress(i2cPort, camera->I2CAddress);

    switch (command) {
        case CameraCommandResume:
            return i2cPort->WriteValue(i2cPort, I2CTransferA2D1, 0x3007, 0xFF);
        case CameraCommandSuspend:
            return i2cPort->WriteValue(i2cPort, I2CTransferA2D1, 0x3007, 0x00);
        default:
            return false;
    }
}

static bool Init(CameraStruct* camera) {
#if CONFIG_BOARD != 0
    // I2C setup
    I2CPortStruct* i2cPort = camera->I2CPort;

    // Change default I2C address
    if (camera->I2CAddress != OV5640_DEFAULT_I2C_ADDR && !i2cPort->WriteValue(i2cPort, I2CTransferA2D1, OV5640_REG_SLAVE_ID, camera->I2CAddress << 1)) {
        return false;
    }
    i2cPort->SetAddress(i2cPort, camera->I2CAddress);

    if (!i2cPort->ShortLoad(i2cPort, OV5640ConfigClock)) {
        return false;
    }

    if (!i2cPort->ShortLoad(i2cPort, camera->Port == CameraPortDVP ? OV5640ConfigDVP : OV5640ConfigMIPI)) {
        return false;
    }

    if (!i2cPort->ShortLoad(i2cPort, OV5640ConfigISP)) {
        return false;
    }

    switch (camera->Color) {
        case CameraColorRGB565:
            OV5640ConfigFormatTiming[0].Value = 0x6F;
            OV5640ConfigFormatTiming[1].Value = 0x01;
            break;
        case CameraColorYUV420:
            OV5640ConfigFormatTiming[0].Value = 0x4F;
            OV5640ConfigFormatTiming[1].Value = 0x00;
            break;
        case CameraColorYUV422:
            OV5640ConfigFormatTiming[0].Value = 0x32;
            OV5640ConfigFormatTiming[1].Value = 0x00;
            break;
        case CameraColorRaw8:
        case CameraColorRaw10:
            OV5640ConfigFormatTiming[0].Value = 0x00;
            OV5640ConfigFormatTiming[1].Value = 0x03;
            OV5640ConfigFormatTiming[2].Value = camera->Color == CameraColorRaw10 ? 10 : 8;
            break;
        default:
            break;
    }

    if (!i2cPort->ShortLoad(i2cPort, OV5640ConfigFormatTiming)) {
        return false;
    }

    if (!i2cPort->WriteValue(i2cPort, I2CTransferA2D1, OV5640_REG_SYS_CTRL0, OV5640_REG_SYS_CTRL0_SW_PWUP))
        return false;
#endif

    return CameraInit(camera);
}

bool OV5640DVPConstractor(CameraStruct* camera) {
    camera->Window     = (CameraWindowStruct) {0, 0, OV5640_CROP_WIDTH, OV5640_CROP_HEIGHT};
    camera->I2CAddress = OV5640_DEFAULT_I2C_ADDR;

    camera->IsHsyncValidLow       = true;
    camera->IsVsyncValidLow       = false;
    camera->IsPclkFallingSampling = true;

    camera->Color       = CONFIG_OV5640_DEFAULT_COLOR;
    camera->MemoryColor = camera->Color;

    if (camera->ClockIO != IONone)
        camera->FrequencyKHz = CONFIG_OV5640_XCLK / 1000;

    // PreInit
    if (!CameraClockInit(camera))
        return false;

#if CONFIG_BOARD != 0
    if (!CameraStartupSequence(camera, true, true, true, 8, 7, 25))
        return false;

    // I2C setup
    I2CPortStruct* i2cPort = camera->I2CPort;
    i2cPort->SetAddress(i2cPort, OV5640_DEFAULT_I2C_ADDR);

    uint16_t ov5640ID = 0;
    if (!i2cPort->ReadValue(i2cPort, I2CTransferA2D2, OV5640_REG_CHIP_ID, &ov5640ID) || ov5640ID != 0x5640)
        return false;
#endif

    camera->Init = Init;

    camera->I2CAddress = CONFIG_OV5640_DVP_I2C_ADDR;

    camera->ExecuteCommand = ExcuteCommand;

    return true;
}

/*
 * After trying the various combinations, reading various
 * documentations spread around the net, and from the various
 * feedback, the clock tree is probably as follows:
 *
 *   +--------------+
 *   |  Ext. Clock  |
 *   +-+------------+
 *     |  +----------+
 *     +->|   PLL1   | - reg 0x3036, for the multiplier
 *        +-+--------+ - reg 0x3037, bits 0-3 for the pre-divider
 *          |  +--------------+
 *          +->| System Clock |  - reg 0x3035, bits 4-7
 *             +-+------------+
 *               |  +--------------+
 *               +->| MIPI Divider | - reg 0x3035, bits 0-3
 *               |  +-+------------+
 *               |    +----------------> MIPI SCLK
 *               |    +  +-----+
 *               |    +->| / 2 |-------> MIPI BIT CLK
 *               |       +-----+
 *               |  +--------------+
 *               +->| PLL Root Div | - reg 0x3037, bit 4
 *                  +-+------------+
 *                    |  +---------+
 *                    +->| Bit Div | - reg 0x3034, bits 0-3
 *                       +-+-------+
 *                         |  +-------------+
 *                         +->| SCLK Div    | - reg 0x3108, bits 0-1
 *                         |  +-+-----------+
 *                         |    +---------------> SCLK
 *                         |  +-------------+
 *                         +->| SCLK 2X Div | - reg 0x3108, bits 2-3
 *                         |  +-+-----------+
 *                         |    +---------------> SCLK 2X
 *                         |  +-------------+
 *                         +->| PCLK Div    | - reg 0x3108, bits 4-5
 *                            ++------------+
 *                             +  +-----------+
 *                             +->|   P_DIV   | - reg 0x3035, bits 0-3
 *                                +-----+-----+
 *                                       +------------> PCLK
 *
 * There seems to be also constraints:
 *  - the PLL pre-divider output rate should be in the 4-27MHz range
 *  - the PLL multiplier output rate should be in the 500-1000MHz range
 *  - PCLK >= SCLK * 2 in YUV, >= SCLK in Raw or JPEG
 */
static const I2CShortConfigStruct OV5640ConfigClock[] = {
    {
        OV5640_REG_SYS_CTRL0,
        OV5640_REG_SYS_CTRL0_SW_RST,
        0,
        30,
    }, // Software Reset
    {
        OV5640_REG_SYS_CTRL0,
        OV5640_REG_SYS_CTRL0_SW_PWDN,
        0,
        300,
    }, // Software Release Reset and Power down
    {
        0x3007,
        0x00,
        0,
        0,
    }, // Clock Disable

    {OV5640_REG_PAD_OUTPUT_ENABLE01, 0x00}, // DVP Output Disable
    {OV5640_REG_PAD_OUTPUT_ENABLE02, 0x00}, // GPIO and DVP Output Disable
    {OV5640_REG_PAD_OUTPUT00, 0x00},        // MIPI Pad Disable
    {OV5640_REG_SCCB_SYS_CTRL1, 0x03},      // Use PLL Clock as system clock
    {
        OV5640_REG_SYS_ROOT_DIVIDER,
        OV5640_PCLK_DIV << 4 | OV5640_SCLK2_DIV << 2 | OV5640_SCLK_DIV,
        0x3F,
        10,
    }, // PCLK, SCLK, SCLK2 DIV
    {
        OV5640_REG_SC_PLL_CTRL0,
        OV5640_BIT_DIV,
        0x0F,
        10,
    }, // MIPI Bit mode
    {
        OV5640_REG_SC_PLL_CTRL1,
        OV5640_SYS_DIV << 4 | OV5640_MIPI_DIV,
        0xFF,
        10,
    }, // SYS, MIPI DIV
    {
        OV5640_REG_SC_PLL_CTRL2,
        OV5640_PLL_MULT,
        0xFF,
        10,
    }, // PLL multiple
    {
        OV5640_REG_SC_PLL_CTRL3,
        OV5640_PLL_ROOT_DIV << 4 | OV5640_PLL_PRE_DIV,
        0x1F,
        10,
    }, // PLL Div

    {0x4825, 0}, // LPX (PCLK)
    {0x4830, 0}, // LPX (UI)

    {0x4826, 0},
    {0x4827, 40}, // Prepare (ns)
    {0x4831, 5},  // Prepare (UI)

    {0x4819, 160}, // HS zero (ns)
    {0x482A, 20},  // HS zero (UI) > Prepare (UI)
    {
        OV5640_REG_PCLK_PERIOD,
        OV5640_PCLK_PERIOD,
        0xFF,
    }, // PCLK period
    {
        OV5640_REG_SC_PLLS_CTRL1,
        CONFIG_OV5640_PLLS_MULT,
        0x1F,
    }, // PLLS Multiplier: default 25
    {
        OV5640_REG_SC_PLLS_CTRL2,
        0x11,
    }, // PLLS system divider
    {
        OV5640_REG_SC_PLLS_CTRL3,
        CONFIG_OV5640_PLLS_PRE_DIV << 4 | CONFIG_OV5640_PLLS_ROOT_MULT << 2,
        0x37,
    }, // PLLS DIV

    // {0x503d, 0x80 | BIT1},
    {},
};

static const I2CShortConfigStruct OV5640ConfigDVP[] = {
    /*
        configure parallel port control lines polarity

        POLARITY CTRL0
         - [5]: PCLK polarity (0: active low, 1: active high)
         - [1]: HREF polarity (0: active low, 1: active high)
         - [0]: VSYNC polarity (mismatch here between
                datasheet and hardware, 0 is active high
                and 1 is active low...)
    */
    {
        OV5640_REG_POLARITY_CTRL00,
        BIT0 | BIT1 | BIT5,
        0xFF,
        10,
    },
    /*
        powerdown MIPI TX/RX PHY & enable DVP

        MIPI CONTROL 00
         - [4] = 1 : Power down MIPI HS Tx
         - [3] = 1 : Power down MIPI LS Rx
         - [2] = 0 : DVP enable (MIPI disable)
    */
    {
        OV5640_REG_IO_MIPI_CTRL00,
        0x58,
    },
    /*
        enable VSYNC/HREF/PCLK DVP control lines & D[9:6] DVP data lines

        PAD OUTPUT ENABLE 01
         - 6     : VSYNC output enable
         - 5     : HREF output enable
         - 4     : PCLK output enable
         - [3:0] : D[9:6] output enable
    */
    {
        OV5640_REG_PAD_OUTPUT_ENABLE01,
        0x7F,
    },
    /*
        enable D[5:0] DVP data lines

        PAD OUTPUT ENABLE 02
            - [7:2] :   D[5:0] output enable
    */
    {
        OV5640_REG_PAD_OUTPUT_ENABLE02,
        0xFC,
    },
    {0x4709, 0x01},
    {0x470A, 0x00},
    {0x470B, 0x00},
    {0x471D, 0x00},

    {0x460C, 0x20 | BIT1},
    {0x3824, 1, BITS(4, 0)},
    // {0x4741, 0x07},
    // {0x503d, 0x80},
    {},
};

static const I2CShortConfigStruct OV5640ConfigMIPI[] = {
    {
        OV5640_REG_IO_MIPI_CTRL00,
        CONFIG_OV5640_LANES << 5,
        0xFF,
        10,
    }, // Disable MIPI
    {
        OV5640_REG_MIPI_CTRL00,
        0x04,
        0xFF,
        10,
    }, // MIPI Clock
    {
        0x4805,
        0x30, // lp_min
    },
    {
        OV5640_REG_PAD_OUTPUT00,
        0x70,
        0xFF,
        10,
    }, // MIPI Pad
    {
        OV5640_REG_IO_MIPI_CTRL00,
        BIT2 | BIT0,
        BIT4 | BIT2 | BIT0,
        10,
    }, // Enable MIPI
    {
        OV5640_REG_FRAME_CTRL01,
        0x00,
        0,
        10,
    }, // Bypass frame control
    {
        OV5640_REG_TIMING_TC_REG21,
        BIT0,
        BIT0,
        10,
    }, // Binning enable
    {
        OV5640_REG_SYSTEM_CONTROL1,
        0x08,
        0xFF,
        0,
    },
    {
        OV5640_REG_PAD_CTRL00,
        0x82,
        0xFF,
        0,
    },
    {},
};

static const I2CShortConfigStruct OV5640ConfigISP[] = {
    {0x3630, 0x36},
    {0x3631, 0x0e},
    {0x3632, 0xe2},
    {0x3633, 0x12},
    {0x3621, 0xe0},
    {0x3704, 0xa0},
    {0x3703, 0x5a},
    {0x3715, 0x78},
    {0x3717, 0x01},
    {0x370b, 0x60},
    {0x3705, 0x1a},
    {0x3905, 0x02},
    {0x3906, 0x10},
    {0x3901, 0x0a},
    {0x3731, 0x12},
    {0x3600, 0x08}, // VCM control
    {0x3601, 0x33}, // VCM control
    {0x302d, 0x60}, // system control
    {0x3620, 0x52},
    {0x371b, 0x20},
    {0x471c, 0x50},
    {0x3a13, 0x43}, // pre-gain = 1.047x
    {0x3a18, 0x00}, // gain ceiling
    {0x3a19, 0xf8}, // gain ceiling = 15.5x
    {0x3635, 0x13},
    {0x3636, 0x03},
    {0x3634, 0x40},
    {0x3622, 0x01},

    // 50/60Hz
    {0x3c01, 0x34}, // Band auto, bit[7]
    {0x3c04, 0x28}, // threshold low sum
    {0x3c05, 0x98}, // threshold high sum
    {0x3c06, 0x00}, // light meter 1 threshold[15:8]
    {0x3c07, 0x08}, // light meter 1 threshold[7:0]
    {0x3c08, 0x00}, // light meter 2 threshold[15:8]
    {0x3c09, 0x1c}, // light meter 2 threshold[7:0]
    {0x3c0a, 0x9c}, // sample number[15:8]
    {0x3c0b, 0x40}, // sample number[7:0]
    {0x3810, 0x00}, // Timing Hoffset[11:8]
    {0x3811, 0x10}, // Timing Hoffset[7:0]
    {0x3812, 0x00}, // Timing Voffset[10:8]
    {0x3708, 0x64},
    {0x4001, 0x02}, // BLC start from line 2
    {0x4005, 0x1a}, // BLC always update
    {0x3000, 0x00}, // enable blocks
    {0x3004, 0xff}, // enable clocks

    {0x440e, 0x00},
    {0x5000, 0xa7}, // Lenc on, raw gamma on, BPC on, WPC on, CIP on

    // AEC target 自动曝光控制
    {0x3a0f, 0x30}, // stable range in high
    {0x3a10, 0x28}, // stable range in low
    {0x3a1b, 0x30}, // stable range out high
    {0x3a1e, 0x26}, // stable range out low
    {0x3a11, 0x60}, // fast zone high
    {0x3a1f, 0x14}, // fast zone low

    // Lens correction
    {0x5800, 0x23},
    {0x5801, 0x14},
    {0x5802, 0x0f},
    {0x5803, 0x0f},
    {0x5804, 0x12},
    {0x5805, 0x26},
    {0x5806, 0x0c},
    {0x5807, 0x08},
    {0x5808, 0x05},
    {0x5809, 0x05},
    {0x580a, 0x08},
    {0x580b, 0x0d},
    {0x580c, 0x08},
    {0x580d, 0x03},
    {0x580e, 0x00},
    {0x580f, 0x00},
    {0x5810, 0x03},
    {0x5811, 0x09},
    {0x5812, 0x07},
    {0x5813, 0x03},
    {0x5814, 0x00},
    {0x5815, 0x01},
    {0x5816, 0x03},
    {0x5817, 0x08},
    {0x5818, 0x0d},
    {0x5819, 0x08},
    {0x581a, 0x05},
    {0x581b, 0x06},
    {0x581c, 0x08},
    {0x581d, 0x0e},
    {0x581e, 0x29},
    {0x581f, 0x17},
    {0x5820, 0x11},
    {0x5821, 0x11},
    {0x5822, 0x15},
    {0x5823, 0x28},
    {0x5824, 0x46},
    {0x5825, 0x26},
    {0x5826, 0x08},
    {0x5827, 0x26},
    {0x5828, 0x64},
    {0x5829, 0x26},
    {0x582a, 0x24},
    {0x582b, 0x22},
    {0x582c, 0x24},
    {0x582d, 0x24},
    {0x582e, 0x06},
    {0x582f, 0x22},
    {0x5830, 0x40},
    {0x5831, 0x42},
    {0x5832, 0x24},
    {0x5833, 0x26},
    {0x5834, 0x24},
    {0x5835, 0x22},
    {0x5836, 0x22},
    {0x5837, 0x26},
    {0x5838, 0x44},
    {0x5839, 0x24},
    {0x583a, 0x26},
    {0x583b, 0x28},
    {0x583c, 0x42},
    {0x583d, 0xce}, // lenc BR offset

    // AWB
    {0x5180, 0xff}, // AWB B block
    {0x5181, 0xf2}, // AWB control
    {0x5182, 0x00}, // [7:4] max local counter, [3:0] max fast counter
    {0x5183, 0x14}, // AWB advanced
    {0x5184, 0x25},
    {0x5185, 0x24},
    {0x5186, 0x09},
    {0x5187, 0x09},
    {0x5188, 0x09},
    {0x5189, 0x75},
    {0x518a, 0x54},
    {0x518b, 0xe0},
    {0x518c, 0xb2},
    {0x518d, 0x42},
    {0x518e, 0x3d},
    {0x518f, 0x56},
    {0x5190, 0x46},
    {0x5191, 0xf8}, // AWB top limit
    {0x5192, 0x04}, // AWB bottom limit
    {0x5193, 0x70}, // red limit
    {0x5194, 0xf0}, // green limit
    {0x5195, 0xf0}, // blue limit
    {0x5196, 0x03}, // AWB control
    {0x5197, 0x01}, // local limit
    {0x5198, 0x04},
    {0x5199, 0x12},
    {0x519a, 0x04},
    {0x519b, 0x00},
    {0x519c, 0x06},
    {0x519d, 0x82},
    {0x519e, 0x38}, // AWB control

    // Gamma
    {0x5480, 0x01}, // Gamma bias plus on, bit[0]
    {0x5481, 0x08},
    {0x5482, 0x14},
    {0x5483, 0x28},
    {0x5484, 0x51},
    {0x5485, 0x65},
    {0x5486, 0x71},
    {0x5487, 0x7d},
    {0x5488, 0x87},
    {0x5489, 0x91},
    {0x548a, 0x9a},
    {0x548b, 0xaa},
    {0x548c, 0xb8},
    {0x548d, 0xcd},
    {0x548e, 0xdd},
    {0x548f, 0xea},
    {0x5490, 0x1d},

    // color matrix
    {0x5381, 0x1e}, // CMX1 for Y
    {0x5382, 0x5b}, // CMX2 for Y
    {0x5383, 0x08}, // CMX3 for Y
    {0x5384, 0x0a}, // CMX4 for U
    {0x5385, 0x7e}, // CMX5 for U
    {0x5386, 0x88}, // CMX6 for U
    {0x5387, 0x7c}, // CMX7 for V
    {0x5388, 0x6c}, // CMX8 for V
    {0x5389, 0x10}, // CMX9 for V
    {0x538a, 0x01}, // sign[9]
    {0x538b, 0x98}, // sign[8:1]

    // UV
    {0x5580, 0x06}, // saturation on, bit[1]
    {0x5583, 0x40},
    {0x5584, 0x10},
    {0x5589, 0x10},
    {0x558a, 0x00},
    {0x558b, 0xf8},
    {0x501d, 0x40}, // enable manual offset of contrast

    // CIP
    {0x5300, 0x08}, // CIP sharpen MT threshold 1
    {0x5301, 0x30}, // CIP sharpen MT threshold 2
    {0x5302, 0x10}, // CIP sharpen MT offset 1
    {0x5303, 0x00}, // CIP sharpen MT offset 2
    {0x5304, 0x08}, // CIP DNS threshold 1
    {0x5305, 0x30}, // CIP DNS threshold 2
    {0x5306, 0x08}, // CIP DNS offset 1
    {0x5307, 0x16}, // CIP DNS offset 2
    {0x5309, 0x08}, // CIP sharpen TH threshold 1
    {0x530a, 0x30}, // CIP sharpen TH threshold 2
    {0x530b, 0x04}, // CIP sharpen TH offset 1
    {0x530c, 0x06}, // CIP sharpen TH offset 2
    {0x5025, 0x00},
    // Debug
    {0x3618, 0x00},
    {0x3612, 0x29},
    {0x3708, 0x64},
    {0x3709, 0x52},
    {0x370c, 0x03},

    {0x3a02, 0x0A}, // 60Hz max exposure
    {0x3a03, 0xac}, // 60Hz max exposure

    // ISP Enable
    //{0x5000, BIT5 | BIT2 | BIT1 | BIT0}, // 5: RAW GMA 2: BlackCancelation 1: WhiteCancelation 0: Color Interpolation
    {0x5001, BIT7 | BIT1 | BIT0}, // 1: Color Matrix 0: AWB
    //{0x501D, BIT4},               // 4: Average size manual enable
    {},
};

// Format & Timing
static I2CShortConfigStruct OV5640ConfigFormatTiming[] = {
    // Format
    {OV5640_REG_FORMAT_CONTROL00, OV5640_FORMAT_DEFAULT, 0, 0},          // RGB565
    {OV5640_REG_ISP_FORMAT_MUX_CTRL, OV5640_ISP_FORMAT, 0, 0},           // ISP RGB
    {OV5640_REG_TIMING_TC_REG21, BIT0, BIT5 | BIT0, 0},                  // 5: JPEG; 0: H Binning
    {OV5640_REG_SYS_RESET02, BIT4 | BIT3 | BIT2, BIT4 | BIT3 | BIT2, 0}, // Reset: J/SFIFO JPG
    {OV5640_REG_SYS_CLOCK_ENABLE02, 0, BIT5 | BIT3, 0},                  // Disable JPEG clocks
    // Timing
    {0x3814, 0x31, 0xFF, 0}, // X Subsample: Odd inc 3, even inc 1
    {0x3815, 0x31, 0xFF, 0}, // Y Subsample: Odd inc 3, even inc 1
    {OV5640_REG_TIMING_HS, OV5640_PIXEL_ARRAY_LEFT, 0x0FFF, 0},
    {OV5640_REG_TIMING_VS, OV5640_PIXEL_ARRAY_TOP, 0x0FFF, 0},
    {OV5640_REG_TIMING_HW, OV5640_PIXEL_ARRAY_LEFT + OV5640_PIXEL_ARRAY_WIDTH - 1, 0x0FFF, 0},
    {OV5640_REG_TIMING_VH, OV5640_PIXEL_ARRAY_TOP + OV5640_PIXEL_ARRAY_HEIGHT - 1, 0x0FFF, 0},
    {OV5640_REG_TIMING_DVPHO, OV5640_CROP_WIDTH, 0x0FFF, 0},
    {OV5640_REG_TIMING_DVPVO, OV5640_CROP_HEIGHT, 0x0FFF, 0},
    {OV5640_REG_TIMING_HOFFS, OV5640_CROP_LEFT, 0x0FFF, 0},
    {OV5640_REG_TIMING_VOFFS, OV5640_CROP_TOP, 0x0FFF, 0},
    {OV5640_REG_TIMING_HTS, OV5640_MIPI_HTS, 0x1FFF, 0},
    {OV5640_REG_TIMING_VTS, OV5640_MIPI_VTS, 0xFFFF, 0},
    // ISP AvA Window
    {0x5680, 0, 0x0FFF, 0},
    {0x5682, 0, 0x0FFF, 0},
    {0x5684, OV5640_CROP_WIDTH, 0x0FFF, 0},
    {0x5686, OV5640_CROP_HEIGHT, 0x0FFF, 0},
    {},
};
