/*
 * ADCT_PhSensor.c
 *
 *  Created on: Aug 12, 2017
 *      Author: Administrator
 */


#include "ADCT_PhSensor.h"
volatile uint8_t idx_u8;
uint16_t SampleData_pu16[10];
uint8_t Data_avai = 0 ;
#define offset 0.8188
void ADCT_PhSensorInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/* Configure PB.00 (ADC Channel8) as analog input -------------------------*/
	RCC_APB2PeriphClockCmd(ADCT_PH_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADCT_PH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure clocks for ADC and GPIO PORT */
	RCC_APB2PeriphClockCmd(ADCT_PH_ADC_CLK, ENABLE);

	/* ADCx configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADC1 Regular Channel 8 Configuration */
	ADC_RegularChannelConfig(ADCT_PH_ADC_HW, ADCT_PH_ADC_HW_Channel, 1, ADC_SampleTime_1Cycles5);
	/* Enable ADC1 */
	ADC_Cmd(ADCT_PH_ADC_HW, ENABLE);

	/* Enable Vrefint channel enable temperature sensor for ADC module */
	ADC_TempSensorVrefintCmd(ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADCT_PH_ADC_HW);

	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADCT_PH_ADC_HW));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADCT_PH_ADC_HW);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADCT_PH_ADC_HW));
	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADCT_PH_ADC_HW, ENABLE);
}
unsigned long int avgValue;  //Store the average value of the sensor feedback
int temp;
double ADCT_Ph_getValue(void)
{
	for(int i=0;i<9;i++)        //sort the analog from small to large
	  {
	    for(int j=i+1;j<10;j++)
	    {
	      if(SampleData_pu16[i]>SampleData_pu16[j])
	      {
	        temp=SampleData_pu16[i];
	        SampleData_pu16[i]=SampleData_pu16[j];
	        SampleData_pu16[j]=temp;
	      }
	    }
	  }
	avgValue=0;
	  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
	    avgValue+=SampleData_pu16[i];
	  float phValue=(float)avgValue*5/4096/6; //convert the analog into millivolt
	  phValue=3.5*phValue + offset;
	  return phValue;//convert the millivolt into pH value
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
   // Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)(amount/(number-2));
  }//if
  return avg;
}

void ADCT_Ph_GetSample(uint8_t * available_ui8)
{
	if ( idx_u8 < 10 )
	{
		*available_ui8 = 0;
		SampleData_pu16[idx_u8] = ADC_GetConversionValue(ADCT_PH_ADC_HW);
		idx_u8 ++;
	}
	else
	{

		*available_ui8 =1;
		idx_u8 = 0;
	}
}

uint8_t  ADCT_Ph_GetStatus(void)
{
	return Data_avai;
}
