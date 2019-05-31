//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

//	Declare in main.c
extern int SWITCH_State;
extern struct loc_struct object[6];
extern struct loc_struct object_past[6];
extern struct loc_Update_struct loc_Update;
extern struct track_struct track;
extern struct wheel_counter_struct wheel_counter;
extern struct kick_struct kick;

//	Declare in IRQ.c
extern int dist;	

void PWM_Update(int whl_sel, int whl_dir, int whl_pwm)
{
	/*	Usage
	//		INPUT:
	//			whl_sel: Wheel Selection
	//				0: Left + Right Wheel
	//				1: Left Wheel
	//				2: Right Wheel
	//			whl_dir: Wheel Direction
	//				0: No Change
	//				1: Forward 
	//				2: Backward
	//			whl_pwm: New PWM (DC%)
	//
	*/			
	
	//	Update PWM
	TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = whl_pwm; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	
	switch (whl_sel)
	{
		case 0:
		{
			TIM_OC1Init(TIM3, &outputChannelInit);
			TIM_OC2Init(TIM3, &outputChannelInit);
			TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);		
			if (whl_dir == 1)
			{
				GPIO_WriteBit(LEFTPHASE_GPIO, LEFTPHASE_PIN, Bit_SET);
				GPIO_WriteBit(RIGHTPHASE_GPIO, RIGHTPHASE_PIN, Bit_SET);
			}
			else if (whl_dir == 2)
			{
				GPIO_WriteBit(LEFTPHASE_GPIO, LEFTPHASE_PIN, Bit_RESET);
				GPIO_WriteBit(RIGHTPHASE_GPIO, RIGHTPHASE_PIN, Bit_RESET);
			}
			break;
		}
		case 1:
		{
			TIM_OC2Init(TIM3, &outputChannelInit);
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
			if (whl_dir == 1)
				GPIO_WriteBit(LEFTPHASE_GPIO, LEFTPHASE_PIN, Bit_SET);
			else if (whl_dir == 2)
				GPIO_WriteBit(LEFTPHASE_GPIO, LEFTPHASE_PIN, Bit_RESET);
			break;
		}
		case 2:
		{
			TIM_OC1Init(TIM3, &outputChannelInit);
			TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
			if (whl_dir == 1)
				GPIO_WriteBit(RIGHTPHASE_GPIO, RIGHTPHASE_PIN, Bit_SET);
			else if (whl_dir == 2)
				GPIO_WriteBit(RIGHTPHASE_GPIO, RIGHTPHASE_PIN, Bit_RESET);
			break;
		}

	}
}
void var_clear(int sel)
{
    if ((sel & (1<<DELAY_FLAG)) != 0)
    {
        kick.one_time_flag = true;
    }
    if ((sel & (1<<WHEEL_COUNTER_COUNT)) != 0)
    {
        wheel_counter.count_left = 0;
        wheel_counter.count_right = 0;
    }

    if ((sel & (1<<TRACK_CLEAR_ALL)) != 0)
    {
				track.car_x_diff = 0;
				track.car_y_diff = 0;
				track.path_x_diff = 0;
				track.path_y_diff = 0;
				track.car_k = 0;
				track.path_k = 0;
				track.path_angle = 0;
				track.car_angle = 0;
        track.p_error = 0;
        track.i_error = 0;
        track.d_error = 0;
        track.update_speed_left = 0;
        track.update_speed_right = 0;
    }

}
void PID_track(void)
{
	if (SWITCH_State == 3 || SWITCH_State == 6)
	{
		update_path();
		track.d_error = track.car_angle - track.path_angle - track.p_error;
		track.p_error = track.car_angle - track.path_angle;
		track.i_error += track.p_error;
		
		track.update_speed_left = (int)(40 * (track.base_speed - (track.kp * track.p_error + track.ki * track.i_error + track.kd * track.d_error)));
		track.update_speed_right = (int)(40 * (track.base_speed + (track.kp * track.p_error + track.ki * track.i_error + track.kd * track.d_error)));
		
		if (SWITCH_State == 3)
		{
			PWM_Update(1, 0, track.update_speed_left);
			PWM_Update(2, 0, track.update_speed_right);
		}
		else if (SWITCH_State == 6)
		{
			PWM_Update(1, 0, track.update_speed_right);
			PWM_Update(2, 0, track.update_speed_left);
		}
	}
}

void update_path(void)
{
	track.car_x_diff = (double)(object[car].X - object_past[car].X);
	track.car_y_diff = (double)(object[car].Y - object_past[car].Y);
	track.path_x_diff = (double)(object[(obj)kick.ball_current].X - object[car].X);
	track.path_y_diff = (double)(object[(obj)kick.ball_current].Y - object[car].Y);
	
	if (track.car_x_diff != 0 && track.car_y_diff != 0)
	{
		if (track.car_x_diff != 0)
			track.car_k = track.car_y_diff / track.car_x_diff;
		if (track.path_x_diff != 0)
			track.path_k = track.path_y_diff / track.path_x_diff;
	
		k_to_deg(track.car_x_diff, track.car_y_diff, track.car_k, &track.car_angle);
		k_to_deg(track.path_x_diff, track.path_y_diff, track.path_k, &track.path_angle);
		
		if (track.car_angle > 270 && track.path_angle < 90)
			track.car_angle -= 360;
		if (track.path_angle > 270 && track.car_angle < 90)
			track.path_angle -= 360;
		
		object_past[car].X = object[car].X;
		object_past[car].Y = object[car].Y;
	}
}

void update_control(void)
{
	if (dist > 500)
		track.kp = 0.4 * (SWITCH_State == 3) + 0.2 * (SWITCH_State == 6) ;
	else
		track.kp = ((float)(dist)/1250) * (SWITCH_State == 3) + ((float)(dist)/3000) * (SWITCH_State == 6);

}
void k_to_deg(double x_diff, double y_diff, double k , volatile double* angle)	
{
	//	Scenario 4, 2, 1 & 3
	if (x_diff == 0)
		*angle = 270 * (y_diff < 0) + 90 * (y_diff > 0);
	else if (y_diff == 0)
		*angle = 180 * (x_diff < 0);
	else if (x_diff > 0 && y_diff > 0)
		*angle = (atan(k) * 180) / (3.1415);
	else if (x_diff > 0)
		*angle = 360 + (atan(k) * 180) / (3.1415);
	else
		*angle = 180 + (atan(k) * 180) / (3.1415);
}
