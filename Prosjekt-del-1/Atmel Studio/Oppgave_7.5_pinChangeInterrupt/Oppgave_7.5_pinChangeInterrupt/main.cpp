/*
 * Oppgave_7.5_pinChangeInterrupt.cpp
 *
 * Created: 28.10.2017 16:53:38
 * Author : Fredrik
 */ 

#define F_CPU   16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void PCI_init()
{
	DDRB &= ~(1 << PB0);		// set PB0 as input
	PORTB |= (1 << PB0);		// set PB0 true (pull up)	
	
	PCICR |= (1 << PCIE0);		// use PCIE0 (PB0) as pin change interrupt
	PCMSK0 |= (1 << PCINT0);	// set pin change mask as PCINT0
	
	sei();						// enables external interrupt
}

int main(void)
{
	DDRD |= (1 << PD6);			// set PD6 as output
	PCI_init();					// enables pin change interrupt

	while (1)	
	{
	}
}


ISR(PCINT0_vect)
{	
	
	cli();						// disables global interrupt (to prevent de-bounce)
	
	_delay_ms (5);
	
	sei();						// enables global interrupt
	
	if (!(PINB & (1 << PB0)))	// is set high when button is pressed (input = false)
	{
		PORTD ^= (1 << PD6);	// toggles led
	}

}
