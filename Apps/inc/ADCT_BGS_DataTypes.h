/**
 ******************************************************************************
 * @file    adct_datatypes_fpt.h
 * @author  ADCT Team - LinhNT
 * @version V0.0.1
 * @date    6-Aug-2016
 * @brief   This file contains data structure for SNMP_FPT project.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCT_BGS_DATATYPES_FPT_H
#define __ADCT_BGS_DATATYPES_FPT_H
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/**
 * @brief Data definition of DHT22
 */
/******************************************************************************
 *
 * States of the system
 * By Tran Dien Viet Phuong Apr 30 2012
 *
 ******************************************************************************/

// Main states of system (see state diagram for more details)
// When in this state, system read temp and humidity and display on LCD screen
#define STATE_IDLE                      1
#define STATE_SETUP_CHOOSE_PARAMETERS   2
#define STATE_SETUP_CHOOSE_DATALOG      3
#define STATE_SETUP_VIEW_DATALOG        4
#define STATE_SETUP_ENTER_PARAMETERS    5
#define STATE_ALARM 6
#define ADCT_TRUE 1
#define ADCT_FALSE 0
typedef enum
{
	MAINSTATE_IDLE = 1,
	MAINSTATE_SETUP_CHOOSE_PARAMETERS,
	MAINSTATE_SETUP_CHOOSE_DATALOG,
	MAINSTATE_SETUP_VIEW_DATALOG,
	MAINSTATE_SETUP_ENTER_PARAMETERS,
	MAINSTATE_ALARM
}ADCT_BGS_Main_States_Types;
typedef enum
{
	SUBSTATE_SETUP_LowTemp = 1,
	SUBSTATE_SETUP_HighTemp,
	SUBSTATE_SETUP_LowHumidity,
	SUBSTATE_SETUP_HighHumidity,
	SUBSTATE_SETUP_T_MIN,
	SUBSTATE_SETUP_T_DELOFF,
	SUBSTATE_SETUP_T_CHANGE,
	SUBSTATE_SETUP_CURRENT_DATE,
	SUBSTATE_SETUP_CURRENT_MONTH,
	SUBSTATE_SETUP_CURRENT_YEAR,
	SUBSTATE_SETUP_CURRENT_TIME_HOUR,
	SUBSTATE_SETUP_CURRENT_TIME_MINUTE,
	SUBSTATE_SETUP_ALARM_ENABLE,
	SUBSTATE_SETUP_ALARM_TIME,
	SUBSTATE_SETUP_ALARM_ENABLE_DOOR_BURGLAR
}ADCT_BGS_Sub_States_Types;

// Alarm substates
#define SUBSTATE_ALARM_FIRE_ON        0
#define SUBSTATE_ALARM_FIRE_OFF       1
#define SUBSTATE_ALARM_SMOKE_ON       2
#define SUBSTATE_ALARM_SMOKE_OFF      3
#define SUBSTATE_ALARM_DOOR_ON        4
#define SUBSTATE_ALARM_DOOR_OFF       5
#define SUBSTATE_ALARM_BURGLAR_ON     6
#define SUBSTATE_ALARM_BURGLAR_OFF    7
#define SUBSTATE_ALARM_WATER_ON       8
#define SUBSTATE_ALARM_WATER_OFF      9
#define SUBSTATE_ALARM_AC_ON          10
#define SUBSTATE_ALARM_AC_OFF         11
#define SUBSTATE_ALARM_GENERATIOR_ON  12
#define SUBSTATE_ALARM_GENERATIOR_OFF 13
#define SUBSTATE_ALARM_LOW_TEMP_ON    14
#define SUBSTATE_ALARM_LOW_TEMP_OFF   15
#define SUBSTATE_ALARM_HIGH_TEMP_ON   16
#define SUBSTATE_ALARM_HIGH_TEMP_OFF  17
#define SUBSTATE_ALARM_LOW_HUMD_ON    18
#define SUBSTATE_ALARM_LOW_HUMD_OFF   19
#define SUBSTATE_ALARM_HIGH_HUMD_ON   20
#define SUBSTATE_ALARM_HIGH_HUMD_OFF  21



