#ifndef _039563838_D_h__
#define _039563838_D_h__

#include "Direction.h"
#include "DirectionExt.h"
#include "AbstractAlgorithm.h"
#include "FactoryDefinition.h"  //header file where factory is defined
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "Battery.h"
#include "GeneralizedPoint.h"

using namespace std;

class _039563838_D :	public AbstractAlgorithm
{
//~~~~~~~~~~~~~~~~~~~~` Macros and definitions ~~~~~~~~~~~~~~~~~~~~~~~~~
//enum class AlgorithmStatus {ChargingInDocking,			// the algorithm is charging until battery is full
//														Exploring,				// the algorithm is exploring. Always prefers to visit cells it hasn't been in
//														Returning,				// the algorithm battery is running out,  so it's returning to docking via the same path it travelled from docking to current position
//														ReturningRapidly 	//the algorithm doesn't have enough steps left to return via the path it travelled from docking to current position. Therefore, the algorithm returns via the shortest path to docking.
//};	

enum class AlgorithmStatus {ChargingInDocking, Exploring, Returning, ReturningRapidly };			
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	AlgorithmStatus _robotStatus;
	const AbstractSensor* _robotSensor;
	map<string,int> _configs;
	Battery _battery;
	deque<Direction> _pathFromDocking; // the path back to docking
	int _dirtInCurrentLocation;

	struct CellInfo {
		int dirt = -1; // -1 represents "unknown"
		int stepsToDocking = -1; // -1 represents "unknown"
		bool isWall;
	};
    	map<GeneralizedPoint, CellInfo> houseMapping;
	//    Direction currDir = Direction::East;
    	GeneralizedPoint position = {0,0};  //as far as the algorithm is concerned, the docking point is at (0,0)
//    SensorInformation s;
       int stepsFromDocking = -1;
       int totalSteps = 0;
	int _stepsTillFinishing = -8;  //_stepsTillFinishing == MaxStepsAfterWinner when aboutToFinish is called by the simulation
	bool AboutToFinishWasCalled = false;  //equals true if aboutToFinish was called by the simulation
//    int phase = -1; // phases 0-7: explore surrounding, phase 8: select new direction
//    const static Direction dir_by_phase[8];
//    Direction requestedStep = Direction::Stay;
	int _debug = 0;  //when _debug ==1 we uncomment the debug prints

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	_039563838_D(void);
	_039563838_D(const AbstractSensor& sensor, map<string, int> config);
	~_039563838_D(void);
	void initiallize(); //function called in each c'tor

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	virtual Direction step(Direction lastStep);
	virtual void aboutToFinish(int stepsTillFinishing); 
	virtual void setSensor(const AbstractSensor& sensor);
	virtual void setConfiguration(map<string, int> config);
protected:
	virtual bool IsInDocking() const; //: check if the algorithm is in the docking station
	virtual void UpdateState(); //: change the state
	static Direction OppositeDirection(Direction d);
	virtual vector<Direction> GetPossibleDirections(Direction lastStep) const;
	Direction Handle_Explore_State(vector<Direction> possibleDirections);


	void updateAlgorithmInfo(Direction lastStep);
	Direction FindShortestPath();
	int calcStepsToDocking(int stepsFromDocking, const GeneralizedPoint& position);
	void updateStepsToDocking(int stepsToDocking, const GeneralizedPoint& position);


	void printDebugHouseMapping();
	void PrintLastDirection(Direction lastStep); //for debug
	void PrintAlgorithmStatus();  //for debug
	void PrintPossibleDirections(vector<Direction> possible_directions); //for debug
	void PrintPossibleDirection(Direction possibleDirection); //for debug
	void PrintDirection(Direction chosen); //for debug


}; // end of class _039563838_D

inline void _039563838_D::setSensor(const AbstractSensor& sensor) 
{
	_robotSensor = &sensor;
}

inline void _039563838_D::setConfiguration(map<string, int> config)
{
	_configs = map<string,int>(config);
	_battery = Battery(_configs);
}

//: check if the algorithm is in the docking station
inline bool _039563838_D::IsInDocking() const
{
	return (_pathFromDocking.size() == 0);
}

#endif // _039563838_D_h__
