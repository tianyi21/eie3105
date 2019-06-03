/*
 * EIE3105_Lab1_G.c
 *
 * Created: 2019-01-25 5:12:45 PM
 * Author : Bao
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

int result_0 = 0;
int result_1 = 0;
int result_2 = 0;
int thres = 800;
volatile unsigned char result_l_0 = '\0';
volatile unsigned char result_h_0 = '\0';
volatile unsigned char result_l_1 = '\0';
volatile unsigned char result_h_1 = '\0';
volatile unsigned char result_l_2 = '\0';
volatile unsigned char result_h_2 = '\0';
unsigned int flag_adc_done = 0;
unsigned char result[10] = "";
unsigned int loc = 0;
unsigned int ch = 0;
unsigned int state_0 = 1;
unsigned int state_1 = 1;
unsigned int state_2 = 1;

void io_init()
{
	DDRC = 0x00;
	PORTC = 0xFF;
	DDRD |= (1<<DDD5) |	//	PD5 PWM OC0B 
			(1<<DDD3);		//	PD3 PWM OC2B
	DDRB |= (1<<DDD2);		//	PB2 PWM	OC1B
}

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0x67;
}

void adc_init()
{
	ADMUX = 0;				//	ext Vref, ADC0
	ADCSRA |= (1<<ADIE) | (1<<ADEN) |  (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	//	enable, CLK/128
	//PRR &= (~(1));
	ADCSRA |= (1<<ADSC);
}

void pwm_init()
{
	//	Timer 0
	TCCR0A = (1<<WGM01) | (1<<WGM00) |
			(1<<COM0A1) | (0<<COM0A0) |
			(1<<COM0B1) | (0<<COM0B0);
	TCCR0B = (1<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);
	OCR0A = 124;
	OCR0B = 0;
	
	//	Timer 1
	TCCR1A = (1<<WGM11) | (1<<WGM10) |
			(1<<COM1A1) | (0<<COM1A0) |
			(1<<COM1B1) | (0<<COM1B0);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	OCR1A = 124;
	OCR1B = 0;
	
	//	Timer 2
	TCCR2A = (1<<WGM21) | (1<<WGM20) |
			(1<<COM2A1) | (0<<COM2A0) |
			(1<<COM2B1) | (0<<COM2B0);
	TCCR2B = (1<<WGM22) | (1<<CS22) | (1<<CS21) | (0<<CS20); 
	OCR2A = 124;
	OCR2B = 0;
}

void pwm_update(unsigned int ch, unsigned int pwm_val)
{
	if (ch == 0)
		OCR0B = pwm_val;
	else if (ch == 1)
		OCR1B = pwm_val;
	else if (ch == 2)
		OCR2B = pwm_val;
}

void USART_send(unsigned char data)
{	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_send(*StringPtr);
		StringPtr++;
	}
}

int main(void)
{
	io_init();
	usart_init();
	adc_init();
	pwm_init();
	sei();

    while (1)
	{
		if ((result_0 > thres) && !state_0)
		{
			pwm_update(0, 124);
			state_0 ^= 1;
		}
		else if ((result_0 < thres) && state_0)
		{
			pwm_update(0, 0);
			state_0 ^= 1;
		}

		if ((result_1 > thres) && !state_1)
		{
			pwm_update(1, 124);
			state_1 ^= 1;
		}
		else if ((result_1 < thres) && state_1)
		{
			pwm_update(1, 0);
			state_1 ^= 1;
		}

		if ((result_2 > thres) && !state_2)
		{
			pwm_update(2, 50);
			state_2 ^= 1;
		}
		else if ((result_2 < thres) && state_2)
		{
			pwm_update(2, 0);
			state_2 ^= 1;
		}
	}
}

ISR(ADC_vect)
{
	if (ADMUX == 0x00)
	{
		sprintf(result, "ch0=%d, ch1=%d, ch2=%d\r\n", result_0, result_1, result_2);
		USART_putstring(result);
	}
	if (ADMUX == 0x00)
	{
		result_l_0 = ADCL;
		result_h_0 = ADCH;
		result_0 = 256 * result_h_0 + result_l_0;
		ADMUX = 0x01;
	}
	else if (ADMUX == 0x01)
	{
		result_l_1 = ADCL;
		result_h_1 = ADCH;
		result_1 = 256 * result_h_1 + result_l_1;
		ADMUX = 0x02;
	}
	else if (ADMUX == 0x02)
	{
		result_l_2 = ADCL;
		result_h_2 = ADCH;
		result_2 = 256 * result_h_2 + result_l_2;
		ADMUX = 0x00;
	}
	ADCSRA |= (1<<ADSC);
};