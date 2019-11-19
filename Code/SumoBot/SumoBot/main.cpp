/*
 * SumoBot.cpp
 *
 * Created: 11/17/2019 2:37:51 PM
 * Author : melab15
 */ 

#include <avr/io.h>

int16_t ReadADC(uint8_t Channel) 
{
	// Four channels: four separate pairs of MUX control registers
	if (!(ADCA.CTRLA & ADC_ENABLE_bm)) // If the enable pin has not been set for the ADC
	{
		ADCA.CTRLA = ADC_ENABLE_bm;  // Enable the ADC
		
		ADCA.PRESCALER = ADC_PRESCALER_DIV256_gc;  // ADC Frequency = 
	}
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc; // Set single ended ADC input mode and gain of 1
	
	
	
	
}


uint8_t Edge_Detection()
{
	// Setup Edge Detection
	edgeAchan = 
	edgeBchan = 
	edgeCchan =
	edgeDchan =
	edgeEchan =
	edgeFchan =
	edge_threshold = 
	
	for(;;)
	{
		if (ReadADC(edgeAchan) > edge_threshold)
		{
			edge_out = 1;
		}
		
		if (ReadADC(edgeBchan) > edge_threshold)
		{
			edge_out = edge_out | (1<<1);
		}
		
		if (ReadADC(edgeCchan) > edge_threshold)
		{
			edge_out = edge_out | (1<<2);
		}
		
		if (ReadADC(edgeDchan) > edge_threshold)
		{
			edge_out = edge_out | (1<<3);
		}
		
		if (ReadADC(edgeEchan) > edge_threshold)
		{
			edge_out = edge_out | (1<<4);
		}
		
		if (ReadADC(edgeFchan) > edge_threshold)
		{
			edge_out = edge_out | (1<<5);
		}
		
		return edge_out;
		vTaskDelay();
	}
}

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

