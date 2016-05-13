/******************************************************************************
 * @file INA.c
 * This is a power INA219 validation file
 *
 * @authors Cristian Ignat
 *
 * @date 2015-Apr-04
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
 * Ver   Who          	Date        Changes
 * ----- ------------ ----------- --------------------------------------------
 * 1.00  Cristian Ignat 2015-Apr-04 First release
 *
 * </pre>
 *
 *****************************************************************************/

/***************************** Include Files *********************************/
#include "INA.h"
#include "xparameters.h"
#include <string.h>

// INA219 IIC device ID as defined in xparameters.h
#define SYS_IIC_BASEADDR XPAR_POWER_IIC_BASEADDR

// Number of used INA219 devices
#define	ina_number 6

// Commands for INA219
#define CR_ADDR 0x00
#define PWRR_ADDR 0x03
#define CURR_ADDR 0x04
#define BVR_ADDR 0x02
#define SVR_ADDR 0x01
#define CALR_ADDR 0x05

#define CR_SADC_MASK 0x0078
#define CR_RST_MASK 0x8000
#define BVR_CNVR_MASK 0x2

#define CR_SADC_AVG16 0x0060
#define CR_SADC_12B 0x0018


#define EXIT_ON_FAIL(result, msg) if (XST_SUCCESS != (result)) \
	{\
		xil_printf(msg);\
		return 1;\
	}

/************************** Variable Definitions *****************************/
// Signal names
char *arr_str_INA_names[] = {"vcc1v0", "vcc1v5", "vcc1v8", "vadj", "vcc3v3", "vcc5v0"};

XStatus WriteINA(u8 slaveAddr, u8 regAddr, u16 dwData);
XStatus ReadINA(u8 slaveAddr, u8 regAddr, s16* pdwData);


// INA_ic structure with the following members:
//		- address
//		- configuration register
//		- Sense resistor value (in mOhm)
//		- current divider
//		- voltage expected value
//		- voltage range
//		- current expected value
//		- current range
//		- name
struct INA {
	u8 addr;
	u16 config_reg;
	u8 rSense;
	u8 currentDivider;
	s16 voltageExpectedValue;
	s16 voltageRange;
	s16 currentExpectedValue;
	s16 currentRange;
	char *name;
} INA_ic[ina_number] = {
		{0X45, 0x4000, 5, 2, 1000, 30, 940, 120, "vcc1v0"},
		{0X4C, 0x4000, 5, 2, 1500, 150, 560, 40, "vcc1v5"},
		{0X48, 0x4000, 5, 2, 1800, 180, 3440, 340, "vcc1v8"},
		{0X41, 0x4000, 5, 2, 1200, 120, 0, 20, "vadj"},
		{0X44, 0x4000, 5, 2, 3300, 330, 674, 61, "vcc3v3"},
		{0X40, 0x4000, 5, 2, 5000, 500, 218, 15, "vcc5v0"}
	};

/************************** Function Definitions *****************************/

/******************************************************************************
 * This function reads a register from INA219 chip.
 *
 * @param	slaveAddr is the slave address
 * @param	regAddr is the register address
 * @param	pdwData is the pointer to the data readed
 *
 * @return	XST_SUCCESS - Everything went well
 * 			XST_FAILURE - Failure
 *****************************************************************************/
