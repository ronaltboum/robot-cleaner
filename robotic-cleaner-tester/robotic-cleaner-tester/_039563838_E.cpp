//#include "stdafx.h"
#include "_039563838_E.h"
#include "AlgorithmRegistration.h"
#include <iostream>

using namespace std;

_039563838_E::_039563838_E(const AbstractSensor& sensor, map<string, int> config)
{
	initiallize();
	setSensor(sensor);
	setConfiguration(config);

	int _maxSteps =  _battery.GetOneWayDistanceFromDocking();
	cout << 0;
	if(_debug) cout << "one way: "  << _maxSteps << endl;
}

_039563838_E::_039563838_E(void)
{
	initiallize();
}

//function called in each c'tor
void _039563838_E::initiallize()
{
	position = {0,0};  //as far as the algorithm is concerned, the docking point is at (0,0)
	_configs = map<string,int>();
	_battery = Battery();
	_robotStatus = AlgorithmStatus::FirstStep;
	CellInfo startingPointInfo;
	startingPointInfo.dirt = 0;
	startingPointInfo.stepsToDocking = 0;
	startingPointInfo.isWall = false;
	_cleaningPathCache = { vector<GeneralizedPoint>(), 0, 0};

	houseMapping[position] = startingPointInfo;
	totalSteps = 0;
	_stepsTillFinishing = -8;  //_stepsTillFinishing == MaxStepsAfterWinner when aboutToFinish is called by the simulation
	_aboutToFinishWasCalled = false;  //equals true if aboutToFinish was called by the simulation
	_allExplored = false;
	_allCleaned = false;
}

_039563838_E::~_039563838_E(void)
{
}

//************************************
// Brief:	choose a step and do it
// Recieves:	The ACTUAL last step that was taken by the simulation.  May be differnet than what the algorithm recommended
// Returns:   	Direction where it went
//************************************
Direction _039563838_E::step(Direction lastStep)
{
	if(_debug) {
		cout << "step: call number #" << totalSteps;
		if(_aboutToFinishWasCalled) cout << "_stepsTillFinishing = " << _stepsTillFinishing << endl;  //delete !!!
	}
	UpdateAlgorithmInfo(lastStep);  //updates lastStep
	AlgorithmStatus previous = _robotStatus;
	UpdateState();
	if(previous == AlgorithmStatus::Cleaning && _robotStatus != previous) // reseting cache
	{
		_cleaningPathCache = { vector<GeneralizedPoint>(),0, 0};
	}
	return ChooseStepAccordingToState(lastStep);
}

Direction _039563838_E::ChooseStepAccordingToState(Direction lastStep)
{
	Direction d;
	switch(_robotStatus)
	{
	case AlgorithmStatus::Done:
	case AlgorithmStatus::ChargingInDocking:
		if(_debug) cout << "charging" << endl;
		 return Direction::Stay;
	case AlgorithmStatus::Cleaning:
		return Handle_Cleaning_State(lastStep);
	case AlgorithmStatus::Returning:
		return Handle_Returning_State();
	case AlgorithmStatus::Exploring:
		d = Handle_Explore_State();
		return d;
	case AlgorithmStatus::FirstStep:
		return Direction::Stay; //shouldn't get here
	} //closes switch

	if(_debug) cout << "i'm not supposed to get here!!!!"  << endl;  //delete later!!!

	return Direction::Stay;
	
}

Direction _039563838_E::Handle_Returning_State(){
	deque<vector<GeneralizedPoint>> SPToDocking = GetSPToDocking();
	GeneralizedPoint newDestination = SPToDocking.front()[1];
	return position.GetDirection(newDestination);
}

int _039563838_E::GetRemainingSteps()
{
	if(_aboutToFinishWasCalled)
		return min(_stepsTillFinishing, _battery.GetStepsBeforeRecharge(IsInDocking()));
	else
		return _battery.GetStepsBeforeRecharge(IsInDocking());
}

