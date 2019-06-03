#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
	GPIOA->CRL &= ~0x00F00000;
	GPIOA->CRL |= 0<<22;
	GPIOA->BSRR |= 0x20;
	while (1)
	{
		
	}
}