/*
 * firmware_laser_tag.c
 *
 * Created: 4/28/2015 10:45:49 PM
 *  Author: Aman
 */ 

#define Button_up (PINA & (1<<PA2))
#define Button_ok (PINA & (1<<PA1))
#define Button_down (PINA & (1<<PA0))

#define buzzer_on (PORTB |= (1<<PB0));
#define buzzer_off (PORTB &= ~(1<<PB0));

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "my_lcd.h"

uint8_t got_signal=0, tcnt_read=0, direction=1;
uint16_t time_light_glow=0;
//timing settings
#define time_count 55 //for 5ms @ 7.813kHz// here 39 for 5ms and alternative 55 for 7ms
#define tol 2

void count_tag(void)
{
	got_signal = 2;
}

ISR(INT0_vect) //if he get shotted
{
	count_tag();
}

ISR(INT1_vect) //if he shot oters
{
	PORTD |= (1<<PD1);
	_delay_ms(500);
	PORTD &= ~(1<<PD1);
}

int main(void)
{
	DDRD = (1<<PD0)|(1<<PD1)|(1<<PD5)|(1<<PD4)|(1<<PD7);  //for led control //pd7 is for gun power
	DDRC = 0xFE;
	DDRB = 0x01;
	PORTD &= ~(1<<PD7);
	
	// timer 0 Clock value: 7.813 kHz
	TCCR0=0x05;

	// Clock value: 1000.000 kHz
	TCCR1A=0xA2;
	TCCR1B=0x1A;
	ICR1=100;
	OCR1A=49;
	OCR1B=49;
	
	// Clock value: 1000.000 kHz top FFh
	//TCCR2=0x6A;
	//OCR2=1;

	
	lcd_init();
	
	GICR |= (1<<INT0)|(1<<INT1); //|(1<<INT2);				// Enable INT0
	//MCUCSR |= (1<<ISC2);					// Trigger INT0 settings
	MCUCR |= (1<<ISC01)|(1<<ISC00)|(1<<ISC11)|(1<<ISC10);	//trigger int0, 1 & 2 on rising edge
	GIFR = (1<<INTF0)|(1<<INTF1); //|(INTF2);				//Interrupt flag 0 is cleared by writing a logical one into INTF0
	
	lcd_gotoxy(0,1);
	lcd_putsf("LASER TAG GAME");
	_delay_ms(1000);
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_putsf("Press OK to PLAY");
	
	while(Button_ok)
	{		
		if (!Button_up)
		{
			lcd_gotoxy(1,0);
			lcd_putsf("No Operation");
			_delay_ms(300);
			lcd_gotoxy(1,0);
			lcd_putsf("            ");
		}
		else if (!Button_down)
		{
			lcd_gotoxy(1,0);
			lcd_putsf("No Operation");
			_delay_ms(300);
			lcd_gotoxy(1,0);
			lcd_putsf("            ");
		}
		_delay_us(10);
	}
	_delay_ms(100);
	while(!Button_ok);
	_delay_ms(100);
	
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_putsf("GAME IS RUNNING");
	lcd_gotoxy(1,0);
	lcd_puts("PLAY..");
	
	PORTD |= (1<<PD7);
	sei();							//Global Interrupt enable
	
    while(1)
    {
		if (got_signal == 2)
		{
			PORTD &= ~(1<<PD7);
			cli();
			lcd_gotoxy(1,0);
			lcd_puts("DISABLED");
			PORTD |= (1<<PD0);
			buzzer_on;
			_delay_ms(250);
			buzzer_off;
			_delay_ms(250);
			PORTD &= ~(1<<PD0);
			_delay_ms(2250);
			lcd_gotoxy(1,0);
			lcd_puts("PLAY..  ");
			got_signal = 0;
			GIFR = (1<<INTF0)|(1<<INTF1);//|(INTF2);
			sei();
			PORTD |= (1<<PD7);
		}
		_delay_us(10);
		time_light_glow = time_light_glow + 1;
		if (time_light_glow >= 700)
		{
			time_light_glow = 0;
			if (direction)
			{
				OCR1A = OCR1A + 3;
				OCR1B = OCR1B - 3;
				if (OCR1A >= 98)
				{
					direction = 0;
				}
			}
			else
			{
				OCR1A = OCR1A - 3;
				OCR1B = OCR1B + 3;
				if (OCR1B >= 98)
				{
					direction = 1;
				}
			}
		}
    }
}