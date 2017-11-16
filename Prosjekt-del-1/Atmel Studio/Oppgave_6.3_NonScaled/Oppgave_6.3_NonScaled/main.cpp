/*
 * Oppgave_6.3_NonScaled.cpp
 *
 * Created: 28.10.2017 16:21:20
 * Author : Fredrik
 */ 

#define F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dutyCycle = 0.0;	// double is used due to value with decimal points
int counterDir = 1;

void PWM_init_8bit()	// initializing function for PWM 8-bit
{
	DDRD |= (1 << PD6);	// set PD6 as output
	
	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);	// set FAST 8-bit PWM, with clear OC0A on compare
	TIMSK0 = (1 << TOIE0);	// set interrupt register to overflow interrupt
	
	sei();	// set global interrupt enable
	
	TCCR0B = (1 << CS02);	// set prescaling register to clk/256
	OCR0A = (dutyCycle/100.0)*255.0;	// set clear on compare value
}

int main(void)
{
	PWM_init_8bit(); // initializing FAST PWM 8-bit
	
	while(1)
	{
	}
}

ISR(TIMER0_OVF_vect)	// timer0/counter0 overflow interrupt vector
{
	if (dutyCycle < 0.0)	// Prevents dutycycle from decreasing below 100 (causing OCR0A error)
	{
		dutyCycle = 0.0;
	}
	if (dutyCycle > 100.0)// Prevents dutycycle from increasing above 100 (causing OCR0A error)
	{
	dutyCycle = 100.0;		
	}

	OCR0A = (dutyCycle/100.0)*255.0;	// updates OCR0A value at every interrupt
	
	if (dutyCycle >= 100.0)	// flag for dimming down
	{
		counterDir = 0;
	}
	else if (dutyCycle <= 0.0)	// flag for dimming up
	{
		counterDir = 1.;
	}
	
	if (counterDir == 1)	// increases dutycycle at each interrupt
	{
		dutyCycle += 0.4096;	// 0.4096 sets blinking cycle to 2s
	}
	else if (counterDir == 0)	// decreases dutycycle at each interrupt
	{
		dutyCycle -= 0.4096;	// 0.4096 sets blinking cycle to 2s
	}
}
