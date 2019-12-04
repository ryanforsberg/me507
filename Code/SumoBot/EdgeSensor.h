/*
 * EdgeSensor.h
 *
 * Created: 11/17/2019 2:37:51 PM
 * Author : Ryan Forsberg
 */ 

struct QTRsensor
{
	register16_t* CTRLPORT;
	
	};


class EdgeSensor
{
	// QTR Edge Sensor class for Sumo Bot
	EdgeSensor(void)
	void EdgeSensorRead(void);
	bool EdgeSensorReadDone(void);
};