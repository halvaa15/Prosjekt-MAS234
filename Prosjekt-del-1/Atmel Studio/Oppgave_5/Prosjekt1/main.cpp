/*
 * Prosjekt1.cpp
 *
 * Created: 09.10.2017 11.08.05
 * Author : Barstad
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif 

#include <avr/io.h>		//Headerfil som gir oss forskjellige input/output operasjoner
#include <util/delay.h>		//bibliotek for delay-funksjoner




int main(void)
{
    DDRD = 0b00000001;	//DDR = data direction. B = område B på mikrokontrolleren. Her setter vi pinne 0 på området B til å være en utgang. 
	PORTD = 0b00000000;	
	
    while (1) 
    {
		//Skru på lyset
		PORTD |= 0b00000001;		//Setter pinnen PB0 til å være høy.
		_delay_ms(100);
		
		//Skru av lyset
		PORTD &= 0b11111110;		//Setter pinnen til å være lav.
		_delay_ms(100);
		
    }
}

