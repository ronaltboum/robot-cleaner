#include "DynamicPathFinder.h"
	
	DynamicPathFinder::DynamicPathFinder(const GeneralizedPoint & startingLocation, 
		const map<GeneralizedPoint, CellInfo> & completeHouseMapping, int requestedDistance)
		: _startingLocation(startingLocation), _currentQueue(_dynamicQueue), _nextQueue(_dynamicQueue2), _currentHouseMap(completeHouseMapping)
	{
		_remainingSteps = requestedDistance;
		_iterationNum = 1;
		_dynamicQueue = map<GeneralizedPoint, PointInfo>();
		_dynamicQueue2 = map<GeneralizedPoint, PointInfo>();
		PointInfo startingLocationInfo;
		startingLocationInfo._amountCleaned = 0;
		startingLocationInfo._mapToPaths = {{completeHouseMapping , {startingLocation}}};
		_currentQueue[startingLocation] = startingLocationInfo;
		if(_debug) cout << "requestedDistance: " << requestedDistance << endl;
	}

	DynamicPathFinder::~DynamicPathFinder(void){

	}

	// check all paths that leads to src if adding a step to dest could clean
	bool DynamicPathFinder::MoveCleans(const GeneralizedPoint & src, const GeneralizedPoint & dest)
	{
		const map<HouseMap, Path> & HouseMapToPaths =  _currentQueue.at(src)._mapToPaths;
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
			[&moveCleans, &dest](const pair<HouseMap, Path> & pair) -> bool { 
				return ((! moveCleans) || (pair.first.at(dest).dirt > 0)); 
			};

		map<HouseMap, Path> & HouseMapToPaths =  _currentQueue.at(src)._mapToPaths;


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
			Path & pathsFromSrc = houseMapItr->second;
			HouseMap newHouseMap = HouseMap(houseMapItr->first);
			if(moveCleans) --newHouseMap[dest].dirt;
			Path newPath = Path(pathsFromSrc);
			newPath.push_back(dest);
			//add the new path in the currect map
			_nextQueue[dest]._mapToPaths[newHouseMap] = newPath;
		}
	}


	void DynamicPathFinder::RunIterationStep()
	{
		if(_debug) 
			cout  << endl << "iteration " << _iterationNum << " start:" << endl;

		//runing on all GeneralizedPoint-PointInfo pair which are already set and adding new paths
		int pointsIndex = 1;
		for(auto it = _currentQueue.begin(); it != _currentQueue.end(); ++it)
		{
			const GeneralizedPoint & currentPoint = it->first;
			if(_debug) cout << "running on point " << pointsIndex++ << ": " << currentPoint << endl;
			if(_debug) 	cout << currentPoint << "Step(): HouseMapToPaths size: " <<  _currentQueue.at(currentPoint)._mapToPaths.size() << endl;
			auto & HouseMapToPaths = it->second._mapToPaths;

			//iterating on all known directions from point
			IteratorMoveUdate(currentPoint, currentPoint); // stay step iteration
			for(const Direction & dir : HouseMapToPaths.cbegin()->first.at(currentPoint).possibleKnownDirections)
			{
				GeneralizedPoint dest = currentPoint;
				dest.move(dir);
				IteratorMoveUdate(currentPoint, dest);
			}
			if(_debug) cout << "possible directions num: \t" << HouseMapToPaths.cbegin()->first.at(currentPoint).possibleKnownDirections.size() << endl;
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

	/*
	*	Get a path and returns it's score - the score is based on the amount of dirt cleaned on path
	*	and how far away from the docking was the dirt.
	*/
	int DynamicPathFinder::GetPathScore(const Path & path) const
	{
		int score  = 0;
		map<GeneralizedPoint, int> scoringMap = map<GeneralizedPoint, int>();
		for(const GeneralizedPoint & p : path)
		{
			if(scoringMap.find(p) != scoringMap.end())
				scoringMap[p]++;
			else
				scoringMap[p] = 1;
		}
		for(const auto & pointScorePair : scoringMap){
			const GeneralizedPoint & p = pointScorePair.first;
			const CellInfo & pInfo = _currentHouseMap.at(p);
			score += pInfo.stepsToDocking * min(scoringMap[p], pInfo.dirt);
		}
		return score;
	}

	/*
	*	Get the best path from all the paths to point dest of size n.
	*	The best path of all the path that cleans the biggest amout is the one that cleans the largest amout
	*	of dirt from points distant from docking
	*/
	const DynamicPathFinder::Path DynamicPathFinder::GetBestPathTo(const GeneralizedPoint & dest) const
	{		
		map<Path, int> scoringPathsMap = map<Path, int>();
		map<HouseMap, Path> & mapToPaths = _currentQueue.at(dest)._mapToPaths;
		
		if(_debug) cout << "GetBestPathTo: num of maps: " << mapToPaths.size() << endl;
		if(mapToPaths.size() == 1)
			return mapToPaths.cbegin()->second;
		for(const auto & HouseMapPathsPair : mapToPaths)
		{
			scoringPathsMap[HouseMapPathsPair.second] = GetPathScore(HouseMapPathsPair.second);
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
			cout << "amount cleaned: " << _currentQueue.at(dest)._amountCleaned << " size of map: " << _currentQueue.at(dest)._mapToPaths.size() << endl;
			cout << "path: ";
			for(auto & p: bestPath)
				cout << p;
			cout << endl << "path len: " << bestPath.size() << endl;
		}
		return bestPath;
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