/******************************************************************************
 *
 * Constants
 *
 ******************************************************************************/
// Define TRUE FALSE for Boolean variables
#define TRUE  1
#define FALSE 0

// Define constant for keypad variables
#define KEY_PRESSED     1
#define KEY_NOT_PRESSED 0

// Define constant for alarm variables
#define ALARMING   1
#define ALARM_FREE 0

// Number of characters per line (LCD display)
#define LINE1 0
#define LINE2 1

// Value of key debouncing
#define KEY_DEBOUNCE    25

// Size of event available for 23C128 (16KByte, 8Byte each event)
#define EEPROM_EVENT_SIZE 2048

#define EEPROM_WAIT_VALUE 0x10000
//Keys of keypad
#define NO_KEY      5                 // No key has been pressed at the moment
#define KEY_UP      3
#define KEY_DOWN    4
#define KEY_SETUP   1
#define KEY_CANCEL  2





// Screen saver time
#define SCREEN_SAVER_TIME 60 // Number of seconds the idle screen go to mobifone

// Alarm running time // Number of seconds the siren allowed to sound
#define ALARM_RUNNING_TIME 60

/******************************************************************************
 *
 * System variables
 *
 ******************************************************************************/
typedef struct {
	uint8_t hour;	 		// Hours 0..24
	uint8_t minute;	 	// Minute 0..59
	uint8_t second; 		// Second 0..59
} ADCT_BGS_Time_Types;

typedef struct {
	uint8_t date; 		// Date 0..31
	uint8_t month; 		// Month 0..12
	uint16_t year; 		// Year 0..99
} ADCT_BGS_Date_Types;

typedef struct {
	uint8_t flag;
	uint8_t pin_value;
	uint16_t id;
} ADCT_BGS_Alarm_Types;

typedef struct {
	uint8_t  RTC_Sec;
	uint8_t  RTC_Min;
	uint8_t  RTC_Hour;
	uint8_t  RTC_Mday;
	uint8_t  RTC_Mon;
	uint16_t RTC_Year;
} ADCT_BGS_RTC_Types;

