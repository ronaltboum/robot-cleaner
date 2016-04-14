#ifndef Simulator_h__
#define Simulator_h__


#include <string>
#include "AlgorithmSingleRun.h"
#include "Sensor.h"
#include "House.h"
#include "Battery.h"
#include "SingletonConfigReader.h"
#include <sstream> 
#include <fstream>
#include "AlgorithmFactory.h"
using namespace std;

class Simulator
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	vector<House *> _houses;
	map<string, int> _configs;
	vector< AlgorithmSingleRun *> _runs;
	Battery * _defaultBattery;
	bool _winnerAlgorithmExist;
	AlgorithmFactory * _algorithmFactory;

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
	int LoadAlgorithms(string algorithmFolder);
	int LoadRuns();
	void RunAll(void);
	void printScores(int winner_num_steps);
private:
	void MoveAllOneStep(int & currentRankAlgorithmsCompetingOn);
	

};

#endif // Simulator_h__
