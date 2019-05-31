//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

struct loc_struct object[4];
struct loc_struct object_past[4];
struct loc_Update_struct loc_Update;
struct track_struct track;
struct track_struct finish;
struct wifi_struct wifi;
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
	Struct_Init();
	SWITCH_Init();
	LED_Init();
	USART2_Init();
	DEBUG_Init();
	WiFi_Connect();
	TIM2_Init();
	TIM4_Init();
	var_clear(0xFF);
	PWM_Update(0, 1, 0);
	
	while(1)
	{
		switch (SWITCH_State)
		{
			case 0:
			{
				var_clear(0xFF);
				PWM_Update(0, 1, 0);
				break;
			}
			case 1:
			{
				kick_main();
				break;
			}
		}
	}
}

