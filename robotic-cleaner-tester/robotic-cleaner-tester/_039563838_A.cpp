//#include "stdafx.h"
#include "_039563838_A.h"
#include "AlgorithmRegistration.h"
#include <iostream>

using namespace std;

_039563838_A::_039563838_A(const AbstractSensor& sensor, map<string, int> config)
{
	initiallize();
	setSensor(sensor);
	setConfiguration(config);
}

_039563838_A::_039563838_A(void)
{
	initiallize();
}

//function called in each c'tor
void _039563838_A::initiallize()
{
	_dirtInCurrentLocation = 0;
	_configs = map<string,int>();
	_battery = Battery();
	_pathFromDocking = deque<Direction>();
	_robotStatus = AlgorithmStatus::ChargingInDocking;
	_previousStatus = AlgorithmStatus::ChargingInDocking;
}

_039563838_A::~_039563838_A(void)
{
}

//************************************
// Brief:	choose a step and do it
// Recieves:	The ACTUAL last step that was taken by the simulation.  May be differnet than what the algorithm recommended
// Returns:   	Direction where it went
//************************************
Direction _039563838_A::step(Direction lastStep)
{
	
	//printDebugHouseMapping();
	if(_debug) {
		PrintLastDirection(lastStep);  //for debug.  delete later!!!
		cout << "_stepsTillFinishing = " << _stepsTillFinishing << endl << endl;  //delete !!!
	}
//	if(_stepsTillFinishing == 1) {  //debugging
//		_debug = true;
//		printDebugHouseMapping();
//	}
//	else if(totalSteps <= 10) { //debugging
//		
//		printDebugHouseMapping();
//	}

	if(_robotStatus != AlgorithmStatus::Returning)  {
		if(lastStep != Direction::Stay && _doNotPush == false)  
			_pathFromDocking.push_back(lastStep); 
	}

	_doNotPush = false;

	updateAlgorithmInfo(lastStep);  //updates lastStep

	UpdateState();

	switch(_robotStatus)
	{
	case AlgorithmStatus::ReturningRapidly:
		return FindShortestPath();
	case AlgorithmStatus::ChargingInDocking:
	case AlgorithmStatus::StayingUntilClean:
	case AlgorithmStatus::StayingUntilHalfClean: 
		 return Direction::Stay;
	case AlgorithmStatus::Returning:
		{
		Direction recommendedStep  = _pathFromDocking.back();
		_pathFromDocking.pop_back();
		 return OppositeDirection(recommendedStep);
		} 
	case AlgorithmStatus::Exploring:
		{
			//Always prefers to visit cells it hasn't been in
			vector<Direction> possible_directions = GetPossibleDirections(lastStep); // get all directions besides of {opposite of lastStep, stay}

			if(_debug)
				PrintPossibleDirections(possible_directions);  //for debug.  delete later !!!

			if(possible_directions.empty()){
				if(_debug)
					cout << "possible_directions is empty !!!" << endl;  //delete !!!!

				//_pathFromDocking.pop_back();          //old ron
				//return OppositeDirection(lastStep);   // old ron
				Direction recommendedStep  = _pathFromDocking.back();  //TODO: handle case where simulation doesn't do what algo suggested.  In that case, the algo needs to check it's not hitting a wall.
				_pathFromDocking.pop_back();
				_doNotPush = true;
		 		return OppositeDirection(recommendedStep);  //TODO: also need to make sure, i don't push_back in next step
		
			}
			else{
				//Direction chosen = possible_directions[0];
				Direction chosen = Handle_Explore_State(possible_directions, lastStep);
				if(_debug) {
					cout << "chosen Direction in Exlporing is:  "; //delete later!!!
					PrintDirection(chosen); //delete later !!!
				}
				//_pathFromDocking.push_back(chosen);  // should push_back the actual direction taken by the simulation and not chosen 
				return chosen;
			}

		} //closes Exploring case
	
	} //closes switch

	if(_debug)
		cout << "i'm not supposed to get here!!!!"  << endl;  //delete later!!!

	return Direction::Stay;
}


