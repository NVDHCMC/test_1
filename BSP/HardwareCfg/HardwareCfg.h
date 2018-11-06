/*
 * HardwareCfg.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Administrator
 */

#ifndef HARDWARECFG_H_
#define HARDWARECFG_H_
#include "stm32f10x.h"

#define LED2_PIN                         GPIO_Pin_2
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOD

#define LED4_PIN                         GPIO_Pin_11
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_APB2Periph_GPIOC

#define RESERVE_1_PIN                     	  GPIO_Pin_13
#define RESERVE_1_GPIO_PORT                   GPIOC
#define RESERVE_1_GPIO_CLK                    RCC_APB2Periph_GPIOC

#define RESERVE_2_PIN                         GPIO_Pin_11
#define RESERVE_2_GPIO_PORT                   GPIOC
#define RESERVE_2_GPIO_CLK                    RCC_APB2Periph_GPIOC

#define RESERVE_3_PIN                         GPIO_Pin_8
#define RESERVE_3_GPIO_PORT                   GPIOB
#define RESERVE_3_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define RESERVE_4_PIN                         GPIO_Pin_8
#define RESERVE_4_GPIO_PORT                   GPIOA
#define RESERVE_4_GPIO_CLK                    RCC_APB2Periph_GPIOA
/**
 * @brief Enter -button
 */
#define ENTER_BUTTON_PIN                GPIO_Pin_0
#define ENTER_BUTTON_GPIO_PORT          GPIOB
#define ENTER_BUTTON_GPIO_CLK           RCC_APB2Periph_GPIOB
#define ENTER_BUTTON_EXTI_LINE          EXTI_Line0
#define ENTER_BUTTON_EXTI_PORT_SOURCE   GPIO_PortSourceGPIOB
#define ENTER_BUTTON_EXTI_PIN_SOURCE    GPIO_PinSource0
#define ENTER_BUTTON_EXTI_IRQn          EXTI0_IRQn


/**
 * @brief ESC -button
 */
#define ESC_BUTTON_PIN                GPIO_Pin_5
#define ESC_BUTTON_GPIO_PORT          GPIOC
#define ESC_BUTTON_GPIO_CLK           RCC_APB2Periph_GPIOC
#define ESC_BUTTON_EXTI_LINE          EXTI_Line5
#define ESC_BUTTON_EXTI_PORT_SOURCE   GPIO_PortSourceGPIOC
#define ESC_BUTTON_EXTI_PIN_SOURCE    GPIO_PinSource5
#define ESC_BUTTON_EXTI_IRQn          EXTI9_5_IRQn


/**
 * @brief UP -button
 */
#define UP_BUTTON_PIN                GPIO_Pin_2
#define UP_BUTTON_GPIO_PORT          GPIOB
#define UP_BUTTON_GPIO_CLK           RCC_APB2Periph_GPIOB
#define UP_BUTTON_EXTI_LINE          EXTI_Line2
#define UP_BUTTON_EXTI_PORT_SOURCE   GPIO_PortSourceGPIOB
#define UP_BUTTON_EXTI_PIN_SOURCE    GPIO_PinSource2
#define UP_BUTTON_EXTI_IRQn          EXTI2_IRQn

/**
 * @brief DOWN -button
 */
#define DOWN_BUTTON_PIN                GPIO_Pin_1
#define DOWN_BUTTON_GPIO_PORT          GPIOB
#define DOWN_BUTTON_GPIO_CLK           RCC_APB2Periph_GPIOB
#define DOWN_BUTTON_EXTI_LINE          EXTI_Line1
#define DOWN_BUTTON_EXTI_PORT_SOURCE   GPIO_PortSourceGPIOB
#define DOWN_BUTTON_EXTI_PIN_SOURCE    GPIO_PinSource1
#define DOWN_BUTTON_EXTI_IRQn          EXTI1_IRQn

/**
 * @brief  I2C EEPROM Interface pins
 */
#define EEPROM_I2C                          I2C1
#define EEPROM_I2C_CLK                      RCC_APB1Periph_I2C1
#define EEPROM_I2C_SCL_PIN                  GPIO_Pin_6                 /* PB.6 */
#define EEPROM_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define EEPROM_I2C_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
#define EEPROM_I2C_SDA_PIN                  GPIO_Pin_7                  /* PB.7 */
#define EEPROM_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define EEPROM_I2C_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB
#define EEPROM_M24C64_32

