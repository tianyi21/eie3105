//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

void USART_Send_Char(unsigned char ch)
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
	USART_Send_String(buffer, sizeof(buffer));
}
void USART_Send_Int(int num)
{
	/*	Usage:
	//		INPUT:
	//			num: int to be sent
	//
	*/
	
	char buffer[20] = "";
	sprintf(buffer, "%d\r\n",num);
	USART_Send_String(buffer, sizeof(buffer));
}
void USART_Send_String(char* pucBuffer, unsigned long ulCount)
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
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, *pucBuffer++);
	}
}
