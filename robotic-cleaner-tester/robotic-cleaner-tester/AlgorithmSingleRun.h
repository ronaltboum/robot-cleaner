#ifndef AlgorithmSingleRun_h__
#define AlgorithmSingleRun_h__

#include "AbstractAlgorithm.h"
#include "Battery.h"
#include "House.h"
#include "Point.h"
#include "RandomRobotAlgorithm.h"

namespace ns_robotic_cleaner_simulator
{

class AlgorithmSingleRun
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	int _simulationSteps; // steps committed in the run
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

