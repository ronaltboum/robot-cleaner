#ifndef RandomRobotAlgorithm_h__
#define RandomRobotAlgorithm_h__

#include "AbstractAlgorithm.h"
#include "Battery.h"

namespace ns_robotic_cleaner
{
	class RandomRobotAlgorithm : public AbstractAlgorithm
	{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		private:
			//const AbstractSensor robotSensor;  //compiler says error
			const AbstractSensor& robotSensor;   //the sensor is passed to the algorithm by
			//the simulator when the algorithm is initialized
			map<string,int> configs;
			const Battery& robotBattery;
	};
} // end of namespace ns_robotic_cleaner

//Class RandomRobotAlgorithm		 implements algorithm – includes random step()
//	members (private):
//	const AbstractSensor robotSensor;
//map<string,int> configs;
//const battery & robotBattery;
//functions:
//void SetSensor(const AbstractSensor & sensor)
//	void SetConfiguration(map<string,int> configs)
//	Direction step()
//	void AboutToFinish(StepTillFinishing)



#endif //RandomRobotAlgorithm