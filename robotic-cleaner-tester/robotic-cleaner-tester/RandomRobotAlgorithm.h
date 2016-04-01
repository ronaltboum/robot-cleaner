#ifndef RandomRobotAlgorithm_h__
#define RandomRobotAlgorithm_h__

#include "AbstractAlgorithm.h"
#include "Sensor.h"
#include <stdlib.h>
#include <stdio.h>

namespace ns_robotic_cleaner_simulator
{


class RandomRobotAlgorithm : public AbstractAlgorithm
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	const AbstractSensor* robotSensor;
	map<string,int> configs;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	RandomRobotAlgorithm(const AbstractSensor& sensor, map<string, int> config);
	~RandomRobotAlgorithm(void){}

public:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Direction step();

	void setSensor(const AbstractSensor& sensor) 
	{
		robotSensor = &sensor;
	}

	void setConfiguration(map<string, int> config)
	{
		configs = map<string,int>(config);
	}

	void aboutToFinish(int stepsTillFinishing){}

};


} // end of namespace ns_robotic_cleaner_simulator



#endif //RandomRobotAlgorithm
