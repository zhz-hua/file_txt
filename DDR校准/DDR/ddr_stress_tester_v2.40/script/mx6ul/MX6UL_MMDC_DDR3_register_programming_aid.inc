//=============================================================================							
// init script for i.MX6UL 	DDR3						
//=============================================================================							
// Revision History							
//  0.3							
//=============================================================================							
							
wait = on							
//=============================================================================							
// Disable	WDOG						
//=============================================================================							
//setmem /16	0x020BC000 =	0x30					
							
//=============================================================================							
// Enable all clocks (they are disabled by ROM code)							
//=============================================================================							
setmem /32	0x020C4068 =	0xFFFFFFFF	//[CCM_CCGR0]CCM Clock Gating Register 0				
setmem /32	0x020C406C =	0xFFFFFFFF	//[CCM_CCGR1]CCM Clock Gating Register 1				
setmem /32	0x020C4070 =	0xFFFFFFFF	//[CCM_CCGR2]CCM Clock Gating Register 2				
setmem /32	0x020C4074 =	0xFFFFFFFF	//[CCM_CCGR3]CCM Clock Gating Register 3				
setmem /32	0x020C4078 =	0xFFFFFFFF	//[CCM_CCGR4]CCM Clock Gating Register 4				
setmem /32	0x020C407C =	0xFFFFFFFF	//[CCM_CCGR5]CCM Clock Gating Register 5				
setmem /32	0x020C4080 =	0xFFFFFFFF	//[CCM_CCGR6]CCM Clock Gating Register 6				
							
//=============================================================================							
// IOMUX							
//=============================================================================							
//DDR IO TYPE:							
setmem /32	0x020E04B4 =	0x000C0000	// IOMUXC_SW_PAD_CTL_GRP_DDR_TYPE				
setmem /32	0x020E04AC =	0x00000000	// IOMUXC_SW_PAD_CTL_GRP_DDRPKE 				
							
//CLOCK:							
setmem /32	0x020E027C =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK0_P				
							
//Control:							
setmem /32	0x020E0250 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_CAS				
setmem /32	0x020E024C =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_RAS				
setmem /32	0x020E0490 =	0x00000030	// IOMUXC_SW_PAD_CTL_GRP_ADDDS 				
setmem /32	0x020E0288 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_RESET				
setmem /32	0x020E0270 =	0x00000000	// IOMUXC_SW_PAD_CTL_PAD_DRAM_SDBA2 - DSE can be configured using Group Control Register: IOMUXC_SW_PAD_CTL_GRP_CTLDS				
setmem /32	0x020E0260 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_ODT0				
setmem /32	0x020E0264 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_ODT1				
setmem /32	0x020E04A0 =	0x00000030	// IOMUXC_SW_PAD_CTL_GRP_CTLDS 				
							
//Data Strobes:							
setmem /32	0x020E0494 =	0x00020000	// IOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL 				
setmem /32	0x020E0280 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS0_P				
setmem /32	0x020E0284 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS1_P				
							
//Data:							
setmem /32	0x020E04B0 =	0x00020000	// IOMUXC_SW_PAD_CTL_GRP_DDRMODE				
setmem /32	0x020E0498 =	0x00000030	// IOMUXC_SW_PAD_CTL_GRP_B0DS 				
setmem /32	0x020E04A4 =	0x00000030	// IOMUXC_SW_PAD_CTL_GRP_B1DS 				
setmem /32	0x020E0244 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0				
setmem /32	0x020E0248 =	0x00000030	// IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1				
							
//=============================================================================							
// DDR Controller Registers							
//=============================================================================							
// Manufacturer:	Micron						
// Device Part Number:	MT41K256M16HA-125E						
// Clock Freq.: 	400MHz						
// Density per CS in Gb: 	4						
// Chip Selects used:	1						
// Total DRAM density (Gb)	4						
// Number of Banks:	8						
// Row address:    	15						
// Column address: 	10						
// Data bus width	16						
//=============================================================================							
setmem /32	0x021B001C =	0x00008000	// [MMDC_MDSCR] MMDC Core Special Command Register 				
							
//======================================================							
//Calibrations:							
//======================================================							
setmem /32	0x021B0800 =	0xA1390003 	// DDR_PHY_P0_MPZQHWCTRL, enable both one-time & periodic HW ZQ calibration.				
							
