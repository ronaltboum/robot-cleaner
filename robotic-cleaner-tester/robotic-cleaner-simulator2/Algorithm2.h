#ifndef __ALGORITHM2_H
#define __ALGORITHM2_H
#include "AbstractAlgorithm.h"
#include "Direction.h"
#include <map>
#include <string>
using namespace std;

class Algorithm2 : public AbstractAlgorithm {
public:	
	Algorithm2(){
		cout << "algorithm1 c'tor" << endl;
	}
	
	~Algorithm2() {
		cout << "algorithm1 d'tor" << endl;
	}
	
    // setSensor is called once when the Algorithm is initialized 
    void setSensor(const AbstractSensor& sensor){
		cout << "algorithm1 setSensor" << endl;
	}
    
    // setConfiguration is called once when the Algorithm is initialized - see below 
    void setConfiguration(map<string, int> config){
		cout << "algorithm1 setConfigs" << endl;
	}
    
    // step is called by the simulation for each time unit 
    Direction step(){
		cout << "algorithm1 step" << endl;
		return Direction(4);
	}
    
    // this method is called by the simulation either when there is a winner or 
    // when steps == MaxSteps - MaxStepsAfterWinner 
    // parameter stepsTillFinishing == MaxStepsAfterWinner 
    void aboutToFinish(int stepsTillFinishing){
		cout << "algorithm1 stepsTillFinishing" << endl;
	}
};
#endif // __ALGORITHM2_H
