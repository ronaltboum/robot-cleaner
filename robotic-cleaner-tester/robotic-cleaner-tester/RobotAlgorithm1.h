#ifndef RobotAlgorithm1_h__
#define RobotAlgorithm1_h__

#include "abstractalgorithm.h"
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Battery.h"

namespace ns_robotic_cleaner_simulator
{

class RobotAlgorithm1 :	public AbstractAlgorithm
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	RobotAlgorithm1(void);
	RobotAlgorithm1(const AbstractSensor& sensor, map<string, int> config);
	~RobotAlgorithm1(void);
	void initiallize(); //function called in each c'tor

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	virtual Direction step();
	virtual void aboutToFinish(int stepsTillFinishing) = 0;
	virtual void setSensor(const AbstractSensor& sensor);
	virtual void setConfiguration(map<string, int> config);
protected:
	virtual bool IsInDocking() const; //: check if the algorithm is in the docking station
	virtual void UpdateState(); //: change the state
	static Direction OppositeDirection(Direction d);
	virtual vector<Direction> GetPossibleDirections(Direction lastStep) const;


}; // end of class RobotAlgorithm1

inline void RobotAlgorithm1::setSensor(const AbstractSensor& sensor) 
{
	_robotSensor = &sensor;
}

inline void RobotAlgorithm1::setConfiguration(map<string, int> config)
{
	_configs = map<string,int>(config);
	_battery = Battery(_configs);
}

//: check if the algorithm is in the docking station
inline bool RobotAlgorithm1::IsInDocking() const
{
	return (_pathFromDocking.size() == 0);
}
	
} //end of namespace ns_robotic_cleaner_simulator

#endif // RobotAlgorithm1_h__
