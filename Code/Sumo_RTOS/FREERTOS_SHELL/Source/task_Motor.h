/* 
* MotorDriver.h
*
* Created: 12/4/2019 9:09:17 AM
* Author: Phil
*/


#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

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

class task_Motor : public frt_task
{
	//variables
	public:

	protected:
		uint8_t compare;
		uint8_t effort;
		bool FWD;
		bool BCK;
		bool RHT;
		bool LFT;
		enum Motor_states {
			IDLE,
			STRAIGHT,
			PIVOT,
			TURN
		};
	//functions
	public:
		// This constructor creates a user interface task object
		task_Motor (const char*, unsigned portBASE_TYPE, size_t, emstream*);
		
		void run(void);
	protected:
	private:

}; //MotorDriver

#endif //__MOTORDRIVER_H__
