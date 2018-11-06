/**
 ******************************************************************************
 * @file    ADCT_BGS_State.c
 * @author  ADCT Team - VuNTDH
 * @version V0.0.1
 * @date    23-Mar-2017
 * @brief   State machine for SNMP project.
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

#include "ADCT_BGS_State.h"
#include "ADCT_BGS_Modbus.h"
#include "ADCT_BGS_System.h"
#include "HardwareCfg.h"
#include "ADCT_BGS_DataTypes.h"
#include "ADCT_AT24C64.h"
#include "ADCT_DHT22.h"
#include "ADCT_Sensor.h"
#include "ADCT_RTC.h"
#include "ADCT_Relay.h"
#include <PZEM004T.h>
ADCT_BGS_TimerManagedDatatypes g_ADCT_BGS_TimerManager;
ADCT_BGS_StateOutputDatatypes g_ADCT_BGS_StateOutput;
ADCT_BGS_RTC_Types g_ADCT_BGS_Current_RTC;
uint8_t count_dht22;
uint8_t count_ac;
extern ADCT_BGS_ASW_TaskDatatypes g_ASW_Task_flag;
extern FPT_SystemStatus    GstSystemStatus;
void ADCT_BGS_SwitchAIR_TimeCal(void);
void ADCT_BGS_CheckMinSwitchAIR1_2(void);
void ADCT_BGS_CheckMinSwitchAIR3_4(void);
void ADCT_BGS_CheckBuzzerTimerRun(void);
void ADCT_BGS_CheckTminFan(void);
void ADCT_BGS_CheckDelayOfftime(void);
void ADCT_BGS_AlarmBuzzer(void);
void ADCT_BGS_GetCurrentRTCData(void );
void ADCT_BGS_StateManger(void)
{

	if(g_ASW_Task_flag.ASW_1s_flag == ADCT_TRUE)
	{
		if(count_dht22 >3)
		{
			/* Call sequence start reading DHT 22 */
			/* DHT 22 main */
			ADCT_BGS_DHT22Start(DHT22_Device_1);
			/* DHT22 sub */
#ifdef SUB_DHT22
			ADCT_BGS_DHT22Start(DHT22_Device_2);
			count_dht22 =0;
#endif
		}
		else
		{
			count_dht22 ++;
		}
		/* Get the latest RTC value */
		ADCT_BGS_GetCurrentRTCData();
		/* Get all input data */
		ADCT_IO_FPT_GetValue(&GstSystemStatus.ioData);
		/* check state modbus */
		ADCT_SYSTEM_FPT_CheckState_MODBUS();
		/* Check state IO */
		ADCT_SYSTEM_FPT_CheckState_IO();
		/* Check buzzer timer */
		ADCT_BGS_CheckBuzzerTimerRun();
		/* Check switching time */
		ADCT_BGS_SwitchAIR_TimeCal();
		/* Check time min swith for AIR 1_2*/
		ADCT_BGS_CheckMinSwitchAIR1_2();
		/* Check time min switch for AIR 3_4 */
		ADCT_BGS_CheckMinSwitchAIR3_4();
		/* Check state DHT22 */
		ADCT_SYSTEM_FPT_CheckState_DHT();
		/* Check t min fan */
		ADCT_BGS_CheckTminFan();
		/* Get AC Data */
		if(count_ac > 3)
		{
			ADCT_AC_GetData();
			count_ac = 0;
		}
		else
		{
			count_ac ++;
		}
		/* Check delay off time */
		ADCT_BGS_CheckDelayOfftime();
		ADCT_BGS_AlarmBuzzer();
		g_ASW_Task_flag.ASW_1s_flag = ADCT_FALSE;
	}
}
void ADCT_BGS_GetCurrentRTCData(void )
{
	ADCT_BGS_Run_RTC(&g_ADCT_BGS_Current_RTC);
}
void ADCT_BGS_SwitchAIR_TimeCal(void)
{
	if(g_ADCT_BGS_TimerManager.SWitchingTimer.second  == 0)
	{
		g_ADCT_BGS_TimerManager.SWitchingTimer.second = 59;
		if(g_ADCT_BGS_TimerManager.SWitchingTimer.minute == 0)
		{
			g_ADCT_BGS_TimerManager.SWitchingTimer.minute = 59;
			if(g_ADCT_BGS_TimerManager.SWitchingTimer.hour == 0)
			{
				if(g_ADCT_BGS_StateOutput.WITCH_AIR == ADCT_TRUE)
				{
					g_ADCT_BGS_StateOutput.WITCH_AIR = ADCT_FALSE;
				}
				else
				{
					g_ADCT_BGS_StateOutput.WITCH_AIR = ADCT_TRUE;
				}
				g_ADCT_BGS_TimerManager.SWitchingTimer.hour = ADCT_AT24C64_ReadOneByte(EPPROM_SWITCHING_HOUR);
				g_ADCT_BGS_TimerManager.SWitchingTimer.minute = ADCT_AT24C64_ReadOneByte(EPPROM_SWITCHING_MINUTE);
				g_ADCT_BGS_TimerManager.SWitchingTimer.second = 0;
			}
			else
			{
				g_ADCT_BGS_TimerManager.SWitchingTimer.hour -- ;
			}
		}else
		{
			g_ADCT_BGS_TimerManager.SWitchingTimer.minute --;
		}
	}
	else
	{
		g_ADCT_BGS_TimerManager.SWitchingTimer.second --;
	}
}

