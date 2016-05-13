/******************************************************************************
 * @file qspi.c
 * Bootloader implementation on a Spansion serial flash medium.
 *
 * @authors Elod Gyorgy
 *
 * @date 2015-Dec-23
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
 * The SREC bootloader will call these functions to read data from the flash.
 * These functions use the Xilinx Quad SPI IP driver.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date        Changes
 * ----- ------------ ----------- --------------------------------------------
 * 1.00  Elod Gyorgy  2015-Dec-23 First release
 *
 *
 * </pre>
 *
 *****************************************************************************/

#include "qspi.h"
#include "xparameters.h"
#include <string.h>

#define QSPI_DEVICE_ID  XPAR_QSPI_FLASH_DEVICE_ID
#define QSPI_SS_MASK	0x1	//Slave select 0 is tied to the SS of the flash

#define PAGE_SIZE 	256 //Also maximum data byte count
#define OTP_SIZE	1024 //Size of the OTP address space S25FL256S
/*
 * Definitions of the commands shown in this example.
 */
#define COMMAND_RDSR1			0x05 /* Status read command */
#define COMMAND_RDCR			0x35 /* Status read command */
#define COMMAND_4QOR			0x6C /* Read Quad Out (4-byte Address) */
#define COMMAND_OTPR			0x4B /* OTP Read */

/**
 * This definitions number of bytes in each of the command
 * transactions. This count includes Command byte, address bytes and any
 * don't care bytes needed.
 */
#define RDSR1_BYTES				2 /* Status read bytes count */
#define RDCR_BYTES				2 /* Status read bytes count */

/*
 * Flash not busy mask in the status register of the flash device.
 */
#define FLASH_SR_IS_READY_MASK		0x01 /* Ready mask */

/*
 * Byte Positions.
 */
#define BYTE1				0 /* Byte 1 position */
#define BYTE2				1 /* Byte 2 position */
#define BYTE3				2 /* Byte 3 position */
#define BYTE4				3 /* Byte 4 position */
#define BYTE5				4 /* Byte 5 position */
#define BYTE6				5 /* Byte 6 position */
#define BYTE7				6 /* Byte 7 position */
#define BYTE8				7 /* Byte 8 position */

/*
 * Buffers used during read and write transactions.
 */
u8 rgbReadBuffer[PAGE_SIZE + READ_WRITE_EXTRA_BYTES];
u8 rgbWriteBuffer[PAGE_SIZE + READ_WRITE_EXTRA_BYTES];

// The following variables are shared between non-interrupt processing an
// interrupt processing such that they must be global.
volatile static int TransferInProgress;
static int ErrorCount;

void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount)
{
	/*
	 * Indicate the transfer on the SPI bus is no longer in progress
	 * regardless of the status event.
	 */
	TransferInProgress = FALSE;

	/*
	 * If the event was not transfer done, then track it as an error.
	 */
	if (StatusEvent != XST_SPI_TRANSFER_DONE) {
		ErrorCount++;
	}
}

