// Copyright (c) 2011-2023 Megahunt Technologies Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef __PERIPH_FSMC_H__
#define __PERIPH_FSMC_H__

#include "mh2457.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------  Register Definition  ------------------------------------------- */

/**
 * @brief Flexible Static Memory Controller
 */
typedef struct
{
    __IO uint32_t BTCR[8]; /*!< NOR/PSRAM chip-select control register(BCR) and chip-select timing register(BTR), Address offset: 0x00-1C */
} FSMC_Bank1_TypeDef;

/**
 * @brief Flexible Static Memory Controller Bank1E
 */
typedef struct
{
    __IO uint32_t BWTR[7]; /*!< NOR/PSRAM write timing registers, Address offset: 0x104-0x11C */
} FSMC_Bank1E_TypeDef;

/* --------  Register Bit Definition  --------------------------------------- */

/******************  Bit definition for FSMC_BCR1 register  *******************/
#define FSMC_BCR1_MBKEN ((uint32_t)0x00000001) /*!<Memory bank enable bit                 */
#define FSMC_BCR1_MUXEN ((uint32_t)0x00000002) /*!<Address/data multiplexing enable bit   */

#define FSMC_BCR1_MTYP   ((uint32_t)0x0000000C) /*!<MTYP[1:0] bits (Memory type)           */
#define FSMC_BCR1_MTYP_0 ((uint32_t)0x00000004) /*!<Bit 0 */
#define FSMC_BCR1_MTYP_1 ((uint32_t)0x00000008) /*!<Bit 1 */

#define FSMC_BCR1_MWID   ((uint32_t)0x00000030) /*!<MWID[1:0] bits (Memory data bus width) */
#define FSMC_BCR1_MWID_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BCR1_MWID_1 ((uint32_t)0x00000020) /*!<Bit 1 */

#define FSMC_BCR1_FACCEN    ((uint32_t)0x00000040) /*!<Flash access enable                    */
#define FSMC_BCR1_BURSTEN   ((uint32_t)0x00000100) /*!<Burst enable bit                       */
#define FSMC_BCR1_WAITPOL   ((uint32_t)0x00000200) /*!<Wait signal polarity bit               */
#define FSMC_BCR1_WRAPMOD   ((uint32_t)0x00000400) /*!<Wrapped burst mode support             */
#define FSMC_BCR1_WAITCFG   ((uint32_t)0x00000800) /*!<Wait timing configuration              */
#define FSMC_BCR1_WREN      ((uint32_t)0x00001000) /*!<Write enable bit                       */
#define FSMC_BCR1_WAITEN    ((uint32_t)0x00002000) /*!<Wait enable bit                        */
#define FSMC_BCR1_EXTMOD    ((uint32_t)0x00004000) /*!<Extended mode enable                   */
#define FSMC_BCR1_ASYNCWAIT ((uint32_t)0x00008000) /*!<Asynchronous wait                      */
#define FSMC_BCR1_CBURSTRW  ((uint32_t)0x00080000) /*!<Write burst enable                     */

/******************  Bit definition for FSMC_BCR2 register  *******************/
#define FSMC_BCR2_MBKEN ((uint32_t)0x00000001) /*!<Memory bank enable bit                */
#define FSMC_BCR2_MUXEN ((uint32_t)0x00000002) /*!<Address/data multiplexing enable bit   */

#define FSMC_BCR2_MTYP   ((uint32_t)0x0000000C) /*!<MTYP[1:0] bits (Memory type)           */
#define FSMC_BCR2_MTYP_0 ((uint32_t)0x00000004) /*!<Bit 0 */
#define FSMC_BCR2_MTYP_1 ((uint32_t)0x00000008) /*!<Bit 1 */

#define FSMC_BCR2_MWID   ((uint32_t)0x00000030) /*!<MWID[1:0] bits (Memory data bus width) */
#define FSMC_BCR2_MWID_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BCR2_MWID_1 ((uint32_t)0x00000020) /*!<Bit 1 */

