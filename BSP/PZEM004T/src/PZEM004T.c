/*
 * ADCT_SIM900_LL.c
 *
 *  Created on: May 12, 2017
 *      Author: Administrator
 */
#include <PZEM004T.h>
#include <stdio.h>
#include <string.h>
#include "fifo.h"
#include "ADCT_BGS_FPT_Delay.h"
#include "ADCT_BGS_DataTypes.h"
#include "string.h"
#include "math.h"
extern FPT_SystemStatus    GstSystemStatus;
static uint8_t ADCT_PZEM004T_RXBUFFER[ADCT_PZEM004T_BUF_LEN]; ///< Buffer for received data.
static uint8_t ADCT_PZEM004T_TXBUFFER[ADCT_PZEM004T_BUF_LEN]; ///< Buffer for transmitted data.

static FIFO_TypeDef rxFifo; ///< RX FIFO
static FIFO_TypeDef txFifo; ///< TX FIFO
uint8_t AC_DataBuf[NUM_O_INFOR][6];
PZEM004_ReceiveData AC_DataReceive_Cfg[NUM_O_INFOR]=
{
		{
				VOLTAGE,
				RESP_VOLTAGE,
				AC_DataBuf[VOLTAGE],
				0
		},
		{
				CURRENT,
				RESP_CURRENT,
				AC_DataBuf[CURRENT],
				0
		},
		{
				ACTIVE_POWER,
				RESP_POWER,
				AC_DataBuf[ACTIVE_POWER],
				0
		},
		{
				ENERGY,
				RESP_ENERGY,
				AC_DataBuf[ENERGY],
				0

		},
		{
				SET_ADDR,
				RESP_SET_ADDRESS,
				AC_DataBuf[SET_ADDR],
				0
		},
		{
				SET_POWER_ALARM_THRES,
				RESP_POWER_ALARM,
				AC_DataBuf[SET_POWER_ALARM_THRES],
				0
		}
};


uint8_t IP_addr[4] = {192,168,1,1};
uint8_t PZEM004T_TxCallback(uint8_t* c);
void    PZEM004T_RxCallback(uint8_t c);
static void    (*rxCallback)(uint8_t);   ///< Callback function for receiving data
static uint8_t (*txCallback)(uint8_t*);  ///< Callback function for transmitting data
uint8_t ADCT_PZEM004T_crc(uint8_t *data, uint8_t sz);
uint8_t ADCT_PZEM004T_receiveData(uint8_t resp, uint8_t *data);
void ADCT_PZEM0004T_sendData(const uint8_t *addr, uint8_t cmd, uint8_t data);
void ADCT_PZEM004T_PutChar(uint8_t  ch)
{
	// disable IRQ so it doesn't screw up FIFO count - leads to errors in transmission
	ADCT_PZEM004T_IrqDisable;
	FIFO_Push(&txFifo,ch); // Put data in TX buffer
	UART_PZEM004T_TransmitEnable();// Enable low level transmitter
	// enable IRQ again
	ADCT_PZEM004T_IrqEnable;
}
/**
 * @brief Get a char from USART1
 * @return Received char.
 * @warning Blocking function! Waits until char is received.
 */
uint8_t ADCT_PZEM004T_GetChar(void) {

	uint8_t c;

	while (FIFO_IsEmpty(&rxFifo) == 1); // wait until buffer is not empty
	// buffer not empty => char was received

	FIFO_Pop(&rxFifo,&c); // Get data from RX buffer

	return c;
}
void ADCT_PZEM004T_PutString(char * str)
{
	while(*str != 0)
	{
		ADCT_PZEM004T_PutChar(*str);
		str++;
	}
}


/**
 * @brief Send a zero terminated string to SIm900.
 *
 * @param buf String to send.
 */
void ADCT_PZEM004T_PutFrame(char* buf) {

	uint16_t len = strlen(buf);

	for(int i = 0; i < len; i++) {
		ADCT_PZEM004T_PutChar(buf[i]);
	}

}
float ADCT_PZEM004T_getVoltage(const uint8_t *addr)
{
	uint8_t data[RESPONSE_DATA_SIZE];

	ADCT_PZEM0004T_sendData(addr, PZEM_VOLTAGE,0);
	if(!ADCT_PZEM004T_receiveData(RESP_VOLTAGE, data))
		return PZEM_ERROR_VALUE;

	return (data[0] << 8) + data[1] + (data[2] / 10.0);
}

