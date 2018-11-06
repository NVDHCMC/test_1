/*
 * ADCT_AT24C64.h
 *
 *  Created on: Dec 25, 2016
 *      Author: Administrator
 */

#ifndef DRIVERS_AT24C64_INC_ADCT_AT24C64_H_
#define DRIVERS_AT24C64_INC_ADCT_AT24C64_H_

#include "stm32f10x.h"
#include "HardwareCfg.h"
#ifdef EEPROM_M24C64_32
/* For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device
  address selection (ne need for additional address lines). According to the
  Harware connection on the board (on STM3210C-EVAL board E0 = E1 = E2 = 0) */
#define I2C_SPEED               300000
#define EEPROM_HW_ADDRESS       0xA0   /* E0 = E1 = E2 = 0 */
#define I2C_SLAVE_ADDRESS7      0xA0
#define EEPROM_PAGESIZE         32
/* Uncomment this line to use the default start and end of critical section
   callbacks (it disables then enabled all interrupts) */
#define USE_DEFAULT_CRITICAL_CALLBACK
#endif /* EEPROM_M24C64_32 */
/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define EEPROM_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define EEPROM_LONG_TIMEOUT         ((uint32_t)(10 * EEPROM_FLAG_TIMEOUT))
#define USE_DEFAULT_TIMEOUT_CALLBACK
/* Maximum number of trials for EEPROM_WaitEepromStandbyState() function */
#define EEPROM_MAX_TRIALS_NUMBER     150

/* Defintions for the state of the DMA transfer */
#define EEPROM_STATE_READY           0
#define EEPROM_STATE_BUSY            1
#define EEPROM_STATE_ERROR           2

#define EEPROM_OK                    0
#define EEPROM_FAIL                  1


void     EEPROM_DeInit(void);
void     EEPROM_Init(void);
uint32_t EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t EEPROM_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void     EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t EEPROM_WaitEepromStandbyState(void);

/* USER Callbacks: These are functions for which prototypes only are declared in
   EEPROM driver and that should be implemented into user applicaiton. */
/* EEPROM_TIMEOUT_UserCallback() function is called whenever a timeout condition
   occure during communication (waiting on an event that doesn't occur, bus
   errors, busy devices ...).
   You can use the default timeout callback implementation by uncommenting the
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32_evel_i2c_ee.h file.
   Typically the user implementation of this callback should reset I2C peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t EEPROM_TIMEOUT_UserCallback(void);

/* Start and End of critical section: these callbacks should be typically used
   to disable interrupts when entering a critical section of I2C communication
   You may use default callbacks provided into this driver by uncommenting the
   define USE_DEFAULT_CRITICAL_CALLBACK in stm32_evel_i2c_ee.h file..
   Or you can comment that line and implement these callbacks into your
   application */
void EEPROM_EnterCriticalSection_UserCallback(void);
void EEPROM_ExitCriticalSection_UserCallback(void);

uint8_t ADCT_AT24C64_ReadOneByte(uint16_t Address_ui16 );
uint16_t ADCT_AT24C64_ReadTwoBytes(uint16_t Address_ui16);
void ADCT_AT24C64_WriteTwoBytes(uint16_t ui16MemAddr , uint16_t ui16data);
void ADCT_AT24C64_WriteOneBytes(uint16_t ui16MemAddr, uint8_t ui8data);
#endif /* DRIVERS_AT24C64_INC_ADCT_AT24C64_H_ */
