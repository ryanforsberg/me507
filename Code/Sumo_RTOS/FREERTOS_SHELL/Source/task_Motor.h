//***********************************
/** @file	task_Motor.h
 *	@brief	Header for the task_Motor class. This class is not designed	to be instantiated more than once as the
 *			pinouts are hard coded (IE: not passed to the constructor).
 * @details The task_Motor class sets up the XMEGA PWM for 1kHz operation on each of the four motor driver channels. 
 *			Each motor driver will be driven at the same duty cycle. Based on the four input booleans and one uint8_t,
 *			the task will set the outputs according to the following truth table. NOTE! Back Left is interpreted as
 *			turning to FACE left, which will result in the chassis moving to the RIGHT.
 *
 *			Inputs				Direction				Outputs
 *			FWD BCK LFT RHT							FWD_A	BCK_A	FWD_B	BCK_B
 *			1	0	0	0		Forward					1		0		1		0
 *			0	1	0	0		Backward				0		1		0		1
 *			0	0	1	0		Pivot Left				0		1		1		0
 *			0	0	0	1		Pivot Right				1		0		0		1
 *			1	0	1	0		Left Forward Turn		0		0		1		0
 *			1	0	0	1		Right Forward Turn		1		0		0		0
 *			0	1	1	0		Left Backward Turn		0		0		0		1
 *			0	1	0	1		Right Backward Turn		0		1		0		0
 *		
 *		Revisions:
 *		\li 12-07-2019 PDH Too many revisions to count due to accelerated development cycle. This is at least
 *		the second full rewrite.
 *		
 *		License:
 *		HA! Have fun with it. No warranty of any kind. It might chop your fingers off. This is a "Do not dumb" zone,
 *		so don't dumb here.
 */
//***********************************
/*
* Created: 12/4/2019 9:09:17 AM
* Author: Phil
*/


#ifndef __MOTORDRIVER_H__					// These lines are automatically incorporated by the IDE when a new
#define __MOTORDRIVER_H__					// class is created.

#include <stdlib.h>                         // Prototype declarations for I/O functions
#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // FreeRTOS inter-task communication queues

#include "rs232int.h"                       // ME405/507 library for serial comm.
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_task.h"                       // Header for ME405/507 base task class
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_text_queue.h"                 // Header for a "<<" queue class
#include "frt_shared_data.h"                // Header for thread-safe shared data

#include "shares.h"                         // Global ('extern') queue declarations

//-----------------------------------------
/** @brief		Motor driver class used to drive the motors for the 507 Sumobot. Uses inter-task communication
				variables: commForward, commBackward, commRight, commLeft, and commEffort.
 *	@details	This class controls the motors according to the following inputs: commForward, commBackward,
 *				commRight, commLeft, and commEffort. The direction _comm_ands are interpreted according to the 
 *				truth table presented in the documentation of this file. The effort command is a uint8_t number
 *				which represents a duty cycle. It is linearly scaled (0 = 0% duty and 255 = 100% duty). Performance
 *				at low duty cycles is not guaranteed due to the switching time of the half-bridges.
 */

class task_Motor : public frt_task
{
	//variables
	protected:
		/** @brief		Uint8_t used to store the calculation of the new compare value.
		 *	@details	The new compare value is calculated as follows: effort*124/0xFF. This results in a 1kHz signal
		 *				with a hypothetical duty cycle ranging from 0 - 100%. For more information see the ATMel
		 *				application note - AVR1306.
		 */
		uint8_t compare;
		
		/** @brief		Uint8_t used to store the commanded effort (commEffort) value.
		 *	@details	After reading commEffort from the inter-task communication, it is stored in this variable for
		*				further use.
		 */
		uint8_t effort;
		
		/** @brief		Boolean used to store the commanded forward (commForward) value.
		 *	@details	After reading commForward from the inter-task communication, it is stored in this variable
		 *				for further use.
		 */
		bool FWD;
		
		/** @brief		Boolean used to store the commanded backward (commBackward) value.
		 *	@details	After reading commForward from the inter-task communication, it is stored in this variable
		 *				for further use.
		 */
		bool BCK;
		
		/** @brief		Boolean used to store the commanded right (commRight) value.
		 *	@details	After reading commForward from the inter-task communication, it is stored in this variable
		 *				for further use.
		 */
		bool RHT;
		
		/** @brief		Boolean used to store the commanded left (commLeft) value.
		 *	@details	After reading commForward from the inter-task communication, it is stored in this variable
		 *				for further use.
		 */
		bool LFT;
		
		/** @brief		The four states for task_Motor are: Idle, Straight, Pivot, and Turn.
		 *	@details	The four states for task_Motor represent 3 types of movement and a stationary one. When
		 *				combined with the state-transition logic they create a truth table. The Straight state
		 *				controls straight movements forwards or backwards (pure translation). The Pivot state
		 *				controls in-place rotation (pure rotation about the CG). The Turn state controls a turn
		 *				about one wheel or the other (pure rotation about the wheel contact patch).
		 */
		enum Motor_states {
			IDLE,
			STRAIGHT,
			PIVOT,
			TURN
		};
	//functions
	public:
		/** @brief		task_Motor constructor. Just needs to be passed the frt_task constructor arguments.
		 *	@details	The task_Motor constructor incorporates no special functionality, it simply needs
		 *				to be passed the arguments for the constructor for the class it extends (frt_task).
		 *	@returns	The constructor returns nothing.
		 */
		task_Motor (const char*, unsigned portBASE_TYPE, size_t, emstream*);
		
		/** @brief		Run function called by the freeRTOS scheduler. Contains all of the operational logic.
		 *	@details	The task_Motor run function reads the inter-task communication variables, determines
		 *				the next state required, and passes the private set_outputs function the appropriate
		 *				inputs. Run is also responsible for the initial setup of the XMEGA timer counter, pins,
		 *				and ports.
		 *	@returns	Never. It left for cigarettes and milk right after the scheduler called it. Nobody has
		 *				heard from it since.
		 */
		void run(void);
	private:
	
		/** @brief		Internal function which sets the compare registers to the appropriate values.
		 *	@details	The set_outputs function is called by various states of the run function. It's five inputs
		 *				are four booleans controlling the various compare and capture channels, and an effort which
		 *				is universally applied to all of the channels. 
		 *	@returns	Nothing is returned by this function.
		 */
		void set_outputs(bool, bool, bool, bool, uint8_t)

};

#endif //__MOTORDRIVER_H__
