/*
 * Serial_Port.c
 *
 * Created: 2018-11-13 1:02:57 PM
 * Author : Bao
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char ch = 'a';
unsigned char dir = 1;
unsigned char input = '\0';
unsigned char flag = 0;

ISR(USART_RX_vect)
{
	if (true)
	{
		if (ch != 'a' & ch != 'z')
		{
			UDR0 = ch;
			ch = ch + dir;
			flag = 0;
		}
		else if (ch == 'a')
		{
			UDR0 = ch;
			dir = 1;
			ch = ch + dir;
			flag = 0;
		}
		else if (ch == 'z')
		{
			UDR0 = ch;
			dir = -1;
			ch = ch + dir;
			flag = 0;
		}
	}
};

ISR(USART_UDRE_vect)
{
	input = UDR0;
	if (input == '1')
	{
		dir *= -1;
		input = '\0';
	}
	else
	{
		input = '\0';
	}
};

ISR(TIMER1_COMPA_vect)
{
	flag = 1;
};

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<UDRIE0) | (1<<RXCIE0);		//	Send, Write, Int
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);									//	Char = 8
	UBRR0L = 0x67;		// BR = 9600
}

void timer1_init()
{
	OCR1AH = 0x3D;
	OCR1AL = 0x08;
	TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
	TIMSK1 = (1<<OCIE1A);
}

int main(void)
{
    usart_init();
	timer1_init();
	sei();
    while (1) 
    {
		;
    }
}