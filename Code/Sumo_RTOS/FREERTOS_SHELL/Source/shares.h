//*************************************************************************************
/** \file shares.h
 *    This file contains extern declarations for queues and other inter-task data
 *    communication objects used in a ME405/507/FreeRTOS project. 
 *
 *  Revisions:
 *    \li 09-30-2012 JRR Original file was a one-file demonstration with two tasks
 *    \li 10-05-2012 JRR Split into multiple files, one for each task plus a main one
 *    \li 10-29-2012 JRR Reorganized with global queue and shared data references
 *	  \li 12-07-2019 PDH Added the inter-task communication variables for the 507 bot.
 *
 *  License:
 *		This file is copyright 2012 by JR Ridgely and released under the Lesser GNU 
 *		Public License, version 2. It intended for educational use only, but its use
 *		is not limited thereto. */
/*		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *		AND	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * 		IMPLIED 	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * 		ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * 		LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 * 		TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * 		OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * 		CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * 		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * 		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _SHARES_H_
#define _SHARES_H_


//-------------------------------------------------------------------------------------
// Externs:  In this section, we declare variables and functions that are used in all
// (or at least two) of the files in the data acquisition project. Each of these items
// will also be declared exactly once, without the keyword 'extern', in one .cpp file
// as well as being declared extern here. 


/**
 * \var print_ser_queue
 * \brief This queue allows tasks to send characters to the user interface task for display.
 */
extern frt_text_queue print_ser_queue;			// This queue allows tasks to send characters to the user interface task for display.

/**
 *	@var		commForward
 *	@brief		Used in combination with the other comm__ variables to command the task_Motor task
 *	@details	commForward is used to indicate some type of forward motion - either pure translation,
 *				or a forward turn (in combination with commLeft or commRight).
 */
extern shared_data<bool>* commForward;

/**
 *	@var		commBackward
 *	@brief		Used in combination with the other comm__ variables to command the task_Motor task
 *	@details	commBackward is used to indicate some type of backward motion - either pure translation,
 *				or a backward turn (in combination with commLeft or commRight).
 */
extern shared_data<bool>* commBackward;

/**
 *	@var		commLeft
 *	@brief		Used in combination with the other comm__ variables to command the task_Motor task
 *	@details	commLeft is used to indicate some type of left turn - either pure rotation,
 *				or a left-facing turn (in combination with commForward or commBackward).
 */
extern shared_data<bool>* commLeft;

/**
 *	@var		commRight
 *	@brief		Used in combination with the other comm__ variables to command the task_Motor task
 *	@details	commRight is used to indicate some type of right turn - either pure rotation,
 *				or a right-facing turn (in combination with commForward or commBackward).
 */
extern shared_data<bool>* commRight;

/**
 *	@var		commEffort
 *	@brief		Used in combination with the other comm__ variables to command the task_Motor task
 *	@details	commEffort is used to indicate the duty cycle requested of the motor driver.
 */
extern shared_data<uint8_t>* commEffort;

/**
 *	@var		edge_out
 *	@brief		Bitwise communication of which of the 6 edge sensors is currently seeing white.
 */
extern shared_data<uint8_t>* edge_out;

#endif // _SHARES_H_
