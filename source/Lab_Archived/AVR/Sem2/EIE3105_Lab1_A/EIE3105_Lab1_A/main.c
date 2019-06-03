/*
 * EIE3105_Lab1_A.c
 *
 * Created: 2019-01-21 2:29:03 PM
 * Author : Bao
 */ 

#include <avr/io.h>

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

	//	DC = 50%
	OCR0B = 62;

}

int main(void)
{
	DDRD |= (1<<DDD5);
    pwm_init();
	
    while (1) 
    {
    }
}

