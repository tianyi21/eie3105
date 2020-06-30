/*
 * Quiz3BQ1.c
 *
 * Created: 11/13/2018 4:04:57 PM
 * Author : LIU Tianyi - 15102892d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char tim = 1;
unsigned char tim_flag = 0;
unsigned char match = 0;
unsigned char str[3] = "3B";
unsigned char ch[3] = "";
unsigned char ch_loc = 0;
unsigned int loc = 0;
unsigned char rcv = '\0';

ISR(TIMER1_COMPA_vect)
{
	tim_flag = 1;
	if (match)
	{
		if (tim == 1)
		{
			TCCR1B = 0x00;
			OCR1AH = 0x7A;
			OCR1AL = 0x11;
			tim = 2;
			match = 0;
			TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
		}
		else if (tim == 2)
		{
			TCCR1B = 0x00;
			OCR1AH = 0x3D;
			OCR1AL = 0x09;
			tim = 1;
			match = 0;
			TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
		}
	}
};

ISR(USART_RX_vect)
{
	rcv = UDR0;
	if (ch[ch_loc] == '\0')
	{
		if (rcv == '3' && ch_loc == 0)
		{
			ch[ch_loc] = rcv;
			ch_loc++;
		}
		if (rcv == 'B' && ch_loc == 1)
		{
			ch[ch_loc] = rcv;
			ch_loc=0;
			ch[0]='\0';
			ch[1]='\0';
			ch[2]='\0';
			match = 1;
		}
	}
};

ISR(USART_UDRE_vect)
{
	if (tim_flag == 1)
	{
		if (str[loc]!='\0')
		{
			UDR0 = str[loc];
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

int main(void)
{    
	usart_init();
	timer1_init();
	sei();
	while (1) 
    {
    }
}

