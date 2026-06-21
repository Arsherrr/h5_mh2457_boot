// Copyright (c) 2011-2024 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_LTDC_H__
#define __PERIPH_LTDC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mh2457.h"

/* --------  Feature Definition  -------------------------------------------- */

#ifndef FEATURE_LTDC_SUPPORT_BURST
#define FEATURE_LTDC_SUPPORT_BURST 0
#endif

/* --------  Register Definition  ------------------------------------------- */

typedef struct
{
    __I uint32_t  RSVD00[2]; /*!< Reserved, 0x00-0x04 */
    __IO uint32_t SSCR;      /*!< LTDC Synchronization Size Configuration Register,    Address offset: 0x08 */
    __IO uint32_t BPCR;      /*!< LTDC Back Porch Configuration Register,              Address offset: 0x0C */
    __IO uint32_t AWCR;      /*!< LTDC Active Width Configuration Register,            Address offset: 0x10 */
    __IO uint32_t TWCR;      /*!< LTDC Total Width Configuration Register,             Address offset: 0x14 */
    __IO uint32_t GCR;       /*!< LTDC Global Control Register,                        Address offset: 0x18 */
    __I uint32_t  RSVD1C[2]; /*!< Reserved, 0x1C-0x20 */
    __IO uint32_t SRCR;      /*!< LTDC Shadow Reload Configuration Register,           Address offset: 0x24 */
    __I uint32_t  RSVD28[1]; /*!< Reserved, 0x28 */
    __IO uint32_t BCCR;      /*!< LTDC Background Color Configuration Register,        Address offset: 0x2C */
    __I uint32_t  RSVD30[1]; /*!< Reserved, 0x30 */
    __IO uint32_t IER;       /*!< LTDC Interrupt Enable Register,                      Address offset: 0x34 */
    __IO uint32_t ISR;       /*!< LTDC Interrupt Status Register,                      Address offset: 0x38 */
    __IO uint32_t ICR;       /*!< LTDC Interrupt Clear Register,                       Address offset: 0x3C */
    __IO uint32_t LIPCR;     /*!< LTDC Line Interrupt Position Configuration Register, Address offset: 0x40 */
    __IO uint32_t CPSR;      /*!< LTDC Current Position Status Register,               Address offset: 0x44 */
    __IO uint32_t CDSR;      /*!< LTDC Current Display Status Register,                Address offset: 0x48 */
    __IO uint32_t DSI_WCFGR; // 0x4C
    __IO uint32_t DSI_WCR;   // 0x50
    __IO uint32_t DSI_WIER;  // 0x54
    __IO uint32_t DSI_WIFCR; // 0x58
    __IO uint32_t DSI_WISR;  // 0x5C
#if FEATURE_LTDC_SUPPORT_BURST
    __IO uint32_t AHBCFG;
#endif
} LTDC_TypeDef;

typedef struct
{
    __IO uint32_t CR;        /*!< LTDC Layerx Control Register                                  Address offset: 0x84 */
    __IO uint32_t WHPCR;     /*!< LTDC Layerx Window Horizontal Position Configuration Register Address offset: 0x88 */
    __IO uint32_t WVPCR;     /*!< LTDC Layerx Window Vertical Position Configuration Register   Address offset: 0x8C */
    __IO uint32_t CKCR;      /*!< LTDC Layerx Color Keying Configuration Register               Address offset: 0x90 */
    __IO uint32_t PFCR;      /*!< LTDC Layerx Pixel Format Configuration Register               Address offset: 0x94 */
    __IO uint32_t CACR;      /*!< LTDC Layerx Constant Alpha Configuration Register             Address offset: 0x98 */
    __IO uint32_t DCCR;      /*!< LTDC Layerx Default Color Configuration Register              Address offset: 0x9C */
    __IO uint32_t BFCR;      /*!< LTDC Layerx Blending Factors Configuration Register           Address offset: 0xA0 */
    __I uint32_t  RSVDA4[2]; /*!< Reserved */
    __IO uint32_t CFBAR;     /*!< LTDC Layerx Color Frame Buffer Address Register               Address offset: 0xAC */
    __IO uint32_t CFBLR;     /*!< LTDC Layerx Color Frame Buffer Length Register                Address offset: 0xB0 */
    __IO uint32_t CFBLNR;    /*!< LTDC Layerx ColorFrame Buffer Line Number Register            Address offset: 0xB4 */
    uint32_t      RSVDB8[3]; /*!< Reserved */
    __IO uint32_t CLUTWR;    /*!< LTDC Layerx CLUT Write Register                               Address offset: 0x144 */
} LTDC_Layer_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/********************  Bit definition for LTDC_SSCR register  *****************/

