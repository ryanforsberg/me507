/*
 * GccApplication1.cpp
 *
 * Created: 10/24/2019 3:56:54 PM
 * Author : melab15
 */ 

#include <avr/io.h>

#define F_CPU 2000000UL // Unsigned long 32 bit
#include <util/delay.h> // Remove this once switching over to Real time operating system

int main(void)
{
	uint8_t ON = 1;
	uint8_t OFF = 0;

	uint8_t state = OFF; // Initial state

	uint16_t	run = 10000;

	PORTA.DIRSET = 0b00010000; // Binary 4: Set directory PA4

	
    while (run!=0) 
    {
		if (state)
		{
			PORTA.OUTCLR = 0b00010000; // clear bit porta.4
			state = OFF;
		}
		else
		{
			PORTA.OUTSET = 0b00010000; // set bit porta.4
			state = ON;
		}
		_delay_us(13); // microseconds delay
		run--;
    }
}

