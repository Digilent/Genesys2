/******************************************************************************
 * @file demo.h
 *
 * This is the main Nexys Video Demo application.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date     Changes
 * ----- ------------ -------- -----------------------------------------------
 * 1.00  Mihaita Nagy 05/01/15 First release
 *
 * </pre>
 *
 *****************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xil_io.h"
#include "xparameters.h"

/************************** Constant Definitions *****************************/
#define RETURN_ON_FAILURE(x) if ((x) != XST_SUCCESS) return XST_FAILURE;

/**************************** Type Definitions *******************************/
typedef struct {
	u8 sbLinkFailed;
	u8 u8Verbose;
	u8 fUserIOEvent;
	u8 fBtnTimerCount;
	u8 fDebounceEvent;
	u8 fVideoEvent;
	u8 fAudioRecord;
	u8 fAudioPlayback;
	u8 fDmaError;
	u8 fDmaS2MMEvent;
	u8 fDmaMM2SEvent;
	int fDVIClockLock;
	u8 fVtdLock;
	char chBtn;
	u8 bSwitches;
	u8 fLinkEvent;
	u8 fLinkStatus;
	int linkSpeed;
} sDemo_t;

/************************** Function Prototypes ******************************/
void fnEthPHYIsr(void *CallbackRef);
void DemoWaitMs(u32 p_dwMilliSeconds);


#endif /* MAIN_H_ */
