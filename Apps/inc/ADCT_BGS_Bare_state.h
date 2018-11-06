/*
 * ADCT_BGS_Bare_state.h
 *
 *  Created on: May 6, 2017
 *      Author: Administrator
 */

#ifndef APPS_INC_ADCT_BGS_BARE_STATE_H_
#define APPS_INC_ADCT_BGS_BARE_STATE_H_
#include "ADCT_BGS_DataTypes.h"
typedef struct
{
  void (*ptrFunct) (void);
  uint8_t NextState;
} FSM_STATE_TABLE;
typedef enum
{
	IDLE = 0 ,
	ALARM  ,
	SETUP_CHOOSE_DATALOG,
	SETUP_VIEW_DATALOG,
	SETUP_CHOOSE_PARAMETERS,
	SETUP_ENTER_PARAMETERS,
	NUM_MAIN_STATE
}BGS_MainStateDataTypes;
typedef enum
{
	ENTER_BUTTON = 0 ,
	CANCEL_BUTTON_ALARM_ON,
	CANCEL_BUTTON_ALARM_OFF,
	UP_BUTTON,
	DOWN_BUTTON,
	NUM_EVENTS
}BGS_BareEventDataTypes;
#endif /* APPS_INC_ADCT_BGS_BARE_STATE_H_ */