#define FSMC_BCR2_FACCEN    ((uint32_t)0x00000040) /*!<Flash access enable                    */
#define FSMC_BCR2_BURSTEN   ((uint32_t)0x00000100) /*!<Burst enable bit                       */
#define FSMC_BCR2_WAITPOL   ((uint32_t)0x00000200) /*!<Wait signal polarity bit               */
#define FSMC_BCR2_WRAPMOD   ((uint32_t)0x00000400) /*!<Wrapped burst mode support             */
#define FSMC_BCR2_WAITCFG   ((uint32_t)0x00000800) /*!<Wait timing configuration              */
#define FSMC_BCR2_WREN      ((uint32_t)0x00001000) /*!<Write enable bit                       */
#define FSMC_BCR2_WAITEN    ((uint32_t)0x00002000) /*!<Wait enable bit                        */
#define FSMC_BCR2_EXTMOD    ((uint32_t)0x00004000) /*!<Extended mode enable                   */
#define FSMC_BCR2_ASYNCWAIT ((uint32_t)0x00008000) /*!<Asynchronous wait                      */
#define FSMC_BCR2_CBURSTRW  ((uint32_t)0x00080000) /*!<Write burst enable                     */

/******************  Bit definition for FSMC_BCR3 register  *******************/
#define FSMC_BCR3_MBKEN ((uint32_t)0x00000001) /*!<Memory bank enable bit                 */
#define FSMC_BCR3_MUXEN ((uint32_t)0x00000002) /*!<Address/data multiplexing enable bit   */

#define FSMC_BCR3_MTYP   ((uint32_t)0x0000000C) /*!<MTYP[1:0] bits (Memory type)           */
#define FSMC_BCR3_MTYP_0 ((uint32_t)0x00000004) /*!<Bit 0 */
#define FSMC_BCR3_MTYP_1 ((uint32_t)0x00000008) /*!<Bit 1 */

#define FSMC_BCR3_MWID   ((uint32_t)0x00000030) /*!<MWID[1:0] bits (Memory data bus width) */
#define FSMC_BCR3_MWID_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BCR3_MWID_1 ((uint32_t)0x00000020) /*!<Bit 1 */

#define FSMC_BCR3_FACCEN    ((uint32_t)0x00000040) /*!<Flash access enable                    */
#define FSMC_BCR3_BURSTEN   ((uint32_t)0x00000100) /*!<Burst enable bit                       */
#define FSMC_BCR3_WAITPOL   ((uint32_t)0x00000200) /*!<Wait signal polarity bit               */
#define FSMC_BCR3_WRAPMOD   ((uint32_t)0x00000400) /*!<Wrapped burst mode support             */
#define FSMC_BCR3_WAITCFG   ((uint32_t)0x00000800) /*!<Wait timing configuration              */
#define FSMC_BCR3_WREN      ((uint32_t)0x00001000) /*!<Write enable bit                       */
#define FSMC_BCR3_WAITEN    ((uint32_t)0x00002000) /*!<Wait enable bit                        */
#define FSMC_BCR3_EXTMOD    ((uint32_t)0x00004000) /*!<Extended mode enable                   */
#define FSMC_BCR3_ASYNCWAIT ((uint32_t)0x00008000) /*!<Asynchronous wait                      */
#define FSMC_BCR3_CBURSTRW  ((uint32_t)0x00080000) /*!<Write burst enable                     */

/******************  Bit definition for FSMC_BCR4 register  *******************/
#define FSMC_BCR4_MBKEN ((uint32_t)0x00000001) /*!<Memory bank enable bit */
#define FSMC_BCR4_MUXEN ((uint32_t)0x00000002) /*!<Address/data multiplexing enable bit   */

#define FSMC_BCR4_MTYP   ((uint32_t)0x0000000C) /*!<MTYP[1:0] bits (Memory type)           */
#define FSMC_BCR4_MTYP_0 ((uint32_t)0x00000004) /*!<Bit 0 */
#define FSMC_BCR4_MTYP_1 ((uint32_t)0x00000008) /*!<Bit 1 */

#define FSMC_BCR4_MWID   ((uint32_t)0x00000030) /*!<MWID[1:0] bits (Memory data bus width) */
#define FSMC_BCR4_MWID_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BCR4_MWID_1 ((uint32_t)0x00000020) /*!<Bit 1 */

#define FSMC_BCR4_FACCEN    ((uint32_t)0x00000040) /*!<Flash access enable                    */
#define FSMC_BCR4_BURSTEN   ((uint32_t)0x00000100) /*!<Burst enable bit                       */
#define FSMC_BCR4_WAITPOL   ((uint32_t)0x00000200) /*!<Wait signal polarity bit               */
#define FSMC_BCR4_WRAPMOD   ((uint32_t)0x00000400) /*!<Wrapped burst mode support             */
#define FSMC_BCR4_WAITCFG   ((uint32_t)0x00000800) /*!<Wait timing configuration              */
#define FSMC_BCR4_WREN      ((uint32_t)0x00001000) /*!<Write enable bit                       */
#define FSMC_BCR4_WAITEN    ((uint32_t)0x00002000) /*!<Wait enable bit                        */
#define FSMC_BCR4_EXTMOD    ((uint32_t)0x00004000) /*!<Extended mode enable                   */
#define FSMC_BCR4_ASYNCWAIT ((uint32_t)0x00008000) /*!<Asynchronous wait                      */
#define FSMC_BCR4_CBURSTRW  ((uint32_t)0x00080000) /*!<Write burst enable                     */

