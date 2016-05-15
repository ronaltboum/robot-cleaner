//#include "stdafx.h"
#include "_039563838_D.h"
//#include "AlgorithmRegistration.h"
#include <iostream>

using namespace std;

_039563838_D::_039563838_D(const AbstractSensor& sensor, map<string, int> config)
{
	initiallize();
	setSensor(sensor);
	setConfiguration(config);
}

_039563838_D::_039563838_D(void)
{
	initiallize();
}

//function called in each c'tor
void _039563838_D::initiallize()
{
	_dirtInCurrentLocation = 0;
	_configs = map<string,int>();
	_battery = Battery();
	_pathFromDocking = deque<Direction>();
	_robotStatus = AlgorithmStatus::ChargingInDocking;
}

_039563838_D::~_039563838_D(void)
{
}

//************************************
// Brief:		choose a step and do it
// Returns:   	Direction where it went
//************************************
Direction _039563838_D::step(Direction lastStep)
{
	//Direction lastStep;
	updateAlgorithmInfo(lastStep);
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
		vector<Direction> possible_directions = GetPossibleDirections(lastStep); // get all directions besides of  {lastStep, opposite of lastStep, stay}
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
void _039563838_D::UpdateState()
{
  
	
 	SensorInformation info = _robotSensor->sense();
 	_dirtInCurrentLocation = info.dirtLevel;
	
	//cout << "in A dirt = " << _dirtInCurrentLocation << endl;
	
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
Direction _039563838_D::OppositeDirection(Direction d)
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
// Brief:		Gets the last committed step and returns all possible steps excluding going back and staying
// Gets:	 	Direction lastStep - the last direction the algorithm moved to
// Returns:   	vector<Direction> - all possible steps excluding going back and staying. empty if non possiblle
//************************************
vector<Direction> _039563838_D::GetPossibleDirections(Direction lastStep) const
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



void _039563838_D::updateAlgorithmInfo(Direction lastStep) 
{
        //++totalSteps;
	SensorInformation s =  _robotSensor ->sense();   
        position.move(lastStep); // update the robot position, as managed by the algorithm, to the new position. It's NOT the same as the Point* field of the sensor

//        s = sensor->sense();
//        debug << "position: " << position << ", dirtLevel = " << s.dirtLevel << endl;
//        // update the map with the info on the dirt level
        stepsFromDocking = calcStepsToDocking(stepsFromDocking + 1, position);
	CellInfo cellInfo;
	if(s.dirtLevel != 0)
		cellInfo.dirt = s.dirtLevel - 1;
	else
		cellInfo.dirt = 0;
	cellInfo.stepsToDocking = stepsFromDocking;
	//cellInfo.isWall = false;
	//houseMapping[position] = {(s.dirtLevel != 0)? s.dirtLevel - 1 : 0, stepsFromDocking};
	houseMapping[position] = cellInfo;
        // update all 4 cells around me with Wall information and if possible also with stepsToDocking
        for(Direction d : directions) {
            GeneralizedPoint checkPosition = position;
            checkPosition.move(d);
            auto neighbour = houseMapping.find(checkPosition);
            if(neighbour == houseMapping.end()) {
                houseMapping[checkPosition].isWall = s.isWall[(int)d];
                if(!s.isWall[(int)d]) {
                    houseMapping[checkPosition].stepsToDocking = calcStepsToDocking(stepsFromDocking + 1, checkPosition);
                }
            }
        }
        // for debug
        printDebugHouseMapping();	
}


int _039563838_D::calcStepsToDocking(int stepsFromDocking, const GeneralizedPoint& position) 
{
        // if this cell already has a better route - keep it!
        auto thisCellInfo = houseMapping.find(position);
        if(thisCellInfo != houseMapping.end() && thisCellInfo->second.stepsToDocking != -1 &&
           thisCellInfo->second.stepsToDocking < stepsFromDocking) {
                return thisCellInfo->second.stepsToDocking;
        }
        // check if there is a better route via neighbours that we visited already
        int bestRouteSteps = stepsFromDocking;
        for(Direction d: directions) {
            GeneralizedPoint pointToCheck = position;
            pointToCheck.move(d);
            auto neighbourInfo = houseMapping.find(pointToCheck);
            if(neighbourInfo != houseMapping.end() && neighbourInfo->second.stepsToDocking != -1 &&
               neighbourInfo->second.stepsToDocking < bestRouteSteps - 1) {
                    bestRouteSteps = neighbourInfo->second.stepsToDocking + 1;
            }
        }
        if(bestRouteSteps < stepsFromDocking) {
            updateStepsToDocking(bestRouteSteps, position);
        }
        return bestRouteSteps;   
         
    }


void _039563838_D::updateStepsToDocking(int stepsToDocking, const GeneralizedPoint& position) 
{
	if(houseMapping[position].stepsToDocking > stepsToDocking || houseMapping[position].stepsToDocking == -1) {
	    houseMapping[position].stepsToDocking = stepsToDocking;
	    for(Direction d: directions) {
		GeneralizedPoint neighbour = position;
		neighbour.move(d);
		auto neighbourInfo = houseMapping.find(neighbour);
		if(neighbourInfo != houseMapping.end()) {
		    CellInfo cell = neighbourInfo->second;
		    if(cell.stepsToDocking > stepsToDocking + 1 || (!cell.isWall && cell.stepsToDocking == -1) ) {
		        updateStepsToDocking(stepsToDocking + 1, neighbour);
		    }
		}
	    }
	}
}


//for debug:
void _039563838_D::printDebugHouseMapping() 
{

	if(true) {
	
            cout << "---------------------------" << endl;
            //cout << "totalSteps: " << totalSteps << endl;
            cout << "---------------------------" << endl;
            for(auto& a : houseMapping) {
                // debug print of all the points mapped so far
                cout << "( " << a.first.getX() << " , " << a.first.getY() << ") : dirt = " << a.second.dirt 
                        << ", stepsToDocking = " << a.second.stepsToDocking 
                        << ", isWall = " << (a.second.isWall? "true":"false") << endl;
            }
            cout << "---------------------------" << endl;
        }


	
//        if(debug) {
//	
//            debug << "---------------------------" << endl;
//            //debug << "totalSteps: " << totalSteps << endl;
//            debug << "---------------------------" << endl;
//            for(auto& a : houseMapping) {
//                // debug print of all the points mapped so far
//                debug << a.first << ": dirt = " << a.second.dirt 
//                        << ", stepsToDocking = " << a.second.stepsToDocking 
//                        << ", isWall = " << (a.second.isWall? "true":"false") << endl;
//            }
//            debug << "---------------------------" << endl;
//        }

}





//REGISTER_ALGORITHM (_039563838_D)


 extern "C" {
 AbstractAlgorithm *maker(){
    return new _039563838_D;
 }
 class proxy { 
 public:
    proxy(){
       // register the maker with the factory using file name 
       factory["039563838_D_"] = maker;
    }
 };
 // our one instance of the proxy
 proxy p;
 }

