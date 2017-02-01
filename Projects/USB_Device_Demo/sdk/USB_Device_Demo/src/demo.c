
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "platform.h"
#include "xil_io.h"
#include "xintc.h"	//Interrupt Controller API functions
#include "xparameters.h"  //The hardware configuration describing constants
#include "xusbps.h"			/* USB controller driver */
#include "xgpio.h"

#define USBCMD            0x44a00140
#define USBINTR           0x44a00148
#define ENDPTCOMPLETE     0x44a001BC
//#define USB_IRPT_ID       0

#define BUTTON_CHANNEL 1
#define GPIO_ALL_BUTTONS  0x1F	/* The GPIO bits 0 to 4. */
#define INTC_GPIO_INTERRUPT_ID XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_1_IP2INTC_IRPT_INTR
#define CENTER_BUTTON_MASK 0x01
#define UP_BUTTON_MASK 0x02
#define RIGHT_BUTTON_MASK  0x04
#define DOWN_BUTTON_MASK  0x08
#define LEFT_BUTTON_MASK  0x10
#define BUTTON_INTERRUPT 1

#define USB_IRPT_ID XPAR_MICROBLAZE_0_AXI_INTC_USB_DEVICE_1_INTERRUPT_INTR
#define XPAR_XUSBPS_0_DEVICE_ID 0

#include "xusbps_ch9.h"		/* Generic Chapter 9 handling code */
#include "xusbps_class_storage.h"	/* Storage class handling code */

#define MEMORY_SIZE (64 * 1024)

u8 Buffer[MEMORY_SIZE] ALIGNMENT_CACHELINE;
int extern configuration_set;

static XIntc IntcInstance;	/* The instance of the IRQ Controller */
static XUsbPs UsbInstance;	/* The instance of the USB Controller */

static void UsbIntrHandler(void *CallBackRef, u32 Mask);
static void GpioIsr(void *InstancePtr);
static void XUsbPs_Ep0EventHandler(void *CallBackRef, u8 EpNum, u8 EventType, void *Data);
static void XUsbPs_Ep1EventHandler(void *CallBackRef, u8 EpNum, u8 EventType, void *Data);
static int UsbSetupIntrSystem(XIntc *IntcInstancePtr, XUsbPs *UsbInstancePtr, XGpio *GpioInstance, u16 UsbIntrId, u16 GpioIntrID);
static void UsbDisableIntrSystem(XIntc *IntcInstancePtr, u16 UsbIntrId);
static void UsbIntrHandler(void *CallBackRef, u32 Mask);

