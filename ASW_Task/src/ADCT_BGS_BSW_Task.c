/*
 * ADCT_BGS_BSW_Task.c
 *
 *  Created on: Mar 22, 2017
 *      Author: Administrator
 */
#include "ADCT_BGS_BSW_Task.h"
#include "HardwareCfg.h"
#include "ADCT_DHT22.h"
#include "ADCT_PhSensor.h"
extern FPT_SystemStatus    GstSystemStatus;
ADCT_BGS_ASW_TaskDatatypes g_ASW_Task_flag;
void ADCT_BGS_ASW_100ms_TaskHandler(TM_DELAY_Timer_t* timer, void * parameter)
{
	(void)timer;
	(void)parameter;
	ADCT_Ph_GetSample(&Data_avai);
	ADCT_DHT22_FPT_GetValue(&GstSystemStatus.dhtData);
	g_ASW_Task_flag.ASW_100ms_flag = TRUE;
}
void ADCT_BGS_ASW_100ms_Init(void)
{
	TM_DELAY_TimerCreate(100, 1, 1, ADCT_BGS_ASW_100ms_TaskHandler, NULL);
}

void ADCT_BGS_ASW_10ms_TaskHandler(TM_DELAY_Timer_t* timer, void * parameter)
{
	(void)timer;
	(void)parameter;
	g_ASW_Task_flag.ASW_10ms_flag = ADCT_TRUE;
}

void ADCT_BGS_ASW_1ms_TaskHandler(TM_DELAY_Timer_t* timer, void * parameter)
{
	(void)timer;
	(void)parameter;
	g_ASW_Task_flag.ASW_1ms_flag = ADCT_TRUE;
}

void ADCT_BGS_ASW_1s_TaskHandler(TM_DELAY_Timer_t* timer, void * parameter)
{
	(void)timer;
	(void)parameter;
	g_ASW_Task_flag.ASW_1s_flag = ADCT_TRUE;

}
void ADCT_BGS_ASW_10ms_Init(void)
{
	g_ASW_Task_flag.ASW_10ms_flag = ADCT_FALSE;
	TM_DELAY_TimerCreate(10, 1, 1, ADCT_BGS_ASW_10ms_TaskHandler, NULL);
}

void ADCT_BGS_ASW_1ms_Init(void)
{
	g_ASW_Task_flag.ASW_1ms_flag =ADCT_FALSE;
	TM_DELAY_TimerCreate(1, 1, 1, ADCT_BGS_ASW_1ms_TaskHandler, NULL);
}

void ADCT_BGS_ASW_1s_Init(void)
{
	g_ASW_Task_flag.ASW_1s_flag =ADCT_FALSE;
	TM_DELAY_TimerCreate(1000, 1, 1, ADCT_BGS_ASW_1s_TaskHandler, NULL);
}
