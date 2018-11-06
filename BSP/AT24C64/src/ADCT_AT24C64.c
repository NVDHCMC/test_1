/*
 * ADCT_AT24c64.c
 *
 *  Created on: Dec 25, 2016
 *      Author: Administrator
 */

#include "ADCT_AT24C64.h"

DMA_InitTypeDef   EEPROMDMA_InitStructure;
__IO uint16_t  EEPROMAddress = 0;
__IO uint32_t  EEPROMTimeout = EEPROM_LONG_TIMEOUT;
__IO uint16_t* EEPROMDataReadPointer;
__IO uint8_t*  EEPROMDataWritePointer;
__IO uint8_t   EEPROMDataNum;
/* Private typedef -----------------------------------------------------------*/

/**
 * @brief  Initializes peripherals used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
void EEPROM_LowLevel_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*!< EEPROM_I2C_SCL_GPIO_CLK and EEPROM_I2C_SDA_GPIO_CLK Periph clock enable */
	RCC_APB2PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK, ENABLE);

	/*!< EEPROM_I2C Periph clock enable */
	RCC_APB1PeriphClockCmd(EEPROM_I2C_CLK, ENABLE);

	/*!< GPIO configuration */
	/*!< Configure EEPROM_I2C pins: SCL */
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/*!< Configure EEPROM_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* Configure and enable I2C DMA TX Channel interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EEPROM_I2C_DMA_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EEPROM_I2C_DMA_PREPRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = EEPROM_I2C_DMA_SUBPRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure and enable I2C DMA RX Channel interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EEPROM_I2C_DMA_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EEPROM_I2C_DMA_PREPRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = EEPROM_I2C_DMA_SUBPRIO;
	NVIC_Init(&NVIC_InitStructure);

	/*!< I2C DMA TX and RX channels configuration */
	/* Enable the DMA clock */
	RCC_AHBPeriphClockCmd(EEPROM_I2C_DMA_CLK, ENABLE);

	/* I2C TX DMA Channel configuration */
	DMA_DeInit(EEPROM_I2C_DMA_CHANNEL_TX);
	EEPROMDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)EEPROM_I2C_DR_Address;
	EEPROMDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;   /* This parameter will be configured durig communication */
	EEPROMDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
	EEPROMDMA_InitStructure.DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
	EEPROMDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	EEPROMDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	EEPROMDMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	EEPROMDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	EEPROMDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	EEPROMDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	EEPROMDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(EEPROM_I2C_DMA_CHANNEL_TX, &EEPROMDMA_InitStructure);

	/* I2C RX DMA Channel configuration */
	DMA_DeInit(EEPROM_I2C_DMA_CHANNEL_RX);
	DMA_Init(EEPROM_I2C_DMA_CHANNEL_RX, &EEPROMDMA_InitStructure);

	/* Enable the DMA Channels Interrupts */
	DMA_ITConfig(EEPROM_I2C_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);
	DMA_ITConfig(EEPROM_I2C_DMA_CHANNEL_RX, DMA_IT_TC, ENABLE);
}
/**
 * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
void EEPROM_LowLevel_DeInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* EEPROM_I2C Peripheral Disable */
	I2C_Cmd(EEPROM_I2C, DISABLE);

	/* EEPROM_I2C DeInit */
	I2C_DeInit(EEPROM_I2C);

	/*!< EEPROM_I2C Periph clock disable */
	RCC_APB1PeriphClockCmd(EEPROM_I2C_CLK, DISABLE);

	/*!< GPIO configuration */
	/*!< Configure EEPROM_I2C pins: SCL */
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/*!< Configure EEPROM_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* Configure and enable I2C DMA TX Channel interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EEPROM_I2C_DMA_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EEPROM_I2C_DMA_PREPRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = EEPROM_I2C_DMA_SUBPRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure and enable I2C DMA RX Channel interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EEPROM_I2C_DMA_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EEPROM_I2C_DMA_PREPRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = EEPROM_I2C_DMA_SUBPRIO;
	NVIC_Init(&NVIC_InitStructure);

	/* Disable and Deinitialize the DMA channels */
	DMA_Cmd(EEPROM_I2C_DMA_CHANNEL_TX, DISABLE);
	DMA_Cmd(EEPROM_I2C_DMA_CHANNEL_RX, DISABLE);
	DMA_DeInit(EEPROM_I2C_DMA_CHANNEL_TX);
	DMA_DeInit(EEPROM_I2C_DMA_CHANNEL_RX);
}

