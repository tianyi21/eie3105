/*
 * EIE3105 Lab 1
 * main.c
 *
 * Created: 9/14/2018 1:54:43 PM
 * Author : LIU Tianyi - 15102892d
*/

#include <avr/io.h>


int main(void)
{
	DDRD = 0xFF;				// PD: OUT
	DDRB &= ~(1<<0);			// PB0: IN
	PORTB != (1<<0);			// PB0: Pull-Up
	while (1)					// INF Loop
	{
		if (!(PINB & (1<<0)))	// Check PB0 IN
		PORTD = 0x00;		// Low: Off
		else
		PORTD = 0xFF;		// High: On
	}
}
