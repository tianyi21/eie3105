//	EIE3105 Quiz 1 Version A Question 1
// 	Written by 15102892d LIU Tianyi
//	Feb 12th, 2019

/*
 * T1A_15102892d.c
 *
 * Created: 2/12/2019 2:35:57 PM
 * Author : 15102892d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

unsigned char rcv = 0;
unsigned int state = 0;

void pwm_init()
{
	//	Fast PWM, Non-inverting
	TCCR0A = (1<<WGM01) | (1<<WGM00) |
	(1<<COM0A1) | (0<<COM0A0) |
	(1<<COM0B1) | (0<<COM0B0);
	
	//	Pre = 256, Non-inverting
	TCCR0B = (1<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);

	//	Freq = 500Hz
	OCR0A = 124;

	//	DC
	OCR0B = 0;

	//	Fast PWM, Non-inverting
	TCCR2A = (1<<WGM21) | (1<<WGM20) |
			(1<<COM2A1) | (0<<COM2A0) |
			(1<<COM2B1) | (0<<COM2B0);
	
	//	Pre = 256, Non-inverting
	TCCR2B = (1<<WGM22) | (1<<CS22) | (1<<CS21) | (0<<CS20);

	//	Freq = 500Hz
	OCR2A = 124;

	//	DC
	OCR2B = 0;
}

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0x67;
}



int main(void)
{
    DDRD |= (1<<DDD5) | (1<<DDD3);
	pwm_init();
	usart_init();
	sei();
    while (1) 
    {
		switch (state)
		{
		case 0: 
		{	
			OCR0B = 110;
			OCR2B = 0;
			break;
		}
		case 1:
		{
			OCR0B = 0;
			OCR2B = 15;
			break;
		}
		}
    }
}

ISR(USART_RX_vect)
{
	rcv = UDR0;
	if (rcv == 'a')
		state ^= 0x01;
}