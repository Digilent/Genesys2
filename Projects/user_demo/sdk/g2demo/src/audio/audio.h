/******************************************************************************
 * @file audio.c
 * Audio driver.
 *
 * @authors RoHegbeC
 *
 * @date 2015-Jul-20
 *
 * @copyright
 * (c) 2015 Copyright Digilent Incorporated
 * All Rights Reserved
 *
 * This program is free software; distributed under the terms of BSD 3-clause
 * license ("Revised BSD License", "New BSD License", or "Modified BSD License")
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name(s) of the above-listed copyright holder(s) nor the names
 *    of its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 * @desciption
 *
 * This program was initially developed to be run from within the BRAM. It is
 * constructed to run in a polling mode, in which the program poles the Empty and
 * Full signals of the two FIFO's which are implemented in the audio I2S VHDL core.
 * In order to have a continuous and stable Sound both when recording and playing
 * the user must ensure that DDR cache is enabled. This is only mandatory when the
 * program is loaded in to the DDR, if the program is stored in the BRAM then
 * the cache is not mandatory. It contains different function which enables the
 * I2S VHDL core to be use either in streaming mode in association with the DMA and
 * also some new functions which allows the generation of a sinus wave and a compare
 * function which will compare two waves in table form
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date     Changes
 * ----- ------------ ----------- -----------------------------------------------
 * 1.01  RoHegbeC 2015-Jul-20 Added functions
 * 1.00  RoHegbeC 2014-Oct-30 First release
 *
 * </pre>
 *
 *****************************************************************************/

#ifndef AUDIO_H_
#define AUDIO_H_

#include <math.h>
#include <stdio.h>
#include "xparameters.h"
#include "xil_io.h"
#include "xiic.h"
#include "xil_printf.h"
#include "xil_cache.h"
#include "xstatus.h"
#include "../demo.h"
#include "../dma/dma.h"

/************************** Constant Definitions *****************************/
#define DDR_OFFSET				0x2F000000

// Base Addresses
#define SW_ADDR					XPAR_SWITCHES_0_BASEADDR
#define AUDIO_CTL_ADDR			XPAR_AUDIO_D_AXI_I2S_AUDIO_0_AXI_L_BASEADDR
#define AUDIO_IIC_ADDR			XPAR_AUDIO_AXI_IIC_0_BASEADDR

//SLave address of the ADAU audio controller
#define IIC_SLAVE_ADDR			0x3B
#define DDR_BASEADDR			XPAR_MIG_7SERIES_0_BASEADDR

#define MEM_BASE_ADDR			(DDR_BASEADDR + DDR_OFFSET)

#define CMD_VERBOSE(x,...) 	{ extern volatile sDemo_t Demo; if (Demo.u8Verbose) { xil_printf(x,##__VA_ARGS__); print("\r\n"); } }


//Bit field construction
struct bits {
	u32 u32bit0:1;
	u32 u32bit1:1;
	u32 u32bit2:1;
	u32 u32bit3:1;
	u32 u32bit4:1;
	u32 u32bit5:1;
	u32 u32bit6:1;
	u32 u32bit7:1;
	u32 u32bit8:1;
	u32 u32bit9:1;
	u32 u32bit10:1;
	u32 u32bit11:1;
	u32 u32bit12:1;
	u32 u32bit13:1;
	u32 u32bit14:1;
	u32 u32bit15:1;
	u32 u32bit16:1;
	u32 u32bit17:1;
	u32 u32bit18:1;
	u32 u32bit19:1;
	u32 u32bit20:1;
	u32 u32bit21:1;
	u32 u32bit22:1;
	u32 u32bit23:1;
	u32 u32bit24:1;
	u32 u32bit25:1;
	u32 u32bit26:1;
	u32 u32bit27:1;
	u32 u32bit28:1;
	u32 u32bit29:1;
	u32 u32bit30:1;
	u32 u32bit31:1;
};

union ubitField{
	u8 rgu8[4];
	u32 l;
	struct bits bit;
};


