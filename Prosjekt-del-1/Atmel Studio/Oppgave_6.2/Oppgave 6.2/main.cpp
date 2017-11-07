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

void PWM_init_8bit()	// initializing PWM 8-bit
{	
	DDRD = (1 << PD6);	// set PD6 as output
	
	TCCR1A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);	// set FAST 8-bit PWM, with clear OC0A on compare
	TIMSK0 = (1 << TOIE0);	// set interrupt register to overflow interrupt
	
	sei();	// enables external interrupt
	
	TCCR0B = (1 << CS02);	// set prescaling register to clk/256
	OCR0A = (dutyCycle/100.0)*255;	// set clear on compare value
}

void PWM_init_10bit()	// initializing PWM 10-bit
{
	DDRB = (1 << PB1);	// set PB1 as output
	
	TCCR1A = (1 << COM1A1) | (1 << WGM10) | (1 << WGM11) | (1 << WGM12);	// set FAST 10-bit PWM, with clear OC1A on compare
	TIMSK1 = (1 << TOIE1);	// set interrupt register to overflow interrupt
	
	sei();	// enables external interrupt
	
	TCCR1B = (1 << CS12) | (1 << CS10);	// set prescaling register to clk/1024
	OCR1A = (dutyCycle/100.0)*1023;	// set clear on compare value	
}

int main(void)
{
	PWM_init_10bit();	// enables FAST PWM 10-bit
//	PWM_init_8bit();	// enables FAST PWM 8-bit
	
	while(1)
	{
	}
}

ISR(TIMER1_OVF_vect)
{
}