// Timer to check if air_1 has run at least t_min (s)
void ADCT_BGS_CheckMinSwitchAIR1_2(void)
{
	if(g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE == ADCT_TRUE)
	{
		if(g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.second == 0)
		{
			g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.second = 59;
			if(g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.minute == 0)
			{
				g_ADCT_BGS_StateOutput.AIR_1_2TIMER_STATE = FALSE;
				g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.second = 0;
			}
			else
			{
				g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.minute --;
			}
		}
		else
		{
			g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR1_2.second --;
		}
	}
}
void ADCT_BGS_CheckMinSwitchAIR3_4(void)
{
	if(g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE == ADCT_TRUE)
	{
		if(g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.second == 0)
		{
			g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.second = 59;
			if(g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.minute == 0)
			{
				g_ADCT_BGS_StateOutput.AIR_3_4TIMER_STATE = FALSE;
				g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.second = 0;
			}
			else
			{
				g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.minute --;
			}
		}
		else
		{
			g_ADCT_BGS_TimerManager.MinSwitchingTimerAIR3_4.second --;
		}
	}
}
void ADCT_BGS_CheckBuzzerTimerRun(void)
{
	if(g_ADCT_BGS_StateOutput.BUZZER_TIMER_STATE == ADCT_TRUE)
	{
		if(g_ADCT_BGS_TimerManager.BuzzerTimer.second == 0)
		{
			g_ADCT_BGS_TimerManager.BuzzerTimer.second = 59;
			if(g_ADCT_BGS_TimerManager.BuzzerTimer.minute ==0)
			{
				g_ADCT_BGS_TimerManager.BuzzerTimer.minute = 59;
				if(g_ADCT_BGS_TimerManager.BuzzerTimer.hour == 0)
				{
					g_ADCT_BGS_StateOutput.BUZZER_TIMER_STATE = ADCT_FALSE;
					g_ADCT_BGS_TimerManager.BuzzerTimer.hour = ADCT_AT24C64_ReadOneByte(EPPROM_ALARMDURATION_LOW);
					g_ADCT_BGS_TimerManager.BuzzerTimer.minute = ADCT_AT24C64_ReadOneByte(EPPROM_ALARMDURATION_HIGH);
					g_ADCT_BGS_TimerManager.BuzzerTimer.second =0;
				}
			}
			else
			{
				g_ADCT_BGS_TimerManager.BuzzerTimer.minute --;
			}
		}else
		{
			g_ADCT_BGS_TimerManager.BuzzerTimer.second--;
		}
	}
}

