/*
 * Oppgave 6.2.cpp
 *
 * Created: 28.10.2017 14:45:56
 * Author : Fredrik
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

bool pinChange = 0;


int main(void)
{
	DDRB = (0 << PB0);
	DDRD = (1 << PD6);
	//PORTB |= (1 << PB0);
	
	PCMSK0 |= (1<<PCINT0);
	
	PCICR |= (1<<PCIE0);
	
	sei();
}

ISR(PCINT0_vect)
{
	pinChange = !pinChange;
	
	if (pinChange == 1)
	{
		PORTD |= (1 << PD6);
	}
	
	if (pinChange == 0)
	{
		PORTD &= 0b10111111;
	}

}