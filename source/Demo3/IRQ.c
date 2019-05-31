//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

int TIM4_Flag = 0;
int dist = 0;

//	Declare in main.c
extern int SWITCH_State;
extern struct loc_struct car_past;
extern struct loc_struct object[6];
extern struct loc_Update_struct loc_Update;
extern struct track_struct track;
extern struct wifi_struct wifi;
extern struct wheel_counter_struct wheel_counter;
extern struct kick_struct kick;

//	IRQ Handler
/*
//	EXTI9_5_IRQHandler()			-	BUTTON EXTI + LEFT WHEEL COUNTER		!!!WHEEL COUNTER NOT IN USE
//	EXTI1_IRQHandler()				-	RIGHT WHEEL COUNTER 								!!!NOT IN USE
//	TIM2_IRQHandler()					-	TRACK TIMER
//	TIM4_IRQHandler()					-	LOCTION UPDATE
//	USART2_IRQHandler()				-	WiFi RX Handler											!!!NOT IN USE
//
*/

//	LEFT WHEEL COUNTER + SWITCH S1 EXTI Handler
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) != RESET) 
	{	
		wheel_counter.count_left++;
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if (EXTI_GetITStatus(S1_EXTI_LINE) != RESET) 
	{	
		SWITCH_State++;
		if (SWITCH_State == 4)
			SWITCH_State = 1;
		EXTI_ClearITPendingBit(S1_EXTI_LINE);
	}
}

//	RIGHT WHEEL COUNTER
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		wheel_counter.count_right++;
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

//	Track WHEEL COUNTER	TIM2
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		if (SWITCH_State == 3 || SWITCH_State == 6)
		{
			dist = cal_dist(car, (obj)kick.ball_current);
			if (dist > track.thres)
				PID_track();
			else
			{
				SWITCH_State = 5 * (SWITCH_State == 3) + 2 * (SWITCH_State == 6);
				TIM_Cmd(TIM2, DISABLE);
			}
		}
	}                                    
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

//	WiFi TIM4
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{
		get_loc();
		//TIM4_Flag = 1;
	}                                    
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
//	WiFi USART2 RX 
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		if (TIM4_Flag)
		{
			wifi.Rcv = (unsigned char) USART_ReceiveData(USART2);
			
			if (wifi.Flag_USART_Info == 0)
			{
				if (wifi.Rcv == ':')
				{	
					wifi.Flag_USART_Info = 1;
					GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_SET);
					TIM_Cmd(TIM4, DISABLE);
				}
			}
			else if (wifi.Rcv != '\n' && wifi.Rcv != '\r')
			{
				if (wifi.Flag_X == 0 && wifi.Flag_Y == 0)
				{
					if (wifi.Rcv_Count < 3)
					{
						wifi.Rcv_Temp[wifi.Rcv_Count] = wifi.Rcv;
						wifi.Rcv_Count++;
					}
					else if (strcmp(wifi.Rcv_Temp, wifi.Tag_Car) == 0)
					{
						loc_Update.object = car;
						wifi.Flag_X = 1;
						wifi.Rcv_Count = 1;
						wifi.Rcv_Temp[0] = wifi.Rcv;
					}
					else if (strcmp(wifi.Rcv_Temp, wifi.Tag_Ball_1) == 0)
					{
						loc_Update.object = ball_1;
						wifi.Flag_X = 1;
						wifi.Rcv_Count = 1;
						wifi.Rcv_Temp[0] = wifi.Rcv;
					}
					else
					{
						wifi.Flag_USART_Info = 0;
						wifi.Rcv_Count = 0;
						GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_RESET);
						TIM4_Flag = 0;
						TIM_Cmd(TIM4, ENABLE);
					}
				}
				else if (wifi.Flag_X == 1)
				{
					if (wifi.Rcv_Count < 3)
					{
						wifi.Rcv_Temp[wifi.Rcv_Count] = wifi.Rcv;
						wifi.Rcv_Count++;
					}
					else
					{
						sscanf(wifi.Rcv_Temp, "%x", &loc_Update.X);
						wifi.Flag_X = 0;
						wifi.Flag_Y = 1;
						wifi.Rcv_Count = 1;
						wifi.Rcv_Temp[0] = wifi.Rcv;
					}
				}
				else if (wifi.Flag_Y == 1)
				{
					if (wifi.Rcv_Count < 3)
					{
						wifi.Rcv_Temp[wifi.Rcv_Count] = wifi.Rcv;
						wifi.Rcv_Count++;
					}
					else
					{
						sscanf(wifi.Rcv_Temp, "%x", &loc_Update.Y);
						update_loc(loc_Update);
						wifi.Flag_Y = 0;
						wifi.Rcv_Count = 1;
						wifi.Rcv_Temp[0] = wifi.Rcv;
					}
				}
			}
		}
	}
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}