/**
 * @brief  Initializes DMA channel used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
void EEPROM_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{
	/* Initialize the DMA with the new parameters */
	if (Direction == EEPROM_DIRECTION_TX)
	{
		/* Configure the DMA Tx Channel with the buffer address and the buffer size */
		EEPROMDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
		EEPROMDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		EEPROMDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;
		DMA_Init(EEPROM_I2C_DMA_CHANNEL_TX, &EEPROMDMA_InitStructure);
	}
	else
	{
		/* Configure the DMA Rx Channel with the buffer address and the buffer size */
		EEPROMDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
		EEPROMDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		EEPROMDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;
		DMA_Init(EEPROM_I2C_DMA_CHANNEL_RX, &EEPROMDMA_InitStructure);
	}
}

/**
 * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
void EEPROM_DeInit(void)
{
	EEPROM_LowLevel_DeInit();
}

/**
 * @brief  Initializes peripherals used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
void EEPROM_Init(void)
{
	I2C_InitTypeDef  I2C_InitStructure;

	EEPROM_LowLevel_Init();

	/*!< I2C configuration */
	/* EEPROM_I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

	/* EEPROM_I2C Peripheral Enable */
	I2C_Cmd(EEPROM_I2C, ENABLE);
	/* Apply EEPROM_I2C configuration after enabling it */
	I2C_Init(EEPROM_I2C, &I2C_InitStructure);

	/* Enable the EEPROM_I2C peripheral DMA requests */
	I2C_DMACmd(EEPROM_I2C, ENABLE);

#if defined (EEPROM_M24C64_32)
	/*!< Select the EEPROM address according to the state of E0, E1, E2 pins */
	EEPROMAddress = EEPROM_HW_ADDRESS;
#endif /*!< EEPROM_M24C64_32 */
}

/**
 * @brief  Reads a block of data from the EEPROM.
 * @param  pBuffer : pointer to the buffer that receives the data read from
 *         the EEPROM.
 * @param  ReadAddr : EEPROM's internal address to start reading from.
 * @param  NumByteToRead : pointer to the variable holding number of bytes to
 *         be read from the EEPROM.
 *
 *        @note The variable pointed by NumByteToRead is reset to 0 when all the
 *              data are read from the EEPROM. Application should monitor this
 *              variable in order know when the transfer is complete.
 *
 * @note When number of data to be read is higher than 1, this function just
 *       configures the communication and enable the DMA channel to transfer data.
 *       Meanwhile, the user application may perform other tasks.
 *       When number of data to be read is 1, then the DMA is not used. The byte
 *       is read in polling mode.
 *
 * @retval EEPROM_OK (0) if operation is correctly performed, else return value
 *         different from EEPROM_OK (0) or the timeout user callback.
 */
