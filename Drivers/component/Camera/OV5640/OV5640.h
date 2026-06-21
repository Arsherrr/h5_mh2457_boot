#ifndef __OV5640_H__
#define __OV5640_H__

#include "Camera.h"

// ------- Config Definitions --------

#ifndef CONFIG_OV5640_XCLK
#define CONFIG_OV5640_XCLK 15000000
#endif

// { CameraColorRGB565, CameraColorYUV422, CameraColorRAW8 }
#ifndef CONFIG_OV5640_DEFAULT_COLOR
#define CONFIG_OV5640_DEFAULT_COLOR CameraColorRGB565
#endif

// {0, 1, 2, 3} : {2, 3, 4, 6}
#ifndef CONFIG_OV5640_PLLS_PRE_DIV
#define CONFIG_OV5640_PLLS_PRE_DIV 1
#endif

// {0, 1} : {1, 2}
#ifndef CONFIG_OV5640_PLLS_ROOT_MULT
#define CONFIG_OV5640_PLLS_ROOT_MULT 1
#endif

// (0, 31]
#ifndef CONFIG_OV5640_PLLS_MULT
#define CONFIG_OV5640_PLLS_MULT 25
#endif

#ifndef CONFIG_OV5640_LANES
#define CONFIG_OV5640_LANES 1
#endif

#ifndef CONFIG_OV5640_CSI_I2C_ADDR
#define CONFIG_OV5640_CSI_I2C_ADDR 0x3D
#endif

#ifndef CONFIG_OV5640_DVP_I2C_ADDR
#define CONFIG_OV5640_DVP_I2C_ADDR 0x3E
#endif

// ------- Static Definitions --------

#define OV5640_BIT_DIV      8 // 8: 2; 10: 2.5
#define OV5640_SCLK_DIV     1
#define OV5640_SCLK2_DIV    0
#define OV5640_PCLK_DIV     0
#define OV5640_MIPI_DIV     1
#define OV5640_SYS_DIV      1
#define OV5640_PLL_ROOT_DIV 1
#define OV5640_PLL_MULT     (360 / 15)
#define OV5640_PLL_PRE_DIV  (CONFIG_OV5640_XCLK / 15000000)

#define OV5640_CROP_LEFT   16
#define OV5640_CROP_TOP    16
#define OV5640_CROP_WIDTH  800
#define OV5640_CROP_HEIGHT 800

#define OV5640_MIPI_VBLANK 32
#define OV5640_MIPI_HTS    2000                                      // >= OV5640_PIXEL_ARRAY_WIDTH + 98
#define OV5640_MIPI_VTS    (OV5640_MIPI_VBLANK + OV5640_CROP_HEIGHT) // >= (OV5640_PIXEL_ARRAY_HEIGHT + 10 + VSYNC_LINES)

// ------- CONFIGS END --------

#define OV5640_SYS_CLK     (CONFIG_OV5640_XCLK / OV5640_PLL_PRE_DIV * OV5640_PLL_MULT / OV5640_SYS_DIV)              // 384M
#define OV5640_MIPI_SCLK   (OV5640_SYS_CLK / OV5640_MIPI_DIV)                                                        // 384M
#define OV5640_MIPI_CLK    (OV5640_MIPI_SCLK / 2)                                                                    // 192M
#define OV5640_BIT_CLK     (OV5640_SYS_CLK / ((OV5640_PLL_ROOT_DIV + 1) * 10 / (OV5640_BIT_DIV == 8 ? 5 : 4)))       // 96M
#define OV5640_SCLK_CLK    (OV5640_BIT_CLK / (1 << OV5640_SCLK_DIV))                                                 // 48M
#define OV5640_SCLK2_CLK   (OV5640_BIT_CLK / (1 << OV5640_SCLK2_DIV))                                                // 96M
#define OV5640_PCLK_CLK    (OV5640_BIT_CLK / (1 << OV5640_PCLK_DIV) / (OV5640_MIPI_DIV * (3 - CONFIG_OV5640_LANES))) // 96M
#define OV5640_PCLK_PERIOD (2000000U / (OV5640_PCLK_CLK / 1000U))                                                    // 40ns

#define OV5640_ADC_CLK                                                                                                                         \
    ((CONFIG_OV5640_XCLK * 2 / (CONFIG_OV5640_PLLS_PRE_DIV < 3 ? (CONFIG_OV5640_PLLS_PRE_DIV + 2) : 6)) * (CONFIG_OV5640_PLLS_ROOT_MULT + 1) * \
     CONFIG_OV5640_PLLS_MULT)

#define OV5640_LINE_TIME (1000000000 / OV5640_PCLK_CLK * OV5640_MIPI_HTS)

#define OV5640_XCLK_MIN 6000000
#define OV5640_XCLK_MAX 54000000

#define OV5640_NATIVE_WIDTH   2624 // dummy left 16         + active 2592 + dummy left 16
#define OV5640_NATIVE_HEIGHT  1964 // black 8 + dummy top 6 + active 1944 + dummy buttom 6
#define OV5640_MAX_ACT_WIDTH  2592
#define OV5640_MAX_ACT_HEIGHT 1944

#define OV5640_PIXEL_ARRAY_TOP    14
#define OV5640_PIXEL_ARRAY_LEFT   16
#define OV5640_PIXEL_ARRAY_WIDTH  2592
#define OV5640_PIXEL_ARRAY_HEIGHT 1632

/* FIXME: not documented. */
#define OV5640_MIN_VBLANK 24
#define OV5640_MAX_VTS    3375

#define OV5640_DEFAULT_I2C_ADDR 0x3C