#define EEPROM_I2C_DMA                      DMA1
#define EEPROM_I2C_DMA_CHANNEL_TX           DMA1_Channel6
#define EEPROM_I2C_DMA_CHANNEL_RX           DMA1_Channel7
#define EEPROM_I2C_DMA_FLAG_TX_TC           DMA1_IT_TC6
#define EEPROM_I2C_DMA_FLAG_TX_GL           DMA1_IT_GL6
#define EEPROM_I2C_DMA_FLAG_RX_TC           DMA1_IT_TC7
#define EEPROM_I2C_DMA_FLAG_RX_GL           DMA1_IT_GL7
#define EEPROM_I2C_DMA_CLK                  RCC_AHBPeriph_DMA1
#define EEPROM_I2C_DR_Address               ((uint32_t)0x40005410   )
#define EEPROM_USE_DMA

#define EEPROM_I2C_DMA_TX_IRQn              DMA1_Channel6_IRQn
#define EEPROM_I2C_DMA_RX_IRQn              DMA1_Channel7_IRQn
#define EEPROM_I2C_DMA_TX_IRQHandler        DMA1_Channel6_IRQHandler
#define EEPROM_I2C_DMA_RX_IRQHandler        DMA1_Channel7_IRQHandler
#define EEPROM_I2C_DMA_PREPRIO              3
#define EEPROM_I2C_DMA_SUBPRIO              3

#define EEPROM_DIRECTION_TX                 0
#define EEPROM_DIRECTION_RX                 1

/* Time constant for the delay caclulation allowing to have a millisecond
   incrementing counter. This value should be equal to (System Clock / 1000).
   ie. if system clock = 72MHz then EEPROM_TIME_CONST should be 24. */
#define EEPROM_TIME_CONST          72

/**********************************************************
 *  Define pin for sensor
 **********************************************************/
#define ADCT_BGS_INT_SMOKE			GPIO_Pin_0
#define ADCT_BGS_INT_DOOR			GPIO_Pin_1
#define ADCT_BGS_INT_BURGLAR		GPIO_Pin_2
#define ADCT_BGS_INT_FIRE			GPIO_Pin_3
#define ADCT_BGS_INT_WATER			GPIO_Pin_4
#define ADCT_BGS_INPUT_CLK 			RCC_APB2Periph_GPIOC
#define ADCT_BGS_INPUT_PORT 		GPIOC

#define ADCT_BGS_INPUT_AC			GPIO_Pin_14
#define ADCT_BGS_INPUT_GEN			GPIO_Pin_13
#define ADCT_BGS_AC_GEN_INPUT_CLK 	RCC_APB2Periph_GPIOB
#define ADCT_BGS_AC_GEN_INPUT_PORT 	GPIOB

/**********************************************************
 *  Define pin for Relay
 **********************************************************/
#define ADCT_BGS_CTRL_RELAY_S01   		GPIO_Pin_0
#define ADCT_BGS_CTRL_RELAY_S01_CLK		RCC_APB2Periph_GPIOA
#define ADCT_BGS_CTRL_RELAY_S01_PORT	GPIOA

#define ADCT_BGS_CTRL_RELAY_S02			GPIO_Pin_1
#define ADCT_BGS_CTRL_RELAY_S02_CLK		RCC_APB2Periph_GPIOA
#define ADCT_BGS_CTRL_RELAY_S02_PORT	GPIOA

#define ADCT_BGS_CTRL_RELAY_S03			GPIO_Pin_2
#define ADCT_BGS_CTRL_RELAY_S03_CLK		RCC_APB2Periph_GPIOA
#define ADCT_BGS_CTRL_RELAY_S03_PORT	GPIOA

#define ADCT_BGS_CTRL_RELAY_S04   		GPIO_Pin_3
#define ADCT_BGS_CTRL_RELAY_S04_CLK		RCC_APB2Periph_GPIOA
#define ADCT_BGS_CTRL_RELAY_S04_PORT	GPIOA

#define ADCT_BGS_CTRL_RELAY_S05			GPIO_Pin_6
#define ADCT_BGS_CTRL_RELAY_S05_CLK		RCC_APB2Periph_GPIOC
#define ADCT_BGS_CTRL_RELAY_S05_PORT	GPIOC


#define ADCT_BGS_CTRL_RELAY_S06			GPIO_Pin_7
#define ADCT_BGS_CTRL_RELAY_S06_CLK		RCC_APB2Periph_GPIOC
#define ADCT_BGS_CTRL_RELAY_S06_PORT	GPIOC

