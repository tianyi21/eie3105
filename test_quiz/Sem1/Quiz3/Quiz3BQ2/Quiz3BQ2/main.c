/*
 * Quiz3BQ2.c
 *
 * Created: 11/13/2018 4:04:57 PM
 * Author : LIU Tianyi - 15102892d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char tim = 1;
unsigned char tim_flag = 0;
unsigned char str1[5] = "3B";
unsigned char str2[5] = "INT1";
unsigned int loc = 0;
unsigned char input = '\0';

ISR(INT1_vect)
{
	if (tim == 1)
	{
		TCCR1B = 0x00;
		OCR1AH = 0x7A;
		OCR1AL = 0x11;
		tim = 2;
		tim_flag = 1;
		TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
	}
	else if (tim == 2)
	{
		TCCR1B = 0x00;
		OCR1AH = 0x3D;
		OCR1AL = 0x09;
		tim = 1;
		tim_flag = 1;
		TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
	}
}

ISR(TIMER1_COMPA_vect)
{
	tim_flag = 1;
};

ISR(USART_UDRE_vect)
{
	if (tim == 1 && tim_flag == 1)
	{
		if (str1[loc]!='\0')
		{
			UDR0 = str1[loc];
			loc++;
		}
		else
		{
			tim_flag = 0;
			loc = 0;
		}
	}
	else if (tim == 2 && tim_flag == 1)
	{
		if (str2[loc]!='\0')
		{
			UDR0 = str2[loc];
			loc++;
		}
		else
		{
			tim_flag = 0;
			loc = 0;
		}
	}
};

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<UDRE0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0x67;
}

void timer1_init()
{
	OCR1AH = 0x3D;
	OCR1AL = 0x08;
	TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
	TIMSK1 = (1<<OCIE1A);
}

void ext_init()
{
	PORTD |= (1<<3);
	EIMSK |= (1<<INT1);
	EICRA = (1<<ISC11) | (1<<ISC10);
}

int main(void)
{    
	usart_init();
	timer1_init();
	ext_init();
	sei();
	while (1) 
    {
		;
    }
}