XStatus init_qspi(XSpi *psQSpi)
{
	int Status;
	XSpi_Config *pConfigPtr;	/* Pointer to Configuration data */

    /* Init Quad SPI too
     * Since the Quad SPI Flash is not memory-mapped, memory access instructions
     * need to be replaced by function calls to the AXI Quad SPI core to fetch data */


	/*
	 * Initialize the SPI driver so that it is  ready to use.
	 */
	pConfigPtr = XSpi_LookupConfig(QSPI_DEVICE_ID);
	if (pConfigPtr == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XSpi_CfgInitialize(psQSpi, pConfigPtr,
			pConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly
	 */
	Status = XSpi_SelfTest(psQSpi);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the SPI device as a master and in manual slave select mode such
	 * that the slave select signal does not toggle for every byte of a
	 * transfer, this must be done before the slave select is set.
	 */
	Status = XSpi_SetOptions(psQSpi, XSP_MASTER_OPTION |
				 XSP_MANUAL_SSELECT_OPTION);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Specify our handler that will be called by the driver interrupt handler
	XSpi_SetStatusHandler(psQSpi, psQSpi, (XSpi_StatusHandler)SpiHandler);

	/*
	 * Start the SPI driver.
	 */
	XSpi_Start(psQSpi);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function reads the data from Serial Flash Memory. It uses a caller-
* provided memory buffer to construct the SPI Read command. This means the
* buffer has to be at least cbLen + READ_WRITE_EXTRA_BYTES long. Actual data
* read from the Flash will be available from address pTo+READ_WRITE_EXTRA_BYTES.
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
* @param	Addr is the starting address in the Flash Memory from which the
*		data is to be read.
* @param	ByteCount is the number of bytes to be read.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
XStatus SpiFlashReadInPlace(XSpi *psQSpi, u32 fromAddr, u32 cbLen, u8* pTo)
{
	XStatus Status;

	/*
	 * Select the quad flash device on the SPI bus, so that it can be
	 * read and written using the SPI bus.
	 */
	Status = XSpi_SetSlaveSelect(psQSpi, QSPI_SS_MASK);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(psQSpi);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = SpiFlashGetControl(psQSpi);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the rgbWriteBuffer.
	 */
	*pTo = COMMAND_4QOR;
	*(pTo+1) = (u8) (fromAddr >> 24);
	*(pTo+2) = (u8) (fromAddr >> 16);
	*(pTo+3) = (u8) (fromAddr >> 8);
	*(pTo+4) = (u8) (fromAddr);

	//Transfer only the command,address and dummy bytes first
	TransferInProgress = TRUE;
	Status = XSpi_Transfer(psQSpi, pTo, pTo, cbLen + READ_WRITE_EXTRA_BYTES);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till the Transfer is complete and check if there are any errors
	 * in the transaction..
	 */
	while(TransferInProgress);
	if(ErrorCount != 0) {
		ErrorCount = 0;
		return XST_FAILURE;
	}

	/*
	 * Select the quad flash device on the SPI bus, so that it can be
	 * read and written using the SPI bus.
	 */
	Status = XSpi_SetSlaveSelect(psQSpi, 0);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function reads the data from the Winbond Serial Flash Memory
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
* @param	Addr is the starting address in the Flash Memory from which the
*		data is to be read.
* @param	ByteCount is the number of bytes to be read.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
XStatus SpiFlashRead(XSpi *psQSpi, u32 Addr, u32 ByteCount)
{
	int Status;

	if (ByteCount > PAGE_SIZE)
		return XST_FAILURE;

	/*
	 * Select the quad flash device on the SPI bus, so that it can be
	 * read and written using the SPI bus.
	 */
	Status = XSpi_SetSlaveSelect(psQSpi, QSPI_SS_MASK);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(psQSpi);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = SpiFlashGetControl(psQSpi);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the rgbWriteBuffer.
	 */
	rgbWriteBuffer[BYTE1] = COMMAND_4QOR;
	rgbWriteBuffer[BYTE2] = (u8) (Addr >> 24);
	rgbWriteBuffer[BYTE3] = (u8) (Addr >> 16);
	rgbWriteBuffer[BYTE4] = (u8) (Addr >> 8);
	rgbWriteBuffer[BYTE5] = (u8) (Addr);

	/*
	 * Initiate the Transfer.
	 */
	TransferInProgress = TRUE;
	Status = XSpi_Transfer(psQSpi, &rgbWriteBuffer[0], &rgbReadBuffer[0],
			(ByteCount + READ_WRITE_EXTRA_BYTES));
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till the Transfer is complete and check if there are any errors
	 * in the transaction..
	 */
	while(TransferInProgress);
	if(ErrorCount != 0) {
		ErrorCount = 0;
		return XST_FAILURE;
	}

	/*
	 * Select the quad flash device on the SPI bus, so that it can be
	 * read and written using the SPI bus.
	 */
	Status = XSpi_SetSlaveSelect(psQSpi, 0);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function waits until the flash is ready to accept the next
* command.
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		This function reads the status register of the Buffer and waits
*.		till the WIP bit of the status register becomes 0.
*
******************************************************************************/
XStatus SpiFlashWaitForFlashReady(XSpi *psQSpi)
{
	int Status;
	u8 StatusReg;

	while(1) {

		/*
		 * Get the Status Register.
		 */
		Status = SpiFlashGetStatus(psQSpi);
		if(Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		/*
		 * Check if the flash is ready to accept the next command.
		 * If so break.
		 */
		StatusReg = rgbReadBuffer[1];
		if((StatusReg & FLASH_SR_IS_READY_MASK) == 0) {
			break;
		}
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function reads the Status register SR1
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		The status register content is stored at the second byte pointed
*		by the rgbReadBuffer.
*
******************************************************************************/
XStatus SpiFlashGetStatus(XSpi *psQSpi)
{
	int Status;

	/*
	 * Prepare the Write Buffer.
	 */
	rgbReadBuffer[BYTE1] = COMMAND_RDSR1;

	/*
	 * Initiate the Transfer.
	 */
	TransferInProgress = TRUE;
	Status = XSpi_Transfer(psQSpi, &rgbReadBuffer[0], &rgbReadBuffer[0],
			RDSR1_BYTES);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till the Transfer is complete and check if there are any errors
	 * in the transaction..
	 */
	while(TransferInProgress);
	if(ErrorCount != 0) {
		ErrorCount = 0;
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function reads the Control register CR1
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		The status register content is stored at the second byte pointed
*		by the rgbReadBuffer.
*
******************************************************************************/
XStatus SpiFlashGetControl(XSpi *psQSpi)
{
	int Status;

	/*
	 * Prepare the Write Buffer.
	 */
	rgbReadBuffer[BYTE1] = COMMAND_RDCR;

	/*
	 * Initiate the Transfer.
	 */
	TransferInProgress = TRUE;
	Status = XSpi_Transfer(psQSpi, &rgbReadBuffer[0], &rgbReadBuffer[0],
			RDCR_BYTES);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till the Transfer is complete and check if there are any errors
	 * in the transaction..
	 */
	while(TransferInProgress);
	if(ErrorCount != 0) {
		ErrorCount = 0;
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*!
 * SpiReadOTP reads the OTP region of the Flash using the OTPR 0x4B command
 * Reads maximum PAGE_SIZE bytes, which is a limitation of this software
 * implementation.
 *
 * @param	psQSpi is a pointer to the instance of the Spi device.
 * @param	Addr is the byte address of the OTP region
 * @param	pbBuf is a pointer to a byte buffer where data will be stored
 * @param	cbLen is the number of bytes to read
 *
 * @return	XST_SUCCESS if successful else XST_FAILURE.

 */
XStatus SpiReadOTP(XSpi *psQSpi, u32 Addr, u8* pbBuf, u32 cbLen)
{
	XStatus Status;

	if (cbLen > OTP_SIZE) return XST_INVALID_PARAM;
	if (cbLen > PAGE_SIZE) return XST_INVALID_PARAM;
	if (Addr > OTP_SIZE) return XST_INVALID_PARAM;

	/*
	 * Select the quad flash device on the SPI bus, so that it can be
	 * read and written using the SPI bus.
	 */
	Status = XSpi_SetSlaveSelect(psQSpi, QSPI_SS_MASK);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(psQSpi);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = SpiFlashGetControl(psQSpi);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the rgbWriteBuffer.
	 */
	rgbWriteBuffer[BYTE1] = COMMAND_OTPR;
	rgbWriteBuffer[BYTE2] = (u8) (Addr >> 16);
	rgbWriteBuffer[BYTE3] = (u8) (Addr >> 8);
	rgbWriteBuffer[BYTE4] = (u8) (Addr);

	/*
	 * Initiate the Transfer.
	 */
	do
	{
		Status = XSpi_Transfer(psQSpi, &rgbWriteBuffer[0], &rgbReadBuffer[0],
				(cbLen + OTPR_EXTRA_BYTES));
	}
	while (Status == XST_DEVICE_BUSY);

	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	memcpy(pbBuf, &rgbReadBuffer[OTPR_EXTRA_BYTES], cbLen);
	return XST_SUCCESS;
}