#define LTDC_SSCR_VSH ((uint32_t)0x000007FF) /*!< Vertical Synchronization Height */
#define LTDC_SSCR_HSW ((uint32_t)0x0FFF0000) /*!< Horizontal Synchronization Width */

/********************  Bit definition for LTDC_BPCR register  *****************/

#define LTDC_BPCR_AVBP ((uint32_t)0x000007FF) /*!< Accumulated Vertical Back Porch */
#define LTDC_BPCR_AHBP ((uint32_t)0x0FFF0000) /*!< Accumulated Horizontal Back Porch */

/********************  Bit definition for LTDC_AWCR register  *****************/

#define LTDC_AWCR_AAH ((uint32_t)0x000007FF) /*!< Accumulated Active heigh */
#define LTDC_AWCR_AAW ((uint32_t)0x0FFF0000) /*!< Accumulated Active Width */

/********************  Bit definition for LTDC_TWCR register  *****************/

#define LTDC_TWCR_TOTALH ((uint32_t)0x000007FF) /*!< Total Heigh */
#define LTDC_TWCR_TOTALW ((uint32_t)0x0FFF0000) /*!< Total Width */

/********************  Bit definition for LTDC_GCR register  ******************/

#define LTDC_GCR_LTDCEN ((uint32_t)0x00000001) /*!< LCD-TFT controller enable bit */
#define LTDC_GCR_DBW    ((uint32_t)0x00000070) /*!< Dither Blue Width */
#define LTDC_GCR_DGW    ((uint32_t)0x00000700) /*!< Dither Green Width */
#define LTDC_GCR_DRW    ((uint32_t)0x00007000) /*!< Dither Red Width */
#define LTDC_GCR_DEN    ((uint32_t)0x00010000) /*!< Dither Enable */
#define LTDC_GCR_PCPOL  ((uint32_t)0x10000000) /*!< Pixel Clock Polarity */
#define LTDC_GCR_DEPOL  ((uint32_t)0x20000000) /*!< Data Enable Polarity */
#define LTDC_GCR_VSPOL  ((uint32_t)0x40000000) /*!< Vertical Synchronization Polarity */
#define LTDC_GCR_HSPOL  ((uint32_t)0x80000000) /*!< Horizontal Synchronization Polarity */

/* Legacy defines */
#define LTDC_GCR_DTEN LTDC_GCR_DEN

/********************  Bit definition for LTDC_SRCR register  *****************/

#define LTDC_SRCR_IMR ((uint32_t)0x00000001) /*!< Immediate Reload */
#define LTDC_SRCR_VBR ((uint32_t)0x00000002) /*!< Vertical Blanking Reload */

/********************  Bit definition for LTDC_BCCR register  *****************/

#define LTDC_BCCR_BCBLUE  ((uint32_t)0x000000FF) /*!< Background Blue value */
#define LTDC_BCCR_BCGREEN ((uint32_t)0x0000FF00) /*!< Background Green value */
#define LTDC_BCCR_BCRED   ((uint32_t)0x00FF0000) /*!< Background Red value */

/********************  Bit definition for LTDC_IER register  ******************/

#define LTDC_IER_LIE    ((uint32_t)0x00000001) /*!< Line Interrupt Enable */
#define LTDC_IER_FUIE   ((uint32_t)0x00000002) /*!< FIFO Underrun Interrupt Enable */
#define LTDC_IER_TERRIE ((uint32_t)0x00000004) /*!< Transfer Error Interrupt Enable */
#define LTDC_IER_RRIE   ((uint32_t)0x00000008) /*!< Register Reload interrupt enable */

/********************  Bit definition for LTDC_ISR register  ******************/