//pre: Assumes possibleDirections isn't empty
Direction _039563838_A::Handle_Explore_State(vector<Direction> possibleDirections, Direction lastDirection)
{
	//requestedStep = dir_by_phase[phase];
	int maxDirt = -1;
	int minVisitations = INT_MAX; //in case we already visited all our neighbours,then we want to visit the one we visited the least
	//Direction maxDirtDirection;
	//Direction minVisits;
	vector<int> sameDirt = vector<int>();
	int directionIndex = 4;  //enum class Direction {East, West, South, North, Stay}
	int minDirectionIndex = 4;

	for(Direction dir: possibleDirections) {
		if(dir == lastDirection) {
			GeneralizedPoint newPos1 = position;
			newPos1.move(dir);
			auto neighbour1 = houseMapping.find(newPos1);
			if(  !(neighbour1 != houseMapping.end() && neighbour1->second.dirt != -1) ) {
				return dir;
			}
		}

	}

	

	for(Direction d: possibleDirections) {
		GeneralizedPoint newPos = position;
		newPos.move(d);
		auto neighbour = houseMapping.find(newPos);
		//if((neighbour != houseMapping.end() && neighbour->second.dirt != -1) || s.isWall[(int)requestedStep]) { 
		//all directions in possibleDirections aren't walls
		if((neighbour != houseMapping.end() && neighbour->second.dirt != -1) ) { 
			// if we already have info on this cell dirt level, skip
			if(neighbour->second.dirt >= maxDirt) {
				maxDirt = neighbour->second.dirt;

				//possibleDirections.push_back(Direction(i));
				directionIndex = ReturnDirectionIndex(d);
				//maxDirtDirection = d;  //TODO change
				sameDirt.push_back(directionIndex);  //only relevant when all neighbours have dirt level of 0
				if(neighbour->second.numOfVisitations <= minVisitations) {
					minVisitations = neighbour->second.numOfVisitations;
					//minVisits = d;  //TODO: change.  only relevant when all neighbours have dirt level of 0
					minDirectionIndex = ReturnDirectionIndex(d);

				}
			}	
		}
		else {	// we don't have info on this cell and it's not a wall, so let's visit it
			return d;
		}
	} // closes for loop

	//to avoid loop:  case where all directions have dirt level 0. Therefore we do not want to choose the same direction each time:
	if(maxDirt == 0){
		if(sameDirt.size() >=2 ) { //if we already visited all our neighbours and they all have dirt =0, then we choose the neighbour which we visited the minimum number of times 
			//cout << "maxDirt ==0 , and minVisits direction is : ";   //delete later
			//PrintDirection(Direction(minDirectionIndex));                    //delelte later !!!
			
			return Direction(minDirectionIndex);  //old algoX
			//new technique is to choose the direction with the most dirt in the overall houseMapping:
//			int maximalDirt = -1;      int temp = 0;  int dIndex = 0;  int naiveDistance = 0;  int min_d_Closest = 0;
//			int currentBatteryLevel = _battery.GetBattery_level();
//			int batteryHalfLife = (int)( _battery.GetBattery_capacity() / 2);
//			//if(_battery.GetBattery_level() <= (  (int)( _battery.GetBattery_capacity() / 2) + 1  )  )
//			for(Direction d: possibleDirections) { 
//				temp = calcDirt(d);
//				if( temp > maximalDirt){
//					//naiveDistance = Find_Closest_Dirty_Point(d);
//					//cout <<"naiveDistance = " << naiveDistance << endl;  //delete !!!!!
//					//if( (currentBatteryLevel + naiveDistance * _battery.GetConsumption_rate()  ) <= batteryHalfLife ) {
//						maximalDirt = temp;
//						dIndex = ReturnDirectionIndex(d);
//					//}
//				}
//			}
//			//if(maximalDirt <= 0 || _chooseMinimalVisitations) {
//			if(maximalDirt <= 0 ) {
//				//_chooseMinimalVisitations = false;  //next time choose according to maximalDirt
//				return Direction(minDirectionIndex);  //cell we visited the least number of times
//			}
//			else {
//				//_chooseMinimalVisitations = true;  //next time choose according to minDirectionIndex
//				return Direction(dIndex);
//			}
		}
		

	}

	//if we got here then we didn't choose any direction since we already visited all directions in possibleDirections. Therefore we'll choose the direction with the highest dirt level:
	return Direction(directionIndex);
}



