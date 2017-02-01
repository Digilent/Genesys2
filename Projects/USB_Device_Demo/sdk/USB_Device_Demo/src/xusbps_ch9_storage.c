/******************************************************************************
*
* Copyright (C) 2010 - 2015 Xilinx, Inc.  All rights reserved.
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
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/
/*****************************************************************************/
/**
 * @file xusbps_ch9_storage.c
 *
 * This file contains the implementation of the storage specific chapter 9 code
 * for the example.
 *
 *<pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- ---------------------------------------------------------
 * 1.00a wgr  10/10/10 First release
 *</pre>
 ******************************************************************************/

/***************************** Include Files *********************************/

#include <string.h>

#include "xparameters.h"	/* XPAR parameters */
#include "xusbps.h"		/* USB controller driver */

#include "xusbps_ch9.h"
#include "xusbps_ch9_storage.h"
#include "demo.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	unsigned char ReportDesc[0x34];
#ifdef __ICCARM__
}USB_REPORT_DESC;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_REPORT_DESC;
#endif

//#ifdef __ICCARM__
//#pragma pack(push, 1)
//#endif
typedef struct __attribute__((__packed__)){
	u8  bLength;
	u8  bDescriptorType;
	u16 bcdUSB;
	u8  bDeviceClass;
	u8  bDeviceSubClass;
	u8  bDeviceProtocol;
	u8  bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8  iManufacturer;
	u8  iProduct;
	u8  iSerialNumber;
	u8  bNumConfigurations;
//#ifdef __ICCARM__
//}USB_STD_DEV_DESC;
//#pragma pack(pop)
//#else
}USB_STD_DEV_DESC;
//#endif

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	u8  bLength;
	u8  bDescriptorType;
	u16 wTotalLength;
	u8  bNumInterfaces;
	u8  bConfigurationValue;
	u8  iConfiguration;
	u8  bmAttributes;
	u8  bMaxPower;
#ifdef __ICCARM__
}USB_STD_CFG_DESC;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_STD_CFG_DESC;
#endif

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	u8  bLength;
	u8  bDescriptorType;
	u8  bInterfaceNumber;
	u8  bAlternateSetting;
	u8  bNumEndPoints;
	u8  bInterfaceClass;
	u8  bInterfaceSubClass;
	u8  bInterfaceProtocol;
	u8  iInterface;
#ifdef __ICCARM__
}USB_STD_IF_DESC;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_STD_IF_DESC;
#endif

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	u8 bLength;//u8 bLength;
	u8 bDescriptorType_1;//u8 bDescriptorType;
	u16 bcdHID;//int bcdUSB;
	u8 bCountryCode;
	u8 bNumDescriptors;
	u8 bDescriptorType_2;
	u16 wDescriptorLength;//u8 bMaxPacketSize0;
#ifdef __ICCARM__
}USB_STD_HID_DESC;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_STD_HID_DESC;
#endif

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	u8  bLength;
	u8  bDescriptorType;
	u8  bEndpointAddress;
	u8  bmAttributes;
	u16 wMaxPacketSize;
	u8  bInterval;
#ifdef __ICCARM__
}USB_STD_EP_DESC;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_STD_EP_DESC;
#endif

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	u8  bLength;
	u8  bDescriptorType;
	u16 wLANGID[1];
#ifdef __ICCARM__
}USB_STD_STRING_DESC;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_STD_STRING_DESC;
#endif

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	USB_STD_CFG_DESC stdCfg;
	USB_STD_IF_DESC ifCfg;
	USB_STD_EP_DESC epCfg1;
	USB_STD_EP_DESC epCfg2;
#ifdef __ICCARM__
}USB_CONFIG;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_CONFIG;
#endif

