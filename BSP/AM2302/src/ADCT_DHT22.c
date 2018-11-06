/**************************************************************************//**
 * @file     usbserial.c
 * @brief    USB Virtual COM-port (CDC) func source file
 * @version  v1.0
 * @date     24.06.2012
 *
 * @note
 * Copyright (C) 2012 kab
 *
 ******************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <ADCT_DHT22.h>
#include <ADCT_DHT22_exti.h>
#include <ADCT_DHT22_interrupt.h>
#include "inttypes.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "ADCT_BGS_FPT_Delay.h"
#include <string.h>
#include "ADCT_BGS_DataTypes.h"
extern FPT_SystemStatus    GstSystemStatus;
uint32_t ADCT_BGS_Temp[DHT22_NO_Device] ={0,0};
uint32_t ADCT_BGS_Hummid[DHT22_NO_Device] ={0,0};
/******************************************************************************
 * Defines
 *****************************************************************************/

void DHT22_Module_Init_Hardware(void)
{
	vExtiInit();
	ADCT_BGS_DHT22Init();
	vExtiStart();
}
void ADCT_DHT22_FPT_GetValue(DHT22_MODULE_DATA *pDHT22Data)
{
	if(ADCT_BGS_DHT22Measuring(DHT22_Device_1) == ADCT_FALSE)
	{
		if (ADCT_BGS_DHT22_CheckCRC(DHT22_Device_1))
		{
			uint32_t temp_value = 0;
			uint32_t humid_value = 0;

			temp_value = ADCT_BGS_DHT22GetTemp(DHT22_Device_1);
			humid_value = ADCT_BGS_DHT22GetHumidity(DHT22_Device_1) ;
			if(temp_value < 5)
			{

			}
			else
			{
				pDHT22Data->usTemp_Sub = temp_value ;

			}

			if (!humid_value)
			{

			}
			else
			{
				pDHT22Data->usHumid_Sub = humid_value;

			}
		}
	}
#ifdef SUB_DHT22
	if(ADCT_BGS_DHT22Measuring(DHT22_Device_2) == ADCT_FALSE)
	{

		if (ADCT_BGS_DHT22_CheckCRC(DHT22_Device_2))
		{
			uint32_t temp_value = 0;
			uint32_t humid_value = 0;

			temp_value = ADCT_BGS_DHT22GetTemp(DHT22_Device_2) ;
			humid_value = ADCT_BGS_DHT22GetHumidity(DHT22_Device_2) ;
			if(temp_value < 5)
			{

			}
			else
			{
				pDHT22Data->usTemp_Main = temp_value;
			}

			if(! humid_value)
			{

			}
			else
			{
				pDHT22Data->usHumid_Main = humid_value ;
			}

		}
	}
#endif
}
void ADCT_BGS_DHT22_GetData(ADCT_BGS_DHT22_devicetypes device )
{
	ADCT_BGS_DHT22Start(device);
	Delay(1000000);
	while (ADCT_BGS_DHT22Measuring(device)){};

	if (ADCT_BGS_DHT22_CheckCRC(device))
	{
		ADCT_BGS_Temp[device] = ADCT_BGS_DHT22GetTemp(device) ;
		ADCT_BGS_Hummid[device] = ADCT_BGS_DHT22GetHumidity(device) ;
	}
}
/******************************************************************************
 * Global variables
 *****************************************************************************/

static  uint8_t g_DhtRaw[DHT22_NO_Device][DHT22_BUFFER_LENGTH];
static  uint8_t g_DhtMeasuring[DHT22_NO_Device]={0,0};

/******************************************************************************
 * Function declarations
 *****************************************************************************/
static void ADCT_BGS_DHT22_01_CbkState(ExtiLineState_t state);
#ifdef SUB_DHT22
static void ADCT_BGS_DHT22_02_CbkState(ExtiLineState_t state);
#endif
/******************************************************************************
 * Function definitions
 *****************************************************************************/
void ADCT_BGS_DHT22_PWR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT22_PWR_01_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(DHT22_PWR_02_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DHT22_PWR_01_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DHT22_PWR_01_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DHT22_PWR_02_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DHT22_PWR_02_PORT, &GPIO_InitStructure);
	DHT22_01_PWR_OFF();
	DHT22_02_PWR_OFF();
}

void ADCT_BGS_DHT22Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT22_01_CLK, ENABLE);
#ifdef SUB_DHT22
	RCC_APB2PeriphClockCmd(DHT22_02_CLK, ENABLE);
#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	vExtiAddCb(DHT22_01_PortSource, DHT22_01_PinSource, ADCT_BGS_DHT22_01_CbkState);
#ifdef SUB_DHT22
	vExtiAddCb(DHT22_02_PortSource, DHT22_02_PinSource, ADCT_BGS_DHT22_02_CbkState);
