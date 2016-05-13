/******************************************************************************
 * @file demo.c
 * This is the 'Out of Box' demo application for the Digilent Genesys 2 board.
 * It runs as a standalone app on an embedded Microblaze system.
 *
 * @authors Elod Gyorgy, Mihaita Nagy
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
 * User I/O Demo:	This demo shows a scanning light bar on the LEDs. Any of
 * 					the slide switches in the UP position overrides the scanning
 * 					bar and forces the LED above ON.
 *
 * Audio Demo:		The audio demo records a 5 second sample from microphone
 * 					(J12) or line in (J13) and plays it back on headphone out
 * 					(J10) or line out (J11). Record and playback is started by
 * 					pushbuttons:
 * 					BTNU: record from microphone
 * 					BTNR: record from line in
 * 					BTND: playback on headphone
 * 					BTNL: playback on line out
 * 					For example, with the push of BTNU this demo records 5
 * 					seconds of audio data from the MIC (J6) input. Consequently
 * 					by pressing BTND the demo plays on the HPH OUT (J4) the
 * 					recorded samples.
 *
 * Ethernet Demo:	The demo uses the lwIP stack to implement an pingable echo
 * 					server. The on-board EEPROM is read to find out the MAC
 * 					address of the board. The network interface initializes as
 * 					soon as a cable is plugged in and link is detected. The DHCP
 * 					client will	try to get an IP, which will be shown on both the
 * 					on-board OLED (DISP1) and the terminal.
 * 					Once the IP address is displayed, the server replies to ping
 * 					requests on port 7.
 *
 * Video Demo: 		A static image is loaded from the Flash and displayed on
 * 					all the video outputs (VGA, DVI, DisplayPort) using the DDR3
 * 					as frame buffer.
 *
 * Power Demo:		Power monitor circuits are being read and current readings
 * 					displayed on the OLED.
 *
 * XADC Demo:		The XADC is set up to monitor the internal FPGA temperature,
 * 					VCCINT voltage and VCCAUX voltage. The XADC is periodically
 * 					read and the readings are displayed on the OLED and the
 * 					terminal. The user temperature alert feature is used to turn
 * 					the fan ON and OFF when upper and lower limits are reached.
 *
 * OLED Demo:		The on-board OLED shows the Digilent logo on power-up. After
 * 					initialization is completed it switches to display various
 * 					other information on several pages. Advance between pages
 * 					by pressing BTNC.
 * 					Page 1: FPGA internal temperature, VCCINT, VCCAUX voltages
 * 					Page 2: Current readings for VCC1V0, VCC1V5, VCC1V8
 * 					Page 3: Current readings for VCC3V3, VCC5V0
 * 					Page 4: MAC address and IP address
 * 					The OLED driver uses the GPIO and SPI IPs to talk to the
 * 					display.
 *
 * UART Demo:		Status messages and XADC readings are shown on the terminal.
 *
 * @note
 *
 * UART setup:		In order to successfully communicate with this demo you
 * 					must set your terminal to 115200 Baud, 8 data bits, 1 stop
 * 					bit, no parity.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date        Changes
 * ----- ------------ ----------- --------------------------------------------
 * 2.00  Elod Gyorgy  2015-Jul-20 Updates/changes for Genesys 2
 * 1.01  Elod Gyorgy  2015-Jan-21 Warm-reset fix.
 * 1.00  Mihaita Nagy 2015-Jan-15 First release
 *  	 Elod Gyorgy
 *
 * </pre>
 *
 *****************************************************************************/

/***************************** Include Files *********************************/
#include "demo.h"
#include <string.h>
#include "xil_cache.h"
#include "xuartns550_l.h"

#include "audio/audio.h"
#include "dma/dma.h"
#include "dp/dptx.h"
#include "intc/intc.h"
#include "oled/oled.h"
#include "userio/userio.h"
#include "video/video.h"
#include "xadc/xadc.h"
#include "eth/eth.h"
#include "qspi/qspi.h"
#include "xtmrctr.h"
#include "leds/leds.h"
#include "INA/ina.h"

