/**
 ******************************************************************************
 * @file    ADCT_BGS_System.c
 * @author  ADCT Team - VuNTDH
 * @version V0.0.1
 * @date    23-Mar-2017
 * @brief   Core System for SNMP_FPT project.
 *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================

  @endverbatim
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 ADCT TEAM</center></h2>
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
#include "ADCT_BGS_System.h"
#include "ADCT_BGS_DataTypes.h"
#include "ADCT_BGS_Modbus.h"
#include "ADCT_DHT22.h"
#include "ADCT_BGS_FPT_Delay.h"
#include "ADCT_Relay.h"
/** @addtogroup ADCT_SNMP_FPT
 * @{
 */

/** @defgroup SYSTEM_FPT
 * @brief CORE SYSTEM for SNMP_FPT project
 * @{
 */
/* Exported variables --------------------------------------------------------*/

/* System Status */
FPT_SystemStatus    GstSystemStatus;
/* System Settings */
FPT_UserSetting     GstSystemSetting;
/* System Warning */
FPT_SystemWarning   GstSystemWarning;
extern ADCT_BGS_TimerManagedDatatypes g_ADCT_BGS_TimerManager;
extern ADCT_BGS_StateOutputDatatypes g_ADCT_BGS_StateOutput;

void ADCT_SYSTEM_FPT_TempTimer_Task(TM_DELAY_Timer_t* timer, void * parameter);
TM_DELAY_Timer_t *TempStateTimer;
//#define CHECK_AC_AND_GEN
/**
 * @brief Initializes all devices of system
 * @param None
 * @retval None
 */
void ADCT_SYSTEM_FPT_Init(void)
{
#ifdef ADCT_MODBUS_ENABLED
	ADCT_BGS_MODBUS_RS485_Init();
#endif
#ifdef ADCT_DHT22_ENABLED
	ADCT_BGS_DHT22Init();
#endif
	/* I/O */
	/* Initialize variables */
	TempStateTimer = TM_DELAY_TimerCreate(1000, 0, 0, ADCT_SYSTEM_FPT_TempTimer_Task, NULL);
}
#ifdef ADCT_MODBUS_ENABLED
void ADCT_SYSTEM_FPT_CheckState_MODBUS(void )
{
	/* High Main voltage */
	if((GstSystemStatus.mbData.usCurrentMainVoltage > GstSystemSetting.usHighMainVoltage)&&
			(ReadBit(GstSystemWarning.othWarn, WARN_HIGHMAINVOLTAGE) == WARN_OFF))
	{
		/*Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_HIGHMAINVOLTAGE, WARN_ON);

		/* Store log into EPPRROM */
	}
	else if ((GstSystemStatus.mbData.usCurrentMainVoltage < GstSystemSetting.usHighMainVoltage) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_HIGHMAINVOLTAGE) == WARN_ON))
	{
		/* Set warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_HIGHMAINVOLTAGE, WARN_OFF);
		/* Store log into Epprom */
	}
	/* Low Main Voltage (SNMP_SYT_013) */
	if((GstSystemStatus.mbData.usCurrentMainVoltage < GstSystemSetting.usLowMainVoltage) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWMAINVOLTAGE) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWMAINVOLTAGE, WARN_ON);
		/* Set log */
	}
	else if ((GstSystemStatus.mbData.usCurrentMainVoltage > GstSystemSetting.usLowMainVoltage) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWMAINVOLTAGE) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWMAINVOLTAGE, WARN_OFF);
		/* Set Log */
	}

	/* Low Fuel (SNMP_SYT_015) */
	if((GstSystemStatus.mbData.usCurrentGenFuel < GstSystemSetting.usGenFuelLowValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWFUEL) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWFUEL, WARN_ON);
		/* Set Log */
	}
	else if ((GstSystemStatus.mbData.usCurrentGenFuel > GstSystemSetting.usGenFuelLowValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWFUEL) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWFUEL, WARN_OFF);
		/* Set Log */
	}
	/* Low Oil Pressure (SNMP_SYT_016) */
	if((GstSystemStatus.mbData.usCurrentGenOilPressure < GstSystemSetting.usGenOilPressureAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWOILPRESS) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWOILPRESS, WARN_ON);
		/* Set Log */

	}
	else if((GstSystemStatus.mbData.usCurrentGenOilPressure > GstSystemSetting.usGenOilPressureAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWOILPRESS) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWOILPRESS, WARN_OFF);
		/* Set Log */

	}

	/* Low Generate Voltage (SNMP_SYT_017) */
	if((GstSystemStatus.mbData.usCurrentGenVoltage < GstSystemSetting.usGenVoltageAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWGENVOLTAGE) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWGENVOLTAGE, WARN_ON);
		/* Set Log */

	}
	else if((GstSystemStatus.mbData.usCurrentGenVoltage > GstSystemSetting.usGenVoltageAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWGENVOLTAGE) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWGENVOLTAGE, WARN_OFF);
		/* Set Log */

	}

	/* Low Generate Battery (SNMP_SYT_018) */
	if((GstSystemStatus.mbData.usCurrentGenBatteryVoltage < GstSystemSetting.usGenBatteryVoltageAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWGENBAT) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWGENBAT, WARN_ON);
		/* Set Log */

	}
	else if((GstSystemStatus.mbData.usCurrentGenBatteryVoltage > GstSystemSetting.usGenBatteryVoltageAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWGENBAT) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWGENBAT, WARN_OFF);
		/* Set Log */

	}

	/* Low Engine Speed (SNMP_SYT_019) */
	if((GstSystemStatus.mbData.usCurrentGenEngineSpeed < GstSystemSetting.usGenEngineSpeedAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWENSPEED) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWENSPEED, WARN_ON);
		/* Set Log */

	}
	else if((GstSystemStatus.mbData.usCurrentGenEngineSpeed > GstSystemSetting.usGenEngineSpeedAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWENSPEED) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWENSPEED, WARN_OFF);
		/* Set Log */

	}

	/* Low Coolant Temperature (SNMP_SYT_020) */
	if((GstSystemStatus.mbData.usCurrentGenCoolantTemp < GstSystemSetting.usGenCoolantTempAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWCOOLTEMP) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWCOOLTEMP, WARN_ON);
		/* Set Log */

	}
	if((GstSystemStatus.mbData.usCurrentGenCoolantTemp > GstSystemSetting.usGenCoolantTempAlarmValue) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWCOOLTEMP) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWCOOLTEMP, WARN_OFF);
		/* Set Log */

	}
}
#endif
/**
 * @brief Check system state of I/O.
 * @param None
 * @retval None
 */
