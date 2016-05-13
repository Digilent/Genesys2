/******************************************************************************
 * @file video.c
 * This file contains initialization and control functions for the video path:
 * VDMA, TPG, Mouse overlay,
 *
 * @authors Elod Gyorgy
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

 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date        Changes
 * ----- ------------ ----------- --------------------------------------------
 * 1.00  Elod Gyorgy  2015-Jul-20 Initial version

 * </pre>
 *
 *****************************************************************************/

#include "video.h"
#include "xparameters.h"
#include "../qspi/qspi.h"
#include "../demo.h"

#define DMA_DEVICE_ID				XPAR_VIDEO_AXI_VDMA_0_DEVICE_ID
#define READ_INTR_ID				XPAR_INTC_0_AXIVDMA_0_MM2S_INTROUT_VEC_ID
#define TPG_DEVICE_ID				XPAR_VIDEO_V_TPG_0_DEVICE_ID
#define DDR_BASE_ADDR				XPAR_MIG_7SERIES_0_BASEADDR
#define PS2_MOUSE_BASEADDR			XPAR_VIDEO_PS2_MOUSE_OVERLAY_0_BASEADDR

#define VIDEO_PATH_CTRL_INPUT_CHANNEL 1
#define VIDEO_PATH_CTRL_INTERRUPT XGPIO_IR_CH1_MASK  // Channel 1 Interrupt Mask
#define VIDEO_PATH_CTRL_OUTPUT_CHANNEL 2
#define VIDEO_PATH_CTRL_DVICLOCK_LOCKED_BIT_MASK 0x1
#define VIDEO_PATH_CTRL_VTD_LOCKED_BIT_MASK 0x2
#define VIDEO_PATH_CTRL_VIDOUT_LOCKED_BIT_MASK 0x4

/* Memory space for the frame buffers
 *
 * This example only needs one set of frame buffers, because one video IP is
 * to write to the frame buffers, and the other video IP is to read from the
 * frame buffers.
 *
 * For 16 frames of 1080p, it needs 0x07E90000 memory for frame buffers
 */
// Add a 256 (0x100) offset, to make sure the frame address is aligned
// and that we have space for QSPI command bytes
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x20000000 + 0x100)
#define MEM_HIGH_ADDR		DDR_HIGH_ADDR
#define MEM_SPACE		(MEM_HIGH_ADDR - MEM_BASE_ADDR)

//! Starting address of the video demo image in flash
#define IMAGE_FLASH_BASE_ADDR 0x00800000

/* Read channel and write channel start from the same place
 *
 * One video IP write to the memory region, the other video IP read from it
 */
#define READ_ADDRESS_BASE	MEM_BASE_ADDR
#define WRITE_ADDRESS_BASE	MEM_BASE_ADDR

/* Frame size related constants
 */
#define FRAME_HRES  1920   /* 1920 pixels*/
#define FRAME_VRES    1080    /* 1080 lines */

typedef struct vdma_context_t
{
	/* The state variable to keep track if the initialization is done*/
	unsigned int init_done;

	/* The XAxiVdma_DmaSetup structure contains all the necessary information to
	 * start a frame write or read. */
	XAxiVdma_DmaSetup ReadCfg;
	XAxiVdma_DmaSetup WriteCfg;
	/* Horizontal size of frame */
	unsigned int hsize;
	/* Vertical size of frame */
	unsigned int vsize;
	/* Buffer address from where read and write will be done by VDMA */
	unsigned int buffer_address;
	/* Flag to tell VDMA to interrupt on frame completion*/
	unsigned int enable_frm_cnt_intr;
	/* The counter to tell VDMA on how many frames the interrupt should happen*/
	unsigned int number_of_frame_count;
} vdma_context_t;

vdma_context_t vdma_context = {}; //Will init to 0 (C99 6.7.8.21)
extern volatile sDemo_t Demo;

static void ReadCallBack(void *CallbackRef, u32 Mask)
{
	/* User can add his code in this call back function */
}