// I2S Status Register Flags
enum i2sStatusFlags {
	TX_FIFO_EMPTY				= 0,
	TX_FIFO_FULL				= 1,
	RX_FIFO_EMPTY				= 16,
	RX_FIFO_FULL				= 17
};

// I2S Fifo Control Register Bits
enum i2sFifoControlBits {
	TX_FIFO_WR_EN				= 0,
	RX_FIFO_RD_EN				= 1,
	TX_FIFO_RST					= 30,
	RX_FIFO_RST					= 31
};

// I2S Fifo Transfer Control Register Bits
enum i2sFifoTransferControlBits {
	TX_RS						= 0,
	RX_RS						= 1
};

// I2S CLK control register
enum i2sClockControlBits {
	SAMPLING_RATE_BIT0			= 0,
	SAMPLING_RATE_BIT1			= 1,
	SAMPLING_RATE_BIT2			= 2,
	SAMPLING_RATE_BIT3			= 3,
	MASTER_MODE_ENABLE      	= 16,
};

//Audio controller registers

//Audio controller registers
enum i2sRegisters {
	I2S_RESET_REG				= AUDIO_CTL_ADDR,
	I2S_TRANSFER_CONTROL_REG	= AUDIO_CTL_ADDR + 0x04,
	I2S_FIFO_CONTROL_REG      	= AUDIO_CTL_ADDR + 0x08,
	I2S_DATA_IN_REG         	= AUDIO_CTL_ADDR + 0x0c,
	I2S_DATA_OUT_REG          	= AUDIO_CTL_ADDR + 0x10,
	I2S_STATUS_REG           	= AUDIO_CTL_ADDR + 0x14,
	I2S_CLOCK_CONTROL_REG     	= AUDIO_CTL_ADDR + 0x18,
	I2S_PERIOD_COUNT_REG       	= AUDIO_CTL_ADDR + 0x1C,
	I2S_STREAM_CONTROL_REG     	= AUDIO_CTL_ADDR + 0x20
};


//ADAU internal register addresses
enum adauRegisterAdresses {
	R0_CLOCK_CONTROL								= 0x00,
	R1_PLL_CONTROL 									= 0x02,
	R2_DIGITAL_MIC_JACK_DETECTION_CONTROL 			= 0x08,
	R3_RECORD_POWER_MANAGEMENT						= 0x09,
	R4_RECORD_MIXER_LEFT_CONTROL_0 					= 0x0A,
	R5_RECORD_MIXER_LEFT_CONTROL_1 					= 0x0B,
	R6_RECORD_MIXER_RIGHT_CONTROL_0 				= 0x0C,
	R7_RECORD_MIXER_RIGHT_CONTROL_1 				= 0x0D,
	R8_LEFT_DIFFERENTIAL_INPUT_VOLUME_CONTROL 		= 0x0E,
	R9_RIGHT_DIFFERENTIAL_INPUT_VOLUME_CONTROL 		= 0x0F,
	R10_RECORD_MICROPHONE_BIAS_CONTROL 				= 0x10,
	R11_ALC_CONTROL_0								= 0x11,
	R12_ALC_CONTROL_1								= 0x12,
	R13_ALC_CONTROL_2								= 0x13,
	R14_ALC_CONTROL_3								= 0x14,
	R15_SERIAL_PORT_CONTROL_0 						= 0x15,
	R16_SERIAL_PORT_CONTROL_1 						= 0x16,
	R17_CONVERTER_CONTROL_0 						= 0x17,
	R18_CONVERTER_CONTROL_1 						= 0x18,
	R19_ADC_CONTROL									= 0x19,
	R20_LEFT_INPUT_DIGITAL_VOLUME 					= 0x1A,
	R21_RIGHT_INPUT_DIGITAL_VOLUME 					= 0x1B,
	R22_PLAYBACK_MIXER_LEFT_CONTROL_0 				= 0x1C,
	R23_PLAYBACK_MIXER_LEFT_CONTROL_1 				= 0x1D,
	R24_PLAYBACK_MIXER_RIGHT_CONTROL_0 				= 0x1E,
	R25_PLAYBACK_MIXER_RIGHT_CONTROL_1 				= 0x1F,
	R26_PLAYBACK_LR_MIXER_LEFT_LINE_OUTPUT_CONTROL 	= 0x20,
	R27_PLAYBACK_LR_MIXER_RIGHT_LINE_OUTPUT_CONTROL = 0x21,
	R28_PLAYBACK_LR_MIXER_MONO_OUTPUT_CONTROL 		= 0x22,
	R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL 		= 0x23,
	R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL 	= 0x24,
	R31_PLAYBACK_LINE_OUTPUT_LEFT_VOLUME_CONTROL 	= 0x25,
	R32_PLAYBACK_LINE_OUTPUT_RIGHT_VOLUME_CONTROL 	= 0x26,
	R33_PLAYBACK_MONO_OUTPUT_CONTROL 				= 0x27,
	R34_PLAYBACK_POP_CLICK_SUPPRESSION 				= 0x28,
	R35_PLAYBACK_POWER_MANAGEMENT 					= 0x29,
	R36_DAC_CONTROL_0 								= 0x2A,
	R37_DAC_CONTROL_1 								= 0x2B,
	R38_DAC_CONTROL_2 								= 0x2C,
	R39_SERIAL_PORT_PAD_CONTROL 					= 0x2D,
	R40_CONTROL_PORT_PAD_CONTROL_0 					= 0x2F,
	R41_CONTROL_PORT_PAD_CONTROL_1 					= 0x30,
	R42_JACK_DETECT_PIN_CONTROL 					= 0x31,
	R67_DEJITTER_CONTROL 							= 0x36,
	R58_SERIAL_INPUT_ROUTE_CONTROL					= 0xF2,
	R59_SERIAL_OUTPUT_ROUTE_CONTROL					= 0xF3,
	R60_SERIAL_DATA_GPIO_CONGIURATION				= 0xF4,
	R61_DSP_ENABLE									= 0xF5,
	R62_DSP_RUN										= 0xF6,
	R63_DSP_SLEW_MODES								= 0xF7,
	R64_SERIAL_PORT_SAMPLING_RATE 					= 0xF8,
	R65_CLOCK_ENABLE_0 								= 0xF9,
	R66_CLOCK_ENABLE_1 								= 0xFA
};