#define LTDC_ISR_LIF    ((uint32_t)0x00000001) /*!< Line Interrupt Flag */
#define LTDC_ISR_FUIF   ((uint32_t)0x00000002) /*!< FIFO Underrun Interrupt Flag */
#define LTDC_ISR_TERRIF ((uint32_t)0x00000004) /*!< Transfer Error Interrupt Flag */
#define LTDC_ISR_RRIF   ((uint32_t)0x00000008) /*!< Register Reload interrupt Flag */

/********************  Bit definition for LTDC_ICR register  ******************/

#define LTDC_ICR_CLIF    ((uint32_t)0x00000001) /*!< Clears the Line Interrupt Flag */
#define LTDC_ICR_CFUIF   ((uint32_t)0x00000002) /*!< Clears the FIFO Underrun Interrupt Flag */
#define LTDC_ICR_CTERRIF ((uint32_t)0x00000004) /*!< Clears the Transfer Error Interrupt Flag */
#define LTDC_ICR_CRRIF   ((uint32_t)0x00000008) /*!< Clears Register Reload interrupt Flag */

/********************  Bit definition for LTDC_LIPCR register  ****************/

#define LTDC_LIPCR_LIPOS ((uint32_t)0x000007FF) /*!< Line Interrupt Position */

/********************  Bit definition for LTDC_CPSR register  *****************/

#define LTDC_CPSR_CYPOS ((uint32_t)0x0000FFFF) /*!< Current Y Position */
#define LTDC_CPSR_CXPOS ((uint32_t)0xFFFF0000) /*!< Current X Position */

/********************  Bit definition for LTDC_CDSR register  *****************/

#define LTDC_CDSR_VDES   ((uint32_t)0x00000001) /*!< Vertical Data Enable Status */
#define LTDC_CDSR_HDES   ((uint32_t)0x00000002) /*!< Horizontal Data Enable Status */
#define LTDC_CDSR_VSYNCS ((uint32_t)0x00000004) /*!< Vertical Synchronization Status */
#define LTDC_CDSR_HSYNCS ((uint32_t)0x00000008) /*!< Horizontal Synchronization Status */

/********************  Bit definition for LTDC_LxCR register  *****************/

#define LTDC_LxCR_LEN    ((uint32_t)0x00000001) /*!< Layer Enable */
#define LTDC_LxCR_COLKEN ((uint32_t)0x00000002) /*!< Color Keying Enable */
#define LTDC_LxCR_CLUTEN ((uint32_t)0x00000010) /*!< Color Lockup Table Enable */

/********************  Bit definition for LTDC_LxWHPCR register  **************/

#define LTDC_LxWHPCR_WHSTPOS ((uint32_t)0x00000FFF) /*!< Window Horizontal Start Position */
#define LTDC_LxWHPCR_WHSPPOS ((uint32_t)0xFFFF0000) /*!< Window Horizontal Stop Position */

/********************  Bit definition for LTDC_LxWVPCR register  **************/

#define LTDC_LxWVPCR_WVSTPOS ((uint32_t)0x00000FFF) /*!< Window Vertical Start Position */
#define LTDC_LxWVPCR_WVSPPOS ((uint32_t)0xFFFF0000) /*!< Window Vertical Stop Position */

/********************  Bit definition for LTDC_LxCKCR register  ***************/

#define LTDC_LxCKCR_CKBLUE  ((uint32_t)0x000000FF) /*!< Color Key Blue value */
#define LTDC_LxCKCR_CKGREEN ((uint32_t)0x0000FF00) /*!< Color Key Green value */
#define LTDC_LxCKCR_CKRED   ((uint32_t)0x00FF0000) /*!< Color Key Red value */

/********************  Bit definition for LTDC_LxPFCR register  ***************/

#define LTDC_LxPFCR_PF ((uint32_t)0x00000007) /*!< Pixel Format */

/********************  Bit definition for LTDC_LxCACR register  ***************/

#define LTDC_LxCACR_CONSTA ((uint32_t)0x000000FF) /*!< Constant Alpha */

/********************  Bit definition for LTDC_LxDCCR register  ***************/

