//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

void DEBUG_Init()
{
    //	USART3 TX RX
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //	USART3 ST-LINK USB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);
}

void DEBUG_Send_Char(unsigned char ch)
{
	/*	Usage:
	//		char sent as int value
	//		INPUT:
	//			ch: int to be sent
	//
	*/
	
	int int_ch = (int)ch;
	char buffer[20] = "";
	sprintf(buffer, "%d\r\n", int_ch);
	DEBUG_Send_String(buffer, sizeof(buffer));
}
void DEBUG_Send_Int(int num)
{
	/*	Usage:
	//		INPUT:
	//			num: int to be sent
	//
	*/
	
	char buffer[20] = "";
	sprintf(buffer, "%d\r\n",num);
	DEBUG_Send_String(buffer, sizeof(buffer));
}
void DEBUG_Send_String(char* pucBuffer, unsigned long ulCount)
{
	/*	Usage
	//		INPUT:
	//			pucBuffer:	String to be sent
	//			ulCount:		length of string
	//		EXAMPLE:
	//			sprintf(buffer, "ch0=%d ch1=%d ch4=%d\r\n", ADC_values[0], ADC_values[1], ADC_values[2]);
	//			USART_Send(buffer, sizeof(buffer));
	*/
	
	while(ulCount--)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		USART_SendData(USART3, *pucBuffer++);
	}
			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		USART_SendData(USART3, '\r');
			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		USART_SendData(USART3, '\n');
}