float ADCT_PZEM004T_getCurrent(const uint8_t *addr)
{
	uint8_t data[RESPONSE_DATA_SIZE];

	ADCT_PZEM0004T_sendData(addr, PZEM_CURRENT,0);
	if(!ADCT_PZEM004T_receiveData(RESP_CURRENT, data))
		return PZEM_ERROR_VALUE;

	return (data[0] << 8) + data[1] + (data[2] / 100.0);
}

float ADCT_PZEM004T_getPower(const uint8_t *addr)
{
	uint8_t data[RESPONSE_DATA_SIZE];

	ADCT_PZEM0004T_sendData(addr, PZEM_POWER,0);
	if(!ADCT_PZEM004T_receiveData(RESP_POWER, data))
		return PZEM_ERROR_VALUE;

	return (data[0] << 8) + data[1];
}

float ADCT_PZEM004T_getEnergy(const uint8_t *addr)
{
	uint8_t data[RESPONSE_DATA_SIZE];

	ADCT_PZEM0004T_sendData(addr, PZEM_ENERGY,0);
	if(!ADCT_PZEM004T_receiveData(RESP_ENERGY, data))
		return PZEM_ERROR_VALUE;

	return ((uint32_t)data[0] << 16) + ((uint16_t)data[1] << 8) + data[2];
}

uint8_t ADCT_PZEM004T_setAddress(const uint8_t *addr)
{
	uint8_t data[RESPONSE_DATA_SIZE];
	ADCT_PZEM0004T_sendData(addr, PZEM_SET_ADDRESS,0);
	return  ADCT_PZEM004T_receiveData(RESP_SET_ADDRESS, data);
}

uint8_t ADCT_PZEM004T_setPowerAlarm(const uint8_t *addr, uint8_t threshold)
{
	uint8_t data[RESPONSE_DATA_SIZE];
	ADCT_PZEM0004T_sendData(addr, PZEM_POWER_ALARM, threshold);
	return  ADCT_PZEM004T_receiveData(RESP_POWER_ALARM, data);
}
uint8_t ADCT_PZEM004T_receiveData(uint8_t resp, uint8_t *data)
{
	uint8_t buffer[RESPONSE_SIZE];
	uint8_t c;
	uint32_t ui32startTime = TM_DELAY_Time();
	uint8_t len = 0;
	while ((len < RESPONSE_SIZE)&&((TM_DELAY_Time() - ui32startTime)<PZEM_DEFAULT_READ_TIMEOUT))
	{
		if (!FIFO_IsEmpty(&rxFifo))
		{
			FIFO_Pop(&rxFifo, &c);
			if(!c && !len)
				continue; // skip 0 at startup
				buffer[len++] = c;
		}
	}
	if(len != RESPONSE_SIZE)
		return 0;

	if(buffer[6] != ADCT_PZEM004T_crc(buffer, len - 1))
		return 0;

	if(buffer[0] != resp)
		return 0;

	if(data)
	{
		for(uint8_t i=0; i<RESPONSE_DATA_SIZE; i++)
			data[i] = buffer[1 + i];
	}

	return 1;

}
void ADCT_PZEM0004T_sendData(const uint8_t *addr, uint8_t cmd, uint8_t data)
{
	uint8_t ui8index;
	PZEMCommand pzem;
	pzem.command = cmd;
	for(ui8index = 0;ui8index < sizeof (pzem.addr); ui8index++ )
	{
		pzem.addr[ui8index] = addr[ui8index];
	}
	pzem.data = data;
	uint8_t *bytes = (uint8_t *)&pzem;
	pzem.crc = ADCT_PZEM004T_crc(bytes, sizeof(pzem) - 1);
	for(ui8index = 0; ui8index < sizeof(pzem) ; ui8index++)
	{
		ADCT_PZEM004T_PutChar(bytes[ui8index]);
	}
}
uint8_t ADCT_PZEM004T_crc(uint8_t *data, uint8_t sz)
{
	uint16_t crc = 0;
	for(uint8_t i=0; i<sz; i++)
		crc += *data++;
	return (uint8_t)(crc & 0xFF);
}
void ADCT_PZEM004T_Init(uint32_t baud)
{
	// pass baud rate
	// callback for received data and callback for
	// transmitted data
	UART_PZEM004T_Init(baud, PZEM004T_RxCallback, PZEM004T_TxCallback );
	// Initialize RX FIFO
	rxFifo.buf = ADCT_PZEM004T_RXBUFFER;
	rxFifo.len = ADCT_PZEM004T_BUF_LEN;
	FIFO_Add(&rxFifo);

	// Initialize TX FIFO
	txFifo.buf = ADCT_PZEM004T_TXBUFFER;
	txFifo.len = ADCT_PZEM004T_BUF_LEN;
	FIFO_Add(&txFifo);
}

