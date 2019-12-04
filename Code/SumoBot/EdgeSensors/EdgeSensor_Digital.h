/*
 * EdgeSensor.h
 *
 * Created: 11/17/2019 2:37:51 PM
 * Author : Ryan Forsberg
 */ 

#include "shares.h"

class EdgeSensor : public frt_task
{
	// QTR Edge Sensor class for Sumo Bot
	public:
	EdgeSensor(const char*, unsigned portBASE_TYPE, size_t, emstream* );
	void Edge_Detection(void);
};