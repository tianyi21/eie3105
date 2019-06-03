/*
 * EIE3105_Lab2_C.c
 *
 * Created: 2018-09-21 10:49:06 PM
 * Author : Bao
 */ 

#include <avr/io.h>
#include <stdio.h>
int main(void)
{
	DDRC = 0x01;			//	PC0 as Output
	DDRD = 0x00;			//	PD4 Input
	PORTD = 0x10;			//	Pull Up
	PORTC = 0x00;			//	Initial OFF
	OCR0A = 0x02;		//	CTC Mode, 3 - 1 = 2
	TCCR0A = 0x02;		//	CTC Mode, Counter 0 with PD4 T0
	TCCR0B = 0x07;		//	Ext Source Falling Edge
	TCNT0 = 0x00;
	while (1) 
    {
		while((TIFR0 & (1<<OCF0A))==0);	//	Continue roll over
		TIFR0 = (1<<OCF0A);	//	Reset OCF0A Flag
		PORTC ^= 0x01;
	}
}