void ADCT_SYSTEM_FPT_CheckState_IO(void)
{
	/* Fire (SNMP_SYT_007) */
	if(GstSystemStatus.ioData.ucFire == (ReadBit(GstSystemWarning.othWarn, WARN_FIRE)))
	{
		/* Set Warning */
		WriteBit(GstSystemWarning.othWarn, WARN_FIRE, !GstSystemStatus.ioData.ucFire);
		if(!GstSystemStatus.ioData.ucFire)
		{
			GstSystemWarning.State = MAINSTATE_ALARM;
			/* Set Log */
			g_ADCT_BGS_StateOutput.ALARM_RUNNING_Timer_Cnt = GstSystemSetting.ulAlarmDuration;
			g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag = ADCT_TRUE;
		}
		else
		{
			/* Set Log */
			GstSystemWarning.State = MAINSTATE_IDLE;
		}
	}

	/* Smoke (SNMP_SYT_008) */
	if(GstSystemStatus.ioData.ucSmoke == (ReadBit(GstSystemWarning.othWarn, WARN_SMOKE)))
	{
		/* Set Warning */
		WriteBit(GstSystemWarning.othWarn, WARN_SMOKE, !GstSystemStatus.ioData.ucSmoke);
		if(!GstSystemStatus.ioData.ucSmoke)
		{
			GstSystemWarning.State = MAINSTATE_ALARM;
			/* Set Log */
			g_ADCT_BGS_StateOutput.ALARM_RUNNING_Timer_Cnt = GstSystemSetting.ulAlarmDuration;
			g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag = ADCT_TRUE;
		}
		else
		{
			/* Set Log */
			GstSystemWarning.State = MAINSTATE_IDLE;
		}
	}
	/* Door (SNMP_SYT_009) */
	if((GstSystemStatus.ioData.ucDoor[0] ||
			GstSystemStatus.ioData.ucDoor[1] ||
			GstSystemStatus.ioData.ucDoor[2] ||
			GstSystemStatus.ioData.ucDoor[3] ||
			GstSystemStatus.ioData.ucDoor[4]) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_DOOR) == WARN_OFF))
	{
		GstSystemWarning.State = MAINSTATE_ALARM;
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_DOOR, WARN_ON);
		/* Set Log */
		g_ADCT_BGS_StateOutput.ALARM_RUNNING_Timer_Cnt = GstSystemSetting.ulAlarmDuration;
		g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag = ADCT_TRUE;
	}
	else if((GstSystemStatus.ioData.ucDoor[0] == 0 &&
			GstSystemStatus.ioData.ucDoor[1] == 0 &&
			GstSystemStatus.ioData.ucDoor[2] == 0 &&
			GstSystemStatus.ioData.ucDoor[3] == 0 &&
			GstSystemStatus.ioData.ucDoor[4] == 0) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_DOOR) == WARN_ON))
	{
		/* Set Warning OFF */
		WriteBit(GstSystemWarning.othWarn, WARN_DOOR, WARN_OFF);
		/* Set Log */
		GstSystemWarning.State = MAINSTATE_IDLE;
	}
	/* Burglar (SNMP_SYT_010) */
	if(GstSystemStatus.ioData.ucBurglar != (ReadBit(GstSystemWarning.othWarn, WARN_BURGLAR)))
	{

		/* Set Warning */
		WriteBit(GstSystemWarning.othWarn, WARN_BURGLAR, GstSystemStatus.ioData.ucBurglar);
		if(GstSystemStatus.ioData.ucBurglar)
		{
			GstSystemWarning.State = MAINSTATE_ALARM;
			/* Set Log */
			g_ADCT_BGS_StateOutput.ALARM_RUNNING_Timer_Cnt = GstSystemSetting.ulAlarmDuration;
			g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag = ADCT_TRUE;
		}
		else
		{
			/* Set Log */
			GstSystemWarning.State = MAINSTATE_IDLE;
		}
	}

	/* Water (SNMP_SYT_011) */
	if(GstSystemStatus.ioData.ucWater != (ReadBit(GstSystemWarning.othWarn, WARN_WATER)))
	{

		/* Set Warning */
		WriteBit(GstSystemWarning.othWarn, WARN_WATER, GstSystemStatus.ioData.ucWater);
		if(GstSystemStatus.ioData.ucWater)
		{
			GstSystemWarning.State = MAINSTATE_ALARM;
			/* Set Log */
			g_ADCT_BGS_StateOutput.ALARM_RUNNING_Timer_Cnt = GstSystemSetting.ulAlarmDuration;
			g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag = ADCT_TRUE;
		}
		else
		{
			/* Set Log */
			GstSystemWarning.State = MAINSTATE_IDLE;
		}
	}
}
/**
 * @brief Check system state of DHT22.
 * @param None
 * @retval None
 */
