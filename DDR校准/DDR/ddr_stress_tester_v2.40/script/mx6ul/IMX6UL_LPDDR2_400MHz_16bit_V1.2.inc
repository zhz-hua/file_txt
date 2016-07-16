//*================================================================================================
//* Copyright (C) 2011, Freescale Semiconductor, Inc. All Rights Reserved
//* THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
//* BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
//* Freescale Semiconductor, Inc.
//*================================================================================================


//*=======================================================*/
//1. Update LPDDR2 ZQ params [MMDC_MPZQLP2CTL]
//
//*=======================================================*/



wait = on

//*================================================================================================
// Disable WDOG
//*================================================================================================
//setmem /16 0x020bc000 = 0x30

//*================================================================================================
// Enable all clocks (they are disabled by ROM code)
//*================================================================================================
setmem /32 0x020c4068 = 0xffffffff  //[CCM_CCGR0]CCM Clock Gating Register 0
setmem /32 0x020c406c = 0xffffffff  //[CCM_CCGR1]CCM Clock Gating Register 1
setmem /32 0x020c4070 = 0xffffffff  //[CCM_CCGR2]CCM Clock Gating Register 2
setmem /32 0x020c4074 = 0xffffffff  //[CCM_CCGR3]CCM Clock Gating Register 3
setmem /32 0x020c4078 = 0xffffffff  //[CCM_CCGR4]CCM Clock Gating Register 4
setmem /32 0x020c407c = 0xffffffff  //[CCM_CCGR5]CCM Clock Gating Register 5
setmem /32 0x020c4080 = 0xffffffff  //[CCM_CCGR6]CCM Clock Gating Register 6
setmem /32 0x020c4084 = 0xffffffff  //[CCM_CCGR7]CCM Clock Gating Register 7


//*================================================================================================
// Initialize 16-bit LPDDR2
//*================================================================================================

//======================================================
// IOMUX 
//======================================================

//DDR IO TYPE:
setmem /32 0x020E04B4 = 0x00080000	// [IOMUXC_SW_PAD_CTL_GRP_DDR_TYPE]
                                    //      <19:18> DDR_SEL         = 2'b10:    set pads group DDR_TYPE(ADDRs/CAS_B/RAS_B/CS_Bs/SDWE_B/SDBAs/DATAs/DQMs/ODTs/SDCKEs/CLK_Ps/DQS_Ps) as DDR3 Mode;
setmem /32 0x020E04AC = 0x00000000	// [IOMUXC_SW_PAD_CTL_GRP_DDRPKE]
                                    //      <12>    PKE             = 1'b0:     set pads group DDRPKE(ADDRs/CAS_B/RAS_B/CS_Bs/SDWE_B/SDBAs/DATAs/DQMs/ODTs/SDCKEs/CLK_Ps/DQS_Ps) Pull/Keep Enable Field: Disabled;

//CLOCK:
setmem /32 0x020E027C = 0x00000030  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK0_P]
                                    //      <25:24> DO_TRIM_PADN    = 2'b00:    min delay;
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_SDCLK0_P as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_SDCLK0_P Hysteresis as Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_SDCLK0_P ODT off;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_SDCLK0_P DSE as R0/6;

//ADDRESS:
setmem /32 0x020E0250 = 0x00000030 // [IOMUXC_SW_PAD_CTL_PAD_DRAM_CAS_B]
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_CAS_B as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_CAS_B Hysteresis as Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_CAS_B ODT off;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_CAS_B DSE as R0/6;
setmem /32 0x020E024C = 0x00000030 // [IOMUXC_SW_PAD_CTL_PAD_DRAM_RAS_B]
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_RAS_B as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_RAS_B Hysteresis as Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_RAS_B ODT off;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_RAS_B DSE as R0/6;
setmem /32 0x020E0490 = 0x00000030 // [IOMUXC_SW_PAD_CTL_GRP_ADDDS]
                                    //      <5:3>   DSE             = 3'b110:   set ADDDS (ADDRs/SDBA0/SDBA1) DSE as R0/6;

//CONTROL:
setmem /32 0x020E0288 = 0x00000030 // [IOMUXC_SW_PAD_CTL_PAD_DRAM_RESET]
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <19:18> DDR_SEL         = 2'b11:    set DRAM_RESET as DDR3 Mode;
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_RESET as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_RESET Hysteresis as Disabled;
                                    //      <15:14> PUS             = 2'b00:    set DRAM_RESET 100K Ohm Pull Down;
                                    //      <13>    PUE             = 1'b0:     set DRAM_RESET Pull/Keep Select Field: Keeper;
                                    //      <12>    PKE             = 1'b0:     set DRAM_RESET Pull/Keep Enable Field: Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_RESET ODT off;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_RESET DSE as R0/6;
