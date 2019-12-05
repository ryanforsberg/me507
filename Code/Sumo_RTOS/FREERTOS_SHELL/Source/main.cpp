//*************************************************************************************
/** \file lab1_main.cpp
 *    This file contains the main() code for a program which runs a port of the FreeRTOS
 *    for AVR devices. This port is specific to the XMEGA family.
 *
 *  Revisions:
 *    \li 09-14-2017 CTR Adapted from JRR code for AVR to be compatible with xmega 
 *
 *  License:
 *    This file is released under the Lesser GNU Public License, version 2. This 
 *    program is intended for educational use only, but it is not limited thereto. 
 */
//*************************************************************************************


#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header
#include <avr/interrupt.h>
#include <string.h>                         // Functions for C string handling

#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // FreeRTOS inter-task communication queues
#include "croutine.h"                       // Header for co-routines and such

#include "rs232int.h"                       // ME405/507 library for serial comm.
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_task.h"                       // Header of wrapper for FreeRTOS tasks
#include "frt_text_queue.h"                 // Wrapper for FreeRTOS character queues
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_shared_data.h"                // Header for thread-safe shared data
#include "shares.h"                         // Global ('extern') queue declarations

#include "xmega_util.h"

#include "task_user.h"                      // Header for user interface task
#include "task_LED.h"                      // Header for user interface task

#include "task_Motor.h"
#include "task_mastermind.h"

#include "EdgeSensor_Digital.h"


						//Motors Stuff
shared_data<bool>* commForward;
shared_data<bool>* commBackward;
shared_data<bool>* commLeft;
shared_data<bool>* commRight;
shared_data<uint8_t>* commEffort;

						//Edge Stuff
shared_data<uint8_t>* edge_out;	

frt_text_queue print_ser_queue (32, NULL, 10);


//=====================================================================================
/** The main function sets up the RTOS.  Some test tasks are created. Then the 
 *  scheduler is started up; the scheduler runs until power is turned off or there's a 
 *  reset.
 *  @return This is a real-time microcontroller program which doesn't return. Ever.
 */

int main (void)
{
	cli();
	// Configure the system clock to use internal oscillator at 32 MHz
	config_SYSCLOCK();
	
	// Disable the watchdog timer unless it's needed later. This is important because
	// sometimes the watchdog timer may have been left on...and it tends to stay on	 
	wdt_disable ();

	commForward = new shared_data<bool>;
	commBackward = new shared_data<bool>;
	commLeft = new shared_data<bool>;
	commRight = new shared_data<bool>;
	commEffort = new shared_data<uint8_t>;
	
	edge_out = new shared_data<uint8_t>;

	// Configure a serial port which can be used by a task to print debugging infor-
	// mation, or to allow user interaction, or for whatever use is appropriate.  The
	// serial port will be used by the user interface task after setup is complete and
	// the task scheduler has been started by the function vTaskStartScheduler()
	rs232 ser_dev(0,&USARTC0); // Create a serial device on USART E0
	ser_dev << clrscr << "FreeRTOS Xmega Testing Program" << endl << endl;
	
	// The user interface is at low priority; it could have been run in the idle task
	// but it is desired to exercise the RTOS more thoroughly in this test program
	//new task_user ("UserInt", task_priority (0), 260, &ser_dev);
	
	// The LED blinking task is also low priority and is used to test the timing accuracy
	// of the task transitions.
	// new task_LED ("LED BLINKER", task_priority (1), 260, &ser_dev);
	
	task_Motor* MOTORS = new task_Motor("MOTORS", task_priority (2), 500, &ser_dev);
	
	new task_mastermind("Mastermind", task_priority(2), 500, &ser_dev);
	//new EdgeSensor("Edge", task_priority(2), 500, &ser_dev);
	
	// Enable high - low level interrupts and enable global interrupts
	PMIC_CTRL = (1 << PMIC_HILVLEN_bp | 1 << PMIC_MEDLVLEN_bp | 1 << PMIC_LOLVLEN_bp);
	sei();
	
	// Here's where the RTOS scheduler is started up. It should never exit as long as
	// power is on and the microcontroller isn't rebooted
	vTaskStartScheduler ();
}