uint32_t EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{
	/* Set the pointer to the Number of data to be read. This pointer will be used
      by the DMA Transfer Completer interrupt Handler in order to reset the
      variable to 0. User should check on this variable in order to know if the
      DMA transfer has been complete or not. */
	EEPROMDataReadPointer = NumByteToRead;

	/*!< While the bus is busy */
	EEPROMTimeout = EEPROM_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send START condition */
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(EEPROM_I2C, EEPROMAddress, I2C_Direction_Transmitter);

	/*!< Test on EV6 and clear it */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

#ifdef EEPROM_M24C08

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(EEPROM_I2C, ReadAddr);

#elif defined (EEPROM_M24C64_32)

	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(EEPROM_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

	/*!< Test on EV8 and clear it */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send the EEPROM's internal address to read from: LSB of the address */
	I2C_SendData(EEPROM_I2C, (uint8_t)(ReadAddr & 0x00FF));

#endif /*!< EEPROM_M24C08 */

	/*!< Test on EV8 and clear it */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BTF) == RESET)
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send STRAT condition a second time */
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(EEPROM_I2C, EEPROMAddress, I2C_Direction_Receiver);

	/* If number of data to be read is 1, then DMA couldn't be used */
	/* One Byte Master Reception procedure (POLLING) ---------------------------*/
	if ((uint16_t)(*NumByteToRead) < 2)
	{
		/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
		EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR) == RESET)
		{
			if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
		}

		/*!< Disable Acknowledgement */
		I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);

		/* Call User callback for critical section start (should typically disable interrupts) */
		EEPROM_EnterCriticalSection_UserCallback();

		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void)EEPROM_I2C->SR2;

		/*!< Send STOP Condition */
		I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

		/* Call User callback for critical section end (should typically re-enable interrupts) */
		EEPROM_ExitCriticalSection_UserCallback();

		/* Wait for the byte to be received */
		EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_RXNE) == RESET)
		{
			if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
		}

		/*!< Read the byte received from the EEPROM */
		*pBuffer = I2C_ReceiveData(EEPROM_I2C);

		/*!< Decrement the read bytes counter */
		(uint16_t)(*NumByteToRead)--;

		/* Wait to make sure that STOP control bit has been cleared */
		EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
		while(EEPROM_I2C->CR1 & I2C_CR1_STOP)
		{
			if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
		}

		/*!< Re-Enable Acknowledgement to be ready for another reception */
		I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
	}
	else/* More than one Byte Master Reception procedure (DMA) -----------------*/
	{
		/*!< Test on EV6 and clear it */
		EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
		}

		/* Configure the DMA Rx Channel with the buffer address and the buffer size */
		EEPROM_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), EEPROM_DIRECTION_RX);

		/* Inform the DMA that the next End Of Transfer Signal will be the last one */
		I2C_DMALastTransferCmd(EEPROM_I2C, ENABLE);

		/* Enable the DMA Rx Channel */
		DMA_Cmd(EEPROM_I2C_DMA_CHANNEL_RX, ENABLE);
	}

	/* If all operations OK, return EEPROM_OK (0) */
	return EEPROM_OK;
}
/**
 * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
 *
 * @note   The number of bytes (combined to write start address) must not
 *         cross the EEPROM page boundary. This function can only write into
 *         the boundaries of an EEPROM page.
 *         This function doesn't check on boundaries condition (in this driver
 *         the function EEPROM_WriteBuffer() which calls EEPROM_WritePage() is
 *         responsible of checking on Page boundaries).
 *
 * @param  pBuffer : pointer to the buffer containing the data to be written to
 *         the EEPROM.
 * @param  WriteAddr : EEPROM's internal address to write to.
 * @param  NumByteToWrite : pointer to the variable holding number of bytes to
 *         be written into the EEPROM.
 *
 *        @note The variable pointed by NumByteToWrite is reset to 0 when all the
 *              data are written to the EEPROM. Application should monitor this
 *              variable in order know when the transfer is complete.
 *
 * @note This function just configure the communication and enable the DMA
 *       channel to transfer data. Meanwhile, the user application may perform
 *       other tasks in parallel.
 *
 * @retval EEPROM_OK (0) if operation is correctly performed, else return value
 *         different from EEPROM_OK (0) or the timeout user callback.
 */
uint32_t EEPROM_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{
	/* Set the pointer to the Number of data to be written. This pointer will be used
      by the DMA Transfer Completer interrupt Handler in order to reset the
      variable to 0. User should check on this variable in order to know if the
      DMA transfer has been complete or not. */
	EEPROMDataWritePointer = NumByteToWrite;

	/*!< While the bus is busy */
	EEPROMTimeout = EEPROM_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send START condition */
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);

	/*!< Test on EV5 and clear it */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send EEPROM address for write */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	I2C_Send7bitAddress(EEPROM_I2C, EEPROMAddress, I2C_Direction_Transmitter);

	/*!< Test on EV6 and clear it */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