setmem /32 0x020E0270 = 0x00000000  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_SDBA2]
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_SDBA2 as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_SDBA2 Hysteresis as Disabled;
                                    //      <13>    PUE             = 1'b0:     set DRAM_SDBA2 Pull/Keep Select Field: Keeper;
                                    //      <12>    PKE             = 1'b0:     set DRAM_SDBA2 Pull/Keep Enable Field: Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_SDBA2 ODT off;
setmem /32 0x020E0260 = 0x00000000  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_ODT0]
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_ODT0 as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_ODT0 Hysteresis as Disabled;
                                    //      <15:14> PUS             = 2'b00:    set DRAM_ODT0 100K Ohm Pull Down;
                                    //      <13>    PUE             = 1'b0:     set DRAM_ODT0 Pull/Keep Select Field: Keeper;
                                    //      <12>    PKE             = 1'b0:     set DRAM_ODT0 Pull/Keep Enable Field: Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_ODT0 ODT off;
                                    //      <5:3>   DSE             = 3'b000:   set DRAM_ODT0 DSE output driver disabled;
setmem /32 0x020E0264 = 0x00000000  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_ODT1]
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_ODT1 as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_ODT1 Hysteresis as Disabled;
                                    //      <15:14> PUS             = 2'b00:    set DRAM_ODT1 100K Ohm Pull Down;
                                    //      <13>    PUE             = 1'b0:     set DRAM_ODT1 Pull/Keep Select Field: Keeper;
                                    //      <12>    PKE             = 1'b0:     set DRAM_ODT1 Pull/Keep Enable Field: Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_ODT1 ODT off;
                                    //      <5:3>   DSE             = 3'b000:   set DRAM_ODT1 DSE output driver disabled;
setmem /32 0x020E04A0 = 0x00000030  // [IOMUXC_SW_PAD_CTL_GRP_CTLDS]
                                    //      <5:3>   DSE             = 3'b110:   set pads group CTLDS(CS0_B/CS1_B/SDBA2/SDCKE0/SDCKE1/SDWE_B) DSE as R0/6;

//DATA STROBE:
setmem /32 0x020E0494 = 0x00020000  // [IOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL]
                                    //      <17>    DDR_INPUT       = 1'b1:     set pads group DDRMODE_CTL(SDQS0_P/SDQS1_P) as Differential input type;
    //in case of DDR timing issue on your board you can try DDR_MODE:  [= 0x00020000]

setmem /32 0x020E0280 = 0x00003030  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS0_P]
                                    //      <25:24> DO_TRIM_PADN    = 2'b00:    min delay;
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <15:14> PUS             = 2'b00:    set DRAM_SDQS0_P 100K Ohm Pull Down;
                                    //      <13>    PUE             = 1'b1:     set DRAM_SDQS0_P Pull/Keep Select Field: Pull;
                                    //      <12>    PKE             = 1'b1:     set DRAM_SDQS0_P Pull/Keep Enable Field: Enabled;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_SDQS0_P DSE as R0/6;
setmem /32 0x020E0284 = 0x00003030  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS1_P]
                                    //      <25:24> DO_TRIM_PADN    = 2'b00:    min delay;
                                    //      <21:20> DO_TRIM         = 2'b00:    min delay;
                                    //      <15:14> PUS             = 2'b00:    set DRAM_SDQS1_P 100K Ohm Pull Down;
                                    //      <13>    PUE             = 1'b1:     set DRAM_SDQS1_P Pull/Keep Select Field: Pull;
                                    //      <12>    PKE             = 1'b1:     set DRAM_SDQS1_P Pull/Keep Enable Field: Enabled;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_SDQS1_P DSE as R0/6;

//DATA:
setmem /32 0x020E04B0 = 0x00020000  // [IOMUXC_SW_PAD_CTL_GRP_DDRMODE]
                                    //      <17>    DDR_INPUT       = 1'b1:     set pads group DDRMODE(DRAM_DATAs) as Differential input type;
    //in case of DDR timing issue on your board you can try DDR_MODE:  [= 0x00020000]