setmem /32	0x021B080C = 	0x00000000	// [MMDC_MPWLDECTRL0] MMDC PHY Write Leveling Delay Control Register 0				
setmem /32	0x021B083C =	0x41490145	// [MMDC_MPDGCTRL0] MMDC PHY Read DQS Gating Control Register 0				
setmem /32	0x021B0848 =	0x40404546	// [MMDC_MPRDDLCTL] MMDC PHY Read delay-lines Configuration Register				
setmem /32	0x021B0850 =	0x4040524D	// [MMDC_MPWRDLCTL] MMDC PHY Write delay-lines Configuration Register				
							
setmem /32	0x021B081C =	0x33333333	// [MMDC_MPRDDQBY0DL] MMDC PHY Read DQ Byte0 Delay Register				
setmem /32	0x021B0820 =	0x33333333	// [MMDC_MPRDDQBY1DL] MMDC PHY Read DQ Byte1 Delay Register				
							
setmem /32	0x021B082C =	0xf3333333	// [MMDC_MPWRDQBY0DL] MMDC PHY Write DQ Byte0 Delay Register				
setmem /32	0x021B0830 =    0xf3333333	// [MMDC_MPWRDQBY1DL] MMDC PHY Write DQ Byte1 Delay Register				
							
setmem /32	0x021B08C0 =	0x00921012	// [MMDC_MPDCCR] MMDC Duty Cycle Control Register				
							
// Complete calibration by forced measurement:							
setmem /32	0x021B08B8 =	0x00000800 	// DDR_PHY_P0_MPMUR0, frc_msr				
							
//======================================================							
//MMDC init:							
//======================================================							
setmem /32	0x021B0004 =	0x0002002D	// MMDC0_MDPDC 				
setmem /32	0x021B0008 =	0x1B333030	// MMDC0_MDOTC				
setmem /32	0x021B000C =	0x676B52F3	// MMDC0_MDCFG0				
setmem /32	0x021B0010 =	0xB66D0B63	// MMDC0_MDCFG1				
setmem /32	0x021B0014 =	0x01FF00DB	// MMDC0_MDCFG2				
setmem /32	0x021B0018 =	0x00201740	// MMDC0_MDMISC				
setmem /32	0x021B002C =	0x000026D2	// MMDC0_MDRWD; recommend to maintain the default values				
setmem /32	0x021B0030 =	0x006B1023	// MMDC0_MDOR				
setmem /32	0x021B0040 =	0x0000004F	// CS0_END 				
setmem /32	0x021B0000 =	0x84180000	// MMDC0_MDCTL				
							
// Mode register writes for CS0							
setmem /32	0x021B001C =	0x02008032	// MMDC0_MDSCR, MR2 write, CS0				
setmem /32	0x021B001C =	0x00008033	// MMDC0_MDSCR, MR3 write, CS0				
setmem /32	0x021B001C =	0x00048031	// MMDC0_MDSCR, MR1 write, CS0				
setmem /32	0x021B001C =	0x15208030	// MMDC0_MDSCR, MR0 write, CS0				
setmem /32	0x021B001C =	0x04008040	// MMDC0_MDSCR, ZQ calibration command sent to device on CS0				
							
// Mode register writes for CS, commented out automatically if only one chip select used							
//setmem /32 	0x021B001C =	0x0200803A	// MMDC0_MDSCR, MR2 write, CS1				
//setmem /32 	0x021B001C =	0x0000803B	// MMDC0_MDSCR, MR3 write, CS1				
//setmem /32 	0x021B001C =	0x00048039	// MMDC0_MDSCR, MR1 write, CS1				
//setmem /32 	0x021B001C =	0x15208038	// MMDC0_MDSCR, MR0 write, CS1				
//setmem /32 	0x021B001C =	0x04008048	// MMDC0_MDSCR, ZQ calibration command sent to device on CS1				
							
// final DDR setup, before operation start:							
setmem /32	0x021B0020 =	0x00000800	// MMDC0_MDREF				
setmem /32	0x021B0818 =	0x00000227	// DDR_PHY_P0_MPODTCTRL				
setmem /32	0x021B0004 =	0x0002556D	// MMDC0_MDPDC now SDCTL power down enabled				
setmem /32	0x021B0404 =	0x00011006 	// MMDC0_MAPSR ADOPT power down enabled				
setmem /32	0x021B001C =	0x00000000	// MMDC0_MDSCR, clear this register (especially the configuration bit as initialization is complete)				
