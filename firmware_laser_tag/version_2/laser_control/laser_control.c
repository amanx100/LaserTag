/*
 * laser_control.c
 *
 * Created: 6/7/2015 2:12:39 AM
 *  Author: Aman
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = (1<<PB0)|(1<<PB1);
	
	// Clock value: 1000.000 kHz
	TCCR1A=0x82;
	TCCR1B=0x1A;
	ICR1=7000;
	OCR1A=3500;


	
    while(1)
    {
		PORTB |= (1<<PB0);
		_delay_ms(117);
		PORTB &= ~(1<<PB0);
		_delay_ms(1000);
		TCCR1A=0x00;
		TCCR1B=0x00;
		PORTB &= ~(1<<PB1);
		DDRB = 0x00;
		while(1);
    }
}