Direction _039563838_E::Handle_Cleaning_State(Direction lastStep)
{
	int remainingSteps = GetRemainingSteps();
	if(_debug) cout << " _039563838_E::Handle_Cleaning_State\tremainingSteps: " << remainingSteps << endl;
	if(! _cleaningPathCache.WasCorrectStepCommited(lastStep, remainingSteps))
	{
		DynamicPathFinder pf = DynamicPathFinder(position, houseMapping, remainingSteps);
		pf.RunIteration();
		_cleaningPathCache = {pf.GetBestPathTo({0,0}), 0 , (size_t)remainingSteps};
		if(_cleaningPathCache.savedPath.size() == 0){	
			if(_debug) cout << "about to finish was called and Algo E couldn't make it to docking" << endl;
			//about to finish was called and we can't go back to docking - choose any best path.
			_cleaningPathCache.savedPath = pf.GetBestPathToAny();
		}
		// _cleaningPathCache.index = 0;
		// _cleaningPathCache.steps_remaining = remainingSteps;
		if(_debug){
			cout << endl << endl << _battery << endl;
			cout  << endl << "remainingSteps : " << remainingSteps <<" cleaning path: ";
			for(auto & p: _cleaningPathCache.savedPath)
				cout << p;
			cout << endl << "path len: " << _cleaningPathCache.savedPath.size() << endl;
		}
	}

	return _cleaningPathCache.GetDirection();
}

// check if all unexplored points are unreachable. if so updates _allExplored and houseMapping
// returns true if any unexplored point is reachable
bool _039563838_E::CheckUnexploredPointsReachability()
{
	if( GetSPToUnexplored(true).front().size() <= (size_t)_battery.GetOneWayDistanceFromDocking()) // some are still reachable
		return true;
	// all are unreachable
	for ( auto & pointInfoPair : houseMapping) 
    {
    	if(pointInfoPair.second.isUnexplored())
    	{
    		pointInfoPair.second = {0, -1, true, vector<Direction>(), vector<Direction>()}; // set it as a wall
    	}
    }
    _allExplored = true;
    _allCleaned = CheckAllCleaned();
    return false;
}

Direction _039563838_E::Handle_Explore_State()
{
	cout << "Handle_Explore_State" << endl;
	deque<vector<GeneralizedPoint>> SPToUnexplored = GetSPToUnexplored();
	cout << "GetSPToUnexplored size" << SPToUnexplored.size() << endl;
	size_t remainingSteps = GetRemainingSteps();

	if(SPToUnexplored.front().size() > remainingSteps){
		if(CheckUnexploredPointsReachability()){ // exploring not finished, but can't explore without returning
			_robotStatus = AlgorithmStatus::Returning;
		}
		else{
			//going back to update state and going from there.
			_cleaningPathCache = { vector<GeneralizedPoint>(),0, 0};
			UpdateState();
			return ChooseStepAccordingToState(Direction::Stay); 
		}
	}
	vector<Direction> possibleDirections;
	for(const auto & path : SPToUnexplored)
	{
		Direction d = position.GetDirection(path[1]); // direction from current position
		if( find(possibleDirections.begin(), possibleDirections.end(), d) == possibleDirections.end())
			possibleDirections.push_back(d);

	}
	// prefered directions by order - algorithm always prefer to go sideways and then up/down.
	// this help getting lawnmower effect in exploring
	vector<Direction> preferedDirections = {Direction::West, Direction::East, Direction::North, Direction::South};
	for(const auto & dir : preferedDirections)
	{
		if(find(possibleDirections.begin(), possibleDirections.end(), dir) != possibleDirections.end())
			return dir;
	}
	return Direction::Stay; //shouldn't get here
}

