#include "stdafx.h"
#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <string>
#include "AbstractAlgorithm.h"
#include "RandomRobotAlgorithm.h"
using namespace std;


namespace ns_robotic_cleaner_simulator
{
	Simulator::Simulator(void)
	{
		_houses.push_back(new House());
	}

	Simulator::Simulator(_TCHAR * configFilePath)
	{
		ReadConfigFromFile(configFilePath);
		_defaultBattery = new Battery(_configs);
	}


	//************************************
	// Brief:		Gets a folder with house files and loads them to memory.
	//				not implemented yet - just load hardcoded house
	// Gets:	 	_TCHAR * houseFolder - string of the folder path
	// Returns:   	int - the number of houses loaded successfully
	// Post:		_houses
	//************************************
	int Simulator::LoadHouses( _TCHAR * houseFolder)
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
	int Simulator::LoadAlgorithms()
	{
		Sensor dummySensor = Sensor();
		AbstractAlgorithm * randAlgorithm = new RandomRobotAlgorithm(dummySensor,_configs);
		_algorithms.push_back(randAlgorithm);
		return 1;
	}

	


	
	Simulator::~Simulator(void)
	{
		for (auto i = _houses.begin(); i != _houses.end(); ++i) {
			delete *i;
		}
		_houses.clear();
		for (auto j = _algorithms.begin(); j != _algorithms.end(); ++j) {
			delete *j;
		}
		_algorithms.clear();
	}

	void Simulator::SimulateAll()
	{

		_houses.at(0)->Print();
	}

	//TODO: Add space removes and also handle problemtic values - check that the parameter name is correct
	void Simulator::ReadConfigFromFile(_TCHAR * configFilePath)
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

	void Simulator::InitializeRuns()
	{
		int houseNum = _houses.size();
		int algorithmNum = _algorithms.size();
		for( int houseIndex = 0 ; houseIndex < houseNum; ++houseIndex){
			for( int algorithmIndex = 0 ; algorithmIndex < algorithmNum; ++algorithmIndex){
				AlgorithmSingleRun * asr = new AlgorithmSingleRun(_configs, _algorithms[algorithmIndex],*_defaultBattery,_houses[houseIndex]);	
			}
		}
	}

} //end of namespace ns_robotic_cleaner_simulator