setmem /32 0x020E0498 = 0x00000030  // [IOMUXC_SW_PAD_CTL_GRP_B0DS]
                                    //      <5:3>   DSE             = 3'b110:   set pads group B0DS(DRAM_DATA00-07) DSE as R0/6;
setmem /32 0x020E04A4 = 0x00000030  // [IOMUXC_SW_PAD_CTL_GRP_B1DS]
                                    //      <5:3>   DSE             = 3'b110:   set pads group B1DS(DRAM_DATA08-15) DSE as R0/6;

setmem /32 0x020E0244 = 0x00000030  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0]
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_DQM0 as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_DQM0 Hysteresis as Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_DQM0 ODT off;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_DQM0 DSE as R0/6;
setmem /32 0x020E0248 = 0x00000030  // [IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1] - DSE=110, DDR_INPUT=1, HYS=0
                                    //      <17>    DDR_INPUT       = 1'b0:     set DRAM_DQM1 as CMOS input type;
                                    //      <16>    HYS             = 1'b0:     set DRAM_DQM1 Hysteresis as Disabled;
                                    //      <10:8>  ODT             = 3'b000:   set DRAM_DQM1 ODT off;
                                    //      <5:3>   DSE             = 3'b110:   set DRAM_DQM1 DSE as R0/6;


            
//======================================================
// DDR Controller Registers
//======================================================
// Manufacturer:            
// Device Part Number:      
// Clock Freq.:             400MHz
// Density per CS in Gb:    
// Chip Selects used:       1
// Number of Banks:         8
// Row address:             14
// Column address:          10
// Data bus width:          16
//======================================================

setmem /32 0x021B001C = 0x00008000 // [MMDC_MDSCR] MMDC Core Special Command Register
                                    //      <31:24> CMD_ADDR_MSB_MR_OP  = 8'h00
                                    //      <23:16> CMD_ADDR_LSB_MR_OP  = 8'h00
                                    //      <15>    CON_REQ             = 1'b1:     A request to configure MMDC is valid;
                                    //      <9>     WL_EN               = 1'b0:     This bit controls the DQS pads direction during write-leveling calibration process;
                                    //      <6:4>   CMD                 = 3'b000:   This field contains the command to be executed. 0x0 stands for Normal Operation;
                                    //      <3>     CMD_CS              = 1'b0:     This field determines which chip select the command is targeted to;
                                    //      <2:0>   CMD_BA              = 3'b000:   This field determines the bank addr within the selected chip-select...;

setmem /32 0x021b085c = 0x1b4700c7 //LPDDR2 ZQ params [MMDC_MPZQLP2CTL]

//======================================================
//Calibrations:
//======================================================
// ZQ:
setmem /32 0x021B0800 = 0xA1390003 // [MMDC_MPZQHWCTRL] MMDC PHY ZQ HW control register, enable both one-time & periodic HW ZQ calibration.
                                    //      <31:27> ZQ_EARLY_COMPARATOR_EN_TIMER    = 5'h14:    set interval as 21 cycles(default);
                                    //      <25:23> TZQ_CS                          = 3'b010:   set Device ZQ short time as 128 cycles;
                                    //      <22:20> TZQ_OPER                        = 3'b011:   set Device ZQ long/oper time as 256 cycles;
                                    //      <19:17> TZQ_INIT                        = 3'b100:   set Device ZQ long/init time as 512 cycles;
                                    //      <16>    ZQ_HW_FOR                       = 1'b1:     When this bit is asserted, the MMDC will issue one ZQ auto calibration;
                                    //      <5:2>   ZQ_HW_PER                       = 4'b0000:  set ZQ Periodic Calibration Time as 1 ms;
                                    //      <1:0>   ZQ_MODE                         = 2'b11:    set ZQ Calibration Mode;
                                    
//CA
setmem /32 0x021b0890 = 0x00470000  // []
setmem /32 0x021B08b8 = 0x00000800  // [MMDC_MPMUR0] MMDC PHY Measure Unit Register

//DQS gating, read delay, write delay calibration values based on calibration compare of 0x00ffff00:
// It is highly recommended for the user to run calibration code on her/his specific board 
//and replace following delay values accordingly: 



