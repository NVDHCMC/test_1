/*
 * ADCT_Relay.c
 *
 *  Created on: Dec 25, 2016
 *      Author: Administrator
 */
#include "ADCT_Relay.h"
void ADCT_Relay_Init(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(ADCT_BGS_CTRL_RELAY_S01_CLK|ADCT_BGS_CTRL_RELAY_S02_CLK|ADCT_BGS_CTRL_RELAY_S03_CLK|ADCT_BGS_CTRL_RELAY_S04_CLK|ADCT_BGS_CTRL_RELAY_S05_CLK|ADCT_BGS_CTRL_RELAY_S06_CLK|ADCT_BGS_CTRL_RELAY_S07_CLK|ADCT_BGS_CTRL_RELAY_S08_CLK|ADCT_BGS_CTRL_RELAY_B09_CLK|ADCT_BGS_CTRL_RELAY_B10_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S01;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S01_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S02;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S02_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S03;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S03_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S04;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S04_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S05;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S05_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S06;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S06_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S07;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S07_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_S08;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_S08_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_B09;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_B09_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_CTRL_RELAY_B10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADCT_BGS_CTRL_RELAY_B10_PORT, &GPIO_InitStructure);
}


