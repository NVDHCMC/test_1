/*
 * ADCT_Sensor.c
 *
 *  Created on: Dec 26, 2016
 *      Author: Administrator
 */
#include "ADCT_Sensor.h"

void ADCT_BGS_Sensor_Input_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(ADCT_BGS_INPUT_CLK|ADCT_BGS_AC_GEN_INPUT_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_INT_SMOKE|ADCT_BGS_INT_DOOR|ADCT_BGS_INT_BURGLAR|ADCT_BGS_INT_FIRE|ADCT_BGS_INT_WATER;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ADCT_BGS_INPUT_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_INPUT_AC|ADCT_BGS_INPUT_GEN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ADCT_BGS_AC_GEN_INPUT_PORT, &GPIO_InitStructure);
}
