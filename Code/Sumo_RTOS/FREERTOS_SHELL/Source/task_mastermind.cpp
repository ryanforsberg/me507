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
	fake = 1;
	while(1)
	{	
		delay_ms(1000);
		switch(state)
		{
			case(INIT):
				if(fake)
				{
					transition_to(FORWARD);	
				}
				
			break;
			case(FORWARD):
				commBackward->put(0);
				commForward->put(1);
				commLeft->put(0);
				commRight->put(0);
				transition_to(BACKWARD);
			break;
			case(BACKWARD):
				commBackward->put(1);
				commForward->put(0);
				commLeft->put(0);
				commRight->put(0);
				transition_to(RIGHT);
			break;
			case(RIGHT):
				commBackward->put(0);
				commForward->put(0);
				commLeft->put(0);
				commRight->put(1);
				transition_to(LEFT);
			break;
			case(LEFT):
				commBackward->put(0);
				commForward->put(0);
				commLeft->put(1);
				commRight->put(0);
				transition_to(IDLE);
			break;
			case(IDLE):
				commBackward->put(0);
				commForward->put(0);
				commLeft->put(0);
				commRight->put(0);
				transition_to(FORWARD);
			break;
		}
	}
}