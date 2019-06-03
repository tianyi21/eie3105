/*
 * EIE3105 Lab 1
 * main.c
 *
 * Created: 9/14/2018 1:54:43 PM
 * Author : LIU Tianyi - 15102892d
*/

#include <avr/io.h>

// Part 2
int main(void)
{
	DDRD = 0xFF;						// PD: OUT
	while (1)							// INF Loop
	{
		PORTD = 0x55;
		for (int i = 0; i < 80; i++)
		{
			for (int j = 0; j < 252; j++)
			{
				for (int k = 0 ; k <252 ; k++)
				;
			}
		}
		PORTD = 0xAA;
		for (int i = 0; i < 80; i++)
		{
			for (int j = 0; j < 252; j++)
			{
				for (int k = 0 ; k <252 ; k++)
				;
			}
		}

	}
}

/*	
	Calculation
	Clock Rate: 16 MHz
	1 s delay -> 16 M cycles ~ 252 ^ 3
	However, the actual time is longer than 1 s
	To make it flash every 1 s, the total cycles are changed to around 5 M
	As taught in the lecture, the C language i++ may take at least 3 MC to execute
	Therefore, 5 MC, around 1/3 of the theoretical value also coincide what taught in the lecture
*/