int main()
{
	int Status;
	const u8 NumEndpoints = 2;
	u8	*MemPtr = NULL;
	unsigned long   mask;
	XUsbPs_Config		*UsbConfigPtr;
	XUsbPs_DeviceConfig	DeviceConfig;
	XGpio Gpio; 		/* The Instance of the GPIO Driver */
	XGpio_Config *GpioConfigPtr;	/* Pointer to the GPIO config structure */

	//Configure GPIO (buttons)
    GpioConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_1_DEVICE_ID);
	if (GpioConfigPtr == NULL) {
		return XST_FAILURE;
	}
	Status = XGpio_CfgInitialize(&Gpio, GpioConfigPtr, GpioConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XGpio_SetDataDirection(&Gpio, BUTTON_CHANNEL, GPIO_ALL_BUTTONS);

	UsbConfigPtr = XUsbPs_LookupConfig(XPAR_XUSBPS_0_DEVICE_ID);
	if (NULL == UsbConfigPtr) {
		return 0;
	}
	//Configure USB controller
	Status = XUsbPs_CfgInitialize(&UsbInstance, UsbConfigPtr, UsbConfigPtr->BaseAddress);
	if (XST_SUCCESS != Status) {
		return 0;
	}
	//Configure DQH
	DeviceConfig.EpCfg[0].Out.Type		= XUSBPS_EP_TYPE_CONTROL;
	DeviceConfig.EpCfg[0].Out.NumBufs	= 2;
	DeviceConfig.EpCfg[0].Out.BufSize	= 64;
	DeviceConfig.EpCfg[0].Out.MaxPacketSize	= 64;
	DeviceConfig.EpCfg[0].In.Type		= XUSBPS_EP_TYPE_CONTROL;
	DeviceConfig.EpCfg[0].In.NumBufs	= 2;
	DeviceConfig.EpCfg[0].In.MaxPacketSize	= 64;

	DeviceConfig.EpCfg[1].Out.Type		= XUSBPS_EP_TYPE_BULK;
	DeviceConfig.EpCfg[1].Out.NumBufs	= 16;
	DeviceConfig.EpCfg[1].Out.BufSize	= 512;
	DeviceConfig.EpCfg[1].Out.MaxPacketSize	= 512;
	DeviceConfig.EpCfg[1].In.Type		= XUSBPS_EP_TYPE_BULK;
	DeviceConfig.EpCfg[1].In.NumBufs	= 16;
	DeviceConfig.EpCfg[1].In.MaxPacketSize	= 512;

	DeviceConfig.NumEndpoints = NumEndpoints;

	MemPtr = (u8 *)&Buffer[0];
	memset(MemPtr,0,MEMORY_SIZE);
	microblaze_flush_dcache_range((unsigned int)MemPtr, MEMORY_SIZE);
	DeviceConfig.DMAMemPhys = (u32) MemPtr;

	Status = XUsbPs_ConfigureDevice(&UsbInstance, &DeviceConfig);
	if (XST_SUCCESS != Status) {
		return 0;
	}

	Status = UsbSetupIntrSystem(&IntcInstance, &UsbInstance, &Gpio, USB_IRPT_ID, INTC_GPIO_INTERRUPT_ID);
	if (XST_SUCCESS != Status)
	{
		return 0;
	}

	Status = XUsbPs_IntrSetHandler(&UsbInstance, UsbIntrHandler, NULL, XUSBPS_IXR_UE_MASK);
	if (XST_SUCCESS != Status) {
		return 0;
	}

	Status = XUsbPs_EpSetHandler(&UsbInstance, 0, XUSBPS_EP_DIRECTION_OUT, XUsbPs_Ep0EventHandler, &UsbInstance);
	if (XST_SUCCESS != Status) {
		return 0;
	}
	//Set the RS(start/stop) bit in USBCMD register
	Xil_Out32(USBCMD, 0x00000001);

	while (configuration_set == 0)
	{
	}

    while(1)
    {

    }
    return 0;
}

