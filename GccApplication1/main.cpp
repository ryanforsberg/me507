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

	PORTA.DIRSET = 0b00010000; // Binary 4: Set directory PA4

	
    while (1) 
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
		_delay_ms(26); // milliseconds delay
    }
}

