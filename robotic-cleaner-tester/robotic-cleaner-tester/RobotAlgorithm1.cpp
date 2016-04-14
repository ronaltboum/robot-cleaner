//#include "stdafx.h"
#include "RobotAlgorithm1.h"
#include <iostream>

RobotAlgorithm1::RobotAlgorithm1(const AbstractSensor& sensor, map<string, int> config)
{
	initiallize();
	setSensor(sensor);
	setConfiguration(config);
}

RobotAlgorithm1::RobotAlgorithm1(void)
{
	initiallize();
}

//function called in each c'tor
void RobotAlgorithm1::initiallize()
{
	_dirtInCurrentLocation = 0;
	_configs = map<string,int>();
	_battery = Battery();
	_pathFromDocking = deque<Direction>();
	_robotStatus = AlgorithmStatus::ChargingInDocking;
}

RobotAlgorithm1::~RobotAlgorithm1(void)
{
}

//************************************
// Brief:		choose a step and do it
// Returns:   	Direction where it went
//************************************
Direction RobotAlgorithm1::step()
{
	Direction lastStep;
	UpdateState();
	switch(_robotStatus)
	{
		
	case AlgorithmStatus::StayingUntilClean:
	case AlgorithmStatus::ChargingInDocking:
		 return Direction::Stay;
	case AlgorithmStatus::Returning:
		lastStep  = _pathFromDocking.back();
		_pathFromDocking.pop_back();
		return OppositeDirection(lastStep);
	case AlgorithmStatus::Exploring:
		//set the last step, or stay if no last step found
		lastStep  = _pathFromDocking.empty() ? Direction(4) : _pathFromDocking.back();
		vector<Direction> possible_directions = GetPossibleDirections(lastStep); // get all other directions than lastStep
		if(possible_directions.empty()){
			_pathFromDocking.pop_back();
			return OppositeDirection(lastStep);
		}
		else{
			Direction chosen = possible_directions[0];
			_pathFromDocking.push_back(chosen);
			return chosen;
		}
	}
	return Direction::Stay;
}

//************************************
// Brief:		change the algorithm state, consume/recharge battery, and clean current location by 1 if not cleaned
// Returns:   	void
void RobotAlgorithm1::UpdateState()
{
	switch(_robotStatus)
	{
	case AlgorithmStatus::ChargingInDocking:
		if(_battery.OneRechargeBeforeFullyRecharged())
			_robotStatus = AlgorithmStatus::Exploring;
		_battery.Recharge();
		break;
	case AlgorithmStatus::StayingUntilClean:
	case AlgorithmStatus::Exploring:
		_battery.Consume();
		_dirtInCurrentLocation -= _dirtInCurrentLocation ? 1 : 0; //clean by one (or stays 0)
		if(_battery.GetStepsBeforeRecharge() == _pathFromDocking.size())
			_robotStatus = AlgorithmStatus::Returning;
		else if(_dirtInCurrentLocation > 0)
			_robotStatus = AlgorithmStatus::StayingUntilClean;
		else
			_robotStatus = AlgorithmStatus::Exploring;
		break;
	case AlgorithmStatus::Returning:
		_battery.Consume();
		if(IsInDocking())
			_robotStatus = AlgorithmStatus::ChargingInDocking;
		break;
	}

}

// Brief: returns opposite direction
Direction RobotAlgorithm1::OppositeDirection(Direction d)
{
	switch (d)
	{
	case Direction::North:
		return Direction::South;
	case Direction::South:
		return Direction::North;
	case Direction::East:
		return Direction::West;
	case Direction::West:
		return Direction::East;
	default:
		return Direction::Stay;
	}
}

//************************************
// Brief:		Gets the last committed steps and return all possible steps excluding going back and staying
// Gets:	 	Direction lastStep - the last direction the algorithm moved to
// Returns:   	vector<Direction> - all possible steps excluding going back and staying. empty if non possiblle
//************************************
vector<Direction> RobotAlgorithm1::GetPossibleDirections(Direction lastStep) const
{
	Direction back = OppositeDirection(lastStep);
	SensorInformation info = _robotSensor->sense();
	vector<Direction> possibleDirections =  vector<Direction>();
	for(int i=0;i<4;++i){
		if( ! info.isWall[i] && Direction(i) != back)
			possibleDirections.push_back(Direction(i));
	}
	return possibleDirections;
}

extern "C" {
AbstractAlgorithm *maker(){
   return new RobotAlgorithm1;
}
class proxy { 
public:
   proxy(){
      // register the maker with the factory 
      factory["RobotAlgorithm1"] = maker;
   }
};
// our one instance of the proxy
proxy p;
}

