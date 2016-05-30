//#include "stdafx.h"
#include "_039563838_C.h"
#include "AlgorithmRegistration.h"
#include <iostream>

using namespace std;

_039563838_C::_039563838_C(const AbstractSensor& sensor, map<string, int> config)
{
	initiallize();
	setSensor(sensor);
	setConfiguration(config);

	int _maxSteps =  _battery.GetOneWayDistanceFromDocking();
	if(_debug) cout << "one way: "  << _maxSteps << endl;
}

_039563838_C::_039563838_C(void)
{
	initiallize();
}

//function called in each c'tor
void _039563838_C::initiallize()
{
	position = {0,0};  //as far as the algorithm is concerned, the docking point is at (0,0)
	_configs = map<string,int>();
	_battery = Battery();
	_robotStatus = AlgorithmStatus::FirstStep;
	_pathCache = { vector<GeneralizedPoint>(), 0, 0};

	houseMapping[position] = CellInfo(0,0,false);
	totalSteps = 0;
	_stepsTillFinishing = -8;  //_stepsTillFinishing == MaxStepsAfterWinner when aboutToFinish is called by the simulation
	_aboutToFinishWasCalled = false;  //equals true if aboutToFinish was called by the simulation
	_allExplored = false;
	_allCleaned = false;
}

_039563838_C::~_039563838_C(void)
{
}

//************************************
// Brief:	choose a step and do it
// Recieves:	The ACTUAL last step that was taken by the simulation.  May be differnet than what the algorithm recommended
// Returns:   	Direction where it went
//************************************
Direction _039563838_C::step(Direction lastStep)
{
	if(_debug) {
		cout << "step: call number #" << totalSteps << endl;
		if(_aboutToFinishWasCalled) cout << "_stepsTillFinishing = " << _stepsTillFinishing << endl;  //delete !!!
		PrintAlgorithmStatus();
		cout << _battery;
	}
	UpdateAlgorithmInfo(lastStep);  //updates lastStep
	AlgorithmStatus previous = _robotStatus;
	UpdateState();
	if(previous == AlgorithmStatus::Cleaning && _robotStatus != previous) // reseting cache
	{
		_pathCache = { vector<GeneralizedPoint>(),0, 0};
	}
	// return ChooseStepAccordingToState(lastStep);
	Direction d = ChooseStepAccordingToState(lastStep);
	if(_debug){ 
		cout << "choosen direction: " << d << endl;
		cout << "choosen direction: " << d << endl;
		cout << "after step:" << endl;
		for(auto & pi : houseMapping)
			cout << pi.first << " info: " << pi.second;
	}
	return d; 
}

Direction _039563838_C::ChooseStepAccordingToState(Direction lastStep)
{
	Direction d;
	switch(_robotStatus)
	{
	case AlgorithmStatus::Done:
	case AlgorithmStatus::StayingToClean:
	case AlgorithmStatus::ChargingInDocking:
		return Direction::Stay;
	case AlgorithmStatus::Cleaning:
		return Handle_Cleaning_State(lastStep);
	case AlgorithmStatus::Returning:
		return Handle_Returning_State(lastStep);
	case AlgorithmStatus::Exploring:
		d = Handle_Explore_State();
		if(_debug) cout << "explore state returned" << d << endl;
		return d;
	case AlgorithmStatus::FirstStep:
		return Direction::Stay; //shouldn't get here
	} //closes switch

	if(_debug) cout << "i'm not supposed to get here!!!!"  << endl;  //delete later!!!

	return Direction::Stay;
	
}

Direction _039563838_C::Handle_Returning_State(Direction lastStep){
	if(_debug)	cout << "Handle_Returning_State step number: " << totalSteps << endl;
	int remainingSteps = GetRemainingSteps();
	if(_debug) 
		cout << " _039563838_C::Handle_Returning_State\tremainingSteps: " << remainingSteps << endl;
	if(! _pathCache.StillCorrect(lastStep, remainingSteps, AlgorithmStatus::Returning))
	{
		_pathCache = {GetSPToDocking(), 0 , (size_t)remainingSteps,  AlgorithmStatus::Returning};
		if(_debug) cout << "_pathCache was set. size: " << _pathCache.savedPath.size() << " remainingSteps: " << remainingSteps << endl;
	}
	return _pathCache.GetDirection();
}

