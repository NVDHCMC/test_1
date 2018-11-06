/*
 * ADCT_BGS_RTC.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Administrator
 */

#ifndef MODULES_RTC_INC_ADCT_BGS_RTC_H_
#define MODULES_RTC_INC_ADCT_BGS_RTC_H_

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "ADCT_BGS_DataTypes.h"
/* Exported types ------------------------------------------------------------*/
/* Time Structure definition */
struct Time_s
{
  uint8_t SecLow;
  uint8_t SecHigh;
  uint8_t MinLow;
  uint8_t MinHigh;
  uint8_t HourLow;
  uint8_t HourHigh;
};
extern struct Time_s s_TimeStructVar;

/* Alarm Structure definition */
struct AlarmTime_s
{
  uint8_t SecLow;
  uint8_t SecHigh;
  uint8_t MinLow;
  uint8_t MinHigh;
  uint8_t HourLow;
  uint8_t HourHigh;
};
extern struct AlarmTime_s s_AlarmStructVar;

/* Date Structure definition */
struct Date_s
{
  uint8_t Month;
  uint8_t Day;
  uint16_t Year;
};
extern struct Date_s s_DateStructVar;

/* Alarm Date Structure definition */
struct AlarmDate_s
{
  uint8_t Month;
  uint8_t Day;
  uint16_t Year;
};
extern struct AlarmDate_s s_AlarmDateStructVar;
typedef struct
{
	uint8_t ADCT_BGS_RTC_Sec;
	uint8_t ADCT_BGS_RTC_Min;
	uint8_t ADCT_BGS_RTC_Hour;
	uint8_t ADCT_BGS_RTC_Day;
	uint8_t ADCT_BGS_RTC_Month;
	uint16_t ADCT_BGS_RTC_Year;
}ADCT_RTC_Types;
/* Exported constants --------------------------------------------------------*/
#define BATTERY_REMOVED 98
#define BATTERY_RESTORED 99
#define SECONDS_IN_DAY 86399
#define CONFIGURATION_DONE 0xAAAA
#define CONFIGURATION_RESET 0x0000
#define OCTOBER_FLAG_SET 0x4000
#define MARCH_FLAG_SET 0x8000
#define DEFAULT_DAY 9
#define DEFAULT_MONTH 7
#define DEFAULT_YEAR 2017
#define DEFAULT_HOURS 10
#define DEFAULT_MINUTES 37
#define DEFAULT_SECONDS 0
#define LEAP 1
#define NOT_LEAP 0

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_Configuration(void);
void SetTime(uint8_t,uint8_t,uint8_t);
void SetAlarm(uint8_t,uint8_t,uint8_t);
void SetDate(uint8_t,uint8_t,uint16_t);
void RTC_Application(void);
void DateUpdate(void);
uint16_t WeekDay(uint16_t,uint8_t,uint8_t);
uint8_t CheckLeap(uint16_t);
void DisplayTime(void);
void DisplayAlarm(void);
void RTC_NVIC_Configuration(void);
void GPIO_Configuration(void);
void CheckForDaysElapsed(void);
void SummerTimeCorrection(void);
void ReturnFromStopMode(void);
void ADCT_BGS_RTC_Init(void);
void ADCT_BGS_Run_RTC(ADCT_BGS_RTC_Types  *ptr);
void SetTime(uint8_t Hour,uint8_t Minute,uint8_t Seconds);
void SetDate(uint8_t Day, uint8_t Month, uint16_t Year);
void RTC_IRQHandler(void);
void ADCT_BGS_Setting_Date_Time(ADCT_BGS_RTC_Types  *ptr);
#endif /* MODULES_RTC_INC_ADCT_BGS_RTC_H_ */