void ADCT_SYSTEM_FPT_CheckState_DHT(void)
{
	/* Low Humidity (SNMP_SYT_005) */
	if((GstSystemStatus.dhtData.usHumid_Main < GstSystemSetting.usLowHumidityH1) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWHUMID) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWHUMID, WARN_ON);
		/* Set Log */
		GstSystemWarning.State = MAINSTATE_ALARM;
	}
	else if((GstSystemStatus.dhtData.usHumid_Main > GstSystemSetting.usLowHumidityH1) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWHUMID) == WARN_ON))
	{
		/* Set Warning */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWHUMID, WARN_OFF);
		/* Set Log */
		GstSystemWarning.State = MAINSTATE_IDLE;
	}

	/* High Humidity (SNMP_SYT_006) */
	else if((GstSystemStatus.dhtData.usHumid_Main > GstSystemSetting.usHighHumidityH2) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_HIGHHUMID) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_HIGHHUMID, WARN_ON);
		/* Set Log */
		GstSystemWarning.State = MAINSTATE_ALARM;
	}
	else if((GstSystemStatus.dhtData.usHumid_Main < GstSystemSetting.usHighHumidityH2) &&
			(ReadBit(GstSystemWarning.othWarn, WARN_HIGHHUMID) == WARN_ON))
	{
		/* Set Warning */
		WriteBit(GstSystemWarning.othWarn, WARN_HIGHHUMID, WARN_OFF);
		/* Set Log */
		GstSystemWarning.State = MAINSTATE_IDLE;
	}

	/* Low Temperature (SNMP_SYT_002) */
	else if((GstSystemStatus.dhtData.usTemp_Main < GstSystemSetting.usLowTemperatureT1)&&
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWTEMP) == WARN_OFF))
	{
		/* Set Warning ON */
		WriteBit(GstSystemWarning.othWarn, WARN_LOWTEMP, WARN_ON);
		/* Set next state is LOW */
		GstSystemWarning.State = MAINSTATE_ALARM;
		/* Set Log */
		//ADCT_SDCARD_FPT_SetAlarmID(ALARM_LOW_TEMP_START);
	}
	else if((GstSystemStatus.dhtData.usTemp_Main >= GstSystemSetting.usLowTemperatureT1) &&
			(GstSystemStatus.dhtData.usTemp_Main <= GstSystemSetting.usHighTemperatureT2))
	{
		/* Set next state is NORMAL */
		if(ReadBit(GstSystemWarning.othWarn, WARN_LOWTEMP) == WARN_ON)
		{
			WriteBit(GstSystemWarning.othWarn, WARN_LOWTEMP, WARN_OFF);
			/* Set log */
			GstSystemWarning.State = MAINSTATE_IDLE;
		}
		if(ReadBit(GstSystemWarning.othWarn, WARN_HIGHTEMP) == WARN_ON)
		{

			WriteBit(GstSystemWarning.othWarn, WARN_HIGHTEMP, WARN_OFF);
			/* Set log */
			GstSystemWarning.State = MAINSTATE_IDLE;
		}

	}

	/* High Temperature (SNMP_SYT_003) */
	else if((GstSystemStatus.dhtData.usTemp_Main > GstSystemSetting.usHighTemperatureT2) &&
			(GstSystemStatus.dhtData.usTemp_Main <= GstSystemSetting.usFanTemperatureT3))
	{
		if(ReadBit(GstSystemWarning.othWarn, WARN_HIGHTEMP) == WARN_OFF)
		{
			WriteBit(GstSystemWarning.othWarn, WARN_HIGHTEMP, WARN_ON);
			/*Setlog*/
			GstSystemWarning.State = MAINSTATE_ALARM;
		}
		if(ReadBit(GstSystemWarning.othWarn, WARN_HIGHFANTEMP) == WARN_ON)
		{
			WriteBit(GstSystemWarning.othWarn, WARN_HIGHFANTEMP, WARN_OFF);
			/*Set log */
			//GstSystemWarning.State = MAINSTATE_IDLE;
		}
	}
	/* Fan Temperature (SNMP_SYT_004) */
	else
	{
		/* Set next state is FAN */

		if(ReadBit(GstSystemWarning.othWarn, WARN_HIGHFANTEMP) == WARN_OFF)
		{
			WriteBit(GstSystemWarning.othWarn, WARN_HIGHFANTEMP, WARN_ON);
			/*Set log */
			GstSystemWarning.State = MAINSTATE_ALARM;
		}
	}

}

