/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/
#if __MICROBLAZE__ || __PPC__
#include "arch/cc.h"
#include "platform.h"
#include "platform_config.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"
#ifdef STDOUT_IS_16550
#include "xuartns550_l.h"
#endif

#include "lwip/tcp.h"

#if LWIP_DHCP==1
volatile int dhcp_timoutcntr = 24;
void dhcp_fine_tmr();
void dhcp_coarse_tmr();
#endif

/*!
 *	timer_callback is called periodically every 50ms. TCP timer needs to be called every 250ms.
 *	DHCP fine timer every 500ms. DHCP coarse timer every minute.
 */
void
timer_callback()
{
	static unsigned int timer = 0;

	timer++;

	/* echo server data processing */
	fnProcessEthData();

	// 50ms * 5 = 250 ms
	if (timer % 5 == 0)
	{
		tcp_tmr();
	}

#if LWIP_DHCP==1
	// 50 ms * 10 = 500 ms
	if (timer % 10 == 0)
	{
		dhcp_timoutcntr--;
		dhcp_fine_tmr();
	}

	// 50 ms * 1200 = 60000 ms = 1 min
	if (timer % 60000 == 0)
	{
		dhcp_coarse_tmr();
		timer = 0;
	}
#endif

}

void platform_setup_interrupts(XIntc *psIntc)
{
	// Interrupt controller initialized outside
	platform_setup_timer();

#ifdef XPAR_ETHERNET_MAC_IP2INTC_IRPT_MASK
	/* Enable timer and EMAC interrupts in the interrupt controller */
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
#ifdef __MICROBLAZE__
			PLATFORM_TIMER_INTERRUPT_MASK |
#endif
			XPAR_ETHERNET_MAC_IP2INTC_IRPT_MASK);
#endif


#ifdef XPAR_INTC_0_LLTEMAC_0_VEC_ID
#ifdef __MICROBLAZE__
	XIntc_Enable(psIntc, PLATFORM_TIMER_INTERRUPT_INTR);
#endif
	XIntc_Enable(psIntc, XPAR_INTC_0_LLTEMAC_0_VEC_ID);
#endif


#ifdef XPAR_INTC_0_AXIETHERNET_0_VEC_ID
	XIntc_Enable(psIntc, PLATFORM_TIMER_INTERRUPT_INTR);
	XIntc_Enable(psIntc, XPAR_INTC_0_AXIETHERNET_0_VEC_ID);
#endif


#ifdef XPAR_INTC_0_EMACLITE_0_VEC_ID
#ifdef __MICROBLAZE__
	XIntc_Enable(psIntc, PLATFORM_TIMER_INTERRUPT_INTR);
#endif
	XIntc_Enable(psIntc, XPAR_INTC_0_EMACLITE_0_VEC_ID);
#endif


}
#endif
