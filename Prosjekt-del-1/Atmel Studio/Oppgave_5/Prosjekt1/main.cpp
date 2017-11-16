/*
 * Prosjekt1.cpp
 *
 * Created: 09.10.2017 11.08.05
 * Author : Barstad
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif 

#include <avr/io.h>			// Header file for input/output functions
#include <util/delay.h>		// delay/timer libraries




int main(void)
{
    DDRD |= (1 << PD0);		// set PD0 as output
	PORTD &= ~(1 << PD0);	// set PD0 to low	
	
    while (1) 
    {
		// LED on
		PORTD |= (1 << PD0);		// set PD0 high
		_delay_ms(1000);
		
		// LED off
		PORTD &= ~(1 << PD0);		// set PD0 low
		_delay_ms(1000);
		
    }
}

