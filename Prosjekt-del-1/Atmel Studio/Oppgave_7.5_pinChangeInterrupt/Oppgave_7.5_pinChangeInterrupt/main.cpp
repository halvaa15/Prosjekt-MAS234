/*
 * Oppgave_7.5_pinChangeInterrupt.cpp
 *
 * Created: 28.10.2017 16:53:38
 * Author : Fredrik
 */ 

#define 	F_CPU   16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	DDRB &= ~(1 << PB0);		// set pin 0 PORTB as input
	PORTB |= (1 << PB0);		// set pin 0 PORTB true (pull up)
	
	DDRD |= (1 << PD6);			// set pin 6 PORTD as output
	
	PCICR |= (1 << PCIE0);		// 
	PCMSK0 |= (1 << PCINT0);
	
	sei();
	
	while (1)
	{
	}
}

ISR(PCINT0_vect)
{	
	cli();
	
	if (!(PINB & (1 << PB0)))
	{
PORTD ^= (1 << PD6);
	}
	
	_delay_ms (5);
	
	sei();
}
