/*
 * ADCT_Sensor.h
 *
 *  Created on: Dec 26, 2016
 *      Author: Administrator
 */

#ifndef DRIVERS_SENSORS_INC_ADCT_SENSOR_H_
#define DRIVERS_SENSORS_INC_ADCT_SENSOR_H_

#include "stm32f10x.h"
#include "HardwareCfg.h"
#define ADCT_BGS_SMOKE_READ				GPIO_ReadInputDataBit(ADCT_BGS_INPUT_PORT, ADCT_BGS_INT_SMOKE)
#define ADCT_BGS_WATER_READ				GPIO_ReadInputDataBit(ADCT_BGS_INPUT_PORT, ADCT_BGS_INT_WATER)
#define ADCT_BGS_FIRE_READ				GPIO_ReadInputDataBit(ADCT_BGS_INPUT_PORT, ADCT_BGS_INT_FIRE)
#define ADCT_BGS_BURGLAR_READ			GPIO_ReadInputDataBit(ADCT_BGS_INPUT_PORT, ADCT_BGS_INT_BURGLAR)
#define ADCT_BGS_DOOR_READ				GPIO_ReadInputDataBit(ADCT_BGS_INPUT_PORT, ADCT_BGS_INT_DOOR)
#define ADCT_BGS_AC_READ				GPIO_ReadInputDataBit(ADCT_BGS_AC_GEN_INPUT_PORT, ADCT_BGS_INPUT_AC)
#define ADCT_BGS_GEN_READ				GPIO_ReadInputDataBit(ADCT_BGS_AC_GEN_INPUT_PORT, ADCT_BGS_INPUT_GEN)
//#define ADCT_BGS_BUZZER_READ			GPIO_ReadInputDataBit(ADCT_BGS_CTRL_RELAY_S_PORT, ADCT_BGS_CTRL_RELAY_S02)
//#define ADCT_BGS_LAMP_READ				GPIO_ReadInputDataBit(ADCT_BGS_CTRL_RELAY_S_PORT, ADCT_BGS_CTRL_RELAY_S01)
void ADCT_BGS_Sensor_Input_Init(void);
#endif /* DRIVERS_SENSORS_INC_ADCT_SENSOR_H_ */