//read data bit delay: (3 is the reccommended default value, although out of reset value is 0):
setmem /32 0x021B081C = 0x33333333  // [MMDC_MPRDDQBY0DL] MMDC PHY Read DQ Byte0 Delay Register
                                    //      <30:28> rd_dq7_del          = 3'b011:   Read DQS0 to DQ7  Delay FineTuing: add DQ7  delay of 3 delay units;
                                    //      <26:24> rd_dq6_del          = 3'b011:   Read DQS0 to DQ6  Delay FineTuing: add DQ6  delay of 3 delay units;
                                    //      <22:20> rd_dq5_del          = 3'b011:   Read DQS0 to DQ5  Delay FineTuing: add DQ5  delay of 3 delay units;
                                    //      <18:16> rd_dq4_del          = 3'b011:   Read DQS0 to DQ4  Delay FineTuing: add DQ4  delay of 3 delay units;
                                    //      <14:12> rd_dq3_del          = 3'b011:   Read DQS0 to DQ3  Delay FineTuing: add DQ3  delay of 3 delay units;
                                    //      <10:8>  rd_dq2_del          = 3'b011:   Read DQS0 to DQ2  Delay FineTuing: add DQ2  delay of 3 delay units;
                                    //      <6:4>   rd_dq1_del          = 3'b011:   Read DQS0 to DQ1  Delay FineTuing: add DQ1  delay of 3 delay units;
                                    //      <2:0>   rd_dq0_del          = 3'b011:   Read DQS0 to DQ0  Delay FineTuing: add DQ0  delay of 3 delay units;
setmem /32 0x021B0820 = 0x33333333  // [MMDC_MPRDDQBY1DL] MMDC PHY Read DQ Byte1 Delay Register
                                    //      <30:28> rd_dq15_del         = 3'b011:   Read DQS1 to DQ15 Delay FineTuing: add DQ15 delay of 3 delay units;
                                    //      <26:24> rd_dq14_del         = 3'b011:   Read DQS1 to DQ14 Delay FineTuing: add DQ14 delay of 3 delay units;
                                    //      <22:20> rd_dq13_del         = 3'b011:   Read DQS1 to DQ13 Delay FineTuing: add DQ13 delay of 3 delay units;
                                    //      <18:16> rd_dq12_del         = 3'b011:   Read DQS1 to DQ12 Delay FineTuing: add DQ12 delay of 3 delay units;
                                    //      <14:12> rd_dq11_del         = 3'b011:   Read DQS1 to DQ11 Delay FineTuing: add DQ11 delay of 3 delay units;
                                    //      <10:8>  rd_dq10_del         = 3'b011:   Read DQS1 to DQ10 Delay FineTuing: add DQ10 delay of 3 delay units;
                                    //      <6:4>   rd_dq9_del          = 3'b011:   Read DQS1 to DQ9  Delay FineTuing: add DQ9  delay of 3 delay units;
                                    //      <2:0>   rd_dq8_del          = 3'b011:   Read DQS1 to DQ8  Delay FineTuing: add DQ8  delay of 3 delay units;
                                    
//write data bit delay
setmem /32 0x021B082C = 0xf3333333  // [MMDC_MPWRDQBY0DL] MMDC PHY Write DQ Byte0 Delay Register
                                    //      <31:30> wr_dm0_del          = 2'b11:    Write DM0 Delay FineTuing: add DM0 delay of 3 delay units;
                                    //      <29:28> wr_dq7_del          = 2'b11:    Write DQS0 to DQ7  Delay FineTuing: add DQ7  delay of 3 delay units;
                                    //      <25:24> wr_dq6_del          = 2'b11:    Write DQS0 to DQ6  Delay FineTuing: add DQ6  delay of 3 delay units;
                                    //      <21:20> wr_dq5_del          = 2'b11:    Write DQS0 to DQ5  Delay FineTuing: add DQ5  delay of 3 delay units;
                                    //      <17:16> wr_dq4_del          = 2'b11:    Write DQS0 to DQ4  Delay FineTuing: add DQ4  delay of 3 delay units;
                                    //      <13:12> wr_dq3_del          = 2'b11:    Write DQS0 to DQ3  Delay FineTuing: add DQ3  delay of 3 delay units;
                                    //      <9:8>   wr_dq2_del          = 2'b11:    Write DQS0 to DQ2  Delay FineTuing: add DQ2  delay of 3 delay units;
                                    //      <5:4>   wr_dq1_del          = 2'b11:    Write DQS0 to DQ1  Delay FineTuing: add DQ1  delay of 3 delay units;
                                    //      <1:0>   wr_dq0_del          = 2'b11:    Write DQS0 to DQ0  Delay FineTuing: add DQ0  delay of 3 delay units;
