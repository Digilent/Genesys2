////////////////////////////////////////////////////////////////////////////////
/// @file dptx.c
///
/// @authors Mihaita Nagy
///
/// @date 2015-Jun-18
///
/// @copyright
/// (c) 2015 Copyright Digilent Incorporated
/// All Rights Reserved
///
/// This program is free software; distributed under the terms of BSD 3-clause
/// license ("Revised BSD License", "New BSD License", or "Modified BSD License")
///
/// Redistribution and use in source and binary forms, with or without modification,
/// are permitted provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this
///    list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
/// 3. Neither the name(s) of the above-listed copyright holder(s) nor the names
///    of its contributors may be used to endorse or promote products derived
///    from this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// @desciption This source file contains functions that creates one main link
/// and trains it using Xilinx's XDptx driver.
///
/// @note It requires an interrupt controller connected to the processor and the
/// DisplayPort TX core in the system.
/// Inspired by the examples provided by Xilinx in their driver pack.
///
/// <pre>
/// MODIFICATION HISTORY:
///
/// Ver   Who          Date        Changes
/// ----- ------------ ----------- --------------------------------------------
/// 1.00  Mihaita Nagy 2015-Jun-18 First release
///
/// </pre>
///
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Include Files /////////////////////////////////
#include "dptx.h"
#include "xparameters.h"
#include "string.h"
#include "xintc.h"
#include "../demo.h"

///////////////////////////// Function Prototypes //////////////////////////////
XStatus DptxStartLink(XDptx* p_pSDptx);
static void DptxStartVideoStreaming(XDptx* p_pSDptx);
static void DptxHpdEventHandler(void* p_pInst);
static void DptxHpdPulseHandler(void* p_pInst);

///////////////////////////// Variable Definitions /////////////////////////////
extern volatile sDemo_t Demo;
volatile sDemo_t* pSDemo = &Demo; ///< Points to the Demo related stuff

