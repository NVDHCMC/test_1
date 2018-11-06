/**************************************************************************//**
 * @file     dht22.h
 * @brief    DHT22 sensor func definitions header file
 * @version  v1.0
 * @date     26.06.2012
 *
 * @note
 * Copyright (C) 2012 kab
 *
 ******************************************************************************/
#ifndef __DHT22_H__
#define __DHT22_H__

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <inttypes.h>
#include "ADCT_BGS_DataTypes.h"
#include "HardwareCfg.h"
/******************************************************************************
 * Defines and Macros
 *****************************************************************************/

/******************************************************************************
 * Types declarations
 *****************************************************************************/

/******************************************************************************
 * Function declarations
 *****************************************************************************/

#define DHT22_01_PIN_LOW()	GPIO_ResetBits(DHT22_01_PORT, DHT22_01_PIN)
#define DHT22_01_PIN_HIGH()	GPIO_SetBits(DHT22_01_PORT, DHT22_01_PIN)


#define DHT22_02_PIN_HIGH()	GPIO_SetBits(DHT22_02_PORT, DHT22_02_PIN)
#define DHT22_02_PIN_LOW()	GPIO_ResetBits(DHT22_02_PORT, DHT22_02_PIN)

#define DHT22_01_PWR_ON()		GPIO_SetBits(DHT22_PWR_01_PORT, DHT22_PWR_01_PIN)
#define DHT22_01_PWR_OFF()		GPIO_ResetBits(DHT22_PWR_01_PORT, DHT22_PWR_01_PIN)

#define DHT22_02_PWR_ON()		GPIO_SetBits(DHT22_PWR_02_PORT, DHT22_PWR_02_PIN)
#define DHT22_02_PWR_OFF()		GPIO_ResetBits(DHT22_PWR_02_PORT, DHT22_PWR_02_PIN)

#define SUB_DHT22
#define DHT22_BUFFER_LENGTH 5
typedef enum
{
	DHT22_Device_1 ,
	DHT22_Device_2,
	DHT22_NO_Device
}ADCT_BGS_DHT22_devicetypes;

void vDhtGetData(void);
void ADCT_BGS_DHT22_PWR_Init(void);
void ADCT_BGS_DHT22Init(void);
uint8_t ADCT_BGS_DHT22Measuring(ADCT_BGS_DHT22_devicetypes device);
uint32_t ADCT_BGS_DHT22GetHumidity(ADCT_BGS_DHT22_devicetypes device);
uint32_t ADCT_BGS_DHT22GetTemp(ADCT_BGS_DHT22_devicetypes device);
void ADCT_BGS_DHT22Start(ADCT_BGS_DHT22_devicetypes device);
uint8_t ADCT_BGS_DHT22_CheckCRC(ADCT_BGS_DHT22_devicetypes device);
void ADCT_BGS_DHT22_GetData(ADCT_BGS_DHT22_devicetypes device );
void DHT22_Module_Init_Hardware(void);
void ADCT_DHT22_FPT_GetValue(DHT22_MODULE_DATA *pDHT22Data);
#endif
