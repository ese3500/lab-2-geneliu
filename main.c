/*
 * GccApplication1.c
 *
 * Created: 2/9/2023 5:30:05 PM
 * Author : genel
 */ 
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart.h"

char buffer[25];
volatile int rise_tick = 0;
volatile int fall_tick = -1;
volatile int morse_code = 1;
volatile int light_on_tick = -1;

void init() 
{
	cli();
	//gpio setup
	DDRB &= ~(1 << DDB0);
	DDRB |= (1<<DDB5);
	DDRB |= (1<<DDB1);
	DDRB |= (1<<DDB4);
	
	//timer prescale by 256
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	//first look for rising edge
	TCCR1B |= (1<<ICES1);
	
	TIFR1 |= (1<<ICF1);
	
	TIMSK1 |= (1<<ICIE1);
	
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	//rising edge handling
	if (TCCR1B & (1<<ICES1)){
		//slight debounce handling to avoid mechanical retriggers
		if (fall_tick != -1 && ICR1 > fall_tick && ICR1 - fall_tick < 1500) return;
		rise_tick = ICR1;
		fall_tick = -1;
		PORTB |= (1<<PORTB5);
		TCCR1B &= ~(1<<ICES1);
	//falling edge handling
	} else {
		int ticks = ICR1 - rise_tick;
		if (ICR1 < rise_tick) {
			ticks = 62500 + ticks;
		}
		if (ticks >= 1875 && ticks < 12500) {
			morse_code = (morse_code << 1);
			PORTB |= (1<<PORTB4);
			fall_tick = ICR1;
		} else if (ticks >= 12500 && ticks < 25000) {
			morse_code = (morse_code << 1) + 1;
			PORTB |= (1<<PORTB1);
			fall_tick = ICR1;
		}
		PORTB &= ~(1<<PORTB5);
		TCCR1B |= (1<<ICES1);
	}
}

int main(void)
{
	UART_init(BAUD_PRESCALER); 
	init();
    while (1) 
    {
		//space detection and handling
		if (fall_tick != -1) {
			int empty = TCNT1 - fall_tick;
			if (empty < 0) {
				empty = 62500 + empty;
			}
			if (empty > 3125) {
				PORTB &= ~(1<<PORTB1);
				PORTB &= ~(1<<PORTB4);
			}
			if ((empty >= 25000)) {
				 fall_tick = -1;
				 switch (morse_code) {
					 case (0b101):
						sprintf(buffer, "A");
						break;
					case (0b11000):
						sprintf(buffer, "B");
						break;
					case (0b11010):
						sprintf(buffer, "C");
						break;
					case (0b1100):
						sprintf(buffer, "D");
						break;
					case (0b10):
						sprintf(buffer, "E");
						break;
					case (0b10010):
						sprintf(buffer, "F");
						break;
					case (0b1110):
						sprintf(buffer, "G");
						break;
					case (0b10000):
						sprintf(buffer, "H");
						break;
					case (0b100):
						sprintf(buffer, "I");
						break;
					case (0b10111):
						sprintf(buffer, "J");
						break;
					case (0b1101):
						sprintf(buffer, "K");
						break;
					case (0b10100):
						sprintf(buffer, "L");
						break;
					case (0b111):
						sprintf(buffer, "M");
						break;
					case (0b110):
						sprintf(buffer, "N");
						break;
					case (0b1111):
						sprintf(buffer, "O");
						break;
					case (0b10110):
						sprintf(buffer, "P");
						break;
					case (0b11101):
						sprintf(buffer, "Q");
						break;
					case (0b1010):
						sprintf(buffer, "R");
						break;
					case (0b1000):
						sprintf(buffer, "S");
						break;
					case (0b11):
						sprintf(buffer, "T");
						break;
					case (0b1001):
						sprintf(buffer, "U");
						break;
					case (0b10001):
						sprintf(buffer, "V");
						break;
					case (0b1011):
						sprintf(buffer, "W");
						break;
					case (0b11001):
						sprintf(buffer, "X");
						break;
					case (0b11011):
						sprintf(buffer, "Y");
						break;
					case (0b11100):
						sprintf(buffer, "Z");
						break;
					case (0b101111):
						sprintf(buffer, "1");
						break;
					case (0b100111):
						sprintf(buffer, "2");
						break;
					case (0b100011):
						sprintf(buffer, "3");
						break;
					case (0b100001):
						sprintf(buffer, "4");
						break;
					case (0b100000):
						sprintf(buffer, "5");
						break;
					case (0b110000):
						sprintf(buffer, "6");
						break;
					case (0b111000):
						sprintf(buffer, "7");
						break;
					case (0b111100):
						sprintf(buffer, "8");
						break;
					case (0b111110):
						sprintf(buffer, "9");
						break;
					case (0b111111):
						sprintf(buffer, "0");
						break;
				 }
				 UART_putstring(buffer);
				 morse_code = 1;
			 }
		}
    }
}