setmem /32 0x021B0830 = 0xf3333333  // [MMDC_MPWRDQBY1DL] MMDC PHY Write DQ Byte1 Delay Register
                                    //      <31:30> wr_dm1_del          = 2'b11:    Write DM1 Delay FineTuing: add DM1 delay of 3 delay units;
                                    //      <29:28> wr_dq15_del         = 2'b11:    Write DQS1 to DQ15 Delay FineTuing: add DQ15 delay of 3 delay units;
                                    //      <25:24> wr_dq14_del         = 2'b11:    Write DQS1 to DQ14 Delay FineTuing: add DQ14 delay of 3 delay units;
                                    //      <21:20> wr_dq13_del         = 2'b11:    Write DQS1 to DQ13 Delay FineTuing: add DQ13 delay of 3 delay units;
                                    //      <17:16> wr_dq12_del         = 2'b11:    Write DQS1 to DQ12 Delay FineTuing: add DQ12 delay of 3 delay units;
                                    //      <13:12> wr_dq11_del         = 2'b11:    Write DQS1 to DQ11 Delay FineTuing: add DQ11 delay of 3 delay units;
                                    //      <9:8>   wr_dq10_del         = 2'b11:    Write DQS1 to DQ10 Delay FineTuing: add DQ10 delay of 3 delay units;
                                    //      <5:4>   wr_dq9_del          = 2'b11:    Write DQS1 to DQ9  Delay FineTuing: add DQ9  delay of 3 delay units;
                                    //      <1:0>   wr_dq8_del          = 2'b11:    Write DQS1 to DQ8  Delay FineTuing: add DQ8  delay of 3 delay units;

//DQS Gating
setmem /32 0x021B083C = 0x20000000  //[MMDC_MPDGCTRL0] MMDC PHY Read DQS Gating Control Register 0

//Read Calibration
setmem /32 0x021B0848 = 0x4040484F  // [MMDC_MPRDDLCTL] MMDC PHY Read delay-lines Configuration Register
                                    //      <14:8>  RD_DL_ABS_OFFSET1   = 7'b       Absolute Read Delay Offset for Byte1
                                    //      <6:0>   RD_DL_ABS_OFFSET0   = 7'b       Absolute Read Delay Offset for Byte0

//Write Calibration
setmem /32 0x021B0850 = 0x40405247  // [MMDC_MPWRDLCTL] MMDC PHY Write delay-lines Configuration Register
                                    //      <14:8>  WR_DL_ABS_OFFSET1   = 7'b       Absolute Write Delay Offset for Byte1
                                    //      <6:0>   WR_DL_ABS_OFFSET0   = 7'b       Absolute Write Delay Offset for Byte0


//DQS&CLK Duty Cycle
setmem /32 0x021B08C0 = 0x00921012  // [MMDC_MPDCCR] MMDC Duty Cycle Control Register
                                    //      <24:22> RD_DQS1_FT_DCC      = 3'b010:   Read DQS Duty Cycle Fine Tuning of Byte1, 010 stands for 50%;
                                    //      <21:19> RD_DQS0_FT_DCC      = 3'b010:   Read DQS Duty Cycle Fine Tuning of Byte0, 010 stands for 50%;
                                    //      <18:16> CK_FT1_DCC          = 3'b010:   Secondary Duty Cycle Fine Tuning Control of CLK, 010 stands for 50%;
                                    //      <14:12> CK_FT0_DCC          = 3'b010:   Primary   Duty Cycle Fine Tuning Control of CLK, 010 stands for 50%;
        //Note: The duty cycle may be changed during DDR initialization without having to be placed in self-refresh mode.
        //Note: If the duty cycle need to be modified after DDR initialization, the DDR will have to be placed in self-refresh mode.

//Perform Calibration by Forcing Measurment
setmem /32 0x021B08b8 = 0x00000800  // [MMDC_MPMUR0] MMDC PHY Measure Unit Register
                                    //      <11>    FRC_MSR             = 1'b1:     Force Measurement on Delay Lines. Self cleared;
                                    //      <10>    MU_BYP_EN           = 1'b0:     Measure Unit Bypass Enable. This bit is used in debug mode;
                                    //      <9:0>   MU_BYP_VAL          = 10'd0:    Number of Delay Units for Measurement Bypass. This bit is used in debug mode;


                                    
//======================================================
//MMDC init:
//======================================================

