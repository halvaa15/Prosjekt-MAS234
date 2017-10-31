/*
 * Oppgave 6.2.cpp
 *
 * Created: 28.10.2017 14:45:56
 * Author : Fredrik
 */ 

#define F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dutyCycle = 100.0;



int main(void)
{
	DDRB = (1 << PB1);
	
	TCCR1A = (1 << COM1A1) | (1 << WGM10) | (1 << WGM11) | (1 << WGM12);
	TIMSK1 = (1 << TOIE1);
	
	sei();
	
	TCCR1B = (1 << CS12) | (1 << CS10);
	OCR1A = (dutyCycle/100.0)*1023;
	
	while(1)
	{
	}
}

ISR(TIMER1_OVF_vect)
{
}