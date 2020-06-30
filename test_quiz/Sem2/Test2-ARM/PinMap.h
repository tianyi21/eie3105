#ifndef _PINMAP_H
#define _PINMAP_H

//Usage
// PA0 - ADC1_IN0
// PA1 - ADC1_IN1
// PA2 - USART2 TX
// PA3 - USART2 RX
// PA4 - ADC1_IN4
// PA5 - LD2
// PA6 - Tim3 Ch1 PWM
// PA7 - Tim3 Ch2 PWM
// PB0 - Tim3 Ch3 PWM
// PC13 - B1

//ADC1_0 PA0
#define ADC1_0_RCC_GPIO  RCC_APB2Periph_GPIOA
#define ADC1_0_GPIO      GPIOA
#define ADC1_0_PIN       GPIO_Pin_0

void ADC1_3channels_init(void);
void DMA1_init(void);
#define ARRAYSIZE 3
#define ADC1_DR    ((uint32_t)0x4001244C)

void USART2_init(void);
void USARTSend(char *pucBuffer, unsigned long ulCount);

#endif
