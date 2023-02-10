/*
 * GccApplication1.c
 *
 * Created: 2/9/2023 5:30:05 PM
 * Author : genel
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init() 
{
	cli();
	DDRB &= ~(1 << DDB0);
	DDRB |= (1<<DDB5);
	
	TCCR1B &= ~(1<<CS10);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12);
	
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	TCCR1B |= (1<<ICES1);
	
	TIFR1 |= (1<<ICF1);
	
	TIMSK1 |= (1<<ICIE1);
	
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	PORTB ^= (1<<PORTB5);
	TCCR1B ^= (1<<ICES1);
}

int main(void)
{
	init();
    while (1) 
    {
    }
}

