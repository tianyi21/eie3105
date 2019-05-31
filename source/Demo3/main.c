//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

struct loc_struct object[6];
struct loc_struct object_past[6];
struct loc_Update_struct loc_Update;
struct track_struct track;
struct track_struct finish; 
struct wifi_struct wifi;
struct wheel_counter_struct wheel_counter;
struct kick_struct kick;

//	SWTICH EXTI
int SWITCH_State = 1;


//	Main
int main(void)
{
	//	Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Init();
	WHEEL_PWM_Init();
	//WHEEL_COUNTER_EXTI_Init();
	Struct_Init();
	SWITCH_Init();
	LED_Init();
	USART2_Init();
	DEBUG_Init();
	WiFi_Connect();
	TIM2_Init();
	TIM4_Init();
	
	while(1)
	{
		char str[50] = {};
		sprintf(str, "X: %d, Y: %d\r\n", object[ball_3].X, object[ball_3].Y);
		DEBUG_Send_String(str, sizeof(str));
		switch (SWITCH_State)
		{
			case 1:
			{
				//TIM_Cmd(TIM2, DISABLE);
				var_clear(0xFFFF);
				PWM_Update(0, 1, 0);
				break;
			}
			case 2:
			{
				//	FORWARD
				track.ki = 0.1;
				track.base_speed = 45;
				track.thres = 25;
				kick.ball_current++;
				//	Update Car Init loc
				object_past[car].X = object[car].X;
				object_past[car].Y = object[car].Y;
				//	Init
				PWM_Update(0, 1, 1500);
				//	Delay
				var_clear((1<<DELAY_FLAG));
				one_time_delay(50);
				//	Enable TIM2
				TIM_Cmd(TIM2, ENABLE);
				SWITCH_State++;
			}
			case 3:
			{
				break;
			}
			case 5:
			{
				//	BACKWARD
				track.ki = 0;
				track.base_speed = 30;
				track.thres = 25;
				kick.ball_current++;
				//	Update Car Init loc
				object_past[car].X = object[car].X;
				object_past[car].Y = object[car].Y;
				//	Init
				PWM_Update(0, 2, 1500);
				//	Delay
				var_clear((1<<DELAY_FLAG));
				one_time_delay(2500);
				//	Enable TIM2
				TIM_Cmd(TIM2, ENABLE);
				SWITCH_State = 2;
			}
			case 6:
			{
				break;
			}
		}
	}
}