//************************************
// Brief:		change the algorithm state, consume/recharge battery, and clean current location by 1 if not cleaned
// Returns:   	void
void _039563838_A::UpdateState()
{
  
 	//SensorInformation info = _robotSensor->sense();  //dust in sensor is still not updated
 	//_dirtInCurrentLocation = info.dirtLevel;
	auto pointMe = houseMapping.find(position);  //current position is in houseMapping because we called updateAlgorithmInfo earlier from step function
	int dlevel  = (pointMe -> second.dirt);  
				
	//if(_debug)
		//PrintAlgorithmStatus();  //for debug.  delete later!!!!

	_previousStatus = _robotStatus;
	switch(_robotStatus)
	{


	case AlgorithmStatus::ChargingInDocking:
		{

			if( _stepsTillFinishing == 1 || _stepsTillFinishing == 0) {
				_robotStatus = AlgorithmStatus::ChargingInDocking;
			}
			else if(_battery.OneRechargeBeforeFullyRecharged()) {
				
				_robotStatus = AlgorithmStatus::Exploring;

			}
			_battery.Recharge();
		}
		break;
	
	case AlgorithmStatus::StayingUntilHalfClean:
		{
			_battery.Consume();
			if(AboutToFinishWasCalled == true) {
				int shortestDistanceFromDocking = houseMapping[position].stepsToDocking;
				if(shortestDistanceFromDocking >=  (_stepsTillFinishing) -1 )  {
					if(_stepsTillFinishing <  _explore_steps ) {
						_robotStatus = AlgorithmStatus::ReturningRapidly;
						return;
					}
					else {
						_robotStatus = AlgorithmStatus::Returning;
						return;
					}
				}
			}
			if(_battery.GetBattery_level() <= (  (int)( _battery.GetBattery_capacity() / 2) + 1  )  )  {
				_robotStatus = AlgorithmStatus::Returning;
				return;
			}
			else if(dlevel > 0) {
				if(_dirtInSpot % 2 == 0) {
					if(_cleanedDust >= _dirtInSpot / 2)  { //already cleaned half
						_robotStatus = AlgorithmStatus::Exploring;
						return;
					}
				}
				else { 
					if(_cleanedDust >= ( ((int)(_dirtInSpot /2)) + 1) )  { //already cleaned half
						_robotStatus = AlgorithmStatus::Exploring;
						return;
					}
				}
			}
			else if(dlevel <= 0)
		 		_robotStatus = AlgorithmStatus::Exploring;
		}
	case AlgorithmStatus::Exploring:
		{
			_battery.Consume();
			if(AboutToFinishWasCalled == true) {
				int shortestDistanceFromDocking = houseMapping[position].stepsToDocking;
				if(shortestDistanceFromDocking >=  (_stepsTillFinishing) -1 )  {  //this algo prefers to return in the same path it travelled from docking to current position. However, if it doesn't have enough steps left, it returns via the shortest route to docking
					if(_stepsTillFinishing <  _explore_steps ) {  //TODO: change so it will consider battery, because in Return we clean half!!!!!!!!!!!    must return via shortest route to docking
						_robotStatus = AlgorithmStatus::ReturningRapidly;
						return;
					}
					else {
						_robotStatus = AlgorithmStatus::Returning;
						return;
					}

				}
			}

			if(_battery.GetBattery_level() <= (  (int)( _battery.GetBattery_capacity() / 2) + 1  )  )  {
				_robotStatus = AlgorithmStatus::Returning;
				//cout << endl << "starting to return when battery level = " << _battery.GetBattery_level() << "   and battery capacity = " <<  _battery.GetBattery_capacity() <<  endl;  //delete !!!!!!!!!!

				//cout << "current position when starting to return : ( " << position.getX() << ", " << position.getY() << ")" << endl; //delete!!!
				//auto p = houseMapping.find(position);   //delete !!!!!!!!!!!!!!!!!!!!!!!!
				//cout << "stepsToDocking when starting to return = " <<  p->second.stepsToDocking  << endl; 	//delete !!!!!
				return; 
			}
			else if(dlevel > 0)
				_robotStatus = AlgorithmStatus::StayingUntilHalfClean;
			else 
				_robotStatus = AlgorithmStatus::Exploring;	
		}
		break;
	case AlgorithmStatus::Returning:
		{
			_battery.Consume();
			if(IsInDocking()) {
				_robotStatus = AlgorithmStatus::ChargingInDocking;
				return;
			}

			if(AboutToFinishWasCalled == true) {
				if(_stepsTillFinishing <  _explore_steps ) { 
					_robotStatus = AlgorithmStatus::ReturningRapidly;
					return;
				}
			}
			if(dlevel > 0)
				_robotStatus = AlgorithmStatus::StayingUntilClean;
		}
		break;
	case AlgorithmStatus::StayingUntilClean:  //algorithm is returning to docking
		{
			_battery.Consume();
			if(AboutToFinishWasCalled == true) {
					if(_stepsTillFinishing <  _explore_steps ) {
						_robotStatus = AlgorithmStatus::ReturningRapidly;
							return;
					}		
			}
			if(dlevel > 0)
				_robotStatus = AlgorithmStatus::StayingUntilClean;   //keep cleaning
			else //dlevel == 0
				_robotStatus = AlgorithmStatus::Returning;
		}
		break;
	case AlgorithmStatus::ReturningRapidly:	
		{
			//cout << "status is ReturningRapidly " << endl;   // delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			_battery.Consume();
			if(IsInDocking())
				_robotStatus = AlgorithmStatus::ChargingInDocking;
		}
		break;
	}

}


