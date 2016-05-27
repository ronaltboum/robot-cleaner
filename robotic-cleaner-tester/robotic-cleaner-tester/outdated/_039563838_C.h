#ifndef _039563838_C_h__
#define _039563838_C_h__


#include <iostream>
#include <functional>
#include <memory>
#include <list>
#include <cassert>
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "Direction.h"
#include "AbstractSensor.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistration.h"
#include "FactoryDefinition.h"  //header file where factory is defined
#include "Battery.h"

using namespace std;

class _039563838_C :	public AbstractAlgorithm
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
	_039563838_C(void);
	_039563838_C(const AbstractSensor& sensor, map<string, int> config);
	~_039563838_C(void);
	void initiallize(); //function called in each c'tor
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	virtual Direction step() override;
	virtual void aboutToFinish(int stepsTillFinishing) override{}
	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(map<string, int> config) override;
protected:
	virtual bool IsInDocking() const; //: check if the algorithm is in the docking station
	virtual void UpdateState(); //: change the state
	static Direction OppositeDirection(Direction d);
	Direction ChooseDirectionByPreference() const;
	//virtual vector<Direction> GetPossibleDirections() const;

  
};  // end of class _039563838_C


inline void _039563838_C::setSensor(const AbstractSensor& sensor) 
{
	_robotSensor = &sensor;
}

inline void _039563838_C::setConfiguration(map<string, int> config) 
{
	_configs = map<string,int>(config);
	_battery = Battery(_configs);
}

//: check if the algorithm is in the docking station
inline bool _039563838_C::IsInDocking() const
{
	return (_pathFromDocking.size() == 0);
}

#endif // _039563838_C_h__