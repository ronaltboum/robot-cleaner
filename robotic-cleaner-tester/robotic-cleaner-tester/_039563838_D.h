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
enum class AlgorithmStatus {ChargingInDocking,			// the algorithm is charging until battery is full
								Exploring,				// the algorithm is exploring
								StayingUntilClean,		// the algorithm is in dirty spot so it stays there until it's cleaned
								Returning};				// the algorithm battery is about to finish so it's returning to docking
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
//    int totalSteps = 0;
//    int phase = -1; // phases 0-7: explore surrounding, phase 8: select new direction
//    const static Direction dir_by_phase[8];
//    Direction requestedStep = Direction::Stay;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	_039563838_D(void);
	_039563838_D(const AbstractSensor& sensor, map<string, int> config);
	~_039563838_D(void);
	void initiallize(); //function called in each c'tor

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	virtual Direction step(Direction lastStep);
	virtual void aboutToFinish(int stepsTillFinishing) {}
	virtual void setSensor(const AbstractSensor& sensor);
	virtual void setConfiguration(map<string, int> config);
protected:
	virtual bool IsInDocking() const; //: check if the algorithm is in the docking station
	virtual void UpdateState(); //: change the state
	static Direction OppositeDirection(Direction d);
	virtual vector<Direction> GetPossibleDirections(Direction lastStep) const;
	void updateAlgorithmInfo(Direction lastStep);
	int calcStepsToDocking(int stepsFromDocking, const GeneralizedPoint& position);
	void updateStepsToDocking(int stepsToDocking, const GeneralizedPoint& position);
	void printDebugHouseMapping();


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
