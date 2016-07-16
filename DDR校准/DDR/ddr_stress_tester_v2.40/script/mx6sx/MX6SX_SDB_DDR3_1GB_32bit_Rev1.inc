//*================================================================================================
//* Copyright (C) 2011, Freescale Semiconductor, Inc. All Rights Reserved
//* THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
//* BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
//* Freescale Semiconductor, Inc.
//*================================================================================================


// DDR init script, written in ARM RVDS syntax
// Target the CPUDDR3 board
// DDR part number: MT41K256M16HA-125: (1cs,1GB)
// Timing optimized to 400MHz. 32-bit data bus 
//
// IOMUX DSE and DELAY registers optmized for MX6SX SDB
//
//*================================================================================================



wait = on

//*================================================================================================
// Disable WDOG
//*================================================================================================
//setmem /16 0x020bc000 = 0x30  


//*================================================================================================
// Enable all clocks (they are disabled by ROM code)
//*================================================================================================
setmem /32 0x020c4068 = 0xffffffff
setmem /32 0x020c406c = 0xffffffff
setmem /32 0x020c4070 = 0xffffffff
setmem /32 0x020c4074 = 0xffffffff
setmem /32 0x020c4078 = 0xffffffff
setmem /32 0x020c407c = 0xffffffff
setmem /32 0x020c4080 = 0xffffffff
setmem /32 0x020c4084 = 0xffffffff


//*================================================================================================
// Initialize 64-bit DDR3 
//*================================================================================================

//######################################################
// IOMUX 
//######################################################

//DDR IO TYPE:
setmem /32 0x020e0618 = 0x000c0000 // IOMUXC_SW_PAD_CTL_GRP_DDR_TYPE - DDR_SEL=11 
setmem /32 0x020e05fc = 0x00000000 // IOMUXC_SW_PAD_CTL_GRP_DDRPKE - PKE=0 , Pull disabled for all, except DQS.

//CLOCK:
setmem /32 0x020e032c = 0x00000030 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK_0 - DSE=110, DDR_INPUT=0, HYS=0

//ADDRESS:
setmem /32 0x020e0300 = 0x00000020 // IOMUXC_SW_PAD_CTL_PAD_DRAM_CAS - DSE=100, DDR_INPUT=1, HYS=0
setmem /32 0x020e02fc = 0x00000020 // IOMUXC_SW_PAD_CTL_PAD_DRAM_RAS - DSE=100, DDR_INPUT=1, HYS=0
setmem /32 0x020e05f4 = 0x00000020 // IOMUXC_SW_PAD_CTL_GRP_ADDDS - DSE= 100

//CONTROL:
setmem /32 0x020e0340 = 0x00000020 // IOMUXC_SW_PAD_CTL_PAD_DRAM_RESET - DSE=100, DDR_INPUT=1, HYS=0, DDR_SEL=00

setmem /32 0x020e0320 = 0x00000000 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDBA2
setmem /32 0x020e0310 = 0x00000020 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDODT0
setmem /32 0x020e0314 = 0x00000020 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDODT1
setmem /32 0x020e0614 = 0x00000020 // IOMUXC_SW_PAD_CTL_GRP_CTLDS - DSE=100


//DATA STROBE:
setmem /32 0x020e05f8 = 0x00020000 // IOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL - DDR_INPUT=1

setmem /32 0x020e0330 = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS0 - DSE=011
setmem /32 0x020e0334 = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS1 - DSE=011
setmem /32 0x020e0338 = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS2 - DSE=011
setmem /32 0x020e033c = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS3 - DSE=011


//DATA:
setmem /32 0x020e0608 = 0x00020000 // IOMUXC_SW_PAD_CTL_GRP_DDRMODE- DDR_INPUT 1,diff

setmem /32 0x020e060c = 0x00000028 // IOMUXC_SW_PAD_CTL_GRP_B0DS - DSE=101
setmem /32 0x020e0610 = 0x00000028 // IOMUXC_SW_PAD_CTL_GRP_B1DS - DSE=101
setmem /32 0x020e061c = 0x00000028 // IOMUXC_SW_PAD_CTL_GRP_B2DS - DSE=101
setmem /32 0x020e0620 = 0x00000028 // IOMUXC_SW_PAD_CTL_GRP_B3DS - DSE=101


