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
	DDRC &= ~(1<<PC5); 
	
    while(1)
    {
		if (PINC & (1<<PC5))
		{
			PORTB |= (1<<PB0)|(1<<PB1);
			_delay_ms(117);
			PORTB &= ~(1<<PB0);
			_delay_ms(883);
			PORTB &= ~(1<<PB1);
			while(PINC & (1<<PC5));
		}
		_delay_us(10);
    }
}