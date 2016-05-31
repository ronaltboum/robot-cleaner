#ifndef _039563838_B_h__
#define _039563838_B_h__

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
#include "DynamicAlgo.h" 

using namespace std;

typedef vector<GeneralizedPoint> Path;

class _039563838_B :	public DynamicAlgo
{
	friend class DynamicPathFinder;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	_039563838_B(void);
	_039563838_B(const AbstractSensor& sensor, map<string, int> config);
	~_039563838_B(void);
	void initiallize(); //function called in each c'tor

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	Direction ChooseStepAccordingToState(Direction lastStep) override; // choose the step to do according to step
	virtual void UpdateState() override; //: change the state

}; // end of class _039563838_B

#endif // _039563838_B_h__
