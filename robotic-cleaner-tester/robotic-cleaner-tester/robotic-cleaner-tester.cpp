 //robotic-cleaner-tester.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Simulator.h"
#include "AbstractAlgorithm.h"
#include "RandomRobotAlgorithm.h"
#include <time.h>

using namespace ns_robotic_cleaner_simulator;

int _tmain(int argc, char * argv[])
{
	srand(time(NULL));
	char * configFilePath = "config.ini";
	char * houseFolder = ".\\";
	if(argc >= 3) 
	{
		if ( ! strcmp(argv[1], "–config"))
		{
			configFilePath = argv[2];
		}
		if (! strcmp(argv[1], "–house_path"))
		{
			houseFolder = argv[2];
		}
	}
	if(argc == 5) 
	{
		if ( ! strcmp(argv[3], "–config"))
		{
			configFilePath = argv[4];
		}
		if ( ! strcmp(argv[3], "–house_path"))
		{
			houseFolder = argv[4];
		}
	}
	Simulator s = Simulator(configFilePath);
	s.LoadHouses(houseFolder);
	s.LoadAlgorithms();
	s.InitializeRuns();

	s.RunAll();
	std::cout << "main method";
	getchar();
	return 0;
}

