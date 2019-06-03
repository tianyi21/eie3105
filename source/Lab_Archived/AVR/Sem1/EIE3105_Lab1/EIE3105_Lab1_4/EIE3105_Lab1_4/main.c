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
	DDRD = 0xFF;							// PD: OUT
	DDRB &= ~(1<<0);						// PB0: IN
	PORTB != (1<<0);						// PB0: Pull-Up
	int stage = 0, value_0 = 0, value;		// Stage: Represent State; value_0: Past Value; value: Present Value
	while (1)								// INF Loop
	{
		for (double i=0;i<576;i++);			// Introduce a short delay, improve stability of the button.
		value = (PINB & (1<<0));			// Read Input Present Value
		if (value == value_0)				// If Present == Past
		continue;						// No Change, Break Present Loop, until Change
		if (stage)							// Toggle //Present 0: On -> Off Present 1: Off -> On
		PORTD = 0x00;					// Low: Off
		else
		PORTD = 0xFF;					// High: On
		value_0 = value;					// Update Past Value
		if (!value)							// Check if the button is released
		{									// If release,
			if (stage)						// Update Stage
			stage = 0;					// 1 -> 0
			else
			stage = 1;					// 0 -> 1
		}
	}
}
