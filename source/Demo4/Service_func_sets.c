//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

//	Declare in main.c
extern struct loc_struct object[4];
extern struct loc_struct object_past[4];
extern struct loc_Update_struct loc_Update;
extern struct track_struct track;
extern struct track_struct finish;
extern struct kick_struct kick;

void kick_main(void)
{
	switch (kick.state)
	{
		case BALL_LOC:
		{
			//	Clear
			//var_clear(0xFF);
			//	Ball -> TARGET
			kick.ball_current = TARGET;
			//	Stop
			PWM_Update(0, 1, 0);
			//	If Ball in region && Detected
			if ((object[BALL].X < kick.ball_thres) && object[BALL].X != 0 && object[BALL].Y != 0)
				kick.state = BALL_WAIT;
			break;
		}
		case BALL_WAIT:
		{
			//	Stop
			PWM_Update(0, 1, 0);
			//	Polling for Ball Stop
			loc_struct ball_pos_1 = object[BALL];
			one_time_delay(200);
			loc_struct ball_pos_2 = object[BALL];
			if (cal_dist_struct(ball_pos_1, ball_pos_2) < 2)
				kick.state = FORWARD_INIT;
			break;
		}
		case FORWARD_INIT:
		{
			//	Clear Var
			var_clear((1<<TRACK_CLEAR_ALL));
			//	Stop
			PWM_Update(0, 1, 0);
			//	Update Overshoot
			update_target();
			//	Enable TIM2 (PID)
			TIM_Cmd(TIM2, ENABLE);
			//	Update Car Init loc
			object_past[CAR].X = object[CAR].X;
			object_past[CAR].Y = object[CAR].Y;
			//	Init
			PWM_Update(0, 1, 1500);
			//	Delay
			one_time_delay(100);
			kick.state = FORWARD;
		}
		case FORWARD:
		{
			break;
		}
		case ROTATE:
		{
			//	Stop
			PWM_Update(0, 0, 0);
			one_time_delay(200);
			//	Update
			kick.ball_current = BASE;
			track.base_speed = 40;
			//	Rotate
			if (track.overshoot < 0)
			{
				PWM_Update(1, 1, 1500);
				PWM_Update(2, 2, 1500);
			}
			else
			{
				PWM_Update(1, 2, 1500);
				PWM_Update(2, 1, 1500);
			}
			var_clear((1<<DELAY_FLAG));
			one_time_delay(500);
			//	Next
			kick.state = FORWARD_INIT;
			break;
		}
		case FINISH:
		{
			//	Stop
			PWM_Update(0, 0, 0);
			//	Delay
			one_time_delay(100);
			//	Rotate
			if (track.car_angle > 180)
			{	
				//	LR -> UL
				finish.p_error = 360 - track.car_angle;
				PWM_Update(1, 1, 1500);
				PWM_Update(2, 2, 1500);
			}
			else
			{
				//	UR -> LL
				finish.p_error = track.car_angle;
				PWM_Update(1, 2, 1500);
				PWM_Update(2, 1, 1500);
			}
			//	Delay ~ Roughly to 0 deg.
			one_time_delay((int) (3.5 * finish.p_error));
			//	Back and Clear
			PWM_Update(0, 2, 1000);
			one_time_delay(400);
			kick.state = BALL_LOC;
			var_clear(0xFF);
			break;
		}
	}		
}	
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
void var_clear(char sel)
{
    if ((sel & (1<<DELAY_FLAG)) != 0)
    {
			kick.one_time_flag = 1;
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

			track.kp = 0.4;
			track.ki = 0.1;
			track.kd = 0;
			track.base_speed = 40;
			track.p_error = 0;
			track.i_error = 0;
			track.d_error = 0;
			track.thres = 30;
			track.update_speed_left = 0;
			track.update_speed_right = 0;
			
			track.overshoot = 0;
			track.overhead = 0;
			track.anchor_x = 0;
			track.anchor_y = 0;
    }
		if ((sel & (1<<KICK_CLEAR_ALL)) != 0)
		{
			kick.state = BALL_LOC;
			kick.dist = 0;
			kick.ball_thres = 500;
			kick.ball_current = TARGET;
			kick.one_time_flag = 1;
		}
		if ((sel & (1<<FINISH_CLEAR_ALL)) != 0)
		{
			finish.car_x_diff = 0;
			finish.car_y_diff = 0;
			finish.path_x_diff = 0;
			finish.path_y_diff = 0;
			finish.car_k = 0;
			finish.path_k = 0;
			finish.path_angle = 0;
			finish.car_angle = 0;
	
			finish.kp = 0.4;
			finish.ki = 0.1;
			finish.kd = 0;
			finish.base_speed = 40;
			finish.p_error = 0;
			finish.i_error = 0;
			finish.d_error = 0;
			finish.thres = 25;
			finish.update_speed_left = 0;
			finish.update_speed_right = 0;
			
			finish.overshoot = 0;
			finish.anchor_x = 0;
			finish.anchor_y = 0;
		}

		if ((sel & (1<<OBJECT_STRUCT)) != 0)
		{
			for (int i = 0; i < 4; i++)
			{
				if (i == BASE)
				{
					object[i].X = 50;
					object[i].Y = 300;
					object_past[i].X = 50;
					object_past[i].Y = 300;
				}
				else 
				{
					object[i].X = 0;
					object[i].Y = 0;
					object_past[i].X = 0;
					object_past[i].Y = 0;
				}
			}
		}
}
void PID_track(void)
{
	update_path();
	update_control();
	track.d_error = track.car_angle - track.path_angle - track.p_error;
	track.p_error = track.car_angle - track.path_angle;
	track.i_error += track.p_error;
		
	track.update_speed_left = (int)(40 * (track.base_speed - (track.kp * track.p_error + track.ki * track.i_error + track.kd * track.d_error)));
	track.update_speed_right = (int)(40 * (track.base_speed + (track.kp * track.p_error + track.ki * track.i_error + track.kd * track.d_error)));

	if (track.update_speed_left >= 0 && track.update_speed_right >= 0)
	{
		PWM_Update(1, 0, track.update_speed_left);
		PWM_Update(2, 0, track.update_speed_right);
	}
}

