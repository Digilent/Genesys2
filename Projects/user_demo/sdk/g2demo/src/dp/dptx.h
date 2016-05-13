////////////////////////////////////////////////////////////////////////////////
/// @file dptx.h
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
/// @desciption
///
/// @note
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

#ifndef DPTX_H_
#define DPTX_H_

//////////////////////////////// Include Files /////////////////////////////////
#include "xstatus.h"
#include "xdp.h"
#include "xdp_hw.h"

///////////////////////////// Constant Definitions /////////////////////////////
/// The link rate setting to begin link training with. Valid values are:
/// XDPTX_LINK_BW_SET_540GBPS, XDPTX_LINK_BW_SET_270GBPS, and
/// XDPTX_LINK_BW_SET_162GBPS.
static const u8 kbyTrainUseLinkRate		= XDP_TX_LINK_BW_SET_540GBPS;

/// The lane count setting to begin link training with. Valid values are:
/// XDPTX_LANE_COUNT_SET_4, XDPTX_LANE_COUNT_SET_2, and XDPTX_LANE_COUNT_SET_1.
static const u8 kbyTrainUseLaneCount	= XDP_TX_LANE_COUNT_SET_4;

/// If set to TRUE, kTrainUseLinkRate and kTrainUseLaneCount will be ignored.
/// Instead, the maximum common link capabilities between the DisplayPort TX core
/// and the sink device will be used when establishing a link.
/// If set to FALSE, kTrainUseLinkRate and kTrainUseLaneCount will determine the
/// link rate and lane count settings that the link training process will begin
/// with.
static const u8 kbTrainUseMaxLink		= TRUE;

///////////////////////////// Function Prototypes //////////////////////////////
XStatus DptxInit(XDptx* p_pSDptx, u16 p_wDeviceId);


#endif /* DPTX_H_ */