///////////////////////////// Function Definitions /////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// This function will setup and initialize the DisplayPort TX core. The core's
/// configuration parameters will be retrieved based on the configuration to the
/// DisplayPort TX core instance with the specified device ID.
/// This function also registers HPD interrupt handlers so interrupt must already
/// be initialized.
/// @param	p_pSDptx is a pointer to the XDptx instance.
/// @param	p_wDeviceId is the unique device ID of the DisplayPort TX core
///			instance.
/// @return	None.
////////////////////////////////////////////////////////////////////////////////
XStatus DptxInit(XDptx* p_pSDptx, u16 p_wDeviceId)
{
	XDptx_Config* pSConfig = NULL;
	XStatus Status = XST_FAILURE;

	/// Obtain the device configuration for DisplayPort TX core
	pSConfig = XDptx_LookupConfig(p_wDeviceId);
	if (pSConfig == NULL)
	{
		if (pSDemo->u8Verbose)
		{
			xil_printf("%s function failed in %s file at %d line\r\n",
					__func__, __FILE__, __LINE__);
		}
		return XST_FAILURE;
	}

	/// Copy the device configuration into the p_pSDptx's Config structure
	XDptx_CfgInitialize(p_pSDptx, pSConfig, pSConfig->BaseAddr);

	/// Now initialize the TX core
	Status = XDptx_InitializeTx(p_pSDptx);
	if (Status != XST_SUCCESS)
	{
		if (pSDemo->u8Verbose)
		{
			xil_printf("%s function failed in %s file at %d line\r\n",
					__func__, __FILE__, __LINE__);
		}
		return XST_FAILURE;
	}

	/// Disable MST, we're using SST mode
	XDp_TxMstCfgModeDisable(p_pSDptx);
	/// @brief
	/// If enabled, the link training process will continue training despite
	/// failing by attempting training at a reduced link rate. It will also
	/// continue attempting to train the link at a reduced lane count if needed.
	/// With this option enabled, link training will return failure only when
	/// all link rate and lane count combinations have been exhausted - that is,
	/// training fails using 1-lane and a 1.62Gbps link rate.
	XDp_TxEnableTrainAdaptive(p_pSDptx, TRUE);

	/// @brief
	/// A value of 1 is used to indicate that the DisplayPort output path has a
    /// redriver on the board that adjusts the voltage swing and pre-emphasis
	/// levels that are output from the FPGA. If this is the case, the voltage
	/// swing and pre-emphasis values supplied to the DisplayPort TX core will
	/// be evenly distributed among the available levels as specified in the IP
	/// documentation.
    /// Otherwise, a value of 0 is used to indicate that no redriver is present.
	/// In order to meet the necessary voltage swing and pre-emphasis levels
	/// required by a DisplayPort sink device, the level values specified to the
	/// DisplayPort TX core will require some compensation.
    /// @note
	/// There are 16 possible voltage swing levels and 32 possible pre-emphasis
    /// levels in the DisplayPort TX core that will be mapped to 4 possible
    /// voltage swing and 4 possible pre-emphasis levels in the sink device.
	XDp_TxSetHasRedriverInPath(p_pSDptx, TRUE);

	/// Set the HPD handlers
	XDp_TxSetHpdEventHandler(p_pSDptx, &DptxHpdEventHandler, p_pSDptx);
	XDp_TxSetHpdPulseHandler(p_pSDptx, &DptxHpdPulseHandler, p_pSDptx);

	return XST_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// This function is the main function of the XDptx interrupt example. If the
/// DptxIntrExample function, which sets up the system succeeds, this function
/// will wait for interrupts. Once a connection event or pulse is detected, link
/// training will commence (if needed) and a video stream will start being sent
/// over the main link.
/// @param	p_pInst is a pointer to the XDptx instance.
/// @return XST_FAILURE if the interrupt example was unsuccessful - system
///		  	setup failed.
/// @note	Unless setup failed, main will never return since
///			DptxIntrExample is blocking (it is waiting on interrupts for
///			Hot-Plug-Detect (HPD) events.
////////////////////////////////////////////////////////////////////////////////
XStatus DptxStartLink(XDptx* p_pSDptx)
{
	u32 dwVsLevelTx = 0;
	u32 dwPeLevelTx = 0;
	u8 byLaneCount = 0;
	u8 byLinkRate = 0;
	XStatus Status = XST_FAILURE;

	/// Disable HPD pulse interrupts during link training (xapp1178)

	u32 dwIntrMask = XDp_ReadReg(p_pSDptx->Config.BaseAddr, XDP_TX_INTERRUPT_MASK);
	XDp_WriteReg(p_pSDptx->Config.BaseAddr, XDP_TX_INTERRUPT_MASK, dwIntrMask |
			XDP_TX_INTERRUPT_MASK_HPD_PULSE_DETECTED_MASK);

	/// Obtain the capabilities of the sink device by reading its DPCD
	Status = XDp_TxGetRxCapabilities(p_pSDptx);
	if (Status != XST_SUCCESS)
	{
		if (pSDemo->u8Verbose)
		{
			xil_printf("%s function failed in %s file at %d line\r\n",
					__func__, __FILE__, __LINE__);
		}
		/// Enable HPD pulse interrupts
		XDp_WriteReg(p_pSDptx->Config.BaseAddr, XDP_TX_INTERRUPT_MASK, dwIntrMask);

		return XST_FAILURE;
	}

	/// Train with the maximum settings or use the custom ones
	if (kbTrainUseMaxLink)
	{
		byLaneCount = p_pSDptx->TxInstance.LinkConfig.MaxLaneCount;
		byLinkRate = p_pSDptx->TxInstance.LinkConfig.MaxLinkRate;
	}
	else
	{
		byLaneCount = kbyTrainUseLaneCount;
		byLinkRate = kbyTrainUseLinkRate;
	}

	XDp_TxSetDownspread(p_pSDptx, TRUE);
	XDp_TxSetEnhancedFrameMode(p_pSDptx, TRUE);

	/// Configure the main link based on the maximum common capabilities if the
	/// DisplayPort TX core and the sink device at the other end
	if (kbTrainUseMaxLink)
	{
		Status = XDp_TxCfgMainLinkMax(p_pSDptx);
		if (Status != XST_SUCCESS)
		{
			if (pSDemo->u8Verbose)
			{
				xil_printf("%s function failed in %s file at %d line\r\n",
						__func__, __FILE__, __LINE__);
				xil_printf("Sink device not connected!\r\n");
			}
			/// Enable HPD pulse interrupts
			XDp_WriteReg(p_pSDptx->Config.BaseAddr, XDP_TX_INTERRUPT_MASK,
					dwIntrMask);

			return XST_FAILURE;
		}
	}
	else ///< Use the custom settings instead
	{
		XDp_TxSetLinkRate(p_pSDptx, byLinkRate);
		XDp_TxSetLaneCount(p_pSDptx, byLaneCount);
	}

	/// Finally, training the link
	Status = XDp_TxEstablishLink(p_pSDptx);
	if (Status != XST_SUCCESS)
	{
		if (pSDemo->u8Verbose)
		{
			xil_printf("%s function failed in %s file at %d line\r\n",
					__func__, __FILE__, __LINE__);
			xil_printf("Link training failed\r\n");
		}
		/// Enable HPD pulse interrupts
		XDp_WriteReg(p_pSDptx->Config.BaseAddr, XDP_TX_INTERRUPT_MASK, dwIntrMask);

		return XST_FAILURE;
	}

	/// Get the VSwing from the sink,
	dwVsLevelTx = XDptx_ReadReg(p_pSDptx->Config.BaseAddr,
			XDP_TX_PHY_VOLTAGE_DIFF_LANE_0);

	/// the Pre-Emphasis value
	dwPeLevelTx = XDptx_ReadReg(p_pSDptx->Config.BaseAddr,
			XDP_TX_PHY_POSTCURSOR_LANE_0);

	if (pSDemo->u8Verbose)
	{
		xil_printf("Link successfully trained at %s on %d lanes, VS=%d (TX=%d),"
				" PE=%d (TX=%d)\r\n",
				(p_pSDptx->TxInstance.LinkConfig.LinkRate == 0x0a) ? "2.7 Gbps" :
				(p_pSDptx->TxInstance.LinkConfig.LinkRate == 0x14) ? "5.4 Gbps" : "1.62 Gbps",
				p_pSDptx->TxInstance.LinkConfig.LaneCount,
				p_pSDptx->TxInstance.LinkConfig.VsLevel, dwVsLevelTx,
				p_pSDptx->TxInstance.LinkConfig.PeLevel, dwPeLevelTx);
	}

	/// Enable HPD pulse interrupts after link training
	XDp_WriteReg(p_pSDptx->Config.BaseAddr, XDP_TX_INTERRUPT_MASK, dwIntrMask);

	return XST_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// This function will start sending a video stream over the main link.
/// The input video stream has to be already configured and running.
/// The settings to be used are as follows:
///	- 8 bits per color.
///	- Video timing and screen resolution used:
///	- The connected monitor's preferred timing is used to determine the
///	  video resolution (and associated timings) for the stream.
/// @param	InstancePtr is a pointer to the XDptx instance.
/// @return	None.
/// @note	The Extended Display Identification Data (EDID) is read in order
///			to obtain the video resolution and timings. If this read fails,
///			a resolution of 640x480 is used at a refresh rate of 60Hz.
////////////////////////////////////////////////////////////////////////////////
static void DptxStartVideoStreaming(XDptx* p_pSDptx)
{
	XStatus Status = XST_FAILURE;
	u8 rgbyEdid[XDP_EDID_BLOCK_SIZE];

	/// Initialize rgbyEdid
	memset(rgbyEdid, XDP_EDID_BLOCK_SIZE, 0);

	/// Set the BPC (bits per color) value, if not set the default is 6
	XDp_TxCfgMsaSetBpc(p_pSDptx, XDP_TX_STREAM_ID1, 8);

	/// Set asynchronous clock mode
	XDp_TxCfgMsaEnSynchClkMode(p_pSDptx, XDP_TX_STREAM_ID1, FALSE);

	XDp_TxClearMsaValues(p_pSDptx, XDP_TX_STREAM_ID1);
	XDp_TxClearMsaValues(p_pSDptx, XDP_TX_STREAM_ID2);
	XDp_TxClearMsaValues(p_pSDptx, XDP_TX_STREAM_ID3);
	XDp_TxClearMsaValues(p_pSDptx, XDP_TX_STREAM_ID4);

	/// @note: Bug workaround: it seems that with version 2.0 of the dptx
	/// driver the USER_PIXEL_WIDTH (0x1B8) register of the DisplayPort TX
	/// core never gets set (0x0).
	p_pSDptx->TxInstance.MsaConfig->OverrideUserPixelWidth = 1;

	Status = XDp_TxGetEdid(p_pSDptx, rgbyEdid);
	if (Status == XST_SUCCESS)
	{
		//XDptx_CfgMsaUseEdidPreferredTiming(p_pSDptx, XDPTX_STREAM_ID0, rgbyEdid);
		if (pSDemo->u8Verbose)
		{
			xil_printf("Setting video resolution to 1080p... ");
		}
		XDp_TxCfgMsaUseStandardVideoMode(p_pSDptx, XDP_TX_STREAM_ID1,
				XDP_DPCD_DOWNSP_X_CAP_NON_EDID_ATTR_1920_1080_I_60);
		if (pSDemo->u8Verbose)
		{
			xil_printf("done.\r\n");
		}
	}
	else ///< Cannot set the desired resolution and so
	{    /// using the default lowest one
		if (pSDemo->u8Verbose)
		{
			xil_printf("Cannot set the desired resolution, "
					"changing to the default one (640x480p)\r\n");
		}
		XDp_TxCfgMsaUseStandardVideoMode(p_pSDptx, XDP_TX_STREAM_ID1,
				XDP_DPCD_DOWNSP_X_CAP_NON_EDID_ATTR_720_480_I_60);
	}

	/// Disable MST since we're using only one
	XDp_TxMstDisable(p_pSDptx);

	/// Disable the main stream to force sending IDLE patterns
	XDp_TxDisableMainLink(p_pSDptx);

	/// Reset the transmitter
	XDp_WriteReg(p_pSDptx->Config.BaseAddr, XDP_TX_SOFT_RESET,
			XDP_TX_SOFT_RESET_VIDEO_STREAM_ALL_MASK);
	XDp_WriteReg(p_pSDptx->Config.BaseAddr, XDP_TX_SOFT_RESET, 0x0);

	/// Set the DisplayPort TX video mode
	XDp_TxSetVideoMode(p_pSDptx, XDP_TX_STREAM_ID1);

	/// Enable main stream
	XDp_TxEnableMainLink(p_pSDptx);
}

////////////////////////////////////////////////////////////////////////////////
/// This function is called when a Hot-Plug-Detect (HPD) pulse is received by the
/// DisplayPort TX core. The XDPTX_INTERRUPT_STATUS_HPD_EVENT_MASK bit of the
/// core's XDPTX_INTERRUPT_STATUS register indicates that an HPD event has
/// occurred.
/// @param	p_pInst is a pointer to the XDptx instance.
/// @return	None.
/// @note	Use the XDptx_SetHpdPulseHandler driver function to set this
///			function as the handler for HPD pulses.
////////////////////////////////////////////////////////////////////////////////
static void DptxHpdEventHandler(void* p_pInst)
{
	XDptx* pXDptx = (XDptx *)p_pInst;
	XStatus Status = XST_FAILURE;

	if (XDp_TxIsConnected(pXDptx)) ///< Connected
	{
		if (pSDemo->u8Verbose)
		{
			xil_printf("DisplayPort HPD connected\r\n");
		}

		/// Configure and establish the link
		Status = DptxStartLink(pXDptx);
		if (Status != XST_FAILURE)
		{
			DptxStartVideoStreaming(pXDptx);
			pSDemo->sbLinkFailed = FALSE;
		}
		else
		{
			pSDemo->sbLinkFailed = TRUE;
		}
	}
	else ///< Disconnected
	{
		if (pSDemo->u8Verbose)
		{
			xil_printf("DisplayPort HPD disconnected\r\n");
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// This function is called when a Hot-Plug-Detect (HPD) pulse is received by the
/// DisplayPort TX core. The XDPTX_INTERRUPT_STATUS_HPD_PULSE_DETECTED_MASK bit
/// of the core's XDPTX_INTERRUPT_STATUS register indicates that an HPD event has
/// occurred.
/// @param	p_pInst is a pointer to the XDptx instance.
/// @return	None.
/// @note	Use the XDptx_SetHpdPulseHandler driver function to set this
///			function as the handler for HPD pulses.
////////////////////////////////////////////////////////////////////////////////
static void DptxHpdPulseHandler(void* p_pInst)
{
	XDptx* pXDptx = (XDptx *)p_pInst;
	XStatus Status = XST_FAILURE;

	/// HPD edge detected
	if (pSDemo->u8Verbose)
	{
		xil_printf("HPD edge detected\r\n");
	}

	/// Configure and establish the link
	Status = DptxStartLink(pXDptx);
	if (Status != XST_FAILURE)
	{
		DptxStartVideoStreaming(pXDptx);
		pSDemo->sbLinkFailed = FALSE;
	}
	else
	{
		pSDemo->sbLinkFailed = TRUE;
	}
}