void update_path(void)
{
	track.car_x_diff = (double)(object[CAR].X - object_past[CAR].X);
	track.car_y_diff = (double)(object[CAR].Y - object_past[CAR].Y);
	track.path_x_diff = (double)(object[(obj)kick.ball_current].X - object[CAR].X);
	track.path_y_diff = (double)(object[(obj)kick.ball_current].Y - object[CAR].Y);
	
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
		
		object_past[CAR].X = object[CAR].X;
		object_past[CAR].Y = object[CAR].Y;
	}
}

void update_control(void)
{
	if (kick.dist > 500)
	{		
		//	Saturate
		track.kp = 0.4;
		track.ki = 0.1;
	}
	else
	{
		//	Linear mapping
		track.kp = (float)(kick.dist)/1250;
		track.ki = (float)(kick.dist)/4000;
	}
	if (kick.ball_current == TARGET)
	{
		if (kick.dist > 100)
			track.base_speed = 30;
		else 
			track.base_speed = 60 + abs((int)(track.anchor_y / 2)) + (int)track.overhead;	
	}
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
void update_target()
{
	if (kick.ball_current == TARGET)
	{	
		track.overhead = object[BALL].X - 200; 
		track.overshoot = object[BALL].Y - object[CAR].Y;
	}
	else if (kick.ball_current == BASE)
		track.overshoot = object[BASE].Y - object[CAR].Y;
	track.anchor_x = 0;
	track.anchor_y = (int)(track.overshoot/10);
  track.overhead = 20 - (track.overhead / 10);
	if (kick.ball_current == BASE)
		object[BASE].Y = 300 + track.anchor_y;///////////*******//////////////
	
}
