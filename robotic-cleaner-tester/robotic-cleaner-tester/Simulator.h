#ifndef Simulator_h__
#define Simulator_h__


#include <string>
#include "AlgorithmSingleRun.h"
#include "RandomRobotAlgorithm.h"
#include "Sensor.h"
#include "House.h"
#include "Battery.h"
#include "SingletonConfigReader.h"
#include <sstream> 
#include <fstream>
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
	Simulator(const string & configFilePath);
	~Simulator(void);
	void initiallize();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	void ReadConfigFile(const string & configFilePath);
	int LoadHouses( string houseFolder);
	int LoadAlgorithmsAndRuns();
	void RunAll(void);
	void printScores(int winner_num_steps);
private:
	void MoveAllOneStep(int & currentRankAlgorithmsCompetingOn);
};

} // end of namespace ns_robotic_cleaner_simulator

#endif // Simulator_h__
