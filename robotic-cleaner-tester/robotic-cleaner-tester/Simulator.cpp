//#include "stdafx.h"
#include "Simulator.h"

namespace ns_robotic_cleaner_simulator
{
	Simulator::Simulator(void)
	{
		_winnerAlgorithmExist = false;
		ReadConfigFromFile("config.ini");
		_defaultBattery = new Battery(_configs);
	}

	Simulator::Simulator(char * configFilePath)
	{
		_winnerAlgorithmExist = false;
		ReadConfigFromFile(configFilePath);
		_defaultBattery = new Battery(_configs);
	}


	//************************************
	// Brief:		Gets a folder with house files and loads them to memory.
	//				not implemented yet - just load hardcoded house
	// Gets:	 	char * houseFolder - string of the folder path
	// Returns:   	int - the number of houses loaded successfully
	// Post:		_houses
	//************************************
	int Simulator::LoadHouses( char * houseFolder)
	{
		House * h = new House();
		if(h->isValid()){
			_houses.push_back(h);
			//h->Print();
			return 1;
		}
		h->Print();
		return 0;
	}

	//************************************
	// Brief:		Load algorithms from libraries
	//				not implemented yet - just load random-move algorithm
	// Returns:   	int - the number of algorithm loaded successfully
	// Post:		_algorithms
	//************************************
	int Simulator::LoadAlgorithmsAndRuns()
	{
		int houseNum = _houses.size();
		for( int houseIndex = 0 ; houseIndex < houseNum; ++houseIndex)
		{
			Point * startingPoint = _houses[houseIndex]->GetDockingStation();
			Sensor * algoSensor = new Sensor(_houses[houseIndex],startingPoint);
			AbstractAlgorithm * randAlgorithm = new RandomRobotAlgorithm(*algoSensor,_configs);
			AlgorithmSingleRun * newRunCreated = new AlgorithmSingleRun(_configs, randAlgorithm, *_defaultBattery, _houses[houseIndex] ,algoSensor, startingPoint);	
			_runs.push_back(newRunCreated);
		}
		return 1;
	}
	
	Simulator::~Simulator(void)
	{
		for (vector<House *>::iterator i = _houses.begin(); i != _houses.end(); ++i) {
			delete *i;
		}
		_houses.clear();
		delete _defaultBattery;
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
			bool is_battery_empty = runIterator->IsAlgorithmBatteryEmpty();
			bool is_back_in_docking = runIterator->IsBackInDocking(); 
			score = 2000 
				- (position_in_copmetition - 1) * 50 
				+ (winner_num_steps - this_num_steps) * 10 
				- (sum_dirt_in_house - dirt_collected) * 3 
				+ (is_back_in_docking? 50 : -200);
			score = max(0, score);
			const string houseName = runIterator->GetCurrentHouse()->GetShortName();
			cout << houseName << "\t" << score << "\n";
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

	//TODO: Add space removes and also handle problematic values - check that the parameter name is correct
	void Simulator::ReadConfigFromFile(char * configFilePath)
	{
		string line;
		ifstream myfile(configFilePath);
		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				size_t equalPosition = line.find("=");      // position of "=" in str
				string parmaterName = line.substr(0,equalPosition);
				string paramaterValueString = line.substr (equalPosition+1);     // get from "=" + 1 to the end
				int parameterValue = stoi(paramaterValueString);
				_configs.insert(pair<string,int>(parmaterName,parameterValue));
			}
			myfile.close();
		}

		else cout << "Unable to open file"; 
	}

} //end of namespace ns_robotic_cleaner_simulator