uint8_t ADCT_SystemFPT_CheckAlarm(void)
{
	if(
			(ReadBit(GstSystemWarning.othWarn, WARN_FIRE) == WARN_ON) || \
			(ReadBit(GstSystemWarning.othWarn, WARN_SMOKE) == WARN_ON) ||
			(ReadBit(GstSystemWarning.othWarn, WARN_DOOR) == WARN_ON) ||
			(ReadBit(GstSystemWarning.othWarn, WARN_BURGLAR) == WARN_ON )||
			(ReadBit(GstSystemWarning.othWarn, WARN_WATER) == WARN_ON )||
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWHUMID) == WARN_ON )||
			(ReadBit(GstSystemWarning.othWarn, WARN_HIGHHUMID) == WARN_ON)||
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWFUEL) == WARN_ON)||
			(ReadBit(GstSystemWarning.othWarn, WARN_HIGHTEMP) == WARN_ON)||
			(ReadBit(GstSystemWarning.othWarn, WARN_LOWTEMP) == WARN_ON)||
			(ReadBit(GstSystemWarning.othWarn, WARN_HIGHFANTEMP) == WARN_ON)
	)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
uint8_t ADCT_SystemFPT_CheckAlarm_Siren(void)
{
	if(ReadBit(GstSystemWarning.othWarn, WARN_FIRE) == WARN_ON||\
			ReadBit(GstSystemWarning.othWarn, WARN_SMOKE) == WARN_ON||
			ReadBit(GstSystemWarning.othWarn, WARN_DOOR) == WARN_ON||
			ReadBit(GstSystemWarning.othWarn, WARN_BURGLAR) == WARN_ON||
			ReadBit(GstSystemWarning.othWarn, WARN_WATER) == WARN_ON
	)	{
		return TRUE;
	}
	else
	{

		return FALSE;
	}
}
void ADCT_SystemFPT_RunOutput(void)
{
	if(ReadBit(GstSystemWarning.othWarn, WARN_FIRE) == WARN_ON||ReadBit(GstSystemWarning.othWarn, WARN_SMOKE) == WARN_ON)
	{
		/* Off all AIR and FAN */
		ADCT_BGS_AIR_1_2_OFF();
		ADCT_BGS_AIR_3_4_OFF();
		g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = ADCT_FALSE;
		g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = ADCT_FALSE;
		ADCT_BGS_FAN_OFF();
		g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE = ADCT_FALSE;
	}
	/* Check Main Voltage is lower than LOW setting, greater than 0 and
       Gen Voltage is different than 0 */
	else 
	{
#ifdef CHECK_AC_AND_GEN
		if((GstSystemStatus.mbData.usCurrentMainVoltage < GstSystemSetting.usLowMainVoltage) &&(GstSystemStatus.mbData.usCurrentGenVoltage == 0))
		{
			if(g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_STATE == FALSE &&g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_ELAPSED == FALSE)
			{
				uint32_t temp_variable;
				temp_variable = GstSystemSetting.ulStayOffTime%3600 ;
				g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_STATE = TRUE;
				g_ADCT_BGS_TimerManager.StayOffTimer.hour = GstSystemSetting.ulStayOffTime /3600;
				g_ADCT_BGS_TimerManager.StayOffTimer.minute = (uint8_t)(temp_variable/60);
				g_ADCT_BGS_TimerManager.StayOffTimer.second = (uint8_t)(temp_variable%60);
			}
		}
		else
		{
			//AC or gen recover
			g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_ELAPSED = FALSE;
			g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_STATE = FALSE;
		}
		if(g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_ELAPSED == TRUE)
		{
			/* Off all AIR and FAN */
			ADCT_BGS_AIR_1_2_OFF();
			ADCT_BGS_AIR_3_4_OFF();
			g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = ADCT_FALSE;
			g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = ADCT_FALSE;
		}
		else
#endif
		{
			/* Low Temperature (SNMP_SYT_002) */
			if(GstSystemStatus.dhtData.usTemp_Main < GstSystemSetting.usLowTemperatureT1)
			{
				// Detect if the t_min_timer is still running, if YES, output remains high
				if((g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE == TRUE) && (g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE == TRUE))
				{
					ADCT_BGS_AIR_1_2_ON();
				}
				else
				{
					ADCT_BGS_AIR_1_2_OFF();
					g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = FALSE;
				}
				// Detect if the t_min_timer is still running, if YES, output remains high
				if((g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE == TRUE) && (g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE == TRUE))
				{
					ADCT_BGS_AIR_3_4_ON();
				}
				else
				{
					ADCT_BGS_AIR_3_4_OFF();
					g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = FALSE;
				}
				// Detect if the t_min_timer is still running, if YES, output remains high
				if((g_ADCT_BGS_StateOutput.FAN_TIMER_STATE == TRUE )&&( g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE))
				{
					ADCT_BGS_FAN_ON();
				}
				else
				{
					ADCT_BGS_FAN_OFF();
					g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE = FALSE;
				}
			}
			else if ((GstSystemStatus.dhtData.usTemp_Main >= GstSystemSetting.usLowTemperatureT1 ) &&
					(GstSystemStatus.dhtData.usTemp_Main <= GstSystemSetting.usHighTemperatureT2))
			{
				if(g_ADCT_BGS_StateOutput.WITCH_AIR == TRUE)
				{
					// Detect if the t_min_timer is still running, if YES, output remains high
					if(g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE == TRUE && g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE == TRUE)
					{
						ADCT_BGS_AIR_1_2_ON();
					}
					else
					{
						ADCT_BGS_AIR_1_2_OFF();
						g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = FALSE;
					}
					// Detect if air 2 output just activate and run min_timer
					if(g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE == FALSE && g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE == FALSE)
					{
						uint32_t temp_variable;
						temp_variable = GstSystemSetting.ulMinSwitchingTime%3600 ;
						ADCT_BGS_AIR_3_4_ON();
						g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = TRUE;
						g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE = TRUE;
						g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.hour = GstSystemSetting.ulMinSwitchingTime/3600;
						g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.minute =temp_variable/60;
						g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.second =temp_variable%60;
					}
					else
					{
						ADCT_BGS_AIR_3_4_ON();
						g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = TRUE;
					}
				}
				else
				{
					if(g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE == FALSE && g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE ==FALSE )
					{
						ADCT_BGS_AIR_1_2_ON();
						g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = TRUE;
						g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE = TRUE;
						uint32_t temp_variable;
						temp_variable = GstSystemSetting.ulMinSwitchingTime%3600 ;
						g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.hour = GstSystemSetting.ulMinSwitchingTime/3600;
						g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.minute =temp_variable/60;
						g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.second =temp_variable%60;

					}
					else
					{
						ADCT_BGS_AIR_1_2_ON();
						g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = TRUE;
					}
					// Detect if the t_min_timer is still running, if YES, output remains high
					if(g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE == TRUE && g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE == TRUE)
					{
						ADCT_BGS_AIR_3_4_ON();
					}
					else
					{
						ADCT_BGS_AIR_3_4_OFF();
						g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = FALSE;
					}
				}

				if(g_ADCT_BGS_StateOutput.FAN_TIMER_STATE == TRUE && g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE == TRUE)
				{
					ADCT_BGS_FAN_ON();
				}
				else
				{
					ADCT_BGS_FAN_OFF();
					g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE = FALSE;
				}
			}
			/* High Temperature (SNMP_SYT_003) */
			else if((GstSystemStatus.dhtData.usTemp_Main > GstSystemSetting.usHighTemperatureT2) &&
					(GstSystemStatus.dhtData.usTemp_Main <= GstSystemSetting.usFanTemperatureT3))
			{
				// Detect if air 1 output just activate and run min_timer
				if (g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE == FALSE && g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE == FALSE) {
					ADCT_BGS_AIR_1_2_ON();
					g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = TRUE;
					g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE = TRUE;
					uint32_t temp_variable;
					temp_variable = GstSystemSetting.ulMinSwitchingTime%3600 ;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.hour = GstSystemSetting.ulMinSwitchingTime/3600;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.minute =temp_variable/60;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.second =temp_variable%60;
				}
				else {
					ADCT_BGS_AIR_1_2_ON();
					g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = TRUE;
				}

				// Detect if air 2 output just activate and run min_timer
				if (g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE == FALSE && g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE == FALSE) {
					ADCT_BGS_AIR_3_4_ON();
					g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = TRUE;
					g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE = TRUE;
					uint32_t temp_variable;
					temp_variable = GstSystemSetting.ulMinSwitchingTime%3600 ;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.hour = GstSystemSetting.ulMinSwitchingTime/3600;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.minute =temp_variable/60;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.second =temp_variable%60;
				}
				else {
					ADCT_BGS_AIR_3_4_ON();
					g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = TRUE;
				}

				// Detect if the t_min_timer is still running, if YES, output remains high
				if (g_ADCT_BGS_StateOutput.FAN_TIMER_STATE == TRUE && g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE == TRUE) {
					ADCT_BGS_FAN_ON();
				}
				else {
					ADCT_BGS_FAN_OFF();
					g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE = FALSE;
				}
			}
			else
				/* Fan Temperature (SNMP_SYT_004) */
			{
				// Detect if air 1 output just activate and run min_timer
				if (g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE == FALSE && g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE == FALSE) {
					ADCT_BGS_AIR_1_2_ON();
					g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = TRUE;
					g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE = TRUE;
					uint32_t temp_variable;
					temp_variable = GstSystemSetting.ulMinSwitchingTime%3600 ;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.hour = GstSystemSetting.ulMinSwitchingTime/3600;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.minute =temp_variable/60;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.second =temp_variable%60;
				}
				else {
					ADCT_BGS_AIR_1_2_ON();
					g_ADCT_BGS_StateOutput.AIR_1_2_OUTPUT_STATE = TRUE;
				}

				// Detect if air 2 output just activate and run min_timer
				if (g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE == FALSE && g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE == FALSE) {
					ADCT_BGS_AIR_3_4_ON();
					g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = TRUE;
					g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE = TRUE;
					uint32_t temp_variable;
					temp_variable = GstSystemSetting.ulMinSwitchingTime%3600 ;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.hour = GstSystemSetting.ulMinSwitchingTime/3600;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.minute =temp_variable/60;
					g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.second =temp_variable%60;
				}
				else {
					ADCT_BGS_AIR_3_4_ON();
					g_ADCT_BGS_StateOutput.AIR_3_4_OUTPUT_STATE = TRUE;
				}

				// Detect if a fan output just activate and run min_timer
				if (g_ADCT_BGS_StateOutput.FAN_TIMER_STATE == FALSE && g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE == FALSE) {
					ADCT_BGS_FAN_ON();
					g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE = TRUE;
					g_ADCT_BGS_StateOutput.FAN_TIMER_STATE = TRUE;
					uint32_t temp_variable;
					temp_variable = GstSystemSetting.ulMinSwitchingTime%3600 ;
					g_ADCT_BGS_TimerManager.MinRuningTimerFan.hour = GstSystemSetting.ulMinSwitchingTime/3600;
					g_ADCT_BGS_TimerManager.MinRuningTimerFan.minute = temp_variable/60;
					g_ADCT_BGS_TimerManager.MinRuningTimerFan.second = temp_variable%60;
				}
				else {
					ADCT_BGS_FAN_ON();
					g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE = TRUE;
				}
			}
		}
	}
	// Lamp alarm
	if (ADCT_SystemFPT_CheckAlarm()) {
		ADCT_BGS_RELAY_LAMP_ON();
		//ADCT_BGS_FAN_ON();
	}
	else {
		ADCT_BGS_RELAY_LAMP_OFF();
	}

	if(GstSystemWarning.State == MAINSTATE_ALARM )
	{
		// Check if siren should be activated or not. If ADCT_BGS_sys.siren_timer_run == FALSE
		// siren will be activated
		if ((g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag == TRUE) && (g_ADCT_BGS_StateOutput.BUZZER_TIMER_STATE == FALSE) && (ADCT_SystemFPT_CheckAlarm_Siren() == TRUE) ){
			g_ADCT_BGS_StateOutput.BUZZER_OUTPUT_STATE =  TRUE;
		}
		else {
			g_ADCT_BGS_StateOutput.BUZZER_OUTPUT_STATE =FALSE;
		}

	}
	// Siren alarm
	if (GstSystemSetting.ucEnableAlarm == FALSE) {
		ADCT_BGS_RELAY_BUZZER_OFF();
	}
	else {
		if (g_ADCT_BGS_StateOutput.BUZZER_OUTPUT_STATE == TRUE && ADCT_SystemFPT_CheckAlarm_Siren()) {
			if (ReadBit(GstSystemWarning.othWarn, WARN_FIRE) == WARN_ON ||
					ReadBit(GstSystemWarning.othWarn, WARN_SMOKE) == TRUE||
					ReadBit(GstSystemWarning.othWarn, WARN_WATER) == TRUE) {
				ADCT_BGS_RELAY_BUZZER_ON();
			}
			else if (ReadBit(GstSystemWarning.othWarn, WARN_DOOR) == WARN_ON  || ReadBit(GstSystemWarning.othWarn, WARN_BURGLAR) == WARN_ON) {
				if (GstSystemSetting.ucDoorBurglarEnableAlarm == TRUE) {
					ADCT_BGS_RELAY_BUZZER_ON();
				}
				else {
					ADCT_BGS_RELAY_BUZZER_OFF();
				}
			}
		}
		else {
			ADCT_BGS_RELAY_BUZZER_OFF();
		}
	}
}
