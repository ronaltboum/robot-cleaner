#ifndef DynamicAlgo_h__
#define DynamicAlgo_h__

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
#include "DynamicPathFinder.h"

using namespace std;

typedef vector<GeneralizedPoint> Path;

class DynamicAlgo :	public AbstractAlgorithm
{
	friend class DynamicPathFinder;
//~~~~~~~~~~~~~~~~~~~~` Macros and definitions ~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	// a struct used to save the best cleaning path, and recalculate it only when needed 
	// (= when the simulator chose to ignore our recommendations)
	enum class AlgorithmStatus {
		FirstStep, 			// the algorithm didn't made any step yet
		ChargingInDocking, 	// the algorithm is charging until battery is full
		Exploring, 			// the algorithm is trying to reach all points in the house it has no information on
		Cleaning,			// the algorithm is done exploring and now it's greedily cleans
		Returning, 			// the algorithm battery is running out,  so it's returning to docking via the same path it travelled from docking to current position
		Done,				// the algorithm is done cleaning the house
		StayingToClean		// the algorithm found dirt in a spot while exploring and stays there to clean
	};	

	struct PathCache
	{
		vector<GeneralizedPoint> savedPath;
		size_t index = 0;
		size_t steps_remaining = 0;
		AlgorithmStatus status;

		bool StillCorrect(Direction lastStep, int remainingSteps, AlgorithmStatus currentStatus ){
			if(currentStatus != status)
				return false;
			if(steps_remaining != (size_t)remainingSteps){
				return false;
			}
			if(savedPath.empty() || ReachedEnd() || index == 0) //out of boundries
				return false;
			return savedPath[index - 1].GetDirection(savedPath[index]) == lastStep;
		}
		bool ReachedEnd(){
			return index >= savedPath.size() - 1; // index in last element
		}
		Direction GetDirection()
		{
			Direction d = savedPath[index].GetDirection(savedPath[index + 1]);
			++index;
			--steps_remaining;
			return d;
		}

		PathCache(vector<GeneralizedPoint> savedPath = vector<GeneralizedPoint>(), size_t index = 0, size_t steps_remaining = 0, AlgorithmStatus status = AlgorithmStatus::FirstStep)
			: savedPath(savedPath), index(index), steps_remaining(steps_remaining), status(status){}
	};


	typedef DynamicPathFinder::CellInfo CellInfo;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	AlgorithmStatus _robotStatus;
	const AbstractSensor* _robotSensor;
	map<string,int> _configs;
	Battery _battery;
	bool _allExplored; // true when all the house was explored
	bool _allCleaned;
	bool _aboutToFinishWasCalled;
	map<GeneralizedPoint, CellInfo> houseMapping;
	GeneralizedPoint position; // current position
	int totalSteps;
	int _stepsTillFinishing;  //_stepsTillFinishing == MaxStepsAfterWinner when aboutToFinish is called by the simulation
								// otherwise it's -1
	bool AboutToFinishWasCalled;  //equals true if aboutToFinish was called by the simulation
	PathCache _pathCache; // used for saving cleaning path or returning path and reusing it
	bool _debug = false;  //when _debug ==1 we uncomment the debug prints

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	DynamicAlgo(void);
	DynamicAlgo(const AbstractSensor& sensor, map<string, int> config);
	~DynamicAlgo(void);
protected:
	void initiallize(); //function called in each c'tor

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	virtual Direction step(Direction lastStep);
	virtual void aboutToFinish(int stepsTillFinishing); 
	virtual void setSensor(const AbstractSensor& sensor);
	virtual void setConfiguration(map<string, int> config);
protected:
	virtual Direction ChooseStepAccordingToState(Direction lastStep); // choose the step to do according to step
	int GetRemainingSteps();
	bool IsInDocking() const; //: check if the algorithm is in the docking station
	virtual void UpdateState(); //: change the state
	Direction Handle_Explore_State();
	Direction Handle_Returning_State(Direction lastStep);
	Direction Handle_Cleaning_State(Direction lastStep);
	bool CheckAllCleaned();
	bool isInMap(GeneralizedPoint pointToCheck);
	void UpdateNeighboors(SensorInformation s);
	void UpdateInMap(GeneralizedPoint point, CellInfo & newInfo);
	bool CheckAllExplored();
	void UpdateDirt(CellInfo & oldInfo, int newDirtLevel);
	deque<vector<GeneralizedPoint>> GetSPToUnexplored(bool fromDocking = false);
	vector<GeneralizedPoint> GetSPToDocking();
	bool CheckUnexploredPointsReachability();
	void UpdateAlgorithmInfo(Direction lastStep);
	void printDebugHouseMapping();
	void PrintAlgorithmStatus();  //for debug
	int GetPathTotalCleaning(const vector<GeneralizedPoint> & path) const;
	vector<GeneralizedPoint> GetBestPathToDocking(const deque<vector<GeneralizedPoint>> & pathsToDocking);


}; // end of class DynamicAlgo

inline void DynamicAlgo::setSensor(const AbstractSensor& sensor) 
{
	_robotSensor = &sensor;
}

inline void DynamicAlgo::setConfiguration(map<string, int> config)
{
	_configs = map<string,int>(config);
	_battery = Battery(_configs);
}

//: check if the algorithm is in the docking station
inline bool DynamicAlgo::IsInDocking() const
{
	return (position.getX() == 0 && position.getY() == 0);
}

inline bool DynamicAlgo::isInMap(GeneralizedPoint pointToCheck){
	return (houseMapping.find(pointToCheck) != houseMapping.end());
}

#endif // DynamicAlgo_h__