static void ReadErrorCallBack(void *CallbackRef, u32 Mask)
{
	/* User can add his code in this call back function */
	xil_printf("VDMA error code %8x\r\n", Mask);
}

static void TpgErrorCallBack(void *CallBackRef, u32 ErrorMask)
{
	xil_printf("TPG error code %8x\r\n", ErrorMask);
}

/*!
 * VideoInit initializes the VDMA, TPG and Quad SPI drivers and then copies the static image
 * from the Flash memory to the frame buffer.
 *
 * Parameters:
 * 	@param XAxiVdma* psVdma - driver instance to initialize
 * 	@param XV_tpg* psTpg - driver instance to initialize
 * 	@param XSpi* psQspi - driver instance to initialize
 */
XStatus VideoInit(XAxiVdma* psVdma, XSpi* psQspi)
{
	{ // VDMA driver init
		XAxiVdma_Config* psConf;
		XStatus Status;
		vdma_context_t* pContext = &vdma_context;

		if (!pContext->init_done) {
			psConf = XAxiVdma_LookupConfig(DMA_DEVICE_ID);
			if (!psConf) {
				xil_printf("No video DMA found for ID %d\r\n", DMA_DEVICE_ID);
				return XST_FAILURE;
			}

			Status = XAxiVdma_CfgInitialize(psVdma, psConf, psConf->BaseAddress);
			if (Status != XST_SUCCESS) {
				xil_printf("Configuration Initialization failed %d\r\n", Status);
				return XST_FAILURE;
			}
			// Set interrupt handlers
			XAxiVdma_SetCallBack(psVdma, XAXIVDMA_HANDLER_GENERAL, ReadCallBack,
				(void *)psVdma, XAXIVDMA_READ);
			XAxiVdma_SetCallBack(psVdma, XAXIVDMA_HANDLER_ERROR,
				ReadErrorCallBack, (void *)psVdma, XAXIVDMA_READ);

		}
	}

	{ // TPG init
		//XV_tpg_Config *psTpgConfig;
		//XStatus Status;

		//psTpgConfig = XV_tpg_LookupConfig(TPG_DEVICE_ID);
		//if (NULL == psTpgConfig) {
		//	return XST_FAILURE;
		//}

		//Status = XV_tpg_CfgInitialize(psTpg, psTpgConfig, psTpgConfig->BaseAddress);
		//if (Status != XST_SUCCESS) {
		//	return XST_FAILURE;
		//}
		// Reset Test Pattern Generator
		//XV_tpg_Reset(psTpg);

		//XV_tpg_SetCallBack(psTpg, XV_TPG_HANDLER_ERROR, TpgErrorCallBack, psTpg);
	}

	{ // Read demo still from QSPI
		u32 addr = READ_ADDRESS_BASE - READ_WRITE_EXTRA_BYTES;
		XStatus status;
		xil_printf("\n\rLoading display image (this might take a while)... ");
		status = SpiFlashReadInPlace(psQspi, IMAGE_FLASH_BASE_ADDR, 1920*1080*(24/8), (u8*)(addr));
		xil_printf("done\r\n");
		if (status != XST_SUCCESS)
			return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*!
 * VideoShowPattern starts the VDMA read channel and sets the TPG to pass-through mode. The VDMA
 * is parked on the frame containing the static image loaded from the Flash.
 *
 * Parameters:
 * 	@param XAxiVdma* psVdma - initialized driver instance
 * 	@param XV_tpg* psTpg - initialized driver instance
 */
XStatus VideoShowPattern(XAxiVdma* psVdma)
{
	{ //Start VDMA read channel
		XAxiVdma_Config* psConf;
		XAxiVdma_FrameCounter sFrameCnt;
		XStatus status;
		u32 addr;
		int iFrm;
		vdma_context_t* pContext = &vdma_context;
		psConf = XAxiVdma_LookupConfig(DMA_DEVICE_ID);
		if (!psConf) {
			xil_printf("No video DMA found for ID %d\r\n", DMA_DEVICE_ID);
			return XST_FAILURE;
		}

		pContext->ReadCfg.HoriSizeInput = FRAME_HRES * (psConf->Mm2SStreamWidth>>3);
		pContext->ReadCfg.VertSizeInput = FRAME_VRES;
		pContext->ReadCfg.Stride = pContext->ReadCfg.HoriSizeInput;
		pContext->ReadCfg.FrameDelay = 0;
		pContext->ReadCfg.EnableCircularBuf = 0; //stay on one frame until we have write
		pContext->ReadCfg.EnableSync = 0; //no write yet, so we don't need to sync with anybody else
		pContext->ReadCfg.PointNum = 0;
		pContext->ReadCfg.EnableFrameCounter = 0;
		pContext->ReadCfg.FixedFrameStoreAddr = 0; //park it on 0 until we sync
		status = XAxiVdma_DmaConfig(psVdma, XAXIVDMA_READ, &pContext->ReadCfg);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
		addr = READ_ADDRESS_BASE;
		for (iFrm=0; iFrm<psVdma->MaxNumFrames; ++iFrm) {
			pContext->ReadCfg.FrameStoreStartAddr[iFrm] = addr;
			addr += pContext->ReadCfg.HoriSizeInput * pContext->ReadCfg.VertSizeInput;
		}
		status = XAxiVdma_DmaSetBufferAddr(psVdma, XAXIVDMA_READ, pContext->ReadCfg.FrameStoreStartAddr);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
		sFrameCnt.ReadDelayTimerCount = 1; //how many periods after the end of the last line, 0=disable
		sFrameCnt.ReadFrameCount = 60; // how many frames will result in an interrupt
		sFrameCnt.WriteDelayTimerCount = 1; //how many periods after the end of the last line, 0=disable
		sFrameCnt.WriteFrameCount = 60; // how many frames will result in an interrupt
		status = XAxiVdma_SetFrameCounter(psVdma, &sFrameCnt);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}

		//Enable read channel error and frame count interrupts
		XAxiVdma_IntrEnable(psVdma, XAXIVDMA_IXR_FRMCNT_MASK|XAXIVDMA_IXR_ERROR_MASK, XAXIVDMA_READ);

		//Start read channel
		status = XAxiVdma_DmaStart(psVdma, XAXIVDMA_READ);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
	    DemoWaitMs(100); //Wait for the line buffers to fill up
	}

	{ //Set TPG to pass-through
		/*XV_tpg_Set_bckgndId(psTpg, XTPG_BKGND_LAST);
		XV_tpg_Set_ovrlayId(psTpg, 0);
		//XV_tpg_SetPattern(psTpg, XTPG_PASS_THROUGH);//XTPG_TARAN_BARS);//
		XV_tpg_Set_height(psTpg,FRAME_VRES);
		XV_tpg_Set_width(psTpg,FRAME_HRES);
		XV_tpg_WriteReg(psTpg->Config.BaseAddress, XV_TPG_CTRL_ADDR_AP_CTRL, 0x81);
		//psTpg->HSize = FRAME_HRES;
		//psTpg->VSize = FRAME_VRES;
		//XTpg_Setup(psTpg);
		//XV_tpg_ruptEnable(psTpg, ); //Enable error interrupt
		XV_tpg_Set_enableInput(psTpg, 1);
		// Go
		XV_tpg_Start(psTpg);*/
	}

	return XST_SUCCESS;
}

/*
 *  Function: fnPS2MouseShow
 *      Enables/disables mouse overlay on video output.
 *
 *	Parameters:
 *      int fVisible = 0 to disable the mouse pointer, any other value to enable
 *
 *
*/
void fnPS2MouseShow(int fVisible)
{
	Xil_Out32(PS2_MOUSE_BASEADDR + 0x0, fVisible ? 1 : 0);
}