#endif
	GPIO_InitStructure.GPIO_Pin = DHT22_01_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DHT22_01_PORT, &GPIO_InitStructure);
#ifdef SUB_DHT22
	GPIO_InitStructure.GPIO_Pin = DHT22_02_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DHT22_02_PORT, &GPIO_InitStructure);
#endif
}

void ADCT_BGS_DHT22_01_CbkState(ExtiLineState_t state)
{
	static uint32_t timeUs=0;
	static uint8_t cnt=0;
	uint32_t t;

	t=TimerGetUS();

	if (g_DhtMeasuring[DHT22_Device_1])
	{
		if (state == LineState_Low) // last was low
		{
			if (cnt)
			{
				g_DhtRaw[DHT22_Device_1][ (cnt-1) / 8] <<= 1;
				if ((t - timeUs) > 50) // 1
					g_DhtRaw[DHT22_Device_1][(cnt-1) / 8] |= 1;
			}

			cnt++;

			if (cnt >= 41) {
				g_DhtMeasuring[DHT22_Device_1] = 0;
				cnt = 0;
			}
		}
	} else
		cnt = 0;

	timeUs=t;
}
#ifdef SUB_DHT22
void ADCT_BGS_DHT22_02_CbkState(ExtiLineState_t state)
{
	static uint32_t timeUs=0;
	static uint8_t cnt=0;
	uint32_t t;

	t=TimerGetUS();

	if (g_DhtMeasuring[DHT22_Device_2])
	{
		if (state == LineState_Low) // last was low
		{
			if (cnt)
			{
				g_DhtRaw[DHT22_Device_2][ (cnt-1) / 8] <<= 1;
				if ((t - timeUs) > 50) // 1
					g_DhtRaw[DHT22_Device_2][(cnt-1) / 8] |= 1;
			}

			cnt++;

			if (cnt >= 41) {
				g_DhtMeasuring[DHT22_Device_2] = 0;
				cnt = 0;
			}
		}
	} else
		cnt = 0;

	timeUs=t;
}
#endif
uint8_t ADCT_BGS_DHT22_CheckCRC(ADCT_BGS_DHT22_devicetypes device)
{
	uint8_t crc;
	crc=g_DhtRaw[device][0]+g_DhtRaw[device][1]+g_DhtRaw[device][2]+g_DhtRaw[device][3];
	return crc==g_DhtRaw[device][4];
}

void ADCT_BGS_DHT22Start(ADCT_BGS_DHT22_devicetypes device)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	switch (device)
	{

	case DHT22_Device_1:
	{

		GPIO_InitStructure.GPIO_Pin = DHT22_01_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(DHT22_01_PORT, &GPIO_InitStructure);

		DHT22_01_PIN_LOW();
		Delay(500);
		DHT22_01_PIN_HIGH();
		Delay(40);


		GPIO_InitStructure.GPIO_Pin = DHT22_01_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(DHT22_01_PORT, &GPIO_InitStructure);

		g_DhtMeasuring[DHT22_Device_1] = 1;
		memset(&g_DhtRaw[DHT22_Device_1], 0, 5);
		break;
	}
#ifdef SUB_DHT22
	case DHT22_Device_2:
	{
		GPIO_InitStructure.GPIO_Pin = DHT22_02_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(DHT22_02_PORT, &GPIO_InitStructure);

		DHT22_02_PIN_LOW();
		Delay(500);
		DHT22_02_PIN_HIGH();
		Delay(40);


		GPIO_InitStructure.GPIO_Pin = DHT22_02_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(DHT22_02_PORT, &GPIO_InitStructure);

		g_DhtMeasuring[DHT22_Device_2] = 1;
		memset(&g_DhtRaw[DHT22_Device_2], 0, 5);
	}
#endif
	default : break;
	}
}


uint8_t ADCT_BGS_DHT22Measuring(ADCT_BGS_DHT22_devicetypes device)
{
	return 	g_DhtMeasuring[device];
}

uint32_t ADCT_BGS_DHT22GetTemp(ADCT_BGS_DHT22_devicetypes device)
{
	uint32_t res=0;
	res=g_DhtRaw[device][2];
	res<<=8;
	res|=g_DhtRaw[device][3];
	return res;
}

uint32_t ADCT_BGS_DHT22GetHumidity(ADCT_BGS_DHT22_devicetypes device)
{
	uint32_t res=0;
	res=g_DhtRaw[device][0];
	res<<=8;
	res|=g_DhtRaw[device][1];
	return res;
}