#ifdef EEPROM_M24C08

	/*!< Send the EEPROM's internal address to write to : only one byte Address */
	I2C_SendData(EEPROM_I2C, WriteAddr);

#elif defined(EEPROM_M24C64_32)

	/*!< Send the EEPROM's internal address to write to : MSB of the address first */
	I2C_SendData(EEPROM_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

	/*!< Test on EV8 and clear it */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/*!< Send the EEPROM's internal address to write to : LSB of the address */
	I2C_SendData(EEPROM_I2C, (uint8_t)(WriteAddr & 0x00FF));

#endif /*!< EEPROM_M24C08 */

	/*!< Test on EV8 and clear it */
	EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/* Configure the DMA Tx Channel with the buffer address and the buffer size */
	EEPROM_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), EEPROM_DIRECTION_TX);

	/* Enable the DMA Tx Channel */
	DMA_Cmd(EEPROM_I2C_DMA_CHANNEL_TX, ENABLE);

	/* If all operations OK, return EEPROM_OK (0) */
	return EEPROM_OK;
}

/**
 * @brief  Writes buffer of data to the I2C EEPROM.
 * @param  pBuffer : pointer to the buffer  containing the data to be written
 *         to the EEPROM.
 * @param  WriteAddr : EEPROM's internal address to write to.
 * @param  NumByteToWrite : number of bytes to write to the EEPROM.
 * @retval None
 */
void EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
	uint16_t Addr = 0;

	Addr = WriteAddr % EEPROM_PAGESIZE;
	count = EEPROM_PAGESIZE - Addr;
	NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
	NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;

	/*!< If WriteAddr is EEPROM_PAGESIZE aligned  */
	if(Addr == 0)
	{
		/*!< If NumByteToWrite < EEPROM_PAGESIZE */
		if(NumOfPage == 0)
		{
			/* Store the number of data to be written */
			EEPROMDataNum = NumOfSingle;
			/* Start writing data */
			EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
			/* Wait transfer through DMA to be complete */
			EEPROMTimeout = EEPROM_LONG_TIMEOUT;
			while (EEPROMDataNum > 0)
			{
				if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
			}
			EEPROM_WaitEepromStandbyState();
		}
		/*!< If NumByteToWrite > EEPROM_PAGESIZE */
		else
		{
			while(NumOfPage--)
			{
				/* Store the number of data to be written */
				EEPROMDataNum = EEPROM_PAGESIZE;
				EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();
				WriteAddr +=  EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}

			if(NumOfSingle!=0)
			{
				/* Store the number of data to be written */
				EEPROMDataNum = NumOfSingle;
				EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();
			}
		}
	}
	/*!< If WriteAddr is not EEPROM_PAGESIZE aligned  */
	else
	{
		/*!< If NumByteToWrite < EEPROM_PAGESIZE */
		if(NumOfPage== 0)
		{
			/*!< If the number of data to be written is more than the remaining space
      in the current page: */
			if (NumByteToWrite > count)
			{
				/* Store the number of data to be written */
				EEPROMDataNum = count;
				/*!< Write the data conained in same page */
				EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();

				/* Store the number of data to be written */
				EEPROMDataNum = (NumByteToWrite - count);
				/*!< Write the remaining data in the following page */
				EEPROM_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();
			}
			else
			{
				/* Store the number of data to be written */
				EEPROMDataNum = NumOfSingle;
				EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();
			}
		}
		/*!< If NumByteToWrite > EEPROM_PAGESIZE */
		else
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
			NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;

			if(count != 0)
			{
				/* Store the number of data to be written */
				EEPROMDataNum = count;
				EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();
				WriteAddr += count;
				pBuffer += count;
			}

			while(NumOfPage--)
			{
				/* Store the number of data to be written */
				EEPROMDataNum = EEPROM_PAGESIZE;
				EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();
				WriteAddr +=  EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}
			if(NumOfSingle != 0)
			{
				/* Store the number of data to be written */
				EEPROMDataNum = NumOfSingle;
				EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEPROMDataNum));
				/* Wait transfer through DMA to be complete */
				EEPROMTimeout = EEPROM_LONG_TIMEOUT;
				while (EEPROMDataNum > 0)
				{
					if((EEPROMTimeout--) == 0) {EEPROM_TIMEOUT_UserCallback(); return;};
				}
				EEPROM_WaitEepromStandbyState();
			}
		}
	}
}

