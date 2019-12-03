#include MotorDriver.h

/* Motor Driver Task CPP File
* TODO: Turn on the ENABLE pins.
*		Verify that compare = 0 is effectively 0% PWM
*
* DOCUMENTATION:
* Truth Table:
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
#define period 8000	// used for prescaler and effort calculations

class MotorDriver : public TaskBase
{
protected:
	unit16_t compare = 0;	//start at 0 power
	bool FWD = 0;			//start at 0 movement
	bool BCK = 0;
	bool RHT = 0;
	bool LFT = 0;
	
	// turn pins on/off function
	void set_outputs (bool FWD_A, bool FWD_B, bool BCK_A, bool BCK_B)
	{
		TCE0_CCA = FWD_A*compare; // Hardware pin E4
		TCE0_CCA = BCK_A*compare; // Hardware pin E5
		TCE0_CCC = BCK_B*compare; // Hardware pin E2
		TCE0_CCD = FWD_B*compare; // Hardware pin E3
	}
	
public:
	// constructor
	MotorDriver::MotorDriver (const char* instance_name)
	: TaskBase (instance_name)
	//arguments are (in order) - a_name, a_priority, a_stack_size, p_ser_dev)
	{
		
	}
	
	// run function
	void MotorDriver::run (void)
	{
		//!!!---- SETUP ----!!!
		PORTE_DIRSET = 0b01111110;	// config pins 2 through 6 as output
		PORTE_REMAP = 0b00000011;	// remap compare channels A to pin 4.
									// and compare channel B to pin 5.
									// see REMAP (page 151).
		
		TCE0_PER = period;			// set the period to 8000 (see calculation on the project excel sheet)
									// fpwm = fclk/(prescale*(PER+1))
									// aiming for 1 KHz right now (6.6us rise time on half-bridge, recommends at least 10x slower).
		
		// config register B (page 175), single slope PWM, enable compare and capture on channels A, B, C, and D.
		TCE0_CTRLB = TC_WGMODE_SINGLESLOPE_gc | TC0_CCAEN_bm | TC0_CCBEN_bm| TC0_CCCEN_bm| TC0_CCDEN_bm;
		
		set_outputs(0,0,0,0);		// make sure all the directions are off at the start.
		
		TCE0_CTRLA = 0x01;			// turn ON the timer counter - no prescaler
									// see CTRLA (page 175)
		
		
		//!!!----  RUN  ----!!!
		while(1)
		{
			// pull the new data from the communication variables
			FWD = commForward.get();
			BCK = commBack.get();
			LFT = commLeft.get();
			RHT = commRight.get();
			
			// convert effort into compare
			compare = effort*period/0xFF;	//convert an 8bit input value to a number between 8000 and 0.
											// compare of 0 is effectively 0% PWM (VERIFY)

			// check for a state change			
			((LFT^RHT)&(!FWD+BCK))?transition_to(3):;   // pivot	(Left XOR Right) AND NOT (Forward OR Back)
			((LFT^RHT)&(FWD^BCK))?transition_to(4):;	// turn		(Left XOR Right) AND (Forward XOR Back)
			((FWD^BCK)&(!(LFT+RHT)))?transition_to(2):;	// straight (Forward XOR Back) AND NOT (Left OR Right)
			(FWD+BCK+LFT+RHT)?transition_to(1):;		// idle
			
			switch (state)
			{
				case (1): //idle state
					set_outputs(0,0,0,0);
					break;
				
				case (2): // forward/back state
					set_outputs(FWD,BCK,BCK,FWD);
					break;
					
				case (3): // pivot state
					set_outputs(RHT,LFT,LFT,RHT);
					break;
				
				case (4): // turn state
					set_outputs(FWD&RHT,FWD&LFT,BCK&RHT,BCK&LFT);
					break;
			};
		}
	}
};