void ADCT_BGS_CheckDelayOfftime(void)
{
	if(g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_STATE == ADCT_TRUE)
	{
		if(g_ADCT_BGS_TimerManager.StayOffTimer.second ==0)
		{
			g_ADCT_BGS_TimerManager.StayOffTimer.second = 59;
			if(g_ADCT_BGS_TimerManager.StayOffTimer.minute == 0)
			{
				g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_STATE = ADCT_FALSE;
				g_ADCT_BGS_TimerManager.StayOffTimer.second = 0;
				g_ADCT_BGS_StateOutput.DELAY_OFFTIMER_ELAPSED = ADCT_TRUE;
			}
			else
			{
				g_ADCT_BGS_TimerManager.StayOffTimer.minute --;
			}
		}
		else
		{
			g_ADCT_BGS_TimerManager.StayOffTimer.second -- ;
		}
	}
}
void ADCT_BGS_CheckTminFan(void)
{
	if(g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE == ADCT_TRUE)
	{
		if(g_ADCT_BGS_TimerManager.MinRuningTimerFan.second == 0)
		{
			g_ADCT_BGS_TimerManager.MinRuningTimerFan.second = 59;
			if(g_ADCT_BGS_TimerManager.MinRuningTimerFan.minute == 0)
			{
				g_ADCT_BGS_StateOutput.FAN_OUTPUT_STATE = ADCT_FALSE;
				g_ADCT_BGS_TimerManager.MinRuningTimerFan.second = 0;
			}
			else
			{
				g_ADCT_BGS_TimerManager.MinRuningTimerFan.minute--;
			}
		}
		else
		{
			g_ADCT_BGS_TimerManager.MinRuningTimerFan.second -- ;
		}
	}
}

void ADCT_BGS_AlarmBuzzer(void)
{
	if(g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag == ADCT_TRUE)
	{
		if(g_ADCT_BGS_StateOutput.ALARM_RUNNING_Timer_Cnt == 0)
		{
			g_ADCT_BGS_StateOutput.ALARM_TIMER_Flag = ADCT_FALSE;
			g_ADCT_BGS_StateOutput.BUZZER_OUTPUT_STATE = ADCT_FALSE;
			if(g_ADCT_BGS_StateOutput.BUZZER_TIMER_STATE == ADCT_FALSE)
			{
				g_ADCT_BGS_StateOutput.BUZZER_TIMER_STATE = ADCT_TRUE;
				g_ADCT_BGS_TimerManager.BuzzerTimer.hour = ADCT_AT24C64_ReadOneByte(EPPROM_ALARMDURATION_HIGH );
				g_ADCT_BGS_TimerManager.BuzzerTimer.minute = ADCT_AT24C64_ReadOneByte(EPPROM_ALARMDURATION_LOW );
			}
		}
		else
		{
			g_ADCT_BGS_StateOutput.ALARM_RUNNING_Timer_Cnt -- ;
		}
	}
}
/**
 * @brief Get value of I/Os.
 * @param pIOData        : data of I/Os
 * @retval None
 */
void ADCT_IO_FPT_GetValue(IO_MODULE_DATA *pIOData)
{
	/*DOOR*/
	pIOData->ucDoor [0] = !ADCT_BGS_DOOR_READ ;
	pIOData->ucDoor [1] = !ADCT_BGS_DOOR_READ ;
	pIOData->ucDoor [2] = !ADCT_BGS_DOOR_READ ;
	pIOData->ucDoor [3] = !ADCT_BGS_DOOR_READ ;
	pIOData->ucDoor [4] = !ADCT_BGS_DOOR_READ ;
	/*FIRE*/
	pIOData->ucFire = ADCT_BGS_FIRE_READ;
	/*WATER*/
	pIOData->ucWater = ADCT_BGS_WATER_READ;
	/*SMOKE*/
	pIOData->ucSmoke = ADCT_BGS_SMOKE_READ;

	/*BURGLAR*/
	pIOData->ucBurglar = ADCT_BGS_BURGLAR_READ;
	/*BUZZER*/
	pIOData->ucBuzzer = ADCT_BGS_BUZZER_READ();
	/*LED*/
	pIOData->ucLed = ADCT_BGS_LAMP_READ();
}
