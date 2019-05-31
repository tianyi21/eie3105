//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

//	Declare in main.c
extern int SWITCH_State;
extern struct loc_struct car_past;
extern struct loc_struct object[4];
extern struct loc_Update_struct loc_Update;
extern struct track_struct track;
extern struct wifi_struct wifi;
extern struct kick_struct kick;

//	IRQ Handler
/*
//	EXTI9_5_IRQHandler()			-	SWITCH S1 EXTI IRQ
//	TIM2_IRQHandler()					-	TRACK IRQ
//	TIM4_IRQHandler()					-	LOCTION UPDATE IRQ
//
*/

//	SWITCH S1 EXTI
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(S1_EXTI_LINE) != RESET) 
	{	
		SWITCH_State++;
		if (SWITCH_State == 3)
			SWITCH_State = 1;
		EXTI_ClearITPendingBit(S1_EXTI_LINE);
	}
}

//	TRACK IRQ
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		if (kick.state == FORWARD)
		{
			kick.dist = cal_dist(CAR, (obj)kick.ball_current);
			if (kick.dist > track.thres)
				PID_track();
			else
			{
				if (kick.ball_current == TARGET)
					kick.state = ROTATE;
				else if (kick.ball_current == BASE)
					kick.state = FINISH;
				TIM_Cmd(TIM2, DISABLE);
			}
		}
	}                                    
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

//	LOCTION UPDATE IRQ
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
		get_loc();                                  
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
