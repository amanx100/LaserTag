/*
 * firmware.c
 *
 * Created: 8/12/2015 1:37:43 PM
 *  Author: Aman
 */ 

#define PINA_0 (PINA & (1<<0))
#define PINA_1 (PINA & (1<<1))
#define PINA_2 (PINA & (1<<2))
#define PINA_3 (PINA & (1<<3))
#define PINA_4 (PINA & (1<<4))
#define PINA_5 (PINA & (1<<5))
#define PINA_6 (PINA & (1<<6))
#define PINA_7 (PINA & (1<<7))

#define PINC_0 (PINC & (1<<0))
#define PINC_1 (PINC & (1<<1))
#define PINC_2 (PINC & (1<<2))
#define PINC_3 (PINC & (1<<3))
#define PINC_4 (PINC & (1<<4))
#define PINC_5 (PINC & (1<<5))
#define PINC_6 (PINC & (1<<6))
#define PINC_7 (PINC & (1<<7))

#define PINB_0 (PINB & (1<<0))
#define PINB_1 (PINB & (1<<1))
#define PINB_2 (PINB & (1<<2))
#define PINB_3 (PINB & (1<<3))

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "myusart.h"

int main(void)
{
	uint8_t transmission=0;
	DDRA = 0x00;
	
	//unused pin assigned as output.. // make it input when need
	DDRB = 0x0F;
	PORTB = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	///////////////////
	
	usart_init(14400);
	
    while(1)
    {
        if (PINA_0)
        {
			if (transmission)
			{
				_delay_ms(10);
				if (PINA_1)
				{
					usart_putstr("111+\r");
				}
				else
				{
					usart_putstr("111-\r");
				}
				transmission=0;
			}
        }
		else if (PINA_2)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINA_3)
				{
					usart_putstr("112+\r");
				}
				else
				{
					usart_putstr("112-\r");
				}
				transmission=0;
			}
		}
		else if (PINA_4)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINA_5)
				{
					usart_putstr("113+\r");
				}
				else
				{
					usart_putstr("113-\r");
				}
				transmission=0;
			}
		}
		else if (PINA_6)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINA_7)
				{
					usart_putstr("114+\r");
				}
				else
				{
					usart_putstr("114-\r");
				}
				transmission=0;
			}
		}
		else if (PINC_7)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINC_6)
				{
					usart_putstr("115+\r");
				}
				else
				{
					usart_putstr("115-\r");
				}
				transmission=0;
			}
		}
		else if (PINC_5)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINC_4)
				{
					usart_putstr("116+\r");
				}
				else
				{
					usart_putstr("116-\r");
				}
				transmission=0;
			}
		}
		else if (PINC_3)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINC_2)
				{
					usart_putstr("117+\r");
				}
				else
				{
					usart_putstr("117-\r");
				}
				transmission=0;
			}
		}
		else if (PINC_1)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINC_0)
				{
					usart_putstr("118+\r");
				}
				else
				{
					usart_putstr("118-\r");
				}
				transmission=0;
			}
		}
		else if (PINB_0)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINB_1)
				{
					usart_putstr("119+\r");
				}
				else
				{
					usart_putstr("119-\r");
				}
				transmission=0;
			}
		}
		else if (PINB_2)
		{
			if (transmission)
			{
				_delay_ms(10);
				if (PINB_3)
				{
					usart_putstr("120+\r");
				}
				else
				{
					usart_putstr("120-\r");
				}
				transmission=0;
			}
		}
		else
		{
			transmission=1;
		}
    }
}