//400MHz
setmem /32 0x021B0004 = 0x00020052 // [MMDC0_MDPDC] MMDC Core Power Down Control Register
                                    //      <30:28> PRCT_1              = 3'b000:   Determines the amount of idle cycle for which CS 1 will be automatically precharged;
                                    //      <26:24> PRCT_0              = 3'b000:   Determines the amount of idle cycle for which CS 0 will be automatically precharged;
                                    //      <18:16> tCKE                = 3'b010:   CKE Minimum Pulse Width: 3'b010 stands for 3CK;
                                    //      <15:12> PWDT_1              = 4'b0000:  Determines the amount of idle cycle for which CS 1 will be automatically get into precharge/active power down;
                                    //      <11:8>  PWDT_0              = 4'b0000:  Determines the amount of idle cycle for which CS 0 will be automatically get into precharge/active power down;
                                    //      <7>     SLOW_PD             = 1'b0:     set Slow/Fast Power Down as Fast Mode(Note: Memory should be configured the same);
                                    //      <6>     BOTH_CS_PD          = 1'b1:     Each chip select can enter power down independently according to its configuration;
                                    //      <5:3>   tCKSRX              = 3'b010:   Determines the amount of clock cycles before self-refresh exit: 1'b010 stands for 2CKs;
                                    //      <2:0>   tCKSRE              = 3'b010:   Determines the amount of clock cycles after self-refresh exit:  1'b010 stands for 2CKs;
setmem /32 0x021B0008 = 0x00000000 // [MMDC_MDOTC] MMDC Core ODT Timing Control Register
                                    //      <29:27> tAOFPD              = 3'b011:   Asynchronous RTT turn-off delay(power down with DLL frozen); ???
                                    //      <26:24> tAONPD              = 3'b011:   Asynchronous RTT turn-on  delay(power down with DLL frozen); ???
                                    //      <23:20> tANPD               = 4'b0011:  Asynchronous ODT to power down entry delay. In DDR3 should be set to tCWL-1; ???
                                    //      <19:16> tAXPD               = 4'b0011:  Asynchronous ODT to power down exit delay. In DDR3 should be set to tCWL-1; ???
                                    //      <14:12> tODTLon             = 3'b000:   ODT turn on latency;
                                    //      <8:4>   tODT_idle_off       = 5'b00000: ODT turn off latency;
setmem /32 0x021B000C = 0x33374133 // [MMDC_MDCFG0] MMDC Core Timing Configuration Register 0
                                    //      <31:24> tRFC                = 8'h33:    Refresh command to Active or Refresh command Time: 8'h33 stands for 52CKs;
                                    //      <23:16> tXS                 = 8'h37:    Exit Self Refresh to non READ command: 8'h37 stands for 56CKs;
                                    //      <15:13> tXP                 = 3'b010:   Exit power down with DLL-on to any valid command;
                                    //      <12:9>  tXPDLL              = 4'b0000:  Exit precharge power down with DLL frozen to commands requiring DLL;
                                    //      <8:4>   tFAW                = 5'b10011: Four Active Window (all banks): 5'b10011 stands for 20CKs;
                                    //      <3:0>   tCL                 = 4'b0011:  CAS Read Latency: 0x3 stands for 6 cycles;
setmem /32 0x021B0010 = 0x00100A82 // [MMDC_MDCFG1] MMDC Core Timing Configuration Register 1
                                    //      <31:29> tRCD                = 3'b000:   see MMDC_MDCFG3LP <tRCD_LP>;
                                    //      <28:26> tRP                 = 3'b000:   see MMDC_MDCFG3LP <tRPpb_LP>;
                                    //      <25:21> tRC                 = 5'b00000: see MMDC_MDCFG3LP <tRC_LP>;
                                    //      <20:16> tRAS                = 5'b10000: 5'b10000 stands for 17CKs;
                                    //      <15>    tRPA                = 1'b0:     see MMDC_MDCFG3LP <tRPab_LP>;
                                    //      <11:9>  tWR                 = 3'b101:   15ns = 6CKs@400MHz. 3'b101 stands for 6CKs;
                                    //      <8:5>   tMRD                = 4'b0100:  4'b0100 stands for 5CKs;
                                    //      <2:0>   tCWL                = 3'b010:   3CKs;
setmem /32 0x021B0038 = 0x00170557 // [MMDC_MDCFG3LP] MMDC Core Timing Configuration Register 3
									//      <21:16> RC_LP               = 6'b010111:Active to Active or Refresh command period (same bank).;
                                    //      <11:8>  tRCD_LP             = 4'b0101:  Active command to internal read or write delay time (same bank);
                                    //      <7:4>   tRPpb_LP            = 4'b0101:  Precharge (per bank) command period (same bank);
                                    //      <3:0>   tRPab_LP            = 4'b0111:  Precharge (all banks) command period;