typedef struct {
	uint8_t debug1, debug2;
	uint8_t switch_air;
	uint8_t _10msecond;   // CPU tick, used to scan the values of the LED display and show them
	uint8_t state;     // Current state of the system
	uint8_t substate_setup; // Substate indicate which parameter being set (see state diagrams for more details)
	uint8_t  key;
	uint8_t  key_debouncing; // Debouncing value for keypad
	uint16_t temperature; // Current temperature
	uint16_t humidity;  // Current humidity
	uint16_t alarm_id; // ID of alarm, using to identify each and every alarm has occurred
	uint16_t temp_alarm_id; // Temporary alarm id for view datalog
	uint8_t  blink; // Switch every second, the LCD is blinked via this variable

	// Alarms
	ADCT_BGS_Alarm_Types fire_alarm;
	ADCT_BGS_Alarm_Types smoke_alarm;
	ADCT_BGS_Alarm_Types door_alarm;
	ADCT_BGS_Alarm_Types burglar_alarm;
	ADCT_BGS_Alarm_Types water_alarm;
	ADCT_BGS_Alarm_Types AC_alarm;
	ADCT_BGS_Alarm_Types generator_alarm;
	ADCT_BGS_Alarm_Types low_temperature_alarm;
	ADCT_BGS_Alarm_Types fan_temperature_alarm;
	ADCT_BGS_Alarm_Types high_temperature_alarm;
	ADCT_BGS_Alarm_Types low_humidity_alarm;
	ADCT_BGS_Alarm_Types high_humidity_alarm;

	// Parameters
	uint16_t low_temperature_t1; 	// Value of low temperature
	uint16_t high_temperature_t2; // Value of high temperature
	uint16_t fan_temperature_t3;  // Value to run fan
	uint16_t low_humidity_h1; 	// Value of low humidity
	uint16_t high_humidity_h2; 	// Value of high humidity
	uint8_t  time_min; 			// Time duration which 1 air conditioner must run before switch to another (5 to 30minute)
	uint8_t  time_deloff; 		// Time 2 air conditioners must be off when both AC and GEN fail
	ADCT_BGS_Time_Types     time_change; 			// Store the time remaining from switching between 2 air conditioners
	ADCT_BGS_Time_Types     current_time; 		// Current time for datalog
	ADCT_BGS_Date_Types     current_date; 		// Current date for datalog
	uint8_t  alarm_enable; 						// If OFF, disable alarm and siren
	uint8_t  alarm_enable_door_burglar; 			// If OFF, even when door alarm is activated, siren doesn't sound
	ADCT_BGS_Time_Types    alarm_time; 			// Time duration for alarm once it's set
	uint8_t  				cancel_pressed; 		// Variable holds the cancel button state in alarm state
	uint16_t    genTimeThisMonth;
	uint16_t    genTimeLastMonth;
	uint16_t    genTimeLastTwoMonth;
	uint8_t     generatorRunningTime; 			// If one minute has passed since generator is running, increase the genTimeThisMonth variable
	uint8_t     isNewMonth;
	uint16_t    currentMonth;          			// This variable holds the value of current month by this form:
	// Month 01/2001: currentMonth = 1
	// Month 02/2001: currentMonth = 2
	// Month 12/2001: currentMonth = 12
	// Month 01/2002: currentMonth = 13
	// Month 12/2002: currentMonth = 24
	// Month 01/2012: currentMonth = 135
	// Month 12/2012: currentMonth = 144
	// Month 01/2099: currentMonth = 1177
	// Month 12/2099: currentMonth = 1188

	// Temporary parameters
	uint16_t temp_low_temperature_t1;  // Value of low temperature
	uint16_t temp_high_temperature_t2; // Value of high temperature
	uint16_t temp_fan_temperature_t3;  // Value to run fan
	uint16_t temp_low_humidity_h1;     // Value of low humidity
	uint16_t temp_high_humidity_h2;    // Value of high humidity
	uint8_t  temp_time_min;            // Time duration which 1 air conditioner must run before switch to another (5 to 30minute)
	uint8_t  temp_time_deloff;         // Time 2 air conditioners must be off when both AC and GEN fail
	ADCT_BGS_Time_Types     temp_time_change;         // Store the time remaining from switching between 2 air conditioners
	ADCT_BGS_Time_Types     temp_current_time;        // Current time for datalog
	ADCT_BGS_Date_Types     temp_current_date;        // Current date for datalog
	uint8_t  temp_alarm_enable;        // If OFF, disable alarm and siren
	ADCT_BGS_Time_Types     temp_alarm_time;          // Time duration for alarm once it's set
	uint8_t  temp_alarm_enable_door_burglar; // If OFF, the door and burglar alarms dont sound the siren

	// Output states
	uint8_t state_air_1_output;
	uint8_t state_air_2_output;
	uint8_t state_fan_output;
	uint8_t state_siren_output;
	uint8_t siren_timer_run;
	uint8_t air_1_timer_run;
	uint8_t air_2_timer_run;
	uint8_t fan_timer_run;
	uint8_t deloff_timer_run;
	uint8_t deloff_timer_elapsed;

	ADCT_BGS_Time_Types min_timer_air_1;
	ADCT_BGS_Time_Types min_timer_air_2;
	ADCT_BGS_Time_Types min_timer_fan;
	ADCT_BGS_Time_Types deloff_timer;

	// Variable for screen saver
	uint8_t screen_saver_timer;
	uint8_t screen_saver_timer_flag;

	// Variable for running timer
	uint8_t alarm_running_timer;
	uint8_t alarm_running_timer_flag;

	// Variable for scrolling the LCD display in case of an 16x2 LCD
	uint8_t scroll_lcd;

	// Variable for smart system, use to detect if any fan is failed
	uint8_t smart_system;

	// Variable for detecting if time_change is set to 0 hour 0 minute
	uint8_t zero_time_change;

	// Variable to display the LCD
	char lcd_line1[16];
	char lcd_line2[16];

	/* RTC mm/dd/yyy hh:mm:ss of the system */
	ADCT_BGS_RTC_Types RTC_System;
} ADCT_BGS_System;
/* Exported types ------------------------------------------------------------*/
/** @defgroup SNMP_FPT project Data.
  * @{
  */
