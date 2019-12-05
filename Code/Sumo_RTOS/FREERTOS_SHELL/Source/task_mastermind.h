/* 
* mastermind.h
*
* Created: 12/4/2019 10:34:10 AM
* Author: Phil
*/


#ifndef __MASTERMIND_H__
#define __MASTERMIND_H__

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

class task_mastermind : public frt_task
{
	//variables
	public:
	protected:
		enum mastermind_states {
			INIT,
			FORWARD,
			BACKWARD,
			RIGHT,
			LEFT,
			IDLE
		};
		volatile bool fake;
	private:

	//functions
	public:
		//constructor function
		task_mastermind (const char*, unsigned portBASE_TYPE, size_t, emstream*);
		
		//run function
		void run(void);
	protected:
	private:

}; //mastermind

#endif //__MASTERMIND_H__
