#include "stm32f10x.h"                  // Device header

void init_pin()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStr;
	
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStr);
	
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStr);
}

int state = 0;
int press = 0;

int main(void)
{
	init_pin();
	while(1)
	{
		if (!press)
		{
			if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
			{
				press = 1;
			}
		}
		else
		{
			if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
			{
				press = 0;
				state ^= 1;
				if (state)
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
				else
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
			}
		}
	}
}