#ifdef ADCT_MODBUS_ENABLED
/**
  * @brief Data definition of MODBUS
  */
typedef struct
{
    uint16_t        usCurrentMainVoltage;
    uint16_t        usCurrentGenFuel;
    uint16_t        usCurrentGenVoltage;
    uint16_t        usCurrentGenBatteryVoltage;
    uint16_t        usCurrentGenCoolantTemp;
    uint16_t        usCurrentGenEngineSpeed;
    uint16_t        usCurrentGenOilPressure;
    uint16_t        usCurrentGenCurVoltage;
    uint16_t        usCurrentMainFreq;
    uint16_t        usCurrentGenFreq;
    uint16_t	    usCurrentGenMode;
} MODBUS_MODULE_DATA;
#endif /* ADCT_MODBUS_ENABLED */

#ifdef ADCT_DHT22_ENABLED
/**
  * @brief Data definition of DHT22
  */
typedef struct
{
    uint16_t usTemp_Main;                /* Temparature of Main sensor  */
    uint16_t usHumid_Main;               /* Humidity of Main sensor     */
    uint16_t usTemp_Sub;                 /* Temparature of Sub sensor  */
    uint16_t usHumid_Sub;                /* Humidity of Sub sensor     */
} DHT22_MODULE_DATA;
#endif /* ADCT_DHT22_ENABLED */

/**
  * @brief Data definition of I/O (Sensors, Actuator...)
  */
typedef struct
{
    uint8_t       	ucFire;
    uint8_t       	ucSmoke;
    uint8_t       	ucWater;
    uint8_t       	ucDoor[5];
    uint8_t       	ucBurglar;
    uint8_t       	ucAIR[4];
    uint8_t       	ucFAN[4];
    uint8_t	   	    ucBuzzer;
    uint8_t	   	    ucLed;
} IO_MODULE_DATA;
typedef struct
{
	float voltage;
	float current;
	float energy;
	float power;
}AC_Value;
/**
  * @brief System Configuration
  */
typedef struct STag_UserSetting
{
    uint16_t        usLowTemperatureT1;
    uint16_t        usHighTemperatureT2;
    uint16_t        usFanTemperatureT3;
    uint16_t        usLowHumidityH1;
    uint16_t        usHighHumidityH2;
    uint16_t        usLowMainVoltage;
    uint16_t        usHighMainVoltage;
    uint16_t        usGenFuelLowValue;
    uint16_t        usGenVoltageAlarmValue;
    uint16_t        usGenBatteryVoltageAlarmValue;
    uint16_t        usGenCoolantTempAlarmValue;
    uint16_t        usGenEngineSpeedAlarmValue;
    uint16_t        usGenOilPressureAlarmValue;
    uint32_t        ulSwitchingTime;
    uint32_t        ulMinSwitchingTime;
    uint32_t        ulStayOffTime;
    uint8_t         ucEnableAlarm;
    uint8_t         ucDoorBurglarEnableAlarm;
    uint32_t        ulAlarmDuration;
    uint8_t        	ucSystemIP[4];
    uint8_t        	ucSystemMask[4];
    uint8_t 		ucSystemGateWay[4];
    uint8_t 		ucSNMPSystem[4];
} FPT_UserSetting;