#ifdef __ICCARM__
#pragma pack(push, 1)
#endif
typedef struct {
	USB_STD_CFG_DESC stdCfg;
	USB_STD_IF_DESC ifCfg;
	USB_STD_HID_DESC HIDdesc;
	USB_STD_EP_DESC epCfg2;
#ifdef __ICCARM__
}USB_CONFIG_HID;
#pragma pack(pop)
#else
}__attribute__((__packed__))USB_CONFIG_HID;
#endif

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

#define USB_ENDPOINT0_MAXP		0x40

#define USB_BULKIN_EP			1
#define USB_BULKOUT_EP			1

#define USB_DEVICE_DESC			0x01
#define USB_CONFIG_DESC			0x02
#define USB_STRING_DESC			0x03
#define USB_INTERFACE_CFG_DESC		0x04
#define USB_ENDPOINT_CFG_DESC		0x05

int volatile configuration_set = 0;


/*****************************************************************************/
/**
*
* This function returns the device descriptor for the device.
*
* @param	BufPtr is pointer to the buffer that is to be filled
*		with the descriptor.
* @param	BufLen is the size of the provided buffer.
*
* @return 	Length of the descriptor in the buffer on success.
*		0 on error.
*
******************************************************************************/
u32 XUsbPs_Ch9SetupDevDescReply(u8 *BufPtr, u32 BufLen)
{
	USB_STD_DEV_DESC deviceDesc = {
		sizeof(USB_STD_DEV_DESC),	/* bLength */
		USB_DEVICE_DESC,		/* bDescriptorType */
		be2les(0x0200),			/* bcdUSB 2.0 */
		0x00,				/* bDeviceClass */
		0x00,				/* bDeviceSubClass */
		0x00,				/* bDeviceProtocol */
		USB_ENDPOINT0_MAXP,		/* bMaxPackedSize0 */
		be2les(0x0d7d),			/* idVendor */
		be2les(0x0100),			/* idProduct */
		be2les(0x0100),			/* bcdDevice */
		0x01,				/* iManufacturer */
		0x02,				/* iProduct */
		0x03,				/* iSerialNumber */
		0x01				/* bNumConfigurations */
	};

	/* Check buffer pointer is there and buffer is big enough. */
	if (!BufPtr) {
		return 0;
	}

	if (BufLen < sizeof(USB_STD_DEV_DESC)) {
		return 0;
	}

	memcpy(BufPtr, &deviceDesc, sizeof(USB_STD_DEV_DESC));

	return sizeof(USB_STD_DEV_DESC);
}

u32 XUsbPs_Ch9SetupDevDescReply_HID(u8 *BufPtr, u32 BufLen)
{
	USB_STD_DEV_DESC deviceDesc = {
		sizeof(USB_STD_DEV_DESC),	/* bLength */
		USB_DEVICE_DESC,		/* bDescriptorType */
		be2les(0x0200),			/* bcdUSB 2.0 */
		0xFF,				/* bDeviceClass */
		0x00,				/* bDeviceSubClass */
		0x00,				/* bDeviceProtocol */
		USB_ENDPOINT0_MAXP,		/* bMaxPackedSize0 */
		be2les(0x03FD),			/* idVendor */
		be2les(0x0300),			/* idProduct */
		be2les(0x0100),			/* bcdDevice */
		0x01,				/* iManufacturer */
		0x02,				/* iProduct */
		0x03,				/* iSerialNumber */
		0x01				/* bNumConfigurations */
	};

	/* Check buffer pointer is there and buffer is big enough. */
	if (!BufPtr) {
		return 0;
	}

	if (BufLen < sizeof(USB_STD_DEV_DESC)) {
		return 0;
	}

	memcpy(BufPtr, &deviceDesc, sizeof(USB_STD_DEV_DESC));

	return sizeof(USB_STD_DEV_DESC);
}