int _039563838_C::GetRemainingSteps()
{
	if(_aboutToFinishWasCalled)
		return min(_stepsTillFinishing, _battery.GetStepsBeforeRecharge(IsInDocking()));
	else
		return _battery.GetStepsBeforeRecharge(IsInDocking());
}

Direction _039563838_C::Handle_Cleaning_State(Direction lastStep)
{
	if(_debug)	cout << "Handle_Cleaning_State step number: " << totalSteps << endl;
	int remainingSteps = GetRemainingSteps();
	if(_debug) 
		cout << " _039563838_C::Handle_Cleaning_State\tremainingSteps: " << remainingSteps << endl;
	if(! _pathCache.StillCorrect(lastStep, remainingSteps, AlgorithmStatus::Cleaning))
	{
		DynamicPathFinder pf = DynamicPathFinder(position, houseMapping, remainingSteps);
		pf.RunIteration();
		_pathCache = {pf.GetBestPathTo({0,0}), 0 , (size_t)remainingSteps,  AlgorithmStatus::Cleaning};
		if(_debug) cout << "_pathCache was set. size: " << _pathCache.savedPath.size() << " remainingSteps: " << remainingSteps << endl;
		if(_pathCache.savedPath.size() == 0){	
			if(_debug) cout << "about to finish was called and Algo E couldn't make it to docking" << endl;
			//about to finish was called and we can't go back to docking - choose any best path.
			_pathCache.savedPath = pf.GetBestPathToAny();
		}
	}

	return _pathCache.GetDirection();
}

// check if all unexplored points are unreachable. if so updates _allExplored and houseMapping
// returns true if any unexplored point is reachable
bool _039563838_C::CheckUnexploredPointsReachability()
{
	if( GetSPToUnexplored(true).front().size() <= (size_t)_battery.GetOneWayDistanceFromDocking()) // some are still reachable
		return true;
	// all are unreachable
	for ( auto & pointInfoPair : houseMapping) 
    {
    	if(pointInfoPair.second.isUnexplored())
    	{
    		pointInfoPair.second = CellInfo(); // set it as a wall
    	}
    }
    _allExplored = true;
    _allCleaned = CheckAllCleaned();
    if(_debug && _allExplored) cout << "allExplored" << endl;
    	if(_debug && _allCleaned) cout << "allCleaned" << endl	;
    return false;
}