// returns a deque of the shortests paths from position to docking
deque<vector<GeneralizedPoint>> _039563838_E::GetSPToDocking() const
{
	deque<vector<GeneralizedPoint>> shortestPaths;
	vector<GeneralizedPoint> v;
	v.push_back(position);
	shortestPaths.push_back(v);
	int steps = houseMapping.at(position).stepsToDocking;
	//int steps = houseMapping.find(position)->second.stepsToDocking;
    for(int stepsIndex = 0; stepsIndex< steps; ++stepsIndex)
    {
    	int pathsNum = shortestPaths.size();
    	for (int pathIndex = 0; pathIndex< pathsNum; ++pathIndex) 
	    {
	    	auto path = shortestPaths.front();
	    	shortestPaths.pop_front();
	    	GeneralizedPoint lastPointInPath = path.back();

	    	const vector<Direction> & parentsDirections = houseMapping.at(lastPointInPath).parents;
	    	size_t parentIndex = 0;
	    	for(; parentIndex< parentsDirections.size() - 1; ++parentIndex)
	    	{
	    		vector<GeneralizedPoint> newSP = vector<GeneralizedPoint>(path);
				GeneralizedPoint lastPointParent = lastPointInPath;
				lastPointParent.move(parentsDirections[parentIndex]);
				newSP.push_back(lastPointParent);
				shortestPaths.push_back(newSP);
	    	}

			GeneralizedPoint lastPointParent2 = lastPointInPath;
			lastPointParent2.move(parentsDirections[parentIndex]);
			path.push_back(lastPointParent2);
			shortestPaths.push_back(path);
	    }
    }

    return shortestPaths;
}

// returns a deque of the shortests paths from position to unexplored points
// if fromDocking - then from docking. otherwise from current position
deque<vector<GeneralizedPoint>> _039563838_E::GetSPToUnexplored(bool fromDocking /* = false */)
{
	GeneralizedPoint p = fromDocking ? GeneralizedPoint(0,0) : position;
	if(_debug) cout << "GetSPToUnexplored on " << p << " info: " << houseMapping[p];
	map<GeneralizedPoint, int> distanceFromP;
	for ( const auto &pointInfoPair : houseMapping) 
    {
    	distanceFromP[pointInfoPair.first] = -1; 
    }
    map<GeneralizedPoint , vector<GeneralizedPoint>> parents;
	vector<GeneralizedPoint> Q1 = vector<GeneralizedPoint>();;
	vector<GeneralizedPoint> Q2 = vector<GeneralizedPoint>();;
	distanceFromP[p] = 0; 
    parents[p] = vector<GeneralizedPoint>();
	parents[p].push_back(p);
	int currDistanceFromP = 0;
	bool currentQueueIs1 = true, foundUnexplored = false;
	vector<GeneralizedPoint> & Q = currentQueueIs1 ? Q1 : Q2;
	vector<GeneralizedPoint> & OtherQ = currentQueueIs1 ? Q2 : Q1;

	Q.push_back(p);
	int iterationNum = 1; // for debug TODO: delete
	while(! foundUnexplored && iterationNum < 8){
		while( !Q.empty() ){
			GeneralizedPoint point = Q.back();
			Q.pop_back();
			if(_debug) cout << "queue run on " << point << " directions num: " << houseMapping.at(point).possibleKnownDirections.size();

			for ( const auto &dir : houseMapping.at(point).possibleKnownDirections) 
		    {
		    	GeneralizedPoint neighboor = point;
				neighboor.move(dir);
				int neighboorDistance = distanceFromP[neighboor];
				bool addToQueue = (neighboorDistance == -1);
				if(_debug) cout << "point at " << dir << ": " << neighboor << " distance: " << neighboorDistance << (addToQueue ? " added" : "") << endl;
				if( addToQueue){
					distanceFromP[neighboor] = currDistanceFromP + 1;
					OtherQ.push_back(neighboor);
					parents[neighboor].push_back(point);
				}
				else if(neighboorDistance == currDistanceFromP + 1){
					parents[neighboor].push_back(point);
				}
				if(houseMapping.at(neighboor).isUnexplored()){
					foundUnexplored = true;
				}
		    }
		}
		if(_debug) cout << "q size: " << Q.size() << " other q size: " << OtherQ.size() << endl;
		// switch q's
		if(_debug) ++iterationNum;
		if(_debug) iterationNum = iterationNum;
		currentQueueIs1 = (! currentQueueIs1);
		std::swap(Q, OtherQ);
		//Q = currentQueueIs1 ? Q1 : Q2;
		//OtherQ = currentQueueIs1 ? Q2 : Q1;
		OtherQ = vector<GeneralizedPoint>();
		++currDistanceFromP;
	}
	_debug = false;
	//building the vectors of the shortest paths from p to unexplored points
	//removing from Q points which are explored
	Q.erase(
		std::remove_if (Q.begin(), Q.end(), 
			[=](GeneralizedPoint p) -> bool{ return (! houseMapping.at(p).isUnexplored()); }), 
		Q.end()
		);

	deque<vector<GeneralizedPoint>> shortestPathsToUnexplored;
	for ( const auto &unexploredPoint : Q) 
    {
    	vector<GeneralizedPoint> v;
    	v.push_back(unexploredPoint);
    	shortestPathsToUnexplored.push_back(v);
    }
    for(int stepsIndex = 0; stepsIndex< currDistanceFromP; ++stepsIndex)
    {
    	int pathsNum = shortestPathsToUnexplored.size();
    	for (int pathIndex = 0; pathIndex< pathsNum; ++pathIndex) 
	    {
	    	auto path = shortestPathsToUnexplored.front();
	    	shortestPathsToUnexplored.pop_front();
	    	GeneralizedPoint lastPointInPath = path.back();
	    	vector<GeneralizedPoint> pointParents = parents[lastPointInPath];
	    	size_t parentsIndex = 0;
	    	for(; parentsIndex <  pointParents.size() - 1; ++parentsIndex){
	    		// case more than one parent
	    		vector<GeneralizedPoint> newPath = vector<GeneralizedPoint>(path);
	    		newPath.push_back(pointParents[parentsIndex]);
	    		shortestPathsToUnexplored.push_back(newPath);
	    	}
	    	path.push_back(pointParents[parentsIndex]); //append the last parent to "path"
	    	shortestPathsToUnexplored.push_back(path);
	    }
    }

    for (auto &shortestPath : shortestPathsToUnexplored)
    {
    	std::reverse(shortestPath.begin(), shortestPath.end());
    } 
    return shortestPathsToUnexplored;
} 

