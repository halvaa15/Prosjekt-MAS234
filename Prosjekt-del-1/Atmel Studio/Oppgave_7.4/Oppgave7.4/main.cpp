/*
 * Prosjekt1.cpp
 *
 * Created: 17.10.2017 11.08.05
 * Author : Barstad
 */ 

#include <avr/io.h>		//Headerfil som gir oss forskjellige input/output operasjoner

#define BIT_IS_SET(byte, bit) (byte & (1 << bit))	//Sjekker et byte (f.eks. PINB) og sjekker om det tilhørende bit (f.eks. PB0) er 1
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit))	//Gjør det samme, men sjekker at tilhørende bit er 0

int main(void)
{
    DDRB &= ~(1 << PB0);	//DDR = data direction. B = område B på mikrokontrolleren. Setter PB0 (pinne 14) til å være 0, altså input
	
	PORTB |= (1 << PB0);	//Aktiverer intern pull-up resistor på inngang PB0 (pinne 14)
	
	DDRD |= (1 << PD0);		//Setter PD0 (pinne 2) til å være 1 (definerer den til å være en utgang)
	
	
    while (1)	//Begynner uendelig loop
    {
		//Skru på lyset
		if(BIT_IS_CLEAR(PINB, PB0)))		//Hvis pinne 14 er høy
		{
			PORTD |= (1 << PD0);	//Setter PD0 (pinne 2) høy, og LED-en lyser
		}
		
		else if(BIT_IS_SET(PINB, PB0))	//Hvis pinne 14 er lav
		{
			PORTD &= ~(1 << PD0);	//Setter PD0 (pinne 2) lav, og LED-en lyser ikke
		}
		
		else
		{
		}
		
    }
	
	return (0);		//Kun for å ikke få anmerkning på kompileringen
}

