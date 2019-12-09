/*********
* @file		EdgeSensor_Digital.cpp
* @brief	The .cpp file for the Edge Detection task.
* @details	This file contains the two functions needed for the Edge Detection task. They are:
*			\li constructor function - passes the needed parameters to the constructor of the
*			base class.
*			\li Run function - called by the scheduler. Contains an initial setup and then
*			an infinite loop.
*
*  @date 2019-12-2 RRF Original file: Header file include and constructor setup. New file created when switching from analog to digital input
*  @date 2019-12-3 RRF MCU pin setup
*  @date 2019-12-4 RRF Correctly read each sensor and adjust share value accordingly
*
*	License:
*	You can have the header file, but this file is licensed only under an hourly rental.
*	The fee is $1000/hour plus 10% of any profits made. How else am I supposed to get through
*	writing all of this documentation?
*
* Created: 12/2/2019 10:42:35 AM
* Author: Ryan Forsberg
*/

/**	@brief		Detects the edge of the sumo ring using Pololu QTR IR sensors
 *	@details	Sets up sensors by turning on LED emitters, sets up input pins to read sensor output signals, and adjusts share value accordingly when an edge is detected
 */
//*****************************************************************************


#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>                        // Watchdog timer header

#include "shared_data_sender.h"				// Set up Shares
#include "shared_data_receiver.h"
#include "EdgeSensor_Digital.h"				// Header File
#define F_CPU 32000000UL  // needed for accurate Task Delay

//-------------------------------------------------------------------------------------
// Constructor
EdgeSensor::EdgeSensor(const char* a_name,
						unsigned portBASE_TYPE a_priority,
						size_t a_stack_size,
						emstream* p_ser_dev
						)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
	{
	
	} 

//-------------------------------------------------------------------------------------
// Run function: consists of pin setup and infinite loop of read sensor output signals
void EdgeSensor::run(void)
{	
	PORTC_DIRSET = PIN0_bm;	  // Edge CTRL: Pin C0. Set CTRL as output
	PORTC_OUTSET = PIN0_bm;	// Drive CTRL pin high to turn on emitters
	
	PORTA_DIRCLR = PIN2_bm & PIN3_bm & PIN4_bm & PIN5_bm & PIN6_bm & PIN7_bm;  // Edge OUT: Pins A2-A7. Set OUTs as input	
	
	for(;;)
	{
		edge_out->put(0);	// Reset output values each loop
		
		if (!(PORTA_IN & PIN2_bm)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(1);    // 00000001
		}
		
		if (!(PORTA_IN & PIN3_bm)) // If edge detection sensor B output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<1)); // 00000010
		}
		
		if (!(PORTA_IN & PIN4_bm)) // If edge detection sensor C output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<2)); // 00000100
		}
		
		if (!(PORTA_IN & PIN5_bm)) // If edge detection sensor D output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<3)); // 00001000
		}
		
		if (!(PORTA_IN & PIN6_bm)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<4));  // 00010000
		}
		
		if (!(PORTA_IN & PIN7_bm)) // If edge detection sensor A output is low (reading white)
		{
			edge_out->put(edge_out->get() | (1<<5));  // 00100000
		}
		
		vTaskDelay(1); // Delay task 1ms
	} 
}