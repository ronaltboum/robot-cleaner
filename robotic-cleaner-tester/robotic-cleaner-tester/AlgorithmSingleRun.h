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
	const map<string, int> & _configs;
	AbstractAlgorithm * _currentAlgorithm;
	Battery * _robotBattery;
	House * _currentHouse;
	Point * _currentPosition;
	int _numberOfStepsCommited;
	AbstractSensor * _algorithmSensor;
	bool _canStillRun; // boolean which is false after made illegal move, when battery is empty, or when members are uninitiallized
	int _dirtCollected;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	AlgorithmSingleRun(const map<string, int> & configs, AbstractAlgorithm * currentAlgorithmPointer, Battery robotBattery, House * currentHousePointer);
	AlgorithmSingleRun(void);
	~AlgorithmSingleRun(void);
	void initialize();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	void Run();
	void HandleNewMove();
};

} //end of namespace ns_robotic_cleaner_simulator

#endif // AlgorithmSingleRun_h__

