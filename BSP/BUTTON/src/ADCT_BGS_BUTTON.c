/*
 * ADCT_BGS_BUTTON.c
 *
 *  Created on: Mar 22, 2017
 *      Author: Administrator
 */

#include "ADCT_BGS_BUTTON.h"
#include "HardwareCfg.h"
#ifdef ADCT_BGS_USE_STANDALONE
#include "ADCT_LCD_16x2.h"
#endif
/* Button pointer */
TM_BUTTON_t* EnterButton;
TM_BUTTON_t* CancelButton;
TM_BUTTON_t* UpButton;
TM_BUTTON_t* DownButton;


void ADCT_BGS_Button_Init(void )
{
	/* Enter button initialization  */
	EnterButton = TM_BUTTON_Init(ENTER_BUTTON_GPIO_PORT,ENTER_BUTTON_GPIO_CLK,ENTER_BUTTON_PIN, 0, BUTTON_ENTER_Callback);
	/* ESC button initialization */
	CancelButton = TM_BUTTON_Init(ESC_BUTTON_GPIO_PORT,ESC_BUTTON_GPIO_CLK,ESC_BUTTON_PIN, 0, BUTTON_CANCEL_Callback);
	/* Up button initialization */
	UpButton = TM_BUTTON_Init(UP_BUTTON_GPIO_PORT,UP_BUTTON_GPIO_CLK,UP_BUTTON_PIN, 0, BUTTON_UP_Callback);
	/* Down button initialization */
	DownButton = TM_BUTTON_Init(DOWN_BUTTON_GPIO_PORT,DOWN_BUTTON_GPIO_CLK,DOWN_BUTTON_PIN, 0, BUTTON_DOWN_Callback);

	TM_BUTTON_SetPressTime(EnterButton, 2000, 3000);
	TM_BUTTON_SetPressTime(CancelButton, 2000, 3000);
	TM_BUTTON_SetPressTime(UpButton, 2000, 3000);
	TM_BUTTON_SetPressTime(DownButton, 2000, 3000);
}