XStatus ReadINA(u8 slaveAddr, u8 regAddr, s16* pdwData)
{
	u8 rgbData[2];
	u32 dwStatusReg;

	// Make sure all the Fifo's are cleared and Bus is Not busy.
	while (((dwStatusReg = XIic_ReadReg(SYS_IIC_BASEADDR,
				XIIC_SR_REG_OFFSET)) &
				(XIIC_SR_RX_FIFO_EMPTY_MASK |
				XIIC_SR_TX_FIFO_EMPTY_MASK |
		 		XIIC_SR_BUS_BUSY_MASK)) !=
		 		(XIIC_SR_RX_FIFO_EMPTY_MASK |
				XIIC_SR_TX_FIFO_EMPTY_MASK)) ;

	//Write reg address first (1 byte)
	if (1 != XIic_Send(SYS_IIC_BASEADDR, slaveAddr, &regAddr, 1, XIIC_STOP))
	{
		return XST_FAILURE;
	}

	//Read necessary number of bytes
	if (sizeof(rgbData)/sizeof(rgbData[0]) != XIic_Recv(SYS_IIC_BASEADDR, slaveAddr, rgbData, sizeof(rgbData)/sizeof(rgbData[0]), XIIC_STOP))
	{
		return XST_FAILURE;
	}

	*pdwData = (rgbData[0] << 8) | rgbData[1];

	return XST_SUCCESS;
}


/******************************************************************************
 * This function write a register on the INA219 chip.
 *
 * @param	slaveAddr is the slave address
 * @param	regAddr is the register address
 * @param	dwData is the data write
 *
 * @return	XST_SUCCESS - Everything went well
 * 			XST_FAILURE - Failure
 *****************************************************************************/