void _039563838_A::aboutToFinish(int stepsTillFinishing)
{
	AboutToFinishWasCalled = true;
	_stepsTillFinishing = stepsTillFinishing;
	
	//cout << "aboutToFinish was called !!" << endl;  // delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//cout << "current position : ( " << position.getX() << ", " << position.getY() << ")" << endl; //delete!!!
	
	//auto p = houseMapping.find(position);   //delete !!!!!!!!!!!!!!!!!!!!!!!!
	//cout << "stepsToDocking = " <<  p->second.stepsToDocking  << endl; 	//delete !!!!! 
	//_debug = true;  //delete!!!
	//cout << "batteryLevel = " <<  _battery.GetBattery_level() << endl;	//delete !!!!! 
} 


// Brief: returns opposite direction
Direction _039563838_A::OppositeDirection(Direction d)
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
vector<Direction> _039563838_A::GetPossibleDirections(Direction lastStep) const
{
	Direction back = OppositeDirection(lastStep);  //in the first step of the algorithm lastStep = Direction::Stay
	SensorInformation info = _robotSensor->sense();
	vector<Direction> possibleDirections =  vector<Direction>();
	for(int i=0;i<4;++i){
		if( ! info.isWall[i] && Direction(i) != back)
			possibleDirections.push_back(Direction(i));
	}
	return possibleDirections;
}