//************************************
// Brief:		change the algorithm state, consume/recharge battery, and clean current location by 1 if not cleaned
// Returns:   	void
void _039563838_E::UpdateState()
{
	// if(_debug){
	// 	cout << "_039563838_E::UpdateState" << endl;
	// 	PrintAlgorithmStatus();  //for debug.  delete later!!!!
	// }

	if(_robotStatus == AlgorithmStatus::Done)
		return;

	if(_allExplored && _allCleaned && IsInDocking()){
		_robotStatus = AlgorithmStatus::Done;
		return;
	}

	int remainingSteps = GetRemainingSteps();
	if(_debug) cout << " updatestate remaining: "<< remainingSteps << " from here " << position << ": "<< houseMapping.at(position).stepsToDocking << endl ;
	//TODO: check that when it reaches 0 it stays
	if(remainingSteps <= houseMapping.at(position).stepsToDocking && _robotStatus != AlgorithmStatus::Returning){
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
				cout << "UpdateState shouldn't get here" << endl;
				PrintAlgorithmStatus();

	}

}


void _039563838_E::aboutToFinish(int stepsTillFinishing)
{
	_aboutToFinishWasCalled = true;
	_stepsTillFinishing = stepsTillFinishing;
	
	cout << "aboutToFinish was called !!" << endl;  // delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	cout << "current position : ( " << position.getX() << ", " << position.getY() << ")" << endl; //delete!!!
	
	auto p = houseMapping.find(position);   //delete !!!!!!!!!!!!!!!!!!!!!!!!
	cout << "stepsToDocking = " <<  p->second.stepsToDocking  << endl; 	//delete !!!!! 
} 