/******************  Bit definition for FSMC_BTR1 register  ******************/
#define FSMC_BTR1_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BTR1_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BTR1_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BTR1_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BTR1_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BTR1_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BTR1_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BTR1_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BTR1_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BTR1_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BTR1_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BTR1_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BTR1_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BTR1_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BTR1_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BTR1_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define FSMC_BTR1_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BTR1_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BTR1_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BTR1_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BTR1_CLKDIV   ((uint32_t)0x00F00000) /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define FSMC_BTR1_CLKDIV_0 ((uint32_t)0x00100000) /*!<Bit 0 */
#define FSMC_BTR1_CLKDIV_1 ((uint32_t)0x00200000) /*!<Bit 1 */
#define FSMC_BTR1_CLKDIV_2 ((uint32_t)0x00400000) /*!<Bit 2 */
#define FSMC_BTR1_CLKDIV_3 ((uint32_t)0x00800000) /*!<Bit 3 */

#define FSMC_BTR1_DATLAT   ((uint32_t)0x0F000000) /*!<DATLA[3:0] bits (Data latency) */
#define FSMC_BTR1_DATLAT_0 ((uint32_t)0x01000000) /*!<Bit 0 */
#define FSMC_BTR1_DATLAT_1 ((uint32_t)0x02000000) /*!<Bit 1 */
#define FSMC_BTR1_DATLAT_2 ((uint32_t)0x04000000) /*!<Bit 2 */
#define FSMC_BTR1_DATLAT_3 ((uint32_t)0x08000000) /*!<Bit 3 */

#define FSMC_BTR1_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BTR1_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BTR1_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/******************  Bit definition for FSMC_BTR2 register  *******************/
#define FSMC_BTR2_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BTR2_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BTR2_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BTR2_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BTR2_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BTR2_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BTR2_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BTR2_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BTR2_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BTR2_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BTR2_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BTR2_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BTR2_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BTR2_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BTR2_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BTR2_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define FSMC_BTR2_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BTR2_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BTR2_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BTR2_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BTR2_CLKDIV   ((uint32_t)0x00F00000) /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define FSMC_BTR2_CLKDIV_0 ((uint32_t)0x00100000) /*!<Bit 0 */
#define FSMC_BTR2_CLKDIV_1 ((uint32_t)0x00200000) /*!<Bit 1 */
#define FSMC_BTR2_CLKDIV_2 ((uint32_t)0x00400000) /*!<Bit 2 */
#define FSMC_BTR2_CLKDIV_3 ((uint32_t)0x00800000) /*!<Bit 3 */

#define FSMC_BTR2_DATLAT   ((uint32_t)0x0F000000) /*!<DATLA[3:0] bits (Data latency) */
#define FSMC_BTR2_DATLAT_0 ((uint32_t)0x01000000) /*!<Bit 0 */
#define FSMC_BTR2_DATLAT_1 ((uint32_t)0x02000000) /*!<Bit 1 */
#define FSMC_BTR2_DATLAT_2 ((uint32_t)0x04000000) /*!<Bit 2 */
#define FSMC_BTR2_DATLAT_3 ((uint32_t)0x08000000) /*!<Bit 3 */

#define FSMC_BTR2_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BTR2_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BTR2_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/*******************  Bit definition for FSMC_BTR3 register  *******************/
#define FSMC_BTR3_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BTR3_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BTR3_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BTR3_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BTR3_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BTR3_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BTR3_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BTR3_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BTR3_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BTR3_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BTR3_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BTR3_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BTR3_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BTR3_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BTR3_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BTR3_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define FSMC_BTR3_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BTR3_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BTR3_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BTR3_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BTR3_CLKDIV   ((uint32_t)0x00F00000) /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define FSMC_BTR3_CLKDIV_0 ((uint32_t)0x00100000) /*!<Bit 0 */
#define FSMC_BTR3_CLKDIV_1 ((uint32_t)0x00200000) /*!<Bit 1 */
#define FSMC_BTR3_CLKDIV_2 ((uint32_t)0x00400000) /*!<Bit 2 */
#define FSMC_BTR3_CLKDIV_3 ((uint32_t)0x00800000) /*!<Bit 3 */

