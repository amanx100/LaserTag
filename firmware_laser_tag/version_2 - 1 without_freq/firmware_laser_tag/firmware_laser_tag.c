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

int life=5;
uint8_t got_signal=0, tcnt_read=0, direction=1, direction_2=1;
uint16_t time_light_glow=0, time_light_glow_2=0;
//timing settings
#define time_count 55 //for 5ms @ 7.813kHz// here 39 for 5ms and alternative 55 for 7ms
#define tol 2

void count_tag(void)
{
	got_signal = 2;
	/*
	tcnt_read = TCNT0;
	if ((tcnt_read<=(time_count+tol)) && (tcnt_read>=(time_count-tol)))
		got_signal = got_signal + 1;
	else
		got_signal = 0;
	TCNT0 = 0x00;
	*/
}

ISR(INT0_vect)
{
	count_tag();
}

ISR(INT1_vect)
{
	count_tag();
}

int main(void)
{
	DDRD = (1<<PD5)|(1<<PD4)|(1<<PD7);
	DDRC = 0xFE;
	DDRB = 0x01;
	
	// timer 0 Clock value: 7.813 kHz
	TCCR0=0x05;

	// Clock value: 1000.000 kHz
	TCCR1A=0xA2;
	TCCR1B=0x1A;
	ICR1=100;
	OCR1A=49;
	OCR1B=49;
	
	// Clock value: 1000.000 kHz top FFh
	TCCR2=0x6A;
	OCR2=1;

	
	lcd_init();
	
	GICR |= (1<<INT0)|(1<<INT1);				// Enable INT0
	MCUCSR |= 0x00;					// Trigger INT0 settings
	MCUCR |= (1<<ISC01)|(1<<ISC00)|(1<<ISC11)|(1<<ISC10);	//trigger int0 & 1 on rising edge
	GIFR = (1<<INTF0)|(1<<INTF1);				//Interrupt flag 0 is cleared by writing a logical one into INTF0
	
	lcd_gotoxy(0,1);
	lcd_putsf("LASER TAG GAME");
	_delay_ms(1000);
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_putsf("SET LIFE: ");
	lcd_gotoxy(0,10);
	lcd_putsint_z(life,2);
	
	while(Button_ok)
	{		
		if (!Button_up)
		{
			life = life + 1;
			if (life>99)
			{
				lcd_gotoxy(1,0);
				lcd_putsf("MAXIMUM LEVEL");
				_delay_ms(750);
				lcd_gotoxy(1,0);
				lcd_putsf("             ");
				life = life - 1;
			}
			lcd_gotoxy(0,10);
			lcd_putsint_z(life,2);
			_delay_ms(100);
			while(!Button_up);
			_delay_ms(100);
		}
		else if (!Button_down)
		{
			life = life - 1;
			if (life<1)
			{
				lcd_gotoxy(1,0);
				lcd_putsf("MINIMUM LEVEL");
				_delay_ms(750);
				lcd_gotoxy(1,0);
				lcd_putsf("             ");
				life = life + 1;
			}
			lcd_gotoxy(0,10);
			lcd_putsint_z(life,2);
			_delay_ms(100);
			while(!Button_down);
			_delay_ms(100);
		}
		
		
	}
	_delay_ms(100);
	while(!Button_ok);
	_delay_ms(100);
	
	lcd_gotoxy(0,0);
	lcd_putsf("LIFE   /        ");
	lcd_gotoxy(0,5);
	lcd_putsint_z(life,2);
	lcd_gotoxy(0,8);
	lcd_putsint_z(life,2);
	lcd_gotoxy(1,0);
	lcd_puts("PLAY    ");
	
	
	sei();							//Global Interrupt enable
	
    while(1)
    {
		if (got_signal == 2)
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
			lcd_puts("PLAY    ");
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
		_delay_us(10);
		time_light_glow = time_light_glow + 1;
		if (time_light_glow >= 700)
		{
			time_light_glow = 0;
			if (direction)
			{
				OCR1A = OCR1A + 1;
				OCR1B = OCR1B - 1;
				if (OCR1A >= 98)
				{
					direction = 0;
				}
			}
			else
			{
				OCR1A = OCR1A - 1;
				OCR1B = OCR1B + 1;
				if (OCR1B >= 98)
				{
					direction = 1;
				}
			}
		}
		
		time_light_glow_2 = time_light_glow_2 + 1;
		if (time_light_glow_2 >= 400)
		{
			time_light_glow_2 = 0;
			if (direction_2)
			{
				OCR2 = OCR2 + 5;
				if (OCR2 >= 250)
				{
					direction_2 = 0;
				}
			}
			else
			{
				OCR2 = OCR2 - 3;
				if (OCR2 <= 5)
				{
					direction_2 = 1;
				}
			}
		}
    }
}