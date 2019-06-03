/*
 * EIE3105_Lab2_B_1.c
 *
 * Created: 2018-09-21 3:02:30 PM
 * Author : Bao
 */ 

#include <avr/io.h>

void Timer_1_Delay();		// Prototype for Delay Function 
void Timer_1_Delay_5();
void Timer_1_Delay_2();
void state1();
void state2();
void state3();
void state4();
void state5();
void state6();
void state7();
void state8();

int main(void)
{
    DDRB = 0x3F;			// PB0 - 5 as Output
	DDRC = 0x30;			// PC 4 - 5 as Output
	PORTB = 0x00;			// Initial LOW
	PORTC = 0x00;			// Initial LOW
    while (1)				// INF Loop
    {
		state1();			// 1 R 2 G 3 R
		Timer_1_Delay_5();	// Call 5 s Delay
		state2();			// 1 R 2 Y 3 R
		Timer_1_Delay();	// Call 1 s Delay
		state3();			// 1 R 2 R 3 R
		Timer_1_Delay();	// Call 1 s Delay
		state4();			// 1 R 2 R 3 R+Y
		Timer_1_Delay();	// Call 1 s Delay
		state5();			// 1 G 2 R 3 G
		Timer_1_Delay_5();	// Call 1 s Delay
		state6();			// 1 GB 2 R 3 R
		state7();			// 1 R 2 R 3 R
		Timer_1_Delay();	// Call 1 s Delay
		state8();			// 1 R 2 R+Y 3 R
		Timer_1_Delay();	// Call 1 s Delay
    }
}

//	Create 1 s Delay
void Timer_1_Delay()
{
// Timer 1 16 bit timer with prescaler = 1024
// 1 * 16M / 1024 = 15625
	TCNT1H = 0xC2;		
	TCNT1L = 0xF7;		// 65536 - 15625 = 49911 = 0xC2F7
	TCCR1A = 0x00;		// Normal Mode
	TCCR1B = 0x05;		// Prescaler = 1024
	while ((TIFR1 &(1<<TOV1))==0);	// Continue Until Overflow
	TCCR1B = 0x00;		// Stop Timer 1
	TIFR1 = (1<<TOV1);	// Reset TOV1
}

//	Create 5 s Delay
void Timer_1_Delay_5()
{
	Timer_1_Delay();
	Timer_1_Delay();
	Timer_1_Delay();
	Timer_1_Delay();
	Timer_1_Delay();
}

//	Create 0.2 s Delay
void Timer_1_Delay_2()
{
	// Timer 1 16 bit timer with prescaler = 1024
	// 0.2 * 16M / 1024 = 3125
	TCNT1H = 0xF3;
	TCNT1L = 0xCB;		// 65536 - 3125 = 62411 = 0xF3CB
	TCCR1A = 0x00;		// Normal Mode
	TCCR1B = 0x05;		// Prescaler = 1024
	while ((TIFR1 &(1<<TOV1))==0);	// Continue Until Overflow
	TCCR1B = 0x00;		// Stop Timer 1
	TIFR1 = (1<<TOV1);	// Reset TOV1
}

//	State 1
void state1()
{
	PORTC = 0x10;		//	Light 1 Red
	PORTB = 0x21;		//	Light 2 Green Light 3 Red
}

//	State 2
void state2()
{
	PORTC = 0x10;		//	Light 1 Red
	PORTB = 0x11;		//	Light 2 Yellow Light 3 Red 
}

//	State 3
void state3()
{
	PORTC = 0x10;		//	Light 1 Red
	PORTB = 0x09;		//	Light 2 Red Light 3 Red
}

//	State 4
void state4()
{
	PORTC = 0x10;		// Light 1 Red
	PORTB = 0x0B;		// Light 2 Red Light 3 Red + Yellow
}

//	State 5
void state5()
{
	PORTC = 0x20;		//	Light 1 Green
	PORTB = 0x0C;		//	Light 2 Red Light 3 Green
}

//	State 6
void state6()
{
	PORTC = 0x20;		// Light 1 Green
	PORTB =	0x0A;		// Light 2 Red Light 3 Green 
	Timer_1_Delay_2();	// Call 0.2 s Delay
	PORTC = 0x00;		//	Blinking
	Timer_1_Delay_2();	// Call 0.2 s Delay
	PORTC = 0x20;		
	Timer_1_Delay_2();	// Call 0.2 s Delay
	PORTC = 0x00;		//	Blinking
	Timer_1_Delay_2();	// Call 0.2 s Delay
	PORTC = 0x20;
	Timer_1_Delay_2();	// Call 0.2 s Delay
}

//	State 7
void state7()
{
	PORTC = 0x10;		//	Light 1 Red
	PORTB = 0x09;		//	Light 2 Red Light 3 Red
}

//	State 8
void state8()
{
	PORTC = 0x10;		//	Light 1 Red
	PORTB = 0x19;		//	Light 1 Red Light 2 Red + Yellow Light 3 Red
}