setmem /32 0x021B0014 = 0x00000093  // [MMDC_MDCFG2] MMDC Core Timing Configuration Register 2
setmem /32 0x021B0018 = 0x00201748  // [MMDC_MDMISC] MMDC Core Miscellaneous Register
                                    //      <21>    CK1_GATING          = 1'b1:     MMDC drives only one clock toward the DDR memory(CK0);
                                    //      <20>    CALIB_PER_CS        = 1'b0:     Calibration is targeted to: CS0;
                                    //      <19>    ADDR_MIRROR         = 1'b0:     set Address Mirroring as disabled;
                                    //      <18>    LHD                 = 1'b0:     This is a debug feature: When set to "1" the MMDC will handle one read/write access at a time;
                                    //      <17:16> WALAT               = 2'b01:    ???;
                                    //      <12>    BI_ON??             = 1'b1:     ???;
                                    //      <11>    LPDDR2_S2           = 1'b:      ???;
                                    //      <10:9>  MIF3_MODE           = 2'b:      ???;
                                    //      <8:6>   RALAT               = 3'b101:   : 3'b101 stands for 5CKs;
                                    //      <5>     DDR_4_BANK          = 1'b0:     set Number of banks per DDR device. 0 stands for 8 banks;
                                    //      <4:3>   DDR TYPE            = 2'b00:    DDR3 SDRAM device is used.
                                    //      <1>     RST                 = 1'b0:     Software Reset: 0 means do nothing;
//MDMISC: RALAT kept to the high level of 5. 
//MDMISC: consider reducing RALAT if your 528MHz board design allow that. Lower RALAT benefits: 
//a. better operation at low frequency, for LPDDR2 freq < 100MHz, change RALAT to 3
//b. Small performence improvment


setmem /32 0x021B002C = 0x0F9F26D2 // [MMDC_MDRWD]
//setmem /32 0x021B002C = 0x0F9F0682 // [MMDC_MDRWD]
                                    //      <28:16> tDAI                = 13'hF9F:  MMDC drives only one clock toward the DDR memory(CK0);
                                    //      <14:12> RTW_SAME            = 3'b0:     Read to write delay for the same chip-select.;
                                    //      <11:9>  WTR_DIFF            = 3'b011:   Write to read delay for different chip-select;
                                    //      <8:6>   WTW_DIFF            = 3'b010:   Write to write delay for different chip-select; (=BL/2);
                                    //      <5:3>   RTW_DIFF            = 3'b01:    Read to write delay for different chip-select;
                                    //      <2:0>   RTR_DIFF            = 3'b001:     Read to read delay for different chip-select;
setmem /32 0x021B0030 = 0x009F0010  //[MMDC_MDOR]
                                    //0x10 14 cycles (JEDEC value for LPDDR2) - total of 200 us
                                    //The field “tXPR”<23:16> in MDOR register is mentioned as “not relevant” in lpddr2 mode, so in some of our released script we set it to 0.
                                    //However, we find that set it to 0 will impact mmdc initialization timing,  making initial timing violate Jedec spec.
                                    //So you can left it with default value instead of setting the “tXPR” field to 0. 								
setmem /32 0x021B0040 = 0x00000047  // [MMDC_MDASP]
                                    //      <6:0>   CS0_END             = 7'h47:    (8000_0000 + 1000_0000) - 1 = 8FFF_FFFF;
//setmem /32 0x021B0400 = 0x12420000    // [disable mmdc re-aorder in MMDC
setmem /32 0x021B0000 = 0x83100000  // [MMDC_MDCTL] MMDC Core Control Register
                                    //      <31>    SDE_0               = 1'b1:     MMDC Enable CS0;
                                    //      <30>    SDE_1               = 1'b0:     MMDC Enable CS1;
                                    //      <26:24> ROW                 = 3'b011:   set ROW Address Width, 3'b011 stands for 14bits Row;
                                    //      <22:20> COL                 = 3'b001:   set COL Address Width, 3'b001 stands for 10bits Column;
                                    //      <19>    BL                  = 1'b0:     set Burst Length as 4;
                                    //      <17:16> DSIZ                = 2'b00:    set DDR Data Bus Size as 16-bit;
