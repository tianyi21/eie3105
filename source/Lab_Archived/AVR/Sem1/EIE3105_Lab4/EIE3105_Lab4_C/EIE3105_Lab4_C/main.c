/*
 * EIE3105_Lab4_C.c
 *
 * Created: 10/23/2018 4:37:57 PM
 * Author : 15102892d
 */ 

#include <avr/io.h>

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0x67;
} 

void usart_send(unsigned char ch)
{
	while(!(UCSR0A &(1<<UDRE0)));
	UDR0 = ch;
}

void usart_read(unsigned char* ptr_ch)
{
	while(!(UCSR0A & (1<<RXC0)));
	*ptr_ch = UDR0;
}

int main(void)
{
	usart_init();
	const unsigned char ch = 'a';
	unsigned char ch2 = '\0';
	unsigned char ch3 = '\0';
    while (1) 
    {
		do
		{
			usart_send(ch);
		}while(!(UCSR0A & (1<<RXC0)));
		usart_read(&ch2);
		ch3 = ch2;
		for (unsigned int i = 0; i < 10; i++)
		{
			usart_send(ch2);
		}
		ch2 = '\0';
		do
		{
			usart_read(&ch2);
		}while (ch2 != ch3);
	}
}