void _039563838_E::UpdateAlgorithmInfo(Direction lastStep) 
{
	if(_aboutToFinishWasCalled)
		--_stepsTillFinishing;
	++totalSteps;
	SensorInformation s =  _robotSensor ->sense();

	if(IsInDocking())
		_battery.Recharge();
	else{
		if(! _battery.Consume())
			if(_debug) cout << "algorithm E battery error at step " << totalSteps << endl;
	}

	position.move(lastStep); // update the robot position, as managed by the algorithm, to the new position. It's NOT the same as the Point* field of the sensor
	//no need to update neighboors on stay or allExplored
	bool shouldUpdateNeighboors = 
		((! _allExplored) && (lastStep != Direction::Stay)) ||
		(_robotStatus == AlgorithmStatus::FirstStep);
	if(shouldUpdateNeighboors){
		UpdateNeighboors(s);
	}

	int newDirtLevel = s.dirtLevel;
	newDirtLevel = newDirtLevel - ((newDirtLevel > 0) ? 1 : 0);
	UpdateDirt( houseMapping[position], newDirtLevel);
}

//gets the sensorinformation of the current point, and update its neighboors on the map
void _039563838_E::UpdateNeighboors(SensorInformation s){
	int neighboorStepsFromDocking = houseMapping.at(position).stepsToDocking + 1;
	for(Direction d : directions) 
	{
		GeneralizedPoint neighboorPosition = position;
		neighboorPosition.move(d);
		CellInfo neighboorInfo;
		bool isNeiboorWall = s.isWall[(int) d];
		neighboorInfo.isWall = isNeiboorWall;
		if(isNeiboorWall){
			neighboorInfo.dirt = 0;
		}
		else if (neighboorStepsFromDocking > _battery.GetOneWayDistanceFromDocking())
		{ 
			// end case when we cannot reach neighboor from location because of the battery
			s.isWall[(int) d] = true; // no passage from location to neighboor
			// if it's not in map treat it like a wall for now- if it's reachable from othe point it will be updated later
				continue; // do not update it

		}
		else{
			neighboorInfo.stepsToDocking = neighboorStepsFromDocking;
			neighboorInfo.parents.push_back(OppositeDirection(d));
			neighboorInfo.possibleKnownDirections.push_back(OppositeDirection(d));
		}
		UpdateInMap(neighboorPosition, neighboorInfo);
	}
	//if(_robotStatus == AlgorithmStatus::FirstStep){ // update possible directions for docking
	CellInfo & dockingCellInfo = houseMapping[position];
	for(auto d: directions){
		if(! s.isWall[(int) d])
			if (find(dockingCellInfo.possibleKnownDirections.begin(), dockingCellInfo.possibleKnownDirections.end(), d) == dockingCellInfo.possibleKnownDirections.end())
				dockingCellInfo.possibleKnownDirections.push_back(d);
	}
	if(_debug)
	{
		cout << "UpdateNeighboors end:"  << endl;
		cout << position << " info: " << houseMapping[position];
		for(Direction d : directions) 
		{
			GeneralizedPoint neighboorPosition = position;
			neighboorPosition.move(d);
			cout << neighboorPosition << " info: " << houseMapping[neighboorPosition];
		}
	}
}

//returns true only if all is unexplored
bool _039563838_E::CheckAllExplored(){
    for ( const auto &pointInfoPair : houseMapping) 
    {
        if(pointInfoPair.second.isUnexplored())
        	return false;
    }
    return true;
}

bool _039563838_E::CheckAllCleaned(){
    for ( const auto &pointInfoPair : houseMapping) 
    {
        if( ! pointInfoPair.second.isClean())
        	return false;
    }
    return true;
}

