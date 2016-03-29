#include "stdafx.h"
#include "AlgorithmSingleRun.h"


namespace ns_robotic_cleaner_simulator
{
	
	AlgorithmSingleRun::AlgorithmSingleRun(void)
		: _configs(map<string,int>())
	{
		initialize();
	}

	AlgorithmSingleRun::AlgorithmSingleRun(const map<string, int> & configs, AbstractAlgorithm * currentAlgorithmPointer, Battery robotBattery, House * currentHousePointer)
		: _configs(configs)
	{
		initialize();
		_currentAlgorithm = currentAlgorithmPointer;
		_robotBattery = new Battery(robotBattery);
		_currentHouse = new House(*currentHousePointer);
		_currentPosition = _currentHouse->GetDockingStation(); // start at docking points
		_currentAlgorithm->setSensor(Sensor(_currentHouse,_currentPosition));
	}


	
	AlgorithmSingleRun::~AlgorithmSingleRun(void)
	{
		//no need to check if not null - delete(NULL) = no_op
		delete _currentPosition;
		delete _currentAlgorithm;
		delete _currentHouse;
		delete _robotBattery;
	}

	void AlgorithmSingleRun::initialize()
	{
		_currentAlgorithm = NULL;
		_currentHouse = NULL;
		_currentPosition = NULL;
		_robotBattery = NULL;
		_valid = false;
		_dirtCollected = 0;
	}

}
