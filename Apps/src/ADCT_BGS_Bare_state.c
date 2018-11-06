/*
 * ADCT_BGS_Bare_state.c
 *
 *  Created on: May 6, 2017
 *      Author: Administrator
 */
#include "ADCT_BGS_Bare_state.h"
#include "ADCT_BGS_BUTTON.h"
#ifdef ADCT_BGS_USE_STANDALONE
#include "ADCT_LCD_16x2.h"
extern ADCT_LCD_Buffer_Types LCD_Buffer;
#endif
BGS_MainStateDataTypes ActState = IDLE;
void adct_bgs_choose_views		(void);
void adct_bgs_enter_parameter   (void);
void adct_bgs_choose_datalog    (void);
void adct_bgs_runstate_idle     (void);
void adct_bgs_runstate_alarm    (void);
void adct_bgs_view_datalog		(void);
void adct_bgs_choose_views		(void)
{
#ifdef ADCT_BGS_USE_STANDALONE
	sprintf(LCD_Buffer.lcd_line1, "EN  -> CAI DAT  ");
	sprintf(LCD_Buffer.lcd_line2, "UP-> XEM NHAT KY");
	(void)lcd_update(LCD_Buffer.lcd_line1, LCD_Buffer.lcd_line2);
#endif
}
void adct_bgs_enter_parameter(void)
{

}
void adct_bgs_choose_datalog(void)
{
#ifdef ADCT_BGS_USE_STANDALONE
	sprintf(LCD_Buffer.lcd_line1, "EN  -> CAI DAT  ");
	sprintf(LCD_Buffer.lcd_line2, "UP-> XEM NHAT KY");
	(void)lcd_update(LCD_Buffer.lcd_line1, LCD_Buffer.lcd_line2);
#endif
}
void adct_bgs_runstate_idle(void)
{
#ifdef ADCT_BGS_USE_STANDALONE
	sprintf(LCD_Buffer.lcd_line1, "    ADCTTeam    ");
	sprintf(LCD_Buffer.lcd_line2, "  GIAM SAT V1.0 ");
	(void)lcd_update(LCD_Buffer.lcd_line1, LCD_Buffer.lcd_line2);
#endif
}
void adct_bgs_runstate_alarm(void)

{
#ifdef ADCT_BGS_USE_STANDALONE
	sprintf(LCD_Buffer.lcd_line1, "CANH BAO CO LUA ");
	sprintf(LCD_Buffer.lcd_line2, "                ");
	(void)lcd_update(LCD_Buffer.lcd_line1, LCD_Buffer.lcd_line2);
#endif
}
void adct_bgs_view_datalog(void)
{

}



FSM_STATE_TABLE adct_bgs_main_state_table[NUM_MAIN_STATE][NUM_EVENTS] =
{
		{
				{adct_bgs_choose_views, SETUP_CHOOSE_PARAMETERS},
				{adct_bgs_runstate_idle, IDLE},
				{adct_bgs_runstate_idle, IDLE},
				{adct_bgs_runstate_idle, IDLE},
				{adct_bgs_runstate_idle,IDLE}
		},
		{
				{adct_bgs_choose_views, SETUP_CHOOSE_PARAMETERS},
				{adct_bgs_runstate_alarm, ALARM},
				{adct_bgs_runstate_idle,IDLE},
				{adct_bgs_runstate_alarm, ALARM},
				{adct_bgs_runstate_alarm, ALARM},
		},
		{
				{adct_bgs_view_datalog, SETUP_VIEW_DATALOG},
				{adct_bgs_runstate_alarm, ALARM},
				{adct_bgs_runstate_idle, IDLE},
				{adct_bgs_choose_datalog, SETUP_CHOOSE_DATALOG},
				{adct_bgs_choose_views, SETUP_CHOOSE_PARAMETERS}
		},
		{
				{adct_bgs_view_datalog, SETUP_VIEW_DATALOG},
				{adct_bgs_choose_datalog, SETUP_CHOOSE_DATALOG},
				{adct_bgs_choose_datalog, SETUP_CHOOSE_DATALOG},
				{adct_bgs_view_datalog, SETUP_VIEW_DATALOG},
				{adct_bgs_view_datalog,SETUP_VIEW_DATALOG}
		},
		{
				{adct_bgs_enter_parameter, SETUP_ENTER_PARAMETERS},
				{adct_bgs_runstate_alarm,ALARM },
				{adct_bgs_runstate_idle,IDLE },
				{adct_bgs_choose_views, SETUP_CHOOSE_PARAMETERS},
				{adct_bgs_choose_views, SETUP_CHOOSE_PARAMETERS}
		},
		{
				{adct_bgs_enter_parameter, SETUP_ENTER_PARAMETERS},
				{adct_bgs_enter_parameter, SETUP_ENTER_PARAMETERS},
				{adct_bgs_enter_parameter, SETUP_ENTER_PARAMETERS},
				{adct_bgs_enter_parameter, SETUP_ENTER_PARAMETERS},
				{adct_bgs_enter_parameter, SETUP_ENTER_PARAMETERS}
		}
};


void BUTTON_ENTER_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType)
 {

	(void)(ButtonPtr);
	(void)(PressType);
	 if(adct_bgs_main_state_table[ActState][ENTER_BUTTON].ptrFunct != NULL)
	 {
		 adct_bgs_main_state_table[ActState][ENTER_BUTTON].ptrFunct();
		 ActState =  adct_bgs_main_state_table[ActState][ENTER_BUTTON].NextState;
	 }
}
 void BUTTON_CANCEL_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType)
{
		(void)(ButtonPtr);
		(void)(PressType);
	 if(adct_bgs_main_state_table[ActState][CANCEL_BUTTON_ALARM_OFF].ptrFunct != NULL)
	 {
		 adct_bgs_main_state_table[ActState][CANCEL_BUTTON_ALARM_OFF].ptrFunct();
		 ActState =  adct_bgs_main_state_table[ActState][CANCEL_BUTTON_ALARM_OFF].NextState;
	 }
}
 void BUTTON_UP_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType)
{
		(void)(ButtonPtr);
		(void)(PressType);
	 if(adct_bgs_main_state_table[ActState][UP_BUTTON].ptrFunct != NULL)
	 {
		 adct_bgs_main_state_table[ActState][UP_BUTTON].ptrFunct();
		 ActState =  adct_bgs_main_state_table[ActState][UP_BUTTON].NextState;
	 }
}
 void BUTTON_DOWN_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType)
{
		(void)(ButtonPtr);
		(void)(PressType);
	 if(adct_bgs_main_state_table[ActState][DOWN_BUTTON].ptrFunct != NULL)
	 {
		 adct_bgs_main_state_table[ActState][DOWN_BUTTON].ptrFunct();
		 ActState =  adct_bgs_main_state_table[ActState][DOWN_BUTTON].NextState;
	 }
}