#define FSMC_BTR3_DATLAT   ((uint32_t)0x0F000000) /*!<DATLA[3:0] bits (Data latency) */
#define FSMC_BTR3_DATLAT_0 ((uint32_t)0x01000000) /*!<Bit 0 */
#define FSMC_BTR3_DATLAT_1 ((uint32_t)0x02000000) /*!<Bit 1 */
#define FSMC_BTR3_DATLAT_2 ((uint32_t)0x04000000) /*!<Bit 2 */
#define FSMC_BTR3_DATLAT_3 ((uint32_t)0x08000000) /*!<Bit 3 */

#define FSMC_BTR3_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BTR3_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BTR3_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/******************  Bit definition for FSMC_BTR4 register  *******************/
#define FSMC_BTR4_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BTR4_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BTR4_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BTR4_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BTR4_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BTR4_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BTR4_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BTR4_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BTR4_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BTR4_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BTR4_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BTR4_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BTR4_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BTR4_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BTR4_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BTR4_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define FSMC_BTR4_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BTR4_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BTR4_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BTR4_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BTR4_CLKDIV   ((uint32_t)0x00F00000) /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define FSMC_BTR4_CLKDIV_0 ((uint32_t)0x00100000) /*!<Bit 0 */
#define FSMC_BTR4_CLKDIV_1 ((uint32_t)0x00200000) /*!<Bit 1 */
#define FSMC_BTR4_CLKDIV_2 ((uint32_t)0x00400000) /*!<Bit 2 */
#define FSMC_BTR4_CLKDIV_3 ((uint32_t)0x00800000) /*!<Bit 3 */

#define FSMC_BTR4_DATLAT   ((uint32_t)0x0F000000) /*!<DATLA[3:0] bits (Data latency) */
#define FSMC_BTR4_DATLAT_0 ((uint32_t)0x01000000) /*!<Bit 0 */
#define FSMC_BTR4_DATLAT_1 ((uint32_t)0x02000000) /*!<Bit 1 */
#define FSMC_BTR4_DATLAT_2 ((uint32_t)0x04000000) /*!<Bit 2 */
#define FSMC_BTR4_DATLAT_3 ((uint32_t)0x08000000) /*!<Bit 3 */

#define FSMC_BTR4_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BTR4_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BTR4_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/******************  Bit definition for FSMC_BWTR1 register  ******************/
#define FSMC_BWTR1_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BWTR1_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BWTR1_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BWTR1_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BWTR1_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BWTR1_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BWTR1_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BWTR1_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BWTR1_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BWTR1_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BWTR1_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BWTR1_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BWTR1_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BWTR1_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BWTR1_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BWTR1_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround duration) */
#define FSMC_BWTR1_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BWTR1_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BWTR1_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BWTR1_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BWTR1_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BWTR1_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BWTR1_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/******************  Bit definition for FSMC_BWTR2 register  ******************/
#define FSMC_BWTR2_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BWTR2_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BWTR2_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BWTR2_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BWTR2_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BWTR2_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BWTR2_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BWTR2_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BWTR2_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BWTR2_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BWTR2_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BWTR2_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BWTR2_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BWTR2_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BWTR2_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BWTR2_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround duration) */
#define FSMC_BWTR2_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BWTR2_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BWTR2_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BWTR2_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BWTR2_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BWTR2_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BWTR2_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/******************  Bit definition for FSMC_BWTR3 register  ******************/
#define FSMC_BWTR3_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BWTR3_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BWTR3_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BWTR3_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BWTR3_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BWTR3_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BWTR3_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BWTR3_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BWTR3_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BWTR3_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BWTR3_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BWTR3_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BWTR3_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BWTR3_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BWTR3_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BWTR3_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround duration) */
#define FSMC_BWTR3_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BWTR3_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BWTR3_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BWTR3_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BWTR3_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BWTR3_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BWTR3_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/******************  Bit definition for FSMC_BWTR4 register  ******************/
#define FSMC_BWTR4_ADDSET   ((uint32_t)0x0000000F) /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define FSMC_BWTR4_ADDSET_0 ((uint32_t)0x00000001) /*!<Bit 0 */
#define FSMC_BWTR4_ADDSET_1 ((uint32_t)0x00000002) /*!<Bit 1 */
#define FSMC_BWTR4_ADDSET_2 ((uint32_t)0x00000004) /*!<Bit 2 */
#define FSMC_BWTR4_ADDSET_3 ((uint32_t)0x00000008) /*!<Bit 3 */