typedef struct
{
	ADCT_BGS_Time_Types SWitchingTimer;
	ADCT_BGS_Time_Types MinSwitchingTimerAIR1_2;
	ADCT_BGS_Time_Types MinSwitchingTimerAIR3_4;
	ADCT_BGS_Time_Types StayOffTimer;
	ADCT_BGS_Time_Types BuzzerTimer;
	ADCT_BGS_Time_Types AlarmTimer;
	ADCT_BGS_Time_Types MinRuningTimerFan;
}ADCT_BGS_TimerManagedDatatypes;
typedef struct
{
	uint8_t AIR_1_2_OUTPUT_STATE;
	uint8_t AIR_3_4_OUTPUT_STATE;
	uint8_t FAN_OUTPUT_STATE;
	uint8_t BUZZER_OUTPUT_STATE;
	uint8_t BUZZER_TIMER_STATE;
	uint8_t AIR_1_2TIMER_STATE;
	uint8_t AIR_3_4TIMER_STATE;
	uint8_t FAN_TIMER_STATE;
	uint8_t DELAY_OFFTIMER_STATE;
	uint8_t DELAY_OFFTIMER_ELAPSED;
	uint8_t WITCH_AIR;
	uint8_t ALARM_TIMER_Flag;
	uint8_t ALARM_RUNNING_Timer_Cnt;
}ADCT_BGS_StateOutputDatatypes;

typedef struct
{
	uint8_t ASW_1s_flag;
	uint8_t ASW_100ms_flag;
	uint8_t ASW_10ms_flag;
	uint8_t ASW_1ms_flag;
}ADCT_BGS_ASW_TaskDatatypes;
/**
  * @brief System Status
  */
typedef struct STag_SystemStatus
{
#ifdef ADCT_MODBUS_ENABLED
	/* MODBUS data */
	MODBUS_MODULE_DATA	mbData;
#endif /* ADCT_MODBUS_ENABLED */
#ifdef ADCT_DHT22_ENABLED
	/* DHT22 data */
	DHT22_MODULE_DATA	dhtData;
#endif /* ADCT_DHT22_ENABLED */
	/* I/O data */
	IO_MODULE_DATA		ioData;

	/* AC measurement */
	AC_Value acData;
} FPT_SystemStatus;

/**
  * @brief Temperature Mode/Warning
  */
typedef enum
{
    TEMP_MODE_NORMAL    = 0,
    TEMP_MODE_LOW,
    TEMP_MODE_HIGH,
    TEMP_MODE_FAN,
    TEMP_MODE_STAYOFF,
    TEMP_MODE_MANUAL,
    TEMP_MODE_NONE
} FPT_TempMode;

/**
  * @brief Temperature State Struct
  */
typedef struct
{
    FPT_TempMode    preState;   /* Previous State (reserved) */
    FPT_TempMode    curState;   /* Current State */
    FPT_TempMode    nextState;  /* Next State */
} FPT_TempState;

/**
  * @brief Other Warning Mask
  */
typedef enum
{
	WARN_RESERVED = 0,
	WARN_HIGHTEMP,
	WARN_LOWTEMP,
	WARN_HIGHFANTEMP,
	WARN_LOWHUMID   ,
	WARN_HIGHHUMID,
	WARN_FIRE,
	WARN_SMOKE,
	WARN_DOOR,
	WARN_BURGLAR,
	WARN_WATER,
	WARN_HIGHMAINVOLTAGE,
	WARN_LOWMAINVOLTAGE,
	WARN_RESERVED_2,
	WARN_LOWFUEL,
	WARN_LOWOILPRESS,
	WARN_LOWGENVOLTAGE,
	WARN_LOWGENBAT,
	WARN_LOWENSPEED,
	WARN_LOWCOOLTEMP,
	WARN_RESERVED_3,
} FPT_OtherWarning;

/**
  * @brief System Warning Mask
  */
typedef struct
{
    uint8_t       State;
    uint32_t            othWarn;
} FPT_SystemWarning;

/**
  * @brief Definition of Alarm ID. The ID will be stored to event log
  */