//setmem /32 0x021B0890 = 0x00000b48


//======================================================
// Initialize DDR3 SDRAM Device
//======================================================
//MR63_Reset:
setmem /32 0x021B001C = 0x003F8030  // [MMDC_MDSCR]
//MR10_Calibration:
setmem /32 0x021B001C = 0xFF0A8030  // [MMDC_MDSCR]
//MR1_Device Feature 1:
setmem /32 0x021B001C = 0x82018030  // [MMDC_MDSCR]	BL=4, BT is Sequential, 
//MR2_Device Feature 2:
setmem /32 0x021B001C = 0x04028030  // [MMDC_MDSCR]	RL=6/WL=3, So,OP[3:0]=0100
//MR3_IO Configuration 1:
setmem /32 0x021B001C = 0x01038030  // [MMDC_MDSCR]	Drive = 240/7, So, OP[3:0]=0001

//======================================================
//final DDR setup, before operation start:
//======================================================
setmem /32 0x021B0020 = 0x00001800  // [MMDC_MDREF] enable auto refresh, set refresh rate.
                                    //      <31:16> REF_CNT             = 16'h0000:     0;
                                    //      <15:14> REF_SEL             = 2'b00:        Periodic refresh cycles will be triggered in frequency of 64KHz;
                                    //      <13:11> REFR                = 3'b011:       2 refreshes every refresh cycle;
                                    //      <0>     START_REF           = 1'b0:         Do nothing;

//Following ODT setup (0x11117) represents(along with obove DDR device configs) : i.mx_ODT=DDR_device_ODT=120OHm.
//User might to also interested in trying the value of 0x00000007,which represents: i.mx_ODT disabled, DDR_device_ODT=120Ohm.
//0x00000007 saves more power, and seen to run very well with Freescale RDKs. Still, running with no ODT has it's implications 
// of signal integrity and should be carefully simulated during board design.

setmem /32 0x021B0818 = 0x00000000  // [MMDC_MPODTCTRL] MMDC PHY ODT control register
                                    //      <10:8>  ODT1_INT_RES        = 3'b010:       Determines the Rtt_Nom of the on chip ODT byte1 resistor as 60Ohm;
                                    //      <6:4>   ODT0_INT_RES        = 3'b010:       Determines the Rtt_Nom of the on chip ODT byte0 resistor as 60Ohm;
                                    //      <3>     ODT_RD_ACT_EN       = 1'b0:         ;
                                    //      <2>     ODT_RD_PAS_EN       = 1'b1:         ;
                                    //      <1>     ODT_WR_ACT_EN       = 1'b1:         ;
                                    //      <0>     ODT_WR_PAS_EN       = 1'b1:         ;

setmem /32 0x021B0800 = 0xA1310003  //DDR_PHY_P0_MPZQHWCTRL

setmem /32 0x021B0004 = 0x00025552  // [MMDC_MDPDC]
                                    //      <30:28> PRCT_1              = 3'b000:   Determines the amount of idle cycle for which CS 1 will be automatically precharged;
                                    //      <26:24> PRCT_0              = 3'b000:   Determines the amount of idle cycle for which CS 0 will be automatically precharged;
                                    //      <18:16> tCKE                = 3'b010:   CKE Minimum Pulse Width: 3'b010 stands for 3CK;
                                    //      <15:12> PWDT_1              = 4'b0101:  Determines the amount of idle cycle for which CS 1 will be automatically get into precharge/active power down;
                                    //      <11:8>  PWDT_0              = 4'b0101:  Determines the amount of idle cycle for which CS 0 will be automatically get into precharge/active power down;
                                    //      <7>     SLOW_PD             = 1'b0:     set Slow/Fast Power Down as Fast Mode(Note: Memory should be configured the same);
                                    //      <6>     BOTH_CS_PD          = 1'b1:     Each chip select can enter power down independently according to its configuration;
                                    //      <5:3>   tCKSRX              = 3'b010:   Determines the amount of clock cycles before self-refresh exit: 3'b010 stands for 2CKs;
                                    //      <2:0>   tCKSRE              = 3'b010:   Determines the amount of clock cycles after self-refresh exit:  3'b010 stands for 2CKs;
setmem /32 0x021B0404 = 0x00011006  // [MMDC_MAPSR] MMDC Core Power Saving Control and Status Register



setmem /32 0x021B001C = 0x00000000  // [MMDC_MDSCR]	//De-assert CON_REQ