/*****************************************************************************/
/**
*
* This function setups the interrupt system such that interrupts can occur for
* the USB controller. This function is application specific since the actual
* system may or may not have an interrupt controller. The USB controller could
* be directly connected to a processor without an interrupt controller.  The
* user should modify this function to fit the application.
*
* @param	IntcInstancePtr is a pointer to instance of the Intc controller.
* @param	UsbInstancePtr is a pointer to instance of the USB controller.
* @param	UsbIntrId is the Interrupt Id and is typically
* 		XPAR_<INTC_instance>_<USB_instance>_VEC_ID value
* 		from xparameters.h
*
* @return
* 		- XST_SUCCESS if successful
* 		- XST_FAILURE on error
*
******************************************************************************/
static int UsbSetupIntrSystem(XIntc *IntcInstancePtr, XUsbPs *UsbInstancePtr, XGpio *GpioInstance, u16 UsbIntrId, u16 GpioIntrID)
{
	int Status;
	XIntc_Config *IntcConfig;

/*	IntcConfig = XIntc_LookupConfig(XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}*/

	Status = XIntc_Initialize(IntcInstancePtr, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XIntc_Connect(IntcInstancePtr, GpioIntrID, (XInterruptHandler)GpioIsr, GpioInstance);
	if (Status != XST_SUCCESS) {
		return Status;
	}
	XGpio_InterruptEnable(GpioInstance, BUTTON_INTERRUPT);
	XGpio_InterruptGlobalEnable(GpioInstance);
	XIntc_Enable(IntcInstancePtr, INTC_GPIO_INTERRUPT_ID);

	Status = XIntc_Connect(IntcInstancePtr, UsbIntrId, (XInterruptHandler)XUsbPs_IntrHandler, (void *)UsbInstancePtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	XUsbPs_IntrEnable(UsbInstancePtr, XUSBPS_IXR_UR_MASK | XUSBPS_IXR_UI_MASK); //UI and RESET

	XIntc_Enable(IntcInstancePtr, UsbIntrId);
	microblaze_register_handler(XIntc_DeviceInterruptHandler, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	microblaze_enable_interrupts();
	XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);

	return XST_SUCCESS;
}

static void UsbIntrHandler(void *CallBackRef, u32 Mask)
{

}

/*****************************************************************************/
/**
* This funtion is registered to handle callbacks for endpoint 0 (Control).
*
* It is called from an interrupt context such that the amount of processing
* performed should be minimized.
*
*
* @param	CallBackRef is the reference passed in when the function
*		was registered.
* @param	EpNum is the Number of the endpoint on which the event occured.
* @param	EventType is type of the event that occured.
*
* @return	None.
*
******************************************************************************/

static void XUsbPs_Ep0EventHandler(void *CallBackRef, u8 EpNum, u8 EventType, void *Data)
{
	XUsbPs			*InstancePtr;
	int			Status;
	XUsbPs_SetupData	SetupData;
	u8	*BufferPtr;
	u32	BufferLen;
	u32	Handle;

	Xil_AssertVoid(NULL != CallBackRef);

	InstancePtr = (XUsbPs *) CallBackRef;

	switch (EventType) {

	/* Handle the Setup Packets received on Endpoint 0. */
	case XUSBPS_EP_EVENT_SETUP_DATA_RECEIVED:
		Status = XUsbPs_EpGetSetupData(InstancePtr, EpNum, &SetupData);
		if (XST_SUCCESS == Status) {
			/* Handle the setup packet. */
			(int) XUsbPs_Ch9HandleSetupPacket(InstancePtr,
							   &SetupData);
		}
		break;

	/* We get data RX events for 0 length packets on endpoint 0. We receive
	 * and immediately release them again here, but there's no action to be
	 * taken.
	 */
	case XUSBPS_EP_EVENT_DATA_RX:
		/* Get the data buffer. */
		Status = XUsbPs_EpBufferReceive(InstancePtr, EpNum,
					&BufferPtr, &BufferLen, &Handle);
		if (XST_SUCCESS == Status) {
			/* Return the buffer. */
			XUsbPs_EpBufferRelease(Handle);
		}
		break;

	default:
		/* Unhandled event. Ignore. */
		break;
	}
}

/*****************************************************************************/
/**
* This funtion is registered to handle callbacks for the GPIO controller.
*
* It is called from an interrupt context such that the amount of processing
* performed should be minimized.
*
*
* @param	InstancePtr is a pointer to instance of the GPIO controller

* @return	None.
*
******************************************************************************/

void GpioIsr(void *InstancePtr)
{
	XGpio *GpioPtr = (XGpio *)InstancePtr;
	u8 Reply[4];
	u32 Buttons;

	XGpio_InterruptDisable(GpioPtr, BUTTON_INTERRUPT);
	Reply[0] = 0;
	Reply[1] = 0;
	Reply[2] = 0;
	Reply[3] = 0;

	//Find out which button was pressed and prepare the HID report
	Buttons = (XGpio_DiscreteRead(GpioPtr, BUTTON_CHANNEL) & 0x1F);
	if ((Buttons & RIGHT_BUTTON_MASK) != 0)
	{
		Reply[0] = 0;
		Reply[1] = 4;
		Reply[2] = 0;
		Reply[3] = 0;
		xil_printf("RIGHT\n", Buttons);
	}
    if ((Buttons & LEFT_BUTTON_MASK) != 0)
	{
		Reply[0] = 0;
		Reply[1] = -4;
		Reply[2] = 0;
		Reply[3] = 0;
		xil_printf("LEFT\n", Buttons);
	}
    if ((Buttons & DOWN_BUTTON_MASK) != 0)
	{
		Reply[0] = 0;
		Reply[1] = 0;
		Reply[2] = 4;
		Reply[3] = 0;
		xil_printf("UP\n", Buttons);
	}
    if ((Buttons & UP_BUTTON_MASK) != 0)
	{
		Reply[0] = 0;
		Reply[1] = 0;
		Reply[2] = -4;
		Reply[3] = 0;
		xil_printf("DOWN\n", Buttons);
	}

	XUsbPs_EpSetupBufferSend(&UsbInstance, 1, Reply, 4);

	(void)XGpio_InterruptClear(GpioPtr, BUTTON_INTERRUPT);
	XGpio_InterruptEnable(GpioPtr, BUTTON_INTERRUPT);
}
