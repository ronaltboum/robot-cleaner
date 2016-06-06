#ifndef Simulator_h__
#define Simulator_h__


#include <string>

#include <iostream>
#include <functional>
#include <memory>
#include <list>
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>


//#include "AlgorithmSingleRun.h"    //SubSimulation.h  includes it already
#include "Direction.h"
#include "Sensor.h"
#include "House.h"
#include "Battery.h"
#include "SingletonConfigReader.h"
#include "SubSimulation.h"
#include <sstream> 
#include <fstream>
#include "AlgorithmFactory.h"
#include "ScoreRegistrar.h"
#include "Encoder.h"

using namespace std;

class Simulator
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	
	atomic_size_t _indexOfHouse{0}; // atomic_size_t is a language typedef for std::atomic<size_t>
	//atomic_size_t _indexOfHouse;
	vector<House *> _houses;
	map<string, int> _configs;
	Battery * _defaultBattery;

	AlgorithmFactory * _algorithmFactory;
	map<string, string> _badHousesMap;  //entry is of the form: "house1.house", "missing docking station"
	map<string, map<string, int> > _scoresMap;     //map<algo name, map<house name, int result> >
	vector< SubSimulation *> _subSimulations;
	vector<string> _hitWallErrorMessages;   //contains all the error messages of algorithms that hit walls
	map<double, vector<string>, std::greater<int>> _sortedAlgorithmsMap;
	bool _videoEnabled;
	bool _scoreFormulaProblem = false;  //becomes true if calc_score returns -1 for some algorithm
	bool debugSimulator = false;   //for debugging

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	Simulator(void);
	~Simulator(void);
	void initiallize();

	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	map<string, string> GetBadHousesMap() const;
	int GetMaxStepsAfterWinner() const;
	bool GetScoreFormulaProblem() { return _scoreFormulaProblem; }
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	bool ReadConfigFile(const string & configFilePath);
	int LoadHouses( vector<std::string> housesVector );
	int LoadAlgorithms(vector<string> algorithmFiles);
	void RunAllHouses(size_t num_threads);
	void RunSingleSubSimulationThread();
	int LoadRuns();
	void RunAll(int houseIndex);  //Run SubSimulation on house
	void registerScores(int winner_num_steps, int houseIndex, int simulationSteps);
	void printScores();
	void addScore(string algoName, string trimmedHouseName, int score);
	void printDashes(int numCharsInRow);
	void printSpaces(int quantity);
	vector<string> GetValidTrimmedHousesNames();
	void printFirstRow(int houseNum, vector<string> trimmedValidHouseNames);
	void printRow(string algoName);
	vector<int> GetAlgoResults(string algoName);
	string ConvertFromInt(int res);
	string CovertFromDouble(double avg);
	void PrintHouseErrors();
	void PrintAlgoErrors();
	string* GetAbsPath(string relativePath);  //returns absolute path or null in case of error
	void PrintDirection(Direction chosen); //for debug
	void InitizalizeVideoCreation();
	void SortAlgorithms(); //sorts algoritms by AVG
	void MakeVideos();
	string FixAlgoNameForPrint(string algoName);
	vector<string> split(const string &s, char delim);

	string FixAlgoNameForPrint(string algoName);  //ron added !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	vector<string> split(const string &s, char delim);   //ron added !!!!!!!!!!!!!!!!!!!!!!!!!!!

private:
	static bool CreateDirectoryIfNotExists(const string& dirPath);
	bool MoveAllOneStep(int & currentRankAlgorithmsCompetingOn, int houseIndex);
	void CallAboutToFinish(int houseIndex);

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~
inline map<string, string> Simulator::GetBadHousesMap() const
{
	return _badHousesMap;
}

#endif // Simulator_h__