XStatus WriteINA(u8 slaveAddr, u8 regAddr, u16 dwData)
{
	u8 rgbData[3];
	u32 dwStatusReg;

	rgbData[0] = regAddr;
	rgbData[1] = dwData >> 8;
	rgbData[2] = dwData;

	// Make sure all the Fifo's are cleared and Bus is Not busy.
	while (	(	(	dwStatusReg = XIic_ReadReg(SYS_IIC_BASEADDR,
					XIIC_SR_REG_OFFSET)
				) &
				(	XIIC_SR_RX_FIFO_EMPTY_MASK |
					XIIC_SR_TX_FIFO_EMPTY_MASK |
					XIIC_SR_BUS_BUSY_MASK
				)
			) !=
			(		XIIC_SR_RX_FIFO_EMPTY_MASK |
					XIIC_SR_TX_FIFO_EMPTY_MASK
			)
		) ;

	//Write necessary number of bytes
	if (sizeof(rgbData)/sizeof(rgbData[0]) != XIic_Send(SYS_IIC_BASEADDR, slaveAddr, rgbData, sizeof(rgbData)/sizeof(rgbData[0]), XIIC_STOP))
	{
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


/******************************************************************************
 * This function returns the voltage/current when the convertion is done.
 *
 * @param	slaveAddr is the slave address
 * @param	regAddr is the register address
 * @param	data is the pointer to the data readed
 *
 * @return	XST_SUCCESS - Everything went well
 * 			XST_FAILURE - Failure
 *****************************************************************************/
XStatus dataRead(u8 slaveAddr, u8 regAddr, s16* data)
{
    s16 dwPower, dwBusVoltage, rgdwShunt = 0;

    if (XST_SUCCESS != ReadINA(slaveAddr, BVR_ADDR, &dwBusVoltage))
    {
		return XST_FAILURE;
    }
    while (!(dwBusVoltage & BVR_CNVR_MASK) || rgdwShunt == 0)
    {
		if ((XST_SUCCESS == ReadINA(slaveAddr, regAddr, &rgdwShunt)) && (XST_SUCCESS == ReadINA(slaveAddr, BVR_ADDR, &dwBusVoltage)))
		{
			//See, if this is a new conversion
			if (dwBusVoltage & BVR_CNVR_MASK)
			{
				//Reset CNVR bit by reading power register
				ReadINA(slaveAddr, PWRR_ADDR, &dwPower);
			}
		}
		else
		{
			return XST_FAILURE;
		}
    }

    *data = rgdwShunt;

    return XST_SUCCESS;
}

/******************************************************************************
 * This function returns the voltage value.
 *
 * @param	name is the name of the signal measurement
 *
 * @return	voltage value
 *****************************************************************************/
s16 fnGetINAVoltage(char *name)
{
	s16 dwConf;
	s16 VValue = 0;
	u8 i;

	for(i = 0; i < ina_number; i ++)
		if(strcmp(name, arr_str_INA_names[i]) == 0)
			break;

	if(i == ina_number)
	{
		xil_printf("Wrong INA Name: %s\n", name);
		return 0;
	}

	//Reset
	EXIT_ON_FAIL(WriteINA(INA_ic[i].addr, CR_ADDR, CR_RST_MASK), "\r\nConfiguration failed.");
	EXIT_ON_FAIL(ReadINA(INA_ic[i].addr, CR_ADDR, &dwConf), "\r\nConfiguration failed.");

	// set the 16 reads averaging
	dwConf &= ~CR_SADC_MASK;
	dwConf |= CR_SADC_AVG16;

	EXIT_ON_FAIL(WriteINA(INA_ic[i].addr, CR_ADDR, dwConf), "\r\nConfiguration failed.");

	//read voltage
	EXIT_ON_FAIL(dataRead(INA_ic[i].addr, BVR_ADDR, &VValue), "\r\nRead failed.");

	return (VValue >> 3) * 4;
}

/******************************************************************************
 * This function returns the current value.
 *
 * @param	name is the name of the signal measurement
 *
 * @return	current value
 *****************************************************************************/
s16 fnGetINACurrent(char *name)
{
	s16 CValue = 0;
	u8 i;

	for(i = 0; i < ina_number; i ++)
		if(strcmp(name,arr_str_INA_names[i]) == 0)
			break;

	if(i == ina_number)
	{
		xil_printf("Wrong INA Name: %s\n", name);
		return 0;
	}

	EXIT_ON_FAIL(WriteINA(INA_ic[i].addr, CR_ADDR, CR_RST_MASK), "Configuration failed.");

	EXIT_ON_FAIL(WriteINA(INA_ic[i].addr, CR_ADDR, 0x0867), "\r\nConfiguration failed.");
	EXIT_ON_FAIL(WriteINA(INA_ic[i].addr, CALR_ADDR, INA_ic[i].config_reg), "\r\nConfiguration failed.");

	//read current
	EXIT_ON_FAIL(dataRead(INA_ic[i].addr, CURR_ADDR, &CValue), "\r\nRead failed.");

	return CValue / INA_ic[i].currentDivider;
}

/******************************************************************************
 * This function check if the readed current and voltage for all INA219 chips
 * are in range.
 *
 * @param	none
 *
 * @return	XST_SUCCESS - Everything went well
 * 			XST_FAILURE - Failure
 *****************************************************************************/
XStatus fnValidateINA()
{
	u8 ina_counter = 0;
	s16 CValue = 0, VValue = 0;
	XStatus ret = XST_SUCCESS;

	for(ina_counter = 0; ina_counter < ina_number; ina_counter++)
	{
		VValue = fnGetINAVoltage(arr_str_INA_names[ina_counter]);

		if(INA_ic[ina_counter].voltageExpectedValue + INA_ic[ina_counter].voltageRange < VValue ||
				INA_ic[ina_counter].voltageExpectedValue - INA_ic[ina_counter].voltageRange > VValue)
		{
			xil_printf("voltage for %s is out of range. Expected %d, measured %d", INA_ic[ina_counter].name, INA_ic[ina_counter].voltageExpectedValue, VValue);
			ret = XST_FAILURE;
		}

		CValue = fnGetINACurrent(arr_str_INA_names[ina_counter]);

		if(INA_ic[ina_counter].currentExpectedValue + INA_ic[ina_counter].currentRange < CValue ||
				INA_ic[ina_counter].currentExpectedValue - INA_ic[ina_counter].currentRange > CValue)
		{
			xil_printf("current for %s is out of range. Expected %d, measured %d", INA_ic[ina_counter].name, INA_ic[ina_counter].currentExpectedValue, CValue);
			ret = XST_FAILURE;
		}
	}

	return ret;
}

