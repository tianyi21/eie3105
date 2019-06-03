/*
 * EIE3105_Lab4_B.c
 *
 * Created: 10/23/2018 4:37:57 PM
 * Author : 15102892d
 */ 

#include <avr/io.h>

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0xCF; 
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

void welcome_mess()
{
	unsigned char welcome[32] = "We are ready!";
	unsigned int loc = 0;
	for ( ; welcome[loc]!='\0' ; loc++)
		usart_send(welcome[loc]);
}

void mess()
{
	unsigned char message[32] = "Bye!";
	unsigned int loc = 0;
	for ( ; message[loc]!='\0' ; loc++)
	usart_send(message[loc]);
}

int main(void)
{
	usart_init();
	welcome_mess();	
	unsigned char ch;
    while (1) 
    {
		usart_read(&ch);
		if (ch != 'H')
			continue;
		
		usart_read(&ch);
		if (ch != 'i')
			continue;

		mess();
    }
}