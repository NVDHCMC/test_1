/*
 * ADCT_SIM900_LL.h
 *
 *  Created on: May 12, 2017
 *      Author: Administrator
 */

#ifndef INC_ADCT_SIM900_LL_H_
#define INC_ADCT_SIM900_LL_H_
#include "stm32f10x.h"
#include "PZEM004T.h"
#include "HardwareCfg.h"
#include "stdint.h"
#define PZEM_VOLTAGE (uint8_t)0xB0
#define RESP_VOLTAGE (uint8_t)0xA0

#define PZEM_CURRENT (uint8_t)0xB1
#define RESP_CURRENT (uint8_t)0xA1

#define PZEM_POWER   (uint8_t)0xB2
#define RESP_POWER   (uint8_t)0xA2

#define PZEM_ENERGY  (uint8_t)0xB3
#define RESP_ENERGY  (uint8_t)0xA3

#define PZEM_SET_ADDRESS (uint8_t)0xB4
#define RESP_SET_ADDRESS (uint8_t)0xA4

#define PZEM_POWER_ALARM (uint8_t)0xB5
#define RESP_POWER_ALARM (uint8_t)0xA5

#define RESPONSE_SIZE sizeof(PZEMCommand)
#define RESPONSE_DATA_SIZE RESPONSE_SIZE - 2

#define PZEM_BAUD_RATE 9600
#define PZEM_DEFAULT_READ_TIMEOUT 1000

#define PZEM_ERROR_VALUE -1.0

typedef struct  {
    uint8_t command;
    uint8_t addr[4];
    uint8_t data;
    uint8_t crc;
}PZEMCommand;
union {
	uint8_t bytes[4];  // IPv4 address
	uint32_t dword;
} _address;
typedef enum
{
	VOLTAGE = 0,
	CURRENT,
	ACTIVE_POWER,
	ENERGY,
	SET_ADDR,
	SET_POWER_ALARM_THRES,
	NUM_O_INFOR
}PZEMDataInfo;
typedef enum
{
	START = 0,
	PROCESS,
	FINISH
}PZEMState;
typedef struct
{
	PZEMState state;
	PZEMDataInfo index;
	uint8_t Header;
	uint8_t data[5];
	uint8_t crc;
	uint8_t req_flag;
}PZEMData;
typedef struct
{
	PZEMDataInfo func;
	uint8_t response;
	uint8_t *data;
	uint8_t gotframe;
}PZEM004_ReceiveData;
#define ADCT_PZEM004T_BUF_LEN			4096		///< PZEM004T buffer lengths
#define ADCT_PZEM004T_TERMINATOR 		'\n'			///< SIM900 frame terminator character
#define ADCT_PZEM004T_IrqEnable 		NVIC_EnableIRQ(ADCT_PZEM004T_USART_IRQ);
#define ADCT_PZEM004T_IrqDisable 		NVIC_DisableIRQ(ADCT_PZEM004T_USART_IRQ);
#define UART_PZEM004T_TransmitEnable() 	USART_ITConfig(ADCT_PZEM004T_USART_PORT, USART_IT_TXE, ENABLE);
float ADCT_PZEM004T_getVoltage(const uint8_t *addr);
float ADCT_PZEM004T_getCurrent(const uint8_t *addr);
float ADCT_PZEM004T_getPower(const uint8_t *addr);
float ADCT_PZEM004T_getEnergy(const uint8_t *addr);
uint8_t ADCT_PZEM004T_setAddress(const uint8_t *addr);
uint8_t ADCT_PZEM004T_setPowerAlarm(const uint8_t *addr, uint8_t threshold);
uint8_t ADCT_PZEM004T_receiveData(uint8_t resp, uint8_t *data);
void ADCT_PZEM0004T_sendData(const uint8_t *addr, uint8_t cmd, uint8_t data);
uint8_t ADCT_PZEM004T_crc(uint8_t *data, uint8_t sz);
void ADCT_PZEM004T_Init(uint32_t baud);
void UART_PZEM004T_Init(uint32_t baud, void(*rxCb)(uint8_t), uint8_t(*txCb)(uint8_t*) );
void ADCT_AC_GetData(void);
void ADCT_AC_Init(void);
void ADCT_AC_ConvertCurrent(float data, uint32_t *Sig, uint8_t *exp);
void ADCT_AC_Convert_Energy(float data, uint32_t *Sig, uint8_t * exp);
void ADCT_AC_Convert_Power(float data, uint32_t *Sig, uint8_t * exp);

#endif /* INC_ADCT_SIM900_LL_H_ */
