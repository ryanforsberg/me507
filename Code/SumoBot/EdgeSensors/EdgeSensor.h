/*
 * EdgeSensor.h
 *
 * Created: 11/17/2019 2:37:51 PM
 * Author : Ryan Forsberg
 */ 

#include "shares.h"

struct QTRsensor
{
	uint16_t CtrlPort; // Memory address for port that holds CtrlPin (Port C, Pin 0)
	uint8_t CtrlPinBM;		// Bit Mask for CtrlPin used to set to output
	
	uint16_t OutPort;		// Memory address for ADC port
	uint8_t OutPinBM;		// Bit Mask for OutPin used to set to input
	
	uint16_t ADCChan;		// Memory address of ADC channel
	uint16_t CtrlA;			// Memory address of CTRLA in the ADC register
	uint16_t CtrlB;			// Memory address of CTRLB in the ADC register
	
	uint16_t MuxCtrl;		// Memory address of the MuxCtrl in ADCChan
	uint8_t MuxPos;			// Bit Mask for which ADC pin is to be read
	
	uint16_t IntFlags;		// Memory address for IntFlags in ADCChan, used for determining if conversion is complete
	
	
	// uint8_t ChxIf;			// BitMask for Interrupt Flag
	uint8_t ChxStart;		// BitMask for which channel (out of 4) is beginning a conversion: bits 2-5 are options for CtrlA
	
	
	uint16_t ChxResult;		// Memory address of ADC conversion result
	
	};


class EdgeSensor : public frt_task
{
	// QTR Edge Sensor class for Sumo Bot
	protected:
	struct QTRsensor EdgeSensorPerm;
	
	public:
	EdgeSensor(struct QTRsensor EdgeSensorInfo, const char*, unsigned portBASE_TYPE, size_t, emstream* );
	void EdgeSensorRead(void);
	bool EdgeSensorReadDone(void);
	bool EdgeSensorVal(void);
};