#define FSMC_BWTR4_ADDHLD   ((uint32_t)0x000000F0) /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define FSMC_BWTR4_ADDHLD_0 ((uint32_t)0x00000010) /*!<Bit 0 */
#define FSMC_BWTR4_ADDHLD_1 ((uint32_t)0x00000020) /*!<Bit 1 */
#define FSMC_BWTR4_ADDHLD_2 ((uint32_t)0x00000040) /*!<Bit 2 */
#define FSMC_BWTR4_ADDHLD_3 ((uint32_t)0x00000080) /*!<Bit 3 */

#define FSMC_BWTR4_DATAST   ((uint32_t)0x0000FF00) /*!<DATAST [3:0] bits (Data-phase duration) */
#define FSMC_BWTR4_DATAST_0 ((uint32_t)0x00000100) /*!<Bit 0 */
#define FSMC_BWTR4_DATAST_1 ((uint32_t)0x00000200) /*!<Bit 1 */
#define FSMC_BWTR4_DATAST_2 ((uint32_t)0x00000400) /*!<Bit 2 */
#define FSMC_BWTR4_DATAST_3 ((uint32_t)0x00000800) /*!<Bit 3 */

#define FSMC_BWTR4_BUSTURN   ((uint32_t)0x000F0000) /*!<BUSTURN[3:0] bits (Bus turnaround duration) */
#define FSMC_BWTR4_BUSTURN_0 ((uint32_t)0x00010000) /*!<Bit 0 */
#define FSMC_BWTR4_BUSTURN_1 ((uint32_t)0x00020000) /*!<Bit 1 */
#define FSMC_BWTR4_BUSTURN_2 ((uint32_t)0x00040000) /*!<Bit 2 */
#define FSMC_BWTR4_BUSTURN_3 ((uint32_t)0x00080000) /*!<Bit 3 */

#define FSMC_BWTR4_ACCMOD   ((uint32_t)0x30000000) /*!<ACCMOD[1:0] bits (Access mode) */
#define FSMC_BWTR4_ACCMOD_0 ((uint32_t)0x10000000) /*!<Bit 0 */
#define FSMC_BWTR4_ACCMOD_1 ((uint32_t)0x20000000) /*!<Bit 1 */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  Timing parameters For NOR/SRAM Banks
 */
typedef struct
{
    uint32_t FSMC_AddressSetupTime; /*!< Defines the number of HCLK cycles to configure
                                         the duration of the address setup time.
                                         This parameter can be a value between 0 and 0xF.
                                         @note This parameter is not used with synchronous NOR Flash memories. */

    uint32_t FSMC_AddressHoldTime; /*!< Defines the number of HCLK cycles to configure
                                        the duration of the address hold time.
                                        This parameter can be a value between 0 and 0xF.
                                        @note This parameter is not used with synchronous NOR Flash memories.*/

    uint32_t FSMC_DataSetupTime; /*!< Defines the number of HCLK cycles to configure
                                      the duration of the data setup time.
                                      This parameter can be a value between 0 and 0xFF.
                                      @note This parameter is used for SRAMs, ROMs and asynchronous multiplexed NOR Flash memories. */

    uint32_t FSMC_BusTurnAroundDuration; /*!< Defines the number of HCLK cycles to configure
                                              the duration of the bus turnaround.
                                              This parameter can be a value between 0 and 0xF.
                                              @note This parameter is only used for multiplexed NOR Flash memories. */

    uint32_t FSMC_CLKDivision; /*!< Defines the period of CLK clock output signal, expressed in number of HCLK cycles.
                                    This parameter can be a value between 1 and 0xF.
                                    @note This parameter is not used for asynchronous NOR Flash, SRAM or ROM accesses. */

    uint32_t FSMC_DataLatency; /*!< Defines the number of memory clock cycles to issue
                                    to the memory before getting the first data.
                                    The parameter value depends on the memory type as shown below:
                                     - It must be set to 0 in case of a CRAM
                                     - It is don't care in asynchronous NOR, SRAM or ROM accesses
                                     - It may assume a value between 0 and 0xF in NOR Flash memories
                                       with synchronous burst mode enable */

    uint32_t FSMC_AccessMode; /*!< Specifies the asynchronous access mode.
                                   This parameter can be a value of @ref FSMC_Access_Mode */
} FSMC_NORSRAMTimingInitTypeDef;

/**
 * @brief  FSMC NOR/SRAM Init structure definition
 */
