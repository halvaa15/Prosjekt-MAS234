/*
 * Prosjekt1.cpp
 *
 * Created: 17.10.2017 11.08.05
 * Author : Barstad
 */ 

#include <avr/io.h>

#define BIT_IS_SET(byte, bit) (byte & (1 << bit))	//Sjekker et byte (f.eks. PINB) og sjekker om det tilhørende bit (f.eks. PB0) er 1
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit))	//Gjør det samme, men sjekker at tilhørende bit er 0

int main(void)
{
    DDRB &= ~(1 << PB0);	// set PB0 as input
	
	PORTB |= (1 << PB0);	// set PB0 high (pull up)
	
	DDRD |= (1 << PD6);		// set PD6 as output
	
	
    while (1)
    {
		// LED on
		if(!(PINB & (1 << PB0)))		// true if PB0 is low (button pushed)
		{
			PORTD |= (1 << PD6);	// set PD6 high
		}
		// LED off
		else if(PINB & (1 << PB0))	// true if PB0 is high (button not pushed)
		{
			PORTD &= ~(1 << PD6);	// set PD6 low
		}
    }
}

