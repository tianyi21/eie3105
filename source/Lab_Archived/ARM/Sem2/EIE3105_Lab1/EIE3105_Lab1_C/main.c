#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "stdio.h"
#include "pin_map.h"

char buffer[50] = {'\0'};
bool pulseHigh = false; 
u32 pulseWidth = 0;

char rcv = '\0';
	
int main()
{
	TIM4_CH1_IC1_Init();
	TIM3_CH1_PWM_Init();
	USART2_Init();
	while(1)
	{
		;
	}
}



void TIM4_IRQHandler(void) {
	
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) !=RESET) {
		if(!pulseHigh) {
			pulseHigh=true;        //pulse starts
			TIM_SetCounter(TIM4,0);
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling); //change to detect falling
		} else {
			pulseWidth += TIM_GetCounter(TIM4);
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //change to detect raising
			sprintf(buffer, "%d\r\n", pulseWidth);
		  USARTSend(buffer, sizeof(buffer));
			pulseHigh= false;
			pulseWidth=0;
		}
	}
  TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC1); //Clear interrupt flag
}
