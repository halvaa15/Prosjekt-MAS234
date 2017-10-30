/*
 * Oppgaev_6.3_Scaled.cpp
 *
 * Created: 28.10.2017 16:37:02
 * Author : Fredrik
 */ 

#define F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dutyCycle = 0.0;
int counterDir = 1;
double lightConv = 0.0;



int main(void)
{
	DDRD = (1 << PORTD6);
	
	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
	TIMSK0 = (1 << TOIE0);
	
	sei();
	
	TCCR0B = (1 << CS02) | (1 << CS00);
	OCR0A = (lightConv/100.0)*255.0;
	
	while(1)
	{
	}
}

ISR(TIMER0_OVF_vect)
{	
	if (dutyCycle < 0)
	{
		dutyCycle = 0;
	}
	if (dutyCycle > 100)
	{
		dutyCycle = 100;
	}

	lightConv = (pow(dutyCycle,2)) / (100);
	
	OCR0A = (lightConv/100.0)*255.0;
	
	if (dutyCycle == 100)
	{
		counterDir = 0;
	}
	else if (dutyCycle == 0)
	{
		counterDir = 1.;
	}
	
	if (counterDir == 1)
	{
		dutyCycle += 1.64;
	}
	
	else if (counterDir == 0)
	{
		dutyCycle -= 1.64;
	}
}

