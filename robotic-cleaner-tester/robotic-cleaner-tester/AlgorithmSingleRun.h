
#ifndef AlgorithmSingleRun_h__
#define AlgorithmSingleRun_h__

#include "House.h"
//#include "Point.h"  ////don't think we need it because House.h includes Point.h
//#include "AbstractAlgorithm.h"  //don't think we need it because RandomRobotAlgorithm.h includes "AbstractAlgorithm.h"
//#include "Battery.h" //don't think we need it because House.h includes Battery.h
#include "RandomRobotAlgorithm.h"

namespace ns_robotic_cleaner_simulator
{

class AlgorithmSingleRun
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	int _simulationSteps; // steps commited in the run
	//AbstractAlgorithm &_currentAlgorithm;
	Battery _robotBattery;
	House &_currentHouse;
	Point _currentPosition;
	bool _valid;
	int _dirtCollected;

public:
	AlgorithmSingleRun(void);
	~AlgorithmSingleRun(void);
};

} //end of namespace ns_robotic_cleaner_simulator

#endif // AlgorithmSingleRun_h__

