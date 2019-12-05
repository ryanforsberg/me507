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
	
	// Edge detection sensor bit masks. Each sensor has a corresponding bit in edge_out->get()
	// edge_out->get()= 00	F		E		D		C		B		A
	uint8_t LR_bm = 1;		// Edge Sensor A is Left Rear
	uint8_t RR_bm = 1<<1;	// Edge Sensor B is Right Rear
	uint8_t RM_bm = 1<<2;	// Edge Sensor C is Right Middle
	uint8_t RF_bm = 1<<3;	// Edge Sensor D is Right Front
	uint8_t LF_bm = 1<<4;	// Edge Sensor E is Left Front
	uint8_t LM_bm = 1<<5;	// Edge Sensor F is Left Middle

	// Status of each edge detection sensor
	uint8_t LF = 0;
	uint8_t RF = 0;
	uint8_t LM = 0;
	uint8_t RM = 0;
	uint8_t LR = 0;
	uint8_t RR = 0;
	
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
			task_mastermind::Forward(); // Strategy when no edge detected
		}
		
		vTaskDelay(5); // Delay task 5ms
	}
	
}

// Functions below adjust shares to proper values based on desired motion
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

void task_mastermind::Left_Backward(void)	// Note: For backward turning motion, right/left is switched due to a difference in interpretation during backward motion 
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

/*

 Truth Table for Motor Task:
 *	Inputs			Direction			Outputs
 *	FWD	BCK	LFT	RHT						FWD_A	BCK_A	FWD_B	BCK_B
 *	1	0	0	0	Forward				1		0		1		0
 *	0	1	0	0	Backward			0		1		0		1
 *	0	0	1	0	Pivot Left			0		1		1		0
 *	0	0	0	1	Pivot Right			1		0		0		1
 *	1	0	1	0	Left Forward Turn	0		0		1		0
 *	1	0	0	1	Right Forward Turn	1		0		0		0
 *	0	1	1	0	Left Backward Turn	0		0		0		1
 *	0	1	0	1	Right Backward Turn	0		1		0		0
 *
*/ 