/******************************************************************************
 * @file eth.h
 * Ethernet driver.
 *
 * @author Elod Gyorgy
 *
 * @date 2015-Jan-15
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
 * Contains PHY and MAC initialization functions and PHY interrupt service
 * routine.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date     Changes
 * ----- ------------ ----------- -----------------------------------------------
 * 1.00  Elod Gyorgy  2015-Jan-15 First release
 *
 * </pre>
 *
 *****************************************************************************/

#ifndef ETH_H_
#define ETH_H_

#include "xstatus.h"
#include "xintc.h"
#include "lwip/ip_addr.h"

/*
 * Uncomment if a wait is desired after starting the dhcp client
 * If not defined the init function will return with IP 0.0.0.0 set, and
 * a DHCP-assigned IP will be set on the interface when it arrives.
 * If defined, the init function will block with a timeout to wait for
 * a DHCP reply. If the timeout expires a link-local IP is set.
 */
//#define DHCP_WAIT_FOR_IP_WITH_TIMEOUT

typedef struct
{
	u8 rgbMac[6];
} macAddress_t;

XStatus fnPhyInit();
XStatus fnStartEth(XIntc *psIntc,  const macAddress_t *mac);
XStatus fnCloseEth();
int transfer_data();

void fnPrintIpSettings();
int fnPhyTest();
void fnProcessEthData();
void print_ip(char *msg, struct ip_addr *ip) ;
void print_echo_ip();
void sprint_ip(char *msg);

// Timer ISR
void xadapter_timer_handler(void *p);

#endif /* ETH_H_ */
