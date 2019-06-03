/*
 * EIE3105_Lab4_D2.c
 *
 * Created: 10/23/2018 4:37:57 PM
 * Author : 15102892d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

const unsigned char default_char = 'a';
unsigned char rcv = '\0'; 
unsigned char rcv2 = '\0';
unsigned char count = 0;
unsigned char state = 0;

ISR(USART_UDRE_vect)
{
	if (state == 0)
	{
		UDR0 = default_char;
	}
	else if (state == 1)
	{
		if (count < 10)
		{
			UDR0 = rcv;
			count++;
		}
	}
};

ISR(USART_RX_vect)
{
	if (state == 0)
	{
		rcv = UDR0;
		state = 1;
	}
	else if (state == 1)
	{
		rcv2 = UDR0;
		if (rcv2 == rcv)
		{
			rcv = '\0';
			rcv2 = '\0';
			count = 0;
			state = 0;
		}
	}
};

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<UDRE0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0x67;
} 

int main(void)
{
	usart_init();
	sei();
    while (1) 
    {
		;
	}
}
