/******************************************************************************
 * @file eth.c
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

/***************************** Include Files *********************************/
#include "eth.h"
#include "../demo.h"
#include <stdio.h>

#include "xparameters.h"

#include "lwip/netif.h"
#include "netif/xadapter.h"

#include "xaxiethernet.h"	/* defines Axi Ethernet APIs */

#include "platform.h"
#include "platform_config.h"
#ifdef __arm__
#include "xil_printf.h"
#endif

#if LWIP_DHCP==1
#include "lwip/dhcp.h"
#endif

/************************** Constant Definitions *****************************/
#define ETHERNET_MAC_DEVICE_ID XPAR_AXIETHERNET_0_DEVICE_ID

/* Use MII register 1 (MII status register) to detect PHY */
#define PHY_DETECT_REG  1
#define PHY_DETECT_MASK 0x1808
#define PHY_R0_DFT_SPD_1000  0x0040

#define PHY_R0_RESET         0x8000

#define PHY_R0_CTRL_REG	0
#define PHY_R2_PHY_IDENT_REG	2
#define PHY_R3_PHY_IDENT_REG	3
#define PHY_R18_INER	0x12
#define PHY_R19_INSR	0x13

#define PHY_INER_LINK_STATUS_IRPT_MASK 0x0400

#define PHY_R17_SPECIFIC_STATUS_REG	17

#define RTL_8211E_ID	0x732
#define RTL_8211E_MODEL	0x11


/************************** Function Prototypes ******************************/
/* defined by each RAW mode application */
void print_app_header();
void print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw);
int start_application();
int stop_application();
int transfer_data();

/* missing declaration in lwIP */
void lwip_init();

#if LWIP_DHCP==1
extern volatile int dhcp_timoutcntr;
err_t dhcp_start(struct netif *netif);
#endif

/************************** Variable Definitions *****************************/
static XAxiEthernet sAxiEthMac;
static unsigned int PhyAddr = 0;
static u8 fAppStarted = 0;

struct netif server_netif;
struct netif *echo_netif = NULL;

extern sDemo_t Demo;

/************************** Function Definitions *****************************/

/*
 * Initialize Ethernet echo server. Ethernet requires the interrupt controller
 * initialized and ready.
 */
XStatus fnStartEth(XIntc *psIntc,  const macAddress_t *mac)
{
	struct ip_addr ipaddr, netmask, gw;

	// If interface is not initialized yet
	if (echo_netif == NULL)
	{
		xil_printf("\r\nInitializing network interface...");

		echo_netif = &server_netif;

	#if LWIP_DHCP==1
		ipaddr.addr = 0;
		gw.addr = 0;
		netmask.addr = 0;
	#else
		/* initialize IP addresses to be used */
		IP4_ADDR(&ipaddr,  192, 168, 0, 100);
		IP4_ADDR(&netmask, 255, 255, 255,  0);
		IP4_ADDR(&gw,      192, 168,   0,  1);
	#endif

		lwip_init();

		/* Add network interface to the netif_list, and set it as default */
		if (!xemac_add(echo_netif, &ipaddr, &netmask,
							&gw, (unsigned char*)mac,
							PLATFORM_EMAC_BASEADDR)) {
			if (Demo.u8Verbose) xil_printf("Error adding N/W interface\n\r");
			echo_netif = NULL;
			return XST_FAILURE;
		}

		netif_set_default(echo_netif);
	}

	/* enable interrupts */
	platform_setup_interrupts(psIntc);

	/* specify that the network if is up */
	netif_set_up(echo_netif);

#if (LWIP_DHCP==1)
	/* Create a new DHCP client for this interface.
	 * Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
	 * the predefined regular intervals after starting the client.
	 */

	xil_printf("\r\nStarting DHCP client to get an IP address...");

	dhcp_start(echo_netif);
	dhcp_timoutcntr = 30; //timeout in dhcp_finetmr units (500ms*30=15s)

//#ifdef DHCP_WAIT_FOR_IP_WITH_TIMEOUT
	while(((echo_netif->ip_addr.addr) == 0) && (dhcp_timoutcntr > 0))
		xemacif_input(echo_netif);

	if (dhcp_timoutcntr <= 0) {
		if ((echo_netif->ip_addr.addr) == 0) {
			xil_printf("DHCP Timeout\r\n");
			xil_printf("Configuring link-local IP\r\n");
			IP4_ADDR(&(echo_netif->ip_addr),  169, 254,   123, 123);
			IP4_ADDR(&(echo_netif->netmask), 255, 255, 0,  0);
			echo_netif->gw.addr = 0;
		}
	}
//#endif

	ipaddr.addr = echo_netif->ip_addr.addr;
	gw.addr = echo_netif->gw.addr;
	netmask.addr = echo_netif->netmask.addr;
#endif

	/* start the application (web server, rxtest, txtest, etc..) */
	fAppStarted = (!start_application());

	return 0;
}

