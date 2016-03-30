#ifndef Simulator_h__
#define Simulator_h__

#include <fstream>
#include <string>
#include "AlgorithmSingleRun.h"
#include "RandomRobotAlgorithm.h"
#include "Sensor.h"
#include "House.h"
#include "Battery.h"
//#include ""
#include <sstream> 
using namespace std;

namespace ns_robotic_cleaner_simulator
{

class Simulator
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	vector<House *> _houses;
	map<string, int> _configs;
	vector< AlgorithmSingleRun *> _runs;
	Battery * _defaultBattery;
	bool _winnerAlgorithmExist;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	Simulator(void);
	Simulator(const char * configFilePath);
	~Simulator(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	void RunAll(void);
	void ReadConfigFromFile(char const * configFilePath);
	int LoadHouses( char * houseFolder);
	int LoadAlgorithmsAndRuns();
	void printScores(int winner_num_steps);
private:
	void MoveAllOneStep(int & currentRankAlgorithmsCompetingOn);
};

} // end of namespace ns_robotic_cleaner_simulator

#endif // Simulator_h__
