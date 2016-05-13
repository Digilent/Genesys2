/******************************************************************************
 * @file oled.h
 *
 * @authors Mihaita Nagy
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
 *
 * @note
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date        Changes
 * ----- ------------ ----------- --------------------------------------------
 * 1.00  Mihaita Nagy 2015-Jan-15 First release
 *
 * </pre>
 *
 *****************************************************************************/

#ifndef OLED_H_
#define OLED_H_

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xgpio.h"
#include "xspi.h"
#include "microblaze_sleep.h"

/************************** Constant Definitions *****************************/
// Display buffer size for the OLED display
// 32 x 128 bit monochrome = 512 Bytes
#define OLED_DISPLAY_BUF_SZ		512
// 128 bits wide and current char width is 8 bit
#define OLED_MAX_LINE_LEN		16
#define OLED_MAX_ROW			4
// Number of display pages in OLED controller
#define OLED_MAX_PG_CNT			4
#define OLED_CONTROLLER_PG_SZ	128

/************************** Type Definitions *****************************/
// Type for the pixel array
typedef struct {
	u8 rgbPixmap[OLED_DISPLAY_BUF_SZ];
} sPixmap_t;

/************************** Function Prototypes ******************************/
XStatus fnOledDriverInit(XSpi *psSpi, XGpio *psGpio);
XStatus fnOledDriverOptions(XSpi *psSpi);
XStatus fnOledDisplayInit(XSpi *psSpi, XGpio *psGpio);
XStatus fnOledStringToDisplay(XSpi *psSpi, XGpio *psGpio,
		const char *pchScreenBuf);
void fnOledSpiIsr(void *pvInst, u32 u32StatusEvent, unsigned int uwByteCnt);
XStatus fnOledPixelToDisplay(XSpi *psSpi, XGpio *psGpio,
		const sPixmap_t *psScreenBuf);

#endif /* OLED_H_ */
