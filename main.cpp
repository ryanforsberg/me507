/*
 * GccApplication1.cpp
 *
 * Created: 10/24/2019 3:56:54 PM
 * Author : melab15
 */ 

#include <avr/io.h>

#define F_CPU 2000000UL // Unsigned long 32 bit - define CPU frequency
#include <util/delay.h> // Remove this once switching over to Real time operating system

// Custom stuff
// Define PWM Frequency
#define F_PWM 38000U

#define T_ON 0.120*F_PWM*2
#define T_TOT 0.5*F_PWM*2

#define ON 1
#define OFF 0

int main(void)
{


	uint8_t i = 0; //Increment
	uint8_t p = 0; //Pin power
	
	PORTA.DIRSET = 1 << 5; // Set direction PA5
	PORTA.OUTSET = 1 << 5; // Turn off PA5
	
    while (1) 
    {
		_delay_us( (1/F_PWM) *1000 *1000 /2) //define delay by PWM frequency.
		i++;
		if (p == 0 && i < T_ON)
		{
			p = 1;
			PORTA.OUTSET = 1 << 5; //turn on PA5
		}
		else
		{
			PORTA.OUTCLR = 1 << 5; //turn off PA5
		}
		
		if (i > T_TOT)
		{
			i = 0;
		}
    }
}

