//	EIE3105 Quiz 1 Version A Question 2
// 	Written by 15102892d LIU Tianyi
//	Feb 12th, 2019

#include "stm32f10x.h"                  // Device header
#include "PinMap.h"
#include "stdio.h"
#include "misc.h"

volatile uint32_t status = 0;
extern volatile uint16_t ADC_values[ARRAYSIZE];
int thres = 4000;
int state = 0;
char dim[50] = "DIM\r\n";
char bright[50] = "BRIGHT\r\n";

int main(void) {
	
	char buffer[50] = {'\0'};
	
	
	USART2_init();
	ADC1_3channels_init();
	DMA1_init();
	
	//Enable DMA1 Channel transfer
	// DMA_Cmd(DMA1_Channel1, ENABLE);
	// start conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion (will be endless as we are in continuous mode)
	
	while(1) {
		while(!status);
		//sprintf(buffer, "ch0=%d\r\n", ADC_values[0]);
		//USARTSend(buffer, sizeof(buffer));
		TIM3->CCR1 = ADC_values[0];
		if (ADC_values[0] < thres)
			USARTSend(dim, sizeof(dim));
		else if (ADC_values[0] >= thres)
			USARTSend(bright, sizeof(bright));
		
		status = 0;
		//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	}
}

void DMA1_Channel1_IRQHandler(void)
{
  //Test on DMA1 Channel1 Transfer Complete interrupt
  if(DMA_GetITStatus(DMA1_IT_TC1))
  {
	  status=1;
		
   //Clear DMA1 interrupt pending bits
    DMA_ClearITPendingBit(DMA1_IT_GL1);
  }
}