typedef struct
{
    uint32_t FSMC_Bank; /*!< Specifies the NOR/SRAM memory bank that will be used.
                             This parameter can be a value of @ref FSMC_NORSRAM_Bank */

    uint32_t FSMC_DataAddressMux; /*!< Specifies whether the address and data values are
                                       multiplexed on the data bus or not.
                                       This parameter can be a value of @ref FSMC_Data_Address_Bus_Multiplexing */

    uint32_t FSMC_MemoryType; /*!< Specifies the type of external memory attached to
                                   the corresponding memory bank.
                                   This parameter can be a value of @ref FSMC_Memory_Type */

    uint32_t FSMC_MemoryDataWidth; /*!< Specifies the external memory device width.
                                        This parameter can be a value of @ref FSMC_Data_Width */

    uint32_t FSMC_BurstAccessMode; /*!< Enables or disables the burst access mode for Flash memory,
                                        valid only with synchronous burst Flash memories.
                                        This parameter can be a value of @ref FSMC_Burst_Access_Mode */

    uint32_t FSMC_AsynchronousWait; /*!< Enables or disables wait signal during asynchronous transfers,
                                        valid only with asynchronous Flash memories.
                                        This parameter can be a value of @ref FSMC_AsynchronousWait */

    uint32_t FSMC_WaitSignalPolarity; /*!< Specifies the wait signal polarity, valid only when accessing
                                           the Flash memory in burst mode.
                                           This parameter can be a value of @ref FSMC_Wait_Signal_Polarity */

    uint32_t FSMC_WrapMode; /*!< Enables or disables the Wrapped burst access mode for Flash
                                 memory, valid only when accessing Flash memories in burst mode.
                                 This parameter can be a value of @ref FSMC_Wrap_Mode */

    uint32_t FSMC_WaitSignalActive; /*!< Specifies if the wait signal is asserted by the memory one
                                         clock cycle before the wait state or during the wait state,
                                         valid only when accessing memories in burst mode.
                                         This parameter can be a value of @ref FSMC_Wait_Timing */

    uint32_t FSMC_WriteOperation; /*!< Enables or disables the write operation in the selected bank by the FSMC.
                                       This parameter can be a value of @ref FSMC_Write_Operation */

    uint32_t FSMC_WaitSignal; /*!< Enables or disables the wait state insertion via wait
                                   signal, valid for Flash memory access in burst mode.
                                   This parameter can be a value of @ref FSMC_Wait_Signal */

    uint32_t FSMC_ExtendedMode; /*!< Enables or disables the extended mode.
                                     This parameter can be a value of @ref FSMC_Extended_Mode */

    uint32_t FSMC_WriteBurst; /*!< Enables or disables the write burst operation.
                                   This parameter can be a value of @ref FSMC_Write_Burst */

    FSMC_NORSRAMTimingInitTypeDef* FSMC_ReadWriteTimingStruct; /*!< Timing Parameters for write and read access if the  Extended Mode is not used*/

    FSMC_NORSRAMTimingInitTypeDef* FSMC_WriteTimingStruct; /*!< Timing Parameters for write access if the  Extended Mode is used*/
} FSMC_NORSRAMInitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup FSMC_Exported_Constants
 * @{
 */

/** @defgroup FSMC_NORSRAM_Bank
 * @{
 */
#define FSMC_Bank1_NORSRAM1 ((uint32_t)0x00000000)
#define FSMC_Bank1_NORSRAM2 ((uint32_t)0x00000002)
#define FSMC_Bank1_NORSRAM3 ((uint32_t)0x00000004)
#define FSMC_Bank1_NORSRAM4 ((uint32_t)0x00000006)
/**
 * @}
 */

#define IS_FSMC_NORSRAM_BANK(BANK) \
    (((BANK) == FSMC_Bank1_NORSRAM1) || ((BANK) == FSMC_Bank1_NORSRAM2) || ((BANK) == FSMC_Bank1_NORSRAM3) || ((BANK) == FSMC_Bank1_NORSRAM4))

/** @defgroup FSMC_NOR_SRAM_Controller
 * @{
 */

/** @defgroup FSMC_Data_Address_Bus_Multiplexing
 * @{
 */

#define FSMC_DataAddressMux_Disable ((uint32_t)0x00000000)
#define FSMC_DataAddressMux_Enable  ((uint32_t)0x00000002)
#define IS_FSMC_MUX(MUX)            (((MUX) == FSMC_DataAddressMux_Disable) || ((MUX) == FSMC_DataAddressMux_Enable))
/**
 * @}
 */

/** @defgroup FSMC_Memory_Type
 * @{
 */

#define FSMC_MemoryType_SRAM   ((uint32_t)0x00000000)
#define FSMC_MemoryType_PSRAM  ((uint32_t)0x00000004)
#define FSMC_MemoryType_NOR    ((uint32_t)0x00000008)
#define IS_FSMC_MEMORY(MEMORY) (((MEMORY) == FSMC_MemoryType_SRAM) || ((MEMORY) == FSMC_MemoryType_PSRAM) || ((MEMORY) == FSMC_MemoryType_NOR))
/**
 * @}
 */

/** @defgroup FSMC_Data_Width
 * @{
 */

#define FSMC_MemoryDataWidth_8b     ((uint32_t)0x00000000)
#define FSMC_MemoryDataWidth_16b    ((uint32_t)0x00000010)
#define IS_FSMC_MEMORY_WIDTH(WIDTH) (((WIDTH) == FSMC_MemoryDataWidth_8b) || ((WIDTH) == FSMC_MemoryDataWidth_16b))
/**
 * @}
 */

/** @defgroup FSMC_Burst_Access_Mode
 * @{
 */

#define FSMC_BurstAccessMode_Disable ((uint32_t)0x00000000)
#define FSMC_BurstAccessMode_Enable  ((uint32_t)0x00000100)
#define IS_FSMC_BURSTMODE(STATE)     (((STATE) == FSMC_BurstAccessMode_Disable) || ((STATE) == FSMC_BurstAccessMode_Enable))
/**
 * @}
 */

/** @defgroup FSMC_AsynchronousWait
 * @{
 */
#define FSMC_AsynchronousWait_Disable ((uint32_t)0x00000000)
#define FSMC_AsynchronousWait_Enable  ((uint32_t)0x00008000)
#define IS_FSMC_ASYNWAIT(STATE)       (((STATE) == FSMC_AsynchronousWait_Disable) || ((STATE) == FSMC_AsynchronousWait_Enable))
/**
 * @}
 */

/** @defgroup FSMC_Wait_Signal_Polarity
 * @{
 */
#define FSMC_WaitSignalPolarity_Low     ((uint32_t)0x00000000)
#define FSMC_WaitSignalPolarity_High    ((uint32_t)0x00000200)
#define IS_FSMC_WAIT_POLARITY(POLARITY) (((POLARITY) == FSMC_WaitSignalPolarity_Low) || ((POLARITY) == FSMC_WaitSignalPolarity_High))
/**
 * @}
 */

/** @defgroup FSMC_Wrap_Mode
 * @{
 */
#define FSMC_WrapMode_Disable   ((uint32_t)0x00000000)
#define FSMC_WrapMode_Enable    ((uint32_t)0x00000400)
#define IS_FSMC_WRAP_MODE(MODE) (((MODE) == FSMC_WrapMode_Disable) || ((MODE) == FSMC_WrapMode_Enable))
/**
 * @}
 */

/** @defgroup FSMC_Wait_Timing
 * @{
 */
#define FSMC_WaitSignalActive_BeforeWaitState ((uint32_t)0x00000000)
#define FSMC_WaitSignalActive_DuringWaitState ((uint32_t)0x00000800)
#define IS_FSMC_WAIT_SIGNAL_ACTIVE(ACTIVE)    (((ACTIVE) == FSMC_WaitSignalActive_BeforeWaitState) || ((ACTIVE) == FSMC_WaitSignalActive_DuringWaitState))
/**
 * @}
 */

/** @defgroup FSMC_Write_Operation
 * @{
 */
#define FSMC_WriteOperation_Disable        ((uint32_t)0x00000000)
#define FSMC_WriteOperation_Enable         ((uint32_t)0x00001000)
#define IS_FSMC_WRITE_OPERATION(OPERATION) (((OPERATION) == FSMC_WriteOperation_Disable) || ((OPERATION) == FSMC_WriteOperation_Enable))
/**
 * @}
 */

/** @defgroup FSMC_Wait_Signal
 * @{
 */
#define FSMC_WaitSignal_Disable      ((uint32_t)0x00000000)
#define FSMC_WaitSignal_Enable       ((uint32_t)0x00002000)
#define IS_FSMC_WAITE_SIGNAL(SIGNAL) (((SIGNAL) == FSMC_WaitSignal_Disable) || ((SIGNAL) == FSMC_WaitSignal_Enable))
/**
 * @}
 */

/** @defgroup FSMC_Extended_Mode
 * @{
 */
#define FSMC_ExtendedMode_Disable ((uint32_t)0x00000000)
#define FSMC_ExtendedMode_Enable  ((uint32_t)0x00004000)

#define IS_FSMC_EXTENDED_MODE(MODE) (((MODE) == FSMC_ExtendedMode_Disable) || ((MODE) == FSMC_ExtendedMode_Enable))
/**
 * @}
 */

/** @defgroup FSMC_Write_Burst
 * @{
 */

#define FSMC_WriteBurst_Disable    ((uint32_t)0x00000000)
#define FSMC_WriteBurst_Enable     ((uint32_t)0x00080000)
#define IS_FSMC_WRITE_BURST(BURST) (((BURST) == FSMC_WriteBurst_Disable) || ((BURST) == FSMC_WriteBurst_Enable))
/**
 * @}
 */

/** @defgroup FSMC_Address_Setup_Time
 * @{
 */
#define IS_FSMC_ADDRESS_SETUP_TIME(TIME) ((TIME) <= 0xF)
/**
 * @}
 */

/** @defgroup FSMC_Address_Hold_Time
 * @{
 */
#define IS_FSMC_ADDRESS_HOLD_TIME(TIME) ((TIME) <= 0xF)
/**
 * @}
 */

/** @defgroup FSMC_Data_Setup_Time
 * @{
 */
#define IS_FSMC_DATASETUP_TIME(TIME) (((TIME) > 0) && ((TIME) <= 0xFF))
/**
 * @}
 */

/** @defgroup FSMC_Bus_Turn_around_Duration
 * @{
 */
#define IS_FSMC_TURNAROUND_TIME(TIME) ((TIME) <= 0xF)
/**
 * @}
 */

/** @defgroup FSMC_CLK_Division
 * @{
 */
#define IS_FSMC_CLK_DIV(DIV) ((DIV) <= 0xF)
/**
 * @}
 */

/** @defgroup FSMC_Data_Latency
 * @{
 */
#define IS_FSMC_DATA_LATENCY(LATENCY) ((LATENCY) <= 0xF)
/**
 * @}
 */

/** @defgroup FSMC_Access_Mode
 * @{
 */
#define FSMC_AccessMode_A ((uint32_t)0x00000000)
#define FSMC_AccessMode_B ((uint32_t)0x10000000)
#define FSMC_AccessMode_C ((uint32_t)0x20000000)
#define FSMC_AccessMode_D ((uint32_t)0x30000000)
#define IS_FSMC_ACCESS_MODE(MODE) \
    (((MODE) == FSMC_AccessMode_A) || ((MODE) == FSMC_AccessMode_B) || ((MODE) == FSMC_AccessMode_C) || ((MODE) == FSMC_AccessMode_D))
/**
 * @}
 */

/**
 * @}
 */

/** @defgroup FSMC_NAND_PCCARD_Controller
 * @{
 */

/** @defgroup FSMC_Wait_feature
 * @{
 */
#define FSMC_Waitfeature_Disable      ((uint32_t)0x00000000)
#define FSMC_Waitfeature_Enable       ((uint32_t)0x00000002)
#define IS_FSMC_WAIT_FEATURE(FEATURE) (((FEATURE) == FSMC_Waitfeature_Disable) || ((FEATURE) == FSMC_Waitfeature_Enable))
/**
 * @}
 */

/** @defgroup FSMC_TCLR_Setup_Time
 * @{
 */
#define IS_FSMC_TCLR_TIME(TIME) ((TIME) <= 0xFF)
/**
 * @}
 */

/** @defgroup FSMC_TAR_Setup_Time
 * @{
 */
#define IS_FSMC_TAR_TIME(TIME) ((TIME) <= 0xFF)
/**
 * @}
 */

/** @defgroup FSMC_Setup_Time
 * @{
 */
#define IS_FSMC_SETUP_TIME(TIME) ((TIME) <= 0xFF)
/**
 * @}
 */

/** @defgroup FSMC_Wait_Setup_Time
 * @{
 */
#define IS_FSMC_WAIT_TIME(TIME) ((TIME) <= 0xFF)
/**
 * @}
 */

/** @defgroup FSMC_Hold_Setup_Time
 * @{
 */
#define IS_FSMC_HOLD_TIME(TIME) ((TIME) <= 0xFF)
/**
 * @}
 */

/** @defgroup FSMC_HiZ_Setup_Time
 * @{
 */
#define IS_FSMC_HIZ_TIME(TIME) ((TIME) <= 0xFF)
/**
 * @}
 */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* NOR/SRAM Controller functions **********************************************/
void FSMC_NORSRAMDeInit(uint32_t FSMC_Bank);
void FSMC_NORSRAMInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct);
void FSMC_NORSRAMStructInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct);
void FSMC_NORSRAMCmd(uint32_t FSMC_Bank, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif // __PERIPH_FSMC_H__