void _039563838_E::UpdateDirt(CellInfo & oldInfo, int newDirtLevel)
{
	if(newDirtLevel == -1 || (oldInfo.dirt == newDirtLevel))	//no need to update unkown or same
		return;

	if(oldInfo.dirt == -1){ //unexplored turned to explored
		oldInfo.dirt = newDirtLevel; 
		_allExplored = CheckAllExplored();
	}
	else{
		oldInfo.dirt = newDirtLevel;
	}
	if(_allExplored  && newDirtLevel == 0)
		_allCleaned = CheckAllCleaned();
}

//gets a point and new info and add/update it in the map
void _039563838_E::UpdateInMap(GeneralizedPoint point, CellInfo newInfo)
{

	if( ! isInMap(point)){
		//save new point to map
		// if(_debug) cout << "new point was added: (" << point.getX() << ","  << point.getY() << ") d:" << newInfo.stepsToDocking << endl;
		houseMapping[point] = newInfo;
		return;
	}
	//point exists, update it
	CellInfo & oldInfo = houseMapping[point];

	if(oldInfo.isWall)
		return;

	UpdateDirt(oldInfo, newInfo.dirt);
	vector<Direction> & oldKnownDir = oldInfo.possibleKnownDirections;
	//updates possible directions
	for(Direction d : newInfo.possibleKnownDirections) {
		if(find(oldKnownDir.begin(), oldKnownDir.end(), d) == oldKnownDir.end())
			oldInfo.possibleKnownDirections.push_back(d);
	}

	//handle same length path to docking
	if(oldInfo.stepsToDocking == newInfo.stepsToDocking){
		if( find(oldInfo.parents.begin(), oldInfo.parents.end(), newInfo.parents[0]) != oldInfo.parents.end())
			return;	//path is already saved
		// if(_debug)	cout << "point: (" << point.getX() << ","  << point.getY() << ") update. new parent direction: " << newInfo.parents[0] << endl;
		oldInfo.parents.push_back(newInfo.parents[0]);
	} 

	//shorter path found
	if(oldInfo.stepsToDocking > newInfo.stepsToDocking){
		// if(_debug) cout << endl << "SP2D was found for " << point 
		// 	<< " old path d: " << oldInfo.stepsToDocking << " from: " << DirectionsToString(oldInfo.parents)
		// 	<< " new path d: " << newInfo.stepsToDocking << " from: " << DirectionsToString(newInfo.parents) << endl;
		oldInfo.stepsToDocking = newInfo.stepsToDocking;

		for ( const auto & d : houseMapping.at(point).possibleKnownDirections) 
	    {
	    	GeneralizedPoint neighboor = point;
			neighboor.move(d);
			CellInfo neighboorInfo;
			neighboorInfo.isWall = false;
			neighboorInfo.stepsToDocking = newInfo.stepsToDocking + 1;
			neighboorInfo.parents.push_back(OppositeDirection(d));
			UpdateInMap(neighboor, neighboorInfo);
		}
	}
}


//for debug:
void _039563838_E::printDebugHouseMapping() 
{

	if(_debug) {
	
            cout << "---------------------------" << endl;
            for(auto& a : houseMapping) {
                // debug print of all the points mapped so far
                vector<Direction> possibleKnownDirections;
    			for ( const auto & dir : a.second.possibleKnownDirections) 
	    			possibleKnownDirections.push_back(dir);
                if(a.second.isWall){
                	cout << a.first << " Wall" << endl;               	
                }
                else{
                	cout << a.first << " : dirt = " << a.second.dirt
                        << ", stepsToDocking = " << a.second.stepsToDocking 
                        << ", parents:" << DirectionsToString(a.second.parents)
                        << " known-directions:" << DirectionsToString(possibleKnownDirections) << endl;
                }
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
void _039563838_E::PrintAlgorithmStatus()
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
	case AlgorithmStatus::Cleaning:
		cout << "status = Cleaning" << endl;
		return;
	case AlgorithmStatus::Done:
		cout << "status = Done" << endl;
		return;
	case AlgorithmStatus::FirstStep:
		cout << "status = FirstStep" << endl;
		return;
	default:
		cout << "status =  unspecified" << endl;
	}
}




REGISTER_ALGORITHM (_039563838_E)