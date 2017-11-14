/*
 * Oppgavee_8.4_ADC.cpp
 *
 * Created: 29.10.2017 14:38:51
 * Author : Fredrik
 */ 

#define F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dutyCycle = 0.0;
double lightConv = 0.0;
double analogverdi = 0.0;

// initialize adc
void ADC_init()
{
	DDRC &= ~(1 << PC0); // set PC0 as input
	
	ADMUX = (1 << REFS0);	// set reference selection to AVCC with external capacitor at AREF pin
	
	// enables  ADC with auto trigger, starts conversion and sets prescaler to clk/128
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void PWM_init_8bit()	// initializing PWM 8-bit
{
	DDRD = (1 << PD6);	// set PD6 as output
	
	TCCR1A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);	// set FAST 8-bit PWM, with clear OC0A on compare
	TIMSK0 = (1 << TOIE0);	// set interrupt register to overflow interrupt
	
	sei();	// enables external interrupt
	
	TCCR0B = (1 << CS02) | (1 << CS00);	// set prescaling register to clk/1024
	OCR0A = (dutyCycle/100.0)*255.0;	// set clear on compare value
}

int main(void)
{
	ADC_init();	// enables ADC
	PWM_init_8bit();	// enables FAST PWM 8-bit
	
	while(1)
	{
	}
}

ISR(ADC_vect)
{
	analogverdi = static_cast<double>(ADC);	// converts ADC value from uint16_t to double
	dutyCycle = (analogverdi/1023.0)*100.0;	// converts analogverdi to a 0-100 value (need 0-100 value for existing PWM program)
}

ISR(TIMER0_OVF_vect)
{	
	if (dutyCycle < 0.0)	// Prevents dutycycle from decreasing below 100 (causing OCR0A error)
		{
			dutyCycle = 0.0;
		}
	if (dutyCycle > 100.0)// Prevents dutycycle from increasing above 100 (causing OCR0A error)
		{
			dutyCycle = 100.0;
		}

	lightConv = (pow(dutyCycle,2)) / (100);	// converts calculated light value to perceived light value

	OCR0A = (lightConv/100.0)*255.0;	// updates OCR0A value at every interrupt
}



