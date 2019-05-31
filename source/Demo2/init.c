//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "stm32f10x.h"                  // Device header
#include "pin_map.h"
#include "stdio.h"
#include "stdbool.h"

volatile unsigned char spi_detect = '\0';
volatile unsigned char spi_detect_0 = '\0';
volatile unsigned char spi_detect_past = '\0';
volatile int pos = 0;
volatile int error_p = 0;
volatile int error_i = 0;
volatile int error_d = 0;
volatile int error_past = 0;
volatile int kp = 14;
volatile int ki = 0;
volatile int kd = 0;
volatile int TIM_Count = 0;
volatile int base_speed = 75;
volatile int update_speed_left = 0;
volatile int update_speed_right = 0;
volatile int black_count = -1;
volatile bool flag = true;

void update_base_speed(int input)
{
	base_speed = input;
}

void update_TIM_Count(int input)
{
	TIM_Count = input;
}

int read_TIM_Count()
{
	return TIM_Count;
}

//	LEFT RIGHT WHEEL PWM ~ TIM3
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
  timerInitStructure.TIM_Prescaler = 360 - 1;	//	1	/	(72M/360) = 0.02 ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 100 - 1;  	//	0.02 ms * 100 ~ 500 Hz
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



//	Update PWM
void PWM_Update(int whl_sel, int whl_dir, int whl_pwm)
{
	/*	Usage
	//		whl_sel: Wheel Selection
	//			0: Left + Right Wheel
	//			1: Left Wheel
	//			2: Right Wheel
	//
	//		whl_dir: Wheel Direction
	//			0: No Change
	//			1: Forward 
	//			2: Backward
	//		
	//		whl_pwm: New PWM (DC%)
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

//	WHEEL COUNTER 			~	TIM2 TIM4	
void WHEEL_COUNTER_Init(void)
{
	//	TIM2, TIM4 Set Up
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
  TIM_TimeBaseInitTypeDef timerInitStructure; 
	timerInitStructure.TIM_Prescaler = 0;
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 5000; 
  timerInitStructure.TIM_ClockDivision = 0;
  timerInitStructure.TIM_RepetitionCounter = 0;
 	TIM_TimeBaseInit(TIM2, &timerInitStructure);
  TIM_Cmd(TIM2, ENABLE);
	
	timerInitStructure.TIM_Prescaler = 0;
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 5000; 
  timerInitStructure.TIM_ClockDivision = 0;
  timerInitStructure.TIM_RepetitionCounter = 0;
 	TIM_TimeBaseInit(TIM4, &timerInitStructure);
  TIM_Cmd(TIM4, ENABLE);
	
	TIM_TIxExternalClockConfig(TIM2, TIM_TIxExternalCLK1Source_TI2, TIM_ICPolarity_Rising, 0);
	TIM_TIxExternalClockConfig(TIM4, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
	
	//	TIM2, TIM4 Enable
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	//	TIM2, TIM4 Interrupt
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM4_IRQn);
}
//	TIM2	EXTI
void TIM2_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 
  TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 18000;  //1/(72Mhz/18000)=0.25ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 2000;  //0.25ms*4000 = 1s
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &timerInitStructure);
  TIM_Cmd(TIM2, ENABLE);
	
	//Enable update event for Timer2
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
}
//	SWITCH S1 EXTI
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
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
}
//	SPI2 Init
void SPI2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//	SPI Configuration
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	//	Enable
	SPI_Cmd(SPI2, ENABLE);

}
//	SPI2 Detect
void SPI2_Detect(void)
{
	while((SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)) == RESET);
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
	SPI_I2S_SendData(SPI2,'A');
	while((SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)) == RESET);
	spi_detect_0 = SPI_I2S_ReceiveData(SPI2);
	SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_FLAG_RXNE);
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
	SPI_I2S_SendData(SPI2,'A');
	while((SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)) == RESET);
	spi_detect = SPI_I2S_ReceiveData(SPI2);
	SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_FLAG_RXNE);
	
	if (spi_detect == 0 & spi_detect_past != 0)
	{
		GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_SET);
		
		//	Past error
		black_count++;
//		update_speed_left = base_speed + (int)(kp/8) * error_past;
//		update_speed_right = base_speed - (int)(kp/8) * error_past;
		update_speed_left = base_speed + kp * error_past;
		update_speed_right = base_speed - kp * error_past;
		if (black_count == 5)
		{	
			update_speed_left = 100;
			update_speed_right = 42;
			for (int i = 0; i < 2000; i++)
				for (int j = 0; j < 800; j++)
					;
		}
	}
	else 
	{
		GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_RESET);
		
		//	Update Error
			//	Left Alignment
		if (black_count == 5)
		{
			PWM_Update(1, 1, 50);
			PWM_Update(2, 2, 50);
			for (int i = 0; i < 2000; i++)
				for (int j = 0; j < 2700; j++)
				{
					;
				}
			black_count++;
		}
		else if (black_count != 4 && black_count != 6 & black_count != 7 & black_count != 8)
		{
			GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_RESET);
			for(pos = 0; pos < 8; pos++)
			{
				if((spi_detect & (1 << pos)) == 0)
				{
					error_p = pos - 3;
					break;
				}
			}
			if (error_p != error_past)
			{
				error_past = error_p;
			}
		}
		//	Right Alignment
		else 
		{
			GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_SET);
			for(pos = 7; pos > -1; pos--)
			{
				if((spi_detect & (1 << pos)) == 0)
				{
					error_p = pos - 4;
					break;
				}
			}
			if (error_p != error_past)
			{
				error_past = error_p;
			}
		}
		update_speed_left = base_speed + kp * error_past;
		update_speed_right = base_speed - kp * error_past;
	}
	PWM_Update(1, 1, update_speed_left);
	PWM_Update(2, 1, update_speed_right);
	spi_detect_past = spi_detect;

}
//	LED
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

//	USART2 Function Sets
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
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}



void USART_Send_Char(unsigned char ch)
{
	int int_ch = (int)ch;
	char buffer[20] = "";
	sprintf(buffer, "%d\r\n", int_ch);
	USART_Send_String(buffer, sizeof(buffer));
}

void USART_Send_Int(int num)
{
	char buffer[20] = "";
	sprintf(buffer, "%d\r\n",num);
	USART_Send_String(buffer, sizeof(buffer));
}

//	USART2 Send String
void USART_Send_String(char* pucBuffer, unsigned long ulCount)
{
	/*	Usage
	//		sprintf(buffer, "ch0=%d ch1=%d ch4=%d\r\n", ADC_values[0], ADC_values[1], ADC_values[2]);
	//		USART_Send(buffer, sizeof(buffer));
	*/
	
	while(ulCount--)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, *pucBuffer++);
	}
}