#define LTDC_LxDCCR_DCBLUE  ((uint32_t)0x000000FF) /*!< Default Color Blue */
#define LTDC_LxDCCR_DCGREEN ((uint32_t)0x0000FF00) /*!< Default Color Green */
#define LTDC_LxDCCR_DCRED   ((uint32_t)0x00FF0000) /*!< Default Color Red */
#define LTDC_LxDCCR_DCALPHA ((uint32_t)0xFF000000) /*!< Default Color Alpha */

/********************  Bit definition for LTDC_LxBFCR register  ***************/

#define LTDC_LxBFCR_BF2 ((uint32_t)0x00000007) /*!< Blending Factor 2 */
#define LTDC_LxBFCR_BF1 ((uint32_t)0x00000700) /*!< Blending Factor 1 */

/********************  Bit definition for LTDC_LxCFBAR register  **************/

#define LTDC_LxCFBAR_CFBADD ((uint32_t)0xFFFFFFFF) /*!< Color Frame Buffer Start Address */

/********************  Bit definition for LTDC_LxCFBLR register  **************/

#define LTDC_LxCFBLR_CFBLL ((uint32_t)0x00001FFF) /*!< Color Frame Buffer Line Length */
#define LTDC_LxCFBLR_CFBP  ((uint32_t)0x1FFF0000) /*!< Color Frame Buffer Pitch in bytes */

/********************  Bit definition for LTDC_LxCFBLNR register  *************/

#define LTDC_LxCFBLNR_CFBLNBR ((uint32_t)0x000007FF) /*!< Frame Buffer Line Number */

/********************  Bit definition for LTDC_LxCLUTWR register  *************/

#define LTDC_LxCLUTWR_BLUE    ((uint32_t)0x000000FF) /*!< Blue value */
#define LTDC_LxCLUTWR_GREEN   ((uint32_t)0x0000FF00) /*!< Green value */
#define LTDC_LxCLUTWR_RED     ((uint32_t)0x00FF0000) /*!< Red value */
#define LTDC_LxCLUTWR_CLUTADD ((uint32_t)0xFF000000) /*!< CLUT address */

/* --------  Exported types  ------------------------------------------------ */

// LTDC Init structure definition
typedef struct
{
    uint32_t LTDC_HSPolarity;     /*!< configures the horizontal synchronization polarity. This parameter can be one value of @ref LTDC_HSPolarity */
    uint32_t LTDC_VSPolarity;     /*!< configures the vertical synchronization polarity. This parameter can be one value of @ref LTDC_VSPolarity */
    uint32_t LTDC_DEPolarity;     /*!< configures the data enable polarity. This parameter can be one of value of @ref LTDC_DEPolarity */
    uint32_t LTDC_PCPolarity;     /*!< configures the pixel clock polarity. This parameter can be one of value of @ref LTDC_PCPolarity */
    uint32_t LTDC_HorizontalSync; /*!< configures the number of Horizontal synchronization width. This parameter must range from 0x000 to 0xFFF. */
    uint32_t LTDC_VerticalSync;   /*!< configures the number of Vertical synchronization height. This parameter must range from 0x000 to 0x7FF. */
    uint32_t LTDC_AccumulatedHBP; /*!< configures the accumulated horizontal back porch width. This parameter must range from LTDC_HorizontalSync to 0xFFF. */
    uint32_t LTDC_AccumulatedVBP; /*!< configures the accumulated vertical back porch height. This parameter must range from LTDC_VerticalSync to 0x7FF. */
    uint32_t LTDC_AccumulatedActiveW;   /*!< configures the accumulated active width. This parameter must range from LTDC_AccumulatedHBP to 0xFFF. */
    uint32_t LTDC_AccumulatedActiveH;   /*!< configures the accumulated active height. This parameter  must range from LTDC_AccumulatedVBP to 0x7FF. */
    uint32_t LTDC_TotalWidth;           /*!< configures the total width. This parameter must range from LTDC_AccumulatedActiveW to 0xFFF. */
    uint32_t LTDC_TotalHeigh;           /*!< configures the total height. This parameter must range from LTDC_AccumulatedActiveH to 0x7FF. */
    uint32_t LTDC_BackgroundRedValue;   /*!< configures the background red value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_BackgroundGreenValue; /*!< configures the background green value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_BackgroundBlueValue;  /*!< configures the background blue value. This parameter must range from 0x00 to 0xFF. */
} LTDC_InitTypeDef;