u32 XUsbPs_Ch9SetupHidDescReply(u8 *BufPtr, u32 BufLen)
{
	USB_STD_HID_DESC deviceDesc = {
			 0x09, //HID_desc.bLength
			 0x21, //HID_desc.bDescriptorType_1
			 be2les(0x0110), //HID_desc.bcdHID
			 0x00, //HID_desc.bCountryCode
			 0x01, //HID_desc.bNumDescriptors
			 0x22, //HID_desc.bDescriptorType_2
			 be2les(sizeof(USB_CONFIG_HID)), //HID_desc.wDescriptorLength
	};

	/* Check buffer pointer is there and buffer is big enough. */
	if (!BufPtr) {
		return 0;
	}

	if (BufLen < sizeof(USB_STD_DEV_DESC)) {
		return 0;
	}

	memcpy(BufPtr, &deviceDesc, sizeof(USB_STD_DEV_DESC));

	return sizeof(USB_STD_DEV_DESC);
}

u32 XUsbPs_Ch9SetupReportDescReply(u8 *BufPtr, u32 BufLen)
{
	USB_REPORT_DESC deviceDesc = {
			{
			0x05,0x01,		/* Usage Page (generic desktop) */
			0x09,0x02,		/* Usage (mouse) */
			0xA1,0x01,		/* Collection (Application)*/
			0x09,0x01,		/*   Usage Pointer () */
			0xA1,0x00,		/*   Collection (Physical) */
			0x05,0x09,		/*   Usage Page (Button) */
			0x19,0x01,		/*   Usage Minimum = 0 */
			0x29,0x05,		/*   Usage Maximum = 5 */
			0x15,0x00,		/*   Logical Minimum = 0 */
			0x25,0x01,		/*   Logical Maximum = 1 */
			0x95,0x03,		/*   Report Count = 3 */
			0x75,0x01,		/*   Report Size = 1 */
			0x81,0x02,		/*   Input (Data, Variable, Absolute, Bit Field) */
			0x95,0x01,		/*   Report Count = 1 */
			0x75,0x05,		/*   Report Size =  5 */
			0x81,0x01,		/*   Input (Constant, Array, Absolute, Bit Field) */
			0x05,0x01,		/*   Usage Page (Generic Desktop Controls) */
			0x09,0x30,		/*   Usage (X) */
			0x09,0x31,		/*   Usage (Y) */
			0x09,0x38,		/*   Usage (Wheel) */
			0x15,0x81,		/*   Logical Minimum = -127 */
			0x25,0x7F,		/*   Logical Maximum = 127 */
			0x75,0x08,		/*   Report Size = 8 */
			0x95,0x03,		/*   Report Count = 3 */
			0x81,0x06,		/*   Input (Data, Variable, Relative, Bit Field) */
			0xC0,
			0xC0}			/* End Collection */
	};

	/* Check buffer pointer is there and buffer is big enough. */
	if (!BufPtr) {
		return 0;
	}

	if (BufLen < sizeof(USB_REPORT_DESC)) {
		return 0;
	}

	memcpy(BufPtr, &deviceDesc, sizeof(USB_REPORT_DESC));

	return sizeof(USB_REPORT_DESC);
}
/*****************************************************************************/
/**
*
* This function returns the configuration descriptor for the device.
*
* @param	BufPtr is the pointer to the buffer that is to be filled with
*		the descriptor.
* @param	BufLen is the size of the provided buffer.
*
* @return 	Length of the descriptor in the buffer on success.
*		0 on error.
*
******************************************************************************/
u32 XUsbPs_Ch9SetupCfgDescReply_HID(u8 *BufPtr, u32 BufLen)
{
	USB_CONFIG_HID config = {
		/* Std Config */
		{sizeof(USB_STD_CFG_DESC),	/* bLength */
		 USB_CONFIG_DESC,		/* bDescriptorType */
		 be2les(sizeof(USB_CONFIG_HID)),	/* wTotalLength */
		 0x01,				/* bNumInterfaces */
		 0x01,				/* bConfigurationValue */
		 0x00,				/* iConfiguration */
		 0xc0,				/* bmAttribute */
		 0x01},				/* bMaxPower  */

		/* Interface Config */
		{sizeof(USB_STD_IF_DESC),	/* bLength */
		 USB_INTERFACE_CFG_DESC,	/* bDescriptorType */
		 0x00,				/* bInterfaceNumber */
		 0x00,				/* bAlternateSetting */
		 0x01,				/* bNumEndPoints */
		 0x03,				/* bInterfaceClass */
		 0x01,				/* bInterfaceSubClass */
		 0x2,				/* bInterfaceProtocol */
		 0x00},				/* iInterface */

		{
	     sizeof(USB_STD_HID_DESC), //HID_desc.bLength
		 0x21, //HID_desc.bDescriptorType_1
		 be2les(0x0110), //HID_desc.bcdHID
		 0x00, //HID_desc.bCountryCode
		 0x01, //HID_desc.bNumDescriptors
		 0x22, //HID_desc.bDescriptorType_2
		 0x34,//be2les(sizeof(USB_REPORT_DESC)), //HID_desc.wDescriptorLength
		},

		/* Bulk Out Endpoint Config */
		{sizeof(USB_STD_EP_DESC),	/* bLength */
		 USB_ENDPOINT_CFG_DESC,		/* bDescriptorType */
		 0x81,		/* bEndpointAddress */
		 0x03,				/* bmAttribute  */
		 be2les(0x0040),			/* wMaxPacketSize */
		 0x0A}			/* bInterval */

	};

	/* Check buffer pointer is OK and buffer is big enough. */
	if (!BufPtr) {
		return 0;
	}

	if (BufLen < sizeof(USB_STD_DEV_DESC)) {
		return 0;
	}

	memcpy(BufPtr, &config, sizeof(USB_CONFIG_HID));

	return sizeof(USB_CONFIG_HID);
}

