/*
 * EIE3105_Lab1_B.c
 *
 * Created: 2019-01-21 3:51:57 PM
 * Author : Bao
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char rcv = '\0';
unsigned char rcv_0 = '\0';
unsigned char rcv_1 = '\0';
unsigned char rcv_store = '\0';
unsigned int input_clk = 0;
unsigned int flag_non_in = 0;
unsigned int flag_change = 0;
unsigned int flag_input = 0;
unsigned int flag_en_mes = 0;
unsigned int flag_mes_done = 0;
unsigned char str_err[30] = "No Input! Input Again\n";
unsigned char str_mes[30] = "\nMeasurement: ";
unsigned char str_res[30] = "";
unsigned int loc = 0;
unsigned int result = 0;

void pwm_init()
{
	//	Fast PWM, Non-inverting
	TCCR0A = (1<<WGM01) | (1<<WGM00) | 
			(1<<COM0A1) | (0<<COM0A0) |
			(1<<COM0B1) | (0<<COM0B0);
	
	//	Pre = 256, Non-inverting
	TCCR0B = (1<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);

	//	Freq = 500Hz
	OCR0A = 124;

	//	DC
	OCR0B = 0;
}

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<UDRIE0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0L = 0x67;
}

unsigned char capture()
{
	unsigned char width;
	TCCR1A = 0;
	TCCR1B = (1<<ICES1) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	TIFR1 = (1<<ICF1);
	while((TIFR1&(1<<ICF1)) == 0);
	width = ICR1L;

	TIFR1 = (1<<ICF1);
	TCCR1B = (0<<ICES1) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	while((TIFR1&(1<<ICF1)) == 0);
	width = ICR1L - width;
	return width;
}

void measure()
{
	result = capture();
	str_res[0] = (unsigned char)((result/10) + 48);
	str_res[1] = (unsigned char)(result%10 + 48);
	str_res[2] = '\n';
	str_res[3] = 13;
	flag_mes_done = 1;
}

ISR(USART_RX_vect)
{
	rcv = UDR0;
	flag_input = 1;
	rcv_store = rcv;
	if (rcv == 13)
	{
		if (rcv_0 == '\0')
		{	
			flag_non_in = 1;
			rcv = '\0';
		}
		else if (rcv_1 == '\0')
		{
			OCR0B = rcv_0 - 48;
			flag_change = 1;
			flag_en_mes = 1;
			rcv_0 = '\0';
			rcv = '\0';
			measure();
		}
		else
		{
			OCR0B = (rcv_1 - 48) * 10 + (rcv_0 - 48);
			flag_change = 1;
			flag_en_mes = 1;
			rcv_0 = '\0';
			rcv_1 = '\0';
			rcv = '\0';
			measure();
		}
	}
	else if (rcv_0 == '\0')
	{
		rcv_0 = rcv;
		rcv = '\0';
	}
	else
	{
		rcv_1 = rcv_0;
		rcv_0 = rcv;
		rcv = '\0';
	}
};

ISR(USART_UDRE_vect)
{
	if (flag_non_in == 1)
	{
		if (str_err[loc] != '\0')
		{
			UDR0 = str_err[loc];
			loc++;
		}
		else
		{	
			loc = 0;
			flag_non_in = 0;
		}
	}
	else if (flag_input == 1)
	{
		UDR0 = rcv_store;
		flag_input = 0;
	}
	else if (flag_en_mes == 1)
	{
		if (str_mes[loc] != '\0')
		{
			UDR0 = str_mes[loc];
			loc++;
		}
		else
		{
			loc = 0;
			flag_en_mes = 0;
		}
	}
	else if (flag_mes_done == 1)
	{
		if (str_res[loc] != '\0')
		{
			UDR0 = str_res[loc];
			loc++;
		}
		else
		{
			loc = 0;
			flag_mes_done = 0;
		}
	}
};

int main(void)
{
	DDRD |= (1<<DDD5);
	usart_init();
    pwm_init();
	sei();

    while (1) 
    {
    }
}


