/*********
* @file		task_Motor.cpp
* @brief	The .cpp file for the Motor Driver task.
* @details	This file contains the three functions needed for the Motor Driver task. They are:
*			\li Run function - called by the scheduler. Contains an initial setup and then
*			an infinite loop.
*			\li set_outputs - the last step between software and hardware. Writes the compare
*			values to the appropriate registers.
*			\li constructor function - passes the needed parameters to the constructor of the
*			base class.
*
*	Revisions:
*	\li	12-08-2019 PDH As with the header file, this was not extensively tracked due to the
*	rapid development cycle. The github page has at least 6 commits for this file.
*
*	License:
*	You can have the header file, but this file is licensed only under an hourly rental.
*	The fee is $1000/hour plus 10% of any profits made. How else am I supposed to get through
*	writing all of this documentation.
*
* Created: 12/4/2019 9:09:17 AM
* Author: Phil
*/

/**	@brief		The result of a calculation to set the PWM frequency to 1kHz.
 *	@details	The calculation presented in ATmel document AVR1306 was used to calculate
 *				the appropriate prescaler and period values for a 1kHz PWM frequency when
 *				the microcontroller clock operating at 32MHz.
 */
#define period 124							// used for prescaler and effort calculations

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

/*
 *
 */
void task_Motor::set_outputs(bool FWD_A, bool FWD_B, bool BCK_A, bool BCK_B, uint8_t compare)
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
	effort = 0x00;
	FWD = 0;					//start at 0 movement
	BCK = 0;
	RHT = 0;
	LFT = 0;
	
	PORTE_DIRSET = 0b01111110;					// config pins 1 through 6 as output
	PORTE_REMAP = PORT_TC0A_bm | PORT_TC0B_bm;	// remap compare channels A to pin 4.
												// and compare channel B to pin 5.
												// see REMAP (page 151).
	
	TCE0_PER = period;							// set the period to 124 (see calculation on
												// the project excel sheet)
												// fpwm = fclk/(prescale*(PER+1))
												// aiming for 1 KHz right now (6.6us rise
												// time on half-bridge, datasheet recommends at least 10x slower).
	
	TCE0_CTRLB = TC_WGMODE_SINGLESLOPE_gc | TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm| TC0_CCDEN_bm;
					// control register B (page 175)
					// * single slope PWM
					// * enable compare and capture on channels A, B, C, and D.
	
	set_outputs(0,0,0,0,0);					// make sure all the directions are off at the start.
	
	TCE0_CTRLA = TC_CLKSEL_DIV256_gc;		// turn ON the timer counter - no prescaler
											// see CTRLA (page 175)
	
	PORTE_OUTSET = PIN6_bm | PIN1_bm;		// enable all of the half bridges with pins 1 and 6.
	
	//!!!----  RUN  ----!!!
	while(1)
	{
		// get the latest motion commands.
		FWD = commForward->get();
		BCK = commBackward->get();
		LFT = commLeft->get();
		RHT = commRight->get();
		//effort = commEffort->get();		//the effort commad has not yet been implemented in mastermind.
		
		// state transition logic
		// ternary command functions as follows (boolean statement)?(executed if true):(executed if false)
		// the (void)0 is a NOP. The compiler was complaining about statements like (true)?foo(bar):;
		// the boolean statements are written in a more polite form to the right of each line.
		((LFT^RHT)&(!FWD+BCK))?transition_to(PIVOT):(void)0;		// pivot	(Left XOR Right) AND NOT (Forward OR Back)
		((LFT^RHT)&(FWD^BCK))?transition_to(TURN):(void)0;			// turn		(Left XOR Right) AND (Forward XOR Back)
		((FWD^BCK)&(!(LFT+RHT)))?transition_to(STRAIGHT):(void)0;	// straight (Forward XOR Back) AND NOT (Left OR Right)
		!(FWD+BCK+LFT+RHT)?transition_to(IDLE):(void)0;				// idle		

		//compare calculation. duty cycle is 0% to 100% linearly scaled to 0 to 255.
		// this value is ultimately written to the compare and capture registers
		compare = effort*period/0xFF;

		switch(state)
		{
			// States. Ultimately multiple states were probably not necessary. However, the combination of the 
			// states and transition logic allowed for a single output function to command the motors in multiple
			// basic motions.
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