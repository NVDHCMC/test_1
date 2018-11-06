/*
 * ADCT_BGS_BUTTON.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Administrator
 */

#ifndef INC_ADCT_BGS_BUTTON_H_
#define INC_ADCT_BGS_BUTTON_H_

#include "stm32f10x.h"
#include "tm_stm32_button.h"
#include "string.h"
/* Button callback function */
 void BUTTON_ENTER_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType);
 void BUTTON_CANCEL_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType);
 void BUTTON_UP_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType);
 void BUTTON_DOWN_Callback(TM_BUTTON_t* ButtonPtr, TM_BUTTON_PressType_t PressType);
void ADCT_BGS_Button_Init(void );
#endif /* INC_ADCT_BGS_BUTTON_H_ */
