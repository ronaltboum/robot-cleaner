#ifndef _039563838_E_h__
#define _039563838_E_h__

#include "Direction.h"
#include "DirectionExt.h"
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

class _039563838_E :	public AbstractAlgorithm
{
	friend class DynamicPathFinder;
//~~~~~~~~~~~~~~~~~~~~` Macros and definitions ~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	// a struct used to save the best cleaning path, and recalculate it only when needed 
	// (= when the simulator chose to ignore our recommendations)
	struct CleaningPathCache
	{
		vector<GeneralizedPoint> savedPath;
		size_t index = 0;
		size_t steps_remaining = 0;

		bool WasCorrectStepCommited(Direction lastStep, int remainingSteps){
			if(steps_remaining != (size_t)remainingSteps)
				return false;
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
	};

	enum class AlgorithmStatus {
		FirstStep, 			// the algorithm didn't made any step yet
		ChargingInDocking, 	// the algorithm is charging until battery is full
		Exploring, 			// the algorithm is trying to reach all points in the house it has no information on
		Cleaning,			// the algorithm is done exploring and now it's greedily cleans
		Returning, 			// the algorithm battery is running out,  so it's returning to docking via the same path it travelled from docking to current position
		Done 				// the algorithm is done cleaning the house
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
	CleaningPathCache _cleaningPathCache; // used for saving cleaning path and reusing it
	bool _debug = true;  //when _debug ==1 we uncomment the debug prints

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	_039563838_E(void);
	_039563838_E(const AbstractSensor& sensor, map<string, int> config);
	~_039563838_E(void);
	void initiallize(); //function called in each c'tor

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	virtual Direction step(Direction lastStep);
	virtual void aboutToFinish(int stepsTillFinishing); 
	virtual void setSensor(const AbstractSensor& sensor);
	virtual void setConfiguration(map<string, int> config);
protected:
	Direction ChooseStepAccordingToState(Direction lastStep); // choose the step to do according to step
	int GetRemainingSteps();
	virtual bool IsInDocking() const; //: check if the algorithm is in the docking station
	virtual void UpdateState(); //: change the state
	Direction Handle_Explore_State();
	Direction Handle_Returning_State();
	Direction Handle_Cleaning_State(Direction lastStep);
	bool CheckAllCleaned();
	bool isInMap(GeneralizedPoint pointToCheck);
	void UpdateNeighboors(SensorInformation s);
	void UpdateInMap(GeneralizedPoint point, CellInfo newInfo);
	bool CheckAllExplored();
	void UpdateDirt(CellInfo & oldInfo, int newDirtLevel);
	deque<vector<GeneralizedPoint>> GetSPToUnexplored(bool fromDocking = false);
	deque<vector<GeneralizedPoint>> GetSPToDocking() const;
	bool CheckUnexploredPointsReachability();
	void UpdateAlgorithmInfo(Direction lastStep);
	void printDebugHouseMapping();
	void PrintAlgorithmStatus();  //for debug


}; // end of class _039563838_E

inline void _039563838_E::setSensor(const AbstractSensor& sensor) 
{
	_robotSensor = &sensor;
}

inline void _039563838_E::setConfiguration(map<string, int> config)
{
	_configs = map<string,int>(config);
	_battery = Battery(_configs);
}

//: check if the algorithm is in the docking station
inline bool _039563838_E::IsInDocking() const
{
	return (position.getX() == 0 && position.getY() == 0);
}

inline bool _039563838_E::isInMap(GeneralizedPoint pointToCheck){
	return (houseMapping.find(pointToCheck) != houseMapping.end());
}

#endif // _039563838_E_h__