Direction _039563838_C::Handle_Explore_State()
{
	//cout << "Handle_Explore_State" << endl;
	deque<vector<GeneralizedPoint>> SPToUnexplored = GetSPToUnexplored();
	//cout << "GetSPToUnexplored size" << SPToUnexplored.size() << endl;
	size_t remainingSteps = GetRemainingSteps();

	if(SPToUnexplored.front().size() > remainingSteps){
		if(CheckUnexploredPointsReachability()){ // exploring not finished, but can't explore without returning
			_robotStatus = AlgorithmStatus::Returning;
		}
		else{
			//going back to update state and going from there.
			_pathCache.steps_remaining = 0;
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
	if(_debug) cout << "Handle_Explore_State possible directions: " << DirectionsToString(possibleDirections)<< endl;
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

int _039563838_C::GetPathTotalCleaning(const Path & path) const
{
	int score  = 0;
	map<GeneralizedPoint, int> scoringMap = map<GeneralizedPoint, int>();
	for(const GeneralizedPoint & p : path)
	{
		if(houseMapping.find(p) != houseMapping.end())
			scoringMap[p]++;
		else
			scoringMap[p] = 1;
	}
	for(const auto & pointScorePair : scoringMap){
		const GeneralizedPoint & p = pointScorePair.first;
		score += min(scoringMap[p], houseMapping.at(p).dirt);
	}
	return score;
}

Path _039563838_C::GetBestPathToDocking(const deque<Path> & pathsToDocking)
{
	map<Path, int> scoringPathsMap = map<Path, int>();
	if(_debug) cout << "GetBestPathToDocking: num of paths: " << pathsToDocking.size() << endl;
	if(pathsToDocking.size() == 1)
		return pathsToDocking.front();
	for(const auto & path : pathsToDocking)
	{
		scoringPathsMap[path] = GetPathTotalCleaning(path);
	}
	int maxPoints = -1;
	Path bestPath = scoringPathsMap.begin()->first;
	for(const auto & pathScorePair : scoringPathsMap)
	{
		if(pathScorePair.second > maxPoints){
			maxPoints = pathScorePair.second;
			bestPath = pathScorePair.first;
		}
	}
	 
	if(_debug){
		cout <<  "path: ";
		for(auto & p: bestPath)
			cout << p;
	}
	return bestPath;
}

// returns a deque of the shortests paths from position to docking
Path _039563838_C::GetSPToDocking()
{
	if(_debug) 
		cout << " updatestate remaining: "<< GetRemainingSteps() << " from here " << position << ": "<< houseMapping.at(position).stepsToDocking << endl ;
	deque<Path> shortestPaths;
	Path v;
	v.push_back(position);
	shortestPaths.push_back(v);
	int steps = houseMapping.at(position).stepsToDocking;
	//int steps = houseMapping.find(position)->second.stepsToDocking;
    for(int stepsIndex = 0; stepsIndex< steps; ++stepsIndex)
    {
    	if(_debug) cout << "GetSPToDocking iteration: " <<  stepsIndex << endl;
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
	    		Path newSP = Path(path);
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

    return GetBestPathToDocking(shortestPaths);
}

// returns a deque of the shortests paths from position to unexplored points
// if fromDocking - then from docking. otherwise from current position
deque<Path> _039563838_C::GetSPToUnexplored(bool fromDocking /* = false */)
{
	GeneralizedPoint p = fromDocking ? GeneralizedPoint(0,0) : position;
	if(_debug) cout << "GetSPToUnexplored on " << p << " info: " << houseMapping[p];
	map<GeneralizedPoint, int> distanceFromP;
	for ( const auto &pointInfoPair : houseMapping) 
    {
    	distanceFromP[pointInfoPair.first] = -1; 
    }
    map<GeneralizedPoint , Path> parents;
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
	while(! foundUnexplored){
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
		if(_debug) cout << "q size: " << Q.size() << " other q size: " << OtherQ.size() << endl << endl;
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
	//building the vectors of the shortest paths from p to unexplored points
	//removing from Q points which are explored
	if(_debug) cout << "found" << endl;
	Q.erase(
		std::remove_if (Q.begin(), Q.end(), 
			[=](GeneralizedPoint p) -> bool{ return (! houseMapping.at(p).isUnexplored()); }), 
		Q.end()
		);
	if(_debug) cout << "Q after filter: " << Q.size()<< endl;

	deque<Path> shortestPathsToUnexplored;
	for ( const auto &unexploredPoint : Q) 
    {
    	Path v;
    	v.push_back(unexploredPoint);
    	shortestPathsToUnexplored.push_back(v);
    }
    if(_debug) cout << "shortestPathsToUnexplored init size: " << shortestPathsToUnexplored.size() << endl;
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
	    		Path newPath = Path(path);
	    		newPath.push_back(pointParents[parentsIndex]);
	    		shortestPathsToUnexplored.push_back(newPath);
	    	}
	    	path.push_back(pointParents[parentsIndex]); //append the last parent to "path"
	    	shortestPathsToUnexplored.push_back(path);
	    }
    }
    if(_debug) cout << "shortestPathsToUnexplored after size: " << shortestPathsToUnexplored.size() << endl;

    for (auto &shortestPath : shortestPathsToUnexplored)
    {
    	std::reverse(shortestPath.begin(), shortestPath.end());
    } 
    if(_debug){
    	for (auto p : shortestPathsToUnexplored.front())
    		cout << p;
    	cout << endl;
    }
    return shortestPathsToUnexplored;
} 

//************************************
// Brief:		change the algorithm state, consume/recharge battery, and clean current location by 1 if not cleaned
// Returns:   	void
void _039563838_C::UpdateState()
{
	// if(_debug){
	// 	cout << "_039563838_C::UpdateState" << endl;
	// 	PrintAlgorithmStatus();  //for debug.  delete later!!!!
	// }

	if(_robotStatus == AlgorithmStatus::Done)
		return;

	if(_allExplored && _allCleaned && IsInDocking()){
		_robotStatus = AlgorithmStatus::Done;
		return;
	}

	int remainingSteps = GetRemainingSteps();
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
				else if(houseMapping[position].dirt > 1) // while exploring found dirt and need to stay until cleaned
				{
					if(_debug) cout << "staying to clean " << position << " dirt:" << houseMapping[position].dirt;
					_robotStatus = AlgorithmStatus::StayingToClean;
				}
			}
			return;
		case AlgorithmStatus::StayingToClean:
			{
				if(houseMapping[position].dirt <= 1) // no need to stay
				{
					_robotStatus = AlgorithmStatus::Exploring;
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


void _039563838_C::aboutToFinish(int stepsTillFinishing)
{
	_aboutToFinishWasCalled = true;
	_stepsTillFinishing = stepsTillFinishing;
	if(_debug){
		cout << "aboutToFinish was called !!" << endl;  // delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!
		cout << "current position : ( " << position.getX() << ", " << position.getY() << ")" << endl; //delete!!!
	}
	auto p = houseMapping.find(position);   //delete !!!!!!!!!!!!!!!!!!!!!!!!
	if(_debug)	cout << "stepsToDocking = " <<  p->second.stepsToDocking  << endl; 	//delete !!!!! 
} 


void _039563838_C::UpdateAlgorithmInfo(Direction lastStep) 
{
	if(_aboutToFinishWasCalled)
		--_stepsTillFinishing;
	//update last dirt level
	int newDirtLevel = houseMapping[position].dirt;
	newDirtLevel = newDirtLevel - ((newDirtLevel > 0) ? 1 : 0);
	UpdateDirt( houseMapping[position], newDirtLevel);

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

	//update current dirt level
	UpdateDirt( houseMapping[position], s.dirtLevel);
}

//gets the sensorinformation of the current point, and update its neighboors on the map
void _039563838_C::UpdateNeighboors(SensorInformation s){
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
		cout << endl <<"UpdateNeighboors end:"  << endl;
		cout << position << " info: " << houseMapping[position];
		for(Direction d2 : directions) 
		{
			GeneralizedPoint neigh = GeneralizedPoint(position);
			neigh.move(d2);
			cout << neigh << " info: " << houseMapping[neigh];
		}
	}
}

//returns true only if all is unexplored
bool _039563838_C::CheckAllExplored(){
    for ( const auto &pointInfoPair : houseMapping) 
    {
        if(pointInfoPair.second.isUnexplored())
        	return false;
    }
    return true;
}

bool _039563838_C::CheckAllCleaned(){
    for ( const auto &pointInfoPair : houseMapping) 
    {
        if( ! pointInfoPair.second.isClean())
        	return false;
    }
    return true;
}

void _039563838_C::UpdateDirt(CellInfo & oldInfo, int newDirtLevel)
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
void _039563838_C::UpdateInMap(GeneralizedPoint point, CellInfo newInfo)
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
		oldInfo.parents = newInfo.parents;

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
void _039563838_C::printDebugHouseMapping() 
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
void _039563838_C::PrintAlgorithmStatus()
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

REGISTER_ALGORITHM (_039563838_C)
