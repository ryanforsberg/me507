/*
 * EdgeSensor.cpp
 *
 * Created: 11/17/2019 2:37:51 PM
 * Author : Ryan Forsberg
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>                        // Watchdog timer header

#include "shared_data_sender.h"
#include "shared_data_receiver.h"
#include "EdgeSensor_Digital.h"
#define F_CPU 32000000UL  // needed for accurate Task Delay

EdgeSensor::EdgeSensor(const char* a_name,
						unsigned portBASE_TYPE a_priority,
						size_t a_stack_size,
						emstream* p_ser_dev
						)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
	{
	
	} //MotorDriver


void EdgeSensor::run(void)
{
	//Share <uint8_t> edge_out->put();  // Define share to be read by Mastermind
	
	PORTC_DIRSET = 1;	  // Edge CTRL: Pin C0. Set CTRL as output
	PORTA_DIRCLR = 0b11111100;  // Edge OUT: Pins A2-A7. Set OUTs as input
/*
	for(;;)
	{
		edge_out->put(0);	// Reset output values each loop
		
		if (!(PORTA_IN.A2)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(1);    // 00000001
			
			PORTD_OUTSET = (1<<2);  // Drive pin D2 high
		}
		else
		{
			PORTD_OUTCLR = (1<<2);  // Drive pin D2 low
		}
		
		if (!(PORTA_IN.A3)) // If edge detection sensor B output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<1)); // 00000010
		}
		
		if (!(PORTA_IN.A4)) // If edge detection sensor C output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<2)); // 00000100
		}
		
		if (!(PORTA_IN.A5)) // If edge detection sensor D output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<3)); // 00001000
		}
		
		if (!(PORTA_IN.A6)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<4));  // 00010000
		}
		
		if (!(PORTA_IN.A7)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<5));  // 00100000
		}
		
		vTaskDelay(1); // Delay task 1ms
	}*/
}