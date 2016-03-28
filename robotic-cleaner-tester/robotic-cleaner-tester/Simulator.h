#ifndef Simulator_h__
#define Simulator_h__

#include "Point.h"
#include "Matrix.h"
#include <iostream>
#include "House.h"


namespace ns_robotic_cleaner_simulator
{

class Simulator
{
private:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//Battery  startingBattery;
	vector<House> houses;
	//vector< algorithm> algorithms;
	//map<string, int> configs;
	//vector< AlgorithmSingleRun> runs;

public:
	Simulator(void);
	~Simulator(void);

	void SimulateAll(void);
};

} // end of namespace ns_robotic_cleaner

#endif // Simulator_h__