u32 XUsbPs_Ch9SetupCfgDescReply(u8 *BufPtr, u32 BufLen)
{
	USB_CONFIG config = {
		/* Std Config */
		{sizeof(USB_STD_CFG_DESC),	/* bLength */
		 USB_CONFIG_DESC,		/* bDescriptorType */
		 be2les(sizeof(USB_CONFIG)),	/* wTotalLength */
		 0x01,				/* bNumInterfaces */
		 0x01,				/* bConfigurationValue */
		 0x04,				/* iConfiguration */
		 0xc0,				/* bmAttribute */
		 0x00},				/* bMaxPower  */

		/* Interface Config */
		{sizeof(USB_STD_IF_DESC),	/* bLength */
		 USB_INTERFACE_CFG_DESC,	/* bDescriptorType */
		 0x00,				/* bInterfaceNumber */
		 0x00,				/* bAlternateSetting */
		 0x02,				/* bNumEndPoints */
		 0x08,				/* bInterfaceClass */
		 0x06,				/* bInterfaceSubClass */
		 0x50,				/* bInterfaceProtocol */
		 0x05},				/* iInterface */

		/* Bulk Out Endpoint Config */
		{sizeof(USB_STD_EP_DESC),	/* bLength */
		 USB_ENDPOINT_CFG_DESC,		/* bDescriptorType */
		 0x00 | USB_BULKOUT_EP,		/* bEndpointAddress */
		 0x02,				/* bmAttribute  */
		 be2les(0x200),			/* wMaxPacketSize */
		 0x00},				/* bInterval */

		/* Bulk In Endpoint Config */
		{sizeof(USB_STD_EP_DESC),	/* bLength */
		 USB_ENDPOINT_CFG_DESC,		/* bDescriptorType */
		 0x80 | USB_BULKIN_EP,		/* bEndpointAddress */
		 0x02,				/* bmAttribute  */
		 be2les(0x200),			/* wMaxPacketSize */
		 0x00}				/* bInterval */
	};

	/* Check buffer pointer is OK and buffer is big enough. */
	if (!BufPtr) {
		return 0;
	}

	if (BufLen < sizeof(USB_STD_DEV_DESC)) {
		return 0;
	}

	memcpy(BufPtr, &config, sizeof(USB_CONFIG));

	return sizeof(USB_CONFIG);
}
/*****************************************************************************/
/**
*
* This function returns a string descriptor for the given index.
*
* @param	BufPtr is a  pointer to the buffer that is to be filled with
*		the descriptor.
* @param	BufLen is the size of the provided buffer.
* @param	Index is the index of the string for which the descriptor
*		is requested.
*
* @return 	Length of the descriptor in the buffer on success.
*		0 on error.
*
******************************************************************************/
u32 XUsbPs_Ch9SetupStrDescReply(u8 *BufPtr, u32 BufLen, u8 Index)
{
	int i;
#ifdef MOUSE_SIMULATION
	static char *StringList[] = {
		"UNUSED",
		"Digilent",
		"Digilent Mouse Example Revision1",
		"2A49876D9CC1AA4",
		"Default Configuration",
		"Default Interface",
	};
#else

	static char *StringList[] = {
		"UNUSED",
		"Digilent",
		"Digilent Mouse Example Revision1",//"EPB USB Flash Drive Disk Emulation",
		"2A49876D9CC1AA4",
		"Default Configuration",
		"Default Interface",
	};
#endif
	char *String;
	u32 StringLen;
	u32 DescLen;
	u8 TmpBuf[128];

	USB_STD_STRING_DESC *StringDesc;

	if (!BufPtr) {
		return 0;
	}

	if (Index >= sizeof(StringList) / sizeof(char *)) {
		return 0;
	}

	String = StringList[Index];
	StringLen = strlen(String);

	StringDesc = (USB_STD_STRING_DESC *) TmpBuf;

	/* Index 0 is special as we can not represent the string required in
	 * the table above. Therefore we handle index 0 as a special case.
	 */
	if (0 == Index) {
		StringDesc->bLength = 4;
		StringDesc->bDescriptorType = USB_STRING_DESC;
		StringDesc->wLANGID[0] = be2les(0x0409);
	}
	/* All other strings can be pulled from the table above. */
	else {
		StringDesc->bLength = StringLen * 2 + 2;
		StringDesc->bDescriptorType = USB_STRING_DESC;

		for (i = 0; i < StringLen; i++) {
			StringDesc->wLANGID[i] = be2les((u16) String[i]);
		}
	}
	DescLen = StringDesc->bLength;

	/* Check if the provided buffer is big enough to hold the descriptor. */
	if (DescLen > BufLen) {
		return 0;
	}

	memcpy(BufPtr, StringDesc, DescLen);

	return DescLen;
}


