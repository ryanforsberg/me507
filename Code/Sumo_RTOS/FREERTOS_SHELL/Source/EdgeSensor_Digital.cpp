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
	
	} 


void EdgeSensor::run(void)
{
	//Share <uint8_t> edge_out->put();  // Define share to be read by Mastermind
	
	/*
	
	
	PORTC_DIRSET = PIN0_bm;	  // Edge CTRL: Pin C0. Set CTRL as output
	PORTC_OUTSET = PIN0_bm;	// Drive CTRL pin high to turn on emitters
	
	PORTA_DIRCLR = PIN2_bm & PIN3_bm & PIN4_bm & PIN5_bm & PIN6_bm & PIN7_bm;  // Edge OUT: Pins A2-A7. Set OUTs as input
	
	PORTD_DIRSET = PIN2_bm;	  // Set Pin 2 of Port D as output for testing
	
	
	for(;;)
	{
		edge_out->put(0);	// Reset output values each loop
		
		if (!(PORTA_IN & PIN2_bm)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(1);    // 00000001
			
			PORTD_OUTSET = PIN2_bm;  // Drive pin D2 high
		}
		
		if (!(PORTA_IN & PIN3_bm)) // If edge detection sensor B output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<1)); // 00000010
			PORTD_OUTSET = PIN2_bm;  // Drive pin D2 high
		}
		
		if (!(PORTA_IN & PIN4_bm)) // If edge detection sensor C output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<2)); // 00000100
			PORTD_OUTSET = PIN2_bm;  // Drive pin D2 high
		}
		
		if (!(PORTA_IN & PIN5_bm)) // If edge detection sensor D output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<3)); // 00001000
			PORTD_OUTSET = PIN2_bm;  // Drive pin D2 high
		}
		
		if (!(PORTA_IN & PIN6_bm)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<4));  // 00010000
			PORTD_OUTSET = PIN2_bm;  // Drive pin D2 high
		}
		
		if (!(PORTA_IN & PIN7_bm)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<5));  // 00100000
			PORTD_OUTSET = PIN2_bm;  // Drive pin D2 high
		}
		
		if ((PORTA_IN & (PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)))
		{
			PORTD_OUTCLR = PIN2_bm;  // Drive pin D2 low
		}
		
		vTaskDelay(1); // Delay task 1ms
	} */
}