//#include "stdafx.h"
#include "AlgorithmSingleRun.h"

AlgorithmSingleRun::AlgorithmSingleRun(const map<string, int> & configs, AbstractAlgorithm * currentAlgorithmPointer, const Battery & robotBattery, House * currentHouse, AbstractSensor * algoSensor, Point * startingPoint, string algorithmFileName)
	: _configs(configs)
{
	initialize();
	_algorithmSensor = algoSensor;
	_currentAlgorithm = currentAlgorithmPointer; 
	_robotBattery = new Battery(robotBattery);
	_currentHouse = currentHouse;
	_currentPosition = startingPoint;
	_canStillRun = true;
	_sumOfDirtBeforeCleaning = _currentHouse->SumOfDirtInTheHouse();
	_algorithmFileName = algorithmFileName;
	_lastDirection = Direction::Stay;
}



AlgorithmSingleRun::~AlgorithmSingleRun(void)
{
// 	//Tuval:
//   
// 	//no need to check if not null - delete(NULL) = no_op
// 	delete _currentPosition;
// 	// delete _currentHouse; //is done by simulator
// 	delete _algorithmSensor;
// 	delete _currentAlgorithm;
// 	delete _robotBattery;
  
  	//no need to check if not null - delete(NULL) = no_op
	delete _currentPosition;
	delete _currentHouse; 
	delete _algorithmSensor;
	delete _currentAlgorithm;
	delete _robotBattery;
}

void AlgorithmSingleRun::initialize()
{
	_algorithmSensor = NULL;
	_currentAlgorithm = NULL;
	_currentHouse = NULL;
	_currentPosition = NULL;
	_robotBattery = NULL;
	_canStillRun = false;
	_hitWall = false;
	_dirtCollected = 0;
	_numberOfStepsCommited = 0;
	_actual_position_in_copmetition = 0;
	_sumOfDirtBeforeCleaning = 0;
	_algorithmFileName = "";
}

bool AlgorithmSingleRun::CanDoStep() const
{
  //Tuval:
// 	return (	(_numberOfStepsCommited < _configs.find("MaxSteps")->second) &&
// 				(_canStillRun) &&
// 				( ! _currentHouse->IsHouseClean()));
	
	
	return (	(_numberOfStepsCommited <  (_currentHouse -> GetMaxSteps() )  ) &&
				(_canStillRun) &&
				 (! HasWon() ) )  ;
				//( ! _currentHouse->IsHouseClean()));
}

bool AlgorithmSingleRun::IsHouseCleaned() const
{
	return _currentHouse->IsHouseClean();
}

bool AlgorithmSingleRun::IsBackInDocking() const
{
	return _currentHouse->IsDockingStation(*_currentPosition);
} 

bool AlgorithmSingleRun::HasWon() const
{
	return IsBackInDocking() && IsHouseCleaned();
} 


//************************************
// Brief:		Moves one step and change the status of members according to the move committed
// Pre:			-
// Post:		-
//************************************
Direction AlgorithmSingleRun::DoStep(Direction lastStep)
{
	// charging and cleaning before the move
	if(_currentHouse->IsDockingStation(*_currentPosition))
		_robotBattery->Recharge();
	else
		_robotBattery->Consume();

	if( _currentHouse->IsDirty(*_currentPosition) ){
		_currentHouse->Clean(*_currentPosition);
		_dirtCollected++;
	}

	//making the move and updating 
	Direction chosenDirection = _currentAlgorithm->step(lastStep);
	_currentPosition->Move(chosenDirection);
	
	if(debug_AlgorithmSingleRun)  {  //for debugging
 		int pRow = _currentPosition -> GetRow();
 		int pCol = _currentPosition -> GetCol();
 		cout<< "In AlgorithmSingleRun:  new Point = " << pRow << " , " << pCol << endl;  //delte !!!!!!!!!!
	}
	
	if(HasMadeIllegalStep())
	{
		
		//Algorithm 331332334_E_ when running on House 003 went on a wall in step 7
		//http://moodle.tau.ac.il/mod/forum/discuss.php?d=57047
		_canStillRun = false;
		_hitWall = true;
	}
	if(IsAlgorithmBatteryEmpty())   //in this case we print the score, and we do not print error, as instructed in the link:  http://moodle.tau.ac.il/mod/page/view.php?id=375915
		_canStillRun = false;
	
	++_numberOfStepsCommited;
	_lastDirection = chosenDirection;
	return chosenDirection;
}

bool AlgorithmSingleRun::IsAlgorithmBatteryEmpty() const
{
	return (_robotBattery->IsBatteryEmpty() && ! _currentHouse->IsDockingStation(*_currentPosition));
}

bool AlgorithmSingleRun::HasMadeIllegalStep() const
{
	return (! _currentHouse->IsPositionValid(*_currentPosition));
}

