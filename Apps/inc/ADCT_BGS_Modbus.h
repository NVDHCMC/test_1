/**
  ******************************************************************************
  * @file    ADCT_BGS_Modbus.h
  * @author  ADCT Team - VuNTDH
  * @version V0.0.1
  * @date    23-Mar-2017
  * @brief   This file contains data structure and function prototype for the
  *          MOSBUS communication in SNMP_FPT project.
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
#ifndef APPS_INC_ADCT_BGS_MODBUS_H_
#define APPS_INC_ADCT_BGS_MODBUS_H_
#include "stm32f10x.h"
#include "mb.h"
#include "mbport.h"
#include "stm32f10x_conf.h"
#include "mb.h"

#define ADCT_REG_HOLDING_START   40001
//#define ADCT_REG_HOLDING_START   00001
#define ADCT_REG_HOLDING_NREGS   4
#define ADCT_BGS_MODBUS_ADDR     (uint16_t)(0x03)
#define ADCT_BGS_BAULDRATE			9600//115200

typedef enum
{
	USTEMP_MAIN     = 1          ,
	USHUMID_MAIN                 ,
	USTEMP_SUB                   ,
	USHUMID_SUB                   ,
	PH_DATA                   ,
	RESERVED_2                   ,
	RESERVED_3                   ,
	RESERVED_4                   ,
	RESERVED_5                   ,
	RESERVED_6                   ,
	UCFIRE                       ,
	UCSMOKE                      ,
	UCWATER                      ,
	DOOR1                        ,
	DOOR2                        ,
	DOOR3                        ,
	DOOR4                        ,
	DOOR5                        ,
	AIR1                         ,
	AIR2                         ,
	AIR3                         ,
	AIR4                         ,
	FAN1                         ,
	FAN2                         ,
	FAN3                         ,
	FAN4                         ,
	BUZZER                       ,
	LED                          ,
	BUGLAR                   ,
	RESERVED_8                   ,
	ALARM_WARNING_HIGH                ,
	ALARM_WARNING_LOW                  ,
	RESERVED_11                  ,
	RESERVED_12                  ,
	RESERVED_13                  ,
	RESERVED_14                  ,
	USCURRENTMAINVOLTAGE         ,
	USCURRENTGENFUEL             ,
	USCURRENTGENVOLTAGE          ,
	USCURRENTGENBATTERYVOLTAGE   ,
	USCURRENTGENCOOLANTTEMP      ,
	USCURRENTGENENGINESPEED      ,
	USCURRENTGENOILPRESSURE      ,
	USCURRENTGENCURVOLTAGE       ,
	USCURRENTMAINFREQ            ,
	USCURRENTGENFREQ             ,
	USCURRENTGENMODE             ,
	RESERVED_15                  ,
	RESERVED_16                  ,
	RESERVED_17                  ,
	USLOWTEMPERATURET1           ,
	USHIGHTEMPERATURET2          ,
	USFANTEMPERATURET3           ,
	USLOWHUMIDITYH1              ,
	USHIGHHUMIDITYH2             ,
	USLOWMAINVOLTAGE             ,
	USHIGHMAINVOLTAGE            ,
	USGENFUELLOWVALUE            ,
	USGENVOLTAGEALARMVALUE       ,
	USGENBATTERYVOLTAGEALARMVALUE,
	USGENCOOLANTTEMPALARMVALUE   ,
	USGENENGINESPEEDALARMVALUE   ,
	USGENOILPRESSUREALARMVALUE   ,
	ULSWITCHINGTIME              ,
	ULMINSWITCHINGTIME           ,
	ULSTAYOFFTIME                ,
	UCENABLEALARM                ,
	UCDOORBURGLARENABLEALARM     ,
	ULALARMDURATION              ,
	UC_REBOOT                  ,
	RESERVED_19                  ,
	US_RTC_SEC,
	US_RTC_MIN,
	US_RTC_HOUR,
	US_RTC_DAY,
	US_RTC_MONTH,
	US_RTC_YEAR,
	US_SW_SEC,
	US_SW_MINUTE,
	US_SW_HOUR,
	VOLTAGE_RMS         ,
	CURRENT_SIG         ,
	CURRENT_EXP         ,
	ACTIVE_POWER_SIG    ,
	ACTIVE_POWER_EXP    ,
	ENERGY_SIG          ,
	ENERGY_EXP          ,
	REG_HOLDING_NREGS
}ADCT_RegHoldingAddressOffset_t;
eMBErrorCode ADCT_BGS_MODBUS_RS485_Init(void);
void ADCT_BGS_MODBUS_RS485_UpdateData(void );
void ADCT_BGS_UpdateBoardSettingFromModbus(void);
void ADCT_BGS_MODBUS_Get_BE_DKG_Data(void);
void ADCT_BGS_MODBUS_GetSetting(void);
#endif /* APPS_INC_ADCT_BGS_MODBUS_H_ */