void _039563838_A::updateAlgorithmInfo(Direction lastStep) 
{
	//1st: lastStep = stay. curr = D, pos = 00. return to step. step chooses east. step notifies DoStep. 
	//2nd: in DoStep: curr is still D. doesn't clean D. then move east to (1,0). Then call step()
	//step calls update(East). update moves pos to (1,0). Then senses. it shoud sense with decrement! Then it returns to
	//step. step know pos is at (1,0) with the correct d.level.  GOOD.

	if(AboutToFinishWasCalled == true)
		--_stepsTillFinishing;
	++totalSteps;
	if(_previousStatus == AlgorithmStatus::Exploring || _previousStatus == AlgorithmStatus::StayingUntilHalfClean)
		++_explore_steps;  //becomes 0 when switching to Return, ReturningRapidly, Recharge states
	else if(_previousStatus == AlgorithmStatus::Returning || _previousStatus == AlgorithmStatus::StayingUntilClean)
		--_explore_steps;
	else if(_previousStatus == AlgorithmStatus::ChargingInDocking)
		_explore_steps = 0;

	position.move(lastStep);
	SensorInformation s =  _robotSensor ->sense();
	//if(totalSteps <=10) 
		//cout << "sensor information: s.dirtLevel == " << s.dirtLevel << endl;  //delete !!!!!!!!!!!!!!!!!
  
	

	if(_debug) {	
		cout << "position after move is: 	( " << position.getX() << ", " << position.getY() << ")" << endl; //delete!!!
		
	}
	
	// update the map with the info on the current position
//    houseMapping[position] = {(s.dirtLevel != 0)? s.dirtLevel - 1 : 0, stepsFromDocking};

	stepsFromDocking = calcStepsToDocking(stepsFromDocking + 1, position);

	int visits = 0;    int dlevel = 0;
	if(s.dirtLevel > 0)
		dlevel = s.dirtLevel - 1; 
	//dlevel = s.dirtLevel;
	
	auto pointMe = houseMapping.find(position);
		if(pointMe != houseMapping.end()) {
			visits = (pointMe -> second.numOfVisitations) + 1;  //upadate the number of times we visited the cell
			houseMapping[position] = {dlevel, stepsFromDocking, visits};
		}
		else {
			CellInfo cellInfo;
			cellInfo.numOfVisitations = 0;
			cellInfo.dirt = dlevel;
			cellInfo.stepsToDocking = stepsFromDocking;
			houseMapping[position] = cellInfo;
		}

	if(_robotStatus == AlgorithmStatus::StayingUntilHalfClean && _previousStatus != AlgorithmStatus::StayingUntilHalfClean){
		_dirtInSpot = dlevel;
		_cleanedDust = 0;
	}
	else if( _robotStatus ==  AlgorithmStatus::StayingUntilHalfClean && _previousStatus == AlgorithmStatus::StayingUntilHalfClean)
		++_cleanedDust;

	//cellInfo.isWall = false;
	
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
		//if(_debug)
			//printDebugHouseMapping();

//		if(position.getY() == -10 && position.getX() == 1) {  //delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//			cout << "i'm in point (1,-10) : dirt = " << dlevel << endl;
//			cout << "totalSteps = " << totalSteps << endl;
//			cout << "_robotStatus = " << endl;
//			PrintAlgorithmStatus();
//			cout <<  "battery level = " << _battery.GetBattery_level() << endl;
//		}
		//printDebugHouseMapping();  //delete !!!!!!!!!!!!!!!!!!!!!!	
}


//Iterates over all directions and returns the direction with minimal stepsToDocking
Direction _039563838_A::FindShortestPath()  //TODO:  debug this function with simulations
{
	int min = 0;   int i = 0;
	Direction ret = Direction::Stay;
	for(Direction d: directions) {
            GeneralizedPoint pointToCheck = position;
            pointToCheck.move(d);
            auto neighbourInfo = houseMapping.find(pointToCheck);
            if(neighbourInfo != houseMapping.end() && neighbourInfo->second.stepsToDocking != -1 && (neighbourInfo->second.isWall == false) ) {
				++i;
				if(i == 1) {  //just in order to give min it's initial value at the first iteration
				min = neighbourInfo->second.stepsToDocking;
				ret = d;
				}

            	if(neighbourInfo->second.stepsToDocking < min )  {
					min = neighbourInfo->second.stepsToDocking;
					ret = d;
				}
           }
        }

//	if(_debug) {
//		cout << "in FindShortestPath and chosen direction = " ;
//		PrintDirection(ret); // delete !!!!
//	}

	return ret;
}


