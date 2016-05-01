//#include "stdafx.h"
#include "_039563838_B.h"
#include <iostream>

_039563838_B::_039563838_B(const AbstractSensor& sensor, map<string, int> config)
{
	initiallize();
	setSensor(sensor);
	setConfiguration(config);
}

_039563838_B::_039563838_B(void)
{
	initiallize();
}

//function called in each c'tor
void _039563838_B::initiallize()
{
	_dirtInCurrentLocation = 0;
	_configs = map<string,int>();
	_battery = Battery();
	_pathFromDocking = deque<Direction>();
	_robotStatus = AlgorithmStatus::ChargingInDocking;
}

_039563838_B::~_039563838_B(void)
{
}

//************************************
// Brief:		choose a step and do it
// Returns:   	Direction where it went
//************************************
Direction _039563838_B::step()
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
		//preferences:  1.North  2.South  3.West  4.East  5.Stay
		Direction preferredDirection  = ChooseDirectionByPreference();
		if(preferredDirection != Direction::Stay)
		    _pathFromDocking.push_back(preferredDirection);
		return preferredDirection;
	}
	return Direction::Stay;
}

//************************************
// Brief:		change the algorithm state, consume/recharge battery, and clean current location by 1 if not cleaned
// Returns:   	void
void _039563838_B::UpdateState()
{
  
	SensorInformation info = _robotSensor->sense();
 	_dirtInCurrentLocation = info.dirtLevel;
  
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
		if(_battery.GetStepsBeforeRecharge() ==    (int)_pathFromDocking.size())
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
Direction _039563838_B::OppositeDirection(Direction d)
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
//Brief: returns Direction according to the following preferences: 1.NORTH 2.SOUTH 3.WEST 4.EAST 5.STAY
//i.e. info[0,3] in reverse order
//************************************
Direction _039563838_B::ChooseDirectionByPreference() const
{
	
	SensorInformation info = _robotSensor->sense();
	for(int i = 3; i >= 0 ; --i){
		if( ! info.isWall[i] )
			return Direction(i);
	}
	
	return Direction::Stay;
}

extern "C" {
AbstractAlgorithm *maker(){
   return new _039563838_B;
}
class proxy { 
public:
   proxy(){
      // register the maker with the factory using file name 
      factory["039563838_B_"] = maker;
   }
};
// our one instance of the proxy
proxy p;
}

