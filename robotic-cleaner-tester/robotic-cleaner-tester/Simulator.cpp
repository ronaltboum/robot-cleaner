//#include "stdafx.h"
#include "Simulator.h"
#include <string>
using namespace std;

Simulator::Simulator(void)
{
	initiallize();
}

Simulator::Simulator(const string & configFilePath)
{
	initiallize();
	ReadConfigFile(configFilePath);
}

//************************************
// Brief:		called in each c'tor to initialize all
//************************************
void Simulator::initiallize()
{
	_houses = vector<House *>();
	_configs = map<string, int>();
	_runs = vector< AlgorithmSingleRun *>();
	_defaultBattery = NULL;
	_algorithmFactory = NULL;
	_winnerAlgorithmExist = false;
}

Simulator::~Simulator(void)
{
	for (vector<House *>::iterator i = _houses.begin(); i != _houses.end(); ++i) {
		delete *i;
	}
	_houses.clear();
	for (vector<AlgorithmSingleRun *>::iterator j = _runs.begin(); j != _runs.end(); ++j) {
		delete *j;
	}
	_runs.clear();

	delete _defaultBattery;
	delete _algorithmFactory;
}

void Simulator::ReadConfigFile(const string & configFilePath){
	_configs = SingletonConfigReader::instance(configFilePath)->ReadConfigFromFile();
	_defaultBattery = new Battery(_configs);
}


//************************************
// Brief:		Gets a folder with house files and loads them to memory.
//				not implemented yet - just load hard-coded house
// Gets:	 	char * houseFolder - string of the folder path
// Returns:   	int - the number of houses loaded successfully
// Post:		_houses
//************************************
int Simulator::LoadHouses( string houseFolder)
{
	House * h = SingletonHouseIOManager::instance()->ReadHouseFromFile("house1.house");
	if(h->isValid()){
		_houses.push_back(h);
		return 1;
	}
	return 0;
}

int Simulator::LoadAlgorithms(string algorithmFolder)
{
	_algorithmFactory = new AlgorithmFactory();
	_algorithmFactory->ReadAlgorithms(algorithmFolder);
	_algorithmFactory->tryAlgorithms();
}

//************************************
// Brief:		Load algorithms from libraries
//				not implemented yet - just load random-move algorithm
// Returns:   	int - the number of algorithm loaded successfully
// Post:		_algorithms
//************************************
int Simulator::LoadRuns()
{
	int houseNum = _houses.size();
	vector<AbstractAlgorithm *> algorithms;
	vector<AbstractAlgorithm *>::iterator _algoIterator; 
	for( int houseIndex = 0 ; houseIndex < houseNum; ++houseIndex)
	{
		////old
		//Point * startingPoint = _houses[houseIndex]->GetDockingStation(); //new is deallocated by AlgorithmSingleRun
		//Sensor * algoSensor = new Sensor(_houses[houseIndex],startingPoint); // new is deallocated by AlgorithmSingleRun
		//AbstractAlgorithm * randAlgo = new RandomRobotAlgorithm(*algoSensor,_configs);
		//AlgorithmSingleRun * newRunCreated = new AlgorithmSingleRun(_configs, randAlgo, *_defaultBattery, _houses[houseIndex] ,algoSensor, startingPoint);	
		//_runs.push_back(newRunCreated);

		algorithms = _algorithmFactory->CreateSetOfAllAlgorithms();
		for(_algoIterator=algorithms.begin() ; _algoIterator!=algorithms.end() ; _algoIterator++){
			Point * startingPoint = _houses[houseIndex]->GetDockingStation(); //new is deallocated by AlgorithmSingleRun
			Sensor * algoSensor = new Sensor(_houses[houseIndex],startingPoint); // new is deallocated by AlgorithmSingleRun
			(*_algoIterator)->setConfiguration(_configs);
			(*_algoIterator)->setSensor(*algoSensor);
			AlgorithmSingleRun * newRunCreated = new AlgorithmSingleRun(_configs, (*_algoIterator), *_defaultBattery, _houses[houseIndex] ,algoSensor, startingPoint);	
			_runs.push_back(newRunCreated);
		}
	}
	return 1;
}

void Simulator::RunAll()
{
	int maxSteps = _configs["MaxSteps"];
	int currentStep = 0;
	int currentRankAlgorithmsCompetingOn = 1;
	//run until someone one or sombodywon
	for(;currentStep < maxSteps && ! _winnerAlgorithmExist; ++currentStep){
		MoveAllOneStep(currentRankAlgorithmsCompetingOn);
	}
	//move all if possible after winning
	int winnerStepsNumber = currentStep;
	int maxStepsAfterWinner = _configs["MaxStepsAfterWinner"];
	for(;currentStep < maxSteps && currentStep < winnerStepsNumber + maxStepsAfterWinner ; ++currentStep){
		MoveAllOneStep(currentRankAlgorithmsCompetingOn);
	}

	printScores(winnerStepsNumber);
}

// winner_num_steps == maxSteps if nobody won
void Simulator::printScores(int winner_num_steps)
{
	int score;
	int runNum = _runs.size();
	for( int runIndex = 0 ; runIndex < runNum; ++runIndex)
	{
		AlgorithmSingleRun * runIterator = _runs[runIndex];
		if(runIterator->HasMadeIllegalStep())
			return; //already printed by algorithm single run
		int actual_position = runIterator->GetActualPosition() ? runIterator->GetActualPosition() : 10;
		int position_in_copmetition = min(actual_position, 4);
		int this_num_steps = runIterator->GetNumberOfStepsCommited();
		int sum_dirt_in_house = runIterator->GetSumOfDirtBeforeCleaning();
		int dirt_collected = runIterator->GetDirtCollected();
		//bool is_battery_empty = runIterator->IsAlgorithmBatteryEmpty();
		bool is_back_in_docking = runIterator->IsBackInDocking(); 
		score = 2000 
			- (position_in_copmetition - 1) * 50 
			+ (winner_num_steps - this_num_steps) * 10 
			- (sum_dirt_in_house - dirt_collected) * 3 
			+ (is_back_in_docking? 50 : -200);
		score = max(0, score);
		const string houseName = runIterator->GetCurrentHouse()->GetShortName();
		cout << score;	//for ex1 only
		//cout << houseName << "\t" << score << "\n"; for future reference - the correct printing format
	}
}

void Simulator::MoveAllOneStep(int & currentRankAlgorithmsCompetingOn)
{
	int runNum = _runs.size();
	int numAlgorithmWon = 0; //for this step
	for( int runIndex = 0 ; runIndex < runNum; ++runIndex)
	{
		AlgorithmSingleRun * runIterator = _runs[runIndex];
		if(runIterator->CanDoStep()){
			runIterator->DoStep();
			if(runIterator->HasWon()){
				runIterator->SetActualPosition(currentRankAlgorithmsCompetingOn);
				++numAlgorithmWon;
				_winnerAlgorithmExist = true;
			}
		}
	}
	currentRankAlgorithmsCompetingOn += numAlgorithmWon;
}