int _039563838_A::calcStepsToDocking(int stepsFromDocking, const GeneralizedPoint& position) 
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


void _039563838_A::updateStepsToDocking(int stepsToDocking, const GeneralizedPoint& position) 
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
void _039563838_A::printDebugHouseMapping() 
{

	if(_debug) {
	
            cout << "---------------------------" << endl;
            //cout << "totalSteps: " << totalSteps << endl;
            cout << "---------------------------" << endl;
            for(auto& a : houseMapping) {
		//if(a.second.dirt > 0) {
                	// debug print of all the points mapped so far
                	cout << "( " << a.first.getX() << " , " << a.first.getY() << ") : dirt = " << a.second.dirt 
                        << ", stepsToDocking = " << a.second.stepsToDocking 
                        << ", isWall = " << (a.second.isWall? "true":"false") << endl;
		//}
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


//for debug
void _039563838_A::PrintPossibleDirections(vector<Direction> possible_directions)
{
	for(Direction d: possible_directions) {
		PrintPossibleDirection(d);
	}
}

//for debug
void _039563838_A::PrintLastDirection(Direction lastStep)
{
	switch (lastStep)
	{
	case Direction::North:
		cout << "lastStep = North" << endl;
		return;
	case Direction::South:
		cout << "lastStep = South" << endl;
		return;
	case Direction::East:
		cout << "lastStep = East" << endl;
		return;
	case Direction::West:
		cout << "lastStep = West" << endl;
		return;
	default:
		cout << "lastStep = Stay" << endl;
	}
}

//for debug
void _039563838_A::PrintPossibleDirection(Direction possibleDirection)
{
	switch (possibleDirection)
	{
	case Direction::North:
		cout << "possibleDirection = North" << endl;
		return;
	case Direction::South:
		cout << "possibleDirection = South" << endl;
		return;
	case Direction::East:
		cout << "possibleDirection = East" << endl;
		return;
	case Direction::West:
		cout << "possibleDirection = West" << endl;
		return;
	default:
		cout << "possibleDirection = Stay" << endl;
	}
}


//for debug
void _039563838_A::PrintDirection(Direction chosen)
{
	switch (chosen)
	{
	case Direction::North:
		cout << "North" << endl;
		return;
	case Direction::South:
		cout << "South" << endl;
		return;
	case Direction::East:
		cout << "East" << endl;
		return;
	case Direction::West:
		cout << "West" << endl;
		return;
	case Direction::Stay:
		cout << "Stay" << endl;
		return;
	default:
		cout << "unspecified" << endl;
	}
}

//for debug
void _039563838_A::PrintAlgorithmStatus()
{
	//AlgorithmStatus status = _robotStatus;
	switch (_robotStatus)
	{
	case AlgorithmStatus::ChargingInDocking:
		cout << "status = ChargingInDocking" << endl;
		return;
	case AlgorithmStatus::Exploring:
		cout << "status = Exploring" << endl;
		return;
	case AlgorithmStatus::Returning:
		cout << "status = Returning" << endl;
		return;
	case AlgorithmStatus::ReturningRapidly:
		cout << "status = ReturningRapidly" << endl;
		return;
	case AlgorithmStatus::StayingUntilHalfClean:
		cout << "status = StayingUntilHalfClean" << endl;
		return;
	case AlgorithmStatus::StayingUntilClean:
		cout << "status = StayingUntilClean" << endl;
		return;
	default:
		cout << "status =  unspecified" << endl;
	}
}


int _039563838_A::ReturnDirectionIndex(Direction d)
{
	switch (d)                  //enum class Direction {East, West, South, North, Stay};
	{
	case Direction::North:
		return 3;
	case Direction::South:
		return 2;
	case Direction::East:
		return 0;
	case Direction::West:
		return 1;
	case Direction::Stay:
		return 4;
	default:
		cout << "in ReturnDirectionIndex in default = 4" << endl;  //delete !!!!!!!!!!!!!!!!
		return 4;  //notice default is Stay
	}
}


bool _039563838_A::IsInDocking() const
{
//	bool wow = (position.GeneralizedPoint::getX() == 0) && (position.GeneralizedPoint::getY() == 0);
//	if(wow)
//		cout << "in docking and _stepsTillFinishing = " << _stepsTillFinishing << endl;  //delete !!!!!!!
	return ( (position.GeneralizedPoint::getX() == 0) && (position.GeneralizedPoint::getY() == 0) );
}


//returns the most eastern index in houseMapping known so far
int _039563838_A::GetEastMost()
{
	int mostEast = 0;  //docking is at (0,0)
	//int yValue = 0;
	for(auto& a : houseMapping) {
		if(a.first.getX() > mostEast) {
			mostEast = a.first.getX();
			//yValue = a.first.getY();
		}
	}
	//return GeneralizedPoint(mostEast, yValue);
	return mostEast;
}

//returns the most eastern index in houseMapping known so far
int _039563838_A::GetWestMost()
{
	int mostWest = 0;  //docking is at (0,0)
	//int yValue = 0;
	for(auto& a : houseMapping) {
		if(a.first.getX() < mostWest) {
			mostWest = a.first.getX();
			//yValue = a.first.getY();
		}
	}
	//return GeneralizedPoint(mostEast, yValue);
	return mostWest;
}

int _039563838_A::GetNorthMost()
{
	int mostNorth = 0;  //docking is at (0,0)
	//int yValue = 0;
	for(auto& a : houseMapping) {
		if(a.first.getY() > mostNorth) {
			mostNorth = a.first.getY();
			//yValue = a.first.getY();
		}
	}
	//return GeneralizedPoint(mostNorth, yValue);
	return mostNorth;
}

int _039563838_A::GetSouthMost()
{
	int mostSouth = 0;  //docking is at (0,0)
	//int yValue = 0;
	for(auto& a : houseMapping) {
		if(a.first.getY() < mostSouth) {
			mostSouth = a.first.getY();
			//yValue = a.first.getY();
		}
	}
	//return GeneralizedPoint(mostNorth, yValue);
	return mostSouth;
}


int _039563838_A::calcDirt(Direction d)
{
	
	switch (d)
	{
	case Direction::North:
		return SumDustNorthOfMe();
	case Direction::South:
		return SumDustSouthOfMe();
	case Direction::East:
		return SumDustEastOfMe();
	case Direction::West:
		return SumDustWestOfMe();
	case Direction::Stay:
		return 0; //shoulden't get here
	default:
		//cout << "in ReturnDirectionIndex in default = 4" << endl;  //delete !!!!!!!!!!!!!!!!
		return 0;  //shouldn't get here
	}
}

//sum all the known at this moment dust in houseMapping east of current position 
int _039563838_A::SumDustEastOfMe()
{
	int currentX = position.getX();
	int sum = 0;
	int d = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentX < a.first.getX() )
			sum = sum + d;
	}
	return sum;
}

int _039563838_A::SumDustWestOfMe()
{
	int currentX = position.getX();
	int sum = 0;
	int d = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentX > a.first.getX() )
			sum = sum + d;
	}
	return sum;
}

