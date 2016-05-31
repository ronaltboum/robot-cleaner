#ifndef _DYNAMICPATHFINDER_H__
#define _DYNAMICPATHFINDER_H__

#include "Direction.h"
#include "DirectionExt.h"
#include "MakeUnique.h"
#include "AlgorithmRegistration.h"
#include "AbstractAlgorithm.h"
#include "FactoryDefinition.h"  //header file where factory is defined
#include "AbstractSensor.h"
#include <algorithm>
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "Battery.h"
#include "GeneralizedPoint.h"

using namespace std;

//This class implements Dynamic algorithm used by algorithm _039563838_E 
// in order to find the best paths of size n from current point to all other points 
class DynamicPathFinder
{

//~~~~~~~~~~~~~~~~~~~~` Macros and definitions ~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	struct CellInfo {
		int dirt; // -1 represents "unknown"
		int stepsToDocking; // -1 represents "unknown" when it's wall
		bool isWall;
		vector<Direction> parents; // directions where you can go from here to get to docking fastest
		//directions of where we know we can go back. false might be unkown yet and change in the future.
		vector<Direction> possibleKnownDirections;
		bool isUnexplored() const { return (dirt == -1); }
		bool isClean() const { return (dirt == 0); }

	    bool operator<(const CellInfo& other)const {
	        return (dirt < other.dirt); 
    	}

    	CellInfo()
    		: dirt(0), stepsToDocking(-1), isWall(true), parents(vector<Direction>()), possibleKnownDirections(vector<Direction>()){ }

    	CellInfo(int dirt, int stepsToDocking, bool isWall)
    		: dirt(dirt), stepsToDocking(stepsToDocking), isWall(isWall), parents(vector<Direction>()), possibleKnownDirections(vector<Direction>()){ }

    	CellInfo(int dirt, int stepsToDocking, bool isWall, vector<Direction> parents, vector<Direction> possibleKnownDirections)
    		: dirt(dirt), stepsToDocking(stepsToDocking), isWall(isWall), parents(parents), possibleKnownDirections(possibleKnownDirections){ }
	};

	typedef std::vector<GeneralizedPoint> Path;

	typedef map<GeneralizedPoint, CellInfo> HouseMap;
	

	struct PointInfo
	{
		int _amountCleaned = -1;				//nothing is known
		map<HouseMap, Path> _mapToPaths; // a map of all the housemaps with one of the path leading to them
	};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	GeneralizedPoint _startingLocation;
	map<GeneralizedPoint, PointInfo> _dynamicQueue;
	map<GeneralizedPoint, PointInfo> _dynamicQueue2;
	map<GeneralizedPoint, PointInfo> & _currentQueue;
	map<GeneralizedPoint, PointInfo> & _nextQueue;
	HouseMap _currentHouseMap;
	size_t _iterationNum;
	size_t _remainingSteps;
	bool _debug = false;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	DynamicPathFinder(const GeneralizedPoint & startingLocation, 
		const map<GeneralizedPoint, CellInfo> & completeHouseMapping, int requestedDistance);
	~DynamicPathFinder(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	bool MoveCleans(const GeneralizedPoint & src, const GeneralizedPoint & dest);
	void IteratorMoveUdate(const GeneralizedPoint & src , const GeneralizedPoint & dest);
	void RunIterationStep();
	int GetPathScore(const Path & path) const;
public:
	void RunIteration();
	const Path GetBestPathTo(const GeneralizedPoint & dest) const;
	const Path GetBestPathToAny() const;
};


// can sort two identical maps (with different dirts on points)
// the map which has more dirt in the first point where they not identical is bigger
inline bool operator< (const map<GeneralizedPoint, DynamicPathFinder::CellInfo> & lhs, const map<GeneralizedPoint, DynamicPathFinder::CellInfo>& rhs){ 
	for(const auto & pointToInfoPair : lhs){
		const GeneralizedPoint & p = pointToInfoPair.first;
		int leftDirt = lhs.at(p).dirt;
		int rightDoirt = rhs.at(p).dirt;
		if(leftDirt < rightDoirt)
			return true;
		if(leftDirt > rightDoirt)
			return false;
	}
	return false;
}

inline std::ostream& operator<<(std::ostream& out, const DynamicPathFinder::CellInfo & c) {
	if(c.isWall) 
		return out << "wall" << endl;
	
	return out << "dirt: " << c.dirt << " distance: " << c.stepsToDocking << endl 
		<< "parents: " << DirectionsToString(c.parents)  << endl
		<< "possibleKnownDirections: " << DirectionsToString(c.possibleKnownDirections) << endl;
}

#endif // _DYNAMICPATHFINDER_H__
