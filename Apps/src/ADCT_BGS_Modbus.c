/**
 ******************************************************************************
 * @file    ADCT_BGS_Modbus.c
 * @author  ADCT Team - VuNTDH
 * @version V0.0.1
 * @date    23-Mar-2017
 * @brief   MODBUS module for SNMP project.
 *
 @verbatim
 ==============================================================================
 ##### How to use this driver #####
 ==============================================================================

 @endverbatim
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 ADCT TEAM</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
#include "ADCT_BGS_Modbus.h"
#include "ADCT_BGS_DataTypes.h"
#include "ADCT_Relay.h"
#include "ADCT_AT24C64.h"
#include <PZEM004T.h>
volatile uint16_t temp_setting;
volatile uint8_t ADCT_BGS_REBOOT_BOARD ;
extern ADCT_BGS_RTC_Types g_ADCT_BGS_Current_RTC;
extern FPT_SystemStatus GstSystemStatus;
extern FPT_SystemWarning GstSystemWarning;
extern FPT_UserSetting GstSystemSetting;
extern ADCT_BGS_TimerManagedDatatypes g_ADCT_BGS_TimerManager;
#ifdef SUPPORT_PHSENSOR
extern double ADCT_PHVal;
#endif
volatile FPT_UserSetting GTempSetting;
static USHORT usRegHoldingStart = ADCT_REG_HOLDING_START;
static volatile USHORT usRegHoldingBuf[REG_HOLDING_NREGS];
void ADCT_BGS_MODBUS_GetSetting(void);
static void ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(
		ADCT_RegHoldingAddressOffset_t offset, uint16_t data);

eMBErrorCode ADCT_BGS_MODBUS_RS485_Init(void) {
	eMBErrorCode eStatus;
	eStatus = eMBInit(MB_RTU, ADCT_BGS_MODBUS_ADDR, 0, ADCT_BGS_BAULDRATE,
			MB_PAR_NONE);
	eStatus = eMBEnable();
	return eStatus;
}

static void ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(
		ADCT_RegHoldingAddressOffset_t offset, uint16_t data) {
	usRegHoldingBuf[(uint16_t) (offset)] = data;
}
static void ADCT_AC_MODBUS_UpdateData(void )
{
	uint32_t Significand ;
	uint8_t Exponent;

	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(VOLTAGE_RMS, (uint16_t)(GstSystemStatus.acData.voltage));

	ADCT_AC_ConvertCurrent(GstSystemStatus.acData.current, &Significand, &Exponent);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(CURRENT_SIG, Significand);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(CURRENT_EXP, Exponent);

	ADCT_AC_Convert_Power(GstSystemStatus.acData.power, &Significand, &Exponent);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(ACTIVE_POWER_SIG, Significand);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(ACTIVE_POWER_EXP, Exponent);

	ADCT_AC_Convert_Energy(GstSystemStatus.acData.energy, &Significand, &Exponent);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(ENERGY_SIG, Significand);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(ENERGY_EXP, Exponent);

}
#ifdef SUPPORT_PHSENSOR
static void ADCT_PH_MODBUS_UpdateData(void)
{
	uint16_t Significand;
	Significand = (uint16_t)(ADCT_PHVal * 100);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(PH_DATA, Significand);
}
#endif
void ADCT_BGS_MODBUS_RS485_UpdateData(void) {
	ENTER_CRITICAL_SECTION();
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(USTEMP_MAIN,
			GstSystemStatus.dhtData.usTemp_Main);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(USHUMID_MAIN,
			GstSystemStatus.dhtData.usHumid_Main);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(USTEMP_SUB,
			GstSystemStatus.dhtData.usTemp_Sub);

	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(USHUMID_SUB,
			GstSystemStatus.dhtData.usHumid_Sub);
	//Update data for PH sensor
#ifdef SUPPORT_PHSENSOR
	ADCT_PH_MODBUS_UpdateData();
#endif
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(
			UCFIRE, ReadBit(GstSystemWarning.othWarn, WARN_FIRE));
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(
			UCSMOKE, ReadBit(GstSystemWarning.othWarn, WARN_SMOKE));
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(
			UCWATER, ReadBit(GstSystemWarning.othWarn, WARN_WATER));
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(DOOR1,
			GstSystemStatus.ioData.ucDoor[0]);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(DOOR2,
			GstSystemStatus.ioData.ucDoor[0]);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(DOOR3,
			GstSystemStatus.ioData.ucDoor[0]);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(DOOR4,
			GstSystemStatus.ioData.ucDoor[0]);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(DOOR5,
			GstSystemStatus.ioData.ucDoor[0]);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg( BUGLAR, GstSystemStatus.ioData.ucBurglar);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(AIR1, !ADCT_BGS_AIR_1_2_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(AIR2, !ADCT_BGS_AIR_1_2_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(AIR3, !ADCT_BGS_AIR_3_4_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(AIR4, !ADCT_BGS_AIR_3_4_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(FAN1, ADCT_BGS_FAN_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(FAN2, ADCT_BGS_FAN_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(FAN3, ADCT_BGS_FAN_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(FAN4, ADCT_BGS_FAN_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(BUZZER, ADCT_BGS_BUZZER_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(LED, ADCT_BGS_LAMP_READ());
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(ALARM_WARNING_HIGH,
			(uint16_t)((GstSystemWarning.othWarn >> 16)&(0xFFFF)));
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(ALARM_WARNING_LOW,
			(uint16_t)(GstSystemWarning.othWarn &(0xFFFF)));

	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_RTC_SEC, g_ADCT_BGS_Current_RTC.RTC_Sec);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_RTC_MIN, g_ADCT_BGS_Current_RTC.RTC_Min);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_RTC_HOUR, g_ADCT_BGS_Current_RTC.RTC_Hour);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_RTC_DAY, g_ADCT_BGS_Current_RTC.RTC_Mday);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_RTC_MONTH, g_ADCT_BGS_Current_RTC.RTC_Mon);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_RTC_YEAR, g_ADCT_BGS_Current_RTC.RTC_Year);

	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_SW_SEC, g_ADCT_BGS_TimerManager.SWitchingTimer.second);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_SW_MINUTE, g_ADCT_BGS_TimerManager.SWitchingTimer.minute);
	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_SW_HOUR, g_ADCT_BGS_TimerManager.SWitchingTimer.hour);

	ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(US_SW_HOUR, g_ADCT_BGS_TimerManager.SWitchingTimer.hour);

	ADCT_AC_MODBUS_UpdateData();
	GTempSetting.usLowTemperatureT1 = usRegHoldingBuf[USLOWTEMPERATURET1];
	GTempSetting.usHighTemperatureT2 = usRegHoldingBuf[USHIGHTEMPERATURET2];
	GTempSetting.usFanTemperatureT3 = usRegHoldingBuf[USFANTEMPERATURET3];
	GTempSetting.usLowHumidityH1 = usRegHoldingBuf[USLOWHUMIDITYH1];
	GTempSetting.usHighHumidityH2 = usRegHoldingBuf[USHIGHHUMIDITYH2];
	GTempSetting.usLowMainVoltage = usRegHoldingBuf[USLOWMAINVOLTAGE];
	GTempSetting.usHighMainVoltage = usRegHoldingBuf[USHIGHMAINVOLTAGE];
	GTempSetting.usGenFuelLowValue = usRegHoldingBuf[USGENFUELLOWVALUE];
	GTempSetting.usGenVoltageAlarmValue = usRegHoldingBuf[USGENVOLTAGEALARMVALUE];
	GTempSetting.usGenBatteryVoltageAlarmValue =
			usRegHoldingBuf[USGENBATTERYVOLTAGEALARMVALUE];
	GTempSetting.usGenCoolantTempAlarmValue =
			usRegHoldingBuf[USGENCOOLANTTEMPALARMVALUE];
	GTempSetting.usGenEngineSpeedAlarmValue =
			usRegHoldingBuf[USGENENGINESPEEDALARMVALUE];
	GTempSetting.usGenOilPressureAlarmValue =
			usRegHoldingBuf[USGENOILPRESSUREALARMVALUE];
	GTempSetting.ulSwitchingTime = usRegHoldingBuf[ULSWITCHINGTIME];
	GTempSetting.ulMinSwitchingTime = usRegHoldingBuf[ULMINSWITCHINGTIME];
	GTempSetting.ulStayOffTime = usRegHoldingBuf[ULSTAYOFFTIME];
	GTempSetting.ucEnableAlarm = usRegHoldingBuf[UCENABLEALARM];
	GTempSetting.ucDoorBurglarEnableAlarm =
			usRegHoldingBuf[UCDOORBURGLARENABLEALARM];
	GTempSetting.ulAlarmDuration = usRegHoldingBuf[ULALARMDURATION];
	EXIT_CRITICAL_SECTION();
}

void ADCT_BGS_MODBUS_Get_BE_DKG_Data(void) {
	ENTER_CRITICAL_SECTION();

	GstSystemStatus.mbData.usCurrentMainVoltage =
			usRegHoldingBuf[USCURRENTMAINVOLTAGE];
	GstSystemStatus.mbData.usCurrentGenFuel = usRegHoldingBuf[USCURRENTGENFUEL];
	GstSystemStatus.mbData.usCurrentGenVoltage =
			usRegHoldingBuf[USCURRENTGENVOLTAGE];
	GstSystemStatus.mbData.usCurrentGenBatteryVoltage =
			usRegHoldingBuf[USCURRENTGENBATTERYVOLTAGE];
	GstSystemStatus.mbData.usCurrentGenCoolantTemp =
			usRegHoldingBuf[USCURRENTGENCOOLANTTEMP];
	GstSystemStatus.mbData.usCurrentGenEngineSpeed =
			usRegHoldingBuf[USCURRENTGENENGINESPEED];
	GstSystemStatus.mbData.usCurrentGenOilPressure =
			usRegHoldingBuf[USCURRENTGENOILPRESSURE];
	GstSystemStatus.mbData.usCurrentGenCurVoltage =
			usRegHoldingBuf[USCURRENTGENCURVOLTAGE];
	GstSystemStatus.mbData.usCurrentMainFreq = usRegHoldingBuf[USCURRENTMAINFREQ];
	GstSystemStatus.mbData.usCurrentGenFreq = usRegHoldingBuf[USCURRENTGENFREQ];
	GstSystemStatus.mbData.usCurrentGenMode = usRegHoldingBuf[USCURRENTGENMODE];

	ADCT_BGS_REBOOT_BOARD = usRegHoldingBuf[UC_REBOOT];
	EXIT_CRITICAL_SECTION();
}
void ADCT_BGS_MODBUS_GetSetting(void) {
	if (GstSystemSetting.usLowTemperatureT1
			!= GTempSetting.usLowTemperatureT1 * 10) {
		GstSystemSetting.usLowTemperatureT1 = GTempSetting.usLowTemperatureT1 * 10;
		ADCT_AT24C64_WriteTwoBytes(EEPROM_LOW_TEMPERATURE_T1_LOW,
				GstSystemSetting.usLowTemperatureT1);
	}
	else
	{
		// ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(USLOWTEMPERATURET1, GstSystemSetting.usLowTemperatureT1 /10);
	}

	if (GstSystemSetting.usHighTemperatureT2
			!= GTempSetting.usHighTemperatureT2 * 10) {
		GstSystemSetting.usHighTemperatureT2 = GTempSetting.usHighTemperatureT2 * 10;
		ADCT_AT24C64_WriteTwoBytes(EEPROM_HIGH_TEMPERATURE_T2_LOW,
				GstSystemSetting.usHighTemperatureT2);
	}
	else
	{
		// ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(EEPROM_HIGH_TEMPERATURE_T2_LOW, GstSystemSetting.usLowTemperatureT1 /10);
	}
	if (GstSystemSetting.usHighHumidityH2 != GTempSetting.usHighHumidityH2 * 10) {
		GstSystemSetting.usHighHumidityH2 = GTempSetting.usHighHumidityH2 * 10;
		ADCT_AT24C64_WriteTwoBytes(EEPROM_HIGH_HUMIDITY_H2_LOW,
				GstSystemSetting.usHighHumidityH2);
	}
	else
	{
		// ADCT_BGS_MODBUS_RS485_UpdateHoldingReg(USHIGHHUMIDITYH2, GstSystemSetting.usHighHumidityH2 /10);
	}

	if (GstSystemSetting.usFanTemperatureT3
			!= GTempSetting.usFanTemperatureT3 * 10) {
		GstSystemSetting.usFanTemperatureT3 = GTempSetting.usFanTemperatureT3 * 10;
		ADCT_AT24C64_WriteTwoBytes(EEPROM_FAN_TEMPERATURE_T3_LOW,
				GstSystemSetting.usFanTemperatureT3);
	}
	if (GstSystemSetting.usLowMainVoltage != GTempSetting.usLowMainVoltage ) {
		GstSystemSetting.usLowMainVoltage = GTempSetting.usLowMainVoltage ;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_LOWMAINVOLTAGE_LOW,
				GstSystemSetting.usLowMainVoltage);
	}

	if (GstSystemSetting.usHighMainVoltage
			!= GTempSetting.usHighMainVoltage ) {
		GstSystemSetting.usHighMainVoltage = GTempSetting.usHighMainVoltage;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_HIGHMAINVOLTAGE_LOW,
				GstSystemSetting.usHighMainVoltage);
	}
	if (GstSystemSetting.usGenFuelLowValue != GTempSetting.usGenFuelLowValue ) {
		GstSystemSetting.usGenFuelLowValue = GTempSetting.usGenFuelLowValue ;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_GENFUELLOWVALUE_LOW,
				GstSystemSetting.usGenFuelLowValue);
	}

	if (GstSystemSetting.usGenVoltageAlarmValue
			!= GTempSetting.usGenVoltageAlarmValue ) {
		GstSystemSetting.usGenVoltageAlarmValue = GTempSetting.usGenVoltageAlarmValue;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_GENVOLTAGEALARMVALUE_LOW,
				GstSystemSetting.usGenVoltageAlarmValue);
	}
	if (GstSystemSetting.usGenBatteryVoltageAlarmValue != GTempSetting.usGenBatteryVoltageAlarmValue ) {
		GstSystemSetting.usGenBatteryVoltageAlarmValue = GTempSetting.usGenBatteryVoltageAlarmValue ;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_GENBATTERYVOLTAGEALARMVALUE_LOW,
				GstSystemSetting.usGenBatteryVoltageAlarmValue);
	}

	if (GstSystemSetting.usGenCoolantTempAlarmValue
			!= GTempSetting.usGenCoolantTempAlarmValue ) {
		GstSystemSetting.usGenCoolantTempAlarmValue = GTempSetting.usGenCoolantTempAlarmValue;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_GENCOOLANTTEMPALARMVALUE_LOW,
				GstSystemSetting.usGenCoolantTempAlarmValue);
	}
	if (GstSystemSetting.usGenEngineSpeedAlarmValue != GTempSetting.usGenEngineSpeedAlarmValue ) {
		GstSystemSetting.usGenEngineSpeedAlarmValue = GTempSetting.usGenEngineSpeedAlarmValue ;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_GENENGINESPEEDALARMVALUE_LOW,
				GstSystemSetting.usGenEngineSpeedAlarmValue);
	}

	if (GstSystemSetting.usGenOilPressureAlarmValue
			!= GTempSetting.usGenOilPressureAlarmValue ) {
		GstSystemSetting.usGenOilPressureAlarmValue = GTempSetting.usGenOilPressureAlarmValue;
		ADCT_AT24C64_WriteTwoBytes(EPPROM_GENOILPRESSUREALARMVALUE_LOW,
				GstSystemSetting.usGenOilPressureAlarmValue);
	}
	if (GstSystemSetting.ulSwitchingTime != GTempSetting.ulSwitchingTime ) {
		GstSystemSetting.ulSwitchingTime = GTempSetting.ulSwitchingTime ;
		uint32_t temp_ui32;
		temp_ui32 = GstSystemSetting.ulSwitchingTime %3600;
		ADCT_AT24C64_WriteOneBytes(EPPROM_SWITCHING_HOUR, GstSystemSetting.ulSwitchingTime /3600);

		ADCT_AT24C64_WriteOneBytes(EPPROM_SWITCHING_MINUTE,
				temp_ui32/60);
	}

	if (GstSystemSetting.ulMinSwitchingTime
			!= GTempSetting.ulMinSwitchingTime ) {
		GstSystemSetting.ulMinSwitchingTime = GTempSetting.ulMinSwitchingTime;
		uint32_t temp_ui32;
		temp_ui32 = GstSystemSetting.ulMinSwitchingTime %3600;
		ADCT_AT24C64_WriteOneBytes(EPPROM_MINSWITCHINGTIME_HOUR,
				GstSystemSetting.ulMinSwitchingTime /3600);

		ADCT_AT24C64_WriteOneBytes(EPPROM_MINSWITCHINGTIME_MINUTE,
				temp_ui32/60);
	}

	if (GstSystemSetting.ulStayOffTime
			!= GTempSetting.ulStayOffTime ) {
		GstSystemSetting.ulStayOffTime = GTempSetting.ulStayOffTime;

		uint32_t temp_ui32;
		temp_ui32 = GstSystemSetting.ulStayOffTime %3600;
		ADCT_AT24C64_WriteOneBytes(EPPROM_STAYOFFTIME_HOUR,
				GstSystemSetting.ulStayOffTime /3600);

		ADCT_AT24C64_WriteOneBytes(EPPROM_STAYOFFTIME_MINUTE,
				temp_ui32/60);
	}

	if (GstSystemSetting.ucEnableAlarm
			!= GTempSetting.ucEnableAlarm ) {
		GstSystemSetting.ucEnableAlarm = GTempSetting.ucEnableAlarm;
		ADCT_AT24C64_WriteOneBytes(EPPROM_ENABLE_ALARM,
				GstSystemSetting.ucEnableAlarm);
	}

	if (GstSystemSetting.ucDoorBurglarEnableAlarm
			!= GTempSetting.ucDoorBurglarEnableAlarm ) {
		GstSystemSetting.ucDoorBurglarEnableAlarm = GTempSetting.ucDoorBurglarEnableAlarm;
		ADCT_AT24C64_WriteOneBytes(EPPROM_DOORBURGLAR_ENABLEALARM,
				GstSystemSetting.ucDoorBurglarEnableAlarm);
	}
}
eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs) {
	(void)pucRegBuffer;
	(void)usAddress;
	(void)usNRegs;
	return MB_ENOREG;
}

eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode) {
	eMBErrorCode eStatus = MB_ENOERR;
	int iRegIndex;

	if ((usAddress >= ADCT_REG_HOLDING_START)
			&& (usAddress + usNRegs <= ADCT_REG_HOLDING_START + REG_HOLDING_NREGS)) {
		iRegIndex = (int) (usAddress - usRegHoldingStart);

		switch (eMode)
		{
		case MB_REG_READ:
			while (usNRegs > 0) {
				*pucRegBuffer++ = (UCHAR) (usRegHoldingBuf[iRegIndex] >> 8);
				*pucRegBuffer++ = (UCHAR) (usRegHoldingBuf[iRegIndex] & 0xFF);
				iRegIndex++;
				usNRegs--;
			}
			break;
		case MB_REG_WRITE:
			while (usNRegs > 0) {
				usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
				usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
				iRegIndex++;
				usNRegs--;
			}
			break;
		}
	} else {
		eStatus = MB_ENOREG;
	}
	return eStatus;
}

eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNCoils, eMBRegisterMode eMode) {
	(void)pucRegBuffer;
	(void)usAddress;
	(void)usNCoils;
	(void)eMode;
	return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNDiscrete) {
	(void)pucRegBuffer;
	(void)usAddress;
	(void)usNDiscrete;
	return MB_ENOREG;
}

void ADCT_SendDebugData(void)
{

}