/************************** Variable Definitions *****************************/

// general reg
extern volatile sDemo_t Demo;

/************************** Function Definitions *****************************/

XStatus fnAudioWriteToReg(u8 u8RegAddr, u8 u8Data);
XStatus fnAudioReadFromReg(u8 u8RegAddr, u8 *u8RxData);
XStatus fnAudioPllConfig();
XStatus fnAudioStartupConfig ();
XStatus fnInitAudio();
u32 fnAudioGenSin(u32 *pu32SinArray, u32 u32NrSamples, u32 u32SinFreq, u32 u32SamplingFreq, u32 u32Amplitude);
//void fnAudioMemDump(u32 *pu32AudioSampleArray, u32 u32StartIndex, u32 u32NrSamples, float fDivFact, float fMulFact, int iSubFact, int iAddFact);
XStatus fnAudioCompare(u32 *pu32AudioSampleArrayPlay, u32 *pu32AudioSampleArrayRec,
		u32 u32StartIndex, u32 u32NrSamples, float u32Tolerance, u32 u32NrSamplesInPeriod,
		float fDivFact, float fMulFact, int iSubFact, int iAddFact);
void fnAudioRecord(u32 *pu32AudioSampleArray, u32 u32NrSamples);
void fnAudioPlay(u32 *pu32AudioSampleArray, u32 u32NrSamples);
void fnAudioPlayAndRecord(u32 *pu32AudioSampleArrayPlay, u32 *pu32AudioSampleArrayRec, u32 u32NrSamplesInPeriod, u32 u32NrSamples);
void fnAudioStreamRecord(XAxiDma AxiDma, u32 u32NrSamples);
void fnAudioStreamPlay(XAxiDma AxiDma, u32 u32NrSamples);
void fnSetLineInput();
void fnSetLineOutput();
void fnSetMicInput();
void fnSetHpOutput();
#endif /* AUDIO_H_ */