/*
 * Shuts down the echo server, DHCP and network interface
 */
XStatus fnCloseEth()
{
	struct ip_addr ipaddr, netmask, gw;

	// Shut down everything in backwards order
	//if (fAppStarted) stop_application();
	if (echo_netif != NULL)
	{
		dhcp_stop(echo_netif);
		netif_set_down(echo_netif);
		ipaddr.addr = 0; gw.addr = 0; netmask.addr = 0;
		netif_set_addr(echo_netif, &ipaddr, &netmask, &gw);
	}

	//We need to let timer run because tcp closure and deallocation is done later

	// Set flags
	fAppStarted = 0;
	return 0;
}

/*
 * Call this periodically to process data packets. Called in timer_interrupt in this project.
 */
void fnProcessEthData()
{
	if (fAppStarted)
	{
		xemacif_input(echo_netif);
		transfer_data();
	}
}

/*
 * Queries PHYs on an MDIO bus and returns the first address
 * fitting the PHY_DETECT_MASK.
 */
u32 AxiEthernetDetectPHY(XAxiEthernet * AxiEthernetInstancePtr)
{
	u16 PhyReg;
	int PhyAddr;

	for (PhyAddr = 31; PhyAddr >= 0; PhyAddr--) {
		XAxiEthernet_PhyRead(AxiEthernetInstancePtr, PhyAddr,
				 		PHY_DETECT_REG, &PhyReg);

		if ((PhyReg != 0xFFFF) &&
		   ((PhyReg & PHY_DETECT_MASK) == PHY_DETECT_MASK)) {
			/* Found a valid PHY address */
			return PhyAddr;
		}
	}

	return 0;		/* Default to zero */
}

/*
 * This function checks the existence of the Ethernet PHY and enables Link status change interrupts in it.
 */
XStatus fnPhyInit()
{
	int Status;

	XAxiEthernet_Config *pMacConfig;

	u16 PhyReg2;
	u16 PhyReg3, PhyReg18;

	u16 dwInsr;

	u32 Phy_ID;
	u32 Phy_Model;

	u8 u8Verbose = Demo.u8Verbose;

	// Get the configuration of AxiEthernet hardware.
	pMacConfig = XAxiEthernet_LookupConfig(ETHERNET_MAC_DEVICE_ID);

	// Initialize AxiEthernet hardware.
	Status = XAxiEthernet_CfgInitialize(&sAxiEthMac, pMacConfig,
			pMacConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		if (u8Verbose) xil_printf("\r\nError initializing ethernet instance");
		return XST_FAILURE;
	}

	//Detect phy
	PhyAddr = AxiEthernetDetectPHY(&sAxiEthMac);
	if (u8Verbose) xil_printf("\r\nPhy Detected Address = %d", PhyAddr);

	//Check Phy Model
	XAxiEthernet_PhyRead(&sAxiEthMac, PhyAddr, PHY_R2_PHY_IDENT_REG, &PhyReg2);
	XAxiEthernet_PhyRead(&sAxiEthMac, PhyAddr, PHY_R3_PHY_IDENT_REG, &PhyReg3);

	Phy_ID = ((PhyReg2 << 6) | ((PhyReg3 >> 10) & 0x3F));
	Phy_Model = ((PhyReg3 >> 4) & 0x3F);

	if (u8Verbose) xil_printf("\r\nPhy ID = 0x%X, Phy Model Number = 0x%X", Phy_ID, Phy_Model);

	if ((Phy_ID != RTL_8211E_ID) && (Phy_Model != RTL_8211E_MODEL)){
		if (u8Verbose) xil_printf("\r\nError: Phy ID and/or Model Number Incorrect");
		return XST_FAILURE;
	}

	// Enable just the Link status change interrupt
	XAxiEthernet_PhyWrite(&sAxiEthMac, PhyAddr, PHY_R18_INER, PHY_INER_LINK_STATUS_IRPT_MASK);
	XAxiEthernet_PhyRead(&sAxiEthMac, PhyAddr, PHY_R18_INER, &PhyReg18);

	// Clear interrupts by reading status reg
	XAxiEthernet_PhyRead(&sAxiEthMac, PhyAddr, PHY_R19_INSR, &dwInsr);
	XAxiEthernet_PhyRead(&sAxiEthMac, PhyAddr, PHY_R19_INSR, &dwInsr);

	// Reset PHY
	XAxiEthernet_PhyWrite(&sAxiEthMac, PhyAddr, PHY_R0_CTRL_REG, PHY_R0_RESET);

	return XST_SUCCESS;
}

