/*
 * EIE3105 Lab 1
 * main.c
 *
 * Created: 9/14/2018 1:54:43 PM
 * Author : LIU Tianyi - 15102892d
*/

#include <avr/io.h>

// Part 1

int main(void)
{
	DDRD=0xFF;
	while(1)
	{
		PORTD=0x55;
	}
}