// LTDC Layer structure definition
typedef struct
{
    uint32_t LTDC_HorizontalStart;   /*!< Configures the Window Horizontal Start Position. This parameter must range from 0x000 to 0xFFF. */
    uint32_t LTDC_HorizontalStop;    /*!< Configures the Window Horizontal Stop Position. This parameter must range from 0x0000 to 0xFFFF. */
    uint32_t LTDC_VerticalStart;     /*!< Configures the Window vertical Start Position. This parameter must range from 0x000 to 0xFFF. */
    uint32_t LTDC_VerticalStop;      /*!< Configures the Window vaertical Stop Position. This parameter must range from 0x0000 to 0xFFFF. */
    uint32_t LTDC_PixelFormat;       /*!< Specifies the pixel format. This parameter can be one of value of @ref LTDC_Pixelformat */
    uint32_t LTDC_ConstantAlpha;     /*!< Specifies the constant alpha used for blending. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_DefaultColorBlue;  /*!< Configures the default blue value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_DefaultColorGreen; /*!< Configures the default green value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_DefaultColorRed;   /*!< Configures the default red value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_DefaultColorAlpha; /*!< Configures the default alpha value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_BlendingFactor_1;  /*!< Select the blending factor 1. This parameter can be one of value of @ref LTDC_BlendingFactor1 */
    uint32_t LTDC_BlendingFactor_2;  /*!< Select the blending factor 2. This parameter can be one of value of @ref LTDC_BlendingFactor2 */
    uint32_t LTDC_CFBStartAdress;    /*!< Configures the color frame buffer address */
    uint32_t LTDC_CFBLineLength;     /*!< Configures the color frame buffer line length. This parameter must range from 0x0000 to 0x1FFF. */
    uint32_t LTDC_CFBPitch;          /*!< Configures the color frame buffer pitch in bytes. This parameter must range from 0x0000 to 0x1FFF. */
    uint32_t LTDC_CFBLineNumber;     /*!< Specifies the number of line in frame buffer. This parameter must range from 0x000 to 0x7FF. */
} LTDC_Layer_InitTypeDef;

// LTDC Position structure definition
typedef struct
{
    uint32_t LTDC_POSX; /*!<  Current X Position */
    uint32_t LTDC_POSY; /*!<  Current Y Position */
} LTDC_PosTypeDef;

// LTDC RGB structure definition
typedef struct
{
    uint32_t LTDC_BlueWidth;  /*!< Blue width */
    uint32_t LTDC_GreenWidth; /*!< Green width */
    uint32_t LTDC_RedWidth;   /*!< Red width */
} LTDC_RGBTypeDef;

// LTDC Color Keying structure definition
typedef struct
{
    uint32_t LTDC_ColorKeyBlue;  /*!< Configures the color key blue value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_ColorKeyGreen; /*!< Configures the color key green value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_ColorKeyRed;   /*!< Configures the color key red value. This parameter must range from 0x00 to 0xFF. */
} LTDC_ColorKeying_InitTypeDef;

// LTDC CLUT structure definition
typedef struct
{
    uint32_t LTDC_CLUTAdress; /*!< Configures the CLUT address. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_BlueValue;  /*!< Configures the blue value.This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_GreenValue; /*!< Configures the green value. This parameter must range from 0x00 to 0xFF. */
    uint32_t LTDC_RedValue;   /*!< Configures the red value. This parameter must range from 0x00 to 0xFF. */
} LTDC_CLUT_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
#define LTDC_HorizontalSYNC ((uint32_t)0x00000FFF)
#define LTDC_VerticalSYNC   ((uint32_t)0x000007FF)

#define IS_LTDC_HSYNC(HSYNC)   ((HSYNC) <= LTDC_HorizontalSYNC)
#define IS_LTDC_VSYNC(VSYNC)   ((VSYNC) <= LTDC_VerticalSYNC)
#define IS_LTDC_AHBP(AHBP)     ((AHBP) <= LTDC_HorizontalSYNC)
#define IS_LTDC_AVBP(AVBP)     ((AVBP) <= LTDC_VerticalSYNC)
#define IS_LTDC_AAW(AAW)       ((AAW) <= LTDC_HorizontalSYNC)
#define IS_LTDC_AAH(AAH)       ((AAH) <= LTDC_VerticalSYNC)
#define IS_LTDC_TOTALW(TOTALW) ((TOTALW) <= LTDC_HorizontalSYNC)
#define IS_LTDC_TOTALH(TOTALH) ((TOTALH) <= LTDC_VerticalSYNC)

