//***********************************
/** @file	EdgeSensor_Digital.h
 *	@brief	Header for the Edge Detection task.
 * @details The task_Motor class sets up the XMEGA PWM for 1kHz operation on each of the four motor driver channels. 
 *			Each motor driver will be driven at the same duty cycle. Based on the four input booleans and one uint8_t,
 *			the task will set the outputs according to the following truth table. NOTE! Back Left is interpreted as
 *			turning to FACE left, which will result in the chassis moving to the RIGHT. PS - I can't get Doxygen tables
 *			to work, please see the source code.
 *
 *  @date 2019-12-1 RRF Initial file creation
 *  @date 2019-12-2 RRF Implement a few includes
 *  @date 2019-12-2 RRF Create class
 *  @date 2019-12-2 RRF Adjustments to class for RTOS functionality
 *  @date 2019-12-3 RRF Adjustments to included header files for functionality
 *		
 *		License:
 *		HA! Have fun with it. No warranty of any kind. It might chop your fingers off. This is a "Do not dumb" zone,
 *		so don't dumb here.
 */
 
//***********************************
/*
 * Created: 12/1/2019 2:37:51 PM
 * Author : Ryan Forsberg
 */

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

#include "shares.h"							// Set up Shares

//-----------------------------------------
/** @brief		Edge Detection class used to read the sensors for the 507 Sumobot. Uses inter-task communication variable: edge_out.
 *	@details	This class controls the edge detection sensors: Their setup, reading, and interpretation. 
 
 */

class EdgeSensor : public frt_task
{
	// QTR Edge Sensor class for Sumo Bot
	public:
		// This constructor creates a FreeRTOS task with the given task run function,
		// name, priority, and stack size
		EdgeSensor(const char*, unsigned portBASE_TYPE, size_t, emstream* );
		
		/** @brief	    Determine if sumo bot is on the edge
		*	@details	Edge Detection run function used to setup the edge detection MCU pins, repetitively read edge detection sensors, and interpret which are on the edge
		*/
		void run(void);
};