void UART_PZEM004T_Init(uint32_t baud, void(*rxCb)(uint8_t), uint8_t(*txCb)(uint8_t*) )
{
	// assign the callbacks
	rxCallback = rxCb;
	txCallback = txCb;

	NVIC_InitTypeDef        NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(ADCT_PZEM004T_TX_CLK |ADCT_PZEM004T_RX_CLK| RCC_APB2Periph_AFIO, ENABLE);
	/* Enable UART clock, if using USART2 or USART3 ... we must use RCC_APB1PeriphClockCmd(RCC_APB1Periph_USARTx, ENABLE) */
	RCC_APB2PeriphClockCmd(ADCT_PZEM004T_USART_CLK, ENABLE);

	/* Configure PA2 for USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = ADCT_PZEM004T_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADCT_PZEM004T_TX_PORT, &GPIO_InitStructure);

	/* Configure PA3 for USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = ADCT_PZEM004T_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(ADCT_PZEM004T_RX_PORT, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = ADCT_PZEM004T_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* USARTx configured as follow:
	        - BaudRate = baud
	        - Word Length = 8 Bits
	        - One Stop Bit
	        - No parity
	        - Hardware flow control disabled (RTS and CTS signals)
	        - Receive and transmit enabled
	 */
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
	USART_Init(ADCT_PZEM004T_USART_PORT, &USART_InitStructure);

	USART_ITConfig( ADCT_PZEM004T_USART_PORT, USART_IT_RXNE, ENABLE );
	//USART_ITConfig( ADCT_PZEM004T_USART_PORT, USART_IT_TXE, ENABLE );
	/* Enable USART */
	USART_Cmd(ADCT_PZEM004T_USART_PORT, ENABLE);
}

/**
 * @brief Callback for receiving data from PC.
 * @param c Data sent from lower layer software.
 */
static uint8_t idElement = 0 ,start_get = 0;
void PZEM004T_RxCallback(uint8_t c) {

	uint8_t index ;

	FIFO_Push(&rxFifo, c); // Put data in RX buffer
	uint8_t idFunc = 0 ;

	//  for (index =0; index < NUM_O_INFOR; index ++)
	//  {
	//	  switch (PZEM004_DataBuff[index].state)
	//	  {
	//	  case START:
	//	  {
	//		  if(c == PZEM004_DataBuff[index].Header)
	//		  {
	//			  PZEM004_DataBuff[index].state = PROCESS;
	//		  }
	//		  else
	//		  {
	//			  PZEM004_DataBuff[index].state = START;
	//		  }
	//		  break;
	//	  }
	//	  case PROCESS:
	//	  {
	//		  if (idElement < 5)
	//		  {
	//			  PZEM004_DataBuff[index].data[idElement] = c;
	//			  idElement++;
	//			  PZEM004_DataBuff[index].state = PROCESS;
	//		  }
	//		  else
	//		  {
	//			  PZEM004_DataBuff[index].state = FINISH;
	//		  }
	//		  break;
	//	  }
	//	  case FINISH:
	//	  {
	//		  PZEM004_DataBuff[index].crc = c;
	//		  PZEM004_DataBuff[index].state = START;
	//		  break;
	//	  }
	//	  }
	//
	//
	//  }
//
//	for (index = 0 ; index < NUM_O_INFOR ; index ++)
//	{
//		if(c == AC_DataReceive_Cfg[index].response)
//		{
//			idFunc = index;
//			start_get =1;
//			break;
//		}
//	}
//	if(start_get == 1)
//	{
//		//AC_DataBuf[idFunc][idElement++] = c;
//		AC_DataBuf[idFunc][idElement++] = c;
//			if(idElement > 7)
//			{
//				AC_DataReceive_Cfg[idFunc].gotframe = 1;
//				idElement = 0;
//				start_get = 0 ;
//			}
//	}
}
/**
 * @brief Callback for transmitting data to lower layer
 * @param c Transmitted data
 * @retval 0 There is no more data in buffer (stop transmitting)
 * @retval 1 Valid data in c
 */
