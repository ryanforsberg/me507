//***********************************
/** @file	task_mastermind.cpp
 *	@brief	C++ file for the Mastermind task.
 * @details The Mastermind task controls the motion strategy of the sumo bot in ME 507. Based on sensor inputs, mastermind determined which direction to move.
 *
 *  @date 2019-12-4 RRF Initial file creation, edge detection strategy written, tested, and debugged.
 *		
 *		License:
 *		If you're reading this, you're already breaking the licensing agreement.
 */
 
//***********************************
/*
* Created: 12/4/2019 10:34:10 AM
* Author: Ryan Forsberg
*/

/**	@brief		Strategizes sumo bot movement based on collected sensor information
 *	@details	Reads shared information from the edge detection task. Depending on the combination of sensors that are detecting the edge, a corresponding motion function is executed
 */
//*****************************************************************************

#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header

#include "shared_data_sender.h"			// Headers for proper share functionality
#include "shared_data_receiver.h"
#include "task_mastermind.h"			// Mastermind header file

//-------------------------------------------------------------------------------------
// Constructor
task_mastermind::task_mastermind(const char* a_name,
						unsigned portBASE_TYPE a_priority,
						size_t a_stack_size,
						emstream* p_ser_dev
						)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{

} 

//-------------------------------------------------------------------------------------

// Run: Determine which edge sensors are detecting the edge, and choose a corresponding logical strategy i.e. Move backwards if both front sensors are detecting the edge
void task_mastermind::run(void)
{
	
	// Edge detection sensor bit masks. Each sensor has a corresponding bit in edge_out->get()
	// edge_out->get()= 00	F		E		D		C		B		A
	
	uint8_t LR_bm = 1;		// Edge Sensor A is Left Rear
	uint8_t RR_bm = 1<<1;	// Edge Sensor B is Right Rear
	uint8_t RM_bm = 1<<2;	// Edge Sensor C is Right Middle
	uint8_t RF_bm = 1<<3;	// Edge Sensor D is Right Front
	uint8_t LF_bm = 1<<4;	// Edge Sensor E is Left Front
	uint8_t LM_bm = 1<<5;	// Edge Sensor F is Left Middle

	// Status of each edge detection sensor
	uint8_t LF = 0;  // Left Front
	uint8_t RF = 0;  // Right Front
	uint8_t LM = 0;  // Left Middle
	uint8_t RM = 0;  // Right Middle
	uint8_t LR = 0;  // Left Rear
	uint8_t RR = 0;  // Right Rear
	
	while(1)	// Forever
	{
		// Determine statuses of each edge sensor
		LF = edge_out->get() & LF_bm;
		RF = edge_out->get() & RF_bm;
		LM = edge_out->get() & LM_bm;
		RM = edge_out->get() & RM_bm;
		LR = edge_out->get() & LR_bm;
		RR = edge_out->get() & RR_bm;
		
		// Execute edge sensor truth table to send desired motion to Motor
		if (LF)
		{
			if (RF)
			{
				if (LM)
				{
					if (RM)
					{
						task_mastermind::Backward(); // LF & RF & LM & RM
					}
					else
					{
						task_mastermind::Right_Backward(); // 1. LF & RF & LM    2. LF & RF & LM & LR
					}
				}
				else if (RM)
				{
					task_mastermind::Left_Backward(); // 1. LF & RF & RM   2. LF & RF & RM & RR
				}
				else
				{
					task_mastermind::Backward();	// LF & RF
				}
			}
			else if (LM)
			{
				if (LR)
				{
					task_mastermind::Right_Forward(); // LF & LM & LR
				}
				else
				{
					task_mastermind::Right_Backward(); // LF & LM
				}
			}
			else
			{
				task_mastermind::Right_Backward(); // LF
			}
		}

		// NO LF beyond this point

		else if (RF)
		{
			if (RM && RR)
			{
				task_mastermind::Left_Forward(); // RF & RM & RR
			}
			else
			{
				task_mastermind::Left_Backward(); // 1. RF     2. RF & RM
			}
		}

		// NO RF beyond this point

		else if (LM)
		{
			task_mastermind::Right_Forward(); // 1. LM   2. LM & LR    3. LM & LR & RR
		}

		// NO LM beyond this point

		else if (RM)
		{
			task_mastermind::Left_Forward(); // 1. RM   2. RM & RR  3. RM & RR & LR
		}

		// No RM beyond this point

		else if (LR)
		{
			if (RR)
			{
				task_mastermind::Forward(); // LR & RR
			}
			else
			{
				task_mastermind::Right_Forward(); // LR
			}
		}
		else if (RR)
		{
			task_mastermind::Left_Forward(); // RR
		}
		
		else
		{
			task_mastermind::Forward(); // Strategy when no edge detected. If more time available, opponent detection and navigation inputs would affect this strategy.
		}
		
		vTaskDelay(5); // Delay task 5ms
	}
	
}

// Functions below adjust shares to proper values based on desired motion

/*
 Truth Table for Mastermind Strategizing:
 *	Inputs			Direction			
 *	FWD	BCK	LFT	RHT						
 *	1	0	0	0	Forward				
 *	0	1	0	0	Backward			
 *	0	0	1	0	Pivot Left			
 *	0	0	0	1	Pivot Right			
 *	1	0	1	0	Left Forward Turn	
 *	1	0	0	1	Right Forward Turn	
 *	0	1	1	0	Left Backward Turn	
 *	0	1	0	1	Right Backward Turn	
 *
*/ 

void task_mastermind::Forward(void)
{
	commBackward->put(0);
	commForward->put(1);
	commLeft->put(0);
	commRight->put(0);
}

void task_mastermind::Backward(void)
{
	commBackward->put(1);
	commForward->put(0);
	commLeft->put(0);
	commRight->put(0);
}

void task_mastermind::Pivot_Left(void)
{
	commBackward->put(0);
	commForward->put(0);
	commLeft->put(1);
	commRight->put(0);
}

void task_mastermind::Pivot_Right(void)
{
	commBackward->put(0);
	commForward->put(0);
	commLeft->put(0);
	commRight->put(1);
}

void task_mastermind::Left_Forward(void)
{
	commBackward->put(0);
	commForward->put(1);
	commLeft->put(1);
	commRight->put(0);
}

void task_mastermind::Right_Forward(void)
{
	commBackward->put(0);
	commForward->put(1);
	commLeft->put(0);
	commRight->put(1);
}

void task_mastermind::Left_Backward(void)	// Note: For backward turning motion, right/left is switched due to a difference in interpretation between team members during backward motion 
{											
	commBackward->put(1);
	commForward->put(0);
	commLeft->put(0);
	commRight->put(1);	
}

void task_mastermind::Right_Backward(void)
{
	commBackward->put(1);
	commForward->put(0);
	commLeft->put(1);
	commRight->put(0);
}