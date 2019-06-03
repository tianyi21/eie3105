#include "stm32f10x.h"                  // Device header

int count = 0;

void init_pin()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStr;
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStr);
	
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStr);
}


void exti_init()
{
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
	
	EXTI_InitTypeDef EXTI_InitStr;
	EXTI_InitStr.EXTI_Line = EXTI_Line0;
	EXTI_InitStr.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStr.EXTI_LineCmd = ENABLE;
	EXTI_InitStr.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStr);
	
	NVIC_InitTypeDef NVIC_InitStr;
	NVIC_InitStr.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStr.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_Init(&NVIC_InitStr);
}


void EXTI0_IRQHandler()
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		count++;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

int main(void)
{
	init_pin();
	exti_init();
	while(1)
	{
		if(count == 3)
		{
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10))
				GPIO_WriteBit(GPIOA,GPIO_Pin_10, Bit_RESET);
			else
				GPIO_WriteBit(GPIOA,GPIO_Pin_10, Bit_SET);
			count = 0;
		}
	}
}