#define OV5640_REG_SYS_RESET02         0x3002
#define OV5640_REG_SYS_CLOCK_ENABLE02  0x3006
#define OV5640_REG_SYS_CTRL0           0x3008
#define OV5640_REG_SYS_CTRL0_SW_PWDN   0x42
#define OV5640_REG_SYS_CTRL0_SW_PWUP   0x02
#define OV5640_REG_SYS_CTRL0_SW_RST    0x82
#define OV5640_REG_CHIP_ID             0x300a
#define OV5640_REG_IO_MIPI_CTRL00      0x300e
#define OV5640_REG_PAD_OUTPUT_ENABLE01 0x3017
#define OV5640_REG_PAD_OUTPUT_ENABLE02 0x3018
#define OV5640_REG_PAD_OUTPUT00        0x3019
#define OV5640_REG_SYSTEM_CONTROL1     0x302e
#define OV5640_REG_PAD_CTRL00          0x302c
#define OV5640_REG_SC_PLL_CTRL0        0x3034
#define OV5640_REG_SC_PLL_CTRL1        0x3035
#define OV5640_REG_SC_PLL_CTRL2        0x3036
#define OV5640_REG_SC_PLL_CTRL3        0x3037
#define OV5640_REG_SC_PLLS_CTRL1       0x303B
#define OV5640_REG_SC_PLLS_CTRL2       0x303C
#define OV5640_REG_SC_PLLS_CTRL3       0x303D
#define OV5640_REG_SLAVE_ID            0x3100
#define OV5640_REG_SCCB_SYS_CTRL1      0x3103
#define OV5640_REG_SYS_ROOT_DIVIDER    0x3108
#define OV5640_REG_AWB_R_GAIN          0x3400
#define OV5640_REG_AWB_G_GAIN          0x3402
#define OV5640_REG_AWB_B_GAIN          0x3404
#define OV5640_REG_AWB_MANUAL_CTRL     0x3406
#define OV5640_REG_AEC_PK_EXPOSURE_HI  0x3500
#define OV5640_REG_AEC_PK_EXPOSURE_MED 0x3501
#define OV5640_REG_AEC_PK_EXPOSURE_LO  0x3502
#define OV5640_REG_AEC_PK_MANUAL       0x3503
#define OV5640_REG_AEC_PK_REAL_GAIN    0x350a
#define OV5640_REG_AEC_PK_VTS          0x350c
#define OV5640_REG_TIMING_HS           0x3800
#define OV5640_REG_TIMING_VS           0x3802
#define OV5640_REG_TIMING_HW           0x3804
#define OV5640_REG_TIMING_VH           0x3806
#define OV5640_REG_TIMING_DVPHO        0x3808
#define OV5640_REG_TIMING_DVPVO        0x380a
#define OV5640_REG_TIMING_HTS          0x380c
#define OV5640_REG_TIMING_VTS          0x380e
#define OV5640_REG_TIMING_HOFFS        0x3810
#define OV5640_REG_TIMING_VOFFS        0x3812
#define OV5640_REG_TIMING_TC_REG20     0x3820
#define OV5640_REG_TIMING_TC_REG21     0x3821
#define OV5640_REG_AEC_CTRL00          0x3a00
#define OV5640_REG_AEC_B50_STEP        0x3a08
#define OV5640_REG_AEC_B60_STEP        0x3a0a
#define OV5640_REG_AEC_CTRL0D          0x3a0d
#define OV5640_REG_AEC_CTRL0E          0x3a0e
#define OV5640_REG_AEC_CTRL0F          0x3a0f
#define OV5640_REG_AEC_CTRL10          0x3a10
#define OV5640_REG_AEC_CTRL11          0x3a11
#define OV5640_REG_AEC_CTRL1B          0x3a1b
#define OV5640_REG_AEC_CTRL1E          0x3a1e
#define OV5640_REG_AEC_CTRL1F          0x3a1f
#define OV5640_REG_HZ5060_CTRL00       0x3c00
#define OV5640_REG_HZ5060_CTRL01       0x3c01
#define OV5640_REG_SIGMADELTA_CTRL0C   0x3c0c
#define OV5640_REG_FRAME_CTRL01        0x4202
#define OV5640_REG_FORMAT_CONTROL00    0x4300
#define OV5640_REG_VFIFO_HSIZE         0x4602
#define OV5640_REG_VFIFO_VSIZE         0x4604
#define OV5640_REG_JPG_MODE_SELECT     0x4713
#define OV5640_REG_CCIR656_CTRL00      0x4730
#define OV5640_REG_POLARITY_CTRL00     0x4740
#define OV5640_REG_MIPI_CTRL00         0x4800
#define OV5640_REG_DEBUG_MODE          0x4814
#define OV5640_REG_PCLK_PERIOD         0x4837
#define OV5640_REG_ISP_FORMAT_MUX_CTRL 0x501f
#define OV5640_REG_PRE_ISP_TEST_SET1   0x503d
#define OV5640_REG_SDE_CTRL0           0x5580
#define OV5640_REG_SDE_CTRL1           0x5581
#define OV5640_REG_SDE_CTRL3           0x5583
#define OV5640_REG_SDE_CTRL4           0x5584
#define OV5640_REG_SDE_CTRL5           0x5585
#define OV5640_REG_AVG_READOUT         0x56a1

#define OV5640_FORMAT_YUYV   0x30
#define OV5640_FORMAT_UYVY   0x32
#define OV5640_FORMAT_RGB565 0x6F

#define OV5640_FORMAT_DEFAULT OV5640_FORMAT_RGB565
#define OV5640_FORMAT_TYPE    ((OV5640_FORMAT_DEFAULT & 0xF0) >> 4)
#define OV5640_ISP_FORMAT     0x01

extern bool OV5640DVPConstractor(CameraStruct* ov5640);

#endif