#define ADCT_BGS_CTRL_RELAY_S07			GPIO_Pin_8
#define ADCT_BGS_CTRL_RELAY_S07_CLK		RCC_APB2Periph_GPIOC
#define ADCT_BGS_CTRL_RELAY_S07_PORT	GPIOC

#define ADCT_BGS_CTRL_RELAY_S08			GPIO_Pin_9
#define ADCT_BGS_CTRL_RELAY_S08_CLK		RCC_APB2Periph_GPIOC
#define ADCT_BGS_CTRL_RELAY_S08_PORT	GPIOC

#define ADCT_BGS_CTRL_RELAY_B09			GPIO_Pin_10
#define ADCT_BGS_CTRL_RELAY_B09_CLK		RCC_APB2Periph_GPIOC
#define ADCT_BGS_CTRL_RELAY_B09_PORT	GPIOC

#define ADCT_BGS_CTRL_RELAY_B10			GPIO_Pin_5
#define ADCT_BGS_CTRL_RELAY_B10_CLK		RCC_APB2Periph_GPIOB
#define ADCT_BGS_CTRL_RELAY_B10_PORT	GPIOB
/* Define Pin for Usart debug */
#define ADCT_COMPORT_TX_PIN			GPIO_Pin_9
#define ADCT_COMPORT_RX_PIN			GPIO_Pin_10
#define ADCT_COMPORT_TX_PORT		GPIOA
#define ADCT_COMPORT_RX_PORT		GPIOA
#define ADCT_COMPORT_TX_CLK			RCC_APB2Periph_GPIOA
#define ADCT_COMPORT_RX_CLK			RCC_APB2Periph_GPIOA
#define ADCT_COMPORT_USART_PORT		USART1
#define ADCT_COMPORT_USART_CLK		RCC_APB2Periph_USART1
#define ADCT_COMPORT_USART_IRQ    	USART1_IRQn

#define TFT_RESET_PIN   		GPIO_Pin_4
#define TFT_RESET_GPIO_PORT		GPIOB
#define TFT_RESET_GPIO_CLK		RCC_APB2Periph_GPIOB

#define TFT_DC_PIN      		GPIO_Pin_3
#define TFT_DC_GPIO_PORT		GPIOB
#define TFT_DC_GPIO_CLK			RCC_APB2Periph_GPIOB

#define TFT_CS_PIN      		GPIO_Pin_4
#define TFT_CS_GPIO_PORT		GPIOA
#define TFT_CS_GPIO_CLK			RCC_APB2Periph_GPIOA

#define TFT_SPI_MASTER                   SPI1
#define TFT_SPI_MASTER_CLK               RCC_APB2Periph_SPI1
#define TFT_SPI_MASTER_GPIO              GPIOA
#define TFT_SPI_MASTER_GPIO_CLK          RCC_APB2Periph_GPIOA
#define TFT_SPI_MASTER_PIN_NSS           GPIO_Pin_4
#define TFT_SPI_MASTER_PIN_SCK           GPIO_Pin_5
#define TFT_SPI_MASTER_PIN_MISO          GPIO_Pin_6
#define TFT_SPI_MASTER_PIN_MOSI          GPIO_Pin_7
/**********************************************************
 *  Define pin for Modbus
 **********************************************************/
#define ADCT_BGS_RS485_TX_PIN		GPIO_Pin_10
#define ADCT_BGS_RS485_RX_PIN		GPIO_Pin_11
#define ADCT_BGS_RS485_PORT			GPIOB
#define ADCT_BGS_RS485_CLK			RCC_APB2Periph_GPIOB
#define ADCT_BGS_RS485_USART_PORT	USART3
#define ADCT_BGS_RS485_USART_CLK	RCC_APB1Periph_USART3
#define ADCT_BGS_RS485_USART_IRQ    USART3_IRQn

#define ADCT_BGS_RS485_DE_RE_PIN	GPIO_Pin_12
#define ADCT_BGS_RS485_DE_RE_CLK	RCC_APB2Periph_GPIOB
#define ADCT_BGS_RS485_DE_RE_PORT	GPIOB
/**********************************************************
 *  Define pin for dht22
 **********************************************************/

#define DHT22_01_PORT		GPIOB
#define DHT22_01_PIN		GPIO_Pin_9
#define DHT22_01_CLK		RCC_APB2Periph_GPIOB
#define DHT22_01_PortSource	GPIO_PortSourceGPIOB
#define DHT22_01_PinSource	GPIO_PinSource9

#define DHT22_02_PORT		GPIOC
#define DHT22_02_PIN		GPIO_Pin_12
#define DHT22_02_CLK		RCC_APB2Periph_GPIOC
#define DHT22_02_PortSource	GPIO_PortSourceGPIOC
#define DHT22_02_PinSource	GPIO_PinSource12