/**
 * @brief  Wait for EEPROM Standby state.
 *
 * @note  This function allows to wait and check that EEPROM has finished the
 *        last Write operation. It is mostly used after Write operation: after
 *        receiving the buffer to be written, the EEPROM may need additional
 *        time to actually perform the write operation. During this time, it
 *        doesn't answer to I2C packets addressed to it. Once the write operation
 *        is complete the EEPROM responds to its address.
 *
 * @note  It is not necessary to call this function after EEPROM_WriteBuffer()
 *        function (EEPROM_WriteBuffer() already calls this function after each
 *        write page operation).
 *
 * @param  None
 * @retval EEPROM_OK (0) if operation is correctly performed, else return value
 *         different from EEPROM_OK (0) or the timeout user callback.
 */
uint32_t EEPROM_WaitEepromStandbyState(void)
{
	__IO uint16_t tmpSR1 = 0;
	__IO uint32_t EEPROMTrials = 0;

	/*!< While the bus is busy */
	EEPROMTimeout = EEPROM_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
	{
		if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
	}

	/* Keep looping till the slave acknowledge his address or maximum number
     of trials is reached (this number is defined by EEPROM_MAX_TRIALS_NUMBER define
     in stm32_eval_i2c_ee.h file) */
	while (1)
	{
		/*!< Send START condition */
		I2C_GenerateSTART(EEPROM_I2C, ENABLE);

		/*!< Test on EV5 and clear it */
		EEPROMTimeout = EEPROM_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
		{
			if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
		}

		/*!< Send EEPROM address for write */
		I2C_Send7bitAddress(EEPROM_I2C, EEPROMAddress, I2C_Direction_Transmitter);

		/* Wait for ADDR flag to be set (Slave acknowledged his address) */
		EEPROMTimeout = EEPROM_LONG_TIMEOUT;
		do
		{
			/* Get the current value of the SR1 register */
			tmpSR1 = EEPROM_I2C->SR1;

			/* Update the timeout value and exit if it reach 0 */
			if((EEPROMTimeout--) == 0) return EEPROM_TIMEOUT_UserCallback();
		}
		/* Keep looping till the Address is acknowledged or the AF flag is
       set (address not acknowledged at time) */
		while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);

		/* Check if the ADDR flag has been set */
		if (tmpSR1 & I2C_SR1_ADDR)
		{
			/* Clear ADDR Flag by reading SR1 then SR2 registers (SR1 have already
         been read) */
			(void)EEPROM_I2C->SR2;

			/*!< STOP condition */
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

			/* Exit the function */
			return EEPROM_OK;
		}
		else
		{
			/*!< Clear AF flag */
			I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
		}

		/* Check if the maximum allowed numbe of trials has bee reached */
		if (EEPROMTrials++ == EEPROM_MAX_TRIALS_NUMBER)
		{
			/* If the maximum number of trials has been reached, exit the function */
			return EEPROM_TIMEOUT_UserCallback();
		}
	}
}

/**
 * @brief  This function handles the DMA Tx Channel interrupt Handler.
 * @param  None
 * @retval None
 */
void EEPROM_I2C_DMA_TX_IRQHandler(void)
{
	/* Check if the DMA transfer is complete */
	if(DMA_GetFlagStatus(EEPROM_I2C_DMA_FLAG_TX_TC) != RESET)
	{
		/* Disable the DMA Tx Channel and Clear all its Flags */
		DMA_Cmd(EEPROM_I2C_DMA_CHANNEL_TX, DISABLE);
		DMA_ClearFlag(EEPROM_I2C_DMA_FLAG_TX_GL);

		/*!< Wait till all data have been physically transferred on the bus */
		EEPROMTimeout = EEPROM_LONG_TIMEOUT;
		while(!I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BTF))
		{
			if((EEPROMTimeout--) == 0) EEPROM_TIMEOUT_UserCallback();
		}

		/*!< Send STOP condition */
		I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

		/* Perform a read on SR1 and SR2 register to clear eventualaly pending flags */
		(void)EEPROM_I2C->SR1;
		(void)EEPROM_I2C->SR2;

		/* Reset the variable holding the number of data to be written */
		*EEPROMDataWritePointer = 0;
	}
}

