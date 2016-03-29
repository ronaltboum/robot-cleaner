 //robotic-cleaner-tester.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Simulator.h"
#include <winnt.rh>
#include "AbstractAlgorithm.h"
#include "RandomRobotAlgorithm.h"
#include <time.h>

using namespace ns_robotic_cleaner_simulator;

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));
	_TCHAR* configFilePath = TEXT("config.ini");
	_TCHAR* houseFolder = TEXT(".\\");
	if(argc >= 3) 
	{
		if (!_tcscmp(argv[1], _T("–config")))
		{
			configFilePath = argv[2];
		}
		if (!_tcscmp(argv[1], _T("–house_path")))
		{
			houseFolder = argv[2];
		}
	}
	if(argc == 5) 
	{
		if (!_tcscmp(argv[3], _T("–config")))
		{
			configFilePath = argv[4];
		}
		if (!_tcscmp(argv[3], _T("–house_path")))
		{
			houseFolder = argv[4];
		}
	}
	Simulator s = Simulator(configFilePath, houseFolder);
	s.SimulateAll();
	std::cout << "main method";
	getchar();
	return 0;
}

