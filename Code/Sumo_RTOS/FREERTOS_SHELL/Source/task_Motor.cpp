/* 
* MotorDriver.cpp
*
* Created: 12/4/2019 9:09:17 AM
* Author: Phil
*/

#define period 124						// used for prescaler and effort calculations

#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header

#include "shared_data_sender.h"
#include "shared_data_receiver.h"
#include "task_Motor.h"

// default constructor
task_Motor::task_Motor(const char* a_name,
						unsigned portBASE_TYPE a_priority,
						size_t a_stack_size,
						emstream* p_ser_dev
						)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
	
} //MotorDriver

void set_outputs(bool FWD_A, bool FWD_B, bool BCK_A, bool BCK_B,uint16_t compare)
{
	TCE0_CCA = FWD_A*compare; // Hardware pin E4
	TCE0_CCB = BCK_A*compare; // Hardware pin E5
	TCE0_CCC = BCK_B*compare; // Hardware pin E2
	TCE0_CCD = FWD_B*compare; // Hardware pin E3
}

//run function
void task_Motor::run(void)
{
	//!!!---- SETUP ----!!!
	compare = 0;				//start at 0 power
	effort = 0x60;
	FWD = 0;					//start at 0 movement
	BCK = 0;
	RHT = 0;
	LFT = 0;
	
	PORTE_DIRSET = 0b01111110;	// config pins 1 through 6 as output
	PORTE_REMAP = PORT_TC0A_bm | PORT_TC0B_bm;	// remap compare channels A to pin 4.
								// and compare channel B to pin 5.
								// see REMAP (page 151).
	
	TCE0_PER = period;			// set the period to 8000 (see calculation on the project excel sheet)
								// fpwm = fclk/(prescale*(PER+1))
								// aiming for 1 KHz right now (6.6us rise time on half-bridge, recommends at least 10x slower).
	
								// config register B (page 175), single slope PWM, enable compare and capture on channels A, B, C, and D.
	TCE0_CTRLB = TC_WGMODE_SINGLESLOPE_gc | TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm| TC0_CCDEN_bm;
	
	set_outputs(0,0,0,0,0);		// make sure all the directions are off at the start.
	
	TCE0_CTRLA = TC_CLKSEL_DIV256_gc;			// turn ON the timer counter - no prescaler
								// see CTRLA (page 175)
	
	PORTE_OUTSET = PIN6_bm | PIN1_bm; // enable all of the half bridges with pins 1 and 6.
	
	//!!!----  RUN  ----!!!
	while(1)
	{
		FWD = commForward->get();
		BCK = commBackward->get();
		LFT = commLeft->get();
		RHT = commRight->get();
		//effort = commEffort->get();
		
		((LFT^RHT)&(!FWD+BCK))?transition_to(PIVOT):(void)0;    // pivot	(Left XOR Right) AND NOT (Forward OR Back)
		((LFT^RHT)&(FWD^BCK))?transition_to(TURN):(void)0;		// turn		(Left XOR Right) AND (Forward XOR Back)
		((FWD^BCK)&(!(LFT+RHT)))?transition_to(STRAIGHT):(void)0;	// straight (Forward XOR Back) AND NOT (Left OR Right)
		!(FWD+BCK+LFT+RHT)?transition_to(IDLE):(void)0;			// idle		

		compare = effort*period/0xFF;

		switch(state)
		{
			case(IDLE):
				set_outputs(0,0,0,0,0);
			break;
			case(STRAIGHT):
				set_outputs(FWD,FWD,BCK,BCK,compare);
			break;
			case(PIVOT):
				set_outputs(RHT,LFT,LFT,RHT,compare);
			break;		
			case(TURN):
				set_outputs(FWD&LFT,FWD&RHT,BCK&RHT,BCK&LFT,compare);
			break;	
		}
		
		vTaskDelay(10); // Delay task 10ms
	}
}