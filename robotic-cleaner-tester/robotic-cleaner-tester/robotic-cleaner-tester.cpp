 //robotic-cleaner-tester.cpp : Defines the entry point for the console application.
//#include "stdafx.h"
#include "Simulator.h"
#include "AbstractAlgorithm.h"
#include "RandomRobotAlgorithm.h"
#include <time.h>
#include <string.h>

using namespace ns_robotic_cleaner_simulator;

int main(int argc, char * argv[])
{
	srand((unsigned int)time(NULL));
	char * configFilePath = (char *)("config.ini");
	char * houseFolder = (char *)(".\\");
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
	Simulator s = Simulator(configFilePath);
	s.LoadHouses(houseFolder);
	s.LoadAlgorithmsAndRuns();
	s.RunAll();
	return 0;
}

