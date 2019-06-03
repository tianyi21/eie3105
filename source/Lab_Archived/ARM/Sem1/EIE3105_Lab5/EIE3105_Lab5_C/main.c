#include "stm32f10x.h"                  // Device header

static __IO uint32_t msTicks;

void init_pin()
{
	GPIO_InitTypeDef GPIO_InitStr;
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStr);
}

void DelayMs(uint32_t ms)
{
	msTicks = ms;
	while (msTicks);
}

void SysTick_Handler()
{
	if (msTicks != 0)
		msTicks--;
}

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	init_pin();
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	
	while(1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		DelayMs(1000);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		DelayMs(1000);
	}
}