/*****************************************************************************/
/**
* This function handles a "set configuration" request.
*
* @param	InstancePtr is a pointer to XUsbPs instance of the controller.
* @param	ConfigIdx is the Index of the desired configuration.
*
* @return	None
*
******************************************************************************/
void XUsbPs_SetConfiguration(XUsbPs *InstancePtr, int ConfigIdx)
{
	Xil_AssertVoid(InstancePtr != NULL);

	/* We only have one configuration. Its index is 1. Ignore anything
	 * else.
	 */
	if (1 != ConfigIdx) {
		return;
	}

	XUsbPs_EpEnable(InstancePtr, 1, XUSBPS_EP_DIRECTION_OUT);
	XUsbPs_EpEnable(InstancePtr, 1, XUSBPS_EP_DIRECTION_IN);

	/* Set BULK mode for both directions.  */
	XUsbPs_SetBits(InstancePtr, XUSBPS_EPCR1_OFFSET,
						XUSBPS_EPCR_TXT_BULK_MASK |
						XUSBPS_EPCR_RXT_BULK_MASK |
						XUSBPS_EPCR_TXR_MASK |
						XUSBPS_EPCR_RXR_MASK);

	/* Prime the OUT endpoint. */
	configuration_set = 1;
	XUsbPs_EpPrime(InstancePtr, 1, XUSBPS_EP_DIRECTION_OUT);
}