/**
 * @brief  This function handles the DMA Rx Channel interrupt Handler.
 * @param  None
 * @retval None
 */
void EEPROM_I2C_DMA_RX_IRQHandler(void)
{
	/* Check if the DMA transfer is complete */
	if(DMA_GetFlagStatus(EEPROM_I2C_DMA_FLAG_RX_TC) != RESET)
	{
		/*!< Send STOP Condition */
		I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

		/* Disable the DMA Rx Channel and Clear all its Flags */
		DMA_Cmd(EEPROM_I2C_DMA_CHANNEL_RX, DISABLE);
		DMA_ClearFlag(EEPROM_I2C_DMA_FLAG_RX_GL);

		/* Reset the variable holding the number of data to be read */
		*EEPROMDataReadPointer = 0;
	}
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
 * @brief  Basic management of the timeout situation.
 * @param  None.
 * @retval None.
 */
uint32_t EEPROM_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	while (1)
	{
	}
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

#ifdef USE_DEFAULT_CRITICAL_CALLBACK
/**
 * @brief  Start critical section: these callbacks should be typically used
 *         to disable interrupts when entering a critical section of I2C communication
 *         You may use default callbacks provided into this driver by uncommenting the
 *         define USE_DEFAULT_CRITICAL_CALLBACK.
 *         Or you can comment that line and implement these callbacks into your
 *         application.
 * @param  None.
 * @retval None.
 */
void EEPROM_EnterCriticalSection_UserCallback(void)
{
	__disable_irq();
}

/**
 * @brief  Start and End of critical section: these callbacks should be typically used
 *         to re-enable interrupts when exiting a critical section of I2C communication
 *         You may use default callbacks provided into this driver by uncommenting the
 *         define USE_DEFAULT_CRITICAL_CALLBACK.
 *         Or you can comment that line and implement these callbacks into your
 *         application.
 * @param  None.
 * @retval None.
 */
void EEPROM_ExitCriticalSection_UserCallback(void)
{
	__enable_irq();
}

uint8_t ADCT_AT24C64_ReadOneByte(uint16_t Address_ui16 )
{
	volatile uint16_t NumDataRead_ui16 = 1;
	uint8_t data_ui8;
	EEPROM_ReadBuffer(&data_ui8, Address_ui16, (uint16_t *)(&NumDataRead_ui16));
	while(NumDataRead_ui16 >0)
	{

	}
	return data_ui8;
}

uint16_t ADCT_AT24C64_ReadTwoBytes(uint16_t Address_ui16)
{
	volatile uint16_t NumDataRead_ui16 = 2;
	uint8_t pDataui8[NumDataRead_ui16];
	EEPROM_ReadBuffer(pDataui8, Address_ui16, (uint16_t *)(&NumDataRead_ui16));
	while (NumDataRead_ui16 > 0)
	{}
	return (pDataui8[1] * 256 + pDataui8[0]);
}

void ADCT_AT24C64_WriteTwoBytes(uint16_t ui16MemAddr , uint16_t ui16data)
{
	uint8_t txbuffer_a[2] = {(uint8_t)(ui16data%256), (uint8_t)(ui16data >> 8) };
	EEPROM_WriteBuffer(txbuffer_a , ui16MemAddr , 2 );
}
void ADCT_AT24C64_WriteOneBytes(uint16_t ui16MemAddr, uint8_t ui8data)
{
	EEPROM_WriteBuffer(&ui8data , ui16MemAddr , 1 );
}

#endif /* USE_DEFAULT_CRITICAL_CALLBACK */
