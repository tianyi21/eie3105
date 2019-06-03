#include "stm32f10x.h"                  // Device header

void delay(int t)
{
	int i,j;
	for(i = 0; i < t; i++)
		j++;
}

void init_pin()
{
	GPIO_InitTypeDef GPIO_InitStr;
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStr);
}
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	init_pin();
	while(1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		delay(8000000);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		delay(8000000);
	}
}
