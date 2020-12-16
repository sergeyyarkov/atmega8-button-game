
#ifndef F_CPU
#define F_CPU 8000000UL
#endif


#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

#define BUTTON1 0 // PORT C to PIN 0
#define BUTTON2 1 // PORT C to PIN 1
#define MAX_LED_COUNT 16 // COUNT OF LEDS ON BOARD

void init_ports() {
	
	/************************************************************************/
	/* 1st led row                                                           */
	/************************************************************************/
	DDRB = 0xFF;
	PORTB = 0x00;

	
	/************************************************************************/
	/* 2nd led row                                                          */
	/************************************************************************/
	DDRD = 0xFF;
	PORTD = 0x00;
	
	/************************************************************************/
	/* buttons and led for player 1 and 2                                    */
	/************************************************************************/
	DDRC &= ~(1<<BUTTON1);
	DDRC &= ~(1<<BUTTON2);
	DDRC |= 1<<PC2;
	PORTC |= 1<<BUTTON1; // pull-up resistor on input PC0
	PORTC |= 1<<BUTTON2; // pull-up resistor on input PC1
	
}

unsigned char btn_state_one()
{
	if (bit_is_clear(PINC, BUTTON1)) {
		_delay_ms(10);
		return 1;
	}
	
	return 0;
}

unsigned char btn_state_second()
{
	if (bit_is_clear(PINC, BUTTON2)) {
		_delay_ms(10);
		return 1;
	}
	
	return 0;
}

void indicate_row_A()
{
	PORTB = 0xAA;
	_delay_ms(10);
	PORTB = 0x00;
	PORTB |= ~0xAA;
	_delay_ms(10);
}

void indicate_row_B()
{
	PORTD = 0xAA;
	_delay_ms(10);
	PORTD = 0x00;
	PORTD |= ~0xAA;
	_delay_ms(10);
}

int main(void)
{
	init_ports();
	
	uint8_t player_count_one = 0;
	uint8_t player_count_second = 0;
	uint8_t player_isPressed_one = 0;
	uint8_t player_isPressed_second = 0;
	
	while (1)
	{
		if (player_count_one == MAX_LED_COUNT / 2 && player_count_second != MAX_LED_COUNT / 2) {
			PORTC |= 1<<PC2;
			
			for (uint8_t i = 0; i < 7; i++)
			{
				indicate_row_A();
			}
			
			_delay_ms(100);
			
			player_count_one = 0;
			player_count_second = 0;
			
			PORTC &= ~(1<<PC2);
			PORTB = 0x00;
			PORTD = 0x00;
		}

		if (player_count_second == MAX_LED_COUNT / 2 && player_count_one != MAX_LED_COUNT / 2) {
			
			PORTC |= 1<<PC2;
			for (uint8_t i = 0; i < 7; i++)
			{
				indicate_row_B();
			}
			_delay_ms(100);
			
			player_count_second = 0;
			player_count_one = 0;
			
			PORTC &= ~(1<<PC2);
			PORTD = 0x00;
			PORTB = 0x00;
		}
		
		if (btn_state_one()) {
			if (player_isPressed_one == 0) {
				PORTB |= 1<<player_count_one;
				player_count_one++;
				player_isPressed_one = 1;
			}
			} else {
			player_isPressed_one = 0;
		}
		
		if (btn_state_second()) {
			if (player_isPressed_second == 0) {
				PORTD |= 1<<player_count_second;
				player_count_second++;
				player_isPressed_second = 1;
			}
			} else {
			player_isPressed_second = 0;
		}
	}
}

