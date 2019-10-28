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
#define F_PWM 38000UL	// Frequency of PWM signal, Hertz

#define T_ON 0.005*F_PWM*2UL  // Duration of on signal, seconds
#define T_TOT 0.5*F_PWM*2UL   // Total time of a full signal cycle, seconds

#define ON 1
#define OFF 0

int main(void)
{


	uint16_t i = 0; //Increment
	uint8_t p = 0; //Pin power
	
	PORTA.DIRSET = 1 << 5; // Set direction PA5
	PORTA.OUTSET = 1 << 5; // Turn off PA5
	
    while (1) 
    {
		i++;
		if (p == 0 && i < T_ON) // If power is off and increment is within the time range of when it should be on
		{
			p = 1;	// Raise power flag
			PORTA.OUTSET = 1 << 5; //turn on PA5
			_delay_us( (1/F_PWM) *1000 *1000 /2); //define delay by PWM frequency. Delay
		}
		else  
		{
			PORTA.OUTCLR = 1 << 5; //turn off PA5
			p=0;
			_delay_us( (1/F_PWM) *1000 *1000 /2); //define delay by PWM frequency. Delay
		}
		
		if (i > T_TOT)
		{
			i = 0;
		}
    }
}