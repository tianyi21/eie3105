/*
 * EIE3105_Lab4_D1.c
 *
 * Created: 10/23/2018 4:37:57 PM
 * Author : 15102892d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

const unsigned char welcome[32] = "We are ready!";
const unsigned char bye[32] = "Bye!";
unsigned char rcv = '\0'; 
unsigned int loc = 0;
unsigned int ch_loc = 0;
unsigned char state = 0;
unsigned char match = 0;
unsigned char ch[3] = "";

ISR(USART_UDRE_vect)
{
	if (!state)
	{
		if (welcome[loc] == '\0')
		{
			state = 1;
			loc = 0;
		}
		else
		{
			UDR0 = welcome[loc];
			loc++;
		}
	}
	if (state == 1 && match == 1)
	{
		if (bye[loc] == '\0')
		{
			loc = 0;
			ch[0] = "\0";
			ch[1] = "\0";
			match = 0;
		}
		else
		{
			UDR0 = bye[loc];
			loc++;
		}
	}
};

ISR(USART_RX_vect)
{
	rcv = UDR0;
	if (ch[ch_loc] == '\0')
	{
		if (rcv == 'H' && ch_loc == 0)
		{
			ch[ch_loc] = rcv;
			ch_loc++;
		}
		if (rcv == 'i' && ch_loc == 1)
		{
			ch[ch_loc] = rcv;
			ch_loc=0;
			match = 1;
		}
	}
};

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<UDRIE0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0xCF;
} 

int main(void)
{
	usart_init();
	sei();
    while (1) 
    {
	}
}
