/*
 * ADCT_PHSensor.h
 *
 *  Created on: Aug 12, 2017
 *      Author: Administrator
 */

#ifndef ADCT_PHSENSOR_H_
#define ADCT_PHSENSOR_H_
#include "HardwareCfg.h"
extern uint16_t SampleData_pu16[10];
extern uint8_t Data_avai;
void ADCT_PhSensorInit(void);
void ADCT_Ph_GetSample(uint8_t * available_ui8);
double avergearray(int* arr, int number);
double ADCT_Ph_getValue(void);
uint8_t  ADCT_Ph_GetStatus(void);
#endif /* ADCT_PHSENSOR_H_ */
