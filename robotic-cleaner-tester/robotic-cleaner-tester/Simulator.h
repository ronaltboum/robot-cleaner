#ifndef Simulator_h__
#define Simulator_h__

#include "Point.h"
#include "Matrix.h"
#include <iostream>
#include "House.h"
#include <map>
#include "AlgorithmSingleRun.h"


namespace ns_robotic_cleaner_simulator
{

class Simulator
{
private:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//Battery  startingBattery;
	vector<House *> houses;
	vector<AbstractAlgorithm *> algorithms;
	map<string, int> configs;
	vector< AlgorithmSingleRun> runs;

public:
	Simulator(void);
	Simulator(_TCHAR * configFilePath, _TCHAR * houseFolder);
	~Simulator(void);

	void SimulateAll(void);
	void ReadConfigFromFile(_TCHAR * configFilePath);
};

} // end of namespace ns_robotic_cleaner_simulator

#endif // Simulator_h__