/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *   - Initial version and ATmega168 support
 * Modfications Copyright (C) 2006 Tran Minh Hoang:
 *   - ATmega8, ATmega16, ATmega32 support
 *   - RS485 support for DS75176
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.6 2006/09/17 16:45:53 wolti Exp $
 */

#include "stm32f10x_conf.h"
#include "HardwareCfg.h"
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	if( xRxEnable )
	{
		/* enable RX interrupt */
		/* switch 485 to receive mode */
		GPIO_ResetBits(ADCT_BGS_RS485_DE_RE_PORT, ADCT_BGS_RS485_DE_RE_PIN);
		USART_ITConfig( ADCT_BGS_RS485_USART_PORT, USART_IT_RXNE, ENABLE );
	}
	else
	{
		/* disable RX interrupt */
		USART_ITConfig( ADCT_BGS_RS485_USART_PORT, USART_IT_RXNE, DISABLE );
	}

	if ( xTxEnable )
	{
		/* switch 485 to transmit mode */
		GPIO_SetBits(ADCT_BGS_RS485_DE_RE_PORT, ADCT_BGS_RS485_DE_RE_PIN);
		/* enable RX interrupt */
		USART_ITConfig( ADCT_BGS_RS485_USART_PORT, USART_IT_TXE, ENABLE );
	}
	else
	{
		/* disable TX interrupt */
		USART_ITConfig( ADCT_BGS_RS485_USART_PORT, USART_IT_TXE, DISABLE );
	}
}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
		eMBParity eParity )
{
	NVIC_InitTypeDef        NVIC_InitStructure;
	GPIO_InitTypeDef        GPIO_InitStructure;
	USART_InitTypeDef       USART_InitStructure;

	/* prevent compiler warning. */
	(void) ucPORT;

	RCC_APB1PeriphClockCmd( ADCT_BGS_RS485_USART_CLK, ENABLE );
	RCC_APB2PeriphClockCmd( ADCT_BGS_RS485_DE_RE_CLK|ADCT_BGS_RS485_CLK|RCC_APB2Periph_AFIO, ENABLE );


	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_RS485_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( ADCT_BGS_RS485_PORT, &GPIO_InitStructure );


	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_RS485_RX_PIN    ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( ADCT_BGS_RS485_PORT, &GPIO_InitStructure );
	NVIC_InitStructure.NVIC_IRQChannel = ADCT_BGS_RS485_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADCT_BGS_RS485_DE_RE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( ADCT_BGS_RS485_DE_RE_PORT, &GPIO_InitStructure );

	USART_InitStructure.USART_BaudRate = (uint32_t)ulBaudRate;

	if( ucDataBits == 9 )
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	else
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_InitStructure.USART_StopBits = USART_StopBits_1;

	switch( eParity )
	{
	case MB_PAR_NONE:
		USART_InitStructure.USART_Parity = USART_Parity_No;
		break;
	case MB_PAR_ODD:
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		break;
	case MB_PAR_EVEN:
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		break;
	default:
		USART_InitStructure.USART_Parity = USART_Parity_No;
		break;
	};

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init( ADCT_BGS_RS485_USART_PORT, &USART_InitStructure );

	vMBPortSerialEnable( TRUE, TRUE );

	USART_Cmd( ADCT_BGS_RS485_USART_PORT, ENABLE );

	return TRUE;
}

BOOL xMBPortSerialPutByte( CHAR ucByte )
{
	USART_SendData( ADCT_BGS_RS485_USART_PORT, (uint16_t) ucByte );
	return TRUE;
}

BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
	*pucByte = (CHAR) USART_ReceiveData( ADCT_BGS_RS485_USART_PORT );
	return TRUE;
}

void USART3_IRQHandler( void )
{
	if ( USART_GetITStatus( ADCT_BGS_RS485_USART_PORT, USART_IT_RXNE ) != RESET )
	{
		USART_ClearITPendingBit( ADCT_BGS_RS485_USART_PORT, USART_IT_RXNE );
		pxMBFrameCBByteReceived();

	}
	if ( USART_GetITStatus( ADCT_BGS_RS485_USART_PORT, USART_IT_TXE ) != RESET )
	{
		pxMBFrameCBTransmitterEmpty();
		//TODO: put this line after calling pxMBFrameCBTransmitterEmpty() fucntion
		USART_ClearITPendingBit( ADCT_BGS_RS485_USART_PORT, USART_IT_TXE );
	}
}