typedef enum ETag_AlarmTagID
{
    ALARM_FIRE_START = 0,
    ALARM_FIRE_STOP,
    ALARM_SMOKE_START,
    ALARM_SMOKE_STOP,
    ALARM_DOOR1_START,
    ALARM_DOOR1_STOP,
    ALARM_DOOR2_START,
    ALARM_DOOR2_STOP,
    ALARM_DOOR3_START,
    ALARM_DOOR3_STOP,
    ALARM_DOOR4_START,
    ALARM_DOOR4_STOP,
    ALARM_DOOR5_START,
    ALARM_DOOR5_STOP,
    ALARM_WATER_START,
    ALARM_WATER_STOP,
    ALARM_BURGLAR_START,
    ALARM_BURGLAR_STOP,
    ALARM_HIGH_TEMP_START,
    ALARM_HIGH_TEMP_STOP,
    ALARM_LOW_TEMP_START,
    ALARM_LOW_TEMP_STOP,
    ALARM_FAN_TEMP_START,
    ALARM_FAN_TEMP_STOP,
    ALARM_LOW_HUMD_START,
    ALARM_LOW_HUMD_STOP,
    ALARM_HIGH_HUMD_START,
    ALARM_HIGH_HUMD_STOP,
    ALARM_LOW_MAIN_START,
    ALARM_LOW_MAIN_STOP,
    ALARM_HIGH_MAIN_START,
    ALARM_HIGH_MAIN_STOP,
    ALARM_LOW_FUEL_START,
    ALARM_LOW_FUEL_STOP,
    ALARM_GEN_START,
    ALARM_GEN_STOP,
    ALARM_BAT_START,
    ALARM_BAT_STOP,
    ALARM_COOLANT_START,
    ALARM_COOLANT_STOP,
    ALARM_ENGINE_START,
    ALARM_ENGINE_STOP,
    ALARM_OIL_START,
    ALARM_OIL_STOP,
    ALARM_RESERVED
} FPT_AlarmTagID_t;

/**
  * @brief Definition of Alarm Log. Log will contain time + content of alarm
  */
typedef struct STag_AlarmLog
{
    /* Date of the Alarm ON or OFF - Range: 0..31 */
    uint8_t             ucDay;
    /* Month of the Alarm ON or OFF - Range: 1..12 */
    uint8_t             ucMonth;
    /* Year of the Alarm ON or OFF - Range: 0x0..0xFFFF */
    uint16_t            ucYear;
    /* Hour of the Alarm ON or OFF - Range: 0..23 */
    uint8_t             ucHour;
    /* Minute of the Alarm ON or OFF - Range: 0..59 */
    uint8_t             ucMinute;
    /* Second of the Alarm ON or OFF  - Range: 0..59 */
    uint8_t             ucSecond;
    /* Alarm ID ON/OFF */
    FPT_AlarmTagID_t    AlarmID;
    /* Alarm String ON/OFF */
    char                AlarmStr[15];
} FPT_AlarmLog_t;

/**
  * @brief Definition of temp timer
  */
typedef enum
{
    TEMP_TIMER_START = 0,
    TEMP_TIMER_MINSWITCH,
    TEMP_TIMER_STAYOFF,
    TEMP_TIMER_SWITCH
} FPT_TempTimer_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/* Warning ON */
#define WARN_ON     1
/* Warning OFF */
#define WARN_OFF	0

/* Flag enabled */
#define FLG_ON     	1
/* Flag disabled */
#define FLG_OFF		0

/* Exported macro ------------------------------------------------------------*/
#define ReadBit(value, bit)             (((value) >> (bit)) & 0x01)
#define SetBit(value, bit)              ((value) |= (1UL << (bit)))
#define ClearBit(value, bit)            ((value) &= ~(1UL << (bit)))
#define WriteBit(value, bit, bitvalue)  (bitvalue ? SetBit(value, bit) : ClearBit(value, bit))

/* Exported variables --------------------------------------------------------*/
/* System Status */
extern FPT_SystemStatus    GstSystemStatus;
/* System Settings */
extern FPT_UserSetting     GstSystemSetting;
/* System Warning */
extern FPT_SystemWarning   GstSystemWarning;


#endif
