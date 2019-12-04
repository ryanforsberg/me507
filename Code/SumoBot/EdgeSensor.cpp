/*
 * EdgeSensor.cpp
 *
 * Created: 11/17/2019 2:37:51 PM
 * Author : Ryan Forsberg
 */ 

#include "EdgeSensor.h"

uint16_t EdgeSensor::EdgeSensorReadStart(uint8_t Channel)
{
	// Four channels: four separate pairs of MUX control registers
	if (!(ADCA.CTRLA & ADC_ENABLE_bm)) // If the enable pin has not been set for the ADC
	{
		ADCA.CTRLA = ADC_ENABLE_bm;  // Enable the ADC
		
		ADCA.PRESCALER = ADC_PRESCALER_DIV256_gc;  // ADC Frequency = CPU frequency/ADC Prescaler
	}
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc; // Set single ended ADC input mode and gain of 1
	
}

bool EdgeSensor::EdgeSensorReadDone(void)
{
	bool ReadDone;
	ReadDone = 
	return ReadDone;
}

bool EdgeSensor::EdgeSensorReadVal(void)
{
	bool ReadVal;
	uint16_t threshold = 2000;
	uint16_t ADC_Val = // Obtains ADC value
	ReadVal = (ADC_Val>threshold?true:0); // Return true if edge detected or false if not
	return ReadVal;
}

void Edge_Detection()
{
	// Setup Edge Detection
	edgeAchan =
	edgeBchan =
	edgeCchan =
	edgeDchan =
	edgeEchan =
	edgeFchan =
	
	Share <uint8_t> edge_out;  // Define share to be read by Mastermind
	
	for(;;)
	{
		if (EdgeSensorReadVal(edgeAchan)) // If edge detection sensor reading is above the threshold
		{
			edge_out = 1;    // 00000001
		}
		
		if (EdgeSensorReadVal(edgeBchan))
		{
			edge_out = edge_out | (1<<1); // 00000010
		}
		
		if (EdgeSensorReadVal(edgeCchan))
		{
			edge_out = edge_out | (1<<2); // 00000100
		}
		
		if (EdgeSensorReadVal(edgeDchan))
		{
			edge_out = edge_out | (1<<3); // 00001000
		}
		
		if (EdgeSensorReadVal(edgeEchan))
		{
			edge_out = edge_out | (1<<4);  // 00010000
		}
		
		if (EdgeSensorReadVal(edgeFchan))
		{
			edge_out = edge_out | (1<<5);  // 00100000
		}
		
		// return edge_out;
		vTaskDelay(1); // Delay task 1ms
	}
}