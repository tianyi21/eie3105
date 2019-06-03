#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "stdio.h"
#include "pin_map.h"

char buffer[50] = {'\0'};
bool pulseHigh = false; 

char rcv = '\0';
char rcv_0 = '\0';
char rcv_1 = '\0';
bool flag_non_in = false;
bool flag_en_mes = false;
bool flag_change = false;
bool flag_mes_done = false;
bool flag_nl = false;
char str_err[30] = "No Input! Input Again!\n";
char str_res[30] = "";
int loc = 0;
int measure = 0;
int pwm_ic = 1;

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

void TIM4_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) !=RESET) 
	{
		if(!pulseHigh) 
		{
			measure = 0;
			pulseHigh=true;        //pulse starts
			TIM_SetCounter(TIM4,0);
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling); //change to detect falling
		} 
		else 
		{
			measure += TIM_GetCounter(TIM4);
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //change to detect raising
			pulseHigh= false;
			if (pwm_ic == 1)
			{
				flag_en_mes = false;
				sprintf(buffer, "%d\r\n", measure);
				USARTSend(buffer, sizeof(buffer));
			}
		}
	}
  TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC1); //Clear interrupt flag
}

void USART2_IRQHandler()
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rcv = (unsigned char) USART_ReceiveData(USART2);
		if (rcv == 'm' || rcv == 'M')
		{
			pwm_ic ^= 1;
			USART_SendData(USART2, '\0');
		}
		else
		{
			USART_SendData(USART2, rcv);
			if (rcv == 13)
			{
				flag_nl = true;
				if (rcv_0 == '\0')
				{
					flag_non_in = true;
					rcv = '\0';
					rcv_0 = '\0';
					rcv_1 = '\0';
				}
				else if (rcv_1 == '\0')
				{
					PWM_Update(rcv_0 - 48);
					flag_change = true;
					rcv = '\0';
					rcv_0 = '\0';
				}
				else
				{
					PWM_Update((rcv_1 - 48) * 10 + (rcv_0 - 48));
					flag_change = true;
					rcv = '\0';
					rcv_0 = '\0';
					rcv_1 = '\0';
				}
			}
			else if (rcv_0 == '\0')
			{
				rcv_0 = rcv;
				rcv = '\0';
			}
			else
			{
				rcv_1 = rcv_0;
				rcv_0 = rcv;
				rcv = '\0';
			}
		}
	}
	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		if (flag_nl)
		{	
			USART_SendData(USART2, '\n');
			flag_nl = false;
		}
		if (flag_non_in)
		{
			if (str_err[loc] != '\0')
			{
				USART_SendData(USART2, str_err[loc++]);
			}
			else
			{
				loc = 0;
				flag_non_in = false;
			}
		}
		else if (flag_mes_done)
		{
			if (str_res[loc] != '\0')
			{
				USART_SendData(USART2, str_res[loc++]);
			}
			else
			{
				loc = 0;
				flag_mes_done = false;
				flag_en_mes = true;
			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
}
