#ifndef RandomRobotAlgorithm_h__
#define RandomRobotAlgorithm_h__

#include "AbstractAlgorithm.h"
#include "Battery.h"
#include "Direction.h"
#include "Point.h"
#include "Sensor.h"
#include <vector>
#include <stdlib.h>
#include <array>

namespace ns_robotic_cleaner_simulator
{


class RandomRobotAlgorithm : public AbstractAlgorithm
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	//const AbstractSensor robotSensor;  //compiler says error
	AbstractSensor & robotSensor;   //the sensor is passed to the algorithm by
	//the simulator when the algorithm is initialized
	map<string,int> configs;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	RandomRobotAlgorithm(void);
	RandomRobotAlgorithm(AbstractSensor& sensor, map<string, int> config)
		: robotSensor(sensor)
	{
		setSensor(sensor);
		setConfiguration(config);
	}
	~RandomRobotAlgorithm(void){}

public:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	Direction step()
	{
		SensorInformation info = robotSensor.sense();
		vector<Direction> possibleDirections;
		for(int i=0;i<4;++i){
			if(info.isWall[i])
				possibleDirections.push_back(Direction(i));
		}
		possibleDirections.push_back(Direction::Stay);
		int chosenDirectionIndex = rand() % possibleDirections.size();
		return possibleDirections.at(chosenDirectionIndex);
	}

	void setSensor(const AbstractSensor& sensor) override
	{
		robotSensor = sensor;
	}

	void setConfiguration(map<string, int> config) override
	{
		configs = map<string,int>(config);
	}

	void aboutToFinish(int stepsTillFinishing) override{}

	//points we can go to, i.e. points which aren't walls or outside the house


};


} // end of namespace ns_robotic_cleaner_simulator



#endif //RandomRobotAlgorithm