/*
 * EdgeSensor.cpp
 *
 * Created: 11/17/2019 2:37:51 PM
 * Author : Ryan Forsberg
 */ 

#include "EdgeSensor.h"

EdgeSensor::EdgeSensor(struct QTRsensor EdgeSensorInfo, const char* a_name, unsigned portBASE_TYPE a_priority, size_t a_stack_size, emstream* p_ser_dev) : frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
	this->EdgeSensorPerm=EdgeSensorInfo;
	
	(*EdgeSensorPerm.CtrlPort).DIRSET |= ; // Set Ctrl Pin to output
	(*EdgeSensorPerm.OutPort).DIRCLR |= ;  // Set Out Pin to input
	
	// Entire ADC
	(*EdgeSensorPerm.CtrlA) |= ADC_ENABLE_bm;  // Enable ADC
	(*EdgeSensorPerm.CtrlB) |= ADC_RESOLUTION_12BIT_gc; // Conversion result resolution: 12 bit, right adjusted
	
	// ADC Channel
	(*EdgeSensorPerm.ADCChan) = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc; // Set single ended ADC input mode and gain of 1
	// Gain definition may be optional and the reference value is automatically internal GND
	(*EdgeSensorPerm.) = 

}

void EdgeSensor::EdgeSensorReadStart(void)
{
	
	(*EdgeSensorPerm.CtrlA) |= EdgeSensorPerm.ChxStart; // Start ADC read at specified channel
	
	// Four channels: four separate pairs of MUX control registers
	/* if (!(ADCA.CTRLA & ADC_ENABLE_bm)) // If the enable pin has not been set for the ADC
	{
		ADCA.PRESCALER = ADC_PRESCALER_DIV256_gc;  // ADC Frequency = CPU frequency/ADC Prescaler
	} */
	
}

bool EdgeSensor::EdgeSensorReadDone(void)
{
	bool ReadDone;
	ReadDone = ((*EdgeSensorPerm.IntFlags)&&);  // Determine if conversion is complete
	return ReadDone;  // True if conversion complete, False if conversion ongoing
}

bool EdgeSensor::EdgeSensorVal(void)
{
	bool ReadVal;
	uint16_t threshold = 2000;
	// uint16_t ADC_Val = // Obtains ADC value
	ReadVal = (ADC_Val>threshold?true:0); // Return true if edge detected or false if not
	return ReadVal;
}

void Edge_Detection()
{
	
	Share <uint8_t> edge_out->put();  // Define share to be read by Mastermind
	
	for(;;)
	{
		edge_out->put(0);
		
		if (EdgeSensorReadDone())
		{
			EdgeSensorVal()
		}
		
		if (EdgeSensorVal(edgeAchan)) // If edge detection sensor reading is above the threshold
		{
			edge_out->put(1);    // 00000001
		}
		
		if (EdgeSensorVal(edgeBchan))
		{
			edge_out->put(edge_out->get() | (1<<1)); // 00000010
		}
		
		if (EdgeSensorVal(edgeCchan))
		{
			edge_out->put(edge_out->get() | (1<<2)); // 00000100
		}
		
		if (EdgeSensorVal(edgeDchan))
		{
			edge_out->put(edge_out->get() | (1<<3)); // 00001000
		}
		
		if (EdgeSensorVal(edgeEchan))
		{
			edge_out->put(edge_out->get() | (1<<4));  // 00010000
		}
		
		if (EdgeSensorVal(edgeFchan))
		{
			edge_out->put(edge_out->get() | (1<<5));  // 00100000
		}
		
		// return edge_out;
		vTaskDelay(1); // Delay task 1ms
	}
}