/*
 * Prints the current network interface IP to stdout.
 * The interface IP might change due to late DHCP
 */
void print_echo_ip()
{
	struct ip_addr ip;
	ip.addr = (echo_netif ? echo_netif->ip_addr.addr : 0);
	xil_printf("%d.%d.%d.%d", ip4_addr1(&ip), ip4_addr2(&ip),
				ip4_addr3(&ip), ip4_addr4(&ip));
}

/*
 * Prints a message and a formatted IP to the stdout
 */
void
print_ip(char *msg, struct ip_addr *ip) 
{
	print(msg);
	xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip), 
			ip4_addr3(ip), ip4_addr4(ip));
}

/*
 * Prints the current network interface IP to a string. Msg is
 * caller-allocated, at least 16 characters long.
 */
void sprint_ip(char *msg)
{
	struct ip_addr ip;
	ip.addr = (echo_netif ? echo_netif->ip_addr.addr : 0);
	sprintf(msg, "%3d.%3d.%3d.%3d    ", ip4_addr1(&ip), ip4_addr2(&ip), ip4_addr3(&ip), ip4_addr4(&ip));
}

/*
 * Prints IP, mask and gateway IP to the stdout
 */
void
print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{

	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}

/*
 * Prints the current echo server network interface IP, mask and GW to stdout
 */
void fnPrintIpSettings()
{

	struct ip_addr ipaddr, netmask, gw;

	ipaddr.addr = echo_netif->ip_addr.addr;
	gw.addr = echo_netif->gw.addr;
	netmask.addr = echo_netif->netmask.addr;

	print_ip_settings(&ipaddr, &netmask, &gw);
}

/*
 * Ethernet PHY interrupt service routine. PHY will request an interrupt when change in the medium is detected.
 * The PHY status register needs to be read here.
 */
void fnEthPHYIsr(void *CallbackRef)
{
	u16 linkstatus;
	u16 speed_bits;
	u16 speed;
	u16 PhyReg17, dwInsr;

	// Read interrupt status reg
	XAxiEthernet_PhyRead(&sAxiEthMac, PhyAddr, PHY_R19_INSR, &dwInsr);

	// Detect Link change
	if (dwInsr & PHY_INER_LINK_STATUS_IRPT_MASK)
	{
		//Check PHY Link and speed
		XAxiEthernet_PhyRead(&sAxiEthMac, PhyAddr, PHY_R17_SPECIFIC_STATUS_REG, &PhyReg17);

		if (Demo.u8Verbose) xil_printf("\r\n Phy Reg 17 value = 0x%04X", PhyReg17);

		linkstatus = ((PhyReg17 & 0x0400) >> 10);
		speed_bits  = ((PhyReg17 & 0xC000) >> 14);

		if (Demo.u8Verbose) xil_printf("\r\n linkstatus = 0x%X, speed_bits = 0x%X", linkstatus, speed_bits);

		switch(speed_bits){
			case 0: speed = 10; break;
			case 1: speed = 100; break;
			case 2:	speed = 1000; break;
			default: speed = 0;
		}

		if (Demo.u8Verbose) xil_printf ("\r\nSpeed is %d", speed);

		Demo.fLinkEvent = 1;
		Demo.fLinkStatus = linkstatus;
		Demo.linkSpeed = speed;
	}
}


