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
		houses.push_back(new House());
	}

	Simulator::Simulator(_TCHAR * configFilePath, _TCHAR * houseFolder)
	{
		House * h = new House();
		Point * p = h->GetDockingStation();
		houses.push_back(h);
		ReadConfigFromFile(configFilePath);
		Sensor * s = new Sensor(h,p);
		AbstractAlgorithm * randAlgorithm = new RandomRobotAlgorithm(*s,configs);
		//TODO: add Algorithm single run
	}


	
	Simulator::~Simulator(void)
	{
		for (auto i = houses.begin(); i != houses.end(); ++i) {
			delete *i;
		}
		houses.clear();
	}

	void Simulator::SimulateAll()
	{
		houses.at(0)->Print();
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
				configs.insert(pair<string,int>(parmaterName,parameterValue));
			}
			myfile.close();
		}

		else cout << "Unable to open file"; 
	}

} //end of namespace ns_robotic_cleaner_simulator