#include "stm32f10x.h"                  // Device header
#include "pin_map.h"

void TIM4_CH1_IC1_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	// Configure I/O for IC1
	GPIO_InitStruct.GPIO_Pin = TIM4_CH1_IT1_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM4_CH1_IT1_GPIO, &GPIO_InitStruct);
	
	//Tim4 set up 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 
  TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 720-1;  //1/(72Mhz/720)=0.01ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 50000-1;  //0.01ms*50000 = 500ms
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &timerInitStructure);
  TIM_Cmd(TIM4, ENABLE);
	
	//Enable Tim4 Ch1 Input Capture
	TIM_ICInitTypeDef InputCaptureInitStructure;
	InputCaptureInitStructure.TIM_Channel = TIM_Channel_1;  //Seclect IC1
	InputCaptureInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //Capture rising
	InputCaptureInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Map to TI1
	InputCaptureInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //Configure input frequency
	InputCaptureInitStructure.TIM_ICFilter = 0; //no filter
	TIM_ICInit(TIM4, &InputCaptureInitStructure);
	
	//Enable Input Capture Interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;    //TIM4 interrupt
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //Preemptive priority level 2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //From the priority level 0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //The IRQ channel is enabled
  NVIC_Init(&NVIC_InitStructure);
    
  TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE); //Allow updates to interrupt, allows the CC1IE to capture interrupt
	
}


void TIM3_CH1_PWM_Init(void) {
	RCC_APB2PeriphClockCmd(TIM3_CH1_PWM_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure I/O for Tim3 Ch1 PWM pin
	GPIO_InitStructure.GPIO_Pin = TIM3_CH1_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM3_CH1_PWM_GPIO, &GPIO_InitStructure);
	
	//Tim3 set up 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
  TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 720 - 1;  //1/(72Mhz/720)=0.01ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 200 - 1;  
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_Cmd(TIM3, ENABLE);
	
	//Enable Tim3 Ch1 PWM
	TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 100 - 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
}


void USART2_Init(void) {
	//USART2 TX RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART2 ST-LINK USB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStructure; 
	
	USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStr;
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_InitStr.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStr.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStr.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStr);
}
	
void USARTSend(char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
        USART_SendData(USART2, *pucBuffer++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
					;
        }
    }
}