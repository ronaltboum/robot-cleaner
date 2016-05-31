//#include "stdafx.h"
#include "_039563838_B.h"
#include "AlgorithmRegistration.h"
#include <iostream>

using namespace std;

_039563838_B::_039563838_B(const AbstractSensor& sensor, map<string, int> config)
{
	DynamicAlgo::initiallize();
	DynamicAlgo::setSensor(sensor);
	DynamicAlgo::setConfiguration(config);

	int _maxSteps =  _battery.GetOneWayDistanceFromDocking();
	if(_debug) cout << "one way: "  << _maxSteps << endl;
}

_039563838_B::_039563838_B(void)
{
	DynamicAlgo::initiallize();
}

//function called in each c'tor
void _039563838_B::initiallize()
{
	DynamicAlgo::initiallize();
}

_039563838_B::~_039563838_B(void)
{
}

Direction _039563838_B::ChooseStepAccordingToState(Direction lastStep)
{
	if(_debug) PrintAlgorithmStatus();
	Direction d;
	switch(_robotStatus)
	{
	case AlgorithmStatus::Done:
	case AlgorithmStatus::ChargingInDocking:
		if(_debug) cout << "charging" << endl;
		 return Direction::Stay;
	case AlgorithmStatus::Cleaning:
		return DynamicAlgo::Handle_Cleaning_State(lastStep);
	case AlgorithmStatus::Returning:
		return DynamicAlgo::Handle_Returning_State(lastStep);
	case AlgorithmStatus::Exploring:
		d = DynamicAlgo::Handle_Explore_State();
		if(_debug) cout << "explore state returned" << d << endl;
		return d;
	//shouldn't get here
	case AlgorithmStatus::StayingToClean:
	case AlgorithmStatus::FirstStep:
	default:
		return Direction::Stay;
	} //closes switch
}

//************************************
// Brief:		change the algorithm state, consume/recharge battery, and clean current location by 1 if not cleaned
// Returns:   	void
void _039563838_B::UpdateState()
{
	// if(_debug){
	// 	cout << "_039563838_B::UpdateState" << endl;
	// 	PrintAlgorithmStatus();  //for debug.  delete later!!!!
	// }

	if(_robotStatus == AlgorithmStatus::Done)
		return;

	if(_allExplored && _allCleaned && DynamicAlgo::IsInDocking()){
		_robotStatus = AlgorithmStatus::Done;
		return;
	}

	int remainingSteps = DynamicAlgo::GetRemainingSteps();
	if(_debug) 
		cout << " updatestate remaining: "<< remainingSteps << " from here " << position << ": "<< houseMapping.at(position).stepsToDocking << endl ;
	if(remainingSteps - 1 <= houseMapping.at(position).stepsToDocking && _robotStatus != AlgorithmStatus::Returning){
		_robotStatus = AlgorithmStatus::Returning;
		if(_debug){
			auto p = houseMapping.find(position);
			cout << endl << "starting to return. battery= " << _battery.GetBattery_level() << "/" <<  _battery.GetBattery_capacity()
				<< "from: "<< position << " d: " << p->second.stepsToDocking  << endl;
		}
		return;	
	}

	switch(_robotStatus)
	{
		case AlgorithmStatus::FirstStep:
			_robotStatus = _allExplored ? AlgorithmStatus::Cleaning : AlgorithmStatus::Exploring ;
			return;
		case AlgorithmStatus::ChargingInDocking:
			{
				if(_battery.OneRechargeBeforeFullyRecharged())
					_robotStatus = _allExplored ? AlgorithmStatus::Cleaning : AlgorithmStatus::Exploring ;
			}
			return;
		case AlgorithmStatus::Exploring:
			{
				if(_allExplored){
					if(_debug && ! _allCleaned) cout << endl << "all explored. step: " << totalSteps << endl;
					if(_debug && _allCleaned) cout << endl << "all explored + cleaned. step: " << totalSteps << endl;
					_robotStatus = _allCleaned ? AlgorithmStatus::Returning : AlgorithmStatus::Cleaning;
				}
			}
			return;
		case AlgorithmStatus::Returning:
			{
				if(IsInDocking()){
					_robotStatus = _allCleaned ? AlgorithmStatus::Done : AlgorithmStatus::ChargingInDocking;
				}
			}
			return;
			case AlgorithmStatus::Cleaning: //staying on clean for now
				if(_allCleaned){
					if(_debug) cout << endl << "all cleaned" << endl;
					_robotStatus = AlgorithmStatus::Returning;
				}
				return;
			case AlgorithmStatus::Done:
			default:
				return;
				//cout << "UpdateState shouldn't get here" << endl;
				// PrintAlgorithmStatus();

	}

}


REGISTER_ALGORITHM (_039563838_B)
