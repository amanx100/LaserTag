/*
 * firmware_laser_tag.c
 *
 * Created: 4/28/2015 10:45:49 PM
 *  Author: Aman
 */ 

#define LED_A_ON (PORTD |= (1<<PD5))
#define LED_A_OFF (PORTD &= ~(1<<PD5))
#define LED_B_ON (PORTD |= (1<<PD4))
#define LED_B_OFF (PORTD &= ~(1<<PD4))
#define LED_FRZ_ON (PORTD |= (1<<PD6))
#define LED_FRZ_OFF (PORTD &= ~(1<<PD6))


#define SHOOT_P_ON (PORTD |= (1<<PD7))
#define SHOOT_P_OFF (PORTD &= ~(1<<PD7))

#define GK_ON (PORTC |= (1<<PC0))
#define GK_OFF (PORTC &= ~(1<<PC0))
#define GL_ON (PORTC |= (1<<PC1))
#define GL_OFF (PORTC &= ~(1<<PC1))

#define BEEP_ON (PORTB |= (1<<PB0));
#define BEEP_OFF (PORTB &= ~(1<<PB0));

#define SHOTED_B 1
#define SHOTED_F 2
#define SHOT_RQST 3
#define NT 0
#define ON 1
#define OFF 0


#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t INP=0, LED_LATCH=0, SHTP=1;

ISR(INT0_vect) //if he get shot by back side
{
	INP = SHOTED_B;
}

ISR(INT1_vect) //if he get shot by front side
{
	INP = SHOTED_F;
}

ISR(INT2_vect) // request to permit shoot
{
	INP = SHOT_RQST;
	if (SHTP==OFF)
	{
		BEEP_ON;
		_delay_ms(100);
		BEEP_OFF;
		_delay_ms(50);
		BEEP_ON;
		_delay_ms(100);
		BEEP_OFF;
	}
}

ISR(TIMER1_COMPA_vect)
{
	if (LED_LATCH)
	{
		LED_B_OFF;
		LED_A_ON;
		LED_LATCH=0;
	}
	else
	{
		LED_A_OFF;
		LED_B_ON;
		LED_LATCH=1;
	}
}

int main(void)
{
	DDRB = 0x01;
	DDRC = 0x03;
	DDRD = (1<<4)|(1<<5)|(1<<6)|(1<<7);

	// Clock value: 7.813 kHz
	// Mode: Normal top=FFFFh
	// Compare A Match Interrupt: On
	TCCR1A=0x00;
	TCCR1B=0x0D;
	OCR1A=3907;

	GICR |= (1<<INT0)|(1<<INT1)|(1<<INT2);
	MCUCSR &= ~(1<<ISC2);					// falling edge
	MCUCR |= (1<<ISC01)|(1<<ISC11);	//falling edge
	GIFR = (1<<INTF0)|(1<<INTF1)|(INTF2);
	
	TIMSK=0x10;
	
	sei();	//Global Interrupt enable
	
    while(1)
    {
		if (INP==SHOTED_B)
		{
			GL_ON;
			BEEP_ON;
			LED_FRZ_ON;
			SHTP=OFF;
			_delay_ms(250);
			GL_OFF;
			BEEP_OFF;
			_delay_ms(4550);
			SHTP=ON;
			_delay_ms(200);
			LED_FRZ_OFF;
			INP = NT;
		}
		else if (INP==SHOTED_F)
		{
			GL_ON;
			BEEP_ON;
			LED_FRZ_ON;
			SHTP=OFF;
			_delay_ms(250);
			GL_OFF;
			BEEP_OFF;
			_delay_ms(9550);
			SHTP=ON;
			_delay_ms(200);
			LED_FRZ_OFF;
			INP = NT;
		}
		else if (INP==SHOT_RQST)
		{
			INP = NT;
			SHOOT_P_ON;
			GK_ON;
			_delay_ms(200);
			SHOOT_P_OFF;
			GK_OFF;
		}
		_delay_us(10);
    }
}