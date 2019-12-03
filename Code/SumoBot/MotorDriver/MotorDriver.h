//*****
/** @file		MotorDriver.h
 *	@brief		Motor driver header file - task which takes four inputs and controls the two motors
 *	@details	Takes four inputs booleans (Forward, Backward, Left, and Right), and commands the two motors. Forward/Back, in-place pivots, and forward/back turns are supported
 *	Revised:
 *	\ 11-26-2019 PDH Hahahaha who keeps track of this at this stage in crunch time
 **/
//*****

#include "FreeRTOS.h"
#include "shares.h"

class MotorDriver
{
	protected:
	/** @brief	Takes the four input booleans and sets the outputs according the truth table
	*/
		set_outputs(bool FWD_A, bool FWD_B, bool BCK_A, bool BCK_B);
	public:
	/** @brief	The method called to start the task.
	*/
		run(void);		
};