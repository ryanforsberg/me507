#include "Navigation.h"
// Units must match (encoder conversion and chassis dimensions)
// Todo:
// * Fix defines (diameter, width, length)
// * Fix trig table (optional)
// * Check encoder setup
// * Build Run function
// * 
#define diameter 
#define lineCount //for the TimerCounter for Quadrature decoding, see AVR1600 pg6

class Navigation : public TaskBase
{
	protected:
		void Navigation::setup(PORT_t * port,
								uint8_t pin_0,
								uint8_t pin_1,
								uint8_t channel,
								TC0_t * timer,
								EVSYS_CHMUX_t EV_pin,
								TC_EVSEL_t EV_chan){
			//Function for setting up each port/pins
			
			//PORT SETUP
			port->DIRCLR = pin_0 | pin_1; //pass the pointer to the port you want into var &PORTD
			PORTCFG.MPCMASK = pin_0 | pin_1;
			port->PIN0CTRL = (port->PIN0CTRL & ~PORT_ISC_gm) | PORT_ISC_LEVEL_gc;
						
			//EVENT SYS SETUP
			switch (channel)
			{
				case(0):
					EVSYS.CH0MUX = EV_pin; //set the input pin
					EVSYS.CH0CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT0_bm; //enable the quadrature decoding, no filtering, no indexing signal
					break;	
				case(2):
					EVSYS.CH2MUX = EV_pin; //set the input pin
					EVSYS.CH2CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT0_bm; //enable the quadrature decoding, no filtering, no indexing signal
					break;
			}
			
			//TIMER COUNTER SETUP
			timer->CTRLD = TC_EVACT_QDEC_gc | EV_chan; //enable quadrature counting, event channel 0
			timer->PER = (lineCount * 4) - 1; //set the period
			timer->CTRLA = TC_CLKSEL_DIV1_gc; //select the clock division
		}
		
		void Navigation::Table(void){
			//Function for setting up the trig table
			uint8_t table[];
			for (){
				
			}
		}
		
		uint8_t Navigation::Table(uint8_t theta, bool sin){
			//Function for getting results from the trig table
			if (sin){
				return 
			}
			else{
				
				return 
			}
			
		}
		
		uint8_t* Navigation::CalcXYT(uint16_t left, uint16_t right, uint16_t omega){
			#define width 	//distance between the wheel centerlines
			#define length 	//distance from the COG back to the axle centerline
							//both in WHICH units
			
			//Function for calculating incremental change in X,Y, and Theta of the bot
			//inputs are the left and right counts of the encoders
			
			uint8_t omega = (right-left)/width + omega;	//update the angle
			
			uint8_t x_p = omega*length;					// path-frame (vehicle ref)
			uint8_t y_p = (right+left)/2;
			
			uint8_t x = x_p*cos(omega) - y_p*sine(omega);	//absolute coordinates
			uint8_t y = x_p*sine(omega) + x_p*cos(omega);
			
			result[] = omega,x,y;
			return result;
		}
	
	public:
	// constructor
	Navigation::Navigation (const char* instance_name)
	: TaskBase (instance_name)
	//arguments are (in order) - a_name, a_priority, a_stack_size, p_ser_dev)
	{
		
	}
	
	// run function
	void Navigation::run (void)
	{
		//!!!---- SETUP ----!!!
		Navigation::setup(&PORTD,0x01<<0,0x01<<1,0,&TCD0,EVSYS_CHMUX_PORTD_PIN0_gc,TC_EVSEL_CH0_gc);	//Port D pins 1 and 0, evsys 0, timer 0
		Navigation::setup(&PORTF,0x01<<6,0x01<<7,2,&TCF0,EVSYS_CHMUX_PORTF_PIN6_gc,TC_EVSEL_CH2_gc); //Port F pins 6 and 7, evsys 2, timer 0
		
		//!!!----  RUN  ----!!!
		while(1)
		{
			switch (state)
			{
				case (1): //
				
				break;
			};
		}
	}
};