/******************************************************************************
 * @file leds.c
 * This is a power INA219 validation file
 *
 * @authors Cristian Ignat
 *
 * @date 2015-Apr-04
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
 * @note
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          	Date        Changes
 * ----- ------------ ----------- --------------------------------------------
 * 1.00  Cristian Ignat 2015-Apr-04 First release
 *
 * </pre>
 *
 *****************************************************************************/

/***************************** Include Files *********************************/

#include "leds.h"
#include "../demo.h"

// Defineds
#define TIMER_CNTR_0		0
#define LEDS_NUMBER 		8
#define LIT_LEDS_NUMBER 	5

#define TIMER_INTERRUPT_PERIOD_MS 	4
#define SHIFT_LEDS_PERIOD_MS		100

#define LEDS_GPIO_BASEADDR 		XPAR_USERIO_BASEADDR + 0x08

#define BTN_GPIO_BASEADDR		XPAR_USERIO_BASEADDR

#define MAX_TMR_SHIFT_LEDS		SHIFT_LEDS_PERIOD_MS / TIMER_INTERRUPT_PERIOD_MS
#define TIMER_PERIOD 			(XPAR_LED_TIMER_CLOCK_FREQ_HZ * TIMER_INTERRUPT_PERIOD_MS) / 1000

#define BTN_DEBOUNCE_TMR 4

// Variables
char queue1[LEDS_NUMBER];
char queue2[LEDS_NUMBER];

extern sDemo_t Demo;


/******************************************************************************
 * This function is used to shift the queues.
 * queue1 and queue2 arrays are used
 * Shifting sequence example for 8 used leds:
 * 		0  1  2  3  4  -1 -1 -1		- queue1 initial values
 * 		→  →  →  →  →  →  →  →
 *		↑			  		 ↓
 *		←  ←  ←  ←  ←  ←  ←  ←
 *		-1 -1 -1 -1 -1 -1 -1 -1		- queue2 initial values
 * @param	none
 *
 * @return	none
 *****************************************************************************/
void ShiftQueues()
{
	int i;
	char var;
	var = queue1[LEDS_NUMBER - 1];

	for(i = LEDS_NUMBER - 1; i > 0; i--)
	{
		queue1[i] = queue1[i - 1];
	}
	queue1[0] = queue2[0];
	for(i = 0; i < LEDS_NUMBER - 1; i++)
	{
		queue2[i] = queue2[i + 1];
	}
	queue2[LEDS_NUMBER - 1] = var;
}


/******************************************************************************
 * This function is the handler which performs processing for the TMR driver.
 *
 * @param	baseaddr_p is a reference passed to the base address
 *
 * @return	none
 *****************************************************************************/
void fnLedsTimerIntrHandler(void * baseaddr_p)
{
	u32 TCSR;
	static char Leds = 0;

	static char Tmr_PWM = 0;
	static int Tmr_shift_leds = 0;
	char i;

	u32 btn_data;

	//Test if FAN Timer interrupt occured
	TCSR = Xil_In32 (XPAR_LED_TIMER_BASEADDR);

	if (TCSR & XTC_CSR_INT_OCCURED_MASK)
	{
		Tmr_PWM = (Tmr_PWM + 1) % LIT_LEDS_NUMBER;
		Tmr_shift_leds ++;

		if(Tmr_shift_leds >= MAX_TMR_SHIFT_LEDS)
		{
			Tmr_shift_leds = 0;
			ShiftQueues();
		}

		//Create the debounce code for the center button
		if (Demo.fUserIOEvent)
		{
			//Check if the center button is pressed
			btn_data = Xil_In32(BTN_GPIO_BASEADDR);

			if ((Demo.fBtnTimerCount == BTN_DEBOUNCE_TMR) && ((btn_data & 0x1000) == 0x1000))
			{
				Demo.fBtnTimerCount = BTN_DEBOUNCE_TMR;

			}
			else if ((btn_data & 0x1000) == 0x1000)
			{
				Demo.fBtnTimerCount++;
			}
			else if (Demo.fBtnTimerCount == BTN_DEBOUNCE_TMR)
			{
				Demo.fDebounceEvent = 1;
				Demo.fBtnTimerCount = 0;
			}
			else
			{
				Demo.fBtnTimerCount = 0;
			}
		}

		Leds = 0;
		for(i = 0; i < 8; i++)
		{
			if(queue1[i] > queue2[i] && Tmr_PWM <= queue1[i])
				Leds = Leds | (1 << i);
			else if(queue1[i] < queue2[i] && Tmr_PWM <= queue2[i])
				Leds = Leds | (1 << i);
		}
		// If sw is UP, force that LED to ON, no matter the scanning bar
		Xil_Out8(LEDS_GPIO_BASEADDR, Leds | Demo.bSwitches);

	}
	Xil_Out32 ((XPAR_LED_TIMER_BASEADDR + XTC_TCSR_OFFSET), TCSR);
}


/******************************************************************************
 * This function initializes the TIMER driver and the first leds state.
 *
 * @param	none
 *
 * @return	XST_SUCCESS - Everything went well
 *****************************************************************************/
XStatus fnInitTimerLeds(void)
{
	u32 Tctrl_Reg;
	u8 i;

	for(i = 0; i < LEDS_NUMBER; i++)
	{
		if(i < LIT_LEDS_NUMBER)
			queue1[i] = i;
		else
			queue1[i] = -1;

		queue2[i] = -1;
	}
	
	//Configure LEDS Timer
	//Set the timer interrupt period to TIMER_INTERRUPT_PERIOD_MS ms
	XTmrCtr_WriteReg(XPAR_LED_TIMER_BASEADDR, TIMER_CNTR_0, XTC_TLR_OFFSET, TIMER_PERIOD);

	//Initialize Timer
	Tctrl_Reg = XTmrCtr_ReadReg(XPAR_LED_TIMER_BASEADDR, TIMER_CNTR_0, XTC_TCSR_OFFSET) ;

	Tctrl_Reg |= ( XTC_CSR_ENABLE_ALL_MASK |
				   XTC_CSR_ENABLE_INT_MASK |
				   XTC_CSR_AUTO_RELOAD_MASK |
				   XTC_CSR_DOWN_COUNT_MASK |
				   XTC_CSR_LOAD_MASK);

	XTmrCtr_WriteReg(XPAR_LED_TIMER_BASEADDR, TIMER_CNTR_0, XTC_TCSR_OFFSET, Tctrl_Reg);

	//Clear the Load bit to start the timer
	Tctrl_Reg &= (~XTC_CSR_LOAD_MASK);
	XTmrCtr_WriteReg(XPAR_LED_TIMER_BASEADDR, TIMER_CNTR_0, XTC_TCSR_OFFSET, Tctrl_Reg);

	return XST_SUCCESS;
}

