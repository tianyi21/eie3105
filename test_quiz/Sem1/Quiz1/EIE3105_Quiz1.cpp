EIE3105_Quiz1.cpp

//	Set as Input
DDRA = 0x00;
PORTA = 0xFF;
var = PINA;

//  Set as Output
DDRA = 0xFF;
    //  Output 0
    PORTA = 0x00;
    //  Output 1
    PORTA = 0xFF;

//  Set a bit to 1
PORTA |= (1<<4);

//  Clear a bit to 0
PORTA &= ~(1<<4);

//  Check a bit
var = PINA & (1<<4);

//  Toggle
^=
//  Shift Operation
    //  Shift R 3
    var>>3
    //  Shift L 2
    var>>2

//  Create Delay
    //16M MC per second
for (unsigned int i = 0; i < 255; i++)
    for (unsigned int j = 0; j < 255; j++)

//  Timer Normal Mode
//  Timer 0, 2 8 Bit

//  Timer 0: P106
//  Timer 1: P134
//  Timer 2: P158

void time0_Delay()
{
    TCNT0 = 0x54;                       //  256 - TIME
    TCCR0A = 0x00;                      //  Normal Mode
    TCCR0B = 0x05;                      //  Prescaler
    while((TIFR0 & (1<<TOV0)) == 0);    //  Wait
    TCCR0A = 0x00;                      //  Stop Timer
    TIFR0 = (1<<TOV0);                  //  Clear Flag
}

void time1_Delay()
{
    TCNT1H = 0xC2;
    TCNT1L = 0xF7;                      //  65536 - TIME
    TCCR1A = 0x00;                      //  Normal Mode
    TCCR1B = 0x05;                      //  Prescaler
    while ((TIFR1 &(1<<TOV1))==0);      //  Wait
    TCCR1B = 0x00;                      //  Stop Timer
    TIFR1 = (1<<TOV1);                  //  Clear Flag
}

//  Timer CTC Mode
//  Timer 0, 2 8 Bit

//  Timer 0: P106
//  Timer 1: P134
//  Timer 2: P158

void time0_Delay()
{
    OCR0A = 0x54;                       //  TIME - 1
    TCCR0A = 0x00;                      //  CTC Mode
    TCCR0B = 0x08;                      //  Prescaler
    while((TIFR0 & (1<<OCF0A) == 0);    //  Wait
    TCCR0B = 0x00;                      //  Stop Timer
    TIFR0 = (1<<OCF0A);                  //  Clear Flag
}

void Timer_1_Delay()
{
    OCR1AH = 0x3D;
    OCR1AL = 0x08;                      //  TIME - 1
    TCCR1A = 0x00;                      //  CTC Mode
    TCCR1B = 0x0D;                      //  Prescaler
    while ((TIFR1 &(1<<OCF1A))==0);     //  Wait
    TCCR1B = 0x00;                      //  Stop Timer
    TIFR1 = (1<<OCF1A);                 //  Clear Flag
}

//  Counter
//  Timer 0, 2 8 Bit
        
//  Timer 0: P106
//  Timer 1: P134
//  Timer 2: P158

//  T0 Timer 0
void counter()
{
    TCCR0A = 0x00;
    TCCR0B = 0x06;                      // Falling
    TCNT0 = 0x00;
    while (1)
    {
        do
        {
            ;
        }while((TIFR & (1<<TOV1)) == 0);
        TIFR = 1<<TOV1;
    }
}
          

//
        int stage = 0, value_0 = 0, value;        // Stage: Represent State; value_0: Past Value; value: Present Value
          while (1)                                // INF Loop
          {
              delay_20ms();
              value = (PINB & (1<<0));            // Read Input Present Value
              if (value == value_0)                // If Present == Past
                  continue;                        // No Change, Break Present Loop, until Change
              
              /*
               *
               */
              
              if (!value)                            // Check if the button is released
              {                                    // If release,
                  if (stage)                        // Update Stage
                      stage = 0;                    // 1 -> 0
                  else
                      stage = 1;                    // 0 -> 1
                  }

//  Timer1
void delay_20ms()
{
    OCR1AH = 0x01;
    OCR1AL = 0x38;                      //  TIME - 1
    TCCR1A = 0x00;                      //  CTC Mode
    TCCR1B = 0x0D;                      //  Prescaler
    while ((TIFR1 &(1<<OCF1A))==0);     //  Wait
    TCCR1B = 0x00;                      //  Stop Timer
    TIFR1 = (1<<OCF1A);                 //  Clear Flag
}
              
//  Timer0
void delay_20ms()
{
    for (unsigned char i = 0; i < 8; i++)
    {
        OCR0A = 0x9B;                       //  TIME - 1
        TCCR0A = 0x00;                      //  CTC Mode
        TCCR0B = 0x04;                      //  Prescaler = 256
        while((TIFR0 & (1<<OCF0A) == 0);    //  Wait
        TCCR0B = 0x00;                      //  Stop Timer
        TIFR0 = (1<<OCF0A);                 //  Clear Flag
    }
}