// LTDC_HSPolarity
#define LTDC_HSPolarity_AL ((uint32_t)0x00000000) /*!< Horizontal Synchronization is active low. */
#define LTDC_HSPolarity_AH LTDC_GCR_HSPOL         /*!< Horizontal Synchronization is active high. */

#define IS_LTDC_HSPOL(HSPOL) (((HSPOL) == LTDC_HSPolarity_AL) || ((HSPOL) == LTDC_HSPolarity_AH))

// LTDC_VSPolarity
#define LTDC_VSPolarity_AL ((uint32_t)0x00000000) /*!< Vertical Synchronization is active low. */
#define LTDC_VSPolarity_AH LTDC_GCR_VSPOL         /*!< Vertical Synchronization is active high. */

#define IS_LTDC_VSPOL(VSPOL) (((VSPOL) == LTDC_VSPolarity_AL) || ((VSPOL) == LTDC_VSPolarity_AH))

// LTDC_DEPolarity
#define LTDC_DEPolarity_AL ((uint32_t)0x00000000) /*!< Data Enable, is active low. */
#define LTDC_DEPolarity_AH LTDC_GCR_DEPOL         /*!< Data Enable, is active high. */

#define IS_LTDC_DEPOL(DEPOL) (((DEPOL) == LTDC_VSPolarity_AL) || ((DEPOL) == LTDC_DEPolarity_AH))

// LTDC_PCPolarity
#define LTDC_PCPolarity_IPC  ((uint32_t)0x00000000) /*!< input pixel clock. */
#define LTDC_PCPolarity_IIPC LTDC_GCR_PCPOL         /*!< inverted input pixel clock. */

#define IS_LTDC_PCPOL(PCPOL) (((PCPOL) == LTDC_PCPolarity_IPC) || ((PCPOL) == LTDC_PCPolarity_IIPC))

// LTDC_Reload
#define LTDC_IMReload LTDC_SRCR_IMR /*!< Immediately Reload. */
#define LTDC_VBReload LTDC_SRCR_VBR /*!< Vertical Blanking Reload. */

#define IS_LTDC_RELOAD(RELOAD) (((RELOAD) == LTDC_IMReload) || ((RELOAD) == LTDC_VBReload))

// LTDC_Back_Color
#define LTDC_Back_Color ((uint32_t)0x000000FF)

#define IS_LTDC_BackBlueValue(BBLUE)   ((BBLUE) <= LTDC_Back_Color)
#define IS_LTDC_BackGreenValue(BGREEN) ((BGREEN) <= LTDC_Back_Color)
#define IS_LTDC_BackRedValue(BRED)     ((BRED) <= LTDC_Back_Color)

// LTDC_Position
#define LTDC_POS_CY LTDC_CPSR_CYPOS
#define LTDC_POS_CX LTDC_CPSR_CXPOS

#define IS_LTDC_GET_POS(POS) (((POS) <= LTDC_POS_CY))

// LTDC_LIPosition
#define IS_LTDC_LIPOS(LIPOS) ((LIPOS) <= 0x7FF)

// LTDC_CurrentStatus
#define LTDC_CD_VDES  LTDC_CDSR_VDES
#define LTDC_CD_HDES  LTDC_CDSR_HDES
#define LTDC_CD_VSYNC LTDC_CDSR_VSYNCS
#define LTDC_CD_HSYNC LTDC_CDSR_HSYNCS

#define IS_LTDC_GET_CD(CD) (((CD) == LTDC_CD_VDES) || ((CD) == LTDC_CD_HDES) || ((CD) == LTDC_CD_VSYNC) || ((CD) == LTDC_CD_HSYNC))

// LTDC_Interrupts
#define LTDC_IT_LI   LTDC_IER_LIE
#define LTDC_IT_FU   LTDC_IER_FUIE
#define LTDC_IT_TERR LTDC_IER_TERRIE
#define LTDC_IT_RR   LTDC_IER_RRIE

