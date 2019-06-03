#include "stm32f10x.h"                  // Device header

int state_exti = 0;
int count = 0;

void init_pin()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStr;
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
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

void timer_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInitStr;
	timerInitStr.TIM_Prescaler = 18000 - 1;
	timerInitStr.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStr.TIM_Period = 800 - 1;
	timerInitStr.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStr.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStr);
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{	
		count++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
void trf_light()
{
	if (!state_exti)
	{	
		if (count >= 65 && count < 70)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
			if (count%2 == 1)
			{
				GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
				GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
				GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
			}
			else
			{
				GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
				GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
				GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
			}
		}
		else
		{
			switch (count)
			{
				case 0:
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
				
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
					break;
				}
				case 25:
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
				
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
					break;
				}
				case 30:
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
				
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
					break;
				}
				case 35:
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
					break;
				}
				case 40:
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
				
					GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
					break;
				}
				case 70:
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
				
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
					break;
				}
				case 75:
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
					break;
				}
				case 80:
				{
					count = 0;
					break;
				}
			}
		}
	}
	else 
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET);
		if (count>=80)
			count=0;
	}
}

void EXTI0_IRQHandler()
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		state_exti ^= 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

int main(void)
{
	init_pin();
	exti_init();
	timer_init();
	while(1)
	{
		trf_light();
	}
}
