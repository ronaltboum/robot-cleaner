#include "DynamicPathFinder.h"
	
	DynamicPathFinder::DynamicPathFinder(const GeneralizedPoint & startingLocation, 
		const map<GeneralizedPoint, CellInfo> & completeHouseMapping, int requestedDistance)
		: _startingLocation(startingLocation), _currentQueue(_dynamicQueue), _nextQueue(_dynamicQueue2)
	{
		_remainingSteps = requestedDistance;
		_iterationNum = 1;
		_dynamicQueue = map<GeneralizedPoint, PointInfo>();
		_dynamicQueue2 = map<GeneralizedPoint, PointInfo>();
		map<HouseMap, vector<Path>> startingLocationMap = {{completeHouseMapping , {{startingLocation}}}};
		PointInfo startingLocationInfo = {0, startingLocationMap};
		_currentQueue[startingLocation] = startingLocationInfo;
		if(_debug) cout << "requestedDistance: " << requestedDistance << endl;
	}

	DynamicPathFinder::~DynamicPathFinder(void){

	}

	// check all paths that leads to src if adding a step to dest could clean
	bool DynamicPathFinder::MoveCleans(const GeneralizedPoint & src, const GeneralizedPoint & dest)
	{
		const map<HouseMap, vector<Path>> & HouseMapToPaths =  _currentQueue.at(src)._mapToPaths;
		for(const auto& HouseMapPathsPair : HouseMapToPaths){
			if(HouseMapPathsPair.first.at(dest).dirt > 0) // there's dirt in the current map in the current location 
				return true;
		}
		return false;
	}

	void DynamicPathFinder::IteratorMoveUdate(const GeneralizedPoint & src , const GeneralizedPoint & dest)
	{
		if(_debug) 	cout << "IteratorMoveUdate: " << src << " HouseMapToPaths size: " <<  _currentQueue.at(src)._mapToPaths.size() << endl;
		int srcDirt = _currentQueue.at(src)._amountCleaned;
		bool destInNextQueue = (_nextQueue.find(dest) != _nextQueue.end());
		int destDirt = destInNextQueue ? _nextQueue.at(dest)._amountCleaned : -1;
		if(destDirt > srcDirt + 1) // we can't cllean more than one in each step
			return;
		bool moveCleans = MoveCleans(src, dest);
		int newDirt = moveCleans ? srcDirt + 1 : srcDirt;
		if(_debug) cout << "dirt: " << src << ": " << srcDirt << " , " << dest << ": " << destDirt<< ", after: " << newDirt << endl; 
		if(destDirt > newDirt)
			return;
		
		auto houseMapFilter = 	//filter all the maps which doesn't clean if we could clean
			[&moveCleans, &dest](const pair<HouseMap, vector<Path>> & pair) -> bool { 
				return ((! moveCleans) || (pair.first.at(dest).dirt > 0)); 
			};

		map<HouseMap, vector<Path>> & HouseMapToPaths =  _currentQueue.at(src)._mapToPaths;


		if(_debug){
			int i = 0;
			for(
				auto houseMapItr = find_if( HouseMapToPaths.begin(), HouseMapToPaths.end(), houseMapFilter);
				houseMapItr != HouseMapToPaths.end();
				houseMapItr = find_if(++houseMapItr, HouseMapToPaths.end(), houseMapFilter))
			{++i;}
			cout << "housemap after filtering: " << i << endl;	
		}
		PointInfo newPointInfo;
		newPointInfo._amountCleaned = newDirt;
		_nextQueue[dest] = newPointInfo;

		for(
			auto houseMapItr = find_if( HouseMapToPaths.begin(), HouseMapToPaths.end(), houseMapFilter);
			houseMapItr != HouseMapToPaths.end();
			houseMapItr = find_if(++houseMapItr, HouseMapToPaths.end(), houseMapFilter))
		{
			vector<Path> & pathsFromSrc = houseMapItr->second;
			// runing on all paths of the maps and adding the new paths with dest to _nextQueue
			for( auto pathItr = pathsFromSrc.begin(); pathItr != pathsFromSrc.end() ; ++pathItr) {
				//if(_debug) cout << "DynamicPathFinder::IteratorMoveUdate\tinside for for loop";
				HouseMap newHouseMap = HouseMap(houseMapItr->first);
				if(moveCleans) --newHouseMap[dest].dirt;
				Path newPath = Path(*pathItr);
				newPath.push_back(dest);
				//add the new path in the currect map
				_nextQueue[dest]._mapToPaths[newHouseMap].push_back(newPath);
			}
		}
	}


	void DynamicPathFinder::RunIterationStep()
	{
		if(_debug) cout << endl << "iteration " << _iterationNum << " start:" << endl;

		//runing on all GeneralizedPoint-PointInfo pair which are already set and adding new paths
		int pointsIndex = 1;
		for(auto it = _currentQueue.begin(); it != _currentQueue.end(); ++it)
		{
			const GeneralizedPoint & currentPoint = it->first;
			if(_debug) cout << "running on point " << pointsIndex++ << ": " << currentPoint << endl;
			if(_debug) 	cout << currentPoint << "Step(): HouseMapToPaths size: " <<  _currentQueue.at(currentPoint)._mapToPaths.size() << endl;
			auto & HouseMapToPaths = it->second._mapToPaths;

			//getting known directions from point
			vector<Direction> directionsFromPoint = {Direction::Stay};
			for(auto & directionAvailablePair : HouseMapToPaths.cbegin()->first.at(currentPoint).possibleKnownDirections)
			{
				if(directionAvailablePair.second)
					directionsFromPoint.push_back(directionAvailablePair.first);
			}
			if(_debug) cout << "possible directions num: \t" << directionsFromPoint.size() << endl;
			
			for(Direction & d : directionsFromPoint){
				GeneralizedPoint dest = currentPoint;
				dest.move(d);
				IteratorMoveUdate(currentPoint, dest);
			}
		}

		if(_debug){
			cout << "iteration " << _iterationNum << " end. current: ";
			for(auto it = _currentQueue.begin(); it != _currentQueue.end(); ++it)
			{
				cout << it->first;
			}
			cout << " next: ";
			for(auto it = _nextQueue.begin(); it != _nextQueue.end(); ++it)
			{
				cout << it->first;
			} 
			cout << endl;
		}
		_currentQueue = map<GeneralizedPoint, PointInfo>();
		swap(_currentQueue, _nextQueue);
		++_iterationNum;

	}

	void DynamicPathFinder::RunIteration(){
		while(_iterationNum < _remainingSteps + 1)
			RunIterationStep();
	}
	
	const DynamicPathFinder::Path DynamicPathFinder::GetBestPathTo(const GeneralizedPoint & dest) const
	{		
		map<HouseMap, vector<Path>> mapToPaths = _currentQueue.at(dest)._mapToPaths;
		Path firstPath = mapToPaths.cbegin()->second.back();
		if(_debug){
			cout << "amount cleaned: " << _currentQueue.at(dest)._amountCleaned << " size of map: " << _currentQueue.at(dest)._mapToPaths.size() << endl;
			cout << "path: ";
			for(auto & p: firstPath)
				cout << p;
			cout << endl << "path len: " << firstPath.size() << endl;
		}
		return firstPath;
	}

	const DynamicPathFinder::Path DynamicPathFinder::GetBestPathToAny() const
	{
		int maxCleaned = -1;
		GeneralizedPoint maxDest;
		for(const auto & PointToInfo : _currentQueue)
		{
			if(PointToInfo.second._amountCleaned > maxCleaned)
			{
				maxCleaned = PointToInfo.second._amountCleaned;
				maxDest = PointToInfo.first;
			}

		}
		return GetBestPathTo(maxDest);
	}