#define IS_LTDC_IT(IT) ((((IT) & (uint32_t)0xFFFFFFF0) == 0x00) && ((IT) != 0x00))

// LTDC_Flag
#define LTDC_FLAG_LI   LTDC_ISR_LIF
#define LTDC_FLAG_FU   LTDC_ISR_FUIF
#define LTDC_FLAG_TERR LTDC_ISR_TERRIF
#define LTDC_FLAG_RR   LTDC_ISR_RRIF

#define IS_LTDC_FLAG(FLAG) (((FLAG) == LTDC_FLAG_LI) || ((FLAG) == LTDC_FLAG_FU) || ((FLAG) == LTDC_FLAG_TERR) || ((FLAG) == LTDC_FLAG_RR))

// LTDC_Pixelformat
#define LTDC_Pixelformat_ARGB8888 ((uint32_t)0x00000000)
#define LTDC_Pixelformat_RGB888   ((uint32_t)0x00000001)
#define LTDC_Pixelformat_RGB565   ((uint32_t)0x00000002)
#define LTDC_Pixelformat_ARGB1555 ((uint32_t)0x00000003)
#define LTDC_Pixelformat_ARGB4444 ((uint32_t)0x00000004)
#define LTDC_Pixelformat_L8       ((uint32_t)0x00000005)
#define LTDC_Pixelformat_AL44     ((uint32_t)0x00000006)
#define LTDC_Pixelformat_AL88     ((uint32_t)0x00000007)

#define IS_LTDC_Pixelformat(Pixelformat)                                                                                                         \
    (((Pixelformat) == LTDC_Pixelformat_ARGB8888) || ((Pixelformat) == LTDC_Pixelformat_RGB888) || ((Pixelformat) == LTDC_Pixelformat_RGB565) || \
     ((Pixelformat) == LTDC_Pixelformat_ARGB1555) || ((Pixelformat) == LTDC_Pixelformat_ARGB4444) || ((Pixelformat) == LTDC_Pixelformat_L8) ||   \
     ((Pixelformat) == LTDC_Pixelformat_AL44) || ((Pixelformat) == LTDC_Pixelformat_AL88))

#define LTDC_BlendingFactor1_CA    ((uint32_t)0x00000400)
#define LTDC_BlendingFactor1_PAxCA ((uint32_t)0x00000600)

#define IS_LTDC_BlendingFactor1(BlendingFactor1) (((BlendingFactor1) == LTDC_BlendingFactor1_CA) || ((BlendingFactor1) == LTDC_BlendingFactor1_PAxCA))

// LTDC_BlendingFactor2
#define LTDC_BlendingFactor2_CA    ((uint32_t)0x00000005)
#define LTDC_BlendingFactor2_PAxCA ((uint32_t)0x00000007)

#define IS_LTDC_BlendingFactor2(BlendingFactor2) (((BlendingFactor2) == LTDC_BlendingFactor2_CA) || ((BlendingFactor2) == LTDC_BlendingFactor2_PAxCA))

// LTDC_LAYER_Config
#define LTDC_STOPPosition  ((uint32_t)0x0000FFFF)
#define LTDC_STARTPosition ((uint32_t)0x00000FFF)

#define LTDC_DefaultColorConfig ((uint32_t)0x000000FF)
#define LTDC_ColorFrameBuffer   ((uint32_t)0x00001FFF)
#define LTDC_LineNumber         ((uint32_t)0x000007FF)

#define IS_LTDC_HCONFIGST(HCONFIGST) ((HCONFIGST) <= LTDC_STARTPosition)
#define IS_LTDC_HCONFIGSP(HCONFIGSP) ((HCONFIGSP) <= LTDC_STOPPosition)
#define IS_LTDC_VCONFIGST(VCONFIGST) ((VCONFIGST) <= LTDC_STARTPosition)
#define IS_LTDC_VCONFIGSP(VCONFIGSP) ((VCONFIGSP) <= LTDC_STOPPosition)

#define IS_LTDC_DEFAULTCOLOR(DEFAULTCOLOR) ((DEFAULTCOLOR) <= LTDC_DefaultColorConfig)

