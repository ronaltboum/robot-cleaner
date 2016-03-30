 //robotic-cleaner-tester.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Simulator.h"
#include "AbstractAlgorithm.h"
#include "RandomRobotAlgorithm.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define configFileName "config.ini"

using namespace ns_robotic_cleaner_simulator;

int main(int argc, char * argv[])
{
	srand((unsigned int)time(NULL));
	char * configFilePath = (char *)("");
	char * houseFolder = (char *)("");
	if(argc >= 3) 
	{
		if ( ! strcmp(argv[1], "–config"))
		{
			configFilePath = argv[2];
		}
		else if (! strcmp(argv[1], "–house_path"))
		{
			houseFolder = argv[2];
		}
		else
		{
			cout << "invalid flags. flags are -config and -house_path";
		}
	}
	if(argc == 5) 
	{
		if ( ! strcmp(argv[3], "–config"))
		{
			configFilePath = argv[4];
		}
		else if ( ! strcmp(argv[3], "–house_path"))
		{
			houseFolder = argv[4];
		}
		else
		{
			cout << "invalid flags. flags are -config and -house_path";
		}
	}
	std::string configFile(configFilePath);
	configFile.append(configFileName);
	Simulator s = Simulator(configFile.c_str());
	if(s.LoadHouses(houseFolder) == 0){
		cout << "Cannot load house properly. exiting";
		return EXIT_FAILURE;
	}

	s.LoadAlgorithmsAndRuns();
	s.RunAll();
	return EXIT_SUCCESS;
}

