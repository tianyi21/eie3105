/*
 * EIE3105_Lab2_A_2.c
 *
 * Created: 2018-09-21 3:02:30 PM
 * Author : Bao
 */ 

#include <avr/io.h>

void Timer_1_Delay();		// Prototype for Delay Function 

int main(void)
{
    DDRB = 1<<0;			// PB0 as Output
	PORTB = 1<<0;			// Initial LOW
    while (1)				// INF Loop
    {
		Timer_1_Delay();	// Call 1 s Delay
		PORTB ^= (1<<0);	// Toggle
    }
}

void Timer_1_Delay()
{
// Timer 1 16 bit timer with prescaler = 1024
// 1 * 16M / 1024 = 15625
	OCR1AH = 0x3D;		// CTC OCR
	OCR1AL = 0x08;		// 15625 - 1 = 0x3D08
	TCCR1A = 0x00;		// CTC Mode
	TCCR1B = 0x0D;		// CTC Mode, Prescaler = 1024
	while ((TIFR1 &(1<<OCF1A))==0);	// Continue Until Overflow
	TCCR1B = 0x00;		// Stop Timer 1
	TIFR1 = (1<<OCF1A);	// Reset OCR1A
}