/*
 * Define Pin for power supply dht22
 * */
#define DHT22_PWR_01_PORT		GPIOA
#define DHT22_PWR_01_PIN		GPIO_Pin_8
#define DHT22_PWR_01_CLK		RCC_APB2Periph_GPIOA
#define DHT22_PWR_01_PortSource	GPIO_PortSourceGPIOA
#define DHT22_PWR_01_PinSource	GPIO_PinSource8

/*
 * Define Pin for power supply dht22
 * */
#define DHT22_PWR_02_PORT		GPIOB
#define DHT22_PWR_02_PIN		GPIO_Pin_8
#define DHT22_PWR_02_CLK		RCC_APB2Periph_GPIOB
#define DHT22_PWR_02_PortSource	GPIO_PortSourceGPIOB
#define DHT22_PWR_02_PinSource	GPIO_PinSource8
/**********************************************************
 *  Define pin for PZEM004T
 **********************************************************/
#define ADCT_PZEM004T_TX_PIN		GPIO_Pin_9
#define ADCT_PZEM004T_RX_PIN		GPIO_Pin_10
#define ADCT_PZEM004T_TX_PORT		GPIOA
#define ADCT_PZEM004T_RX_PORT		GPIOA
#define ADCT_PZEM004T_TX_CLK		RCC_APB2Periph_GPIOA
#define ADCT_PZEM004T_RX_CLK		RCC_APB2Periph_GPIOA
#define ADCT_PZEM004T_USART_PORT	USART1
#define ADCT_PZEM004T_USART_CLK		RCC_APB2Periph_USART1
#define ADCT_PZEM004T_USART_IRQ    	USART1_IRQn


/************************************************************
 * Define pin for PhSensor
 ************************************************************/

#define ADCT_PH_PIN		GPIO_Pin_0
#define ADCT_PH_PORT 	GPIOB
#define ADCT_PH_GPIO_CLK	RCC_APB2Periph_GPIOB

#define ADCT_PH_ADC_CLK RCC_APB2Periph_ADC1
#define ADCT_PH_ADC_HW ADC1
#define ADCT_PH_ADC_HW_Channel ADC_Channel_8

/******************************************************************************
 *
 * EEPROM addresses
 *
 ******************************************************************************/