uint8_t PZEM004T_TxCallback(uint8_t* c) {

	if (FIFO_Pop(&txFifo, c) == 0) { // If buffer not empty
		return 1;
	} else {
		return 0;
	}

}

void USART1_IRQHandler( void )
{
	// If transmit buffer empty interrupt
	if(USART_GetITStatus(ADCT_PZEM004T_USART_PORT, USART_IT_TXE) != RESET) {

		uint8_t c;

		if (txCallback) { // if not NULL
			// get data from higher layer using callback
			if (txCallback(&c)) {
				USART_SendData(ADCT_PZEM004T_USART_PORT, c); // Send data
			} else { // if no more data to send disable the transmitter
				USART_ITConfig(ADCT_PZEM004T_USART_PORT, USART_IT_TXE, DISABLE);
			}
		}
	}

	// If RX buffer not empty interrupt
	if(USART_GetITStatus(ADCT_PZEM004T_USART_PORT, USART_IT_RXNE) != RESET) {

		uint8_t c = USART_ReceiveData(ADCT_PZEM004T_USART_PORT); // Get data from UART

		if (rxCallback) { // if not NULL
			rxCallback(c); // send received data to higher layer
		}
		USART_ClearITPendingBit( ADCT_PZEM004T_USART_PORT, USART_IT_RXNE );
	}
}
void ADCT_AC_Init(void)
{

	ADCT_PZEM004T_Init(9600);
	ADCT_PZEM004T_setAddress(IP_addr);
	ADCT_PZEM004T_setPowerAlarm(IP_addr, 21);
}
static uint8_t ac_cnt = 0;
void ADCT_AC_GetData(void)
{
	if(ac_cnt ==0)
	{
	GstSystemStatus.acData.voltage = ADCT_PZEM004T_getVoltage(IP_addr);
	}else if (ac_cnt == 1 )
	{
	GstSystemStatus.acData.current = ADCT_PZEM004T_getCurrent(IP_addr);
	}
	else if (ac_cnt ==2)
	{
	GstSystemStatus.acData.power = ADCT_PZEM004T_getPower(IP_addr);
	}else
	{

	GstSystemStatus.acData.energy = ADCT_PZEM004T_getEnergy(IP_addr);
	}
	if(ac_cnt >3)
	{
		ac_cnt =0;
	}
	else
	{
	ac_cnt ++;
	}
}
void ADCT_AC_ConvertCurrent(float data, uint32_t *Sig, uint8_t *exp)
{
	//Extract integer part
	int ipart = (int)data;
	//extract float part
	float fpart = data -(float)ipart;
	fpart = fpart *pow(10, 3);
	if(ipart == 0 )
	{
		*Sig = (uint32_t)(fpart);
		*exp = 1;
	}
	else
	{
		*Sig = (uint32_t) ((ipart *1000) + fpart);
		*exp = 2;
	}

}
void ADCT_AC_Convert_Power(float data, uint32_t *Sig, uint8_t * exp)
{
	// Extract integer part

	int ipart = (int)data;
	// Extract floating part
	float fpart = data - (float)ipart;
	fpart = fpart *pow(10,3);

	if(ipart == 0)
	{
		*Sig = (uint32_t)(fpart);
		*exp = 1;
	}else if( ipart < 64)
	{
		*Sig = (uint32_t)((ipart *1000) +fpart) ;
		*exp = 2;
	}
	else
	{
		*Sig = (uint32_t)((ipart *10)  + fpart /100);
		*exp = 3;
	}
}

void ADCT_AC_Convert_Energy(float data, uint32_t *Sig, uint8_t * exp)
{
	// Extract integer part

	int ipart = (int)data;
	// Extract floating part
	float fpart = data - (float)ipart;
	fpart = fpart *pow(10,3);

	if(ipart == 0)
	{
		*Sig = (uint32_t)(fpart);
		*exp = 1;
	}else if (ipart < 650)
	{
		*Sig = (uint32_t)((ipart *100) + fpart/10) ;
		*exp = 2;
	}
	else if( ipart <6552)
	{
		*Sig = (uint32_t)((ipart *10) + fpart/100) ;
		*exp = 3;
	}
	else
	{
		*Sig = (uint32_t)(ipart ) ;
		*exp = 4;
	}
}