#define IS_LTDC_CFBP(CFBP)   ((CFBP) <= LTDC_ColorFrameBuffer)
#define IS_LTDC_CFBLL(CFBLL) ((CFBLL) <= LTDC_ColorFrameBuffer)

#define IS_LTDC_CFBLNBR(CFBLNBR) ((CFBLNBR) <= LTDC_LineNumber)

// LTDC_colorkeying_Config
#define LTDC_colorkeyingConfig ((uint32_t)0x000000FF)

#define IS_LTDC_CKEYING(CKEYING) ((CKEYING) <= LTDC_colorkeyingConfig)

// LTDC_CLUT_Config
#define LTDC_CLUTWR ((uint32_t)0x000000FF)

#define IS_LTDC_CLUTWR(CLUTWR) ((CLUTWR) <= LTDC_CLUTWR)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*  Function used to set the LTDC configuration to the default reset state *****/
void LTDC_DeInit(void);

/* Initialization and Configuration functions *********************************/
void LTDC_Init(LTDC_InitTypeDef* LTDC_InitStruct);
void LTDC_StructInit(LTDC_InitTypeDef* LTDC_InitStruct);
void LTDC_Cmd(FunctionalState NewState);
void LTDC_DitherCmd(FunctionalState NewState);

LTDC_RGBTypeDef LTDC_GetRGBWidth(void);

void LTDC_RGBStructInit(LTDC_RGBTypeDef* LTDC_RGB_InitStruct);
void LTDC_LIPConfig(uint32_t LTDC_LIPositionConfig);
void LTDC_ReloadConfig(uint32_t LTDC_Reload);
void LTDC_LayerInit(LTDC_Layer_TypeDef* LTDC_Layerx, LTDC_Layer_InitTypeDef* LTDC_Layer_InitStruct);
void LTDC_LayerStructInit(LTDC_Layer_InitTypeDef* LTDC_Layer_InitStruct);
void LTDC_LayerCmd(LTDC_Layer_TypeDef* LTDC_Layerx, FunctionalState NewState);

LTDC_PosTypeDef LTDC_GetPosStatus(void);

void LTDC_PosStructInit(LTDC_PosTypeDef* LTDC_Pos_InitStruct);

FlagStatus LTDC_GetCDStatus(uint32_t LTDC_CD);

void LTDC_ColorKeyingConfig(LTDC_Layer_TypeDef* LTDC_Layerx, LTDC_ColorKeying_InitTypeDef* LTDC_colorkeying_InitStruct, FunctionalState NewState);
void LTDC_ColorKeyingStructInit(LTDC_ColorKeying_InitTypeDef* LTDC_colorkeying_InitStruct);
void LTDC_CLUTCmd(LTDC_Layer_TypeDef* LTDC_Layerx, FunctionalState NewState);
void LTDC_CLUTInit(LTDC_Layer_TypeDef* LTDC_Layerx, LTDC_CLUT_InitTypeDef* LTDC_CLUT_InitStruct);
void LTDC_CLUTStructInit(LTDC_CLUT_InitTypeDef* LTDC_CLUT_InitStruct);
void LTDC_LayerPosition(LTDC_Layer_TypeDef* LTDC_Layerx, uint16_t OffsetX, uint16_t OffsetY);
void LTDC_LayerAlpha(LTDC_Layer_TypeDef* LTDC_Layerx, uint8_t ConstantAlpha);
void LTDC_LayerAddress(LTDC_Layer_TypeDef* LTDC_Layerx, uint32_t Address);
void LTDC_LayerSize(LTDC_Layer_TypeDef* LTDC_Layerx, uint32_t Width, uint32_t Height);
void LTDC_LayerPixelFormat(LTDC_Layer_TypeDef* LTDC_Layerx, uint32_t PixelFormat);

/* Interrupts and flags management functions **********************************/
void       LTDC_ITConfig(uint32_t LTDC_IT, FunctionalState NewState);
FlagStatus LTDC_GetFlagStatus(uint32_t LTDC_FLAG);
void       LTDC_ClearFlag(uint32_t LTDC_FLAG);
ITStatus   LTDC_GetITStatus(uint32_t LTDC_IT);
void       LTDC_ClearITPendingBit(uint32_t LTDC_IT);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_LTDC_H__
