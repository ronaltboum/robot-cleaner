#ifndef __ABSTRACT_ALGORITHM__H_
#define __ABSTRACT_ALGORITHM__H_

#include <map>
using namespace std;

#include "AbstractSensor.h"

class AbstractAlgorithm 
{ 
public: 
	virtual ~AbstractAlgorithm() {}
	
    // setSensor is called once when the Algorithm is initialized 
    virtual void setSensor(const AbstractSensor& sensor) = 0; 
    
    // setConfiguration is called once when the Algorithm is initialized - see below 
    virtual void setConfiguration(map<string, int> config) = 0; 
    
    // step is called by the simulation for each time unit
    virtual Direction step(Direction prevStep) = 0;         //this is the new signature for step function !!!!!!!!!!!!!!!!

//    the robot (the simulator in our case) 
//    provides the algorithm the previous step’s direction that was actually taken. In the first call to 
//    ‘step’ when starting cleaning the house, simulation will send STAY as prevStep. 
//    Your simulation can always perform the recommended step, but your algorithm shall not 
//    assume that. Our simulation may occasionally decide not to follow the recommended step!
//    When calculating path through the house (e.g. calculating way back to the docking station or 
//    getting back to previously located dust) the algorithm MUST take into account the actual 
//    steps that were taken, provided as a parameter to ‘step’, not relying on the recommended 
//    steps returned from the calls to ‘step’ to be the actual steps taken. 
    
//virtual Direction step() = 0;   //the old signature
    
    // this method is called by the simulation either when there is a winner or 
    // when steps == MaxSteps - MaxStepsAfterWinner 
    // parameter stepsTillFinishing == MaxStepsAfterWinner 
    virtual void aboutToFinish(int stepsTillFinishing) = 0; 
};

#endif //__ABSTRACT_ALGORITHM__H_