int _039563838_A::SumDustNorthOfMe()
{
	int currentY = position.getY();
	int sum = 0;
	int d = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentY < a.first.getY() )
			sum = sum + d;
	}
	return sum;
}

int _039563838_A::SumDustSouthOfMe()
{
	int currentY = position.getY();
	int sum = 0;
	int d = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentY > a.first.getY() )
			sum = sum + d;
	}
	return sum;
}

int _039563838_A::Find_Closest_Dirty_Point(Direction d)
{
	switch (d)
	{
	case Direction::North:
		return Find_Closest_Dirty_Point_North_of_Me();
	case Direction::South:
		return Find_Closest_Dirty_Point_South_of_Me();
	case Direction::East:
		return Find_Closest_Dirty_Point_East_of_Me();
	case Direction::West:
		return Find_Closest_Dirty_Point_West_of_Me();
	case Direction::Stay:
		return 0; //shoulden't get here
	default:
		//cout << "in ReturnDirectionIndex in default = 4" << endl;  //delete !!!!!!!!!!!!!!!!
		return 0;  //shouldn't get here
	}
}

//vector<int> _039563838_A::Find_Naive_Distance_to_Closest_Dirty_Point (Direction d)  
int _039563838_A::Find_Closest_Dirty_Point_East_of_Me()
{
	int currentX = position.getX();
	int currentY = position.getY();
	int d = 0;   int maxDistance = 0;  int temp = 0;   //int maxX = 0;   int maxY = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentX < a.first.getX() )
			temp = abs(a.first.getX() - currentX) + abs(a.first.getY() - currentY);
			if(maxDistance <= temp) {
				maxDistance = temp;
				//maxX = a.first.getX();
				//maxY = a.first.getY();
			}
	}
	//std::vector<int> myvector;
	//myvector.push_back(maxX);	myvector.push_back(maxY);	myvector.push_back(maxDistance);
	//return myvector;
	return maxDistance;
}

