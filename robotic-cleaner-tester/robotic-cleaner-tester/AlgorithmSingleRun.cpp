#include "stdafx.h"
#include "AlgorithmSingleRun.h"
#include <windows.h>

//comment just to test git


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
		_algorithmSensor = new Sensor(_currentHouse,_currentPosition);
		_currentAlgorithm->setSensor(*_algorithmSensor);
		_canStillRun = true;
	}


	
	AlgorithmSingleRun::~AlgorithmSingleRun(void)
	{
		//no need to check if not null - delete(NULL) = no_op
		delete _currentPosition;
		delete _currentAlgorithm;
		delete _currentHouse;
		delete _robotBattery;
		delete _algorithmSensor;
	}

	void AlgorithmSingleRun::initialize()
	{
		_algorithmSensor = NULL;
		_currentAlgorithm = NULL;
		_currentHouse = NULL;
		_currentPosition = NULL;
		_robotBattery = NULL;
		_canStillRun = false;
		_dirtCollected = 0;
		_numberOfStepsCommited = 0;
	}

	//************************************
	// Brief:		Run algorithm
	// Returns:   	void
	// Access:    	public 
	// Pre:			_currentAlgorithm = NULL;
	//				_currentHouse != NULL;
	//				_currentPosition != NULL;
	//				_robotBattery != NULL;
	//				_configs.find("MaxSteps") != _configs.end()
	// Post:		-
	//************************************
	void AlgorithmSingleRun::Run()
	{
		int maximalStepsNumber = _configs.find("MaxSteps")->second;
		for(	_numberOfStepsCommited = 0; 
				((_numberOfStepsCommited < maximalStepsNumber) && (_canStillRun) && ( ! _currentHouse->IsClean()));
				++_numberOfStepsCommited)
		{
			_currentHouse->Print(*_currentPosition);
			Direction chosenDirection = _currentAlgorithm->step();
			_currentPosition->Move(chosenDirection);
			HandleNewMove();
			Sleep(1000*2);
		}
	}

	
	//************************************
	// Brief:		Change the status of members according to the move commited
	// Pre:			-
	// Post:		-
	//************************************
	void AlgorithmSingleRun::HandleNewMove()
	{
		if(_currentHouse->IsDockingStation(*_currentPosition))
			_robotBattery->Recharge();
		else
			_robotBattery->Consume();
		if( ( ! _currentHouse->IsPositionValid(*_currentPosition)) || (_robotBattery->IsBatteryEmpty()))
			_canStillRun = false;
		if(! _currentHouse->IsDirty(*_currentPosition) ){
			_currentHouse->Clean(*_currentPosition);
			_dirtCollected++;
		}
	}

}
