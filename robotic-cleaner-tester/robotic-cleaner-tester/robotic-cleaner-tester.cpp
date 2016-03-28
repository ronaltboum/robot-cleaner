 //robotic-cleaner-tester.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Simulator.h"

using namespace ns_robotic_cleaner_simulator;

int _tmain(int argc, _TCHAR* argv[])
{
	Simulator s = Simulator();
	s.SimulateAll();
	std::cout << "main method";
	getchar();
	return 0;
}

