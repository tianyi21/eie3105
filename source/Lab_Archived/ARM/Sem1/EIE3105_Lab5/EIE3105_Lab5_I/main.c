#include "stm32f10x.h"                  // Device header

unsigned char rcv = '\0';
unsigned char rcv2 = '\0';
int state = 0;
int loc = 0;

void usart2_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStr;
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStr);
	
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStr);
	
	USART_InitTypeDef USART_InitStr;
	USART_InitStr.USART_BaudRate = 9600;
	USART_InitStr.USART_WordLength = USART_WordLength_8b;
	USART_InitStr.USART_StopBits = USART_StopBits_1;
	USART_InitStr.USART_Parity = USART_Parity_No;
	USART_InitStr.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStr.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStr);
	USART_Cmd(USART2, ENABLE);
}


void usart2_int_init()
{
	NVIC_InitTypeDef NVIC_InitStr;
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	NVIC_InitStr.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStr.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStr.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStr);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_InitStr.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStr.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStr.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStr);
}



void USART2_IRQHandler()
{
	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		if (state == 0)
			USART_SendData(USART2, 'a');
		else if (state == 1)
		{
			if (loc < 10)
			{
				USART_SendData(USART2, rcv);
				loc++;
			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		if (state == 0)
		{
			rcv = (unsigned char) USART_ReceiveData(USART2);
			state = 1;
		}
		else if (state == 1)
		{
			rcv2 = (unsigned char) USART_ReceiveData(USART2);
			if (rcv2 == rcv)
			{
				rcv = '\0';
				rcv2 = '\0';
				loc = 0;
				state = 0;
			}
		}
		//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART_SendData(USART2, '\0');
	}
}

int main()
{
	usart2_init();
	usart2_int_init();
	while(1)
	{
	}
}