setmem /32 0x020e02ec = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0 - DSE=101, DDR_INPUT=1, HYS=0
setmem /32 0x020e02f0 = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1 - DSE=101, DDR_INPUT=1, HYS=0
setmem /32 0x020e02f4 = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM2 - DSE=101, DDR_INPUT=1, HYS=0
setmem /32 0x020e02f8 = 0x00000028 // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM3 - DSE=101, DDR_INPUT=1, HYS=0

//######################################################
//Calibrations:
//######################################################
// ZQ:
setmem /32 0x021b0800 = 0xa1390003      // DDR_PHY_P0_MPZQHWCTRL, enable both one-time & periodic HW ZQ calibration.


// write leveling
setmem /32 0x021b080c = 0x00290025
setmem /32 0x021b0810 = 0x00220022


//DQS gating, read delay, write delay calibration values based on calibration compare of 0x00ffff00:

setmem /32 0x021b083c = 0x41480144
setmem /32 0x021b0840 = 0x01340130

//setmem /32	0x021b08c0 =	0x24912492

setmem /32 0x021b0848 = 0x3C3E4244
setmem /32 0x021b0850 = 0x34363638


//read data bit delay: (3 is the reccommended default value, although out of reset value is 0):
setmem /32 0x021b081c = 0x33333333      // DDR_PHY_P0_MPREDQBY0DL3
setmem /32 0x021b0820 = 0x33333333      // DDR_PHY_P0_MPREDQBY1DL3
setmem /32 0x021b0824 = 0x33333333      // DDR_PHY_P0_MPREDQBY2DL3
setmem /32 0x021b0828 = 0x33333333      // DDR_PHY_P0_MPREDQBY3DL3


//######################################################
// Complete calibration by forced measurment:
//######################################################
setmem /32 0x021b08b8 = 0x00000800      // DDR_PHY_P0_MPMUR0, frc_msr


//######################################################
//MMDC init:

//in DDR3, 64-bit mode, only MMDC0 is initiated:
setmem /32 0x021b0004 = 0x0002002d		// MMDC0_MDPDC see spread sheet for timings
setmem /32 0x021b0008 = 0x00333030		// MMDC0_MDOTC see spread sheet for timings
setmem /32 0x021b000c = 0x676b52f3		// MMDC0_MDCFG0 see spread sheet for timings. CL=6
setmem /32 0x021b0010 = 0xb66d8b63		// MMDC0_MDCFG1 see spread sheet for timings
setmem /32 0x021b0014 = 0x01ff00db      // MMDC0_MDCFG2 - tRRD - 4ck; tWTR - 4ck; tRTP - 4ck; tDLLK - 512ck
setmem /32 0x021b0018 = 0x00011740      // MMDC0_MDMISC, RALAT=0x5


setmem /32 0x021b001c = 0x00008000      // MMDC0_MDSCR

setmem /32 0x021b002c = 0x000026d2      // MMDC0_MDRWD
setmem /32 0x021b0030 = 0x006b1023      // MMDC0_MDOR - tXPR - 69ck
setmem /32 0x021b0040 = 0x0000005f      // CS0_END - 0xbfffffff 

setmem /32 0x021b0000 = 0x84190000      // MMDC0_MDCTL - row - 14bits; col = 10bits; burst length 8; 64-bit data bus



//######################################################
// Initialize MT41K256M16HA-125: 
//MR2:
setmem /32 0x021b001c = 0x04008032      // MMDC0_MDSCR

//MR3:
setmem /32 0x021b001c = 0x00008033      // MMDC0_MDSCR

//MR1:
setmem /32 0x021b001c = 0x00048031      // MMDC0_MDSCR


//MR0:
setmem /32 0x021b001c = 0x05208030      // MMDC0_MDSCR


//DDR device ZQ calibration:
setmem /32 0x021b001c = 0x04008040      // MMDC0_MDSCR, 

//######################################################
//final DDR setup, before operation start:

setmem /32 0x021b0020 = 0x00000800      // MMDC0_MDREF, enable auto refresh, set refresh rate.

setmem /32 0x021b0818 = 0x00011117      // DDR_PHY_P0_MPODTCTRL, ODT enable

//setmem /32 0x021b0004 = 0x0002556d		// MMDC0_MDPDC see spread sheet for timings, SDCTL power down enabled

//setmem /32 0x021b0404 = 0x00011006      //MMDC0_MAPSR ADOPT power down enabled

setmem /32 0x021b001c = 0x00000000      // MMDC0_MDSCR
 
