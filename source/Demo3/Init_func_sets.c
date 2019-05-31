//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

//	Declare in main.c
extern struct loc_struct object[6];
extern struct loc_struct object_past[6];
extern struct loc_Update_struct loc_Update;
extern struct track_struct track;
extern struct track_struct finish;
extern struct wifi_struct wifi;
extern struct wheel_counter_struct wheel_counter;
extern struct kick_struct kick;

char def_Car[4] = "CYW";
char def_Ball_1[4] = "BBE";
char def_Ball_2[4] = "BOE";
char def_Ball_3[4] = "BRD";
char def_End[4] = "CMD";

void SysTick_Init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	//NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 2, 0);
	NVIC_SetPriority(SysTick_IRQn, 2);
}
void Struct_Init(void)
{
	//	Obj Structure
	for (int i = 0; i < 6; i++)
	{
		if (i == 2 || i == 4)
		{
			object[i].X = 750;
			object[i].Y = 300;
			object_past[i].X = 650;
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
	//	Track Structure
	track.car_x_diff = 0;
	track.car_y_diff = 0;
	track.path_x_diff = 0;
	track.path_y_diff = 0;
	track.car_k = 0;
	track.path_k = 0;
	track.path_angle = 0;
	track.car_angle = 0;
	
	track.base_speed = 0;
	track.kp = 0.3;
	track.ki = 0.1;
	track.kd = 0;
	track.base_speed = 40;
	track.p_error = 0;
	track.i_error = 0;
	track.d_error = 0;
	track.update_speed_left = 0;
	track.update_speed_right = 0;
	track.thres = 25;
	track.TIM_flag = 0;
	
	finish.car_x_diff = 0;
	finish.car_y_diff = 0;
	finish.path_x_diff = 0;
	finish.path_y_diff = 0;
	finish.car_k = 0;
	finish.path_k = 0;
	finish.path_angle = 0;
	finish.car_angle = 0;
	
	finish.base_speed = 0;
	finish.kp = 0.3;
	finish.ki = 0;
	finish.kd = 0;
	finish.base_speed = 30;
	finish.p_error = 0;
	finish.i_error = 0;
	finish.d_error = 0;
	finish.thres = 25;
	finish.update_speed_left = 0;
	finish.update_speed_right = 0;
	finish.TIM_flag = 0;

	//	WiFi Structure
	for (int i = 0; i < 4; i++)
	{
		if (i != 3)
		{
			wifi.Tag_Car[i] = def_Car[i];
			wifi.Tag_Ball_1[i] = def_Ball_1[i];
			wifi.Tag_Ball_2[i] = def_Ball_2[i];
			wifi.Tag_Ball_3[i] = def_Ball_3[i];
			wifi.Tag_End[i] = def_End[i];
			wifi.Rcv_Temp[i] = 0;
		}
		else
		{
			wifi.Tag_Car[i] = 0;
			wifi.Tag_Ball_1[i] = 0;
			wifi.Tag_Ball_2[i] = 0;
			wifi.Tag_Ball_3[i] = 0;
			wifi.Tag_End[i] = 0;
			wifi.Rcv_Temp[i] = 0;
		}
	}
	wifi.Rcv = 0;
	wifi.Rcv_Count = 0;
	wifi.Calc = 0;
	wifi.Rcv_enable = 1;
	wifi.Flag_USART_Info = 0;
	wifi.Flag_X = 0;
	wifi.Flag_Y = 0;
	//	Wheel Counter Structure
	wheel_counter.count_left = 0;
	wheel_counter.count_right = 0;
	//	Kick Structure
	kick.state = 0;
	kick.ball_current = 0;
	kick.one_time_flag = 1;
}
void WHEEL_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(LEFT_WHEEL_PWM_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RIGHT_WHEEL_PWM_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(LEFTPHASE_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RIGHTPHASE_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(SERVO_PWM_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//	GPIO Enable
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LEFT_WHEEL_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LEFT_WHEEL_PWM_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RIGHT_WHEEL_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(RIGHT_WHEEL_PWM_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LEFTPHASE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LEFTPHASE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RIGHTPHASE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(RIGHTPHASE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SERVO_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(SERVO_PWM_GPIO, &GPIO_InitStructure);
	
	GPIO_WriteBit(LEFTPHASE_GPIO, LEFTPHASE_PIN, Bit_SET);
	GPIO_WriteBit(RIGHTPHASE_GPIO, RIGHTPHASE_PIN, Bit_SET);
	
	//	TIM3 Enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 36 - 1;	//	1	/	(72M/360) = 0.02 ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 4000 - 1;  	//	0.02 ms * 100 ~ 500 Hz
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_Cmd(TIM3, ENABLE);
	
	//	PWM
	TIM_OCInitTypeDef outputChannelInit;
	
	//	LEFT_WHEEL PWM Enable 
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1 - 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//	RIGHT_WHEEL PWM Enable 
 	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1 - 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
	
	//	Servo PWM Enable
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1 - 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &outputChannelInit);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
}
void WHEEL_COUNTER_EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Configure I/O for EXTI13
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// EXTI Configuration
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// Enable Interrupt
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
		// Configure I/O for EXTI13
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// EXTI Configuration
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// Enable Interrupt
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}
void SWITCH_Init(void)
{
	RCC_APB2PeriphClockCmd(S1_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	// Enable GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = S1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(S1_GPIO, &GPIO_InitStruct);

	// EXTI Configuration
	GPIO_EXTILineConfig(S1_GPIO_PORTSOURCE, S1_GPIO_PINSOURCE);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = S1_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// Enable EXTI
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}
void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(LED_RCC_GPIO, ENABLE);

    //	GPIO Enable
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
}
void USART2_Init()
{
    //	USART2 TX RX
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //	USART2 ST-LINK USB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

    //	RX Interrupt
    /*NVIC_InitTypeDef NVIC_InitStructure;
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);*/
}

void TIM2_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 18000;  //1/(72Mhz/18000)=0.25ms
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 600;  // 200 ms
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    //TIM_Cmd(TIM2, ENABLE);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
    
}
void TIM4_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 18000;  //1/(72Mhz/18000)=0.25ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 400;  // 150 ms
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &timerInitStructure);
  TIM_Cmd(TIM4, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