#define EEPROM_LOW_TEMPERATURE_T1_LOW			(uint16_t)(1 )
#define EEPROM_LOW_TEMPERATURE_T1_HIGH			(uint16_t)(2 )
#define EEPROM_HIGH_TEMPERATURE_T2_LOW			(uint16_t)(3 )
#define EEPROM_HIGH_TEMPERATURE_T2_HIGH			(uint16_t)(4 )
#define EEPROM_FAN_TEMPERATURE_T3_LOW			(uint16_t)(5 )
#define EEPROM_FAN_TEMPERATURE_T3_HIGH			(uint16_t)(6 )
#define EEPROM_LOW_HUMIDITY_H1_LOW				(uint16_t)(7 )
#define EEPROM_LOW_HUMIDITY_H1_HIGH				(uint16_t)(8 )
#define EEPROM_HIGH_HUMIDITY_H2_LOW				(uint16_t)(9 )
#define EEPROM_HIGH_HUMIDITY_H2_HIGH			(uint16_t)(10)
#define EPPROM_SWITCHING_HOUR					(uint16_t)(11)
#define EPPROM_SWITCHING_MINUTE					(uint16_t)(12)
#define EPPROM_DOORBURGLAR_ENABLEALARM			(uint16_t)(13)
#define EPPROM_ENABLE_ALARM						(uint16_t)(14)
#define EPPROM_ALARMDURATION_LOW				(uint16_t)(15)
#define EPPROM_ALARMDURATION_HIGH				(uint16_t)(16)
#define EPPROM_GENBATTERYVOLTAGEALARMVALUE_LOW	(uint16_t)(17)
#define EPPROM_GENBATTERYVOLTAGEALARMVALUE_HIGH	(uint16_t)(18)
#define EPPROM_GENCOOLANTTEMPALARMVALUE_LOW		(uint16_t)(19)
#define EPPROM_GENCOOLANTTEMPALARMVALUE_HIGH	(uint16_t)(20)
#define EPPROM_GENENGINESPEEDALARMVALUE_LOW		(uint16_t)(21)
#define EPPROM_GENENGINESPEEDALARMVALUE_HIGH	(uint16_t)(22)
#define EPPROM_GENFUELLOWVALUE_LOW				(uint16_t)(23)
#define EPPROM_GENFUELLOWVALUE_HIGH				(uint16_t)(24)
#define EPPROM_GENOILPRESSUREALARMVALUE_LOW		(uint16_t)(25)
#define EPPROM_GENOILPRESSUREALARMVALUE_HIGH	(uint16_t)(26)
#define EPPROM_GENVOLTAGEALARMVALUE_LOW			(uint16_t)(27)
#define EPPROM_GENVOLTAGEALARMVALUE_HIGH		(uint16_t)(28)
#define EPPROM_LOWMAINVOLTAGE_LOW				(uint16_t)(29)
#define EPPROM_LOWMAINVOLTAGE_HIGH				(uint16_t)(30)
#define EPPROM_MINSWITCHINGTIME_HOUR			(uint16_t)(31)
#define EPPROM_MINSWITCHINGTIME_MINUTE			(uint16_t)(32)
#define EPPROM_HIGHMAINVOLTAGE_LOW				(uint16_t)(33)
#define EPPROM_HIGHMAINVOLTAGE_HIGH				(uint16_t)(34)
#define EPPROM_STAYOFFTIME_HOUR					(uint16_t)(35)
#define EPPROM_STAYOFFTIME_MINUTE				(uint16_t)(36)
#define EEPROM_DEFAULT_DATE						(uint16_t)(37)
#define EEPROM_DEFAULT_MONTH					(uint16_t)(38)
#define EEPROM_DEFAULT_YEAR						(uint16_t)(39)
#define EEPROM_DEFAULT_HOUR						(uint16_t)(40)
#define EEPROM_DEFAULT_MINUTE					(uint16_t)(41)
#define EEPROM_DEFAULT_SECOND					(uint16_t)(42)
#define EEPROM_ALARM_ID_LOW						(uint16_t)(43)
#define EEPROM_ALARM_ID_HIGH					(uint16_t)(44)
// Default value when setup and cancel buttons were pressed on power on.
#define DEFAULT_TIME_MIN                   5
#define DEFAULT_TIME_DELOFF                3
#define DEFAULT_TIME_CHANGE_HOUR           1
#define DEFAULT_TIME_CHANGE_MINUTE         0
#define DEFAULT_BGS_DATE                   9
#define DEFAULT_BGS_MONTH					7
#define DEFAULT_BGS_YEAR					17
#define DEFAULT_BGS_HOUR					10
#define DEFAULT_BGS_MINUTE					30
#define DEFAULT_BGS_SECOND					0
#define DEFAULT_ALARM_ID					8

#define DEFAULT_LOWTEMPERATURET1              160
#define DEFAULT_HIGHTEMPERATURET2             280
#define DEFAULT_FANTEMPERATURET3              330
#define DEFAULT_LOWHUMIDITYH1                 400
#define DEFAULT_HIGHHUMIDITYH2                900
#define DEFAULT_LOWMAINVOLTAGE                190
#define DEFAULT_HIGHMAINVOLTAGE               240
#define DEFAULT_GENFUELLOWVALUE               50
#define DEFAULT_GENVOLTAGEALARMVALUE          200
#define DEFAULT_GENBATTERYVOLTAGEALARMVALUE   12
#define DEFAULT_GENCOOLANTTEMPALARMVALUE      98
#define DEFAULT_GENENGINESPEEDALARMVALUE      2900
#define DEFAULT_GENOILPRESSUREALARMVALUE      0
#define DEFAULT_SWITCHINGTIME_HOUR            0
#define DEFAULT_SWITCHINGTIME_MINUTE          2
#define DEFAULT_MINSWITCHINGTIME_HOUR         0
#define DEFAULT_MINSWITCHINGTIME_MINUTE       4

#define DEFAULT_STAYOFFTIME_HOUR              0
#define DEFAULT_STAYOFFTIME_MINUTE            5
#define DEFAULT_ENABLEALARM                   1
#define DEFAULT_DOORBURGLARENABLEALARM        1
#define DEFAULT_ALARMDURATION                 1

#define DEFAULT_HOUR                       12
#define DEFAULT_MINUTE                     30
#define DEFAULT_SECOND                     0
#define DEFAULT_ALARM_ENABLE               1
#define DEFAULT_ALARM_TIME_HOUR            0
#define DEFAULT_ALARM_TIME_MINUTE          2
#define DEFAULT_ALARM_ENABLE_DOOR_BURGLAR  0
#endif /* HARDWARECFG_H_ */