int _039563838_A::Find_Closest_Dirty_Point_West_of_Me()
{
	int currentX = position.getX();
	int currentY = position.getY();
	int d = 0;   int maxDistance = 0;  int temp = 0;   //int maxX = 0;   int maxY = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentX > a.first.getX() )
			temp = abs(a.first.getX() - currentX) + abs(a.first.getY() - currentY);
			if(maxDistance <= temp) {
				maxDistance = temp;
				//maxX = a.first.getX();
				//maxY = a.first.getY();
			}
	}
	//std::vector<int> myvector;
	//myvector.push_back(maxX);	myvector.push_back(maxY);	myvector.push_back(maxDistance);
	//return myvector;
	return maxDistance;
}

int _039563838_A::Find_Closest_Dirty_Point_North_of_Me()
{
	int currentX = position.getX();
	int currentY = position.getY();
	int d = 0;   int maxDistance = 0;  int temp = 0;   //int maxX = 0;   int maxY = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentY < a.first.getY() )
			temp = abs(a.first.getX() - currentX) + abs(a.first.getY() - currentY);
			if(maxDistance <= temp) {
				maxDistance = temp;
				//maxX = a.first.getX();
				//maxY = a.first.getY();
			}
	}
	//std::vector<int> myvector;
	//myvector.push_back(maxX);	myvector.push_back(maxY);	myvector.push_back(maxDistance);
	//return myvector;
	return maxDistance;
}

int _039563838_A::Find_Closest_Dirty_Point_South_of_Me()
{
	int currentX = position.getX();
	int currentY = position.getY();
	int d = 0;   int maxDistance = 0;  int temp = 0;   //int maxX = 0;   int maxY = 0;
	for(auto& a : houseMapping) {
		d = a.second.dirt;
		if(d > 0 && currentY > a.first.getY() )
			temp = abs(a.first.getX() - currentX) + abs(a.first.getY() - currentY);
			if(maxDistance <= temp) {
				maxDistance = temp;
				//maxX = a.first.getX();
				//maxY = a.first.getY();
			}
	}
	//std::vector<int> myvector;
	//myvector.push_back(maxX);	myvector.push_back(maxY);	myvector.push_back(maxDistance);
	//return myvector;
	return maxDistance;
}

REGISTER_ALGORITHM (_039563838_A)


// extern "C" {
// AbstractAlgorithm *maker(){
//    return new _039563838_A;
// }
// class proxy { 
// public:
//    proxy(){
//       // register the maker with the factory using file name 
//       factory["039563838_A_"] = maker;
//    }
// };
// // our one instance of the proxy
// proxy p;
// }

