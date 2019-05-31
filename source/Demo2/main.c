//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "stm32f10x.h"                  // Device header
#include "pin_map.h"
#include "stdio.h"
#include "stdbool.h"

//	Debug
char debug_string[50] = "THE PROGRAM WORKS PROPERLY.\r\n";
char start_string[50] = "THE PROGRAM STARTS HERE.\r\n";

bool TIM_IT = false;

//	Global Variables
//	SWTICH EXTI
int SWITCH_State = 0;

//	Main
int main(void)
{
	WHEEL_PWM_Init();
	SWITCH_Init();
	LED_Init();
	SPI2_Init();
	USART2_Init();
	USART_Send_String(start_string, sizeof(start_string));
	while(1)
	{
		if (SWITCH_State == 0)
		{
			PWM_Update(0, 1, 0);
			GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_RESET);
		}
		else if (SWITCH_State == 1)
			SPI2_Detect();
	}
}

//	IRQ Handler
/*
//	EXTI9_5_IRQHandler()			-	BUTTON EXTI
//
*/

//	SWITCH S1 EXTI Handler
void EXTI9_5_IRQHandler()
{
	if (EXTI_GetITStatus(S1_EXTI_LINE) != RESET) 
		SWITCH_State++;
	if (SWITCH_State == 2)
		SWITCH_State = 0;
	EXTI_ClearITPendingBit(S1_EXTI_LINE);
}
