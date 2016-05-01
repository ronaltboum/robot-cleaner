//#include "stdafx.h"
#include "_039563838_C.h"
#include <iostream>

_039563838_C::_039563838_C(const AbstractSensor& sensor, map<string, int> config)
{
	initiallize();
	setSensor(sensor);
	setConfiguration(config);
}

_039563838_C::_039563838_C(void)
{
	initiallize();
}

//function called in each c'tor
void _039563838_C::initiallize()
{
	_dirtInCurrentLocation = 0;
	_configs = map<string,int>();
	_battery = Battery();
	_pathFromDocking = deque<Direction>();
	_robotStatus = AlgorithmStatus::ChargingInDocking;
}

_039563838_C::~_039563838_C(void)
{
}

//************************************
// Brief:		choose a step and do it
// Returns:   	Direction where it went
//************************************
Direction _039563838_C::step()
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
		//preferences:  1.East  2.West  3.South  4.North  5.Stay
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
void _039563838_C::UpdateState()
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
		if(_battery.GetStepsBeforeRecharge() ==  (int)_pathFromDocking.size())
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
Direction _039563838_C::OppositeDirection(Direction d)
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
// Brief: returns Direction according to the following preferences: 1.EAST 2.WEST 3.SOUTH 4.NORTH 5.STAY
//************************************
Direction _039563838_C::ChooseDirectionByPreference() const
{
	
	SensorInformation info = _robotSensor->sense();
	for(int i=0;i < 4; ++i){
		if( ! info.isWall[i] )
			return Direction(i);
	}
	
	return Direction::Stay;
}

extern "C" {
AbstractAlgorithm *maker(){
   return new _039563838_C;
}
class proxy { 
public:
   proxy(){
      // register the maker with the factory using file name 
      factory["039563838_C_"] = maker;
   }
};
// our one instance of the proxy
proxy p;
}

