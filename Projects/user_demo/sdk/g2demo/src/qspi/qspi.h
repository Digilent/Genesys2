/******************************************************************************
 * @file qspi.h
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

#ifndef QSPI_H_
#define QSPI_H_

#include "xspi.h"

#define READ_WRITE_EXTRA_BYTES		9 /* Read/Write extra bytes = command + 4 byte address + 0 dummy (LC=11, <=50MHz) + 4 whodafrakknows */
#define OTPR_EXTRA_BYTES		5 //cmd + 24-bit address + 1 dummy
#define MAC_OTP_ADDR			0x20 //Base address of MAC address (6 bytes)
#define MAC_OUI_DIGILENT 		0x00183E

XStatus init_qspi(XSpi *psQSpi);
XStatus SpiFlashWaitForFlashReady(XSpi *psQSpi);
XStatus SpiFlashGetStatus(XSpi *psQSpi);
XStatus SpiFlashGetControl(XSpi *psQSpi);
XStatus SpiFlashRead(XSpi *psQSpi, u32 Addr, u32 ByteCount);
XStatus SpiFlashReadInPlace(XSpi *psQspi, u32 fromAddr, u32 cbLen, u8* pTo);
XStatus SpiReadOTP(XSpi *psQSpi, u32 Addr, u8* pbBuf, u32 cbLen);

#endif /* QSPI_H_ */
