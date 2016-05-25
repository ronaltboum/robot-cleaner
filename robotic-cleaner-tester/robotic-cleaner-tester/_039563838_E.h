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

using namespace std;

class _039563838_E :	public AbstractAlgorithm
{
//~~~~~~~~~~~~~~~~~~~~` Macros and definitions ~~~~~~~~~~~~~~~~~~~~~~~~~
//enum class AlgorithmStatus {ChargingInDocking,			
//														Exploring,				// the algorithm is exploring. Always prefers to visit cells it hasn't been in
//														Returning,				
//														ReturningRapidly 	
//};	

enum class AlgorithmStatus {
	FirstStep, 			// the algorithm didn't made any step yet
	ChargingInDocking, 	// the algorithm is charging until battery is full
	Exploring, 			// the algorithm is trying to reach all points in the house it has no information on
	Cleaning,			// the algorithm is done exploring and now it's greedily cleans
	Returning, 			// the algorithm battery is running out,  so it's returning to docking via the same path it travelled from docking to current position
	Done 				// the algorithm is done cleaning the house
};			
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	struct CellInfo {
		int dirt = -1; // -1 represents "unknown"
		int stepsToDocking = -1; // -1 represents "unknown" when it's wall
		bool isWall = false;
		vector<Direction> parents = vector<Direction>(); // directions where you can go from here to get to docking fastest
		//directions of where we know we can go back. false might be unkown yet and change in the future.
		map<Direction, bool> possibleKnownDirections = { {Direction::East, false}, {Direction::West, false}, 
												{Direction::South, false}, {Direction::North, false}};
		bool isUnexplored() const { return (dirt == -1); }
		bool isClean() const { return (dirt == 0); }
	};

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
	int _debug;  //when _debug ==1 we uncomment the debug prints

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	_039563838_E(void);
	_039563838_E(const AbstractSensor& sensor, map<string, int> config);
	~_039563838_E(void);
	void initiallize(); //function called in each c'tor

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	bool isInMap(GeneralizedPoint pointToCheck);
	void UpdateNeighboors(SensorInformation s);
	void UpdateInMap(GeneralizedPoint point, CellInfo newInfo);
	bool CheckAllExplored();
	void UpdateDirt(CellInfo & oldInfo, int newDirtLevel);
	deque<vector<GeneralizedPoint>> GetSPToUnexplored() const;
	deque<vector<GeneralizedPoint>> GetSPToDocking() const;

public:
	virtual Direction step(Direction lastStep);
	virtual void aboutToFinish(int stepsTillFinishing); 
	virtual void setSensor(const AbstractSensor& sensor);
	virtual void setConfiguration(map<string, int> config);
protected:
	virtual bool IsInDocking() const; //: check if the algorithm is in the docking station
	virtual void UpdateState(); //: change the state
	Direction Handle_Explore_State();
	Direction Handle_Returning_State();
	bool CheckAllCleaned();


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
