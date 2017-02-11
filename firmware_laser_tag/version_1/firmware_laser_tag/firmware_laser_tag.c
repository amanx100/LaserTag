/*
 * firmware_laser_tag.c
 *
 * Created: 4/28/2015 10:45:49 PM
 *  Author: Aman
 */ 

#define buzzer_on (PORTB |= (1<<PB0));
#define buzzer_off (PORTB &= ~(1<<PB0));

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "my_lcd.h"

int life=10;
uint8_t got_signal=0;

ISR(INT0_vect)
{
	got_signal = 1;
}

int main(void)
{
	DDRC = 0xFF;
	DDRB = 0x01;
	
	lcd_init();
	
	GICR |= (1<<INT0);				// Enable INT0
	MCUCSR |= 0x00;					// Trigger INT0 settings
	MCUCR |= (1<<ISC01)|(1<<ISC00);	//trigger int0 on rising edge
	GIFR = (1<<INTF0);				//Interrupt flag 0 is cleared by writing a logical one into INTF0
	
	lcd_gotoxy(0,1);
	lcd_putsf("LASER TAG GAME");
	_delay_ms(1000);
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_putsf("LIFE 10/10");
	
	sei();							//Global Interrupt enable
	
    while(1)
    {
		if (got_signal)
		{
			cli();
			life--;
			lcd_gotoxy(0,5);
			lcd_putsint_z(life,2);
			lcd_gotoxy(1,0);
			lcd_puts("DISABLED");
			buzzer_on;
			_delay_ms(250);
			buzzer_off;
			_delay_ms(1750);
			lcd_gotoxy(1,0);
			lcd_puts("        ");
			got_signal = 0;
			GIFR = (1<<INTF0);
			sei();
		}
		if (life < 1)
		{
			buzzer_on;
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_putsf("DEAD");
			cli();
			while(1);
		}
    }
}