/************************** Variable Definitions *****************************/
static XGpio sUserIO;
//static XIic sIic;
static XGpio sOledGpio;
static XSpi sOledSpi;
static XAxiDma sAxiDma;
static XAxiVdma sVdma; //Video DMA
static XSysMon sXADC;
static XIntc sIntc;
static XDptx sSDptx;
static XTmrCtr sSTmr;
static XSpi sFlashSpi;
static XTmrCtr sLedTmr;

// This variable holds the demo related settings
volatile sDemo_t Demo = {TRUE, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/************************** Constant Definitions *****************************/
// UART baud rate
#define UART_BAUD 				115200

#define DELAY_TMR_CHANNEL		1
// Audio constants
// Number of seconds to record/playback
#define NR_SEC_TO_REC_PLAY		5

// ADC/DAC sampling rate in Hz
#define AUDIO_SAMPLING_RATE		96000

// Number of samples to record/playback
#define NR_AUDIO_SAMPLES		(NR_SEC_TO_REC_PLAY*AUDIO_SAMPLING_RATE)

/// Microblaze system clock frequency [kHz]
static const u32 kdwUBlazeFreqkHz	= (XPAR_MICROBLAZE_CORE_CLOCK_FREQ_HZ / 1000);

// Interrupt vector table
const ivt_t ivt[] = {
	//User I/O (buttons, switches, LEDs)
	{XPAR_MICROBLAZE_0_AXI_INTC_USERIO_IP2INTC_IRPT_INTR, (XInterruptHandler)fnUserIOIsr, &sUserIO},
	//OLED SPI Interrupt handler
	{XPAR_MICROBLAZE_0_AXI_INTC_OLED_AXI_QUAD_SPI_0_IP2INTC_IRPT_INTR, (XInterruptHandler)XSpi_InterruptHandler, &sOledSpi},
	//DMA Stream to MemoryMap Interrupt handler
	{XPAR_MICROBLAZE_0_AXI_INTC_AXI_DMA_0_S2MM_INTROUT_INTR, (XInterruptHandler)fnS2MMInterruptHandler, &sAxiDma},
	//DMA MemoryMap to Stream Interrupt handler
	{XPAR_MICROBLAZE_0_AXI_INTC_AXI_DMA_0_MM2S_INTROUT_INTR, (XInterruptHandler)fnMM2SInterruptHandler, &sAxiDma},
	//VDMA
	{XPAR_MICROBLAZE_0_AXI_INTC_VIDEO_AXI_VDMA_0_MM2S_INTROUT_INTR, (XInterruptHandler)XAxiVdma_ReadIntrHandler, &sVdma},
	//Flash QSPI Interrupt handler
	{XPAR_MICROBLAZE_0_AXI_INTC_QSPI_FLASH_IP2INTC_IRPT_INTR, (XInterruptHandler)XSpi_InterruptHandler, &sFlashSpi},
	//DisplayPort HPD
	{XPAR_INTC_0_DP_0_VEC_ID, (XInterruptHandler)XDptx_HpdInterruptHandler, &sSDptx},
	//LED
	{XPAR_MICROBLAZE_0_AXI_INTC_LED_TIMER_INTERRUPT_INTR, (XInterruptHandler)fnLedsTimerIntrHandler, &sLedTmr},
	//ETH_INT_B PHY interrupt
	{XPAR_MICROBLAZE_0_AXI_INTC_SYSTEM_ETH_INTB_INTR, (XInterruptHandler)fnEthPHYIsr, NULL}
};

/************************** Function Prototypes ******************************/
void Asserted (const char *File, int Line);

/************************** Function Definitions *****************************/
XStatus main() {

	XStatus Status, CustomStatus = XST_FAILURE;
	char ScreenBuf[64];
	u8 fOledDisp = 0;

	// Enabling caches
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_DCacheDisable(); //AXI Quad SPI is not compatible with the data cache
#endif

	// Empty assert function. We can put a breakpoint there to look for asserts
	Xil_AssertSetCallback(Asserted);

    // Initialize UART
    XUartNs550_SetBaud(XPAR_AXI_UART16550_0_BASEADDR,
    		XPAR_XUARTNS550_CLOCK_HZ, UART_BAUD);
    XUartNs550_SetLineControlReg(XPAR_AXI_UART16550_0_BASEADDR,
    		XUN_LCR_8_DATA_BITS);

    xil_printf("\033[H\033[J"); //Clear the terminal
    xil_printf("\r\nInitializing demo: ");

    // Initialize the generic timer
    Status = XTmrCtr_Initialize(&sSTmr, XPAR_GENERIC_TIMER_0_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
    	xil_printf("Error initializing generic timer\r\n");
    	return XST_FAILURE;
    }

	// Initialize the interrupt controller
	Status = fnInitInterruptController(&sIntc);
	if(Status != XST_SUCCESS) {
		xil_printf("Error initializing interrupts");
		return XST_FAILURE;
	}

	//Initialize Flash quad spi controller
	Status = init_qspi(&sFlashSpi);
	if(Status != XST_SUCCESS) {
		xil_printf("Error initializing QSpi");
		return XST_FAILURE;
	}

	// Initialize the OLED driver
	Status = fnOledDriverInit(&sOledSpi, &sOledGpio);
	if(Status != XST_SUCCESS) {
		xil_printf("OLED driver initialization ERROR");
		return XST_FAILURE;
	}

	// Initialize XADC
	Status = fnXADCInit(&sXADC);
	if(Status != XST_SUCCESS) {
		xil_printf("XADC initialization ERROR");
		return XST_FAILURE;
	}

	// Initialize Leds Timer
	Status = fnInitTimerLeds();
	if(Status != XST_SUCCESS) {
		xil_printf("Leds Timer initialization ERROR");
		return XST_FAILURE;
	}

    // Initialize User I/O driver
    Status = fnInitUserIO(&sUserIO);
    if(Status != XST_SUCCESS) {
    	xil_printf("User I/O ERROR");
    	return XST_FAILURE;
    }

	//Initialize DMA
	Status = fnConfigDma(&sAxiDma);
	if(Status != XST_SUCCESS) {
		xil_printf("DMA configuration ERROR");
		return XST_FAILURE;
	}

	//Initialize Audio I2S
	Status = fnInitAudio();
	if(Status != XST_SUCCESS) {
		xil_printf("Audio initializing ERROR");
		return XST_FAILURE;
	}

	// Initialize Ethernet PHY
	Status = fnPhyInit();
	if (Status != XST_SUCCESS)
	{
		xil_printf("Ethernet PHY init ERROR\r\n");
		return XST_FAILURE;
	}

	// Initialize DP Tx
	CustomStatus = DptxInit(&sSDptx, XPAR_XDP_0_DEVICE_ID);

	// Enable all interrupts in our interrupt vector table
	// Make sure all driver instances using interrupts are initialized first
	fnEnableInterrupts(&sIntc, &ivt[0], sizeof(ivt)/sizeof(ivt[0]));

	// Setup OLED driver options after interrupts are initialized
	Status = fnOledDriverOptions(&sOledSpi);
	if(Status != XST_SUCCESS) {
		xil_printf("OLED driver options setup ERROR");
		return XST_FAILURE;
	}
	else {
		Status = fnOledDisplayInit(&sOledSpi, &sOledGpio);
		if(Status != XST_SUCCESS) {
			xil_printf("OLED display initialization ERROR");
			return XST_FAILURE;
		}
	}

	//Initialize Video, needs interrupts
    Status = VideoInit(&sVdma, &sFlashSpi);
    if (Status != XST_SUCCESS) {
    	xil_printf("Video init ERROR");
    	return XST_FAILURE;
    }

    //This needs to be called after interrupts are enabled
    Status = VideoShowPattern(&sVdma);
	if(Status != XST_SUCCESS) {
		xil_printf("Video pattern ERROR");
		return XST_FAILURE;
	}

	// Enable mouse pointer
	fnPS2MouseShow(TRUE);

	//Wait a bit for video sync
	DemoWaitMs(2000);

	// Check the DPTX HPD status after the interrupts are enabled
	CustomStatus |= Demo.sbLinkFailed;
	if (CustomStatus != XST_SUCCESS)
	{
		xil_printf("DisplayPort unconnected\r\n");
		//return XST_FAILURE;
	}

	macAddress_t mac = {};
	{ //Read MAC and start Ethernet to get DHCP; interrupts must be enabled
		Status = SpiReadOTP(&sFlashSpi, MAC_OTP_ADDR, (u8*)&mac, sizeof(mac));
		if (XST_SUCCESS != Status) {
			xil_printf("Ethernet init ERROR. Error reading MAC from QSPI\r\n");
			return XST_FAILURE;
		}
		xil_printf("Ethernet MAC found in Flash OTP: %02x-%02x-%02x-%02x-%02x-%02x\r\n", mac.rgbMac[0],mac.rgbMac[1],mac.rgbMac[2],mac.rgbMac[3],mac.rgbMac[4],mac.rgbMac[5]);

		//Check MAC for Digilent OUI
		if (mac.rgbMac[0] != (u8)(MAC_OUI_DIGILENT >> 16) ||
			mac.rgbMac[1] != (u8)(MAC_OUI_DIGILENT >> 8) ||
			mac.rgbMac[2] != (u8)(MAC_OUI_DIGILENT))
		{
			xil_printf("Ethernet init ERROR. Non-Digilent MAC found in Flash OTP: %02x-%02x-%02x-%02x-%02x-%02x\r\n", mac.rgbMac[0],mac.rgbMac[1],mac.rgbMac[2],mac.rgbMac[3],mac.rgbMac[4],mac.rgbMac[5]);
			return XST_FAILURE;
		}
	}

	int temp = 0, vccint = 0, vccaux = 0;
	int v1=0, v1v5=0, v1v8=0, v3v3=0, v5=0;

	xil_printf ("\r\nStarting Demo");

    while(1) {

    	u32 dwXADCStatus;

    	xil_printf("\033[H\033[J"); //Clear the terminal
    	xil_printf("----------------------------------------------------------\r\n");
		xil_printf("Genesys 2 demo application\r\n");
		xil_printf("----------------------------------------------------------\r\n");

		if ((dwXADCStatus = XSysMon_GetStatus(&sXADC)) & XSM_SR_EOS_MASK) {
			// If XADC reads 0, ignore it
			int read = (int)XSysMon_RawToTemperature(XSysMon_GetAdcData(&sXADC, XSM_CH_TEMP));
			if (read != -273) temp = read;
			read = (int)(XSysMon_RawToVoltage(XSysMon_GetAdcData(&sXADC, XSM_CH_VCCINT)) * 1000);
			if (read != 0) vccint = read;
			read = (int)(XSysMon_RawToVoltage(XSysMon_GetAdcData(&sXADC, XSM_CH_VCCAUX)) * 1000);
			if (read != 0) vccaux = read;

			switch (fOledDisp)
			{
			case 0:
				// Formating the XADC data for the OLED display
				sprintf(&ScreenBuf[0],  "FPGA temp:%4d C", temp);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN], "VCCINT:  %4d mV", vccint);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*2], "VCCAUX:  %4d mV", vccaux);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*3],  "  Press BTNC  ");
				fnOledStringToDisplay(&sOledSpi, &sOledGpio, &ScreenBuf[0]);
				break;
			case 1:
				//Read Currents on 1v0, 1v5, 1v8
				read = (int)fnGetINACurrent("vcc1v0");
				if (read != 0) v1 = read;
				read = (int)fnGetINACurrent("vcc1v5");
				if (read != 0) v1v5 = read;
				read = (int)fnGetINACurrent("vcc1v8");
				if (read != 0) v1v8 = read;

				sprintf(&ScreenBuf[0],  "BUS 1V: %5d mA", v1);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN],  "BUS 1V5:%5d mA", v1v5);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*2],  "BUS 1V8:%5d mA", v1v8);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*3],  "  Press BTNC  ");
				fnOledStringToDisplay(&sOledSpi, &sOledGpio, &ScreenBuf[0]);
				break;
			case 2:
				//Read Currents on 5v0 and 3v3
				read = (int)fnGetINACurrent("vcc5v0");
				if (read != 0) v5 = read;
				read = (int)fnGetINACurrent("vcc3v3");
				if (read != 0) v3v3 = read;

				sprintf(&ScreenBuf[0],  "BUS 5V: %5d mA", v5);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN],  "BUS 3V3:%5d mA", v3v3);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*2],  "                ");
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*3],  "  Press BTNC  ");
				fnOledStringToDisplay(&sOledSpi, &sOledGpio, &ScreenBuf[0]);
				break;
			case 3:
				sprintf(&ScreenBuf[0], "IP address:     ");
				sprint_ip(&ScreenBuf[OLED_MAX_LINE_LEN]);
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*2],  "                ");
				sprintf(&ScreenBuf[OLED_MAX_LINE_LEN*3],  "  Press BTNC  ");
				fnOledStringToDisplay(&sOledSpi, &sOledGpio, &ScreenBuf[0]);
				break;
			default: break;
			}


			xil_printf("\r\nInternal temperature: %d C", temp);
			xil_printf("\r\nVCCINT voltage: %d mV", vccint);
			xil_printf("\r\nVCCAUX voltage: %d mV", vccaux);
    	}

    	// Report Ethernet link status
    	xil_printf("\r\nEthernet Link ");
    	if (Demo.fLinkStatus) {
    		xil_printf("up. Speed %dMbps. IP ", Demo.linkSpeed); print_echo_ip();
		}
    	else {
    		xil_printf("down.");
    	}

    	// Report Ethernet link status
		if (Demo.fLinkEvent) {

			if (Demo.fLinkStatus) {
				fnStartEth(&sIntc, &mac);
			}
			else {
				fnCloseEth();
			}
			Demo.fLinkEvent = 0;
		}

		// Checking the DMA S2MM event flag
		if (Demo.fDmaS2MMEvent)
		{
			xil_printf("\r\nRecording Done...");

			// Disable Stream function to send data (S2MM)
			Xil_Out32(I2S_STREAM_CONTROL_REG, 0x00000000);
			Xil_Out32(I2S_TRANSFER_CONTROL_REG, 0x00000000);
			//Flush cache
			microblaze_flush_dcache();
			microblaze_invalidate_dcache();
			// Reset S2MM event and record flag
			Demo.fDmaS2MMEvent = 0;
			Demo.fAudioRecord = 0;
		}

		// Checking the DMA MM2S event flag
		if (Demo.fDmaMM2SEvent)
		{
			xil_printf("\r\nPlayback Done...");

			// Disable Stream function to send data (S2MM)
			Xil_Out32(I2S_STREAM_CONTROL_REG, 0x00000000);
			Xil_Out32(I2S_TRANSFER_CONTROL_REG, 0x00000000);
			//Flush cache
			microblaze_flush_dcache();
			microblaze_invalidate_dcache();
			// Reset MM2S event and playback flag
			Demo.fDmaMM2SEvent = 0;
			Demo.fAudioPlayback = 0;
		}

		// Checking the DMA Error event flag
		if (Demo.fDmaError)
		{
			xil_printf("\r\nDma Error...");
			xil_printf("\r\nDma Reset...");
			Demo.fDmaError = 0;
			Demo.fAudioPlayback = 0;
			Demo.fAudioRecord = 0;
		}

		// Checking the btn change event
		if(Demo.fUserIOEvent) {

			switch(Demo.chBtn) {
				case 'u':
					if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
					{
						xil_printf("\r\nStart Recording...");
						fnSetMicInput();
						fnAudioStreamRecord(sAxiDma,NR_AUDIO_SAMPLES);
						Demo.fAudioRecord = 1;
					}
					else
					{
						if (Demo.fAudioRecord)
						{
							xil_printf("\r\nStill Recording...");
						}
						else
						{
							xil_printf("\r\nStill Playing back...");
						}
					}
					break;
				case 'd':
					if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
					{
						xil_printf("\r\nStart Playback...");
						fnSetHpOutput();
						fnAudioStreamPlay(sAxiDma,NR_AUDIO_SAMPLES);
						Demo.fAudioPlayback = 1;
					}
					else
					{
						if (Demo.fAudioRecord)
						{
							xil_printf("\r\nStill Recording...");
						}
						else
						{
							xil_printf("\r\nStill Playing back...");
						}
					}
					break;
				case 'r':
					if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
					{
						xil_printf("\r\nStart Recording...");
						fnSetLineInput();
						fnAudioStreamRecord(sAxiDma,NR_AUDIO_SAMPLES);
						Demo.fAudioRecord = 1;
					}
					else
					{
						if (Demo.fAudioRecord)
						{
							xil_printf("\r\nStill Recording...");
						}
						else
						{
							xil_printf("\r\nStill Playing back...");
						}
					}
					break;
				case 'l':
					if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
					{
						xil_printf("\r\nStart Playback...");
						fnSetLineOutput();
						fnAudioStreamPlay(sAxiDma,NR_AUDIO_SAMPLES);
						Demo.fAudioPlayback = 1;
					}
					else
					{
						if (Demo.fAudioRecord)
						{
							xil_printf("\r\nStill Recording...");
						}
						else
						{
							xil_printf("\r\nStill Playing back...");
						}
					}
					break;
				case 'c':
					if (Demo.fDebounceEvent == 1)
					{
						fOledDisp++;
						if (fOledDisp>3)
						{
							fOledDisp = 0;
						}
						Demo.fDebounceEvent = 0;
					}
					break;
				default:
					break;
			}

			// Reset the user I/O flag
			Demo.chBtn = 0;
			Demo.fUserIOEvent = 0;
		}

    	fflush(stdout);
    	DemoWaitMs(500);
    }

    // Disabling caches
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_ICacheDisable();
#endif

    return XST_SUCCESS;
}

//! The assert function is called when something goes wrong and execution cannot continue
void Asserted (const char *File, int Line) {
	xil_printf("\r\nAssertion at line %d in %s.", Line, File);
	while (1) ; //Stay here
}

/// Millisecond delay routine
/// @note: Use this instead of the built-in delay
void DemoWaitMs(u32 p_dwMilliSeconds)
{
	XTmrCtr* pSTmr = &sSTmr;
	u32 dwTmrVal = 0;
	u32 dwNumTicks = (p_dwMilliSeconds * kdwUBlazeFreqkHz);

	/// Reset the timer
	XTmrCtr_Reset(pSTmr, DELAY_TMR_CHANNEL);
	XTmrCtr_Start(pSTmr, DELAY_TMR_CHANNEL);

	/// Wait the specified number of us
	do
	{
		dwTmrVal = XTmrCtr_GetValue(pSTmr, DELAY_TMR_CHANNEL);
	}
	while (dwTmrVal < dwNumTicks);

	XTmrCtr_Stop(pSTmr, DELAY_TMR_CHANNEL);
}

