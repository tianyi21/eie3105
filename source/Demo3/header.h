//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#ifndef _HEADER_H
#define _HEADER_H

#include "stm32f10x.h"                  // Device header
#include "pin_map.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "math.h"

//	enum
typedef enum{car, ball_1, ball_2 = 3, ball_3 = 5} obj;

//	Structure
typedef struct loc_struct
{
	volatile int X;					//	x	axis
	volatile int Y;					//	y axis
}loc_struct;

typedef struct loc_Update_struct
{
	volatile obj object;			//	obj enum variable
	volatile int X;					//	new x axis
	volatile int Y;					//	new y axis
}loc_Update_struct;

typedef struct track_struct
{
	volatile double car_x_diff;
	volatile double car_y_diff;
	volatile double path_x_diff;
	volatile double path_y_diff;
	volatile double car_k;
	volatile double path_k;
	volatile double path_angle;
	volatile double car_angle;
	
	volatile float kp;
	volatile float ki;
	volatile float kd;
	volatile int base_speed;
	volatile double p_error;
	volatile double i_error;
	volatile double d_error;
	volatile int thres;
	volatile int update_speed_left;
	volatile int update_speed_right;
	volatile int TIM_flag;
	

}track_struct;

typedef struct wifi_struct
{
	char Tag_Car[4];
	char Tag_Ball_1[4];
	char Tag_Ball_2[4];
	char Tag_Ball_3[4];
	char Tag_End[4];
	char Rcv_Temp[4];
	volatile char Rcv;
	volatile int Rcv_Count;
	volatile int Calc;
	volatile int Rcv_enable;
	volatile int Flag_USART_Info;
	volatile int Flag_X;
	volatile int Flag_Y;
	int set;
}wifi_struct;

typedef struct wheel_counter_struct
{
	volatile int count_left;
	volatile int count_right;
}wheel_counter_struct;

typedef struct kick_struct
{
	volatile int state;
	volatile int ball_current;
	volatile int one_time_flag;
}kick_struct;



//	Function Prototype

//	Init_func_sets.c
void SysTick_Init(void);
void Struct_Init(void);
void WHEEL_PWM_Init(void);
void WHEEL_COUNTER_EXTI_Init(void);
void SWITCH_Init(void);
void LED_Init(void);
void USART2_Init(void);
void TIM2_Init(void);
void TIM4_Init(void);

//	USART_func_sets.c
void USART_Send_Char(unsigned char);
void USART_Send_Int(int);
void USART_Send_String(char*, unsigned long);

//	Struct_func_sets.c
loc_struct read_loc(obj);
void clear_loc(obj);
void update_loc(loc_Update_struct);
loc_struct cal_diff(obj, obj);
int cal_dist(obj, obj);

//	Delay_func_sets.c
void DelayMs(int ms);
void SysTick_Handler(void);
void one_time_delay(int duration);

//	WiFi_func_sets.c
void get_loc(void);
void WiFi_Connect(void);

//	Debug_func_sets.c
void DEBUG_Init(void);
void DEBUG_Send_Char(unsigned char);
void DEBUG_Send_Int(int);
void DEBUG_Send_String(char*, unsigned long);

//	Service_func_sets.c
void PWM_Update(int, int, int);
void var_clear(int);
void PID_track(void);
void update_path(void);
void update_control(void);
void k_to_deg(double, double, double, volatile double*);

#endif
