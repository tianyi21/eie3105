//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#ifndef _PINMAP_H
#define _PINMAP_H

//	Pin Map
// 	PA6 	- Tim3 Ch1 PWM 		- Right Wheel		~	WHEEL_PWM_Init()
// 	PA7 	- Tim3 Ch2 PWM		-	Left Wheel		~	WHEEL_PWM_Init()
//	PC15	- RightPHASE 			-	Right Wheel 	~	WHEEL_PWM_Init()
//	PA0		- LeftPHASE				-	Left Wheel 		~	WHEEL_PWM_Init()
//  PB6 	- Tim4 Ch1 				- Left Wheel		~ WHEEL_COUNTER_Init()
//	PA1 	- Tim2 Ch2 				- Right Wheel		~ WHEEL_COUNTER_Init()
//*	PB0		-	Tim3 Ch3 PWM		-	Servo PWM			~ WHEEL_PWM_Init()
//	PB12	-	SPI2 NSS				-	SPI2					~	SPI2_Init()
//	PB13	-	SPI2 SCK				-	SPI2					~	SPI2_Init()
//	PB14	-	SPI2 MISO				-	SPI2					~	SPI2_Init()
//	PB15	-	SPI2 MOSI				-	SPI2					~	SPI2_Init()
// 	PB8 	- Switch EXTI			- S1(EXTI)			~	SWITCH_Init()
//	PB7		-	LED							-	RA0						~	LED_Init()
//	PA2		- USART2 (WiFi)		-	USART2 TX			~	USART2_Init()
//	PA3		- USART2 (WiFi)		-	USART2 RX			~	USART2_Init()

//	Alias
//	PA6		-	Tim3 Ch1 PWM		- Left Wheel		~	WHEEL_PWM_Init()
#define RIGHT_WHEEL_PWM_RCC_GPIO  			RCC_APB2Periph_GPIOA
#define RIGHT_WHEEL_PWM_GPIO      			GPIOA
#define RIGHT_WHEEL_PWM_PIN       			GPIO_Pin_6
// 	PA7 	- Tim3 Ch2 PWM		-	Right Wheel		~	WHEEL_PWM_Init()
#define LEFT_WHEEL_PWM_RCC_GPIO  		RCC_APB2Periph_GPIOA
#define LEFT_WHEEL_PWM_GPIO      		GPIOA
#define LEFT_WHEEL_PWM_PIN       		GPIO_Pin_7
//	PC15	-	LeftPHASE				- Left Wheel		~	WHEEL_PWM_Init()
#define RIGHTPHASE_RCC_GPIO  					RCC_APB2Periph_GPIOC
#define RIGHTPHASE_GPIO    						GPIOC
#define RIGHTPHASE_PIN       					GPIO_Pin_15
// 	PA0 	- RightPHASE			-	Right Wheel		~	WHEEL_PWM_Init()
#define LEFTPHASE_RCC_GPIO  					RCC_APB2Periph_GPIOA
#define LEFTPHASE_GPIO      					GPIOA
#define LEFTPHASE_PIN       					GPIO_Pin_0
//	PB6 	- Tim4 Ch1 				- Left Wheel		~	WHEEL_COUNTER_Init()
#define LEFT_WHEEL_COUNTER_RCC_GPIO  	RCC_APB2Periph_GPIOB
#define LEFT_WHEEL_COUNTER_GPIO      	GPIOB
#define LEFT_WHEEL_COUNTER_PIN       	GPIO_Pin_6
//	PA1 	- Tim2 Ch2 				- Right Wheel		~ WHEEL_COUNTER_Init()
#define RIGHT_WHEEL_COUNTER_RCC_GPIO  RCC_APB2Periph_GPIOA
#define RIGHT_WHEEL_COUNTER_GPIO      GPIOA
#define RIGHT_WHEEL_COUNTER_PIN       GPIO_Pin_1
//	PB0 	- Tim3 Ch3 PWM 		- Servo PWM			~ WHEEL_PWM_Init()
#define SERVO_PWM_RCC_GPIO  					RCC_APB2Periph_GPIOB
#define SERVO_PWM_GPIO      					GPIOB
#define SERVO_PWM_PIN       					GPIO_Pin_0
//	PB8		- Switch EXTI			-	S1(EXTI)			~ SWITCH_Init()
#define S1_RCC_GPIO        						RCC_APB2Periph_GPIOB
#define S1_GPIO	           						GPIOB
#define S1_GPIO_PIN        						GPIO_Pin_8
#define S1_EXTI_LINE       						EXTI_Line8
#define S1_GPIO_PORTSOURCE 						GPIO_PortSourceGPIOB
#define S1_GPIO_PINSOURCE  						GPIO_PinSource8
//	PB7		-	LED							-	RA0						~	LED_Init()
#define LED_RCC_GPIO  								RCC_APB2Periph_GPIOB
#define LED_GPIO      								GPIOB
#define LED_PIN       								GPIO_Pin_7

//	Function Prototype
void WHEEL_PWM_Init(void);
void PWM_Update(int, int, int);
void WHEEL_COUNTER_Init(void);
void TIM2_Init(void);
void SWITCH_Init(void);
void LED_Init(void);
void SPI2_Init(void);
void SPI2_Detect(void);
void USART2_Init(void);
void USART_Send_Char(unsigned char ch);
void USART_Send_Int(int num);
void USART_Send_String(char*, unsigned long);

void update_base_speed(int input);
void update_TIM_Count(int input);
int read_TIM_Count(void);
#endif
