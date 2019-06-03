/*
 * EIE3105_Lab3_C.c
 *
 * Created: 2018-10-07 12:16:34 AM
 * Author : Bao
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int stage = 0;
unsigned int count = 0;
unsigned int flag = 1;
void led_light();

int main(void)
{
    DDRB = 0x3F;			// PB0 - 5 as Output
	DDRC = 0x31;			// PC0, 4 - 5 as Output
	DDRD = 0x00;			// PD2 Input
	PORTB = 0x00;			// Initial LOW
	PORTC = 0x00;			// Initial LOW
	PORTD = 0x08;			// Pull Up

	OCR1AH = 0x0C;
	OCR1AL = 0x35;
	TCCR1A = 0x00;
	TCCR1B = 0x0D;
	
	TIMSK1 = (1<<OCIE1A);
	EIMSK = (1<<INT1);
	EICRA = 0x08;
	sei();
    
	while (1)				// INF Loop
    {
		led_light();
    }
}

void reset()
{
	count = 0;
	if (stage == 7)
		stage = 0;
	else
		stage++;
}

void led_light()
{
	switch(stage)
	{
		case 0:
		{
			if (!flag)
			{
				if (count == 25)
				{
					PORTC |= 1<<5;
					PORTC &= ~(1<<4);	//	Light 1 Red
					PORTB = 0x11;		//	Light 2 Yellow Light 3 Red
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count != 25)
			{
				PORTC |= 1<<5;
				PORTC &= ~(1<<4);	//	Light 1 Red
				PORTB = 0x21;		//	Light 2 Green Light 3 Red
				break;
			}
			else if (count == 25)
			{
				PORTC |= 1<<5;
				PORTC &= ~(1<<4);	//	Light 1 Red
				PORTB = 0x11;		//	Light 2 Yellow Light 3 Red
				reset();
				break;
			}
		}
		case 1:
		{
			if (!flag)
			{
				if (count == 5)
				{
					PORTC |= 1<<5;
					PORTC &= ~(1<<4);	//	Light 1 Red
					PORTB = 0x09;		//	Light 2 Red Light 3 Red
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count != 5)
				break;
			else if (count == 5)
			{
				PORTC |= 1<<5;
				PORTC &= ~(1<<4);	//	Light 1 Red
				PORTB = 0x09;		//	Light 2 Red Light 3 Red
				reset();
				break;
			}
		}
		case 2:
		{
			if (!flag)
			{
				if (count == 5)
				{
					PORTC |= 1<<5;
					PORTC &= ~(1<<4);	//	Light 1 Red
					PORTB = 0x0B;		// Light 2 Red Light 3 Red + Yellow
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count != 5)
				break;
			else if (count == 5)
			{
				PORTC |= 1<<5;
				PORTC &= ~(1<<4);	//	Light 1 Red
				PORTB = 0x0B;		// Light 2 Red Light 3 Red + Yellow
				reset();
				break;
			}
		}
		case 3:
		{
			if (!flag)
			{
				if (count == 5)
				{
					PORTC |= 1<<4;
					PORTC &= ~(1<<5);	//	Light 1 Green
					PORTB = 0x0C;		//	Light 2 Red Light 3 Green
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count != 5)
				break;
			else if (count == 5)
			{
				PORTC |= 1<<4;
				PORTC &= ~(1<<5);	//	Light 1 Green
				PORTB = 0x0C;		//	Light 2 Red Light 3 Green
				reset();
				break;
			}
		}
		case 4:
		{
			if (!flag)
			{
				if (count == 25)
				{
					PORTC |= 1<<4;
					PORTC &= ~(1<<5);	//	Light 1 Green
					PORTB = 0x0C;		// Light 2 Red Light 3 Green
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count != 25)
				break;
			else if (count == 25)
			{
				PORTC |= 1<<4;
				PORTC &= ~(1<<5);	//	Light 1 Green
				PORTB = 0x0C;		// Light 2 Red Light 3 Green
				reset();
				break;
			}
		}
		case 5:
		{
			if (!flag)
			{
				if (count == 5)
				{
					PORTC |= 1<<5;
					PORTC &= ~(1<<4);	//	Light 1 Red
					PORTB = 0x09;		//	Light 2 Red Light 3 Red
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count == 5)
			{
				PORTC |= 1<<5;
				PORTC &= ~(1<<4);	//	Light 1 Red
				PORTB = 0x09;		//	Light 2 Red Light 3 Red
				reset();
				break;
			}
			else if (count % 2 == 0)
			{
				PORTC |= 1<<4;
				PORTC &= ~(1<<5);	//	Light 1 Green
				PORTB = 0x0A;		// Light 2 Red Light 3 Yellow
				break;
			}
			else
			{
				PORTC &= ~(1<<4);
				PORTC &= ~(1<<5);	//	Light 1 OFF Blinking
				PORTB = 0x0A;		// Light 2 Red Light 3 Yellow
				break;
			}
		}
		case 6:
		{
			if (!flag)
			{
				if (count == 5)
				{
					PORTC |= 1<<5;
					PORTC &= ~(1<<4);	//	Light 1 Red
					PORTB = 0x19;		//	Light 1 Red Light 2 Red + Yellow Light 3 Red
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count != 5)
				break;
			else if (count == 5)
			{
				PORTC |= 1<<5;
				PORTC &= ~(1<<4);	//	Light 1 Red
				PORTB = 0x19;		//	Light 1 Red Light 2 Red + Yellow Light 3 Red
				reset();
				break;
			}
		}
		case 7:
		{
			if (!flag)
			{
				if (count == 5)
				{
					PORTC |= 1<<5;
					PORTC &= ~(1<<4);	//	Light 1 Red
					PORTB = 0x21;		//	Light 2 Green Light 3 Red
					reset();
					break;
				}
				PORTC = 0;
				PORTB = 0;
				break;
			}
			else if (count != 5)
				break;
			else if (count == 5)
			{
				PORTC |= 1<<5;
				PORTC &= ~(1<<4);	//	Light 1 Red
				PORTB = 0x21;		//	Light 2 Green Light 3 Red
				reset();
				break;
			}
		}
	}
}

ISR (INT1_vect)
{
	if (flag)
		flag = 0;
	else 
		flag = 1;
}

ISR (TIMER1_COMPA_vect)
{
	count++;
}