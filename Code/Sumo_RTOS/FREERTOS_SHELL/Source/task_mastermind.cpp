/* 
* mastermind.cpp
*
* Created: 12/4/2019 10:34:10 AM
* Author: Phil
*/

#define startdelay 5000
#define rundelay 500

#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header

#include "shared_data_sender.h"
#include "shared_data_receiver.h"
#include "task_mastermind.h"

// default constructor
task_mastermind::task_mastermind(const char* a_name,
						unsigned portBASE_TYPE a_priority,
						size_t a_stack_size,
						emstream* p_ser_dev
						)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
	
} //mastermind

void task_mastermind::run(void)
{
	delay_ms(startdelay); //wait a second for the user to be ready
	while(1)
	{
		delay_ms(rundelay);
		current_state = get_state();
		switch(state)
		{
			case(FORWARD):
				commForward->put(true);
				commBackward->put(false);
				commLeft->put(false);
				commRight->put(false);
				commEffort->put(0x1999);		//10% effort
				transition_to(BACKWARD);
			break;
			case(BACKWARD):
				commForward->put(false);
				commBackward->put(true);
				commLeft->put(false);
				commRight->put(false);
				commEffort->put(0x1999);		//10% effort
				transition_to(FORWARD);
			break;
			case(PIVOT_RIGHT):
				commForward->put(false);
				commBackward->put(false);
				commLeft->put(false);
				commRight->put(true);
				commEffort->put(0x1999);		//10% effort
				transition_to(PIVOT_LEFT);
			break;
			case(PIVOT_LEFT):
				commForward->put(false);
				commBackward->put(false);
				commLeft->put(true);
				commRight->put(false);
				commEffort->put(0x1999);		//10% effort
				transition_to(FORWARD_LEFT);
			break;
			case(FORWARD_LEFT):
				commForward->put(true);
				commBackward->put(false);
				commLeft->put(true);
				commRight->put(false);
				commEffort->put(0x1999);		//10% effort
				transition_to(FORWARD_RIGHT);
			break;
			case(FORWARD_RIGHT):
				commForward->put(true);
				commBackward->put(false);
				commLeft->put(false);
				commRight->put(true);
				commEffort->put(0x1999);		//10% effort
				transition_to(BACKWARD_LEFT);
			break;
			case(BACKWARD_LEFT):
				commForward->put(false);
				commBackward->put(true);
				commLeft->put(true);
				commRight->put(false);
				commEffort->put(0x1999);		//10% effort
				transition_to(BACKWARD_RIGHT);
			break;
			case(BACKWARD_RIGHT):
				commForward->put(false);
				commBackward->put(true);
				commLeft->put(true);
				commRight->put(false);
				commEffort->put(0x1999);		//10% effort
				transition_to(FULL_POWER);
			break;
			case(FULL_POWER):
				commForward->put(true);
				commBackward->put(false);
				commLeft->put(false);
				commRight->put(false);
				commEffort->put(0x7FFF);		//10% effort
				transition_to(FORWARD);
			break;
		}
	}
}